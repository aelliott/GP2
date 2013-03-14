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

SET(libGP_C_SRCS
    src/libgp/global.c
    src/libgp/errors.c
)

ADD_LIBRARY(libGP ${libGP_C_SRCS})
