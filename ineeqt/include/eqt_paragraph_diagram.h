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
* This header defines the \ref EQt::ParagraphDiagram class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PARAGRAPH_DIAGRAM_H
#define EQT_PARAGRAPH_DIAGRAM_H

#include <QWidget>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QLine>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QList>
#include <QMultiMap>

#include "eqt_common.h"

class QPaintEvent;
class QResizeEvent;
class QTimer;

namespace EQt {
    /**
     * Class that provides a trivial widget you can use to display a diagram showing a pargraph, with various
     * indentation values called out.
     */
    class EQT_PUBLIC_API ParagraphDiagram:public QWidget {
        Q_OBJECT

        public:
            /**
             * Enumeration of supported justification settings.
             */
            enum class Justification {
                /**
                 * No justification mode has been set.
                 */
                NONE = 0,

                /**
                 * Contents are left justified.
                 */
                LEFT = 1,

                /**
                 * Contents are right justified.
                 */
                RIGHT = 2,

                /**
                 * Contents are centered in the block.
                 */
                CENTER = 4,

                /**
                 * Contents are aligned to each edge by stretching or similar operation.
                 */
                JUSTIFY = 8
            };

            /**
             * The default color used for the widget background area.
             */
            static const QColor defaultBackgroundColor;

            /**
             * Default color used for the margins.
             */
            static const QColor defaultMarginColor;

            /**
             * Default color used for the active region.
             */
            static const QColor defaultPageContentsBackgroundColor;

            /**
             * Default color used for the widget area border.
             */
            static const QColor defaultWidgetBorderLineColor;

            /**
             * Default color used to draw border lines around the page extents.
             */
            static const QColor defaultMarginsBorderLineColor;

            /**
             * Default color used to draw border lines around the page extents.
             */
            static const QColor defaultExtentsBorderLineColor;

            /**
             * Default color used to draw annotation lines and arrows.
             */
            static const QColor defaultAnnotationColor;

            /**
             * Default color used to draw text line block fill.
             */
            static const QColor defaultTextLineFillColor;

            /**
             * Default color used to draw text line borders.
             */
            static const QColor defaultTextLineBorderColor;

            /**
             * Default color used to draw the bullet block fill.
             */
            static const QColor defaultBulletFillColor;

            /**
             * Default color used to draw borders around the bullet block.
             */
            static const QColor defaultBulletBorderLineColor;

            /**
             * Default color used to draw text margin and center line indicators.
             */
            static const QColor defaultTextMarginIndicatorColor;

            /**
             * Default width of the pen used to draw the widget border.
             */
            static const float defaultWidgetBorderLineWidth;

            /**
             * Default width of the pen used to draw the inner page margin border.
             */
            static const float defaultMarginBorderLineWidth;

            /**
             * Default width of the pen used to draw the outer page margin border.
             */
            static const float defaultExtentsBorderLineWidth;

            /**
             * Default width of lines used for annotations.
             */
            static const float defaultAnnotationLineWidth;

            /**
             * Default width of text borders.  This value is also used for the bullet border line.
             */
            static const float defaultTextLineBorderWidth;

            /**
             * Default width of the text margin indicator line.
             */
            static const float defaultTextMarginIndicatorWidth;

            /**
             * The default page width.
             */
            static const float defaultPageWidth;

            /**
             * The default page left margin.
             */
            static const float defaultPageLeftMargin;

            /**
             * The default page right margin.
             */
            static const float defaultPageRightMargin;

            /**
             * Default text lines per paragraph.
             */
            static const unsigned defaultTextLinesPerParagraph;

            /**
             * Default text line fill area height.
             */
            static const float defaultTextLineHeight;

            /**
             * The default justification mode.
             */
            static const Justification defaultJustification;

            /**
             * The default left indentation value.
             */
            static const float defaultLeftIndentation;

            /**
             * The default right indentation value.
             */
            static const float defaultRightIndentation;

            /**
             * The default top spacing value.
             */
            static const float defaultTopSpacing;

            /**
             * The default bottom spacing value.
             */
            static const float defaultBottomSpacing;

