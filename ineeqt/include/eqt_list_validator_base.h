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
* This header defines the \ref EQt::ListValidatorBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_LIST_VALIDATOR_BASE_H
#define EQT_LIST_VALIDATOR_BASE_H

#include <QString>
#include <QMap>
#include <QValidator>

class QObject;

#include "eqt_common.h"

namespace EQt {
    /**
     * Pure virtual base class for validators that validate entries in a list or similar data structure.
     */
    class EQT_PUBLIC_API ListValidatorBase:public QValidator {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this menu.
             */
            ListValidatorBase(QObject* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] prefix A prefix that is forced to be in front of all entries.
             *
             * \param[in] parent The parent object for this menu.
             */
            ListValidatorBase(const QString& prefix, QObject* parent = Q_NULLPTR);

            ~ListValidatorBase() override;

            /**
             * Method you can call to define a new prefix.
             *
             * \param[in] newPrefix The new prefix string.
             */
            void setPrefix(const QString& newPrefix);

            /**
             * Method you can use to determine the prefix being enforced at the front of all strings.
             *
             * \return Returns the current prefix.
             */
            QString prefix() const;

            /**
             * Method that makes a best attempt to fix-up the string.
             *
             * \param[in,out] input The input to be fixed up.
             */
            void fixup(QString& input) const final;

            /**
             * Method you can use to determine if a string is valid.
             *
             * \param[in,out] input    The string to test.
             *
             * \param[in,out] position The current cursor location in the string.
             *
             * \return Returns the result of the validation test.  Values are: QValidator::Invalid,
             *         QValidator::Intermediate, and QValidator::Acceptable.
             */
            State validate(QString& input, int& position) const final;

        protected:
            /**
             * Method that returns the first item in the validation list that is equal to or follows the provided
             * string.
             *
             * \param[in] input The input to test against.
             *
             * \return Returns the closest string that would follow the input, in case sensitive alphabetical order.
             */
            virtual QString closestMatch(const QString& input) const = 0;

        private:
            /**
             * The current prefix.
             */
            QString currentPrefix;

            /**
             * A map that represents a sorted list to validate against.
             */
            QMap<QString, bool> currentList;
    };
}

#endif
