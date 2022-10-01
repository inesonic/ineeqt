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
* This file is the main entry point for the ineeqt unit tests.
***********************************************************************************************************************/

#include <QDebug>

#include "application_wrapper.h"

#include "test_global_setting.h"
#include "test_signal_aggregator.h"
#include "test_unique_application.h"
#include "test_programmatic_dock_widget.h"
#include "test_programmatic_main_window.h"

int main(int argumentCount, char** argumentValues) {
    ApplicationWrapper wrapper(argumentCount, argumentValues);

    wrapper.includeTest(new TestGlobalSetting);
    wrapper.includeTest(new TestSignalAggregator);
    wrapper.includeTest(new TestUniqueApplication);
    wrapper.includeTest(new TestProgrammaticDockWidget);
    wrapper.includeTest(new TestProgrammaticMainWindow);

    int status = wrapper.exec();

    return status;
}
