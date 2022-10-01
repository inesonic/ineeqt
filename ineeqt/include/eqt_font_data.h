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
* This header defines the \ref EQt::FontData class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_FONT_DATA_H
#define EQT_FONT_DATA_H

#include <QFont>
#include <QSet>
#include <QFontInfo>

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that provides several static functions you can use to obtain additional information on a font.
     */
    class EQT_PUBLIC_API FontData {
        public:
            /**
             * Static method you can use to obtain a list of all unique weights supported by a given font.
             *
             * \param[in] font The font we wish to determine the supported weights for.
             *
             * \return Returns a list of every supported standard weight.
             */
            static QSet<QFont::Weight> uniqueWeights(const QFont& font);

            /**
             * Static method that returns a set of all standard font weights.
             *
             * \return Returns a set of all standard font weights.
             */
            static QSet<QFont::Weight> standardWeights();
    };
}

#endif
