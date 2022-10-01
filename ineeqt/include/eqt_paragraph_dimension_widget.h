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
* This header defines the \ref EQt::ParagraphDimensionWidget class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PARAGRAPH_DIMENSION_WIDGET_H
#define EQT_PARAGRAPH_DIMENSION_WIDGET_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>

#include "eqt_common.h"
#include "eqt_paragraph_diagram.h"

class QComboBox;
class QHBoxLayout;
class QSpacerItem;
class QTimer;
class QLabel;

namespace EQt {
    class DimensionLineEdit;

    /**
     * Class that provides a trivial widget you can use to display a diagram showing a pargraph, with various
     * indentation values called out.
     */
    class EQT_PUBLIC_API ParagraphDimensionWidget:public QWidget {
        Q_OBJECT

        public:
            /**
             * Enumeration of supported justification settings.
             */
            typedef ParagraphDiagram::Justification Justification;

            /**
             * Constructor
             *
             * \param[in] parent Pointer to the parent widget.
             */
            ParagraphDimensionWidget(QWidget* parent = Q_NULLPTR);

            ~ParagraphDimensionWidget() override;

            /**
             * Method you can use to determine the maximum allowed height.
             *
             * \return Returns the maximum allowed height for the widget.
             */
            unsigned maximumHeight() const;

            /**
             * Method you can use to determine if the list controls are visible or hidden.
             *
             * \return Returns true if the list controls are visibie.  Returns false if the list controls are hidden.
             */
            bool listControlsShown() const;

            /**
             * Method you can use to determine if the list controls are hidden or visible.
             *
             * \return Returns true if the list controls are hidden.  Returns false if the list controls are shown.
             */
            bool listControlsHidden() const;

            /**
             * Method you can use to determine if the bullet/number is being shown.
             *
             * \return Returns true if the bullet/number mark is being shown.  Returns false if the bullet/number mark
             *         is being hidden.
             */
            bool bulletShown() const;

            /**
             * Method you can use to determine if the bullet/number is being hidden.
             *
             * \return Returns true if the bullet/number mark is hidden.  Returns false if the bullet/number mark is
             *         being shown.
             */
            bool bulletHidden() const;

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
             * Method you can use to obtain the current list indentation value.
             *
             * \return Returns the list indentation value.
             */
            float listIndentation() const;

            /**
             * Method you can use to obtain the current first line left indentation value.
             *
             * \return Returns the first line left indentation value.
             */
            float firstLineIndentation() const;

            /**
             * Method you can use to obtain the currently displayed line spacing.
             *
             * \return Returns the current line spacing.  A value of 1.0 indicates single space, a value of 1.5
             *         indicates 1 1/2 line spacing.  A value of 2.0 indicates double spacing.
             */
            float lineSpacing() const;

            /**
             * Method that determines if the left indentation value is cleared.
             */
            bool isLeftIndentationClear() const;

            /**
             * Method that determines if the right indentation value is cleared.
             *
             * \return Returns true if the field is cleared.  Returns false if the field holds a value.
             */
            bool isRightIndentationClear() const;

            /**
             * Method that determines if the top spacing value is cleared.
             *
             * \return Returns true if the field is cleared.  Returns false if the field holds a value.
             */
            bool isTopSpacingClear() const;

            /**
             * Method that determines if the bottom spacing value is cleared.
             *
             * \return Returns true if the field is cleared.  Returns false if the field holds a value.
             */
            bool isBottomSpacingClear() const;

            /**
             * Method that determines if the list indentation value is cleared.
             *
             * \return Returns true if the field is cleared.  Returns false if the field holds a value.
             */
            bool isListIndentationClear() const;

            /**
             * Method that determines if the first line left indentation value is cleared.
             *
             * \return Returns true if the field is cleared.  Returns false if the field holds a value.
             */
            bool isFirstLineIndentationClear() const;

            /**
             * Method that determines if the line spacing value is cleared.
             *
             * \return Returns true if the field is cleared.  Returns false if the field holds a value.
             */
            bool isLineSpacingClear() const;

