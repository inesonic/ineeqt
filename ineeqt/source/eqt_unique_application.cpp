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
* This file implements the \ref EQt::UniqueApplication class.
***********************************************************************************************************************/

#include <QApplication>
#include <QMetaType>
#include <QSettings>
#include <QTimer>
#include <QLocalServer>
#include <QLocalSocket>
#include <QIODevice>
#include <QString>
#include <QMessageBox>
#include <QDebug>

#include <util_system.h>

#include "eqt_application.h"
#include "eqt_unique_application.h"

namespace EQt {
    UniqueApplication::UniqueApplication(
            int&           applicationCount,
            char**         applicationValues,
            const QString& applicationName,
            const QString& versionNumber,
            const QString& organization,
            const QString& domain
        ):Application(
            applicationCount,
            applicationValues,
            applicationName,
            versionNumber,
            organization,
            domain
        ) {
        qRegisterMetaType<EQt::UniqueApplication::StartupCondition>("EQt::UniqueApplication::StartupCondition");

        ipcServer = Q_NULLPTR;
        ipcSocket = Q_NULLPTR;
    }


    UniqueApplication::~UniqueApplication() {
    }


    int UniqueApplication::exec() {
        ApplicationStatus status = checkInstance();
        int            exitStatus;

        if (status == PRIMARY_INSTANCE_CONFIGURED) {
            exitStatus = QApplication::exec();

            #if (defined(Q_OS_WIN))

                if (exitStatus == 0) {
                    Application::settings()->setValue("instanceRunning", false);
                }

            #endif

        } else if (status == SECONDARY_INSTANCE_CONFIGURED) {
            exitStatus = 0;
        } else {
            exitStatus = 1;
        }

        return exitStatus;
    }


    void UniqueApplication::secondaryInstanceConnected() {
        Q_ASSERT(ipcSocket == Q_NULLPTR);

        ipcSocket = new QLocalSocket(this);
        ipcSocket = ipcServer->nextPendingConnection();
        ipcSocket->waitForReadyRead(retryDelay);

        QByteArray rawData = ipcSocket->readLine();
        QString s(rawData);
        bool ok = false;
        int numberArguments = s.toInt(&ok);

        QStringList commandLine;
        for (unsigned remainingEntries=numberArguments ; remainingEntries ; --remainingEntries) {
            QByteArray rawData = ipcSocket->readLine();
            QString argument(rawData);
            commandLine << argument.trimmed();
        }

        ipcSocket->disconnectFromServer();
        ipcSocket->deleteLater();
        ipcSocket = Q_NULLPTR;

        emit instanceLoaded(commandLine);
    }


    UniqueApplication::ApplicationStatus UniqueApplication::checkInstance() {
        #if (defined(Q_OS_WIN))

            QString serverName = QApplication::organizationDomain() + "_" + QApplication::applicationName();

        #else

            QString serverName =   QApplication::organizationDomain() + "_" + QApplication::applicationName()
                                 + "." + Util::username();

        #endif

        ApplicationStatus status           = ERROR_REPORTED;
        StartupCondition  startupCondition = NORMAL_START_UP;

        for (
              unsigned attemptsRemaining=connectionRetries+1
            ; attemptsRemaining && (status == ERROR_REPORTED)
            ; --attemptsRemaining
        ) {
            QLocalSocket ipcSocket;
            ipcSocket.connectToServer(serverName);
            if (!ipcSocket.waitForConnected(retryDelay)) {
                QLocalSocket::LocalSocketError errorCode = ipcSocket.error();

                #if (defined(Q_OS_WIN))

                    /* Qt5.7 on Windows reports UnknownSocketError if the server's named pipe can't be opened. */
                    if (errorCode == QLocalSocket::UnknownSocketError) {
                        errorCode = QLocalSocket::ServerNotFoundError;
                    }

                #endif

                if (errorCode == QLocalSocket::ServerNotFoundError) {
                    ipcServer = new QLocalServer(this);
                    ipcServer->setSocketOptions(QLocalServer::UserAccessOption);

                    if (ipcServer->listen(serverName)) {
                        status = PRIMARY_INSTANCE_CONFIGURED;
                        connect(ipcServer, SIGNAL(newConnection()), this, SLOT(secondaryInstanceConnected()));

                        #if (defined(Q_OS_WIN))
                            if (Application::settings()->value("instanceRunning", false).toBool()) {
                                startupCondition = PREVIOUS_INSTANCE_CRASHED;
                            }
                        #endif

                        emit startUserInterface(startupCondition);
                    }
                } else if (errorCode == QLocalSocket::ConnectionRefusedError) {
                    if (!QLocalServer::removeServer(serverName)) {
                        qDebug() << "Attempt to remove server failed.";

                        startupCondition = START_UP_FAILED;
                        status = PRIMARY_INSTANCE_CONFIGURED;

                        emit startUserInterface(startupCondition);
                    } else {
                        startupCondition = PREVIOUS_INSTANCE_CRASHED;
                    }
                } else {
                    qDebug() << "ipcSocket.error() " << errorCode;
                }
            } else {
                QStringList arguments = QApplication::arguments();
                QString s;
                s = QString("%1\n").arg(arguments.size());

                for (auto it=arguments.begin(), end=arguments.end() ; it!=end ; ++it) {
                    s += *it + "\n";
                }

                ipcSocket.write(s.toLocal8Bit());

                ipcSocket.flush();
                ipcSocket.waitForBytesWritten(retryDelay);
                ipcSocket.waitForDisconnected(retryDelay);

                status = SECONDARY_INSTANCE_CONFIGURED;

                emit primaryInstanceAlreadyRunning();
            }
        }

        return status;
    }
}
