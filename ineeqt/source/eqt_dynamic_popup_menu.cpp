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
* This file implements the \ref EQt::DynamicPopupMenu class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QList>
#include <QPointer>
#include <QActionGroup>
#include <QMenu>

#include "eqt_common.h"
#include "eqt_dynamic_popup_menu.h"

namespace EQt {
    DynamicPopupMenu::DynamicPopupMenu(QWidget* parent):QMenu(parent) {
        configureWidget();
    }


    DynamicPopupMenu::DynamicPopupMenu(const QString& title, QWidget* parent):QMenu(title, parent) {
        configureWidget();
    }


    DynamicPopupMenu::~DynamicPopupMenu() {}


    void DynamicPopupMenu::prependStaticAction(QAction* action) {
        prependedActions.append(action);
        updateDynamicContent();
    }


    void DynamicPopupMenu::appendStaticAction(QAction* action) {
        appendedActions.append(action);
        updateDynamicContent();
    }


    void DynamicPopupMenu::updateDynamicContent() {
        clear();
        staticActions.clear();

        if (!prependedActions.isEmpty())  {
            addStaticActions(prependedActions);
            addSeparator();
        }

        addDynamicActions();

        if (!appendedActions.isEmpty())  {
            addSeparator();
            addStaticActions(appendedActions);
        }
    }


    void DynamicPopupMenu::dynamicActionSelected(QAction*) {}


    void DynamicPopupMenu::triggered(QAction* action) {
        if (staticActions.contains(action)) {
            QAction* requestingAction = staticActions.value(action);
            requestingAction->trigger();
        } else {
            dynamicActionSelected(action);
        }
    }

    void DynamicPopupMenu::configureWidget() {
        connect(this, SIGNAL(triggered(QAction*)), this, SLOT(triggered(QAction*)));
    }


    void DynamicPopupMenu::addStaticActions(const QList<QPointer<QAction>>& actionList) {
        for (auto it=actionList.begin(),end=actionList.end() ; it!=end ; ++it) {
            QAction* requestingAction = *it;
            QString  text        = requestingAction->text();
            QIcon    icon        = requestingAction->icon();
            QString  iconText    = requestingAction->iconText();
            bool     iconVisible = requestingAction->isIconVisibleInMenu();
            QString  toolTip     = requestingAction->toolTip();
            QString  statusTip   = requestingAction->statusTip();
            QString  whatsThis   = requestingAction->whatsThis();

            Q_ASSERT(!requestingAction->isCheckable());

            QAction* newAction = addAction(text);

            if (!icon.isNull()) {
                newAction->setIcon(icon);
                newAction->setIconVisibleInMenu(iconVisible);
                if (!iconText.isEmpty()) {
                    newAction->setIconText(iconText);
                }
            }

            newAction->setToolTip(toolTip);
            newAction->setStatusTip(statusTip);
            newAction->setWhatsThis(whatsThis);

            staticActions.insert(newAction, requestingAction);
        }
    }
}
