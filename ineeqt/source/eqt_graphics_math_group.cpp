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
* This file implements the \ref EQt::GraphicsMathGroup class.
***********************************************************************************************************************/

#include <QWidget>
#include <QList>
#include <QBrush>
#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>

#include "eqt_graphics_multi_text_group.h"
#include "eqt_graphics_math_group.h"

namespace EQt {
    GraphicsMathGroup::GraphicsMathGroup() {
        currentParenthesisPen         = QPen(QColor(Qt::black));
        currentParenthesisBrush       = QBrush(QColor(Qt::black));
        currentLeftParenthesisStyle   = ParenthesisStyle::NONE;
        currentRightParenthesisStyle  = ParenthesisStyle::NONE;
        currentBottomParenthesisStyle = ParenthesisStyle::NONE;
        currentParenthesisCenterLine  = -1.0F;
    }


    GraphicsMathGroup::~GraphicsMathGroup() {}


    int GraphicsMathGroup::type() const {
        return Type;
    }


    void GraphicsMathGroup::setLeftParenthesisStyle(GraphicsMathGroup::ParenthesisStyle newParenthesisStyle) {
        currentLeftParenthesisStyle = newParenthesisStyle;
        update();
    }


    GraphicsMathGroup::ParenthesisStyle GraphicsMathGroup::leftParenthesisStyle() const {
        return currentLeftParenthesisStyle;
    }


    void GraphicsMathGroup::setLeftParenthesisBoundingRectangle(const QRectF& parenthesisBoundingRectangle) {
        currentLeftParenthesisBoundingRectangle = parenthesisBoundingRectangle;
        prepareGeometryChange();
    }


    QRectF GraphicsMathGroup::leftParenthesisBoundingRectangle() const {
        return currentLeftParenthesisBoundingRectangle;
    }


    void GraphicsMathGroup::setLeftParenthesis(
            GraphicsMathGroup::ParenthesisStyle newParenthesisStyle,
            const QRectF&                       parenthesisBoundingRectangle
        ) {
        currentLeftParenthesisStyle             = newParenthesisStyle;
        currentLeftParenthesisBoundingRectangle = parenthesisBoundingRectangle;
        prepareGeometryChange();
    }


    void GraphicsMathGroup::setRightParenthesisStyle(GraphicsMathGroup::ParenthesisStyle newParenthesisStyle) {
        currentRightParenthesisStyle = newParenthesisStyle;
        update();
    }


    GraphicsMathGroup::ParenthesisStyle GraphicsMathGroup::rightParenthesisStyle() const {
        return currentRightParenthesisStyle;
    }


    void GraphicsMathGroup::setRightParenthesisBoundingRectangle(const QRectF& parenthesisBoundingRectangle) {
        currentRightParenthesisBoundingRectangle = parenthesisBoundingRectangle;
        prepareGeometryChange();
    }


    QRectF GraphicsMathGroup::rightParenthesisBoundingRectangle() const {
        return currentRightParenthesisBoundingRectangle;
    }


    void GraphicsMathGroup::setRightParenthesis(
            ParenthesisStyle newParenthesisStyle,
            const QRectF&    parenthesisBoundingRectangle
        ) {
        currentRightParenthesisStyle             = newParenthesisStyle;
        currentRightParenthesisBoundingRectangle = parenthesisBoundingRectangle;
        prepareGeometryChange();
    }


    void GraphicsMathGroup::setParenthesisCenterLine(float newCenterLine) {
        currentParenthesisCenterLine = newCenterLine;
        update();
    }


    float GraphicsMathGroup::parenthesisCenterLine() const {
        return currentParenthesisCenterLine;
    }


    void GraphicsMathGroup::setBottomParenthesisStyle(ParenthesisStyle newParenthesisStyle) {
        currentBottomParenthesisStyle = newParenthesisStyle;
        update();
    }


    GraphicsMathGroup::ParenthesisStyle GraphicsMathGroup::bottomParenthesisStyle() const {
        return currentBottomParenthesisStyle;
    }


