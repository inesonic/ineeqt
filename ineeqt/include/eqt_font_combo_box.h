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
* This header defines the \ref EQt::FontComboBox class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_FONT_COMBO_BOX_H
#define EQT_FONT_COMBO_BOX_H

#include <QFontComboBox>

class QWidget;
class QAction;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that extends QFontComboBox to support use with actions.  You can use this class to integrate QFontComboBox
     * instances with the Qt action framework.
     */
    class EQT_PUBLIC_API FontComboBox:public QFontComboBox {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            FontComboBox(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this combo box.
             *
             * \param[in] parent The parent object for this window.
             */
            FontComboBox(QAction* action, QWidget* parent = Q_NULLPTR);

            ~FontComboBox() override;

            /**
             * Determines the default action that is receiving updates for this combo box.
             *
             * \return Returns a pointer to the default action.
             */
            QAction* defaultAction();

        public slots:
            /**
             * Sets the default action for this slot.
             *
             * \param action The action to associate with this combo box.
             */
            void setDefaultAction(QAction* action);

        private slots:
            /**
             * Slot that receives notification when a new font is selected through the dialog.  This slot exists to
             * forward font change notifications to the associated action.
             *
             * \param[in] font The newly selected font.
             */
            void currentFontChanged(const QFont& font);

            /**
             * Slot that receives triggered updates for the action.
             */
            void triggered();

            /**
             * Slot that receives changed updates for the action.
             */
            void changed();

        private:
            /**
             * Performs operations needed by all constructors.
             */
            void configureWidget();
    };
}

#endif
