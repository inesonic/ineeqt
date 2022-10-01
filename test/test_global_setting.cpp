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
* This file implements tests for the \ref GlobalSetting class.
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

#include <eqt_global_setting.h>

#include "test_global_setting.h"

/***********************************************************************************************************************
 * Class TestGlobalSetting:
 */

TestGlobalSetting::TestGlobalSetting() {
    eventLoop  = new QEventLoop(this);
    eventTimer = new QTimer(this);

    eventTimer->setSingleShot(true);

    simpleAction = new QAction(this);
    toggleAction = new QAction(this);
    toggleAction->setCheckable(true);

    connect(eventTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(simpleAction, SIGNAL(triggered()), this, SLOT(simpleActionTriggered()));
    connect(toggleAction, SIGNAL(toggled(bool)), this, SLOT(toggleActionTriggered(bool)));
}


TestGlobalSetting::~TestGlobalSetting() {
}


void TestGlobalSetting::changeReported() {
    changed = true;
}


void TestGlobalSetting::changeReported(EQt::GlobalSetting* globalSetting) {
    reportedGlobalSetting = globalSetting;
    globalSettingChanged  = true;
}


void TestGlobalSetting::changeReported(int value) {
    reportedInt = value;
    intChanged  = true;
}


void TestGlobalSetting::changeReported(unsigned value) {
    reportedUnsignedInt = value;
    unsignedIntChanged  = true;
}


void TestGlobalSetting::changeReported(long value) {
    reportedLong = value;
    longChanged  = true;
}


void TestGlobalSetting::changeReported(unsigned long value) {
    reportedUnsignedLong = value;
    unsignedLongChanged  = true;
}


void TestGlobalSetting::changeReported(long long value) {
    reportedLongLong = value;
    longLongChanged  = true;
}


void TestGlobalSetting::changeReported(unsigned long long value) {
    reportedUnsignedLongLong = value;
    unsignedLongLongChanged  = true;
}


void TestGlobalSetting::changeReported(float value) {
    reportedFloat = value;
    floatChanged  = true;
}


void TestGlobalSetting::changeReported(double value) {
    reportedDouble = value;
    doubleChanged  = true;
}


void TestGlobalSetting::changeReported(bool value) {
    reportedBool = value;
    boolChanged  = true;
}


void TestGlobalSetting::changeReported(const char* value) {
    reportedConstChar = value;
    constCharChanged  = true;
}


void TestGlobalSetting::changeReported(const QByteArray& value) {
    reportedByteArray = value;
    byteArrayChanged  = true;
}


void TestGlobalSetting::changeReported(const QBitArray& value) {
    reportedBitArray = value;
    bitArrayChanged  = true;
}


void TestGlobalSetting::changeReported(const QString& value) {
    reportedString = value;
    stringChanged  = true;
}


void TestGlobalSetting::changeReported(const QStringList& value) {
    reportedStringList = value;
    stringListChanged  = true;
}


void TestGlobalSetting::changeReported(QChar value) {
    reportedChar = value;
    charChanged  = true;
}


void TestGlobalSetting::changeReported(const QDate& value) {
    reportedDate = value;
    dateChanged  = true;
}


void TestGlobalSetting::changeReported(const QTime& value) {
    reportedTime = value;
    timeChanged  = true;
}


void TestGlobalSetting::changeReported(const QDateTime& value) {
    reportedDateTime = value;
    dateTimeChanged  = true;
}


void TestGlobalSetting::simpleActionTriggered() {
    simpleActionWasTriggered = true;
}


void TestGlobalSetting::toggleActionTriggered(bool nowChecked) {
    reportedActionChecked    = nowChecked;
    toggleActionWasTriggered = true;
}


void TestGlobalSetting::timeout() {
    eventLoop->quit();
}


void TestGlobalSetting::testGlobalDatabase() {
    QCOMPARE(EQt::GlobalSetting::exists("test_setting"), false);
    QCOMPARE(EQt::GlobalSetting::doesNotExist("test_setting"), true);

    {
        EQt::GlobalSetting globalSetting("test_setting");

        QCOMPARE(EQt::GlobalSetting::exists("test_setting"), true);
        QCOMPARE(EQt::GlobalSetting::doesNotExist("test_setting"), false);

        QCOMPARE(EQt::GlobalSetting::setting("test_setting"), &globalSetting);
    }
}


void TestGlobalSetting::testLoadSaveSettings() {
    {
        EQt::GlobalSetting globalSetting("test_setting");
        globalSetting = 5;
        EQt::GlobalSetting::saveSettings();
    }

    {
        QCOMPARE(EQt::GlobalSetting::doesNotExist("test_setting"), true); // Confirms object was destroyed.
        bool success = EQt::GlobalSetting::loadSettings();
        QCOMPARE(success, false); // Should fail because "test_setting" does not exist.
    }

    {
        EQt::GlobalSetting globalSetting("test_setting");

        bool success = EQt::GlobalSetting::loadSettings();
        QCOMPARE(success, true); // Should now pass because "test_setting" now exists.

        bool conversionOk;
        int  value = EQt::GlobalSetting::setting("test_setting")->toInt(&conversionOk);

        QCOMPARE(conversionOk, true);
        QCOMPARE(value, 5);
    }
}


void TestGlobalSetting::testInt() {
    int testValue = 5;
    int reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    bool conversionOk;
    reportedValue = reportedGlobalSetting->toInt(&conversionOk);

    QCOMPARE(conversionOk, true);
    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, true);
    QCOMPARE(reportedInt, testValue);

    if (testValue >= 0) {
        QCOMPARE(unsignedIntChanged, true);
        QCOMPARE(reportedUnsignedInt, static_cast<unsigned int>(testValue));

        QCOMPARE(unsignedLongChanged, true);
        QCOMPARE(reportedUnsignedLong, static_cast<unsigned long>(testValue));

        QCOMPARE(unsignedLongLongChanged, true);
        QCOMPARE(reportedUnsignedLongLong, static_cast<unsigned long long>(testValue));
    } else {
        QCOMPARE(unsignedIntChanged, false);
        QCOMPARE(unsignedLongChanged, false);
        QCOMPARE(unsignedLongLongChanged, false);
    }

    QCOMPARE(longChanged, true);
    QCOMPARE(reportedLong, static_cast<long>(testValue));

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, static_cast<long long>(testValue));

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, static_cast<float>(testValue));

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, static_cast<double>(testValue));

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString::number(testValue));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, QString::number(testValue).toUtf8());

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString::number(testValue));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testUnsigned() {
    unsigned testValue = 6;
    unsigned reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    bool conversionOk;
    reportedValue = reportedGlobalSetting->toUInt(&conversionOk);

    QCOMPARE(conversionOk, true);
    QCOMPARE(reportedValue, testValue);

    QCOMPARE(unsignedIntChanged, true);
    QCOMPARE(reportedUnsignedInt, testValue);

    if (testValue <= static_cast<int>(static_cast<unsigned int>(-1) >> 1)) {
        QCOMPARE(intChanged, true);
        QCOMPARE(reportedInt, static_cast<int>(testValue));
    } else {
        QCOMPARE(intChanged, false);
    }

    QCOMPARE(longChanged, true);
    QCOMPARE(reportedLong, static_cast<long>(testValue));

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, static_cast<long long>(testValue));

    QCOMPARE(unsignedLongChanged, true);
    QCOMPARE(reportedUnsignedLong, static_cast<unsigned long>(testValue));

    QCOMPARE(unsignedLongLongChanged, true);
    QCOMPARE(reportedUnsignedLongLong, static_cast<unsigned long long>(testValue));

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, static_cast<float>(testValue));

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, static_cast<double>(testValue));

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString::number(testValue));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, QString::number(testValue).toUtf8());

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString::number(testValue));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testLong() {
    long testValue = 5;
    long reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    bool conversionOk;
    reportedValue = static_cast<long>(reportedGlobalSetting->toLongLong(&conversionOk));

    QCOMPARE(conversionOk, true);
    QCOMPARE(reportedValue, testValue);

    if (testValue <= static_cast<int>(static_cast<unsigned int>(-1) >> 1)    &&
        testValue >= static_cast<int>(~(static_cast<unsigned int>(-1) >> 1))    ) {
        QCOMPARE(intChanged, true);
        QCOMPARE(static_cast<long>(reportedInt), testValue);
    } else {
        QCOMPARE(intChanged, false);
    }

    if (testValue >= 0) {
        if (testValue <= static_cast<long long>(static_cast<unsigned int>(-1))) {
            QCOMPARE(unsignedIntChanged, true);
            QCOMPARE(reportedUnsignedInt, static_cast<unsigned int>(testValue));
        } else {
            QCOMPARE(unsignedIntChanged, false);
        }

        QCOMPARE(unsignedLongChanged, true);
        QCOMPARE(reportedUnsignedLong, static_cast<unsigned long>(testValue));

        QCOMPARE(unsignedLongLongChanged, true);
        QCOMPARE(reportedUnsignedLongLong, static_cast<unsigned long long>(testValue));
    } else {
        QCOMPARE(unsignedIntChanged, false);
        QCOMPARE(unsignedLongChanged, false);
        QCOMPARE(unsignedLongLongChanged, false);
    }

    QCOMPARE(longChanged, true);
    QCOMPARE(reportedLong, static_cast<long>(testValue));

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, static_cast<long long>(testValue));

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, static_cast<float>(testValue));

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, static_cast<double>(testValue));

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString::number(testValue));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, QString::number(testValue).toUtf8());

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString::number(testValue));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(static_cast<long long>(testValue)));
}


