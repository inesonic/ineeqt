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
* This header defines the \ref EQt::GraphicsMathGroupWithLine class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_MATH_GROUP_WITH_LINE_H
#define EQT_GRAPHICS_MATH_GROUP_WITH_LINE_H

#include <QString>
#include <QBrush>
#include <QPen>
#include <QPointF>
#include <QLineF>
#include <QRectF>

#include "eqt_common.h"
#include <eqt_graphics_math_group.h>

class QWidget;
class QAction;
class QFocusEvent;

namespace EQt {
    /**
     * Class that extends the EQt:GraphicsMathGroup class to support the addition of a single line.  You can use this
     * class to draw overlines and fraction operators.
     */
    class EQT_PUBLIC_API GraphicsMathGroupWithLine:public GraphicsMathGroup {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 8 };

            GraphicsMathGroupWithLine();

            ~GraphicsMathGroupWithLine() override;

            /**
             * Method that returns the type ID of this QGraphicsItem.
             *
             * \return Returns the type ID of this QGraphicsItem.
             */
            int type() const override;

            /**
             * Method you can use to set the line position.
             *
             * \param[in] newLine The new line coordinates.
             */
            void setLine(const QLineF& newLine);

            /**
             * Method you can use to set the line position.
             *
             * \param[in] p1 The new first point for the line.
             *
             * \param[in] p2 The new second point for the line.
             */
            void setLine(const QPointF& p1, const QPointF& p2);

            /**
             * Method you can use to set the line position.
             *
             * \param[in] x1 The X coordinate of the first point.
             *
             * \param[in] y1 The Y coordinate of the first point.
             *
             * \param[in] x2 The X coordinate of the second point.
             *
             * \param[in] y2 The Y coordinate of the second point.
             */
            void setLine(float x1, float x2, float y1, float y2);

            /**
             * Method you can use to set the first point for the list.
             *
             * \param[in] newPosition The new position for the first line point.
             */
            void setP1(const QPointF& newPosition);

            /**
             * Method you can use to set the first point for the list.
             *
             * \param[in] x The X coordinate of the first point.
             *
             * \param[in] y The Y coordinate of the first point.
             */
            void setP1(float x, float y);

            /**
             * Method you can use to set the second point for the list.
             *
             * \param[in] newPosition The new position for the second line point.
             */
            void setP2(const QPointF& newPosition);

            /**
             * Method you can use to set the second point for the list.
             *
             * \param[in] x The X coordinate of the second point.
             *
             * \param[in] y The Y coordinate of the second point.
             */
            void setP2(float x, float y);

            /**
             * Method you can use to query the current line position.
             *
             * \return Returns the current line.
             */
            QLineF line() const;

            /**
             * Method you can use to query the first point position.
             *
             * \return Returns the current first point position.
             */
            QPointF p1() const;

            /**
             * Method you can use to query the second point position.
             *
             * \return Returns the current second point position.
             */
            QPointF p2() const;

            /**
             * Method you can use to query the first point X coordinate.
             *
             * \return Returns the X coordinate of the first point.
             */
            float x1() const;

            /**
             * Method you can use to query the first point Y coordinate.
             *
             * \return Returns the Y coordinate of the first point.
             */
            float y1() const;

            /**
             * Method you can use to query the second point X coordinate.
             *
             * \return Returns the X coordinate of the second point.
             */
            float x2() const;

            /**
             * Method you can use to query the second point Y coordinate.
             *
             * \return Returns the Y coordinate of the second point.
             */
            float y2() const;

            /**
             * Method you can use to set the pen used to draw the line.
             *
             * \param[in] pen The new pen to use.
             */
            void setLinePen(const QPen& pen);

            /**
             * Method you can use to obtain the pen to be used to draw the line.
             *
             * \return Returns the pen used to draw the line.
             */
            QPen linePen() const;

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
             * The current line pen
             */
            QPen currentLinePen;

            /**
             * The line position.
             */
            QLineF currentLine;
    };
}

#endif
