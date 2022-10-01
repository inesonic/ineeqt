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
* This header defines the \ref EQt::ToolButton class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_TOOL_BUTTON_H
#define EQT_TOOL_BUTTON_H

#include <QList>
#include <QPointer>
#include <QToolButton>

class QMenu;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that extends QToolButton to support scalable icons as well as automatic configuration of several application
     * default values.
     */
    class EQT_PUBLIC_API ToolButton:public QToolButton {
        Q_OBJECT

        public:
            /**
             * The default size of tool buttons.
             */
            static const unsigned defaultButtonSize;

            /**
             * Constructor.
             *
             * \param[in] showBorder If true, the border will be displayed on the button at all times.  If false, the
             *                       border will only be displayed when the user hovers over the button.
             *
             * \param[in] parent     The parent object for this tool button.
             */
            ToolButton(bool showBorder = true, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action     The action to link to this tool button.
             *
             * \param[in] showBorder If true, the border will be displayed on the button at all times.  If false, the
             *                       border will only be displayed when the user hovers over the button.
             *
             * \param[in] parent     The parent object for this tool button.
             */
            ToolButton(QAction* action, bool showBorder = true, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] showBorder            If true, the border will be displayed on the button at all times.  If
             *                                  false, the border will only be displayed when the user hovers over the
             *                                  button.
             *
             * \param[in] parent                The parent object for this window.
             */
            ToolButton(int newRelativeSizeFactor, bool showBorder = true, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] action                The action to link to this tool editor.
             *
             * \param[in] showBorder            If true, the border will be displayed on the button at all times.  If
             *                                  false, the border will only be displayed when the user hovers over the
             *                                  button.
             *
             * \param[in] parent                The parent object for this window.
             */
            ToolButton(
                int      newRelativeSizeFactor,
                QAction* action,
                bool     showBorder = true,
                QWidget* parent = Q_NULLPTR
            );

            /**
             * Constructor.
             *
             * \param[in] menu       A menu that should pop-up when the button is pressed.
             *
             * \param[in] showBorder If true, the border will be displayed on the button at all times.  If false, the
             *                       border will only be displayed when the user hovers over the button.
             *
             * \param[in] parent     The parent object for this tool button.
             */
            ToolButton(QMenu* menu, bool showBorder = true, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action     The action to link to this tool button.
             *
             * \param[in] menu       A menu that should pop-up when the button is pressed.
             *
             * \param[in] showBorder If true, the border will be displayed on the button at all times.  If false, the
             *                       border will only be displayed when the user hovers over the button.
             *
             * \param[in] parent     The parent object for this tool button.
             */
            ToolButton(QAction* action, QMenu* menu, bool showBorder = true, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] menu                  A menu that should pop-up when the button is pressed.
             *
             * \param[in] showBorder            If true, the border will be displayed on the button at all times.  If
             *                                  false, the border will only be displayed when the user hovers over the
             *                                  button.
             *
             * \param[in] parent                The parent object for this window.
             */
            ToolButton(int newRelativeSizeFactor, QMenu* menu, bool showBorder = true, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.  for details, see the
             *                                  \ref ToolButton::setRelativeSizeFactor method.
             *
             * \param[in] action                The action to link to this tool editor.
             *
             * \param[in] menu                  A menu that should pop-up when the button is pressed.
             *
             * \param[in] showBorder            If true, the border will be displayed on the button at all times.  If
             *                                  false, the border will only be displayed when the user hovers over the
             *                                  button.
             *
             * \param[in] parent                The parent object for this window.
             */
            ToolButton(
                int      newRelativeSizeFactor,
                QAction* action,
                QMenu*   menu,
                bool     showBorder = true,
                QWidget* parent = Q_NULLPTR
            );

            ~ToolButton() override;

            /**
             * Determines the relative size factor to use for this button.
             *
             * \return Returns the relative size factor to use for this button.
             */
            inline int relativeSizeFactor() const {
                return currentRelativeSizeFactor;
            }

            /**
             * Method you can use to determine the X scaling factor for this button.
             *
             * \return Returns the X scaling factor.
             */
            float xScaling() const;

            /**
             * Returns a list of the supported tool button sizes.
             *
             * \return Returns a list of supported tool button sizes.
             */
            static const QList<unsigned>& supportedButtonSizes();

            /**
             * Globally sets the button size for all tool buttons.  You can use this button to adjust button sizes on
             * HiDPI displays.  For details, see the \ref EQt::ToolButton::setRelativeSizeFactor method.
             *
             * Note that this method is not thread safe.
             *
             * Button have the ability to be told to ignore requests for global factor change.  See
             * \ref EQt::ToolButton::setGlobalScaleFactorSettingsEnabled and
             * \ref EQt::ToolButton::setGlobalScaleFactorSettingsDisabled methods for details.  By default, buttons
             * will obey global scale factor change requests.
             *
             * \param[in] newSizeIndex The new scale factor for all the tool buttons.
             */
            static void setGlobalSizeIndex(unsigned newSizeIndex);

            /**
             * Globally sets the button size for all tool buttons.  You can use this button to adjust button sizes on
             * HiDPI displays.  For details, see the \ref EQt::ToolButton::setRelativeSizeFactor method.
             *
             * Note that this method is not thread safe.
             *
             * Button have the ability to be told to ignore requests for global factor change.  See
             * \ref EQt::ToolButton::setGlobalScaleFactorSettingsEnabled and
             * \ref EQt::ToolButton::setGlobalScaleFactorSettingsDisabled methods for details.  By default, buttons
             * will obey global scale factor change requests.
             *
             * \param[in] newTargetButtonHeightInPixels The target height for buttons.  By default, buttons will take
             *                                          on this size for both height and width.  You can use the
             *                                          \ref EQt::ToolButton::setXScaling to set the X scaling relative
             *                                          to this height.
             */
            static void setGlobalSize(unsigned newTargetButtonHeightInPixels);

            /**
             * Reports the current nominal button icon height, in pixels.
             *
             * \return Returns the current nominal button icon size, in pixels. The button will be slightly larger than
             *         this value.
             */
            static unsigned globalSize();

            /**
             * Method you can use to determine if this button will obey global scale factor change requests.
             *
             * \return Returns true if global scale factor settings will be obeyed.  Returns false if global scale
             *         factor settings will be ignored.
             */
            bool obeyGLobalScaleFactorSettingChange() const;

            /**
             * Method you can use to determine if this button will ignore global scale factor change requests.
             *
             * \return Returns true if global scale factor settings will be ignored.  Returns false if global scale
             *         factor settings will be obeyed.
             */
            bool ignoreGLobalScaleFactorSettingChange() const;

            /**
             * Method you can use to obtain the expected size of a tool button icon in pixels based on a relative size
             * factor value.
             *
             * \param[in] relativeSizeFactor The button relative size factor.
             */
            static unsigned iconSizePixels(int relativeSizeFactor = 0);

            /**
             * Method you can use to obtain the expected size of a tool button in pixels based on a relative size
             * factor value.
             *
             * \param[in] relativeSizeFactor The button relative size factor.
             *
             * \param[in] xScaling
             */
            static QSize sizePixels(int relativeSizeFactor = 0, float xScaling = 1.0);

        public slots:
            /**
             * Slot you can use to enable obeying global scale factor settings.
             *
             * \param[in] nowEnabled If true, this button will obey global scale factor setting requests.  If false,
             *                       this button will ignore global scale factor setting requests.
             */
            void setGlobalScaleFactorSettingsEnabled(bool nowEnabled = true);

            /**
             * Slot you can use to disable obeying global scale factor settings.
             *
             * \param[in] nowDisabled If true, this button will ignore global scale factor setting requests.  If false,
             *                        this button will obey global scale factor setting requests.
             */
            void setGlobalScaleFactorSettingsDisabled(bool nowDisabled = true);

            /**
             * Sets the relative size factor to use for this button.
             *
             * To avoid dynamic resizing of buttons by Qt, button icon sizes are based on a table of allowed button
             * sizes.  Icons are expected to be drawn in each of the sizes listed in the table.  You can use the
             * \ref EQt::ToolButton::supportedButtonSizes method to obtain a list of allowed button sizes.
             *
             * Note that button size values are in multiples of 1.5 and 2.  As an example, button sizes 32, 48, and 64
             * are expected to be supported where 48 is \f[ 1.5 \times 32 \f] and 64 is \f[ 2 \times 32 \f].
             *
             * The default button size, indicated by the \ref ToolButton::defaultButtonSize static value, is translated
             * into an index into this table.  The actual button size is determined by adding the relative size factor
             * and the scale factor to the index to determine the actual button size.
             *
             * The relative size factor is intended to allow for scaling of one button relative to another.  The scale
             * factor is intended to scale up the entire suite of buttons on HiDPI displays.
             *
             * \param[in] newRelativeSizeFactor The new scale factor to apply to this button.
             */
            void setRelativeSizeFactor(int newRelativeSizeFactor);

            /**
             * Method you can use to set the X to Y scaling.
             *
             * \param[in] newXScaling The new X scaling value.  A value of 2.0 will indicate that the button should
             *                        be twice as wide as it tall.  A value of 0.5 will indicate that the button should
             *                        be twice as tall as it is wide.
             */
            void setXScaling(float newXScaling);

        private:
            /**
             * Raw array used to provide a list of supported button sizes.
             */
            static const unsigned buttonSizeData[];

            /**
             * Performs operations needed by all constructors.
             *
             * \param[in] showBorder            If true, the border will be displayed on the button at all times.  If
             *                                  false, the border will only be displayed when the user hovers over the
             *                                  button.
             *
             * \param[in] menu                  The menu associated with this button.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the icon.
             */
            void configureWidget(bool showBorder, QMenu* menu = Q_NULLPTR, int newRelativeSizeFactor = 0);

            /**
             * Performs operations needed by all constructors.
             *
             * \param[in] showBorder            If true, the border will be displayed on the button at all times.  If
             *                                  false, the border will only be displayed when the user hovers over the
             *                                  button.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the icon.
             *
             * \param[in] menu                  The menu associated with this button.
             */
            inline void configureWidget(bool showBorder, int newRelativeSizeFactor, QMenu* menu = Q_NULLPTR) {
                configureWidget(showBorder, menu, newRelativeSizeFactor);
            }

            /**
             * Builds the button size list.
             */
            static void buildButtonSizes();

            /**
             * Resizes the button.
             */
            void resizeButton();

            /**
             * The current relative size factor for this button.
             */
            int currentRelativeSizeFactor;

            /**
             * Flag indicating if global scale factor settings should be ignored for this button.
             */
            bool currentObeyGlobalScaleFactor;

            /**
             * Value used to represent the X scaling.
             */
            float currentXScaling;

            /**
             * The index into buttonSizes indicating the default button size.
             */
            static unsigned defaultSizeIndex;

            /**
             * List of supported button icon sizes.
             */
            static QList<unsigned> buttonSizes;

            /**
             * List of buttons.  This list is used to allow the application to dynamically adjust the sizes of button
             * based on the current screen resolution.
             */
            static QList<QPointer<ToolButton>> buttons;
    };
}

#endif
