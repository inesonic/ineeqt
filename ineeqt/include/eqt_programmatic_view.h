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
* This header defines the \ref EQt::ProgrammaticView class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_VIEW_H
#define EQT_PROGRAMMATIC_VIEW_H

#include <QMap>
#include <QString>
#include <QWidget>

#include "eqt_common.h"
#include "eqt_programmatic_view_proxy.h"
#include "eqt_programmatic_window.h"

namespace EQt {
    class ProgrammaticViewProxy;
    class ProgrammaticMainWindow;

    /**
     * Class that manages an \ref EQt::ProgrammaticView widget.
     *
     * The class is setup to support a programmatic model/view paradigm where a \ref EQt::ProgrammaticView manages the
     * view to an underlying model.
     */
    class EQT_PUBLIC_API ProgrammaticView:public QWidget,
                                          public ProgrammaticWindow<ProgrammaticViewProxy, ProgrammaticView> {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] window The main window associated with this view.
             *
             * \param[in] parent The parent object for this window.
             */
            ProgrammaticView(ProgrammaticMainWindow* window, QWidget* parent = Q_NULLPTR);

            ~ProgrammaticView() override;

            /**
             * Determines the window that this view is working under.  Note that the window may not be the direct
             * parent of this view.
             *
             * \return Returns a pointer to a ProgrammaticMainWindow instance.
             */
            inline ProgrammaticMainWindow* window() const {
                return currentWindow;
            }

        public slots:
            /**
             * Method that performs dynamic binding of slots to this specific view.
             */
            virtual void bindDynamic();

            /**
             * Method that performs dynamic unbinding of slots from this specific view.
             */
            virtual void unbindDynamic();

        protected:
            /**
             * Method that can be called from derived class constructors to run any registered builders.
             */
            void runBuilders();

        private:
            /**
             * The window this view is working under.
             */
            ProgrammaticMainWindow* currentWindow;
    };

    #if (defined(Q_OS_WIN))

        /**
         * Explicit template specializations needed for Windows.
         */
        template class EQT_PUBLIC_API ProgrammaticWindow<ProgrammaticViewProxy, ProgrammaticView>;

    #endif
}

#endif
