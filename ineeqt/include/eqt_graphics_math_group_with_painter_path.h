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
* This header defines the \ref EQt::GraphicsMathGroupWithPainterPath class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_MATH_GROUP_WITH_PAINTER_PATH_H
#define EQT_GRAPHICS_MATH_GROUP_WITH_PAINTER_PATH_H

#include <QString>
#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QPainterPath>

#include "eqt_common.h"
#include <eqt_graphics_math_group.h>

class QWidget;
class QAction;
class QFocusEvent;

namespace EQt {
    /**
     * Class that extends the EQt:GraphicsMathGRoup to include support for rendering a single painter path.  You can
     * this class to render complex mathematical constructs such as roots and integrals that can't be readily rendered
     * using the available fonts.
     */
    class EQT_PUBLIC_API GraphicsMathGroupWithPainterPath:public GraphicsMathGroup {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 9 };

            GraphicsMathGroupWithPainterPath();

            ~GraphicsMathGroupWithPainterPath() override;

            /**
             * Method that returns the type ID of this QGraphicsItem.
             *
             * \return Returns the type ID of this QGraphicsItem.
             */
            int type() const override;

            /**
             * Method you can use to set the new painter path for subsequent rendering operations.
             *
             * \param[in] newPainterPath The new painter path to be rendered.
             */
            void setPainterPath(const QPainterPath& newPainterPath);

            /**
             * Method you can use to obtain the painter path that this graphics item is rendering.
             *
             * \return Returns the painter path used to render the item.
             */
            const QPainterPath& painterPath() const;

            /**
             * Method you can use to set the pen used for rendering the painter path.
             *
             * \param[in] newPen The new painter path pen.
             */
            void setPainterPathPen(const QPen& newPen);

            /**
             * Method you can use to obtain the pen being used to render the painter path.
             *
             * \return Returns the pen being used to render the painter path.
             */
            const QPen& painterPathPen() const;

            /**
             * Method you can use to set the fill brush used to fill the painter path.
             *
             * \param[in] newBrush The new brush used to render the painter path.
             */
            void setPainterPathBrush(const QBrush& newBrush);

            /**
             * Method you can use to obtain the painter path brush currently being used to render the painter path.
             *
             * \return Returns the brush currently being used to render the painter path.
             */
            const QBrush& painterPathBrush() const;

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
             * The current painter path.
             */
            QPainterPath currentPainterPath;

            /**
             * The current painter path pen.
             */
            QPen currentPainterPathPen;

            /**
             * The current painter path brush.
             */
            QBrush currentPainterPathBrush;
    };
}

#endif
