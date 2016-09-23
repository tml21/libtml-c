# Install libTML-c from source {#tml_install}

The libTML-c library is intended to be used on multiple platforms. CMake is used to simplify building, installing or creating project files for a preferred development environment and platform. All instructions in this section require to install [CMake](http://www.cmake.org/).
You can download CMake here: [http://www.cmake.org/download/](http://www.cmake.org/download/)

All instructions require the sources of the [libTML-c library from GitHub](https://github.com/tml21/libtml-c). Download the ZIP archive and extract the files on your target or build system.

Currently the following target systems are tested:

- [Android Cross Build](#AndroidLink)
- [freeBSD x86](#freeBSDLink)
- [Linux ARM](#linuxArmLink)
- [Linux x86](#LinuxLink)
- [OS X](#OsxLink)
- [Windows](#WinLink)
	- [Build with MinGW](#MinGWLink)
		- [Build with MinGW32](#MinGWLink32)
		- [Build with MinGW64](#MinGWLink64)
	- [Build with Visual Studio](#WinVCLink)

<br>

----------

<a name="TLSOption"></a>
## Optional libTML TLS build ##

It is optional to build the libTML TLS profile supporting library.

To enable the libTML TLS build option you have to add the parameter **BUILD_TLS=yes** to the make file generation command line.<br><br>
Example:<br> <br>
`cmake .. -G "Unix Makefiles" -DTML_TARGET_PLATFORM=linux_x86-32 -DBUILD_TLS=yes`

<br>

----------

<a name="RequiredLibs"></a>
## Library dependencies ##

To build libTML various libraries need to be present. Refer to the documentation of the particular library for installation.

#### AXL ####

The AXL library is an XML 1.0 implementation. It is optimized for speed and a small memory footprint.

- **Homepage:** [`http://www.aspl.es/axl`](http://www.aspl.es/axl)
- **Download:** [`http://www.aspl.es/axl/doc.html`](http://www.aspl.es/axl/doc.html)
- **Documentation:** [`http://www.aspl.es/axl/doc.html`](http://www.aspl.es/axl/doc.html)
- **Required for:** Vortex, libTML
- **Remarks:** On Unix-like systems the AXL library can be compiled from source.

    	./configure --disable-py-axl
    	make
    	make install

If you want to use a Windows installer, download the Vortex installer instead of the AXL installer. It includes the AXL library. During the installation you need to select to install the development libraries and include files along with the binaries.

**Note:** Our binary distribution package includes the AXL library. 

#### Vortex ####

Vortex is a BEEP core implementation.

- **Homepage:** [`http://www.aspl.es/vortex`](http://www.aspl.es/vortex)
- **Download:** [`http://www.aspl.es/vortex/download.html`](http://www.aspl.es/vortex/download.html)
- **Documentation:** [`http://www.aspl.es/vortex/doc.html`](http://www.aspl.es/vortex/doc.html)
- **Required for:** libTML
- **Remarks:** On Unix-like systems the Vortex library can be compiled from source

To build the TLS module, the SSL development library (libssl-dev) is required. On Debian wheezy, for example, you can install the libraries with using apt-get. 

	apt-get install libssl-dev	

The normal sequence of commands to build Vortex from source:

    	./configure
    	make
    	make install

Building Vortex on OS X may show errors. Use the options to work around them.

    	./configure --disable-vortex-log --disable-vortex-client --disable-py-vortex --disable-sasl-support CFLAGS="-Qunused-arguments -Wno-deprecated"
    	make
    	make install

The Vortex Windows installer includes all binaries for Windows. During the installation you need to select to install the development libraries and include files along with the binaries.


**Note:** Our binary distribution package includes the Vortex library. 

#### libiconv ####

For Unicode and string encoding support, libTML is using the libiconv library.

- **Homepage:** [`http://www.gnu.org/software/libiconv`](http://www.gnu.org/software/libiconv)
- **Download:** [`http://www.gnu.org/software/libiconv/#downloading`](http://www.gnu.org/software/libiconv/#downloading)
- **Documentation:** [`http://www.gnu.org/software/libiconv/#introduction`](http://www.gnu.org/software/libiconv/#introduction)
- **Required for:** libTML
- **Remarks:** If not available on the target platform, the library can be compiled from source

    	./configure
    	make
    	make install

On Windows the libiconv library is not required.

#### openSSL ####

In order to use the [optional libTML TLS profile support](#TLSOption) it is required to install openssl library and binaries.
The libTML library can be used without the optional TLS support and installing openSSL can be skipped in that case. TLS profile will not be available in this case tough.

- **Homepage:** [`http://www.openssl.org`](http://www.openssl.org)
- **Download:**  [`http://www.openssl.org/source/`](http://www.openssl.org/source/)
- **Documentation:** [`http://www.openssl.org/docs/`](http://www.openssl.org/docs/)
- **Required for:** libTML-TLS
- **Remarks:** This software is commonly found on every Unix/GNU/Linux installation. Binaries are also available for Microsoft Windows platforms.

<br>

----------

## Compiler ##

The libTML-c library API makes use of the char16\_t data type. It is mandatory, to use a compiler, that supports char16\_t data type (gcc >= 4.4).

### Set library dependencies manually ###

By default cmake finds the library and include paths automatically. If the libraries are not installed in the default location, the **CMakeLists.txt** in the root directory of libTML-c has to be modified. These modifications are usually necessary on Windows.
Find the intended build target in the **CMakeLists.txt** file and modify the directories.

Set include directories:

	set(AXLINCLUDE "C:\\Program Files (x86)\\AxlW32\\include\\axl")
	set(VORTEXINCLUDE "C:\\Program Files (x86)\\VortexLibrary-1.1-W32\\include\\vortex")
	set(ICONVINCLUDE usr/local/include/iconv)

Set library directories:

	LINK_DIRECTORIES("C:\\Program Files (x86)\\AxlW32\\lib")
	LINK_DIRECTORIES("C:\\Program Files (x86)\\VortexLibrary-1.1-W32\\lib")

For multiple library directories add this line with a different path.

<br>

----------

<a name="WinLink"></a>
## Build libTML-c on Windows ##

To build libTML-c on Windows you can use either Visual Studio or MinGW.

- [Build with MinGW](#MinGWLink)
	- [Build MinGW32](#MinGWLink32)
	- [Build MinGW64](#MinGWLink64)
	- [Build with Visual Studio](#WinVCLink)


<a name="MinGWLink"></a>
### Build with MinGW ###

> The compiled libraries require the MinGW run time libraries.

Download the MinGW Installer from [http://www.mingw.org/](http://www.mingw.org/). After installation start the `guimain.exe` and mark the required packages:

#### win32 #####

- MinGW / MinGW Base System "`mingw32-base`"
- MinGW / MinGW Base System "`mingw32-gcc-g++`"
- MSYS / MSYS Base System "`msys-base`"

Apply changes to install the packages.
Open the MSys console (`<MinGW Installdir>\msys\1.0\msys.bat`) and go to the root directory of the libTML-c files.

#### win64 #####

To compile 64 bit libraries with MinGW a 64 bit compiler is required. After installing MinGW for 32 bit you need to download the 64 bit MinGW package from [**here**](https://code.google.com/p/mingw-builds/downloads/detail?name=x86_64-mingw32-gcc-4.7.0-release-c,c%2b%2b,fortran-sjlj.zip&can=2&q=)
Extract the package but do not overwrite the previous 32 bit MinGW installation.

 Use the `win_mingw64.cmake` tool chain to set the compiler path for win64 binary build.
 (see [Build libTML-c win64 with MinGW](#MinGWLink64))


<br>

----------

<a name="MinGWLink32"></a>
## Build libTML-c win32 with MinGW ##

To build libTML-c win32 binaries on Windows with MinGW, the settings for the tool chain have to be adjusted. CMake needs the information where to find win32 compiler. Edit the tool chain file `win_mingw32.cmake`. The file is located in the sources root.

* Set the cross compilers and tool chain utilities in `win_mingw32.cmake`.
	- Root path to search on the file system for the win32 compilers and tool chain utilities e.g.<br>`set(MINGW_BIN_DIRECTORY C:/mingw/bin)`

- Open MSYS development console (`<MinGW Installdir>\msys\1.0\msys.bat`) and go to the libTML directory.

- Create a build directory.<br/>`mkdir build`

- go to the build directory<br/>`cd build`

- Generate the make file<br>`cmake .. -DCMAKE_TOOLCHAIN_FILE=win_mingw32.cmake -G "Unix Makefiles" -DTML_TARGET_PLATFORM=win_x86-32_mingw`

> The CMake cache file `CMakeCache.txt` is storing parameters from a previous build. If you create the binaries for different platform you need to delete `CMakeCache.txt` and the `CMakeFiles` directory in order to create a clean new set of project files.

- Build with cmake<br>`cmake --build . --clean-first`

	- The binaries are located in:
		- `build\win_x86-32_mingw\sidex\sidex12.dll`
		- `build\win_x86-32_mingw\tml\tmlcore12.dll`
		- `build\win_x86-32_mingw\tml-tls\tml-tls12.dll` [*)TLS build option enabled](#TLSOption)
		- `build\win_x86-32_mingw\test\libTmlTest.exe`


<br>

----------


<a name="MinGWLink64"></a>
## Build libTML-c win64 with MinGW ##

To build libTML-c win64 binaries on Windows with MinGW, the settings for the tool chain have to be adjusted. CMake needs the information where to find win64 compiler. Edit the tool chain file `win_mingw64.cmake`. The file is located in the sources root.

* Set the cross compilers and tool chain utilities in `win_mingw64.cmake`.
	- Root path to search on the file system for the win64 compilers and tool chain utilities e.g.<br>`set(MINGW_BIN_DIRECTORY C:/mingw64/bin)`

- Open MSYS development console (`<MinGW Installdir>\msys\1.0\msys.bat`) and go to the libTML directory.

- Create a build directory.<br/>`mkdir build`

- go to the build directory<br/>`cd build`

- Generate the make file<br>`cmake .. -DCMAKE_TOOLCHAIN_FILE=win_mingw64.cmake -G "Unix Makefiles" -DTML_TARGET_PLATFORM=win_x86-64_mingw`

> The CMake cache file `CMakeCache.txt` is storing parameters from a previous build. If you create the binaries for different platform you need to delete `CMakeCache.txt` and the `CMakeFiles` directory in order to create a clean new set of project files.

- Build with cmake<br>`cmake --build . --clean-first`

	- the binaries are located in:
		- `build\win_x86-64_mingw\sidex\sidex12_64.dll`
		- `build\win_x86-64_mingw\tml\tmlcore12_64.dll`
		- `build\win_x86-64_mingw\tml-tls\tml-tls12_64.dll` [*)TLS build option enabled](#TLSOption)
		- `build\win_x86-64_mingw\test\libTmlTest64.exe`

<br>

----------

<a name="WinVCLink"></a>
## Build libTML-c on Windows with Visual Studio ##

- Open a windows command prompt and go to the libTML directory.

- Create a build directory.<br/>`mkdir build`

- go to the build directory<br/>`cd build`

- Generate the Visual Studio Project files. If you intend to use a different version of Visual Studio, call `cmake --help` and find the matching generator.
	-	for win32 binary build:<br>`cmake .. -G "Visual Studio 10 2010" -DTML_TARGET_PLATFORM=win_x86-32`
	-	for x64 binary build:<br>`cmake .. -G "Visual Studio 10 2010 Win64" -DTML_TARGET_PLATFORM=win_x86-64`

> The CMake cache file `CMakeCache.txt` is storing parameters from a previous build. If you create the binaries for different platform you need to delete `CMakeCache.txt` and the `CMakeFiles` directory in order to create a clean new set of project files.

- Build with cmake<br>`cmake --build . --clean-first --config Release`

- Depending on the `TML_TARGET_PLATFORM` parameter the binaries are located in:

	* win_x86-32:
		- `build\win_x86-32\sidex\Release\sidex12.dll`
		- `build\win_x86-32\sidex\Release\sidex12.lib`
		- `build\win_x86-32\tml\Release\tmlcore12.dll`
		- `build\win_x86-32\tml\Release\tmlcore12.lib`
		- `build\win_x86-32\tml-tls\Release\tml-tls12.dll` [*)TLS build option enabled](#TLSOption)
		- `build\win_x86-32\tml-tls\Release\tml-tls12.lib` [*)TLS build option enabled](#TLSOption)
		- `build\win_x86-32\test\Release\libTmlTest.exe`
	* win_x86-64:
	    - `build\win_x86-64\sidex\Release\sidex12_64.dll`
	    - `build\win_x86-64\sidex\Release\sidex12_64.lib`
		- `build\win_x86-64\tml\Release\tmlcore12_64.dll`
		- `build\win_x86-64\tml\Release\tmlcore12_64.lib`
		- `build\win_x86-64\tml-tls\Release\tml-tls12_64.dll` [*)TLS build option enabled](#TLSOption)
		- `build\win_x86-64\tml-tls\Release\tml-tls12_64.lib` [*)TLS build option enabled](#TLSOption)
		- `build\win_x86-64\test\Release\libTmlTest64.exe`

<br>

----------

<a name="LinuxLink"></a>
## Build libTML-c on LINUX x86 ##

- Open a terminal and go to the libTML directory.

- Create a build directory.<br/>`mkdir build`

- go to the build directory<br/>`cd build`

- Generate the make file
	-	for 32 bit binary build:<br>`cmake .. -G "Unix Makefiles" -DTML_TARGET_PLATFORM=linux_x86-32`
	-	for 64 bit binary build:<br>`cmake .. -G "Unix Makefiles" -DTML_TARGET_PLATFORM=linux_x86-64`

> The CMake cache file `CMakeCache.txt` is storing parameters from a previous build. If you create the binaries for different platform you need to delete `CMakeCache.txt` and the `CMakeFiles` directory in order to create a clean new set of project files.

- Build with cmake<br>`cmake --build . --clean-first`

- Depending on the `TML_TARGET_PLATFORM` parameter the binaries are located in:
	* linux_x86-32
		- `build\linux_x86-32\sidex\libsidex12.so`
		- `build\linux_x86-32\tml\libtmlcore12.so`
		- `build\linux_x86-32\tml-tls\libtml-tls12.so` [*)TLS build option enabled](#TLSOption)
		- `build\linux_x86-32\test\libTmlTest`
	* linux_x86-64
		- `build\linux_x86-64\sidex\libsidex12_64.so`
		- `build\linux_x86-64\tml\libtmlcore12_64.so`
		- `build\linux_x86-64\tml-tls\libtml-tls12_64.so` [*)TLS build option enabled](#TLSOption)
		- `build\linux_x86-64\test\libTmlTest64`


- Install the libraries (superuser required)<br/>`make install`<br/>or<br/>`sudo make install`

<br>

----------

<a name="OsxLink"></a>
## Build libTML-c on OS X ##

- Open a terminal and go to the libTML directory.

- Create a build directory.<br/>`mkdir build`

- go to the build directory<br/>`cd build`

- Generate the make file<br>
	-	for 32 bit binary build:<br>`cmake .. -G "Unix Makefiles" -DTML_TARGET_PLATFORM=osx_x86-32`
	-	for 64 bit binary build:<br>`cmake .. -G "Unix Makefiles" -DTML_TARGET_PLATFORM=osx_x86-64`

> The CMake cache file `CMakeCache.txt` is storing parameters from a previous build. If you create the binaries for different platform you need to delete `CMakeCache.txt` and the `CMakeFiles` directory in order to create a clean new set of project files.

- Build with cmake<br>`cmake --build . --clean-first`

- Depending on the `TML_TARGET_PLATFORM` parameter the binaries are located in:
	- osx_x86-32
		- `build\osx_x86-32\sidex\libsidex12.dylib`
		- `build\osx_x86-32\tml\libtmlcore12.dylib`
		- `build\osx_x86-32\tml-tls\libtml-tls12.dylib` [*)TLS build option enabled](#TLSOption)
		- `build\osx_x86-32\test\libTmlTest`
	- osx_x86-64
		- `build\osx_x86-64\sidex\libsidex12_64.dylib`
		- `build\osx_x86-64\tml\libtmlcore12_64.dylib`
		- `build\osx_x86-64\tml-tls\libtml-tls12_64.dylib` [*)TLS build option enabled](#TLSOption)
		- `build\osx_x86-64\test\libTmlTest64`

- Install the libraries (superuser required)<br/>`make install`<br/>or<br/>`sudo make install`

<br>

----------

<a name="linuxArmLink"></a>
## Build libTML-c on LINUX ARM ##

- Open a terminal and go to the libTML directory.<br>

- Create a build directory.<br/>`mkdir build`

- go to the build directory<br/>`cd build`

- Generate the makefile<br>`cmake .. -G "Unix Makefiles" -DTML_TARGET_PLATFORM=linux_arm32`

- Build with cmake<br>`cmake --build . --clean-first`

- the binaries are located in:
	- `build\linux_arm32\sidex\libsidex12.so`
	- `build\linux_arm32\tml\libtmlcore12.so`
	- `build\linux_arm32\tml-tls\libtml-tls12.so` [*)TLS build option enabled](#TLSOption)
	- `build\linux_arm32\test\libTmlTest`


- Install the libraries (superuser required)<br/>`make install`<br/>or<br/>`sudo make install`

<br>

----------

<a name="AndroidLink"></a>
## libTML-c cross build for Android ##

To build libTML-c for Android with CMAKE, the settings for the tool chain have to be adjusted. CMake needs the information where to find the Android cross compiler. Edit the tool chain file `android_arm32.cmake`. The file is located in the sources root.

The [Android NDK](https://developer.android.com/ndk/index.html) is required for cross compilation. Depending on the intended target platform you need to download and install the matching version. The Android NDK can be downloaded from [https://developer.android.com/ndk/downloads](https://developer.android.com/ndk/downloads/index.html).

>The [required libraries](#RequiredLibs) have to be build as well. Some of the libraries do not include instructions on how to build on Android. The `3rdParty` directory of the libTML source distribution contains additional files and instructions in` AndroidBuild-ReadMe.md` to fill this gap. Depending on the version of the library additional modifications may be required.

A cross compile for Android on Windows requires [MinGW](http://www.mingw.org/) with the MSYS packages.

- Set the cross compilers and tool chain utilities in `android_arm32.cmake`.
	-	 Root path to search on the file system for the cross compilers and tool chain utilities e.g.
		`set(CMAKE_FIND_ROOT_PATH t:/android/standalone-20/arm-linux-androideabi-4.6/arm-linux-androideabi/bin )`
	-	 Path of C cross compiler e.g.
		`set(CMAKE_C_COMPILER t:/android/standalone-20/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gcc.exe )`
	-	 Path of C++ cross compiler e.g.
		`set(CMAKE_CXX_COMPILER t:/android/standalone-20/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-g++.exe )`
<br><br>
- Open MSYS development console (`<MinGW Installdir>\msys\1.0\msys.bat`) and go to the libTML directory.

- Create a build directory.<br/>`mkdir build`

- go to the build directory<br/>`cd build`

- Generate the makefile<br>`cmake .. -DCMAKE_TOOLCHAIN_FILE=android_arm32.cmake -G "Unix Makefiles" -DTML_TARGET_PLATFORM=android_arm32`

- Build with cmake<br>`cmake --build . --clean-first`

- the binaries are located in:
	- `build\android_arm32\sidex\libsidex12.so`
	- `build\android_arm32\tml\libtmlcore12.so`
	- `build\android_arm32\tml-tls\libtml-tls12.so` [\*)TLS build  option enabled](#TLSOption)

<br>

----------

<a name="freeBSDLink"></a>
## Build libTML-c on freeBSD x86 ##

- Open a terminal and go to the libTML directory.

- Create a build directory.<br/>`mkdir build`

- go to the build directory<br/>`cd build`

- Generate the make file
	-	for 32 bit binary build:<br>`cmake .. -G "Unix Makefiles" -DTML_TARGET_PLATFORM=freeBSD_x86-32`
	-	for 64 bit binary build:<br>`cmake .. -G "Unix Makefiles" -DTML_TARGET_PLATFORM=freeBSD_x86-64`

> The CMake cache file `CMakeCache.txt` is storing parameters from a previous build. If you create the binaries for different platform you need to delete `CMakeCache.txt` and the `CMakeFiles` directory in order to create a clean new set of project files.

- Build with cmake<br>`cmake --build . --clean-first`

- Depending on the `TML_TARGET_PLATFORM` parameter the binaries are located in:
	* freeBSD_x86-32
		- `build\freeBSD_x86-32\sidex\libsidex12.so`
		- `build\freeBSD_x86-32\tml\libtmlcore12.so`
		- `build\freeBSD_x86-32\tml-tls\libtml-tls12.so` [*)TLS build option enabled](#TLSOption)
		- `build\freeBSD_x86-32\test\libTmlTest`
	* freeBSD_x86-64
		- `build\freeBSD_x86-64\sidex\libsidex12_64.so`
		- `build\freeBSD_x86-64\tml\libtmlcore12_64.so`
		- `build\freeBSD_x86-64\tml-tls\libtml-tls12_64.so` [*)TLS build option enabled](#TLSOption)
		- `build\freeBSD_x86-64\test\libTmlTest64`


- Install the libraries (superuser required)<br/>`make install`<br/>or<br/>`sudo make install`
