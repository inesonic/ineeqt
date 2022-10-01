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
* This file implements the \ref EQt::ParagraphDiagram class.
***********************************************************************************************************************/

#include <QWidget>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QLine>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QList>
#include <QMultiMap>

#include <algorithm>
#include <cmath>

#include "eqt_paragraph_diagram.h"

namespace EQt {
    const float                           ParagraphDiagram::defaultPageWidth = 72.0F * 8.5F;
    const float                           ParagraphDiagram::defaultPageLeftMargin = 72.0F * 1.0F;
    const float                           ParagraphDiagram::defaultPageRightMargin = 72.0F * 1.0F;
    const unsigned                        ParagraphDiagram::defaultTextLinesPerParagraph = 4;
    const float                           ParagraphDiagram::defaultTextLineHeight = 12.0F;
    const ParagraphDiagram::Justification ParagraphDiagram::defaultJustification = \
                                          ParagraphDiagram::Justification::LEFT;
    const float                           ParagraphDiagram::defaultLeftIndentation = 24.0F;
    const float                           ParagraphDiagram::defaultRightIndentation = 12.0F;
    const float                           ParagraphDiagram::defaultTopSpacing = 12.0F;
    const float                           ParagraphDiagram::defaultBottomSpacing = 12.0F;
    const float                           ParagraphDiagram::defaultFirstLineIndentation = 0.0F;
    const float                           ParagraphDiagram::defaultListIndentation = 0.0F;
    const bool                            ParagraphDiagram::defaultShowList = false;
    const bool                            ParagraphDiagram::defaultShowBullet = true;
    const float                           ParagraphDiagram::defaultLineSpacing = 1.0F;

    const QColor   ParagraphDiagram::defaultBackgroundColor(Qt::lightGray);
    const QColor   ParagraphDiagram::defaultMarginColor(0xF0, 0xF0, 0xF0, 128);
    const QColor   ParagraphDiagram::defaultPageContentsBackgroundColor(Qt::white);
    const QColor   ParagraphDiagram::defaultWidgetBorderLineColor(Qt::black);
    const QColor   ParagraphDiagram::defaultMarginsBorderLineColor(Qt::lightGray);
    const QColor   ParagraphDiagram::defaultExtentsBorderLineColor(Qt::black);
    const QColor   ParagraphDiagram::defaultAnnotationColor(Qt::black);
    const QColor   ParagraphDiagram::defaultTextLineFillColor(Qt::lightGray);
    const QColor   ParagraphDiagram::defaultTextLineBorderColor(Qt::darkGray);
    const QColor   ParagraphDiagram::defaultBulletFillColor(Qt::darkGray);
    const QColor   ParagraphDiagram::defaultBulletBorderLineColor(0x40, 0x40,0x40);
    const QColor   ParagraphDiagram::defaultTextMarginIndicatorColor(0x60, 0x60, 0x60);
    const float    ParagraphDiagram::defaultWidgetBorderLineWidth       = 1.0F;
    const float    ParagraphDiagram::defaultMarginBorderLineWidth       = 1.0F;
    const float    ParagraphDiagram::defaultExtentsBorderLineWidth      = 1.0F;
    const float    ParagraphDiagram::defaultAnnotationLineWidth         = 1.0F;
    const float    ParagraphDiagram::defaultTextLineBorderWidth         = 1.0F;
    const float    ParagraphDiagram::defaultTextMarginIndicatorWidth    = 1.0F;


    ParagraphDiagram::ParagraphDiagram(QWidget* parent):QWidget(parent) {
        configureWidget();
    }


    ParagraphDiagram::~ParagraphDiagram() {}


    unsigned ParagraphDiagram::maximumHeight() const {
        return currentMaximumHeight;
    }


    float ParagraphDiagram::pageWidth() const {
        return currentPageWidth;
    }


    float ParagraphDiagram::pageLeftMargin() const {
        return currentPageLeftMargin;
    }


    float ParagraphDiagram::pageRightMargin() const {
        return currentPageRightMargin;
    }


    unsigned ParagraphDiagram::numberTextLinesPerParagraph() const {
        return currentNumberTextLinesPerParagraph;
    }


    float ParagraphDiagram::textLineHeight() const {
        return currentTextLineHeight;
    }


    ParagraphDiagram::Justification ParagraphDiagram::justification() const {
        return currentJustification;
    }


    float ParagraphDiagram::leftIndentation() const {
        return currentLeftIndentation;
    }


    float ParagraphDiagram::rightIndentation() const {
        return currentRightIndentation;
    }


    float ParagraphDiagram::topSpacing() const {
        return currentTopSpacing;
    }


    float ParagraphDiagram::bottomSpacing() const {
        return currentBottomSpacing;
    }


    float ParagraphDiagram::firstLineIndentation() const {
        return currentFirstLineIndentation;
    }


    float ParagraphDiagram::listIndentation() const {
        return currentListIndentation;
    }


    bool ParagraphDiagram::showList() const {
        return currentShowList;
    }


    bool ParagraphDiagram::showBullet() const {
        return currentShowBullet;
    }


    float ParagraphDiagram::lineSpacing() const {
        return currentLineSpacing;
    }


    QBrush ParagraphDiagram::backgroundBrush() const {
        return currentBackgroundBrush;
    }


    QBrush ParagraphDiagram::pageContentsBackgroundBrush() const {
        return currentPageContentsBackgroundBrush;
    }


    QBrush ParagraphDiagram::marginsBrush() const {
        return currentMarginBrush;
    }


    QPen ParagraphDiagram::widgetBorderLinePen() const {
        return currentWidgetBorderLinePen;
    }


