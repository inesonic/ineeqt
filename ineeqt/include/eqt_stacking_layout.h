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
* This header defines the \ref EQt::StackingLayout class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_STACKING_LAYOUT_H
#define EQT_STACKING_LAYOUT_H

#include <QGridLayout>
#include <QList>

#include <utility>

#include <util_bit_array.h>

#include "eqt_common.h"

class QWidget;
class QLayout;
class QLayoutItem;

namespace EQt {
    class ToolButton;

    /**
     * A layout that allows you to insert layout items sequentially.  Items can be arranged into a 2D grid going from
     * left to right or from top to bottom.
     *
     * The layout is based on QGridLayout and can also be used as a QGridLayout object.
     */
    class EQT_PUBLIC_API StackingLayout:public QGridLayout {
        Q_OBJECT

        public:
            /**
             * Enumeration indicating which dimension should be fixed.
             */
            enum class FixedDimension {
                /**
                 * Value indicating fixed width.
                 */
                WIDTH,

                /**
                 * Value indicating fixed height.
                 */
                HEIGHT
            };

            StackingLayout();

            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this layout.
             */
            StackingLayout(QWidget* parent);

            /**
             * Constructor.
             *
             * \param[in] fixedDimension Value indicating the fixed dimension.
             *
             * \param[in] numberSlots    The number of "slots" in the fixed dimension.
             */
            StackingLayout(FixedDimension fixedDimension, unsigned numberSlots);

            /**
             * Constructor.
             *
             * \param[in] fixedDimension Value indicating the fixed dimension.
             *
             * \param[in] numberSlots    The number of "slots" in the fixed dimension.
             *
             * \param[in] parent         The parent object for this layout.
             */
            StackingLayout(FixedDimension fixedDimension, unsigned numberSlots, QWidget* parent);

            ~StackingLayout() override;

            /**
             * Method you can use to set the fixed dimension for the layout.  Note that this method does not cause the
             * layout contents to be adjusted.  If needed, you should call \ref EQt::StackingLayout::restack after
             * calling this method.
             *
             * \param[in] newFixedDimension The fixed dimension.
             */
            void setFixedDimension(FixedDimension newFixedDimension);

            /**
             * Method you can use to determine the fixed dimension.
             *
             * \return Returns the currently fixed dimension.
             */
            FixedDimension fixedDimension() const;

            /**
             * Method you can use to set the number of slots (rows or columns) in the fixed dimension.  Note that this
             * method does not cause the layout contents to be adjusted.  If needed, you should call
             * \ref EQt::StackingLayout::restack after calling this method.
             *
             * \param[in] numberFixedSlots The maximum number of rows or columns in the fixed dimension.
             */
            void setMaximumSlotsInFixedDimension(unsigned numberFixedSlots);

            /**
             * Method you can use to determine the number of slots in the fixed dimension.
             *
             * \return Returns the maximum number of slots in the fixed dimension.
             */
            unsigned maximumSlotsInFixedDimension() const;

            /**
             * Method you can use to append a layout item.
             *
             * \param[in] item       The layout item to be inserted.
             *
             * \param[in] rowSpan    The width of the layout in rows.
             *
             * \param[in] columnSpan The width of the layout in columns.
             *
             * \param[in] alignment  The desired alignment setting for the item.
             */
            void appendItem(
                QLayoutItem*  item,
                unsigned      rowSpan = 1,
                unsigned      columnSpan = 1,
                Qt::Alignment alignment = Qt::Alignment()
            );

            /**
             * Method you can use to append a layout item.
             *
             * \param[in] item      The layout item to be inserted.
             *
             * \param[in] alignment The desired alignment setting for the item.
             */
            inline void appendItem(QLayoutItem* item, Qt::Alignment alignment) {
                appendItem(item, 1, 1, alignment);
            }

            /**
             * Method you can use to append a layout.
             *
             * \param[in] layout     The layout item to be inserted.
             *
             * \param[in] rowSpan    The width of the layout in rows.
             *
             * \param[in] columnSpan The width of the layout in columns.
             *
             * \param[in] alignment  The desired alignment setting for the item.
             */
            void appendLayout(
                QLayout*      layout,
                unsigned      rowSpan = 1,
                unsigned      columnSpan = 1,
                Qt::Alignment alignment = Qt::Alignment()
            );

