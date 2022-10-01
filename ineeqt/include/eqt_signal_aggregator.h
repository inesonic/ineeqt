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
* This header defines the \ref EQt::SignalAggregator class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_SIGNAL_AGGREGATOR_H
#define EQT_SIGNAL_AGGREGATOR_H

#include <QObject>
#include <QList>
#include <QScopedPointer>
#include <QHash>
#include <QTimer>
#include <QDebug>

#include <typeinfo>

#include "eqt_common.h"

namespace EQt {
    /**
     * Class you can use to aggregate multiple received signals together such that multiple generated messages are
     * handled by a single slot that is triggered once by the Qt polling mechanism.
     *
     * This class exists primarily to reduce the overhead incurred by repeatedly servicing the same signal.
     */
    class EQT_PUBLIC_API SignalAggregator:public QObject {
        private:
            /**
             * Base class for handler objects.  You should never use this class directly.
             */
            class EQT_PUBLIC_API HandlerBase {
                public:
                    virtual ~HandlerBase() = default;

                    /**
                     * Pure virtual method that can be used to call the assigned processing method.
                     */
                    virtual void callHandler() = 0;
            };

            /**
             * Template class that calls an underlying handler method.
             */
            template<typename C> class Handler0:public HandlerBase {
                public:
                    /**
                     * Type used to represent the handler method pointer.
                     */
                    typedef void(C::*HandlerMethodPointer)();

                    /**
                     * Constructor
                     *
                     * \param[in] handlerInstance The handler class instance.
                     *
                     * \param[in] handlerMethod   A method pointer in class C.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD Handler0(C* handlerInstance, HandlerMethodPointer handlerMethod) {
                        currentHandlerInstance = handlerInstance;
                        currentHandlerMethod   = handlerMethod;
                    };

                    virtual ~Handler0() = default;

                    /**
                     * Method that stores pending values.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void addParameters() {}

                    /**
                     * Method that calls the underlying handler.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void callHandler() override {
                        (currentHandlerInstance->*currentHandlerMethod)();
                    }

                private:
                    /**
                     * The class instance associated with the handler.
                     */
                    C* currentHandlerInstance;

                    /**
                     * The handler method pointer.
                     */
                    HandlerMethodPointer currentHandlerMethod;
            };

            /**
             * Template class that calls an underlying handler method.
             */
            template<typename C, typename P1> class Handler1:public HandlerBase {
                public:
                    /**
                     * Type used to represent the handler method pointer.
                     *
                     * \param[l1] l1 A list of values provided on each call.
                     */
                    typedef void(C::*HandlerMethodPointer)(const QList<P1>& l1);

                    /**
                     * Constructor
                     *
                     * \param[in] handlerInstance The handler class instance.
                     *
                     * \param[in] handlerMethod   A method pointer in class C.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD Handler1(C* handlerInstance, HandlerMethodPointer handlerMethod) {
                        currentHandlerInstance = handlerInstance;
                        currentHandlerMethod   = handlerMethod;
                    };

                    virtual ~Handler1() = default;

                    /**
                     * Method that stores pending values.
                     *
                     * \param[in] p1 The first parameter to be saved off.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void addParameters(const P1& p1) {
                        currentList1.push_back(p1);
                    }

                    /**
                     * Method that calls the underlying handler.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void callHandler() override {
                        (currentHandlerInstance->*currentHandlerMethod)(currentList1);
                        currentList1.clear();
                    }

                private:
                    /**
                     * The class instance associated with the handler.
                     */
                    C* currentHandlerInstance;

                    /**
                     * The handler method pointer.
                     */
                    HandlerMethodPointer currentHandlerMethod;

                    /**
                     * List of first pending aggregated values.
                     */
                    QList<P1> currentList1;
            };