            /**
             * Method that determines if the left indentation value is set.
             *
             * \return Returns true if the field holds a value.  Returns false if the dield is cleared.
             */
            bool isLeftIndentationSet() const;

            /**
             * Method that determines if the right indentation value is set.
             *
             * \return Returns true if the field holds a value.  Returns false if the dield is cleared.
             */
            bool isRightIndentationSet() const;

            /**
             * Method that determines if the top spacing value is set.
             *
             * \return Returns true if the field holds a value.  Returns false if the dield is cleared.
             */
            bool isTopSpacingSet() const;

            /**
             * Method that determines if the bottom spacing value is set.
             *
             * \return Returns true if the field holds a value.  Returns false if the dield is cleared.
             */
            bool isBottomSpacingSet() const;

            /**
             * Method that determines if the list indentation value is set.
             *
             * \return Returns true if the field holds a value.  Returns false if the dield is cleared.
             */
            bool isListIndentationSet() const;

            /**
             * Method that determines if the first line left indentation value is set.
             *
             * \return Returns true if the field holds a value.  Returns false if the dield is cleared.
             */
            bool isFirstLineIndentationSet() const;

            /**
             * Method that determines if the line spacing value is set.
             *
             * \return Returns true if the field holds a value.  Returns false if the dield is cleared.
             */
            bool isLineSpacingSet() const;

            /**
             * Method you can use to set the new maximum height for the widget.
             *
             * \param[in] newMaximumHeight The new maximum height for the widget, in pixels.
             */
            void setMaximumHeight(unsigned newMaximumHeight);

            /**
             * Method you can use to set or change the current \ref EQt::ParagraphDiagram instance.
             *
             * \param[in] newParagraphDiagram The new paragraph diagram to be used by this instance.  Note that this
             *                                class will take ownership of the paragraph diagram.  This class will also
             *                                create a default instance when instantiated so you only need to call this
             *                                method if you wish to use a custom paragraph diagram.
             */
            void setParagraphDiagram(ParagraphDiagram* newParagraphDiagram);

            /**
             * Method that returns the current \ref EQt::ParagraphDiagram instance.
             *
             * \return Returns a pointer to the current \ref EQt::ParagraphDiagram instance.
             */
            ParagraphDiagram* paragraphDiagram() const;

            /**
             * Method you can use to set or change the dimension line editor used for the first line left indentation
             * adjustment.
             *
             * \param[in] newDimensionLineEdit The new dimension line editor to use to adjust the first line left.
             *                                 indentation.  Note that this class will take ownership of the dimension
             *                                 line editor.  The class will also automatically create a default
             *                                 instance so you will normally never need to call this method.
             */
            void setFirstLineIndentationLineEdit(DimensionLineEdit* newDimensionLineEdit);

            /**
             * Method that returns the current \ref EQt::DimensionLineEdit used to adjust the first line left
             * indentation.
             *
             * \return Returns a pointer to the current first line left indentation \ref EQt::DimensionLineEdit
             * instance.
             */
            DimensionLineEdit* firstLineIndentationLineEdit() const;

            /**
             * Method you can use to set or change the dimension line editor used for the list indentation adjustment.
             *
             * \param[in] newDimensionLineEdit The new dimension line editor to use to adjust the list indentation.
             *                                 Note that this class will take ownership of the dimension line editor.
             *                                 The class will also automatically create a default instance so you will
             *                                 normally never need to call this method.
             */
            void setListIndentationLineEdit(DimensionLineEdit* newDimensionLineEdit);

            /**
             * Method that returns the current \ref EQt::DimensionLineEdit used to adjust the list indentation.
             *
             * \return Returns a pointer to the current list indentation \ref EQt::DimensionLineEdit instance.
             */
            DimensionLineEdit* listIndentationLineEdit() const;

            /**
             * Method you can use to set or change the dimension line editor used for the left indentation adjustment.
             *
             * \param[in] newDimensionLineEdit The new dimension line editor to use to adjust the left indentation.
             *                                 Note that this class will take ownership of the dimension line editor.
             *                                 The class will also automatically create a default instance so you will
             *                                 normally never need to call this method.
             */
            void setLeftIndentationLineEdit(DimensionLineEdit* newDimensionLineEdit);

