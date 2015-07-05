# Cross compile Vortex for Android on Windows.

> A cross compile for Android on Windows requires [MinGW](http://www.mingw.org/) with the MSYS packages.

Building Vortex requires the AXL library. Check if you already created the AXL library before proceeding with building Vortex.

- Download the latest Vortex library [from here](http://www.aspl.es/vortex) ( e.g. vortex-1.1.14.b5256.g5258.tar.gz).

- Extract the Vortex package in the directory of this ReadMe file.

- copy the `src` directory, and the files `android_arm32.cmake` and `CMakeLists.txt` into the extracted root directory and confirm to overwrite the `src` directory.


- The resulting directory structure contains the library source and the additional files:

	- `..../libtml-c/3rdParty/libvortex/vortex-1.1.14.b5256.g5258`
		- `src`
			- `CMakeLists.txt`
		- `CMakeLists.txt`
		- `android_arm32.cmake`


- Open the file `src/vortex_errno.c` in the extracted root directory with an editor and search for the string  **`return strerror (errno)`**
- change it to   **`return strerror (__errno)`** and save the file

Before building Vortex, the *copied* tool chain file `android_arm32.cmake` has to be modified in order to tell CMake where to find the Android cross compiler. 

The [Android NDK](https://developer.android.com/ndk/index.html) is required for cross compilation. Depending on the intended target platform you need to download and install the matching version. The Android NDK can be downloaded from [https://developer.android.com/ndk/downloads](https://developer.android.com/ndk/downloads/index.html).

- Set the cross compilers and tool chain utilities in the copied `android_arm32.cmake`.
	-	 Root path to search on the file system for the cross compilers and tool chain utilities e.g.
		`set(CMAKE_FIND_ROOT_PATH t:/android/standalone-20/arm-linux-androideabi-4.6/arm-linux-androideabi/bin )`
	-	 Path of C cross compiler e.g. 
		`set(CMAKE_C_COMPILER t:/android/standalone-20/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gcc.exe )`
	-	 Path of C++ cross compiler e.g. 
		`set(CMAKE_CXX_COMPILER t:/android/standalone-20/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-g++.exe )`

Edit the **CMakeLists.txt** located in the extracted Vortex root directory and change the libraries include and link path to the AXL location on your build system.

Set AXL library include:

	set(AXLINCLUDE "/x/workdir/libtml-c/3rdParty/libaxl/axl-0.7.0.b5340.g5351/src")
 
Set AXL library link:

	LINK_DIRECTORIES("/x/workdir/libtml-c/3rdParty/libaxl/axl-0.7.0.b5340.g5351/build/android_arm32/libaxl")

Open MSYS development console (`<MinGW Installdir>\msys\1.0\msys.bat`) and go to the extracted Vortex directory (e.g. `libtml-c/3rdParty/libvortex/vortex-1.1.14.b5256.g5258`).

- Create a build directory.
`mkdir build`

- go to the build directory
- `cd build`

- Generate the make file
`cmake .. -DCMAKE_TOOLCHAIN_FILE=android_arm32.cmake -G "Unix Makefiles"`

- Build with cmake
`cmake --build . --clean-first`

- the binary is located in:
	`...build\android_arm32\libvortex\libvortex-1.1.so`



