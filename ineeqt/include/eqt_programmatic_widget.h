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
* This header defines the \ref EQt::ProgrammaticWidget class.
***********************************************************************************************************************/

/* .. sphinx-project ineeqt */

#ifndef EQT_PROGRAMMATIC_WIDGET_H
#define EQT_PROGRAMMATIC_WIDGET_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>

class QWidget;
class QLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QFormLayout;

#include "eqt_common.h"

namespace EQt {
    /**
     * Base class that provides a set of features that are common to multiple programmatic classes.  Classes such as
     * \ref ProgrammaticMainWindow and \ref ProgrammaticDialog should inherit from the template class
     * \ref ProgrammaticWidget which will in turn inherit this class.
     */
    class EQT_PUBLIC_API ProgrammaticWidget {
        public:
            ProgrammaticWidget();

            virtual ~ProgrammaticWidget();

            /**
             * Obtains a layout, by name.
             *
             * \tparam    T          The layout type.
             *
             * \param[in] layoutName The name to use to reference the layout.
             *
             * \return Returns a pointer to the layout.  A null pointer is returned if the layout does not exist or if
             *         the incorrect type is specified.
             */
            template<class T> inline T* layout(const QString& layoutName) const {
                return dynamic_cast<T*>(layouts.value(layoutName));
            }

            /**
             * Registers a layout with this dialog.
             *
             * \param[in] layout     The layout to be registered.
             *
             * \param[in] layoutName The name to use to reference the layout.  If the name is an empty string, the
             *                       layout is not registered.
             */
            void registerLayout(QLayout* layout, const QString& layoutName);

            /**
             * Obtains a widget, by name.
             *
             * \tparam    T          The widget type.
             *
             * \param[in] widgetName The name to use to reference the widget.
             *
             * \return Returns a pointer to the widget.  A null pointer is returned if the widget does not exist or
             *         if the incorrect type is specified.
             */
            template<class T> EQT_PUBLIC_TEMPLATE_METHOD T* widget(const QString& widgetName) const {
                return dynamic_cast<T*>(widgets.value(widgetName));
            }

            /**
             * Registers a widget with this dialog.
             *
             * \param[in] widget     The widget to be registered.
             *
             * \param[in] widgetName The name to use to reference the widget.  If the name is an empty string, the
             *                       widget is not registered.
             */
            void registerWidget(QWidget* widget, const QString& widgetName);

            /**
             * Registers or updates a widget with this dialog.
             *
             * \param[in] widget     The widget to be registered.
             *
             * \param[in] widgetName The name to use to reference the widget.  If the name is an empty string, the
             *                       widget is not registered.
             */
            void reRegisterWidget(QWidget* widget, const QString& widgetName);

            /**
             * Obtains an object, by name.
             *
             * \param[in] objectName The name to use to reference the object.
             *
             * \return Returns a pointer to the object.  A null pointer is returned if the object does not exist or
             *         if the incorrect type is specified.
             */
             template<class T> EQT_PUBLIC_TEMPLATE_METHOD T* object(const QString& objectName) const {
                return dynamic_cast<T*>(objects.value(objectName));
            }

            /**
             * Registers a object with this dialog.
             *
             * \param[in] object     The object to be registered.
             *
             * \param[in] objectName The name to use to reference the object.  If the name is an empty string, the
             *                       object is not registered.
             */
            void registerObject(QObject* object, const QString& objectName);

            /**
             * Creates a horizonal layout for the dialog with the proper settings.  The layout is automatically
             * registered with the dialog.
             *
             * \param[in] layoutName The name to use to reference the layout.  If the name is an empty string, the
             *                       layout is not registered.
             *
             * \return Returns a pointer to the newly created layout.
             */
            QHBoxLayout* newHBoxLayout(const QString& layoutName);

            /**
             * Creates a vertical layout for the dialog with the proper settings.  The layout is automatically
             * registered with the dialog.
             *
             * \param[in] layoutName The name to use to reference the layout.  If the name is an empty string, the
             *                       layout is not registered.
             *
             * \return Returns a pointer to the newly created layout.
             */
            QVBoxLayout* newVBoxLayout(const QString& layoutName);

            /**
             * Creates a grid layout for the dialog with the proper settings.  The layout is automatically registered
             * with the dialog.
             *
             * \param[in] layoutName The name to use to reference the layout.  If the name is an empty string, the
             *                       layout is not registered.
             *
             * \return Returns a pointer to the newly created layout.
             */
            QGridLayout* newGridLayout(const QString& layoutName);

            /**
             * Creates a form layout for the dialog with the proper settings.  The layout is automatically registered
             * with the dialog.
             *
             * \param[in] layoutName The name to use to reference the layout.  If the name is an empty string, the
             *                       layout is not registered.
             *
             * \return Returns a pointer to the newly created layout.
             */
            QFormLayout* newFormLayout(const QString& layoutName);

        protected:
            /**
             * Method that is called by the layout functions to perform any customization of the layouts.
             *
             * \param[in] layout The layout to be customized.
             */
            virtual void customizeLayout(QLayout* layout);

            /**
             * Method that is called by \ref ProgrammaticWidget::newHBoxLayout to customize a layout.  The default
             * implementation calls \ref ProgrammaticWidget::customizeLayout.
             *
             * \param[in] layout The layout to be customized.
             */
            virtual void customizeHBoxLayout(QHBoxLayout* layout);

            /**
             * Method that is called by \ref ProgrammaticWidget::newVBoxLayout to customize a layout.  The default
             * implementation calls \ref ProgrammaticWidget::customizeLayout.
             *
             * \param[in] layout The layout to be customized.
             */
            virtual void customizeVBoxLayout(QVBoxLayout* layout);

            /**
             * Method that is called by \ref ProgrammaticWidget::newGridLayout to customize a layout.  The default
             * implementation calls \ref ProgrammaticWidget::customizeLayout.
             *
             * \param[in] layout The layout to be customized.
             */
            virtual void customizeGridLayout(QGridLayout* layout);

            /**
             * Method that is called by \ref ProgrammaticWidget::newFormLayout to customize a layout.  The default
             * implementation calls \ref ProgrammaticWidget::customizeLayout.
             *
             * \param[in] layout The layout to be customized.
             */
            virtual void customizeFormLayout(QFormLayout* layout);

        private:
            /**
             * Map of layouts.  Builders can access this map via the \ref ProgrammaticWidget::layout method.
             */
            QMap<QString, QLayout*> layouts;

            /**
             * Map of widgets.  Builders can register widgets that may be needed by multiple proxy classes.  You can
             * access this map via the \ref ProgrammaticWidget::widget method.  You can also register widgets using the
             * \ref ProgrammaticWidget::registerWidget method.
             */
            QMap<QString, QWidget*> widgets;

            /**
             * Map of objects.  Builders can register objects that may be needed by multiple proxy classes.  You can
             * access this map via the \ref ProgrammaticWidget::object method.  You can also register objects using the
             * \ref ProgrammaticWidget::registerObject method.
             */
            QMap<QString, QObject*> objects;
    };
}

#endif
