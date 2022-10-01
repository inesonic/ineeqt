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
* This file implements the \ref EQt::CustomPaperSizeDialog class.
***********************************************************************************************************************/

#include <QWidget>
#include <QDialog>
#include <QLocale>
#include <QString>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>

#include "eqt_dimension_line_edit.h"
#include "eqt_custom_paper_size_dialog.h"

namespace EQt {
    CustomPaperSizeDialog::CustomPaperSizeDialog(QWidget* parent):QDialog(parent) {
        setWindowTitle(tr("Manage Paper Sizes"));
        setSizeGripEnabled(true);

        configureDialog();
        bind();
    }


    CustomPaperSizeDialog::~CustomPaperSizeDialog() {}


    double CustomPaperSizeDialog::width() const {
        return widthLineEdit->value();
    }


    void CustomPaperSizeDialog::setWidth(double newPageWidth) {
        widthLineEdit->setValue(newPageWidth);
    }


    double CustomPaperSizeDialog::height() const {
        return heightLineEdit->value();
    }

    void CustomPaperSizeDialog::setHeight(double newPageHeight) {
        heightLineEdit->setValue(newPageHeight);
    }


    void CustomPaperSizeDialog::fieldChanged(const QString&) {
        bool valuesOk = widthLineEdit->valueOk() && heightLineEdit->valueOk();

        if (valuesOk) {
            double w = widthLineEdit->value();
            double h = heightLineEdit->value();

            if (w <= 0 || h <= 0 || w > h) {
                valuesOk = false;
            }
        }

        okPushButton->setEnabled(valuesOk);
    }


    void CustomPaperSizeDialog::configureDialog() {
        QVBoxLayout* verticalLayout = new QVBoxLayout;
        setLayout(verticalLayout);

        QFormLayout* mainLayout = new QFormLayout;
        verticalLayout->addLayout(mainLayout);

        widthLineEdit = new EQt::DimensionLineEdit;
        mainLayout->addRow(tr("Width"), widthLineEdit);

        heightLineEdit = new EQt::DimensionLineEdit;
        mainLayout->addRow(tr("Height"), heightLineEdit);

        QDialogButtonBox* dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        okPushButton = dialogButtonBox->button(QDialogButtonBox::Ok);
        cancelPushButton = dialogButtonBox->button(QDialogButtonBox::Cancel);

        okPushButton->setEnabled(false);

        verticalLayout->addWidget(dialogButtonBox);
    }


    void CustomPaperSizeDialog::bind() {
        connect(widthLineEdit, SIGNAL(textChanged(QString)), this, SLOT(fieldChanged(QString)));
        connect(heightLineEdit, SIGNAL(textChanged(QString)), this, SLOT(fieldChanged(QString)));

        connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
    }
}
