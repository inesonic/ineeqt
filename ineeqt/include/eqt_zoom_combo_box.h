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
* This header defines the \ref EQt::ZoomComboBox class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_ZOOM_COMBO_BOX_H
#define EQT_ZOOM_COMBO_BOX_H

#include <QList>

#include "eqt_common.h"
#include "eqt_combo_box.h"

class QWidget;
class QAction;

namespace EQt {
    /**
     * Class that extends ComboBox to include values and configuration for an application zoom combo box.
     */
    class EQT_PUBLIC_API ZoomComboBox:public ComboBox {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            ZoomComboBox(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor.
             *
             * \param[in] action The action to link to this combo box.
             *
             * \param[in] parent The parent object for this window.
             */
            ZoomComboBox(QAction* action, QWidget* parent = Q_NULLPTR);

            ~ZoomComboBox() override;

            /**
             * Method you can use to obtain the current minimum zoom level.
             *
             * \return Returns the current minimum zoom level.
             */
            unsigned minimumZoomLevel() const;

            /**
             * Method you can use to obtain the maximum zoom level.
             *
             * \return Returns the maximum zoom level.
             */
            unsigned maximumZoomLevel() const;

            /**
             * Method you can use to determine if the "Page Width" string will be included or excluded.
             *
             * \return Returns true if the "Page Width" string is included.  Returns false if the "Page Width" string
             *         is excluded.
             */
            bool includePageWidth() const;

            /**
             * Method you can use to determine if the "Page Width" string will be excluded or included.
             *
             * \return Returns true if the "Page Width" string is excluded.  Returns false if the "Page Width" string
             *         is included.
             */
            bool excludePageWidth() const;

            /**
             * Method you can use to determine if the "Full Page" string will be included or excluded.
             *
             * \return Returns true if the "Full Page" string is included.  Returns false if the "Full Page" string
             *         is excluded.
             */
            bool includeFullPage() const;

            /**
             * Method you can use to determine if the "Full Page" string will be excluded or included.
             *
             * \return Returns true if the "Full Page" string is excluded.  Returns false if the "Full Page" string
             *         is included.
             */
            bool excludeFullPage() const;

            /**
             * Method you can use to obtain the list of current scaler values used by this zoom combo box.  The
             * default values will be generated using this list multiplied by powers of 10.  The provides values should
             * rangeo between 1.0 (inclusive) and 10.0 (exclusive).
             *
             * \return Returns a list of scaler values.
             */
            QList<float> scalers() const;

            /**
             * Method you can use to determine if it is possible to zoom-in.
             *
             * \return Returns true if zoom-in is available.  Returns false if zoom-in is not available.
             */
            bool zoomInAvailable() const;

            /**
             * Method you can use to determine if it is possible to zoom-out.
             *
             * \return Returns true if zoom-out is available.  Returns false if zoom-out is not available.
             */
            bool zoomOutAvailable() const;

        signals:
            /**
             * Signal that is emitted if this zoom combo box can support a zoom-in operation.
             *
             * \param[out] zoomInAvailable If true, zoom-in can be used.  If false, zoom-in will be ignored.
             */
            void canZoomIn(bool zoomInAvailable);

            /**
             * Signal that is emitted if this zoom combo box can support a zoom-out operation.
             *
             * \param[out] zoomOutAvailable If true, zoom-out can be used.  If false, zoom-out will be ignored.
             */
            void canZoomOut(bool zoomOutAvailable);

        public slots:
            /**
             * Slot you can use to set the minimum allowed zoom.
             *
             * \param[in] newMinimumZoom The new minimum zoom level.
             */
            void setMinimumZoom(unsigned newMinimumZoom);

            /**
             * Slot you can use to set the maximum allowed zoom.
             *
             * \param[in] newMaximumZoom The new maximum zoom level.
             */
            void setMaximumZoom(unsigned newMaximumZoom);

            /**
             * Slot you can use to set the allowed range of zoom values.
             *
             * \param[in] newMinimumValue The new minimum zoom level.
             *
             * \param[in] newMaximumValue The new maximum zoom level.
             */
            void SetZoomRange(unsigned newMinimumValue, unsigned newMaximumValue);

            /**
             * Slot you can use to increase the zoom level.  This slot will advance the zoom to the next programmed
             * zoom level.
             */
            void zoomIn();

            /**
             * Slot you can use to decrease the zoom level.  This slot will advance the zoom to the previous programmed
             * zoom level.
             *
             * \param[in] currentZoom The current zoom level.  The zoom operation will be performed relative to this
             *                        value.
             */
            void zoomOut();

            /**
             * Slot you can use to increase the zoom level.  This slot will advance the zoom to the next programmed
             * zoom level.
             *
             * \param[in] currentZoom The current zoom level.  The zoom operation will be performed relative to this
             *                        value.
             */
            void zoomIn(unsigned currentZoom);

            /**
             * Slot you can use to decrease the zoom level.  This slot will advance the zoom to the previous programmed
             * zoom level.
             *
             * \param[in] currentZoom The current zoom level.  The zoom operation will be performed relative to this
             *                        value.
             */
            void zoomOut(unsigned currentZoom);

            /**
             * Slot you can use to indicate if the "Page Width" string should be included in the values.
             *
             * \param[in] includePageWidth If true, the "Page Width" string will be included.  If false, the
             *                             "Page Width" string will be excluded.
             */
            void setPageWidthStringIncluded(bool includePageWidth = true);

            /**
             * Slot you can use to indicate if the "Page Width" string should be excluded from the values.
             *
             * \param[in] excludePageWidth If true, the "Page Width" string will be excluded.  If false, the
             *                             "Page Width" string will be included.
             */
            void setPageWidthStringExcluded(bool excludePageWidth = true);

            /**
             * Slot you can use to indicate if the "Full Page" string should be included in the values.
             *
             * \param[in] includeFullPage If true, the "Full Page" string will be included.  If false, the
             *                            "Full Page" string will be excluded.
             */
            void setFullPageStringIncluded(bool includeFullPage = true);

            /**
             * Slot you can use to indicate if the "Full Page" string should be excluded from the values.
             *
             * \param[in] excludeFullPage If true, the "Full Page" string will be excluded.  If false, the
             *                            "Full Page" string will be included.
             */
            void setFullPageStringExcluded(bool excludeFullPage = true);

            /**
             * Slot you can use to update the list of scaler values.
             *
             * \param[in] newScalers The new list of scaler values.  Values will be multipled by increasing powers of
             *                       10 in order to produce the default zoom values.
             */
            void setScalers(const QList<float>& newScalers);

        private:
            /**
             * Indicates the default smallest allowed value.
             */
            static const unsigned defaultMinimumValue = 20;

            /**
             * Indicates the default largest allowed value.
             */
            static const unsigned defaultMaximumValue = 500;

            /**
             * Performs operations needed by all constructors.
             */
            void configureWidget();

            /**
             * Method that recalculates the zoom values in the combo box.
             */
            void recalculateZoomValues();

            /**
             * The current minimum zoom level.
             */
            unsigned currentMinimumValue;

            /**
             * The current maximum zoom level.
             */
            unsigned currentMaximumValue;

            /**
             * If true, the "Page Width" string will be included.
             */
            bool currentIncludePageWidth;

            /**
             * If true, the "Full Page" string will be included.
             */
            bool currentIncludeFullPage;

            /**
             * The current list of scaler values.
             */
            QList<float> currentScalers;
    };
}

#endif
