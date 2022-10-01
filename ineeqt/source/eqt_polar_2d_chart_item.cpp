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
* This file implements the \ref EQt::Polar2DChartItem class.
***********************************************************************************************************************/

#include <QGraphicsScene>
#include <QPolarChart>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include "eqt_graphics_item.h"
#include "eqt_polar_2d_chart_item.h"

namespace EQt {
    Polar2DChartItem::Polar2DChartItem(
            QGraphicsItem*  parent,
            Qt::WindowFlags windowFlags
        ):QPolarChart(
            parent,
            windowFlags
        ) {}


    Polar2DChartItem::~Polar2DChartItem() {}


    void Polar2DChartItem::setGeometry(const QRectF& rectangle) {
        currentEnforcedBoundingRectangle = rectangle;
        QPolarChart::setGeometry(rectangle);
    }


    QRectF Polar2DChartItem::boundingRect() const {
        return currentEnforcedBoundingRectangle;
    }


    int Polar2DChartItem::type() const {
        return Type;
    }


    void Polar2DChartItem::removeFromScene() {
        prepareGeometryChange();
        removeGraphicsItemFromScene(this);
    }
}
