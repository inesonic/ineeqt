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
* This header defines the \ref EQt::ProgrammaticApplication instance.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_APPLICATION_H
#define EQT_PROGRAMMATIC_APPLICATION_H

#include <QApplication>
#include <QString>
#include <QList>

#include "eqt_common.h"
#include "eqt_unique_application.h"

class QSettings;
class QAction;

namespace EQt {
    class BuilderBase;

    /**
     * An application base class.  This class provides a thin wrapper on the \ref EQt::UniqueApplication class
     * providing the following additional functions:
     *
     *     * Support for plug-in modules.
     *     * Support for a database of builder instances that are used to create and extend the GUI.
     */
    class EQT_PUBLIC_API ProgrammaticApplication:public UniqueApplication {
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
            ProgrammaticApplication(
                int&           applicationCount,
                char**         applicationValues,
                const QString& applicationName,
                const QString& versionNumber,
                const QString& organization,
                const QString& domain
            );

            ~ProgrammaticApplication() override;

            /**
             * Returns the instance of this application.
             *
             * \return A pointer to the application instance.
             */
            static inline ProgrammaticApplication* instance() {
                return dynamic_cast<ProgrammaticApplication*>(QApplication::instance());
            }

            /**
             * Adds a builder instance to the database of builder instances.  Note that this class will take ownership
             * of the builder.
             *
             * \param[in] newBuilder The builder instance to be used.
             */
            static void addBuilder(BuilderBase* newBuilder);

            /**
             * Returns an iterator indicating the start of the ordered builder list.
             *
             * \return Returns an iterator into the builder list.
             */
            static inline QList<BuilderBase*>::Iterator begin() {
                return builderList.begin();
            }

            /**
             * Returns an iterator indicating the end of the ordered builder list.
             *
             * \return Returns an iterator into the builder list.
             */
            static inline QList<BuilderBase*>::Iterator end() {
                return builderList.end();
            }

            /**
             * Processes all the builders to register builder specific resources with the application.
             */
            void runBuilders();

        private:
            /**
             * List of builder classes that have been instantiated with the application.
             */
            static QList<BuilderBase*> builderList;
    };
}

#endif
