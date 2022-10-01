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
* This file implements the \ref EQt::DoubleSpinBox class.
***********************************************************************************************************************/

#include <QDoubleSpinBox>
#include <QWidget>
#include <QAction>
#include <QList>

#include "eqt_double_spin_box.h"

namespace EQt {
    DoubleSpinBox::DoubleSpinBox(QWidget* parent):QDoubleSpinBox(parent) {
        configureWidget();
    }


    DoubleSpinBox::DoubleSpinBox(QAction* action, QWidget* parent):QDoubleSpinBox(parent) {
        configureWidget();
        setDefaultAction(action);
    }


    DoubleSpinBox::~DoubleSpinBox() {}


    QAction* DoubleSpinBox::defaultAction() {
        if (actions().empty()) {
            return Q_NULLPTR;
        } else {
            Q_ASSERT(actions().size() == 1);
            return actions().first();
        }
    }


    void DoubleSpinBox::setDefaultAction(QAction* action) {
        if (!actions().empty()) {
            Q_ASSERT(actions().size() == 1);

            QAction* oldAction = actions().first();
            disconnect(oldAction, SIGNAL(triggered()), this, SLOT(triggered()));
            disconnect(oldAction, SIGNAL(changed()), this, SLOT(changed()));

            removeAction(oldAction);
        }

        addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(triggered()));
        connect(action, SIGNAL(changed()), this, SLOT(changed()));
    }


    void DoubleSpinBox::valueChanged(double newValue) {
        if (!actions().empty()) {
            Q_ASSERT(actions().size() == 1);

            QAction* action = actions().first();

            action->setData(newValue);
            action->trigger();
        }
    }


    void DoubleSpinBox::triggered() {
        if (sender() != this) {
            Q_ASSERT(actions().size() == 1);
            QAction* action = actions().first();

            QVariant variant = action->data();

            bool   ok;
            double newValue = variant.toDouble(&ok);
            if (ok && value() != newValue) {
                setValue(newValue);
            }
        }
    }


    void DoubleSpinBox::changed() {
        if (!actions().empty()) {
            Q_ASSERT(actions().size() == 1);

            QAction* action = actions().first();

            if (action->isEnabled() != isEnabled()) {
                setEnabled(action->isEnabled());
            }
        }
    }


    void DoubleSpinBox::configureWidget() {
        connect(this, SIGNAL(valueChanged(double)), this, SLOT(valueChanged(double)));
    }
}