            /**
             * Template class that calls an underlying handler method.
             */
            template<typename C, typename P1, typename P2> class Handler2:public HandlerBase {
                public:
                    /**
                     * Type used to represent the handler method pointer.
                     *
                     * \param[l1] l1 A list of first values provided on each call.
                     *
                     * \param[l1] l2 A list of second values provided on each call.
                     */
                    typedef void(C::*HandlerMethodPointer)(const QList<P1>& l1, const QList<P2>& l2);

                    /**
                     * Constructor
                     *
                     * \param[in] handlerInstance The handler class instance.
                     *
                     * \param[in] handlerMethod   A method pointer in class C.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD Handler2(C* handlerInstance, HandlerMethodPointer handlerMethod) {
                        currentHandlerInstance = handlerInstance;
                        currentHandlerMethod   = handlerMethod;
                    };

                    virtual ~Handler2() = default;

                    /**
                     * Method that stores pending values.
                     *
                     * \param[in] p1 The first parameter to be saved off.
                     *
                     * \param[in] p2 The second parameter to be saved off.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void addParameters(const P1& p1, const P2& p2) {
                        currentList1.push_back(p1);
                        currentList2.push_back(p2);
                    }

                    /**
                     * Method that calls the underlying handler.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void callHandler() override {
                        (currentHandlerInstance->*currentHandlerMethod)(currentList1, currentList2);
                        currentList1.clear();
                        currentList2.clear();
                    }

                private:
                    /**
                     * The class instance associated with the handler.
                     */
                    C* currentHandlerInstance;

                    /**
                     * The handler method pointer.
                     */
                    HandlerMethodPointer currentHandlerMethod;

                    /**
                     * List of first pending aggregated values.
                     */
                    QList<P1> currentList1;

                    /**
                     * List of second pending aggregated values.
                     */
                    QList<P2> currentList2;
            };

            /**
             * Template class that calls an underlying handler method.
             */
            template<typename C, typename P1, typename P2, typename P3> class Handler3:public HandlerBase {
                public:
                    /**
                     * Type used to represent the handler method pointer.
                     *
                     * \param[l1] l1 A list of first values provided on each call.
                     *
                     * \param[l1] l2 A list of second values provided on each call.
                     *
                     * \param[l1] l3 A list of third values provided on each call.
                     */
                    typedef void(C::*HandlerMethodPointer)(
                        const QList<P1>& l1,
                        const QList<P2>& l2,
                        const QList<P3>& l3
                    );

                    /**
                     * Constructor
                     *
                     * \param[in] handlerInstance The handler class instance.
                     *
                     * \param[in] handlerMethod   A method pointer in class C.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD Handler3(C* handlerInstance, HandlerMethodPointer handlerMethod) {
                        currentHandlerInstance = handlerInstance;
                        currentHandlerMethod   = handlerMethod;
                    };

                    virtual ~Handler3() = default;

                    /**
                     * Method that stores pending values.
                     *
                     * \param[in] p1 The first parameter to be saved off.
                     *
                     * \param[in] p2 The second parameter to be saved off.
                     *
                     * \param[in] p3 The third parameter to be saved off.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void addParameters(const P1& p1, const P2& p2, const P3& p3) {
                        currentList1.push_back(p1);
                        currentList2.push_back(p2);
                        currentList3.push_back(p3);
                    }

                    /**
                     * Method that calls the underlying handler.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void callHandler() override {
                        (currentHandlerInstance->*currentHandlerMethod)(currentList1, currentList2, currentList3);
                        currentList1.clear();
                        currentList2.clear();
                        currentList3.clear();
                    }

                private:
                    /**
                     * The class instance associated with the handler.
                     */
                    C* currentHandlerInstance;

                    /**
                     * The handler method pointer.
                     */
                    HandlerMethodPointer currentHandlerMethod;

                    /**
                     * List of first pending aggregated values.
                     */
                    QList<P1> currentList1;

                    /**
                     * List of second pending aggregated values.
                     */
                    QList<P2> currentList2;

                    /**
                     * List of third pending aggregated values.
                     */
                    QList<P3> currentList3;
            };

