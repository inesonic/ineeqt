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
* This file implements the \ref EQt::StackingLayout class.
***********************************************************************************************************************/

#include <QLayoutItem>
#include <QLayout>
#include <QWidget>
#include <QList>
#include <QSet>
#include <QGridLayout>

#include "eqt_stacking_layout.h"

/***********************************************************************************************************************
 * EQt::StackingLayout::ItemData
 */

namespace EQt {
    void EQt::StackingLayout::ItemData::setItem(QLayoutItem* item, bool deleteExisting) {
        if (deleteExisting && currentItem != nullptr && currentItem != item) {
            delete currentItem;
        }

        currentItem = item;
    }
}

/***********************************************************************************************************************
 * EQt::StackingLayout
 */

namespace EQt {
    StackingLayout::StackingLayout() {
        currentFixedDimension = FixedDimension::HEIGHT;
        currentMaximumSlots   = 2;
    }


    StackingLayout::StackingLayout(QWidget* parent):QGridLayout(parent) {
        currentFixedDimension = FixedDimension::HEIGHT;
        currentMaximumSlots   = 2;

    }


    StackingLayout::StackingLayout(StackingLayout::FixedDimension fixedDimension, unsigned numberSlots) {
        currentFixedDimension = fixedDimension;
        currentMaximumSlots   = numberSlots;
    }


    StackingLayout::StackingLayout(
            StackingLayout::FixedDimension fixedDimension,
            unsigned                       numberSlots,
            QWidget*                       parent
        ):QGridLayout(
            parent
        ) {
        currentFixedDimension = fixedDimension;
        currentMaximumSlots   = numberSlots;
    }


    StackingLayout::~StackingLayout() {}


    void StackingLayout::setFixedDimension(StackingLayout::FixedDimension newFixedDimension) {
        currentFixedDimension = newFixedDimension;
    }


    StackingLayout::FixedDimension StackingLayout::fixedDimension() const {
        return currentFixedDimension;
    }


    void StackingLayout::setMaximumSlotsInFixedDimension(unsigned numberFixedSlots) {
        currentMaximumSlots = numberFixedSlots;
    }


    unsigned StackingLayout::maximumSlotsInFixedDimension() const {
        return currentMaximumSlots;
    }


    void StackingLayout::appendItem(
            QLayoutItem*  item,
            unsigned      rowSpan,
            unsigned      columnSpan,
            Qt::Alignment alignment
        ) {
        ItemPosition position = findAndAllocatePosition(rowSpan, columnSpan);
        addItem(item, position.row(), position.column(), rowSpan, columnSpan, alignment);
    }


    void StackingLayout::appendLayout(
            QLayout*      layout,
            unsigned      rowSpan,
            unsigned      columnSpan,
            Qt::Alignment alignment
        ) {
        ItemPosition position = findAndAllocatePosition(rowSpan, columnSpan);
        addLayout(layout, position.row(), position.column(), rowSpan, columnSpan, alignment);
    }


    void StackingLayout::appendWidget(
            QWidget*      widget,
            unsigned      rowSpan,
            unsigned      columnSpan,
            Qt::Alignment alignment
        ) {
        ItemPosition position = findAndAllocatePosition(rowSpan, columnSpan);
        addWidget(widget, position.row(), position.column(), rowSpan, columnSpan, alignment);
    }


    void StackingLayout::restack() {
        QList<ItemData> items = stackedItems();

        for (QList<ItemData>::const_iterator it=items.constBegin(),end=items.constEnd() ; it!=end ; ++it) {
            removeItem(it->item());
        }

        availableSlots.clear();

        for (QList<ItemData>::const_iterator it=items.constBegin(),end=items.constEnd() ; it!=end ; ++it) {
            appendItem(it->item(), it->rowSpan(), it->columnSpan(), it->item()->alignment());
        }
    }


    StackingLayout::ItemPosition StackingLayout::findAndAllocatePosition(unsigned rowSpan, unsigned columnSpan) {
        unsigned requiredFixed;
        unsigned requiredUnfixed;

        if (currentFixedDimension == FixedDimension::HEIGHT) {
            requiredFixed   = rowSpan;
            requiredUnfixed = columnSpan;
        } else {
            requiredFixed   = columnSpan;
            requiredUnfixed = rowSpan;
        }

        SlotPosition bestSlot = firstAvailableSlot();
        while (!canFit(bestSlot, requiredFixed, requiredUnfixed)) {
            bestSlot = nextAvailableSlot(bestSlot);
        }

        unsigned row;
        unsigned column;
        if (currentFixedDimension == FixedDimension::HEIGHT) {
            row    = bestSlot.fixedIndex();
            column = bestSlot.unfixedIndex();
        } else {
            row    = bestSlot.unfixedIndex();
            column = bestSlot.fixedIndex();
        }

        allocateSpace(bestSlot, requiredFixed, requiredUnfixed);

        return ItemPosition(row, column);
    }


    QList<StackingLayout::ItemData> StackingLayout::stackedItems() const {
        QList<ItemData>    result;
        QSet<QLayoutItem*> trackedItems;

        unsigned numberRows    = static_cast<unsigned>(rowCount());
        unsigned numberColumns = static_cast<unsigned>(columnCount());

        if (currentFixedDimension == FixedDimension::HEIGHT) {
            for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                    QLayoutItem* item = itemAtPosition(rowIndex, columnIndex);
                    if (!trackedItems.contains(item)) {
                        unsigned rowSpan    = 1;
                        unsigned columnSpan = 1;

                        while (itemAtPosition(rowIndex + rowSpan, columnIndex) == item) {
                            ++rowSpan;
                        }

                        while (itemAtPosition(rowIndex, columnIndex + columnSpan) == item) {
                            ++columnSpan;
                        }

                        result.append(ItemData(item, rowSpan, columnSpan));
                        trackedItems.insert(item);
                    }
                }
            }
        } else {
            for (unsigned rowIndex=0 ; rowIndex<numberRows ; ++rowIndex) {
                for (unsigned columnIndex=0 ; columnIndex<numberColumns ; ++columnIndex) {
                    QLayoutItem* item = itemAtPosition(rowIndex, columnIndex);
                    if (!trackedItems.contains(item)) {
                        unsigned rowSpan    = 1;
                        unsigned columnSpan = 1;

                        while (itemAtPosition(rowIndex + rowSpan, columnIndex) == item) {
                            ++rowSpan;
                        }

                        while (itemAtPosition(rowIndex, columnIndex + columnSpan) == item) {
                            ++columnSpan;
                        }

                        result.append(ItemData(item, rowSpan, columnSpan));
                        trackedItems.insert(item);
                    }
                }
            }
        }

        return result;
    }


    bool StackingLayout::canFit(
            unsigned startFixed,
            unsigned startUnfixed,
            unsigned requiredFixed,
            unsigned requiredUnfixed
        ) const {
        bool     fits         = (startFixed + requiredFixed) <= currentMaximumSlots;
        unsigned unfixedIndex = startUnfixed;
        unsigned unfixedEnd   = startUnfixed + requiredUnfixed;
        while (fits && unfixedIndex < unfixedEnd) {
            unsigned fixedIndex = unfixedIndex * currentMaximumSlots + startFixed;
            unsigned fixedEnd   = fixedIndex + requiredFixed;
            while (fits && fixedIndex < fixedEnd) {
                fits = availableSlots.isClear(fixedIndex);
                ++fixedIndex;
            }

            ++unfixedIndex;
        }

        return fits;
    }


    void StackingLayout::allocateSpace(
            unsigned startFixed,
            unsigned startUnfixed,
            unsigned requiredFixed,
            unsigned requiredUnfixed
        ) {
        unsigned requiredFillLength = currentMaximumSlots * (startUnfixed + requiredUnfixed);
        if (availableSlots.length() < requiredFillLength) {
            availableSlots.resize(requiredFillLength);
        }

        for (unsigned unfixedOffset=0 ; unfixedOffset<requiredUnfixed ; ++unfixedOffset) {
            unsigned bitOffset = (unfixedOffset + startUnfixed) * currentMaximumSlots + startFixed;
            availableSlots.setBits(bitOffset, bitOffset + requiredFixed - 1);
        }
    }


    StackingLayout::SlotPosition StackingLayout::firstAvailableSlot() const {
        Util::BitArray::Index firstIndex = availableSlots.firstClearedBit();
        if (firstIndex == Util::BitArray::invalidIndex) {
            firstIndex = availableSlots.length();
        }

        return SlotPosition(firstIndex % currentMaximumSlots, firstIndex / currentMaximumSlots);
    }


    StackingLayout::SlotPosition StackingLayout::nextAvailableSlot(const StackingLayout::SlotPosition& current) const {
        Util::BitArray::Index startIndex = current.unfixedIndex() * currentMaximumSlots + current.fixedIndex();
        Util::BitArray::Index firstIndex = availableSlots.firstClearedBit(startIndex + 1);
        if (firstIndex == Util::BitArray::invalidIndex) {
            firstIndex = availableSlots.length();
        }

        return SlotPosition(firstIndex % currentMaximumSlots, firstIndex / currentMaximumSlots);
    }
}
