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
* This file implements the \ref EQt::CppSyntaxHighlighter class.
***********************************************************************************************************************/

#include <QWidget>
#include <QString>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QVector>
#include <QSyntaxHighlighter>

#include "eqt_cpp_syntax_highlighter.h"

namespace EQt {
    struct HighlightingRule {
        HighlightingRule() {}

        ~HighlightingRule() {}

        QRegularExpression     pattern;
        const QTextCharFormat* format;
    };

    static QVector<HighlightingRule> highlightingRules;
    static QRegularExpression        commentStartExpression;
    static QRegularExpression        commentEndExpression;
    static QTextCharFormat           keywordFormat;
    static QTextCharFormat           singleLineCommentFormat;
    static QTextCharFormat           multiLineCommentFormat;
    static QTextCharFormat           quotationFormat;
    static QTextCharFormat           functionFormat;

    CppSyntaxHighlighter::CppSyntaxHighlighter(QTextDocument* parent):QSyntaxHighlighter(parent) {
        if (highlightingRules.isEmpty()) {
            keywordFormat.setForeground(Qt::darkBlue);
            keywordFormat.setFontWeight(QFont::Bold);

            singleLineCommentFormat.setForeground(Qt::red);
            multiLineCommentFormat.setForeground(Qt::red);
            quotationFormat.setForeground(Qt::darkGreen);

            functionFormat.setFontItalic(true);
            functionFormat.setForeground(Qt::blue);

            commentStartExpression = QRegularExpression("/\\*");
            commentEndExpression   = QRegularExpression("\\*/");

            HighlightingRule rule;

            QStringList keywordPatterns;
            keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                            << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                            << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                            << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                            << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                            << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                            << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                            << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                            << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                            << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b";

            for (  QStringList::const_iterator keywordIterator    = keywordPatterns.constBegin(),
                                               keywordEndIterator = keywordPatterns.constEnd()
                 ; keywordIterator != keywordEndIterator
                 ; ++keywordIterator
                ) {
                rule.pattern = QRegularExpression(*keywordIterator);
                rule.format  = &keywordFormat;

                highlightingRules.append(rule);
            }

            rule.pattern = QRegularExpression("//[^\n]*");
            rule.format  = &singleLineCommentFormat;
            highlightingRules.append(rule);

            rule.pattern = QRegularExpression("\".*\"");
            rule.format  = &quotationFormat;
            highlightingRules.append(rule);

            rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
            rule.format  = &functionFormat;
            highlightingRules.append(rule);
        }
    }


    CppSyntaxHighlighter::~CppSyntaxHighlighter() {}


    void CppSyntaxHighlighter::highlightBlock(const QString& text) {
        for (  QVector<HighlightingRule>::const_iterator ruleIterator    = highlightingRules.constBegin(),
                                                         ruleEndIterator = highlightingRules.constEnd()
             ; ruleIterator != ruleEndIterator
             ; ++ruleIterator
            ) {
            const QRegularExpression& pattern = ruleIterator->pattern;
            const QTextCharFormat*    format  = ruleIterator->format;

            QRegularExpressionMatchIterator matchIterator = pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), *format);
            }
        }

        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1) {
            startIndex = text.indexOf(commentStartExpression);
        }

        while (startIndex >= 0) {
            QRegularExpressionMatch match         = commentEndExpression.match(text, startIndex);
            int                     endIndex      = match.capturedStart();
            unsigned                commentLength = 0;

            if (endIndex < 0) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex + match.capturedLength();
            }

            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
    }
}
