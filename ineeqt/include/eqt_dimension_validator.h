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
* This header defines the \ref EQt::DimensionValidator class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_DIMENSION_VALIDATOR_H
#define EQT_DIMENSION_VALIDATOR_H

#include <QRegularExpression>
#include <QValidator>

#include "eqt_common.h"

class QWidget;
class QAction;
class QMenu;
class QRegularExpression;

class MainWindow;

namespace EQt {
    /**
     * Class that validates user input of dimensions.
     */
    class EQT_PUBLIC_API DimensionValidator:public QValidator {
        Q_OBJECT

        public:
            /**
             * Enumeration of supported units.
             */
            enum Unit {
                /**
                 * Indicates an invalid unit.
                 */
                INVALID = 0,

                /**
                 * Indicates values should be presented in units that are natural for the region, either inches or mm.
                 */
                PER_REGION = 1,

                /**
                 * Indicates values should be presented in mm.
                 */
                MILLIMETER = 2,

                /**
                 * Indicates values should be presented in cm.
                 */
                CENTIMETER = 3,

                /**
                 * Indicates values should be presented in meters.
                 */
                METER = 4,

                /**
                 * Indicates values should be presented in points.
                 */
                POINT = 5,

                /**
                 * Indicates values should be presented in picas.
                 */
                PICA = 6,

                /**
                 * Indicates values should be presented in inches.
                 */
                INCH = 7,

                /**
                 * Indicates values should be presented in feet.
                 */
                FOOT = 8,
            };

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            DimensionValidator(QObject* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] maximumValue The maximum allowed value for this validator.
             *
             * \param[in] parent       Pointer to the parent object.
             */
            DimensionValidator(double maximumValue, QObject* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] minimumValue The minimum allowed value for this validator.
             *
             * \param[in] maximumValue The maximum allowed value for this validator.
             *
             * \param[in] parent       Pointer to the parent object.
             */
            DimensionValidator(double minimumValue, double maximumValue, QObject* parent = Q_NULLPTR);

            ~DimensionValidator() override;

            /**
             * Method that validates an input string.
             *
             * \param[in,out] input    The input string to be updated.
             *
             * \param[in,out] position The current cursor position.
             */
            State validate(QString& input, int& position) const override;

            /**
             * Convenience method you can use to check if a string is valid.
             *
             * \param[in] text The text to be checked.
             *
             * \return Returns true if the text represents a valid value.  Returns false if the text is invalid.
             */
            bool valueOk(const QString& text);

            /**
             * Method you can use to obtain the value represented by in a string value.
             *
             * \param[in]     text The value to be converted.
             *
             * \param[in,out] ok   A pointer to a value you can provide.  The method will populate the pointer with a
             *                     true value if the text could be converted.  The method will populate the pointer
             *                     with a false value if the text could not be converted.
             *
             * \return Returns the currently selected user dimensions, in points.
             */
            double value(const QString& text, bool* ok = Q_NULLPTR) const;

            /**
             * Method you can use to obtain the units represented in a string value.
             *
             * \param[in]     text The value to be converted.
             *
             * \param[in,out] ok   A pointer to a value you can provide.  The method will populate the pointer with a
             *                     true value if the text could be converted.  The method will populate the pointer
             *                     with a false value if the text could not be converted.
             *
             * \return Returns the units indicated by the string.
             */
            Unit units(const QString& text, bool* ok = Q_NULLPTR) const;

            /**
             * Method you can use to convert a value to a textual representation.
             *
             * \param[in] value The value to be converted.
             *
             * \return Returns a text representation of the value with appropriate units for the current locale.
             */
            QString text(double value) const;

            /**
             * Method you can use to determine the units values will be displayed in.
             *
             * \return Returns the units that are currently being used.
             */
            Unit units() const;

            /**
             * Method you can use to obtain the current minimum allowed value, in points.
             *
             * \return Returns the current minimum allowed value, in points.
             */
            double minimumValue() const;

            /**
             * Method you can use to obtain the current maximum allowed value, in points.
             *
             * \return Returns the current maximum allowed value, in points.
             */
            double maximumValue() const;

        public slots:
            /**
             * Method you can use to specify the units that values will be displayed in.  By default, units are
             * displayed differently by region.
             *
             * \param[in] newUnits The new units to display information in.
             */
            void setUnits(Unit newUnits);

            /**
             * Slot you can use to set the minimum allowed value, in points.  If the current value is less than this
             * value, the value will be forced to this value.
             *
             * \param[in] newMinimumValue The new minimum value, in points.
             */
            void setMinimumValue(double newMinimumValue);

            /**
             * Slot you can use to set the maximum allowed value, in points.  If the current value is less than this
             * value, the value will be forced to this value.
             *
             * \param[in] newMaximumValue The new maximum value, in points.
             */
            void setMaximumValue(double newMaximumValue);

        private:
            /**
             * Performs operations needed by all constructors.
             */
            void configure();

            /**
             * The pre-compiled expression used by the validator.
             */
            QRegularExpression validatorRegularExpression;

            /**
             * Currently displayed units.
             */
            Unit currentUnits;

            /**
             * The current minimum value.
             */
            double currentMinimumValue;

            /**
             * The current maximum value.
             */
            double currentMaximumValue;
    };
}

#endif
