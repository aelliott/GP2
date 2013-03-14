# GPDeveloper build file

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(GPDeveloper)

SET(GPDeveloper_VERSION_MAJOR 1)
SET(GPDeveloper_VERSION_MINOR 0)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${GPDeveloper_SOURCE_DIR}/src
    ${GPDeveloper_BINARY_DIR}
)

INCLUDE(CheckIncludeFiles)

SET(QT_MIN_VERSION "4.8.0")
FIND_PACKAGE(Qt4 REQUIRED)

SET(QT_USE_QTMAIN true)
SET(QT_USE_QTXML true)

INCLUDE(${QT_USE_FILE})

FILE(GLOB_RECURSE GPDeveloper_CPP_SRCS src/*.cpp)

SET(GPDeveloper_HPP_SRCS
    src/graphview/edgeitem.hpp
    src/graphview/graphwidget.hpp
    src/graphview/nodeitem.hpp
    src/preferences/preferencespage.hpp
    src/preferences/projectpreferences.hpp
    src/aboutdialog.hpp
    src/edit.hpp
    src/gpfile.hpp
    src/graph.hpp
    src/helpdialog.hpp
    src/mainwindow.hpp
    src/newprojectwizard.hpp
    src/preferencesdialog.hpp
    src/prettytab.hpp
    src/prettytabbar.hpp
    src/prettytabheader.hpp
    src/prettytabmenu.hpp
    src/prettytabwidget.hpp
    src/program.hpp
    src/project.hpp
    src/results.hpp
    src/rule.hpp
    src/run.hpp
    src/runconfiguration.hpp
    src/styledbutton.hpp
    src/welcome.hpp
)

SET(GPDeveloper_UI_SRCS
    src/preferences/projectpreferences.ui
    src/aboutdialog.ui
    src/edit.ui
    src/helpdialog.ui
    src/mainwindow.ui
    src/newprojectwizard.ui
    src/preferencesdialog.ui
    src/prettytabheader.ui
    src/prettytabwidget.ui
    src/results.ui
    src/run.ui
    src/runconfiguration.ui
    src/styledbutton.ui
    src/welcome.ui
)

SET(GPDeveloper_QRC_SRCS
    src/icons.qrc
    src/images.qrc
    src/stylesheets.qrc
    src/templates.qrc
)

SET(GPDeveloper_LINK_LIBS ${QT_LIBRARIES})

QT4_ADD_RESOURCES(GPDeveloper_CPP_SRCS ${GPDeveloper_QRC_SRCS})
QT4_WRAP_UI(GPDeveloper_CPP_SRCS ${GPDeveloper_UI_SRCS})
QT4_WRAP_CPP(GPDeveloper_CPP_SRCS ${GPDeveloper_HPP_SRCS})

IF(UNIX)
    ADD_EXECUTABLE(gpdeveloper ${GPDeveloper_CPP_SRCS})
ELSEIF(APPLE)
    SET(CMAKE_OSX_ARCHITECTURES i386)
    ADD_EXECUTABLE(gpdeveloper MACOSX_BUNDLE ${GPDeveloper_CPP_SRCS})
ELSEIF(WIN32)
    ADD_EXECUTABLE(gpdeveloper WIN32 ${GPDeveloper_CPP_SRCS})
ENDIF()

TARGET_LINK_LIBRARIES(gpdeveloper ${GPDeveloper_LINK_LIBS})
