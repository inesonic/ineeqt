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

#ifndef EQT_FONT_BUTTON_H
#define EQT_FONT_BUTTON_H

#include <QString>
#include <QFont>
#include <QColor>
#include <QPushButton>

#include "eqt_common.h"

class QWidget;

namespace EQt {
    /**
     * Class that extends QPushButton to support displaying A glyph within a button showing a selected font.
     */
    class EQT_PUBLIC_API FontButton:public QPushButton {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this tool button.
             */
            FontButton(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] glyphText Sample text to display within the glyph.
             *
             * \param[in] text      The descriptive text to display in the button.
             *
             * \param[in] parent    The parent object for this tool button.
             */
            FontButton(const QString& glyphText, const QString& text, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] glyphText           Sample text to display within the glyph.
             *
             * \param[in] font                The sample font for the glyph text.
             *
             * \param[in] text                The descriptive text to display in the button.
             *
             * \param[in] parent              The parent object for this tool button.
             */
            FontButton(const QString& glyphText, const QFont& font, const QString& text, QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] glyphText           Sample text to display within the glyph.
             *
             * \param[in] font                The sample font for the glyph text.
             *
             * \param[in] fontColor           The font color.
             *
             * \param[in] text                The descriptive text to display in the button.
             *
             * \param[in] parent              The parent object for this tool button.
             */
            FontButton(
                const QString& glyphText,
                const QFont&   font,
                const QColor&  fontColor,
                const QString& text,
                QWidget*       parent = Q_NULLPTR
            );

            /**
             * Constructor.
             *
             * \param[in] glyphText           Sample text to display within the glyph.
             *
             * \param[in] font                The sample font for the glyph text.
             *
             * \param[in] fontColor           The font color.
             *
             * \param[in] fontBackgroundColor The glyph background color.
             *
             * \param[in] text                The descriptive text to display in the button.
             *
             * \param[in] parent              The parent object for this tool button.
             */
            FontButton(
                const QString& glyphText,
                const QFont&   font,
                const QColor&  fontColor,
                const QColor&  fontBackgroundColor,
                const QString& text,
                QWidget*       parent = Q_NULLPTR
            );

            /**
             * Method you can use to determine the current glyph text.
             *
             * \return Returns the current glyph text.
             */
            QString glyphText() const;

            /**
             * Method you can use to determine the current description text.
             *
             * \return Returns the current description text.
             */
            QString descriptionText() const;

            /**
             * Method you can use to determine the current glyph font.
             *
             * \return Returns the current glyph font.
             */
            QFont glyphFont() const;

            /**
             * Method you can use to determine the current description font.
             *
             * \return Returns the current description font.
             */
            QFont descriptionFont() const;

            /**
             * Method you can use to determine the current glyph foreground color.
             *
             * \return The glyph text foreground color.
             */
            QColor fontColor() const;

            /**
             * Method you can use to determine the current glyph background color.
             *
             * \return The glyph background color.
             */
            QColor fontBackgroundColor() const;

        public slots:
            /**
             * Slot you can use to change the glyph text.
             *
             * \param[in] newText The new glyph text.
             */
            void setGlyphText(const QString& newText);

            /**
             * Slot you can use to change the glyph font.
             *
             * \param[in] newFont The new glyph font.
             */
            void setGlyphFont(const QFont& newFont);

            /**
             * Slot you can use to change the description font.
             *
             * \param[in] newFont The new description font.
             */
            void setDescriptionFont(const QFont& newFont);

            /**
             * Slot you can use to change the glyph foreground color.
             *
             * \param[in] newColor The new foreground color.
             */
            void setFontColor(const QColor& newColor);

            /**
             * Slot you can use to change the glyph background color.
             *
             * \param[in] newColor The new background color.
             */
            void setFontBackgroundColor(const QColor& newColor);

            /**
             * Slot you can use to change the description text.
             *
             * \param[in] newText The new description text.
             */
            void setDescriptionText(const QString& newText);

        private:
            /**
             * Method that updates the button glyph.
             */
            void updateGlyph();

            /**
             * The current glyph text.
             */
            QString currentGlyphText;

            /**
             * The current glyph font.
             */
            QFont currentGlyphFont;

            /**
             * The current glyph foreground color.
             */
            QColor currentForegroundColor;

            /**
             * The current glyph background color.
             */
            QColor currentBackgroundColor;
    };
}

#endif
