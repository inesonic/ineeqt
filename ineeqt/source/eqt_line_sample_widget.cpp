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
* This file implements the \ref EQt::LineSampleWidget class.
***********************************************************************************************************************/

#include <QWidget>
#include <QSizePolicy>
#include <QSize>
#include <QColor>
#include <QPalette>
#include <QSizePolicy>
#include <QPaintDevice>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPointF>

#include "eqt_line_sample_widget.h"

namespace EQt {
    LineSampleWidget::LineSampleWidget(QWidget* parent):QWidget(parent) {
        currentOrientation     = Orientation::HORIZONTAL;
        currentNumberLines     = 1;
        currentLineWidth       = 1.0F;

        setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    }


    LineSampleWidget::~LineSampleWidget() {}


    LineSampleWidget::Orientation LineSampleWidget::orientation() const {
        return currentOrientation;
    }


    unsigned LineSampleWidget::numberLines() const {
        return currentNumberLines;
    }


    float LineSampleWidget::lineWidth() const {
        return currentLineWidth;
    }


    QColor LineSampleWidget::lineColor() const {
        return palette().color(foregroundRole());
    }


    QColor LineSampleWidget::backgroundColor() const {
        return palette().color(backgroundRole());
    }


    void LineSampleWidget::setSize(unsigned width, unsigned height) {
        setSize(QSize(width, height));
    }


    void LineSampleWidget::setSize(const QSize& newSize) {
        setFixedSize(newSize);
        update();
    }


    void LineSampleWidget::setOrientation(Orientation newOrientation) {
        currentOrientation = newOrientation;
        update();
    }


    void LineSampleWidget::setNumberLines(unsigned newNumberLines) {
        currentNumberLines = newNumberLines;
        update();
    }


    void LineSampleWidget::setLineWidth(float newLineWidth) {
        currentLineWidth = newLineWidth;
        update();
    }


    void LineSampleWidget::setLineColor(const QColor& newLineColor) {
        QPalette palette = LineSampleWidget::palette();
        palette.setColor(foregroundRole(), newLineColor);
        setPalette(palette);

        update();
    }


    void LineSampleWidget::setBackgroundColor(const QColor& newBackgroundColor) {
        QPalette palette = LineSampleWidget::palette();
        palette.setColor(backgroundRole(), newBackgroundColor);
        setPalette(palette);

        update();
    }


    void LineSampleWidget::paintEvent(QPaintEvent*) {
        if (currentNumberLines > 0) {
            QSize    widgetSize = LineSampleWidget::size();
            QColor   lineColor  = palette().color(foregroundRole());

            QPen     pen(lineColor);
            pen.setWidthF(currentLineWidth);

            QPainter painter(this);
            painter.setPen(pen);

            if (currentOrientation == Orientation::HORIZONTAL) {
                unsigned height   = widgetSize.height();
                unsigned width    = widgetSize.width();
                float    midPoint = height / 2.0F;
                float    y        = midPoint - currentNumberLines * currentLineWidth;

                for (unsigned lineIndex=0 ; lineIndex<currentNumberLines ; ++lineIndex) {
                    painter.drawLine(QPointF(0, y), QPointF(width, y));
                    y += 2 * currentLineWidth;
                }
            } else {
                Q_ASSERT(currentOrientation == Orientation::VERTICAL);

                unsigned height   = widgetSize.height();
                unsigned width    = widgetSize.width();
                float    midPoint = width / 2.0F;
                float    x        = midPoint - currentNumberLines * currentLineWidth;

                for (unsigned lineIndex=0 ; lineIndex<currentNumberLines ; ++lineIndex) {
                    painter.drawLine(QPointF(x, 0), QPointF(x, height));
                    x += 2 * currentLineWidth;
                }
            }
        }
    }
}
