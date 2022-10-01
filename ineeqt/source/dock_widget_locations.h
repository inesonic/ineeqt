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
* This header defines the \ref DockWidgetLocations class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef DOCK_WIDGET_LOCATIONS_H
#define DOCK_WIDGET_LOCATIONS_H

#include <QMap>

#include "eqt_common.h"
#include "eqt_dock_widget_defaults.h"
#include "dock_widget_location.h"

class QDockWidget;

/**
 * Template class you can use to track the location and ordering of dock widgets.
 *
 * \param[in] T The object used to represent the dock widget.
 */
class DockWidgetLocations:private QMap<DockWidgetLocation, QDockWidget*> {
    public:
        /**
         * Enumeration indicating success status from insertions.
         */
        enum class InsertionStatus {
            /**
             * Indicates we successfully added the requested dock.
             */
            INSERTION_SUCCESSFUL,

            /**
             * Indicates the dock is already present in the list.
             */
            DOCK_ALREADY_INSERTED,

            /**
             * Indicates the sibling is not defined in the list.  The relative placement can not be determined.
             */
            UNKNOWN_SIBLING
        };

        /**
         * The constant iterator used to traverse this list.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::const_iterator;

        /**
         * The constant iterator used to traverse this list.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::ConstIterator;

        /**
         * The difference type to measure distance between elements.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::difference_type;

        /**
         * The size type used to measure the side of the underlying map.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::size_type;

        /**
         * The type used to represent the map keys.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::key_type;

        /**
         * The type used to represent the map values.
         */
        typedef QDockWidget* value_type;

        DockWidgetLocations();

        /**
         * Copy constructor.
         *
         * \param[in] other The instance to assign to this instance.
         */
        DockWidgetLocations(const DockWidgetLocations& other):QMap<DockWidgetLocation, QDockWidget*>(other) {}

        /**
         * Move constructor.
         *
         * \param[in] other The instance to assign to this instance.
         */
        DockWidgetLocations(DockWidgetLocations&& other):QMap<DockWidgetLocation, QDockWidget*>(other) {}

        ~DockWidgetLocations() {}

        /**
         * Method that returns a constant iterator to the start of the underlying map.
         *
         * \return Returns an iterator to the start of the map.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::cbegin;

        /**
         * Method that returns a constant iterator to the end of the underlying map.
         *
         * \return Returns an iterator to the end of the map.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::cend;

        /**
         * Method that returns a constant iterator to the start of the underlying map.
         *
         * \return Returns an iterator to the start of the map.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::constBegin;

        /**
         * Method that returns a constant iterator to the end of the underlying map.
         *
         * \return Returns an iterator to the end of the map.
         */
        using QMap<DockWidgetLocation, QDockWidget*>::constEnd;

        /**
         * Assignment operator.
         *
         * \param[in] other The instance to assign to this instance.
         *
         * \return Returns a reference to this instance.
         */
        DockWidgetLocations& operator=(const DockWidgetLocations& other);

        /**
         * Assignment operator (move semantics)
         *
         * \param[in] other The instance to move to this instance.
         *
         * \return Returns a reference to this instance.
         */
        DockWidgetLocations& operator=(DockWidgetLocations&& other);

        /**
         * Method you can use to determine if a dock has been added to the list of dock locations.
         *
         * \param[in] dock A pointer to the dock to be checked.
         *
         * \return Returns true if the dock is being tracked.  Returns false if the dock is not being tracked.
         */
        bool locationDefined(const QDockWidget* dock) const;

        /**
         * Method you can use to insert a new entry into the dock locations list.
         *
         * \param[in] dock The dock to be placed.
         *
         * \param[in] area The dock area where the dock is to be placed.
         *
         * \return Returns the success/failure status from the insertion.
         */
        InsertionStatus insert(QDockWidget* dock, EQt::DockWidgetDefaults::Area area);

        /**
         * Method you can use to insert a new entry into the dock locations list.
         *
         * \param[in] dock         The dock to be placed.
         *
         * \param[in] relationship The relationship between this dock and its sibling.
         *
         * \param[in] siblingDock  The sibling dock to track against this dock.
         *
         * \return Returns the success/failure status from the insertion.
         */
        InsertionStatus insert(
            QDockWidget*                          dock,
            EQt::DockWidgetDefaults::Relationship relationship,
            QDockWidget*                          siblingDock
        );

    private:
        /**
         * Method that adjusts the tab number of all docks at or after a given position.
         *
         * \param[in] area              The area requiring adjustment.
         *
         * \param[in] iterator          Iterator to the first dock requiring adjustment.
         *
         * \param[in] startingTabNumber The tab number to assign to the first dock.
         */
        void adjustTabNumbers(
            EQt::DockWidgetDefaults::Area                           area,
            const QMap<DockWidgetLocation, QDockWidget*>::iterator& iterator,
            unsigned                                                startingTabNumber
        );

        /**
         * A set used to track the inserted docks.
         */
        QMap<QDockWidget*, DockWidgetLocation> locationsByDock;
        \
        /**
         * A map used to track the number of independent tabs in each dock area.
         */
        QMap<unsigned, unsigned> numberTabsInArea;
};

#endif