            /**
             * Method you can use to append a layout.
             *
             * \param[in] layout    The layout item to be inserted.
             *
             * \param[in] alignment The desired alignment setting for the item.
             */
            inline void appendLayout(QLayout* layout, Qt::Alignment alignment) {
                appendLayout(layout, 1, 1, alignment);
            }

            /**
             * Method you can use to append a widget.
             *
             * \param[in] widget     The widget item to be inserted.
             *
             * \param[in] rowSpan    The width of the widget in rows.
             *
             * \param[in] columnSpan The width of the widget in columns.
             *
             * \param[in] alignment  The desired alignment setting for the item.
             */
            void appendWidget(
                QWidget*      widget,
                unsigned      rowSpan = 1,
                unsigned      columnSpan = 1,
                Qt::Alignment alignment = Qt::Alignment()
            );

            /**
             * Method you can use to append a widget.
             *
             * \param[in] widget    The widget item to be inserted.
             *
             * \param[in] alignment The desired alignment setting for the item.
             */
            inline void appendWidget(QWidget* widget, Qt::Alignment alignment) {
                appendWidget(widget, 1, 1, alignment);
            }

            /**
             * Method you can use to restack the layout contents.
             */
            void restack();

        private:
            /**
             * Class that represents a position within this layout.
             */
            class ItemPosition {
                public:
                    constexpr ItemPosition():currentRow(0),currentColumn(0) {}

                    /**
                     * Constructor
                     *
                     * \param[in] row    The layout row.
                     *
                     * \param[in] column The layout column.
                     */
                    constexpr ItemPosition(
                            unsigned row,
                            unsigned column
                        ):currentRow(
                            row
                        ),currentColumn(
                            column
                        ) {}

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to assign to this instance.
                     */
                    constexpr ItemPosition(
                            const ItemPosition& other
                        ):currentRow(
                            other.currentRow
                        ),currentColumn(
                            other.currentColumn
                        ) {}

                    ~ItemPosition() = default;

                    /**
                     * Method you can use to obtain the row for the tracked item.
                     *
                     * \return Returns the row for the tracked item.
                     */
                    inline unsigned row() const {
                        return currentRow;
                    }

                    /**
                     * Method you can use to set the row for the tracked item.
                     *
                     * \param[in] newRow The new row to assign to this item.
                     */
                    inline void setRow(unsigned newRow) {
                        currentRow = newRow;
                    }

                    /**
                     * Method you can use to obtain the column for the tracked item.
                     *
                     * \return Returns the column for the tracked item.
                     */
                    inline unsigned column() const {
                        return currentColumn;
                    }

                    /**
                     * Method you can use to set the column for the tracked item.
                     *
                     * \param[in] newColumn The new column to assign to this item.
                     */
                    inline void setColumn(unsigned newColumn) {
                        currentColumn = newColumn;
                    }

                    /**
                     * Assignment operator
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    inline ItemPosition& operator=(const ItemPosition& other) {
                        currentRow    = other.currentRow;
                        currentColumn = other.currentColumn;

                        return *this;
                    }

                private:
                    /**
                     * The current row.
                     */
                    unsigned currentRow;

                    /**
                     * The current column.
                     */
                    unsigned currentColumn;
            };

            /**
             * Class that represents a item and size within this layout.
             */
            class ItemData {
                public:
                    constexpr ItemData():currentItem(nullptr), currentRowSpan(1), currentColumnSpan(1) {}

                    /**
                     * Constructor
                     *
                     * \param[in] item       The layout item.
                     *
                     * \param[in] rowSpan    The layout row.
                     *
                     * \param[in] columnSpan The column span.
                     */
                    constexpr ItemData(
                            QLayoutItem* item,
                            unsigned     rowSpan = 1,
                            unsigned     columnSpan = 1
                        ):currentItem(
                            item
                        ),currentRowSpan(
                            rowSpan
                        ),currentColumnSpan(
                            columnSpan
                        ) {}

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to assign to this instance.
                     */
                    constexpr ItemData(
                            const ItemData& other
                        ):currentItem(
                            other.currentItem
                        ),currentRowSpan(
                            other.currentRowSpan
                        ),currentColumnSpan(
                            other.currentColumnSpan
                        ) {}

