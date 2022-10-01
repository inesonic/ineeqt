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
* This file implements the \ref EQt::FileDialog class.
***********************************************************************************************************************/

#include <QFileDialog>
#include <QObject>
#include <QStandardPaths>
#include <QDialogButtonBox>
#include <QToolButton>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QSettings>
#include <QFileInfo>
#include <QWidget>
#include <QCloseEvent>

#include "eqt_common.h"
#include "eqt_application.h"
#include "eqt_file_selector_widget.h"

namespace EQt {
    FileSelectorWidget::FileSelectorWidget(QWidget* parent):QFileDialog(parent) {
        configureWidget();
    }


    FileSelectorWidget::FileSelectorWidget(
            FileSelectorWidget::AcceptMode acceptMode,
            QWidget*                       parent
        ):QFileDialog(
            parent
        ) {
        configureWidget();
        setAcceptMode(acceptMode);
    }


    FileSelectorWidget::FileSelectorWidget(
            FileSelectorWidget::FileMode   fileMode,
            FileSelectorWidget::AcceptMode acceptMode,
            QWidget*                       parent
        ):QFileDialog(
            parent
        ) {
        configureWidget();
        setAcceptMode(acceptMode);
        setFileMode(fileMode);
    }


    FileSelectorWidget::~FileSelectorWidget() {}


    void FileSelectorWidget::loadSettings(const QSettings* settings, const QString& defaultDirectory) {
        QStringList history = settings->value("history").toStringList();
        setHistory(history);

        QString currentDirectory = settings->value("directory").toString();

        if (currentDirectory.isEmpty() && !defaultDirectory.isEmpty()) {
            currentDirectory = defaultDirectory;
        }

        if (currentDirectory.isEmpty()) {
            QStringList locations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
            currentDirectory = locations.first();
        }

        if (!currentDirectory.isEmpty()) {
            setDirectory(currentDirectory);
        }

        QString viewMode = settings->value("view_mode", "DETAIL").toString();
        if (viewMode == "DETAIL") {
            setViewMode(ViewMode::Detail);
        } else if (viewMode == "LIST") {
            setViewMode(ViewMode::List);
        } else {
            setViewMode(ViewMode::Detail);
        }
    }


    void FileSelectorWidget::saveSettings(QSettings* settings) {
        QStringList history = FileSelectorWidget::history();
        settings->setValue("history", history);

        settings->setValue("directory", directory().absolutePath());

        switch (viewMode()) {
            case ViewMode::Detail: {
                settings->setValue("view_mode", "DETAIL");
                break;
            }

            case ViewMode::List: {
                settings->setValue("view_mode", "LIST");
                break;
            }

            default: {
                Q_ASSERT(false);
                break;
            }
        }
    }


    void FileSelectorWidget::setOption(Option option, bool on) {
        if (option != FileSelectorWidget::DontUseNativeDialog) {
            QFileDialog::setOption(option, on);
        }
    }


    void FileSelectorWidget::setOptions(Options options) {
        QFileDialog::setOptions(options | FileSelectorWidget::DontUseNativeDialog);
    }


    bool FileSelectorWidget::lineEditorAutoUpdateEnabled() const {
        return currentLineEditorAutoUpdateEnabled;
    }


    bool FileSelectorWidget::lineEditorAutoUpdateDisabled() const {
        return !currentLineEditorAutoUpdateEnabled;
    }


    void FileSelectorWidget::setLineEditorAutoUpdateEnabled(bool nowAutoUpdate) {
        currentLineEditorAutoUpdateEnabled = nowAutoUpdate;
    }


    void FileSelectorWidget::setLineEditorAutoUpdateDisabled(bool nowNoAutoUpdate) {
        currentLineEditorAutoUpdateEnabled = !nowNoAutoUpdate;
    }


    void FileSelectorWidget::closeEvent(QCloseEvent* event) {
        QFileDialog::closeEvent(event);
        event->ignore();
    }


    void FileSelectorWidget::processCurrentChanged(const QString& newPath) {
        if (currentLineEditorAutoUpdateEnabled) {
            QFileInfo pathInformation(newPath);
            bool     updatePath = false;

            switch (fileMode()) {
                case FileSelectorWidget::FileMode::AnyFile: {
                    updatePath = true;
                    break;
                }

                case FileSelectorWidget::FileMode::ExistingFile:
                case FileSelectorWidget::FileMode::ExistingFiles: {
                    updatePath = pathInformation.exists() && !pathInformation.isDir();
                    break;
                }

                case FileSelectorWidget::FileMode::Directory: {
                    updatePath = pathInformation.exists() && pathInformation.isDir();
                    break;
                }

                default: {
                    Q_ASSERT(false);
                    break;
                }
            }

            if (updatePath) {
                currentLineEditor->setText(pathInformation.fileName());
            }
        }
    }


    void FileSelectorWidget::processCurrentUrlChanged(const QUrl& newUrl) {
        if (currentLineEditorAutoUpdateEnabled) {
            if (newUrl.isLocalFile()) {
                processCurrentChanged(newUrl.toLocalFile());
            } else {
                currentLineEditor->setText(newUrl.fileName());
            }
        }
    }


    void FileSelectorWidget::processDirectoryEntered(const QString&) {
        if (currentLineEditorAutoUpdateEnabled) {
            currentLineEditor->clear();
        }
    }


    void FileSelectorWidget::ProcessDirectoryUrlEntered(const QUrl&) {
        if (currentLineEditorAutoUpdateEnabled) {
            currentLineEditor->clear();
        }
    }


    void FileSelectorWidget::configureWidget() {
        setWindowFlags(Qt::Widget);
        setOptions(QFileDialog::DontUseNativeDialog);
        setSizeGripEnabled(false);

        const QObjectList& children        = FileSelectorWidget::children();
        QDialogButtonBox* dialogButtonBox  = Q_NULLPTR;

        currentLineEditor = Q_NULLPTR;

        for (QObjectList::const_iterator it=children.constBegin(),end=children.constEnd() ; it!=end ; ++it) {
            QObject* childObject = *it;
            QString  objectName  = childObject->objectName();

            if (objectName == "fileNameEdit") {
                Q_ASSERT(currentLineEditor == Q_NULLPTR);
                currentLineEditor = dynamic_cast<QLineEdit*>(childObject);
            } else if (objectName == "buttonBox") {
                Q_ASSERT(dialogButtonBox == Q_NULLPTR);
                dialogButtonBox = dynamic_cast<QDialogButtonBox*>(childObject);
            }
        }

        Q_ASSERT(currentLineEditor != Q_NULLPTR);
        Q_ASSERT(dialogButtonBox  != Q_NULLPTR);

        connect(currentLineEditor, &QLineEdit::textEdited, this, &FileSelectorWidget::fileNameTextEdited);
        connect(currentLineEditor, &QLineEdit::textChanged, this, &FileSelectorWidget::fileNameTextChanged);

        connect(this, &FileSelectorWidget::currentChanged, this, &FileSelectorWidget::processCurrentChanged);
        connect(this, &FileSelectorWidget::currentUrlChanged, this, &FileSelectorWidget::processCurrentUrlChanged);
        connect(this, &FileSelectorWidget::directoryEntered, this, &FileSelectorWidget::processDirectoryEntered);
        connect(this, &FileSelectorWidget::directoryUrlEntered, this, &FileSelectorWidget::ProcessDirectoryUrlEntered);

        dialogButtonBox->hide();
    }
}
