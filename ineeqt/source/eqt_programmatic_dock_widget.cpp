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
* This file implements the \ref EQt::ProgrammaticDockWidget class.
***********************************************************************************************************************/

#include <QDockWidget>
#include <QWidget>
#include <QString>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QMargins>
#include <QGroupBox>
#include <QMap>
#include <QSize>
#include <QRect>
#include <QPoint>
#include <QMenu>
#include <QFont>
#include <QFontMetrics>

#include "eqt_programmatic_application.h"
#include "eqt_programmatic_widget.h"
#include "eqt_programmatic_main_window.h"
#include "eqt_programmatic_dock_widget.h"

namespace EQt {
    const unsigned ProgrammaticDockWidget::recommendedControlsSpacing = 10;
    const unsigned ProgrammaticDockWidget::recommendedVerticalGap = 10;
    const unsigned ProgrammaticDockWidget::recommendedHorizontalGap = 10;

    #if (defined(Q_OS_WINDOWS) || defined(Q_OS_DARWIN))

        const EQt::ProgrammaticDockWidget::DockWidgetFeatures EQt::ProgrammaticDockWidget::defaultFeatures = (
              EQt::ProgrammaticDockWidget::DockWidgetFeature::DockWidgetMovable
            | EQt::ProgrammaticDockWidget::DockWidgetFeature::DockWidgetFloatable
        );

    #elif (defined(Q_OS_LINUX))

        const EQt::ProgrammaticDockWidget::DockWidgetFeatures EQt::ProgrammaticDockWidget::defaultFeatures = (
            EQt::ProgrammaticDockWidget::DockWidgetFeature::DockWidgetMovable
        );

    #else

        #error Unknown QPlatformIntegration

    #endif

    ProgrammaticDockWidget::ProgrammaticDockWidget(
            const QString&          title,
            ProgrammaticMainWindow* parent,
            Qt::WindowFlags         flags
        ):QDockWidget(
            title,
            parent,
            flags
        ) {
        buildWidget();
    }


    ProgrammaticDockWidget::ProgrammaticDockWidget(
            ProgrammaticMainWindow* parent,
            Qt::WindowFlags         flags
        ):QDockWidget(
            parent,
            flags
        ) {
        buildWidget();
    }


    ProgrammaticDockWidget::~ProgrammaticDockWidget() {
        delete currentHorizontalWidget;
        if (currentHorizontalWidget != currentVerticalWidget) {
            delete currentVerticalWidget;
        }
    }


    void ProgrammaticDockWidget::setCloseButtonEnabled(bool nowEnabled) {
        DockWidgetFeatures currentFeatures = features();
        Qt::WindowFlags    currentFlags    = windowFlags();

        if (nowEnabled) {
            currentFeatures |= DockWidgetClosable;
            currentFlags    |= Qt::WindowCloseButtonHint;
        } else {
            currentFeatures &= ~DockWidgetClosable;
            currentFlags    &= ~Qt::WindowCloseButtonHint;
        }

        setFeatures(currentFeatures);
        setWindowFlags(currentFlags);
    }


    void ProgrammaticDockWidget::setCloseButtonDisabled(bool nowDisabled) {
        setCloseButtonEnabled(!nowDisabled);
    }


    bool ProgrammaticDockWidget::closeButtonEnabled() const {
        return (features() | DockWidgetClosable) != 0;
    }


    bool ProgrammaticDockWidget::closeButtonDisabled() const {
        return !closeButtonEnabled();
    }


    void ProgrammaticDockWidget::configureDockWidget(ProgrammaticMainWindow*) {}


    void ProgrammaticDockWidget::setHorizontalLayout(QLayout* newLayout) {
         currentHorizontalWidget->setLayout(newLayout);
    }


    QLayout* ProgrammaticDockWidget::horizontalLayout() {
        return currentHorizontalWidget->layout();
    }


    void ProgrammaticDockWidget::setVerticalLayout(QLayout* newLayout) {
         currentVerticalWidget->setLayout(newLayout);
    }


    QLayout* ProgrammaticDockWidget::verticalLayout() {
        return currentVerticalWidget->layout();
    }


    void ProgrammaticDockWidget::setSingleWidgetMode(QWidget* widget) {
        delete currentHorizontalWidget;
        if (currentHorizontalWidget != currentVerticalWidget) {
            delete currentVerticalWidget;
        }

        if (widget != Q_NULLPTR) {
            currentHorizontalWidget = widget;
            currentVerticalWidget   = widget;
        } else {
            currentHorizontalWidget = new QWidget;
            currentVerticalWidget   = new QWidget;
        }
    }


    bool ProgrammaticDockWidget::isDocked() const {
        return !isFloating();
    }


    bool ProgrammaticDockWidget::isTabDocked() const {
        bool result;

        if (isDocked()) {
            ProgrammaticDockWidget*       me         = const_cast<ProgrammaticDockWidget*>(this);
            const ProgrammaticMainWindow* mainWindow = dynamic_cast<const ProgrammaticMainWindow*>(me->parent());
            QList<QDockWidget*>           otherDocks = mainWindow->tabifiedDockWidgets(me);

            result = !otherDocks.isEmpty();
        } else {
            result = false;
        }

        return result;
    }


    int ProgrammaticDockWidget::minimumLayoutSpacing() {
        #if (defined(Q_OS_DARWIN))

            return osxLayoutSpacing;

        #elif (defined(Q_OS_WIN))

            return windowsLayoutSpacing;

        #elif (defined(Q_OS_LINUX))

            return linuxLayoutSpacing;

        #else

            #error Unknown platform.

        #endif
    }