                    ~ItemData() = default;

                    /**
                     * Method you can use to obtain the layout item.
                     *
                     * \return Returns the layout item tracked by this instance.
                     */
                    inline QLayoutItem* item() const {
                        return currentItem;
                    }

                    /**
                     * Method you can use to set the layout item tracked by this instance.
                     *
                     * \param[in] newItem        The new layout item to be tracked.
                     *
                     * \param[in] deleteExisting If true, any tracked layout item will be deleted.
                     */
                    void setItem(QLayoutItem* newItem, bool deleteExisting = false);

                    /**
                     * Method you can use to obtain the row span for the tracked item.
                     *
                     * \return Returns the row span for the tracked item.
                     */
                    inline unsigned rowSpan() const {
                        return currentRowSpan;
                    }

                    /**
                     * Method you can use to set the row span for the tracked item.
                     *
                     * \param[in] newRowSpan The new row span to assign to this item.
                     */
                    inline void setRowSpan(unsigned newRowSpan) {
                        currentRowSpan = newRowSpan;
                    }

                    /**
                     * Method you can use to obtain the column span for the tracked item.
                     *
                     * \return Returns the column span for the tracked item.
                     */
                    inline unsigned columnSpan() const {
                        return currentColumnSpan;
                    }

                    /**
                     * Method you can use to set the column span for the tracked item.
                     *
                     * \param[in] newColumnSpan The new column span to assign to this item.
                     */
                    inline void setColumnSpan(unsigned newColumnSpan) {
                        currentColumnSpan = newColumnSpan;
                    }

                    /**
                     * Assignment operator.
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    inline ItemData& operator=(const ItemData& other) {
                        currentItem       = other.currentItem;
                        currentRowSpan    = other.currentRowSpan;
                        currentColumnSpan = other.currentColumnSpan;

                        return *this;
                    }

                private:
                    /**
                     * The currently tracked item.
                     */
                    QLayoutItem* currentItem;

                    /**
                     * The current row span.
                     */
                    unsigned currentRowSpan;

                    /**
                     * The current column span.
                     */
                    unsigned currentColumnSpan;
            };

            /**
             * Class that represents a slot position.
             */
            class SlotPosition {
                public:
                    constexpr SlotPosition():currentFixedIndex(0),currentUnfixedIndex(0) {}

                    /**
                     * Constructor
                     *
                     * \param[in] fixedIndex   The slot fixed inswz.
                     *
                     * \param[in] unfixedIndex The slot unfixed index.
                     */
                    constexpr SlotPosition(
                            unsigned fixedIndex,
                            unsigned unfixedIndex
                        ):currentFixedIndex(
                            fixedIndex
                        ),currentUnfixedIndex(
                            unfixedIndex
                        ) {}

                    /**
                     * Copy constructor
                     *
                     * \param[in] other The instance to assign to this instance.
                     */
                    constexpr SlotPosition(
                            const SlotPosition& other
                        ):currentFixedIndex(
                            other.currentFixedIndex
                        ),currentUnfixedIndex(
                            other.currentUnfixedIndex
                        ) {}

                    ~SlotPosition() = default;

                    /**
                     * Method you can use to obtain the fixed index for the tracked item.
                     *
                     * \return Returns the fixed indexfor the tracked item.
                     */
                    inline unsigned fixedIndex() const {
                        return currentFixedIndex;
                    }

                    /**
                     * Method you can use to set the fixed index for the tracked item.
                     *
                     * \param[in] newFixedIndex The new fixed indexto assign to this item.
                     */
                    inline void setFixedIndex(unsigned newFixedIndex) {
                        currentFixedIndex = newFixedIndex;
                    }

                    /**
                     * Method you can use to obtain the unfixed index for the tracked item.
                     *
                     * \return Returns the unfixed index for the tracked item.
                     */
                    inline unsigned unfixedIndex() const {
                        return currentUnfixedIndex;
                    }

                    /**
                     * Method you can use to set the unfixed index for the tracked item.
                     *
                     * \param[in] newUnfixedIndex The new unfixed index to assign to this item.
                     */
                    inline void setUnfixedIndex(unsigned newUnfixedIndex) {
                        currentUnfixedIndex = newUnfixedIndex;
                    }

