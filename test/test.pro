##-*-makefile-*-########################################################################################################
# Copyright 2016 - 2022 Inesonic, LLC
# 
# This file is licensed under two licenses.
#
# Inesonic Commercial License, Version 1:
#   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
#   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
#   strictly prohibited.
#
# GNU Public License, Version 2:
#   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
#   version.
#   
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#   details.
#   
#   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
#   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
########################################################################################################################

########################################################################################################################
# Basic build characteristics
#

TEMPLATE = app
QT += core testlib gui widgets network
CONFIG += testcase c++14

HEADERS = application_wrapper.h \
          test_global_setting.h \
          test_signal_aggregator.h \
          test_unique_application.h \
          test_programmatic_dock_widget.h \
          test_programmatic_main_window.h \

SOURCES = test_ineeqt.cpp \
          application_wrapper.cpp \
          test_global_setting.cpp \
          test_signal_aggregator.cpp \
          test_unique_application.cpp \
          test_programmatic_dock_widget.cpp \
          test_programmatic_main_window.cpp \

########################################################################################################################
# Libraries
#

INEEQT_BASE = $${OUT_PWD}/../ineeqt
INCLUDEPATH += $${PWD}/../ineeqt/include

INCLUDEPATH += $${INECRYPTO_INCLUDE}
INCLUDEPATH += $${INEUTIL_INCLUDE}
INCLUDEPATH += $${BOOST_INCLUDE}

unix {
    CONFIG(debug, debug|release) {
        LIBS += -L$${INEEQT_BASE}/build/debug/ -lineeqt
        !macx {
            PRE_TARGETDEPS += $${INEEQT_BASE}/build/debug/libineeqt.so
        } else {
            PRE_TARGETDEPS += $${INEEQT_BASE}/build/debug/libineeqt.dylib
        }
    } else {
        LIBS += -L$${INEEQT_BASE}/build/release/ -lineeqt
        !macx {
            PRE_TARGETDEPS += $${INEEQT_BASE}/build/release/libineeqt.so
        } else {
            PRE_TARGETDEPS += $${INEEQT_BASE}/build/release/libineeqt.dylib
        }
    }

    LIBS += -L$${INECRYPTO_LIBDIR} -linecrypto
    LIBS += -L$${INEUTIL_LIBDIR} -lineutil
}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += $${INEEQT_BASE}/build/Debug/ineeqt.lib
        PRE_TARGETDEPS += $${INEEQT_BASE}/build/Debug/ineeqt.lib
    } else {
        LIBS += $${INEEQT_BASE}/build/Release/ineeqt.lib
        PRE_TARGETDEPS += $${INEEQT_BASE}/build/Release/ineeqt.lib
    }

    LIBS += -L$${INECRYPTO_LIBDIR}/inecrypto.lib
    LIBS += -L$${INEUTIL_LIBDIR}/ineutil.lib
}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = test_ineeqt

CONFIG(debug, debug|release) {
    unix:DESTDIR = build/debug
    win32:DESTDIR = build/Debug
} else {
    unix:DESTDIR = build/release
    win32:DESTDIR = build/Release
}

OBJECTS_DIR = $${DESTDIR}/objects
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui
