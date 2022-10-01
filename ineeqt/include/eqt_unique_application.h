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
* This header defines the \ref EQt::UniqueApplication class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_UNIQUE_APPLICATION_H
#define EQT_UNIQUE_APPLICATION_H

#include <QApplication>
#include <QMetaType>

#include "eqt_common.h"
#include "eqt_application.h"

class QString;
class QSettings;
class QLocalServer;
class QLocalSocket;

namespace EQt {
    /**
     * An application base class.  This class extends the \ref EQt::Application class to provide support for management
     * of multiple application instances.
     */
    class EQT_PUBLIC_API UniqueApplication:public Application {
        Q_OBJECT

        public:
            /**
             * Enumeration indicating the type of application start-up.
             */
            enum StartupCondition {
                /**
                 * Normal start-up condition.
                 */
                NORMAL_START_UP = 0,

                /**
                 * Indicates that the previous instance of the application crashed unexpectedly but we were able to
                 * recover.
                 */
                PREVIOUS_INSTANCE_CRASHED = 1,

                /**
                 * Indicates that the previous instance crashed and we were unable to recover.  This should be treated
                 * as a catastropic error condition.
                 */
                START_UP_FAILED = 2
            };

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
            UniqueApplication(
                int&           applicationCount,
                char**         applicationValues,
                const QString& applicationName,
                const QString& versionNumber,
                const QString& organization,
                const QString& domain
            );

            ~UniqueApplication() override;

            /**
             * Returns the instance of this application.
             *
             * \return A pointer to the application instance.
             */
            static inline UniqueApplication* instance() {
                return dynamic_cast<UniqueApplication*>(QApplication::instance());
            }

            /**
             * Function that can be used to start the application.  Calling this method will first check for other
             * running instances and, if another instance is running, pass the command line parameters to that
             * instance.  If other instances are not running, this method will schedule the
             * \ref startUserInterface(UniqueApplication::StartupCondition const) signal and then enter the main
             * application loop.
             *
             * You are expected to put the code needed to start the GUI into a slot triggered by the
             * \ref startUserInterface(UniqueApplication::StartupCondition const) signal.
             *
             * \return Returns 0 on success, non-zero if an error occurs that forces unexpected termination of the
             *         application.
             */
            int exec();

        signals:
            /**
             * Signal that is emitted when this class has determined that it is the primary instance and should launch
             * the GUI interface for the user.
             *
             * \param[out] startupCondition Indicates the state of the application when it starts up.
             */
            void startUserInterface(UniqueApplication::StartupCondition const startupCondition);

            /**
             * Signal that is emitted when a new application instance is loaded.
             *
             * \param[out] parameters The parameters passed on the other application instance's command line.
             */
            void instanceLoaded(const QStringList& parameters);

            /**
             * Signal that is emitted if another instance is already running.  This signal is emitted after parameters
             * are passed to the primary instance.
             */
            void primaryInstanceAlreadyRunning();

        private slots:
            /**
             * Respond to another instance of this application connecting to the local server to send us a message.
             */
            void secondaryInstanceConnected();

        private:
            /**
             * Enumeration indicating the status of this application.
             */
            enum ApplicationStatus {
                /**
                 * Indicates that this application is the primary instance. Call Application::exec()
                 */
                PRIMARY_INSTANCE_CONFIGURED = 0,

                /**
                 * Indicates that this application is the secondary instance.  Do not call Application::exec().
                 */
                SECONDARY_INSTANCE_CONFIGURED = 1,

                /**
                 * An error occurred during discovery or message passing.
                 */
                ERROR_REPORTED = 3
            };

            /**
             * Number of allowed connection retries before failing.
             */
            static const unsigned connectionRetries = 1;

            /**
             * Delay for each retry attempt for both server and client connections.  Value is in mSec.
             */
            static const unsigned retryDelay = 120000;

            /**
             * This method will verify that the application is the only running instance and:
             *
             *     * If this is the only instance, issue the "startUserInterface" signal to spawn the GUI, or
             *     * If other instances are currently running, message them with the command line parameters we were
             *       given to let them open up any user requested files.
             *     * Inform the user that another instance appears to be running but not responding.
             *
             * You would typically use this method as shown in the listin below. \code

               ApplicationStatus status = checkInstance();
               int            exitStatus;
               if (status == UniqueApplication::PRIMARY_INSTANCE_CONFIGURED) {
                   #if (defined(Q_OS_WIN))
                       Application::settings()->setValue("instanceRunning", true);
                   #endif

                   exitStatus = QApplication::exec();

                   #if (defined(Q_OS_WIN))
                       if (exitStatus == 0) {
                           Application::settings()->setValue("instanceRunning", false);
                       }
                   #endif
               } else if (status == UniqueApplication::SECONDARY_INSTANCE_CONFIGURED){
                   exitStatus = 0;
               } else {
                   exitStatus = 1;
               }

               return exitStatus; \endcode
             *
             * Windows local domain sockets are automatically destroyed by the OS.  This prevents the mechanisms used
             * on to detect a crash from working properly on Windows.  To work around this, Windows builds should set
             * the top level QSettings variable ``instanceRunning`` to ``true`` when entering the event loop and back
             * to ``false`` on exit.
             *
             * The example code above demonstrates this kludge.
             *
             * \return Returns the status of the application.
             */
            ApplicationStatus checkInstance();

            /**
             * The named pipe or local domain server used for IPC.  This class is used to detect when other instances
             * are running and to pass control off to those instances.
             */
            QLocalServer* ipcServer;

            /**
             * Local socket used when this application is not the primary instance to pass information to the primary
             * instance.
             */
            QLocalSocket* ipcSocket;
    };
}

/**
 * Qt Metatype for EQt::UniqueApplication::startupCondition.
 */
Q_DECLARE_METATYPE(EQt::UniqueApplication::StartupCondition);

#endif
