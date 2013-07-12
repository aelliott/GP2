GP Developer
============

Introduction
------------

This is the working repository for GP Developer, the integrated development
environment being constructed for [GP2](http://www.cs.york.ac.uk/plasma/wiki/index.php?title=GP_%28Graph_Programs%29)
- an experimental graph programming language developed by the University of
York.

Compiling and Installing
------------------------

The project uses CMake to produce build files across all supported platforms.
The CMake build system currently requires:

 - CMake itself
 - Qt version 4.x (due to a structural change, Qt 5.x requires work to be
   supported, but should be compatible)
 - The [OGDF](http://www.ogdf.net/doku.php) library
 - The [Boost](http://www.boost.org/) C++ libraries

When building with either Linux or OS X (with the necessary libraries installed)
the project can be retrieved and built with this process:

    $ git clone https://github.com/aelliott/GP2.git
    Cloning into 'GP2'...
    ...
    $ mkdir GP2-build && cd GP2-build
    $ cmake ../GP2
    ...
    -- Build files have been written to: /home/alex/tmp/GP2-build
    $ make

The project should then be compiled and the executable created, this may take
some time - in particular the graphparser.cpp.o and ruleparser.cpp.o objects.

When building under Windows it is recommended that you use the graphical user
interfaces for CMake to produce a Visual Studio project. The Windows build
includes an early NSIS installer under "package".