            /**
             * Method that returns the current \ref EQt::DimensionLineEdit used to adjust the left indentation.
             *
             * \return Returns a pointer to the current left indentation \ref EQt::DimensionLineEdit instance.
             */
            DimensionLineEdit* leftIndentationLineEdit() const;

            /**
             * Method you can use to set or change the dimension line editor used for the right indentation adjustment.
             *
             * \param[in] newDimensionLineEdit The new dimension line editor to use to adjust the right indentation.
             *                                 Note that this class will take ownership of the dimension line editor.
             *                                 The class will also automatically create a default instance so you will
             *                                 normally never need to call this method.
             */
            void setRightIndentationLineEdit(DimensionLineEdit* newDimensionLineEdit);

            /**
             * Method that returns the current \ref EQt::DimensionLineEdit used to adjust the right indentation.
             *
             * \return Returns a pointer to the current right indentation \ref EQt::DimensionLineEdit instance.
             */
            DimensionLineEdit* rightIndentationLineEdit() const;

            /**
             * Method you can use to set or change the dimension line editor used for the top spacing adjustment.
             *
             * \param[in] newDimensionLineEdit The new dimension line editor to use to adjust the top spacing.  Note
             *                                 that this class will take ownership of the dimension line editor.  The
             *                                 class will also automatically create a default instance so you will
             *                                 normally never need to call this method.
             */
            void setTopSpacingLineEdit(DimensionLineEdit* newDimensionLineEdit);

            /**
             * Method that returns the current \ref EQt::DimensionLineEdit used to adjust the top spacing.
             *
             * \return Returns a pointer to the current top spacing \ref EQt::DimensionLineEdit instance.
             */
            DimensionLineEdit* topSpacingLineEdit() const;

            /**
             * Method you can use to set or change the dimension line editor used for the bottom spacing adjustment.
             *
             * \param[in] newDimensionLineEdit The new dimension line editor to use to adjust the bottom spacing.
             *                                 Note that this class will take ownership of the dimension line editor.
             *                                 The class will also automatically create a default instance so you will
             *                                 normally never need to call this method.
             */
            void setBottomSpacingLineEdit(DimensionLineEdit* newDimensionLineEdit);

            /**
             * Method that returns the current \ref EQt::DimensionLineEdit used to adjust the bottom spacing.
             *
             * \return Returns a pointer to the current bottom spacing \ref EQt::DimensionLineEdit instance.
             */
            DimensionLineEdit* bottomSpacingLineEdit() const;

            /**
             * Method you can use to set the combo box used to set the line spacing.
             *
             * \param[in] newComboBox The new combo box used to adjust the line spacing.  The combo box is expected to
             *                        be pre-populated to include data fields with floating point values representing
             *                        the line spacing options.  Note that the class will take ownership of the combo
             *                        box.  Also note that the class will create a default instance so you will
             *                        normally never need to call this method.
             */
            void setLineSpacingComboBox(QComboBox* newComboBox);

            /**
             * Method you can use to obtain the combo box used to
             *
             * \return Returns the combo box used to adjust the line spacing.
             */
            QComboBox* lineSpacingComboBox() const;

        signals:
            /**
             * Signal that is emitted whenever the widget's dimensions change.
             */
            void geometryChanged();

            /**
             * Signal that is emitted whenever any value changes.
             */
            void valueChanged();

            /**
             * Signal that is emitted when the user changes the first line left indentation.
             *
             * \param[out] newValue The new indentation value.
             */
            void firstLineIdentationChanged(float newValue);

            /**
             * Signal that is emitted when the user changes the list indentation value.
             *
             * \param[out] newValue The new list indentation value.
             */
            void listIndentationChanged(float newValue);

            /**
             * Signal that is emitted when the user changes the left indentation value.
             *
             * \param[out] newValue The new indentation value.
             */
            void leftIdentationChanged(float newValue);

