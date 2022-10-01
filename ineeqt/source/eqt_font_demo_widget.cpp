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
* This file implements the \ref EQt::FontDemoWidget class.
***********************************************************************************************************************/

#include <QWidget>
#include <QString>
#include <QFont>
#include <QSizePolicy>
#include <QSize>
#include <QSizeF>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QList>
#include <QPainter>
#include <QFontMetricsF>
#include <QPointF>
#include <QRectF>
#include <QTimer>

#include "eqt_font_demo_widget.h"

namespace EQt {
    const float FontDemoWidget::defaultScalingFactors[] = {
        0.1F, 0.2F, 0.25F, 1.0F/3.0F, 0.5F, 0.75F, 1.0F, 1.5F, 2.0F, 2.5F, 10.0F/3.0F, 5.0F, 7.5F, 10.0F, 0.0F
    };

    FontDemoWidget::FontDemoWidget(QWidget* parent):QWidget(parent) {
        configureWidget();
    }


    FontDemoWidget::~FontDemoWidget() {}


    QSize FontDemoWidget::scalingLimits() const {
        return currentScalingLimits;
    }


    QList<float> FontDemoWidget::scalingFactors() const {
        return currentScaleFactors;
    }


    QString FontDemoWidget::fillerText(TextPosition position) {
        return currentFillerText.at(static_cast<unsigned>(position));
    }


    QFont FontDemoWidget::fillerFont() const {
        return currentFillerFont;
    }


    bool FontDemoWidget::markersEnabled() const {
        return currentMarkersEnabled;
    }


    bool FontDemoWidget::markersDisabled() const {
        return !currentMarkersEnabled;
    }


    QColor FontDemoWidget::fillerFontColor() const {
        return currentFillerFontColor;
    }


    QColor FontDemoWidget::backgroundColor() const {
        return currentBackgroundColor;
    }


    QPen FontDemoWidget::markerLinePen() const {
        return currentMarkerLinePen;
    }


    QString FontDemoWidget::sampleHeadText() const {
        return currentSampleHeadText;
    }


    QString FontDemoWidget::sampleTailText() const {
        return currentSampleTailText;
    }


    QFont FontDemoWidget::sampleTextFont() const {
        return currentSampleFont;
    }


    QColor FontDemoWidget::sampleTextFontColor() const {
        return currentSampleFontColor;
    }


    QColor FontDemoWidget::sampleTextFontBackgroundColor() const {
        return currentSampleFontBackgroundColor;
    }


    float FontDemoWidget::relativePosition() const {
        return currentRelativePosition;
    }


    float FontDemoWidget::relativeSize() const {
        return currentRelativeSize;
    }


    float FontDemoWidget::relativeWeight() const {
        return currentRelativeWeight;
    }


    float FontDemoWidget::scaleFactor() const {
        return currentScaleFactor;
    }


    void FontDemoWidget::setScalingLimits(const QSize& newScalingLimits) {
        currentScalingLimits = newScalingLimits;
        updateTimer->start(0);
    }


    void FontDemoWidget::setScaleFactors(const QList<float>& newScaleFactors) {
        currentScaleFactors = newScaleFactors;
        updateTimer->start(0);
    }


    void FontDemoWidget::setFillerText(TextPosition position, const QString& newDefaultText) {
        currentFillerText[static_cast<unsigned>(position)] = newDefaultText;
        updateTimer->start(0);
    }


    void FontDemoWidget::setFillerFont(const QFont& newFillerFont) {
        currentFillerFont = newFillerFont;
        updateTimer->start(0);
    }


    void FontDemoWidget::setMarkersEnabled(bool nowEnabled) {
        currentMarkersEnabled = nowEnabled;
        updateTimer->start(0);
    }


    void FontDemoWidget::setMarkersDisabled(bool nowDisabled) {
        setMarkersEnabled(!nowDisabled);
    }


    void FontDemoWidget::setFillerFontColor(const QColor& newFontColor) {
        currentFillerFontColor = newFontColor;
        update();
    }


    void FontDemoWidget::setBackgroundColor(const QColor& newBackgroundColor) {
        currentBackgroundColor = newBackgroundColor;
        update();
    }


    void FontDemoWidget::setMarkerLinePen(const QPen& newPen) {
        currentMarkerLinePen = newPen;
        update();
    }


    void FontDemoWidget::setSampleHeadText(const QString& newSampleText) {
        currentSampleHeadText = newSampleText;
        updateTimer->start(0);
    }


    void FontDemoWidget::setSampleTailText(const QString& newSampleText) {
        currentSampleTailText = newSampleText;
        updateTimer->start(0);
    }


