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
* This header defines the \ref DockWidgetLocation class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef DOCK_WIDGET_LOCATION_H
#define DOCK_WIDGET_LOCATION_H

#include "eqt_common.h"
#include "eqt_dock_widget_defaults.h"

class QDockWidget;

class DockWidgetLocations;

/**
 * Class that tracks location information for a dock widget.
 */
class DockWidgetLocation {
    friend class DockWidgetLocations;

    public:
        DockWidgetLocation();

        /**
         * Constructor
         *
         * \param[in] area        The area where this dock widget should be placed.
         *
         * \param[in] inTabNumber The tab number for the dock within the area.
         *
         * \param[in] placedAfter The dock that should precede this dock within the specified tab.
         */
        DockWidgetLocation(
            EQt::DockWidgetDefaults::Area area,
            unsigned                      inTabNumber,
            QDockWidget*                  placedAfter = Q_NULLPTR
        );

        /**
         * Copy constructor
         *
         * \param[in] other The instance to assign to this instance.
         */
        DockWidgetLocation(const DockWidgetLocation& other);

        ~DockWidgetLocation();

        /**
         * Method that returns the area for this dock widget.
         *
         * \return Returns the area where this dock is to reside.
         */
        EQt::DockWidgetDefaults::Area area() const;

        /**
         * Method that returns the tab number of the tab to contain this dock widget.
         *
         * \return Returns the tab containing this dock widget.
         */
        unsigned inTabNumber() const;

        /**
         * Method that returns the dock that should precede this dock in the specified tab.
         *
         * \return Returns the dock that should precede this dock in the specified tab.  A null pointer is returned
         *         if the dock should not be in a tabbed area.
         */
        QDockWidget* placedAfter() const;

        /**
         * Assignment operator.
         *
         * \param[in] other The instance to assign to this instance.
         *
         * \return Returns a reference to this object.
         */
        DockWidgetLocation& operator=(const DockWidgetLocation& other);

        /**
         * Comparison operator.
         *
         * \param[in] other The location to compare against this location.
         *
         * \return Returns true if the locations are identical.  Returns false if the locations are different.
         */
        bool operator==(const DockWidgetLocation& other) const;

        /**
         * Comparison operator.
         *
         * \param[in] other The location to compare against this location.
         *
         * \return Returns true if the locations are different.  Returns false if the locations are equal.
         */
        inline bool operator!=(const DockWidgetLocation& other) const {
            return !operator==(other);
        }

        /**
         * Comparison operator.
         *
         * \param[in] other The location to compare against this location.
         *
         * \return Returns true if this location should precede the other location.  Returns false if this location
         *         should follow or is equal to the other location.
         */
        bool operator<(const DockWidgetLocation& other) const;

        /**
         * Comparison operator.
         *
         * \param[in] other The location to compare against this location.
         *
         * \return Returns true if this location should follow the other location.  Returns false if this location
         *         should precede or is equal to the other location.
         */
        bool operator>(const DockWidgetLocation& other) const;

        /**
         * Comparison operator.
         *
         * \param[in] other The location to compare against this location.
         *
         * \return Returns true if this location should precede or is equal to the other location.  Returns false if
         *         this location should follow to the other location.
         */
        inline bool operator<=(const DockWidgetLocation& other) const {
            return !operator>(other);
        }

        /**
         * Comparison operator.
         *
         * \param[in] other The location to compare against this location.
         *
         * \return Returns true if this location should follow or is equal to the other location.  Returns false if
         *         this location should precede the other location.
         */
        inline bool operator>=(const DockWidgetLocation& other) const {
            return !operator<(other);
        }

    private:
        /**
         * The current dock area.
         */
        EQt::DockWidgetDefaults::Area currentArea;

        /**
         * The current tab number within the dock area.
         */
        unsigned currentTabNumber;

        /**
         * The dock that should precede this dock in the tab ordering.
         */
        QDockWidget* currentPlacedAfter;

        /**
         * A value used internally to track ordering within the current dock.  Value will range between 0 and 1 and
         * will default to 1.0.
         */
        double dockOrderingWithinTab;
};

#endif
