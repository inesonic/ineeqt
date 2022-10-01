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
* This header defines the \ref EQt::DockWidgetDefaults class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_DOCK_WIDGET_DEFAULTS_H
#define EQT_DOCK_WIDGET_DEFAULTS_H

#include <QString>
#include <QSharedData>
#include <QSharedDataPointer>

#include <util_hash_functions.h>

#include "eqt_common.h"

class QWidget;
class QLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QFormLayout;
class QMenu;
class QAction;
class QGroupBox;

namespace EQt {
    class ProgrammaticDockWidget;

    /**
     * Class that tracks default position and view settings for a dock widget.
     */
    class EQT_PUBLIC_API DockWidgetDefaults {
        public:
            /**
             * Enumeration of supported dock widget areas.  Floating docks maintain a default location that they will
             * fall back to if re-docked.
             */
            enum class Area {
                /**
                 * Indicates the placement area is not defined.  Do not use this value.
                 */
                UNDEFINED,

                /**
                 * Indicates the area is determined by a sibling dock.
                 */
                BASED_ON_SIBLING,

                /**
                 * Indicates the dock should be at the top.
                 */
                TOP,

                /**
                 * Indicates the dock should be on the left side.
                 */
                LEFT,

                /**
                 * Indicates the dock should be on the right side.
                 */
                RIGHT,

                /**
                 * Indicates the dock should be on the bottom.
                 */
                BOTTOM
            };

            /**
             * Enumeration indicating the relationship with another dock.
             */
            enum class Relationship {
                /**
                 * Indicates this dock has no pre-defined relationship with another dock.
                 */
                NONE,

                /**
                 * Indicates this dock should be placed to the left of or above a specfied dock.
                 */
                BEFORE,

                /**
                 * Indicates this dock should be placed to the right of or below a specified dock.
                 */
                AFTER,

                /**
                 * Indicates this dock should be placed to the left of a specified dock within a group of tabs.
                 */
                BEFORE_IN_TABBED_REGION,

                /**
                 * Indicates this dock should be placed to the right of a specified dock within a group of tabs.
                 */
                AFTER_IN_TABBED_REGION
            };

            DockWidgetDefaults();

            /**
             * Constructor
             *
             * \param[in] defaultDockArea Indicates the dock should be placed in the specified area.
             *
             * \param[in] defaultVisible  Indicates the dock should be visible, by default.
             *
             * \param[in] defaultFloating Indicates that the dock should be made to float after being assigned a
             *                            location.
             */
            DockWidgetDefaults(Area defaultDockArea, bool defaultVisible, bool defaultFloating = false);

            /**
             * Constructor
             *
             * \param[in] relationship    Indicates the relationship with another dock.
             *
             * \param[in] siblingDockName The name of the other dock.
             *
             * \param[in] defaultVisible  Indicates the dock should be visible, by default.
             */
            DockWidgetDefaults(
                Relationship   relationship,
                const QString& siblingDockName,
                bool           defaultVisible
            );

            /**
             * Constructor
             *
             * \param[in] dockName        The name of the dock tracked by this class.
             *
             * \param[in] defaultDockArea Indicates the dock should be placed in the specified area (or floating).
             *
             * \param[in] defaultVisible  Indicates the dock should be visible, by default.
             *
             * \param[in] defaultFloating Indicates that the dock should be made to float after being assigned a
             *                            location.
             */
            DockWidgetDefaults(
                const QString& dockName,
                Area           defaultDockArea,
                bool           defaultVisible,
                bool           defaultFloating = false
            );

            /**
             * Constructor
             *
             * \param[in] dockName        The name of the dock tracked by this class.
             *
             * \param[in] relationship    Indicates the relationship with another dock.
             *
             * \param[in] siblingDockName The name of the other dock.
             *
             * \param[in] defaultVisible  Indicates the dock should be visible, by default.
             */
            DockWidgetDefaults(
                const QString& dockName,
                Relationship   relationship,
                const QString& siblingDockName,
                bool           defaultVisible
            );

