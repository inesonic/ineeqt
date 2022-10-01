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
* This file implements the \ref EQt::ParagraphDimensionWidget class.
***********************************************************************************************************************/

#include <QWidget>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QMargins>
#include <QSizePolicy>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QTimer>

#include <cmath>

#include "eqt_dimension_line_edit.h"
#include "eqt_paragraph_diagram.h"
#include "eqt_paragraph_dimension_widget.h"

namespace EQt {
    /*
     * MacOS includes additional padding in the vertical height that I can-not account for.  For now we work around
     * this by expicitly adding padding needed based on measurements.
     */
    #if (defined(Q_OS_WIN) || defined(Q_OS_LINUX))

        const unsigned ParagraphDimensionWidget::platformAdditionalPadding = 0;

    #elif (defined(Q_OS_DARWIN))

        const unsigned ParagraphDimensionWidget::platformAdditionalPadding = 22;

    #else

        #error Unknown platform.

    #endif

    ParagraphDimensionWidget::ParagraphDimensionWidget(QWidget* parent):QWidget(parent) {
        configureWidget();
    }


    ParagraphDimensionWidget::~ParagraphDimensionWidget() {}


    unsigned ParagraphDimensionWidget::maximumHeight() const {
        return currentMaximumHeight;
    }


    bool ParagraphDimensionWidget::listControlsShown() const {
        return currentParagraphDiagram->showList();
    }


    bool ParagraphDimensionWidget::listControlsHidden() const {
        return !listControlsShown();
    }


    bool ParagraphDimensionWidget::bulletShown() const {
        return currentParagraphDiagram->showBullet();
    }


    bool ParagraphDimensionWidget::bulletHidden() const {
        return !bulletShown();
    }


    float ParagraphDimensionWidget::pageWidth() const {
        return currentParagraphDiagram->pageWidth();
    }


    float ParagraphDimensionWidget::pageLeftMargin() const {
        return currentParagraphDiagram->pageLeftMargin();
    }


    float ParagraphDimensionWidget::pageRightMargin() const {
        return currentParagraphDiagram->pageRightMargin();
    }


    ParagraphDimensionWidget::Justification ParagraphDimensionWidget::justification() const {
        return currentParagraphDiagram->justification();
    }


    float ParagraphDimensionWidget::leftIndentation() const {
        return currentParagraphDiagram->leftIndentation();
    }


    float ParagraphDimensionWidget::rightIndentation() const {
        return currentParagraphDiagram->rightIndentation();
    }


    float ParagraphDimensionWidget::topSpacing() const {
        return currentParagraphDiagram->topSpacing();
    }


    float ParagraphDimensionWidget::bottomSpacing() const {
        return currentParagraphDiagram->bottomSpacing();
    }


    float ParagraphDimensionWidget::listIndentation() const {
        return currentParagraphDiagram->listIndentation();
    }


    float ParagraphDimensionWidget::firstLineIndentation() const {
        return currentParagraphDiagram->firstLineIndentation();
    }


    float ParagraphDimensionWidget::lineSpacing() const {
        return currentParagraphDiagram->lineSpacing();
    }


    bool ParagraphDimensionWidget::isLeftIndentationClear() const {
        return control<DimensionLineEdit>(leftIndentationIndex)->text().isEmpty();
    }


    bool ParagraphDimensionWidget::isRightIndentationClear() const {
        return control<DimensionLineEdit>(rightIndentationIndex)->text().isEmpty();
    }


    bool ParagraphDimensionWidget::isTopSpacingClear() const {
        return control<DimensionLineEdit>(topSpacingIndex)->text().isEmpty();
    }


    bool ParagraphDimensionWidget::isBottomSpacingClear() const {
        return control<DimensionLineEdit>(bottomSpacingIndex)->text().isEmpty();
    }


    bool ParagraphDimensionWidget::isListIndentationClear() const {
        return control<DimensionLineEdit>(listIndentationIndex)->text().isEmpty();
    }


    bool ParagraphDimensionWidget::isFirstLineIndentationClear() const {
        return control<DimensionLineEdit>(firstLineIndentationIndex)->text().isEmpty();
    }


    bool ParagraphDimensionWidget::isLineSpacingClear() const {
        QVariant variant = control<QComboBox>(lineSpacingIndex)->currentData();
        return !variant.isValid();
    }


    bool ParagraphDimensionWidget::isLeftIndentationSet() const {
        return !isLeftIndentationClear();
    }