                    /**
                     * Assignment operator
                     *
                     * \param[in] other The instance to assign to this instance.
                     *
                     * \return Returns a reference to this instance.
                     */
                    inline SlotPosition& operator=(const SlotPosition& other) {
                        currentFixedIndex    = other.currentFixedIndex;
                        currentUnfixedIndex = other.currentUnfixedIndex;

                        return *this;
                    }

                private:
                    /**
                     * The current fixed index.
                     */
                    unsigned currentFixedIndex;

                    /**
                     * The current unfixed index.
                     */
                    unsigned currentUnfixedIndex;
            };

            /**
             * Method used internally to identify an optimal position for an item.
             *
             * \param[in] rowSpan    The desired row span.
             *
             * \param[in] columnSpan The desired column span.
             *
             * \return Returns the recommended item position.
             */
            ItemPosition findAndAllocatePosition(unsigned rowSpan, unsigned columnSpan);

            /**
             * Method used internally to get all of the current item data in stacking order.
             *
             * \return Returns a list of item data instances.
             */
            QList<ItemData> stackedItems() const;

            /**
             * Method used internally to determine if a space can be used.
             *
             * \param[in] startFixed      The starting index in the fixed dimension.
             *
             * \param[in] startUnfixed    The starting index in the unfixed dimension.
             *
             * \param[in] requiredFixed   The number of required slots in the fixed dimension.
             *
             * \param[in] requiredUnfixed The number of required slots in the unfixed dimension.
             *
             * \return Returns true if the space at the row/column is available. Returns false if the space at the
             *         row/column is not available.
             */
            bool canFit(
                unsigned startFixed,
                unsigned startUnfixed,
                unsigned requiredFixed,
                unsigned requiredUnfixed
            ) const;

            /**
             * Method used internally to determine if a space can be used.
             *
             * \param[in] position        The slot position of interest.
             *
             * \param[in] requiredFixed   The number of required slots in the fixed dimension.
             *
             * \param[in] requiredUnfixed The number of required slots in the unfixed dimension.
             *
             * \return Returns true if the space at the row/column is available. Returns false if the space at the
             *         row/column is not available.
             */
            inline bool canFit(
                    const SlotPosition& position,
                    unsigned            requiredFixed,
                    unsigned            requiredUnfixed
                ) const {
                return canFit(position.fixedIndex(), position.unfixedIndex(), requiredFixed, requiredUnfixed);
            }

            /**
             * Method used internally to assign space.
             *
             * \param[in] startFixed      The starting index in the fixed dimension.
             *
             * \param[in] startUnfixed    The starting index in the unfixed dimension.
             *
             * \param[in] requiredFixed   The number of required slots in the fixed dimension.
             *
             * \param[in] requiredUnfixed The number of required slots in the unfixed dimension.
             */
            void allocateSpace(
                unsigned startFixed,
                unsigned startUnfixed,
                unsigned requiredFixed,
                unsigned requiredUnfixed
            );

            /**
             * Method used internally to assign space.
             *
             * \param[in] position        The slot position of interest.
             *
             * \param[in] requiredFixed   The number of required slots in the fixed dimension.
             *
             * \param[in] requiredUnfixed The number of required slots in the unfixed dimension.
             */
            void allocateSpace(
                    const SlotPosition& position,
                    unsigned            requiredFixed,
                    unsigned            requiredUnfixed
                ) {
                allocateSpace(position.fixedIndex(), position.unfixedIndex(), requiredFixed, requiredUnfixed);
            }

            /**
             * Method that finds the first available slot.
             *
             * \return Returns the position of the first available slot.
             */
            SlotPosition firstAvailableSlot() const;

            /**
             * Method that finds the next available slot.
             *
             * \param[in] current The starting position for the search.  This position will be excluded.
             *
             * \return Returns the position of the next available slot.
             */
            SlotPosition nextAvailableSlot(const SlotPosition& current) const;

            /**
             * Value indicating which dimension should be fixed.
             */
            FixedDimension currentFixedDimension;

            /**
             * Value indicating the maximum number of slots in the fixed dimension.
             */
            unsigned currentMaximumSlots;

            /**
             * A bit array used to rapidly locate available slots.  A set bit indicates that a slot is used.
             */
            Util::BitArray availableSlots;
    };
}

#endif
