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
* This file implements the \ref EQt::ProgrammaticDialog class.
***********************************************************************************************************************/

#include <QWidget>
#include <QString>
#include <QMap>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QPushButton>
#include <QDialog>

#include "eqt_common.h"
#include "eqt_programmatic_application.h"
#include "eqt_builder.h"
#include "eqt_programmatic_window.h"
#include "eqt_programmatic_dialog_proxy.h"
#include "eqt_programmatic_dialog.h"

namespace EQt {
    ProgrammaticDialog::ProgrammaticDialog(
            const QString&  dialogName,
            QWidget*        parent,
            Qt::WindowFlags windowFlags
        ):QDialog(
            parent,
            windowFlags
        ) {
        currentDialogName = dialogName;
    }


    ProgrammaticDialog::~ProgrammaticDialog() {}


    void ProgrammaticDialog::populate() {
        for (auto it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->populate();
        }
    }


    void ProgrammaticDialog::conditionallyEnableOkButton(const QString& tabOrProxyName, bool okToClick) {
        QPushButton* okPushButton    = widget<QPushButton>("ok_push_button");
        QPushButton* applyPushButton = widget<QPushButton>("apply_push_button");

        if (okPushButton != Q_NULLPTR || applyPushButton != Q_NULLPTR) {
            okStatus[tabOrProxyName] = okToClick;

            bool enableButton = true;
            for (auto it=okStatus.begin(),end=okStatus.end() ; it!=end && enableButton ; ++it) {
                enableButton = it.value();
            }

            if (okPushButton != Q_NULLPTR) {
                okPushButton->setEnabled(enableButton);
            }

            if (applyPushButton != Q_NULLPTR) {
                applyPushButton->setEnabled(enableButton);
            }
        }
    }


    void ProgrammaticDialog::conditionallyDisableOkButton(const QString& tabOrProxyName, bool notOkToClick) {
        conditionallyEnableOkButton(tabOrProxyName, !notOkToClick);
    }


    int ProgrammaticDialog::exec() {
        populate();
        return QDialog::exec();
    }


    void ProgrammaticDialog::accept() {
        for (auto it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->accepted();
        }

        QDialog::accept();
    }


    void ProgrammaticDialog::reject() {
        for (auto it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->rejected();
        }

        QDialog::reject();
    }


    void ProgrammaticDialog::runBuilders() {
        for (auto it=ProgrammaticApplication::begin(),end=ProgrammaticApplication::end() ; it!=end ; ++it) {
            Builder* builder = dynamic_cast<Builder*>(*it);
            if (builder != Q_NULLPTR) {
                builder->buildDialogGui(currentDialogName, this);
            }
        }

        for (auto it=ProgrammaticApplication::begin(),end=ProgrammaticApplication::end() ; it!=end ; ++it) {
            Builder* builder = dynamic_cast<Builder*>(*it);
            if (builder != Q_NULLPTR) {
                ProgrammaticDialogProxy* proxy = builder->createDialogProxy(currentDialogName, this);
                if (proxy != Q_NULLPTR) {
                    addProxy(builder->builderIdentifier(), proxy);
                }
            }
        }

        bind();

        for (auto it=ProgrammaticApplication::begin(),end=ProgrammaticApplication::end() ; it!=end ; ++it) {
            Builder* builder = dynamic_cast<Builder*>(*it);
            if (builder != Q_NULLPTR) {
                builder->buildDialogFinal(currentDialogName, this);
            }
        }
    }


    void ProgrammaticDialog::closeEvent(QCloseEvent* event) {
        auto begin = beginProxies();
        auto end   = endProxies();
        auto it    = begin;

        while (it!=end && (*it)->okToClose()) {
            ++it;
        }

        if (it == end) {
            for (auto it=begin ; it!=end ; ++it) {
                (*it)->closing();
            }

            event->accept();
        } else {
            event->ignore();
        }
    }


    void ProgrammaticDialog::resizeEvent(QResizeEvent* event) {
        QDialog::resizeEvent(event);

        for (auto it=beginProxies(),end=endProxies() ; it!=end ; ++it) {
            (*it)->resized(event);
        }
    }
}
