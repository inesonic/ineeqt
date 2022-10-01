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
* This file implements the \ref EQt::ColorToolButton class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QList>
#include <QSize>
#include <QMenu>
#include <QToolButton>
#include <QPixmap>
#include <QIcon>
#include <QPainter>
#include <QPen>
#include <QBrush>

#include <util_hash_functions.h>

#include <algorithm>
#include <cmath>

#include "eqt_tool_button.h"
#include "eqt_color_tool_button.h"

namespace EQt {
    ColorToolButton::ColorToolButton(QWidget* parent):ToolButton(parent) {
        updateIcon();
    }


    ColorToolButton::ColorToolButton(QAction* action, QWidget* parent):ToolButton(action, true, parent) {
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            int      newRelativeSizeFactor,
            QWidget* parent
        ):ToolButton(
            newRelativeSizeFactor,
            true,
            parent
        ) {
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            int      newRelativeSizeFactor,
            QAction* action,
            QWidget* parent
        ):ToolButton(
            newRelativeSizeFactor,
            action,
            true,
            parent
        ) {
        updateIcon();
    }


    ColorToolButton::ColorToolButton(QMenu* menu, QWidget* parent):ToolButton(menu, true, parent) {
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            QAction* action,
            QMenu*   menu,
            QWidget* parent
        ):ToolButton(
            action,
            menu,
            true,
            parent
        ) {
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            int      newRelativeSizeFactor,
            QMenu*   menu,
            QWidget* parent
        ):ToolButton(
            newRelativeSizeFactor,
            menu,
            true,
            parent
        ) {
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            int      newRelativeSizeFactor,
            QAction* action,
            QMenu*   menu,
            QWidget* parent
        ):ToolButton(
            newRelativeSizeFactor,
            action,
            menu,
            true,
            parent
        ) {
        updateIcon();
    }


