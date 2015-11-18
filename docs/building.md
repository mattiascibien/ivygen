# Building IvyGen

## Required tools

 - [CMake](http://cmake.org) > 3.0 (latest version recommended)
 - A compiler with support for C++ 11 features 
 (the source code has references to [nullptr](http://en.cppreference.com/w/cpp/language/nullptr) and [range basesd for loops](http://en.cppreference.com/w/cpp/language/range-for))
 - [Qt](http://www.qt.io) 5.x (we use the latest version of Qt 5.5 for development)
 
### Windows

The main tested compilers are GCC under [MinGW-w64](http://mingw-w64.org/) and [Visual Studio 2013](https://www.visualstudio.com/). 

#### Compiling under MSYS2

The preferred way to develop using MinGW-w64 is [MSYS2](http://msys2.github.io) as it provides the latest version of GCC and a package manager.

For a complete building environment under MSYS2 we suggest to install the following packages:

 - `mingw-w64-{arch}-toolchain` for gcc and g++ 
 - `mingw-w64-{arch}-cmake` for cmake
 - `mingw-w64-{arch}-qt5` for Qt
 
Once these requirements are satisfied you can safely build IvyGen.

#### Compiling under Visual Studio 2013

In order to get an environment for Visual Studio 2013 you can install CMake from the official website and Qt using [Qt Installer](http://download.qt.io/official_releases/online_installers/qt-unified-windows-x86-online.exe). 
Be sure to select the Qt version compiled for Visual Studio 2013.

### MacOS Compilers

Compiling under MacOS is actually working under Travis but has not been tested

### Linux Compilers

Compiling under Linux has some issues at the moment.

## Compiling

The steps required for compiling are the same as any other CMake project:

	cd [cloned-source-dir]
	cmake . # See the CMake Help file for other options
	[make program] # [make program] is mingw32-make, make or nmake depending on your environment
	[make program] install # Installs the program
	
If you use Visual Studio you can even open the IDE to develop.