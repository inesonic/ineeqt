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
* This header defines the \ref EQt::ColorDialog class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_COLOR_DIALOG_H
#define EQT_COLOR_DIALOG_H

#include <QColorDialog>
#include <QColor>

class QWidget;

#include "eqt_common.h"

namespace EQt {
    /**
     * This class represents a thin wrapper on the QColorDialog class.  The class provides support for loading and
     * saving the application global dialog state to the application QSettings instance.
     */
    class EQT_PUBLIC_API ColorDialog:public QColorDialog {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            ColorDialog(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] initial The initially selected color.
             *
             * \param[in] parent  The pointer to the parent object.
             */
            ColorDialog(const QColor& initial, QWidget* parent = Q_NULLPTR);

            ~ColorDialog() override;

            /**
             * Loads settings to be used globally by the color dialogs.
             */
            static void loadSettings();

            /**
             * Saves the file dialog settings.
             */
            static void saveSettings();
    };
}

#endif
