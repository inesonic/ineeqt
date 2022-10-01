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
* This file implements the \ref EQt::ListValidatorBase class.
***********************************************************************************************************************/

#include <QString>
#include <QObject>
#include <QValidator>

#include "eqt_common.h"
#include "eqt_list_validator_base.h"

namespace EQt {
    ListValidatorBase::ListValidatorBase(QObject* parent):QValidator(parent) {
        currentPrefix = QString();
    }


    ListValidatorBase::ListValidatorBase(const QString& prefix, QObject* parent):QValidator(parent) {
        currentPrefix   = prefix;
    }


    ListValidatorBase::~ListValidatorBase() {}


    void ListValidatorBase::setPrefix(const QString& newPrefix) {
        currentPrefix = newPrefix;
    }


    QString ListValidatorBase::prefix() const {
        return currentPrefix;
    }


    void ListValidatorBase::fixup(QString& input) const {
        unsigned prefixLength = static_cast<unsigned>(currentPrefix.length());
        unsigned inputLength  = static_cast<unsigned>(input.length());

        bool prefixValid;
        if (prefixLength > 0 && inputLength >= prefixLength) {
            QString enteredPrefix = input.left(prefixLength);
            prefixValid = (enteredPrefix == currentPrefix);
        } else {
            prefixValid = true;
        }

        if (prefixValid) {
            QString suffix    = inputLength > prefixLength ? input.mid(prefixLength) : QString();
            QString bestMatch = closestMatch(suffix);

            if (suffix != bestMatch) {
                unsigned suffixLength    = static_cast<unsigned>(suffix.length());
                unsigned bestMatchLength = static_cast<unsigned>(bestMatch.length());

                if (suffixLength < bestMatchLength) {
                    QString bestMatchPartial = bestMatch.left(suffixLength);

                    if (bestMatchPartial == suffix) {
                        input = currentPrefix + bestMatch;
                    }
                }
            }
        } else {
            input = currentPrefix;
        }
    }


    ListValidatorBase::State ListValidatorBase::validate(QString& input, int&) const {
        ListValidatorBase::State result;

        unsigned prefixLength = static_cast<unsigned>(currentPrefix.length());
        unsigned inputLength  = static_cast<unsigned>(input.length());

        bool prefixValid;
        if (prefixLength > 0) {
            if (inputLength >= prefixLength) {
                QString enteredPrefix = input.left(prefixLength);
                prefixValid = (enteredPrefix == currentPrefix);
            } else {
                prefixValid = false;
            }
        } else {
            prefixValid = true;
        }

        if (prefixValid) {
            QString suffix    = inputLength > prefixLength ? input.mid(prefixLength) : QString();
            QString bestMatch = closestMatch(suffix);

            if (suffix == bestMatch) {
                result = ListValidatorBase::Acceptable;
            } else {
                unsigned suffixLength    = static_cast<unsigned>(suffix.length());
                unsigned bestMatchLength = static_cast<unsigned>(bestMatch.length());

                if (suffixLength < bestMatchLength) {
                    QString bestMatchPartial = bestMatch.left(suffixLength);

                    if (bestMatchPartial == suffix) {
                        result = ListValidatorBase::Intermediate;
                    } else {
                        result = ListValidatorBase::Invalid;
                    }
                } else {
                    result = ListValidatorBase::Invalid;
                }
            }
        } else {
            result = ListValidatorBase::Invalid;
        }

        return result;
    }
}
