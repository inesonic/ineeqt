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
* This file implements the \ref EQt::ComboBox class.
***********************************************************************************************************************/

#include <QComboBox>
#include <QWidget>
#include <QAction>
#include <QList>
#include <QEvent>
#include <QHelpEvent>

#include "eqt_common.h"
#include "eqt_combo_box.h"

namespace EQt {
    ComboBox::ComboBox(QWidget* parent):QComboBox(parent) {
        configureWidget();
    }


    ComboBox::ComboBox(QAction* action, QWidget* parent):QComboBox(parent) {
        configureWidget();
        setDefaultAction(action);
    }


    ComboBox::ComboBox(ComboBox::ReportedValue reportedValueType, QWidget* parent):QComboBox(parent) {
        configureWidget();
        setReportedValueType(reportedValueType);
    }


    ComboBox::ComboBox(QAction* action, ComboBox::ReportedValue reportedValueType, QWidget* parent):QComboBox(parent) {
        configureWidget();
        setReportedValueType(reportedValueType);
        setDefaultAction(action);
    }


    ComboBox::~ComboBox() {}


    ComboBox::ReportedValue ComboBox::reportedValueType() const {
        return currentReportedValueType;
    }


    QAction* ComboBox::defaultAction() {
        QList<QAction*> actionsList = actions();
        if (actionsList.empty()) {
            return Q_NULLPTR;
        } else {
            Q_ASSERT(actionsList.size() == 1);
            return actionsList.first();
        }
    }


    void ComboBox::setReportedValueType(ComboBox::ReportedValue newReportedValueType) {
        currentReportedValueType = newReportedValueType;
    }


    void ComboBox::setDefaultAction(QAction* action) {
        QList<QAction*> actionsList = actions();
        if (!actionsList.empty()) {
            Q_ASSERT(actionsList.size() == 1);

            QAction* oldAction = actionsList.first();
            disconnect(oldAction, SIGNAL(triggered()), this, SLOT(triggered()));
            disconnect(oldAction, SIGNAL(changed()), this, SLOT(changed()));

            removeAction(oldAction);
        }

        addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(triggered()));
        connect(action, SIGNAL(changed()), this, SLOT(changed()));

        if (toolTip().isEmpty() && !action->toolTip().isEmpty()) {
            setToolTip(action->toolTip());
        }

        if (statusTip().isEmpty() && !action->statusTip().isEmpty()) {
            setStatusTip(action->statusTip());
        }

        if (whatsThis().isEmpty() && !action->whatsThis().isEmpty()) {
            setWhatsThis(action->whatsThis());
        }
    }


    bool ComboBox::event(QEvent* event) {
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

        return QComboBox::event(event);
    }


    void ComboBox::indexChanged(int index) {
        QList<QAction*> actionsList = actions();
        if (!actionsList.empty()) {
            Q_ASSERT(actionsList.size() == 1);

            QAction* action = actionsList.first();

            QVariant reportedValue;
            switch (currentReportedValueType) {
                case ReportedValue::INDEX: {
                    reportedValue = QVariant::fromValue(index);
                    break;
                }

                case ReportedValue::TEXT: {
                    if (index >= 0) {
                        reportedValue = QVariant::fromValue(itemText(index));
                    }

                    break;
                }

                case ReportedValue::VARIANT: {
                    if (index >= 0) {
                        reportedValue = itemData(index);
                    }

                    break;
                }

                default: {
                    Q_ASSERT(false);
                    break;
                }
            }

            if (action->data() != reportedValue) {
                action->setData(reportedValue);
                action->trigger();
            }
        }
    }


    void ComboBox::triggered() {
        if (sender() != this) {
            QList<QAction*> actionsList = actions();
            Q_ASSERT(actionsList.size() == 1);

            QAction* action  = actionsList.first();
            QVariant variant = action->data();

            int  index = -1;
            bool ok    = true;
            switch (currentReportedValueType) {
                case ReportedValue::INDEX: {
                    index = variant.toInt(&ok);
                    break;
                }

                case ReportedValue::TEXT: {
                    QString textValue = variant.toString();
                    index = 0;
                    while (index < count() && itemText(index) != textValue) {
                        ++index;
                    }

                    if (index >= count()) {
                        index = -1;
                    }

                    break;
                }

                case ReportedValue::VARIANT: {
                    index = 0;
                    while (index < count() && itemData(index) != variant) {
                        ++index;
                    }

                    if (index >= count()) {
                        index = -1;
                    }
                }
            }

            if (ok) {
                setCurrentIndex(index);
            }
        }
    }


    void ComboBox::changed() {
        QList<QAction*> actionsList = actions();
        if (!actionsList.empty()) {
            Q_ASSERT(actionsList.size() == 1);

            QAction* action = actionsList.first();

            if (action->isEnabled() != isEnabled()) {
                setEnabled(action->isEnabled());
            }
        }
    }


    void ComboBox::configureWidget() {
        currentReportedValueType = ReportedValue::INDEX;

        connect(
            this,
            static_cast<void (ComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &ComboBox::indexChanged
        );
    }
}
