/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 - 2022 Inesonic, LLC.
* 
* This file is licensed under two licenses.
*
* Inesonic Commercial License, Version 1:
*   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
*   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
*   strictly prohibited.
*
* GNU Public License, Version 2:
*   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*   
*   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
*   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************************************************//**
* \file
*
* This file implements the \ref EQt::ProgrammaticMainWindow class.
***********************************************************************************************************************/

#include <QWidget>
#include <QMap>
#include <QSet>
#include <QHash>
#include <QPair>
#include <QSize>
#include <QRect>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QString>
#include <QIcon>
#include <QKeySequence>
#include <QDockWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QTimer>
#include <QMainWindow>

#include <limits>
#include <algorithm>

#include "dock_widget_location.h"
#include "dock_widget_locations.h"
#include "eqt_programmatic_application.h"
#include "eqt_programmatic_dock_widget.h"
#include "eqt_dock_widget_defaults.h"
#include "eqt_builder.h"
#include "eqt_programmatic_main_window_proxy.h"
#include "eqt_programmatic_main_window.h"

/***********************************************************************************************************************
 * Class ProgrammaticMainWindow::Node
 */

namespace EQt {
    class ProgrammaticMainWindow::Node {
        public:
            typedef QMap<QString, Node*>::iterator ChildIterator;

            Node(const QString& title, bool topLevel, QWidget* parentWidget, Node* newParent = Q_NULLPTR);

            ~Node();

            inline QMenu* menu() {
                return currentMenu;
            }

            inline Node* parent() {
                return currentParent;
            }

            ProgrammaticMainWindow::Node* addChildMenu(const QString& childName);

            inline Node* child(const QString& childName) {
                return children.value(childName, Q_NULLPTR);
            }

            inline bool childExists(const QString& childName) {
                return children.contains(childName);
            }

            inline ChildIterator begin() {
                return children.begin();
            }

            inline ChildIterator end() {
                return children.end();
            }

        private:
            Node*                currentParent;
            QMap<QString, Node*> children;
            QMenu*               currentMenu;
    };


    ProgrammaticMainWindow::Node::Node(const QString& title, bool topLevel, QWidget* parentWidget, Node* newParent) {
        currentParent = newParent;

        // The hack below is loosely based on the write-up at:
        //
        //     https://stackoverflow.com/questions/15434683/disable-start-dictation-for-an-app-in-qt#16009541

        #if (defined(Q_OS_DARWIN))

            if ((title == tr("&Edit") || title == tr("&Help") || title == tr("&View")) && topLevel) {
                currentMenu = new QMenu(QString("%1%2").arg(QChar(0x200C)).arg(title), parentWidget);
            } else {
                currentMenu = new QMenu(title, parentWidget);
            }

        #else

            (void) topLevel;
            currentMenu   = new QMenu(title, parentWidget);

        #endif
    }


    ProgrammaticMainWindow::Node::~Node() {
        // We rely on Qt to delete the QMenu instance associated with each child.

        for (auto it=children.begin(),end=children.end() ; it!=end ; ++it) {
            delete it.value();
        }
    }


    ProgrammaticMainWindow::Node* ProgrammaticMainWindow::Node::addChildMenu(const QString& childName) {
        Q_ASSERT(!children.contains(childName));

        ProgrammaticMainWindow::Node* newNode = new ProgrammaticMainWindow::Node(
            childName,
            false,
            dynamic_cast<QWidget*>(currentMenu->parent()),
            this
        );

        children.insert(childName, newNode);

        currentMenu->addMenu(newNode->currentMenu);

        return newNode;
    }
}

/***********************************************************************************************************************
 * Class ProgrammaticMainWindow:
 */

namespace EQt {
    ProgrammaticMainWindow::ProgrammaticMainWindow(QWidget* parent):QMainWindow(parent) {
        currentActionState.clear();

        dockWidgetUpdateTimer = new QTimer(this);
        dockWidgetUpdateTimer->setSingleShot(true);

        dockWidgetRestackingTimer = new QTimer(this);
        dockWidgetRestackingTimer->setSingleShot(true);

        showTimer = new QTimer(this);
        showTimer->setSingleShot(true);

        connect(
            this,
            &ProgrammaticMainWindow::tabifiedDockWidgetActivated,
            this,
            &ProgrammaticMainWindow::tabifiedDockWidgetWasActivated
        );

        connect(dockWidgetUpdateTimer, &QTimer::timeout, this, &ProgrammaticMainWindow::updateDocks);
        connect(dockWidgetRestackingTimer, &QTimer::timeout, this, &ProgrammaticMainWindow::restackDockWidgets);
        connect(showTimer, &QTimer::timeout, this, &ProgrammaticMainWindow::nowVisible);
    }


    ProgrammaticMainWindow::~ProgrammaticMainWindow() {}


