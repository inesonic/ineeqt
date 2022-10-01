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
* This header defines the \ref EQt::GraphicsMultiTextGroup class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_GRAPHICS_MULTI_TEXT_GROUP_H
#define EQT_GRAPHICS_MULTI_TEXT_GROUP_H

#include <QGraphicsItemGroup>
#include <QString>
#include <QList>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QPointF>
#include <QRectF>

#include "eqt_common.h"
#include <eqt_graphics_item_group.h>

class QWidget;
class QAction;
class QFocusEvent;

namespace EQt {
    /**
     * Class that extends the EQt:GraphicsItemGroup to include support for an optional background, border, and optional
     * text regions.
     */
    class EQT_PUBLIC_API GraphicsMultiTextGroup:public GraphicsItemGroup {
        public:
            /**
             * The QGraphicsItem type ID.
             */
            enum { Type = QGraphicsItem::UserType + 7 };

            /**
             * Class that is used to track a text entry.
             */
            class EQT_PUBLIC_API TextEntry {
                friend class GraphicsMultiTextGroup;

                public:
                    TextEntry();

                    /**
                     * Constructor
                     *
                     * \param[in] text     The text.
                     *
                     * \param[in] font     The font to apply to the text.
                     *
                     * \param[in] position The position where the text should be placed.  The position represents
                     *                     the left edge of the baseline of the text.
                     */
                    TextEntry(const QString& text, const QFont& font, const QPointF& position);

                    /**
                     * Constructor
                     *
                     * \param[in] text The text.
                     *
                     * \param[in] font The font to apply to the text.
                     *
                     * \param[in] x    The X offset.
                     *
                     * \param[in] y    The Y offset.  The Y offset represents the baseline of the text.
                     */
                    TextEntry(const QString& text, const QFont& font, double x, double y);

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to be copied.
                     */
                    TextEntry(const TextEntry& other);

                    ~TextEntry();

                    /**
                     * Method you can use to set the text to be displayed.
                     *
                     * \param[in] newText The new text.
                     */
                    void setText(const QString& newText);

                    /**
                     * Method you can use to obtain the displayed text.
                     *
                     * \return Returns the displayed text.
                     */
                    const QString& text() const;

                    /**
                     * Method you can use to set the text font.
                     *
                     * \param[in] newFont The new text font.
                     */
                    void setFont(const QFont& newFont);

                    /**
                     * Method you can use to obtain the text font.
                     *
                     * \return Returns the text font.
                     */
                    const QFont& font() const;

                    /**
                     * Method you can use to set the position for the text within this group.
                     *
                     * \param[in] newPosition The new position.  The position represents the left edge of the baseline
                     *                        of the text.
                     */
                    void setPosition(const QPointF& newPosition);

                    /**
                     * Method you can use to set the position for the text within this group.
                     *
                     * \param[in] x The X offset.
                     *
                     * \param[in] y The Y offset.  The Y offset represents the baseline of the text.
                     */
                    void setPosition(double x, double y);

                    /**
                     * Method you can use to obtain the position within the group.
                     *
                     * \return Returns the position within the group.  The returned location is the left edge of the
                     *         baseline of the text.
                     */
                    const QPointF& position() const;

                    /**
                     * Assignment operator
                     *
                     * \param[in] other The instance to be copied.
                     *
                     * \return A reference to this object.
                     */
                    TextEntry& operator=(const TextEntry& other);

                private:
                    /**
                     * Method that is called to trigger updates.
                     */
                    void triggerUpdates();

                    /**
                     * The current text.
                     */
                    QString currentText;

                    /**
                     * The current font.
                     */
                    QFont currentFont;

                    /**
                     * The current position.
                     */
                    QPointF currentPosition;

                    /**
                     * Pointer to the group.
                     */
                    GraphicsMultiTextGroup* itemGroup;
            };

            /**
             * Iterator.
             */
            typedef QList<TextEntry>::iterator Iterator;

            /**
             * Constant iterator.
             */
            typedef QList<TextEntry>::const_iterator ConstIterator;

            GraphicsMultiTextGroup();

            ~GraphicsMultiTextGroup() override;

            /**
             * Method you can use to defer updates.  The default implementation does nothing.
             */
            void deferUpdates() override;

            /**
             * Method you can use to restore updates.  The default implementation does nothing.
             */
            void restoreUpdates() override;

            /**
             * Method you can use to determine if updates are deferred.
             *
             * \return Returns true if updates are currently deferred.  Returns false if updates are not deferred.  The
             *         default implementation returns false.
             */
            bool updatesAreDeferred() const override;

            /**
             * Method that returns the type ID of this QGraphicsItem.
             *
             * \return Returns the type ID of this QGraphicsItem.
             */
            int type() const override;

            /**
             * Method you can use to set the background brush.
             *
             * \param[in] newBrush The new background brush.
             */
            void setBackgroundBrush(const QBrush& newBrush);

            /**
             * Method you can use to obtain the background brush.
             *
             * \return Returns the background brush.
             */
            const QBrush& backgroundBrush() const;

