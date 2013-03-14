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
    ${GP2_SOURCE_DIR}/src
    ${GP2_BINARY_DIR}
)

INCLUDE(CheckIncludeFiles)

SET(QT_MIN_VERSION "4.8.0")
FIND_PACKAGE(Qt4 REQUIRED)

SET(QT_USE_QTMAIN true)
SET(QT_USE_QTXML true)

INCLUDE(${QT_USE_FILE})

# Build GP Developer
INCLUDE("src/developer/CMakeLists.txt")

# Build libGP
INCLUDE("src/libgp/CMakeLists.txt")

# Build gprc
INCLUDE("src/gprc/CMakeLists.txt")

# Build gpxc
INCLUDE("src/gpxc/CMakeLists.txt")

# Build gpmake
INCLUDE("src/gpmake/CMakeLists.txt")
