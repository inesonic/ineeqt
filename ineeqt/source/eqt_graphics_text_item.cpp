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
* This file implements the \ref EQt::GraphicsTextItem class.
***********************************************************************************************************************/

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QString>
#include <QFont>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include "eqt_graphics_text_item.h"

namespace EQt {
    GraphicsTextItem::GraphicsTextItem(QGraphicsItem* parent):QGraphicsSimpleTextItem(parent) {
        drawBackground   = false;
        currentBorderPen = QPen(Qt::NoPen);
    }


    GraphicsTextItem::GraphicsTextItem(
            const QString& text,
            QGraphicsItem* parent
        ):QGraphicsSimpleTextItem(
            text,
            parent
        ) {
        drawBackground   = false;
        currentBorderPen = QPen(Qt::NoPen);
    }


    GraphicsTextItem::GraphicsTextItem(
            const QString& text,
            const QFont&   font,
            QGraphicsItem* parent
        ):QGraphicsSimpleTextItem(
            text,
            parent
        ) {
        setFont(font);
        drawBackground   = false;
        currentBorderPen = QPen(Qt::NoPen);
    }


    GraphicsTextItem::~GraphicsTextItem() {}


    int GraphicsTextItem::type() const {
        return Type;
    }


    void GraphicsTextItem::removeFromScene() {
        prepareGeometryChange();
        removeGraphicsItemFromScene(this);
    }


    bool GraphicsTextItem::isDrawingBackground() const {
        return drawBackground;
    }


    bool GraphicsTextItem::isNotDrawingBackground() const {
        return !drawBackground;
    }


    void GraphicsTextItem::setBackgroundBrush(const QBrush& newBrush) {
        drawBackground         = true;
        currentBackgroundBrush = newBrush;
    }


    void GraphicsTextItem::clearBackgroundBrush() {
        drawBackground = false;
        currentBackgroundBrush = QBrush();
    }


    QBrush GraphicsTextItem::backgroundBrush() const {
        return currentBackgroundBrush;
    }


    void GraphicsTextItem::setBorderPen(const QPen& newPen) {
        currentBorderPen = newPen;
    }


    QPen GraphicsTextItem::borderPen() const {
        return currentBorderPen;
    }


    void GraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        if (drawBackground) {
            painter->setBrush(currentBackgroundBrush);
        } else {
            painter->setBrush(QBrush());
        }

        painter->setPen(currentBorderPen);
        painter->drawRect(boundingRect());
        QGraphicsSimpleTextItem::paint(painter, option, widget);
    }
}
