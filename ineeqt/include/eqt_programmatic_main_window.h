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
* This header defines the \ref EQt::ProgrammaticMainWindow class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_MAIN_WINDOW_H
#define EQT_PROGRAMMATIC_MAIN_WINDOW_H

#include <QMap>
#include <QIcon>
#include <QKeySequence>
#include <QStringList>
#include <QMap>
#include <QSet>
#include <QMultiMap>
#include <QMainWindow>

#include <cstdint>

#include <util_bit_set.h>

#include "eqt_programmatic_application.h"
#include "eqt_programmatic_main_window_proxy.h"
#include "eqt_dock_widget_defaults.h"
#include "eqt_programmatic_window.h"
#include "eqt_builder.h"

class QWidget;
class QAction;
class QMenu;
class QToolBar;
class QCloseEvent;
class QResizeEvent;
class QShowEvent;
class QTimer;

namespace EQt {
    class ProgrammaticDockWidget;

    /**
     * Class that extends QMainWindow to allow simplified, programmatic creation of application main windows.
     *
     * The class inherits from QMainWindow and provides programmatic proxy classes to be used with the main window.
     * The class also provides support for main window global actions, including the ability to group and manage
     * actions based on flags.
     *
     * Note that this class includes hacks specific to MacOS to disable and hide several MacOS specific menu items:
     *
     *     * "Edit | Start Dictation..."
     *     * "Edit | Emoji & Symbols"
     *     * "Help | Search"
     *
     * These menu items were disabled because Qt does not provide a viable mechanism to tie into these menu items thus
     * rendering the menu items unusable with Qt applications.
     */
    class EQT_PUBLIC_API ProgrammaticMainWindow:public QMainWindow,
                                                public ProgrammaticWindow<
                                                    ProgrammaticMainWindowProxy,
                                                    ProgrammaticMainWindow
                                                > {
        Q_OBJECT

        friend class ProgrammaticDockWidget;

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            ProgrammaticMainWindow(QWidget* parent = Q_NULLPTR);

            ~ProgrammaticMainWindow() override;

            /**
             * You can use this method to reset all the docks to their default locations and visible state.
             */
            void resetDocksToDefaultState();

            /**
             * You can use this method to obtain an action given it's name.
             *
             * \param[in] actionName The action name to query.
             *
             * \return Returns the requested action.  A Q_NULLPTR is returned if the action does not exist.
             */
            inline QAction* action(const QString& actionName) {
                return actions.value(actionName, Q_NULLPTR);
            }

            /**
             * You can use this method to determine if an action already exists.
             *
             * \param[in] actionName The action name to query.
             *
             * \return Returns true if the action exists, returns false if the action does not exist.
             */
            inline bool actionExists(const QString& actionName) {
                return actions.contains(actionName);
            }

            /**
             * You can use this method to determine if an action does not exist.
             *
             * \param[in] actionName The action name to query.
             *
             * \return Returns true if the action does not exist, returns false if the action exists.
             */
            inline bool actionDoesNotExist(const QString& actionName) {
                return !actionExists(actionName);
            }

            /**
             * Adds a new menu action to the action registry.  This method will assert if the action already exists.
             *
             * \param[in] newAction  Adds a new action to the action registry.
             *
             * \param[in] actionName The name to use to reference this action.
             *
             * \return Returns a pointer to supplied action.
             */
            QAction* addAction(QAction* newAction, const QString& actionName);

            /**
             * Adds a new menu action to the action registry.  This method will assert if the action already exists.
             *
             * \param[in] newAction  Adds a new action to the action registry.
             *
             * \param[in] actionMask The mask used to enable or disable this action.
             *
             * \param[in] actionName The name to use to reference this action.
             *
             * \return Returns a pointer to supplied action.
             */
            QAction* addAction(QAction* newAction, const Util::BitSet& actionMask, const QString& actionName);

            /**
             * Adds a new menu action to the action registry.  This method will assert if the action already exists.
             *
             * \param[in] icon       Icon to assign to the action.
             *
             * \param[in] iconText   The text to associate with the icon.
             *
             * \param[in] actionName The name to use to reference this action.  If an empty string is specified, the
             *                       menu path is used as the action name.
             *
             * \return Returns a pointer to supplied action.
             */
            QAction* addAction(const QIcon& icon, const QString& iconText, const QString& actionName = QString());

            /**
             * Adds a new menu action to the action registry.  This method will assert if the action already exists.
             *
             * \param[in] icon       Icon to assign to the action.
             *
             * \param[in] iconText   The text to associate with the icon.
             *
             * \param[in] actionMask The mask used to enable or disable this action.
             *
             * \param[in] actionName The name to use to reference this action.  If an empty string is specified, the
             *                       menu path is used as the action name.
             *
             * \return Returns a pointer to supplied action.
             */
            QAction* addAction(
                const QIcon&         icon,
                const QString&       iconText,
                const Util::BitSet&  actionMask,
                const QString&       actionName = QString()
            );

            /**
             * Adds a new menu action to the window's menu.  This method will assert if the action already exists.
             *
             * \param[in] menuLocation A location string used to indicate where this action should reside in the user
             *                         menus.  The name should be a path using vertical bar | separators.  You can also
             *                         include ampersands to identify menu shortcuts.
             *
             * \param[in] actionName   The name to use to reference this action.  If an empty string is specified, the
             *                         menu path is used as the action name.
             *
             * \return Returns a pointer to the newly created action.
             */
            QAction* addMenuAction(const QString& menuLocation, const QString& actionName = QString());

            /**
             * Adds a new menu action to the window's menu.  This method will assert if the action already exists.
             *
             * \param[in] menuLocation A location string used to indicate where this action should reside in the user
             *                         menus.  The name should be a path using vertical bar | separators.  You can also
             *                         include ampersands to identify menu shortcuts.
             *
             * \param[in] actionMask   The mask used to enable or disable this action.
             *
             * \param[in] actionName   The name to use to reference this action.  If an empty string is specified, the
             *                         menu path is used as the action name.
             *
             * \return Returns a pointer to the newly created action.
             */
            QAction* addMenuAction(
                const QString&      menuLocation,
                const Util::BitSet& actionMask,
                const QString&      actionName = QString()
            );

            /**
             * Adds a new menu action to the window's menu.  This method will assert if the action already exists.
             *
             * \param[in] icon         The icon to assign to this action.
             *
             * \param[in] menuLocation A location string used to indicate where this action should reside in the user
             *                         menus.  The name should be a path using vertical bar | separators.  You can also
             *                         include ampersands to identify menu shortcuts.
             *
             * \param[in] actionName   The name to use to reference this action.  If an empty string is specified, the
             *                         menu path is used as the action name.
             *
             * \return Returns a pointer to the newly created action.
             */
            QAction* addMenuAction(
                const QIcon&   icon,
                const QString& menuLocation,
                const QString& actionName = QString()
            );

            /**
             * Adds a new menu action to the window's menu.  This method will assert if the action already exists.
             *
             * \param[in] icon         The icon to assign to this action.
             *
             * \param[in] menuLocation A location string used to indicate where this action should reside in the user
             *                         menu.  The name should be a path using vertical bar | separators.  You can also
             *                         include ampersands to identify menu shortcuts.
             *
             * \param[in] actionMask   The mask used to enable or disable this action.
             *
             * \param[in] actionName   The name to use to reference this action.  If an empty string is specified, the
             *                         menu path is used as the action name.
             *
             * \return Returns a pointer to the newly created action.
             */
            QAction* addMenuAction(
                const QIcon&        icon,
                const QString&      menuLocation,
                const Util::BitSet& actionMask,
                const QString&      actionName = QString()
            );

            /**
             * Adds a new menu action to the window's menu as well as a toolbar.  This method will assert if the action
             * already exists.
             *
             * \param[in] toolbarName  The name of the toolbar to include this action.
             *
             * \param[in] icon         The icon to assign to this action.
             *
             * \param[in] menuLocation A location string used to indicate where this action should reside in the user
             *                         menus.  The name should be a path using vertical bar | separators.  You can also
             *                         include ampersands to identify menu shortcuts.
             *
             * \param[in] actionName   The name to use to reference this action.  If an empty string is specified, the
             *                         menu path is used as the action name.
             *
             * \return Returns a pointer to the newly created action.
             */
            QAction* addMenuAndToolbarAction(
                const QString& toolbarName,
                const QIcon&   icon,
                const QString& menuLocation,
                const QString& actionName = QString()
            );

            /**
             * Adds a new menu action to the window's menu as well as a toolbar.  This method will assert if the action
             * already exists.
             *
             * \param[in] toolbarName  The name of the toolbar to include this action.
             *
             * \param[in] icon         The icon to assign to this action.
             *
             * \param[in] menuLocation A location string used to indicate where this action should reside in the user
             *                         menus.  The name should be a path using vertical bar | separators.  You can also
             *                         include ampersands to identify menu shortcuts.
             *
             * \param[in] actionMask   The mask used to enable or disable this action.
             *
             * \param[in] actionName   The name to use to reference this action.  If an empty string is specified, the
             *                         menu path is used as the action name.
             *
             * \return Returns a pointer to the newly created action.
             */
            QAction* addMenuAndToolbarAction(
                const QString&      toolbarName,
                const QIcon&        icon,
                const QString&      menuLocation,
                const Util::BitSet& actionMask,
                const QString&      actionName = QString()
            );

            /**
             * Adds a new action to a toolbar only.  This method will assert if the action already exists.
             *
             * \param[in] toolbarName The name of the toolbar to include this action.
             *
             * \param[in] icon        The icon to assign to this action.
             *
             * \param[in] iconText    The text to associate with the icon.
             *
             * \param[in] actionName  The name to use to reference this action.  If an empty string is specified, the
             *                        icon text is used as the action name.
             *
             * \return Returns a pointer to the newly created action.
             */
            QAction* addToolbarAction(
                const QString& toolbarName,
                const QIcon&   icon,
                const QString& iconText,
                const QString& actionName = QString()
            );

            /**
             * Adds a new action to a toolbar only.  This method will assert if the action already exists.
             *
             * \param[in] toolbarName The name of the toolbar to include this action.
             *
             * \param[in] icon        The icon to assign to this action.
             *
             * \param[in] iconText    The text to associate with the icon.
             *
             * \param[in] actionMask  The mask used to enable or disable this action.
             *
             * \param[in] actionName  The name to use to reference this action.  If an empty string is specified, the
             *                        icon text is used as the action name.
             *
             * \return Returns a pointer to the newly created action.
             */
            QAction* addToolbarAction(
                const QString&      toolbarName,
                const QIcon&        icon,
                const QString&      iconText,
                const Util::BitSet& actionMask,
                const QString&      actionName = QString()
            );

            /**
             * Adds a separator to a menu.
             *
             * \param[in] menuLocation A location string used to indicate where this separator should reside in the
             *                         user menus.  The name should be a path using vertical bar | separators.  You can
             *                         also include ampersands to identify menu shortcuts.
             */
            void addSeparator(const QString& menuLocation);

            /**
             * Adds a section to a menu.   A section acts much like a separator but can optionally include text on
             * platforms that support text in separators.
             *
             * \param[in] menuLocation A location string used to indicate where this action should reside in the user
             *                         menus.  The name should be a path using vertical bar | separators.  You can also
             *                         include ampersands to identify menu shortcuts.
             *
             * \param[in] actionName   The name to use to reference this action.  If an empty string is specified, the
             *                         menu path is used as the action name.
             *
             * \return Returns a pointer to the newly created section action.
             */
            QAction* addSection(const QString& menuLocation, const QString& actionName = QString());

            /**
             * Adds a menu containing no entries.
             *
             * \param[in] menuLocation A location string used to indicate where this action should reside in the user
             *                         menus.  The name should be a path using vertical bar | separators.  You can also
             *                         include ampersands to identify menu shortcuts.
             *
             * \return Returns a pointer to the newly created section action.
             */
            QMenu* addMenu(const QString& menuLocation);

            /**
             * Adds an externally defined menu.
             *
             * \param[in] menuLocation A location string used to indicate where this menu should reside in the user
             *                         menu tree.  The name should be a path using vertical bar | separators.  You can
             *                         also include ampersands to identify menu shortcuts.
             *
             * \param[in] menu         The menu to be added.
             *
             * \return Returns a pointer to the newly created menu action.
             */
            QAction* addMenu(const QString& menuLocation, QMenu* menu);

            /**
             * Adds a break to a toolbar.
             *
             * \param[in] toolbarName The toolbar to receive the break.  Note that the function will assert if the
             *                        toolbar does not already exist.
             */
            void addToolbarSeparator(const QString& toolbarName);

            /**
             * Adds a tooltip to a registered action.
             *
             * \param[in] actionName The name to use to reference the action.
             *
             * \param[in] toolTip    The tooltip text to display for this action.
             */
            void setToolTip(const QString& actionName, const QString& toolTip);

            /**
             * Adds a status tip to a registered action.
             *
             * \param[in] actionName The name to use to reference the action.
             *
             * \param[in] statusTip  The status tip text to display for this action.
             */
            void setStatusTip(const QString& actionName, const QString& statusTip);

            /**
             * Adds a shortcut to a registered action.
             *
             * \param[in] actionName The name to use to reference the action.
             *
             * \param[in] shortcut   The key sequence to apply as the shortcut.
             */
            void setShortcut(const QString& actionName, const QKeySequence& shortcut);

            /**
             * Adds What's This ? text shortcut to a registered action.
             *
             * \param[in] actionName The name to use to reference the action.
             *
             * \param[in] text       Text to display when the user click's the "What's This?" question mark or
             *                       presses the "What's This" shortcut key.
             */
            void setWhatsThis(const QString& actionName, const QString& text);

            /**
             * Obtains a docket widget associated with this window.  This method will assert if the dock does not
             * already exist.
             *
             * \param[in] dockName The name of the dock.
             *
             * \return Returns a pointer to the desired dock widget.
             */
            ProgrammaticDockWidget* dockWidget(const QString& dockName);

//            /**
//             * Registers a new dock widget with this window.  This method will reparent the dock window.
//             *
//             * \param[in] dockName        The name of the dock.
//             *
//             * \param[in] dock            The dock to be added.
//             *
//             * \param[in] defaultLocation The default location for the dock widget.
//             */
//            void addDockWidget(
//                const QString&           dockName,
//                ProgrammaticDockWidget*  dock,
//                DockWidgetDefaults::Area defaultLocation = DockWidgetDefaults::Area::TOP
//            );

            /**
             * Registers a new dock widget with this window.  This method will reparent the dock window.
             *
             * \param[in] dockName The name of the dock.
             *
             * \param[in] dock     The dock to be added.
             *
             * \param[in] defaults The defaults to use when positioning the dock widget.
             */
            void addDockWidget(
                const QString&            dockName,
                ProgrammaticDockWidget*   dock,
                const DockWidgetDefaults& defaults
            );

//            /**
//             * Creates and registers a new dock widget with this window.
//             *
//             * \param[in] dockName        The name of the dock.
//             *
//             * \param[in] dockTitle       The title to assign to the newly created dock widget.
//             *
//             * \param[in] defaultLocation The default location for the dock widget.
//             *
//             * \return Returns a pointer to the newly created document window.
//             */
//            ProgrammaticDockWidget* addDockWidget(
//                const QString&           dockName,
//                const QString&           dockTitle,
//                DockWidgetDefaults::Area defaultLocation = DockWidgetDefaults::Area::TOP
//            );

            /**
             * Creates and registers a new dock widget with this window.
             *
             * \param[in] dockName  The name of the dock.
             *
             * \param[in] dockTitle The title to assign to the newly created dock widget.
             *
             * \param[in] defaults  The default location for the dock widget.
             *
             * \return Returns a pointer to the newly created document window.
             */
            ProgrammaticDockWidget* addDockWidget(
                const QString&            dockName,
                const QString&            dockTitle,
                const DockWidgetDefaults& defaults
            );

            /**
             * Adds a dock toggle action to a menu.
             *
             * \param[in] dockName     The name of the dock.
             *
             * \param[in] menuLocation The menu location used to access the dock.
             *
             * \param[in] actionName   A name used to reference this action.  This parameter will only be used if it
             *                         is not empty.
             *
             * \return Returns a pointer to the toggle action.
             */
            QAction* addDockToggleMenuItem(
                const QString& dockName,
                const QString& menuLocation,
                const QString& actionName = QString()
            );

            /**
             * Method that obtains a list of all the known actions.
             *
             * \return Returns a string list containing all the known actions.
             */
            inline QStringList allActions() const {
                return QStringList(actions.keys());
            }

            /**
             * Method you can use to set the action state.  The value will be AND'd against the state mask registered
             * with each action.  Actions with non-zero results will be enabled.  Actions with zero results will be
             * disabled.
             *
             * This method will also call the \ref ProgrammaticMainWindowProxy::actionStateChanged method in each proxy
             * class.
             *
             * \param[in] newActionState The new action state value.
             */
            void setActionState(const Util::BitSet& newActionState);

            /**
             * Method you can use to obtain the current action state.
             *
             * \return Return the current action state.
             */
            Util::BitSet actionState() const;

            /**
             * Method you can use to set the action state mask for a given action.
             *
             * \param[in] action     Pointer to the action to have the state mask set for.
             *
             * \param[in] actionMask The mask to tie to this action.
             */
            void setActionMask(QAction* action, const Util::BitSet& actionMask);

            /**
             * Method you can use to set the action state mask for a given action.
             *
             * \param[in] actionName The name of the action to set the mask for.
             *
             * \param[in] actionMask The mask to tie to this action.
             *
             * \return Returns true if the specified action exists.  Returns false if the action does not exist.  If
             *         the action already exists, it's mask will be updated.
             */
            bool setActionMask(const QString& actionName, const Util::BitSet& actionMask);

            /**
             * Method you can use to obtain the action mask for an action, by name.
             *
             * \param[in] action The action to obtain the mask for.
             *
             * \return Returns the action mask.  An empty action mask will be returned if this action has not been
             *         provided with an existing action mask.
             */
            Util::BitSet actionMask(QAction* action);

            /**
             * Method you can use to obtain the action mask for an action, by name.
             *
             * \param[in] actionName The name of the action to obtain the mask for.
             *
             * \return Returns the action mask.  An empty action mask will be returned if this action has not been
             *         provided with an existing action mask.
             */
            Util::BitSet actionMask(const QString& actionName);

        protected:
            /**
             * Method that can be called from derived class constructors to run any registered builders.
             */
            void runBuilders();

            /**
             * Method that can be called from derived classes to determine if any proxy classes wish to block this
             * window from closing.  The method is intended to be called from an instance of closeEvent within a
             * derived class.
             *
             * \return Returns true if the window can be closed.  Returns false if a proxy wants the window to remain
             *         open.
             */
            bool okToClose();

            /**
             * Method that can be called from derived classes to inform proxy classes that this window is in the
             * process of closing.
             *
             * If you decide to overload this method, please be sure to call the base class implementation.
             */
            virtual void closing();

            /**
             * Method that is called by docks to report that a dock state has changed.  You can overload this method if
             * you wish to receive immediate notification that a dock's location has changed.
             *
             * The default implementation will schedule to have dock geometry settings updated.
             *
             * \param[in] dockWidget The dock widget that has changed state.
             */
            virtual void dockStateChanged(ProgrammaticDockWidget* dockWidget);

            /**
             * Event handler that is triggered when the window becomes visible.
             *
             * This event handler will trigger the nowVisible slot.
             *
             * \param[in] event The event that triggered this event handler.
             */
            void showEvent(QShowEvent* event) override;

            /**
             * Event handler that is triggered when the user resizes the window.
             *
             * The event handler will call the \ref ProgrammaticMainWindowProxy::resized method on each proxy for this
             * window.
             *
             * \param[in] event The event that triggered the window to resize.
             */
            void resizeEvent(QResizeEvent* event) override;

        protected slots:
            /**
             * Slot that is triggered when the window becomes visible.  The default implementation will call
             * the \ref EQt::ProgrammaticMainWindowProxy::windowNowVisible method in each registered proxy.
             */
            void nowVisible();

        private slots:
            /**
             * Slot that is triggered when one or more dock sizes need to be updated.
             */
            void updateDocks();

            /**
             * Slot that is triggered when a new tabified dock widget is activated.
             *
             * \param[in] dockWidget The dock widget that was just activated.
             */
            void tabifiedDockWidgetWasActivated(QDockWidget* dockWidget);

            /**
             * Slot that is triggered after a resize event to restack the dock widgets.
             */
            void restackDockWidgets();

        private:
            class EQT_PUBLIC_API Node;

            /**
             * Delay to start the dock update process.
             */
            static constexpr unsigned dockWidgetUpdateDelay1 = 10;

            /**
             * Delay to resize docks after being set to minimum size.
             */
            static constexpr unsigned dockWidgetUpdateDelay2 = 100;

            /**
             * Delay before restacking dock widgets after a resize event.
             */
            static constexpr unsigned dockWidgetRestackingDelay = 300;

            /**
             * Type containing a multi-map of actions indexed by action state.
             */
            typedef QMultiMap<Util::BitSet, QAction*> ActionStateMap;

            /**
             * Type used to represent a set of programmatic docks.
             */
            typedef QSet<ProgrammaticDockWidget*> DockWidgetSet;

            /**
             * Type used to represent a map of dock widgets by name.
             */
            typedef QMap<QString, ProgrammaticDockWidget*> DockWidgetsByName;

            /**
             * Type used to order dock widgets within each dock widget area.
             */
            typedef QMap<unsigned, ProgrammaticDockWidget*> DockWidgetsByPosition;

            /**
             * Type used to order dock widgets by position and dock area.
             */
            typedef QMap<Qt::DockWidgetArea, DockWidgetsByPosition> DockWidgetsByPositionByArea;

            /**
             * Adds or re-adds a dock to the main window.
             *
             * \param[in] location   The location where the dock should be added.
             *
             * \param[in] dockWidget The dock to be added.
             *
             * \return Returns true if the dock was placed.  Returns false if the location of the dock depends on
             *         a sibling dock.
             */
            bool addDockWidget(DockWidgetDefaults::Area location, QDockWidget* dockWidget);

            /**
             * Locates a menu node based on a provided path.  If needed, menu nodes will be constructed in order to
             * create the requested path.
             *
             * \param[in] menuTree        A list of entries representing the path to be followed.  Note that leading
             *                            and trailing spaces on each entry will be trimmed off, if present.
             *
             * \param[in] includeLastItem If true, the last item in the tree will be treated as a menu.  If false, the
             *                            last item will be ignored.
             *
             * \return Returns a pointer to the menu node at the end of the requested path.
             */
            ProgrammaticMainWindow::Node* locateNode(const QStringList& menuTree, bool includeLastItem = false);

            /**
             * Method that adjusts a multimap based on an action state.
             *
             * \param[in]     actionState  The action state that should be used to detemrine the adjustment.
             *
             * \param[in,out] multiMap     The map containing the actions to be traversed.  Actions that intersect the
             *                             action state value will be removed from the map.
             *
             * \param[in]     enableAction If true, the action will be enabled when removed.  If false, the action will
             *                             be disabled when removed.
             *
             * \return Returns a multimap containing the removed actions.
             */
            ActionStateMap processActionState(
                const Util::BitSet& actionState,
                ActionStateMap*     multiMap,
                bool                enableAction
            );

            /**
             * Method that removes an action from an action map.
             *
             * \param[in] action    The action to be removed.
             *
             * \param[in] actionMap The action map containing the action.
             *
             * \return Returns true if the action was found in the map and removed.  Returns false if the action was
             *         not in the map.
             */
            static bool removeFromMap(const QAction* action, ActionStateMap& actionMap);

            /**
             * Method that will combine two docks, taking into account tabbing.
             *
             * \param[in] firstToCombine  The first dock to combine.
             *
             * \param[in] secondToCombine The second dock to combine.
             */
            void combineDocks(ProgrammaticDockWidget* firstToCombine, ProgrammaticDockWidget* secondToCombine);

            /**
             * Timer used when the window becomes visible to perform geometry updates.
             */
            QTimer* showTimer;

            /**
             * Timer used to schedule dock position updates.
             */
            QTimer* dockWidgetUpdateTimer;

            /**
             * Set of dock areas that require update.
             */
            QSet<Qt::DockWidgetArea> modifiedDockAreas;

            /**
             * Map of default locations for each dock widget, by dock name.
             */
            QMap<QString, DockWidgetDefaults> dockWidgetDefaults;

            /**
             * Map used to track the current location of each dock.
             */
            QMap<ProgrammaticDockWidget*, Qt::DockWidgetArea> currentDockLocations;

            /**
             * Timer used to check dock widget stacking after a resize event.
             */
            QTimer* dockWidgetRestackingTimer;

            /**
             * Map of currently defined actions.
             */
            QMap<QString, QAction*> actions;

            /**
             * Root nodes.
             */
            QMap<QString, Node*> rootNodes;

            /**
             * Tool bars.
             */
            QMap<QString, QToolBar*> toolbars;

            /**
             * Dictionary of dock widgets.
             */
            DockWidgetsByName dockWidgets;

            /**
             * The current action state.
             */
            Util::BitSet currentActionState;

            /**
             * Map of action masks by actions.
             */
            QMap<QAction*, Util::BitSet> currentMasksByAction;

            /**
             * Dynamic map of currently enabled actions.
             */
            ActionStateMap currentlyEnabledActions;

            /**
             * Dynamic map of currently disabled actions.
             */
            ActionStateMap currentlyDisabledActions;

            /**
             * Map of actions that have multiple set mask bits.
             */
            ActionStateMap currentMultiBitMaskActions;
    };

    #if (defined(Q_OS_WIN))

        /**
         * Explicit template specializations needed for Windows.
         */
        template class EQT_PUBLIC_API ProgrammaticWindow<ProgrammaticMainWindowProxy, ProgrammaticMainWindow>;

    #endif
}

#endif
