#
# Unit testing file for GP Developer
#
# Copyright 2013 Alex Elliott <alex@alex-elliott.co.uk>
#
# ------------------------------------------------------------------------------
#
# This file enumerates the unit tests available for GP Developer's classes
#

ADD_EXECUTABLE(testProject src/developer/tests/testproject.cxx)
ADD_TEST(test_project testProject)
