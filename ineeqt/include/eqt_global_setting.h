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
* This header defines the \ref EQt::GlobalSetting class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GLOBAL_SETTING_H
#define EQT_GLOBAL_SETTING_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QList>
#include <QPointer>
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

class QAction;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class you can use to track application global settings.  The class derives from QVariant so you can safely use
     * this class in the same way you would use a QVariant class.  Note that changing the value will trigger one or
     * more signals to be emitted.
     *
     * Care should be taken to restrict values to boolean types if this class is used with actions where the
     * QAction::checkable method returns true.  The class will also perform some automatic type conversions based on
     * what the underlying QVariant class claims to support.  Many of these conversions make no sense and in fact
     * provide bad or bugus values.  Eventually there may be reason to clean this up but we've left the weird and bogus
     * conversions in place for now.
     *
     * Note that this class is not designed to be thread safe.
     */
    class EQT_PUBLIC_API GlobalSetting:public QObject, public QVariant {
        Q_OBJECT

        public:
            /**
             * Constuctor
             *
             * \param[in] name   The name to assign to this global settings instance.  You can use this name to locate
             *                   the global settings using the static \ref GlobalSetting::setting method.
             *
             * \param[in] parent The parent object for this setting.  The parent is normally set to the be the main
             *                   application instance.
             */
            GlobalSetting(const QString& name, QObject* parent);

            /**
             * Constuctor.  This variant automatically makes this object a child of the main application class.
             *
             * \param[in] name The name to assign to this global settings instance.  You can use this name to locate
             *                 the global settings using the static \ref GlobalSetting::setting method.
             */
            explicit GlobalSetting(const QString& name);

            /**
             * Constuctor.  This variant assigns a default value during construction.
             *
             * \param[in] name         The name to assign to this global settings instance.  You can use this name to
             *                         locate the global settings using the static \ref GlobalSetting::setting method.
             *
             * \param[in] defaultValue A default/initial value to assign to this \ref GlobalSetting instance.
             *
             * \param[in] parent       The parent object for this setting.  The parent is normally set to the be the
             *                         main application instance.
             */
            GlobalSetting(const QString& name, const QVariant& defaultValue, QObject* parent);

            /**
             * Constuctor.  This variant assigns a default value during construction and makes the instance a child of
             * the main application instance.
             *
             * \param[in] name         The name to assign to this global settings instance.  You can use this name to
             *                         locate the global settings using the static \ref GlobalSetting::setting method.
             *
             * \param[in] defaultValue A default/initial value to assign to this \ref GlobalSetting instance.
             */
            GlobalSetting(const QString& name, const QVariant& defaultValue);

            ~GlobalSetting() override;

            /**
             * Method that returns the name assigned to this global settings instance.
             *
             * \return Returns the global settings name.
             */
            QString name() const;

            /**
             * Method that can be used to associate an action with this global settings instance.  The action will be
             * triggered when the value is changed and triggered events from the action will cause this setting value
             * to be updated automatically.
             *
             * Actions with the checkable property set are automatically treated using a bool type.  All other actions
             * are handled using the QAction::data and QAction::setData methods to propagate information.
             *
             * Note that the class will automatically and cleanly handle cases where the action gets deleted.  You do
             * not need to explicitly disassociate the action with the settings classes prior to destroying the QAction
             * object.
             *
             * \param[in] newAction The action to associate with this global settings instance.
             */
            void associateAction(QAction* newAction);

            /**
             * Method that can be used to disassociate the action from the global settings instance.  The action will
             * no longer be triggered when the value changes and the action will no longer be able to cause the
             * settings value to change.
             *
             * Note that the class will automatically and cleanly handle cases where the action gets deleted.  You do
             * not need to explicitly disassociate the action with the settings classes prior to destroying the QAction
             * object.
             *
             * \param[in] oldAction The action to be disassociated.
             */
            void disassociateAction(QAction* oldAction);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QVariant& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(int newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(unsigned newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(long newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(unsigned long newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(long long newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(unsigned long long newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(float newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(double newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(bool newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const char* newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QByteArray& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QBitArray& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QString& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QStringList& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(QChar newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QDate& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QTime& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QDateTime& newValue);

            /**
             * Assignment operator
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QPoint& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QPointF& newValue);

            /**
             * Assignment operator
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QSize& newValue);

            /**
             * Assignment operator.
             *
             * \param[in] newValue The new value.
             */
            GlobalSetting& operator=(const QSizeF& newValue);

            /**
             * Static method that can be called to load all the application global settings.
             *
             * \param[in] groupName The group name to associate with the global settings.
             *
             * \return Returns true if every item in the settings database had an associated global settings instance.
             *         Returns false if items were missing.
             */
            static bool loadSettings(const QString& groupName = "globalSettings");

            /**
             * Static method that can be called to save all the application global settings.
             *
             * \param[in] groupName The group name to associate with the global settings.
             */
            static void saveSettings(const QString& groupName = "globalSettings");

            /**
             * Static method that can be used to determine if a \ref GlobalSetting instance has been created.
             *
             * \param[in] name The name of the global setting instance.
             *
             * \return Returns true if the global settings instance exists.  Returns false if the global settings
             *         instance does not exist.
             */
            static bool exists(const QString& name);

            /**
             * Static method that can be used to determine if a \ref GlobalSetting instance has not been created.
             *
             * \param[in] name The name of the global setting instance.
             *
             * \return Returns true if the global settings instance does not exist.  Returns false if the global
             *         settings instance exists.
             */
            static inline bool doesNotExist(const QString& name) {
                return !exists(name);
            }

            /**
             * Static method that can be used to obtain a \ref GlobalSetting instance, by name.  This method will
             * assert if the requested global setting value does not exist.
             *
             * \param[in] name The name of the global setting instance.
             *
             * \return Returns the global setting instance.
             */
            static GlobalSetting* setting(const QString& name);

        public slots:
            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QVariant& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(int newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(unsigned newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(long newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(unsigned long newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(long long newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(unsigned long long newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(float newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(double newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(bool newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const char* newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QByteArray& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QBitArray& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QString& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QStringList& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(QChar newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QDate& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QTime& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QDateTime& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QPoint& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QPointF& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QSize& newValue);

            /**
             * Slot you can use to change the settings object value.
             *
             * \param[in] newValue The new value.
             */
            void setValue(const QSizeF& newValue);

        signals:
            /**
             * Signal that is emitted when the value of this setting changes.
             */
            void changed();

            /**
             * Alternate signal that is emitted when the value of this setting changes.
             *
             * \param[out] globalSetting The global setting instance that just changed.
             */
            void changed(GlobalSetting* globalSetting);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(int value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(unsigned value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(long value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(unsigned long value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(long long value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(unsigned long long value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(float value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(double value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(bool value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const char* value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QByteArray& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QBitArray& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QString& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QStringList& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(QChar value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QDate& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QTime& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QDateTime& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QPoint& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QPointF& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QSize& value);

            /**
             * Alternate signal that is emitted when the value of this setting changes.  This signal will only be
             * emitted if the underlying type is compatible with the signal parameter.
             *
             * \param[out] value The new value.
             */
            void changed(const QSizeF& value);

        private slots:
            /**
             * Slot used to receive notification of changes from QAction instances.
             */
            void actionTriggered();

            /**
             * Slot used to receive notification of changes from a chekable QAction instance.
             */
            void actionToggled(bool checked);

        private:
            /**
             * Method called by the constructors to perform common initialization.
             */
            void configureObject(const QString& name);

            /**
             * Method that is called to emit signals when the value changes.
             */
            void emitChangedSignals();

            /**
             * Method that is called to notify actions when the value changes.
             *
             * \param[in] ignoredAction Pointer to an action that should not be notified.  This will generally be used
             *                          to avoid alerting the sending action of its own changes.
             */
            void notifyActions(QAction* ignoredAction = Q_NULLPTR);

            /**
             * Settings name.
             */
            QString currentName;

            /**
             * List of actions associated with this class.
             *
             * Ideally this value would be of type QSet<QPointer<QAction>>; however, there is no qHash function that
             * will perform the conversion from QPointer to int.  For now we use QList to work around this minor
             * naggle.
             */
            QList<QPointer<QAction>> actions;

            /**
             * The global settings database.
             */
            static QMap<QString, GlobalSetting*> globalSettings;
    };
}

#endif