            /**
             * Template class that calls an underlying handler method.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2,
                    typename P3,
                    typename P4
                > class Handler4:public HandlerBase {
                public:
                    /**
                     * Type used to represent the handler method pointer.
                     *
                     * \param[l1] l1 A list of first values provided on each call.
                     *
                     * \param[l1] l2 A list of second values provided on each call.
                     *
                     * \param[l1] l3 A list of third values provided on each call.
                     *
                     * \param[l1] l4 A list of fourth values provided on each call.
                     */
                    typedef void(C::*HandlerMethodPointer)(
                        const QList<P1>& l1,
                        const QList<P2>& l2,
                        const QList<P3>& l3,
                        const QList<P4>& l4
                    );

                    /**
                     * Constructor
                     *
                     * \param[in] handlerInstance The handler class instance.
                     *
                     * \param[in] handlerMethod   A method pointer in class C.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD Handler4(C* handlerInstance, HandlerMethodPointer handlerMethod) {
                        currentHandlerInstance = handlerInstance;
                        currentHandlerMethod   = handlerMethod;
                    };

                    virtual ~Handler4() = default;

                    /**
                     * Method that stores pending values.
                     *
                     * \param[in] p1 The first parameter to be saved off.
                     *
                     * \param[in] p2 The second parameter to be saved off.
                     *
                     * \param[in] p3 The third parameter to be saved off.
                     *
                     * \param[in] p4 The third parameter to be saved off.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void addParameters(
                            const P1& p1,
                            const P2& p2,
                            const P3& p3,
                            const P4& p4
                        ) {
                        currentList1.push_back(p1);
                        currentList2.push_back(p2);
                        currentList3.push_back(p3);
                        currentList4.push_back(p4);
                    }

                    /**
                     * Method that calls the underlying handler.
                     */
                    EQT_PUBLIC_TEMPLATE_METHOD void callHandler() override {
                        (currentHandlerInstance->*currentHandlerMethod)(
                            currentList1,
                            currentList2,
                            currentList3,
                            currentList4
                        );

                        currentList1.clear();
                        currentList2.clear();
                        currentList3.clear();
                        currentList4.clear();
                    }

                private:
                    /**
                     * The class instance associated with the handler.
                     */
                    C* currentHandlerInstance;

                    /**
                     * The handler method pointer.
                     */
                    HandlerMethodPointer currentHandlerMethod;

                    /**
                     * List of first pending aggregated values.
                     */
                    QList<P1> currentList1;

                    /**
                     * List of second pending aggregated values.
                     */
                    QList<P2> currentList2;

                    /**
                     * List of third pending aggregated values.
                     */
                    QList<P3> currentList3;

                    /**
                     * List of third pending aggregated values.
                     */
                    QList<P4> currentList4;
            };

        public:
            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            SignalAggregator(QObject* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] serviceDelayMilliseconds The service delay to apply before servicing aggregated signals.
             *
             * \param[in] parent                   Pointer to the parent object.
             */
            SignalAggregator(unsigned serviceDelayMilliseconds, QObject* parent = Q_NULLPTR);

            ~SignalAggregator() override;

            /**
             * Method you can use to register a deferred/aggregating connection.
             *
             * \param[in] sender         A pointer to the sender object.  The sender must have the prototype
             *                           "sender(P1 p1)" or "sender(const P1& p1)".
             *
             * \param[in] receiver       A pointer to the class receiving deferred/aggregated notifications.
             *
             * \param[in] receiverMethod A method pointer to the method to receive deferred/aggregated notifications.
             *                           The receiver method must have the prototype "receiver(const QList<P1>& l1)".
             *
             * \return Returns true on success.  Returns false if the handler was already registered.
             */
            template<typename C> EQT_PUBLIC_TEMPLATE_METHOD bool registerConnection(
                    QObject* sender,
                    C*       receiver,
                    void (C::*receiverMethod)()
                ) {
                typedef Handler0<C> HandlerType;
                return registerHandler(
                    sender,
                    QString(typeid(HandlerType).name()),
                    new HandlerType(receiver, receiverMethod)
                );
            }

