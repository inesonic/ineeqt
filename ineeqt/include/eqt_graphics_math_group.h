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
* This header defines the \ref EQt::GraphicsMathGroup class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_MATH_GROUP_H
#define EQT_GRAPHICS_MATH_GROUP_H

#include <QString>
#include <QBrush>
#include <QPen>
#include <QRectF>

#include "eqt_common.h"
#include <eqt_graphics_multi_text_group.h>

class QWidget;
class QAction;
class QFocusEvent;

namespace EQt {
    /**
     * Class that extends the EQt:GraphicsMultiTextGroup to include support for rendering surrounding parenthesis.
     * This class exists to facilitate more rational handling of large parenthesis.
     */
    class EQT_PUBLIC_API GraphicsMathGroup:public GraphicsMultiTextGroup {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 6 };

            /**
             * Enumeration of different parenthesis styles.
             */
            enum class ParenthesisStyle: quint8 {
                /**
                 * Indicates an invalid parenthesis style.
                 */
                INVALID = 0,

                /**
                 * Indicates no parenthesis.
                 */
                NONE = 1,

                /**
                 * Indicates traditional parenthesis.
                 */
                PARENTHESIS = 2,

                /**
                 * Indicates bracket parenthesis.
                 */
                BRACKETS = 3,

                /**
                 * Indicates brace parenthesis.
                 */
                BRACES = 4,

                /**
                 * Indicates floor parenthesis.
                 */
                FLOOR = 5,

                /**
                 * Indicates ceiling parenthesis.
                 */
                CEILING = 6,

                /**
                 * Indicates absolute value parenthesis.
                 */
                ABSOLUTE_VALUE = 7
            };

            GraphicsMathGroup();

            ~GraphicsMathGroup() override;

            /**
             * Method that returns the type ID of this QGraphicsItem.
             *
             * \return Returns the type ID of this QGraphicsItem.
             */
            int type() const override;

            /**
             * Method you can use to set the left parenthesis style.
             *
             * \param[in] newParenthesisStyle The new left parenthesis style.
             */
            void setLeftParenthesisStyle(ParenthesisStyle newParenthesisStyle);

            /**
             * Method you can use to obtain the current left parenthesis style.
             *
             * \return Returns the current left parenthesis style.
             */
            ParenthesisStyle leftParenthesisStyle() const;

            /**
             * Method you can use to set the left parenthesis bounding rectangle, The specified parenthesis will be
             * scaled to exactly fit within this bounding rectangle.
             *
             * \param[in] parenthesisBoundingRectangle The bounding rectangle for the left parenthesis.
             */
            void setLeftParenthesisBoundingRectangle(const QRectF& parenthesisBoundingRectangle);

            /**
             * Method you can use to obtain the left parenthesis bounding rectangle.  The specified parenthesis will be
             * scaled to exactly fit within this bounding rectangle.
             *
             * \return Returns the left parenthesis bounding rectangle.
             */
            QRectF leftParenthesisBoundingRectangle() const;

            /**
             * Convenience method you can use to set the left parenthesis parameters.
             *
             * \param[in] newParenthesisStyle          The new left parenthesis style.
             *
             * \param[in] parenthesisBoundingRectangle The bounding rectangle for the left parenthesis.
             */
            void setLeftParenthesis(ParenthesisStyle newParenthesisStyle, const QRectF& parenthesisBoundingRectangle);

            /**
             * Method you can use to set the right parenthesis style.
             *
             * \param[in] newParenthesisStyle The new right parenthesis style.
             */
            void setRightParenthesisStyle(ParenthesisStyle newParenthesisStyle);

            /**
             * Method you can use to obtain the current right parenthesis style.
             *
             * \return Returns the current right parenthesis style.
             */
            ParenthesisStyle rightParenthesisStyle() const;

            /**
             * Method you can use to set the right parenthesis bounding rectangle, The specified parenthesis will be
             * scaled to exactly fit within this bounding rectangle.
             *
             * \param[in] parenthesisBoundingRectangle The bounding rectangle for the right parenthesis.
             */
            void setRightParenthesisBoundingRectangle(const QRectF& parenthesisBoundingRectangle);

            /**
             * Method you can use to obtain the right parenthesis bounding rectangle.  The specified parenthesis will
             * be scaled to exactly fit within this bounding rectangle.
             *
             * \return Returns the right parenthesis bounding rectangle.
             */
            QRectF rightParenthesisBoundingRectangle() const;

            /**
             * Convenience method you can use to set the right parenthesis parameters.
             *
             * \param[in] newParenthesisStyle          The new right parenthesis style.
             *
             * \param[in] parenthesisBoundingRectangle The bounding rectangle for the right parenthesis.
             */
            void setRightParenthesis(ParenthesisStyle newParenthesisStyle, const QRectF& parenthesisBoundingRectangle);