    void FontDemoWidget::setSampleTextFont(const QFont& newSampleFont) {
        currentSampleFont = newSampleFont;
        updateTimer->start(0);
    }


    void FontDemoWidget::setSampleTextFontColor(const QColor& newSampleTextFontColor) {
        currentSampleFontColor = newSampleTextFontColor;
        updateTimer->start(0);
    }


    void FontDemoWidget::setSampleTextFontBackgroundColor(const QColor& newSampleTextFontBackgroundColor) {
        currentSampleFontBackgroundColor = newSampleTextFontBackgroundColor;
        update();
    }


    void FontDemoWidget::setRelativePosition(float newRelativePosition) {
        currentRelativePosition = newRelativePosition;
        updateTimer->start(0);
    }


    void FontDemoWidget::setRelativeSize(float newRelativeSize) {
        currentRelativeSize = newRelativeSize;
        updateTimer->start(0);
    }


    void FontDemoWidget::setRelativeWeight(float newRelativeWeight) {
        currentRelativeWeight = newRelativeWeight;
        updateTimer->start(0);
    }


    void FontDemoWidget::paintEvent(QPaintEvent*) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QSize widgetArea = size();

        painter.setPen(Qt::NoPen);
        painter.setBrush(currentBackgroundColor.isValid() ? currentBackgroundColor : QColor(Qt::white));
        painter.drawRect(QRect(QPoint(0, 0), widgetArea));

        QFont adjustedFillerFont;
        QFont adjustedSampleHeadFont;
        QFont adjustedSampleTailFont;
        calculateAdjustedFonts(
            currentScaleFactor,
            adjustedFillerFont,
            adjustedSampleHeadFont,
            adjustedSampleTailFont
        );

        QFontMetricsF fillerFontMetrics(adjustedFillerFont);
        QFontMetricsF sampleHeadFontMetrics(adjustedSampleHeadFont);
        QFontMetricsF sampleTailFontMetrics(adjustedSampleTailFont);

        float tailPositionAdjustment = sampleHeadFontMetrics.height() * currentRelativePosition;

        QString topFillerText            = fillerText(TextPosition::ABOVE);
        QString leftFillerText           = fillerText(TextPosition::LEFT);
        QString rightFillerText          = fillerText(TextPosition::RIGHT);
        QString bottomFillerText         = fillerText(TextPosition::BELOW);

        QString topFiller;
        QString bottomFiller;
        float   topFillerTextWidth;
        float   bottomFillerTextWidth;

        do {
            topFiller += topFillerText;
            topFillerTextWidth = fillerFontMetrics.horizontalAdvance(topFiller);
        } while (topFillerTextWidth <= widgetArea.width());

        do {
            bottomFiller += bottomFillerText;
            bottomFillerTextWidth = fillerFontMetrics.horizontalAdvance(bottomFiller);
        } while (bottomFillerTextWidth <= widgetArea.width());

        float fillerTextHeight               = fillerFontMetrics.height();
        float leftFillerFirstCharacterWidth  =   leftFillerText.isEmpty()
                                               ? 0.0F
                                               : fillerFontMetrics.horizontalAdvance(leftFillerText.left(1));

        float sampleLineTop    = fillerTextHeight / 2.0;
        float sampleHeadHeight = sampleHeadFontMetrics.height();
        float sampleTailHeight = sampleTailFontMetrics.height() + tailPositionAdjustment;
        float sampleLineHeight = std::max(fillerTextHeight, std::max(sampleHeadHeight, sampleTailHeight));
        float leftFillerWidth  = fillerFontMetrics.horizontalAdvance(leftFillerText);
        float sampleHeadWidth  = sampleHeadFontMetrics.horizontalAdvance(currentSampleHeadText);
        float sampleTailWidth  = sampleTailFontMetrics.horizontalAdvance(currentSampleTailText);

        QColor fillerFontColor = currentFillerFontColor.isValid() ? currentFillerFontColor : QColor(Qt::black);
        painter.setPen(fillerFontColor);
        painter.setBrush(QBrush(fillerFontColor));
        painter.setFont(adjustedFillerFont);

        painter.drawText(QPointF(0.0F, sampleLineTop), topFiller);
        painter.drawText(
            QPointF(-leftFillerFirstCharacterWidth / 2.0, sampleLineTop + sampleLineHeight),
            leftFillerText
        );
        painter.drawText(
            QPointF(
                -leftFillerFirstCharacterWidth / 2.0 + leftFillerWidth + sampleHeadWidth + sampleTailWidth,
                sampleLineTop + sampleLineHeight
            ),
            rightFillerText
        );
        painter.drawText(
            QPointF(widgetArea.width() - bottomFillerTextWidth, sampleLineTop + sampleLineHeight + fillerTextHeight),
            bottomFiller
        );

