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
* This header defines the \ref EQt::Application instance.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_APPLICATION_H
#define EQT_APPLICATION_H

#include <QApplication>
#include <QList>
#include <QIcon>
#include <QMap>
#include <QStringList>

class QString;
class QSettings;
class QStyle;
class QLocale;
class QNetworkAccessManager;

#include "eqt_common.h"

namespace EQt {
    class RecentFilesData;

    /**
     * An application base class.  This class provides a thin wrapper on the QApplication class providing additional
     * application global objects that are accessible through static methods.
     */
    class EQT_PUBLIC_API Application:public QApplication {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in,out] applicationCount  The number of command line parameters.
             *
             * \param[in,out] applicationValues The values passed on the command line.
             *
             * \param[in] applicationName       The name of the application as used by configuration files and the OS.
             *
             * \param[in] versionNumber         The application version number.
             *
             * \param[in] organization          The organization that publishes the application.
             *
             * \param[in] domain                The registered domain for the organization.
             */
            Application(
                int&           applicationCount,
                char**         applicationValues,
                const QString& applicationName,
                const QString& versionNumber,
                const QString& organization,
                const QString& domain
            );

            ~Application() override;

            /**
             * Returns the instance of this application.
             *
             * \return A pointer to the application instance.
             */
            static inline Application* instance() {
                return dynamic_cast<Application*>(QApplication::instance());
            }

            /**
             * Returns a pointer to the application's QSettings class instance.
             *
             * \return Returns a pointer to an instance of QSettings
             */
            static QSettings* settings();

            /**
             * Returns a pointer to the application's global QLocale class instance.
             *
             * \return Returns a pointer to an instance of QLocale.
             */
            static QLocale* locale();

            /**
             * Returns a pointer to the configured network access manager.
             *
             * \return Returns a pointer to a fully configured network access manager.
             */
            static QNetworkAccessManager* networkAccessManager();

            /**
             * Returns a pointer to the application global recent files database.
             *
             * \return Returns a pointer to the application's global recent files database.
             */
            static RecentFilesData* recentFilesData();

            /** \rst:leading-asterisk
             *
             * Returns an icon supporting all available variants.  Icons are searched for in the current application
             * resources based on the supplied base name.  Table :num:`application-icon-name-format` describes how a
             * base name is translated into different icon versions.
             *
             * .. _application-icon-name-format:
             * .. table:: Icon resource names
             *
             *    +--------------------------------+--------------------------------------------------+
             *    | Format                         | Function                                         |
             *    +--------------------------------+--------------------------------------------------+
             *    | ``<name>_<size>.png``          | The normal version of the icon named ``name``.   |
             *    +--------------------------------+--------------------------------------------------+
             *    | ``<name>_active_<size>.png``   | The normal version of the icon named ``name``.   |
             *    +--------------------------------+--------------------------------------------------+
             *    | ``<name>_disabled_<size>.png`` | The disabled version of the icon named ``name``. |
             *    +--------------------------------+--------------------------------------------------+
             *    | ``<name>_hot_<size>.png``      | The hot version of the icon named ``name``.      |
             *    +--------------------------------+--------------------------------------------------+
             *
             * The ``<size>`` field indicates the size of the icon, in pixels.  For example, the 32x32 version of the
             * disabled ``File | New`` icon might be named ``:file_new_disabled_32.png``.
             *
             * The method will endeavor to load all supplied variants of each icon.
             *
             * \endrst
             *
             * \param[in] name The base name of the icon.
             *
             * \return Returns an instance of QIcon.
             */
            static QIcon icon(const QString& name);

            /**
             * Method that returns paper sizes organized and localized to the current locale.
             *
             * \return Returns a list of paper size descriptions.  The descriptions can be converted to Util::PageSize
             *         instances using the Util::PageSize::fromDescription method.  Note that empty strings are
             *         inserted where separators would naturally want to reside in a list of paper sizes.
             */
            static QStringList paperSizeDescriptions();

        protected slots:
            /**
             * Slot that is triggered when there are no more visible windows.  The default implementation does nothing.
             */
            virtual void allWindowsClosed();

        protected:
            /**
             * Method that attempts to automatically determine the proxy settings that need to be used to access the
             * internet.
             */
            void configureNetworkServices();

        private:
            /**
             * The current application settings instance.
             */
            QSettings* currentSettings;

            /**
             * The current application locale data.
             */
            QLocale* currentLocale;

            /**
             * The application's global network access manager.
             */
            QNetworkAccessManager* currentNetworkAccessManager;

            /**
             * The application's database of recently opened filed.
             */
            RecentFilesData* currentRecentFilesData;

            /**
             * Database of configured/sized application icons.
             */
            static QMap<QString, QIcon> icons;
    };
}

#endif
