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
* This file implements the \ref EQt::ProgrammaticWidget class.
***********************************************************************************************************************/

#include <QObject>
#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>

#include "eqt_common.h"
#include "eqt_programmatic_widget.h"

namespace EQt {
    ProgrammaticWidget::ProgrammaticWidget() {}


    ProgrammaticWidget::~ProgrammaticWidget() {}


    void ProgrammaticWidget::registerLayout(QLayout* layout, const QString& layoutName) {
        if (!layoutName.isEmpty()) {
            Q_ASSERT(!layouts.contains(layoutName));
            layouts.insert(layoutName, layout);
        }
    }


    void ProgrammaticWidget::registerWidget(QWidget* widget, const QString& widgetName) {
        if (!widgetName.isEmpty()) {
            Q_ASSERT(!widgets.contains(widgetName));
            widgets.insert(widgetName, widget);
        }
    }


    void ProgrammaticWidget::reRegisterWidget(QWidget* widget, const QString& widgetName) {
        if (!widgetName.isEmpty()) {
            widgets.insert(widgetName, widget);
        }
    }


    void ProgrammaticWidget::registerObject(QObject* object, const QString& objectName) {
        if (!objectName.isEmpty()) {
            Q_ASSERT(!objects.contains(objectName));
            objects.insert(objectName, object);
        }
    }


    QHBoxLayout* ProgrammaticWidget::newHBoxLayout(const QString& layoutName) {
        QHBoxLayout* layout = new QHBoxLayout;

        customizeHBoxLayout(layout);
        registerLayout(layout, layoutName);

        return layout;
    }


    QVBoxLayout* ProgrammaticWidget::newVBoxLayout(const QString& layoutName) {
        QVBoxLayout* layout = new QVBoxLayout;

        customizeVBoxLayout(layout);
        registerLayout(layout, layoutName);

        return layout;
    }


    QGridLayout* ProgrammaticWidget::newGridLayout(const QString& layoutName) {
        QGridLayout* layout = new QGridLayout;

        customizeGridLayout(layout);
        registerLayout(layout, layoutName);

        return layout;
    }


    QFormLayout* ProgrammaticWidget::newFormLayout(const QString& layoutName) {
        QFormLayout* layout = new QFormLayout;

        customizeFormLayout(layout);
        registerLayout(layout, layoutName);

        return layout;
    }


    void ProgrammaticWidget::customizeLayout(QLayout*) {}


    void ProgrammaticWidget::customizeHBoxLayout(QHBoxLayout* layout) {
        customizeLayout(layout);
    }


    void ProgrammaticWidget::customizeVBoxLayout(QVBoxLayout* layout) {
        customizeLayout(layout);
    }


    void ProgrammaticWidget::customizeGridLayout(QGridLayout* layout) {
        customizeLayout(layout);
    }


    void ProgrammaticWidget::customizeFormLayout(QFormLayout* layout) {
        customizeLayout(layout);
    }
}
