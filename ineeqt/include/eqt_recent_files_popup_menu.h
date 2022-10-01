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
* This header defines the \ref EQt::RecentFilesPopupMenu class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_RECENT_FILES_POPUP_MENU_H
#define EQT_RECENT_FILES_POPUP_MENU_H

class QWidget;
class QAction;

#include "eqt_common.h"
#include "eqt_dynamic_popup_menu.h"

namespace EQt {
    class RecentFilesData;

    /**
     * Class that extends the \ref EQt::DynamicPopupMenu to provide a dynamically generated list of recent files.
     */
    class EQT_PUBLIC_API RecentFilesPopupMenu:public EQt::DynamicPopupMenu {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this menu.
             */
            RecentFilesPopupMenu(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] title  The title to assign to this menu.
             *
             * \param[in] parent The parent object for this window.
             */
            RecentFilesPopupMenu(const QString& title, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRecentFilesData The new recent files database used to obtain information on files.
             *
             * \param[in] parent             The parent object for this menu.
             */
            RecentFilesPopupMenu(RecentFilesData* newRecentFilesData, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] title              The title to assign to this menu.
             *
             * \param[in] newRecentFilesData The new recent files database used to obtain information on files.
             *
             * \param[in] parent             The parent object for this window.
             */
            RecentFilesPopupMenu(
                const QString&   title,
                RecentFilesData* newRecentFilesData,
                QWidget*         parent = Q_NULLPTR
            );

            /**
             * Constructor.
             *
             * \param[in] action The action used to report requests to open recent files.
             *
             * \param[in] parent The parent object for this menu.
             */
            RecentFilesPopupMenu(QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] title  The title to assign to this menu.
             *
             * \param[in] action The action used to report requests to open recent files.
             *
             * \param[in] parent The parent object for this window.
             */
            RecentFilesPopupMenu(const QString& title, QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRecentFilesData The new recent files database used to obtain information on files.
             *
             * \param[in] action             The action used to report requests to open recent files.
             *
             * \param[in] parent             The parent object for this menu.
             */
            RecentFilesPopupMenu(RecentFilesData* newRecentFilesData, QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] title              The title to assign to this menu.
             *
             * \param[in] newRecentFilesData The new recent files database used to obtain information on files.
             *
             * \param[in] action             The action used to report requests to open recent files.
             *
             * \param[in] parent             The parent object for this window.
             */
            RecentFilesPopupMenu(
                const QString&   title,
                RecentFilesData* newRecentFilesData,
                QAction*         action,
                QWidget*         parent = Q_NULLPTR
            );

            ~RecentFilesPopupMenu() override;

            /**
             * Sets the database to use to obtain a list of recent files.
             *
             * \param[in] newRecentFilesData The new recent files database used to obtain information on files.
             */
            void setRecentFilesData(RecentFilesData* newRecentFilesData);

            /**
             * Determines the database currently used to obtain a list of recent files.
             *
             * \return Returns a pointer to the \ref RecentFilesData instance this class is using.
             */
            inline RecentFilesData* recentFilesData() const {
                return currentRecentFilesData;
            }

            /**
             * Sets the action used to report requests to open a recent file.
             *
             * \param[in] action The action used to report requests to open a recent file.
             */
            inline void setRecentFilesAction(QAction* action) {
                currentRecentFilesAction = action;
            }

            /**
             * Determines the action used to report requests to open a recent file.
             *
             * \return Returns the action used to report file open requests.
             */
            inline QAction* recentFilesAction() const {
                return currentRecentFilesAction;
            }

        protected:
            /**
             * You can overload this method to update dynamic content of this menu.  You are responsible for adding the
             * necessary glue to respond to the dynamically selected actions.
             */
            void addDynamicActions() override;

            /**
             * You can overload this method to receive notification when a dynamic action was selected.  The default
             * implementation simply returns.
             *
             * \param[in] action The dynamic action that was selected by the user.
             */
            void dynamicActionSelected(QAction* action) override;

        private:
            /**
             * Method that performs configuration required by all constructors.
             */
            void configureWidget();

            QAction*         currentRecentFilesAction;
            RecentFilesData* currentRecentFilesData;
    };
}

#endif