            /**
             * The default first line left indentation value.
             */
            static const float defaultFirstLineIndentation;

            /**
             * The default list indentation value.
             */
            static const float defaultListIndentation;

            /**
             * The default flag indicating if list settings should be displayed.
             */
            static const bool defaultShowList;

            /**
             * The default flag indicating if lists should display a bullet/number.
             */
            static const bool defaultShowBullet;

            /**
             * The defaultly displayed line spacing.
             */
            static const float defaultLineSpacing;

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent widget.
             */
            ParagraphDiagram(QWidget* parent = Q_NULLPTR);

            ~ParagraphDiagram() override;

            /**
             * Method you can use to determine the maximum allowed height.
             *
             * \return Returns the maximum allowed height for the widget.
             */
            unsigned maximumHeight() const;

            /**
             * Method you can use to obtain the current page width.
             *
             * \return Returns the page width value.  All other values are scaled based on this value.
             */
            float pageWidth() const;

            /**
             * Method you can use to obtain the current page left margin.
             *
             * \return Returns the page left margin value.
             */
            float pageLeftMargin() const;

            /**
             * Method you can use to obtain the current page right margin.
             *
             * \return Returns the page right margin value.
             */
            float pageRightMargin() const;

            /**
             * Method you can use to determine the current number of text lines rendered per paragraph.
             *
             * \return Returns the number of text lines rendered per paragraph
             */
            unsigned numberTextLinesPerParagraph() const;

            /**
             * Method you can use to determine the font height to assume per text line.
             *
             * \return Returns the font height to assume per text line.
             */
            float textLineHeight() const;

            /**
             * Method you can use to obtain the current justification.
             *
             * \return Returns the current justification mode.
             */
            Justification justification() const;

            /**
             * Method you can use to obtain the current left indentation value.
             *
             * \return Returns the page left indentation value.
             */
            float leftIndentation() const;

            /**
             * Method you can use to obtain the current right indentation value.
             *
             * \return Returns the page right indentation value.
             */
            float rightIndentation() const;

            /**
             * Method you can use to obtain the current top spacing value.
             *
             * \return Returns the page top spacing.
             */
            float topSpacing() const;

            /**
             * Method you can use to obtain the current bottom spacing value.
             *
             * \return Returns the page bottom spacing.
             */
            float bottomSpacing() const;

            /**
             * Method you can use to obtain the current first line left indentation value.  This value is added to the
             * left indentation to determine the left edge of the first line of the center paragraph.
             *
             * \return Returns the first line left indentation value.
             */
            float firstLineIndentation() const;

            /**
             * Method you can use to obtain the current list indentation value.  This value is added to the left
             * indentation value to determine the indentation of all lines in the central paragraph.  The value is also
             * added, with the first line indentation, to the left indentation on the first line of the center
             * paragraph.  The value is only used if list parameters are shown.
             *
             * \return Returns the current list indentation value.
             */
            float listIndentation() const;

            /**
             * Method you can use to determine if list parameters are currently being displayed.
             *
             * \return Returns true if list parameters are currently being displayed.
             */
            bool showList() const;

            /**
             * Method you can use to determine if a bullet should be displayed in the list.
             *
             * \return Returns true if a bullet should be displayed as part of the list.
             */
            bool showBullet() const;

            /**
             * Method you can use to obtain the currently displayed line spacing.
             *
             * \return Returns the current line spacing.  A value of 1.0 indicates single space, a value of 1.5
             *         indicates 1 1/2 line spacing.  A value of 2.0 indicates double spacing.
             */
            float lineSpacing() const;

            /**
             * Method you can use to obtain the location of the left side indentation call-out.
             *
             * \return Returns the location of the left side indentation call-out.
             */
            QPoint leftIndentationCalloutLocation() const;

            /**
             * Method you can use to obtain the location of the right side indentation call-out.
             *
             * \return Returns the location of the right side indentation call-out.
             */
            QPoint rightIndentationCalloutLocation() const;

            /**
             * Method you can use to obtain the location of the top spacing call-out.
             *
             * \return Returns the top spacing call-out location.
             */
            QPoint topSpacingCalloutLocation() const;

