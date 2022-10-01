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
* This header defines the \ref EQt::PaperSizePopupMenu class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PAPER_SIZE_POPUP_MENU_H
#define EQT_PAPER_SIZE_POPUP_MENU_H

#include <QMenu>
#include <QMap>

class QWidget;
class QAction;
class QActionGroup;
class QShowEvent;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that extends the QMenu to provide a dynamically generated list of paper sizes.
     */
    class EQT_PUBLIC_API PaperSizePopupMenu:public QMenu {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this menu.
             */
            PaperSizePopupMenu(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] title  The title to assign to this menu.
             *
             * \param[in] parent The parent object for this window.
             */
            PaperSizePopupMenu(const QString& title, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action used to report paper size changes.
             *
             * \param[in] parent The parent object for this menu.
             */
            PaperSizePopupMenu(QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] title  The title to assign to this menu.
             *
             * \param[in] action The action used to report paper size changes.
             *
             * \param[in] parent The parent object for this window.
             */
            PaperSizePopupMenu(const QString& title, QAction* action, QWidget* parent = Q_NULLPTR);

            ~PaperSizePopupMenu() override;

            /**
             * Sets the action used to report paper size change requests.
             *
             * \param[in] action The paper size action.
             */
            void setPaperSizeAction(QAction* action);

            /**
             * Determines the action used to report paper size change requests.
             *
             * \return Returns the action used to report paper size change requests.
             */
            QAction* paperSizeAction() const;

        protected:
            /**
             * Method that is triggered when the menu is presented to the user.  This class overloads this method to
             * select the correct action from the menu based on the current value stored in the associated action.
             *
             * \param[in] event The event that is triggering this menu to be shown.
             */
            void showEvent(QShowEvent* event) final;

        private slots:
            /**
             * Slot that is triggered when a new page size is selected.
             *
             * \param[in] triggeredAction The action that was selected by the user.
             */
            void actionTriggered(QAction* triggeredAction);

        private:
            /**
             * Method that performs configuration required by all constructors.
             */
            void configureWidget();

            /**
             * Action used to report updates to the underlying application.
             */
            QAction* currentPaperSizeAction;

            /**
             * Action used to manage custom page sizes.
             */
            QAction* customPageSizeAction;

            /**
             * Action group used to enforce mutual exclusion.
             */
            QActionGroup* actionGroup;

            /**
             * Map used to associate descriptions with actions.
             */
            QMap<QString, QAction*> standardPageSizeActions;
    };
}

#endif