            /**
             * Method you can use to register a deferred/aggregating connection.
             *
             * \param[in] sender         A pointer to the sender object.  The sender must have the prototype
             *                           "sender(P1 p1)".
             *
             * \param[in] receiver       A pointer to the class receiving deferred/aggregated notifications.
             *
             * \param[in] receiverMethod A method pointer to the method to receive deferred/aggregated notifications.
             *                           The receiver method must have the prototype "receiver(const QList<P1>& l1)".
             *
             * \return Returns true on success.  Returns false if the handler was already registered.
             */
            template<typename C, typename P1> EQT_PUBLIC_TEMPLATE_METHOD bool registerConnection(
                    QObject* sender,
                    C*       receiver,
                    void (C::*receiverMethod)(const QList<P1>&)
                ) {
                typedef Handler1<C, P1> HandlerType;
                return registerHandler(
                    sender,
                    QString(typeid(HandlerType).name()),
                    new HandlerType(receiver, receiverMethod)
                );
            }

            /**
             * Method you can use to register a deferred/aggregating connection.
             *
             * \param[in] sender         A pointer to the sender object.  The sender must have the prototype
             *                           "sender(P1 p1, P2 p2)".
             *
             * \param[in] receiver       A pointer to the class receiving deferred/aggregated notifications.
             *
             * \param[in] receiverMethod A method pointer to the method to receive deferred/aggregated notifications.
             *                           The receiver method must have the prototype
             *                           "receiver(const QList<P1>& l1, const QList<P2>& l2)".
             *
             * \return Returns true on success.  Returns false if the handler was already registered.
             */
            template<typename C, typename P1, typename P2> EQT_PUBLIC_TEMPLATE_METHOD bool registerConnection(
                    QObject* sender,
                    C*       receiver,
                    void (C::*receiverMethod)(const QList<P1>&, const QList<P2>&)
                ) {
                typedef Handler2<C, P1, P2> HandlerType;
                return registerHandler(
                    sender,
                    QString(typeid(HandlerType).name()),
                    new HandlerType(receiver, receiverMethod)
                );
            }