    void ProgrammaticMainWindow::resetDocksToDefaultState() {
        // Remove all dock widgets from the window.

        for (  DockWidgetsByName::const_iterator dockWidgetIterator    = dockWidgets.constBegin(),
                                                 dockWidgetEndIterator = dockWidgets.constEnd()
             ; dockWidgetIterator != dockWidgetEndIterator
             ; ++dockWidgetIterator
            ) {
            QDockWidget* dock = dockWidgetIterator.value();
            removeDockWidget(dock);
        }

        // Now work out the relative positioning of each dock in each area.

        DockWidgetLocations               dockWidgetLocations;
        QMap<QString, DockWidgetDefaults> defaults = dockWidgetDefaults;
        do {
            QMap<QString, DockWidgetDefaults>::iterator defaultsIterator = defaults.begin();
            while (defaultsIterator != defaults.constEnd()) {
                const QString&            dockName     = defaultsIterator.key();
                const DockWidgetDefaults& dockDefaults = defaultsIterator.value();
                QDockWidget*              dock         = dockWidgets.value(dockName);

                Q_ASSERT(dock != Q_NULLPTR);

                DockWidgetLocations::InsertionStatus status;
                if (dockDefaults.isExplicitPosition()) {
                    status = dockWidgetLocations.insert(dock, dockDefaults.dockArea());
                } else {
                    QString      siblingDockName = dockDefaults.siblingDockName();
                    QDockWidget* siblingDock     = dockWidgets.value(siblingDockName);

                    Q_ASSERT(siblingDock != Q_NULLPTR);

                    status = dockWidgetLocations.insert(dock, dockDefaults.relationship(), siblingDock);
                }

                if (status == DockWidgetLocations::InsertionStatus::INSERTION_SUCCESSFUL) {
                    defaultsIterator = defaults.erase(defaultsIterator);
                } else {
                    ++defaultsIterator;
                }
            }
        } while (!defaults.empty());

        // Add all the docks back, in order.  Combine docks into tabs where needed.

        DockWidgetDefaults::Area lastArea      = DockWidgetDefaults::Area::UNDEFINED;
        unsigned                 lastTabNumber = static_cast<unsigned>(-1);
        QDockWidget*             lastDock      = Q_NULLPTR;

        DockWidgetLocations::const_iterator locationIterator    = dockWidgetLocations.constBegin();
        DockWidgetLocations::const_iterator locationEndIterator = dockWidgetLocations.constEnd();

        QSet<QDockWidget*> independentDocks;
        QSet<QDockWidget*> docksWithSiblings;
        while (locationIterator != locationEndIterator) {
            const DockWidgetLocation& location = locationIterator.key();
            QDockWidget*              dock     = locationIterator.value();

            addDockWidget(location.area(), dock);
            if (location.area() == lastArea && location.inTabNumber() == lastTabNumber) {
                tabifyDockWidget(lastDock, dock);
                docksWithSiblings.insert(dock);
                docksWithSiblings.insert(lastDock);
            } else {
                independentDocks.insert(dock);
            }

            lastArea      = location.area();
            lastTabNumber = location.inTabNumber();
            lastDock      = dock;

            ++locationIterator;
        }

        // Raise the first dock in each tab region.

        QSet<QDockWidget*> docksToRaise = independentDocks & docksWithSiblings;
        for (  QSet<QDockWidget*>::const_iterator raiseIterator    = docksToRaise.constBegin(),
                                                  raiseEndIterator = docksToRaise.constEnd()
             ; raiseIterator != raiseEndIterator
             ; ++raiseIterator
            ) {
            QDockWidget* dockToRaise = *raiseIterator;
            dockToRaise->raise();
        }

        // Float or hide the docks that should be floating or hidden.
        QMap<QString, DockWidgetDefaults>::const_iterator defaultsIterator    = dockWidgetDefaults.constBegin();
        QMap<QString, DockWidgetDefaults>::const_iterator defaultsEndIterator = dockWidgetDefaults.constEnd();
        while (defaultsIterator != defaultsEndIterator) {
            const QString&            dockName     = defaultsIterator.key();
            QDockWidget*              dock         = dockWidgets.value(dockName);
            const DockWidgetDefaults& dockDefaults = defaultsIterator.value();

            dock->setHidden(dockDefaults.defaultHidden());
            dock->setFloating(dockDefaults.defaultFloating());

            ++defaultsIterator;
        }

        // Finally we tells the docks to re-adjust their sizes.
        dockWidgetUpdateTimer->start(dockWidgetUpdateDelay1);
    }


    QAction* ProgrammaticMainWindow::addAction(QAction* newAction, const QString& actionName) {
        Q_ASSERT(!actions.contains(actionName));
        actions.insert(actionName, newAction);

        return newAction;
    }


    QAction* ProgrammaticMainWindow::addAction(
            QAction*            newAction,
            const Util::BitSet& actionMask,
            const QString&      actionName
        ) {
        QAction* action = addAction(newAction, actionName);

        if (action != Q_NULLPTR) {
            setActionMask(actionName, actionMask);
        }

        return newAction;
    }


    QAction* ProgrammaticMainWindow::addAction(const QIcon& icon, const QString& iconText, const QString& actionName) {
        QString newActionName = actionName.isEmpty() ? iconText : actionName;

        QAction* newAction = new QAction(icon, iconText, this);
        newAction->setVisible(true);

        addAction(newAction, newActionName);

        return newAction;
    }


