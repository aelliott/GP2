#
# Build file for GP Developer
#
# Copyright 2013 Alex Elliott <alex@alex-elliott.co.uk>
#
# ------------------------------------------------------------------------------
#
# This file defines the rules necessary to create GP Developer, the GP2 system's
# new Integrated Development Environment
#

# Get all of the .cpp files via a glob pattern, we need to build all of them
FILE(GLOB_RECURSE GPDeveloper_CPP_SRCS src/developer/*.cpp)

# This is a Qt-based application, and therefore we need a list of header files
# which contain a QObject-derived class so that they can be passed through moc
# (the Qt meta-object compiler)
#
# To any unfamiliar with Qt this is what causes the build system to generate
# moc_${filename}.cxx files and then compile them.
SET(GPDeveloper_HPP_SRCS
    src/developer/graphview/edgeitem.hpp
    src/developer/graphview/graphwidget.hpp
    src/developer/graphview/nodeitem.hpp
    src/developer/preferences/preferencesdialog.hpp
    src/developer/preferences/preferencespage.hpp
    src/developer/preferences/projectpreferences.hpp
    src/developer/aboutdialog.hpp
    src/developer/edit.hpp
    src/developer/gpfile.hpp
    src/developer/graph.hpp
    src/developer/helpdialog.hpp
    src/developer/mainwindow.hpp
    src/developer/newprojectwizard.hpp
    src/developer/prettytab.hpp
    src/developer/prettytabbar.hpp
    src/developer/prettytabheader.hpp
    src/developer/prettytabmenu.hpp
    src/developer/prettytabwidget.hpp
    src/developer/program.hpp
    src/developer/project.hpp
    src/developer/quickrunwidget.hpp
    src/developer/results.hpp
    src/developer/rule.hpp
    src/developer/run.hpp
    src/developer/runconfiguration.hpp
    src/developer/styledbutton.hpp
    src/developer/welcome.hpp
)

# The UI files must be passed to uic as well to be compiled into headers
SET(GPDeveloper_UI_SRCS
    src/developer/preferences/preferencesdialog.ui
    src/developer/preferences/projectpreferences.ui
    src/developer/aboutdialog.ui
    src/developer/edit.ui
    src/developer/helpdialog.ui
    src/developer/mainwindow.ui
    src/developer/newprojectwizard.ui
    src/developer/prettytabheader.ui
    src/developer/prettytabwidget.ui
    src/developer/quickrunwidget.ui
    src/developer/results.ui
    src/developer/run.ui
    src/developer/runconfiguration.ui
    src/developer/styledbutton.ui
    src/developer/welcome.ui
)

# Finally the QRC resource files must also be compiled into .cpp (or as CMake
# uses .cxx) files so that they can become objects in the final linking phase.
SET(GPDeveloper_QRC_SRCS
    src/developer/icons.qrc
    src/developer/images.qrc
    src/developer/stylesheets.qrc
    src/developer/templates.qrc
)

# We need to link against the Qt libraries located in the top-level
# CMakeLists.txt
SET(GPDeveloper_LINK_LIBS ${QT_LIBRARIES})

# These lines tell the build system to perform the tasks mentioned above
QT4_ADD_RESOURCES(GPDeveloper_CPP_SRCS ${GPDeveloper_QRC_SRCS})
QT4_WRAP_UI(GPDeveloper_CPP_SRCS ${GPDeveloper_UI_SRCS})
QT4_WRAP_CPP(GPDeveloper_CPP_SRCS ${GPDeveloper_HPP_SRCS})

# This block covers the actual creation of the gpdeveloper binary
IF(UNIX)
    ADD_EXECUTABLE(gpdeveloper ${GPDeveloper_CPP_SRCS})
ELSEIF(WIN32)
    ADD_EXECUTABLE(gpdeveloper WIN32 ${GPDeveloper_CPP_SRCS})
ENDIF()

# Which is then linked against the located libraries (such as Qt) to produce an
# executable which can actually run
TARGET_LINK_LIBRARIES(gpdeveloper ${GPDeveloper_LINK_LIBS} ${libGP_NAME})

# Per-platform CPack Configuration
IF(UNIX AND NOT APPLE)
    INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/gpdeveloper DESTINATION bin)
ELSEIF(APPLE)
    SET(CPACK_GENERATOR Bundle)
    INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/gpdeveloper DESTINATION bin)
ELSEIF(WIN32)
    SET(CPACK_GENERATOR NSIS)
    SET(CPACK_NSIS_MUI_ICON ${GP2_SOURCE_DIR}/src/developer/images/graph.ico)

    # Get the Qt DLLs for packaging with this executable
    #INSTALL(PROGRAMS ${QT_BINARY_DIR}/QtCore4.dll
    #                 ${QT_BINARY_DIR}/QtGui4.dll
    #                 ${QT_BINARY_DIR}/mingwm10.dll
    #                 ${QT_BINARY_DIR}/libgcc_s_dw2-1.dll
    #        DESTINATION bin)

    INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/gpdeveloper.exe DESTINATION bin)
ENDIF()

# Bring in the CMakeLists.txt for the developer test cases
INCLUDE("src/developer/tests/CMakeLists.txt")