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
* This header defines the \ref EQt::GraphicsRectItem class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_RECT_ITEM_H
#define EQT_GRAPHICS_RECT_ITEM_H

#include <QGraphicsRectItem>
#include <QRectF>

#include "eqt_common.h"
#include "eqt_graphics_item.h"

class QGraphicsItem;
class QFont;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

namespace EQt {
    /**
     * Class that extends QGraphicsRectItem to modify how the parent/child relationship is managed.
     */
    class EQT_PUBLIC_API GraphicsRectItem:public QGraphicsRectItem, public GraphicsItem {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 3 };

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            GraphicsRectItem(QGraphicsItem* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] rectangle The rectangle defining the size and position of this rectangle.
             *
             * \param[in] parent    Pointer to the parent object.
             */
            GraphicsRectItem(const QRectF& rectangle, QGraphicsItem* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] x      The rectangle X offset.
             *
             * \param[in] y      The rectangle Y offset.
             *
             * \param[in] width  The rectangle width.
             *
             * \param[in] height The rectangle height.
             *
             * \param[in] parent Pointer to the parent object.
             */
            GraphicsRectItem(double x, double y, double width, double height, QGraphicsItem* parent = Q_NULLPTR);

            ~GraphicsRectItem() override;

            /**
             * Method that returns the type ID of this QGraphicsItem.
             *
             * \return Returns the type ID of this QGraphicsItem.
             */
            int type() const override;

            /**
             * Method you can call to remove this graphics item from the scene.  You should overload this method in
             * derived classes to remove the graphics item from the scene.
             */
            void removeFromScene() override;
   };
}

#endif