    QAction* ProgrammaticMainWindow::addAction(
            const QIcon&        icon,
            const QString&      iconText,
            const Util::BitSet& actionMask,
            const QString&      actionName
        ) {
        QString  newActionName = actionName.isEmpty() ? iconText : actionName;
        QAction* action        = addAction(icon, iconText, newActionName);

        if (action != Q_NULLPTR) {
            setActionMask(newActionName, actionMask);
        }

        return action;
    }


    QAction* ProgrammaticMainWindow::addMenuAction(const QString& menuLocation, const QString& actionName) {
        QString newActionName = actionName.isEmpty() ? menuLocation : actionName;

        QStringList                   menuTree     = menuLocation.split("|", Qt::SplitBehaviorFlags::SkipEmptyParts);
        QString                       menuItemName = menuTree.last().trimmed();
        ProgrammaticMainWindow::Node* node         = locateNode(menuTree);

        QAction* newAction = node->menu()->addAction(menuItemName);
        addAction(newAction, newActionName);

        return newAction;
    }


    QAction* ProgrammaticMainWindow::addMenuAction(
            const QString&      menuLocation,
            const Util::BitSet& actionMask,
            const QString&      actionName
        ) {
        QString  newActionName = actionName.isEmpty() ? menuLocation : actionName;
        QAction* action        = addMenuAction(menuLocation, newActionName);

        if (action != Q_NULLPTR) {
            setActionMask(newActionName, actionMask);
        }

        return action;
    }


    QAction* ProgrammaticMainWindow::addMenuAction(
            const QIcon&   icon,
            const QString& menuLocation,
            const QString& actionName
        ) {
        QString newActionName = actionName.isEmpty() ? menuLocation : actionName;

        QStringList                   menuTree     = menuLocation.split("|", Qt::SplitBehaviorFlags::SkipEmptyParts);
        QString                       menuItemName = menuTree.last().trimmed();
        ProgrammaticMainWindow::Node* node         = locateNode(menuTree);

        QAction* newAction = node->menu()->addAction(icon, menuItemName);
        addAction(newAction, newActionName);

        return newAction;
    }


    QAction* ProgrammaticMainWindow::addMenuAction(
            const QIcon&        icon,
            const QString&      menuLocation,
            const Util::BitSet& actionMask,
            const QString&      actionName
        ) {
        QString  newActionName = actionName.isEmpty() ? menuLocation : actionName;
        QAction* action        = addMenuAction(icon, menuLocation, newActionName);

        if (action != Q_NULLPTR) {
            setActionMask(action, actionMask);
        }

        return action;
    }


    QAction* ProgrammaticMainWindow::addMenuAndToolbarAction(
            const QString& toolbarName,
            const QIcon&   icon,
            const QString& menuLocation,
            const QString& actionName
        ) {
        QString newActionName = actionName.isEmpty() ? menuLocation : actionName;

        QStringList                   menuTree     = menuLocation.split("|", Qt::SplitBehaviorFlags::SkipEmptyParts);
        QString                       menuItemName = menuTree.last().trimmed();
        ProgrammaticMainWindow::Node* node         = locateNode(menuTree);

        QAction* newAction = node->menu()->addAction(icon, menuItemName);

        QToolBar* toolbar;
        if (!toolbars.contains(toolbarName)) {
            toolbar = addToolBar(toolbarName);
            toolbars.insert(toolbarName, toolbar);
        } else {
            toolbar = toolbars.value(toolbarName);
        }

        toolbar->addAction(newAction);

        addAction(newAction, newActionName);

        return newAction;
    }


    QAction* ProgrammaticMainWindow::addMenuAndToolbarAction(
            const QString&      toolbarName,
            const QIcon&        icon,
            const QString&      menuLocation,
            const Util::BitSet& actionMask,
            const QString&      actionName
        ) {
        QString  newActionName = actionName.isEmpty() ? menuLocation : actionName;
        QAction* action = addMenuAndToolbarAction(toolbarName, icon, menuLocation, newActionName);

        if (action != Q_NULLPTR) {
            setActionMask(newActionName, actionMask);
        }

        return action;
    }


    QAction* ProgrammaticMainWindow::addToolbarAction(
            const QString& toolbarName,
            const QIcon&   icon,
            const QString& iconText,
            const QString& actionName
        ) {
        QString newActionName = actionName.isEmpty() ? iconText : actionName;

        QToolBar* toolbar;
        if (!toolbars.contains(toolbarName)) {
            toolbar = addToolBar(toolbarName);
            toolbars.insert(toolbarName, toolbar);
        } else {
            toolbar = toolbars.value(toolbarName);
        }

        QAction* newAction = toolbar->addAction(icon, iconText);
        newAction->setVisible(true);

        addAction(newAction, newActionName);

        return newAction;
    }


    QAction* ProgrammaticMainWindow::addToolbarAction(
            const QString&      toolbarName,
            const QIcon&        icon,
            const QString&      iconText,
            const Util::BitSet& actionMask,
            const QString&      actionName
        ) {
        QString  newActionName = actionName.isEmpty() ? iconText : actionName;
        QAction* action        = addToolbarAction(toolbarName, icon, iconText, newActionName);

        if (action != Q_NULLPTR) {
            setActionMask(newActionName, actionMask);
        }

        return action;
    }


