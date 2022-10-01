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
* This header defines the \ref EQt::FontDemoWidget class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_FONT_DEMO_WIDGET_H
#define EQT_FONT_DEMO_WIDGET_H

#include <QWidget>
#include <QString>
#include <QFont>
#include <QSize>
#include <QColor>
#include <QPen>
#include <QList>

#include "eqt_common.h"

class QPaintEvent;
class QResizeEvent;
class QTimer;

namespace EQt {
    /**
     * Class that provides a window you can use to present fonts to the user.
     */
    class EQT_PUBLIC_API FontDemoWidget:public QWidget {
        Q_OBJECT

        public:
            /**
             * A list of the default scaling factors.  The list is terminated with a value of 0.
             */
            static const float defaultScalingFactors[];

            /**
             * Enumeration of default text positions
             */
            enum class TextPosition {
                /**
                 * Indicates the text above the sample text.
                 */
                ABOVE,

                /**
                 * Indicates the text to the left of the sample text.
                 */
                LEFT,

                /**
                 * Indicates the text to the right of the sample text.
                 */
                RIGHT,

                /**
                 * Indicates the text below the sample text.
                 */
                BELOW,

                /**
                 * The number of positions.
                 */
                NUMBER_POSITIONS,
            };

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent widget.
             */
            FontDemoWidget(QWidget* parent = Q_NULLPTR);

            ~FontDemoWidget() override;

            /**
             * Method you can use to obtain the current scaling limits.
             *
             * \return Returns the current scaling limits.  A value of 0, 0 will be returned if scaling is disabled.
             */
            QSize scalingLimits() const;

            /**
             * Method you can use to obtain the current list of scale factors.
             *
             * \return Returns a list of scaling factors.
             */
            QList<float> scalingFactors() const;

            /**
             * Method you can use to obtain the filler text to display around the sample text.
             *
             * \param[in] position The position using this text.
             *
             * \return Returns the text at the indicated position.
             */
            QString fillerText(TextPosition position);

            /**
             * Method you can use to determine the current filler font.
             *
             * \return Returns the current filler font.
             */
            QFont fillerFont() const;

            /**
             * Method you can use to determine if marker lines are enabled.
             *
             * \return Returns true if marker lines are enabled.
             */
            bool markersEnabled() const;

            /**
             * Method you can use to determine if marker lines are disabled.
             *
             * \return Returns true if marker lines are disabled.
             */
            bool markersDisabled() const;

            /**
             * Method you can use to determine the current filler font color.
             *
             * \return Returns the current filler font color.
             */
            QColor fillerFontColor() const;

            /**
             * Method you can use to determine the current background color.
             *
             * \return Returns the current background color.
             */
            QColor backgroundColor() const;

            /**
             * Method you can use to determine the current marker line pen.
             *
             * \return Returns the current marker line pen.
             */
            QPen markerLinePen() const;

            /**
             * Method you can use to obtain the current sample text on the left.  This text will *not* be scaled or
             * repositioned based on the values indicated by \ref EQt::FontDemoWidget::setRelativePosition,
             * \ref EQt::FontDemoWidget::setRelativeSize. The text will also not have weight adjustment based on
             * \ref EQt::FontDemoWidget::setRelativeWeight.
             *
             * \return Returns the current sample text.
             */
            QString sampleHeadText() const;

            /**
             * Method you can use to obtain the current sample text on the left.  This text will be scaled and
             * repositioned based on the values indicated by \ref EQt::FontDemoWidget::setRelativePosition,
             * \ref EQt::FontDemoWidget::setRelativeSize. The text will also have weight adjustment based on
             * \ref EQt::FontDemoWidget::setRelativeWeight.
             *
             * \return Returns the current sample text.
             */
            QString sampleTailText() const;

            /**
             * Method you can use to obtain the font used to display the sample font.
             *
             * \return The current font used to display the sample text.
             */
            QFont sampleTextFont() const;

            /**
             * Method you can use to obtain the current font color used for the sample text.
             *
             * \return Returns the current color used for the sample text.
             */
            QColor sampleTextFontColor() const;

            /**
             * Method you can use to obtain the current font background color used for the sample text.
             *
             * \return Returns the current color used behind the sample text.
             */
            QColor sampleTextFontBackgroundColor() const;

