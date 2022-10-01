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
* This file implements the \ref Application class.
***********************************************************************************************************************/

#include <QtGlobal>
#include <QString>
#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QStyle>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkProxyQuery>
#include <QNetworkProxyFactory>
#include <QNetworkProxy>
#include <QList>
#include <QPointer>
#include <QMap>
#include <QIcon>
#include <QDir>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <util_page_size.h>

#include "eqt_graphics_item.h"
#include "eqt_global_setting.h"
#include "eqt_recent_files_data.h"
#include "eqt_application.h"

namespace EQt {
    QList<QNetworkProxy> checkProxy(const QString& url) {
        QUrl webUrl(url);
        QNetworkProxyQuery networkProxyQuery(webUrl);
        return QNetworkProxyFactory::systemProxyForQuery(networkProxyQuery);
    }


    static struct IconNameFormats { char const* re; QIcon::Mode mode; } iconNameFormats[] = {
        { "_hot_([1-9][0-9]*)\\.png",      QIcon::Active },
        { "_disabled_([1-9][0-9]*)\\.png", QIcon::Disabled },
        { "_active_([1-9][0-9]*)\\.png",   QIcon::Normal },
        { "_([1-9][0-9]*)\\.png",          QIcon::Normal },
        { Q_NULLPTR, QIcon::Normal }
    };

    QMap<QString, QIcon> Application::icons;

    Application::Application(
            int&           applicationCount,
            char**         applicationValues,
            const QString& applicationName,
            const QString& versionNumber,
            const QString& organization,
            const QString& domain
        ):QApplication(
            applicationCount,
            applicationValues
        ) {
        setApplicationName(applicationName);
        setApplicationVersion(versionNumber);
        setOrganizationName(organization);
        setOrganizationDomain(domain);

        currentSettings             = Q_NULLPTR;
        currentLocale               = Q_NULLPTR;
        currentNetworkAccessManager = Q_NULLPTR;
        currentRecentFilesData      = Q_NULLPTR;

        connect(this, SIGNAL(lastWindowClosed()), this, SLOT(allWindowsClosed()));

        GraphicsItem::initializeGarbageCollector();
    }


    Application::~Application() {}


    QSettings* Application::settings() {
        Application* application = dynamic_cast<Application*>(QApplication::instance());

        if (application->currentSettings == Q_NULLPTR) {
            application->currentSettings = new QSettings(application);
        }

        return application->currentSettings;
    }


    QLocale* Application::locale() {
        Application* application = dynamic_cast<Application*>(QApplication::instance());

        if (application->currentLocale == Q_NULLPTR) {
            application->currentLocale = new QLocale;
        }

        return application->currentLocale;
    }


    QNetworkAccessManager* Application::networkAccessManager() {
        Application* application = dynamic_cast<Application*>(QApplication::instance());

        if (application->currentNetworkAccessManager == Q_NULLPTR) {
            application->currentNetworkAccessManager = new QNetworkAccessManager(application);
        }

        return application->currentNetworkAccessManager;
    }


    RecentFilesData* Application::recentFilesData() {
        Application* application = dynamic_cast<Application*>(QApplication::instance());

        if (application->currentRecentFilesData == Q_NULLPTR) {
            application->currentRecentFilesData = new RecentFilesData(application);
        }

        return application->currentRecentFilesData;
    }


    QIcon Application::icon(const QString& name) {
        if (!icons.contains(name)) {
            unsigned    numberMatches = 0;
            QStringList iconNames = QDir(":", name+"_*").entryList();
            QIcon       newIcon;

            for (auto it=iconNames.begin(),end=iconNames.end() ; it!=end ; ++it) {
                IconNameFormats*        format = iconNameFormats;
                QRegularExpressionMatch match;

                while (!match.hasMatch() && format->re != Q_NULLPTR) {
                    QRegularExpression re(name + format->re, QRegularExpression::CaseInsensitiveOption);
                    match = re.match(*it);

                    if (!match.hasMatch()) {
                        ++format;
                    }
                }

                if (match.hasMatch()) {
                    QIcon::Mode mode = format->mode;
                    bool        ok   = false;
                    unsigned    size = match.captured(1).toUInt(&ok);
                    Q_ASSERT(ok);

                    newIcon.addFile(":"+(*it), QSize(size, size), mode);

                    ++numberMatches;
                }
            }

            Q_ASSERT(numberMatches > 0);

            icons.insert(name, newIcon);
        }

        return icons.value(name);
    }


    void Application::allWindowsClosed() {}


    void Application::configureNetworkServices() {
        /* Code snippet below shamelessly inspired by:
         *   http://stackoverflow.com/questions/9526473/how-can-i-automatically-detect-a-proxy#9526533
         */

        /* TODO: Test this when a proxy server is available. */

        QString testServer = "http://www." + QApplication::organizationDomain();
        QList<QNetworkProxy> proxyList = checkProxy(testServer);

        if (proxyList.isEmpty()) {
            proxyList = checkProxy("http://www.google.com");
        }

        if (!proxyList.isEmpty()) {
            QNetworkProxy::setApplicationProxy(proxyList[0]);
        }
    }


    QStringList Application::paperSizeDescriptions() {
        QStringList      result;
        QLocale*         locale  = Application::locale();
        QLocale::Country country = locale->country();
        bool             metric  = (locale->measurementSystem() != QLocale::ImperialSystem);

        if (country == QLocale::Country::UnitedStates                     ||
            country == QLocale::Country::UnitedStatesMinorOutlyingIslands ||
            country == QLocale::Country::UnitedStatesVirginIslands        ||
            country == QLocale::Country::Canada                              ) {
            result << Util::PageSize::descriptions(Util::PageSize::Region::Us, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Britain, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Europe, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Japan, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::China, metric);
        } else if (country == QLocale::Country::China) {
            result << Util::PageSize::descriptions(Util::PageSize::Region::China, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Europe, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Japan, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Us, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Britain, metric);
        } else if (country == QLocale::Country::Japan) {
            result << Util::PageSize::descriptions(Util::PageSize::Region::Japan, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Europe, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::China, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Us, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Britain, metric);
        } else { // Assume country follows EU
            result << Util::PageSize::descriptions(Util::PageSize::Region::Europe, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Us, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Britain, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::Japan, metric);
            result << QString();
            result << Util::PageSize::descriptions(Util::PageSize::Region::China, metric);
        }

        return result;
    }
}

