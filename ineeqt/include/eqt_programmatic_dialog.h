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
* This header defines the \ref EQt::ProgrammaticDialog class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_DIALOG_H
#define EQT_PROGRAMMATIC_DIALOG_H

#include <QString>
#include <QMap>
#include <QDialog>

#include "eqt_common.h"
#include "eqt_programmatic_application.h"
#include "eqt_programmatic_dialog_proxy.h"
#include "eqt_builder.h"
#include "eqt_programmatic_window.h"

class QWidget;
class QCloseEvent;
class QResizeEvent;
class QLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QFormLayout;

class DockWidget;

namespace EQt {
    /**
     * Class that extends QDialog to allow simplified, programmatic creation of dialogs.
     */
    class EQT_PUBLIC_API ProgrammaticDialog:public QDialog,
                                            public ProgrammaticWindow<
                                                ProgrammaticDialogProxy,
                                                ProgrammaticDialog
                                            > {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] dialogName  The name to use to reference this dialog during construction by the builder.
             *
             * \param[in] parent      The parent object for this window.
             *
             * \param[in] windowFlags Flags used to modify the behavior of this dialog.
             */
            explicit ProgrammaticDialog(
                const QString&  dialogName,
                QWidget*        parent = Q_NULLPTR,
                Qt::WindowFlags windowFlags = Qt::WindowFlags()
            );

            ~ProgrammaticDialog() override;

            /**
             * Determines the name assigned to this dialog during construction.
             *
             * \return Returns the dialog name provided to the builder object.
             */
            inline QString dialogName() const {
                return currentDialogName;
            }

            /**
             * Method that can be called to populate the dialog's widgets.  The default implementation calls each
             * proxy's \ref ProgrammaticDialogProxy::populate method.
             */
            virtual void populate();

            /**
             * Method that can be called to update the state of the OK button.  This method is intended to be called by
             * proxy classes and by members of this class to allow both proxies and independent tabs in this class to
             * update the state of the OK button in a consistent manner.
             *
             * Note that this method will only operate if a push button called "ok_push_button" is registered as a
             * widget within this class.
             *
             * \param[in] tabOrProxyName A free-form name used to update the status of the tab.  This name can be any
             *                           unique identifier.
             *
             * \param[in] okToClick      If true, this proxy or functions managing widgets on a tab have valid user
             *                           input.
             */
            void conditionallyEnableOkButton(const QString& tabOrProxyName, bool okToClick = true);

            /**
             * Method that can be called to update the state of the OK button.  This method is intended to be called by
             * proxy classes and by members of this class to allow both proxies and independent tabs in this class to
             * update the state of the OK button in a consistent manner.
             *
             * Note that this method will only operate if a push button called "ok_push_button" is registered as a
             * widget within this class.
             *
             * \param[in] tabOrProxyName A free-form name used to update the status of the tab.  This name can be any
             *                           unique identifier.
             *
             * \param[in] notOkToClick   If true, this proxy or functions managing widgets on a tab have invalid user
             *                           input.
             */
            void conditionallyDisableOkButton(const QString& tabOrProxyName, bool notOkToClick = true);

        public slots:
            /**
             * Slot that displays the dialog as a modal dialog.  The default implementation calls the \ref populate
             * method and then runs the dialog's event loop.
             *
             * \return Returns the dialog exit code.
             */
            int exec() override;

            /**
             * Slot that can be used to indicate that the user accepted the dialog contents.  The default
             * implementation calls the \ref ProgrammaticDialogProxy::accepted method for each proxy then calls the
             * base class accept method.
             */
            void accept() override;

            /**
             * Slot that can be used to indicate that the user rejected the dialog contents.  The default
             * implementation calls the \ref ProgrammaticDialogProxy::rejected method for each proxy then calls the
             * base class reject method.
             */
            void reject() override;

        protected:
            /**
             * Method that can be called from derived class constructors to instruct the builder objects to configure
             * this dialog.
             */
            void runBuilders();

            /**
             * Event handler that is triggered when the user attempts to close the window.
             *
             * The event handler will call the \ref ProgrammaticMainWindowProxy::okToClose method on each proxy for
             * this window.  The window will be closed only if every proxy returns a value of true.
             *
             * Prior to closing the window, this method will also call the \ref ProgrammaticMainWindowProxy::closing
             * method in each proxy class.
             *
             * \param[in] event The event that triggered the window to close.
             */
            void closeEvent(QCloseEvent* event) override;

            /**
             * Event handler that is triggered when the user resizes the window.
             *
             * The event handler will call the \ref ProgrammaticMainWindowProxy::resized method on each proxy for this
             * window.
             *
             * \param[in] event The event that triggered the window to close.
             */
            void resizeEvent(QResizeEvent* event) override;

        private:
            /**
             * The name to use when building this dialog.
             */
            QString currentDialogName;

            /**
             * Map used to conditionally enable or disable the OK button.
             */
            QMap<QString, bool> okStatus;
    };

    #if (defined(Q_OS_WIN))

        /**
         * Explicit template specializations needed for Windows.
         */
        template class EQT_PUBLIC_API ProgrammaticWindow<ProgrammaticDialogProxy, ProgrammaticDialog>;

    #endif
}

#endif