    bool ParagraphDimensionWidget::isRightIndentationSet() const {
        return !isRightIndentationClear();
    }


    bool ParagraphDimensionWidget::isTopSpacingSet() const {
        return !isTopSpacingClear();
    }


    bool ParagraphDimensionWidget::isBottomSpacingSet() const {
        return !isBottomSpacingClear();
    }


    bool ParagraphDimensionWidget::isListIndentationSet() const {
        return !isListIndentationClear();
    }


    bool ParagraphDimensionWidget::isFirstLineIndentationSet() const {
        return !isFirstLineIndentationClear();
    }


    bool ParagraphDimensionWidget::isLineSpacingSet() const {
        return !isLineSpacingClear();
    }


    void ParagraphDimensionWidget::setMaximumHeight(unsigned newMaximumHeight) {
        currentMaximumHeight = newMaximumHeight;
        enforceMaximumHeight();
    }


    void ParagraphDimensionWidget::setParagraphDiagram(ParagraphDiagram* newParagraphDiagram) {
        if (currentParagraphDiagram != Q_NULLPTR) {
            disconnect(
                currentParagraphDiagram,
                &ParagraphDiagram::calloutLocationsChanged,
                this,
                &ParagraphDimensionWidget::calloutLocationsUpdated
            );

            delete currentParagraphDiagram;
        }

        currentParagraphDiagram = newParagraphDiagram;
        currentParagraphDiagram->setParent(this);

        connect(
            currentParagraphDiagram,
            &ParagraphDiagram::calloutLocationsChanged,
            this,
            &ParagraphDimensionWidget::calloutLocationsUpdated
        );

        currentParagraphDiagramLayout->addWidget(currentParagraphDiagram);
    }


    ParagraphDiagram* ParagraphDimensionWidget::paragraphDiagram() const {
        return currentParagraphDiagram;
    }


    void ParagraphDimensionWidget::setFirstLineIndentationLineEdit(DimensionLineEdit* newDimensionLineEdit) {
        setControl(
            newDimensionLineEdit,
            firstLineIndentationIndex,
            &DimensionLineEdit::valueChanged,
            &ParagraphDimensionWidget::firstLineIndentationUpdated
        );
    }


    DimensionLineEdit* ParagraphDimensionWidget::firstLineIndentationLineEdit() const {
        return control<DimensionLineEdit>(firstLineIndentationIndex);
    }


    void ParagraphDimensionWidget::setListIndentationLineEdit(DimensionLineEdit* newDimensionLineEdit) {
        setControl(
            newDimensionLineEdit,
            listIndentationIndex,
            &DimensionLineEdit::valueChanged,
            &ParagraphDimensionWidget::listIndentationUpdated
        );
    }


    DimensionLineEdit* ParagraphDimensionWidget::listIndentationLineEdit() const {
        return control<DimensionLineEdit>(listIndentationIndex);
    }


    void ParagraphDimensionWidget::setLeftIndentationLineEdit(DimensionLineEdit* newDimensionLineEdit) {
        setControl(
            newDimensionLineEdit,
            leftIndentationIndex,
            &DimensionLineEdit::valueChanged,
            &ParagraphDimensionWidget::leftIndentationUpdated
        );
    }


    DimensionLineEdit* ParagraphDimensionWidget::leftIndentationLineEdit() const {
        return control<DimensionLineEdit>(leftIndentationIndex);
    }


    void ParagraphDimensionWidget::setRightIndentationLineEdit(DimensionLineEdit* newDimensionLineEdit) {
        setControl(
            newDimensionLineEdit,
            rightIndentationIndex,
            &DimensionLineEdit::valueChanged,
            &ParagraphDimensionWidget::rightIndentationUpdated
        );
    }


    DimensionLineEdit* ParagraphDimensionWidget::rightIndentationLineEdit() const {
        return control<DimensionLineEdit>(rightIndentationIndex);
    }


    void ParagraphDimensionWidget::setTopSpacingLineEdit(DimensionLineEdit* newDimensionLineEdit) {
        setControl(
            newDimensionLineEdit,
            topSpacingIndex,
            &DimensionLineEdit::valueChanged,
            &ParagraphDimensionWidget::topSpacingUpdated
        );
    }


    DimensionLineEdit* ParagraphDimensionWidget::topSpacingLineEdit() const {
        return control<DimensionLineEdit>(topSpacingIndex);
    }


