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
* This header defines the \ref EQt::DimensionLineEdit class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_DIMENSION_LINE_EDIT_H
#define EQT_DIMENSION_LINE_EDIT_H

#include "eqt_common.h"
#include "eqt_dimension_validator.h"
#include "eqt_line_edit.h"

class QWidget;
class QAction;
class QSettings;

class MainWindow;

namespace EQt {
    /**
     * Class that extends the LineEdit widget to provide support for entering dimensions.
     */
    class EQT_PUBLIC_API DimensionLineEdit:public LineEdit {
        Q_OBJECT

        public:
            /**
             * Enumeration of supported units.
             */
            typedef DimensionValidator::Unit Unit;

            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            DimensionLineEdit(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this combo box.
             *
             * \param[in] parent The parent object for this window.
             */
            DimensionLineEdit(QAction* action, QWidget* parent = Q_NULLPTR);

            ~DimensionLineEdit() override;

            /**
             * Method you can use to load settings from a QSettings instance.
             *
             * \param[in] settings    The name of the settings instance.
             *
             * \param[in] entryName   The name of the entry to obtain the settings for.
             *
             * \param[in] defaultUnit The default units to use if the setting does not exist.
             */
            void loadSettings(QSettings* settings, const QString& entryName, Unit defaultUnit);

            /**
             * Method that is called to save settings to a QSettings instance.
             *
             * \param[in] settings  The name of the settings instance.
             *
             * \param[in] entryName The name of the entry to save the units under.
             */
            void saveSettings(QSettings* settings, const QString& entryName);

            /**
             * Method you can use to determine if the currently stored value is valid.
             *
             * \return Returns true if the stored value is valid.  Returns false if the stored value is invalid.
             */
            bool valueOk();

            /**
             * Method you can use to obtain the value represented by this line editor.
             *
             * \return Returns the currently selected user dimensions, in points.
             */
            double value();

            /**
             * Method you can use to determine the units values will be displayed in.
             *
             * \return Returns the units that are currently being used.
             */
            DimensionLineEdit::Unit units() const;

            /**
             * Method you can use to determine if the displayed units will be changed dynamically based on user input.
             *
             * \return Returns true if the units will adjust dynamically based on the user's input.  Returns false if
             *         the displayed units remain fixed independent of how the user enters data.
             */
            bool dynamicUnits() const;

            /**
             * Method you can use to determine if the displayed units will be changed dynamically based on user input.
             *
             * \return Returns true if the displayed units remain fixed independent of how the user enters data.
             *         Returns false if the units will adjust dynamically based on the user's input.
             */
            bool staticUnits() const;

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
             * Method you can use to set the value shown by this line editor.
             *
             * \param[in] newValue The new value, in points.
             */
            void setValue(double newValue);

            /**
             * Method you can use to specify the units that values will be displayed in.
             *
             * \param[in] newUnits The new units to display information in.
             */
            void setUnits(DimensionLineEdit::Unit newUnits);

            /**
             * Method you can use to enable or disable dynamic unit adjustment based on the user input.  By default,
             * units will not be dynamically adjusted with the initial units defined based on region.
             *
             * \param[in] nowDynamicUnits If true, units will be changed dynamically with user input.  If false, the
             *                            displayed units will be fixed.
             */
            void setDynamicUnits(bool nowDynamicUnits = true);

            /**
             * Method you can use to enable or disable static unit display.  By default, units will not be dynamically
             * adjusted with the initial units defined based on region.
             *
             * \param[in] nowStaticUnits If true, units will be fixed independent of user input. If false, the displayed
             *                           units will be changed dynamically.
             */
            void setStaticUnits(bool nowStaticUnits = true);

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

        signals:
            /**
             * Signal that is emitted when the value is successfully changed.
             *
             * \param[out] newValue the new value entered by the user, in points.
             */
            void valueChanged(double newValue);

        protected:
            /**
             * Method that is called every time the action is updated.  You can overload this method to perform
             * additional processing, if needed.
             *
             * The default implementation performs no action.
             *
             * \param[in] newText The new text.
             */
            void updated(const QString& newText) override;

            /**
             * Method that used used to update the line editor based on the contents of the QAction::data() member.
             * This version converts a variant containing a double value to a distance in inches or mm based on locale.
             *
             * \param[in] variant The QVariant to be converted.
             */
            QString variantToText(const QVariant& variant) override;

            /**
             * Method that used used to convert toe value to a variant of the correct type.  This version parses the
             * text string and converts it a a double type representing the distance, in points.
             *
             * \param[in] text The text to be be converted.
             *
             * \return Returns a QVariant containing the converted value.  If the value could not be converted, an
             *         invalid QVariant is returned.
             */
            QVariant textToVariant(const QString& text) override;

        private slots:
            /**
             * Slot that is triggered when return is pressed on the field.  The method reformats the field contents.
             */
            void editingFinished();

        private:
            /**
             * Performs operations needed by all constructors.
             */
            void configureWidget();

            /**
             * The dimension validator.
             */
            DimensionValidator* dimensionValidator;

            /**
             * Flag indicating if the dimension line edit should dynamically adjust units based on successful user
             * input.
             */
            bool currentDynamicUnits;
    };
}

#endif