    void ProgrammaticMainWindow::addSeparator(const QString& menuLocation) {
        QStringList                   menuTree     = menuLocation.split("|", Qt::SplitBehaviorFlags::SkipEmptyParts);
        QString                       menuItemName = menuTree.last().trimmed();
        ProgrammaticMainWindow::Node* node         = locateNode(menuTree, true);

        node->menu()->addSeparator();
    }


    QAction* ProgrammaticMainWindow::addSection(const QString& menuLocation, const QString& actionName) {
        QString newActionName = actionName.isEmpty() ? menuLocation : actionName;

        QStringList                   menuTree     = menuLocation.split("|", Qt::SplitBehaviorFlags::SkipEmptyParts);
        QString                       menuItemName = menuTree.last().trimmed();
        ProgrammaticMainWindow::Node* node         = locateNode(menuTree);

        QAction* newAction = node->menu()->addSection(menuItemName);
        addAction(newAction, newActionName);

        return newAction;
    }


    void ProgrammaticMainWindow::addToolbarSeparator(const QString& toolbarName) {
        Q_ASSERT(toolbars.contains(toolbarName));

        QToolBar* toolbar = toolbars.value(toolbarName);
        toolbar->addSeparator();
    }


    QMenu* ProgrammaticMainWindow::addMenu(const QString& menuLocation) {
        ProgrammaticMainWindow::Node* node = locateNode(
            menuLocation.split("|", Qt::SplitBehaviorFlags::SkipEmptyParts),
            true
        );
        return node->menu();
    }


    QAction* ProgrammaticMainWindow::addMenu(const QString& menuLocation, QMenu* menu) {
        QStringList                   menuTree     = menuLocation.split("|", Qt::SplitBehaviorFlags::SkipEmptyParts);
        QString                       menuItemName = menuTree.last().trimmed();
        ProgrammaticMainWindow::Node* node         = locateNode(menuTree);
        QMenu*                        parentMenu   = node->menu();

        menu->setTitle(menuItemName);
        QAction* action = parentMenu->addMenu(menu);

        return action;
    }


    void ProgrammaticMainWindow::setToolTip(const QString& actionName, const QString& toolTip) {
        Q_ASSERT(actions.contains(actionName));
        actions.value(actionName)->setToolTip(toolTip);
    }


    void ProgrammaticMainWindow::setStatusTip(const QString& actionName, const QString& statusTip) {
        Q_ASSERT(actions.contains(actionName));
        actions.value(actionName)->setStatusTip(statusTip);
    }


    void ProgrammaticMainWindow::setShortcut(const QString& actionName, const QKeySequence& shortcut) {
        Q_ASSERT(actions.contains(actionName));
        actions.value(actionName)->setShortcut(shortcut);
    }


    void ProgrammaticMainWindow::setWhatsThis(const QString& actionName, const QString& text) {
        Q_ASSERT(actions.contains(actionName));
        actions.value(actionName)->setWhatsThis(text);
    }


    ProgrammaticDockWidget* ProgrammaticMainWindow::dockWidget(const QString& dockName) {
        Q_ASSERT(dockWidgets.contains(dockName));
        return dockWidgets.value(dockName);
    }


//    void ProgrammaticMainWindow::addDockWidget(
//            const QString&           dockName,
//            ProgrammaticDockWidget*  dock,
//            DockWidgetDefaults::Area defaultLocation
//        ) {
//        DockWidgetDefaults defaults(dockName, defaultLocation, true);
//        addDockWidget(dockName, dock, defaults);
//    }


    void ProgrammaticMainWindow::addDockWidget(
            const QString&            dockName,
            ProgrammaticDockWidget*   dock,
            const DockWidgetDefaults& defaults
        ) {
        Q_ASSERT(!dockWidgets.contains(dockName));

        dock->setParent(this);
        dock->setObjectName("dock."+dockName);
        dockWidgets.insert(dockName, dock);

        dockWidgetDefaults.insert(dockName, defaults);

        dockWidgetDefaults.insert(dockName, defaults);
        if (!addDockWidget(defaults.dockArea(), dock)) {
            addDockWidget(DockWidgetDefaults::Area::TOP, dock);
        }
    }


//    ProgrammaticDockWidget* ProgrammaticMainWindow::addDockWidget(
//            const QString&           dockName,
//            const QString&           dockTitle,
//            DockWidgetDefaults::Area defaultLocation
//        ) {
//        DockWidgetDefaults defaults(dockName, defaultLocation, true);
//        return addDockWidget(dockName, dockTitle, defaults);
//    }


    ProgrammaticDockWidget* ProgrammaticMainWindow::addDockWidget(
            const QString&            dockName,
            const QString&            dockTitle,
            const DockWidgetDefaults& defaults
        ) {
        Q_ASSERT(!dockWidgets.contains(dockName));

        ProgrammaticDockWidget* dock = new ProgrammaticDockWidget(dockTitle, this);
        dockWidgets.insert(dockName, dock);
        dock->setObjectName("dock."+dockName);

        dockWidgetDefaults.insert(dockName, defaults);

        if (!addDockWidget(defaults.dockArea(), dock)) {
            addDockWidget(DockWidgetDefaults::Area::TOP, dock);
        }

        return dock;
    }