            /**
             * Method you can use to specify the "center-line" for the parenthesis.  The value will be used to place
             * the point of a parenthesis brace.  The value is not used for the bottom parenthesis.
             *
             * \param[in] newCenterLine The new center line for the parenthesis.  A zero or negative value will cause
             *                          the point to be centered.
             */
            void setParenthesisCenterLine(float newCenterLine);

            /**
             * Method you can use to obtain the "center-line" for the parenthesis.  The value is used to place the
             * point of a parenthesis brace.  The value is not used for the bottom parenthesis.
             *
             * \return Returns the parenthesis center line.  A zero or negative value indicates that the point will be
             *         centered,
             */
            float parenthesisCenterLine() const;

            /**
             * Method you can use to set the bottom parenthesis style.
             *
             * \param[in] newParenthesisStyle The new bottom parenthesis style.
             */
            void setBottomParenthesisStyle(ParenthesisStyle newParenthesisStyle);

            /**
             * Method you can use to obtain the current bottom parenthesis style.
             *
             * \return Returns the current bottom parenthesis style.
             */
            ParenthesisStyle bottomParenthesisStyle() const;

            /**
             * Method you can use to set the bottom parenthesis bounding rectangle, The specified parenthesis will be
             * scaled to exactly fit within this bounding rectangle.
             *
             * \param[in] parenthesisBoundingRectangle The bounding rectangle for the bottom parenthesis.
             */
            void setBottomParenthesisBoundingRectangle(const QRectF& parenthesisBoundingRectangle);

            /**
             * Method you can use to obtain the bottom parenthesis bounding rectangle.  The specified parenthesis will
             * be scaled to exactly fit within this bounding rectangle.
             *
             * \return Returns the bottom parenthesis bounding rectangle.
             */
            QRectF bottomParenthesisBoundingRectangle() const;

            /**
             * Convenience method you can use to set the bottom parenthesis parameters.
             *
             * \param[in] newParenthesisStyle          The new bottom parenthesis style.
             *
             * \param[in] parenthesisBoundingRectangle The bounding rectangle for the bottom parenthesis.
             */
            void setBottomParenthesis(
                ParenthesisStyle newParenthesisStyle,
                const QRectF&    parenthesisBoundingRectangle
            );

            /**
             * Method you can use to set the pen used to draw parenthesis.
             *
             * \param[in] newPen The new pen used to draw parenthesis.
             */
            void setParenthesisPen(const QPen& newPen);

            /**
             * Method you can use to obtain the pen used to draw parenthesis.
             *
             * \return Returns the pen currently being used to draw parenthesis.
             */
            QPen parenthesisPen() const;

            /**
             * Method you can use to set the brush used to draw parenthesis.
             *
             * \param[in] newBrush The new brush used to draw parenthesis.
             */
            void setParenthesisBrush(const QBrush& newBrush);

            /**
             * Method you can use to obtain the brush used to draw parenthesis.
             *
             * \return Returns the brush currently being used to draw parenthesis.
             */
            QBrush parenthesisBrush() const;

            /**
             * Method that returns this item's bounding rectangle.
             *
             * \return Returns this item's bounding rectangle.
             */
            QRectF boundingRect() const override;

            /**
             * Method that is called to paint the object.
             *
             * \param[in] painter Painter used to draw the text.
             *
             * \param[in] option  Options that can be used to impact how the text is drawn.  This parameter is ignored.
             *
             * \param[in] widget  Widget to be drawn on.
             */
            void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

        private:
            /**
             * Method that calculates the height to width of a bounding rectangle.
             *
             * \param[in] boundingRectangle The bounding rectangle to calculate the height to width for.
             *
             * \return Returns the height to width.
             */
            static float longToShort(const QRectF& boundingRectangle);

            /**
             * Method that is called to draw a normalized parenthesis of a given style.
             *
             * \param[in] painter          Painter used to draw the parenthesis.  This method expects a transform to be
             *                             applied applied to the painter to correct position, scale, and rotate the
             *                             parenthesis.  The transformation should assume that the default orientation
             *                             is a normalized left side parenthesis.
             *
             * \param[in] transform        The transform used to scale and position the parenthesis.
             *
             * \param[in] heightToWidth    The ratio of the long side to the short side of the parenthesis.
             *
             * \param[in] centerLine       The normalized center-line position.
             *
             * \param[in] parenthesisStyle The parenthesis style to be drawn.
             */
            void drawParenthesis(
                QPainter*         painter,
                const QTransform& transform,
                float             heightToWidth,
                float             centerLine,
                ParenthesisStyle  parenthesisStyle
            );

