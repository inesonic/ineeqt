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
* This file implements the \ref EQt::CodeEditorLineNumberArea class.
***********************************************************************************************************************/

#include <QWidget>
#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QSize>
#include <QRect>
#include <QPaintEvent>
#include <QPalette>
#include <QPainter>
#include <QTextBlock>

#include <cmath>

#include "eqt_application.h"
#include "eqt_code_editor.h"
#include "eqt_code_editor_line_number_area.h"

namespace EQt {
    const unsigned CodeEditorLineNumberArea::defaultLeftPadding  = 2;
    const unsigned CodeEditorLineNumberArea::defaultRightPadding = 2;

    CodeEditorLineNumberArea::CodeEditorLineNumberArea(QWidget* parent):QWidget(parent) {
        currentEditor               = Q_NULLPTR;
        currentDisplayLineNumbers   = true;
        currentFont                 = Application::font();
        currentLeftPadding          = defaultLeftPadding;
        currentRightPadding         = defaultRightPadding;
        currentNumberRequiredDigits = 0;
        currentRequiredWidth        = 0;
        currentLineNumberHeight     = QFontMetrics(currentFont).height();

        setForegroundRole(QPalette::Text);
    }


    CodeEditorLineNumberArea::~CodeEditorLineNumberArea() {}


    CodeEditor* CodeEditorLineNumberArea::editor() const {
        return currentEditor;
    }


    bool CodeEditorLineNumberArea::lineNumbersVisible() const {
        return currentDisplayLineNumbers;
    }


    bool CodeEditorLineNumberArea::lineNumbersHidden() const {
        return !currentDisplayLineNumbers;
    }


    QFont CodeEditorLineNumberArea::lineNumberFont() const {
        return currentFont;
    }


    QColor CodeEditorLineNumberArea::lineNumberTextColor() const {
        const QPalette& palette = CodeEditorLineNumberArea::palette();
        return palette.color(foregroundRole());
    }


    QColor CodeEditorLineNumberArea::backgroundColor() const {
        const QPalette& palette = CodeEditorLineNumberArea::palette();
        return palette.color(backgroundRole());
    }


    unsigned CodeEditorLineNumberArea::leftPadding() const {
        return currentLeftPadding;
    }


    unsigned CodeEditorLineNumberArea::rightPadding() const {
        return currentRightPadding;
    }


    QSize CodeEditorLineNumberArea::sizeHint() const {
        return QSize(currentRequiredWidth, 0);
    }


    void CodeEditorLineNumberArea::setLineNumbersVisible(bool nowVisible) {
        if (nowVisible != currentDisplayLineNumbers) {
            currentDisplayLineNumbers = nowVisible;
            updateWidth();
        }
    }


    void CodeEditorLineNumberArea::setLineNumbersHidden(bool nowHidden) {
        setLineNumbersVisible(!nowHidden);
    }


    void CodeEditorLineNumberArea::setLineNumberFont(const QFont& newFont) {
        if (currentFont != newFont) {
            currentFont             = newFont;
            currentLineNumberHeight = QFontMetrics(currentFont).height();
            updateWidth();
        }
    }


    void CodeEditorLineNumberArea::setLineNumberTextColor(const QColor& newColor) {
        QPalette palette = CodeEditorLineNumberArea::palette();
        palette.setColor(foregroundRole(), newColor);
        setPalette(palette);
        update();
    }


    void CodeEditorLineNumberArea::setBackgroundColor(const QColor& newColor) {
        QPalette palette = CodeEditorLineNumberArea::palette();
        palette.setColor(backgroundRole(), newColor);
        setPalette(palette);
        update();
    }


    void CodeEditorLineNumberArea::setLeftPadding(unsigned newLeftPadding) {
        currentLeftPadding = newLeftPadding;
        updateWidth();
    }


    void CodeEditorLineNumberArea::setRightPadding(unsigned newRightPadding) {
        currentRightPadding = newRightPadding;
        updateWidth();
    }


    void CodeEditorLineNumberArea::blockCountChanged(int newBlockCount) {
        unsigned newNumberRequiredDigits;
        if (newBlockCount <= 9) {
            newNumberRequiredDigits = 1;
        } else {
            newNumberRequiredDigits = std::ceil(std::log10(newBlockCount));
        }

        if (newNumberRequiredDigits != currentNumberRequiredDigits) {
            currentNumberRequiredDigits = newNumberRequiredDigits;
            updateWidth();
        }
    }


