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
* This header defines the \ref EQt::GraphicsItem class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_ITEM_H
#define EQT_GRAPHICS_ITEM_H

#include <QList>
#include <QGraphicsItem>

#include "eqt_common.h"

class QWidget;
class QAction;
class QFocusEvent;
class QGraphicsItem;
class QTimer;

namespace EQt {
    class Application;

    /**
     * Base class for EQt::GraphicsItem* classes.  This class provides a few additional capabilties to be shared across
     * all of the EQt graphics items.
     */
    class EQT_PUBLIC_API GraphicsItem {
        friend class Application;

        public:
            /**
             * Method you should call to enable the graphics item subsystem.  Be sure to call this before using the
             * \ref EQt::GraphicsItem::deleteLater method.
             */
            static void initializeGarbageCollector();

            GraphicsItem();

            virtual ~GraphicsItem();

            /**
             * Method you can call to remove this graphics item from the scene.  You should overload this method in
             * derived classes to remove the graphics item from the scene.
             */
            virtual void removeFromScene() = 0;

            /**
             * Method you can use to defer updates.  The default implementation does nothing.
             */
            virtual void deferUpdates();

            /**
             * Method you can use to restore updates.  The default implementation does nothing.
             */
            virtual void restoreUpdates();

            /**
             * Method you can use to determine if updates are deferred.
             *
             * \return Returns true if updates are currently deferred.  Returns false if updates are not deferred.  The
             *         default implementation returns false.
             */
            virtual bool updatesAreDeferred() const;

            /**
             * Method you can call to queue up this graphics item for eventual garbage collection.  This method will
             * also cleanly remove the graphics item from the scene, if needed.
             */
            void deleteLater();

        protected:
            /**
             * Method you should overload to indicate if this graphics item represents a group and requires group
             * handling when deleted.
             *
             * \return Returns true if this graphics item is a group.  Returns false if this graphics item is not a
             *         group.  The default implementation returns false.
             */
            virtual bool isGroup() const;

            /**
             * Method you can call to remove a specific graphics item from the scene.  You will generally want to
             * overload \ref EQt::GraphicsItem::removeFromScene, having that method call
             * QGraphicsItem::prepareGeometryUpdate followed by a call to this method.
             *
             * \param[in] graphicsItem The graphics item to be removed.
             */
            static void removeGraphicsItemFromScene(QGraphicsItem* graphicsItem);

        private:
            /**
             * Method that is triggered when the cleanup timer fires.
             */
            static void performGarbageCollection();

            /**
             * List of graphics items pending delete.
             */
            static QList<GraphicsItem*> garbageCan;

            /**
             * Timer used to queue up clean-up operations.
             */
            static QTimer* cleanupTimer;
    };
}

#endif
