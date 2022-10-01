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
* This header defines the \ref EQt::GraphicsScene class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_SCENE_H
#define EQT_GRAPHICS_SCENE_H

#include <QObject>
#include <QRectF>
#include <QGraphicsScene>

#include "eqt_common.h"

class QWidget;
class QAction;
class QFocusEvent;

namespace EQt {
    /**
     * Class that extends QGraphicsScene to change the child ownership rules.  Normally the QGraphicsScene
     * owns the child objects.  This complicates code that requires ownership to be maintained by external objects.
     */
    class EQT_PUBLIC_API GraphicsScene:public QGraphicsScene {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            GraphicsScene(QObject* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] sceneRectangle The rectangle that defines the scene boundaries.
             *
             * \param[in] parent         Pointer to the parent object.
             */
            GraphicsScene(const QRectF& sceneRectangle, QObject* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] x      The left edge of the scene rectangle.
             *
             * \param[in] y      The top edge of the scene rectangle.
             *
             * \param[in] width  The scene width.
             *
             * \param[in] height The scene height.
             *
             * \param[in] parent Pointer to the parent object.
             */
            GraphicsScene(double x, double y, double width, double height, QObject* parent = Q_NULLPTR);

            ~GraphicsScene();
    };
}

#endif
