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
* This header defines the \ref EQt::MessageDialog class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_MESSAGE_DIALOG_H
#define EQT_MESSAGE_DIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>

class QWidget;
class QLabel;
class QCheckBox;

#include "eqt_common.h"

namespace EQt {
    /**
     * You can use this dialog to display short messages wil an OK button and checkbox indicating if the dialog should
     * be displayed in future.
     */
    class EQT_PUBLIC_API MessageDialog:public QDialog {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            MessageDialog(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] title   The dialog message title text.
             *
             * \param[in] caption The caption to show in the dialog.
             *
             * \param[in] parent  The pointer to the parent object.
             */
            MessageDialog(const QString& title, const QString& caption, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] title       The dialog message title text.
             *
             * \param[in] caption     The caption to show in the dialog.
             *
             * \param[in] showChecked If true, the checkbox will be checked.  If false, the checkbox will be unchecked.
             *
             * \param[in] parent      The pointer to the parent object.
             */
            MessageDialog(const QString& title, const QString& caption, bool showChecked, QWidget* parent = Q_NULLPTR);

            ~MessageDialog() override;

            /**
             * Method you can use to obtain the current message caption.
             *
             * \return Returns the current message caption.
             */
            QString caption() const;

            /**
             * Method you can use to determine if the checkbox is checked or unchecked.
             *
             * \return Returns true if the checkbox is checked.  Returns false if the checkbox is unchecked.
             */
            bool doNoShowAgain() const;

        public slots:
            /**
             * Slot you can use to set the dialog caption.
             *
             * \param[in] newCaption The new dialog caption.
             */
            void setCaption(const QString& newCaption);

            /**
             * Slot you can use to set the checkbox to checked or unchecked.
             *
             * \param[in] nowChecked If true, the checkbox will be checked.  If false, the checkbox will be unchecked.
             */
            void setDoNotShowAgain(bool nowChecked = true);

        private:
            /**
             * Method used by the constructors to configure this dialog.
             */
            void configure();

            /**
             * The caption label.
             */
            QLabel* captionLabel;

            /**
             * The checkbox.
             */
            QCheckBox* checkbox;
    };
}

#endif
