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
* This file implements the \ref EQt::ComboBox class.
***********************************************************************************************************************/

#include <QFontComboBox>
#include <QWidget>
#include <QAction>
#include <QFont>

#include "eqt_common.h"
#include "eqt_font_combo_box.h"

namespace EQt {
    FontComboBox::FontComboBox(QWidget* parent):QFontComboBox(parent) {
        configureWidget();
    }


    FontComboBox::FontComboBox(QAction* action, QWidget* parent):QFontComboBox(parent) {
        configureWidget();
        setDefaultAction(action);
    }


    FontComboBox::~FontComboBox() {}


    QAction* FontComboBox::defaultAction() {
        QList<QAction*> actionsList = actions();
        if (actionsList.empty()) {
            return Q_NULLPTR;
        } else {
            Q_ASSERT(actionsList.size() == 1);
            return actionsList.first();
        }
    }


    void FontComboBox::setDefaultAction(QAction* action) {
        QList<QAction*> actionsList = actions();
        if (!actionsList.empty()) {
            Q_ASSERT(actionsList.size() == 1);

            QAction* oldAction = actionsList.first();
            disconnect(oldAction, &QAction::triggered, this, &FontComboBox::triggered);
            disconnect(oldAction, &QAction::changed, this, &FontComboBox::changed);

            removeAction(oldAction);
        }

        addAction(action);
        connect(action, &QAction::triggered, this, &FontComboBox::triggered);
        connect(action, &QAction::changed, this, &FontComboBox::changed);

        QString fontFamily = action->data().toString();
        if (!fontFamily.isEmpty()) {
            QFont newFont = QFont(fontFamily);
            setCurrentFont(newFont);
        }
    }


    void FontComboBox::currentFontChanged(const QFont& font) {
        QList<QAction*> actionsList = actions();
        if (!actions().empty()) {
            Q_ASSERT(actions().size() == 1);
            QAction* action = actions().first();

            QString currentFamily = action->data().toString();
            QString newFamily     = font.family();

            if (currentFamily != newFamily) {
                action->setData(font.family());
                action->trigger();
            }
        }
    }


    void FontComboBox::triggered() {
        Q_ASSERT(actions().size() == 1);

        QAction* action       = actions().first();
        QVariant variant      = action->data();
        QString  actionFont   = variant.isValid() ? variant.toString() : QString();
        QString  selectedFont = currentFont().family();

        if (actionFont != selectedFont) {
            setCurrentFont(actionFont);
        }
    }


    void FontComboBox::changed() {
        if (!actions().empty()) {
            Q_ASSERT(actions().size() == 1);
            QAction* action = actions().first();

            if (action->isEnabled() != isEnabled()) {
                setEnabled(action->isEnabled());
            }
        }
    }


    void FontComboBox::configureWidget() {
        connect(this, &QFontComboBox::currentFontChanged, this, &FontComboBox::currentFontChanged);
    }
}