            /**
             * Signal that is emitted when the user changes the right indentation value.
             *
             * \param[out] newValue The new indentation value.
             */
            void rightIdentationChanged(float newValue);

            /**
             * Signal that is emitted when the user changes the top spacing value.
             *
             * \param[out] newValue The new top spacing value.
             */
            void topSpacingChanged(float newValue);

            /**
             * Signal that is emitted when the user changes the bottom spacing value.
             *
             * \param[out] newValue The new bottom spacing value.
             */
            void bottomSpacingChanged(float newValue);

            /**
             * Signal that is emitted when the user changes the line spacing value.
             *
             * \param[out] newValue The new line spacing value.
             */
            void lineSpacingChanged(float newValue);

        public slots:
            /**
             * Slot you can use to display or hide the list controls.
             *
             * \param[in] nowShow If true, the list controls will be presented to the user.  If false, the list controls
             *                    will be hidden.
             */
            void setListControlsShown(bool nowShow = true);

            /**
             * Slot you can use to hide or display the list controls.
             *
             * \param[in] nowHidden If true, the list controls will be hidden from the user.  If false, the list
             *                      controls will be shown.
             */
            void setListControlsHidden(bool nowHidden = true);

            /**
             * Slot you can use to show or hide the bullet/number mark.
             *
             * \param[in] nowVisible If true, the bullet/number mark will be displayed.  If false, the bullet/number
             *                       mark will be hidden.
             */
            void setBulletVisible(bool nowVisible = true);

            /**
             * Slot you can use to hide or show the bullet/number mark.
             *
             * \param[in] nowHidden If true, the bullet/number mark will be hidden.  if false, the bullet/number mark
             *                      will be shown.
             */
            void setBulletHidden(bool nowHidden = true);

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
             * \param[in] newFirstLineIndentation The new first line indentation value.
             */
            void setFirstLineIndentation(float newFirstLineIndentation);

            /**
             * Slot you can use to set the list indentation value.
             *
             * \param[in] newListIndentation The new list indentation value.
             */
            void setListIndentation(float newListIndentation);

            /**
             * Slot you can use to set the line spacing.
             *
             * \param[in] newLineSpacing The new line spacing value.  A value of 1.0 indicates single spacing.  A value
             *                           of 1.5 indicates 1 1/2 line spacing.  A value of 2.0 indicates double spacing.
             */
            void setLineSpacing(float newLineSpacing);

            /**
             * Slot you can use to clear the left indentation value.
             */
            void clearLeftIndentation();

            /**
             * Slot you can use to clear the right indentation value.
             */
            void clearRightIndentation();

            /**
             * Slot you can use to clear the top spacing value.
             */
            void clearTopSpacing();

            /**
             * Slot you can use to clear the bottom spacing value.
             */
            void clearBottomSpacing();

            /**
             * Slot you can use to clear the first line left indentation value.
             */
            void clearFirstLineIndentation();

            /**
             * Slot you can use to clear the list indentation value.
             */
            void clearListIndentation();

            /**
             * Slot you can use to clear the line spacing value.
             */
            void clearLineSpacing();

        private slots:
            /**
             * Slot that is triggered when the callout locations change.
             */
            void calloutLocationsUpdated();

            /**
             * Slot that is triggered when the user changes the first line left indentation.
             *
             * \param[in] newValue The new indentation value.
             */
            void firstLineIndentationUpdated(double newValue);

            /**
             * Slot that is triggered when the user changes the list indentation.
             *
             * \param[in] newValue The new list indentation value.
             */
            void listIndentationUpdated(double newValue);

            /**
             * Slot that is triggered when the user changes the left indentation value.
             *
             * \param[in] newValue The new indentation value.
             */
            void leftIndentationUpdated(double newValue);

            /**
             * Slot that is triggered when the user changes the right indentation value.
             *
             * \param[in] newValue The new indentation value.
             */
            void rightIndentationUpdated(double newValue);

            /**
             * Slot that is triggered when the user changes the top spacing value.
             *
             * \param[in] newValue The new top spacing value.
             */
            void topSpacingUpdated(double newValue);

