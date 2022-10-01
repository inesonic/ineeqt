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
* This file implements the \ref EQt::DimensionValidator class.
***********************************************************************************************************************/

#include <QString>
#include <QLocale>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionValidator>

#include <limits>

#include <util_units.h>

#include "eqt_dimension_validator.h"

namespace EQt {
    DimensionValidator::DimensionValidator(QObject* parent):QValidator(parent) {
        configure();
    }


    DimensionValidator::DimensionValidator(double maximumValue, QObject* parent):QValidator(parent) {
        configure();
        setMaximumValue(maximumValue);
    }


    DimensionValidator::DimensionValidator(
            double   minimumValue,
            double   maximumValue,
            QObject* parent
        ):QValidator(
            parent
        ) {
        configure();
        setMinimumValue(minimumValue);
        setMaximumValue(maximumValue);
    }


    DimensionValidator::~DimensionValidator() {}


    DimensionValidator::State DimensionValidator::validate(QString& input, int&) const {
        State result;

       QRegularExpressionMatch match = validatorRegularExpression.match(
            input,
            0,
            QRegularExpression::PartialPreferCompleteMatch
        );

        if (match.hasPartialMatch()) {
            result = DimensionValidator::Intermediate;
        } else if (match.hasMatch()) {
            QString valueString = match.captured(1);
            if (valueString.isEmpty() || valueString == tr(".") || valueString == tr("-")) {
                result = DimensionValidator::Intermediate;
            } else {
                bool   ok;
                double pointValue = value(input, &ok);

                if (ok) {
                    if (pointValue >= currentMinimumValue && pointValue <= currentMaximumValue) {
                        result = DimensionValidator::Acceptable;
                    } else if (pointValue == 0) {
                        result = DimensionValidator::Intermediate;
                    } else {
                        result = DimensionValidator::Invalid;
                    }
                } else {
                    result = DimensionValidator::Invalid;
                }
            }
        } else {
            result = DimensionValidator::Invalid;
        }

        return result;
    }


    bool DimensionValidator::valueOk(const QString& text) {
        int position = 0;
        QString localText = text;
        return validate(localText, position) == DimensionValidator::Acceptable;
    }


    double DimensionValidator::value(const QString& text, bool* ok) const {
        double                  result  = 0;
        bool                    success = false;
        QRegularExpressionMatch match   = validatorRegularExpression.match(text);

        if (match.hasMatch()) {
            QString valueString = match.captured(1);
            QString units       = match.captured(2);

            double rawValue = valueString.toDouble(&success);
            if (success) {
                double scaleFactor = 0;

                if (units == tr("mm")) {
                    scaleFactor = Util::Units::pointsPerMm;
                } else if (units == tr("cm")) {
                    scaleFactor = Util::Units::pointsPerCm;
                } else if (units == tr("m") || units == tr("meter") || units == tr("meters")) {
                    scaleFactor = 1000.0 * Util::Units::pointsPerMm;
                } else if (units == tr("pt") || units == tr("point") || units == tr("points")) {
                    scaleFactor = 1.0;
                } else if (units == tr("pc") || units == tr("pica") || units == tr("picas")) {
                    scaleFactor = 12.0;
                } else if (units == tr("in") || units == tr("inch") || units == tr("inches") || units == "\"") {
                    scaleFactor = Util::Units::pointsPerInch;
                } else if (units == tr("ft") || units == tr("foot") || units == tr("feet")) {
                    scaleFactor = 12.0 * Util::Units::pointsPerInch;
                } else if (units.isEmpty()) {
                    switch (currentUnits) {
                        case Unit::MILLIMETER: {
                            scaleFactor = Util::Units::pointsPerMm;
                            break;
                        }

                        case Unit::CENTIMETER: {
                            scaleFactor = Util::Units::pointsPerCm;
                            break;
                        }

                        case Unit::METER: {
                            scaleFactor = 1000.0 * Util::Units::pointsPerMm;
                            break;
                        }

                        case Unit::POINT: {
                            scaleFactor = 1.0;
                            break;
                        }

                        case Unit::PICA: {
                            scaleFactor = 12.0;
                            break;
                        }

                        case Unit::INCH: {
                            scaleFactor = Util::Units::pointsPerInch;
                            break;
                        }

                        case Unit::FOOT: {
                            scaleFactor = 12.0 * Util::Units::pointsPerInch;
                            break;
                        }

                        case Unit::PER_REGION: {
                            QLocale::MeasurementSystem measurementSystem = locale().measurementSystem();

                            if (measurementSystem == QLocale::MeasurementSystem::ImperialUSSystem ||
                                measurementSystem == QLocale::MeasurementSystem::ImperialUKSystem    ) {
                                scaleFactor = Util::Units::pointsPerInch;
                            } else {
                                scaleFactor = Util::Units::pointsPerMm;
                            }

                            break;
                        }

                        default: {
                            Q_ASSERT(false);
                            break;
                        }
                    }
                }

                if (scaleFactor > 0) {
                    result  = scaleFactor * rawValue;
                    success = true;
                } else {
                    result  = 0;
                    success = false;
                }
            }
        }

        if (ok != Q_NULLPTR) {
            *ok = success;
        }

        return result;
    }


