###########################################################################################
####  Root path to search on the filesystem for the cross compilers and toolchain utilities

set(MINGW_BIN_DIRECTORY C:/mingw64/bin)
set(CMAKE_FIND_ROOT_PATH ${MINGW_BIN_DIRECTORY} )


###########################################################################################
####  Pathname of C cross compiler
set(CMAKE_C_COMPILER ${MINGW_BIN_DIRECTORY}/gcc.exe )

###########################################################################################
####  Pathname of C++ cross compiler
set(CMAKE_CXX_COMPILER ${MINGW_BIN_DIRECTORY}/c++.exe )

