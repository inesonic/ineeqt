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
* This header defines the \ref EQt::SpinBox class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_SPIN_BOX_H
#define EQT_SPIN_BOX_H

#include <QSpinBox>

class QWidget;
class QAction;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that extends QSpinBox to support use with actions.  You can use this class to integrate QSpinBox instances
     * with the Qt action framework.
     */
    class EQT_PUBLIC_API SpinBox:public QSpinBox {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            SpinBox(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this spin box.
             *
             * \param[in] parent The parent object for this window.
             */
            SpinBox(QAction* action, QWidget* parent = Q_NULLPTR);

            ~SpinBox() override;

            /**
             * Determines the default action that is receiving updates for this spin box.
             *
             * \return Returns a pointer to the default action.
             */
            QAction* defaultAction();

        public slots:
            /**
             * Sets the default action for this slot.
             *
             * \param action The action to associate with this spin box.
             */
            void setDefaultAction(QAction* action);

        private slots:
            /**
             * Slot that receives notification when the value in the spin box changes.  This slot exists to
             * automatically update the action on user input.
             *
             * \param[in] i The new value for the spin box.
             */
            void valueHasChanged(int i);

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
