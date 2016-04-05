#  libTML:  A BEEP based Messaging Suite
#  Copyright (C) 2015 wobe-systems GmbH
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this program; if not, write to the Free
#  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
#  02111-1307 USA
#  
#  You may find a copy of the license under this software is released
#  at COPYING file. This is LGPL software: you are welcome to develop
#  proprietary applications using this library without any royalty or
#  fee but returning back any change, improvement or addition in the
#  form of source code, project image, documentation patches, etc.
#
#  Homepage:
#    http://www.libtml.org
#
#  For professional support contact us:
#
#    wobe-systems GmbH
#    support@libtml.org
#  
# Contributors:
#    wobe-systems GmbH

set(CMAKE_MACOSX_RPATH 1)
set( CMAKE_VERBOSE_MAKEFILE on )
#######################################################
## minimum required cmake version 
cmake_minimum_required(VERSION 3.1)

#######################################################
## set build parameter
#######################################################
SET (TML_TARGET_PLATFORM win_x86-64 CACHE STRING "set the build target")
#######################################################

#######################################################
## Default iconv- Include
set(ICONVINCLUDE "")

set (TML_MAJOR_VERSION 1)
set (TML_MINOR_VERSION 2)
set (TML_MICRO_VERSION 0)
set (TML_API_VERSION 11)
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_VERSION_MAJOR "${TML_MAJOR_VERSION}")
set (CPACK_PACKAGE_VERSION_MINOR "${TML_MINOR_VERSION}")
set (CPACK_PACKAGE_VERSION_PATCH "${TML_MICRO_VERSION}")
set (PLATFORM_FOUND false)
set (CORE_TARGET_NAME tmlcore)
set (TEST_TARGET_NAME libTmlTest)
set (RTTEST_TARGET_NAME sidexRegTest)
set (TMLRTTEST_TARGET_NAME tmlRegTest)


set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr)
set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/local)
#######################################################
## Windows 32 bit / MinGW build
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
  set(PROJECT win_x86-32_mingw)

  set(IS64BIT false)
  set(BUILD_TEST true)
  
  set(AXLINCLUDE "C:\\Program Files (x86)\\VortexW32\\include\\axl")
  set(VORTEXINCLUDE "C:\\Program Files (x86)\\VortexW32\\include\\vortex")

  LINK_DIRECTORIES("C:\\Program Files (x86)\\VortexW32\\lib")  
  include (CPack)
  
  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .lib)

  set (PLATFORM_FOUND true)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)

#######################################################
## Windows 64bit / MinGW build
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)
  set(PROJECT win_x86-64_mingw)

  set(IS64BIT true)
  set(BUILD_TEST true)
  
  set(AXLINCLUDE "C:\\Program Files\\VortexW64\\include\\axl")
  set(VORTEXINCLUDE "C:\\Program Files\\VortexW64\\include\\vortex")

  LINK_DIRECTORIES("C:\\Program Files\\VortexW64\\lib")

  include (CPack)
  
  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .lib)

  set (PLATFORM_FOUND true)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)

#######################################################
## Windows 32 bit
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32)
  set(PROJECT win_x86-32)

  set(IS64BIT false)
  set(BUILD_TEST true)
  
  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/src")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/src")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/Release")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/Release")
  
  include (CPack)
  
  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .lib)

  set (PLATFORM_FOUND true)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32)

#######################################################
## Windows 64 bit
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64)
  set(PROJECT win_x86-64)
  
  set(IS64BIT true)
  set(BUILD_TEST true)
  
  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/src")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/src")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/x64/Release")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/x64/Release")
  
  include (CPack)

  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .lib)

  set (PLATFORM_FOUND true)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64)

#######################################################
## LINUX 32 bit
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
  set(PROJECT linux_x86-32)
  
  set(IS64BIT false)
  set(BUILD_TEST true)
  
  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/Debian/include")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/Debian/include")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/Debian/i386/lib")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/Debian/i386/lib")
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ../COPYING)
  
  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .so)

  include (CPack)

  set (PLATFORM_FOUND true) 
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)

#######################################################
## LINUX 64 bit
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
  set(PROJECT linux_x86-64)
  
  set(IS64BIT true)
  set(BUILD_TEST true)

  
  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/Debian/include")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/Debian/include")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/Debian/amd64/lib")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/Debian/amd64/lib")
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ../COPYING)
  
  include (CPack)

  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .so)

  set (PLATFORM_FOUND true)  
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)

#######################################################
## FreeBSD / 32 bit
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
  set(PROJECT freeBSD_x86-32)
  
  set(IS64BIT false)
  set(BUILD_TEST true)

  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/freeBSD/include")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/freeBSD/include")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/freeBSD/i386/lib")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/freeBSD/i386/lib")
  LINK_DIRECTORIES("../Komponenten/libiconv/libiconv-1.14/freeBSD/i386/lib")
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ../COPYING)
  
  include (CPack)

  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .so)

  set (PLATFORM_FOUND true)  
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)

#######################################################
## FreeBSD / 64 bit
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
  set(PROJECT freeBSD_x86-64)
  
  set(IS64BIT true)
  set(BUILD_TEST true)

  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/freeBSD/include")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/freeBSD/include")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/freeBSD/amd64/lib")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/freeBSD/amd64/lib")
  LINK_DIRECTORIES("../Komponenten/libiconv/libiconv-1.14/freeBSD/amd64/lib")
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ../COPYING)
  
  include (CPack)

  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .so)

  set (PLATFORM_FOUND true)  
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)

