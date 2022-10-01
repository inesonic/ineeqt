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
* This file implements the \ref EQt::LineEdit class.
***********************************************************************************************************************/

#include <QComboBox>
#include <QWidget>
#include <QAction>
#include <QList>
#include <QValidator>
#include <QFocusEvent>

#include "eqt_line_edit.h"

namespace EQt {
    LineEdit::LineEdit(QWidget* parent):QLineEdit(parent) {
        configureWidget();
    }


    LineEdit::LineEdit(QAction* action, QWidget* parent):QLineEdit(parent) {
        configureWidget();
        setDefaultAction(action);
    }


    LineEdit::~LineEdit() {}


    QAction* LineEdit::defaultAction() {
        QList<QAction*> actionsList = actions();
        if (actionsList.empty()) {
            return Q_NULLPTR;
        } else {
            Q_ASSERT(actionsList.size() == 1);
            return actionsList.first();
        }
    }


    bool LineEdit::autoUpdateEnabled() const {
        return currentAutoUpdate;
    }


    bool LineEdit::autoUpdateDisabled() const {
        return !currentAutoUpdate;
    }


    void LineEdit::setDynamicUpdate(bool nowDynamicUpdate) {
        if (currentDynamicUpdate && !nowDynamicUpdate) {
            currentDynamicUpdate = false;
            disconnect(this, &LineEdit::textEdited, this, &LineEdit::textWasEdited);
        } else if (!currentDynamicUpdate && nowDynamicUpdate) {
            currentDynamicUpdate = true;
            connect(this, &LineEdit::textEdited, this, &LineEdit::textWasEdited);
        }
    }


    void LineEdit::setDefaultAction(QAction* action) {
        QList<QAction*> actionsList = actions();
        if (!actionsList.empty()) {
            Q_ASSERT(actionsList.size() == 1);

            QAction* oldAction = actionsList.first();
            disconnect(oldAction, &QAction::triggered, this, &LineEdit::triggered);
            disconnect(oldAction, &QAction::changed, this, &LineEdit::changed);

            removeAction(oldAction);
        }

        addAction(action);
        connect(action, &QAction::triggered, this, &LineEdit::triggered);
        connect(action, &QAction::changed, this, &LineEdit::changed);

        setText(variantToText(action->data()));
    }


    void LineEdit::setAutoUpdateEnabled(bool nowEnabled) {
        currentAutoUpdate = nowEnabled;
    }


    void LineEdit::setAutoUpdateDisabled(bool nowDisabled) {
        setAutoUpdateEnabled(!nowDisabled);
    }


    void LineEdit::textWasEdited(const QString& text) {
        Q_ASSERT(currentDynamicUpdate);
        updateAction(text, false);
    }


    void LineEdit::triggered() {
        if (sender() != this) {
            Q_ASSERT(actions().size() == 1);
            QAction* action = actions().first();

            QString actionText = variantToText(action->data());
            if (actionText != text()) {
                setText(actionText);
            }
        }
    }


    void LineEdit::changed() {
        if (sender() != this) {
            Q_ASSERT(actions().size() == 1);
            QAction* action = actions().first();

            if (action->isEnabled() != isEnabled()) {
                setEnabled(action->isEnabled());
            }
        }
    }


    void LineEdit::returnWasPressed() {
        if (!currentDynamicUpdate) {
            updateAction(text(), true);
        }
    }


    void LineEdit::updated(const QString&) {}


    bool LineEdit::event(QEvent* event) {
        QEvent::Type eventType = event->type();
        if (eventType == QEvent::Type::ToolTip   ||
            eventType == QEvent::Type::WhatsThis ||
            eventType == QEvent::Type::StatusTip    ) {
            QList<QAction*> actionsList = actions();
            if (!actionsList.empty()) {
                Q_ASSERT(actionsList.size() == 1);
                QAction* action = actionsList.first();

                if (eventType == QEvent::Type::ToolTip && !action->toolTip().isEmpty()) {
                    setToolTip(action->toolTip());
                } else if (eventType == QEvent::Type::WhatsThis && !action->whatsThis().isEmpty()) {
                    setWhatsThis(action->whatsThis());
                } else if (eventType == QEvent::Type::StatusTip && !action->statusTip().isEmpty()) {
                    setStatusTip(action->statusTip());
                }
            }
        }

        return QLineEdit::event(event);
    }


    void LineEdit::focusInEvent(QFocusEvent* event) {
        QLineEdit::focusInEvent(event);
        emit receivedFocus();
    }


    void LineEdit::focusOutEvent(QFocusEvent* event) {
        QLineEdit::focusOutEvent(event);
        returnWasPressed();
        emit lostFocus();
    }


    QString LineEdit::variantToText(const QVariant& variant) {
        return variant.toString();
    }


    QVariant LineEdit::textToVariant(const QString& text) {
        return QVariant(text);
    }


    void LineEdit::configureWidget() {
        currentDynamicUpdate = false;
        currentAutoUpdate    = false;
        connect(this, &LineEdit::returnPressed, this, &LineEdit::returnWasPressed);
    }


    void LineEdit::updateAction(const QString& text, bool updateText) {
        updated(text);

        QAction* action;
        if (!actions().isEmpty()) {
            action = actions().first();
        } else {
            action = Q_NULLPTR;
        }

        QVariant v = textToVariant(text);

        if (updateText && currentAutoUpdate) {
            if (v.isValid()) {
                setText(variantToText(v));
            } else if (action != Q_NULLPTR){
                setText(variantToText(action->data()));
            }
        }

        if (v.isValid() && action != Q_NULLPTR) {
            if (action->data() != v) {
                action->setData(v);
                action->trigger();
            }
        }
    }
}
