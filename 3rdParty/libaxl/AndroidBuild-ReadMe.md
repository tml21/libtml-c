# Cross compile AXL library for Android on Windows #

> A cross compile for Android on Windows requires [MinGW](http://www.mingw.org/) with the MSYS packages.

- Download the latest AXL source package [from here](http://www.aspl.es/axl)<br>( e.g. `axl-0.7.0.b5340.g5351.tar.gz` )

- Extract the AXL package in the directory of this ReadMe file.

- copy the `src` directory, `android_arm32.cmake` and the `CMakeLists.txt` file into the AXL root directory and confirm to overwrite the `src` directory.

- The resulting directory structure contains the library source and the additional files:

	- `..../libtml-c/3rdParty/libaxl/axl-0.7.0.b5340.g5351`<br>
		- `src`
			- `CMakeLists.txt`
		- `CMakeLists.txt`
		- `android_arm32.cmake`


Before building libaxl, the *copied* tool chain file `android_arm32.cmake` has to be modified in order to tell CMake where to find the Android cross compiler. 

The [Android NDK](https://developer.android.com/ndk/index.html) is required for cross compilation. Depending on the intended target platform you need to download and install the matching version. The Android NDK can be downloaded from [https://developer.android.com/ndk/downloads](https://developer.android.com/ndk/downloads/index.html).


- Set the cross compilers and tool chain utilities in the copied `android_arm32.cmake`.
	-	 Root path to search on the file system for the cross compilers and tool chain utilities e.g.
		`set(CMAKE_FIND_ROOT_PATH t:/android/standalone-20/arm-linux-androideabi-4.6/arm-linux-androideabi/bin )`
	-	 Path of C cross compiler e.g. 
		`set(CMAKE_C_COMPILER t:/android/standalone-20/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gcc.exe )`
	-	 Path of C++ cross compiler e.g. 
		`set(CMAKE_CXX_COMPILER t:/android/standalone-20/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-g++.exe )`


- Open MSYS development console (`<MinGW Installdir>\msys\1.0\msys.bat`) and go to the extracted AXL directory (e.g. `libtml-c/3rdParty/libaxl/axl-0.7.0.b5340.g5351`).

- Create a build directory.
`mkdir build`

- go to the build directory
- `cd build`

- Generate the make file
`cmake .. -DCMAKE_TOOLCHAIN_FILE=android_arm32.cmake -G "Unix Makefiles"`

- Build with cmake
`cmake --build . --clean-first`

- the binary is located in:
`...build\android_arm32\libaxl\libaxl.so`



