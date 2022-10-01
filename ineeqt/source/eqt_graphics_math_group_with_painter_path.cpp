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
* This file implements the \ref EQt::GraphicsMathGroupWithPainterPath class.
***********************************************************************************************************************/

#include <QWidget>
#include <QList>
#include <QBrush>
#include <QPen>
#include <QPainterPath>
#include <QPainter>

#include "eqt_graphics_math_group.h"
#include "eqt_graphics_math_group_with_painter_path.h"

namespace EQt {
    GraphicsMathGroupWithPainterPath::GraphicsMathGroupWithPainterPath() {
        currentPainterPathPen   = QPen(Qt::black);
        currentPainterPathBrush = QBrush(QColor(Qt::black));
    }


    GraphicsMathGroupWithPainterPath::~GraphicsMathGroupWithPainterPath() {}


    int GraphicsMathGroupWithPainterPath::type() const {
        return Type;
    }


    void GraphicsMathGroupWithPainterPath::setPainterPath(const QPainterPath& newPainterPath) {
        currentPainterPath = newPainterPath;
        prepareGeometryChange();
    }


    const QPainterPath& GraphicsMathGroupWithPainterPath::painterPath() const {
        return currentPainterPath;
    }


    void GraphicsMathGroupWithPainterPath::setPainterPathPen(const QPen& newPen) {
        currentPainterPathPen = newPen;
        update();
    }


    const QPen& GraphicsMathGroupWithPainterPath::painterPathPen() const {
        return currentPainterPathPen;
    }


    void GraphicsMathGroupWithPainterPath::setPainterPathBrush(const QBrush& newBrush) {
        currentPainterPathBrush = newBrush;
        update();
    }


    const QBrush& GraphicsMathGroupWithPainterPath::painterPathBrush() const {
        return currentPainterPathBrush;
    }


    QRectF GraphicsMathGroupWithPainterPath::boundingRect() const {
        return GraphicsMathGroup::boundingRect().united(currentPainterPath.boundingRect());
    }


    void GraphicsMathGroupWithPainterPath::paint(
            QPainter*                       painter,
            const QStyleOptionGraphicsItem* option,
            QWidget*                        widget
        ) {
        GraphicsMathGroup::paint(painter, option, widget);

        painter->setPen(currentPainterPathPen);
        painter->setBrush(currentPainterPathBrush);
        painter->drawPath(currentPainterPath);
    }
}
