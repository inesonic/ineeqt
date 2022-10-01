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
* This header defines the \ref EQt::FileDialog class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_FILE_DIALOG_H
#define EQT_FILE_DIALOG_H

#include <QFileDialog>
#include <QString>
#include <QStringList>

class QWidget;

#include "eqt_common.h"

namespace EQt {
    /**
     * This pure virtual base class represents a thin wrapper on the QFileDialog class allowing for easy customization
     * of several FileDialog features from derived classes.  The class also provides support for loading and saving the
     * dialog state to the application QSettings instance.
     *
     * Derived classes will need to configure:
     *     * The dialog labels (title and caption)
     *     * The dialog accept mode.
     *     * The dialog file mode.
     */
    class EQT_PUBLIC_API FileDialog:public QFileDialog {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             *
             * \param[in] flags     Qt windows flags.
             */
            FileDialog(QWidget* parent, Qt::WindowFlags flags = Qt::Dialog);

            /**
             * Constructor
             *
             * \param[in] caption The caption to show in the dialog.
             *
             * \param[in] parent  The pointer to the parent object.
             */
            FileDialog(const QString& caption, QWidget* parent = Q_NULLPTR);

            ~FileDialog() override;

            /**
             * Loads settings to be used by the file dialog.  If no settings exist, the method will set the current
             * directory to the user's home directory.
             */
            void loadSettings();

            /**
             * Saves the file dialog settings.
             */
            void saveSettings();

            /**
             * This wrapper on the underlying QFileDialog::exec method that performs several configuration steps before
             * running the dialog's event loop.
             *
             * \return Returns the dialog exit code.
             */
            int exec() override;

        protected:
            /**
             * You should overload this method in a derived class to return the group name to use when loading and
             * saving dialog state information.
             *
             * \return Returns the dialog group name.
             */
            virtual QString settingsGroupName() const = 0;

            /**
             * You should overload this method in a derived class to return a QStringList of filters that the user can
             * apply to the dialog.
             *
             * \return Returns a QStringList holding the supported filters.
             */
            virtual QStringList filterList() const = 0;
    };
}

#endif