            /**
             * Method you can use to obtain the location of the bottom spacing call-out.
             *
             * \return Returns the bottom spacing call-out location.
             */
            QPoint bottomSpacingCalloutLocation() const;

            /**
             * Method you can use to obtain the location of the first line left indentation call-out.
             *
             * \return Returns the first line left indentation call-out location.
             */
            QPoint firstLineIdentationCalloutLocation() const;

            /**
             * Method you can use to obtain the call-out location for the list indentation.
             *
             * \return Returns the list indentation call-out location.
             */
            QPoint listIndentationCalloutLocation() const;

            /**
             * Method you can use to obtain the location of the line spacing call-out.
             *
             * \return Returns the location of the line spacing call-out.
             */
            QPoint lineSpacingCalloutLocation() const;

            /**
             * Method you can use to obtain the brush used to render the unused portions of the widget background.
             *
             * \return Returns the brush currently used to render the unused portions of the widget background.
             */
            QBrush backgroundBrush() const;

            /**
             * Method you can use to obtain the current brush used for the background of the page contents.
             *
             * \return Returns the brush used to render the page active area background.
             */
            QBrush pageContentsBackgroundBrush() const;

            /**
             * Method you can use to determine the brush used for the margins.
             *
             * \return Returns the brush used to render the page margins.
             */
            QBrush marginsBrush() const;

            /**
             * Method you can use to obtain the pen used to draw the widget border.
             */
            QPen widgetBorderLinePen() const;

            /**
             * Method you can use to determine the pen used to draw border lines for the page margins.
             *
             * \return Returns the pen used to draw border lines.
             */
            QPen marginBorderLinePen() const;

            /**
             * Method you can use to determine the pen used to draw border lines for the page extents.
             *
             * \return Returns the pen used to draw borders around the page extents.
             */
            QPen extentsBorderLinePen() const;

            /**
             * Method you can use to obtain the annotation brush used to draw annotations.
             */
            QBrush annotationBrush() const;

            /**
             * Method you can use to obtain the annotation pen used to draw annotations.
             */
            QPen annotationPen() const;

            /**
             * Method you can use to obtain the current text line fill brush used to draw text lines.
             */
            QBrush textLineFillBrush() const;

            /**
             * Method you can use to obtain the current text line border pen used to draw borders on each text line.
             */
            QPen textLineBorderPen() const;

            /**
             * Method you can use to determine the pen used to draw indicators for text margins and center lines.
             */
            QPen textMarginIndicatorPen() const;

            /**
             * Method you can use to obtain the current brush used to fill in the bullet/number block.
             */
            QBrush bulletFillBrush() const;

            /**
             * Method you can use to obtain the current pen used to draw a border around the bullet/number block.
             */
            QPen bulletBorderPen() const;

            /**
             * Method that returns the minimum size hint for this widget.
             *
             * \return Returns the minimum size hint for this widget.
             */
            QSize minimumSizeHint() const override;

            /**
             * Method that returns the size hint for this widget.
             *
             * \return Returns the size int for this widget.
             */
            QSize sizeHint() const override;

        signals:
            /**
             * Signal that is emitted when the callout locations change.
             */
            void calloutLocationsChanged();

        public slots:
            /**
             * Slot you can use to set the new maximum height for the widget.
             *
             * \param[in] newMaximumHeight The new maximum height for the widget, in pixels.
             */
            void setMaximumHeight(unsigned newMaximumHeight);

            /**
             * Slot you can use to set the new page width.
             *
             * \param[in] newPageWidth The new page width value.  All other values will be relative to this value.
             */
            void setPageWidth(float newPageWidth);

            /**
             * Slot you can use to set the new page left margin.
             *
             * \param[in] newPageLeftMargin The new page left margin value.
             */
            void setPageLeftMargin(float newPageLeftMargin);

            /**
             * Slot you can use to set the new page right margin.
             *
             * \param[in] newPageRightMargin The new page right margin value.
             */
            void setPageRightMargin(float newPageRightMargin);

