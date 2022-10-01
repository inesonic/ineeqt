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
* This header defines the \ref EQt::ProgrammaticDialogProxy class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_DIALOG_PROXY_H
#define EQT_PROGRAMMATIC_DIALOG_PROXY_H

#include "eqt_common.h"
#include "eqt_programmatic_window_proxy.h"

class QCloseEvent;
class QResizeEvent;

namespace EQt {
    class ProgrammaticDialog;

    /**
     * Base class for proxies for dialogs.  You can use this class to extend the functionality of a dialog.
     */
    class ProgrammaticDialogProxy:public EQt::ProgrammaticWindowProxy {
        Q_OBJECT

        public:
            /**
             * Constuctor
             *
             * \param[in] dialog The dialog this class is proxying for.
             */
            explicit ProgrammaticDialogProxy(ProgrammaticDialog* dialog);

            ~ProgrammaticDialogProxy() override;

            /**
             * Method that returns the window class instance.
             *
             * \returns the main window instance associated with this proxy.
             */
            ProgrammaticDialog* dialog() const;

            /**
             * Method that can be used to populate the dialog's values prior to displaying the dialog.  The default
             * implementation performs no action.
             */
            virtual void populate();

            /**
             * Method that is called when the user clicks the cancel button.  The default implementation performs no
             * action. Note that this method will not be called when the user closes the dialog via the close button in
             * the title bar.  You can detect that event via the \ref ProgrammaticWindowProxy::closing method.
             */
            virtual void rejected();

            /**
             * Method that is called when the user clicks the accept button.  The default implementation performs no
             * action.
             */
            virtual void accepted();
    };
}

#endif
