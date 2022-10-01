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
* This file implements tests for the \ref ProgrammaticMainWindow class.
***********************************************************************************************************************/

#include <QObject>
#include <QApplication>
#include <QStyle>
#include <QtTest/QtTest>
#include <QString>
#include <QMenu>

#include <eqt_programmatic_dock_widget.h>
#include <eqt_programmatic_main_window.h>

#include "test_programmatic_main_window.h"

void TestProgrammaticMainWindow::testActionRegistry() {
    EQt::ProgrammaticMainWindow mainWindow;

    QAction* action = mainWindow.addMenuAction(tr("File | New"));

    QCOMPARE(mainWindow.action(tr("File | New")), action);
    QCOMPARE(mainWindow.actionExists(tr("File | New")), true);
    QCOMPARE(mainWindow.actionDoesNotExist(tr("File | New")), false);
    QCOMPARE(mainWindow.actionExists("Foo"), false);
    QCOMPARE(mainWindow.actionDoesNotExist("Foo"), true);
}


void TestProgrammaticMainWindow::testAddActionMethods() {
    /* For now we won't test the activation of the actions, only that we can successfully create them. */

    EQt::ProgrammaticMainWindow mainWindow;

    QAction* action = mainWindow.addMenuAction(tr("File | New"));
    QVERIFY(action != Q_NULLPTR);

    action = mainWindow.addMenuAction(
        QApplication::style()->standardIcon(QStyle::SP_ComputerIcon),
        tr("File | Open")
    );
    QVERIFY(action != Q_NULLPTR);

    action = mainWindow.addMenuAndToolbarAction(
        "toolbar",
        QApplication::style()->standardIcon(QStyle::SP_ComputerIcon),
        tr("File | Close")
    );
    QVERIFY(action != Q_NULLPTR);
}


void TestProgrammaticMainWindow::testAddToolbarAction() {
    /* For now we won't test the activation of the actions, only that we can successfully create them. */

    EQt::ProgrammaticMainWindow mainWindow;

    QAction* action = mainWindow.addToolbarAction(
        "toolbar",
        QApplication::style()->standardIcon(QStyle::SP_ComputerIcon),
        tr("File | New")
    );
    QVERIFY(action != Q_NULLPTR);
}


void TestProgrammaticMainWindow::testAddSeparatorAndSection() {
    /* For now we won't test the activation of the actions, only that we can successfully create them. */

    EQt::ProgrammaticMainWindow mainWindow;

    mainWindow.addSeparator(tr("&File"));

    QAction* action = mainWindow.addSection(tr("File"));
    QVERIFY(action != Q_NULLPTR);
}


void TestProgrammaticMainWindow::testAddMenu() {
    /* For now we won't test the created menu, only that we can successfully create one. */

    EQt::ProgrammaticMainWindow mainWindow;

    QMenu* menu = mainWindow.addMenu(tr("File"));
    QVERIFY(menu != Q_NULLPTR);
}


void TestProgrammaticMainWindow::testSetTipAndShortcut() {
    /* For now we won't test activation, only that the actions receive the results from the method calls. */

    EQt::ProgrammaticMainWindow mainWindow;

    QAction* action = mainWindow.addMenuAction(tr("File | New"));

    mainWindow.setToolTip(tr("File | New"), tr("Snort"));
    mainWindow.setStatusTip(tr("File | New"), tr("bang"));
    mainWindow.setShortcut(tr("File | New"), QKeySequence::New);

    QCOMPARE(action->toolTip(), tr("Snort"));
    QCOMPARE(action->statusTip(), tr("bang"));
    QCOMPARE(action->shortcut(), QKeySequence(QKeySequence::New));
}


void TestProgrammaticMainWindow::testDockWidgetRegistry() {
    /* We don't test the dock widgets, only that we can create and register them. */

    EQt::ProgrammaticMainWindow mainWindow;

    EQt::ProgrammaticDockWidget* dock = mainWindow.addDockWidget(
        "Snort",
        "Snort Title",
        EQt::DockWidgetDefaults(EQt::DockWidgetDefaults::Area::TOP, true)
    );
    QCOMPARE(mainWindow.dockWidget("Snort"), dock);

    dock = new EQt::ProgrammaticDockWidget("Snort 2", &mainWindow);
    mainWindow.addDockWidget("Snort 2", dock, EQt::DockWidgetDefaults(EQt::DockWidgetDefaults::Area::TOP, true));
    QCOMPARE(mainWindow.dockWidget("Snort 2"), dock);
}
