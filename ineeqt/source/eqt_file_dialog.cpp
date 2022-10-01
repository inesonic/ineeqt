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
* This file implements the \ref EQt::FileDialog class.
***********************************************************************************************************************/

#include <QFileDialog>
#include <QWidget>
#include <QStandardPaths>
#include <QCloseEvent>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QByteArray>
#include <QDir>

#include "eqt_common.h"
#include "eqt_application.h"
#include "eqt_file_dialog.h"

namespace EQt {
    FileDialog::FileDialog(QWidget* parent, Qt::WindowFlags flags):QFileDialog(parent, flags) {}


    FileDialog::FileDialog(const QString& caption, QWidget* parent):QFileDialog(parent, caption) {}


    FileDialog::~FileDialog() {}


    void FileDialog::loadSettings() {
        QSettings* settings = Application::settings();

        settings->beginGroup(settingsGroupName());

        QStringList history = settings->value("history").toStringList();
        setHistory(history);

        QString currentDirectory = settings->value("directory").toString();
        if (currentDirectory.isEmpty()) {
            QStringList locations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
            currentDirectory = locations.first();
        }

        if (!currentDirectory.isEmpty()) {
            setDirectory(currentDirectory);
        }

        QString mode = settings->value("viewMode", "list").toString();
        if (mode == "list") {
            setViewMode(QFileDialog::List);
        } else if (mode == "detail") {
            setViewMode(QFileDialog::Detail);
        } else {
            setViewMode(QFileDialog::List);
        }

        settings->endGroup();
    }


    void FileDialog::saveSettings() {
        QSettings* settings = Application::settings();

        settings->beginGroup(settingsGroupName());

        settings->setValue("state", saveState());

        if (viewMode() == QFileDialog::Detail) {
            settings->setValue("viewMode", "detail");
        } else {
            Q_ASSERT(viewMode() == QFileDialog::List);
            settings->setValue("viewMode", "list");
        }

        settings->endGroup();
    }


    int FileDialog::exec() {
        setNameFilters(filterList());
        return QFileDialog::exec();
    }
}
