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
* This file implements the \ref EQt::SearchLineEdit class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QKeySequence>
#include <QApplication>
#include <QClipboard>

#include "eqt_application.h"
#include "eqt_line_edit.h"
#include "eqt_search_line_edit.h"

namespace EQt {
    SearchLineEdit::SearchLineEdit(QWidget* parent):EQt::LineEdit(parent) {
        configureWidget();
    }


    SearchLineEdit::SearchLineEdit(QAction* action, QWidget* parent):EQt::LineEdit(action, parent) {
        configureWidget();
    }


    SearchLineEdit::~SearchLineEdit() {}


    void SearchLineEdit::setCaseSensitiveSearchAction(QAction* action) {
        currentCaseSensitiveSearchAction = action;
    }


    QAction* SearchLineEdit::caseSensitiveSearchAction() const {
        return currentCaseSensitiveSearchAction;
    }


    void SearchLineEdit::setWholeWordsOnlyAction(QAction* action) {
        currentWholeWordsOnlyAction = action;
    }


    QAction* SearchLineEdit::wholeWordsOnlyAction() const {
        return currentWholeWordsOnlyAction;
    }


    void SearchLineEdit::setRegularExpressionAction(QAction* action) {
        currentRegularExpressionSearchAction = action;
    }


    QAction* SearchLineEdit::regularExpressionAction() const {
        return currentRegularExpressionSearchAction;
    }


    void SearchLineEdit::contextMenuEvent(QContextMenuEvent* event) {
        QMenu* contextMenu = new QMenu();

        if (currentCaseSensitiveSearchAction != Q_NULLPTR) {
            contextMenu->addAction(currentCaseSensitiveSearchAction);
        }

        if (currentWholeWordsOnlyAction != Q_NULLPTR) {
            contextMenu->addAction(currentWholeWordsOnlyAction);
        }

        if (currentRegularExpressionSearchAction != Q_NULLPTR) {
            contextMenu->addAction(currentRegularExpressionSearchAction);
        }

        if (currentCaseSensitiveSearchAction != Q_NULLPTR     ||
            currentWholeWordsOnlyAction != Q_NULLPTR          ||
            currentRegularExpressionSearchAction != Q_NULLPTR    ) {
            contextMenu->addSeparator();
        }

        QAction* undoAction = contextMenu->addAction(Application::icon("edit_undo"), tr("Undo"));
        QAction* redoAction = contextMenu->addAction(Application::icon("edit_redo"), tr("Redo"));
        contextMenu->addSeparator();
        QAction* cutAction = contextMenu->addAction(Application::icon("edit_cut"), tr("Cut"));
        QAction* copyAction = contextMenu->addAction(Application::icon("edit_copy"), tr("Copy"));
        QAction* pasteAction = contextMenu->addAction(Application::icon("edit_paste"), tr("Paste"));
        QAction* deleteAction = contextMenu->addAction(tr("Delete"));
        contextMenu->addSeparator();
        QAction* selectAllAction = contextMenu->addAction(tr("Select All"));

        undoAction->setToolTip("Undo");
        redoAction->setToolTip("Redo");
        cutAction->setToolTip("Cut");
        copyAction->setToolTip("Copy");
        pasteAction->setToolTip("Paste");
        deleteAction->setToolTip("Delete");
        selectAllAction->setToolTip("Select All");

        undoAction->setShortcut(QKeySequence::Undo);
        redoAction->setShortcut(QKeySequence::Redo);
        cutAction->setShortcut(QKeySequence::Cut);
        copyAction->setShortcut(QKeySequence::Copy);
        pasteAction->setShortcut(QKeySequence::Paste);
        deleteAction->setShortcut(QKeySequence::Delete);
        selectAllAction->setShortcut(QKeySequence::SelectAll);

        undoAction->setEnabled(isUndoAvailable());
        redoAction->setEnabled(isRedoAvailable());
        cutAction->setEnabled(hasSelectedText());
        copyAction->setEnabled(hasSelectedText());
        pasteAction->setEnabled(!QApplication::clipboard()->text().isEmpty());
        deleteAction->setEnabled(!text().isEmpty());
        selectAllAction->setEnabled(!text().isEmpty());

        connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));
        connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));
        connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
        connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
        connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(clear()));
        connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));

        contextMenu->exec(event->globalPos());
        delete contextMenu;
    }


    void SearchLineEdit::configureWidget() {
        currentCaseSensitiveSearchAction     = Q_NULLPTR;
        currentWholeWordsOnlyAction          = Q_NULLPTR;
        currentRegularExpressionSearchAction = Q_NULLPTR;

        setToolTip(tr("Search"));
        setClearButtonEnabled(true);
    }
}
