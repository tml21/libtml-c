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
set(TMLINCLUDE ../../tml/src)
set(SIDEXINCLUDE ../../sidex/src)

#######################################################
## set build parameter for install (default = yes)
#######################################################
SET (GEN_TLS_INSTALLER_FILES yes CACHE STRING "set tls installer files")

#######################################################
## Source & Header

set  (TLS_COMMON_SOURCES_HEADER ${COMMON_INCLUDE_PATH}/tmlConnectionManageObjBase.h
                                ${COMMON_INCLUDE_PATH}/unicode.h
                                ${COMMON_INCLUDE_PATH}/tmlCriticalSectionObj.h
                                ${COMMON_INCLUDE_PATH}/tmlCoreWrapperBase.h )

set  (TLS_INSTALL_SOURCES_HEADER tml-tls.h )
                      
                   
set (TLS_COMMON_SOURCES ${COMMON_SRC_PATH}/tmlConnectionManageObjBase.cpp 
                        ${COMMON_SRC_PATH}/unicode.cpp
                        ${COMMON_SRC_PATH}/tmlCriticalSectionObj.cpp
                        ${COMMON_SRC_PATH}/tmlCoreWrapperBase.cpp)

set (TLS_SOURCES      tml-tls.cpp)

source_group("CommonHeaderfiles" FILES ${TLS_COMMON_SOURCES_HEADER})
source_group("InstallHeaderfiles" FILES ${TLS_INSTALL_SOURCES_HEADER})
source_group("CommonSourcefiles" FILES ${TLS_COMMON_SOURCES})
source_group("Sourcefiles" FILES ${TLS_SOURCES})

#######################################################
## Windows 32 bit / MinGW build
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
  set(AXLLIB libaxl.lib)
  set(VORTEXLIB libvortex-1.1.lib)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.lib)

  set(PREPROCESSOR_DEF -DTML_EXPORTS -DMINGW_BUILD)
  
  set(TARGET tml-tls)
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
  set(VORTEXLIB libvortex-1.1.lib)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.lib)

  set(PREPROCESSOR_DEF -DTML_EXPORTS -DMINGW_BUILD)
  
  set(TARGET tml-tls)
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
  set(VORTEXLIB libvortex-1.1.lib)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.lib)
  
  set(PREPROCESSOR_DEF -DTML_EXPORTS)
  
  set(TARGET tml-tls)
  set(IS64BIT false)
  
  set (INST_LIB_PATH ./lib)
  set (INST_INCLUDE_PATH ./include)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32)

#######################################################
## Windows / 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64)

  set(AXLLIB libaxl.lib)
  set(VORTEXLIB libvortex-1.1.lib)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.lib)

  set(PREPROCESSOR_DEF -DTML_EXPORTS)
  
  set(TARGET tml-tls)
  set(IS64BIT true)
  
  set (INST_LIB_PATH ./lib)
  set (INST_INCLUDE_PATH ./include)
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64)

#######################################################
## LINUX 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
  set(AXLLIB libaxl.so)
  set(VORTEXLIB libvortex-1.1.so)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.so)
  set(ICONVLIB libiconv.so)
  set(PTHREADLIB libpthread.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m32")
  set(CMAKE_CXX_FLAGS "-m32")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)
  
  set(INST_PLATFORM_ID i386)
  
  set(TARGET tml-tls)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/lib)
  set (INST_INCLUDE_PATH /usr/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)

#######################################################
## LINUX / 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
  set(AXLLIB libaxl.so)
  set(VORTEXLIB libvortex-1.1.so)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.so)
  set(ICONVLIB libiconv.so)
  set(PTHREADLIB libpthread.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m64")
  set(CMAKE_CXX_FLAGS "-m64")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)

  set(INST_PLATFORM_ID amd64)
  
  set(TARGET tml-tls)
  set(IS64BIT true)
  
  set (INST_LIB_PATH /usr/lib)
  set (INST_INCLUDE_PATH /usr/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)

#######################################################
## FreeBSD / 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
  set(AXLLIB libaxl.so)
  set(VORTEXLIB libvortex-1.1.so)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.so)
  set(ICONVLIB libiconv.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m32")
  set(CMAKE_CXX_FLAGS "-m32")
  set(PREPROCESSOR_DEF -DLINUX -DFREE_BSD -DGCC4)
  
  set(TARGET tml-tls)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)

#######################################################
## FreeBSD / 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
  set(AXLLIB libaxl.so)
  set(VORTEXLIB libvortex-1.1.so)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.so)
  set(ICONVLIB libiconv.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m64")
  set(CMAKE_CXX_FLAGS "-m64")
  set(PREPROCESSOR_DEF -DLINUX -DFREE_BSD -DGCC4)
  
  set(TARGET tml-tls)
  set(IS64BIT true)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)

