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
* This file implements the \ref EQt::GraphicsItem class.
***********************************************************************************************************************/

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QTimer>
#include <QList>

#include "eqt_application.h"
#include "eqt_graphics_item.h"

namespace EQt {
    QList<GraphicsItem*> GraphicsItem::garbageCan;
    QTimer*              GraphicsItem::cleanupTimer;


    void GraphicsItem::initializeGarbageCollector() {
        cleanupTimer = new QTimer(Application::instance());
        cleanupTimer->setSingleShot(true);

        QObject::connect(cleanupTimer, &QTimer::timeout, &GraphicsItem::performGarbageCollection);
    }

    GraphicsItem::GraphicsItem() {}


    GraphicsItem::~GraphicsItem() {}


    void GraphicsItem::deferUpdates() {}


    void GraphicsItem::restoreUpdates() {}


    bool GraphicsItem::updatesAreDeferred() const {
        return false;
    }


    bool GraphicsItem::isGroup() const {
        return false;
    }


    void GraphicsItem::removeGraphicsItemFromScene(QGraphicsItem* graphicsItem) {
        QGraphicsItem* parent = graphicsItem->parentItem();
        if (parent != Q_NULLPTR && parent->type() == QGraphicsItemGroup::Type) {
            static_cast<QGraphicsItemGroup*>(parent)->removeFromGroup(graphicsItem);
        }

        graphicsItem->setParentItem(Q_NULLPTR);

        QGraphicsScene* scene = graphicsItem->scene();
        if (scene != Q_NULLPTR) {
            scene->removeItem(graphicsItem);
        }
    }


    void GraphicsItem::deleteLater() {
        garbageCan.append(this);
        cleanupTimer->start(0);
    }


    void GraphicsItem::performGarbageCollection() {
        // NOTE: As an alternate approach, we could use a factory approach.  We hide the unused items and track them in
        //       a pool.

        for (  QList<GraphicsItem*>::const_iterator graphicsItemIterator    = garbageCan.begin(),
                                                    graphicsItemEndIterator = garbageCan.end()
             ; graphicsItemIterator != graphicsItemEndIterator
             ; ++graphicsItemIterator
            ) {
            GraphicsItem* graphicsItem = *graphicsItemIterator;

            if (graphicsItem->isGroup()) {
                QGraphicsItemGroup* graphicsItemGroup = dynamic_cast<QGraphicsItemGroup*>(graphicsItem);
                QGraphicsScene*     scene             = graphicsItemGroup->scene();

                if (scene != Q_NULLPTR) {
                    scene->destroyItemGroup(graphicsItemGroup);
                } else {
                    delete graphicsItemGroup;
                }
            } else {
                QGraphicsItem*  qGraphicsItem = dynamic_cast<QGraphicsItem*>(graphicsItem);
                QGraphicsScene* scene         = qGraphicsItem->scene();

                if (scene != Q_NULLPTR) {
                    scene->removeItem(qGraphicsItem);
                }

                delete graphicsItem;
            }
        }

        garbageCan.clear();
    }
}
