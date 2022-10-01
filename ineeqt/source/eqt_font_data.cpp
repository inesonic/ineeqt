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
* This file implements the \ref EQt::FontData class.
***********************************************************************************************************************/

#include <QFont>
#include <QSet>
#include <QFontInfo>

#include "eqt_font_data.h"

namespace EQt {
    QSet<QFont::Weight> FontData::uniqueWeights(const QFont& font) {
        QSet<QFont::Weight> result;

        QSet<QFont::Weight> targetWeights = standardWeights();
        QFont               adjustedFont  = font;
        for (  QSet<QFont::Weight>::const_iterator it  = targetWeights.constBegin(),
                                                   end = targetWeights.constEnd()
             ; it != end
             ; ++it
            ) {
            QFont::Weight targetWeight = *it;
            adjustedFont.setWeight(targetWeight);
            QFontInfo fontInfo(adjustedFont);
            result << static_cast<QFont::Weight>(fontInfo.weight());
        }

        return result;
    }


    QSet<QFont::Weight> FontData::standardWeights() {
        QSet<QFont::Weight> result;
        result << QFont::Weight::Thin
               << QFont::Weight::ExtraLight
               << QFont::Weight::Light
               << QFont::Weight::Normal
               << QFont::Weight::Medium
               << QFont::Weight::DemiBold
               << QFont::Weight::Bold
               << QFont::Weight::ExtraBold
               << QFont::Weight::Black;

        return result;
    }

}
