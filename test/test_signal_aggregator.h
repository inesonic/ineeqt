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
* This header provides tests for the \ref SignalAggregator class.
***********************************************************************************************************************/

#ifndef TEST_SIGNAL_AGGREGATOR_H
#define TEST_SIGNAL_AGGREGATOR_H

#include <QtGlobal>
#include <QObject>
#include <QtTest/QtTest>
#include <QStringList>

class QEventLoop;
class QTimer;

namespace EQt {
    class SignalAggregator;
}

class TestSignalAggregator:public QObject {
    Q_OBJECT

    public:
        TestSignalAggregator();

        ~TestSignalAggregator() override;

    signals:
        void trigger0();

        void trigger1(const QString& p1);

        void trigger2(const QString& p1, const QString& p2);

        void trigger3(const QString& p1, const QString& p2, const QString& p3);

        void trigger4(const QString& p1, const QString& p2, const QString& p3, const QString& p4);

    protected slots: // protected to keep the test framework from thinking these are test cases.
        void timeout();

        void handleAggregation0();
        void handleAggregation1(const QList<QString>& l1);
        void handleAggregation2(const QList<QString>& l1, const QList<QString>& l2);
        void handleAggregation3(const QList<QString>& l1, const QList<QString>& l2, const QList<QString>& l3);
        void handleAggregation4(
            const QList<QString>& l1,
            const QList<QString>& l2,
            const QList<QString>& l3,
            const QList<QString>& l4
        );

    private slots:
//        void initTestCase();

        void testNoParameterCase();
        void testSingleParameterCase();
        void testTwoParameterCase();
        void testThreeParameterCase();
        void testFourParameterCase();

//        void cleanupTestCase();

    private:
        void clearResults();

        static constexpr unsigned signalPropagationTime = 5;

        QEventLoop* eventLoop;
        QTimer*     eventTimer;

        unsigned    numberEmptyCalls;
        QStringList parameters1;
        QStringList parameters2;
        QStringList parameters3;
        QStringList parameters4;
};

#endif