#######################################################
## OSX 32 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
  set(ICONVLIB libiconv.dylib)
  set(AXLLIB libaxl.0.dylib)
  set(VORTEXLIB libvortex-1.1.dylib)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.dylib)

  ADD_DEFINITIONS ("-std=c++11")
  ADD_DEFINITIONS ("-arch i386")
  set(PREPROCESSOR_DEF -DLINUX -DOS_X)
  set(CMAKE_CXX_FLAGS "-arch i386")
  
  set(TARGET tml-tls)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)

#######################################################
## OSX / 64 bit
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
  set(ICONVLIB libiconv.dylib)
  set(AXLLIB libaxl.0.dylib)
  set(VORTEXLIB libvortex-1.1.dylib)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.dylib)

  ADD_DEFINITIONS ("-std=c++11")
  ADD_DEFINITIONS ("-arch x86_64")
  set(PREPROCESSOR_DEF -DLINUX -DOS_X)
  set(CMAKE_CXX_FLAGS "-arch x86_64")
 
  set(TARGET tml-tls)
  set(IS64BIT true)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)

#######################################################
## ANDROID ARM32
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  set(ICONVLIB libiconv.so)
  set(AXLLIB libaxl.so)
  set(VORTEXLIB libvortex-1.1.so)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.so)

  ADD_DEFINITIONS ("-std=c++11")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4 -DANDROID)
  
  set(TARGET tml-tls)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/local/lib)
  set (INST_INCLUDE_PATH /usr/local/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)

#######################################################
## LINUX ARM32
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)
  set(ICONVLIB libiconv.so)
  set(AXLLIB libaxl.so)
  set(VORTEXLIB libvortex-1.1.so)
  set(VORTEX-TLS-LIB libvortex-tls-1.1.so)
  set(PTHREADLIB libpthread.so)

  ADD_DEFINITIONS ("-std=c++0x")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)
  
  set(INST_PLATFORM_ID armhf)
  
  set(TARGET tml-tls)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/lib)
  set (INST_INCLUDE_PATH /usr/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)

                    
if (${IS64BIT} STREQUAL false)
  set(TLSLIBNAME_LOCAL  ${TARGET}${TML_API_VERSION})
  set(TLSLIBNAME ${TLSLIBNAME_LOCAL} PARENT_SCOPE)
endif (${IS64BIT} STREQUAL false)
if (${IS64BIT} STREQUAL true)
  set(TLSLIBNAME_LOCAL ${TARGET}${TML_API_VERSION}_64)
  set(TLSLIBNAME ${TLSLIBNAME_LOCAL} PARENT_SCOPE)
endif (${IS64BIT} STREQUAL true)

#######################################################
## The target is a shared library 
ADD_LIBRARY( ${TLSLIBNAME_LOCAL} SHARED ${TLS_INSTALL_SOURCES_HEADER} ${TLS_COMMON_SOURCES} ${TLS_SOURCES} ${TLS_COMMON_SOURCES_HEADER})

#######################################################
## On MinGW build target prefix don't have to be lib:
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
  set_target_properties(${TLSLIBNAME_LOCAL} PROPERTIES PREFIX "")
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)
  set_target_properties(${TLSLIBNAME_LOCAL} PROPERTIES PREFIX "")
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)


#######################################################
## Include- directories:
if (AXL_INCLUDE_DIR)
  INCLUDE_DIRECTORIES (${AXL_INCLUDE_DIR})
endif()
if (VORTEX_INCLUDE_DIR)
  INCLUDE_DIRECTORIES (${VORTEX_INCLUDE_DIR})
endif()
INCLUDE_DIRECTORIES (${AXLINCLUDE})
INCLUDE_DIRECTORIES (${ICONVINCLUDE})
INCLUDE_DIRECTORIES (${VORTEXINCLUDE})
INCLUDE_DIRECTORIES (${VORTEX-TLS-INCLUDE})
INCLUDE_DIRECTORIES (${SIDEXINCLUDE})
INCLUDE_DIRECTORIES (${TMLINCLUDE})
INCLUDE_DIRECTORIES (${COMMON_INCLUDE_PATH})
INCLUDE_DIRECTORIES ( . )

#######################################################
## Preprocessor- definitions:
ADD_DEFINITIONS( ${PREPROCESSOR_DEF})
              
#######################################################
## libraries
TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${AXLLIB} )    
TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${VORTEXLIB} )    
TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${VORTEX-TLS-LIB} )    
TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${SIDEXLIBNAME} )    
TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${TMLLIBNAME} )    

#######################################################
## ICONV library in case of OSX
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${ICONVLIB} )                         
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${PTHREADLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${ICONVLIB} )                         
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${PTHREADLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
if (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${ICONVLIB} )                         
  TARGET_LINK_LIBRARIES(${TLSLIBNAME_LOCAL} ${PTHREADLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)

#######################################################
## Install
if (${GEN_TLS_INSTALLER_FILES} STREQUAL yes)
  install (TARGETS ${TLSLIBNAME_LOCAL} DESTINATION ${INST_LIB_PATH})
  install (FILES ${TLS_INSTALL_SOURCES_HEADER} DESTINATION ${INST_INCLUDE_PATH})
endif (${GEN_TLS_INSTALLER_FILES} STREQUAL yes)
