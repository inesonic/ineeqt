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
* This file implements the \ref EQt::ToolButtonArray class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QList>
#include <QGridLayout>
#include <QShowEvent>
#include <QResizeEvent>
#include <QTimer>

#include "eqt_tool_button.h"
#include "eqt_tool_button_array.h"

namespace EQt {
    ToolButtonArray::ToolButtonArray(QWidget* parent):QWidget(parent) {
        currentFixedDimension = FixedDimension::HEIGHT;
        currentMaximumButtons = static_cast<unsigned>(-1);

        configureWidget();
    }


    ToolButtonArray::ToolButtonArray(
            ToolButtonArray::FixedDimension fixedDimension,
            unsigned                        numberButtons,
            QWidget*                        parent
        ):QWidget(
            parent
        ) {
        currentFixedDimension = fixedDimension;
        currentMaximumButtons = numberButtons;

        configureWidget();
    }


    ToolButtonArray::~ToolButtonArray() {}


    void ToolButtonArray::setFixedDimension(ToolButtonArray::FixedDimension newFixedDimension) {
        if (newFixedDimension != currentFixedDimension) {
            currentFixedDimension = newFixedDimension;
            repositionButtons();
        }
    }


    ToolButtonArray::FixedDimension ToolButtonArray::fixedDimension() const {
        return currentFixedDimension;
    }


    void ToolButtonArray::setMaximumButtonsInFixedDimension(unsigned numberFixedButtons) {
        if (currentMaximumButtons != numberFixedButtons) {
            currentMaximumButtons = numberFixedButtons;
            repositionButtons();
        }
    }


    unsigned ToolButtonArray::maximumButtonsInFixedDimension() const {
        return currentMaximumButtons;
    }


    void ToolButtonArray::setMaximumButtonWidth(unsigned newMaximumButtonWidth) {
        if (currentFixedDimension != FixedDimension::WIDTH || newMaximumButtonWidth != currentMaximumButtons) {
            currentFixedDimension = FixedDimension::WIDTH;
            currentMaximumButtons = newMaximumButtonWidth;
        }
    }


    void ToolButtonArray::setMaximumButtonHeight(unsigned newMaximumButtonHeight) {
        if (currentFixedDimension != FixedDimension::HEIGHT || newMaximumButtonHeight != currentMaximumButtons) {
            currentFixedDimension = FixedDimension::HEIGHT;
            currentMaximumButtons = newMaximumButtonHeight;
        }
    }


    void ToolButtonArray::setButtonSpacing(int newSpacing) {
        QGridLayout* layout = dynamic_cast<QGridLayout*>(QWidget::layout());
        Q_ASSERT(layout != Q_NULLPTR);

        layout->setSpacing(newSpacing);
    }


    int ToolButtonArray::buttonSpacing() const {
        QGridLayout* layout = dynamic_cast<QGridLayout*>(QWidget::layout());
        Q_ASSERT(layout != Q_NULLPTR);

        return layout->spacing();
    }


    ToolButton* ToolButtonArray::addButton(ToolButton* newToolButton) {
        connect(newToolButton, &ToolButton::clicked, this, &ToolButtonArray::propagateButtonClick);
        newToolButton->setGlobalScaleFactorSettingsDisabled();

        currentButtons.append(newToolButton);
        repositionButtons();

        return newToolButton;
    }


    ToolButton* ToolButtonArray::addButton(QAction* action) {
        return addButton(new ToolButton(action));
    }


    unsigned ToolButtonArray::numberButtons() const {
        return static_cast<unsigned>(currentButtons.count());
    }


    ToolButton* ToolButtonArray::button(unsigned index) const {
        ToolButton* result;

        if (index < numberButtons()) {
            result = currentButtons.at(index);
        } else {
            result = Q_NULLPTR;
        }

        return result;
    }


    const QList<ToolButton*>& ToolButtonArray::buttons() const {
        return currentButtons;
    }


    void ToolButtonArray::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);

        if (repositionOnNextShow && !repositionTimer->isActive()) {
            repositionTimer->start(0);
        }
    }


    void ToolButtonArray::resizeEvent(QResizeEvent* event) {
        QWidget::resizeEvent(event);
    }


    void ToolButtonArray::propagateButtonClick() {
        ToolButton* toolButton = dynamic_cast<ToolButton*>(sender());
        Q_ASSERT(toolButton != Q_NULLPTR);

        emit buttonClicked(toolButton);

        QList<QAction*> actions = toolButton->actions();
        for (QList<QAction*>::const_iterator it=actions.constBegin(),end=actions.constEnd() ; it!=end ; ++it) {
            QAction* action = *it;
            emit buttonClicked(action);
        }

        int index = currentButtons.indexOf(toolButton);
        emit buttonClicked(index);
    }


    void ToolButtonArray::performRepositioning() {
        repositionOnNextShow = false;

        QGridLayout* layout = dynamic_cast<QGridLayout*>(QWidget::layout());
        Q_ASSERT(layout != Q_NULLPTR);

        while (!layout->isEmpty()) {
            layout->takeAt(0);
        }

        unsigned numberButtons = ToolButtonArray::numberButtons();
        if (numberButtons > 0) {
            unsigned numberColumns;
            if (currentFixedDimension == FixedDimension::HEIGHT) {
                numberColumns = (numberButtons + currentMaximumButtons - 1)/ currentMaximumButtons;
            } else {
                numberColumns = currentMaximumButtons;
            }

            unsigned row    = 0;
            unsigned column = 0;

            for (  QList<ToolButton*>::const_iterator it  = currentButtons.constBegin(),
                                                      end = currentButtons.constEnd()
                 ; it!=end
                 ; ++it
                ) {
                layout->addWidget(*it, row, column);
                ++column;
                if (column >= numberColumns) {
                    column = 0;
                    ++row;
                }
            }
        }
    }


    void ToolButtonArray::configureWidget() {
        QGridLayout* layout = new QGridLayout;
        layout->setContentsMargins(0, 0, 0, 0);

        setLayout(layout);

        repositionTimer = new QTimer(this);
        repositionTimer->setSingleShot(true);
        connect(repositionTimer, &QTimer::timeout, this, &ToolButtonArray::performRepositioning);

        repositionOnNextShow = false;
    }


    void ToolButtonArray::repositionButtons() {
        if (isVisible()) {
            performRepositioning();
        } else {
            repositionOnNextShow = true;
        }
    }
}