    QPen ParagraphDiagram::marginBorderLinePen() const {
        return currentMarginBorderLinePen;
    }


    QPen ParagraphDiagram::extentsBorderLinePen() const {
        return currentExtentsBorderLinePen;
    }


    QBrush ParagraphDiagram::annotationBrush() const {
        return currentAnnotationBrush;
    }


    QPen ParagraphDiagram::annotationPen() const {
        return currentAnnotationPen;
    }


    QBrush ParagraphDiagram::textLineFillBrush() const {
        return currentTextLineBrush;
    }


    QPen ParagraphDiagram::textLineBorderPen() const {
        return currentTextLinePen;
    }


    QPen ParagraphDiagram::textMarginIndicatorPen() const {
        return currentTextMarginIndicatorPen;
    }


    QBrush ParagraphDiagram::bulletFillBrush() const {
        return currentBulletFillBrush;
    }


    QPen ParagraphDiagram::bulletBorderPen() const {
        return currentBulletBorderPen;
    }


    QPoint ParagraphDiagram::leftIndentationCalloutLocation() const {
        return currentLeftIndentationCalloutLocation;
    }


    QPoint ParagraphDiagram::rightIndentationCalloutLocation() const {
        return currentRightIndentationCalloutLocation;
    }


    QPoint ParagraphDiagram::topSpacingCalloutLocation() const {
        return currentTopSpacingCalloutLocation;
    }


    QPoint ParagraphDiagram::bottomSpacingCalloutLocation() const {
        return currentBottomSpacingCalloutLocation;
    }


    QPoint ParagraphDiagram::firstLineIdentationCalloutLocation() const {
        return currentFirstLineIndentationCalloutLocation;
    }


    QPoint ParagraphDiagram::listIndentationCalloutLocation() const {
        return currentListIndentationCalloutLocation;
    }


    QPoint ParagraphDiagram::lineSpacingCalloutLocation() const {
        return currentLineSpacingCalloutLocation;
    }


    QSize ParagraphDiagram::minimumSizeHint() const {
        return QSize(256, 128);
    }


    QSize ParagraphDiagram::sizeHint() const {
        return minimumSizeHint();
    }


    void ParagraphDiagram::setMaximumHeight(unsigned newMaximumHeight) {
        currentMaximumHeight = newMaximumHeight;
        heightAdjustTimer->start(0);
    }


    void ParagraphDiagram::setPageWidth(float newPageWidth) {
        currentPageWidth = newPageWidth;
        heightAdjustTimer->start(0);
    }


    void ParagraphDiagram::setPageLeftMargin(float newPageLeftMargin) {
        currentPageLeftMargin = newPageLeftMargin;
        update();
    }


    void ParagraphDiagram::setPageRightMargin(float newPageRightMargin) {
        currentPageRightMargin = newPageRightMargin;
        update();
    }


    void ParagraphDiagram::setNumberTextLinesPerParagraph(unsigned newNumberTextLinesPerParagraph) {
        currentNumberTextLinesPerParagraph = newNumberTextLinesPerParagraph;
        heightAdjustTimer->start(0);
    }


    void ParagraphDiagram::setTextLineHeight(float newTextLineHeight) {
        currentTextLineHeight = newTextLineHeight;
        heightAdjustTimer->start(0);
    }


    void ParagraphDiagram::setJustification(Justification newJustification) {
        currentJustification = newJustification;
        update();
    }


    void ParagraphDiagram::setLeftJustified() {
        setJustification(Justification::LEFT);
    }


    void ParagraphDiagram::setCentered() {
        setJustification(Justification::CENTER);
    }


    void ParagraphDiagram::setRightJustified() {
        setJustification(Justification::RIGHT);
    }


    void ParagraphDiagram::setJustified() {
        setJustification(Justification::JUSTIFY);
    }


    void ParagraphDiagram::setLeftIndentation(float newLeftIndentation) {
        currentLeftIndentation = newLeftIndentation;
        update();
    }


    void ParagraphDiagram::setRightIndentation(float newRightIndentation) {
        currentRightIndentation = newRightIndentation;
        update();
    }


    void ParagraphDiagram::setTopSpacing(float newTopSpacing) {
        currentTopSpacing = newTopSpacing;
        heightAdjustTimer->start(0);
    }


    void ParagraphDiagram::setBottomSpacing(float newBottomSpacing) {
        currentBottomSpacing = newBottomSpacing;
        heightAdjustTimer->start(0);
    }


    void ParagraphDiagram::setFirstLineIndentation(float newFirstLineIndentation) {
        currentFirstLineIndentation = newFirstLineIndentation;
        update();
    }


    void ParagraphDiagram::setListIndentation(float newListIndentation) {
        currentListIndentation = newListIndentation;
        update();
    }


    void ParagraphDiagram::setShowListEnabled(bool nowShowList) {
        currentShowList = nowShowList;
        update();
    }


    void ParagraphDiagram::setShowListDisabled(bool nowHideList) {
        setShowListEnabled(!nowHideList);
    }


    void ParagraphDiagram::setShowBulletEnabled(bool nowShowBullet) {
        currentShowBullet = nowShowBullet;
        update();
    }


    void ParagraphDiagram::setShowBulletDisabled(bool nowHideBullet) {
        setShowBulletEnabled(!nowHideBullet);
    }


    void ParagraphDiagram::setLineSpacing(float newLineSpacing) {
        currentLineSpacing = newLineSpacing;
        heightAdjustTimer->start(0);
    }


    void ParagraphDiagram::setBackgroundBrush(const QBrush& newBrush) {
        currentBackgroundBrush = newBrush;
        update();
    }


