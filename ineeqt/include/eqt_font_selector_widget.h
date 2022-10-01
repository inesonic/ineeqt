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
* This header defines the \ref EQt::FontSelectorWidget class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_FONT_SELECTOR_WIDGET_H
#define EQT_FONT_SELECTOR_WIDGET_H

#include <QWidget>

class QAction;

#include "eqt_common.h"

namespace EQt {
    class FontComboBox;
    class SpinBox;

    /**
     * Class that combines a \ref FontComboBox and a \ref SpinBox into a single widget.
     */
    class EQT_PUBLIC_API FontSelectorWidget:public QWidget {
        Q_OBJECT

        public:
            /**
             * The default minimum font size, in points.
             */
            static const unsigned defaultMinimumFontSize;

            /**
             * The default maximum font size, in points.
             */
            static const unsigned defaultMaximumFontSize;

            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            FontSelectorWidget(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newFontFamilyAction The action used to report the font family.
             *
             * \param[in] newFontSizeAction   The action used to report the font size.
             *
             * \param[in] parent              The parent object for this window.
             */
            FontSelectorWidget(QAction* newFontFamilyAction, QAction* newFontSizeAction, QWidget* parent = Q_NULLPTR);

            ~FontSelectorWidget() override;

            /**
             * Determines the action used to track the font family.
             *
             * \return Returns a pointer to the action used to track the font family.
             */
            QAction* fontFamilyAction() const;

            /**
             * Determines the action used to track the font size.
             *
             * \return Returns a pointer to the action used to track the font size.
             */
            QAction* fontSizeAction() const;

            /**
             * Method that determines the minimum supported font size.
             *
             * \return Returns the current minimum supported font size.
             */
            unsigned minimumFontSize() const;

            /**
             * Method that determines the maximum supported font size.
             *
             * \return Returns the current maximum supported font size.
             */
            unsigned maximumFontSize() const;

            /**
             * Method that determines the current font size suffix.
             *
             * \return Returns the suffix displayed in the font size spinbox.
             */
            QString fontSizeSuffix() const;

        public slots:
            /**
             * Sets the action used to track the font family.
             *
             * \param[in] newAction The action used to track the font family.
             */
            void setFontFamilyAction(QAction* newAction);

            /**
             * Sets the action used to track the font size.
             *
             * \param[in] newAction The action used to track the font size.
             */
            void setFontSizeAction(QAction* newAction);

            /**
             * Slot you can use to set the minimum font size.
             *
             * \param[in] newMinimumFontSize The new minimum font size.
             */
            void setMinimumFontSize(unsigned newMinimumFontSize);

            /**
             * Slot you can use to set the maximum font size.
             *
             * \param[in] newMaximumFontSize The new maximum font size.
             */
            void setMaximumFontSize(unsigned newMaximumFontSize);

            /**
             * Slot you can use to set the allowed range of font sizes.
             *
             * \param[in] newMinimumFontSize The new minimum font size.
             *
             * \param[in] newMaximumFontSize The new maximum font size.
             */
            void setFontSizeRange(unsigned newMinimumFontSize, unsigned newMaximumFontSize);

            /**
             * Slot you can use to set the font size suffix.
             *
             * \param[in] newSuffix The new suffix to display in the font size spinbox.
             */
            void setFontSizeSuffix(const QString& newSuffix);

        private:
            /**
             * The visual space between the combo box and spin box.
             */
            static constexpr unsigned gapSizeInPixels = 1;

            /**
             * Performs operations needed by all constructors.
             */
            void configureWidget();

            FontComboBox* fontFamilyComboBox;
            SpinBox*      fontSizeSpinBox;
    };
}

#endif
