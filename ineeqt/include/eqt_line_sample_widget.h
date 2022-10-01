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
* This header defines the \ref EQt::LineSampleWidget class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PARAGRAPH_DIAGRAM_H
#define EQT_PARAGRAPH_DIAGRAM_H

#include <QWidget>
#include <QSize>

#include "eqt_common.h"

class QPaintEvent;

namespace EQt {
    /**
     * Class that provides a trivial widget you can use to display various line styles.
     */
    class EQT_PUBLIC_API LineSampleWidget:public QWidget {
        Q_OBJECT

        public:
            /**
             * Enumeration of supported orientations.
             */
            enum class Orientation {
                /**
                 * Horizontal line.
                 */
                HORIZONTAL,

                /**
                 * Vertical line.
                 */
                VERTICAL
            };

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent widget.
             */
            LineSampleWidget(QWidget* parent = Q_NULLPTR);

            ~LineSampleWidget() override;

            /**
             * Method you can use to obtain the line orientation.
             *
             * \return Returns the line orientation.
             */
            Orientation orientation() const;

            /**
             * Method you can use to obtain the number of adjacent lines.
             *
             * \return Returns the number of adjacent lines to show.
             */
            unsigned numberLines() const;

            /**
             * Method you can use to obtain the line width.
             *
             * \return Returns the line width.
             */
            float lineWidth() const;

            /**
             * Method you can use to determine the line color.
             *
             * \return Returns the line color.
             */
            QColor lineColor() const;

            /**
             * Method you can use to determine the background color.
             *
             * \return Returns the background color.
             */
            QColor backgroundColor() const;

        public slots:
            /**
             * Slot you can use to set the size of the line sample widget.
             *
             * \param[in] width  The desired width of the widget, in points.
             *
             * \param[in] height The desired height of the widget, in points.
             */
            void setSize(unsigned width, unsigned height);

            /**
             * Slot you can use to set the size of the line sample widget.
             *
             * \param[in] newSize The new widget size.
             */
            void setSize(const QSize& newSize);

            /**
             * Slot you can use to set the line orientation.
             *
             * \param[in] newOrientation The line orientation.
             */
            void setOrientation(Orientation newOrientation);

            /**
             * Slot you can use to set the number of adjacent lines.
             *
             * \param[in] newNumberLines The new number of adjacent lines.
             */
            void setNumberLines(unsigned newNumberLines);

            /**
             * Slot you can use to set the line width settings.
             *
             * \param[in] newLineWidth The new line width setting.
             */
            void setLineWidth(float newLineWidth);

            /**
             * Slot you can use to set the new line color.
             *
             * \param[in] newLineColor The new line color.
             */
            void setLineColor(const QColor& newLineColor);

            /**
             * Slot you can use to set the new background color.
             *
             * \param[in] newBackgroundColor The new background color.
             */
            void setBackgroundColor(const QColor& newBackgroundColor);

        protected:
            /**
             * Method that is called when the widget needs to be repainted.
             *
             * \param[in] event The paint event that triggered this call.
             */
            void paintEvent(QPaintEvent* event) override;

        private:
            /**
             * The current line orientation.
             */
            Orientation currentOrientation;

            /**
             * The current number of adjacent lines.
             */
            unsigned currentNumberLines;

            /**
             * The current line width.
             */
            float currentLineWidth;
    };
}

#endif
