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
* This header defines the \ref EQt::CppSyntaxHighlighter class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_CPP_SYNTAX_HIGHLIGHTER_H
#define EQT_CPP_SYNTAX_HIGHLIGHTER_H

#include <QWidget>
#include <QString>
#include <QSyntaxHighlighter>
#include <QVector>

#include "eqt_common.h"

class QTextDocument;

namespace EQt {
    /**
     * Class you can use in conjuction with a QPlainTextEdit instance or \ref EQt::CodeEditor instance to provide
     * syntax highlighting of C++ code.
     *
     * This code was heavily inspired by The Qt 5 syntax highlighter example.
     */
    class EQT_PUBLIC_API CppSyntaxHighlighter:public QSyntaxHighlighter {
        Q_OBJECT

        public:
            /**
             * Constructor.
             *
             * \param[in] parent The parent object for this window.
             */
            CppSyntaxHighlighter(QTextDocument* parent = Q_NULLPTR);

            ~CppSyntaxHighlighter() override;

        protected:
            /**
             * Method that is called to highlight a given block of text.  See the Qt 5 QSyntaxHighlighter documentation
             * for details.
             *
             * \param[in] text The text to be highlighed.
             */
            void highlightBlock(const QString &text) override;
    };
}

#endif