    void GraphicsMathGroup::setBottomParenthesisBoundingRectangle(const QRectF& parenthesisBoundingRectangle) {
        currentBottomParenthesisBoundingRectangle = parenthesisBoundingRectangle;
        prepareGeometryChange();
    }


    QRectF GraphicsMathGroup::bottomParenthesisBoundingRectangle() const {
        return currentBottomParenthesisBoundingRectangle;
    }


    void GraphicsMathGroup::setBottomParenthesis(
            GraphicsMathGroup::ParenthesisStyle newParenthesisStyle,
            const QRectF&                       parenthesisBoundingRectangle
        ) {
        currentBottomParenthesisStyle             = newParenthesisStyle;
        currentBottomParenthesisBoundingRectangle = parenthesisBoundingRectangle;
        prepareGeometryChange();
    }


    void GraphicsMathGroup::setParenthesisPen(const QPen& newPen) {
        currentParenthesisPen = newPen;
        update();
    }


    QPen GraphicsMathGroup::parenthesisPen() const {
        return currentParenthesisPen;
    }


    void GraphicsMathGroup::setParenthesisBrush(const QBrush& newBrush) {
        currentParenthesisBrush = newBrush;
        update();
    }


    QBrush GraphicsMathGroup::parenthesisBrush() const {
        return currentParenthesisBrush;
    }


    QRectF GraphicsMathGroup::boundingRect() const {
        QRectF result = GraphicsMultiTextGroup::boundingRect();

        if (currentLeftParenthesisStyle != ParenthesisStyle::INVALID &&
            currentLeftParenthesisStyle != ParenthesisStyle::NONE       ) {
            result |= currentLeftParenthesisBoundingRectangle;
        }

        if (currentRightParenthesisStyle != ParenthesisStyle::INVALID &&
            currentRightParenthesisStyle != ParenthesisStyle::NONE       ) {
            result |= currentRightParenthesisBoundingRectangle;
        }

        if (currentBottomParenthesisStyle != ParenthesisStyle::INVALID &&
            currentBottomParenthesisStyle != ParenthesisStyle::NONE       ) {
            result |= currentBottomParenthesisBoundingRectangle;
        }

        return result;
    }


    void GraphicsMathGroup::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        GraphicsMultiTextGroup::paint(painter, option, widget);

        if (currentLeftParenthesisStyle != ParenthesisStyle::INVALID &&
            currentLeftParenthesisStyle != ParenthesisStyle::NONE       ) {
            QTransform transform(
                currentLeftParenthesisBoundingRectangle.width(),  // m11
                0.0F,                                             // m12
                0.0F,                                             // m21
                currentLeftParenthesisBoundingRectangle.height(), // m22
                currentLeftParenthesisBoundingRectangle.left(),   // m31
                currentLeftParenthesisBoundingRectangle.top()     // m32
            );

            float normalizedCenterLine = (
                  (currentParenthesisCenterLine - currentLeftParenthesisBoundingRectangle.top())
                / currentLeftParenthesisBoundingRectangle.height()
            );

            drawParenthesis(
                painter,
                transform,
                longToShort(currentLeftParenthesisBoundingRectangle),
                normalizedCenterLine,
                currentLeftParenthesisStyle
            );
        }

        if (currentRightParenthesisStyle != ParenthesisStyle::INVALID &&
            currentRightParenthesisStyle != ParenthesisStyle::NONE       ) {
            QTransform transform(
                -currentRightParenthesisBoundingRectangle.width(), // m11
                0.0F,                                              // m12
                0.0F,                                              // m21
                currentRightParenthesisBoundingRectangle.height(), // m22
                currentRightParenthesisBoundingRectangle.right(),  // m31
                currentRightParenthesisBoundingRectangle.top()     // m32
            );

            float normalizedCenterLine = (
                  (currentParenthesisCenterLine - currentRightParenthesisBoundingRectangle.top())
                / currentRightParenthesisBoundingRectangle.height()
            );

            drawParenthesis(
                painter,
                transform,
                longToShort(currentRightParenthesisBoundingRectangle),
                normalizedCenterLine,
                currentRightParenthesisStyle
            );
        }