            /**
             * Slot you can use to set the number of text lines per paragraph.
             *
             * \param[in] newNumberTextLinesPerParagraph The new number of text lines per paragraph.
             */
            void setNumberTextLinesPerParagraph(unsigned newNumberTextLinesPerParagraph);

            /**
             * Slot you can use to set the font height to assumer per text line.
             *
             * \return Returns the font height to assume per text line.
             */
            void setTextLineHeight(float newTextLineHeight);

            /**
             * Slot you can use to set the justification.
             *
             * \param[in] newJustification The new justification mode.
             */
            void setJustification(Justification newJustification);

            /**
             * Slot you can use to set the justification to left justified.
             */
            void setLeftJustified();

            /**
             * Slot you can use to set the justification to centered.
             */
            void setCentered();

            /**
             * Slot you can use to set the justification to right justified.
             */
            void setRightJustified();

            /**
             * Slot you can use to set the justification to fully justified.
             */
            void setJustified();

            /**
             * Slot you can use to set the new left indentation.
             *
             * \param[in] newLeftIndentation The new left indentation value.
             */
            void setLeftIndentation(float newLeftIndentation);

            /**
             * Slot you can use to set the new right indentation.
             *
             * \param[in] newRightIndentation The new right indentation value.
             */
            void setRightIndentation(float newRightIndentation);

            /**
             * Slot you can use to set the new top spacing.
             *
             * \param[in] newTopSpacing The top spacing value.
             */
            void setTopSpacing(float newTopSpacing);

            /**
             * Slot you can use to set the new bottom spacing.
             *
             * \param[in] newBottomSpacing The bottom spacing value.
             */
            void setBottomSpacing(float newBottomSpacing);

            /**
             * Slot you can use to set the first line left indentation value.
             *
             * \param[in] newFirstLineIndentation The new first line indentation value.  This value is added to the
             * left indentation to determine the left edge of the first line of the center paragraph.
             */
            void setFirstLineIndentation(float newFirstLineIndentation);

            /**
             * Slot you can use to set the list indentation value.  This value is added to the left
             * indentation value to determine the indentation of all lines in the central paragraph.  The value is also
             * added, with the first line indentation, to the left indentation on the first line of the center
             * paragraph.  The value is only used if list parameters are shown.
             *
             * \param[in] newListIndentation The new list indentation value.
             */
            void setListIndentation(float newListIndentation);

            /**
             * Slot you can use to indicate that list parameters should be shown.
             *
             * \param[in] nowShowList If true, list parameters will be shown.  If false, list parameters will be
             *                        hidden.
             */
            void setShowListEnabled(bool nowShowList = true);

            /**
             * Slot you can use to indicate that list parameters should be hidden.
             *
             * \param[in] nowHideList If true, list parameters will be hidden.  If false, list parameters will be
             *                        shown.
             */
            void setShowListDisabled(bool nowHideList = true);

            /**
             * Slot you can use to indicate that lists should show a bullet/number.
             *
             * \param[in] nowShowBullet If true, the list text lines should include a square representing the bullet
             *                          or number.  If false, no square will be shown.
             */
            void setShowBulletEnabled(bool nowShowBullet = true);

            /**
             * Slot you can use to indicate that lists should show a bullet/number.
             *
             * \param[in] nowHideBullet If true, the list should not include a square representing the bullet or
             *                          number.  If false, a square representing the bullet or number will be shown.
             */
            void setShowBulletDisabled(bool nowHideBullet = true);

            /**
             * Slot you can use to set the line spacing.
             *
             * \param[in] newLineSpacing The new line spacing value.  A value of 1.0 indicates single spacing.  A value
             *                           of 1.5 indicates 1 1/2 line spacing.  A value of 2.0 indicates double spacing.
             */
            void setLineSpacing(float newLineSpacing);

            /**
             * Slot you can use to set the background brush for unused portions of the widget.
             *
             * \param[in] newBrush The new brush to used for the unused portions of the widget.
             */
            void setBackgroundBrush(const QBrush& newBrush);

