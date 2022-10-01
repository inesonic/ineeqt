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
* This header defines the \ref EQt::TabWidget class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_TAB_WIDGET_H
#define EQT_TAB_WIDGET_H

#include <QPoint>
#include <QTabWidget>

class QMenu;
class QWidget;

#include "eqt_common.h"

namespace EQt {
    /**
     * Class that extends QTabWidget to support pop-up menus on the tabs.
     */
    class EQT_PUBLIC_API TabWidget:public QTabWidget {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            TabWidget(QWidget* parent = Q_NULLPTR);

            ~TabWidget() override;

        signals:
            /**
             * Signal that is emitted when a tab bar custom context menu is requested.
             *
             * \param[out] position The global position where the user clicked.
             *
             * \param[out] index    The index of the tab on which the user clicked.
             */
            void customContextMenuRequested(const QPoint& position, int index);

        private slots:
            /**
             * Slot that is triggered when the user right clicks somewhere on the tab bar.
             *
             * \param position The position relative to the tool bar where the user clicked.  The location is used to
             *                 identify the appropriate tab and to position the context menu.
             */
            void forwardCustomContextMenuRequested(const QPoint& position);
    };
}

#endif
