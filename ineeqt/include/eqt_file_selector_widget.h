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
* This header defines the \ref EQt::FileSelectorWidget class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_FILE_SELECTOR_WIDGET_H
#define EQT_FILE_SELECTOR_WIDGET_H

#include <QFileDialog>
#include <QString>
#include <QUrl>

class QWidget;
class QLineEdit;

#include "eqt_common.h"

class QSettings;

namespace EQt {
    /**
     * This class munges the QFileDialog class to turn it into a widget that can be embedded into another dialog or
     * control.
     */
    class EQT_PUBLIC_API FileSelectorWidget:public QFileDialog {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            FileSelectorWidget(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] acceptMode The file selector's file accept mode.
             *
             * \param[in] parent     Pointer to the parent object.
             */
            FileSelectorWidget(AcceptMode acceptMode, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] fileMode   The file selector's file mode.
             *
             * \param[in] acceptMode The file selector's file accept mode.
             *
             * \param[in] parent     Pointer to the parent object.
             */
            FileSelectorWidget(FileMode fileMode, AcceptMode acceptMode, QWidget* parent = Q_NULLPTR);

            ~FileSelectorWidget() override;

            /**
             * Method you can call to load settings for this widget.  This method will *not* set a group for the
             * settings.
             *
             * \param[in] settings         The settings class to populate.
             *
             * \param[in] defaultDirectory The default directory.  If this string is empty or if the default directory
             *                             does not exist, this method will set the default directory to the user's
             *                             home directory.
             */
            void loadSettings(const QSettings* settings, const QString& defaultDirectory = QString());

            /**
             * Method you can call to save settings for this widget.  This method will *not* set a group for the
             * settings.
             *
             * \param[in,out] settings The settings class to populate.
             */
            void saveSettings(QSettings* settings);

            /**
             * Method you can use to set the selector widget's options.
             *
             * \param[in] option The option to be set or cleared.
             *
             * \param[in] on     If true, the option will be enabled.  If false, the option will be disabled.
             */
            void setOption(Option option, bool on = true);

            /**
             * Method you can use to set the widget's options.
             *
             * \param[in] options The options to be set.
             */
            void setOptions(Options options);

            /**
             * Method you can use to determine if the line editor will auto-update.
             *
             * \return Returns true if the line editor will auto-update with user activity.  Returns false if the line
             *         editor will not auto-update with user activity.
             */
            bool lineEditorAutoUpdateEnabled() const;

            /**
             * Method you can use to determine if the line editor will auto-update.
             *
             * \return Returns true if the line editor will auto-update with user activity.  Returns false if the line
             *         editor will not auto-update with user activity.
             */
            bool lineEditorAutoUpdateDisabled() const;

            /**
             * Method you can call to indicate if the line editor should update and clear when the current selection
             * changes.
             *
             * \param[in] nowAutoUpdate If true, the line editor will update with actions in the file window.  If
             *                          false, the line editor will no tbe auto-updated.
             */
            void setLineEditorAutoUpdateEnabled(bool nowAutoUpdate = true);

            /**
             * Method you can call to indicate if the line editor should not update and clear when the current
             * selection changes.
             *
             * \param[in] nowNoAutoUpdate If true, the line editor will not auto-update.  If false, the line editor
             *                            will update with user activity in the selection window.
             */
            void setLineEditorAutoUpdateDisabled(bool nowNoAutoUpdate = true);

        signals:
            /**
             * Signal that is emitted whenver the file name line editor value is changed by the user.
             *
             * \param[out] newText The new line editor text value.
             */
            void fileNameTextEdited(const QString& newText);

            /**
             * Signal that is emitted whenver the file name line editor value is changed by the user or system.
             *
             * \param[out] newText The new line editor text value.
             */
            void fileNameTextChanged(const QString& newText);

        protected:
            /**
             * Method that is called when the dialog that defines this widget attempts to close.  We use this to
             * prevent the dialog from disappearing when the return key is pressed.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void closeEvent(QCloseEvent* event) override;

        private slots:
            /**
             * Slot that is triggered when the current selection changes.
             *
             * \param[in] newPath The newly selected path.
             */
            void processCurrentChanged(const QString& newPath);

            /**
             * Slot that is triggered when the current selected URL changes.
             *
             * \param[in] newUrl The newly selected URL.
             */
            void processCurrentUrlChanged(const QUrl& newUrl);

            /**
             * Slot that is triggered when the current directory changes.
             *
             * \param[in] newDirectory The newly selected directory.
             */
            void processDirectoryEntered(const QString& newDirectory);

            /**
             * Slot that is triggered when the directory URL changes.
             *
             * \param[in] directory The newly selected directory URL.
             */
            void ProcessDirectoryUrlEntered(const QUrl& directory);

        private:
            /**
             * Method that performs configuration that is common to all constructors.
             */
            void configureWidget();

            /**
             * Flag indicating if the line editor should auto-update.
             */
            bool currentLineEditorAutoUpdateEnabled;

            /**
             * The line editor where the user can enter new files/directories.
             */
            QLineEdit* currentLineEditor;
    };
}

#endif