            /**
             * Slot you can use to set the background of the page contents.
             *
             * \param[in] newBrush The new brush to use for the background.
             */
            void setPageContentsBackgroundBrush(const QBrush& newBrush);

            /**
             * Slot you can use to set the brush used for the margins.
             *
             * \param[in] newBrush The new brush to apply for the margins.
             */
            void setMarginBrush(const QBrush& newBrush);

            /**
             * Slot you can use to set the pen used to draw the widget border.
             *
             * \param[in] newPen The new pen used to draw the widget border.
             */
            void setWidgetBorderLinePen(const QPen& newPen);

            /**
             * Slot you can use to set the pen used to draw border lines for the page margins.
             *
             * \param[in] newPen The new pen to use to draw margin borders.
             */
            void setMarginBorderLinePen(const QPen& newPen);

            /**
             * Slot you can use to draw border lines for the page extents.
             *
             * \param[in] newPen The new pen to use to draw borders around the page extents.
             */
            void setExtentsBorderLinePen(const QPen& newPen);

            /**
             * Slot you can use to set the annotation brush used to draw annotations.
             *
             * \param[in] newBrush The new brush to use to draw annotations.
             */
            void setAnnotationBrush(const QBrush& newBrush);

            /**
             * Slot you can use to set the annotation pen used to draw annotations.
             *
             * \param[in] newPen The new pen to use to draw annotation lines.
             */
            void setAnnotationPen(const QPen& newPen);

            /**
             * Slot you can use to set the text line fill brush used to draw text lines.
             *
             * \param[in] newBrush The new brush to used for fill areas on each text line.
             */
            void setTextLineFillBrush(const QBrush& newBrush);

            /**
             * Slot you can use to set the border pen used to draw borders on each text line.
             *
             * \param[in] newPen The new pen to use to draw borders on each text line.
             */
            void setTextLineBorderPen(const QPen& newPen);

            /**
             * Slot you can use to set the brush used to fill in the bullet/number block.
             *
             * \param[in] newBrush The new brush to draw the bullet fill.
             */
            void setBulletFillBrush(const QBrush& newBrush);

            /**
             * Slot you can use to draw a border around the bullet/number block.
             *
             * \param[in] newPen THe new pen used to draw the bullet border line.
             */
            void setBulletBorderPen(const QPen& newPen);

            /**
             * Slot you can use to set the pen used to draw the text margins and center lines.
             *
             * \param[in] newPen The new pen to use to draw text margins and center lines.
             */
            void setTextMarginIndicatorPen(const QPen& newPen);

        protected:
            /**
             * Method that is called to repaint the widget.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void paintEvent(QPaintEvent* event) override;

            /**
             * Method that is called when the widget is resized.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void resizeEvent(QResizeEvent* event) override;

        private slots:
            /**
             * Slot that is triggered when a height adjustment is needed.
             */
            void adjustHeight();

        private:
            /**
             * Value used to indicate how much of the edges should be left to make the page width obvious.
             */
            static constexpr float paddingFraction = 0.025F;

            /**
             * Value used to indicate how much of each text line should be empty.
             */
            static constexpr float textLineAscentFraction = 1.0/4.0;

            /**
             * Value used to indicate gap between objects and annotations.
             */
            static constexpr unsigned annotationGap = 4;

            /**
             * Value used to indicate the minimum dimension line length.
             */
            static constexpr unsigned minimumDimensionLineLength = 20;

            /**
             * Value used to indicate the minimum extension line length.
             */
            static constexpr unsigned minimumExtensionLineLength = 30;

            /**
             * Value used to indicate the minimum distance from edge of the extension line to the edge of the widget.
             */
            static constexpr unsigned minimumDistanceToEdge = 4;

            /**
             * Value used to indicate the offset for dimension lines.
             */
            static constexpr unsigned dimensionLineOffset = 10;

            /**
             * Value used to indicate the length of dimension lines when rendering small distances.
             */
            static constexpr unsigned smallDistanceDimensionLineLength = 20;

            /**
             * Value used to indicate the direction an arrow head should point.
             */
            enum Direction {
                /**
                 * Up
                 */
                UP,

