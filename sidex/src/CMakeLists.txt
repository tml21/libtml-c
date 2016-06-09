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


#######################################################
## Common src path
set(COMMON_INCLUDE_PATH ../../common/src)
set(COMMON_SRC_PATH ../../common/src)

#######################################################
## Source & Header
set  (COMMON_INSTALL_SOURCES_HEADER ${COMMON_SRC_PATH}/globalDefines.h 
                    ${COMMON_SRC_PATH}/stdIntTypes.h )

set  (SIDEX_COMMON_SOURCES_HEADER ${COMMON_SRC_PATH}/logValues.h 
                    ${COMMON_SRC_PATH}/unicode.h )

set  (SIDEX_INSTALL_SOURCES_HEADER sidex.h
                    sidexErrors.h
                    sidexStdTypes.h )

set  (SIDEX_SOURCES_HEADER sidexBoolean.h
                    sidexCom.h
                    sidexDateTime.h
                    sidexDefines.h
                    sidexDict.h
                    sidexFloat.h
                    sidexHashTable.h
                    sidexInteger.h
                    sidexList.h
                    sidexLogHandler.h
                    sidexNone.h
                    sidexString.h
                    sidexTable.h
                    sidexTypeBase.h
                    sidexTypes.h
                    sidexUtils.h )

set (SIDEX_COMMON_SOURCES  ${COMMON_SRC_PATH}/unicode.cpp )
                    
set (SIDEX_SOURCES  sidex.cpp
                    sidexBoolean.cpp
                    sidexCom.cpp
                    sidexDateTime.cpp
                    sidexDict.cpp
                    sidexFloat.cpp
                    sidexHashTable.cpp
                    sidexInteger.cpp
                    sidexList.cpp
                    sidexLogHandler.cpp
                    sidexNone.cpp
                    sidexString.cpp
                    sidexTable.cpp
                    sidexTypeBase.cpp
                    sidexUtils.cpp )
                    

source_group("CommonInstallHeaderfiles" FILES ${COMMON_INSTALL_SOURCES_HEADER})
source_group("CommonHeaderfiles" FILES ${SIDEX_COMMON_SOURCES_HEADER})
source_group("InstallHeaderfiles" FILES ${SIDEX_INSTALL_SOURCES_HEADER})
source_group("Headerfiles" FILES ${SIDEX_SOURCES_HEADER})
source_group("CommonSourcefiles" FILES ${SIDEX_COMMON_SOURCES})
source_group("Sourcefiles" FILES ${SIDEX_SOURCES})

#######################################################
## Windows 32 bit / MinGW build
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
  set(AXLLIB libaxl.lib)
  set(PREPROCESSOR_DEF -DSIDEX_EXPORTS -DMINGW_BUILD)
  set(TARGET sidex)
  set(IS64BIT false)
  ADD_DEFINITIONS ("-m32")
  set(CMAKE_CXX_FLAGS "-m32")
  
  set (INST_LIB_PATH ./lib)
  set (INST_INCLUDE_PATH ./include)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)

#######################################################
## Windows 64 bit / MinGW build
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)
  set(AXLLIB libaxl.lib)
  set(PREPROCESSOR_DEF -DSIDEX_EXPORTS -DMINGW_BUILD)
  set(TARGET sidex)
  set(IS64BIT true)
  ADD_DEFINITIONS ("-m64")
  set(CMAKE_CXX_FLAGS "-m64")
  
  set (INST_LIB_PATH ./lib)
  set (INST_INCLUDE_PATH ./include)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)

#######################################################
## Windows 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32)
  set(AXLLIB libaxl.lib)
  set(PREPROCESSOR_DEF -DSIDEX_EXPORTS)
  
  set(TARGET sidex)
  set(IS64BIT false)
  
  set (INST_LIB_PATH ./lib)
  set (INST_INCLUDE_PATH ./include)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32)

#######################################################
## Windows 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64)
  set(AXLLIB libaxl.lib)
  set(PREPROCESSOR_DEF -DSIDEX_EXPORTS)
  
  set(TARGET sidex)
  set(IS64BIT true)
  
  set (INST_LIB_PATH ./lib)
  set (INST_INCLUDE_PATH ./include)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64)

#######################################################
## LINUX 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
  set(AXLLIB libaxl.so)
  set(ICONVLIB libiconv.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m32")
  set(CMAKE_CXX_FLAGS "-m32")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)
  
  set(TARGET sidex)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/lib)
  set (INST_INCLUDE_PATH /usr/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)

#######################################################
## LINUX 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
  set(AXLLIB libaxl.so)
  set(ICONVLIB libiconv.so)
  
  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m64")
  set(CMAKE_CXX_FLAGS "-m64")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)
  
  set(TARGET sidex)
  set(IS64BIT true)
  
  set (INST_LIB_PATH /usr/lib)
  set (INST_INCLUDE_PATH /usr/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)

