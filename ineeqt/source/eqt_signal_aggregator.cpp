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
* This file implements the \ref EQt::SignalAggregator class.
***********************************************************************************************************************/

#include <QObject>
#include <QList>
#include <QScopedPointer>
#include <QHash>
#include <QTimer>

#include "eqt_signal_aggregator.h"

namespace EQt {
    SignalAggregator::SignalAggregator(QObject* parent):QObject(parent) {
        configure();
    }


    SignalAggregator::SignalAggregator(unsigned serviceDelayMilliseconds, QObject* parent):QObject(parent) {
        configure(serviceDelayMilliseconds);
    }


    SignalAggregator::~SignalAggregator() {
        for (  HandlerHashesBySender::const_iterator handlerHashIterator    = currentHandlers.constBegin(),
                                                     handlerHashEndIterator = currentHandlers.constEnd()
             ; handlerHashIterator != handlerHashEndIterator
             ; ++handlerHashIterator
            ) {
            const HandlersByTypeString& handlersByTypeString = handlerHashIterator.value();
            for (  HandlersByTypeString::const_iterator handlerIterator    = handlersByTypeString.constBegin(),
                                                        handlerEndIterator = handlersByTypeString.constEnd()
                 ; handlerIterator != handlerEndIterator
                 ; ++handlerIterator
                ) {
                HandlerBase* handler = handlerIterator.value();
                delete handler;
            }
        }
    }


    void SignalAggregator::processPendingSignals() {
        for (  QSet<HandlerBase*>::const_iterator it  = pendingHandlers.constBegin(),
                                                  end = pendingHandlers.constEnd()
             ; it != end
             ; ++it
            ) {
            HandlerBase* handler = *it;
            handler->callHandler();
        }

        pendingHandlers.clear();
    }


    void SignalAggregator::configure(unsigned long newTimerDelayMilliseconds) {
        currentTimer = new QTimer(this);
        currentTimer->setSingleShot(true);

        connect(currentTimer, &QTimer::timeout, this, &SignalAggregator::processPendingSignals);
        currentTimerDelay = newTimerDelayMilliseconds;
    }


    bool SignalAggregator::registerHandler(QObject* sender, const QString& typeString, HandlerBase* handler) {
        bool success;

        if (!currentHandlers.contains(sender)) {
            currentHandlers.insert(sender, HandlersByTypeString());
        }

        HandlersByTypeString& handlersByTypeString = currentHandlers[sender];
        if (!handlersByTypeString.contains(typeString)) {
            handlersByTypeString.insert(typeString, handler);
            success = true;
        } else {
            success = false;
        }

        return success;
    }


    SignalAggregator::HandlerBase* SignalAggregator::getHandler(QObject* sender, const QString& typeString) {
        HandlerBase* handler = Q_NULLPTR;

        if (currentHandlers.contains(sender)) {
            HandlersByTypeString& handlersByTypeString = currentHandlers[sender];
            handler = handlersByTypeString.value(typeString);
        }

        return handler;
    }


    void SignalAggregator::schedule(HandlerBase* handler) {
        pendingHandlers.insert(handler);

        if (!currentTimer->isActive()) {
            currentTimer->start(currentTimerDelay);
        }
    }
}