            /**
             * Method you can use to obtain the current relative position of the text.
             *
             * \return Returns the relative position of the text.  See the description in
             *         \ref EQt::FontDemoWidget::setRelativePosition for details.
             */
            float relativePosition() const;

            /**
             * Method you can use to obtain the current relative font size.
             *
             * \return Returns the current relative size.
             */
            float relativeSize() const;

            /**
             * Method you can use to obtain the current relative font weight.
             *
             * \return Returns the current relative weight to apply to the font.
             */
            float relativeWeight() const;

            /**
             * Method you can use to obtain the current scale factor.
             *
             * \return Returns the current scale factor.  A value of 1.0 indicates 1:1 scaling.  A value of 2.0
             *         indicates 2:1 scaling.
             */
            float scaleFactor() const;

        signals:
            /**
             * Signal that is emitted whenver the scale factor changes.
             *
             * \param[out] newScaleFactor The new scale factor being used by the widget.
             */
            void scaleFactorChanged(float newScaleFactor);

        public slots:
            /**
             * Method you can use to enable automatic scaling and specify a maximum size for the widget.
             *
             * \param[in] newScalingLimits The new maximum size for the widget.  Setting a size of 0, 0 will disable
             *                             scaling.
             */
            void setScalingLimits(const QSize& newScalingLimits);

            /**
             * Method you can use to specify the scale factors that can be used.
             *
             * \param[in] newScaleFactors A list of supported scale factors.  Values must be positive and in ascending
             *                            order.
             */
            void setScaleFactors(const QList<float>& newScaleFactors);

            /**
             * Method you can use to set the filler text displayed around the sample text.
             *
             * \param[in] position       The position using this text.
             *
             * \param[in] newDefaultText The default text to be displayed.  Note that the text will only be partially
             *                           shown and will be repeated to fill the required space.
             */
            void setFillerText(TextPosition position, const QString& newDefaultText);

            /**
             * Method you can use to set the font used for the filler text.
             *
             * \param[in] newFillerFont The font to use for the filler text.
             */
            void setFillerFont(const QFont& newFillerFont);

            /**
             * Method you can use to enable or disable ascent and baseline markers
             *
             * \param[in] nowEnabled If true, marker lines are enabled.  If false, marker lines are disabled.
             */
            void setMarkersEnabled(bool nowEnabled = true);

            /**
             * Method you can use to disable or enable ascent and baseline markers.
             *
             * \param[in] nowDisabled If true, marker lines are disabled.  If false, marker lines are enabled.
             */
            void setMarkersDisabled(bool nowDisabled = true);

            /**
             * Method you can use to set the filler font color.
             *
             * \param[in] newFontColor The new font color to use for the filler.
             */
            void setFillerFontColor(const QColor& newFontColor);

            /**
             * Method you can use to set the current background color.
             *
             * \param[in] newBackgroundColor The new background color to use for the widget.
             */
            void setBackgroundColor(const QColor& newBackgroundColor);

            /**
             * Method you can use to set the pen used for marker lines.
             *
             * \param[in] newPen The new marker line pen.
             */
            void setMarkerLinePen(const QPen& newPen);

            /**
             * Method you can use to set the sample text to display on the left.  This text will *not* be scaled or
             * repositioned based on the values indicated by \ref EQt::FontDemoWidget::setRelativePosition,
             * \ref EQt::FontDemoWidget::setRelativeSize. The text will also not have weight adjustment based on
             * \ref EQt::FontDemoWidget::setRelativeWeight.
             *
             * \param[in] newSampleText The new sample text to be displayed.
             */
            void setSampleHeadText(const QString& newSampleText);

            /**
             * Method you can use to set the sample text to display on the left.  This text will be scaled and
             * repositioned based on the values indicated by \ref EQt::FontDemoWidget::setRelativePosition,
             * \ref EQt::FontDemoWidget::setRelativeSize. The text will also have weight adjustment based on
             * \ref EQt::FontDemoWidget::setRelativeWeight.
             *
             * \param[in] newSampleText The new sample text to be displayed.
             */
            void setSampleTailText(const QString& newSampleText);

            /**
             * Method you can use to set the font used to display the sample text.
             *
             * \param[in] newSampleFont The new sample text font.
             */
            void setSampleTextFont(const QFont& newSampleFont);