    ColorToolButton::ColorToolButton(const QSet<QColor>& colors, QWidget* parent):ToolButton(true, parent) {
        currentColors = colors;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QSet<QColor>& colors,
            QAction*            action,
            QWidget*            parent
        ):ToolButton(
            action,
            true,
            parent
        ) {
        currentColors = colors;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QSet<QColor>& colors,
            int                 newRelativeSizeFactor,
            QWidget*            parent
        ):ToolButton(
            newRelativeSizeFactor,
            true,
            parent
        ) {
        currentColors = colors;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QSet<QColor>& colors,
            int                 newRelativeSizeFactor,
            QAction*            action,
            QWidget*            parent
        ):ToolButton(
            newRelativeSizeFactor,
            action,
            true,
            parent
        ) {
        currentColors = colors;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QSet<QColor>& colors,
            QMenu*              menu,
            QWidget*            parent
        ):ToolButton(
            menu,
            true,
            parent
        ) {
        currentColors = colors;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QSet<QColor>& colors,
            QAction*            action,
            QMenu*              menu,
            QWidget*            parent
        ):ToolButton(
            action,
            menu,
            true,
            parent
        ) {
        currentColors = colors;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QSet<QColor>& colors,
            int                 newRelativeSizeFactor,
            QMenu*              menu,
            QWidget*            parent
        ):ToolButton(
            newRelativeSizeFactor,
            menu,
            true,
            parent
        ) {
        currentColors = colors;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QSet<QColor>& colors,
            int                 newRelativeSizeFactor,
            QAction*            action,
            QMenu*              menu,
            QWidget*            parent
        ):ToolButton(
            newRelativeSizeFactor,
            action,
            menu,
            true,
            parent
        ) {
        currentColors = colors;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(const QColor& color, QWidget* parent):ToolButton(parent) {
        currentColors << color;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QColor& color,
            QAction*      action,
            QWidget*      parent
        ):ToolButton(
            action,
            true,
            parent
        ) {
        currentColors << color;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QColor& color,
            int           newRelativeSizeFactor,
            QWidget*      parent
        ):ToolButton(
            newRelativeSizeFactor,
            true,
            parent
        ) {
        currentColors << color;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QColor& color,
            int           newRelativeSizeFactor,
            QAction*      action,
            QWidget*      parent
        ):ToolButton(
            newRelativeSizeFactor,
            action,
            true,
            parent
        ) {
        currentColors << color;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QColor& color,
            QMenu*        menu,
            QWidget*      parent
        ):ToolButton(
            menu,
            true,
            parent
        ) {
        currentColors << color;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QColor& color,
            QAction*      action,
            QMenu*        menu,
            QWidget*      parent
        ):ToolButton(
            action,
            menu,
            true,
            parent
        ) {
        currentColors << color;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QColor& color,
            int           newRelativeSizeFactor,
            QMenu*        menu,
            QWidget*      parent
        ):ToolButton(
            newRelativeSizeFactor,
            menu,
            true,
            parent
        ) {
        currentColors << color;
        updateIcon();
    }


    ColorToolButton::ColorToolButton(
            const QColor& color,
            int           newRelativeSizeFactor,
            QAction*      action,
            QMenu*        menu,
            QWidget*      parent
        ):ToolButton(
            newRelativeSizeFactor,
            action,
            menu,
            true,
            parent
        ) {
        currentColors << color;
        updateIcon();
    }


    ColorToolButton::~ColorToolButton() {}


    const QSet<QColor>& ColorToolButton::colors() const {
        return currentColors;
    }


    void ColorToolButton::setColor(const QColor& newColor) {
        currentColors.clear();
        currentColors << newColor;
        updateIcon();
    }


    void ColorToolButton::setColors(const QSet<QColor>& newColors) {
        currentColors = newColors;
        updateIcon();
    }


    void ColorToolButton::resizeEvent(QResizeEvent*) {
        updateIcon();
    }


    void ColorToolButton::updateIcon() {
        QSize    iconSize = ColorToolButton::iconSize();
        QPixmap  pixmap(iconSize);
        QPainter painter(&pixmap);

        unsigned numberColors = static_cast<unsigned>(currentColors.size());
        unsigned remainingColors      = numberColors;
        unsigned numberPrimaryAxis    = 0;

        QList<QSize> gridSizes;

        while (remainingColors > 0) {
            unsigned secondaryAxisSize = std::min(
                static_cast<unsigned>(std::ceil(std::sqrt(numberColors)) + 0.5),
                remainingColors
            );
            unsigned primaryAxisSize   = remainingColors / secondaryAxisSize;
            unsigned colorsThisGrid    = primaryAxisSize * secondaryAxisSize;

            gridSizes.append(QSize(primaryAxisSize, secondaryAxisSize));

            Q_ASSERT(remainingColors >= colorsThisGrid);
            remainingColors   -= colorsThisGrid;
            numberPrimaryAxis += primaryAxisSize;
        }

        bool                         landscapeOrientation = (iconSize.width() >= iconSize.height());
        QSet<QColor>::const_iterator colorIterator        = currentColors.constBegin();
        unsigned                     primaryAxisRemaining = numberPrimaryAxis;
        unsigned                     currentX             = 0;
        unsigned                     currentY             = 0;

        for (  QList<QSize>::const_iterator gridIterator    = gridSizes.constBegin(),
                                            gridEndIterator = gridSizes.constEnd()
             ; gridIterator != gridEndIterator
             ; ++gridIterator
            ) {
            if (landscapeOrientation) {
                unsigned numberColumns = static_cast<unsigned>(gridIterator->width());
                unsigned numberRows    = static_cast<unsigned>(gridIterator->height());

                for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    unsigned width = (iconSize.width() - currentX) / primaryAxisRemaining;

                    for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                        unsigned height = (iconSize.height() - currentY) / (numberRows - rowIndex);
                        drawColorPatch(
                            &painter,
                            QRect(QPoint(currentX, currentY), QSize(width, height)),
                            *colorIterator
                        );

                        currentY += height;
                        ++colorIterator;
                    }

                    currentY = 0;
                    currentX += width;

                    --primaryAxisRemaining;
                }
            } else {
                unsigned numberColumns = static_cast<unsigned>(gridIterator->height());
                unsigned numberRows    = static_cast<unsigned>(gridIterator->width());

                for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    unsigned height = (iconSize.height() - currentY) / primaryAxisRemaining;

                    for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                        unsigned width = (iconSize.width() - currentX) / (numberColumns - columnIndex);
                        drawColorPatch(
                            &painter,
                            QRect(QPoint(currentX, currentY), QSize(width, height)),
                            *colorIterator
                        );

                        currentX += width;
                        ++colorIterator;
                    }

                    currentX = 0;
                    currentY += height;

                    --primaryAxisRemaining;
                }
            }
        }

        setIcon(QIcon(pixmap));
        repaint();
    }


    void ColorToolButton::drawColorPatch(QPainter* painter, const QRect& rectangle, const QColor& color) {
        QPen pen(Qt::white);
        pen.setWidth(2);

        if (color.isValid()) {
            painter->setBrush(QBrush(color));
            painter->setPen(pen);
            painter->drawRect(rectangle);
        } else {
            painter->setBrush(QBrush(Qt::black));
            painter->setPen(pen);
            painter->drawRect(rectangle);

            QPoint points[3] = { rectangle.topRight(), rectangle.bottomRight(), rectangle.bottomLeft() };
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(Qt::lightGray));
            painter->drawConvexPolygon(points, 3);
        }
    }
}
