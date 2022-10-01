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
* This file implements the \ref EQt::GraphicsMathGroupWithLine class.
***********************************************************************************************************************/

#include <QWidget>
#include <QPen>
#include <QPointF>
#include <QLineF>
#include <QRectF>
#include <QMarginsF>
#include <QPainter>

#include <limits>

#include "eqt_graphics_math_group.h"
#include "eqt_graphics_math_group_with_line.h"

namespace EQt {
    GraphicsMathGroupWithLine::GraphicsMathGroupWithLine() {
        currentLinePen = QPen(Qt::black);
        currentLine    = QLineF(0.0F, 0.0F, 0.0F, 0.0F);
    }


    GraphicsMathGroupWithLine::~GraphicsMathGroupWithLine() {}


    int GraphicsMathGroupWithLine::type() const {
        return Type;
    }


    void GraphicsMathGroupWithLine::setLine(const QLineF& newLine) {
        currentLine = newLine;
        prepareGeometryChange();
    }


    void GraphicsMathGroupWithLine::setLine(const QPointF& p1, const QPointF& p2) {
        currentLine.setP1(p1);
        currentLine.setP2(p2);
        prepareGeometryChange();
    }


    void GraphicsMathGroupWithLine::setLine(float x1, float y1, float x2, float y2) {
        currentLine = QLineF(x1, y1, x2, y2);
        prepareGeometryChange();
    }


    void GraphicsMathGroupWithLine::setP1(const QPointF& newPosition) {
        currentLine.setP1(newPosition);
        prepareGeometryChange();
    }


    void GraphicsMathGroupWithLine::setP1(float x, float y) {
        currentLine.setP1(QPointF(x, y));
        prepareGeometryChange();
    }


    void GraphicsMathGroupWithLine::setP2(const QPointF& newPosition) {
        currentLine.setP2(newPosition);
        prepareGeometryChange();
    }


    void GraphicsMathGroupWithLine::setP2(float x, float y) {
        currentLine.setP2(QPointF(x, y));
        prepareGeometryChange();
    }


    QLineF GraphicsMathGroupWithLine::line() const {
        return currentLine;
    }


    QPointF GraphicsMathGroupWithLine::p1() const {
        return currentLine.p1();
    }


    QPointF GraphicsMathGroupWithLine::p2() const {
        return currentLine.p2();
    }


    float GraphicsMathGroupWithLine::x1() const {
        return currentLine.x1();
    }


    float GraphicsMathGroupWithLine::y1() const {
        return currentLine.y1();
    }


    float GraphicsMathGroupWithLine::x2() const {
        return currentLine.x2();
    }


    float GraphicsMathGroupWithLine::y2() const {
        return currentLine.y2();
    }


    void GraphicsMathGroupWithLine::setLinePen(const QPen& pen) {
        currentLinePen = pen;
        update();
    }


    QPen GraphicsMathGroupWithLine::linePen() const {
        return currentLinePen;
    }


    QRectF GraphicsMathGroupWithLine::boundingRect() const {
        QRectF result;

        if (currentLinePen.style() != Qt::NoPen) {
            float  penMargin = std::max(
                std::numeric_limits<float>::epsilon(),
                static_cast<float>(currentLinePen.widthF()) / 2.0F
            );

            QRectF lineRectangle(currentLine.p1(), currentLine.p2());
            result = lineRectangle.normalized().marginsAdded(QMarginsF(penMargin, penMargin, penMargin, penMargin));
        } else {
            result = GraphicsMathGroup::boundingRect();
        }

        return result;
    }


    void GraphicsMathGroupWithLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        GraphicsMathGroup::paint(painter, option, widget);

        painter->setPen(currentLinePen);
        painter->drawLine(currentLine);
    }
}