        QColor sampleBackgroundColor = currentSampleFontBackgroundColor.isValid()
                                       ? currentSampleFontBackgroundColor
                                       : QColor(Qt::white);

        if (sampleBackgroundColor != currentBackgroundColor) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(sampleBackgroundColor));

            if (sampleHeadWidth > 0) {
                QRectF rectangle(
                    QPointF(-leftFillerFirstCharacterWidth / 2.0 + leftFillerWidth, sampleLineTop),
                    QSizeF(sampleHeadWidth, sampleHeadHeight)
                );
                painter.drawRect(rectangle);
            }

            if (sampleTailWidth > 0) {
                QRectF rectangle(
                    QPointF(
                        -leftFillerFirstCharacterWidth / 2.0 + leftFillerWidth + sampleHeadWidth,
                        sampleLineTop + tailPositionAdjustment
                    ),
                    QSizeF(sampleTailWidth, sampleTailHeight)
                );
                painter.drawRect(rectangle);
            }
        }

        QColor sampleTextColor = currentSampleFontColor.isValid() ? currentSampleFontColor : QColor(Qt::black);
        painter.setPen(sampleTextColor);
        painter.setBrush(QBrush(sampleTextColor));
        painter.setFont(adjustedSampleHeadFont);
        painter.drawText(
            QPointF(-leftFillerFirstCharacterWidth / 2.0 + leftFillerWidth, sampleLineTop + sampleHeadHeight),
            currentSampleHeadText
        );

        painter.setFont(adjustedSampleTailFont);
        painter.drawText(
            QPointF(
                -leftFillerFirstCharacterWidth / 2.0 + leftFillerWidth + sampleHeadWidth,
                sampleLineTop + sampleHeadHeight - tailPositionAdjustment
            ),
            currentSampleTailText
        );

        if (currentMarkersEnabled) {
            painter.setPen(currentMarkerLinePen);
            painter.drawLine(
                0, sampleLineTop + sampleHeadHeight,
                widgetArea.width(), sampleLineTop + sampleHeadHeight
            );

            float ascent = sampleHeadFontMetrics.ascent();
            painter.drawLine(
                0, sampleLineTop + sampleHeadHeight - ascent,
                widgetArea.width(), sampleLineTop + sampleHeadHeight - ascent
            );
        }
    }


    void FontDemoWidget::resizeEvent(QResizeEvent* event) {
        QWidget::resizeEvent(event);
    }


    void FontDemoWidget::configureWidget() {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        currentScalingLimits = QSize();

        const float* scaleFactor = defaultScalingFactors;
        while (*scaleFactor != 0) {
            currentScaleFactors.append(*scaleFactor);
            ++scaleFactor;
        }

        for (unsigned i=0 ; i<static_cast<unsigned>(TextPosition::NUMBER_POSITIONS) ; ++i) {
            currentFillerText.append(QString());
        }


        currentFillerText[static_cast<unsigned>(TextPosition::ABOVE)] = tr(
            "Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, "
            "totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae "
        );

        currentFillerText[static_cast<unsigned>(TextPosition::LEFT)]  = tr("psum.  ");
        currentFillerText[static_cast<unsigned>(TextPosition::RIGHT)] = tr("  Lorem");
        currentFillerText[static_cast<unsigned>(TextPosition::BELOW)] = tr(
            "dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, "
            "sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam "
        );

        currentFillerFont                = QFont(tr("Helvetica"), 12, QFont::Weight::Normal, true);
        currentMarkersEnabled            = true;
        currentFillerFontColor           = QColor(Qt::gray);
        currentBackgroundColor           = QColor(Qt::white);
        currentMarkerLinePen             = QPen(QBrush(Qt::black), 0, Qt::DotLine);
        currentSampleHeadText            = tr("");
        currentSampleTailText            = tr("Cwm fjord bank glyphs vext quiz.");
        currentSampleFont                = currentFillerFont;
        currentSampleFontColor           = QColor(Qt::black);
        currentSampleFontBackgroundColor = QColor(Qt::white);
        currentRelativePosition          = 0.0F;
        currentRelativeSize              = 1.0F;
        currentRelativeWeight            = 0.0F;

        currentScaleFactor = -1.0;

        updateTimer = new QTimer(this);
        updateTimer->setSingleShot(true);

        updateWidget();

        connect(updateTimer, &QTimer::timeout, this, &FontDemoWidget::updateWidget);
    }


    void FontDemoWidget::updateWidget() {
        QSize widgetSize;
        float newScaleFactor;

        if (currentScalingLimits.isValid() && !currentScaleFactors.isEmpty()) {
            unsigned lowerIndex = 0;
            unsigned upperIndex = currentScaleFactors.size() - 1;
            unsigned index;

            do {
                index          = (lowerIndex + upperIndex) / 2;
                newScaleFactor = currentScaleFactors.value(index);
                widgetSize     = calculateWidgetSize(newScaleFactor);

                if (widgetSize.width() > currentScalingLimits.width()   ||
                    widgetSize.height() > currentScalingLimits.height()    ) {
                    upperIndex = index;
                } else {
                    lowerIndex = index;
                }
            } while (upperIndex - lowerIndex > 1);

            while (index > 0                                                 &&
                   (widgetSize.width() > currentScalingLimits.width()   ||
                    widgetSize.height() > currentScalingLimits.height()    )    ) {
                --index;
                newScaleFactor = currentScaleFactors.value(index);
                widgetSize     = calculateWidgetSize(newScaleFactor);
            }
        } else {
            newScaleFactor = 1.0F;
            widgetSize     = calculateWidgetSize(newScaleFactor);
        }

        setFixedSize(widgetSize);
        update();

        if (currentScaleFactor != newScaleFactor) {
            currentScaleFactor = newScaleFactor;
            emit scaleFactorChanged(newScaleFactor);
        }
    }


    QSize FontDemoWidget::calculateWidgetSize(float scaleFactor) {
        QFont adjustedFillerFont;
        QFont adjustedSampleHeadFont;
        QFont adjustedSampleTailFont;
        calculateAdjustedFonts(scaleFactor, adjustedFillerFont, adjustedSampleHeadFont, adjustedSampleTailFont);

        QFontMetricsF fillerFontMetrics(adjustedFillerFont);
        QFontMetricsF sampleHeadFontMetrics(adjustedSampleHeadFont);
        QFontMetricsF sampleTailFontMetrics(adjustedSampleTailFont);

        QString leftFiller               = currentFillerText[static_cast<unsigned>(TextPosition::LEFT)];
        QString rightFiller              = currentFillerText[static_cast<unsigned>(TextPosition::RIGHT)];
        QString leftFillerFirstCharacter = leftFiller.left(1);
        QString rightFillerLastCharacter = rightFiller.right(1);

        float tailPositionAdjustment = sampleHeadFontMetrics.height() * currentRelativePosition;

        unsigned width = (
              fillerFontMetrics.horizontalAdvance(leftFiller)
            + sampleHeadFontMetrics.horizontalAdvance(currentSampleHeadText)
            + sampleTailFontMetrics.horizontalAdvance(currentSampleTailText)
            + fillerFontMetrics.horizontalAdvance(rightFiller)
            - fillerFontMetrics.horizontalAdvance(leftFillerFirstCharacter) / 2.0
            - fillerFontMetrics.horizontalAdvance(rightFillerLastCharacter) / 2.0
        );

        unsigned height = (
              1.5 * fillerFontMetrics.height()
            + std::max(
                  static_cast<float>(fillerFontMetrics.height()),
                  std::max(
                      static_cast<float>(sampleHeadFontMetrics.height()),
                      static_cast<float>(sampleHeadFontMetrics.height() + tailPositionAdjustment)
                  )
              )
        );

        return QSize(width, height);
    }


    void FontDemoWidget::calculateAdjustedFonts(
            float  scaleFactor,
            QFont& adjustedFillerFont,
            QFont& adjustedSampleHeadFont,
            QFont& adjustedSampleTailFont
        ) {
        adjustedFillerFont = currentFillerFont;
        adjustedFillerFont.setPointSizeF(currentFillerFont.pointSizeF() * scaleFactor);

        adjustedSampleHeadFont = currentSampleFont;
        adjustedSampleHeadFont.setPointSizeF(currentSampleFont.pointSizeF() * scaleFactor);

        adjustedSampleTailFont = currentSampleFont;
        if (currentRelativeSize != 1.0F || currentRelativeWeight != 0.0F) {
            adjustedSampleTailFont.setPointSizeF(currentSampleFont.pointSizeF() * scaleFactor * currentRelativeSize);

            unsigned weight       = static_cast<unsigned>(currentSampleFont.weight());
            int      black        = static_cast<int>(QFont::Black);
            int      weightOffset = static_cast<int>((black - weight) * currentRelativeWeight);

            adjustedSampleTailFont.setWeight(static_cast<QFont::Weight>(weight + weightOffset));
        } else {
            adjustedSampleTailFont.setPointSizeF(currentSampleFont.pointSizeF() * scaleFactor);
        }
    }
}
