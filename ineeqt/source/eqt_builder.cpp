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
* This file implements the \ref EQt::Builder class.
***********************************************************************************************************************/

#include <QObject>

#include "eqt_application.h"
#include "eqt_builder_base.h"
#include "eqt_builder.h"

class ProgrammaticApplication;

namespace EQt {
    class ProgrammaticMainWindow;
    class ProgrammaticMainWindowProxy;
    class ProgrammaticView;
    class ProgrammaticViewProxy;
    class ProgrammaticDialog;
    class ProgrammaticDialogProxy;

    Builder::Builder(QObject* parent):QObject(parent) {}


    Builder::~Builder() {}


    void Builder::registerResources(ProgrammaticApplication*) {}


    void Builder::buildMainWindowGui(ProgrammaticMainWindow*) {}


    void Builder::buildAdditionalMainWindowActions(ProgrammaticMainWindow*) {}


    void Builder::buildMainWindowDocks(ProgrammaticMainWindow*) {}


    ProgrammaticMainWindowProxy* Builder::createMainWindowProxy(ProgrammaticMainWindow*) {
        return Q_NULLPTR;
    }


    void Builder::buildMainWindowFinal(ProgrammaticMainWindow*) {}


    void Builder::buildViewGui(ProgrammaticView*, ProgrammaticMainWindow*) {}


    ProgrammaticViewProxy* Builder::createViewProxy(ProgrammaticView*, ProgrammaticMainWindow*) {
        return Q_NULLPTR;
    }


    void Builder::buildViewFinal(ProgrammaticView*, ProgrammaticMainWindow*) {}


    void Builder::buildDialogGui(const QString&, ProgrammaticDialog*) {}


    ProgrammaticDialogProxy* Builder::createDialogProxy(const QString&, ProgrammaticDialog*) {
        return Q_NULLPTR;
    }


    void Builder::buildDialogFinal(const QString&, ProgrammaticDialog*) {}
}
