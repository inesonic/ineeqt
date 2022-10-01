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
* This file implements the \ref EQt::GraphicsMultiTextGroup class.
***********************************************************************************************************************/

#include <QGraphicsItemGroup>
#include <QWidget>
#include <QList>
#include <QBrush>
#include <QPen>
#include <QPointF>
#include <QPainter>
#include <QFontMetricsF>
#include <QFontMetrics>

#include <cmath>

#include "eqt_graphics_item_group.h"
#include "eqt_graphics_multi_text_group.h"

/***********************************************************************************************************************
 * EQt::GraphicsMultiTextGroup::TextEntry
 */

namespace EQt {
    GraphicsMultiTextGroup::TextEntry::TextEntry() {
        itemGroup = Q_NULLPTR;
    }


    GraphicsMultiTextGroup::TextEntry::TextEntry(const QString& text, const QFont& font, const QPointF& position) {
        currentText     = text;
        currentFont     = font;
        currentPosition = position;
        itemGroup       = Q_NULLPTR;
    }


    GraphicsMultiTextGroup::TextEntry::TextEntry(const QString& text, const QFont& font, double x, double y) {
        currentText     = text;
        currentFont     = font;
        currentPosition = QPointF(x, y);
        itemGroup       = Q_NULLPTR;
    }


    GraphicsMultiTextGroup::TextEntry::TextEntry(const GraphicsMultiTextGroup::TextEntry& other) {
        currentText     = other.currentText;
        currentFont     = other.currentFont;
        currentPosition = other.currentPosition;
        itemGroup       = Q_NULLPTR;
    }


    GraphicsMultiTextGroup::TextEntry::~TextEntry() {}


    void GraphicsMultiTextGroup::TextEntry::setText(const QString& newText) {
        currentText = newText;
        triggerUpdates();
    }


    const QString& GraphicsMultiTextGroup::TextEntry::text() const {
        return currentText;
    }


    void GraphicsMultiTextGroup::TextEntry::setFont(const QFont& newFont) {
        currentFont = newFont;
        triggerUpdates();
    }


    const QFont& GraphicsMultiTextGroup::TextEntry::font() const {
        return currentFont;
    }


    void GraphicsMultiTextGroup::TextEntry::setPosition(const QPointF& newPosition) {
        currentPosition = newPosition;
        triggerUpdates();
    }


    void GraphicsMultiTextGroup::TextEntry::setPosition(double x, double y) {
        setPosition(QPointF(x, y));
        triggerUpdates();
    }


    const QPointF& GraphicsMultiTextGroup::TextEntry::position() const {
        return currentPosition;
    }


    GraphicsMultiTextGroup::TextEntry& GraphicsMultiTextGroup::TextEntry::operator=(
            const GraphicsMultiTextGroup::TextEntry& other
        ) {
        currentText     = other.currentText;
        currentFont     = other.currentFont;
        currentPosition = other.currentPosition;

        return *this;
    }


    void GraphicsMultiTextGroup::TextEntry::triggerUpdates() {
        if (itemGroup != Q_NULLPTR) {
            itemGroup->updateGeometryNeeded();
            itemGroup->update();
        }
    }
}

/***********************************************************************************************************************
 * EQt::GraphicsMultiTextGroup
 */

namespace EQt {
    GraphicsMultiTextGroup::GraphicsMultiTextGroup() {
        currentBackgroundBrush = QBrush(QColor(255, 255, 255, 0));
        currentBorderPen       = QPen(Qt::NoPen);
        currentTextPen         = QPen(QColor(Qt::black));
    }


    GraphicsMultiTextGroup::~GraphicsMultiTextGroup() {}


    int GraphicsMultiTextGroup::type() const {
        return Type;
    }


    void GraphicsMultiTextGroup::deferUpdates() {
        currentUpdatesAreDeferred = true;
    }


    void GraphicsMultiTextGroup::restoreUpdates() {
        if (currentUpdatesAreDeferred) {
            currentUpdatesAreDeferred = false;
            prepareGeometryChange();
        }
    }


