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
* This file implements the \ref EQt::ProgrammaticViewProxy class.
***********************************************************************************************************************/

#include <QObject>

#include "eqt_common.h"
#include "eqt_programmatic_view.h"
#include "eqt_programmatic_view_proxy.h"

namespace EQt {

    ProgrammaticViewProxy::ProgrammaticViewProxy(
            ProgrammaticView*       view,
            ProgrammaticMainWindow* window
        ):ProgrammaticWindowProxy(
            view
        ) {
        currentWindow = window;
    }


    ProgrammaticViewProxy::~ProgrammaticViewProxy() {}


    ProgrammaticView* ProgrammaticViewProxy::view() const {
        return dynamic_cast<ProgrammaticView*>(parent());
    }


    ProgrammaticMainWindow* ProgrammaticViewProxy::window() const {
        return currentWindow;
    }


    void ProgrammaticViewProxy::bindDynamic() {}


    void ProgrammaticViewProxy::unbindDynamic() {}


    void ProgrammaticViewProxy::sessionConnect(
            const QObject*     sender,
            const char*        signalMethod,
            const QObject*     receiver,
            const char*        slotMethod,
            Qt::ConnectionType connectionType
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


    void ProgrammaticViewProxy::sessionConnect(
            const QObject*     sender,
            const char*        signalMethod,
            const char*        slotMethod,
            Qt::ConnectionType connectionType
        ) {
        activeSessionConnections.append(
            connect(
                sender,
                signalMethod,
                this,
                slotMethod,
                connectionType
            )
        );
    }


    void ProgrammaticViewProxy::sessionDisconnectAll() {
        while (!activeSessionConnections.isEmpty()) {
            QMetaObject::Connection connection = activeSessionConnections.takeLast();
            disconnect(connection);
        }
    }
}