    QAction* ProgrammaticMainWindow::addDockToggleMenuItem(
            const QString& dockName,
            const QString& menuLocation,
            const QString& actionName
        ) {
        Q_ASSERT(dockWidgets.contains(dockName));

        ProgrammaticDockWidget*       dock         = dockWidgets.value(dockName);
        QAction*                      toggleAction = dock->toggleViewAction();

        QStringList                   menuTree     = menuLocation.split("|", Qt::SplitBehaviorFlags::SkipEmptyParts);
        QString                       menuItemName = menuTree.last().trimmed();
        ProgrammaticMainWindow::Node* node         = locateNode(menuTree, true);

        node->menu()->addAction(toggleAction);

        if (!actionName.isEmpty()) {
            addAction(toggleAction, actionName);
        }

        return toggleAction;
    }


    void ProgrammaticMainWindow::setActionState(const Util::BitSet& newActionState) {
        Util::BitSet oldActionState = currentActionState;

        ActionStateMap removedMap  = processActionState(~newActionState, &currentlyEnabledActions, false);
        ActionStateMap insertedMap = processActionState(newActionState, &currentlyDisabledActions, true);

        currentlyDisabledActions += removedMap;
        currentlyEnabledActions += insertedMap;

        for (  ActionStateMap::const_iterator it  = currentMultiBitMaskActions.constBegin(),
                                              end = currentMultiBitMaskActions.constEnd()
             ; it != end
             ; ++it
            ) {
            QAction* action = it.value();
            action->setEnabled(newActionState.intersects(it.key()));
        }

        currentActionState = newActionState;

        for (ProxyList::const_iterator it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->actionStateChanged(oldActionState, newActionState);
        }
    }


    Util::BitSet ProgrammaticMainWindow::actionState() const {
        return currentActionState;
    }


    void ProgrammaticMainWindow::setActionMask(QAction* action, const Util::BitSet& actionMask) {
        if (currentMasksByAction.contains(action)) {
            // Action is already defined, remove it.
            Util::BitSet oldMask = currentMasksByAction.value(action);

            unsigned numberRemoved = currentlyEnabledActions.remove(oldMask, action);
            numberRemoved += currentlyDisabledActions.remove(oldMask, action);

            Q_ASSERT(numberRemoved == 1);
        }

        currentMasksByAction.insert(action, actionMask);

        if (actionMask.numberSetBits() == 1) {
            if (currentActionState.intersects(actionMask)) {
                removeFromMap(action, currentlyDisabledActions);
                removeFromMap(action, currentMultiBitMaskActions);

                currentlyEnabledActions.insert(actionMask, action);
                action->setEnabled(true);
            } else {
                removeFromMap(action, currentlyEnabledActions);
                removeFromMap(action, currentMultiBitMaskActions);

                currentlyDisabledActions.insert(actionMask, action);
                action->setEnabled(false);
            }
        } else {
            removeFromMap(action, currentlyEnabledActions);
            removeFromMap(action, currentlyDisabledActions);

            currentMultiBitMaskActions.insert(actionMask, action);
            action->setEnabled(currentActionState.intersects(actionMask));
        }
    }


    bool ProgrammaticMainWindow::setActionMask(const QString& actionName, const Util::BitSet& actionMask) {
        bool     success = false;

        QAction* action  = actions.value(actionName, Q_NULLPTR);
        if (action != Q_NULLPTR) {
            setActionMask(action, actionMask);
            success = true;
        }

        return success;
    }


    Util::BitSet ProgrammaticMainWindow::actionMask(QAction* action) {
        return currentMasksByAction.value(action);
    }


    Util::BitSet ProgrammaticMainWindow::actionMask(const QString& actionName) {
        Util::BitSet mask;

        QAction* action = actions.value(actionName, Q_NULLPTR);
        mask = actionMask(action);

        return mask;
    }


    void ProgrammaticMainWindow::runBuilders() {
        for (auto it=ProgrammaticApplication::begin(),end=ProgrammaticApplication::end() ; it!=end ; ++it) {
            Builder* builder = dynamic_cast<Builder*>(*it);
            if (builder != Q_NULLPTR) {
                builder->buildMainWindowGui(this);
            }
        }

        for (auto it=ProgrammaticApplication::begin(),end=ProgrammaticApplication::end() ; it!=end ; ++it) {
            Builder* builder = dynamic_cast<Builder*>(*it);
            if (builder != Q_NULLPTR) {
                builder->buildAdditionalMainWindowActions(this);
            }
        }

        for (auto it=ProgrammaticApplication::begin(),end=ProgrammaticApplication::end() ; it!=end ; ++it) {
            Builder* builder = dynamic_cast<Builder*>(*it);
            if (builder != Q_NULLPTR) {
                builder->buildMainWindowDocks(this);
            }
        }

        for (auto it=dockWidgets.begin(),end=dockWidgets.end() ; it!=end ; ++it) {
            it.value()->configureDockWidget(this);
        }

        for (auto it=ProgrammaticApplication::begin(),end=ProgrammaticApplication::end() ; it!=end ; ++it) {
            Builder* builder = dynamic_cast<Builder*>(*it);
            if (builder != Q_NULLPTR) {
                ProgrammaticMainWindowProxy* proxy = builder->createMainWindowProxy(this);
                if (proxy != Q_NULLPTR) {
                    addProxy(builder->builderIdentifier(), proxy);
                }
            }
        }

        bind();

        for (auto it=ProgrammaticApplication::begin(),end=ProgrammaticApplication::end() ; it!=end ; ++it) {
            Builder* builder = dynamic_cast<Builder*>(*it);
            if (builder != Q_NULLPTR) {
                builder->buildMainWindowFinal(this);
            }
        }

        for (auto it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->windowConfigured();
        }
    }


