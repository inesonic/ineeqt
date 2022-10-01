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
* This file implements the \ref EQt::DockWidgetDefaults class.
***********************************************************************************************************************/

#include <QString>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QHash>

#include <util_hash_functions.h>

#include "eqt_dock_widget_defaults.h"

/***********************************************************************************************************************
 * EQt::DockWidgetDefaults::Private
 */

namespace EQt {
    DockWidgetDefaults::Private::Private(
            DockWidgetDefaults::Area defaultDockArea,
            bool                     defaultVisible,
            bool                     defaultFloating
        ) {
        currentDockArea           = defaultDockArea;
        currentIsVisibleByDefault = defaultVisible;
        currentFloatingByDefault  = defaultFloating;
        currentRelationship       = Relationship::NONE;
    }


    DockWidgetDefaults::Private::Private(
            DockWidgetDefaults::Relationship relationship,
            const QString&                   siblingDockName,
            bool                             defaultVisible
        ) {
        currentDockArea           = Area::BASED_ON_SIBLING;
        currentRelationship       = relationship;
        currentSibling            = siblingDockName;
        currentIsVisibleByDefault = defaultVisible;
        currentFloatingByDefault  = false;
    }


    DockWidgetDefaults::Private::Private(
            const QString&           dockName,
            DockWidgetDefaults::Area defaultDockArea,
            bool                     defaultVisible,
            bool                     defaultFloating
        ) {
        currentDockName           = dockName;
        currentDockArea           = defaultDockArea;
        currentIsVisibleByDefault = defaultVisible;
        currentFloatingByDefault  = defaultFloating;
        currentRelationship       = Relationship::NONE;
    }


    DockWidgetDefaults::Private::Private(
            const QString&                   dockName,
            DockWidgetDefaults::Relationship relationship,
            const QString&                   siblingDockName,
            bool                             defaultVisible
        ) {
        currentDockName           = dockName;
        currentDockArea           = Area::BASED_ON_SIBLING;
        currentRelationship       = relationship;
        currentSibling            = siblingDockName;
        currentIsVisibleByDefault = defaultVisible;
        currentFloatingByDefault  = false;
    }


    bool DockWidgetDefaults::Private::isExplicitPosition() const {
        return currentDockArea != Area::BASED_ON_SIBLING;
    }


    bool DockWidgetDefaults::Private::isImplicitPosition() const {
        return !isExplicitPosition();
    }


    void DockWidgetDefaults::Private::setDockName(const QString& newName) {
        currentDockName = newName;
    }


    QString DockWidgetDefaults::Private::dockName() const {
        return currentDockName;
    }


    void DockWidgetDefaults::Private::setDockArea(DockWidgetDefaults::Area newDockArea) {
        currentDockArea     = newDockArea;
        currentRelationship = Relationship::NONE;
        currentSibling.clear();
    }


    DockWidgetDefaults::Area DockWidgetDefaults::Private::dockArea() const {
        return currentDockArea;
    }


    void DockWidgetDefaults::Private::setDefaultVisible(bool nowVisible) {
        currentIsVisibleByDefault = nowVisible;
    }


    void DockWidgetDefaults::Private::setDefaultHidden(bool nowHidden) {
        setDefaultVisible(!nowHidden);
    }


    bool DockWidgetDefaults::Private::defaultVisible() const {
        return currentIsVisibleByDefault;
    }


    bool DockWidgetDefaults::Private::defaultHidden() const {
        return !defaultVisible();
    }


    void DockWidgetDefaults::Private::setDefaultFloating(bool nowFloating) {
        currentFloatingByDefault = nowFloating;
    }


    void DockWidgetDefaults::Private::setDefaultDocked(bool nowDocked) {
        setDefaultFloating(!nowDocked);
    }


    bool DockWidgetDefaults::Private::defaultFloating() const {
        return currentFloatingByDefault;
    }


    bool DockWidgetDefaults::Private::defaultDocked() const {
        return !defaultFloating();
    }


    void DockWidgetDefaults::Private::setRelationship(
            DockWidgetDefaults::Relationship relationship,
            const QString&                   siblingDockName
        ) {
        currentDockArea     = Area::BASED_ON_SIBLING;
        currentRelationship = relationship;
        currentSibling      = siblingDockName;
    }


    DockWidgetDefaults::Relationship DockWidgetDefaults::Private::relationship() const {
        return currentRelationship;
    }


