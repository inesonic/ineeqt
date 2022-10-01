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
* This header defines the \ref EQt::GraphicsTextItem class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_TEXT_ITEM_H
#define EQT_GRAPHICS_TEXT_ITEM_H

#include <QGraphicsSimpleTextItem>
#include <QString>
#include <QBrush>
#include <QPen>

#include "eqt_common.h"
#include "eqt_graphics_item.h"

class QGraphicsItem;
class QFont;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

namespace EQt {
    /**
     * Class that extends QGraphicsSimpleTextItem to include support for background fill.
     */
    class EQT_PUBLIC_API GraphicsTextItem:public QGraphicsSimpleTextItem, public GraphicsItem {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 2 };

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            GraphicsTextItem(QGraphicsItem* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] text   Text to be rendered.
             *
             * \param[in] parent Pointer to the parent object.
             */
            GraphicsTextItem(const QString& text, QGraphicsItem* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] text   Text to be rendered.
             *
             * \param[in] font   Font to use to render the text.
             *
             * \param[in] parent Pointer to the parent object.
             */
            GraphicsTextItem(const QString& text, const QFont& font, QGraphicsItem* parent = Q_NULLPTR);

            ~GraphicsTextItem() override;

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

            /**
             * Method you can use to determine if the background brush is going to be used.
             *
             * \return Returns true if the provided brush will be used.  Returns false if no background is rendered.
             */
            bool isDrawingBackground() const;

            /**
             * Method you can use to determine if the background brush is not going to be used.
             *
             * \return Returns true if the provided brush will not be used.  Returns false if a background is rendered.
             */
            bool isNotDrawingBackground() const;

            /**
             * Method you can use to set the background brush to be used to fill behind the rendered text.
             *
             * \param[in] newBrush The new background brush.
             */
            void setBackgroundBrush(const QBrush& newBrush);

            /**
             * Method you can use to clear the background brush, disabling drawing a background.
             */
            void clearBackgroundBrush();

            /**
             * Method you can use to determine the background brush that will be used.
             *
             * \return Returns the background brush.
             */
            QBrush backgroundBrush() const;

            /**
             * Method you can use to set the border pen.
             *
             * \param[in] newPen The new border pen.
             */
            void setBorderPen(const QPen& newPen);

            /**
             * Method you can use to obtain the current border pen.
             *
             * \return Returns the current border pen.
             */
            QPen borderPen() const;

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
             * Flag that indicates if a background is to be drawn.
             */
            bool drawBackground;

            /**
             * The brush to use to draw the background.
             */
            QBrush currentBackgroundBrush;

            /**
             * The pen used to draw an optional border.
             */
            QPen currentBorderPen;
    };
}

#endif
