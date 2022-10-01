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
* This header defines the \ref EQt::ProgrammaticWindowBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_WINDOW_BASE_H
#define EQT_PROGRAMMATIC_WINDOW_BASE_H

#include <QString>

#include "eqt_common.h"
#include "eqt_programmatic_widget.h"

namespace EQt {
    /**
     * Pure virtual base class based on \ref EQt::ProgrammaticWidget that provides virtual methods needed for windows.
     */
    class EQT_PUBLIC_API ProgrammaticWindowBase:public ProgrammaticWidget {
        public:
            ProgrammaticWindowBase();

            ~ProgrammaticWindowBase() override;

            /**
             * Binds all the proxies to this class.
             */
            virtual void bind() = 0;

        protected:
            /**
             * Loads settings to be used by the window.
             *
             * \param[in] groupName The name of the settings group to use for the window.
             */
            virtual void loadSettings(const QString& groupName) = 0;

            /**
             * Saves settings used by the window.
             *
             * \param[in] groupName The name of the settings group to use for the window.
             */
            virtual void saveSettings(const QString& groupName) = 0;

    };
}

#endif
