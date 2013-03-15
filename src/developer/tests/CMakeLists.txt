#
# Unit testing file for GP Developer
#
# Copyright 2013 Alex Elliott <alex@alex-elliott.co.uk>
#
# ------------------------------------------------------------------------------
#
# This file enumerates the unit tests available for GP Developer's classes
#
# Tests on the GP Developer codebase are often testing Qt-based objects and
# using Qt datatypes. Therefore as with the main GP Developer executable the
# tests must be linked against Qt as well as libGP
#
# Unlike other C++ files in the codebase, the test for GP Developer make use of
# the .cxx extension, the reason for this is that GP Developer's build file
# makes use of a GLOB_RECURSE on *.cpp when locating source files to compile. If
# these files were to use .cpp as well, then they would be located by that GLOB
# expression and the compile will fail as a result of multiple definitions of
# int main().
#

# Build the test
ADD_EXECUTABLE(testProject src/developer/tests/testproject.cxx)
TARGET_LINK_LIBRARIES(testProject ${GPDeveloper_LINK_LIBS} libGP)

# Add the tests to the list
ADD_TEST(test_project testProject)