            /**
             * Method that is called to draw a normalized curved parenthesis.
             *
             * \param[in] painter       Painter used to draw the parenthesis.  This method expects a transform to be
             *                          applied to the painter to correct position, scale, and rotate the parenthesis.
             *                          The transformation should assume that the default orientation is a normalized
             *                          left side parenthesis.
             *
             * \param[in] transform     The transform used to scale and position the parenthesis.
             *
             * \param[in] heightToWidth The ratio of the long side to the short side of the parenthesis.
             */
            void drawNormalizedCurvedParenthesis(QPainter* painter, const QTransform& transform, float heightToWidth);

            /**
             * Method that is called to draw a normalized bracket.
             *
             * \param[in] painter       Painter used to draw the bracket.  This method expects a transform to be
             *                          applied to the painter to correct position, scale, and rotate the bracket.  The
             *                          transformation should assume that the default orientation is a normalized left
             *                          side bracket.
             *
             * \param[in] transform     The transform used to scale and position the parenthesis.
             *
             * \param[in] heightToWidth The ratio of the long side to the short side of the parenthesis.
             */
            void drawNormalizedBracket(QPainter* painter, const QTransform& transform, float heightToWidth);

            /**
             * Method that is called to draw a normalized braces.
             *
             * \param[in] painter       Painter used to draw the brace.  This method expects a transform to be applied
             *                          to the painter to correct position, scale, and rotate the brace.  The
             *                          transformation should assume that the default orientation is a normalized left
             *                          side brace.
             *
             * \param[in] transform     The transform used to scale and position the parenthesis.
             *
             * \param[in] heightToWidth The ratio of the long side to the short side of the parenthesis.
             *
             * \param[in] centerLine    The normalized center-line position.
             */
            void drawNormalizedBrace(
                QPainter*         painter,
                const QTransform& transform,
                float             heightToWidth,
                float             centerLine
            );

            /**
             * Method that is called to draw a normalized floor symbol.
             *
             * \param[in] painter       Painter used to draw the floor symbol.  This method expects a transform to be
             *                          applied to the painter to correct position, scale, and rotate the floor symbol.
             *                          the transformation should assume that the default orientation is a normalized
             *                          left side floor symbol.
             *
             * \param[in] transform     The transform used to scale and position the parenthesis.
             *
             * \param[in] heightToWidth The ratio of the long side to the short side of the parenthesis.
             */
            void drawNormalizedFloor(QPainter* painter, const QTransform& transform, float heightToWidth);

            /**
             * Method that is called to draw a normalized ceiling symbol.
             *
             * \param[in] painter       Painter used to draw the ceiling symbol.  This method expects a transform to be
             *                          applied to the painter to correct position, scale, and rotate the ceiling
             *                          symbol.  The transformation should assume that the default orientation is a
             *                          normalized left side ceiling symbol.
             *
             * \param[in] transform     The transform used to scale and position the parenthesis.
             *
             * \param[in] heightToWidth The ratio of the long side to the short side of the parenthesis.
             */
            void drawNormalizedCeiling(QPainter* painter, const QTransform& transform, float heightToWidth);

            /**
             * Method that is called to draw a normalized absolute value symbol.
             *
             * \param[in] painter       Painter used to draw the absolute value symbol.  This method expects a
             *                          transform to be applied to the painter to correct position, scale, and rotate
             *                          the absolute value symbol.  The transformation should assume that the default
             *                          orientation is a normalized absolute value symbol.
             *
             * \param[in] transform     The transform used to scale and position the parenthesis.
             *
             * \param[in] heightToWidth The ratio of the long side to the short side of the parenthesis.
             */
            void drawNormalizedAbsoluteValue(QPainter* painter, const QTransform& transform, float heightToWidth);

            /**
             * The current parenthesis pen
             */
            QPen currentParenthesisPen;

            /**
             * The current parenthesis brush
             */
            QBrush currentParenthesisBrush;

            /**
             * The current parenthesis center line.
             */
            float currentParenthesisCenterLine;

            /**
             * The left parenthesis style.
             */
            ParenthesisStyle currentLeftParenthesisStyle;

            /**
             * The left parenthesis bounding rectangle.
             */
            QRectF currentLeftParenthesisBoundingRectangle;

            /**
             * The right parenthesis style.
             */
            ParenthesisStyle currentRightParenthesisStyle;

            /**
             * The right parenthesis bounding rectangle.
             */
            QRectF currentRightParenthesisBoundingRectangle;

            /**
             * The bottom parenthesis style.
             */
            ParenthesisStyle currentBottomParenthesisStyle;

            /**
             * The bottom parenthesis bounding rectangle.
             */
            QRectF currentBottomParenthesisBoundingRectangle;
    };
}

#endif
