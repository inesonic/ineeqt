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
* This header defines the \ref EQt::RecentFilesData class as well as several associated derived classes.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_RECENT_FILES_H
#define EQT_RECENT_FILES_H

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QFile>
#include <QFileSystemWatcher>

#include "eqt_common.h"

namespace EQt {
    class RecentFilesData;

    /**
     * Class the holds information on a recent file.  The class marshalls most of the methods of the underlying
     * QFileInfo class instance.
     */
    class EQT_PUBLIC_API RecentFile:public QObject {
        friend class RecentFilesData;

        private:
            /**
             * Private constructor for the RecentFile instance.
             *
             * \param[in] newFilePath The path to the recently selected file.
             */
            RecentFile(const QString& newFilePath = QString());

        public:
            /**
             * Desired descriptive length.
             */
            static constexpr unsigned defaultDescriptiveLength = 40;

            /**
             * Copy constructor.
             *
             * \param[in] other The other instance.
             */
            RecentFile(const RecentFile& other);

            ~RecentFile() override;

            /**
             * You can use this method to obtain the file's absolute file path.
             *
             * \return Returns the absolute file path.
             */
            inline QString absoluteFilePath() const {
                return fileInformation.absoluteFilePath();
            }

            /**
             * You can use this method to obtain the canonical file path.
             *
             * \return Returns the absolute path without symbolic links or redundant ".." or "." elements.  Note that
             *         an empty string is returned if the file does not exist.
             */
            inline QString canonicalFilePath() const {
                return fileInformation.canonicalFilePath();
            }

            /**
             * You can use this method to determine if a file exists.
             *
             * \return Returns true if the file exists.  Returns false if the file does not exist.
             */
            inline bool exists() const {
                return fileInformation.exists();
            }

            /**
             * You can use this method to determine if a file is a normal file.
             *
             * \return Returns true if the file is a normal file.  Returns false if the file is a directory or some
             *         other type of file system object.
             */
            inline bool isFile() const {
                return fileInformation.isFile();
            }

            /**
             * You can use this method to determine if the file can be read.
             *
             * \return Returns true if the file can be read.  Returns false if the file permissions do not allow read
             *         access.
             */
            inline bool isReadable() const {
                return fileInformation.isReadable();
            }

            /**
             * You can use this information to refresh cached information about the file.
             */
            inline void refresh() {
                fileInformation.refresh();
            }

            /**
             * You can use this method to obtain a descriptive name of the file.
             *
             * \return Returns a descriptive name of the file.  The method will make a best attempt to limit the name
             *         to no more than the indicated number of characters.
             */
            QString description() const;

            /**
             * Operator used to compare recent file entries.
             *
             * \param[in] b The entry to compare.
             *
             * \return Returns true if the entries match.  Returns false if the entries do not match.
             */
            inline bool operator==(const RecentFile& b) const {
                return fileInformation == b.fileInformation;
            }

            /**
             * Operator used to compare recent file entries.
             *
             * \param[in] b The entry to compare.
             *
             * \return Returns true if the entries do not match.  Returns false if the entries match.
             */
            inline bool operator!=(const RecentFile& b) const {
                return fileInformation != b.fileInformation;
            }

            /**
             * Operator used to set one instance equal to another.
             *
             * \param[in] other The entry to make this entry equal to.
             */
            inline RecentFile& operator=(const RecentFile& other) {
                fileInformation = other.fileInformation;
                return *this;
            }

        private:
            /**
             * You can use this method in friend classes to path to the recent file.
             *
             * \param[in] newFilePath The path to the recently selected file.
             */
            inline void setFilePath(const QString& newFilePath) {
                fileInformation.setFile(newFilePath);
            }

            QFileInfo fileInformation;
    };

    /**
     * Class that tracks the recent files.
     */
    class EQT_PUBLIC_API RecentFilesData:public QObject {
        Q_OBJECT

        public:
            /**
             * The default history length, in entries.
             */
            static unsigned constexpr defaultHistoryLength = 10;

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            RecentFilesData(QObject* parent = Q_NULLPTR);

            ~RecentFilesData() override;

            /**
             * Returns an iterator pointing to the beginning of the list of recent files.
             *
             * \return Returns an iterator into a QList iterator.
             */
            inline QList<RecentFile>::ConstIterator begin() const {
                return recentFilesData.constBegin();
            }

            /**
             * Returns an iterator pointing to the end of the list of recent files.
             *
             * \return Returns an iterator into a QList iterator.
             */
            inline QList<RecentFile>::ConstIterator end() const {
                return recentFilesData.constEnd();
            }

            /**
             * Method that sets the maximum recent file history length.
             *
             * \param[in] newHistoryLength The new history length.
             */
            void setHistoryLength(unsigned newHistoryLength);

            /**
             * Method that returns the current history length.
             *
             * \return Returns the current history length.
             */
            inline unsigned historyLength() const {
                return currentHistoryLength;
            }

            /**
             * Adds a new entry to the list of recent files.
             *
             * \param[in] newFile The new file to be added.
             *
             * \return Returns true on success.  Returns false if the entry already exists.
             */
            bool add(const QString& newFile);

            /**
             * Convenience method that adds a new file to the list of recent files.
             *
             * \param[in] file The file to be added.
             *
             * \return Returns true on success, returns false if the entry could not be added.
             */
            inline bool add(const QFile& file) {
                return add(file.fileName());
            }

            /**
             * Updates the recent files data.  If the file does not exist, it will be added.   If the file does exist,
             * it will be moved to the top of the list.
             *
             * \param[in] fileName The name of the file in question.
             */
            void update(const QString& fileName);

            /**
             * Convenience method that updates the recent files data.
             *
             * \param[in] file The file in question.
             */
            inline void update(const QFile& file) {
                update(file.fileName());
            }

            /**
             * Loads the list of recent files.
             */
            void loadSettings();

            /**
             * Saves the list of recent files.
             */
            void saveSettings();

        signals:
            /**n
             * Signal that is emitted when a new entry is added.
             *
             * \param[in] entry The newly added entry.
             */
            void added(const RecentFile& entry);

            /**
             * Signal that is emitted when a file entry is removed.
             *
             * \param[in] entry The descriptive name of the entry that was just removed.
             */
            void removed(const RecentFile& entry);

            /**
             * Signal that is emitted when a change is made to the list.  This signal can be emitted when an entry is
             * added or the list is reorganized.
             */
            void changed();

            /**
             * Signal that is emitted when the status of a file changes.
             *
             * \param[in] fileData The data associated with the updated file.
             */
            void fileStatusChanged(const RecentFile& fileData);

        private slots:
            /**
             * Slot that received a file status change from the file system watcher.
             *
             * \param[in] path The path of the changed file.
             */
            void fileChanged(const QString& path);

        private:
            /**
             * Determines if an entry is already in the list.
             *
             * \param[in] entry The entry to be checked.
             *
             * \return Returns true if the entry is in the list, returns false if the entry is not in the list.
             */
            bool inList(const RecentFile& entry);

            unsigned           currentHistoryLength;
            QList<RecentFile>  recentFilesData;
            QFileSystemWatcher fileSystemWatcher;
    };
}

#endif
