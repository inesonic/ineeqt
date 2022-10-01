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
* This file implements the \ref EQt::RecentFile and \ref EQt::RecentFilesData classes.
***********************************************************************************************************************/

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QFile>
#include <QFileSystemWatcher>
#include <QSettings>

#include "eqt_application.h"
#include "eqt_recent_files_data.h"

/***********************************************************************************************************************
 * RecentFile class:
 */

namespace EQt {
    RecentFile::RecentFile(const QString& newFilePath):QObject() {
        fileInformation.setFile(newFilePath);
    }


    RecentFile::RecentFile(const RecentFile& other):QObject() {
        fileInformation = other.fileInformation;
    }


    RecentFile::~RecentFile() {}


    QString RecentFile::description() const {
        QString name = fileInformation.canonicalFilePath();

        if (name.isEmpty()) {
            name = fileInformation.absoluteFilePath();
        }

        if (name.length() > static_cast<int>(defaultDescriptiveLength)) {
            QString fileName = fileInformation.fileName();
            if (fileName.length() >= static_cast<int>(defaultDescriptiveLength)) {
                name = fileName;
            } else if (fileName.length() >= static_cast<int>((defaultDescriptiveLength-4))) {

                #if (defined(Q_OS_WIN))

                    name = tr("\\..\\%1").arg(fileName);

                #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

                    name = tr("/../%1").arg(fileName);

                #else

                    #error Unknown platform.

                #endif

            } else {
                unsigned leftLength = defaultDescriptiveLength - fileName.length() - 3;

                #if (defined(Q_OS_WIN))

                    name = tr("%1..\\%2").arg(name.left(leftLength)).arg(fileName);

                #elif (defined(Q_OS_LINUX) || defined(Q_OS_DARWIN))

                    name = tr("%1../%2").arg(name.left(leftLength)).arg(fileName);

                #else

                    #error Unknown platform.

                #endif
            }
        }

        return name;
    }
}

/***********************************************************************************************************************
 * RecentFilesData class:
 */

namespace EQt {
    RecentFilesData::RecentFilesData(QObject* parent):QObject(parent) {
        currentHistoryLength = defaultHistoryLength;
        connect(&fileSystemWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));
    }


    RecentFilesData::~RecentFilesData() {}


    void RecentFilesData::setHistoryLength(unsigned newHistoryLength) {
        if (recentFilesData.size() > static_cast<int>(newHistoryLength)) {
            do {
                RecentFile oldest = recentFilesData.takeLast();
                fileSystemWatcher.removePath(oldest.absoluteFilePath());
                emit removed(oldest);
            } while (recentFilesData.size() > static_cast<int>(newHistoryLength));

            emit changed();
        }

        currentHistoryLength = newHistoryLength;
    }


    bool RecentFilesData::add(const QString& newFile) {
        RecentFile newEntry(newFile);

        if (!inList(newEntry)) {
            if (recentFilesData.size() >= static_cast<int>(currentHistoryLength)) {
                RecentFile oldest = recentFilesData.takeLast();
                fileSystemWatcher.removePath(oldest.absoluteFilePath());

                emit removed(oldest);
            }

            recentFilesData.prepend(newEntry);
            fileSystemWatcher.addPath(newEntry.absoluteFilePath());

            emit added(recentFilesData.first());
            emit changed();

            return true;
        } else {
            return false;
        }
    }


    void RecentFilesData::update(const QString& fileName) {
        bool newEntry = add(fileName);

        if (!newEntry) {
            RecentFile recentFile(fileName);

            auto it=recentFilesData.begin();
            auto end=recentFilesData.end();

            while (it != end && (*it) != recentFile) {
                ++it;
            }

            Q_ASSERT(it != end);

            recentFilesData.erase(it);
            recentFilesData.prepend(recentFile);

            emit changed();
        }
    }


    void RecentFilesData::loadSettings() {
        QSettings* settings = Application::settings();

        settings->beginGroup("recentFiles");

        currentHistoryLength = settings->value("currentHistoryLength", defaultHistoryLength).toUInt();
        unsigned numberFiles = settings->value("numberFiles", 0).toUInt();

        for (unsigned i=0 ; i<numberFiles ; ++i) {
            QString filePath = settings->value(QString::number(i),QString()).toString();
            if (!filePath.isEmpty()) {
                add(filePath);
            }
        }

        settings->endGroup();
    }


    void RecentFilesData::saveSettings() {
        QSettings* settings = Application::settings();

        settings->beginGroup("recentFiles");

        settings->setValue("currentHistoryLength", currentHistoryLength);
        settings->setValue("numberFiles", recentFilesData.size());

        for (int i=0 ; i<recentFilesData.size() ; ++i) {
            settings->setValue(QString::number(i), recentFilesData[i].absoluteFilePath());
        }

        settings->endGroup();
    }


    void RecentFilesData::fileChanged(const QString& path) {
        RecentFile changedFileData(path);

        for (auto it=recentFilesData.begin(),end=recentFilesData.end() ; it!=end ; ++it) {
            it->refresh();
            if ((*it) == changedFileData) {
                emit fileStatusChanged(*it);
            }
        }

        emit changed();
    }


    bool RecentFilesData::inList(const RecentFile& entry) {
        auto it=recentFilesData.begin();
        auto end=recentFilesData.end();

        while (it != end && (*it) != entry) {
            ++it;
        }

        return it != end;
    }
}