void TestGlobalSetting::testUnsignedLong() {
    unsigned long testValue = 7;
    unsigned long reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    bool conversionOk;
    reportedValue = reportedGlobalSetting->toULongLong(&conversionOk);

    QCOMPARE(conversionOk, true);
    QCOMPARE(reportedValue, testValue);

    if (testValue <= static_cast<unsigned int>(-1)) {
        QCOMPARE(unsignedIntChanged, true);
        QCOMPARE(static_cast<unsigned long>(reportedUnsignedInt), testValue);
    } else {
        QCOMPARE(unsignedIntChanged, false);
    }

    if (testValue <= static_cast<int>(static_cast<unsigned int>(-1L) >> 1)) {
        QCOMPARE(intChanged, true);
        QCOMPARE(reportedInt, static_cast<int>(testValue));
    } else {
        QCOMPARE(intChanged, false);
    }

    if (testValue <= static_cast<long>(static_cast<unsigned long>(-1L) >> 1)) {
        QCOMPARE(longChanged, true);
        QCOMPARE(reportedLong, static_cast<long>(testValue));
    } else {
        QCOMPARE(longChanged, false);
    }

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, static_cast<long long>(testValue));

    QCOMPARE(unsignedLongChanged, true);
    QCOMPARE(reportedUnsignedLong, static_cast<unsigned long>(testValue));

    QCOMPARE(unsignedLongLongChanged, true);
    QCOMPARE(reportedUnsignedLongLong, static_cast<unsigned long long>(testValue));

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, static_cast<float>(testValue));

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, static_cast<double>(testValue));

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString::number(testValue));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, QString::number(testValue).toUtf8());

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString::number(testValue));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(static_cast<unsigned long long>(testValue)));
}


