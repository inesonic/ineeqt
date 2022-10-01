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
* This header provides tests for the \ref UniqueApplication class.
***********************************************************************************************************************/

#ifndef TEST_UNIQUE_APPLICATION_H
#define TEST_UNIQUE_APPLICATION_H

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>
#include <QList>
#include <QStringList>
#include <QProcess>

class QEventLoop;
class QTimer;

class TestUniqueApplication:public QObject {
    Q_OBJECT

    public:
        TestUniqueApplication();

        ~TestUniqueApplication() override;

    protected slots: // protected to keep the test framework from thinking these are test cases.
        void instanceLoaded(QStringList const& parameters);

        void secondInstanceFinished(int exitCode, QProcess::ExitStatus exitStatus);

        void secondInstanceTimedOut();

        void startHelper();

    private slots:
        void testPrimaryInstance();

    private:
        /**
         * Number of helper test iterations to run.
         */
        static const int numberIterations = 8;

        /**
         * Maximum time allowed for the helper application to finish, in mSec.
         */
        static const int helperTimeoutDelay = 30000;

        /**
         * Method that locates the helper executable in the build tree.  Calculates the location based on the location
         * of the test executable.
         *
         * \return Returns the absolute path of the helper executable.
         */
        QString helperExecutable();

        QEventLoop*        eventLoop;
        QTimer*            eventTimer;
        QProcess*          helper;
        unsigned           iterationNumber;
        QList<QStringList> receivedParameters;
        QList<int>         exitCodes;
};

#endif
