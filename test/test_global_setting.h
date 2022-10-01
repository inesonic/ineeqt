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
* This header provides tests for the \ref GlobalSetting class.
***********************************************************************************************************************/

#ifndef TEST_GLOBAL_SETTING_H
#define TEST_GLOBAL_SETTING_H

#include <QtGlobal>
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
#include <QAction>

class QEventLoop;
class QTimer;

namespace EQt {
    class GlobalSetting;
}

class TestGlobalSetting:public QObject {
    Q_OBJECT

    public:
        TestGlobalSetting();

        ~TestGlobalSetting() override;

    protected slots: // protected to keep the test framework from thinking these are test cases.
        void changeReported();
        void changeReported(EQt::GlobalSetting* globalSetting);
        void changeReported(int value);
        void changeReported(unsigned value);
        void changeReported(long value);
        void changeReported(unsigned long value);
        void changeReported(long long value);
        void changeReported(unsigned long long value);
        void changeReported(float value);
        void changeReported(double value);
        void changeReported(bool value);
        void changeReported(const char* value);
        void changeReported(const QByteArray& value);
        void changeReported(const QBitArray& value);
        void changeReported(const QString& value);
        void changeReported(const QStringList& value);
        void changeReported(QChar value);
        void changeReported(const QDate& value);
        void changeReported(const QTime& value);
        void changeReported(const QDateTime& value);

        void simpleActionTriggered();
        void toggleActionTriggered(bool nowChecked);

        void timeout();

    private slots:
//        void initTestCase();

        void testGlobalDatabase();
        void testLoadSaveSettings();
        void testInt();
        void testUnsigned();
        void testLong();
        void testUnsignedLong();
        void testLongLong();
        void testUnsignedLongLong();
        void testFloat();
        void testDouble();
        void testBool();
        void testByteArray();
        void testBitArray();
        void testString();
        void testStringList();
        void testChar();
        void testDate();
        void testTime();
        void testDateTime();
        void testSimpleAction();
        void testToggleAction();

//        void cleanupTestCase();

    private:
        static constexpr unsigned signalPropagationTime = 5;

        void clearStatus();
        void configureSettingObject(EQt::GlobalSetting* const globalSetting, bool const useToggleAction);

        template<typename R> static R round(double const value) {
            if (value < 0) {
                return static_cast<R>(value - 0.5);
            } else {
                return static_cast<R>(value + 0.5);
            }
        }

        QEventLoop*         eventLoop;
        QTimer*             eventTimer;

        QAction*            simpleAction;
        QAction*            toggleAction;

        bool                changed; // Indicates changed() signal emitted.

        bool                globalSettingChanged;
        EQt::GlobalSetting* reportedGlobalSetting;

        bool                intChanged;
        int                 reportedInt;

        bool                unsignedIntChanged;
        unsigned            reportedUnsignedInt;

        bool                longChanged;
        long                reportedLong;

        bool                unsignedLongChanged;
        unsigned long       reportedUnsignedLong;

        bool                longLongChanged;
        long long           reportedLongLong;

        bool                unsignedLongLongChanged;
        unsigned long long  reportedUnsignedLongLong;

        bool                floatChanged;
        float               reportedFloat;

        bool                doubleChanged;
        double              reportedDouble;

        bool                boolChanged;
        bool                reportedBool;

        bool                constCharChanged;
        QString             reportedConstChar;

        bool                byteArrayChanged;
        QByteArray          reportedByteArray;

        bool                bitArrayChanged;
        QBitArray           reportedBitArray;

        bool                stringChanged;
        QString             reportedString;

        bool                stringListChanged;
        QStringList         reportedStringList;

        bool                charChanged;
        QChar               reportedChar;

        bool                dateChanged;
        QDate               reportedDate;

        bool                timeChanged;
        QTime               reportedTime;

        bool                dateTimeChanged;
        QDateTime           reportedDateTime;

        bool                toggleActionWasTriggered;
        bool                reportedActionChecked;

        bool                simpleActionWasTriggered;
};

#endif
