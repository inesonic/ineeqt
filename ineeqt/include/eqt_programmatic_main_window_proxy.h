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
* This header defines the \ref EQt::ProgrammaticMainWindowProxy class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_MAIN_WINDOW_PROXY_H
#define EQT_PROGRAMMATIC_MAIN_WINDOW_PROXY_H

#include <util_bit_set.h>

#include "eqt_common.h"
#include "eqt_programmatic_main_window.h"
#include "eqt_programmatic_window_proxy.h"

class QCloseEvent;
class QResizeEvent;

namespace EQt {
    class ProgrammaticMainWindow;

    /**
     * Base class for proxies for main window actions.  You can use this class to extend the main window's
     * functionality.
     */
    class EQT_PUBLIC_API ProgrammaticMainWindowProxy:public ProgrammaticWindowProxy {
        Q_OBJECT

        public:
            /**
             * Constuctor
             *
             * \param[in] window The main window this class is proxying for.
             */
            explicit ProgrammaticMainWindowProxy(ProgrammaticMainWindow* window);

            ~ProgrammaticMainWindowProxy() override;

            /**
             * Method that returns the window class instance.
             *
             * \returns the main window instance associated with this proxy.
             */
            ProgrammaticMainWindow* window() const;

            /**
             * Method that is called when the action state is changed.  You can use this method in derived classes to
             * update the application state.
             *
             * The default implementation does nothing.
             *
             * \param[in] oldActionState The old action state.
             *
             * \param[in] newActionState The new action state.
             */
            virtual void actionStateChanged(const Util::BitSet& oldActionState, const Util::BitSet& newActionState);

            /**
             * Method that is called after the window is fully configured and before the window first becomes visible.
             * The default implementation simply returns.
             */
            virtual void windowConfigured();

            /**
             * Method that is called after the window is made visible.  The default implementation simply returns.
             */
            virtual void windowNowVisible();
    };
}

#endif
