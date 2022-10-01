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
* This header defines the \ref EQt::ChartItem class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_CHART_ITEM_H
#define EQT_CHART_ITEM_H

#include <QString>
#include <QBrush>
#include <QPen>

#include "eqt_charts.h"
#include "eqt_common.h"
#include "eqt_graphics_item.h"

class QGraphicsItem;
class QFont;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

namespace EQt {
    /**
     * Class that extends QChart class (in the QtCharts frameworks) to address ownership management issues as well as
     * more cleanly handle insertion and removal from the graphics scene.
     */
    class EQT_PUBLIC_API ChartItem:public QChart, public GraphicsItem {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 10 };

            /**
             * Constructor
             *
             * \param[in] parent      Pointer to the parent object.
             *
             * \param[in] windowFlags Windows flags to apply to the underlying QChart type.
             */
            ChartItem(QGraphicsItem* parent = Q_NULLPTR, Qt::WindowFlags windowFlags = Qt::WindowFlags());

            ~ChartItem() override;

            /**
             * Method you can use to set the geometry for the widget.
             *
             * \param[in] rectangle The new bounding rectangle for the plot.
             */
            void setGeometry(const QRectF& rectangle) override;

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
            inline void setGeometry(double x, double y, double width, double height) {
                setGeometry(QRectF(x, y, width, height));
            }

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

        private:
            /**
             * The enforced bounding rectangle for the plot.
             */
            QRectF currentEnforcedBoundingRectangle;
    };
}

#endif
