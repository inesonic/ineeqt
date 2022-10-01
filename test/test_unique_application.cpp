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
* This file implements tests for the \ref UniqueApplication class.
***********************************************************************************************************************/

#include <QDebug>
#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QDir>
#include <QList>
#include <QVariant>
#include <QEventLoop>
#include <QTimer>
#include <QProcess>

#include <eqt_application.h>
#include <eqt_unique_application.h>

#include "test_unique_application.h"


const int TestUniqueApplication::numberIterations;


TestUniqueApplication::TestUniqueApplication() {
    // We setup our spies here so we can catch signals right when the event loop starts.
    EQt::UniqueApplication* application = dynamic_cast<EQt::UniqueApplication*>(EQt::Application::instance());

    eventLoop = new QEventLoop(this);
    eventTimer = new QTimer(this);
    helper = new QProcess(this);

    connect(
        application,
        SIGNAL(instanceLoaded(QStringList)),
        this,
        SLOT(instanceLoaded(QStringList))
    );
    connect(eventTimer, SIGNAL(timeout()), this, SLOT(secondInstanceTimedOut()));
    connect(
        helper,
        SIGNAL(finished(int, QProcess::ExitStatus)),
        this,
        SLOT(secondInstanceFinished(int,QProcess::ExitStatus))
    );
}


TestUniqueApplication::~TestUniqueApplication() {
    disconnect(this, SLOT(instanceLoaded(QStringList)));
}


void TestUniqueApplication::instanceLoaded(QStringList const& parameters) {
    receivedParameters.append(parameters);
}


void TestUniqueApplication::secondInstanceFinished(int exitCode, QProcess::ExitStatus) {
    eventTimer->stop();
    exitCodes.append(exitCode);

    iterationNumber++;
    if (iterationNumber < numberIterations) {
        startHelper();
    } else {
        eventLoop->quit();
    }
}


void TestUniqueApplication::secondInstanceTimedOut() {
    if (helper->state() == QProcess::Running) {
        helper->kill();
        exitCodes.append(-1);
    } else {
        exitCodes.append(-2);
    }

    eventLoop->quit();
}


void TestUniqueApplication::startHelper() {
    QStringList parameters;
    for (unsigned p=0 ; p<iterationNumber ; ++p) {
        parameters << ("a" + QString::number(p+1));
    }

    QString executablePath = helperExecutable();
    helper->start(executablePath, parameters);
    helper->waitForStarted();

    QProcess::ProcessState helperState = helper->state();
    Q_ASSERT(helperState == QProcess::Running);

    eventTimer->setSingleShot(true);
    eventTimer->start(helperTimeoutDelay);
}


void TestUniqueApplication::testPrimaryInstance() {
    iterationNumber = 0;
    QTimer::singleShot(1, this, SLOT(startHelper()));
    eventLoop->exec();

    QCOMPARE(exitCodes.count(), numberIterations);
    QCOMPARE(receivedParameters.count(), numberIterations);

    for (int i=0 ; i<numberIterations ; ++i) {
        QStringList& received = receivedParameters[i];
        QCOMPARE(received.count(), i+1);
        for (int p=0 ; p<i ; ++p) {
            QString e = "a" + QString::number(p+1);
            QString r = received[p+1];
            QCOMPARE(r, e);
        }

        QCOMPARE(exitCodes[i], 0);
    }
}


QString TestUniqueApplication::helperExecutable() {
    #if (defined(Q_OS_LINUX))

        QString relativePath = "../../../ineeqt_helpers/test_unique_application_helper/build/";

        #if (defined(QT_DEBUG))

            relativePath += "debug/";

        #else

            relativePath += "release/";

        #endif

        relativePath += "test_unique_application_helper";

    #elif (defined(Q_OS_WIN))

        QString relativePath = "../../../ineeqt_helpers/test_unique_application_helper/build/";

        #if (defined(QT_DEBUG))

            relativePath += "Debug/";

        #else

            relativePath += "Release/";

        #endif

        relativePath += "test_unique_application_helper";

    #elif (defined(Q_OS_DARWIN))

        QString relativePath = "../../../../../../ineeqt_helpers/test_unique_application_helper/build/";

        #if (defined(QT_DEBUG))

            relativePath += "debug/";

        #else

            relativePath += "release/";

        #endif

        relativePath += "test_unique_application_helper.app/Contents/MacOS/test_unique_application_helper";

    #else

        #error Unknown platform.

    #endif

    QDir testDirectory(QCoreApplication::applicationDirPath());
    return testDirectory.absoluteFilePath(relativePath);
}
