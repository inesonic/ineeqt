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
* This file implements the \ref DockWidgetLocations class.
***********************************************************************************************************************/

#include <QMap>
#include <QDockWidget>

#include "eqt_dock_widget_defaults.h"
#include "dock_widget_location.h"
#include "dock_widget_locations.h"

DockWidgetLocations::DockWidgetLocations() {}


DockWidgetLocations& DockWidgetLocations::operator=(const DockWidgetLocations& other) {
    QMap<DockWidgetLocation, QDockWidget*>::operator=(other);
    locationsByDock   = other.locationsByDock;
    numberTabsInArea  = other.numberTabsInArea;

    return *this;
}



DockWidgetLocations& DockWidgetLocations::operator=(DockWidgetLocations&& other) {
    QMap<DockWidgetLocation, QDockWidget*>::operator=(other);
    locationsByDock   = other.locationsByDock;
    numberTabsInArea  = other.numberTabsInArea;

    return *this;
}


bool DockWidgetLocations::locationDefined(const QDockWidget* dock) const {
    return locationsByDock.contains(const_cast<QDockWidget*>(dock));
}


DockWidgetLocations::InsertionStatus DockWidgetLocations::insert(
        QDockWidget*                  dock,
        EQt::DockWidgetDefaults::Area area
    ) {
    InsertionStatus status;

    if (!locationsByDock.contains(dock)) {
        unsigned& tabNumber = numberTabsInArea[static_cast<unsigned>(area)];

        DockWidgetLocation location(area, tabNumber);

        QMap<DockWidgetLocation, QDockWidget*>::insert(location, dock);
        locationsByDock.insert(dock, location);

        ++tabNumber;

        status = InsertionStatus::INSERTION_SUCCESSFUL;
    } else {
        status = InsertionStatus::DOCK_ALREADY_INSERTED;
    }

    return status;
}