    QString DockWidgetDefaults::Private::siblingDockName() const {
        return currentSibling;
    }
}

/***********************************************************************************************************************
 * EQt::DockWidgetDefaults::Private
 */

namespace EQt {
    DockWidgetDefaults::DockWidgetDefaults() {}


    DockWidgetDefaults::DockWidgetDefaults(
            DockWidgetDefaults::Area defaultDockArea,
            bool                     defaultVisible,
            bool                     defaultFloating
        ):impl(new Private(
            defaultDockArea,
            defaultVisible,
            defaultFloating
        )) {}


    DockWidgetDefaults::DockWidgetDefaults(
            DockWidgetDefaults::Relationship relationship,
            const QString&                   siblingDockName,
            bool                             defaultVisible
        ):impl(new Private(
            relationship,
            siblingDockName,
            defaultVisible
        )) {}


    DockWidgetDefaults::DockWidgetDefaults(
            const QString&           dockName,
            DockWidgetDefaults::Area defaultDockArea,
            bool                     defaultVisible,
            bool                     defaultFloating
        ):impl(new Private(
            dockName,
            defaultDockArea,
            defaultVisible,
            defaultFloating
        )) {}


    DockWidgetDefaults::DockWidgetDefaults(
            const QString&                   dockName,
            DockWidgetDefaults::Relationship relationship,
            const QString&                   siblingDockName,
            bool                             defaultVisible
        ):impl(new Private(
            dockName,
            relationship,
            siblingDockName,
            defaultVisible
        )) {}


    DockWidgetDefaults::DockWidgetDefaults(const DockWidgetDefaults& other):impl(other.impl) {}


    DockWidgetDefaults::~DockWidgetDefaults() {}


    bool DockWidgetDefaults::isValid() const {
        return impl.data() != nullptr;
    }


    bool DockWidgetDefaults::isInvalid() const {
        return !isValid();
    }


    bool DockWidgetDefaults::isExplicitPosition() const {
        return impl->isExplicitPosition();
    }


    bool DockWidgetDefaults::isImplicitPosition() const {
        return impl->isImplicitPosition();
    }


    void DockWidgetDefaults::setDockName(const QString& newName) {
        impl->setDockName(newName);
    }


    QString DockWidgetDefaults::dockName() const {
        return impl->dockName();
    }


    void DockWidgetDefaults::setDockArea(DockWidgetDefaults::Area newDockArea) {
        impl->setDockArea(newDockArea);
    }


    DockWidgetDefaults::Area DockWidgetDefaults::dockArea() const {
        return impl->dockArea();
    }


    void DockWidgetDefaults::setDefaultVisible(bool nowVisible) {
        impl->setDefaultVisible(nowVisible);
    }


    void DockWidgetDefaults::setDefaultHidden(bool nowHidden) {
        impl->setDefaultHidden(nowHidden);
    }


    bool DockWidgetDefaults::defaultVisible() const {
        return impl->defaultVisible();
    }


    bool DockWidgetDefaults::defaultHidden() const {
        return impl->defaultHidden();
    }


    void DockWidgetDefaults::setDefaultFloating(bool nowFloating) {
        impl->setDefaultFloating(nowFloating);
    }


    void DockWidgetDefaults::setDefaultDocked(bool nowDocked) {
        impl->setDefaultDocked(nowDocked);
    }


    bool DockWidgetDefaults::defaultFloating() const {
        return impl->defaultFloating();
    }


    bool DockWidgetDefaults::defaultDocked() const {
        return impl->defaultDocked();
    }


    void DockWidgetDefaults::setRelationship(
            DockWidgetDefaults::Relationship relationship,
            const QString&                   siblingDockName
        ) {
        impl->setRelationship(relationship, siblingDockName);
    }


    DockWidgetDefaults::Relationship DockWidgetDefaults::relationship() const {
        return impl->relationship();
    }


    QString DockWidgetDefaults::siblingDockName() const {
        return impl->siblingDockName();
    }


    DockWidgetDefaults& DockWidgetDefaults::operator=(const DockWidgetDefaults& other) {
        impl = other.impl;
        return *this;
    }


    Util::HashResult qHash(DockWidgetDefaults::Area value, Util::HashSeed seed) {
        return ::qHash(static_cast<int>(value), seed);
    }
}