    void ParagraphDimensionWidget::setBottomSpacingLineEdit(DimensionLineEdit* newDimensionLineEdit) {
        setControl(
            newDimensionLineEdit,
            bottomSpacingIndex,
            &DimensionLineEdit::valueChanged,
            &ParagraphDimensionWidget::bottomSpacingUpdated
        );
    }


    DimensionLineEdit* ParagraphDimensionWidget::bottomSpacingLineEdit() const {
        return control<DimensionLineEdit>(bottomSpacingIndex);
    }


    void ParagraphDimensionWidget::setLineSpacingComboBox(QComboBox* newComboBox) {
        setControl(
            newComboBox,
            lineSpacingIndex,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            &ParagraphDimensionWidget::lineSpacingUpdated
        );
    }


    QComboBox* ParagraphDimensionWidget::lineSpacingComboBox() const {
        return control<QComboBox>(lineSpacingIndex);
    }


    void ParagraphDimensionWidget::setListControlsShown(bool nowShow) {
        currentParagraphDiagram->setShowListEnabled(nowShow);
        control<DimensionLineEdit>(listIndentationIndex)->setVisible(nowShow);
        labels[listIndentationIndex]->setVisible(nowShow);
    }


    void ParagraphDimensionWidget::setListControlsHidden(bool nowHidden) {
        setListControlsShown(!nowHidden);
    }


    void ParagraphDimensionWidget::setBulletVisible(bool nowVisible) {
        currentParagraphDiagram->setShowBulletEnabled(nowVisible);
    }


    void ParagraphDimensionWidget::setBulletHidden(bool nowHidden) {
        currentParagraphDiagram->setShowBulletDisabled(nowHidden);
    }


    void ParagraphDimensionWidget::setPageWidth(float newPageWidth) {
        currentParagraphDiagram->setPageWidth(newPageWidth);
    }


    void ParagraphDimensionWidget::setPageLeftMargin(float newPageLeftMargin) {
        currentParagraphDiagram->setPageLeftMargin(newPageLeftMargin);
    }


    void ParagraphDimensionWidget::setPageRightMargin(float newPageRightMargin) {
        currentParagraphDiagram->setPageRightMargin(newPageRightMargin);
    }


    void ParagraphDimensionWidget::setJustification(ParagraphDimensionWidget::Justification newJustification) {
        currentParagraphDiagram->setJustification(newJustification);
    }


    void ParagraphDimensionWidget::setLeftJustified() {
        currentParagraphDiagram->setLeftJustified();
    }


    void ParagraphDimensionWidget::setCentered() {
        currentParagraphDiagram->setCentered();
    }


    void ParagraphDimensionWidget::setRightJustified() {
        currentParagraphDiagram->setRightJustified();
    }


    void ParagraphDimensionWidget::setJustified() {
        currentParagraphDiagram->setJustified();
    }


    void ParagraphDimensionWidget::setLeftIndentation(float newLeftIndentation) {
        currentParagraphDiagram->setLeftIndentation(newLeftIndentation);
        control<DimensionLineEdit>(leftIndentationIndex)->setValue(newLeftIndentation);
    }


    void ParagraphDimensionWidget::setRightIndentation(float newRightIndentation) {
        currentParagraphDiagram->setRightIndentation(newRightIndentation);
        control<DimensionLineEdit>(rightIndentationIndex)->setValue(newRightIndentation);
    }


    void ParagraphDimensionWidget::setTopSpacing(float newTopSpacing) {
        currentParagraphDiagram->setTopSpacing(newTopSpacing);
        control<DimensionLineEdit>(topSpacingIndex)->setValue(newTopSpacing);
    }


    void ParagraphDimensionWidget::setBottomSpacing(float newBottomSpacing) {
        currentParagraphDiagram->setBottomSpacing(newBottomSpacing);
        control<DimensionLineEdit>(bottomSpacingIndex)->setValue(newBottomSpacing);
    }


    void ParagraphDimensionWidget::setFirstLineIndentation(float newFirstLineIndentation) {
        currentParagraphDiagram->setFirstLineIndentation(newFirstLineIndentation);
        control<DimensionLineEdit>(firstLineIndentationIndex)->setValue(newFirstLineIndentation);
    }


    void ParagraphDimensionWidget::setListIndentation(float newListIndentation) {
        currentParagraphDiagram->setListIndentation(newListIndentation);
        control<DimensionLineEdit>(listIndentationIndex)->setValue(newListIndentation);
    }


