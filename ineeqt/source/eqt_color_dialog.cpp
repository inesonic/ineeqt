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
* This file implements the \ref EQt::ColorDialog class.
***********************************************************************************************************************/

#include <QColorDialog>
#include <QWidget>
#include <QColor>
#include <QSettings>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QString>

#include "eqt_application.h"
#include "eqt_color_dialog.h"

namespace EQt {
    ColorDialog::ColorDialog(QWidget* parent):QColorDialog(parent) {}


    ColorDialog::ColorDialog(const QColor& initial, QWidget* parent):QColorDialog(initial, parent) {}


    ColorDialog::~ColorDialog() {}


    void ColorDialog::loadSettings() {
        QSettings* settings = Application::settings();

        QByteArray settingsData = settings->value("colorDialog", QByteArray()).toByteArray();
        unsigned colorIndex = 0;

        QDataStream colorStream(settingsData);
        while (!colorStream.atEnd()) {
            QColor color;
            colorStream >> color;
            QColorDialog::setCustomColor(colorIndex, color);
        }
    }


    void ColorDialog::saveSettings() {
        QSettings* settings = Application::settings();

        unsigned numberColors = QColorDialog::customCount();

        QByteArray settingsData;
        QDataStream colorStream(&settingsData, QIODevice::WriteOnly);

        for (unsigned i=0 ; i<numberColors ; ++i) {
            colorStream << QColorDialog::customColor(i);
        }

        settings->setValue("colorDialog", settingsData);
    }
}
