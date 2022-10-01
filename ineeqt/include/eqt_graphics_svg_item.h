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
* This header defines the \ref EQt::GraphicsSvgItem class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_SVG_ITEM_H
#define EQT_GRAPHICS_SVG_ITEM_H

#include <QGraphicsSvgItem>
#include <QPixmap>

#include "eqt_common.h"
#include "eqt_graphics_item.h"

class QGraphicsItem;
class QFont;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

namespace EQt {
    /**
     * Trivlal class that extends QGraphicsSvgItem to bring it into the EQt framework.
     */
    class EQT_PUBLIC_API GraphicsSvgItem:public QGraphicsSvgItem, public GraphicsItem {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 5 };

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            GraphicsSvgItem(QGraphicsItem* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] filename The filename of the SVG file to be rendered.
             *
             * \param[in] parent   Pointer to the parent object.
             */
            GraphicsSvgItem(const QString& filename, QGraphicsItem* parent = Q_NULLPTR);

            ~GraphicsSvgItem() override;

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
