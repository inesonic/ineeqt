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
* This header defines the \ref EQt::CommandLineEdit class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_COMMAND_LINE_EDIT_H
#define EQT_COMMAND_LINE_EDIT_H

#include <QString>
#include <QSet>
#include <QLineEdit>

class QWidget;
class QKeyEvent;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that extends the QLineEdit class to include additional capabilities:
     *
     * - Class can optionally treat spaces the same a return.
     * - Class can use the tab key for command completion.  This feature is only supported if a validator with a fixup
     *   method is registered with the line editor.
     */
    class EQT_PUBLIC_API CommandLineEdit:public QLineEdit {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this menu.
             */
            CommandLineEdit(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] contents The contents to pre-load into the line editor.
             *
             * \param[in] parent   The parent object for this window.
             */
            CommandLineEdit(const QString& contents, QWidget* parent = Q_NULLPTR);

            ~CommandLineEdit() override;

            /**
             * Method you can use to indicate if the tab key should support command completion.
             *
             * \param[in] nowEnableCommandCompletion If true, the tab key will support command completion.  If false,
             *                                       command completion will *not* be supported.  Command completion
             *                                       is disabled by default.
             */
            void setCommandCompletionEnabled(bool nowEnableCommandCompletion = true);

            /**
             * Method you can use to indicate if the tab key should support command completion.
             *
             * \param[in] nowDisableCommandCompletion If true, the tab key will *not* support command completion.  If
             *                                        false, command completion will be supported.  Command completion
             *                                         is disabled by default
             */
            void setCommandCompletionDisabled(bool nowDisableCommandCompletion = true);

            /**
             * Method you can use to determine if command completion is enabled or disabled.
             *
             * \return Returns true if command completion is enabled.  Returns false if command completion is disabled.
             */
            bool commandCompletionEnabled() const;

            /**
             * Method you can use to determine if command completion is disabled or enabled.
             *
             * \return Returns true if command completion is disabled.  Returns false if command completion is enabled.
             */
            bool commandCompletionDisabled() const;

            /**
             * Method you can use to set the key to intercept for command completion.
             *
             * \param[in] key The key to use for command completion.  This value should be one of the values defined in
             *                the Qt::Key enumeration.  By default, the tab key is used.
             */
            void setCommandCompletionKey(int key);

            /**
             * Method you can use to determine the key to use for command completion.
             *
             * \return Returns the key used for tab completion.  The value will be one of the values defined in the
             *         enumeration Qt::Key.
             */
            int commandCompletionKey() const;

            /**
             * Method you can use to define additional special keys to terminate user entry.
             *
             * \param[in] keyList A set of values from the enumeration Qt::Key indicating keys that require special
             *                    handling.
             */
            void setSpecialKeys(const QSet<int>& keyList);

            /**
             * Method you can use to obtain a list of all the keys that perform special handling.
             *
             * \return turns a set of keys the will invoke special handling.
             */
            QSet<int> specialKeys() const;

        signals:
            /**
             * Signal that is emitted when the user presses a special key.
             *
             * \param[in] key The key that triggered this signal.
             */
            void specialKeyPressed(int key);

        protected:
            /**
             * Method you can overload to intercept special key handling.  The default implementation emits the signal
             * \ref EQt::CommandLineEdit::specialKeyPressed.
             *
             * \param[in] key The key that triggered the call to this method.
             *
             * \return Returns true if the key should be processed normally.  Returns false if this key stroke should
             *         be ignored by the line editor.  The default implementation returns false.
             */
            virtual bool processSpecialKey(int key);

            /**
             * Method that handles the key press events.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void keyPressEvent(QKeyEvent* event) override;

        private:
            /**
             * Method that performs configuration common to all constructors.
             */
            void configure();

            /**
             * Holds true if command completion is enabled.
             */
            bool currentCommandCompletionEnabled;

            /**
             * The command completion key.
             */
            int currentCommandCompletionKey;

            /**
             * List of keys that trigger special handling.
             */
            QSet<int> currentSpecialKeys;
    };
}

#endif