    bool ProgrammaticMainWindow::okToClose() {
        auto begin = beginProxies();
        auto end   = endProxies();
        auto it    = begin;

        while (it!=end && (*it)->okToClose()) {
            ++it;
        }

        return it == end;
    }


    void ProgrammaticMainWindow::closing() {
        for (auto it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->closing();
        }
    }


    void ProgrammaticMainWindow::showEvent(QShowEvent* event) {
        QMainWindow::showEvent(event);
        showTimer->start(0);
    }


    void ProgrammaticMainWindow::resizeEvent(QResizeEvent* event) {
        QMainWindow::resizeEvent(event);

        for (auto it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->resized(event);
        }

        dockWidgetRestackingTimer->start(dockWidgetRestackingDelay);
    }


    void ProgrammaticMainWindow::nowVisible() {
        for (auto it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->windowNowVisible();
        }

        updateDocks();
    }


    void ProgrammaticMainWindow::updateDocks() {
        // Note: I have seen corner cases where geometry values appear negative.  To avoid issues, we use signed
        //       math in this method.

        int maximumDesiredRightEdgeLeftDock = std::numeric_limits<int>::min();
        int minimumDesiredLeftEdgeRightDock = std::numeric_limits<int>::max();
        int maximumDesiredBottomEdgeTopDock = std::numeric_limits<int>::min();
        int minimumDesiredTopEdgeBottomDock = std::numeric_limits<int>::max();

        for (  DockWidgetsByName::const_iterator dockWidgetIterator    = dockWidgets.constBegin(),
                                                 dockWidgetEndIterator = dockWidgets.constEnd()
             ; dockWidgetIterator != dockWidgetEndIterator
             ; ++dockWidgetIterator
            ) {
            ProgrammaticDockWidget* dock = *dockWidgetIterator;
            if (dock->isVisible() && dock->isDocked() && !dock->visibleRegion().isEmpty()) {
                QString dockName = dock->objectName();

                Qt::DockWidgetArea dockArea = ProgrammaticMainWindow::dockWidgetArea(dock);
                if (modifiedDockAreas.contains(dockArea)) {
                    QRect dockBoundingRectangle = dock->geometry();

                    switch (dockArea) {
                        case Qt::LeftDockWidgetArea: {
                            unsigned minimumWidth = dock->minimumWidthVerticalOrientation();
                            if (minimumWidth != 0) {
                                int rightEdge = dockBoundingRectangle.left() + minimumWidth;
                                if (rightEdge > maximumDesiredRightEdgeLeftDock) {
                                    maximumDesiredRightEdgeLeftDock = rightEdge;
                                }
                            }

                            break;
                        }

                        case Qt::RightDockWidgetArea: {
                            unsigned minimumWidth = dock->minimumWidthVerticalOrientation();
                            if (minimumWidth != 0) {
                                int leftEdge = dockBoundingRectangle.right() - minimumWidth;
                                if (leftEdge < minimumDesiredLeftEdgeRightDock) {
                                    minimumDesiredLeftEdgeRightDock = leftEdge;
                                }
                            }

                            break;
                        }

                        case Qt::TopDockWidgetArea: {
                            unsigned minimumHeight = dock->minimumHeightHorizontalOrientation();
                            if (minimumHeight != 0) {
                                int bottomEdge = dockBoundingRectangle.top() + minimumHeight;
                                if (bottomEdge > maximumDesiredBottomEdgeTopDock) {
                                    maximumDesiredBottomEdgeTopDock = bottomEdge;
                                }
                            }

                            break;
                        }

                        case Qt::BottomDockWidgetArea: {
                            unsigned minimumHeight = dock->minimumHeightHorizontalOrientation();
                            if (minimumHeight != 0) {
                                int topEdge = dockBoundingRectangle.bottom() - minimumHeight;
                                if (topEdge < minimumDesiredTopEdgeBottomDock) {
                                    minimumDesiredTopEdgeBottomDock = topEdge;
                                }
                            }

                            break;
                        }

                        case Qt::NoDockWidgetArea: {
                            Q_ASSERT(false);
                            break;
                        }

                        case Qt::AllDockWidgetAreas: {
                            Q_ASSERT(false);
                            break;
                        }
                    }
                }
            }
        }

        for (  DockWidgetsByName::const_iterator dockWidgetIterator    = dockWidgets.constBegin(),
                                                 dockWidgetEndIterator = dockWidgets.constEnd()
             ; dockWidgetIterator != dockWidgetEndIterator
             ; ++dockWidgetIterator
            ) {
            ProgrammaticDockWidget* dock = *dockWidgetIterator;
            if (dock->isVisible() && dock->isDocked() && !dock->visibleRegion().isEmpty()) {
                Qt::DockWidgetArea dockArea = ProgrammaticMainWindow::dockWidgetArea(dock);
                if (modifiedDockAreas.contains(dockArea)) {
                    QRect dockBoundingRectangle = dock->geometry();

                    switch (dockArea) {
                        case Qt::LeftDockWidgetArea: {
                            unsigned minimumWidth = dock->minimumWidthVerticalOrientation();
                            int adjustment = (
                                  maximumDesiredRightEdgeLeftDock
                                - dockBoundingRectangle.left()
                                - minimumWidth
                            );

                            dock->setWidthAdjustment(std::max(0, adjustment));
                            break;
                        }

                        case Qt::RightDockWidgetArea: {
                            unsigned minimumWidth = dock->minimumWidthVerticalOrientation();
                            int adjustment = (
                                  dockBoundingRectangle.right()
                                - minimumWidth
                                - minimumDesiredLeftEdgeRightDock
                            );

                            dock->setWidthAdjustment(std::max(0, adjustment));
                            break;
                        }

                        case Qt::TopDockWidgetArea: {
                            unsigned minimumHeight = dock->minimumHeightHorizontalOrientation();
                            int adjustment = (
                                  maximumDesiredBottomEdgeTopDock
                                - dockBoundingRectangle.top()
                                - minimumHeight
                            );

                            dock->setHeightAdjustment(std::max(0, adjustment));
                            break;
                        }

                        case Qt::BottomDockWidgetArea: {
                            unsigned minimumHeight = dock->minimumHeightHorizontalOrientation();
                            int adjustment = (
                                  dockBoundingRectangle.bottom()
                                - minimumHeight
                                - minimumDesiredTopEdgeBottomDock
                            );

                            dock->setHeightAdjustment(std::max(0, adjustment));
                            break;
                        }

                        case Qt::NoDockWidgetArea: {
                            Q_ASSERT(false);
                            break;
                        }

                        case Qt::AllDockWidgetAreas: {
                            Q_ASSERT(false);
                            break;
                        }
                    }
                }
            }
        }

        modifiedDockAreas.clear();
    }