DockWidgetLocations::InsertionStatus DockWidgetLocations::insert(
        QDockWidget*                          dock,
        EQt::DockWidgetDefaults::Relationship relationship,
        QDockWidget*                          sibling
    ) {
    InsertionStatus status;

    if (locationsByDock.contains(sibling)) {
        if (!locationsByDock.contains(dock)) {
            const DockWidgetLocation&                        siblingLocation = locationsByDock.value(sibling);
            QMap<DockWidgetLocation, QDockWidget*>::iterator siblingIterator = find(siblingLocation);

            switch (relationship) {
                case EQt::DockWidgetDefaults::Relationship::BEFORE: {
                    unsigned tabNumber = siblingLocation.inTabNumber();

                    DockWidgetLocation newLocation(siblingLocation.area(), tabNumber);

                    adjustTabNumbers(siblingLocation.area(), siblingIterator, tabNumber + 1);

                    QMap<DockWidgetLocation, QDockWidget*>::insert(newLocation, dock);
                    locationsByDock.insert(dock, newLocation);
                    ++numberTabsInArea[static_cast<unsigned>(siblingLocation.area())];

                    status = InsertionStatus::INSERTION_SUCCESSFUL;
                    break;
                }

                case EQt::DockWidgetDefaults::Relationship::AFTER: {
                    unsigned tabNumber = siblingLocation.inTabNumber() + 1;

                    DockWidgetLocation newLocation(siblingLocation.area(), tabNumber);

                    adjustTabNumbers(siblingLocation.area(), siblingIterator + 1, tabNumber + 1);

                    QMap<DockWidgetLocation, QDockWidget*>::insert(newLocation, dock);
                    locationsByDock.insert(dock, newLocation);
                    ++numberTabsInArea[static_cast<unsigned>(siblingLocation.area())];

                    status = InsertionStatus::INSERTION_SUCCESSFUL;
                    break;
                }

                case EQt::DockWidgetDefaults::Relationship::BEFORE_IN_TABBED_REGION: {
                    QMap<DockWidgetLocation, QDockWidget*>::iterator otherSibling = siblingIterator - 1;

                    double       newEntryOrder;
                    QDockWidget* previousSibling          = Q_NULLPTR;
                    double       siblingOrderingWithinTab = siblingLocation.dockOrderingWithinTab;
                    if (otherSibling == end()) {
                        newEntryOrder = siblingOrderingWithinTab - 1;
                    } else {
                        const DockWidgetLocation& otherSiblingLocation = otherSibling.key();
                        if (otherSiblingLocation.area() == siblingLocation.area()               &&
                            otherSiblingLocation.inTabNumber() == siblingLocation.inTabNumber()    ) {
                            double otherSiblingOrderingWithinTab = otherSiblingLocation.dockOrderingWithinTab;

                            newEntryOrder   = (siblingOrderingWithinTab + otherSiblingOrderingWithinTab) / 2;
                            previousSibling = otherSibling.value();
                        } else {
                            newEntryOrder = siblingOrderingWithinTab - 1;
                        }
                    }

                    DockWidgetLocation newLocation(
                        siblingLocation.area(),
                        siblingLocation.inTabNumber(),
                        previousSibling
                    );
                    newLocation.dockOrderingWithinTab = newEntryOrder;

                    QMap<DockWidgetLocation, QDockWidget*>::insert(newLocation, dock);
                    locationsByDock.insert(dock, newLocation);

                    status = InsertionStatus::INSERTION_SUCCESSFUL;
                    break;
                }

                case EQt::DockWidgetDefaults::Relationship::AFTER_IN_TABBED_REGION: {
                    QMap<DockWidgetLocation, QDockWidget*>::iterator otherSibling = siblingIterator + 1;

                    double       newEntryOrder;
                    double       siblingOrderingWithinTab = siblingLocation.dockOrderingWithinTab;
                    if (otherSibling == end()) {
                        newEntryOrder = siblingOrderingWithinTab + 1;
                    } else {
                        const DockWidgetLocation& otherSiblingLocation = otherSibling.key();
                        if (otherSiblingLocation.area() == siblingLocation.area()               &&
                            otherSiblingLocation.inTabNumber() == siblingLocation.inTabNumber()    ) {
                            double otherSiblingOrderingWithinTab = otherSiblingLocation.dockOrderingWithinTab;

                            newEntryOrder = (siblingOrderingWithinTab + otherSiblingOrderingWithinTab) / 2;
                        } else {
                            newEntryOrder = siblingOrderingWithinTab + 1;
                        }
                    }

                    DockWidgetLocation newLocation(
                        siblingLocation.area(),
                        siblingLocation.inTabNumber(),
                        siblingIterator.value()
                    );
                    newLocation.dockOrderingWithinTab = newEntryOrder;

                    QMap<DockWidgetLocation, QDockWidget*>::insert(newLocation, dock);
                    locationsByDock.insert(dock, newLocation);

                    status = InsertionStatus::INSERTION_SUCCESSFUL;
                    break;
                }

                default: {
                    Q_ASSERT(false);

                    status = InsertionStatus::DOCK_ALREADY_INSERTED;
                    break;
                }
            }

        } else {
            status = InsertionStatus::DOCK_ALREADY_INSERTED;
        }
    } else {
        status = InsertionStatus::UNKNOWN_SIBLING;
    }

    return status;
}


void DockWidgetLocations::adjustTabNumbers(
        EQt::DockWidgetDefaults::Area                           area,
        const QMap<DockWidgetLocation, QDockWidget*>::iterator& iterator,
        unsigned                                                startingTabNumber
    ) {
    QMap<DockWidgetLocation, QDockWidget*>           removedEntries;
    QMap<DockWidgetLocation, QDockWidget*>::iterator removalIterator = iterator;
    unsigned                                         tabNumber       = startingTabNumber;

    while (removalIterator != end() && removalIterator.key().area() == area) {
        const DockWidgetLocation& oldLocation = removalIterator.key();
        DockWidgetLocation        newLocation(oldLocation.area(), tabNumber, oldLocation.placedAfter());
        ++tabNumber;

        QDockWidget* removedDock = removalIterator.value();
        removedEntries.insert(newLocation, removedDock);

        locationsByDock.remove(removedDock);
        removalIterator = erase(removalIterator);
    }

    for (  QMap<DockWidgetLocation, QDockWidget*>::const_iterator insertionIterator    = removedEntries.constBegin(),
                                                                  insertionEndIterator = removedEntries.constEnd()
         ; insertionIterator != insertionEndIterator
         ; ++insertionIterator
        ) {
        locationsByDock.insert(insertionIterator.value(), insertionIterator.key());
        QMap<DockWidgetLocation, QDockWidget*>::insert(insertionIterator.key(), insertionIterator.value());
    }
}
