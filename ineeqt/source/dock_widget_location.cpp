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
* This file implements the \ref DockWidgetLocation class.
***********************************************************************************************************************/

#include "eqt_dock_widget_defaults.h"
#include "dock_widget_location.h"

DockWidgetLocation::DockWidgetLocation() {
    currentArea           = EQt::DockWidgetDefaults::Area::UNDEFINED;
    currentTabNumber      = 0;
    currentPlacedAfter    = Q_NULLPTR;
    dockOrderingWithinTab = 0.0;
}


DockWidgetLocation::DockWidgetLocation(
        EQt::DockWidgetDefaults::Area area,
        unsigned                      inTabNumber,
        QDockWidget*                  placedAfter
    ) {
    currentArea           = area;
    currentTabNumber      = inTabNumber;
    currentPlacedAfter    = placedAfter;
    dockOrderingWithinTab = 0.0;
}


DockWidgetLocation::DockWidgetLocation(const DockWidgetLocation& other) {
    currentArea           = other.currentArea;
    currentTabNumber      = other.currentTabNumber;
    currentPlacedAfter    = other.currentPlacedAfter;
    dockOrderingWithinTab = other.dockOrderingWithinTab;
}


DockWidgetLocation::~DockWidgetLocation() {}


EQt::DockWidgetDefaults::Area DockWidgetLocation::area() const {
    return currentArea;
}


unsigned DockWidgetLocation::inTabNumber() const {
    return currentTabNumber;
}


QDockWidget* DockWidgetLocation::placedAfter() const {
    return currentPlacedAfter;
}


DockWidgetLocation& DockWidgetLocation::operator=(const DockWidgetLocation& other) {
    currentArea           = other.currentArea;
    currentTabNumber      = other.currentTabNumber;
    currentPlacedAfter    = other.currentPlacedAfter;
    dockOrderingWithinTab = other.dockOrderingWithinTab;

    return *this;
}


bool DockWidgetLocation::operator==(const DockWidgetLocation& other) const {
    return (
           currentArea == other.currentArea
        && currentTabNumber == other.currentTabNumber
        && currentPlacedAfter == other.currentPlacedAfter
    );
}


bool DockWidgetLocation::operator<(const DockWidgetLocation& other) const {
    bool result = false;

    if (static_cast<int>(currentArea) < static_cast<int>(other.currentArea)) {
        result = true;
    } else if (currentArea == other.currentArea) {
        if (currentTabNumber < other.currentTabNumber) {
            result = true;
        } else if (currentTabNumber == other.currentTabNumber) {
            if (dockOrderingWithinTab < other.dockOrderingWithinTab) {
                result = true;
            }
        }
    }

    return result;
}


bool DockWidgetLocation::operator>(const DockWidgetLocation& other) const {
    bool result = false;

    if (static_cast<int>(currentArea) > static_cast<int>(other.currentArea)) {
        result = true;
    } else if (currentArea == other.currentArea) {
        if (currentTabNumber > other.currentTabNumber) {
            result = true;
        } else if (currentTabNumber == other.currentTabNumber) {
            if (dockOrderingWithinTab > other.dockOrderingWithinTab) {
                result = true;
            }
        }
    }

    return result;
}
