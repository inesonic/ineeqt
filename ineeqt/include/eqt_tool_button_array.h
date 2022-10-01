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
* This header defines the \ref EQt::ToolButtonArray class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_TOOL_BUTTON_ARRAY_H
#define EQT_TOOL_BUTTON_ARRAY_H

#include <QWidget>
#include <QList>

#include "eqt_common.h"

class QAction;
class QShowEvent;
class QResizeEvent;
class QTimer;

namespace EQt {
    class ToolButton;

    /**
     * Widget you can use to create an array of tool buttons with a fixed maximum width or height in buttons or
     * pixels.
     *
     * You can use the methods:
     *
     *     * \ref EQt::ToolButtonArray::setFixedDimension
     *     * \ref EQt::ToolButtonArray::setMaximumButtonsInFixedDimension
     *     * \ref EQt::ToolButtonArray::setMaximumButtonWidth
     *     * \ref EQt::ToolButtonArray::setMaximumButtonHeight
     *
     * To set the constraints based on the number of buttons.  You can force the widget to be fixed width or fixed
     * height to constraint the number of buttons based on pixels.
     *
     * Longer term, this widget may support dynamic adjustment of tool button sizes to facilitate both sets of
     * constraints.
     */
    class EQT_PUBLIC_API ToolButtonArray:public QWidget {
        Q_OBJECT

        public:
            /**
             * Enumeration indicating which dimension should be fixed.
             */
            enum class FixedDimension {
                /**
                 * Value indicating fixed width.
                 */
                WIDTH,

                /**
                 * Value indicating fixed height.
                 */
                HEIGHT
            };

            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ToolButtonArray(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] fixedDimension Value indicating the fixed dimension.
             *
             * \param[in] numberButtons  The number of buttons to limit for this dimension.
             *
             * \param[in] parent         The parent object for this tool button.
             */
            ToolButtonArray(FixedDimension fixedDimension, unsigned numberButtons, QWidget* parent = Q_NULLPTR);

            ~ToolButtonArray() override;

            /**
             * Method you can use to set the fixed dimension for the button array.
             *
             * \param[in] newFixedDimension The fixed dimension.
             */
            void setFixedDimension(FixedDimension newFixedDimension);

            /**
             * Method you can use to determine the fixed dimension.
             *
             * \return Returns the currently fixed dimension.
             */
            FixedDimension fixedDimension() const;

            /**
             * Method you can use to set the number of buttons in the fixed dimension
             *
             * \param[in] numberFixedButtons The maximum number of buttons in the fixed dimension.
             */
            void setMaximumButtonsInFixedDimension(unsigned numberFixedButtons);

            /**
             * Method you can use to determine the number of buttons in the fixed dimension.
             *
             * \return Returns the maximum number of buttons in the fixed dimension.
             */
            unsigned maximumButtonsInFixedDimension() const;

            /**
             * Method you can use to set the maximum width in buttons.  This method is synonymous with calling both
             * \ref EQt::ToolButtonArray::setFixedDimension followed by
             * \ref EQt::ToolButtonArray::setMaximumButtonsInFixedDimension.
             *
             * \param[in] newMaximumButtonWidth The new maximum width, in buttons.
             */
            void setMaximumButtonWidth(unsigned newMaximumButtonWidth);

            /**
             * Method you can use to set the maximum height in buttons.  This method is synonymous with calling both
             * \ref EQt::ToolButtonArray::setFixedDimension followed by
             * \ref EQt::ToolButtonArray::setMaximumButtonsInFixedDimension.
             *
             * \param[in] newMaximumButtonHeight The new maximum height, in buttons.
             */
            void setMaximumButtonHeight(unsigned newMaximumButtonHeight);

            /**
             * Method you can use to set the layout button spacing.
             *
             * \param[in] newSpacing The new button spacing, in pixels.
             */
            void setButtonSpacing(int newSpacing);

            /**
             * Method you can use to obtain the current button spacing.
             *
             * \return Returns the current button spacing, in pixels.
             */
            int buttonSpacing() const;

            /**
             * Method you can use to add a tool button.
             *
             * \param[in] newToolButton The new button to be added.  This widget will take ownership of the button.
             *
             * \return Returns a pointer to the supplied button.
             */
            ToolButton* addButton(ToolButton* newToolButton);

            /**
             * Method you can use to add a tool button.
             *
             * \param[in] action The action that should be driven by (and drive) the tool button.
             *
             * \return Returns a pointer to the newly created button.
             */
            ToolButton* addButton(QAction* action);

            /**
             * Method you can use to obtain the number of buttons.
             *
             * \return Returns a count of the number of buttons.
             */
            unsigned numberButtons() const;

            /**
             * Method you can use to obtain a single button, by index.  Buttons are indexed based on their position
             * in reading order.
             *
             * \param[in] index The zero based index of the desired button.
             *
             * \return Returns a pointer to the desired button.  A null pointer is returned if the button is invalid.
             */
            ToolButton* button(unsigned index) const;

            /**
             * Method you can use to obtain a list of buttons.  Buttons are listed as presented by the widget in
             * reading order.
             *
             * \return Returns a list of pointers to buttons.
             */
            const QList<ToolButton*>& buttons() const;

        signals:
            /**
             * Signal that is emitted when a button is clicked.
             *
             * \param[out] button Pointer to the activated button.
             */
            void buttonClicked(ToolButton* button);

            /**
             * Signal that is emitted when a button is clicked.
             *
             * \param[out] action Action tied to the clicked button.
             */
            void buttonClicked(QAction* action);

            /**
             * Signal that is emitted when a button is clicked.
             *
             * \param[out] index The index of the clicked button.
             */
            void buttonClicked(unsigned index);

        protected:
            /**
             * Method that is called when this widget is shown or hidden.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void showEvent(QShowEvent* event) override;

            /**
             * Method that is called when this widget is resized.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void resizeEvent(QResizeEvent* event) override;

        private slots:
            /**
             * Slot that is triggered when a tool button is clicked.
             */
            void propagateButtonClick();

            /**
             * Slot that is triggered to perform button repositioning.
             */
            void performRepositioning();

        private:
            /**
             * Method that performs configuration common to all constructors.
             */
            void configureWidget();

            /**
             * Method that repositions buttons to meet a new set of constraints.
             */
            void repositionButtons();

            /**
             * Value indicating which dimension should be fixed.
             */
            FixedDimension currentFixedDimension;

            /**
             * Value indicating the maximum number of buttons in the fixed dimension.
             */
            unsigned currentMaximumButtons;

            /**
             * Flag that indicates if button repositioning is needed.
             */
            bool repositionOnNextShow;

            /**
             * Current list of buttons, in reading order.
             */
            QList<ToolButton*> currentButtons;

            /**
             * Timer used to perform deferred updates to the layout.
             */
            QTimer* repositionTimer;
    };
}

#endif
