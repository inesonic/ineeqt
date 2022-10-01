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
* This file implements the \ref EQt::SpecialValueSpinBox class.
***********************************************************************************************************************/

#include <QWidget>
#include <QMap>

#include "eqt_spin_box.h"
#include "eqt_special_value_spin_box.h"

namespace EQt {
    SpecialValueSpinBox::SpecialValueSpinBox(QWidget* parent):SpinBox(parent) {}


    SpecialValueSpinBox::SpecialValueSpinBox(QAction* action, QWidget* parent):SpinBox(action, parent) {}


    SpecialValueSpinBox::~SpecialValueSpinBox() {}


    int SpecialValueSpinBox::currentValue() const {
        return EQt::SpinBox::value();
    }


    QString SpecialValueSpinBox::specialValueText(int spinBoxValue) const {
        return QMap<int, QString>::value(spinBoxValue);
    }


    QString SpecialValueSpinBox::specialValueText() const {
        return QMap<int, QString>::value(minimum());
    }


    void SpecialValueSpinBox::setSpecialValueText(const QString& newText, int spinBoxValue) {
        insert(spinBoxValue, newText);
    }


    void SpecialValueSpinBox::setSpecialValueText(const QString& newText) {
        insert(minimum(), newText);
    }


    QString SpecialValueSpinBox::textFromValue(int numericValue) const {
        QString result;

        if (contains(numericValue)) {
            result = QMap<int, QString>::value(numericValue);
        } else {
            result = SpinBox::textFromValue(numericValue);
        }

        return result;
    }


    int SpecialValueSpinBox::valueFromText(const QString& text) const {
        int result;

        QMap<int, QString>::const_iterator it = constBegin();
        QMap<int, QString>::const_iterator end = constEnd();

        while (it != end && it.value() != text) {
            ++it;
        }

        if (it != end) {
            result = it.key();
        } else {
            result = SpinBox::valueFromText(text);
        }

        return result;
    }
}
