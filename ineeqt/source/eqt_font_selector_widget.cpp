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
* This file implements the \ref EQt::FontSelectorWidget class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QFont>
#include <QHBoxLayout>
#include <QFontMetrics>
#include <QApplication>

#include "eqt_common.h"
#include "eqt_font_combo_box.h"
#include "eqt_spin_box.h"
#include "eqt_font_selector_widget.h"

namespace EQt {
    const unsigned FontSelectorWidget::defaultMinimumFontSize = 8;
    const unsigned FontSelectorWidget::defaultMaximumFontSize = 144;

    FontSelectorWidget::FontSelectorWidget(QWidget* parent):QWidget(parent) {
        configureWidget();
    }


    FontSelectorWidget::FontSelectorWidget(
            QAction* newFontFamilyAction,
            QAction* newFontSizeAction,
            QWidget* parent
        ):QWidget(
            parent
        ) {
        configureWidget();
        setFontFamilyAction(newFontFamilyAction);
        setFontSizeAction(newFontSizeAction);
    }


    FontSelectorWidget::~FontSelectorWidget() {}


    QAction* FontSelectorWidget::fontFamilyAction() const {
        return fontFamilyComboBox->defaultAction();
    }


    QAction* FontSelectorWidget::fontSizeAction() const {
        return fontSizeSpinBox->defaultAction();
    }


    unsigned FontSelectorWidget::minimumFontSize() const {
        return static_cast<unsigned>(fontSizeSpinBox->minimum());
    }


    unsigned FontSelectorWidget::maximumFontSize() const {
        return static_cast<unsigned>(fontSizeSpinBox->maximum());
    }


    QString FontSelectorWidget::fontSizeSuffix() const {
        return fontSizeSpinBox->suffix();
    }


    void FontSelectorWidget::setFontFamilyAction(QAction* newAction) {
        fontFamilyComboBox->setDefaultAction(newAction);
    }


    void FontSelectorWidget::setFontSizeAction(QAction* newAction) {
        fontSizeSpinBox->setDefaultAction(newAction);
    }


    void FontSelectorWidget::setMinimumFontSize(unsigned newMinimumFontSize) {
        fontSizeSpinBox->setMinimum(newMinimumFontSize);
    }


    void FontSelectorWidget::setMaximumFontSize(unsigned newMaximumFontSize) {
        fontSizeSpinBox->setMaximum(newMaximumFontSize);
    }


    void FontSelectorWidget::setFontSizeRange(unsigned newMinimumFontSize, unsigned newMaximumFontSize) {
        fontSizeSpinBox->setRange(newMinimumFontSize, newMaximumFontSize);
    }


    void FontSelectorWidget::setFontSizeSuffix(const QString& newSuffix) {
        fontSizeSpinBox->setSuffix(newSuffix);
    }


    void FontSelectorWidget::configureWidget() {
        QHBoxLayout* layout = new QHBoxLayout;
        setLayout(layout);

        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(gapSizeInPixels);

        fontFamilyComboBox = new FontComboBox;
        fontFamilyComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        layout->addWidget(fontFamilyComboBox);

        fontSizeSpinBox = new SpinBox;
        layout->addWidget(fontSizeSpinBox);

        fontSizeSpinBox->setRange(defaultMinimumFontSize, defaultMaximumFontSize);
        fontSizeSpinBox->setSuffix(tr(" pt."));

        QFontMetrics metrics(QApplication::font());
        fontSizeSpinBox->setFixedWidth(metrics.horizontalAdvance(tr("888 ptxx")));
        fontFamilyComboBox->setMinimumWidth(metrics.horizontalAdvance(tr("Times New Roman")));
    }
}

