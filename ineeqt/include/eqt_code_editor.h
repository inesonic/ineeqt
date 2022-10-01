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
* This header defines the \ref EQt::CodeEditor class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_CODE_EDITOR_H
#define EQT_CODE_EDITOR_H

#include <QWidget>
#include <QString>
#include <QPlainTextEdit>

#include "eqt_common.h"

class QResizeEvent;
class QSyntaxHighlighter;

namespace EQt {
    class CodeEditorLineNumberArea;

    /**
     * Class that extends QPlainTextEdit with support for line numbers.
     *
     * This code was heavily inspired by The Qt 4 code editor example.
     */
    class EQT_PUBLIC_API CodeEditor:public QPlainTextEdit {
        friend class CodeEditorLineNumberArea;

        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            CodeEditor(QWidget* parent = Q_NULLPTR);

            /**
             * Constructor
             *
             * \param[in] text   The text to display in the code editor.
             *
             * \param[in] parent The parent object for this window.
             */
            CodeEditor(const QString& text, QWidget* parent = Q_NULLPTR);

            ~CodeEditor() override;

            /**
             * Method you can use to obtain the current line number area instance.
             *
             * \return Returns a pointer to the line number area instance.
             */
            CodeEditorLineNumberArea* lineNumberArea() const;

        public slots:
            /**
             * Slot you can use to update the line number area.
             *
             * \param[in] newLineNumberArea The new line number area.  If this code editor is the parent of the current
             *                              line number area, the current line number area will be deleted.
             */
            void setLineNumberArea(CodeEditorLineNumberArea* newLineNumberArea);

        protected:
            /**
             * Method that is called when this widget is resized.
             *
             * \param[in] event The event that triggered the call to this method.
             */
            void resizeEvent(QResizeEvent* event) override;

        private:
            /**
             * Method that is called by the constructors to configure the widget.
             */
            void configureWidget();

            /**
             * The current line number area.
             */
            CodeEditorLineNumberArea* currentLineNumberArea;
    };
}

#endif
