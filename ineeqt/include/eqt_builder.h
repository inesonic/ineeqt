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
* This header defines the \ref EQt::Builder class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_BUILDER_H
#define EQT_BUILDER_H

#include <QObject>

#include "eqt_common.h"
#include "eqt_builder_base.h"

namespace EQt {
    class ProgrammaticApplication;
    class ProgrammaticMainWindow;
    class ProgrammaticMainWindowProxy;
    class ProgrammaticDialog;
    class ProgrammaticDialogProxy;
    class ProgrammaticView;
    class ProgrammaticViewProxy;

    /**
     * Pure virtual base class that provides support for dynamic creation of GUIs from a plug-in.
     *
     * A plug-in, when loaded, will be provided the opportunity to provide an instance of this class when the
     * application then takes ownership.  The application can use this instance to include additional functions into
     * the GUI.
     *
     * The user interface makes heavy use of actions.  Actions are bound and unbound at different times during
     * application execution:
     *
     *     * Static actions are bound once when a main window instance is loaded.
     *     * Dynamic actions within a main window are bound dynamically whenever the user selects a new tab or a new
     *       tab in the window becomes the current tab.
     *     * Dynamic actions within a main window are unbound dynamically whenever a tab is deselected.
     *
     * For window creation, methods are called in the following order:
     *     * buildMainWindowGui
     *     * buildAdditionalMainWindowActions
     *     * buildMainWindowDocks
     *     * createMainWindowProxy
     *     * The bind method for the window.  This method should call the bind method on each proxy.
     *     * buildMainWindowFinal
     *
     * For view creation, method are called in the following order:
     *     * buildViewGui
     *     * createViewProxy
     *     * buildViewFinal
     *
     * For dialog creation, methods are called in the following order:
     *     * buildDialogGui
     *     * createDialogProxy
     *     * The bind method for the window.  This method should call the bind method on each proxy.
     *     * buildDialogFinal
     */
    class EQT_PUBLIC_API Builder:public QObject, public BuilderBase {
        Q_OBJECT

        public:
            /**
             * Constuctor
             *
             * \param[in] parent Pointer to the parent object.
             */
            explicit Builder(QObject* parent = Q_NULLPTR);

            ~Builder() override;

            /**
             * Method that is called once when the plug-in is loaded to register any resources and to create any global
             * settings used by the application.  The default implementation simply returns, performing no action.
             *
             * \param[in] application The application resource to receive any registration data.
             */
            virtual void registerResources(ProgrammaticApplication* application);

            /**
             * Method that is called to build a main window's GUI.  Note that this method will be called each time a
             * new main window needs to be created.
             *
             * This method should build the menus and toolbars, including support for custom tool-tips, icons, etc.
             *
             * The default implementation simply returns, performing no action.
             *
             * \param[in] window The main window to build the GUI on.
             */
            virtual void buildMainWindowGui(ProgrammaticMainWindow* window);

            /**
             * Method that is called to build additional actions used by a main window's GUI but not tied to a specific
             * menu action.
             *
             * The default implementation simply returns, performing no action.
             *
             * \param[in] window The main window to build the GUI on.
             */
            virtual void buildAdditionalMainWindowActions(ProgrammaticMainWindow* window);

            /**
             * Method that is called to create docks used by a main window.  Note that this method is called once each
             * time a new main window needs to be created.
             *
             * This method should create actions used by the docks and then create the docking windows.  The docks
             * should be added to a main window.
             *
             * The default implementation simply returns.
             *
             * \param[in] window The main window to add the docks to.
             */
            virtual void buildMainWindowDocks(ProgrammaticMainWindow* window);

            /**
             * Method that can be used to build proxy objects for a main window.
             *
             * \param[in] window The window to associate with the proxy.
             *
             * \return Returns a pointer to the proxy object to be used for the main window.  A null pointer can be
             *         returned if no proxy object is needed.
             */
            virtual ProgrammaticMainWindowProxy* createMainWindowProxy(ProgrammaticMainWindow* window);

            /**
             * Method this is called to perform any final configuration for the window.  This method is called once
             * each time a window is created, after all other build* and create* methods are called.
             *
             * The method exists primarily to implement or adjust the application's central widget.
             *
             * The default implementation simply returns.
             *
             * \param[in] window The main window to perform any final configuration on.
             */
            virtual void buildMainWindowFinal(ProgrammaticMainWindow* window);

            /**
             * Method that is called to build a view's GUI.  Note that this method will be called each time a new
             * view into a main window needs to be created.
             *
             * This method should build the menus and toolbars, including support for custom tool-tips, icons, etc.
             *
             * The default implementation simply returns, performing no action.
             *
             * \param[in] view   The view to be built.
             *
             * \param[in] window The window that contains the view.
             */
            virtual void buildViewGui(ProgrammaticView* view, ProgrammaticMainWindow* window);

            /**
             * Method that can be used to build proxy objects for a view.
             *
             * \param[in] view   The view to be built.
             *
             * \param[in] window The window that contains the view.
             *
             * \return Returns a pointer to the proxy object to be used for the view.  A null pointer can be returned
             *         if no proxy object is needed.
             */
            virtual ProgrammaticViewProxy* createViewProxy(ProgrammaticView* view, ProgrammaticMainWindow* window);

            /**
             * Method this is called to perform any final configuration for the view.  This method is called once each
             * time a view is created, after all other build* and create* methods are called.
             *
             * The method exists primarily to implement or adjust the application's central widget.  This method may
             * also perform the task of adding the view to the window.
             *
             * The default implementation simply returns.
             *
             * \param[in] view   The view to be built.
             *
             * \param[in] window The window that contains the view.
             */
            virtual void buildViewFinal(ProgrammaticView* view, ProgrammaticMainWindow* window);

            /**
             * Method that is called to build a specific dialog.  This method will be called once each time a new
             * dialog object is constructed.  Depending on the dialog, this may be a one-time construction or it may be
             * a new construction each time the dialog is displayed.
             *
             * The default implementation simply returns.
             *
             * \param[in] dialogName The name of the dialog. You can use this to determine which dialog is being built.
             *
             * \param[in] dialog     The dialog object being built.
             */
            virtual void buildDialogGui(const QString& dialogName, ProgrammaticDialog* dialog);

            /**
             * Method that can be used to create proxy objects for a dialog.  This method is called once each time a
             * dialog is constructed.  Depending on the dialog, this may be a one-time construction or it may be a new
             * construction each time the dialog is displayed.  If you don't want a proxy object, you can returns a
             * null pointer.
             *
             * The default implementation returns a null pointer.
             *
             * \param[in] dialogName The name of the dialog. You can use this to determine which dialog is being built.
             *
             * \param[in] dialog     The dialog that requires the proxy.
             *
             * \return Returns a pointer to the new proxy object.
             */
            virtual ProgrammaticDialogProxy* createDialogProxy(const QString& dialogName, ProgrammaticDialog* dialog);

            /**
             * Method this is called to perform any final configuration for the dialog.  This method is called once
             * each time a view is created, after all other build* and create* methods are called.
             *
             * The default implementation simply returns.
             *
             * \param[in] dialogName The name of the dialog. You can use this to determine which dialog is being built.
             *
             * \param[in] dialog     The dialog object being built.
             */
            virtual void buildDialogFinal(const QString& dialogName, ProgrammaticDialog* dialog);
    };
}

#endif
