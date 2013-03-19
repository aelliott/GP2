#
# Build file for the GP2 system
#
# Copyright 2013 Alex Elliott <alex@alex-elliott.co.uk>
#
# ------------------------------------------------------------------------------
#
# This file sets up the environment for building the GP2 toolchain and GP
# Developer - the new GP Integrated Development Environment. Initially shared
# libraries are located, and then the CMakeLists.txt file for each of the tools
# is included in order to build those components.
#
# The components included are:
#   src/developer/CMakeLists.txt    - GP Developer
#   src/libgp/CMakeLists.txt        - GP2 utility library
#   src/gprc/CMakeLists.txt         - GP2 rule compiler
#   src/gpxc/CMakeLists.txt         - GP2 program compiler
#   src/gpmake/CMakeLists.txt       - GP2 build tool
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(GP2)

SET(GP2_VERSION_MAJOR 1)
SET(GP2_VERSION_MINOR 0)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${GP2_SOURCE_DIR}/src/developer
    ${GP2_SOURCE_DIR}/src/libgp
    ${GP2_SOURCE_DIR}/src/gprc
    ${GP2_SOURCE_DIR}/src/gpxc
    ${GP2_SOURCE_DIR}/src/gpmake
    ${GP2_BINARY_DIR}
)

INCLUDE(CheckIncludeFiles)

SET(QT_MIN_VERSION "4.8.0")
FIND_PACKAGE(Qt4 REQUIRED)

SET(QT_USE_QTMAIN true)
SET(QT_USE_QTXML true)

INCLUDE(${QT_USE_FILE})

# Enable unit testing
ENABLE_TESTING()

# Set CPack variables
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GP2 Tools")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_FILE_NAME "GP Developer")
SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/src/developer/images/graph.png")
SET(CPACK_BUNDLE_NAME "GP Developer")
SET(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/src/developer/images/icon.icns")
SET(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
SET(CPACK_BUNDLE_STARTUP_COMMAND gpdeveloper)
SET(CPACK_PACKAGE_EXECUTABLES gpdeveloper "GP Developer")
SET(CPACK_PACKAGE_NAME "GP2")
SET(CPACK_PACKAGE_VERSION_MAJOR ${GP2_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${GP2_VERSION_MINOR})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "GP2")

# Build libGP
INCLUDE("src/libgp/CMakeLists.txt")

# We want to be able to link against libGP, so make sure that the directory is checked when linking
LINK_DIRECTORIES(${GP2_BINARY_DIR})

# Build GP Developer
INCLUDE("src/developer/CMakeLists.txt")

# Build gprc
INCLUDE("src/gprc/CMakeLists.txt")

# Build gpxc
INCLUDE("src/gpxc/CMakeLists.txt")

# Build gpmake
INCLUDE("src/gpmake/CMakeLists.txt")

# Bring in CPack
INCLUDE(CPack)