    void ParagraphDimensionWidget::setLineSpacing(float newLineSpacing) {
        currentParagraphDiagram->setLineSpacing(newLineSpacing);

        unsigned bestIndex = 0;
        float    bestValue = -1;
        QComboBox* comboBox = control<QComboBox>(lineSpacingIndex);
        for (unsigned i=0 ; i<static_cast<unsigned>(comboBox->count()) ; ++i) {
            QVariant variant = comboBox->itemData(i);
            if (variant.isValid()) {
                bool  ok;
                float itemValue = variant.toFloat(&ok);

                if (ok                                                                             &&
                    (bestValue < 0U                                                           ||
                     std::fabs(itemValue - newLineSpacing) < std::fabs(itemValue - bestValue)    )    ) {
                    bestIndex = i;
                    bestValue = itemValue;
                }
            }
        }

        comboBox->setCurrentIndex(bestIndex);
    }


    void ParagraphDimensionWidget::clearLeftIndentation() {
        currentParagraphDiagram->setLeftIndentation(ParagraphDiagram::defaultLeftIndentation);
        control<DimensionLineEdit>(leftIndentationIndex)->clear();
    }


    void ParagraphDimensionWidget::clearRightIndentation() {
        currentParagraphDiagram->setRightIndentation(ParagraphDiagram::defaultRightIndentation);
        control<DimensionLineEdit>(rightIndentationIndex)->clear();
    }


    void ParagraphDimensionWidget::clearTopSpacing() {
        currentParagraphDiagram->setTopSpacing(ParagraphDiagram::defaultTopSpacing);
        control<DimensionLineEdit>(topSpacingIndex)->clear();
    }


    void ParagraphDimensionWidget::clearBottomSpacing() {
        currentParagraphDiagram->setBottomSpacing(ParagraphDiagram::defaultBottomSpacing);
        control<DimensionLineEdit>(bottomSpacingIndex)->clear();
    }


    void ParagraphDimensionWidget::clearFirstLineIndentation() {
        currentParagraphDiagram->setFirstLineIndentation(ParagraphDiagram::defaultFirstLineIndentation);
        control<DimensionLineEdit>(firstLineIndentation())->clear();
    }


    void ParagraphDimensionWidget::clearListIndentation() {
        currentParagraphDiagram->setListIndentation(ParagraphDiagram::defaultListIndentation);
        control<DimensionLineEdit>(listIndentationIndex)->clear();
    }


    void ParagraphDimensionWidget::clearLineSpacing() {
        QComboBox* comboBox = control<QComboBox>(lineSpacingIndex);
        comboBox->clearEditText();
    }


    void ParagraphDimensionWidget::calloutLocationsUpdated() {
        if (currentParagraphDiagram->maximumHeight() == static_cast<unsigned>(-1)) {
            enforceMaximumHeight();
        }

        QPoint positions[7] = {
            currentParagraphDiagram->firstLineIdentationCalloutLocation(), // 0
            currentParagraphDiagram->topSpacingCalloutLocation(),          // 1
            currentParagraphDiagram->lineSpacingCalloutLocation(),         // 2
            currentParagraphDiagram->leftIndentationCalloutLocation(),     // 3
            currentParagraphDiagram->bottomSpacingCalloutLocation(),       // 4
            currentParagraphDiagram->rightIndentationCalloutLocation(),    // 5
            currentParagraphDiagram->listIndentationCalloutLocation()      // 7
        };

        int leftEdge  = currentParagraphDiagram->geometry().left();
        int rightEdge = currentParagraphDiagram->geometry().right();
        int xOffset   = leftEdge;

        unsigned groupHeights[2] = { 0U, 0U };

        for (unsigned i=0 ; i<6 ; ++i) {
            QVBoxLayout* layout                     = layouts[i];
            QWidget*     controlWidget              = controls[i];
            QLabel*      label                      = labels[i];
            QWidget*     spacerWidget               = spacers[i];
            int          controlWidth               = layout->geometry().width();
            int          halfWidth                  = controlWidth / 2;
            int          desiredControlPosition     = currentParagraphDiagram->mapToParent(positions[i]).x();

            int desiredControlLeftPosition = std::max(
                std::min(desiredControlPosition - halfWidth, rightEdge - controlWidth),
                leftEdge
            );

            if (i==3) {
                xOffset = leftEdge;
            }

            int spacerWidgetWidth = std::max(desiredControlLeftPosition - xOffset, static_cast<int>(minimumPadding));
            spacerWidget->setFixedWidth(spacerWidgetWidth);

            xOffset += spacerWidgetWidth + controlWidth;

            unsigned height = (
                  layout->contentsMargins().top()
                + controlWidget->height()
                + layout->spacing()
                + label->height()
                + layout->contentsMargins().bottom()
            );

            if (height > groupHeights[i/3]) {
                groupHeights[i/3] = height;
            }
        }

        unsigned listIndentationSpacerHeight = static_cast<unsigned>(
              positions[listIndentationIndex].y()
            - control<DimensionLineEdit>(listIndentationIndex)->height() / 2
        );

        spacers[listIndentationIndex]->setFixedHeight(listIndentationSpacerHeight);

        QGridLayout* mainLayout = dynamic_cast<QGridLayout*>(layout());
        unsigned     newFixedHeight = (
              mainLayout->contentsMargins().top()
            + 2 * mainLayout->spacing()
            + groupHeights[0]
            + currentParagraphDiagram->height()
            + groupHeights[1]
            + mainLayout->contentsMargins().bottom()
            + platformAdditionalPadding
        );

        if (static_cast<unsigned>(height()) != newFixedHeight) {
            setFixedHeight(newFixedHeight);
            emit geometryChanged();
        }
    }


