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
* This file implements the \ref EQt::MessageDialog class.
***********************************************************************************************************************/

#include <QWidget>
#include <QString>
#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include "eqt_common.h"
#include "eqt_message_dialog.h"

namespace EQt {
    MessageDialog::MessageDialog(QWidget* parent):QDialog(parent) {
        configure();
    }


    MessageDialog::MessageDialog(const QString& title, const QString& caption, QWidget* parent):QDialog(parent) {
        configure();
        setWindowTitle(title);
        setCaption(caption);
    }


    MessageDialog::MessageDialog(
            const QString& title,
            const QString& caption,
            bool           showChecked,
            QWidget*       parent
        ):QDialog(
            parent
        ) {
        configure();
        setWindowTitle(title);
        setCaption(caption);
        setDoNotShowAgain(showChecked);
    }


    MessageDialog::~MessageDialog() {}


    QString MessageDialog::caption() const {
        return captionLabel->text();
    }


    bool MessageDialog::doNoShowAgain() const {
        return checkbox->isChecked();
    }


    void MessageDialog::setCaption(const QString& newCaption) {
        captionLabel->setText(newCaption);
    }


    void MessageDialog::setDoNotShowAgain(bool nowChecked) {
        checkbox->setChecked(nowChecked);
    }


    void MessageDialog::configure() {
        QVBoxLayout* mainLayout = new QVBoxLayout;
        setLayout(mainLayout);

        captionLabel = new QLabel;
        captionLabel->setTextInteractionFlags(Qt::TextInteractionFlag::TextBrowserInteraction);
        captionLabel->setOpenExternalLinks(true);
        mainLayout->addWidget(captionLabel, 1);

        checkbox = new QCheckBox(tr("Do not show this message again."));
        mainLayout->addWidget(checkbox, 0, Qt::AlignmentFlag::AlignRight);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::StandardButton::Ok);
        mainLayout->addWidget(buttonBox, 0);

        QPushButton* okButton = buttonBox->button(QDialogButtonBox::StandardButton::Ok);
        connect(okButton, &QPushButton::clicked, this, &MessageDialog::accept);
    }
}
