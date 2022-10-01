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
* This file implements the \ref EQt::RecentFilesPopupMenu class.
***********************************************************************************************************************/

#include <QWidget>
#include <QAction>
#include <QActionGroup>
#include <QMenu>

#include "eqt_common.h"
#include "eqt_application.h"
#include "eqt_recent_files_data.h"
#include "eqt_recent_files_popup_menu.h"
#include "eqt_dynamic_popup_menu.h"

namespace EQt {
    RecentFilesPopupMenu::RecentFilesPopupMenu(QWidget* parent):EQt::DynamicPopupMenu(parent) {
        configureWidget();
        setRecentFilesData(Application::recentFilesData());
    }


    RecentFilesPopupMenu::RecentFilesPopupMenu(
            const QString& title,
            QWidget*       parent
        ):EQt::DynamicPopupMenu(
            title,
            parent
        ) {
        configureWidget();
        setRecentFilesData(Application::recentFilesData());
    }


    RecentFilesPopupMenu::RecentFilesPopupMenu(
            RecentFilesData* newRecentFilesData,
            QWidget*         parent
        ):EQt::DynamicPopupMenu(
            parent
        ) {
        configureWidget();
        setRecentFilesData(newRecentFilesData);
    }


    RecentFilesPopupMenu::RecentFilesPopupMenu(
            const QString&   title,
            RecentFilesData* newRecentFilesData,
            QWidget*         parent
        ):EQt::DynamicPopupMenu(
            title,
            parent
        ) {
        configureWidget();
        setRecentFilesData(newRecentFilesData);
    }


    RecentFilesPopupMenu::RecentFilesPopupMenu(QAction* action, QWidget* parent):EQt::DynamicPopupMenu(parent) {
        configureWidget();
        setRecentFilesData(Application::recentFilesData());
        setRecentFilesAction(action);
    }


    RecentFilesPopupMenu::RecentFilesPopupMenu(
            const QString& title,
            QAction*       action,
            QWidget*       parent
        ):EQt::DynamicPopupMenu(
            title,
            parent
        ) {
        configureWidget();
        setRecentFilesData(Application::recentFilesData());
        setRecentFilesAction(action);
    }


    RecentFilesPopupMenu::RecentFilesPopupMenu(
            RecentFilesData* newRecentFilesData,
            QAction*         action,
            QWidget*         parent
        ):EQt::DynamicPopupMenu(
            parent
        ) {
        configureWidget();
        setRecentFilesData(newRecentFilesData);
        setRecentFilesAction(action);
    }


    RecentFilesPopupMenu::RecentFilesPopupMenu(
            const QString&   title,
            RecentFilesData* newRecentFilesData,
            QAction*         action,
            QWidget*         parent
        ):EQt::DynamicPopupMenu(
            title,
            parent
        ) {
        configureWidget();
        setRecentFilesData(newRecentFilesData);
        setRecentFilesAction(action);
    }


    RecentFilesPopupMenu::~RecentFilesPopupMenu() {}


    void RecentFilesPopupMenu::setRecentFilesData(RecentFilesData* newRecentFilesData) {
        if (currentRecentFilesData != Q_NULLPTR) {
            disconnect(currentRecentFilesData, SIGNAL(changed()), this, SLOT(updateDynamicContent()));
        }

        currentRecentFilesData = newRecentFilesData;

        if (currentRecentFilesData != Q_NULLPTR) {
            connect(currentRecentFilesData, SIGNAL(changed()), this, SLOT(updateDynamicContent()));
            updateDynamicContent();
        }
    }


    void RecentFilesPopupMenu::addDynamicActions() {
        Q_ASSERT(currentRecentFilesData != Q_NULLPTR);

        clear();
        for (auto it=currentRecentFilesData->begin(),end=currentRecentFilesData->end() ; it!=end ; ++it) {
            RecentFile recentFile = *it;
            QAction*   newAction  = addAction(recentFile.description());

            newAction->setData(recentFile.absoluteFilePath());
            newAction->setEnabled(recentFile.exists() && recentFile.isFile() && recentFile.isReadable());
        }
    }


    void RecentFilesPopupMenu::dynamicActionSelected(QAction* action) {
        Q_ASSERT(currentRecentFilesAction != Q_NULLPTR);

        currentRecentFilesAction->setData(action->data().toString());
        currentRecentFilesAction->trigger();
    }


    void RecentFilesPopupMenu::configureWidget() {
        currentRecentFilesAction = Q_NULLPTR;
        currentRecentFilesData   = Q_NULLPTR;
    }
}