    QMargins ProgrammaticDockWidget::minimumLayoutMargins() {
        #if (defined(Q_OS_DARWIN))

            return QMargins(osxLayoutMargins, osxLayoutMargins, osxLayoutMargins, osxLayoutMargins);

        #elif (defined(Q_OS_WIN))

            return QMargins(windowsLayoutMargins, windowsLayoutMargins, windowsLayoutMargins, windowsLayoutMargins);

        #elif (defined(Q_OS_LINUX))

            return QMargins(linuxLayoutMargins, linuxLayoutMargins, linuxLayoutMargins, linuxLayoutMargins);

        #else

            #error Unknown platform.

        #endif
    }


    void ProgrammaticDockWidget::setHeightAdjustment(unsigned adjustment) {
        if (QDockWidget::widget() == currentHorizontalWidget && currentDockMinimumHeightHorizontalOrientation != 0) {
            currentHorizontalWidget->setFixedHeight(currentDockMinimumHeightHorizontalOrientation + adjustment);

            setFixedHeight(currentDockMinimumHeightHorizontalOrientation + adjustment);
            setFixedWidth(QWIDGETSIZE_MAX);
        }
    }


    void ProgrammaticDockWidget::setWidthAdjustment(unsigned adjustment) {
        if (QDockWidget::widget() == currentVerticalWidget && currentDockMinimumWidthVerticalOrientation != 0) {
            currentVerticalWidget->setFixedWidth(currentDockMinimumWidthVerticalOrientation + adjustment);

            setFixedHeight(QWIDGETSIZE_MAX);
            setFixedWidth(currentDockMinimumWidthVerticalOrientation + adjustment);
        }
    }


    void ProgrammaticDockWidget::setMinimumDockHeight(unsigned newMinimumHeight) {
        currentDockMinimumHeightHorizontalOrientation = newMinimumHeight;
    }


    void ProgrammaticDockWidget::setMinimumDockWidth(unsigned newMinimumWidth) {
        currentDockMinimumWidthVerticalOrientation = newMinimumWidth;
    }


    unsigned ProgrammaticDockWidget::minimumHeightHorizontalOrientation() const {
        return currentDockMinimumHeightHorizontalOrientation;
    }


    unsigned ProgrammaticDockWidget::minimumWidthVerticalOrientation() const {
        return currentDockMinimumWidthVerticalOrientation;
    }


    unsigned ProgrammaticDockWidget::minimumRecommendedHorizontalOrientationTitleBarHeight() const {
        #if (defined(Q_OS_WIN) || defined(Q_OS_LINUX))

            QFont    dockTitleFont = Application::font();
            QString  dockTitle     = windowTitle();
            unsigned minimumHeight = QFontMetrics(dockTitleFont).horizontalAdvance(dockTitle + QString("MMMMMMM"));

        #elif (defined(Q_OS_DARWIN))

            QFont dockTitleFont = Application::font();
            dockTitleFont.setPointSize(dockTitleFont.pointSize() - 2);

            QString  dockTitle     = windowTitle();
            unsigned minimumHeight = QFontMetrics(dockTitleFont).horizontalAdvance(dockTitle + QString("MMMMM"));

        #else

            #error Invalid platform

        #endif

        return minimumHeight;
    }


    QGroupBox* ProgrammaticDockWidget::newGroupBox(const QString& title, QLayout* layout, const QString& objectName) {
        QGroupBox* newGroupBox = new QGroupBox(title);
        newGroupBox->setLayout(layout);
        newGroupBox->setAlignment(Qt::AlignCenter);

        if (!objectName.isEmpty()) {
            newGroupBox->setObjectName(objectName);
            registerWidget(newGroupBox, objectName);
        }

        return newGroupBox;
    }


    void ProgrammaticDockWidget::customizeLayout(QLayout* layout) {
        layout->setSpacing(minimumLayoutSpacing());
        layout->setContentsMargins(minimumLayoutMargins());
    }


    void ProgrammaticDockWidget::dockLocationChanged(Qt::DockWidgetArea area) {
        DockWidgetFeatures newFeatures;

        if        (area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea) {
            setWidget(currentVerticalWidget);
            newFeatures = defaultFeatures;
        } else if (area == Qt::TopDockWidgetArea || area == Qt::BottomDockWidgetArea) {
            setWidget(currentHorizontalWidget);
            newFeatures = defaultFeatures | DockWidgetVerticalTitleBar;
        }

        if (closeButtonEnabled()) {
            newFeatures |= DockWidgetClosable;
        }

        setFeatures(newFeatures);

        ProgrammaticMainWindow* mainWindow = dynamic_cast<ProgrammaticMainWindow*>(parent());
        mainWindow->dockStateChanged(this);
    }


    void ProgrammaticDockWidget::topLevelChanged(bool topLevel) {
        if (topLevel) {
            setWidget(currentHorizontalWidget);
            setFeatures(defaultFeatures);
        }

        ProgrammaticMainWindow* mainWindow = dynamic_cast<ProgrammaticMainWindow*>(parent());
        mainWindow->dockStateChanged(this);
    }


    void ProgrammaticDockWidget::buildWidget() {
        setFeatures(defaultFeatures);
        setContextMenuPolicy(Qt::PreventContextMenu);

        currentHorizontalWidget = new QWidget;
        currentVerticalWidget   = new QWidget;

        currentDockMinimumWidthVerticalOrientation    = 0;
        currentDockMinimumHeightHorizontalOrientation = 0;

        connect(
            this,
            SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            this,
            SLOT(dockLocationChanged(Qt::DockWidgetArea))
        );

        connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(topLevelChanged(bool)));
    }
}
