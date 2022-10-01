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
* This header defines the \ref EQt::BuilderBase class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_BUILDER_BASE_H
#define EQT_BUILDER_BASE_H

#include "eqt_common.h"

namespace EQt {
    /**
     * Pure virtual base class that provides support for dynamic plug-ins to the application.
     */
    class EQT_PUBLIC_API BuilderBase {
        public:
            BuilderBase();

            virtual ~BuilderBase();

            /**
             * Method that returns a priority number used to order this builder with respect to other builders.
             * Builders with lower numbers are executed first.
             *
             * \return The builder priority number.
             */
            virtual unsigned priority() const = 0;

            /**
             * Method that returns a short identifier for builder.  This function exists mostly for debugging
             * purposes.  By convention, the name should be the name of the builder class.
             *
             *     <PlugInName>::<Builder Class Name>
             *
             * \return Returns a string holding the builder name.
             */
            virtual const char* builderIdentifier() const = 0;

            /**
             * Method that returns the name of a plug-in associated with this builder.  An empty string should be used
             * for builders that have not been defined by a plug-in.
             *
             * \return Returns a string holding the plug-in name.
             */
            virtual const char* plugInName() const = 0;
    };
}

#endif