    void ProgrammaticMainWindow::tabifiedDockWidgetWasActivated(QDockWidget* dockWidget) {
        Qt::DockWidgetArea dockArea = ProgrammaticMainWindow::dockWidgetArea(dockWidget);
        modifiedDockAreas.insert(dockArea);

        if (!dockWidgetUpdateTimer->isActive()) {
            dockWidgetUpdateTimer->start(dockWidgetUpdateDelay1);
        }
    }


    void ProgrammaticMainWindow::restackDockWidgets() {
        DockWidgetsByPositionByArea dockWidgetsByPositionByArea;

        for (  DockWidgetsByName::const_iterator dockWidgetIterator = dockWidgets.constBegin(),
                                                 dockWidgetEndIterator = dockWidgets.constEnd()
             ; dockWidgetIterator != dockWidgetEndIterator
             ; ++dockWidgetIterator
            ) {
            ProgrammaticDockWidget* dock = *dockWidgetIterator;
            if (dock->isVisible() && dock->isDocked() && !dock->visibleRegion().isEmpty()) {
                Qt::DockWidgetArea dockWidgetArea = ProgrammaticMainWindow::dockWidgetArea(dock);

                if (!dockWidgetsByPositionByArea.contains(dockWidgetArea)) {
                    dockWidgetsByPositionByArea.insert(dockWidgetArea, DockWidgetsByPosition());
                }

                DockWidgetsByPosition& dockWidgetsByPosition = dockWidgetsByPositionByArea[dockWidgetArea];

                unsigned position = 0;
                switch (dockWidgetArea) {
                    case Qt::LeftDockWidgetArea:
                    case Qt::RightDockWidgetArea: {
                        position = dock->geometry().top();
                        break;
                    }

                    case Qt::TopDockWidgetArea:
                    case Qt::BottomDockWidgetArea: {
                        position = dock->geometry().left();
                        break;
                    }

                    case Qt::NoDockWidgetArea: {
                        Q_ASSERT(false);
                        break;
                    }

                    case Qt::AllDockWidgetAreas: {
                        Q_ASSERT(false);
                        break;
                    }
                }

                dockWidgetsByPosition.insert(position, dock);
            }
        }

        bool restacked = false;
        for (  DockWidgetsByPositionByArea::const_iterator
                   dockAreaIterator    = dockWidgetsByPositionByArea.constBegin(),
                   dockAreaEndIterator = dockWidgetsByPositionByArea.constEnd()
             ; dockAreaIterator != dockAreaEndIterator
             ; ++dockAreaIterator
            ) {
            const DockWidgetsByPosition& dockWidgetsByPosition = dockAreaIterator.value();
            unsigned                     numberDocks           = static_cast<unsigned>(dockWidgetsByPosition.size());

            if (numberDocks > 1) {
                QList<ProgrammaticDockWidget*> docksInOrder = dockWidgetsByPosition.values();
                ProgrammaticDockWidget*        lastDock     = docksInOrder.last();
                if (lastDock->geometry().right() > size().width()) {
                    ProgrammaticDockWidget* secondToLast = docksInOrder.at(numberDocks - 2);
                    combineDocks(secondToLast, lastDock);
                    restacked = true;
                } else {
                    unsigned dockIndex         = numberDocks - 1;
                    bool     restackedThisArea = false;
                    do {
                        --dockIndex;
                        ProgrammaticDockWidget* dock = docksInOrder.at(dockIndex);
                        if (lastDock->geometry().intersects(dock->geometry())) {
                            combineDocks(dock, lastDock);
                            restackedThisArea = true;
                            restacked         = true;
                        } else {
                            lastDock = dock;
                        }
                    } while (dockIndex > 0 && !restackedThisArea);
                }
            }
        }

        if (restacked) {
            dockWidgetRestackingTimer->start(1); // Try again until no more restacking is needed.
        }
    }