void TestGlobalSetting::testLongLong() {
    long long testValue = 8;
    long long reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    bool conversionOk;
    reportedValue = reportedGlobalSetting->toLongLong(&conversionOk);

    QCOMPARE(conversionOk, true);
    QCOMPARE(reportedValue, testValue);

    if (testValue >= 0) {
        if (sizeof(long long) == sizeof(unsigned int)                          ||
            testValue <= static_cast<long long>(static_cast<unsigned int>(-1))    ) {
            QCOMPARE(unsignedIntChanged, true);
            QCOMPARE(reportedUnsignedInt, static_cast<unsigned int>(testValue));
        } else {
            QCOMPARE(unsignedIntChanged, false);
        }

        if (sizeof(long long) == sizeof(unsigned long)                           ||
            testValue <= static_cast<long long>(static_cast<unsigned long>(-1L))    ) {
            QCOMPARE(unsignedLongChanged, true);
            QCOMPARE(reportedUnsignedLong, static_cast<unsigned long>(testValue));
        } else {
            QCOMPARE(unsignedLongChanged, false);
        }

        QCOMPARE(unsignedLongLongChanged, true);
        QCOMPARE(reportedUnsignedLongLong, static_cast<unsigned long long>(testValue));
    } else {
        QCOMPARE(unsignedIntChanged, false);
        QCOMPARE(unsignedLongChanged, false);
        QCOMPARE(unsignedLongLongChanged, false);
    }

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, testValue);

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, static_cast<float>(testValue));

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, static_cast<double>(testValue));

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString::number(testValue));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, QString::number(testValue).toUtf8());

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString::number(testValue));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testUnsignedLongLong() {
    unsigned long long testValue = 9;
    unsigned long long reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    bool conversionOk;
    reportedValue = reportedGlobalSetting->toULongLong(&conversionOk);

    QCOMPARE(conversionOk, true);
    QCOMPARE(reportedValue, testValue);

    if (testValue <= static_cast<unsigned int>(-1)) {
        QCOMPARE(unsignedIntChanged, true);
        QCOMPARE(static_cast<unsigned long long>(reportedUnsignedInt), testValue);
    } else {
        QCOMPARE(unsignedIntChanged, false);
    }

    if (testValue <= static_cast<int>(static_cast<unsigned int>(-1L) >> 1)) {
        QCOMPARE(intChanged, true);
        QCOMPARE(reportedInt, static_cast<int>(testValue));
    } else {
        QCOMPARE(intChanged, false);
    }

    if (testValue <= static_cast<unsigned long>(-1)) {
        QCOMPARE(unsignedLongChanged, true);
        QCOMPARE(static_cast<unsigned long long>(reportedUnsignedLong), testValue);
    } else {
        QCOMPARE(unsignedLongChanged, false);
    }

    if (testValue <= static_cast<long>(static_cast<unsigned long>(-1L) >> 1)) {
        QCOMPARE(longChanged, true);
        QCOMPARE(reportedLong, static_cast<long>(testValue));
    } else {
        QCOMPARE(longChanged, false);
    }

    if (testValue <= static_cast<long long>(static_cast<unsigned long long>(-1L) >> 1)) {
        QCOMPARE(longLongChanged, true);
        QCOMPARE(reportedLongLong, static_cast<long long>(testValue));
    } else {
        QCOMPARE(longLongChanged, false);
    }

    QCOMPARE(unsignedLongLongChanged, true);
    QCOMPARE(reportedUnsignedLongLong, testValue);

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, static_cast<float>(testValue));

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, static_cast<double>(testValue));

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString::number(testValue));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, QString::number(testValue).toUtf8());

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString::number(testValue));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testFloat() {
    float testValue = static_cast<float>(-3.52);
    float reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    bool conversionOk;
    reportedValue = reportedGlobalSetting->toReal(&conversionOk);

    QCOMPARE(conversionOk, true);
    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, true); // Rounding
    QCOMPARE(reportedInt, round<int>(testValue));

    QCOMPARE(longChanged, true);
    QCOMPARE(reportedLong, round<long>(testValue));

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, round<long long>(testValue));

    // What QVariant does for negative values is quite silly and platform specific.  For now we ignore.

    QCOMPARE(unsignedIntChanged, true);
    QCOMPARE(reportedUnsignedInt, round<unsigned int>(testValue));

    QCOMPARE(unsignedLongLongChanged, true);
    QCOMPARE(reportedUnsignedLongLong, round<unsigned long long>(testValue));

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, testValue);

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, static_cast<double>(testValue));

    QByteArray valueAsArray = QVariant(testValue).toByteArray();

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString(valueAsArray));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, valueAsArray);

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString(valueAsArray));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testDouble() {
    double testValue = -3.52;
    double reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    bool conversionOk;
    reportedValue = reportedGlobalSetting->toDouble(&conversionOk);

    QCOMPARE(conversionOk, true);
    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, true); // Rounding
    QCOMPARE(reportedInt, round<int>(testValue));

    QCOMPARE(longChanged, true);
    QCOMPARE(reportedLong, round<long>(testValue));

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, round<long long>(testValue));

    // What QVariant does for negative values is quite silly and platform specific.

    QCOMPARE(unsignedIntChanged, true);
    QCOMPARE(reportedUnsignedInt, round<unsigned int>(testValue));

    QCOMPARE(unsignedLongLongChanged, true);
    QCOMPARE(reportedUnsignedLongLong, round<unsigned long long>(testValue));

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, static_cast<float>(testValue));

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, testValue);

    QByteArray valueAsArray = QVariant(testValue).toByteArray();

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString(valueAsArray));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, valueAsArray);

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString(valueAsArray));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testBool() {
    bool testValue = true;
    bool reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, true);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toBool();

    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, true); // Rounding
    QCOMPARE(reportedInt, static_cast<int>(testValue));

    QCOMPARE(longChanged, true);
    QCOMPARE(reportedLong, static_cast<long>(testValue));

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, static_cast<long long>(testValue));

    QCOMPARE(unsignedIntChanged, true);
    QCOMPARE(reportedUnsignedInt, static_cast<unsigned int>(testValue));

    QCOMPARE(unsignedLongChanged, true);
    QCOMPARE(reportedUnsignedLong, static_cast<unsigned long>(testValue));

    QCOMPARE(unsignedLongLongChanged, true);
    QCOMPARE(reportedUnsignedLongLong, static_cast<unsigned long long>(testValue));

    QCOMPARE(floatChanged, true);
    QCOMPARE(reportedFloat, static_cast<float>(testValue));

    QCOMPARE(doubleChanged, true);
    QCOMPARE(reportedDouble, static_cast<double>(testValue));

    QString stringValue = testValue ? "true" : "false";

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, stringValue);

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, stringValue.toUtf8());

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, stringValue);

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(toggleActionWasTriggered, true);
    QCOMPARE(toggleAction->isChecked(), testValue);
}