    void ParagraphDimensionWidget::firstLineIndentationUpdated(double newValue) {
        currentParagraphDiagram->setFirstLineIndentation(newValue);
        emit valueChanged();
    }


    void ParagraphDimensionWidget::listIndentationUpdated(double newValue) {
        currentParagraphDiagram->setListIndentation(newValue);
        emit valueChanged();
    }


    void ParagraphDimensionWidget::leftIndentationUpdated(double newValue) {
        currentParagraphDiagram->setLeftIndentation(newValue);
        emit valueChanged();
    }


    void ParagraphDimensionWidget::rightIndentationUpdated(double newValue) {
        currentParagraphDiagram->setRightIndentation(newValue);
        emit valueChanged();
    }


    void ParagraphDimensionWidget::topSpacingUpdated(double newValue) {
        currentParagraphDiagram->setTopSpacing(newValue);
        emit valueChanged();
    }


    void ParagraphDimensionWidget::bottomSpacingUpdated(double newValue) {
        currentParagraphDiagram->setBottomSpacing(newValue);
        emit valueChanged();
    }


    void ParagraphDimensionWidget::lineSpacingUpdated(int newValue) {
        QVariant variant = control<QComboBox>(lineSpacingIndex)->itemData(newValue);
        bool     ok;
        float    lineSpacing = variant.toFloat(&ok);

        if (ok) {
            currentParagraphDiagram->setLineSpacing(lineSpacing);
        }

        emit valueChanged();
    }


    void ParagraphDimensionWidget::configureWidget() {
        setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
        buildLayout();

        currentMaximumHeight = static_cast<unsigned>(-1);

        currentParagraphDiagram = Q_NULLPTR;
        setParagraphDiagram(new ParagraphDiagram);

        setFirstLineIndentationLineEdit(new DimensionLineEdit);
        setListIndentationLineEdit(new DimensionLineEdit);
        setLeftIndentationLineEdit(new DimensionLineEdit);
        setRightIndentationLineEdit(new DimensionLineEdit);
        setTopSpacingLineEdit(new DimensionLineEdit);
        setBottomSpacingLineEdit(new DimensionLineEdit);
        setLineSpacingComboBox(new QComboBox);

        for (unsigned i=0 ; i<6 ; ++i) {
            if (i == lineSpacingIndex) {
                QComboBox* comboBox = control<QComboBox>(i);
                comboBox->setEditable(false);
                comboBox->addItem(tr("Single Spacing"), QVariant(1.0F));
                comboBox->addItem(tr("1%1 Line Spacing").arg(QChar(0xBC)), QVariant(1.25F));
                comboBox->addItem(tr("1%1 Line Spacing").arg(QChar(0xBD)), QVariant(1.5F));
                comboBox->addItem(tr("Double Spacing"), QVariant(2.0F));
                comboBox->addItem(tr("2%1 Line Spacing").arg(QChar(0xBD)), QVariant(2.5F));
                comboBox->addItem(tr("Triple Line Spacing"), QVariant(3.0F));
            } else {
                DimensionLineEdit* dimensionLineEdit = control<DimensionLineEdit>(i);
                dimensionLineEdit->setMinimumValue(0);
            }
        }

        setListControlsShown(ParagraphDiagram::defaultShowList);
        setBulletVisible(ParagraphDiagram::defaultShowBullet);
    }


