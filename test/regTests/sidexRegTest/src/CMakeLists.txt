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
#  For professional support contact us:
#
#    wobe-systems GmbH
#    support@libtml.org
# 
# Contributors:
#    wobe-systems GmbH

#######################################################
## Common src path
set(COMMON_INCLUDE_PATH ../../../../common/src)
set(SIDEX_INCLUDE_PATH ../../../../sidex/src)
set(TML_INCLUDE_PATH ../../../../tml/src)

set(TARGETNAME sidexRegTest)  
#######################################################
## Windows 32 bit / MinGW build
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
  ADD_DEFINITIONS("-DSIDEX_UNICODE -DTML_UNICODE -DMINGW_BUILD")
  ADD_DEFINITIONS("-std=c++0x")
  ADD_DEFINITIONS("-fpermissive")
  set(IS64BIT false)

  ADD_DEFINITIONS ("-m32")
  set(CMAKE_CXX_FLAGS "-m32")
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)

#######################################################
## Windows 64 bit / MinGW build
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)
  ADD_DEFINITIONS("-DSIDEX_UNICODE -DTML_UNICODE -DMINGW_BUILD")
  ADD_DEFINITIONS("-std=c++0x")
  ADD_DEFINITIONS("-fpermissive")
  set(IS64BIT true)

  ADD_DEFINITIONS ("-m64")
  set(CMAKE_CXX_FLAGS "-m64")
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)

#######################################################
## Windows 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32)
  ADD_DEFINITIONS("-DSIDEX_UNICODE -DTML_UNICODE")
  set(IS64BIT false)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32)

#######################################################
## Windows 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64)
  ADD_DEFINITIONS("-DSIDEX_UNICODE -DTML_UNICODE")
  set(IS64BIT true)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64)
#######################################################
## LINUX 32 bit
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
  set(IS64BIT false)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS("-m32")
  ADD_DEFINITIONS("-DLINUX -DSIDEX_UNICODE -DTML_UNICODE -DGCC4")
  set(CMAKE_CXX_FLAGS "-m32")
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)

#######################################################
## LINUX 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
  set(IS64BIT true)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS("-m64")
  ADD_DEFINITIONS("-DLINUX -DSIDEX_UNICODE -DTML_UNICODE -DGCC4")
  set(CMAKE_CXX_FLAGS "-m64")
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)

#######################################################
## FreeBSD / 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
  set(IS64BIT false)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS("-m32")
  ADD_DEFINITIONS("-DLINUX -DFREE_BSD -DSIDEX_UNICODE -DTML_UNICODE -DGCC4")
  set(CMAKE_CXX_FLAGS "-m32")
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)

#######################################################
## FreeBSD / 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
  set(IS64BIT true)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS("-m64")
  ADD_DEFINITIONS("-DLINUX -DFREE_BSD -DSIDEX_UNICODE -DTML_UNICODE -DGCC4")
  set(CMAKE_CXX_FLAGS "-m64")
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)

#######################################################
## OSX 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
  set(IS64BIT false)

  ADD_DEFINITIONS ("-std=c++11")
  ADD_DEFINITIONS("-arch i386")
  ADD_DEFINITIONS("-DLINUX -DOS_X -DSIDEX_UNICODE -DTML_UNICODE")
  set(CMAKE_CXX_FLAGS "-arch i386")
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)

#######################################################
## OSX 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
  set(IS64BIT true)

  ADD_DEFINITIONS ("-std=c++11")
  ADD_DEFINITIONS("-arch x86_64")
  ADD_DEFINITIONS("-DLINUX  -DOS_X -DSIDEX_UNICODE -DTML_UNICODE")
  set(CMAKE_CXX_FLAGS "-arch x86_64")
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)

#######################################################
## LINUX ARM32
if (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)
  set(IS64BIT false)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS("-DLINUX -DSIDEX_UNICODE -DTML_UNICODE" -DGCC4)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)

if (${IS64BIT} STREQUAL false)
  set(TARGETNAME_LOCAL  ${TARGETNAME})
else (${IS64BIT} STREQUAL false)
  set(TARGETNAME_LOCAL ${TARGETNAME}64)
endif (${IS64BIT} STREQUAL false)


set(TARGET ${TARGETNAME_LOCAL})
set	 (TEST_SOURCES_HEADER
	  "sdxrt_tchar.h"
	  "sdxrt_test_binary.h"
	  "sdxrt_test_bool.h"
	  "sdxrt_test_datetime.h"
	  "sdxrt_test_dict.h"
	  "sdxrt_test_doc.h"
	  "sdxrt_test_float.h"
	  "sdxrt_test_int.h"
	  "sdxrt_test_list.h"
	  "sdxrt_test_none.h"
	  "sdxrt_test_string.h"
	  "sdxrt_test_table.h"
      "sdxrt_test_get_version.h"
	  "sdxrt_Utils.h")
set  (TEST_SOURCES
	  "sdxrt_tchar.cpp"
	  "sdxrt_test_binary.cpp"
	  "sdxrt_test_bool.cpp"
	  "sdxrt_test_datetime.cpp"
	  "sdxrt_test_dict.cpp"
	  "sdxrt_test_doc.cpp"
	  "sdxrt_test_float.cpp"
	  "sdxrt_test_int.cpp"
	  "sdxrt_test_list.cpp"
	  "sdxrt_test_none.cpp"
	  "sdxrt_test_string.cpp"
	  "sdxrt_test_table.cpp"
      "sdxrt_test_get_version.cpp"
	  "sdxrt_Utils.cpp"
	  "sdxrt_cmd.cpp")
      
add_executable(${TARGETNAME_LOCAL} 
	             ${TEST_SOURCES_HEADER} ${TEST_SOURCES})
				 			 

#######################################################
## Include- directories:
INCLUDE_DIRECTORIES (${SIDEX_INCLUDE_PATH})
INCLUDE_DIRECTORIES (${TML_INCLUDE_PATH})
INCLUDE_DIRECTORIES (".")

#######################################################
## libraries
INCLUDE_DIRECTORIES (${COMMON_INCLUDE_PATH})

TARGET_LINK_LIBRARIES(${TARGET} ${SIDEXLIBNAME})    
TARGET_LINK_LIBRARIES(${TARGET} ${TMLLIBNAME} )    
  