    void ParagraphDiagram::setPageContentsBackgroundBrush(const QBrush& newBrush) {
        currentPageContentsBackgroundBrush = newBrush;
        update();
    }


    void ParagraphDiagram::setMarginBrush(const QBrush& newBrush) {
        currentMarginBrush = newBrush;
        update();
    }


    void ParagraphDiagram::setWidgetBorderLinePen(const QPen& newPen) {
        currentWidgetBorderLinePen = newPen;
        update();
    }


    void ParagraphDiagram::setMarginBorderLinePen(const QPen& newPen) {
        currentMarginBorderLinePen = newPen;
        update();
    }


    void ParagraphDiagram::setExtentsBorderLinePen(const QPen& newPen) {
        currentExtentsBorderLinePen = newPen;
        update();
    }


    void ParagraphDiagram::setAnnotationBrush(const QBrush& newBrush) {
        currentAnnotationBrush = newBrush;
        update();
    }


    void ParagraphDiagram::setAnnotationPen(const QPen& newPen) {
        currentAnnotationPen = newPen;
        update();
    }


    void ParagraphDiagram::setTextLineFillBrush(const QBrush& newBrush) {
        currentTextLineBrush = newBrush;
        update();
    }


    void ParagraphDiagram::setTextLineBorderPen(const QPen& newPen) {
        currentTextLinePen = newPen;
        update();
    }


    void ParagraphDiagram::setBulletFillBrush(const QBrush& newBrush) {
        currentBulletFillBrush = newBrush;
        update();
    }


    void ParagraphDiagram::setBulletBorderPen(const QPen& newPen) {
        currentBulletBorderPen = newPen;
        update();
    }


    void ParagraphDiagram::setTextMarginIndicatorPen(const QPen& newPen) {
        currentTextMarginIndicatorPen = newPen;
        update();
    }


    void ParagraphDiagram::paintEvent(QPaintEvent*) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QRect pageActiveArea;
        float scaleFactor;
        drawBackground(painter, pageActiveArea, scaleFactor);

        QRect previousParagraphExtents;
        QRect bulletExtents;
        QRect centralParagraphFirstLineExtents;
        QRect centralParagraphSecondLineExtents;
        QRect centralParagraphRemainderExtents;
        QRect nextParagraphExtents;
        drawTextLines(
            painter,
            pageActiveArea,
            scaleFactor,
            previousParagraphExtents,
            bulletExtents,
            centralParagraphFirstLineExtents,
            centralParagraphSecondLineExtents,
            centralParagraphRemainderExtents,
            nextParagraphExtents
        );

        QRect    widgetBoundary = QRect(QPoint(0, 0), size());
        unsigned centerX        = (widgetBoundary.left() + widgetBoundary.right()) / 2;

        QPoint newTopSpacingCalloutLocation = drawVerticalDimension(
            painter,
            QPoint(centerX, previousParagraphExtents.bottom()),
            QPoint(centerX, centralParagraphFirstLineExtents.top()),
            Direction::UP,
            false,
            false,
            widgetBoundary
        );

        QPoint newBottomSpacingCalloutLocation = drawVerticalDimension(
            painter,
            QPoint(centerX, centralParagraphRemainderExtents.bottom()),
            QPoint(centerX, nextParagraphExtents.top()),
            Direction::DOWN,
            true,
            true,
            widgetBoundary
        );

        QPoint newLineSpacingCalloutLocation = drawVerticalDimension(
            painter,
            QLine(
                centralParagraphFirstLineExtents.topRight(),
                centralParagraphFirstLineExtents.bottomRight()
            ).center(),
            QLine(
                centralParagraphSecondLineExtents.topRight(),
                centralParagraphSecondLineExtents.bottomRight()
            ).center(),
            Direction::UP,
            true,
            false,
            widgetBoundary
        );

        QPoint rightIndentationPoint = QLine(
            centralParagraphRemainderExtents.topRight(),
            centralParagraphRemainderExtents.bottomRight()
        ).center();

        QPoint rightActiveAreaPoint = QPoint(pageActiveArea.right(), rightIndentationPoint.y());

        QPoint newRightIndentationCalloutLocation = drawHorizontalDimension(
            painter,
            rightIndentationPoint,
            rightActiveAreaPoint,
            Direction::DOWN,
            true,
            true,
            widgetBoundary
        );

        QPoint leftIndentationPoint = bulletExtents.bottomLeft();
        QPoint leftActiveAreaPoint  = QPoint(pageActiveArea.left(), leftIndentationPoint.y());

        QPoint newLeftIndentationCalloutLocation = drawHorizontalDimension(
            painter,
            leftIndentationPoint,
            leftActiveAreaPoint,
            Direction::DOWN,
            true,
            false,
            widgetBoundary
        );

        QPoint newListIndentationCalloutLocation;
        if (currentShowList) {
            QPoint centralParagraphRemainderPoint = centralParagraphRemainderExtents.topLeft();
            newListIndentationCalloutLocation = drawHorizontalDimension(
                painter,
                leftIndentationPoint,
                centralParagraphRemainderPoint,
                Direction::LEFT,
                false,
                false,
                widgetBoundary
            );
        }

        QPoint firstLineIndentationPoint = centralParagraphFirstLineExtents.topLeft();
        QPoint firstLineActiveAreaPoint  = centralParagraphRemainderExtents.topLeft();

        QPoint newFirstLineIndentationCalloutLocation = drawHorizontalDimension(
            painter,
            firstLineIndentationPoint,
            firstLineActiveAreaPoint,
            Direction::UP,
            false,
            true,
            widgetBoundary
        );

        // TODO: Add list indentation

