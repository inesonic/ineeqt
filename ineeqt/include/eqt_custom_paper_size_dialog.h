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
* This header defines the \ref EQt::CustomPaperSizeDialog class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_CUSTOM_PAPER_SIZE_DIALOG_H
#define EQT_CUSTOM_PAPER_SIZE_DIALOG_H

#include <QDialog>
#include <QString>

class QWidget;
class QLineEdit;
class QPushButton;

#include "eqt_common.h"

namespace EQt {
    class DimensionLineEdit;

    /**
     * Dialog used to create custom paper sizes.
     */
    class EQT_PUBLIC_API CustomPaperSizeDialog:public QDialog {
        Q_OBJECT

        public:
            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent object.
             */
            CustomPaperSizeDialog(QWidget* parent = Q_NULLPTR);

            ~CustomPaperSizeDialog() override;

            /**
             * Holds the page width, in points.
             *
             * \returns the page width, in points.
             */
            double width() const;

            /**
             * You can use this method to change the paper width.
             *
             * \param[in] newPageWidth The new page width to use.  The value should be in points.
             */
            void setWidth(double newPageWidth);

            /**
             * Holds the page height, in points.
             *
             * \return Returns the page height, in points.
             */
            double height() const;

            /**
             * You can use this method to change the paper height.
             *
             * \param[in] newPageHeight The page height to use.  The value should be in points.
             */
            void setHeight(double newPageHeight);

        private slots:
            /**
             * Slot that is triggered when either the width or the height line edits are updated.
             *
             * \param[in] newText The text from the line editor.  The value is ignored.
             */
            void fieldChanged(const QString& newText);

        private:
            /**
             * Method that builds the default dialog.
             */
            void configureDialog();

            /**
             * Method that binds various signals to their appropriate slots.
             */
            void bind();

            /**
             * Field used to enter the width of the paper.
             */
            DimensionLineEdit* widthLineEdit;

            /**
             * Field used to enter the height of the paper.
             */
            DimensionLineEdit* heightLineEdit;

            /**
             * The OK push button.  We disable the button if the user's input is invalid.
             */
            QPushButton* okPushButton;

            /**
             * The Cancel push button.
             */
            QPushButton* cancelPushButton;
    };
}

#endif