                /**
                 * Down
                 */
                DOWN,

                /**
                 * Left
                 */
                LEFT,

                /**
                 * Right
                 */
                RIGHT
            };

            /**
             * Method that is called by constructors to perform common initialization of this widget.
             */
            void configureWidget();

            /**
             * Method that is called to draw the page background and margins.
             *
             * \param[in,out] painter        The painter used to draw the background.
             *
             * \param[out]    pageActiveArea A rectangle representing the active area of the page.
             *
             * \param[out]    scaleFactor    The scale factor to use to translate indentations and spacing to window
             *                               coordiantes.
             */
            void drawBackground(QPainter& painter, QRect& pageActiveArea, float& scaleFactor);

            /**
             * Method that is called to draw lines representing the rendered text.
             *
             * \param[in,out] painter                           The painter to use to draw the text.
             *
             * \param[in]     pageActiveArea                    A rectangle representing the page's active area.
             *
             * \param[in]     scaleFactor                       Scale factor used to translate margins and spacing to
             *                                                  window coordinates.
             *
             * \param[out]    previousParagraphBottomLine       A rectangle representing the extents of the previous
             *                                                  paragraph.
             *
             * \param[out]    bulletExtents                     A rectangle representing the square used to draw the
             *                                                  bullet or number.  The value is calculated even if
             *                                                  a bullet/number is not shown.
             *
             * \param[out]    centralParagraphFirstLineExtents  A rectangle representing the first line of text in the
             *                                                  central paragraph.
             *
             * \param[out]    centralParagraphSecondLineExtents A rectangle represent the second line of text in the
             *                                                  central paragraph.
             *
             * \param[out]    centralParagraphRemainderExtents  A rectangle representing the extents of all lines in
             *                                                  the central paragraph, except the first line.
             *
             * \param[out]    nextParagraphTopLine              A rectangle representing the extents of the next
             *                                                  paragraph.
             */
            void drawTextLines(
                QPainter& painter,
                QRect     pageActiveArea,
                float     scaleFactor,
                QRect&    previousParagraphBottomLine,
                QRect&    bulletExtents,
                QRect&    centralParagraphFirstLineExtents,
                QRect&    centralParagraphSecondLineExtents,
                QRect&    centralParagraphRemainderExtents,
                QRect&    nextParagraphTopLine
            );

            /**
             * Method that is called to draw a single line of text.
             *
             * \param[in,out] painter                      The painter used to render the line.
             *
             * \param[in]     textLineBoundingRectangle    A bounding rectangle representing the region in which the
             *                                             line should be drawn.  The value ignores the relative line
             *                                             length and justification.
             *
             * \param[in]     relativeLength               A floating point value representing the relative line length
             *                                             within the provided bounding rectangle.  The value will be
             *                                             ignored if the justification mode is
             *                                             \ref Justification::JUSTIFY.
             *
             * \param[in]     justification                The desired line justification.
             *
             * \return Returns a rectangle representing the drawn text line.
             */
            QRect drawTextLine(
                QPainter&     painter,
                const QRect&  textLineBoundingRectangle,
                float         relativeLength = 1.0,
                Justification justification = Justification::JUSTIFY
            );

            /**
             * Method that is called to draw text margins for a paragraph.
             *
             * \param[in,out] painter       The painter used to render the margins.
             *
             * \param[in]     extents       A bounding rectangle representing the extents of the region requiring
             *                              margin lines.
             *
             * \param[in]     justification The desired line justification.
             */
            void drawTextMargins(
                QPainter&     painter,
                const QRect&  extents,
                Justification justification = Justification::JUSTIFY
            );