void TestGlobalSetting::testByteArray() {
    QByteArray testValue = QString("A test string").toUtf8();
    QByteArray reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toByteArray();

    QCOMPARE(reportedValue, testValue);

    // The correct byte array might cause true to be returned for many of the tests below.

    QCOMPARE(intChanged, false);
    QCOMPARE(unsignedIntChanged, false);

    QCOMPARE(longChanged, false);
    QCOMPARE(unsignedLongChanged, false);

    QCOMPARE(longLongChanged, false);
    QCOMPARE(unsignedLongLongChanged, false);

    QCOMPARE(floatChanged, false);
    QCOMPARE(doubleChanged, false);

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, QString(testValue));

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, testValue);

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, QString(testValue));

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testBitArray() {
    QBitArray testValue(5, false);

    testValue.setBit(0);
    testValue.setBit(2);
    testValue.setBit(4);

    QBitArray reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toBitArray();

    QCOMPARE(reportedValue, testValue);

    // The correct bit array might cause true to be returned for many of the tests below.

    QCOMPARE(intChanged, false);
    QCOMPARE(unsignedIntChanged, false);

    QCOMPARE(longChanged, false);
    QCOMPARE(unsignedLongChanged, false);

    QCOMPARE(longLongChanged, false);
    QCOMPARE(unsignedLongLongChanged, false);

    QCOMPARE(floatChanged, false);
    QCOMPARE(doubleChanged, false);

    // String and byte array report empty values.  Since these are nonsensical, we ignore them.

    QCOMPARE(bitArrayChanged, true);
    QCOMPARE(reportedBitArray, testValue);

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testString() {
    QString testValue("Another test string");
    QString reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toString();

    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, false);
    QCOMPARE(unsignedIntChanged, false);

    QCOMPARE(longChanged, false);
    QCOMPARE(unsignedLongChanged, false);

    QCOMPARE(longLongChanged, false);
    QCOMPARE(unsignedLongLongChanged, false);

    QCOMPARE(floatChanged, false);
    QCOMPARE(doubleChanged, false);

    QCOMPARE(constCharChanged, true);
    QCOMPARE(reportedConstChar, testValue);

    QCOMPARE(byteArrayChanged, true);
    QCOMPARE(reportedByteArray, testValue.toUtf8());

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, testValue);

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testStringList() {
    QStringList testValue;
    testValue << "A string" << "Another string" << "And yet a third string.";

    QStringList reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toStringList();

    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, false);
    QCOMPARE(unsignedIntChanged, false);

    QCOMPARE(longChanged, false);
    QCOMPARE(unsignedLongChanged, false);

    QCOMPARE(longLongChanged, false);
    QCOMPARE(unsignedLongLongChanged, false);

    QCOMPARE(floatChanged, false);
    QCOMPARE(doubleChanged, false);

    // The QVariant class reports successfull conversion to string, byte array, and const char* but the resulting values
    // are empty.  We ignore them here as the conversions don't make much sense.

    QCOMPARE(stringListChanged, true);
    QCOMPARE(reportedStringList, testValue);

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testChar() {
    QChar testValue('X');
    QChar reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toChar();

    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, true);
    QCOMPARE(reportedInt, static_cast<int>(testValue.unicode()));

    QCOMPARE(unsignedIntChanged, true);
    QCOMPARE(reportedUnsignedInt, static_cast<unsigned int>(testValue.unicode()));

    QCOMPARE(longChanged, true);
    QCOMPARE(reportedLong, static_cast<long>(testValue.unicode()));

    QCOMPARE(unsignedLongChanged, true);
    QCOMPARE(reportedUnsignedLong, static_cast<unsigned long>(testValue.unicode()));

    QCOMPARE(longLongChanged, true);
    QCOMPARE(reportedLongLong, static_cast<long long>(testValue.unicode()));

    QCOMPARE(unsignedLongLongChanged, true);
    QCOMPARE(reportedUnsignedLongLong, static_cast<unsigned long long>(testValue.unicode()));

    QCOMPARE(floatChanged, false);
    QCOMPARE(doubleChanged, false);

    // The string and byte array conversions from QChar are nonsensical so we'll ignore them.

    QCOMPARE(charChanged, true);
    QCOMPARE(reportedChar, testValue);

    // Note: We don't test against signals with values that are nonsensical.  The QVariant class, in theory, allows
    //       several odd conversions (e.g. int to QBitArray) that end up returning empty or invalid data.  The only
    //       way we could realistically handle these would be to register a set of accepted QMetaType::Type values with
    //       each instance.

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testDate() {
    QDate testValue = QDate::currentDate();
    QDate reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toDate();

    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, false);
    QCOMPARE(unsignedIntChanged, false);

    QCOMPARE(longChanged, false);
    QCOMPARE(unsignedLongChanged, false);

    QCOMPARE(longLongChanged, false);
    QCOMPARE(unsignedLongLongChanged, false);

    QCOMPARE(floatChanged, false);
    QCOMPARE(doubleChanged, false);

    // String, char, and string list types report as changed but contain empty values.  Since these conversions are
    // nonsensical, we ignore them.

    QCOMPARE(dateChanged, true);
    QCOMPARE(reportedDate, testValue);

    // A time value is reported but with an invalid value.  Again, we'll ignore this conversion.

    QCOMPARE(dateTimeChanged, true);
    QCOMPARE(reportedDateTime, testValue.startOfDay());

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testTime() {
    QTime testValue = QTime::currentTime();
    QTime reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toTime();

    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, false);
    QCOMPARE(unsignedIntChanged, false);

    QCOMPARE(longChanged, false);
    QCOMPARE(unsignedLongChanged, false);

    QCOMPARE(longLongChanged, false);
    QCOMPARE(unsignedLongLongChanged, false);

    QCOMPARE(floatChanged, false);
    QCOMPARE(doubleChanged, false);

    // String, char, and string list types report as changed but contain empty values.  Since these conversions are
    // nonsensical, we ignore them.

    // Date values also report as changed but are invalid.

    QCOMPARE(timeChanged, true);
    QCOMPARE(reportedTime, testValue);

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testDateTime() {
    QDateTime testValue = QDateTime::currentDateTimeUtc();
    QDateTime reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    globalSetting = testValue;
    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toDateTime();

    QCOMPARE(reportedValue, testValue);

    QCOMPARE(intChanged, false);
    QCOMPARE(unsignedIntChanged, false);

    QCOMPARE(longChanged, false);
    QCOMPARE(unsignedLongChanged, false);

    QCOMPARE(longLongChanged, false);
    QCOMPARE(unsignedLongLongChanged, false);

    QCOMPARE(floatChanged, false);
    QCOMPARE(doubleChanged, false);

    // String, char, and string list types report as changed but contain empty values.  Since these conversions are
    // nonsensical, we ignore them.

    QCOMPARE(dateChanged, true);
    QCOMPARE(reportedDate, testValue.date());

    QCOMPARE(timeChanged, true);
    QCOMPARE(reportedTime, testValue.time());

    QCOMPARE(dateTimeChanged, true);
    QCOMPARE(reportedDateTime, testValue);

    QCOMPARE(simpleActionWasTriggered, true);
    QCOMPARE(simpleAction->data(), QVariant(testValue));
}


