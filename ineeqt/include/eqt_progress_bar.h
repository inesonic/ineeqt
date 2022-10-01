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
* This header defines the \ref EQt::ProgressBar class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRESS_BAR_H
#define EQT_PROGRESS_BAR_H

#include <QWidget>
#include <QString>
#include <QColor>
#include <QFont>
#include <QFontMetrics>

#include "eqt_common.h"

class QPaintEvent;

namespace EQt {
    /**
     * Class that provides a narrow progress bar with support for text and toolbar color updates.
     */
    class EQT_PUBLIC_API ProgressBar:public QWidget {
        Q_OBJECT

        public:
            /**
             * The default minimum value.
             */
            static const int defaultMinimumValue;

            /**
             * The default maximum value.
             */
            static const int defaultMaximumValue;

            /**
             * The default value.
             */
            static const int defaultValue;

            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ProgressBar(QWidget* parent = Q_NULLPTR);

            ~ProgressBar() override;

            /**
             * Method you can use to determine the current minimum value.
             *
             * \return Returns the current minimum value.
             */
            int minimumValue() const;

            /**
             * Method you can use to determine the current maximum value.
             *
             * \return Returns the current maximum value.
             */
            int maximumValue() const;

            /**
             * Method you can use to determine the current value.
             *
             * \return Returns the current value.
             */
            int value() const;

            /**
             * Method you can use to determine the current displayed text.
             *
             * \return Returns the current text.
             */
            QString text() const;

            /**
             * Method you can use to determine the current progress bar color.
             *
             * \return Returns the current progress bar color.
             */
            QColor progressBarColor() const;

            /**
             * Method you can use to determine the current background color.
             *
             * \return Returns the current background color.
             */
            QColor backgroundColor() const;

            /**
             * Method you can use to determine the current text font.
             *
             * \return Returns the current text font.
             */
            QFont textFont() const;

            /**
             * Method you can use to determine the current text color.
             *
             * \return Returns the current text color.
             */
            QColor textColor() const;

        public slots:
            /**
             * Slot you can trigger to set the minimum value.
             *
             * \param[in] newMinimumValue The new minimum value.
             */
            void setMinimumValue(int newMinimumValue);

            /**
             * Slot you can trigger to set the maximum value.
             *
             * \param[in] newMaximumValue The new maximum value.
             */
            void setMaximumValue(int newMaximumValue);

            /**
             * Slot you can trigger to change the current value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(int newValue);

            /**
             * Slot you can trigger to update the displayed text.
             *
             * \param[in] newText The new text to be displayed.
             */
            void setText(const QString& newText);

            /**
             * Slot you can trigger to update the progress bar color.
             *
             * \param[in] newColor The new progress bar color.
             */
            void setProgressBarColor(const QColor& newColor);

            /**
             * Slot you can trigger to update the background color.
             *
             * \param[in] newColor The new background color.
             */
            void setBackgroundColor(const QColor& newColor);

            /**
             * Slot you can trigger to update the text font.   The application font will be used by default.
             *
             * \param[in] newFont The new text font.
             */
            void setTextFont(const QFont& newFont);

            /**
             * Slot you can trigger to update the text color.
             *
             * \param[in] newColor The new text color.
             */
            void setTextColor(const QColor& newColor);

        protected:
            /**
             * Method that is called to repaint the window.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void paintEvent(QPaintEvent* event) override;

        private:
            /**
             * Text padding to apply above and below the text.
             */
            static const unsigned verticalPadding = 4;

            /**
             * Method that updates the widget height when a new text font is selected.
             */
            void updateHeight();

            /**
             * The current minimum value.
             */
            int currentMinimumValue;

            /**
             * The current maximum value.
             */
            int currentMaximumValue;

            /**
             * The current value.
             */
            int currentValue;

            /**
             * The current text.
             */
            QString currentText;

            /**
             * The current progress bar color.
             */
            QColor currentProgressBarColor;

            /**
             * The current background color.
             */
            QColor currentBackgroundColor;

            /**
             * The current text font.
             */
            QFont currentTextFont;

            /**
             * The current text color.
             */
            QColor currentTextColor;
    };
}

#endif
