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
* This header defines the \ref EQt::MapKeyValidator class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_MAP_KEY_VALIDATOR_H
#define EQT_MAP_KEY_VALIDATOR_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QValidator>

#include "eqt_list_validator_base.h"

namespace EQt {
    /**
     * Class that validates input using the keys in a map.  The validator allows you to specify a prefix that is
     * prepended in front of each entry that is forced to be present.
     */
    template<typename T> class MapKeyValidator:public ListValidatorBase {
        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this menu.
             */
            MapKeyValidator(QObject* parent = Q_NULLPTR):ListValidatorBase(parent) {
                currentMap = Q_NULLPTR;
            }

            /**
             * Constructor.
             *
             * \param[in] map    Pointer to the map to validate against.
             *
             * \param[in] parent The parent object for this menu.
             */
            MapKeyValidator(const T* map, QObject* parent = Q_NULLPTR):ListValidatorBase(parent) {
                currentMap = map;
            }

            /**
             * Constructor.
             *
             * \param[in] prefix A prefix that is forced to be in front of all entries.
             *
             * \param[in] map    Pointer to the map to validate against.
             *
             * \param[in] parent The parent object for this menu.
             */
            MapKeyValidator(
                    const QString& prefix,
                    const T*       map,
                    QObject*       parent = Q_NULLPTR
                ):ListValidatorBase(
                    prefix,
                    parent
                ) {
                currentMap = map;
            }

            virtual ~MapKeyValidator() {}

            /**
             * Method you can use to specify the map to validate against.
             *
             * \param[in] newMap Pointer to the map to validate against.
             */
            void setMap(const T* newMap) {
                currentMap = newMap;
            }

            /**
             * Method you can use to obtain a poitner to the map we are validating against.
             *
             * \return Returns a pointer to the map we are currently validating against.
             */
            const T* map() const {
                return currentMap;
            }

        protected:
            /**
             * Method that returns the first item in the validation list that is equal to or follows the provided string.
             *
             * \param[in] input The input to test against.
             *
             * \return Returns the closest string that would follow the input, in case sensitive alphabetical order.
             */
            QString closestMatch(const QString& input) const override {
                Q_ASSERT(currentMap != Q_NULLPTR);

                typename T::const_iterator pos = currentMap->lowerBound(input);

                QString bestMatch;
                if (pos != currentMap->constEnd()) {
                    bestMatch = pos.key();
                }

                return bestMatch;
            }

        private:
            /**
             * Pointer to the map we are working with.
             */
            const T* currentMap;
    };
}

#endif
