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
* This header defines the \ref EQt::ProgrammaticDockWidget class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_DOCK_WIDGET_H
#define EQT_PROGRAMMATIC_DOCK_WIDGET_H

#include <QDockWidget>
#include <QString>
#include <QMargins>
#include <QSize>
#include <QRect>
#include <QList>
#include <QMap>

#include "eqt_common.h"
#include "eqt_programmatic_main_window.h"
#include "eqt_programmatic_widget.h"

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
    /**
     * Class that extends QDockWidget to support intelligent controls that can be shuffled around the window as desired
     * by the user.
     *
     * The ProgrammaticDockWidget class includes support to allow the dock widget to support two layouts, a horizontal
     * layout and a vertical layout.  When docked on the left or right side of the main window, the dock widget will
     * use the vertical layout.  When docked on the top or bottom of the main window, the dock widget will use the
     * horizontal layout.  When undocked, the dock layout will take on the horizontal layout.
     */
    class EQT_PUBLIC_API ProgrammaticDockWidget:public QDockWidget, public ProgrammaticWidget {
        Q_OBJECT

        public:
            /**
             * Value that provides a recommend spacing between groups of controls within a dock.
             */
            static const unsigned recommendedControlsSpacing;

            /**
             * Value that provides a recommended spacing between vertically placed docks.
             */
            static const unsigned recommendedVerticalGap;

            /**
             * Value that provides a recommended spacing between horizontally placed docks.
             */
            static const unsigned recommendedHorizontalGap;

            /**
             * Value that provides the default dock widget features
             */
            static const DockWidgetFeatures defaultFeatures;

            /**
             * Constructor.
             *
             * \param[in] title  The title to assign to the dock widget.
             *
             * \param[in] parent The parent object for this window.
             *
             * \param[in] flags  Flags that can be used to modify the behavior of this dock widget.
             */
            ProgrammaticDockWidget(
                const QString&          title,
                ProgrammaticMainWindow* parent = Q_NULLPTR,
                Qt::WindowFlags         flags = Qt::WindowFlags()
            );

            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             *
             * \param[in] flags  Flags that can be used to modify the behavior of this dock widget.
             */
            ProgrammaticDockWidget(
                ProgrammaticMainWindow* parent = Q_NULLPTR,
                Qt::WindowFlags         flags = Qt::WindowFlags()
            );

            ~ProgrammaticDockWidget() override;

            /**
             * Determines the parent \ref ProgrammaticMainWindow that controls this dock.
             *
             * \return Returns a pointer to the controlling parent window.
             */
            inline ProgrammaticMainWindow* parent() {
                return dynamic_cast<ProgrammaticMainWindow*>(QDockWidget::parent());
            }

            /**
             * Method you can use to enable or disable the close button on the titlebar.  The close button is disabled
             * by default.
             *
             * \param[in] nowEnabled If true, the close button will be shown.  If false, the close button will be
             *                       hidden.
             */
            void setCloseButtonEnabled(bool nowEnabled = true);

            /**
             * Method you can use to disable or enable the close button on the titlebar.  The close button is disabled
             * by default.
             *
             * \param[in] nowDisabled If true, the close button will be hidden.  If false, the close button will be
             *                        shown.
             */
            void setCloseButtonDisabled(bool nowDisabled = true);

            /**
             * Method you can use to determine if the close button is enabled.  The close button is disabled by default.
             *
             * \return Returns true if the close button is enabled. Returns false if the close button is disabled.
             */
            bool closeButtonEnabled() const;

            /**
             * Method you can use to determine if the close button is disabled.  The close button is disabled by
             * default.
             *
             * \return Returns true if the close button is disabled.  Returns false if the close button is enabled.
             */
            bool closeButtonDisabled() const;

            /**
             * Method that is called by the main window after all the actions are created to configure this dock widget.
             * The default implementation does nothing.
             *
             * \param[in] mainWindow Pointer to the main window object.
             */
            virtual void configureDockWidget(ProgrammaticMainWindow* mainWindow);

            /**
             * Sets the layout to use for the horizontal layout.  You can use this method to configure the dialog.
             *
             * Note that you are strongly recommended to call the QLayout::setSizeConstraint method on the layout
             * before adding the layout to this doc.
             *
             * \param[in] newLayout The layout to be used for the horizontal layout.
             */
            void setHorizontalLayout(QLayout* newLayout);

            /**
             * Obtains the grid layout used in the horizontal layout.  You can use this method as a hook to extend
             * this dock widget.
             *
             * \return Returns the vertical layout widget.
             */
            QLayout* horizontalLayout();

            /**
             * Sets the layout to use for the vertical layout.  You can use this method to configure the dialog.
             *
             * Note that you are strongly recommended to call the QLayout::setSizeConstraint method on the layout
             * before adding the layout to this doc.
             *
             * \param[in] newLayout The layout to be used for the horizontal layout.
             */
            void setVerticalLayout(QLayout* newLayout);

            /**
             * Obtains the grid layout used in the vertical layout.  You can use this method as a hook to extend this
             * dock widget.
             *
             * \return Returns the vertical layout widget.
             */
            QLayout* verticalLayout();

            /**
             * Modifies the dock to display a single widget in all orientations.  The dock will take ownership of the
             * widget and will destroy the widget at end of scope.
             *
             * \param[in] widget The widget to be managed by the dock.  A null pointer will restore operation to
             *                   use distinct horizontal and vertical layout settings.  You must re-build the layouts
             *                   if this value is null.
             */
            void setSingleWidgetMode(QWidget* widget);

            /**
             * Determines if this dock is currently docked or floating.
             *
             * \return Returns true if this dock is docked, returns false if the dock is floating.
             */
            bool isDocked() const;

            /**
             * Determines if this dock is currently docked and included with other docks within a tabbed region.
             *
             * \return Returns true if the dock is tabbed.  Returns false if the dock is floating or docked
             *         independently from other docks.
             */
            bool isTabDocked() const;

            /**
             * The recommended minimum layout spacing for docks.
             *
             * \return Returns the recommended minimum layout spacing within docks.  You can use this value in
             *         conjunction with the QLayout::setSpacing method to set consistent and rational spacing for the
             *         docks.
             */
            static int minimumLayoutSpacing();

            /**
             * The recommended layout margins for docks.
             *
             * \return Returns the recommended minimum layout margins within docks.  You can use this value in
             *         conjunction with the QLayout::setContentsMargins method to set consistent and rational margins
             *         for the docks.
             */
            static QMargins minimumLayoutMargins();

            /**
             * Method that is called to set the desired height adjustment for the dock widget.
             *
             * \param[in] adjustment The non-negative adjustment from the minimum height required to make the dock
             *                       widget fit nicely.
             */
            virtual void setHeightAdjustment(unsigned adjustment);

            /**
             * Method that is called to set the desired height adjustment for the dock widget.
             *
             * \param[in] adjustment The non-negative adjustment from the minimum width required to make the dock
             *                       widget fit nicely.
             */
            virtual void setWidthAdjustment(unsigned adjustment);

            /**
             * Method you can use to set the minimum height of the dock in the horizontal orientation.
             *
             * \param[in] newMinimumHeight The new minimum dock height in the horizontal orientation.  A value of
             *                             zero will cause the dock's size to not be adjusted.
             */
            void setMinimumDockHeight(unsigned newMinimumHeight);

            /**
             * Method you can use to set the minimum width of the dock in the vertical orientation.
             *
             * \param[in] newMinimumWidth The new minimum dock height in the vertical orientation.  A value of zero
             *                            will cause the dock's size to not be adjusted.
             */
            void setMinimumDockWidth(unsigned newMinimumWidth);

            /**
             * Method you can use obtain the minimum height of the dock in the horizontal orientation.
             *
             * \return Returns the minimum height in the horizontal orientation.
             */
            virtual unsigned minimumHeightHorizontalOrientation() const;

            /**
             * Method you can use to obtain the minimum width in the vertical orientation.
             *
             * \return Returns the minimum width in the vertical orientation.
             */
            virtual unsigned minimumWidthVerticalOrientation() const;

            /**
             * Method you can use to obtain the minimum recommended height of the dock widget title bar.
             *
             * \return Returns the minimum recommended height of the dock widget title bar.
             */
            unsigned minimumRecommendedHorizontalOrientationTitleBarHeight() const;

            /**
             * Creates a new group box for the dock widget.
             *
             * \param[in] title      The title to show in the group box.
             *
             * \param[in] layout     The layout to use for the group box.
             *
             * \param[in] objectName An optional name to assign to the object.  Assigning an object name allows this
             *                       group box to be referenced explicitly from style sheets.
             *
             * \return Returns a pointer to the newly created group box.
             */
            QGroupBox* newGroupBox(
                const QString& title,
                QLayout*       layout,
                const QString& objectName = QString()
            );

        protected:
            /**
             * Method that is called by the layout functions to perform any customization of the layouts.
             *
             * \param[in] layout The layout to be customized.
             */
            void customizeLayout(QLayout* layout) final;

        protected slots:
            /**
             * Slot that is triggered when the dock location is changed by the user.  You can overload this slot if you
             * need to receive notification that the dock location has changed.
             *
             * \param[in] area the new dock location area.
             */
            virtual void dockLocationChanged(Qt::DockWidgetArea area);

            /**
             * Slot that is triggered when the dock is made to float or is docked.  You can overload this slot if you
             * need to receive notation that the dock has been made floating or docked.
             *
             * \param[in] topLevel Holds true if the dock is now floating.
             */
            virtual void topLevelChanged(bool topLevel);

        private:
            /**
             * The layout spacing to use for OS X.
             */
            static const int osxLayoutSpacing = 1;

            /**
             * The layout spacing to use for Windows.
             */
            static const int windowsLayoutSpacing = 1;

            /**
             * The layout spacing to use for Linux.
             */
            static const int linuxLayoutSpacing = 1;

            /**
             * The layout margins to use for OS X.
             */
            static const int osxLayoutMargins = 1;

            /**
             * The layout margins to use for Windows.
             */
            static const int windowsLayoutMargins = 1;

            /**
             * The layout margins to use for Linux.
             */
            static const int linuxLayoutMargins = 1;

            /**
             * Method that is called by the constructors to build the dock widget.
             */
            void buildWidget();

            /**
             * The application global default context menu.
             */
            static QMenu* defaultContextMenu;

            /**
             * Widget that is shown when the dock is in the vertical orientation.
             */
            QWidget* currentVerticalWidget;

            /**
             * Widget that is shown when the dock is in the horizontal orientation.
             */
            QWidget* currentHorizontalWidget;

            /**
             * The dock minimum height in the horizontal orientation.
             */
            unsigned currentDockMinimumHeightHorizontalOrientation;

            /**
             * The dock minimum width in the vertical orientation.
             */
            unsigned currentDockMinimumWidthVerticalOrientation;
    };
}

#endif