void TestGlobalSetting::testSimpleAction() {
    QString testValue("A crazy test string");
    QString reportedValue;

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting");
    configureSettingObject(&globalSetting, false);

    simpleAction->setData(testValue);
    simpleAction->trigger();

    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    reportedValue = reportedGlobalSetting->toString();
    QCOMPARE(reportedValue, testValue);

    QCOMPARE(stringChanged, true);
    QCOMPARE(reportedString, testValue);
}


void TestGlobalSetting::testToggleAction() {
    bool testValue = !toggleAction->isChecked();

    clearStatus();

    EQt::GlobalSetting globalSetting("test_setting", !testValue);
    configureSettingObject(&globalSetting, true);

    toggleAction->setChecked(testValue);

    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    QCOMPARE(globalSetting.toBool(), testValue);

    QCOMPARE(boolChanged, true);
    QCOMPARE(reportedBool, testValue);

    clearStatus();
    toggleAction->setChecked(!testValue);

    eventTimer->start(signalPropagationTime); // Give a little time for things to actually happen.
    eventLoop->exec();

    QCOMPARE(changed, true);

    QCOMPARE(globalSettingChanged, true);
    QCOMPARE(reportedGlobalSetting, &globalSetting);

    QCOMPARE(globalSetting.toBool(), !testValue);

    QCOMPARE(boolChanged, true);
    QCOMPARE(reportedBool, !testValue);
}


