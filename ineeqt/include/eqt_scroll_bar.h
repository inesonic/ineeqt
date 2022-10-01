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
* This header defines the \ref EQt::ScrollBar class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_SCROLL_BAR_H
#define EQT_SCROLL_BAR_H

#include <QScrollBar>
#include <QSize>

class QWidget;
class QResizeEvent;
class QHideEvent;
class QShowEvent;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that extends the QScrollBar class to include several additional signals.
     */
    class EQT_PUBLIC_API ScrollBar:public QScrollBar {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this scrollbar.
             */
            explicit ScrollBar(QWidget* parent);

            /**
             * Constructor.
             *
             * \param[in] orientation The orientation to assign to the scrollbar.
             *
             * \param[in] parent      The parent object for this scrollbar.
             */
            explicit ScrollBar(Qt::Orientation orientation, QWidget* parent = Q_NULLPTR);

            ~ScrollBar();

        signals:
            /**
             * Signal that is emitted when the scrollbar is resized.  The signal can be used to allow
             * other widgets to fine tune their size based on the scrollbar's current size.  Note that
             * care must be taken to avoid infinite recursion due to resize events.
             *
             * \param[out] newSize The new scrollbar size, in pixels.
             */
            void resized(const QSize& newSize);

            /**
             * Signal that is emitted when the scrollbar is made visible.  The signal can be used to allow other
             * widgets to change their visibility based on the state of this widget.
             */
            void shown();

            /**
             * Signal that is emitted when the scrollbar is hidden.  The signal can be used to allow other widgets to
             * change their visibility based on the state of this widget.
             */
            void hidden();

        protected:
            /**
             * Method that is called when the scrollbar is resized.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void resizeEvent(QResizeEvent* event) override;

            /**
             * Method that is called when the scrollbar is hidden.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void hideEvent(QHideEvent* event) override;

            /**
             * Method that is called when the scrollbar is shown.
             *
             * \param[in] event The event that triggered this call.
             */
            void showEvent(QShowEvent* event) override;
    };
}

#endif
