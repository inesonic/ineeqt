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
* This file implements the \ref EQt::FontButton class.
***********************************************************************************************************************/

#include <QWidget>
#include <QString>
#include <QFont>
#include <QColor>
#include <QPushButton>
#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QIcon>

#include "eqt_application.h"
#include "eqt_font_button.h"

namespace EQt {
    FontButton::FontButton(QWidget* parent):QPushButton(parent) {
        currentGlyphText = tr("F");
        currentGlyphFont = Application::font();
        currentForegroundColor = QColor(Qt::GlobalColor::black);
        currentBackgroundColor = QColor();

        updateGlyph();
    }


    FontButton::FontButton(const QString& glyphText, const QString& text, QWidget* parent):QPushButton(text, parent) {
        currentGlyphText = glyphText;
        currentGlyphFont = Application::font();
        currentForegroundColor = QColor(Qt::GlobalColor::black);
        currentBackgroundColor = QColor();

        updateGlyph();
    }


    FontButton::FontButton(
            const QString& glyphText,
            const QFont&   font,
            const QString& text,
            QWidget*       parent
        ):QPushButton(
            text,
            parent
        ) {
        currentGlyphText = glyphText;
        currentGlyphFont = font;
        currentForegroundColor = QColor(Qt::GlobalColor::black);
        currentBackgroundColor = QColor();

        updateGlyph();
    }


    FontButton::FontButton(
            const QString& glyphText,
            const QFont&   font,
            const QColor&  fontColor,
            const QString& text,
            QWidget*       parent
        ):QPushButton(
            text,
            parent
        ) {
        currentGlyphText = glyphText;
        currentGlyphFont = font;
        currentForegroundColor = fontColor;
        currentBackgroundColor = QColor();

        updateGlyph();
    }


    FontButton::FontButton(
            const QString& glyphText,
            const QFont&   font,
            const QColor&  fontColor,
            const QColor&  fontBackgroundColor,
            const QString& text,
            QWidget*       parent
        ):QPushButton(
            text,
            parent
        ) {
        currentGlyphText = glyphText;
        currentGlyphFont = font;
        currentForegroundColor = fontColor;
        currentBackgroundColor = fontBackgroundColor;

        updateGlyph();
    }


    QString FontButton::glyphText() const {
        return currentGlyphText;
    }


    QString FontButton::descriptionText() const {
        return text();
    }


    QFont FontButton::glyphFont() const {
        return currentGlyphFont;
    }


    QFont FontButton::descriptionFont() const {
        return font();
    }


    QColor FontButton::fontColor() const {
        return currentForegroundColor;
    }


    QColor FontButton::fontBackgroundColor() const {
        return currentBackgroundColor;
    }


    void FontButton::setGlyphText(const QString& newText) {
        currentGlyphText = newText;
        updateGlyph();
    }


    void FontButton::setGlyphFont(const QFont& newFont) {
        currentGlyphFont = newFont;
        updateGlyph();
    }


    void FontButton::setDescriptionFont(const QFont& newFont) {
        setFont(newFont);
    }


    void FontButton::setFontColor(const QColor& newColor) {
        currentForegroundColor = newColor;
        updateGlyph();
    }


    void FontButton::setFontBackgroundColor(const QColor& newColor) {
        currentBackgroundColor = newColor;
        updateGlyph();
    }


    void FontButton::setDescriptionText(const QString& newText) {
        setText(newText);
    }


    void FontButton::updateGlyph() {
        QString      contentsString = tr(" %1 ").arg(currentGlyphText);
        QFontMetrics fontMetrics(currentGlyphFont);
        QSize        glyphSize(fontMetrics.horizontalAdvance(contentsString) + 2, fontMetrics.height() + 2);

        QPixmap pixmap(glyphSize);

        if (currentBackgroundColor.isValid()) {
            pixmap.fill(currentBackgroundColor);
        } else {
            pixmap.fill(QColor(255, 255, 255, 0));
        }

        QPainter painter(&pixmap);
        painter.setPen(QPen(QBrush(currentForegroundColor), 1.0));
        painter.setFont(currentGlyphFont);
        painter.setBrush(QBrush());

        painter.drawRect(QRect(1, 1, glyphSize.width() - 2, glyphSize.height() - 2));
        painter.drawText(1, fontMetrics.ascent() + 1, contentsString);

        setIcon(QIcon(pixmap));
        setIconSize(glyphSize);
    }
}