#######################################################
## FreeBSD / 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
  set(AXLLIB libaxl.so)
  set(ICONVLIB libiconv.so)
  
  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m32")
  set(CMAKE_CXX_FLAGS "-m32")
  set(PREPROCESSOR_DEF -DLINUX -DFREE_BSD -DGCC4)
  
  set(TARGET sidex)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)

#######################################################
## FreeBSD / 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
  set(AXLLIB libaxl.so)
  set(ICONVLIB libiconv.so)
  
  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m64")
  set(CMAKE_CXX_FLAGS "-m64")
  set(PREPROCESSOR_DEF -DLINUX -DFREE_BSD -DGCC4)
  
  set(TARGET sidex)
  set(IS64BIT true)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)

#######################################################
## OSX 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
  set(AXLLIB libaxl.0.dylib)
  set(ICONVLIB libiconv.dylib)
  
  ADD_DEFINITIONS ("-std=c++11")
  ADD_DEFINITIONS ("-arch i386")
  set(PREPROCESSOR_DEF -DLINUX -DOS_X)
  set(CMAKE_CXX_FLAGS "-arch i386")
  
  set(TARGET sidex)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)

#######################################################
## OSX 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
  set(AXLLIB libaxl.0.dylib)
  set(ICONVLIB libiconv.dylib)
  
  ADD_DEFINITIONS ("-std=c++11")
  ADD_DEFINITIONS ("-arch x86_64")
  set(PREPROCESSOR_DEF -DLINUX -DOS_X)
  set(CMAKE_CXX_FLAGS "-arch x86_64")
  
  set(TARGET sidex)
  set(IS64BIT true)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)

#######################################################
## ANDROID ARM32
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  set(AXLLIB libaxl.so)
  set(ICONVLIB libiconv.so)
  ADD_DEFINITIONS ("-std=c++11")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4 -DANDROID)
  
  set(TARGET sidex)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)

#######################################################
## LINUX ARM32
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)
  set(AXLLIB libaxl.so)
  set(ICONVLIB libiconv.so)

  ADD_DEFINITIONS ("-std=c++0x")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)
  
  set(TARGET sidex)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/lib)
  set (INST_INCLUDE_PATH /usr/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)
                    
if (${IS64BIT} STREQUAL false)
  set(SIDEXLIBNAME_LOCAL  ${TARGET}${TML_API_VERSION})
  set(SIDEXLIBNAME ${SIDEXLIBNAME_LOCAL} PARENT_SCOPE)
endif (${IS64BIT} STREQUAL false)
if (${IS64BIT} STREQUAL true)
  set(SIDEXLIBNAME_LOCAL ${TARGET}${TML_API_VERSION}_64)
  set(SIDEXLIBNAME ${SIDEXLIBNAME_LOCAL} PARENT_SCOPE)
endif (${IS64BIT} STREQUAL true)

#######################################################
## The target is a shared library 
ADD_LIBRARY( ${SIDEXLIBNAME_LOCAL} SHARED ${SIDEX_COMMON_SOURCES_HEADER} ${COMMON_INSTALL_SOURCES_HEADER} ${SIDEX_SOURCES_HEADER} ${SIDEX_INSTALL_SOURCES_HEADER} ${SIDEX_COMMON_SOURCES} ${SIDEX_SOURCES})

#######################################################
## On MinGW build target prefix don't have to be lib:
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
  set_target_properties(${SIDEXLIBNAME_LOCAL} PROPERTIES PREFIX "")
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)
  set_target_properties(${SIDEXLIBNAME_LOCAL} PROPERTIES PREFIX "")
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)

#######################################################
## Include- directories:
if (AXL_INCLUDE_DIR)
  INCLUDE_DIRECTORIES (${AXL_INCLUDE_DIR})
endif()
INCLUDE_DIRECTORIES (${AXLINCLUDE})
INCLUDE_DIRECTORIES (${ICONVINCLUDE})
INCLUDE_DIRECTORIES (${COMMON_INCLUDE_PATH})
INCLUDE_DIRECTORIES ( . )

#######################################################
## Preprocessor- definitions:
ADD_DEFINITIONS( ${PREPROCESSOR_DEF})
              
#######################################################
## AXL library 
TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${AXLLIB} )    

#######################################################
## ICONV library 
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
  TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
  TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
  TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
  TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
  TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
  TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
if (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)
  TARGET_LINK_LIBRARIES(${SIDEXLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)

#######################################################
## Install
install (TARGETS ${SIDEXLIBNAME_LOCAL} DESTINATION ${INST_LIB_PATH})
install (FILES ${SIDEX_INSTALL_SOURCES_HEADER} DESTINATION ${INST_INCLUDE_PATH})
install (FILES ${COMMON_INSTALL_SOURCES_HEADER} DESTINATION ${INST_INCLUDE_PATH})
