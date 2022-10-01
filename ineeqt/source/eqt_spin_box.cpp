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
* This file implements the \ref EQt::SpinBox class.
***********************************************************************************************************************/

#include <QSpinBox>
#include <QWidget>
#include <QAction>
#include <QList>

#include "eqt_spin_box.h"

namespace EQt {
    SpinBox::SpinBox(QWidget* parent):QSpinBox(parent) {
        configureWidget();
    }


    SpinBox::SpinBox(QAction* action, QWidget* parent):QSpinBox(parent) {
        configureWidget();
        setDefaultAction(action);
    }


    SpinBox::~SpinBox() {}


    QAction* SpinBox::defaultAction() {
        if (actions().empty()) {
            return Q_NULLPTR;
        } else {
            Q_ASSERT(actions().size() == 1);
            return actions().first();
        }
    }


    void SpinBox::setDefaultAction(QAction* action) {
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


    void SpinBox::valueHasChanged(int i) {
        if (!actions().empty()) {
            Q_ASSERT(actions().size() == 1);

            QAction* action = actions().first();

            action->setData(i);
            action->trigger();
        }
    }


    void SpinBox::triggered() {
        Q_ASSERT(actions().size() == 1);
        QAction* action = actions().first();

        QVariant variant = action->data();

        bool ok;
        int  newValue = variant.toInt(&ok);
        if (ok && value() != newValue) {
            setValue(newValue);
        }
    }


    void SpinBox::changed() {
        if (!actions().empty()) {
            Q_ASSERT(actions().size() == 1);

            QAction* action = actions().first();

            if (action->isEnabled() != isEnabled()) {
                setEnabled(action->isEnabled());
            }
        }
    }


    void SpinBox::configureWidget() {
        connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueHasChanged(int)));
    }
}
