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
* This header defines the \ref EQt::ComboBox class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_COMBO_BOX_H
#define EQT_COMBO_BOX_H

#include <QComboBox>

class QWidget;
class QEvent;
class QAction;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that extends QComboBox to support use with actions.  You can use this class to integrate QComboBox
     * instances with the Qt action framework.
     *
     * The class will default to using the combo box item index value as the variant type to report to the action.
     */
    class EQT_PUBLIC_API ComboBox:public QComboBox {
        Q_OBJECT

        public:
            /**
             * Enumeration of supported reported values.
             */
            enum class ReportedValue {
                /**
                 * Indicates the default action should report the index of the selected item.
                 */
                INDEX,

                /**
                 * Indicates the default action should reported the text of the selected item.
                 */
                TEXT,

                /**
                 * Indicates the defaut action should report the variant tied to the selected action.
                 */
                VARIANT
            };

            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            ComboBox(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this combo box.
             *
             * \param[in] parent The parent object for this window.
             */
            ComboBox(QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] reportedValueType The new reported value type.
             *
             * \param[in] parent            The parent object for this window.
             */
            ComboBox(ReportedValue reportedValueType, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action            The action to link to this combo box.
             *
             * \param[in] reportedValueType The new reported value type.
             *
             * \param[in] parent            The parent object for this window.
             */
            ComboBox(QAction* action, ReportedValue reportedValueType, QWidget* parent = Q_NULLPTR);

            ~ComboBox() override;

            /**
             * Method you can use to determine the reported value type.
             *
             * \return Returns the reported value type.
             */
            ReportedValue reportedValueType() const;

            /**
             * Determines the default action that is receiving updates for this combo box.
             *
             * \return Returns a pointer to the default action.
             */
            QAction* defaultAction();

        public slots:
            /**
             * Method you can use to indicate how the user selection should be both set or reported.
             *
             * \param[in] newReportedValueType The new reported value type.
             */
            void setReportedValueType(ReportedValue newReportedValueType);

            /**
             * Sets the default action for this slot.
             *
             * \param action The action to associate with this combo box.
             */
            void setDefaultAction(QAction* action);

        protected:
            /**
             * The event handler for this method.
             *
             * \param[in] event The event that needs to be handled.  Events that support what's this, status tips, and
             *                  tool tips will be intercepted, allowing actions tied to the event to drive the text for
             *                  these events.
             *
             * \return Returns true if the event was recognized, returns false if the event was not recognized.
             */
            bool event(QEvent* event) override;

        private slots:
            /**
             * Slot that receives notification when the current combo box item changes.  This slot exists to
             * automatically update the action on user input.
             *
             * \param[in] index The new combo-box index.
             */
            void indexChanged(int index);

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

            /**
             * Indicates how values should be either reported to the action or updated from the action.
             */
            ReportedValue currentReportedValueType;
    };
}


#endif
