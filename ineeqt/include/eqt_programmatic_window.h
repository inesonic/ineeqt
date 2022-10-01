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
* This header defines the \ref EQt::ProgrammaticWindow class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_WINDOW_H
#define EQT_PROGRAMMATIC_WINDOW_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QList>

#include "eqt_common.h"
#include "eqt_programmatic_application.h"
#include "eqt_programmatic_window_base.h"

namespace EQt {
    /**
     * Template class that you can inherit to incorporate proper type casting on top of \ref ProgrammaticWindowBase.
     *
     * \param[in] P the proxy class this widget should operate with.  Note that this object must be inherted from
     *              QObject.
     *
     * \param[in] D The derived class that this class is going to be used with.  This parameter is used to support
     *              using this object within the Qt framework.
     */
    template<class P, class D> class ProgrammaticWindow:public EQt::ProgrammaticWindowBase {
        public:
            /**
             * Type used for lists of proxies for this widget.
             */
            typedef QList<P*> ProxyList;

            ProgrammaticWindow() {};

            virtual ~ProgrammaticWindow() {};

            /**
             * Loads settings to be used by the window.  This method will set the group for the window and then call
             * the \ref ProgrammaticWidgetProxy::loadSettings method in each of the proxy classes.
             *
             * Be sure to call the base class implementation if you overload this method.
             *
             * \param[in] groupName The name of the settings group to use for the main window.
             */
            virtual void loadSettings(const QString& groupName) {
                QSettings* settings = ProgrammaticApplication::settings();

                settings->beginGroup(groupName);

                for (auto it=proxyList.begin(),end=proxyList.end() ; it!=end ; ++it) {
                    (*it)->loadSettings();
                }

                settings->endGroup();
            }

            /**
             * Saves settings used by the window.  This method will set the group for the window and then call the
             * \ref ProgrammaticWidgetProxy::saveSettings method in each of the proxy classes.
             *
             * Be sure to call the base class implementation if you overload this method.
             *
             * \param[in] groupName The name of the settings group to use for the main window.
             */
            virtual void saveSettings(const QString& groupName)  {
                QSettings* settings = ProgrammaticApplication::settings();

                settings->beginGroup(groupName);

                for (auto it=proxyList.begin(),end=proxyList.end() ; it!=end ; ++it) {
                    (*it)->saveSettings();
                }

                settings->endGroup();
            }

            /**
             * Adds a proxy object to this widget.
             *
             * \param[in] name   The name used to refer to this object.
             *
             * \param[in] object The proxy object.  Note that this class will take ownership of the proxy object.
             */
            void addProxy(const QString& name, P* object) {
                Q_ASSERT(!proxyMap.contains(name));

                object->setParent(dynamic_cast<D*>(this));
                proxyMap.insert(name, object);
                proxyList.append(object);
            }

            /**
             * Obtains a desired proxy, by name.
             *
             * \param[in] name The name used to reference the proxy object.
             *
             * \returns The desired proxy object.  A null pointer is returned if the object does not exist.
             */
            inline P* proxy(const QString& name) {
                P* proxy = proxyMap.value(name);
                Q_ASSERT(proxy != Q_NULLPTR);

                return proxy;
            }

        protected:
            /**
             * Obtains a list of all the proxies registered to this widget.
             *
             * \return Returns a list of proxy objects.
             */
            inline ProxyList const proxies() {
                return proxyList;
            }

            /**
             * Obtains an iterator the beginning of a list of proxy instances.
             *
             * \return Returns a constant iterator to a list of proxy instances.
             */
            inline typename ProxyList::const_iterator beginProxies() const {
                return proxyList.constBegin();
            }

            /**
             * Obtains an iterator the end of a list of proxy instances.
             *
             * \return Returns a constant iterator to a list of proxy instances.
             */
            inline typename ProxyList::const_iterator endProxies() const {
                return proxyList.constEnd();
            }

            /**
             * Binds all the proxies to this class.  Calls the \ref ProgrammaticWidgetProxy::bind method in each proxy
             * class.
             */
            virtual void bind() {
                for (auto it=proxyList.begin(),end=proxyList.end() ; it!=end ; ++it) {
                    (*it)->bind();
                }
            }

        private:
            /**
             * Dictionary of proxy objects that contain slots for specific functions.
             */
            QMap<QString, P*> proxyMap;

            /**
             * List of proxies that contains slots for specific functions.
             */
            ProxyList proxyList;
    };
}

#endif