        if (currentBottomParenthesisStyle != ParenthesisStyle::INVALID &&
            currentBottomParenthesisStyle != ParenthesisStyle::NONE       ) {
            Q_ASSERT(false); // FIXME - not yet implemented
        }
    }


    float GraphicsMathGroup::longToShort(const QRectF& boundingRectangle) {
        float longSide  = std::max(boundingRectangle.height(), boundingRectangle.width());
        float shortSide = std::min(boundingRectangle.height(), boundingRectangle.width());

        return longSide / shortSide;
    }


    void GraphicsMathGroup::drawParenthesis(
            QPainter*         painter,
            const QTransform& transform,
            float             heightToWidth,
            float             centerLine,
            ParenthesisStyle  parenthesisStyle
        ) {
        painter->setPen(currentParenthesisPen);
        painter->setBrush(currentParenthesisBrush);

        switch (parenthesisStyle) {
            case ParenthesisStyle::INVALID: {
                Q_ASSERT(false);
                break;
            }

            case ParenthesisStyle::NONE: {
                Q_ASSERT(false);
                break;
            }

            case ParenthesisStyle::PARENTHESIS: {
                drawNormalizedCurvedParenthesis(painter, transform, heightToWidth);
                break;
            }

            case ParenthesisStyle::BRACKETS: {
                drawNormalizedBracket(painter, transform, heightToWidth);
                break;
            }

            case ParenthesisStyle::BRACES: {
                drawNormalizedBrace(painter, transform, heightToWidth, centerLine);
                break;
            }

            case ParenthesisStyle::FLOOR: {
                drawNormalizedFloor(painter, transform, heightToWidth);
                break;
            }

            case ParenthesisStyle::CEILING: {
                drawNormalizedCeiling(painter, transform, heightToWidth);
                break;
            }

            case ParenthesisStyle::ABSOLUTE_VALUE: {
                drawNormalizedAbsoluteValue(painter, transform, heightToWidth);
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }
    }


    void GraphicsMathGroup::drawNormalizedCurvedParenthesis(
            QPainter*         painter,
            const QTransform& transform,
            float             heightToWidth
        ) {
        QPainterPath painterPath(QPointF(1.0F, 0.0F));

        float widthToHeight       = 1.0F / heightToWidth;
        float halfWidthToHeight   = 0.5F * widthToHeight;
        float doubleWidthToHeight = 2.0F * widthToHeight;
        if (doubleWidthToHeight < 0.5) {
            painterPath.quadTo(QPointF(0.0F, halfWidthToHeight), QPointF(0.0F, doubleWidthToHeight));
            painterPath.lineTo(QPointF(0.0F, 1.0F - doubleWidthToHeight));
            painterPath.quadTo(QPointF(0.0F, 1.0 - halfWidthToHeight), QPointF(1.0F, 1.0F));
            painterPath.quadTo(QPointF(0.1F, 1.0 - halfWidthToHeight), QPointF(0.1F, 1.0F - doubleWidthToHeight));
            painterPath.lineTo(QPointF(0.1F, doubleWidthToHeight));
            painterPath.quadTo(QPointF(0.1F, halfWidthToHeight), QPointF(1.0F, 0.0F));
        } else {
            painterPath.quadTo(QPointF(0.0F, 0.25F), QPointF(0.0F, 0.5F));
            painterPath.quadTo(QPointF(0.0F, 0.75F), QPointF(1.0F, 1.0F));
            painterPath.quadTo(QPointF(0.1F, 0.75F), QPointF(0.1F, 0.5F));
            painterPath.quadTo(QPointF(0.1F, 0.25F), QPointF(1.0F, 0.0F));
        }

        painter->drawPath(transform.map(painterPath));
    }


    void GraphicsMathGroup::drawNormalizedBracket(
            QPainter*         painter,
            const QTransform& transform,
            float             heightToWidth
        ) {
        QPainterPath painterPath(QPointF(1.0F, 0.0F));

        float verticalWidth = 0.1 / heightToWidth;

        painterPath.lineTo(QPointF(0.0, 0.0));
        painterPath.lineTo(QPointF(0.0F, 1.0F));
        painterPath.lineTo(QPointF(1.0F, 1.0F));
        painterPath.lineTo(QPointF(1.0F, 1.0 - verticalWidth));
        painterPath.lineTo(QPointF(0.1F, 1.0 - verticalWidth));
        painterPath.lineTo(QPointF(0.1F, verticalWidth));
        painterPath.lineTo(QPointF(1.0F, verticalWidth));
        painterPath.lineTo(QPointF(1.0F, 0.0F));

        painter->drawPath(transform.map(painterPath));
    }


    void GraphicsMathGroup::drawNormalizedBrace(
            QPainter*         painter,
            const QTransform& transform,
            float             heightToWidth,
            float             centerLine
        ) {
        if (centerLine <= 0.0F) {
            centerLine = 0.5F;
        }

        QPainterPath painterPath(QPointF(1.0F, 0.0F));

        float widthToHeight = std::min(0.25F, 1.0F / heightToWidth);

        centerLine = std::min(1.0F - 2.0F * widthToHeight, std::max(2.0F * widthToHeight, centerLine));

        painterPath.quadTo(QPointF(0.45F, 0.0F), QPointF(0.45F, widthToHeight));
        painterPath.lineTo(QPointF(0.45F, centerLine - widthToHeight));
        painterPath.quadTo(QPointF(0.45F, centerLine), QPointF(0.0F, centerLine));
        painterPath.quadTo(QPointF(0.45F, centerLine), QPointF(0.45F, centerLine + widthToHeight));
        painterPath.lineTo(QPointF(0.45F, 1.0 - widthToHeight));
        painterPath.quadTo(QPointF(0.45F, 1.0F), QPointF(1.0F, 1.0F));

        painterPath.quadTo(QPointF(0.55F, 1.0F), QPointF(0.55F, 1.0 - widthToHeight));
        painterPath.lineTo(QPointF(0.55F, centerLine + widthToHeight));
        painterPath.quadTo(QPointF(0.55F, centerLine), QPointF(0.0F, centerLine));
        painterPath.quadTo(QPointF(0.55F, centerLine), QPointF(0.55F, centerLine - widthToHeight));
        painterPath.lineTo(QPointF(0.55F, widthToHeight));
        painterPath.quadTo(QPointF(0.55F, 0.0F), QPointF(1.0F, 0.0F));

        painter->drawPath(transform.map(painterPath));
    }


    void GraphicsMathGroup::drawNormalizedFloor(QPainter* painter, const QTransform& transform, float heightToWidth) {
        QPainterPath painterPath(QPointF(0.0F, 0.0F));

        float verticalWidth = 0.1 / heightToWidth;

        painterPath.lineTo(QPointF(0.0F, 1.0F));
        painterPath.lineTo(QPointF(1.0F, 1.0F));
        painterPath.lineTo(QPointF(1.0F, 1.0 - verticalWidth));
        painterPath.lineTo(QPointF(0.1F, 1.0 - verticalWidth));
        painterPath.lineTo(QPointF(0.1F, 0.0F));
        painterPath.lineTo(QPointF(0.0F, 0.0F));

        painter->drawPath(transform.map(painterPath));
    }


    void GraphicsMathGroup::drawNormalizedCeiling(
            QPainter*         painter,
            const QTransform& transform,
            float             heightToWidth
        ) {
        QPainterPath painterPath(QPointF(1.0F, 0.0F));

        float verticalWidth = 0.1 / heightToWidth;

        painterPath.lineTo(QPointF(0.0, 0.0));
        painterPath.lineTo(QPointF(0.0F, 1.0F));
        painterPath.lineTo(QPointF(0.1F, 1.0F));
        painterPath.lineTo(QPointF(0.1F, verticalWidth));
        painterPath.lineTo(QPointF(1.0F, verticalWidth));
        painterPath.lineTo(QPointF(1.1F, 0.0F));

        painter->drawPath(transform.map(painterPath));
    }


    void GraphicsMathGroup::drawNormalizedAbsoluteValue(
            QPainter*         painter,
            const QTransform& transform,
            float             /* heightToWidth */
        ) {
        QRectF rectangle(0.45F, 0.0F, 0.1F, 1.0F);
        painter->drawRect(transform.mapRect(rectangle));
    }
}