            /**
             * Copy constructor
             *
             * \param[in] other The instance to be copied.
             */
            DockWidgetDefaults(const DockWidgetDefaults& other);

            ~DockWidgetDefaults();

            /**
             * Method you can use to determine if this relationship is defined.
             *
             * \return Returns true if this dock relationship is defined.  Returns false if this dock relationship is
             *         undefined.
             */
            bool isValid() const;

            /**
             * Method you can use to determine if this relationship is undefined.
             *
             * \return Returns true if this dock relationship is undefined.  Returns false if this dock relationship
             *         is defined.
             */
            bool isInvalid() const;

            /**
             * Method you can use to determine if this dock's position is defined by its position.
             *
             * \return Returns true if this dock's position is explicitely defined in terms of dock location.  Returns
             *         false if this dock's position is implicitely defined in terms of it's relationship to other
             *         docks.
             */
            bool isExplicitPosition() const;

            /**
             * Method you can use to determine if this dock's postiion is defined by its relationship to other docks.
             *
             * \return Returns true if this dock's position is implicitly defined in terms of another dock.  Returns
             *         false if this dock's position is explicitly defined.
             */
            bool isImplicitPosition() const;

            /**
             * Method you can use to set the name of the dock tracked by this instance.
             *
             * \param[in] newName The new dock name.
             */
            void setDockName(const QString& newName);

            /**
             * Method you can use to determine the name of the dock tracked by this instance.
             *
             * \return Returns the dock name tracked by this instance.
             */
            QString dockName() const;

            /**
             * Method you can use to set the dock area to report.
             *
             * \param[in] newDockArea The new dock area.
             */
            void setDockArea(Area newDockArea);

            /**
             * Method you can use to obtain the dock area to report.
             *
             * \return Returns the dock area.
             */
            Area dockArea() const;

            /**
             * Method you can use to set whether the dock should be shown or hidden by default.
             *
             * \param[in] nowVisible If true, the dock will be made visible, by default.  If false, the dock will be
             *                       made hidden by default.
             */
            void setDefaultVisible(bool nowVisible = true);

            /**
             * Method you can use to set whether the dock should be hidden or shown by default.
             *
             * \param[in] nowHidden If true, the dock will be made hidden, by default.  If false, the dock will be made
             *                      visible by default.
             */
            void setDefaultHidden(bool nowHidden = true);

            /**
             * Method you can use to determine if the dock should be visible by default.
             *
             * \return Returns true if the dock should be visible, by default.  Returns false if the dock should be
             *         hidden by default.
             */
            bool defaultVisible() const;

            /**
             * Method you can use to determine if the dock should be hidden by default.
             *
             * \return Returns true if the dock should be hidden by default.  Returns false if the dock should be
             *         visible by default.
             */
            bool defaultHidden() const;

            /**
             * Method you can use set whether the dock should be floating by default.
             *
             * \param[in] nowFloating If true, the dock will default to floating.  If false, the dock will be located
             *                        within the main window.
             */
            void setDefaultFloating(bool nowFloating = true);

            /**
             * Method you can use set whether the dock should be docked by default.
             *
             * \param[in] nowDocked If true, the dock will default to its position within the main window. If false,
             *                      the dock will be made to float after its position is assigned.
             */
            void setDefaultDocked(bool nowDocked = true);

            /**
             * Method you can use to determine if this dock should be floating.
             *
             * \return Returns true if the dock should be floating.  Returns false if the dock should be within the
             *         main window.
             */
            bool defaultFloating() const;

            /**
             * Method you can use to determine if this dock should be docked within the main window.
             *
             * \return Returns true if the dock should be held within the main window.  Returns false if the dock
             *         should be floating.
             */
            bool defaultDocked() const;