    void CodeEditorLineNumberArea::updateRequest(const QRect& rectangle, int dy) {
        if (dy != 0) {
            scroll(0, dy);
        } else {
            update(0, rectangle.y(), currentRequiredWidth, rectangle.height());
        }

        if (currentEditor != Q_NULLPTR && rectangle.contains(currentEditor->viewport()->rect())) {
            currentEditor->setViewportMargins(currentRequiredWidth, 0, 0, 0);
        }
    }


    unsigned CodeEditorLineNumberArea::numberRequiredDigits() const {
        return currentNumberRequiredDigits;
    }


    QRectF CodeEditorLineNumberArea::blockBoundingGeometry(const QTextBlock& block) const {
        QRectF result;

        if (currentEditor != Q_NULLPTR) {
            result = currentEditor->blockBoundingGeometry(block);
        }

        return result;
    }


    QRect CodeEditorLineNumberArea::blockViewportCoordinates(const QTextBlock& block) const {
        QRect result;

        if (currentEditor != Q_NULLPTR) {
            result = currentEditor->blockBoundingGeometry(block).translated(currentEditor->contentOffset()).toRect();
        }

        return result;
    }


    QRectF CodeEditorLineNumberArea::blockBoundingRectangle(const QTextBlock& block) {
        QRectF result;

        if (currentEditor != Q_NULLPTR) {
            result = currentEditor->blockBoundingRect(block);
        }

        return result;
    }


    unsigned CodeEditorLineNumberArea::calculateWidth(unsigned requiredNumberDigits) const {
        unsigned newWidth;

        if (currentDisplayLineNumbers) {
            QString digits = tr("01234567890");

            QFontMetrics fontMetrics(currentFont);
            unsigned maximumDigitWidth = 0;
            for (QString::const_iterator it=digits.constBegin(),end=digits.constEnd() ; it!=end ; ++it) {
                unsigned digitWidth = fontMetrics.horizontalAdvance(*it);
                if (digitWidth > maximumDigitWidth) {
                    maximumDigitWidth = digitWidth;
                }
            }

            newWidth = currentLeftPadding + maximumDigitWidth * requiredNumberDigits + currentRightPadding;
        } else {
            newWidth = 0;
        }

        return newWidth;
    }


    void CodeEditorLineNumberArea::paintEvent(QPaintEvent* event) {
        if (currentEditor != Q_NULLPTR) {
            QPainter painter(this);

            const QPalette& palette         = CodeEditorLineNumberArea::palette();
            QColor          textColor       = palette.color(foregroundRole());
            QColor          backgroundColor = palette.color(backgroundRole());

            painter.fillRect(event->rect(), backgroundColor);

            QPen            pen(textColor);
            painter.setPen(pen);

            int         areaTop        = event->rect().top();
            int         areaBottom     = event->rect().bottom();
            QTextBlock  block          = currentEditor->firstVisibleBlock();
            long        blockNumber    = block.blockNumber();
            QRect       blockRectangle = blockBoundingGeometry(block).toRect();
            int         top            = blockRectangle.top();
            int         bottom         = top + static_cast<int>(blockBoundingRectangle(block).height());

            while (block.isValid() && top <= areaBottom) {
                if (block.isVisible() && bottom >= areaTop) {
                    QString lineNumber = QString::number(blockNumber + 1);
                    painter.drawText(
                        currentLeftPadding,
                        top,
                        currentRequiredWidth - currentRightPadding - currentLeftPadding,
                        currentLineNumberHeight,
                        Qt::AlignRight,
                        lineNumber
                    );
                }

                block  = block.next();
                top    = bottom;
                bottom = top + static_cast<int>(blockBoundingRectangle(block).height());

                ++blockNumber;
            }
        }
    }


    void CodeEditorLineNumberArea::updateWidth() {
        unsigned newRequiredWidth = calculateWidth(currentNumberRequiredDigits);
        if (newRequiredWidth != currentRequiredWidth) {
            currentRequiredWidth = newRequiredWidth;
            updateGeometry();

            if (currentEditor != Q_NULLPTR) {
                currentEditor->setViewportMargins(currentRequiredWidth, 0, 0, 0);
            }
        }
    }
}