            /**
             * Method you can use to set the border pen.
             *
             * \param[in] newPen The new pen to use for the border.
             */
            void setBorderPen(const QPen& newPen);

            /**
             * Method you can use to obtain the current border pen.
             *
             * \return Returns the current border pen.
             */
            const QPen& borderPen() const;

            /**
             * Method you can use to set the text pen.
             *
             * \param[in] newPen The new pen to use when drawing fonts.
             */
            void setTextPen(const QPen& newPen);

            /**
             * Method you can use to obtain the current text pen.
             *
             * \return Returns the current text pen.
             */
            QPen textPen() const;

            /**
             * Method that removes all text entries.
             */
            void clearText();

            /**
             * Method that removes the first text entry.
             */
            void removeFirst();

            /**
             * Method that removes the last text entry.
             */
            void removeLast();

            /**
             * Method that removes a specific entry, by index.
             *
             * \param[in] index The zero based index of the entry to be removed.
             */
            void removeAt(unsigned index);

            /**
             * Method that trims the entries from the tail of the list of entries.
             *
             * \param[in] newNumberEntries The new number of entries.  The graphics item will not be modified if the
             *                             provided value is greater than or equal to the current number of entries.
             */
            void setNumberEntries(unsigned newNumberEntries);

            /**
             * Method you can use to to append a new text region.
             *
             * \param[in] newEntry The new text entry.
             */
            void append(const TextEntry& newEntry);

            /**
             * Convenience method you can use to to append a new text region.
             *
             * \param[in] newText     The new text.
             *
             * \param[in] newFont     The new font.
             *
             * \param[in] newPosition The new position.  The position represents the left edge of the baseline of the
             *                        text.
             */
            void append(const QString& newText, const QFont& newFont, const QPointF& newPosition);

            /**
             * Method you can use to prepend a new text entry.
             *
             * \param[in] newEntry The new text entry.
             */
            void prepend(const TextEntry& newEntry);

            /**
             * Convenience method you can use to to prepend a new text region.
             *
             * \param[in] newText     The new text.
             *
             * \param[in] newFont     The new font.
             *
             * \param[in] newPosition The new position.
             */
            void prepend(const QString& newText, const QFont& newFont, const QPointF& newPosition);

            /**
             * Method you can use to determine the number of text entries.
             *
             * \return Returns the number of text entries.
             */
            unsigned numberTextEntries() const;

            /**
             * Method you can use to obtain a text entry.
             *
             * \param[in] index The zero based index of the desired text entry.
             *
             * \return Returns a reference to the text entry.
             */
            const TextEntry& entry(unsigned index) const;

            /**
             * Method you can use to obtain a text entry.
             *
             * \param[in] index The zero based index of the desired text entry.
             *
             * \return Returns a reference to the text entry.
             */
            TextEntry& entry(unsigned index);

            /**
             * Array operator.
             *
             * \param[in] index The zero based index of the desired text entry.
             *
             * \return Returns a reference to the text entry.
             */
            const TextEntry& operator[](unsigned index) const;

            /**
             * Method you can use to obtain a text entry.
             *
             * \param[in] index The zero based index of the desired text entry.
             *
             * \return Returns a reference to the text entry.
             */
            TextEntry& operator[](unsigned index);

            /**
             * Method that returns an iterator to the first entry in the list.
             *
             * \return Returns an iterator to the first text entry.
             */
            Iterator begin();

            /**
             * Method that returns an iterator past the last entry in the list.
             *
             * \return Returns an iterator past the last entry in the list.
             */
            Iterator end();

            /**
             * Method that returns a constant iterator to the first entry in the list.
             *
             * \return Returns a constant iterator to the first entry in the list.
             */
            ConstIterator constBegin() const;

            /**
             * Method that returns a constant iterator past the last entry in the list.
             *
             * \return Returns a constant iterator past the last entry in the list.
             */
            ConstIterator constEnd();

            /**
             * Method that returns this item's bounding rectangle.
             *
             * \return Returns this item's bounding rectangle.
             */
            QRectF boundingRect() const override;

            /**
             * Method that is called to paint the object.
             *
             * \param[in] painter Painter used to draw the text.
             *
             * \param[in] option  Options that can be used to impact how the text is drawn.  This parameter is ignored.
             *
             * \param[in] widget  Widget to be drawn on.
             */
            void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

        private:
            /**
             * Method that updates the geometry data.
             */
            void updateGeometryNeeded();

            /**
             * Flag that indicates if updates are currently deferred.
             */
            bool currentUpdatesAreDeferred;

            /**
             * The current background brush
             */
            QBrush currentBackgroundBrush;

            /**
             * The current border pen.
             */
            QPen currentBorderPen;

            /**
             * The current text pen.
             */
            QPen currentTextPen;

            /**
             * The current size.
             */
            mutable QRectF currentTextBoundingRectangle;

            /**
             * List of text entries.
             */
            QList<TextEntry> currentTextEntries;
    };
}

#endif
