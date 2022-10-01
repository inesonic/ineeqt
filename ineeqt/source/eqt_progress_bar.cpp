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
* This file implements the \ref EQt::ProgressBar class.
***********************************************************************************************************************/

#include <QWidget>
#include <QString>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFontMetrics>
#include <QSize>
#include <QPalette>

#include "eqt_application.h"
#include "eqt_progress_bar.h"

namespace EQt {
    const int    ProgressBar::defaultMinimumValue = 0;
    const int    ProgressBar::defaultMaximumValue = 100;
    const int    ProgressBar::defaultValue        = 0;

    ProgressBar::ProgressBar(QWidget* parent):QWidget(parent) {
        currentMinimumValue = defaultMinimumValue;
        currentMaximumValue = defaultMaximumValue;
        currentValue        = defaultValue;
        currentTextFont     = Application::font();

        updateHeight();
    }


    ProgressBar::~ProgressBar() {}


    int ProgressBar::minimumValue() const {
        return currentMinimumValue;
    }


    int ProgressBar::maximumValue() const {
        return currentMaximumValue;
    }


    int ProgressBar::value() const {
        return currentValue;
    }


    QString ProgressBar::text() const {
        return currentText;
    }


    QColor ProgressBar::progressBarColor() const {
        return currentProgressBarColor;
    }


    QColor ProgressBar::backgroundColor() const {
        return currentBackgroundColor;
    }


    QFont ProgressBar::textFont() const {
        return currentTextFont;
    }


    QColor ProgressBar::textColor() const {
        return currentTextColor;
    }


    void ProgressBar::setMinimumValue(int newMinimumValue) {
        currentMinimumValue = newMinimumValue;
        update();
    }


    void ProgressBar::setMaximumValue(int newMaximumValue) {
        currentMaximumValue = newMaximumValue;
        update();
    }


    void ProgressBar::setValue(int newValue) {
        currentValue = newValue;
        update();
    }


    void ProgressBar::setText(const QString& newText) {
        currentText = newText;
        update();
    }

    void ProgressBar::setProgressBarColor(const QColor& newColor) {
        currentProgressBarColor = newColor;
        update();
    }


    void ProgressBar::setBackgroundColor(const QColor& newColor) {
        currentBackgroundColor = newColor;
        update();
    }


    void ProgressBar::setTextFont(const QFont& newFont) {
        currentTextFont = newFont;
        updateHeight();
        update();
    }


    void ProgressBar::setTextColor(const QColor& newColor) {
        currentTextColor = newColor;
        update();
    }


    void ProgressBar::paintEvent(QPaintEvent*) {
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        unsigned  width  = ProgressBar::width();
        unsigned  height = ProgressBar::height();

        QFontMetrics textFontMetrics(currentTextFont);
        unsigned  textHeight = static_cast<unsigned>(textFontMetrics.height());
        unsigned  textWidth  = static_cast<unsigned>(textFontMetrics.horizontalAdvance(currentText));
        unsigned  textAscent = static_cast<unsigned>(textFontMetrics.ascent());

        long long numerator = width * (currentValue - currentMinimumValue);
        long long rightEdge = numerator / (currentMaximumValue - currentMinimumValue);

        if (rightEdge < 0) {
            rightEdge = 0;
        } else if (rightEdge > width) {
            rightEdge = width;
        }

        const QPalette& palette = ProgressBar::palette();

        QPen   barPen(Qt::NoPen);
        QBrush barBrush        =   currentProgressBarColor.isValid()
                                 ? QBrush(currentProgressBarColor)
                                 : palette.highlight();
        QBrush backgroundBrush =   currentBackgroundColor.isValid()
                                 ? QBrush(currentBackgroundColor)
                                 : palette.mid();
        QPen   textPen         =   currentTextColor.isValid()
                                 ? QPen(currentTextColor)
                                 : QPen(palette.text().color());

        painter.setBrush(barBrush);
        painter.setPen(barPen);
        painter.drawRect(0, 0, static_cast<int>(rightEdge), height);

        painter.setBrush(backgroundBrush);
        painter.drawRect(static_cast<int>(rightEdge), 0, width - static_cast<int>(rightEdge), height);

        unsigned textY = (height - textHeight) / 2 + textAscent;
        unsigned textX = width > textWidth ? (width - textWidth) / 2 : 0;

        painter.setPen(textPen);
        painter.setFont(currentTextFont);
        painter.drawText(textX, textY, currentText);
    }


    void ProgressBar::updateHeight() {
        QFontMetrics textFontMetrics(currentTextFont);
        unsigned textHeight = static_cast<unsigned>(textFontMetrics.height());
        setFixedHeight(textHeight + verticalPadding);
    }
}
