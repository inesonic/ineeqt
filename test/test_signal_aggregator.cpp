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
* This file implements tests for the \ref SignalAggregator class.
***********************************************************************************************************************/

#include <QDebug>
#include <QObject>
#include <QtTest/QtTest>
#include <QMetaType>
#include <QByteArray>
#include <QBitArray>
#include <QString>
#include <QStringList>
#include <QChar>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QEventLoop>
#include <QTimer>
#include <QAction>

#include <eqt_signal_aggregator.h>

#include "test_signal_aggregator.h"

/***********************************************************************************************************************
 * Class TestSignalAggregator:
 */

TestSignalAggregator::TestSignalAggregator() {
    eventLoop  = new QEventLoop(this);
    eventTimer = new QTimer(this);

    eventTimer->setSingleShot(true);
    connect(eventTimer, &QTimer::timeout, this, &TestSignalAggregator::timeout);
}


TestSignalAggregator::~TestSignalAggregator() {}


void TestSignalAggregator::timeout() {
    eventLoop->quit();
}


void TestSignalAggregator::handleAggregation0() {
    ++numberEmptyCalls;
}


void TestSignalAggregator::handleAggregation1(const QList<QString>& l1) {
    parameters1 = l1;
}


void TestSignalAggregator::handleAggregation2(const QList<QString>& l1, const QList<QString>& l2) {
    parameters1 = l1;
    parameters2 = l2;
}


void TestSignalAggregator::handleAggregation3(
        const QList<QString>& l1,
        const QList<QString>& l2,
        const QList<QString>& l3
    ) {
    parameters1 = l1;
    parameters2 = l2;
    parameters3 = l3;
}


void TestSignalAggregator::handleAggregation4(
        const QList<QString>& l1,
        const QList<QString>& l2,
        const QList<QString>& l3,
        const QList<QString>& l4
    ) {
    parameters1 = l1;
    parameters2 = l2;
    parameters3 = l3;
    parameters4 = l4;
}


void TestSignalAggregator::testNoParameterCase() {
    EQt::SignalAggregator* aggregator = new EQt::SignalAggregator(this);

    aggregator->registerConnection(this, this, &TestSignalAggregator::handleAggregation0);
    connect(
        this,
        &TestSignalAggregator::trigger0,
        aggregator,
        static_cast<void (EQt::SignalAggregator::*)()>(&EQt::SignalAggregator::trigger<TestSignalAggregator>)
    );

    clearResults();
    emit trigger0();
    emit trigger0();

    eventTimer->start(signalPropagationTime);
    eventLoop->exec();

    QCOMPARE(numberEmptyCalls, 1U);
}


void TestSignalAggregator::testSingleParameterCase() {
    EQt::SignalAggregator* aggregator = new EQt::SignalAggregator(this);

    aggregator->registerConnection(this, this, &TestSignalAggregator::handleAggregation1);
    connect(
        this,
        &TestSignalAggregator::trigger1,
        aggregator,
        static_cast<void (EQt::SignalAggregator::*)(const QString&)>(
            &EQt::SignalAggregator::trigger<TestSignalAggregator>
        )
    );

    clearResults();
    emit trigger1("A");
    emit trigger1("B");

    eventTimer->start(signalPropagationTime);
    eventLoop->exec();

    QCOMPARE(parameters1.size(), 2);
    QCOMPARE(parameters1.at(0), QString("A"));
    QCOMPARE(parameters1.at(1), QString("B"));
}


void TestSignalAggregator::testTwoParameterCase() {
    EQt::SignalAggregator* aggregator = new EQt::SignalAggregator(this);

    aggregator->registerConnection(this, this, &TestSignalAggregator::handleAggregation2);
    connect(
        this,
        &TestSignalAggregator::trigger2,
        aggregator,
        static_cast<void (EQt::SignalAggregator::*)(const QString&, const QString&)>(
            &EQt::SignalAggregator::trigger<TestSignalAggregator>
        )
    );

    clearResults();
    emit trigger2("A", "C");
    emit trigger2("B", "D");

    eventTimer->start(signalPropagationTime);
    eventLoop->exec();

    QCOMPARE(parameters1.size(), 2);
    QCOMPARE(parameters1.at(0), QString("A"));
    QCOMPARE(parameters1.at(1), QString("B"));

    QCOMPARE(parameters2.size(), 2);
    QCOMPARE(parameters2.at(0), QString("C"));
    QCOMPARE(parameters2.at(1), QString("D"));
}


void TestSignalAggregator::testThreeParameterCase() {
    EQt::SignalAggregator* aggregator = new EQt::SignalAggregator(this);

    aggregator->registerConnection(this, this, &TestSignalAggregator::handleAggregation3);
    connect(
        this,
        &TestSignalAggregator::trigger3,
        aggregator,
        static_cast<void (EQt::SignalAggregator::*)(const QString&, const QString&, const QString&)>(
            &EQt::SignalAggregator::trigger<TestSignalAggregator>
        )
    );

    clearResults();
    emit trigger3("A", "C", "E");
    emit trigger3("B", "D", "F");

    eventTimer->start(signalPropagationTime);
    eventLoop->exec();

    QCOMPARE(parameters1.size(), 2);
    QCOMPARE(parameters1.at(0), QString("A"));
    QCOMPARE(parameters1.at(1), QString("B"));

    QCOMPARE(parameters2.size(), 2);
    QCOMPARE(parameters2.at(0), QString("C"));
    QCOMPARE(parameters2.at(1), QString("D"));

    QCOMPARE(parameters3.size(), 2);
    QCOMPARE(parameters3.at(0), QString("E"));
    QCOMPARE(parameters3.at(1), QString("F"));
}


void TestSignalAggregator::testFourParameterCase() {
    EQt::SignalAggregator* aggregator = new EQt::SignalAggregator(this);

    aggregator->registerConnection(this, this, &TestSignalAggregator::handleAggregation4);
    connect(
        this,
        &TestSignalAggregator::trigger4,
        aggregator,
        static_cast<void (EQt::SignalAggregator::*)(const QString&, const QString&, const QString&, const QString&)>(
            &EQt::SignalAggregator::trigger<TestSignalAggregator>
        )
    );

    clearResults();
    emit trigger4("A", "C", "E", "G");
    emit trigger4("B", "D", "F", "H");

    eventTimer->start(signalPropagationTime);
    eventLoop->exec();

    QCOMPARE(parameters1.size(), 2);
    QCOMPARE(parameters1.at(0), QString("A"));
    QCOMPARE(parameters1.at(1), QString("B"));

    QCOMPARE(parameters2.size(), 2);
    QCOMPARE(parameters2.at(0), QString("C"));
    QCOMPARE(parameters2.at(1), QString("D"));

    QCOMPARE(parameters3.size(), 2);
    QCOMPARE(parameters3.at(0), QString("E"));
    QCOMPARE(parameters3.at(1), QString("F"));

    QCOMPARE(parameters4.size(), 2);
    QCOMPARE(parameters4.at(0), QString("G"));
    QCOMPARE(parameters4.at(1), QString("H"));
}


void TestSignalAggregator::clearResults() {
    numberEmptyCalls = 0;
    parameters1.clear();
    parameters2.clear();
    parameters3.clear();
    parameters4.clear();
}