    DimensionValidator::Unit DimensionValidator::units(const QString& text, bool* ok) const {
        Unit                    result  = Unit::INVALID;
        bool                    success = false;
        QRegularExpressionMatch match   = validatorRegularExpression.match(text);

        if (match.hasMatch()) {
            QString units = match.captured(2);

            if (units == tr("mm")) {
                result  = Unit::MILLIMETER;
                success = true;
            } else if (units == tr("cm")) {
                result  = Unit::CENTIMETER;
                success = true;
            } else if (units == tr("m") || units == tr("meter") || units == tr("meters")) {
                result  = Unit::METER;
                success = true;
            } else if (units == tr("pt") || units == tr("point") || units == tr("points")) {
                result  = Unit::POINT;
                success = true;
            } else if (units == tr("pc") || units == tr("pica") || units == tr("picas")) {
                result  = Unit::PICA;
                success = true;
            } else if (units == tr("in") || units == tr("inch") || units == tr("inches") || units == tr("\"")) {
                result  = Unit::INCH;
                success = true;
            } else if (units == tr("ft") || units == tr("foot") || units == tr("feet")) {
                result  = Unit::FOOT;
                success = true;
            } else if (units.isEmpty()) {
                result  = currentUnits;
                success = true;
            }
        }

        if (ok != Q_NULLPTR) {
            *ok = success;
        }

        return result;
    }


    QString DimensionValidator::text(double value) const {
        QString result;

        switch (currentUnits) {
            case Unit::MILLIMETER: {
                result = tr("%1 mm").arg(QString::number(value/Util::Units::pointsPerMm, 'f', 1));
                break;
            }

            case Unit::CENTIMETER: {
                result = tr("%1 cm").arg(QString::number(value/Util::Units::pointsPerCm, 'f', 2));
                break;

            }

            case Unit::METER: {
                result = tr("%1 m").arg(QString::number(value/(1000.0 * Util::Units::pointsPerMm), 'f', 4));
                break;
            }

            case Unit::POINT: {
                result = tr("%1 pt").arg(QString::number(value, 'f', 1));
                break;
            }

            case Unit::PICA: {
                result = tr("%1 pc").arg(QString::number(value / 12.0, 'f', 2));
                break;
            }

            case Unit::INCH: {
                result = tr("%1\"").arg(QString::number(value/Util::Units::pointsPerInch, 'f', 3));
                break;
            }

            case Unit::FOOT: {
                result = tr("%1'").arg(QString::number(value/(12.0 * Util::Units::pointsPerInch), 'f', 4));
                break;
            }

            case Unit::PER_REGION: {
                QLocale::MeasurementSystem measurementSystem = locale().measurementSystem();

                if (measurementSystem == QLocale::MeasurementSystem::ImperialUSSystem ||
                    measurementSystem == QLocale::MeasurementSystem::ImperialUKSystem    ) {
                    result = tr("%1\"").arg(QString::number(value/Util::Units::pointsPerInch, 'f', 3));
                } else {
                    result = tr("%1 mm").arg(QString::number(value/Util::Units::pointsPerMm, 'f', 1));
                }

                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }

        return result;
    }


    DimensionValidator::Unit DimensionValidator::units() const {
        return currentUnits;
    }


    double DimensionValidator::minimumValue() const {
        return currentMinimumValue;
    }


    double DimensionValidator::maximumValue() const {
        return currentMaximumValue;
    }


    void DimensionValidator::setUnits(DimensionValidator::Unit newUnits) {
        currentUnits = newUnits;
    }


    void DimensionValidator::setMinimumValue(double newMinimumValue) {
        currentMinimumValue = newMinimumValue;
    }


    void DimensionValidator::setMaximumValue(double newMaximumValue) {
        currentMaximumValue = newMaximumValue;
    }


    void DimensionValidator::configure() {
        validatorRegularExpression = QRegularExpression(
            tr(
               "^(-?[0-9]*\\.?[0-9]*)\\s*"
               "(mm|cm|m|meter|meters|pt|point|points|pc|pica|picas|in|inch|inches|\"|ft|foot|feet|)?$")
        );

        currentUnits        = Unit::PER_REGION;
        currentMinimumValue = std::numeric_limits<double>::lowest();
        currentMaximumValue = std::numeric_limits<double>::max();
    }
}
