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

TEMPLATE = lib

QT += core gui svg widgets network printsupport charts
CONFIG += shared c++14

equals(QT_MAJOR_VERSION, 6) {
    QT += svgwidgets
}

DEFINES += EQT_BUILD

unix:!macx {
    QT += dbus
}

win32 {
    # Windows produces a rather stupid warning indicating branch of a tree inherits a method when virtual inheritance is
    # used.  Since this warning is stating the intent of the inheritance, and clutters things needlessly, we disable it
    # for Windows.
    QMAKE_CXXFLAGS_WARN_ON += -wd4250
}

########################################################################################################################
# Public includes
#

INCLUDEPATH += include
API_HEADERS = include/eqt_common.h \
              include/eqt_charts.h \
              include/eqt_application.h \
              include/eqt_unique_application.h \
              include/eqt_global_setting.h \
              include/eqt_signal_aggregator.h \
              include/eqt_message_dialog.h \
              include/eqt_font_data.h \
              include/eqt_dock_widget_defaults.h \
              include/eqt_graphics_scene.h \
              include/eqt_graphics_item.h \
              include/eqt_graphics_text_item.h \
              include/eqt_graphics_rect_item.h \
              include/eqt_graphics_pixmap_item.h \
              include/eqt_graphics_svg_item.h \
              include/eqt_graphics_item_group.h \
              include/eqt_graphics_multi_text_group.h \
              include/eqt_graphics_math_group.h \
              include/eqt_graphics_math_group_with_line.h \
              include/eqt_graphics_math_group_with_painter_path.h \
              include/eqt_stacking_layout.h \
              include/eqt_chart_item.h \
              include/eqt_polar_2d_chart_item.h \
              include/eqt_paragraph_diagram.h \
              include/eqt_paragraph_dimension_widget.h \
              include/eqt_line_sample_widget.h \
              include/eqt_font_demo_widget.h \
              include/eqt_line_edit.h \
              include/eqt_dimension_validator.h \
              include/eqt_dimension_line_edit.h \
              include/eqt_search_line_edit.h \
              include/eqt_spin_box.h \
              include/eqt_double_spin_box.h \
              include/eqt_special_value_spin_box.h \
              include/eqt_combo_box.h \
              include/eqt_font_combo_box.h \
              include/eqt_zoom_combo_box.h \
              include/eqt_font_selector_widget.h \
              include/eqt_dynamic_popup_menu.h \
              include/eqt_recent_files_data.h \
              include/eqt_recent_files_popup_menu.h \
              include/eqt_scroll_bar.h \
              include/eqt_tool_button.h \
              include/eqt_color_tool_button.h \
              include/eqt_tool_button_array.h \
              include/eqt_font_button.h \
              include/eqt_tab_widget.h \
              include/eqt_list_validator_base.h \
              include/eqt_map_key_validator.h \
              include/eqt_command_line_edit.h \
              include/eqt_color_dialog.h \
              include/eqt_file_dialog.h \
              include/eqt_file_selector_widget.h \
              include/eqt_custom_paper_size_dialog.h \
              include/eqt_paper_size_popup_menu.h \
              include/eqt_progress_bar.h \
              include/eqt_code_editor.h \
              include/eqt_code_editor_line_number_area.h \
              include/eqt_cpp_syntax_highlighter.h \
\
              include/eqt_programmatic_application.h \
              include/eqt_programmatic_widget.h \
              include/eqt_programmatic_widget_proxy.h \
              include/eqt_programmatic_dock_widget.h \
              include/eqt_programmatic_window_base.h \
              include/eqt_programmatic_window_proxy.h \
              include/eqt_programmatic_window.h \
              include/eqt_programmatic_main_window.h \
              include/eqt_programmatic_main_window_proxy.h \
              include/eqt_programmatic_dialog.h \
              include/eqt_programmatic_dialog_proxy.h \
              include/eqt_programmatic_view.h \
              include/eqt_programmatic_view_proxy.h \
              include/eqt_builder_base.h \
              include/eqt_builder.h \

########################################################################################################################
# Source files
#