void TestGlobalSetting::clearStatus() {
    changed                  = false;
    globalSettingChanged     = false;
    intChanged               = false;
    unsignedIntChanged       = false;
    longChanged              = false;
    unsignedLongChanged      = false;
    longLongChanged          = false;
    unsignedLongLongChanged  = false;
    floatChanged             = false;
    doubleChanged            = false;
    boolChanged              = false;
    constCharChanged         = false;
    byteArrayChanged         = false;
    bitArrayChanged          = false;
    stringChanged            = false;
    stringListChanged        = false;
    charChanged              = false;
    dateChanged              = false;
    timeChanged              = false;
    dateTimeChanged          = false;
    toggleActionWasTriggered = false;
    simpleActionWasTriggered = false;
}


void TestGlobalSetting::configureSettingObject(EQt::GlobalSetting* const globalSetting, bool const useToggleAction) {
    connect(globalSetting, SIGNAL(changed()), this, SLOT(changeReported()));
    connect(
        globalSetting,
        static_cast<void (EQt::GlobalSetting::*)(EQt::GlobalSetting*)>(&EQt::GlobalSetting::changed),
        this,
        static_cast<void (TestGlobalSetting::*)(EQt::GlobalSetting*)>(&TestGlobalSetting::changeReported)
    );
    connect(globalSetting, SIGNAL(changed(int)), this, SLOT(changeReported(int)));
    connect(globalSetting, SIGNAL(changed(unsigned)), this, SLOT(changeReported(unsigned)));
    connect(globalSetting, SIGNAL(changed(long)), this, SLOT(changeReported(long)));
    connect(globalSetting, SIGNAL(changed(unsigned long)), this, SLOT(changeReported(unsigned long)));
    connect(globalSetting, SIGNAL(changed(long long)), this, SLOT(changeReported(long long)));
    connect(globalSetting, SIGNAL(changed(unsigned long long)), this, SLOT(changeReported(unsigned long long)));
    connect(globalSetting, SIGNAL(changed(float)), this, SLOT(changeReported(float)));
    connect(globalSetting, SIGNAL(changed(double)), this, SLOT(changeReported(double)));
    connect(globalSetting, SIGNAL(changed(bool)), this, SLOT(changeReported(bool)));
    connect(globalSetting, SIGNAL(changed(const char*)), this, SLOT(changeReported(const char*)));
    connect(globalSetting, SIGNAL(changed(const QByteArray)), this, SLOT(changeReported(const QByteArray)));
    connect(globalSetting, SIGNAL(changed(const QBitArray)), this, SLOT(changeReported(const QBitArray)));
    connect(globalSetting, SIGNAL(changed(const QString)), this, SLOT(changeReported(const QString)));
    connect(globalSetting, SIGNAL(changed(const QStringList)), this, SLOT(changeReported(const QStringList)));
    connect(globalSetting, SIGNAL(changed(QChar)), this, SLOT(changeReported(QChar)));
    connect(globalSetting, SIGNAL(changed(const QDate)), this, SLOT(changeReported(const QDate)));
    connect(globalSetting, SIGNAL(changed(const QTime)), this, SLOT(changeReported(const QTime)));
    connect(globalSetting, SIGNAL(changed(const QDateTime)), this, SLOT(changeReported(const QDateTime)));

    if (useToggleAction) {
        globalSetting->associateAction(toggleAction);
    } else {
        globalSetting->associateAction(simpleAction);
    }
}