            /**
             * Method that can be called to present the horizontal dimension between two points.
             *
             * \param[in,out] painter               The painter used to render the dimension line.
             *
             * \param[in]     point1                The first point to dimension.
             *
             * \param[in]     point2                The second point to dimension.
             *
             * \param[in]     calloutDirection      The direction where the callout should be placed on the outer
             *                                      boundary of the widget.
             *
             * \param[in]     belowPoints           This parameter indicates whether the dimension line should be
             *                                      placed above or below the points.  A true value will place the
             *                                      dimension line below.  A false value will place the dimension
             *                                      line above.
             *
             * \param[in]     preferRightConnection The method will normally prefer the center point of the dimension
             *                                      line as the connection point.  If the points are too close for a
             *                                      dimension line, the method will instead draw a tie line off one of
             *                                      the arrows.  This parameter indicates which arrow to prefer in
             *                                      this situation.
             *
             * \param[in]     widgetRectangle       Rectangle representing the outer boundary of the widget.
             *
             * \return Returns the location of the callout.
             */
            QPoint drawHorizontalDimension(
                QPainter&     painter,
                const QPoint& point1,
                const QPoint& point2,
                Direction     calloutDirection,
                bool          belowPoints,
                bool          preferRightConnection,
                const QRect&  widgetRectangle
            );


            /**
             * Method that can be called to present the vertical dimension between two points.
             *
             * \param[in,out] painter                The painter used to render the dimension line.
             *
             * \param[in]     point1                 The first point to dimension.
             *
             * \param[in]     point2                 The second point to dimension.
             *
             * \param[in]     calloutDirection       The direction where the callout should be placed on the outer
             *                                       boundary of the widget.
             *
             * \param[in]     toRightOf              This parameter indicates whether the dimension line should be
             *                                       placed to the left or right of the points.  A true value will
             *                                       place the dimension line to the right.  A false value will
             *                                       place the dimension line to the left.
             *
             * \param[in]     preferBottomConnection The method will normally prefer the center point of the dimension
             *                                       line as the connection point.  If the points are too close for a
             *                                       dimension line, the method will instead draw a tie line off one of
             *                                       the arrows.  This parameter indicates which arrow to prefer in
             *                                       this situation.
             *
             * \param[in]     widgetRectangle        Rectangle representing the outer boundary of the widget.
             *
             * \return Returns the location of the callout.
             */
            QPoint drawVerticalDimension(
                QPainter&     painter,
                const QPoint& point1,
                const QPoint& point2,
                Direction     calloutDirection,
                bool          toRightOf,
                bool          preferBottomConnection,
                const QRect&  widgetRectangle
            );

            /**
             * Method that can be called to draw a dimension line.  Note that this method supports only horizontal and
             * vertical dimension lines.
             *
             * \param[in,out] painter                     The painter used to render the dimension line.
             *
             * \param[in]     point1                      The first point for the dimension line.
             *
             * \param[in]     point2                      The second point for the dimension line.
             *
             * \param[in]     preferVertical              If true, then the dimension will be treated as vertical if
             *                                            the two points are the same.  If false, the dimension will be
             *                                            treated as horizontal.
             *
             * \param[in]     preferBottomRightConnection The method will normally prefer the center point of the
             *                                            dimension line as the connection point.  If the points are
             *                                            too close for a dimension line, the method will instead
             *                                            a tie line off one of the arrows.  This parameter indicates
             *                                            which arrow to prefer in this situation.
             *
             * \param[out]    smallDistance               A pointer to an optional boolean value that will be populated
             *                                            with true if the dimension line is small distance.
             *
             * \return Returns the connection point where an annotation line should be tied.
             */
            QPoint drawDimensionLine(
                QPainter&     painter,
                const QPoint& point1,
                const QPoint& point2,
                bool          preferVertical,
                bool          preferBottomRightConnection,
                bool*         smallDistance = Q_NULLPTR
            );

            /**
             * Method that can be called to draw a small arrow.
             *
             * \param[in] painter     The painter to use to draw the arrow.
             *
             * \param[in] position    The position of the point of the arrow.
             *
             * \param[in] direction   The arrow direction.
             *
             * \param[in] includeTail If true, a tail will be included on the arrow.  If false, no tail will be
             *                        included.
             */
            void drawArrow(QPainter& painter, const QPoint& position, Direction direction, bool includeTail);

            /**
             * Method that calculates the height required for a given width.
             *
             * \param[in] width The width to calculate the height requirement for.
             */
            unsigned heightRequired(unsigned width) const;