    bool GraphicsMultiTextGroup::updatesAreDeferred() const {
        return currentUpdatesAreDeferred;
    }


    void GraphicsMultiTextGroup::setBackgroundBrush(const QBrush& newBrush) {
        currentBackgroundBrush = newBrush;
        update();
    }


    const QBrush& GraphicsMultiTextGroup::backgroundBrush() const {
        return currentBackgroundBrush;
    }


    void GraphicsMultiTextGroup::setBorderPen(const QPen& newPen) {
        currentBorderPen = newPen;
        update();
    }


    const QPen& GraphicsMultiTextGroup::borderPen() const {
        return currentBorderPen;
    }


    void GraphicsMultiTextGroup::setTextPen(const QPen& newPen) {
        currentTextPen = newPen;
        update();
    }


    QPen GraphicsMultiTextGroup::textPen() const {
        return currentTextPen;
    }


    void GraphicsMultiTextGroup::clearText() {
        currentTextEntries.clear();

        updateGeometryNeeded();
        update();
    }


    void GraphicsMultiTextGroup::removeFirst() {
        currentTextEntries.removeFirst();

        updateGeometryNeeded();
        update();
    }


    void GraphicsMultiTextGroup::removeLast() {
        currentTextEntries.removeLast();

        updateGeometryNeeded();
        update();
    }


    void GraphicsMultiTextGroup::removeAt(unsigned index) {
        currentTextEntries.removeAt(index);

        updateGeometryNeeded();
        update();
    }


    void GraphicsMultiTextGroup::setNumberEntries(unsigned newNumberEntries) {
        if (newNumberEntries < static_cast<unsigned>(currentTextEntries.size())) {
            currentTextEntries.erase(currentTextEntries.begin() + newNumberEntries, currentTextEntries.end());
        }
    }


    void GraphicsMultiTextGroup::append(const GraphicsMultiTextGroup::TextEntry& newEntry) {
        currentTextEntries.append(newEntry);
        currentTextEntries.last().itemGroup = this;

        updateGeometryNeeded();
        update();
    }


    void GraphicsMultiTextGroup::append(const QString& newText, const QFont& newFont, const QPointF& newPosition) {
        append(TextEntry(newText, newFont, newPosition));
    }


    void GraphicsMultiTextGroup::prepend(const GraphicsMultiTextGroup::TextEntry& newEntry) {
        currentTextEntries.prepend(newEntry);
        currentTextEntries.first().itemGroup = this;

        updateGeometryNeeded();
        update();
    }


    void GraphicsMultiTextGroup::prepend(const QString& newText, const QFont& newFont, const QPointF& newPosition) {
        prepend(TextEntry(newText, newFont, newPosition));
    }


    unsigned GraphicsMultiTextGroup::numberTextEntries() const {
        return static_cast<unsigned>(currentTextEntries.size());
    }


    const GraphicsMultiTextGroup::TextEntry& GraphicsMultiTextGroup::entry(unsigned index) const {
        return currentTextEntries.at(index);
    }


    GraphicsMultiTextGroup::TextEntry& GraphicsMultiTextGroup::entry(unsigned index) {
        return currentTextEntries[index];
    }


    const GraphicsMultiTextGroup::TextEntry& GraphicsMultiTextGroup::operator[](unsigned index) const {
        return currentTextEntries.at(index);
    }


    GraphicsMultiTextGroup::TextEntry& GraphicsMultiTextGroup::operator[](unsigned index) {
        return currentTextEntries[index];
    }


    GraphicsMultiTextGroup::Iterator GraphicsMultiTextGroup::begin() {
        return currentTextEntries.begin();
    }


    GraphicsMultiTextGroup::Iterator GraphicsMultiTextGroup::end() {
        return currentTextEntries.end();
    }


    GraphicsMultiTextGroup::ConstIterator GraphicsMultiTextGroup::constBegin() const {
        return currentTextEntries.constBegin();
    }


    GraphicsMultiTextGroup::ConstIterator GraphicsMultiTextGroup::constEnd() {
        return currentTextEntries.constEnd();
    }


