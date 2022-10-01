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
* This header defines the \ref EQt::SearchLineEdit class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_SEARCH_LINE_EDIT_H
#define EQT_SEARCH_LINE_EDIT_H

#include <eqt_line_edit.h>

class QWidget;
class QAction;
class QMenu;

#include "eqt_common.h"
#include "eqt_line_edit.h"

namespace EQt {
    /**
     * Class that extends the LineEdit widget to provide options for text search.
     *
     * This method expects to be able to fetch icons using the \ref EQt::Application::icon method using the names:
     *
     *     * edit_undo
     *     * edit_redo
     *     * edit_cut
     *     * edit_copy
     *     * edit_paste
     *
     * Icons are used in the search line edit's context menu.
     */
    class EQT_PUBLIC_API SearchLineEdit:public LineEdit {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent     The parent object for this window.
             */
            SearchLineEdit(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this combo box.
             *
             * \param[in] parent The parent object for this window.
             */
            SearchLineEdit(QAction* action, QWidget* parent = Q_NULLPTR);

            ~SearchLineEdit() override;

            /**
             * Sets the action used to track case sensitive search.
             *
             * \param[in] action The global case sensitive search action.
             */
            void setCaseSensitiveSearchAction(QAction* action);

            /**
             * Determines the action being used to track the global search option state.
             *
             * \return Returns a pointer to a QAction used to track case sensitive search state across instances of
             *         this widget.
             */
            QAction* caseSensitiveSearchAction() const;

            /**
             * Sets the action used to track whole word only search.
             *
             * \param[in] action The global whole word only search action.
             */
            void setWholeWordsOnlyAction(QAction* action);

            /**
             * Determines the action being used to track the global whole words search option state.
             *
             * \return Returns a pointer to a QAction used to track whole words only search across instances of this
             *         widget.
             */
            QAction* wholeWordsOnlyAction() const;

            /**
             * Sets the action used to track regular expression search search.
             *
             * \param[in] action The global whole word only search action.
             */
            void setRegularExpressionAction(QAction* action);

            /**
             * Determines the action being used to track the global regular expression search option state.
             *
             * \return Returns a pointer to a QAction used to track whole words only search across instances of this
             *         widget.
             */
            QAction* regularExpressionAction() const;

        protected:
            /**
             * Method that will be called by the QLineEdit base class to display the line edit context menu.
             *
             * \param[in] event The event that triggered this call.
             */
            void contextMenuEvent(QContextMenuEvent* event) override;

        private:
            /**
             * Performs operations needed by all constructors.
             */
            void configureWidget();

            /**
             * Action used for case sensitive search.
             */
            QAction* currentCaseSensitiveSearchAction;

            /**
             * Action used for whole words only search.
             */
            QAction* currentWholeWordsOnlyAction;

            /**
             * Action used for regular expression search.
             */
            QAction* currentRegularExpressionSearchAction;
    };
}

#endif
