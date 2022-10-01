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
* This file implements the \ref EQt::ToolButton class.
***********************************************************************************************************************/

#include <QWidget>
#include <QStyle>
#include <QAction>
#include <QList>
#include <QSize>
#include <QPointer>
#include <QMenu>
#include <QToolButton>

#include "eqt_tool_button.h"

namespace EQt {
    const unsigned              ToolButton::buttonSizeData[] = { 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256, 0 };
    const unsigned              ToolButton::defaultButtonSize = 32;

    QList<unsigned>             ToolButton::buttonSizes;
    unsigned                    ToolButton::defaultSizeIndex;
    QList<QPointer<ToolButton>> ToolButton::buttons;

    ToolButton::ToolButton(bool showBorder, QWidget* parent):QToolButton(parent) {
        configureWidget(showBorder);
    }


    ToolButton::ToolButton(QAction* action, bool showBorder, QWidget* parent):QToolButton(parent) {
        configureWidget(showBorder);

        if (action != Q_NULLPTR) {
            setDefaultAction(action);
        }
    }


    ToolButton::ToolButton(int newRelativeSizeFactor, bool showBorder, QWidget* parent):QToolButton(parent) {
        configureWidget(showBorder, newRelativeSizeFactor);
    }


    ToolButton::ToolButton(
            int      newRelativeSizeFactor,
            QAction* action,
            bool     showBorder,
            QWidget* parent
        ):QToolButton(
            parent
        ) {
        configureWidget(showBorder, newRelativeSizeFactor);

        if (action != Q_NULLPTR) {
            setDefaultAction(action);
        }
    }


    ToolButton::ToolButton(QMenu* menu, bool showBorder, QWidget* parent):QToolButton(parent) {
        configureWidget(showBorder, menu);
    }


    ToolButton::ToolButton(QAction* action, QMenu* menu, bool showBorder, QWidget* parent):QToolButton(parent) {
        configureWidget(showBorder, menu);

        if (action != Q_NULLPTR) {
            setDefaultAction(action);
        }
    }


    ToolButton::ToolButton(
            int      newRelativeSizeFactor,
            QMenu*   menu,
            bool     showBorder,
            QWidget* parent
        ):QToolButton(
            parent
        ) {
        configureWidget(showBorder, newRelativeSizeFactor, menu);
    }


    ToolButton::ToolButton(
            int      newRelativeSizeFactor,
            QAction* action,
            QMenu*   menu,
            bool     showBorder,
            QWidget* parent
        ):QToolButton(
            parent
        ) {
        configureWidget(showBorder, newRelativeSizeFactor, menu);

        if (action != Q_NULLPTR) {
            setDefaultAction(action);
        }
    }


    ToolButton::~ToolButton() {}


    float ToolButton::xScaling() const {
        return currentXScaling;
    }


    const QList<unsigned>& ToolButton::supportedButtonSizes() {
        buildButtonSizes();
        return buttonSizes;
    }


    bool ToolButton::obeyGLobalScaleFactorSettingChange() const {
        return currentObeyGlobalScaleFactor;
    }


    bool ToolButton::ignoreGLobalScaleFactorSettingChange() const {
        return !obeyGLobalScaleFactorSettingChange();
    }


    unsigned ToolButton::iconSizePixels(int relativeSizeFactor) {
        buildButtonSizes();

        int index = defaultSizeIndex + relativeSizeFactor;
        if (index < 0) {
            index = 0;
        } else {
            int s = buttonSizes.size();
            if (index >= s) {
                index = s - 1;
            }
        }

       return buttonSizes.at(index);
    }


    QSize ToolButton::sizePixels(int relativeSizeFactor, float xScaling) {
        unsigned iconSize = iconSizePixels(relativeSizeFactor);

        #if (defined(Q_OS_WINDOWS))

            static constexpr unsigned horizontalButtonBorder = 7;
            static constexpr unsigned verticalButtonBorder   = 6;

        #elif (defined(Q_OS_LINUX))

            static constexpr unsigned horizontalButtonBorder = 7;
            static constexpr unsigned verticalButtonBorder   = 2;

        #elif (defined(Q_OS_DARWIN))

            static constexpr unsigned horizontalButtonBorder = 7;
            static constexpr unsigned verticalButtonBorder   = 2;

        #else

            #error Unknown platform.

        #endif

        return QSize(
            static_cast<unsigned>(iconSize * xScaling + 0.5) + horizontalButtonBorder,
            iconSize + verticalButtonBorder
        );
    }


    void ToolButton::setGlobalSizeIndex(unsigned newSizeIndex) {
        if (defaultSizeIndex != newSizeIndex) {
            buildButtonSizes();

            defaultSizeIndex = newSizeIndex;
            if (defaultSizeIndex >= static_cast<unsigned>(buttonSizes.size())) {
                defaultSizeIndex = static_cast<unsigned>(buttonSizes.size());
            }

            for (  QList<QPointer<ToolButton>>::const_iterator it  = buttons.constBegin(),
                                                               end = buttons.constEnd()
                 ; it != end
                 ; ++it
                ) {
                const QPointer<ToolButton>& toolButton = *it;
                if (toolButton->currentObeyGlobalScaleFactor) {
                    toolButton->resizeButton();
                }
            }
        }
    }


    void ToolButton::setGlobalSize(unsigned newTargetButtonHeightInPixels) {
        buildButtonSizes();

        unsigned numberSizes   = static_cast<unsigned>(buttonSizes.size());
        unsigned lastEntry     = numberSizes - 1;
        unsigned sizeIndex     = 0;

        while (sizeIndex < lastEntry && buttonSizes.at(sizeIndex + 1) <= newTargetButtonHeightInPixels) {
            ++sizeIndex;
        }

        setGlobalSizeIndex(sizeIndex);
    }


    unsigned ToolButton::globalSize() {
        buildButtonSizes();
        return buttonSizes.at(defaultSizeIndex);
    }


    void ToolButton::setGlobalScaleFactorSettingsEnabled(bool nowEnabled) {
        currentObeyGlobalScaleFactor = nowEnabled;
    }


    void ToolButton::setGlobalScaleFactorSettingsDisabled(bool nowDisabled) {
        setGlobalScaleFactorSettingsEnabled(!nowDisabled);
    }


    void ToolButton::setRelativeSizeFactor(int newRelativeSizeFactor) {
        currentRelativeSizeFactor = newRelativeSizeFactor;
        resizeButton();
    }


    void ToolButton::setXScaling(float newXScaling) {
        currentXScaling = newXScaling;
        resizeButton();
    }


    void ToolButton::configureWidget(bool showBorder, QMenu* menu, int newRelativeSizeFactor) {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setAutoRaise(!showBorder);

        if (menu != Q_NULLPTR) {
            setMenu(menu);
            setPopupMode(QToolButton::InstantPopup);
        }

        currentRelativeSizeFactor    = newRelativeSizeFactor;
        currentObeyGlobalScaleFactor = true;
        currentXScaling              = 1.0F;

        resizeButton();

        buttons.append(this);
    }


    void ToolButton::buildButtonSizes() {
        if (buttonSizes.isEmpty()) {
            const unsigned* bs = buttonSizeData;
            while (*bs != 0) {
                buttonSizes.append(*bs);
                ++bs;
            }

            setGlobalSize(defaultButtonSize);
        }
    }


    void ToolButton::resizeButton() {
        unsigned buttonSize = iconSizePixels(currentRelativeSizeFactor);
        QSize size(static_cast<unsigned>(buttonSize * currentXScaling + 0.5), buttonSize);

        setMinimumSize(size);
        setIconSize(size);
    }
}