    void ParagraphDimensionWidget::buildLayout() {
        const QString fieldNames[7] = {
            tr("First Line"),        // 0
            tr("Top Spacing"),       // 1
            tr("Line Spacing"),      // 2
            tr("Left Indentation"),  // 3
            tr("Bottom Spacing"),    // 4
            tr("Right Indentation"), // 5
            tr("List Indentation")   // 6
        };
        QGridLayout* mainLayout = new QGridLayout;
        setLayout(mainLayout);

        mainLayout->setColumnStretch(0, 0);
        mainLayout->setColumnStretch(1, 1);

        QHBoxLayout* layoutGroups[3];
        for (unsigned index=0 ; index<3 ; ++index) {
            QHBoxLayout* newLayout = new QHBoxLayout;
            newLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

            mainLayout->addLayout(newLayout, index, 1, 1, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGroups[index] = newLayout;
        }

        currentParagraphDiagramLayout = layoutGroups[1];

        for (unsigned i=0 ; i<3 ; i += 2) {
            QHBoxLayout* layout = layoutGroups[i];
            QMargins     margins = layout->contentsMargins();

            margins.setLeft(0);
            margins.setRight(0);
            layout->setContentsMargins(margins);
            layout->setSpacing(0);
        }

        for (unsigned i=0 ; i<6 ; ++i) {
            QVBoxLayout* newLayout = new QVBoxLayout;
            newLayout->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

            QLabel* newLabel = new QLabel(fieldNames[i]);
            newLayout->addWidget(newLabel);

            QWidget* newSpacer = new QWidget;
            newSpacer->setFixedHeight(1);
            newSpacer->setFixedWidth(1);

            layouts.append(newLayout);
            labels.append(newLabel);
            spacers.append(newSpacer);

            unsigned     layoutGroup = 2U * (i / 3U);
            QHBoxLayout* groupLayout = layoutGroups[layoutGroup];
            groupLayout->addWidget(newSpacer);
            groupLayout->addLayout(newLayout);
        }

        QVBoxLayout* leftVerticalLayout = new QVBoxLayout;
        leftVerticalLayout->setSpacing(0);
        leftVerticalLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
        mainLayout->addLayout(leftVerticalLayout, 1, 0);

//        QWidget* spacer = new QWidget;
//        spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//        spacer->setFixedHeight(1);
//        leftVerticalLayout->addWidget(spacer);

        QLabel* label = new QLabel(fieldNames[listIndentationIndex]);
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        label->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        leftVerticalLayout->addWidget(label);

        QVBoxLayout* listIndentationLayout = new QVBoxLayout;
        listIndentationLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
        leftVerticalLayout->addLayout(listIndentationLayout);

//        QVBoxLayout* listLabelLayout = new QVBoxLayout;
//        listLabelLayout->setContentsMargins(0, 0, 0, 0);
//        listLabelLayout->setSpacing(0);
//        spacer->setLayout(listLabelLayout);
//        listLabelLayout->addWidget(listIndentationLabel, Qt::AlignRight | Qt::AlignBottom);

        layouts.append(listIndentationLayout);
        labels.append(label); // Note we use the label as a spacer and as a label.
        spacers.append(label);

        layoutGroups[0]->addStretch(1);
        layoutGroups[2]->addStretch(1);
        leftVerticalLayout->addStretch(1);
    }


    void ParagraphDimensionWidget::enforceMaximumHeight() {
        unsigned topLayoutHeight    = 0;
        unsigned bottomLayoutHeight = 0;
        for (unsigned i=0 ; i<3 ; ++i) {
            topLayoutHeight    = std::max(static_cast<unsigned>(layouts[i+0]->geometry().height()), topLayoutHeight);
            bottomLayoutHeight = std::max(static_cast<unsigned>(layouts[i+3]->geometry().height()), bottomLayoutHeight);
        }

        QGridLayout* mainLayout = dynamic_cast<QGridLayout*>(layout());

        unsigned maximumDiagramHeight = (
              currentMaximumHeight
            - mainLayout->contentsMargins().top()
            - 2* mainLayout->spacing()
            - topLayoutHeight
            - bottomLayoutHeight
            + mainLayout->contentsMargins().bottom()
            - platformAdditionalPadding
        );

        currentParagraphDiagram->setMaximumHeight(maximumDiagramHeight);
    }
}