        bool calloutsUpdated = (
               (newTopSpacingCalloutLocation != currentTopSpacingCalloutLocation)
            || (newBottomSpacingCalloutLocation != currentBottomSpacingCalloutLocation)
            || (newLineSpacingCalloutLocation != currentLineSpacingCalloutLocation)
            || (newRightIndentationCalloutLocation != currentRightIndentationCalloutLocation)
            || (newLeftIndentationCalloutLocation != currentLeftIndentationCalloutLocation)
            || (newListIndentationCalloutLocation != currentListIndentationCalloutLocation)
            || (newFirstLineIndentationCalloutLocation != currentFirstLineIndentationCalloutLocation)
        );

        if (calloutsUpdated) {
            currentTopSpacingCalloutLocation           = newTopSpacingCalloutLocation;
            currentBottomSpacingCalloutLocation        = newBottomSpacingCalloutLocation;
            currentLineSpacingCalloutLocation          = newLineSpacingCalloutLocation;
            currentRightIndentationCalloutLocation     = newRightIndentationCalloutLocation;
            currentLeftIndentationCalloutLocation      = newLeftIndentationCalloutLocation;
            currentListIndentationCalloutLocation      = newListIndentationCalloutLocation;
            currentFirstLineIndentationCalloutLocation = newFirstLineIndentationCalloutLocation;

            if (!calloutUpdatedTimer->isActive()) {
                calloutUpdatedTimer->start(0);
            }
        }
    }


    void ParagraphDiagram::resizeEvent(QResizeEvent* event) {
        if (event->size().width() != event->oldSize().width()) {
            heightAdjustTimer->start(0);
        }
    }


    void ParagraphDiagram::adjustHeight() {
        setFixedHeight(std::min(currentMaximumHeight, heightRequired(size().width())));
    }


    void ParagraphDiagram::configureWidget() {
        setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);

        currentMaximumHeight               = static_cast<unsigned>(-1);

        currentPageWidth                   = defaultPageWidth;
        currentPageLeftMargin              = defaultPageLeftMargin;
        currentPageRightMargin             = defaultPageRightMargin;
        currentNumberTextLinesPerParagraph = defaultTextLinesPerParagraph;
        currentTextLineHeight              = defaultTextLineHeight;
        currentJustification               = defaultJustification;
        currentLeftIndentation             = defaultLeftIndentation;
        currentRightIndentation            = defaultRightIndentation;
        currentTopSpacing                  = defaultTopSpacing;
        currentBottomSpacing               = defaultBottomSpacing;
        currentFirstLineIndentation        = defaultFirstLineIndentation;
        currentListIndentation             = defaultListIndentation;
        currentShowList                    = defaultShowList;
        currentShowBullet                  = defaultShowBullet;
        currentLineSpacing                 = defaultLineSpacing;

        currentBackgroundBrush             = QBrush(defaultBackgroundColor);
        currentPageContentsBackgroundBrush = QBrush(defaultPageContentsBackgroundColor);
        currentMarginBrush                 = QBrush(defaultMarginColor);
        currentWidgetBorderLinePen         = QPen(QBrush(defaultWidgetBorderLineColor), defaultWidgetBorderLineWidth);
        currentMarginBorderLinePen         = QPen(QBrush(defaultMarginsBorderLineColor), defaultMarginBorderLineWidth);
        currentExtentsBorderLinePen        = QPen(QBrush(defaultExtentsBorderLineColor), defaultExtentsBorderLineWidth);
        currentAnnotationBrush             = QBrush(defaultAnnotationColor);
        currentAnnotationPen               = QPen(currentAnnotationBrush, defaultAnnotationLineWidth);
        currentTextLineBrush               = QBrush(defaultTextLineFillColor);
        currentTextLinePen                 = QPen(QBrush(defaultTextLineBorderColor), defaultTextLineBorderWidth);
        currentBulletFillBrush             = QBrush(defaultBulletFillColor);
        currentBulletBorderPen             = QPen(QBrush(defaultBulletBorderLineColor), defaultTextLineBorderWidth);
        currentTextMarginIndicatorPen      = QPen(
            QBrush(defaultTextMarginIndicatorColor),
            defaultTextMarginIndicatorWidth
        );

        heightAdjustTimer = new QTimer(this);
        heightAdjustTimer->setSingleShot(true);
        connect(heightAdjustTimer, &QTimer::timeout, this, &ParagraphDiagram::adjustHeight);

        calloutUpdatedTimer = new QTimer(this);
        calloutUpdatedTimer->setSingleShot(true);
        connect(calloutUpdatedTimer, &QTimer::timeout, this, &ParagraphDiagram::calloutLocationsChanged);
    }


    void ParagraphDiagram::drawBackground(QPainter& painter, QRect& pageActiveArea, float& scaleFactor) {
        QSize widgetSize = size();

        QRect pageExtentsRectangle = QRect(
            QPoint(widgetSize.width() * paddingFraction, 0),
            QPoint(widgetSize.width() * (1.0 - paddingFraction), widgetSize.height())
        );

        scaleFactor = pageExtentsRectangle.width() / currentPageWidth;


        QRect pageLeftMarginRectangle(
            pageExtentsRectangle.topLeft(),
            QPoint(pageExtentsRectangle.left() + scaleFactor * currentPageLeftMargin, pageExtentsRectangle.bottom())
        );
        QRect pageRightMarginRectangle(
            QPoint(pageExtentsRectangle.right() - scaleFactor * currentPageRightMargin, pageExtentsRectangle.top()),
            pageExtentsRectangle.bottomRight()
        );
        pageActiveArea = QRect(pageLeftMarginRectangle.topRight(), pageRightMarginRectangle.bottomLeft());

        painter.setPen(Qt::NoPen);
        painter.setBrush(currentBackgroundBrush);
        painter.drawRect(QRect(QPoint(0, 0), pageLeftMarginRectangle.bottomLeft()));
        painter.drawRect(QRect(pageRightMarginRectangle.topRight(), QPoint(widgetSize.width(), widgetSize.height())));

        painter.setBrush(currentMarginBrush);
        painter.drawRect(pageLeftMarginRectangle);
        painter.drawRect(pageRightMarginRectangle);

        painter.setBrush(currentPageContentsBackgroundBrush);
        painter.drawRect(pageActiveArea);

        painter.setPen(currentWidgetBorderLinePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(0, 0, widgetSize.width(), widgetSize.height());

        painter.setPen(currentExtentsBorderLinePen);
        painter.drawLine(pageLeftMarginRectangle.topLeft(), pageLeftMarginRectangle.bottomLeft());
        painter.drawLine(pageRightMarginRectangle.topRight(), pageRightMarginRectangle.bottomRight());

        painter.setPen(currentMarginBorderLinePen);
        painter.drawLine(pageLeftMarginRectangle.topRight(), pageLeftMarginRectangle.bottomRight());
        painter.drawLine(pageRightMarginRectangle.topLeft(), pageRightMarginRectangle.bottomLeft());
    }


    void ParagraphDiagram::drawTextLines(
            QPainter& painter,
            QRect     pageActiveArea,
            float     scaleFactor,
            QRect&    previousParagraphExtents,
            QRect&    bulletExtents,
            QRect&    centralParagraphFirstLineExtents,
            QRect&    centralParagraphSecondLineExtents,
            QRect&    centralParagraphRemainderExtents,
            QRect&    nextParagraphExtents
        ) {
        unsigned textBlockSpacing        = static_cast<unsigned>(scaleFactor * currentTextLineHeight + 0.5);
        unsigned textLineGap             = static_cast<unsigned>(textBlockSpacing * textLineAscentFraction + 0.5);
        unsigned textBlockHeight         = textBlockSpacing - textLineGap;
        unsigned topSpacingAdjustment    = static_cast<unsigned>(scaleFactor * currentTopSpacing + 0.5);
        unsigned bottomSpacingAdjustment = static_cast<unsigned>(scaleFactor * currentBottomSpacing + 0.5);
        unsigned lineSpacingAdjustment   = static_cast<unsigned>(currentLineSpacing * textBlockSpacing + 0.5);

        unsigned currentY                = textLineGap;

        previousParagraphExtents = QRect(
            QPoint(pageActiveArea.left(),  currentY - textLineGap),
            QPoint(
                pageActiveArea.right(),
                currentY + textBlockSpacing * currentNumberTextLinesPerParagraph - textLineGap
            )
        );

        for (unsigned i=0 ; i<currentNumberTextLinesPerParagraph ; ++i) {
            QRect textLineBoundingRectangle(
                QPoint(
                    previousParagraphExtents.left(),
                    currentY
                ),
                QPoint(
                    previousParagraphExtents.right(),
                    currentY + textBlockHeight
                )
            );

            drawTextLine(painter, textLineBoundingRectangle);
            currentY += textBlockSpacing;
        }

        currentY += topSpacingAdjustment + textLineGap;

        float listIndentationAdder = currentShowList ? currentListIndentation : 0.0F;

        int firstLineLeft = (
              static_cast<int>(
                    (  currentFirstLineIndentation
                     + currentLeftIndentation
                     + listIndentationAdder
                    ) * scaleFactor
                  + 0.5
              )
            + pageActiveArea.left()
        );

        int left = (
             static_cast<int>(
                   (currentLeftIndentation + listIndentationAdder) * scaleFactor
                 + 0.5
             )
           + pageActiveArea.left()
        );

        int right = pageActiveArea.right() - static_cast<int>(currentRightIndentation * scaleFactor + 0.5);

        centralParagraphFirstLineExtents = QRect(
            QPoint(firstLineLeft, currentY - textLineGap),
            QPoint(right, currentY + textBlockHeight)
        );

        QRect centralParagraphFirstLine(
            QPoint(firstLineLeft, currentY),
            QPoint(right, currentY + textBlockHeight)
        );

        int bulletLeft = static_cast<int>(currentLeftIndentation * scaleFactor + 0.5) + pageActiveArea.left();

        bulletExtents = QRect(
            QPoint(bulletLeft, centralParagraphFirstLine.top()),
            QPoint(bulletLeft + centralParagraphFirstLine.height(), centralParagraphFirstLine.bottom())
        );

        if (currentShowList && currentShowBullet) {
            painter.setPen(currentBulletBorderPen);
            painter.setBrush(currentBulletFillBrush);
            painter.drawRect(bulletExtents);
        }

        float relativeLineLength = currentJustification == Justification::CENTER ? 0.80F : 0.975F;
        drawTextLine(
            painter,
            centralParagraphFirstLine,
            relativeLineLength,
            currentJustification
        );
        drawTextMargins(painter, centralParagraphFirstLineExtents, currentJustification);

        currentY += lineSpacingAdjustment;

        centralParagraphRemainderExtents = QRect(
            QPoint(left, currentY - textLineGap),
            QPoint(right, currentY + lineSpacingAdjustment * (currentNumberTextLinesPerParagraph - 1) - textLineGap)
        );

        centralParagraphSecondLineExtents = QRect(
            QPoint(left,  currentY - textLineGap),
            QPoint(right, currentY + textBlockHeight)
        );

        for (unsigned i=1 ; i<currentNumberTextLinesPerParagraph ; ++i) {
            QRect textLineBoundingRectangle(
                QPoint(
                    centralParagraphRemainderExtents.left(),
                    currentY
                ),
                QPoint(
                    centralParagraphRemainderExtents.right(),
                    currentY + textBlockHeight
                )
            );

            if (currentJustification == Justification::CENTER) {
                relativeLineLength -= 0.07F;
                if (relativeLineLength < 0.70F) {
                    relativeLineLength += 0.20F;
                }
            } else {
                relativeLineLength -= 0.013F;
                if (relativeLineLength < 0.95) {
                    relativeLineLength += 0.05F;
                }
            }

            drawTextLine(
                painter,
                textLineBoundingRectangle,
                relativeLineLength,
                currentJustification
            );
            currentY += lineSpacingAdjustment;
        }

        drawTextMargins(painter, centralParagraphRemainderExtents, currentJustification);

        currentY += bottomSpacingAdjustment + textLineGap;

        nextParagraphExtents = QRect(
            QPoint(pageActiveArea.left(),  currentY - textLineGap),
            QPoint(
                pageActiveArea.right(),
                currentY + textBlockSpacing * currentNumberTextLinesPerParagraph - textLineGap
            )
        );

        for (unsigned i=0 ; i<currentNumberTextLinesPerParagraph ; ++i) {
            QRect textLineBoundingRectangle(
                QPoint(
                    previousParagraphExtents.left(),
                    currentY
                ),
                QPoint(
                    previousParagraphExtents.right(),
                    currentY + textBlockHeight
                )
            );

            drawTextLine(painter, textLineBoundingRectangle);
            currentY += textBlockSpacing;
        }
    }


    QRect ParagraphDiagram::drawTextLine(
            QPainter&                       painter,
            const QRect&                    textLineBoundingRectangle,
            float                           relativeLength,
            ParagraphDiagram::Justification justification
        ) {
        QRect textRectangle;

        if (justification == Justification::JUSTIFY) {
            textRectangle = textLineBoundingRectangle;
        } else {
            unsigned adjustment = static_cast<unsigned>(
                  (1.0 - relativeLength) * textLineBoundingRectangle.width()
                + 0.5
            );

            switch (justification) {
                case Justification::LEFT: {
                    textRectangle = QRect(
                        textLineBoundingRectangle.topLeft(),
                        QPoint(textLineBoundingRectangle.right() - adjustment, textLineBoundingRectangle.bottom())
                    );

                    break;
                }

                case Justification::CENTER: {
                    textRectangle = QRect(
                        QPoint(textLineBoundingRectangle.left() + adjustment / 2,  textLineBoundingRectangle.top()),
                        QPoint(textLineBoundingRectangle.right() - adjustment / 2, textLineBoundingRectangle.bottom())
                    );

                    break;
                }

                case Justification::RIGHT: {
                    textRectangle = QRect(
                        QPoint(textLineBoundingRectangle.left() + adjustment, textLineBoundingRectangle.top()),
                        textLineBoundingRectangle.bottomRight()
                    );

                    break;
                }

                default: {
                    Q_ASSERT(false);
                }
            }
        }

        painter.setPen(currentTextLinePen);
        painter.setBrush(currentTextLineBrush);
        painter.drawRect(textRectangle);

        return textRectangle;
    }


    void ParagraphDiagram::drawTextMargins(QPainter& painter, const QRect& extents, Justification justification) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(currentTextMarginIndicatorPen);

        QLine leftEdge(extents.topLeft(), extents.bottomLeft());
        QLine rightEdge(extents.topRight(), extents.bottomRight());

        painter.drawLine(leftEdge);
        painter.drawLine(rightEdge);
        painter.drawLine(leftEdge.center(), rightEdge.center());

        if (justification == Justification::CENTER) {
            QLine topEdge(extents.topLeft(), extents.topRight());
            QLine bottomEdge(extents.bottomLeft(), extents.bottomRight());

            painter.drawLine(topEdge.center(), bottomEdge.center());
        }
    }


    QPoint ParagraphDiagram::drawHorizontalDimension(
            QPainter&     painter,
            const QPoint& point1,
            const QPoint& point2,
            Direction     calloutDirection,
            bool          belowPoints,
            bool          preferRightConnection,
            const QRect&  widgetRectangle
        ) {
        QPoint result;

        painter.setPen(currentAnnotationPen);
        painter.setBrush(currentAnnotationBrush);

        QPoint annotationLineGap(0, annotationGap);
        QLine  l1;
        QLine  l2;
        QPoint dimensionLineAdjustment;
        bool   smallDistance;

        if (belowPoints) {
            unsigned dimensionY = std::min(
                std::max(
                    point1.y() + annotationGap + minimumExtensionLineLength,
                    point2.y() + annotationGap + minimumExtensionLineLength
                ),
                widgetRectangle.bottom() - minimumDistanceToEdge
            );

            l1 = QLine(point1 + annotationLineGap, QPoint(point1.x(), dimensionY));
            l2 = QLine(point2 + annotationLineGap, QPoint(point2.x(), dimensionY));

            dimensionLineAdjustment = QPoint(0, -static_cast<int>(dimensionLineOffset));
        } else {
            unsigned dimensionY = std::max(
                std::min(
                    point1.y() - annotationGap - minimumExtensionLineLength,
                    point2.y() - annotationGap - minimumExtensionLineLength
                ),
                widgetRectangle.top() + minimumDistanceToEdge
            );

            l1 = QLine(point1 - annotationLineGap, QPoint(point1.x(), dimensionY));
            l2 = QLine(point2 - annotationLineGap, QPoint(point2.x(), dimensionY));

            dimensionLineAdjustment = QPoint(0, dimensionLineOffset);
        }

        painter.drawLine(l1);
        painter.drawLine(l2);

        QPoint tiePoint = drawDimensionLine(
            painter,
            l1.p2() + dimensionLineAdjustment,
            l2.p2() + dimensionLineAdjustment,
            false,
            preferRightConnection,
            &smallDistance
        );

        if (calloutDirection == Direction::LEFT || calloutDirection == Direction::RIGHT) {
            unsigned calloutX = calloutDirection == Direction::LEFT? widgetRectangle.left(): widgetRectangle.right();

            if ((calloutDirection == Direction::LEFT && (preferRightConnection || !smallDistance))   ||
                (calloutDirection == Direction::RIGHT && (!preferRightConnection || !smallDistance))    ) {
                QPoint nextTiePoint = tiePoint - 3 * dimensionLineAdjustment;
                painter.drawLine(tiePoint, nextTiePoint);
                result = QPoint(calloutX, nextTiePoint.y());
                painter.drawLine(nextTiePoint, result);
            } else {
                result = QPoint(calloutX, tiePoint.y());
                painter.drawLine(tiePoint, result);
            }
        } else {
            unsigned calloutY = calloutDirection == Direction::UP ? widgetRectangle.top() : widgetRectangle.bottom();
            result = QPoint(tiePoint.x(), calloutY);
            painter.drawLine(tiePoint, result);
        }

        return result;
    }


    QPoint ParagraphDiagram::drawVerticalDimension(
            QPainter&     painter,
            const QPoint& point1,
            const QPoint& point2,
            Direction     calloutDirection,
            bool          toRightOf,
            bool          preferBottomConnection,
            const QRect&  widgetRectangle
        ) {
        QPoint result;

        painter.setPen(currentAnnotationPen);
        painter.setBrush(currentAnnotationBrush);

        QPoint annotationLineGap(annotationGap, 0);
        QLine  l1;
        QLine  l2;
        QPoint dimensionLineAdjustment;
        bool   smallDistance;

        if (toRightOf) {
            unsigned dimensionX = std::min(
                std::max(
                    point1.x() + annotationGap + minimumExtensionLineLength,
                    point2.x() + annotationGap + minimumExtensionLineLength
                ),
                widgetRectangle.right() - minimumDistanceToEdge
            );

            l1 = QLine(point1 + annotationLineGap, QPoint(dimensionX, point1.y()));
            l2 = QLine(point2 + annotationLineGap, QPoint(dimensionX, point2.y()));

            dimensionLineAdjustment = QPoint(-static_cast<int>(dimensionLineOffset), 0);
        } else {
            unsigned dimensionX = std::max(
                std::min(
                    point1.x() - annotationGap - minimumExtensionLineLength,
                    point2.x() - annotationGap - minimumExtensionLineLength
                ),
                widgetRectangle.left() + minimumDistanceToEdge
            );

            l1 = QLine(point1 - annotationLineGap, QPoint(dimensionX, point1.y()));
            l2 = QLine(point2 - annotationLineGap, QPoint(dimensionX, point2.y()));

            dimensionLineAdjustment = QPoint(dimensionLineOffset, 0);
        }

        painter.drawLine(l1);
        painter.drawLine(l2);

        QPoint tiePoint = drawDimensionLine(
            painter,
            l1.p2() + dimensionLineAdjustment,
            l2.p2() + dimensionLineAdjustment,
            true,
            preferBottomConnection,
            &smallDistance
        );

        if (calloutDirection == Direction::UP || calloutDirection == Direction::DOWN) {
            unsigned calloutY =   calloutDirection == Direction::UP
                                ? widgetRectangle.top()
                                : widgetRectangle.bottom();

            if ((calloutDirection == Direction::UP && (preferBottomConnection || !smallDistance))    ||
                (calloutDirection == Direction::DOWN && (!preferBottomConnection || !smallDistance))    ) {
                QPoint nextTiePoint = tiePoint - 3 * dimensionLineAdjustment;
                painter.drawLine(tiePoint, nextTiePoint);
                result = QPoint(nextTiePoint.x(), calloutY);
                painter.drawLine(nextTiePoint, result);
            } else {
                result = QPoint(tiePoint.x(), calloutY);
                painter.drawLine(tiePoint, result);
            }
        } else {
            unsigned calloutX = calloutDirection == Direction::LEFT ? widgetRectangle.left() : widgetRectangle.right();
            result = QPoint(calloutX, tiePoint.y());
            painter.drawLine(tiePoint, result);
        }

        return result;
    }


    QPoint ParagraphDiagram::drawDimensionLine(
            QPainter&     painter,
            const QPoint& point1,
            const QPoint& point2,
            bool          preferVertical,
            bool          preferBottomRightConnection,
            bool*         smallDistance
        ) {
        QPoint result;
        bool   isSmallDistance;

        bool horizontal = point1.y() == point2.y();
        bool vertical   = point1.x() == point2.x();

        Q_ASSERT(horizontal || vertical);

        if (horizontal && vertical) {
            vertical   = preferVertical;
            horizontal = !preferVertical;
        }

        painter.setPen(currentAnnotationPen);
        painter.setBrush(currentAnnotationBrush);

        if (vertical) {
            QPoint top    = (point1.y() < point2.y() ? point1 : point2)  + QPoint(0, 1);
            QPoint bottom = (point1.y() >= point2.y() ? point1 : point2) - QPoint(0, 1);

            if (static_cast<unsigned>(bottom.y() - top.y()) >= minimumDimensionLineLength) {
                QLine line(top, bottom);
                painter.drawLine(line);

                drawArrow(painter, top, Direction::UP, false);
                drawArrow(painter, bottom, Direction::DOWN, false);

                result          = line.center();
                isSmallDistance = false;
            } else {
                QPoint tieLineOffset(0, smallDistanceDimensionLineLength);
                if (preferBottomRightConnection) {
                    drawArrow(painter, top    - QPoint(0, 2), Direction::DOWN, true);
                    drawArrow(painter, bottom + QPoint(0, 2), Direction::UP, false);

                    result = bottom + tieLineOffset;
                    painter.drawLine(bottom, result);
                } else {
                    drawArrow(painter, top    - QPoint(0, 2), Direction::DOWN, false);
                    drawArrow(painter, bottom + QPoint(0, 2), Direction::UP, true);

                    result = top - tieLineOffset;
                    painter.drawLine(top, result);
                }

                isSmallDistance = true;
            }
        } else {
            QPoint left  = (point1.x()  < point2.x() ? point1 : point2);
            QPoint right = (point1.x() >= point2.x() ? point1 : point2);

            if (static_cast<unsigned>(right.x() - left.x()) >= minimumDimensionLineLength) {
                QLine line(left, right);
                painter.drawLine(line);

                drawArrow(painter, left + QPoint(1, 0), Direction::LEFT, false);
                drawArrow(painter, right - QPoint(1, 0), Direction::RIGHT, false);

                result          = line.center();
                isSmallDistance = false;
            } else {
                QPoint tieLineOffset(smallDistanceDimensionLineLength, 0);
                if (preferBottomRightConnection) {
                    drawArrow(painter, left  - QPoint(1, 0), Direction::RIGHT, true);
                    drawArrow(painter, right + QPoint(1, 0), Direction::LEFT, false);

                    result = right + tieLineOffset - QPoint(1, 0);
                    painter.drawLine(right - QPoint(1, 0), result);
                } else {
                    drawArrow(painter, left  - QPoint(1, 0), Direction::RIGHT, false);
                    drawArrow(painter, right + QPoint(2, 0), Direction::LEFT, true);

                    result = left - tieLineOffset + QPoint(1, 0);
                    painter.drawLine(left + QPoint(1, 0), result);
                }

                isSmallDistance = true;
            }
        }

        if (smallDistance != Q_NULLPTR) {
            *smallDistance = isSmallDistance;
        }

        return result;
    }


    void ParagraphDiagram::drawArrow(
            QPainter&     painter,
            const QPoint& position,
            Direction     direction,
            bool          includeTail
        ) {
        QPoint points[5];
        QLine  tail;

        points[0] = position;

        switch (direction) {
            case Direction::UP: {
                points[1] = position + QPoint(-6, +7);
                points[2] = position + QPoint(-4, +5);
                points[3] = position + QPoint(+4, +5);
                points[4] = position + QPoint(+6, +7);

                tail = QLine(position + QPoint(0, 5), position + QPoint(0, 9));

                break;
            }

            case Direction::DOWN: {
                points[1] = position + QPoint(+6, -7);
                points[2] = position + QPoint(+4, -5);
                points[3] = position + QPoint(-4, -5);
                points[4] = position + QPoint(-6, -7);

                tail = QLine(position - QPoint(0, 5), position - QPoint(0, 9));

                break;
            }

            case Direction::LEFT: {
                points[1] = position + QPoint(+7, +6);
                points[2] = position + QPoint(+5, +4);
                points[3] = position + QPoint(+5, -4);
                points[4] = position + QPoint(+7, -6);

                tail = QLine(position + QPoint(5, 0), position + QPoint(9, 0));

                break;
            }

            case Direction::RIGHT: {
                points[1] = position + QPoint(-7, -6);
                points[2] = position + QPoint(-5, -4);
                points[3] = position + QPoint(-5, +4);
                points[4] = position + QPoint(-7, +6);

                tail = QLine(position - QPoint(5, 0), position - QPoint(9, 0));

                break;
            }

            default: {
                Q_ASSERT(false);
            }
        }

        painter.drawConvexPolygon(points, 5);

        if (includeTail) {
            painter.drawLine(tail);
        }
    }


    unsigned ParagraphDiagram::heightRequired(unsigned width) const {
        // Note truncation during conversion to integer types must align with code elsewhere in this class.
        unsigned extentsLeft  = static_cast<unsigned>(width * paddingFraction);
        unsigned extentsRight = static_cast<unsigned>(width * (1.0 - paddingFraction));
        float    scaleFactor  = (extentsRight - extentsLeft) / currentPageWidth;

        unsigned textBlockSpacing        = static_cast<unsigned>(scaleFactor * currentTextLineHeight + 0.5);
        unsigned textLineTop             = static_cast<unsigned>(textBlockSpacing * textLineAscentFraction + 0.5);
        unsigned textBlockHeight         = textBlockSpacing - textLineTop;
        unsigned topSpacingAdjustment    = static_cast<unsigned>(scaleFactor * currentTopSpacing + 0.5);
        unsigned bottomSpacingAdjustment = static_cast<unsigned>(scaleFactor * currentBottomSpacing + 0.5);
        unsigned lineSpacingAdjustment   = static_cast<unsigned>(currentLineSpacing * textBlockSpacing + 0.5);

        return (
              2 * textBlockSpacing * currentNumberTextLinesPerParagraph        // previous and next paragraph
            + topSpacingAdjustment                                             // top paragraph spacing
            + textBlockSpacing                                                 // first line
            + lineSpacingAdjustment * (currentNumberTextLinesPerParagraph - 1) // all other lines, central paragraph
            + bottomSpacingAdjustment                                          // bottom paragraph spacing
            + textBlockHeight                                                  // additional gap
        );
    }
}
