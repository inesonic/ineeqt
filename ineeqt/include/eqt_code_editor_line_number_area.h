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
* This header defines the \ref EQt::CodeEditor class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_CODE_EDITOR_LINE_NUMBER_AREA_H
#define EQT_CODE_EDITOR_LINE_NUMBER_AREA_H

#include <QWidget>
#include <QColor>
#include <QFont>
#include <QSize>
#include <QRect>
#include <QTextBlock>

#include "eqt_common.h"

class QPaintEvent;

namespace EQt {
    class CodeEditor;

    /**
     * Class you can extend to customize how line numbers are displayed and managed by a code editor.  Code that adjusts
     * the colors does so by modifying the widget's palette.  You can alternately directly manipulate the palette if
     * you so choose.
     *
     * This code was heavily inspired by The Qt 4 code editor example.
     */
    class EQT_PUBLIC_API CodeEditorLineNumberArea:public QWidget {
        friend class CodeEditor;

        Q_OBJECT

        public:
            /**
             * Value that represents the default left padding.
             */
            static const unsigned defaultLeftPadding;

            /**
             * Value that represents the default right padding.
             */
            static const unsigned defaultRightPadding;

            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            CodeEditorLineNumberArea(QWidget* parent = Q_NULLPTR);

            ~CodeEditorLineNumberArea() override;

            /**
             * Method that returns the editor associated with this line number area.
             *
             * \return Returns the code editor associated with this widget.
             */
            CodeEditor* editor() const;

            /**
             * Method you can call to determine if line numbers are being displayed.
             *
             * \return Returns true if line numbers are being displayed.  Returns false if line numbers are being
             *         hidden.
             */
            bool lineNumbersVisible() const;

            /**
             * Method you can call to determine if line numbers are being hidden.
             *
             * \return Returns true if line numbers are hidden.  Returns false if line numbers are being displayed.
             */
            bool lineNumbersHidden() const;

            /**
             * Method you can use to determine the line number font.
             *
             * \return Returns the font used to render the line numbers.
             */
            QFont lineNumberFont() const;

            /**
             * Method you can use to determine the line number text color.
             *
             * \return Returns the color assigned to the line number text.
             */
            QColor lineNumberTextColor() const;

            /**
             * Method you can use to determine the line number area background color.
             *
             * \return Returns the color assigned to the line number area background.
             */
            QColor backgroundColor() const;

            /**
             * Method you can use to determine the left padding.
             *
             * \return Returns the left side padding, in pixels.
             */
            unsigned leftPadding() const;

            /**
             * Method you can use to determine the right padding.
             *
             * \return Returns the right side padding, in pixels.
             */
            unsigned rightPadding() const;

            /**
             * Method that returns a size hint for this widget.
             *
             * \return Returns a size hint to be used to size the widget.
             */
            QSize sizeHint() const override;

        public slots:
            /**
             * Slot you can trigger to display or hide line numbers.
             *
             * \param[in] nowVisible If true, the line numbers will be made visible.  If false, the numbers will be
             *                       hidden.
             */
            void setLineNumbersVisible(bool nowVisible = true);

            /**
             * Slot you can trigger to hide or display line numbers.
             *
             * \param[in] nowHidden If true, the line numbers will be hidden.  if false, the line numbers will be made
             *                      visible.
             */
            void setLineNumbersHidden(bool nowHidden = true);

            /**
             * Slot you can trigger to udapte the line number font.
             *
             * \param[in] newFont The new line number font.
             */
            void setLineNumberFont(const QFont& newFont);

            /**
             * Slot you can trigger to update the line number text color.
             *
             * \param[in] newColor The new line number text color.
             */
            void setLineNumberTextColor(const QColor& newColor);

            /**
             * Slot you can trigger to update the background color.
             *
             * \param[in] newColor The new background color.
             */
            void setBackgroundColor(const QColor& newColor);

            /**
             * Slot you can trigger to update the left side padding.
             *
             * \param[in] newLeftPadding The new left side padding, in pixels.
             */
            void setLeftPadding(unsigned newLeftPadding);

            /**
             * Slot you can trigger to update the right side padding.
             *
             * \param[in] newRightPadding The new right side padding, in pixels.
             */
            void setRightPadding(unsigned newRightPadding);

            /**
             * Slot you can trigger when the block count changes.  The default implementation will quickly calculate
             * the number of required digits and trigger a geometry update, if needed.
             *
             * \param[in] newBlockCount The new number of lines in the editor window.
             */
            virtual void blockCountChanged(int newBlockCount);

            /**
             * Slot you can trigger when a region of the text editor has been updated.
             *
             * \param[in] rectangle The rectangle that was just updated.
             *
             * \param[in] dy        The distance that the viewport was scrolled.
             */
            virtual void updateRequest(const QRect& rectangle, int dy);

        protected:
            /**
             * Method you can use to determine the current number of required digits.
             *
             * \return Returns the current number of required digits.
             */
            unsigned numberRequiredDigits() const;

            /**
             * Method you can use to obtain the current geometry of a block in content coordinates.
             *
             * \param[in] block     The block to obtain the geometry information for.
             */
            QRectF blockBoundingGeometry(const QTextBlock& block) const;

            /**
             * Method you can use to obtain the current geometry of a block in viewport coordinates.
             *
             * \param[in] block The block to obtain the geometry information for.
             */
            QRect blockViewportCoordinates(const QTextBlock& block) const;

            /**
             * Method you can use to obtain the current block geometry geometry of a block in block coordiantes.
             *
             * \param[in] block The block to obtain the geometry information for.
             */
            QRectF blockBoundingRectangle(const QTextBlock& block);

            /**
             * Method you can overload to calculate the required width based on a specified number of digits.  This
             * method should also take into account whether or number line numbers should be displayed as well as any
             * required padding.
             *
             * \param[in] requiredNumberDigits The required number of digits.
             *
             * \return Returns the required width, in pixels.
             */
            virtual unsigned calculateWidth(unsigned requiredNumberDigits) const;

            /**
             * Method that is called to paint the line number area.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void paintEvent(QPaintEvent* event) override;

        private:
            /**
             * Method that updates the width and reports a geometry change.  This method will also update the editor's
             * viewport left margin.
             */
            void updateWidth();

            /**
             * The editor this class is operating with.
             */
            CodeEditor* currentEditor;

            /**
             * Flag indicating if line numbers should be displayed.
             */
            bool currentDisplayLineNumbers;

            /**
             * The current line number font.
             */
            QFont currentFont;

            /**
             * The current left side padding.
             */
            unsigned currentLeftPadding;

            /**
             * The current right side padding.
             */
            unsigned currentRightPadding;

            /**
             * The current number of required digits.
             */
            unsigned currentNumberRequiredDigits;

            /**
             * The current required width.
             */
            unsigned currentRequiredWidth;

            /**
             * Value holding the current font line height.
             */
            unsigned currentLineNumberHeight;
    };
}

#endif