            /**
             * Method you can use to specify a relationship between this dock and another dock.
             *
             * \param[in] relationship    The relationship with the other dock.
             *
             * \param[in] siblingDockName The name of the other dock.
             */
            void setRelationship(Relationship relationship, const QString& siblingDockName);

            /**
             * Method you can use to obtain this dock's relationship with another dock.
             *
             * \return Returns the relative positioning between docks.
             */
            Relationship relationship() const;

            /**
             * Method you can use to obtain the name of the dock that this dock is positioned next to.
             *
             * \return Returns the sibling dock name.
             */
            QString siblingDockName() const;

            /**
             * Assignment operator.
             *
             * \param[in] other The instance to assign to this instance.
             *
             * \return Returns a reference to this instance.
             */
            DockWidgetDefaults& operator=(const DockWidgetDefaults& other);

        private:
            /**
             * The private implementation.
             */
            class Private:public QSharedData {
                public:
                    /**
                     * Constructor
                     *
                     * \param[in] defaultDockArea Indicates the dock should be placed in the specified area (or floating).
                     *
                     * \param[in] defaultVisible  Indicates the dock should be visible, by default.
                     *
                     * \param[in] defaultFloating Indicates that the dock should be made to float after being assigned
                     *                            a location.
                     */
                    Private(Area defaultDockArea, bool defaultVisible, bool defaultFloating);

                    /**
                     * Constructor
                     *
                     * \param[in] relationship    Indicates the relationship with another dock.
                     *
                     * \param[in] siblingDockName The name of the other dock.
                     *
                     * \param[in] defaultVisible  Indicates the dock should be visible, by default.
                     */
                    Private(
                        Relationship   relationship,
                        const QString& siblingDockName,
                        bool           defaultVisible
                    );

                    /**
                     * Constructor
                     *
                     * \param[in] dockName        The name of the dock tracked by this class.
                     *
                     * \param[in] defaultDockArea Indicates the dock should be placed in the specified area (or floating).
                     *
                     * \param[in] defaultVisible  Indicates the dock should be visible, by default.
                     *
                     * \param[in] defaultFloating Indicates that the dock should be made to float after being assigned
                     *                            a location.
                     */
                    Private(const QString& dockName, Area defaultDockArea, bool defaultVisible, bool defaultFloating);

                    /**
                     * Constructor
                     *
                     * \param[in] dockName        The name of the dock tracked by this class.
                     *
                     * \param[in] relationship    Indicates the relationship with another dock.
                     *
                     * \param[in] siblingDockName The name of the other dock.
                     *
                     * \param[in] defaultVisible  Indicates the dock should be visible, by default.
                     */
                    Private(
                        const QString& dockName,
                        Relationship   relationship,
                        const QString& siblingDockName,
                        bool           defaultVisible
                    );

                    /**
                     * Method you can use to determine if this dock's position is defined by its position.
                     *
                     * \return Returns true if this dock's position is explicitely defined in terms of dock location.  Returns
                     *         false if this dock's position is implicitely defined in terms of it's relationship to other
                     *         docks.
                     */
                    bool isExplicitPosition() const;

                    /**
                     * Method you can use to determine if this dock's postiion is defined by its relationship to other docks.
                     *
                     * \return Returns true if this dock's position is implicitly defined in terms of another dock.  Returns
                     *         false if this dock's position is explicitly defined.
                     */
                    bool isImplicitPosition() const;

                    /**
                     * Method you can use to set the name of the dock tracked by this instance.
                     *
                     * \param[in] newName The new dock name.
                     */
                    void setDockName(const QString& newName);

                    /**
                     * Method you can use to determine the name of the dock tracked by this instance.
                     *
                     * \return Returns the dock name tracked by this instance.
                     */
                    QString dockName() const;

                    /**
                     * Method you can use to set the dock area to report.
                     *
                     * \param[in] newDockArea The new dock area.
                     */
                    void setDockArea(Area newDockArea);

