# Cross compile libiconv for Android on Windows #

> A cross compile for Android on Windows requires [MinGW](http://www.mingw.org/) with the MSYS packages.

- Download the libiconv-1.14 library [from here](http://www.gnu.org/software/libiconv/) (e.g. `libiconv-1.14.tar.gz` )

- Extract the libiconv package in the directory of this ReadMe file.

- copy the `src` and `include` directory and the `android_arm32.cmake`, `CMakeLists.txt` and the `config.h` files into the root directory of the extracted package and confirm to overwrite the `src` and `include` directories.

> The files `config.h` and `include\iconv.h` are the result of a `./configure` run of `libiconv`. This a shortcut to skip this step here.

- The resulting directory structure contains the library source and the additional files:

	- `..../libtml-c/3rdParty/libiconv/libiconv-1.14`
		- `src`
			- `CMakeLists.txt`
		- `include`
			- `iconv.h`
		- `CMakeLists.txt`
		- `android_arm32.cmake`
		- `config.h`


- Open the file `libcharset\lib\localcharset.c` in the extracted root directory with an editor and search for the string  **`HAVE_LANGINFO_CODESET`**
- paste **`#undef HAVE_LANGINFO_CODESET`** into the line before and save the file.

Before building libiconv, the *copied* tool chain file `android_arm32.cmake` has to be modified in order to tell CMake where to find the Android cross compiler. 

The [Android NDK](https://developer.android.com/ndk/index.html) is required for cross compilation. Depending on the intended target platform you need to download and install the matching version. The Android NDK can be downloaded from [https://developer.android.com/ndk/downloads](https://developer.android.com/ndk/downloads/index.html).


- Set the cross compilers and tool chain utilities in the copied `android_arm32.cmake`.
	-	 Root path to search on the file system for the cross compilers and tool chain utilities e.g.
		`set(CMAKE_FIND_ROOT_PATH t:/android/standalone-20/arm-linux-androideabi-4.6/arm-linux-androideabi/bin )`
	-	 Path of C cross compiler e.g. 
		`set(CMAKE_C_COMPILER t:/android/standalone-20/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gcc.exe )`
	-	 Path of C++ cross compiler e.g. 
		`set(CMAKE_CXX_COMPILER t:/android/standalone-20/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-g++.exe )`


- Open MSYS development console (`<MinGW Installdir>\msys\1.0\msys.bat`) and go to the extracted libiconv directory (e.g. `libtml-c/3rdParty/libiconv/libiconv-1.14`).

- Create a build directory.
`mkdir build`

- go to the build directory
- `cd build`

- Generate the make file
`cmake .. -DCMAKE_TOOLCHAIN_FILE=android_arm32.cmake -G "Unix Makefiles"`

- Build with cmake
`cmake --build . --clean-first`

- the binary is located in:
	- `.....build\android_arm32\libiconv\libiconv.so`