    QRectF GraphicsMultiTextGroup::boundingRect() const {
        QRectF result;

        if (hasForcedBoundingRectangle()) {
            result = forcedGeometry();
        } else if (currentTextEntries.isEmpty()) {
            result = childrenBoundingRect();
        } else {
            if (currentTextBoundingRectangle.isNull()) {
                QList<TextEntry>::const_iterator textEntryIterator    = currentTextEntries.constBegin();
                QList<TextEntry>::const_iterator textEntryEndIterator = currentTextEntries.constEnd();

                const TextEntry& entry    = *textEntryIterator;
                const QString&   text     = entry.text();
                const QFont&     font     = entry.font();
                const QPointF&   position = entry.position();

                QFontMetricsF fontMetrics(font);
                currentTextBoundingRectangle.setTopLeft(QPointF(position.x(), position.y() - fontMetrics.ascent()));
                currentTextBoundingRectangle.setWidth(fontMetrics.horizontalAdvance(text));
                currentTextBoundingRectangle.setHeight(fontMetrics.height());

                ++textEntryIterator;
                while (textEntryIterator != textEntryEndIterator) {
                    const TextEntry& entry    = *textEntryIterator;
                    const QString&   text     = entry.text();
                    const QFont&     font     = entry.font();
                    const QPointF&   position = entry.position();

                    QFontMetricsF fontMetrics(font);
                    currentTextBoundingRectangle |= QRectF(
                        QPointF(position.x(), position.y() - fontMetrics.ascent()),
                        QSizeF(fontMetrics.horizontalAdvance(text), fontMetrics.height())
                    );

                    ++textEntryIterator;
                }
            }

            result = currentTextBoundingRectangle | childrenBoundingRect();
        }

        return result;
    }


    void GraphicsMultiTextGroup::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        this->GraphicsItemGroup::paint(painter, option, widget);

        QRectF boundingRectangle = boundingRect();

        painter->setBrush(currentBackgroundBrush);
        painter->setPen(currentBorderPen);
        painter->drawRect(boundingRectangle);

        painter->setPen(currentTextPen);

        QPaintDevice* device             = painter->device();
        bool          convertToPixelSize = dynamic_cast<QWidget*>(device) == Q_NULLPTR;

        for (  QList<TextEntry>::const_iterator textEntryIterator    = currentTextEntries.constBegin(),
                                                textEntryEndIterator = currentTextEntries.constEnd()
             ; textEntryIterator != textEntryEndIterator
             ; ++textEntryIterator
            ) {
            const TextEntry& entry    = *textEntryIterator;
            const QString&   text     = entry.text();
            const QPointF&   position = entry.position();
            const QFont&     font     = entry.font();

            if (convertToPixelSize) {
                QFontMetricsF fontMetrics(font);
                unsigned screenHeight = static_cast<unsigned>(fontMetrics.height());
                float    screenWidth  = fontMetrics.horizontalAdvance(text);

                QFont tweakedFont = QFont(font, device);
                tweakedFont.setPixelSize(screenHeight);

                unsigned smallerHeight = screenHeight;
                float    smallerWidth  = QFontMetricsF(tweakedFont).horizontalAdvance(text);
                unsigned biggerHeight  = smallerHeight;
                float    biggerWidth   = smallerWidth;

                while (smallerWidth > screenWidth) {
                    biggerHeight = smallerHeight;
                    biggerWidth = smallerWidth;

                    --smallerHeight;

                    tweakedFont.setPixelSize(smallerHeight);
                    smallerWidth = QFontMetricsF(tweakedFont).horizontalAdvance(text);
                }

                Q_ASSERT(smallerWidth <= screenWidth);

                if (abs(screenWidth - smallerWidth) > abs(biggerWidth - screenWidth)) {
                    tweakedFont.setPixelSize(biggerHeight);
                }

                painter->setFont(tweakedFont);
            } else {
                painter->setFont(font);
            }

            painter->drawText(position, text);
        }
    }


    void GraphicsMultiTextGroup::updateGeometryNeeded() {
        if (!currentUpdatesAreDeferred) {
            prepareGeometryChange();
            currentTextBoundingRectangle = QRectF();
        }
    }
}

