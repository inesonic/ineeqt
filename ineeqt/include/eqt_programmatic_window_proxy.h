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
* This header defines the \ref EQt::ProgrammaticWindowProxy class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_WINDOW_PROXY_H
#define EQT_PROGRAMMATIC_WINDOW_PROXY_H

#include "eqt_common.h"
#include "eqt_programmatic_widget_proxy.h"

class QObject;
class QResizeEvent;

namespace EQt {
    /**
     * Base class for proxies for a window such as a QDialog or a QMainWindow.
     */
    class EQT_PUBLIC_API ProgrammaticWindowProxy:public ProgrammaticWidgetProxy {
        Q_OBJECT

        public:
            /**
             * Constuctor
             *
             * \param[in] window The window instance this class is proxying for.
             */
            explicit ProgrammaticWindowProxy(QObject* window);

            ~ProgrammaticWindowProxy() override;

            /**
             * Method that is called to request permission to close the window.
             *
             * \return Returns true if the window can be closed.  Returns false if the window should not be closed.
             *         The default implementation returns true.
             */
            virtual bool okToClose();

            /**
             * Method that is called to inform this proxy that the window is about to close.
             *
             * The default implementation simply returns.
             */
            virtual void closing();

            /**
             * Method that is called after a window is resized to perform any updates to the GUI.  Note that no
             * repainting of the GUI should be performed within this method.
             *
             * The default implementation simply returns.
             *
             * \param[in] event The resize event that triggered a call to this method.
             */
            virtual void resized(QResizeEvent* event);
    };
}

#endif