    void ProgrammaticMainWindow::dockStateChanged(ProgrammaticDockWidget* dockWidget) {
        if (currentDockLocations.contains(dockWidget)) {
            Qt::DockWidgetArea oldDockLocation = currentDockLocations.value(dockWidget);
            modifiedDockAreas.insert(oldDockLocation);
        }

        Qt::DockWidgetArea dockWidgetArea = ProgrammaticMainWindow::dockWidgetArea(dockWidget);
        currentDockLocations.insert(dockWidget, dockWidgetArea);
        modifiedDockAreas.insert(dockWidgetArea);

        if (!dockWidgetUpdateTimer->isActive()) {
            dockWidgetUpdateTimer->start(dockWidgetUpdateDelay1);
        }
    }


    bool ProgrammaticMainWindow::addDockWidget(DockWidgetDefaults::Area location, QDockWidget* dockWidget) {
        bool addedToWindow;

        switch (location) {
            case DockWidgetDefaults::Area::TOP: {
                QMainWindow::addDockWidget(Qt::TopDockWidgetArea, dockWidget);
                addedToWindow = true;
                break;
            }

            case DockWidgetDefaults::Area::LEFT: {
                QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
                addedToWindow = true;
                break;
            }

            case DockWidgetDefaults::Area::RIGHT: {
                QMainWindow::addDockWidget(Qt::RightDockWidgetArea, dockWidget);
                addedToWindow = true;
                break;
            }

            case DockWidgetDefaults::Area::BOTTOM: {
                QMainWindow::addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
                addedToWindow = true;
                break;
            }

            case DockWidgetDefaults::Area::BASED_ON_SIBLING: {
                addedToWindow = false;
                break;
            }

            default: {
                Q_ASSERT(false);

                addedToWindow = false;
                break;
            }
        }

        return addedToWindow;
    }

    ProgrammaticMainWindow::Node* ProgrammaticMainWindow::locateNode(
            const QStringList& menuTree,
            bool               includeLastItem
        ) {
        QString rootEntry = menuTree.first().trimmed();

        ProgrammaticMainWindow::Node* node;
        if (rootNodes.contains(rootEntry)) {
            node = rootNodes.value(rootEntry);
        } else {
            node = new ProgrammaticMainWindow::Node(rootEntry, true, this);
            menuBar()->addMenu(node->menu());
            rootNodes.insert(rootEntry, node);
        }

        int first = 1;
        int last  = includeLastItem ? menuTree.size() - 1 : menuTree.size() - 2;

        for (int i=first ; i<=last ; ++i) {
            QString menuEntry = menuTree.at(i).trimmed();

            if (node->childExists(menuEntry)) {
                node = node->child(menuEntry);
            } else {
                node = node->addChildMenu(menuEntry);
            }
        }

        return node;
    }


    ProgrammaticMainWindow::ActionStateMap ProgrammaticMainWindow::processActionState(
            const Util::BitSet& actionState,
            ActionStateMap*     multiMap,
            bool                enableAction
        ) {
        Util::BitSetReverseIterator iterator(actionState);
        ActionStateMap              adjustedActions;

        while (iterator.isNotEnd() && !multiMap->isEmpty()) {
            ActionStateMap::iterator mapIterator = multiMap->lowerBound(*iterator);

            while (mapIterator != multiMap->end() && mapIterator.key().intersects(*iterator)) {
                const Util::BitSet& entryActionState = mapIterator.key();
                QAction*            action           = mapIterator.value();

                adjustedActions.insert(entryActionState, action);
                action->setEnabled(enableAction);

                mapIterator = multiMap->erase(mapIterator);
            }

            ++iterator;
        }

        return adjustedActions;
    }


    bool ProgrammaticMainWindow::removeFromMap(const QAction* action, ActionStateMap& actionMap) {
        bool result;

        ActionStateMap::iterator it  = actionMap.begin();
        ActionStateMap::iterator end = actionMap.end();

        while (it != end && it.value() != action) {
            ++it;
        }

        if (it != end) {
            actionMap.erase(it);
            result = true;
        } else {
            result = false;
        }

        return result;
    }


    void ProgrammaticMainWindow::combineDocks(
            ProgrammaticDockWidget* firstToCombine,
            ProgrammaticDockWidget* secondToCombine
        ) {
        QList<QDockWidget*> docks = tabifiedDockWidgets(secondToCombine);
        docks.append(secondToCombine);
        for (QList<QDockWidget*>::const_iterator it=docks.constBegin(),end=docks.constEnd() ; it!=end ; ++it) {
            tabifyDockWidget(firstToCombine, *it);
        }
    }
}