                    /**
                     * Method you can use to obtain the dock area to report.
                     *
                     * \return Returns the dock area.
                     */
                    Area dockArea() const;

                    /**
                     * Method you can use to set whether the dock should be shown or hidden by default.
                     *
                     * \param[in] nowVisible If true, the dock will be made visible, by default.  If false, the dock
                     *                       will be made hidden by default.
                     */
                    void setDefaultVisible(bool nowVisible = true);

                    /**
                     * Method you can use to set whether the dock should be hidden or shown by default.
                     *
                     * \param[in] nowHidden If true, the dock will be made hidden, by default.  If false, the dock will
                     *                      be made visible by default.
                     */
                    void setDefaultHidden(bool nowHidden = true);

                    /**
                     * Method you can use to determine if the dock should be visible by default.
                     *
                     * \return Returns true if the dock should be visible, by default.  Returns false if the dock
                     *         should be hidden by default.
                     */
                    bool defaultVisible() const;

                    /**
                     * Method you can use to determine if the dock should be hidden by default.
                     *
                     * \return Returns true if the dock should be hidden by default.  Returns false if the dock should
                     *         be visible by default.
                     */
                    bool defaultHidden() const;

                    /**
                     * Method you can use set whether the dock should be floating by default.
                     *
                     * \param[in] nowFloating If true, the dock will default to floating.  If false, the dock will be
                     *                        located within the main window.
                     */
                    void setDefaultFloating(bool nowFloating = true);

                    /**
                     * Method you can use set whether the dock should be docked by default.
                     *
                     * \param[in] nowDocked If true, the dock will default to its position within the main window. If
                     *                      false, the dock will be made to float after its position is assigned.
                     */
                    void setDefaultDocked(bool nowDocked = true);

                    /**
                     * Method you can use to determine if this dock should be floating.
                     *
                     * \return Returns true if the dock should be floating.  Returns false if the dock should be
                     *         within the main window.
                     */
                    bool defaultFloating() const;

                    /**
                     * Method you can use to determine if this dock should be docked within the main window.
                     *
                     * \return Returns true if the dock should be held within the main window.  Returns false if the
                     *         dock should be floating.
                     */
                    bool defaultDocked() const;

                    /**
                     * Method you can use to specify a relationship between this dock and another dock.
                     *
                     * \param[in] relationship    The relationship with the other dock.
                     *
                     * \param[in] siblingDockName The name of the other dock.
                     */
                    void setRelationship(Relationship relationship, const QString& siblingDockName);

                    /**
                     * Method you can use to obtain this dock's relationship with another dock.
                     *
                     * \return Returns the relative positioning between docks.
                     */
                    Relationship relationship() const;

                    /**
                     * Method you can use to obtain the name of the dock that this dock is positioned next to.
                     *
                     * \return Returns the sibling dock name.
                     */
                    QString siblingDockName() const;

                private:
                    /**
                     * This dock name.
                     */
                    QString currentDockName;

                    /**
                     * The dock area.
                     */
                    Area currentDockArea;

                    /**
                     * Flag indicating if the dock should be visible, by default.
                     */
                    bool currentIsVisibleByDefault;

                    /**
                     * Flag indicating if the dock should be floating by default.
                     */
                    bool currentFloatingByDefault;

                    /**
                     * The relationship with another dock.
                     */
                    Relationship currentRelationship;

                    /**
                     * The sibling dock name.
                     */
                    QString currentSibling;
            };

            /**
             * The private implementation.
             */
            QSharedDataPointer<Private> impl;
    };

    /**
     * Function that calculates a hash for a \ref EQt::DockWidgetDefaults::Area enumeration.
     *
     * \param[in] value The value to calculate the hash for.
     *
     * \param[in] seed  An optional seed to apply.
     *
     * \return Returns a hash generated from the provided enum.
     */
    Util::HashResult qHash(DockWidgetDefaults::Area value, Util::HashSeed seed = 0);
}

#endif
