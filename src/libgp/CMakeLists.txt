#
# Build file for GP Developer
#
# Copyright 2013 Alex Elliott <alex@alex-elliott.co.uk>
#
# ------------------------------------------------------------------------------
#
# This file builds libGP - a shared library containing data structures and
# utility functions required for the GP2 toolchain or for integration with
# arbitrary C programs.
#

# Unix platform add a "lib" prefix, MSVC doesn't
SET(libGP_NAME GP)
IF(CMAKE_COMPILER_IS_MSVC)
    SET(libGP_NAME libGP)
ENDIF(CMAKE_COMPILER_IS_MSVC)

SET(libGP_C_SRCS
    src/libgp/global.c
    src/libgp/errors.c
)

ADD_LIBRARY(${libGP_NAME} ${libGP_C_SRCS})

# Per-platform CPack Configuration
IF(UNIX AND NOT APPLE)
    INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/lib${libGP_NAME}.a DESTINATION bin)
ELSEIF(APPLE)
    INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/lib${libGP_NAME}.a DESTINATION bin)
ELSEIF(WIN32)
    SET(CPACK_NSIS_MUI_ICON ${GP2_SOURCE_DIR}/src/developer/images/icon.ico)

    # Get the Qt DLLs for packaging with this executable
    #INSTALL(PROGRAMS ${QT_BINARY_DIR}/QtCore4.dll
    #                 ${QT_BINARY_DIR}/QtGui4.dll
    #                 ${QT_BINARY_DIR}/mingwm10.dll
    #                 ${QT_BINARY_DIR}/libgcc_s_dw2-1.dll
    #        DESTINATION bin)

    INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/${libGP_NAME}.lib DESTINATION bin)
ENDIF()