            /**
             * The current maximum height, in pixels.
             */
            unsigned currentMaximumHeight;

            /**
             * The current page width.
             */
            float currentPageWidth;

            /**
             * The current page left margin.
             */
            float currentPageLeftMargin;

            /**
             * The current page right margin.
             */
            float currentPageRightMargin;

            /**
             * The current number of text lines to render per paragraph.
             */
            unsigned currentNumberTextLinesPerParagraph;

            /**
             * The current text line height, in pixels.
             */
            float currentTextLineHeight;

            /**
             * The current justification mode.
             */
            Justification currentJustification;

            /**
             * The current left indentation value.
             */
            float currentLeftIndentation;

            /**
             * The current right indentation value.
             */
            float currentRightIndentation;

            /**
             * The current top spacing value.
             */
            float currentTopSpacing;

            /**
             * The current bottom spacing value.
             */
            float currentBottomSpacing;

            /**
             * The current first line left indentation value.
             */
            float currentFirstLineIndentation;

            /**
             * The current list indentation value.
             */
            float currentListIndentation;

            /**
             * Flag indicating if list parameters should be shown.
             */
            bool currentShowList;

            /**
             * Flag indicating if lists should include a square representing the bullet or number.
             */
            bool currentShowBullet;

            /**
             * The currently displayed line spacing.
             */
            float currentLineSpacing;

            /**
             * Current brush used to render the unused portions of the widget.
             */
            QBrush currentBackgroundBrush;

            /**
             * Current brush used for the background of the page contents.
             */
            QBrush currentPageContentsBackgroundBrush;

            /**
             * Current brush used for the margins.
             */
            QBrush currentMarginBrush;

            /**
             * Current pen used to draw the widget border.
             */
            QPen currentWidgetBorderLinePen;

            /**
             * Current pen used to draw border lines for the page margins.
             */
            QPen currentMarginBorderLinePen;

            /**
             * Current pen used to draw border lines for the page extents.
             */
            QPen currentExtentsBorderLinePen;

            /**
             * Current brush used to draw annotation arrows.
             */
            QBrush currentAnnotationBrush;

            /**
             * Current pen used to draw annotation lines.
             */
            QPen currentAnnotationPen;

            /**
             * The current brush used to draw text.
             */
            QBrush currentTextLineBrush;

            /**
             * The current pen used to draw text.
             */
            QPen currentTextLinePen;

            /**
             * The current brush used to fill in the bullet/number block.
             */
            QBrush currentBulletFillBrush;

            /**
             * The current pen used to draw a border around the bullet/number block.
             */
            QPen currentBulletBorderPen;

            /**
             * The current pen used to draw text margins and center lines.
             */
            QPen currentTextMarginIndicatorPen;

            /**
             * The current left side indentation call-out location.
             */
            QPoint currentLeftIndentationCalloutLocation;

            /**
             * The current right side indentation call-out location.
             */
            QPoint currentRightIndentationCalloutLocation;

            /**
             * The current top spacing call-out location.
             */
            QPoint currentTopSpacingCalloutLocation;

            /**
             * The current bottom spacing call-out location.
             */
            QPoint currentBottomSpacingCalloutLocation;

            /**
             * The current first line left indentation call-out location.
             */
            QPoint currentFirstLineIndentationCalloutLocation;

            /**
             * The current list indentation call-out location.
             */
            QPoint currentListIndentationCalloutLocation;

            /**
             * The current line spacing callout location.
             */
            QPoint currentLineSpacingCalloutLocation;

            /**
             * List of rectangles to be rendered.
             */
            QList<QRect> currentTextLines;

            /**
             * List of lines to be rendered.
             */
            QList<QLine> currentAnnotationLines;

            /**
             * Map of dimension arrows.
             */
            QMultiMap<Direction, QPoint> currentDimensionArrows;

            /**
             * Timer used to perform manual height adjustment.
             */
            QTimer* heightAdjustTimer;

            /**
             * Timer used to trigger the callout updated event.
             */
            QTimer* calloutUpdatedTimer;
    };
}

#endif