            /**
             * Method you can use to register a deferred/aggregating connection.
             *
             * \param[in] sender         A pointer to the sender object.  The sender must have the prototype
             *                           "sender(P1 p1, P2 p2, P3 p3)".
             *
             * \param[in] receiver       A pointer to the class receiving deferred/aggregated notifications.
             *
             * \param[in] receiverMethod A method pointer to the method to receive deferred/aggregated notifications.
             *                           The receiver method must have the prototype
             *                           "receiver(const QList<P1>& l1, const QList<P2>& l2, const QList<P3>& p3)".
             *
             * \return Returns true on success.  Returns false if the handler was already registered.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2,
                    typename P3
                > EQT_PUBLIC_TEMPLATE_METHOD bool registerConnection(
                    QObject* sender,
                    C*       receiver,
                    void (C::*receiverMethod)(const QList<P1>&, const QList<P2>&, const QList<P3>&)
                ) {
                typedef Handler3<C, P1, P2, P3> HandlerType;
                return registerHandler(
                    sender,
                    QString(typeid(HandlerType).name()),
                    new HandlerType(receiver, receiverMethod)
                );
            }

            /**
             * Method you can use to register a deferred/aggregating connection.
             *
             * \param[in] sender         A pointer to the sender object.  The sender must have the prototype
             *                           "sender(P1 p1, P2 p2, P3 p3, P4 p4)".
             *
             * \param[in] receiver       A pointer to the class receiving deferred/aggregated notifications.
             *
             * \param[in] receiverMethod A method pointer to the method to receive deferred/aggregated notifications.
             *                           The receiver method must have the prototype \code
                 receiver(const QList<P1>& l1, const QList<P2>& l2, const QList<P3>& p3, const QList<P4>& p4); \endcode
             *
             * \return Returns true on success.  Returns false if the handler was already registered.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2,
                    typename P3,
                    typename P4
                > EQT_PUBLIC_TEMPLATE_METHOD bool registerConnection(
                    QObject* sender,
                    C*       receiver,
                    void (C::*receiverMethod)(const QList<P1>&, const QList<P2>&, const QList<P3>&, const QList<P4>&)
                ) {
                typedef Handler4<C, P1, P2, P3, P4> HandlerType;
                return registerHandler(
                    sender,
                    QString(typeid(HandlerType).name()),
                    new HandlerType(receiver, receiverMethod)
                );
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] sender The sender object instance.
             *
             * \return Returns true on success, returns false on error.
             */
            template<typename C> EQT_PUBLIC_TEMPLATE_METHOD void forceTrigger(QObject* sender) {
                typedef Handler0<C> HandlerType;
                HandlerBase* rawHandler = getHandler(sender, QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \return Returns true on success, returns false on error.
             */
            template<typename C> EQT_PUBLIC_TEMPLATE_METHOD void trigger() {
                typedef Handler0<C> HandlerType;
                HandlerBase* rawHandler = getHandler(sender(), QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender()
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] sender The sender object instance.
             *
             * \param[in] p1     The first parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            template<typename C, typename P1> EQT_PUBLIC_TEMPLATE_METHOD void forceTrigger(
                    QObject*  sender,
                    const P1& p1
                ) {
                typedef Handler1<C, P1> HandlerType;
                HandlerBase* rawHandler = getHandler(sender, QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    handler->addParameters(p1);
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] p1 The first parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            template<typename C, typename P1> EQT_PUBLIC_TEMPLATE_METHOD void trigger(const P1& p1) {
                typedef Handler1<C, P1> HandlerType;
                HandlerBase* rawHandler = getHandler(sender(), QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    handler->addParameters(p1);
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender()
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] sender The sender object instance.
             *
             * \param[in] p1     The first parameter.
             *
             * \param[in] p2     The second parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2
                > EQT_PUBLIC_TEMPLATE_METHOD void forceTrigger(
                    QObject*  sender,
                    const P1& p1,
                    const P2& p2
                ) {
                typedef Handler2<C, P1, P2> HandlerType;
                HandlerBase* rawHandler = getHandler(sender, QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    handler->addParameters(p1, p2);
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] p1 The first parameter.
             *
             * \param[in] p2 The second parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2
                > EQT_PUBLIC_TEMPLATE_METHOD void trigger(
                    const P1& p1,
                    const P2& p2
                ) {
                typedef Handler2<C, P1, P2> HandlerType;
                HandlerBase* rawHandler = getHandler(sender(), QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    handler->addParameters(p1, p2);
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender()
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] sender The sender object instance.
             *
             * \param[in] p1     The first parameter.
             *
             * \param[in] p2     The second parameter.
             *
             * \param[in] p3     The third parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2,
                    typename P3
                > EQT_PUBLIC_TEMPLATE_METHOD void forceTrigger(
                    QObject*  sender,
                    const P1& p1,
                    const P2& p2,
                    const P3& p3
                ) {
                typedef Handler3<C, P1, P2, P3> HandlerType;
                HandlerBase* rawHandler = getHandler(sender, QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    handler->addParameters(p1, p2, p3);
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] p1 The first parameter.
             *
             * \param[in] p2 The second parameter.
             *
             * \param[in] p3 The third parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2,
                    typename P3
                > EQT_PUBLIC_TEMPLATE_METHOD void trigger(
                    const P1& p1,
                    const P2& p2,
                    const P3& p3
                ) {
                typedef Handler3<C, P1, P2, P3> HandlerType;
                HandlerBase* rawHandler = getHandler(sender(), QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    handler->addParameters(p1, p2, p3);
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender()
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] sender The sender object instance.
             *
             * \param[in] p1     The first parameter.
             *
             * \param[in] p2     The second parameter.
             *
             * \param[in] p3     The third parameter.
             *
             * \param[in] p4     The fourth parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2,
                    typename P3,
                    typename P4
                > EQT_PUBLIC_TEMPLATE_METHOD void forceTrigger(
                    QObject*  sender,
                    const P1& p1,
                    const P2& p2,
                    const P3& p3,
                    const P4& p4
                ) {
                typedef Handler4<C, P1, P2, P3, P4> HandlerType;
                HandlerBase* rawHandler = getHandler(sender, QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    handler->addParameters(p1, p2, p3, p4);
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender
                             << " - " << typeid(HandlerType).name();
                }
            }

            /**
             * Method you can call to trigger a deferred call to an aggregating handler.
             *
             * \param[in] p1 The first parameter.
             *
             * \param[in] p2 The second parameter.
             *
             * \param[in] p3 The third parameter.
             *
             * \param[in] p4 The fourth parameter.
             *
             * \return Returns true on success, returns false on error.
             */
            template<
                    typename C,
                    typename P1,
                    typename P2,
                    typename P3,
                    typename P4
                > EQT_PUBLIC_TEMPLATE_METHOD void trigger(
                    const P1& p1,
                    const P2& p2,
                    const P3& p3,
                    const P4& p4
                ) {
                typedef Handler4<C, P1, P2, P3, P4> HandlerType;
                HandlerBase* rawHandler = getHandler(sender(), QString(typeid(HandlerType).name()));
                HandlerType* handler    = dynamic_cast<HandlerType*>(rawHandler);
                if (handler != Q_NULLPTR) {
                    handler->addParameters(p1, p2, p3, p4);
                    schedule(handler);
                } else {
                    qDebug() << "Missing aggregating handler for sender "<< sender()
                             << " - " << typeid(HandlerType).name();
                }
            }

        private slots:
            /**
             * Slot that is triggered when the aggregated signals need to be processed.
             */
            void processPendingSignals();

        private:
            /**
             * Value indicating the default timer delay, in mSec.
             */
            static constexpr unsigned long defaultTimerDelayMilliseconds = 0;

            /**
             * Method that is called by the constructors to configure this class.
             *
             * \param[in] newTimerDelayMilliseconds The new timer delay, in milliseconds.
             */
            void configure(unsigned long newTimerDelayMilliseconds = defaultTimerDelayMilliseconds);

            /**
             * Method that registers a handler.
             *
             * \param[in] sender Pointer to the sender object.
             *
             * \param[in] typeString A string describing the handler.
             *
             * \param[in] handler    A handler instance to be registered.
             *
             * \return Returns true on success.  Returns false if the handler was already registered.
             */
            bool registerHandler(QObject* sender, const QString& typeString, HandlerBase* handler);

            /**
             * Method that returns a registered handler.
             *
             * \param[in] sender Pointer to the sender object.
             *
             * \param[in] typeString A string describing the handler.
             *
             * \return Returns a pointer to the deferred handler.  Returns a null pointer if the requested handler
             *         could not be located.
             */
            HandlerBase* getHandler(QObject* sender, const QString& typeString);

            /**
             * Method that can be used to schedule a deferred handler.
             *
             * \param[in] handler The handler to be scheduled.
             */
            void schedule(HandlerBase* handler);

            /**
             * Hash of handlers index by string.
             */
            typedef QHash<QString, HandlerBase*> HandlersByTypeString;

            /**
             * Hash of handler hashes indexed by sender.
             */
            typedef QHash<QObject*, HandlersByTypeString> HandlerHashesBySender;

            /**
             * Hash of handlers.
             */
            HandlerHashesBySender currentHandlers;

            /**
             * List of pending handlers.
             */
            QSet<HandlerBase*> pendingHandlers;

            /**
             * Timer used to trigger deferred/aggregated events.
             */
            QTimer* currentTimer;

            /**
             * The timer delay, in milliseconds.
             */
            unsigned long currentTimerDelay;
    };
}

#endif
