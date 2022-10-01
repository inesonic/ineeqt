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
* This header defines the \ref EQt::GraphicsItemGroup class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_ITEM_GROUP_H
#define EQT_GRAPHICS_ITEM_GROUP_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QRectF>

#include "eqt_common.h"
#include "eqt_graphics_item.h"

class QWidget;
class QAction;
class QFocusEvent;

namespace EQt {
    /**
     * Class that extends QGraphicsItemGroup to change the child ownership rules.  Normally the QGraphicsItemGroup
     * owns the child objects.  This complicates code that requires ownership to be maintained by external objects.
     *
     * This class also includes the ability to force the bounding rectangle for the group.  If not forced, the bounding
     * rectangle will be determined based on the supplied child items.
     */
    class EQT_PUBLIC_API GraphicsItemGroup:public QGraphicsItemGroup, public GraphicsItem {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 1 };

            GraphicsItemGroup();

            ~GraphicsItemGroup() override;

            /**
             * Method you can use to set the geometry for the widget.
             *
             * \param[in] rectangle The new bounding rectangle for the plot.
             */
            void setForcedGeometry(const QRectF& rectangle);

            /**
             * Convenience method you can use to set the geometry for the widget.
             *
             * \param[in] x      The desired chart top edge.
             *
             * \param[in] y      The desired chart left edge.
             *
             * \param[in] width  The desired chart width.
             *
             * \param[in] height The desired chart height.
             */
            inline void setForcedGeometry(double x, double y, double width, double height) {
                setForcedGeometry(QRectF(x, y, width, height));
            }

            /**
             * Method you can call to clear the forced geometry for this group.
             */
            void clearForcedGeometry();

            /**
             * Method you can use to determine if the current bounding rectangle is being forced.
             *
             * \return Returns true if the current bounding rectangle is being forced.  Returns false if the current
             *         bounding rectangle is not being forced.
             */
            bool hasForcedBoundingRectangle() const;

            /**
             * Method you can use to obtain the forced bounding rectangle.
             *
             * \return Returns the current forced bounding rectangle.  An invalid bounding rectangle is returned if
             *         the bounding rectangle is not being forced.
             */
            const QRectF& forcedGeometry() const;

            /**
             * Method you can use to obtain the bounding rectangle for the widget.
             *
             * \return Returns the plot's bounding rectangle.
             */
            QRectF boundingRect() const override;

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

        protected:
            /**
             * Method you should overload to indicate if this graphics item represents a group and requires group
             * handling when deleted.
             *
             * \return Returns true if this graphics item is a group.  Returns false if this graphics item is not a
             *         group.  This version returns true.
             */
            bool isGroup() const final;

        private:
            /**
             * The current forced geometry.
             */
            QRectF currentForcedGeometry;
    };
}

#endif
