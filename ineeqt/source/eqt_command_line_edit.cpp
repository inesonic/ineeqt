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
* This file implements the \ref EQt::CommandLineEdit class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QSet>
#include <QKeyEvent>
#include <QValidator>
#include <QWidget>
#include <QLineEdit>

#include "eqt_common.h"
#include "eqt_command_line_edit.h"

namespace EQt {
    CommandLineEdit::CommandLineEdit(QWidget* parent):QLineEdit(parent) {
        configure();
    }


    CommandLineEdit::CommandLineEdit(const QString& contents, QWidget* parent):QLineEdit(contents, parent) {
        configure();
    }


    CommandLineEdit::~CommandLineEdit() {}


    void CommandLineEdit::setCommandCompletionEnabled(bool nowEnableCommandCompletion) {
        currentCommandCompletionEnabled = nowEnableCommandCompletion;
    }


    void CommandLineEdit::setCommandCompletionDisabled(bool nowDisableCommandCompletion) {
        currentCommandCompletionEnabled = !nowDisableCommandCompletion;
    }


    bool CommandLineEdit::commandCompletionEnabled() const {
        return currentCommandCompletionEnabled;
    }


    bool CommandLineEdit::commandCompletionDisabled() const {
        return !currentCommandCompletionEnabled;
    }


    void CommandLineEdit::setCommandCompletionKey(int key) {
        currentCommandCompletionKey = key;
    }


    int CommandLineEdit::commandCompletionKey() const {
        return currentCommandCompletionKey;
    }


    void CommandLineEdit::setSpecialKeys(const QSet<int>& keyList) {
        currentSpecialKeys = keyList;
    }


    QSet<int> CommandLineEdit::specialKeys() const {
        return currentSpecialKeys;
    }


    bool CommandLineEdit::processSpecialKey(int key) {
        emit specialKeyPressed(key);
        return false;
    }


    void CommandLineEdit::keyPressEvent(QKeyEvent* event) {
        int               key              = event->key();
        const QValidator* currentValidator = validator();
        bool              normalKeyHandling       = true;

        if (currentCommandCompletionEnabled && key == currentCommandCompletionKey && currentValidator != Q_NULLPTR) {
            QString currentText = text();
            QString newText     = currentText;

            currentValidator->fixup(newText);

            int position = cursorPosition();

            if (newText != currentText && currentValidator->validate(newText, position) != QValidator::Invalid) {
                setText(newText);
            }

            normalKeyHandling = false;
        } else if (currentSpecialKeys.contains(key)) {
            normalKeyHandling = processSpecialKey(key);
        }

        if (normalKeyHandling) {
            QLineEdit::keyPressEvent(event);
        }
    }


    void CommandLineEdit::configure() {
        currentCommandCompletionEnabled = false;
        currentCommandCompletionKey     = Qt::Key_Tab;

        currentSpecialKeys.clear();
    }
}
