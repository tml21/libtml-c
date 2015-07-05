set( CMAKE_VERBOSE_MAKEFILE on )
#######################################################
## minimum required cmake version 
cmake_minimum_required(VERSION 3.1)

#######################################################
## set build parameter
#######################################################
SET (TML_TARGET_PLATFORM android_arm32 CACHE STRING "set the build target")

#######################################################
## ANDROID ARM32
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  set(PROJECT android_arm32)

  set(AXLINCLUDE "/x/workdir/libtml-c/3rdParty/libaxl/axl-0.7.0.b5340.g5351/src")

  LINK_DIRECTORIES("/x/workdir/libtml-c/3rdParty/libaxl/axl-0.7.0.b5340.g5351/build/android_arm32/libaxl")
  
  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .so)

  set (PLATFORM_FOUND true)
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)

if (${PLATFORM_FOUND} STREQUAL true)
  #######################################################
  ## Project / Solution- File:
  PROJECT( libvortex )
  
  #######################################################
  ## Source and destination directories:
  ADD_SUBDIRECTORY( src ${PROJECT}/libvortex) 
else (${PLATFORM_FOUND} STREQUAL true)
  message( SEND_ERROR "Unknown target platform / please check \"TML_TARGET_PLATFORM\"" )
endif (${PLATFORM_FOUND} STREQUAL true)
