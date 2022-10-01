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
* This file implements the \ref EQt::PaperSizePopupMenu class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QActionGroup>
#include <QShowEvent>
#include <QPageSize>
#include <QLocale>
#include <QSizeF>
#include <QDialog>
#include <QMenu>

#include <util_page_size.h>

#include "eqt_custom_paper_size_dialog.h"
#include "eqt_application.h"
#include "eqt_paper_size_popup_menu.h"

namespace EQt {
    PaperSizePopupMenu::PaperSizePopupMenu(QWidget* parent):QMenu(parent) {
        configureWidget();
    }


    PaperSizePopupMenu::PaperSizePopupMenu(const QString& title, QWidget* parent):QMenu(title, parent) {
        configureWidget();
    }


    PaperSizePopupMenu::PaperSizePopupMenu(QAction* action, QWidget* parent):QMenu(parent) {
        configureWidget();
        setPaperSizeAction(action);
    }


    PaperSizePopupMenu::PaperSizePopupMenu(
            const QString& title,
            QAction*       action,
            QWidget*       parent
        ):QMenu(
            title,
            parent
        ) {
        configureWidget();
        setPaperSizeAction(action);
    }


    PaperSizePopupMenu::~PaperSizePopupMenu() {}


    void PaperSizePopupMenu::setPaperSizeAction(QAction* action) {
        currentPaperSizeAction = action;
    }


    QAction* PaperSizePopupMenu::paperSizeAction() const {
        return currentPaperSizeAction;
    }


    void PaperSizePopupMenu::showEvent(QShowEvent* event) {
        if (currentPaperSizeAction != Q_NULLPTR) {
            bool           ok;
            Util::PageSize pageSize;
            QVariant       variant = currentPaperSizeAction->data();

            if (!variant.canConvert<QString>()) {
                ok = false;
            } else {
                pageSize = Util::PageSize::fromString(variant.value<QString>(), &ok);
            }

            if (ok) {
                QLocale::MeasurementSystem measurementSystem = Application::locale()->measurementSystem();
                bool useMetric;

                if (measurementSystem == QLocale::MeasurementSystem::ImperialUSSystem ||
                    measurementSystem == QLocale::MeasurementSystem::ImperialUKSystem    ) {
                    useMetric = false;
                } else {
                    useMetric = true;
                }

                QString description = pageSize.description(useMetric);

                if (standardPageSizeActions.contains(description)) {
                    standardPageSizeActions.value(description)->setChecked(true);
                    customPageSizeAction->setText(tr("Custom"));
                } else {
                    customPageSizeAction->setChecked(true);
                    customPageSizeAction->setText(description);
                }
            } else {
                QAction* checkedAction = actionGroup->checkedAction();
                if (checkedAction != Q_NULLPTR) {
                    checkedAction->setChecked(false);
                }
            }
        }

        QMenu::showEvent(event);
    }


    void PaperSizePopupMenu::actionTriggered(QAction* triggeredAction) {
        if (currentPaperSizeAction != Q_NULLPTR) {
            if (triggeredAction == customPageSizeAction) {
                EQt::CustomPaperSizeDialog customPaperSizeDialog(dynamic_cast<QWidget*>(parent()));

                QVariant variant = currentPaperSizeAction->data();
                if (variant.canConvert<QString>()) {
                    bool ok = false;
                    Util::PageSize pageSize = Util::PageSize::fromString(variant.toString(), &ok);
                    if (ok) {
                        customPaperSizeDialog.setWidth(pageSize.width());
                        customPaperSizeDialog.setHeight(pageSize.height());
                    }
                }

                int result = customPaperSizeDialog.exec();

                if (result == QDialog::Accepted) {
                    double width  = customPaperSizeDialog.width();
                    double height = customPaperSizeDialog.height();

                    Util::PageSize customPage(width, height);
                    QVariant       variant = customPage.toString();

                    currentPaperSizeAction->setData(variant);
                    currentPaperSizeAction->trigger();
                }
            } else {
                Util::PageSize pageSize = Util::PageSize::fromDescription(triggeredAction->text());
                Q_ASSERT(pageSize.isValid());

                QVariant variant(pageSize.toString());
                currentPaperSizeAction->setData(variant);
                currentPaperSizeAction->trigger();
            }
        }
    }


    void PaperSizePopupMenu::configureWidget() {
        currentPaperSizeAction = Q_NULLPTR;

        QStringList descriptions = Application::paperSizeDescriptions();
        actionGroup = new QActionGroup(this);

        standardPageSizeActions.clear();

        for (auto it=descriptions.begin(),end=descriptions.end() ; it!=end ; ++it) {
            if (it->isEmpty()) {
                addSeparator();
            } else {
                QAction* action = addAction(*it);
                actionGroup->addAction(action);
                standardPageSizeActions.insert(*it, action);

                action->setCheckable(true);
            }
        }

        addSeparator();

        customPageSizeAction = addAction(tr("Custom"));
        actionGroup->addAction(customPageSizeAction);

        customPageSizeAction->setCheckable(true);

        connect(this, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
    }
}