            /**
             * Method you can use to set the font color used for the sample text.
             *
             * \param[in] newSampleTextFontColor The color used for the text displayed.
             */
            void setSampleTextFontColor(const QColor& newSampleTextFontColor);

            /**
             * Method you can use to set the font color used for background for the sample text.
             *
             * \param[in] newSampleTextFontBackgroundColor The color used for the background behind the sample text.
             */
            void setSampleTextFontBackgroundColor(const QColor& newSampleTextFontBackgroundColor);

            /**
             * Method you can use to set the font position relative to the baseline.
             *
             * \param[in] newRelativePosition The new relative position of the font.  Value is a fractional number
             *                                that is applied against the font height.  A negative value will drop
             *                                the text below the baseline.  A positive value will lift the font above
             *                                the baseline.
             */
            void setRelativePosition(float newRelativePosition);

            /**
             * Method you can use to set the relative font size.  Fonts will be scaled based on this value.
             *
             * \param[in] newRelativeSize The new relative size of the font.  Value is a fractional number that is
             *                            multiplied against the font's point size.
             */
            void setRelativeSize(float newRelativeSize);

            /**
             * Method you can use to set the relative font weight.  Font weights will be scaled upwards based on this
             * value by the distance between the current font weight and black.
             *
             * \param[in] newRelativeWeight The relative weight to apply to the font.
             */
            void setRelativeWeight(float newRelativeWeight);

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
             * Slot that recalculates size and internal parameters after a settings change.
             */
            void updateWidget();

        private:
            /**
             * Method that is called by constructors to perform common initialization of this widget.
             */
            void configureWidget();

            /**
             * Method that recalculates the widget size given a scale factor.  This method also created/updates the
             * adjusted fonts intended to be used by \ref EQt::FontDemoWidget::paintEvent.
             *
             * \param[in] scaleFactor The target scale factor.
             *
             * \return Returns the total widget size.
             */
            QSize calculateWidgetSize(float scaleFactor);

            /**
             * Method that calculates the adjusted fonts based on a specified scale factor.
             *
             * \param[in]  scaleFactor            The scale factor to apply.
             *
             * \param[out] adjustedFillerFont     The font to use to render the filler text.
             *
             * \param[out] adjustedSampleHeadFont The font to use to render the sample head text.
             *
             * \param[out] adjustedSampleTailFont The font to use to render the sample tail font.
             */
            void calculateAdjustedFonts(
                float  scaleFactor,
                QFont& adjustedFillerFont,
                QFont& adjustedSampleHeadFont,
                QFont& adjustedSampleTailFont
            );

            /**
             * Timer used to aggregate updates.
             */
            QTimer* updateTimer;

            /**
             * The current scaling limits.
             */
            QSize currentScalingLimits;

            /**
             * List of supported scale factors.
             */
            QList<float> currentScaleFactors;

            /**
             * List holding the current filler text.
             */
            QList<QString> currentFillerText;

            /**
             * The current font used for filler text.
             */
            QFont currentFillerFont;

            /**
             * A flag used to indicate that markers are enabled.
             */
            bool currentMarkersEnabled;

            /**
             * The filler font color.
             */
            QColor currentFillerFontColor;

            /**
             * The current background color.
             */
            QColor currentBackgroundColor;

            /**
             * The pen used for marker lines.
             */
            QPen currentMarkerLinePen;

            /**
             * The current text to use as the sample header text.
             */
            QString currentSampleHeadText;

            /**
             * The current text to use as the sample tail text.
             */
            QString currentSampleTailText;

            /**
             * The current font to use for the sample text.
             */
            QFont currentSampleFont;

            /**
             * The current sample font color.
             */
            QColor currentSampleFontColor;

            /**
             * The current sample font background color.
             */
            QColor currentSampleFontBackgroundColor;

            /**
             * The current relative position for the sample tail text.
             */
            float currentRelativePosition;

            /**
             * The current relative size for the sample tail text.
             */
            float currentRelativeSize;

            /**
             * The current relative weight for the sample tail text.
             */
            float currentRelativeWeight;

            /**
             * The current scale factor for fonts.  Value is calculated by EQt::FontDemoWidget::updateWidget.
             */
            float currentScaleFactor;
    };
}

#endif
