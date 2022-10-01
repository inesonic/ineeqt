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
* This header defines the \ref EQt::LineEdit class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_LINE_EDIT_H
#define EQT_LINE_EDIT_H

#include <QLineEdit>
#include <QVariant>

#include "eqt_common.h"

class QWidget;
class QAction;
class QFocusEvent;

namespace EQt {
    /**
     * Class that extends QLineEdit to support use with actions.  You can use this class to integrate QLineEdit
     * instances with the Qt action framework.
     *
     * The widget understands the concept of static and dynamic updates.  When static updates are enabled (default), the
     * widget will update the action only when the widget loses focus.  When dynamic updates are enabled, the widget
     * will update the action as the user modifies the text.
     */
    class EQT_PUBLIC_API LineEdit:public QLineEdit {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            LineEdit(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this line editor.
             *
             * \param[in] parent The parent object for this window.
             */
            LineEdit(QAction* action, QWidget* parent = Q_NULLPTR);

            ~LineEdit() override;

            /**
             * Determines the default action that is receiving updates for this line edit.
             *
             * \return Returns a pointer to the default action.
             */
            QAction* defaultAction();

            /**
             * Method that can be used to specify whether the action should be dynamically updated by this line edit
             * widget.  By default, this widget will support static updates only.
             *
             * \param[in] nowDynamicUpdate If true, the action will be updated dynamically.  If false, the action will
             *                             be updated only when this line editor loses focus.
             */
            void setDynamicUpdate(bool nowDynamicUpdate = true);

            /**
             * Method that can be used to determine if the widget currently supports dynamic updates to the action.
             *
             * \return Returns true if dynamic updates are enabled.  Returns false if the widget updates the action only
             *         when it loses focus.
             */
            inline bool dynamicUpdate() const {
                return currentDynamicUpdate;
            }

            /**
             * Method that can be used to specify whether the action should be statically updated by this line edit
             * widget.  By default, this widget will support static updates only.
             *
             * \param[in] nowStaticUpdate If true, the action will be updated only when the widget loses focus.  If
             *                            false, the action will be updated as the user types.
             */
            inline void setStaticUpdate(bool nowStaticUpdate = true) {
                setDynamicUpdate(!nowStaticUpdate);
            }

            /**
             * Method that can be used to determine if the widget currently supports static updates to the action.
             *
             * \return Returns true if the widget updates the action only when it loses focus.  Returns false if the
             *         widget will update the action as the user types.
             */
            inline bool staticUpdate() const {
                return !dynamicUpdate();
            }

            /**
             * Method that indicates if this line editor will auto-update the displayed value when the line editor loses
             * focus or return is pressed.
             *
             * \return Returns true if auto-update is enabled.  Returns false if auto-update is disabled.
             */
            bool autoUpdateEnabled() const;

            /**
             * Method that indicates if this line editor will not auto-update the displayed value when the line editor
             * loses focus or return is pressed.
             *
             * \return Returns true if auto-update is disabled.  Returns false if auto-update is disabled.
             */
            bool autoUpdateDisabled() const;

        signals:
            /**
             * Signal that is emitted when this line editor receives the focus.
             */
            void receivedFocus();

            /**
             * Signal that is emitted when this line editor loses focus.
             */
            void lostFocus();

        public slots:
            /**
             * Sets the default action for this slot.
             *
             * \param action The action to associate with this line editor.
             */
            void setDefaultAction(QAction* action);

            /**
             * Slot you can use to enable or disable the auto-update feature.
             *
             * \param[in] nowEnabled if true, then automatic update is enabled.  If false, then automatic update is
             *                       disabled.
             */
            void setAutoUpdateEnabled(bool nowEnabled = true);

            /**
             * Slot you can use to disable or enable the auto-update feature.
             *
             * \param[in] nowDisabled if true, then automatic update is disbled.  If false, then automatic update is
             *                        enabled.
             */
            void setAutoUpdateDisabled(bool nowDisabled = true);

        protected slots:
            /**
             * Slot that receives notification when the text is updated.  This slot exists to automatically update the
             * action on user input.
             *
             * \param[in] text The new line edit text.
             */
            void textWasEdited(const QString& text);

            /**
             * Slot that receives triggered updates for the action.
             */
            void triggered();

            /**
             * Slot that receives changed updates for the action.
             */
            void changed();

            /**
             * Slot that is triggered when the user presses return.
             */
            void returnWasPressed();

        protected:
            /**
             * Method that is called every time the action is updated.  You can overload this method to perform
             * additional processing, if needed.
             *
             * The default implementation performs no action.
             *
             * \param[in] newText The new text.
             */
            virtual void updated(const QString& newText);

            /**
             * The event handler for this method.
             *
             * \param[in] event The event that needs to be handled.  Events that support what's this, status tips, and
             *                  tool tips will be intercepted, allowing actions tied to the event to drive the text for
             *                  these events.
             *
             * \return Returns true if the event was recognized, returns false if the event was not recognized.
             */
            bool event(QEvent* event) override;

            /**
             * Method that is triggered when this widget loses focus.
             *
             * \param[in] event The event causing the widget to lose focus.
             */
            void focusOutEvent(QFocusEvent* event) override;

            /**
             * Method that is triggered when the widget receives focus.
             *
             * \param[in] event The event causing the widget to receive focus.
             */
            void focusInEvent(QFocusEvent* event) override;

            /**
             * Method that is used to update the line editor based on the contents of the QAction::data() member. The
             * default implementation converts the variant directly to a text value.
             *
             * \param[in] variant The QVariant to be converted.
             */
            virtual QString variantToText(const QVariant& variant);

            /**
             * Method that is used to convert the value to a variant of the correct type.  The default implementation
             * converts the text value directly to a variant.
             *
             * \param[in] text The text to be be converted.
             *
             * \return Returns a QVariant containing the converted value.  If the value could not be converted, an
             *         invariant QVariant is returned.
             */
            virtual QVariant textToVariant(const QString& text);

        private:
            /**
             * Performs operations needed by all constructors.
             */
            void configureWidget();

            /**
             * Updates the action from the provided text.
             *
             * The method will block updates from the action back to this class instance when updating the action.
             *
             * \param[in] text       The text to apply to the action.
             *
             * \param[in] updateText If true, the contents of the line editor will be updated.  If false, the text
             *                       will be left unchanged.
             *
             * \return Returns true if the text was valid.  Returns false if the text was invalid.
             */
            void updateAction(const QString& text, bool updateText);

            /**
             * If true, then dynamic updates are enabled.
             */
            bool currentDynamicUpdate;

            /**
             * If true, then automatic update is enabled.
             */
            bool currentAutoUpdate;
    };
}

#endif
