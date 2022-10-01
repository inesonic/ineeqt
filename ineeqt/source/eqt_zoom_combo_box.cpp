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
* This file implements the \ref EQt::ZoomComboBox class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QIntValidator>

#include <QList>

#include <cmath>

#include "eqt_combo_box.h"
#include "eqt_zoom_combo_box.h"

namespace EQt {
    static const float zoomValues[] = { 1.0F, 1.5F, 2.0F, 3.3333333F, 4.0F, 5.0F, 6.666666F, 7.5F, 8.0F, 0.0F };

    ZoomComboBox::ZoomComboBox(QWidget* parent):EQt::ComboBox(EQt::ComboBox::ReportedValue::TEXT, parent) {
        configureWidget();
    }


    ZoomComboBox::ZoomComboBox(
            QAction* action,
            QWidget* parent
        ):EQt::ComboBox(
            action,
            EQt::ComboBox::ReportedValue::TEXT,
            parent
        ) {
        configureWidget();
    }


    ZoomComboBox::~ZoomComboBox() {}


    unsigned ZoomComboBox::minimumZoomLevel() const {
        return currentMinimumValue;
    }


    unsigned ZoomComboBox::maximumZoomLevel() const {
        return currentMaximumValue;
    }


    bool ZoomComboBox::includePageWidth() const {
        return currentIncludePageWidth;
    }


    bool ZoomComboBox::excludePageWidth() const {
        return !currentIncludePageWidth;
    }


    bool ZoomComboBox::includeFullPage() const {
        return currentIncludeFullPage;
    }


    bool ZoomComboBox::excludeFullPage() const {
        return !currentIncludeFullPage;
    }


    QList<float> ZoomComboBox::scalers() const {
        return currentScalers;
    }


    bool ZoomComboBox::zoomInAvailable() const {
        int      index       = currentIndex();
        unsigned numberItems = static_cast<unsigned>(count());

        return index >= 0 && static_cast<unsigned>(index) < (numberItems - 1);
    }


    bool ZoomComboBox::zoomOutAvailable() const {
        unsigned numberSpecialItems = unsigned(currentIncludePageWidth) + unsigned(currentIncludeFullPage);
        int      index              = currentIndex();

        return index > static_cast<int>(numberSpecialItems);
    }


    void ZoomComboBox::setMinimumZoom(unsigned newMinimumZoom) {
        currentMinimumValue = newMinimumZoom;
        recalculateZoomValues();
    }


    void ZoomComboBox::setMaximumZoom(unsigned newMaximumZoom) {
        currentMaximumValue = newMaximumZoom;
        recalculateZoomValues();
    }


    void ZoomComboBox::SetZoomRange(unsigned newMinimumValue, unsigned newMaximumValue) {
        currentMinimumValue = newMinimumValue;
        currentMaximumValue = newMaximumValue;

        recalculateZoomValues();
    }


    void ZoomComboBox::zoomIn() {
        int      index       = currentIndex();
        unsigned numberItems = static_cast<unsigned>(count());

        if (index >= 0 && static_cast<unsigned>(index) < (numberItems - 1)) {
            setCurrentIndex(index + 1);
        }
    }


    void ZoomComboBox::zoomOut() {
        unsigned numberSpecialItems = unsigned(currentIncludePageWidth) + unsigned(currentIncludeFullPage);
        int      index              = currentIndex();

        if (index > static_cast<int>(numberSpecialItems)) {
            setCurrentIndex(index - 1);
        }
    }


    void ZoomComboBox::zoomIn(unsigned currentZoom) {
        unsigned index       = unsigned(currentIncludePageWidth) + unsigned(currentIncludeFullPage);
        unsigned numberItems = static_cast<unsigned>(count());

        bool found = false;
        while (index < numberItems && !found) {
            QString text = itemText(index);

            int percentLocation = text.indexOf(tr("%"));
            Q_ASSERT(percentLocation > 0);

            QString       valueString = text.left(percentLocation);
            bool          ok          = true;
            unsigned long value       = valueString.toULong(&ok);
            Q_ASSERT(ok);

            if (value > currentZoom) {
                found = true;
            } else {
                ++index;
            }
        }

        if (found) {
            setCurrentIndex(index);
        }
    }


    void ZoomComboBox::zoomOut(unsigned currentZoom) {
        unsigned numberSpecialItems = unsigned(currentIncludePageWidth) + unsigned(currentIncludeFullPage);
        unsigned numberItems        = static_cast<unsigned>(count());

        if (numberItems > numberSpecialItems) {
            unsigned      index = numberItems;
            unsigned long value;

            do {
                --index;
                QString text = itemText(index);

                int percentLocation = text.indexOf(tr("%"));
                Q_ASSERT(percentLocation > 0);

                QString valueString = text.left(percentLocation);
                bool    ok          = true;
                value = valueString.toULong(&ok);
                Q_ASSERT(ok);
            } while (value >= currentZoom && index > numberSpecialItems);

            if (value < currentZoom) {
                setCurrentIndex(index);
            }
        }
    }


    void ZoomComboBox::setPageWidthStringIncluded(bool includePageWidth) {
        currentIncludePageWidth = includePageWidth;
        recalculateZoomValues();
    }


    void ZoomComboBox::setPageWidthStringExcluded(bool excludePageWidth) {
        setPageWidthStringIncluded(!excludePageWidth);
    }


    void ZoomComboBox::setFullPageStringIncluded(bool includeFullPage) {
        currentIncludeFullPage = includeFullPage;
        recalculateZoomValues();
    }


    void ZoomComboBox::setFullPageStringExcluded(bool excludeFullPage) {
        setFullPageStringIncluded(!excludeFullPage);
    }


    void ZoomComboBox::setScalers(const QList<float>& newScalers) {
        currentScalers = newScalers;
        recalculateZoomValues();
    }


    void ZoomComboBox::configureWidget() {
        setDuplicatesEnabled(false);
        setEditable(true);
        setFrame(true);
        setInsertPolicy(QComboBox::NoInsert);
        setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        currentMinimumValue     = defaultMinimumValue;
        currentMaximumValue     = defaultMaximumValue;
        currentIncludeFullPage  = true;
        currentIncludePageWidth = true;

        const float* scaler = zoomValues;
        while (*scaler != 0.0) {
            currentScalers << *scaler;
            ++scaler;
        }

        recalculateZoomValues();
    }


    void ZoomComboBox::recalculateZoomValues() {
        QStringList items;

        if (includePageWidth()) {
            items << tr("Page Width");
        }

        if (includeFullPage()) {
            items << tr("Full Page");
        }

        unsigned startPowerOf10 = static_cast<unsigned long>(std::floor(std::log10(currentMinimumValue)) + 0.5);
        unsigned endPowerOf10   = static_cast<unsigned long>(std::ceil(std::log10(currentMaximumValue)) + 0.5);

        for (unsigned powerOf10=startPowerOf10 ; powerOf10<=endPowerOf10 ; ++powerOf10) {
            double multiplier = std::pow(10, powerOf10);

            for (  QList<float>::const_iterator it  = currentScalers.constBegin(),
                                                end = currentScalers.constEnd()
                 ; it!=end
                 ; ++it
                ) {
                float scaler = *it;
                unsigned value = static_cast<unsigned>(multiplier * scaler + 0.000000001);

                if (value >= currentMinimumValue && value <= currentMaximumValue) {
                    items << tr("%1%").arg(QString::number(value));
                }
            }
        }

        setValidator(new QIntValidator(currentMinimumValue, currentMaximumValue, this));

        clear();
        addItems(items);
    }
}