            /**
             * Slot that is triggered when the user changes the bottom spacing value.
             *
             * \param[in] newValue The new bottom spacing value.
             */
            void bottomSpacingUpdated(double newValue);

            /**
             * Slot that is triggered when the user changes the line spacing value.
             *
             * \param[in] newValue The new current index into the combo box.
             */
            void lineSpacingUpdated(int newValue);

        private:
            /**
             * List index for the first line indentation control and spacer.
             */
            static constexpr unsigned firstLineIndentationIndex = 0;

            /**
             * List index for the top spacing control and spacer.
             */
            static constexpr unsigned topSpacingIndex = 1;

            /**
             * List index for the line spacing control and spacer.
             */
            static constexpr unsigned lineSpacingIndex = 2;

            /**
             * List index for the left indentation control and spacer.
             */
            static constexpr unsigned leftIndentationIndex = 3;

            /**
             * List index for the bottom spacing control and spacer.
             */
            static constexpr unsigned bottomSpacingIndex = 4;

            /**
             * List index for the right indentation control and spacer.
             */
            static constexpr unsigned rightIndentationIndex = 5;

            /**
             * List index for the list indentation control and spacer.
             */
            static constexpr unsigned listIndentationIndex = 6;

            /**
             * Minimum padding between controls.
             */
            static constexpr unsigned minimumPadding = 4;

            /**
             * Platform specific additional vertical padding
             */
            static const unsigned platformAdditionalPadding;

            /**
             * Template method that assigns a new control based on an index.
             *
             * \param[in] newControl   The new control to assign.
             *
             * \param[in] controlIndex The zero based index of the control to be assigned.
             *
             * \param[in] signal       The signal method in the control used to generate notification.
             *
             * \param[in] slot         The slot to receive the notification.
             */
            template<typename WidgetPointer, typename SignalType, typename SlotType> void setControl(
                    WidgetPointer newControl,
                    unsigned      controlIndex,
                    SignalType    signal,
                    SlotType      slot
                ) {
                if (controlIndex < static_cast<unsigned>(controls.size())) {
                    WidgetPointer control = dynamic_cast<WidgetPointer>(controls.at(controlIndex));
                    if (control != Q_NULLPTR) {
                        disconnect(control, signal, this, slot);
                        delete control;
                    }
                } else {
                    do {
                        controls.append(Q_NULLPTR);
                    } while (static_cast<unsigned>(controls.size()) <= controlIndex);
                }

                controls[controlIndex] = newControl;
                newControl->setParent(this);

                connect(newControl, signal, this, slot);

                layouts[controlIndex]->addWidget(newControl);
            }

            /**
             * Template method that returns a control by index.
             *
             * \param[in] controlIndex The index of the desired widget.
             *
             * \return Returns a pointer to the requested widget.
             */
            template<typename WidgetType> WidgetType* control(unsigned controlIndex) const {
                return dynamic_cast<WidgetType*>(controls.at(controlIndex));
            }

            /**
             * Method that performs class initialization common to all constructors.
             */
            void configureWidget();

            /**
             * Method that creates the layouts and spacers used by the widget.
             */
            void buildLayout();

            /**
             * Method that updates the underlying widgets to impose a maximum height.
             */
            void enforceMaximumHeight();

            /**
             * The current maximum height, in pixels.
             */
            unsigned currentMaximumHeight;

            /**
             * The current paragraph diagram widget.
             */
            ParagraphDiagram* currentParagraphDiagram;

            /**
             * List of controls.
             */
            QList<QWidget*> controls;

            /**
             * List of labels.
             */
            QList<QLabel*> labels;

            /**
             * Layout used for the paragraph diagram.
             */
            QHBoxLayout* currentParagraphDiagramLayout;

            /**
             * List of layouts used to manage the various widgets.
             */
            QList<QVBoxLayout*> layouts;

            /**
             * Spacer widgets for positioning.
             */
            QList<QWidget*> spacers;
    };
}

#endif
