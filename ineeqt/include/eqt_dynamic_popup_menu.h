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
* This header defines the \ref EQt::DynamicPopupMenu class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_DYNAMIC_POPUP_MENU_H
#define EQT_DYNAMIC_POPUP_MENU_H

#include <QMenu>
#include <QPointer>
#include <QList>
#include <QMap>

class QWidget;
class QAction;
class QActionGroup;

#include "eqt_common.h"

namespace EQt {
    /**
     * Pure virtual class that extends QMenu to allow the menu to be dynamically rebuilt in response to an event.  You
     * can use this class to create menus whose contents change during normal use of the application.
     */
    class EQT_PUBLIC_API DynamicPopupMenu:public QMenu {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this menu.
             */
            DynamicPopupMenu(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] title  The title to assign to this menu.
             *
             * \param[in] parent The parent object for this window.
             */
            DynamicPopupMenu(const QString& title, QWidget* parent = Q_NULLPTR);

            ~DynamicPopupMenu() override;

            /**
             * Prepends a static menu item to this menu.  Static items will be included on top of any dynamic content.
             * Note that checkable actions are not supported.
             *
             * \param[in] action The action to be added.  Details for the menu item will be automatically extracted
             *                   from the action.  Note that this method does *not* take ownership of the action.
             */
            void prependStaticAction(QAction* action);

            /**
             * Appends a static menu item to this menu.  Static items will be included on top of any dynamic content.
             * Note that checkable actions are not supported.
             *
             * \param[in] action The action to be added.  Details for the menu item will be automatically extracted
             *                   from the action.  Note that this method does *not* take ownership of the action.
             */
            void appendStaticAction(QAction* action);

        public slots:
            /**
             * Slot that should be triggered when dynamic content needs to be updated.
             */
            void updateDynamicContent();

        protected:
            /**
             * You can overload this method to update dynamic content of this menu.  You are responsible for adding the
             * necessary glue to respond to the dynamically selected actions.
             */
            virtual void addDynamicActions() = 0;

            /**
             * You can overload this method to receive notification when a dynamic action was selected.  The default
             * implementation simply returns.
             *
             * \param[in] action The dynamic action that was selected by the user.
             */
            virtual void dynamicActionSelected(QAction* action);

        private slots:
            /**
             * Slot that is triggered when the user selects a menu item.
             *
             * \param[in] action The action that represents the user's selection.
             */
            void triggered(QAction* action);

        private:
            /**
             * Method that performs configuration required by all constructors.
             */
            void configureWidget();

            /**
             * Adds actions to the menu from a list of existing actions.
             *
             * \param[in] actionList The list of actions to use to determine what actions to add to the menu.
             */
            void addStaticActions(const QList<QPointer<QAction>>& actionList);

            QList<QPointer<QAction>> prependedActions;
            QList<QPointer<QAction>> appendedActions;
            QMap<QAction*, QAction*> staticActions;
    };
}


#endif
