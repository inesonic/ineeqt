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

#ifndef EQT_SPECIAL_VALUE_SPIN_BOX_H
#define EQT_SPECIAL_VALUE_SPIN_BOX_H

#include <QSpinBox>
#include <QMap>

class QWidget;
class QAction;

#include "eqt_common.h"
#include "eqt_spin_box.h"

namespace EQt {
    /**
     * Class that extends \ref EQt::SpinBox to support multiple special values with custom text.  You can treat this
     * class as a mapping of values to special value strings.
     */
    class EQT_PUBLIC_API SpecialValueSpinBox:public SpinBox, public QMap<int, QString> {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            SpecialValueSpinBox(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this spin box.
             *
             * \param[in] parent The parent object for this window.
             */
            SpecialValueSpinBox(QAction* action, QWidget* parent = Q_NULLPTR);

            ~SpecialValueSpinBox() override;

            /**
             * Method you can use to determine the current spinbox value.
             *
             * \return Returns the current spinbox value.
             */
            int currentValue() const;

            /**
             * Method you can use to determine the special value string tied to a spinbox value.
             *
             * \param[in] spinBoxValue The spinbox value.
             *
             * \return Returns the text tied to the spinbox value.  An empty string is returned if the value has no
             *         special text tied to it.
             */
            QString specialValueText(int spinBoxValue) const;

            /**
             * Method you can use to determine the special value string tied to the minimum spinbox value.
             *
             * \return Returns the text tied to the minimum spinbox value.  An empty string is returned if the value
             *         has no special text tied to it.
             */
            QString specialValueText() const;

            /**
             * Method you can use to set the special value text tied to a value.
             *
             * \param[in] newText      The new text to tie to the special value.
             *
             * \param[in] spinBoxValue The value the text should be tied to.
             */
            void setSpecialValueText(const QString& newText, int spinBoxValue);

            /**
             * Method you can use to set the special value text tied to the minimum value.
             *
             * \param[in] newText The new text to tie to the minimum value.
             */
            void setSpecialValueText(const QString& newText);

        protected:
            /**
             * Method that is called to convert a value to a textual representation.
             *
             * \param[in] numericValue The value to be converted.
             *
             * \return Returns a textual representation of the value.
             */
            QString textFromValue(int numericValue) const override;

            /**
             * Method that is called to convert a textual representation to a value.
             *
             * \param[in] text The text to be converted.
             *
             * \return Returns the value associated with the textual representation.
             */
            int valueFromText(const QString& text) const override;
    };
}

#endif