SOURCES = source/eqt_application.cpp \
          source/eqt_unique_application.cpp \
          source/eqt_global_setting.cpp \
          source/eqt_signal_aggregator.cpp \
          source/eqt_message_dialog.cpp \
          source/eqt_font_data.cpp \
          source/eqt_dock_widget_defaults.cpp \
          source/dock_widget_location.cpp \
          source/dock_widget_locations.cpp \
          source/eqt_graphics_scene.cpp \
          source/eqt_graphics_item.cpp \
          source/eqt_graphics_text_item.cpp \
          source/eqt_graphics_rect_item.cpp \
          source/eqt_graphics_pixmap_item.cpp \
          source/eqt_graphics_svg_item.cpp \
          source/eqt_graphics_item_group.cpp \
          source/eqt_graphics_multi_text_group.cpp \
          source/eqt_graphics_math_group.cpp \
          source/eqt_graphics_math_group_with_line.cpp \
          source/eqt_graphics_math_group_with_painter_path.cpp \
          source/eqt_stacking_layout.cpp \
          source/eqt_chart_item.cpp \
          source/eqt_polar_2d_chart_item.cpp \
          source/eqt_paragraph_diagram.cpp \
          source/eqt_paragraph_dimension_widget.cpp \
          source/eqt_line_sample_widget.cpp \
          source/eqt_font_demo_widget.cpp \
          source/eqt_line_edit.cpp \
          source/eqt_dimension_validator.cpp \
          source/eqt_dimension_line_edit.cpp \
          source/eqt_search_line_edit.cpp \
          source/eqt_spin_box.cpp \
          source/eqt_double_spin_box.cpp \
          source/eqt_special_value_spin_box.cpp \
          source/eqt_combo_box.cpp \
          source/eqt_font_combo_box.cpp \
          source/eqt_zoom_combo_box.cpp \
          source/eqt_font_selector_widget.cpp \
          source/eqt_dynamic_popup_menu.cpp \
          source/eqt_recent_files_data.cpp \
          source/eqt_recent_files_popup_menu.cpp \
          source/eqt_scroll_bar.cpp \
          source/eqt_tool_button.cpp \
          source/eqt_color_tool_button.cpp \
          source/eqt_font_button.cpp \
          source/eqt_tool_button_array.cpp \
          source/eqt_tab_widget.cpp \
          source/eqt_list_validator_base.cpp \
          source/eqt_command_line_edit.cpp \
          source/eqt_color_dialog.cpp \
          source/eqt_file_dialog.cpp \
          source/eqt_file_selector_widget.cpp \
          source/eqt_custom_paper_size_dialog.cpp \
          source/eqt_paper_size_popup_menu.cpp \
          source/eqt_progress_bar.cpp \
          source/eqt_code_editor.cpp \
          source/eqt_code_editor_line_number_area.cpp \
          source/eqt_cpp_syntax_highlighter.cpp \
\
          source/eqt_programmatic_application.cpp \
          source/eqt_programmatic_widget.cpp \
          source/eqt_programmatic_widget_proxy.cpp \
          source/eqt_programmatic_dock_widget.cpp \
          source/eqt_programmatic_window_base.cpp \
          source/eqt_programmatic_window_proxy.cpp \
          source/eqt_programmatic_main_window.cpp \
          source/eqt_programmatic_main_window_proxy.cpp \
          source/eqt_programmatic_dialog.cpp \
          source/eqt_programmatic_dialog_proxy.cpp \
          source/eqt_programmatic_view.cpp \
          source/eqt_programmatic_view_proxy.cpp \
          source/eqt_builder_base.cpp \
          source/eqt_builder.cpp \

########################################################################################################################
# Private includes
#

INCLUDEPATH += source
PRIVATE_HEADERS = source/dock_widget_location.h \
                  source/dock_widget_locations.h \

########################################################################################################################
# Setup headers and installation
#

HEADERS = $$CUSTOMER_HEADERS $$API_HEADERS $$PRIVATE_HEADERS

########################################################################################################################
# Libraries
#

defined(SETTINGS_PRI, var) {
    include($${SETTINGS_PRI})
}

INCLUDEPATH += $${INECRYPTO_INCLUDE}
INCLUDEPATH += $${INEUTIL_INCLUDE}
INCLUDEPATH += $${BOOST_INCLUDE}

LIBS += -L$${INECRYPTO_LIBDIR} -linecrypto
LIBS += -L$${INEUTIL_LIBDIR} -lineutil

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = ineeqt

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

