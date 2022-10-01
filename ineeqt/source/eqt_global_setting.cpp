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
* This file implements the \ref EQt::GlobalSetting class.
***********************************************************************************************************************/

#include <QObject>
#include <QString>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QMap>
#include <QByteArray>
#include <QBitArray>
#include <QStringList>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QSettings>

#include "eqt_application.h"
#include "eqt_global_setting.h"

namespace EQt {
    QMap<QString, GlobalSetting*> GlobalSetting::globalSettings;

    GlobalSetting::GlobalSetting(const QString& name, QObject* parent):QObject(parent) {
        configureObject(name);
    }


    GlobalSetting::GlobalSetting(const QString& name):QObject(QApplication::instance()) {
        configureObject(name);
    }


    GlobalSetting::GlobalSetting(
            const QString&  name,
            const QVariant& defaultValue,
            QObject*        parent
        ):QObject(
             parent
        ),QVariant(
             defaultValue
        ) {
        configureObject(name);
    }


    GlobalSetting::GlobalSetting(
            const QString&  name,
            const QVariant& defaultValue
        ):QObject(
            QApplication::instance()
        ),QVariant(
            defaultValue
        ) {
        configureObject(name);
    }


    GlobalSetting::~GlobalSetting() {
        int count = globalSettings.remove(currentName);
        Q_ASSERT(count == 1);
    }


    QString GlobalSetting::name() const {
        return currentName;
    }


    void GlobalSetting::associateAction(QAction* newAction) {
        Q_ASSERT(!actions.contains(newAction));

        actions.append(newAction);

        connect(newAction, SIGNAL(triggered()), this, SLOT(actionTriggered()));
        connect(newAction, SIGNAL(toggled(bool)), this, SLOT(actionToggled(bool)));
    }


    void GlobalSetting::disassociateAction(QAction* oldAction) {
        Q_ASSERT(actions.contains(oldAction));

        disconnect(oldAction, SIGNAL(triggered()), this, SLOT(actionTriggered()));
        disconnect(oldAction, SIGNAL(toggled(bool)), this, SLOT(actionToggled(bool)));

        actions.removeAll(oldAction);
    }


    GlobalSetting& GlobalSetting::operator=(const QVariant& newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(int newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(unsigned newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(long newValue) {
        setValue(QVariant(static_cast<qlonglong>(newValue)));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(unsigned long newValue) {
        setValue(QVariant(static_cast<qulonglong>(newValue)));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(long long newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(unsigned long long newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(float newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(double newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(bool newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const char* newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QByteArray& newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QBitArray& newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QString& newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QStringList& newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(QChar newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QDate& newValue) {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QTime& newValue)  {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QDateTime& newValue)  {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QPoint& newValue)  {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QPointF& newValue)  {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QSize& newValue)  {
        setValue(QVariant(newValue));
        return *this;
    }


    GlobalSetting& GlobalSetting::operator=(const QSizeF& newValue)  {
        setValue(QVariant(newValue));
        return *this;
    }


    bool GlobalSetting::loadSettings(const QString& groupName) {
        QSettings* settings = Application::settings();

        settings->beginGroup(groupName);

        QStringList keys                 = settings->allKeys();
        bool        missingGlobalSetting = false;

        for (auto it=keys.begin(),end=keys.end() ; it!=end ; ++it) {
            QString name = *it;

            if (globalSettings.contains(name)) {
                GlobalSetting* globalSetting = globalSettings.value(name);
                Q_ASSERT(globalSetting->name() == name);

                QVariant loadedSetting = settings->value(name);
                globalSetting->setValue(loadedSetting);
            } else {
                missingGlobalSetting = true;
            }
        }

        settings->endGroup();

        return !missingGlobalSetting;
    }


    void GlobalSetting::saveSettings(const QString& groupName) {
        QSettings* settings = Application::settings();

        settings->beginGroup(groupName);

        for (auto it=globalSettings.begin(),end=globalSettings.end() ; it!=end ; ++it) {
            QString        name          = it.key();
            GlobalSetting* globalSetting = it.value();

            if (globalSetting != Q_NULLPTR) {
                Q_ASSERT(name == globalSetting->name());
                settings->setValue(name, *globalSetting);
            }
        }

        settings->endGroup();
    }


    bool GlobalSetting::exists(const QString& name) {
        return globalSettings.contains(name);
    }


    GlobalSetting* GlobalSetting::setting(const QString& name) {
        GlobalSetting* desiredValue = globalSettings.value(name);
        Q_ASSERT(desiredValue != Q_NULLPTR);

        return desiredValue;
    }


    void GlobalSetting::setValue(const QVariant& newValue) {
        // We ignore the update if the values are identical.  This protects us from dumb functions that re-trigger a
        // slot in this class blindly, thus causing recursion or an infinite cascade of signals.

        if (*this != newValue) {
            QVariant::setValue(newValue);
            emitChangedSignals();
            notifyActions();
        }
    }


    void GlobalSetting::setValue(int newValue) {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(unsigned newValue) {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(long newValue) {
        setValue(QVariant(static_cast<qlonglong>(newValue)));
    }


    void GlobalSetting::setValue(unsigned long newValue) {
        setValue(QVariant(static_cast<qulonglong>(newValue)));
    }


    void GlobalSetting::setValue(long long newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(unsigned long long newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(float newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(double newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(bool newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const char* newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QByteArray& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QBitArray& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QString& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QStringList& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(QChar newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QDate& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QTime& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QDateTime& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QPoint& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QPointF& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QSize& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::setValue(const QSizeF& newValue)  {
        setValue(QVariant(newValue));
    }


    void GlobalSetting::actionTriggered() {
        QAction* action = dynamic_cast<QAction*>(sender());
        Q_ASSERT(action != Q_NULLPTR);

        if (!action->isCheckable()) {
            if (*this != action->data()) {
                QVariant::setValue(action->data());
                emitChangedSignals();
                notifyActions(action);
            }
        } else {
            // We rely on the GlobalSetting::actionToggled method if this action is checkable.
        }
    }


    void GlobalSetting::actionToggled(bool checked) {
        QAction* action = dynamic_cast<QAction*>(sender());
        Q_ASSERT(action != Q_NULLPTR);
        Q_ASSERT(action->isCheckable());

        if (this->toBool() != checked) {
            QVariant::setValue(checked);
            emitChangedSignals();
            notifyActions(action);
        }
    }


    void GlobalSetting::configureObject(const QString& name) {
        currentName = name;

        Q_ASSERT(!globalSettings.contains(name));
        globalSettings.insert(name, this);
    }


    void GlobalSetting::emitChangedSignals() {
        emit changed();
        emit changed(this);

        QByteArray byteArrayValue = toByteArray();

        emit changed(byteArrayValue.constData());
        emit changed(byteArrayValue);
        emit changed(QString(byteArrayValue));

        emit changed(toBitArray());
        emit changed(toStringList());
        emit changed(toChar());
        emit changed(toDate());
        emit changed(toTime());
        emit changed(toDateTime());

        bool successful;

        int intValue = toInt(&successful);
        if (successful) {
            emit changed(intValue);
        }

        unsigned int uintValue = toUInt(&successful);
        if (successful) {
            emit changed(uintValue);
        }

        long long llValue = toLongLong(&successful);
        if (successful) {
            emit changed(llValue);

            if (llValue <= static_cast<signed long>(static_cast<unsigned long>(-1L) >> 1)    &&
                llValue >= static_cast<signed long>(~(static_cast<unsigned long>(-1L) >> 1))    ) {
                emit changed(static_cast<long>(llValue));
            }
        }

        unsigned long long ullValue = toULongLong(&successful);
        if (successful) {
            emit changed(ullValue);

            if (ullValue <= static_cast<unsigned long>(-1L)) {
                emit changed(static_cast<unsigned long>(ullValue));
            }
        }

        float floatValue = toReal(&successful);
        if (successful) {
            emit changed(floatValue);
        }

        double doubleValue = toDouble(&successful);
        if (successful) {
            emit changed(doubleValue);
        }

        bool boolValue = toBool();
        emit changed(boolValue);

        QPoint pointValue = toPoint();
        emit changed(pointValue);

        QPointF pointFValue = toPointF();
        emit changed(pointFValue);

        QSize sizeValue = toSize();
        emit changed(sizeValue);

        QSizeF sizeFValue = toSizeF();
        emit changed(sizeFValue);
    }


    void GlobalSetting::notifyActions(QAction* ignoredAction) {
        int index = 0;
        int count = actions.count();

        while (index < count) {
            QAction* action = actions.value(index);
            if (action != Q_NULLPTR) {
                if (action != ignoredAction) {
                    if (action->isCheckable()) {
                        action->setChecked(this->toBool());
                    } else {
                        action->setData(*this);
                        action->trigger();
                    }
                }

                ++index;
            } else {
                actions.removeAt(index);
                --count;
            }
        }
    }
}