#######################################################
## OSX 32 bit
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
  set(PROJECT osx_x86-32)
  
  set(IS64BIT false)
  set(BUILD_TEST true)
  
  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/OS-X/include")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/OS-X/include")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/OS-X/i386/lib")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/OS-X/i386/lib")

  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ../COPYING)
  
  include (CPack)

  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .dylib)

  set (PLATFORM_FOUND true)
  endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)

#######################################################
## OSX 64 bit
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
  set(PROJECT osx_x86-64)
  
  set(IS64BIT true)
  set(BUILD_TEST true)
  
  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/OS-X/include")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/OS-X/include")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/OS-X/x86_64/lib")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/OS-X/x86_64/lib")

  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ../COPYING)
  
  include (CPack)

  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .dylib)

  set (PLATFORM_FOUND true)
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)

#######################################################
## ANDROID ARM32
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  set(PROJECT android_arm32)

  set(IS64BIT false)
  set(BUILD_TEST false)
  
  set(AXLINCLUDE /x/workdir/libtml-c/3rdParty/libaxl/axl-0.7.0.b5340.g5351/src)
  set(VORTEXINCLUDE /x/workdir/libtml-c/3rdParty/libvortex/vortex-1.1.14.b5256.g5258/src)
  set(ICONVINCLUDE /x/workdir/libtml-c/3rdParty/libiconv/libiconv-1.14/include)
  
  LINK_DIRECTORIES( /x/workdir/libtml-c/3rdParty/libaxl/axl-0.7.0.b5340.g5351/build/android_arm32/libaxl/)
  LINK_DIRECTORIES( /x/workdir/libtml-c/3rdParty/libvortex/vortex-1.1.14.b5256.g5258/build/android_arm32/libvortex/)
  LINK_DIRECTORIES( /x/workdir/libtml-c/3rdParty/libiconv/libiconv-1.14/build/android_arm32/libiconv/)
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ../COPYING)
  
  include (CPack)

  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .so)

  set (PLATFORM_FOUND true)
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)

#######################################################
## LINUX ARM32
if (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)
  set(PROJECT linux_arm32)
  
  set(IS64BIT false)
  set(BUILD_TEST true)
  
  set(AXLINCLUDE "../../../Komponenten/AXL/${AXLVER}/Debian/include")
  set(VORTEXINCLUDE "../../../Komponenten/Vortex/${VORTEXVER}/Debian/include")

  LINK_DIRECTORIES("../Komponenten/AXL/${AXLVER}/Debian/armhf/lib")
  LINK_DIRECTORIES("../Komponenten/Vortex/${VORTEXVER}/Debian/armhf/lib")
  
  set(CPACK_SET_DESTDIR ON)
  set (CPACK_RESOURCE_FILE_LICENSE ../COPYING)
  
  include (CPack)
  
  set (CMAKE_FIND_LIBRARY_PREFIXES lib)
  set (CMAKE_FIND_LIBRARY_SUFFIXES .so)

  set (PLATFORM_FOUND true)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)

if (${PLATFORM_FOUND} STREQUAL true)
  #######################################################
  ## Project / Solution- File:
  PROJECT( tml )
  
  #######################################################
  ## Source and destination directories:
  ADD_SUBDIRECTORY( sidex/src ${PROJECT}/sidex)
  ADD_SUBDIRECTORY( tml/src ${PROJECT}/tml)
  
  if (${BUILD_TEST} STREQUAL true)
    #######################################################
    ## Regression test
    if (${IS64BIT} STREQUAL false)
      set(CORE_TARGET_NAME_LOCAL  ${CORE_TARGET_NAME}${TML_API_VERSION})
      set(TEST_TARGET_NAME_LOCAL ${TEST_TARGET_NAME})
      set(RTTEST_TARGET_NAME_LOCAL ${RTTEST_TARGET_NAME})
  	  set(TMLRTTEST_TARGET_NAME_LOCAL ${TMLRTTEST_TARGET_NAME})
    endif (${IS64BIT} STREQUAL false)
    if (${IS64BIT} STREQUAL true)
      set(CORE_TARGET_NAME_LOCAL ${CORE_TARGET_NAME}${TML_API_VERSION}_64)
      set(TEST_TARGET_NAME_LOCAL ${TEST_TARGET_NAME}64)
      set(RTTEST_TARGET_NAME_LOCAL ${RTTEST_TARGET_NAME}64)
      set(TMLRTTEST_TARGET_NAME_LOCAL ${TMLRTTEST_TARGET_NAME}64)
    endif (${IS64BIT} STREQUAL true)
    
    
    ADD_SUBDIRECTORY( test/src ${PROJECT}/test)
    add_dependencies(${TEST_TARGET_NAME_LOCAL} ${CORE_TARGET_NAME_LOCAL})
    cmake_policy(SET CMP0046 OLD)

    ADD_SUBDIRECTORY( test/regTests/sidexRegTest/src ${PROJECT}/regTests/sidexRegTest)
    add_dependencies(${RTTEST_TARGET_NAME_LOCAL} ${CORE_TARGET_NAME_LOCAL}) 
    cmake_policy(SET CMP0046 OLD)

    ADD_SUBDIRECTORY( test/regTests/tmlRegTest/src ${PROJECT}/regTests/tmlRegTest)
    add_dependencies(${TMLRTTEST_TARGET_NAME_LOCAL} ${CORE_TARGET_NAME_LOCAL}) 
    cmake_policy(SET CMP0046 OLD)
  endif (${BUILD_TEST} STREQUAL true)
else (${PLATFORM_FOUND} STREQUAL true)
  message( SEND_ERROR "Unknown target platform / please check \"TML_TARGET_PLATFORM\"" )
endif (${PLATFORM_FOUND} STREQUAL true)
