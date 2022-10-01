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
* This header defines the \ref EQt::ProgrammaticViewProxy class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_VIEW_PROXY_H
#define EQT_PROGRAMMATIC_VIEW_PROXY_H

#include <QList>
#include <QMetaObject>

#include "eqt_common.h"
#include "eqt_programmatic_window_proxy.h"

namespace EQt {
    class ProgrammaticView;
    class ProgrammaticMainWindow;

    /**
     * Base class for proxies for main window view actions.  You can use this class to extend the functionality of the
     * application views.
     *
     * \todo As of At 5.13, the QObject::disconnect(const QMetaObject::Connection&) did not appear to work.  Longer
     *       term it would be nice to store connections in a list so that unbindDynamic does not need to be coded in
     *       every view proxy.
     */
    class EQT_PUBLIC_API ProgrammaticViewProxy:public ProgrammaticWindowProxy {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] view   The view this class is proxying for.
             *
             * \param[in] window The main window that this view is within.
             */
            ProgrammaticViewProxy(ProgrammaticView* view, ProgrammaticMainWindow* window);

            ~ProgrammaticViewProxy() override;

            /**
             * Determines the associated view class instance.
             *
             * \returns Returns the view instance associated with this proxy.
             */
            ProgrammaticView* view() const;

            /**
             * Determines the window class instance that owns this view.
             *
             * \return Returns the window instance that manages the view this proxy is supporting.
             */
            ProgrammaticMainWindow* window() const;

            /**
             * Method that is called each time the user interface wants this proxy to be controlled.  The method should
             * bind slots or actions from the main window to slots in this class.
             *
             * The default implementation simply returns.
             */
            virtual void bindDynamic();

            /**
             * Method that is called each time the user interface no longer wants this proxy to be controlled by the
             * main window.  The method should unbind slots or actions from this class.
             *
             * The default implementation simply returns.
             */
            virtual void unbindDynamic();

        protected:
            /**
             * Method you can use to connect a signal to an arbitrary receiver method.  The signal will be
             * automatically disconnected when the \ref EQt::ProgrammaticViewProxy::sessionDisconnectAll method is
             * called.
             *
             * \param[in] sender         The sender that is emitting the message.
             *
             * \param[in] signalMethod   The signal associated with the sender emitting the message.
             *
             * \param[in] receiver       The receiver tied to the slot receiving the notification.
             *
             * \param[in] slotMethod     The method to receive notification from the signal.
             *
             * \param[in] connectionType The connection type to be used.
             */
            void sessionConnect(
                const QObject*     sender,
                const char*        signalMethod,
                const QObject*     receiver,
                const char*        slotMethod,
                Qt::ConnectionType connectionType = Qt::AutoConnection
            );

            /**
             * Method you can use to connect a signal to an receiver method in this view proxy.  The signal will be
             * automatically disconnected when the \ref EQt::ProgrammaticViewProxy::sessionDisconnectAll method is
             * called.
             *
             * \param[in] sender         The sender that is emitting the message.
             *
             * \param[in] signalMethod   The signal associated with the sender emitting the message.
             *
             * \param[in] slotMethod     The method to receive notification from the signal.
             *
             * \param[in] connectionType The connection type to be used.
             */
            void sessionConnect(
                const QObject*     sender,
                const char*        signalMethod,
                const char*        slotMethod,
                Qt::ConnectionType connectionType = Qt::AutoConnection
            );

            /**
             * Method you can use to connect a signal to an arbitrary receiver method.  The signal will be
             * automatically disconnected when the \ref EQt::ProgrammaticViewProxy::sessionDisconnectAll method is
             * called.
             *
             * Note that this method relies on a bit of Qt internal magic so it may not be entirely portable to newer
             * versions of Qt.
             *
             * \param[in] sender         The sender that is emitting the message.
             *
             * \param[in] signalMethod   The signal associated with the sender emitting the message.
             *
             * \param[in] receiver       The receiver tied to the slot receiving the notification.
             *
             * \param[in] slotMethod     The method to receive notification from the signal.
             *
             * \param[in] connectionType The connection type to be used.
             */
            template <
                typename SignalMethod,
                typename SlotMethod
            > void sessionConnect(
                const typename QtPrivate::FunctionPointer<SignalMethod>::Object* sender,
                SignalMethod                                                     signalMethod,
                const typename QtPrivate::FunctionPointer<SlotMethod>::Object*   receiver,
                SlotMethod                                                       slotMethod,
                Qt::ConnectionType                                               connectionType = Qt::AutoConnection
            ) {
                activeSessionConnections.append(
                    connect(
                        sender,
                        signalMethod,
                        receiver,
                        slotMethod,
                        connectionType
                    )
                );
            }

            /**
             * Method you can use to connect a signal to a method in this view proxy.  The signal will be automatically
             * disconnected when the \ref EQt::ProgrammaticViewProxy::sessionDisconnectAll method is called.
             *
             * Note that this method relies on a bit of Qt internal magic so it may not be entirely portable to newer
             * versions of Qt.
             *
             * \param[in] sender         The sender that is emitting the message.
             *
             * \param[in] signalMethod   The signal associated with the sender emitting the message.
             *
             * \param[in] slotMethod     The method to receive notification from the signal.
             *
             * \param[in] connectionType The connection type to be used.
             */
            template <
                typename SignalMethod,
                typename SlotMethod
            > void sessionConnect(
                const typename QtPrivate::FunctionPointer<SignalMethod>::Object* sender,
                SignalMethod                                                     signalMethod,
                SlotMethod                                                       slotMethod,
                Qt::ConnectionType                                               connectionType = Qt::AutoConnection
            ) {
                sessionConnect(
                    sender,
                    signalMethod,
                    dynamic_cast<const typename QtPrivate::FunctionPointer<SlotMethod>::Object*>(this),
                    slotMethod,
                    connectionType);
            }

            /**
             * Method you can call to disconnect any connections made using the
             * \ref EQt::ProgrammaticViewProxy::sessionConnect method.
             */
            void sessionDisconnectAll();

        private:
            /**
             * The main window associated with this view proxy.
             */
            ProgrammaticMainWindow* currentWindow;

            /**
             * List of session limited connections.
             */
            QList<QMetaObject::Connection> activeSessionConnections;
    };
}
#endif
