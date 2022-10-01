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
* This header defines the \ref EQt::ColorToolButton class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_COLOR_TOOL_BUTTON_H
#define EQT_COLOR_TOOL_BUTTON_H

#include <QSet>
#include <QColor>

#include <util_hash_functions.h>

#include "eqt_common.h"
#include "eqt_tool_button.h"

class QMenu;
class QWidget;
class QAction;
class QPainter;

namespace EQt {
    /**
     * Class that extends EQt::ToolButton to include a glyph showing either a single color or a range of colors.
     */
    class EQT_PUBLIC_API ColorToolButton:public ToolButton {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this tool button.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(int newRelativeSizeFactor, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] action                The action to link to this tool editor.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(int newRelativeSizeFactor, QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] menu   A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this tool button.
             *
             * \param[in] menu   A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(QAction* action, QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] menu                  A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(int newRelativeSizeFactor, QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.  for details, see the
             *                                  \ref ColorToolButton::setRelativeSizeFactor method.
             *
             * \param[in] action                The action to link to this tool editor.
             *
             * \param[in] menu                  A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(int newRelativeSizeFactor, QAction* action, QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] colors The list of colors to be displayed.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(const QSet<QColor>& colors, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] colors The list of colors to be displayed.
             *
             * \param[in] action The action to link to this tool button.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(const QSet<QColor>& colors, QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] colors                The list of colors to be displayed.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(const QSet<QColor>& colors, int newRelativeSizeFactor, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] colors                The list of colors to be displayed.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] action                The action to link to this tool editor.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(
                const QSet<QColor>& colors,
                int                 newRelativeSizeFactor,
                QAction*            action,
                QWidget*            parent = Q_NULLPTR
            );

            /**
             * Constructor.
             *
             * \param[in] colors The list of colors to be displayed.
             *
             * \param[in] menu   A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(const QSet<QColor>& colors, QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] colors The list of colors to be displayed.
             *
             * \param[in] action The action to link to this tool button.
             *
             * \param[in] menu   A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(const QSet<QColor>& colors, QAction* action, QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] colors                The list of colors to be displayed.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] menu                  A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(
                const QSet<QColor>& colors,
                int                 newRelativeSizeFactor,
                QMenu*              menu,
                QWidget*            parent = Q_NULLPTR
            );

            /**
             * Constructor.
             *
             * \param[in] colors                The list of colors to be displayed.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.  for details, see the
             *                                  \ref ColorToolButton::setRelativeSizeFactor method.
             *
             * \param[in] action                The action to link to this tool editor.
             *
             * \param[in] menu                  A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(
                const QSet<QColor>& colors,
                int                 newRelativeSizeFactor,
                QAction*            action,
                QMenu*              menu,
                QWidget*            parent = Q_NULLPTR
            );

            /**
             * Constructor.
             *
             * \param[in] color  The color to be displayed.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(const QColor& color, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] color  The color to be displayed.
             *
             * \param[in] action The action to link to this tool button.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(const QColor& color, QAction* action, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] color                 The color to be displayed.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(const QColor& color, int newRelativeSizeFactor, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] color                 The color to be displayed.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] action                The action to link to this tool editor.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(
                const QColor& color,
                int                  newRelativeSizeFactor,
                QAction*             action,
                QWidget*             parent = Q_NULLPTR
            );

            /**
             * Constructor.
             *
             * \param[in] color  The color to be displayed.
             *
             * \param[in] menu   A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(const QColor& color, QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] color  The color to be displayed.
             *
             * \param[in] action The action to link to this tool button.
             *
             * \param[in] menu   A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent The parent object for this tool button.
             */
            ColorToolButton(const QColor& color, QAction* action, QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] color                 The color to be displayed.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.
             *
             * \param[in] menu                  A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(const QColor& color, int newRelativeSizeFactor, QMenu* menu, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] color                 The color to be displayed.
             *
             * \param[in] newRelativeSizeFactor The relative size factor to apply to the button.  for details, see the
             *                                  \ref ColorToolButton::setRelativeSizeFactor method.
             *
             * \param[in] action                The action to link to this tool editor.
             *
             * \param[in] menu                  A menu that should pop-up when the button is pressed.
             *
             * \param[in] parent                The parent object for this window.
             */
            ColorToolButton(
                const QColor& color,
                int           newRelativeSizeFactor,
                QAction*      action,
                QMenu*        menu,
                QWidget*      parent = Q_NULLPTR
            );

            ~ColorToolButton() override;

            /**
             * Method you can use to obtain the list of colors currently displayed by the tool button.
             *
             * \return Returns a list of every color displayed by the tool button.
             */
            const QSet<QColor>& colors() const;

        public slots:
            /**
             * Method you can call to set the tool button to a single solid color.
             *
             * \param[in] newColor The new tool button color.
             */
            void setColor(const QColor& newColor);

            /**
             * Method you can call to set the tool button to show a collection of colors.
             *
             * \param[in] newColors The new list of colors to be shown.  Each color will be shown as a vertical bar
             *                      across the tool button.
             */
            void setColors(const QSet<QColor>& newColors);

        protected:
            /**
             * Method that is called when the tool button is resized.
             *
             * \param[in] event The event that triggered this call.
             */
            void resizeEvent(QResizeEvent* event) override;

        private:
            /**
             * Method that regenerates the displayed icon.  This method is called whenever the button is resized or
             * whenever the button colors are updated.
             */
            void updateIcon();

            /**
             * Method that is called to draw a color patch.
             *
             * \param[in] painter   Painter used to draw the color.
             *
             * \param[in] rectangle The rectangle representing the area to be filled in.
             *
             * \param[in] color     The color to be drawn.
             */
            static void drawColorPatch(QPainter* painter, const QRect& rectangle, const QColor& color);

            /**
             * The current list of displayed colors.
             */
            QSet<QColor> currentColors;
    };
}

#endif
