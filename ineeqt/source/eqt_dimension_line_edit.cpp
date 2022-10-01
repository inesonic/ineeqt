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
* This file implements the \ref EQt::DimensionLineEdit class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QString>
#include <QSettings>
#include <QVariant>
#include <QFontMetrics>

#include "eqt_application.h"
#include "eqt_dimension_validator.h"
#include "eqt_line_edit.h"
#include "eqt_dimension_line_edit.h"

namespace EQt {
    DimensionLineEdit::DimensionLineEdit(QWidget* parent):LineEdit(parent) {
        configureWidget();
    }


    DimensionLineEdit::DimensionLineEdit(QAction* action, QWidget* parent):LineEdit(action, parent) {
        configureWidget();
    }


    DimensionLineEdit::~DimensionLineEdit() {}


    void DimensionLineEdit::loadSettings(
            QSettings*              settings,
            const QString&          entryName,
            DimensionLineEdit::Unit defaultUnit
        ) {
        Unit unit = defaultUnit;
        if (settings->contains(entryName)) {
            QString settingValue = settings->value(entryName).toString();
            if (settingValue == QString("MILLIMETER")) {
                unit = EQt::DimensionLineEdit::Unit::MILLIMETER;
            } else if (settingValue == QString("CENTIMETER")) {
                unit = EQt::DimensionLineEdit::Unit::CENTIMETER;
            } else if (settingValue == QString("METER")) {
                unit = EQt::DimensionLineEdit::Unit::METER;
            } else if (settingValue == QString("POINT")) {
                unit = EQt::DimensionLineEdit::Unit::POINT;
            } else if (settingValue == QString("PICA")) {
                unit = EQt::DimensionLineEdit::Unit::PICA;
            } else if (settingValue == QString("INCH")) {
                unit = EQt::DimensionLineEdit::Unit::INCH;
            } else if (settingValue == QString("FOOT")) {
                unit = EQt::DimensionLineEdit::Unit::FOOT;
            } else if (settingValue == QString("PER_REGION")) {
                unit = EQt::DimensionLineEdit::Unit::PER_REGION;
            }
        }

        setUnits(unit);
    }


    void DimensionLineEdit::saveSettings(QSettings* settings, const QString& widgetName) {
        Unit    unit = units();
        QString settingValue;

        switch (unit) {
            case EQt::DimensionLineEdit::Unit::INVALID:    { settingValue = QString("INVALID");    break; }
            case EQt::DimensionLineEdit::Unit::MILLIMETER: { settingValue = QString("MILLIMETER"); break; }
            case EQt::DimensionLineEdit::Unit::CENTIMETER: { settingValue = QString("CENTIMETER"); break; }
            case EQt::DimensionLineEdit::Unit::METER:      { settingValue = QString("METER");      break; }
            case EQt::DimensionLineEdit::Unit::POINT:      { settingValue = QString("POINT");      break; }
            case EQt::DimensionLineEdit::Unit::PICA:       { settingValue = QString("PICA");       break; }
            case EQt::DimensionLineEdit::Unit::INCH:       { settingValue = QString("INCH");       break; }
            case EQt::DimensionLineEdit::Unit::FOOT:       { settingValue = QString("FOOT");       break; }
            case EQt::DimensionLineEdit::Unit::PER_REGION: { settingValue = QString("PER_REGION"); break; }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        settings->setValue(widgetName, settingValue);
    }


    bool DimensionLineEdit::valueOk() {
        return dimensionValidator->valueOk(text());
    }


    double DimensionLineEdit::value() {
        return dimensionValidator->value(text());
    }


    DimensionLineEdit::Unit DimensionLineEdit::units() const {
        return dimensionValidator->units();
    }


    bool DimensionLineEdit::dynamicUnits() const {
        return currentDynamicUnits;
    }


    bool DimensionLineEdit::staticUnits() const {
        return !currentDynamicUnits;
    }


    double DimensionLineEdit::minimumValue() const {
        return dimensionValidator->minimumValue();
    }


    double DimensionLineEdit::maximumValue() const {
        return dimensionValidator->maximumValue();
    }


    void DimensionLineEdit::setValue(double newValue) {
        QString valueString = dimensionValidator->text(newValue);
        setText(valueString);
        repaint();
    }


    void DimensionLineEdit::setUnits(DimensionLineEdit::Unit newUnits) {
        if (currentDynamicUnits && !text().isEmpty()) {
            double currentValue = value();
            dimensionValidator->setUnits(newUnits);
            QString newText = dimensionValidator->text(currentValue);
            setText(newText);
        } else {
            dimensionValidator->setUnits(newUnits);
        }
    }


    void DimensionLineEdit::setDynamicUnits(bool nowDynamicUnits) {
        currentDynamicUnits = nowDynamicUnits;
    }


    void DimensionLineEdit::setStaticUnits(bool nowStaticUnits) {
        currentDynamicUnits = !nowStaticUnits;
    }


    void DimensionLineEdit::setMinimumValue(double newMinimumValue) {
        dimensionValidator->setMinimumValue(newMinimumValue);

        bool   ok;
        double currentValue = dimensionValidator->value(text(), &ok);

        if (!ok || currentValue < newMinimumValue) {
            setValue(newMinimumValue);
        }
    }


    void DimensionLineEdit::setMaximumValue(double newMaximumValue) {
        dimensionValidator->setMaximumValue(newMaximumValue);

        bool   ok;
        double currentValue = dimensionValidator->value(text(), &ok);

        if (!ok || currentValue > newMaximumValue) {
            setValue(newMaximumValue);
        }
    }


    void DimensionLineEdit::updated(const QString& newText) {
        if (currentDynamicUnits) {
            bool ok;
            Unit newUnit = dimensionValidator->units(newText, &ok);

            if (ok && newUnit != Unit::INVALID) {
                dimensionValidator->setUnits(newUnit);
            }
        }
    }


    QString DimensionLineEdit::variantToText(const QVariant& variant) {
        QString result;

        if (variant.isValid()) {
            bool    valueOk;
            double  newValue = variant.toDouble(&valueOk);

            if (valueOk) {
                result = dimensionValidator->text(newValue);
            } else {
                result = dimensionValidator->text(0);
            }
        }

        return result;
    }


    QVariant DimensionLineEdit::textToVariant(const QString& text) {
        QVariant result;

        bool   ok;
        double newValue = dimensionValidator->value(text, &ok);

        if (ok) {
            result = QVariant::fromValue(newValue);
        }

        return result;
    }


    void DimensionLineEdit::editingFinished() {
        updated(text());

        double newValue = value();
        if (autoUpdateEnabled()) {
            setValue(newValue);
        }

        emit valueChanged(newValue);
    }


    void DimensionLineEdit::configureWidget() {
        dimensionValidator = new DimensionValidator(this);
        setValidator(dimensionValidator);

        currentDynamicUnits = false;

        setClearButtonEnabled(false);

        QFontMetrics metrics(Application::font());
        setFixedWidth(metrics.horizontalAdvance(tr("8.8888 xx")));

        connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
    }
}
