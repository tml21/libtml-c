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
#  For commercial support on build UJO enabled solutions contact us:
#  
# Contributors:
#    wobe-systems GmbH

#######################################################
## Common src path
set(COMMON_INCLUDE_PATH ../../common/src)
set(COMMON_SRC_PATH ../../common/src)
set(SIDEXINCLUDE ../../sidex/src)

#######################################################
## Source & Header
set  (TML_COMMON_SOURCES_HEADER 
                    ${COMMON_SRC_PATH}/globalDefines.h 
                    ${COMMON_SRC_PATH}/logValues.h 
                    ${COMMON_SRC_PATH}/stdIntTypes.h 
                    ${COMMON_SRC_PATH}/systemBase.h 
                    ${COMMON_SRC_PATH}/unicode.h )

set  (TML_INSTALL_SOURCES_HEADER tmlCore.h
                      tmlErrors.h
                      tmlGlobalDefines.h
                      tmlStdTypes.h )
                      
set  (TML_SOURCES_HEADER tmlCCallback.h
                      tmlCmdDefines.h
                      tmlCollectCall.h
                      tmlCollectCallDestinationObj.h
                      tmlCollectCallDestinationObjHandler.h
                      tmlConnectionObj.h
                      tmlCoreDefines.h
                      tmlCoreHash.h
                      tmlCoreListener.h
                      tmlCoreSender.h
                      tmlCoreWrapper.h
                      tmlCriticalSectionObj.h
                      tmlDestinationAddressHandlingHashValue.h
                      tmlDispatcher.h
                      tmlEventCall.h
                      tmlEventCallHandler.h
                      tmlEventData.h
                      tmlEventDataQueue.h
                      tmlEventHandler.h
                      tmlGlobalCallback.h
                      tmlLogHandler.h
                      tmlMultiAsyncMsg.h
                      tmlObjWrapper.h
                      tmlProfileHandler.h
                      tmlReceiverStream.h
                      tmlSenderStream.h
                      tmlSingleCall.h
                      tmlStreamHandler.h
                      tmlUniversalHashTable.h )
                      
set (TML_COMMON_SOURCES ${COMMON_SRC_PATH}/systemBase.cpp
                      ${COMMON_SRC_PATH}/unicode.cpp )        
                                    
set (TML_SOURCES      tmlCollectCall.cpp
                      tmlCollectCallDestinationObj.cpp
                      tmlCollectCallDestinationObjHandler.cpp
                      tmlConnectionObj.cpp
                      tmlCore.cpp
                      tmlCoreHash.cpp
                      tmlCoreListener.cpp
                      tmlCoreSender.cpp
                      tmlCoreWrapper.cpp
                      tmlCriticalSectionObj.cpp
                      tmlDestinationAddressHandlingHashValue.cpp
                      tmlDispatcher.cpp
                      tmlEventCall.cpp
                      tmlEventCallHandler.cpp
                      tmlEventData.cpp
                      tmlEventDataQueue.cpp
                      tmlEventHandler.cpp
                      tmlGlobalCallback.cpp
                      tmlLogHandler.cpp
                      tmlMultiAsyncMsg.cpp
                      tmlObjWrapper.cpp
                      tmlProfileHandler.cpp
                      tmlReceiverStream.cpp
                      tmlSenderStream.cpp
                      tmlSingleCall.cpp
                      tmlStreamHandler.cpp
                      tmlUniversalHashTable.cpp )

source_group("CommonHeaderfiles" FILES ${TML_COMMON_SOURCES_HEADER})
source_group("InstallHeaderfiles" FILES ${TML_INSTALL_SOURCES_HEADER})
source_group("Headerfiles" FILES ${TML_SOURCES_HEADER})
source_group("CommonSourcefiles" FILES ${TML_COMMON_SOURCES})
source_group("Sourcefiles" FILES ${TML_SOURCES})

#######################################################
## Windows 32 bit / MinGW build
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
  set(AXLLIB libaxl.lib)
  set(VORTEXLIB libvortex-1.1.lib)

  set(PREPROCESSOR_DEF -DTML_EXPORTS -DMINGW_BUILD)
  
  set(TARGET tmlcore)
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

  set(PREPROCESSOR_DEF -DTML_EXPORTS -DMINGW_BUILD)
  
  set(TARGET tmlcore)
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
  
  set(PREPROCESSOR_DEF -DTML_EXPORTS)
  
  set(TARGET tmlcore)
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

  set(PREPROCESSOR_DEF -DTML_EXPORTS)
  
  set(TARGET tmlcore)
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
  set(ICONVLIB libiconv.so)
  set(PTHREADLIB libpthread.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m32")
  set(CMAKE_CXX_FLAGS "-m32")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)
  
  set(TARGET tmlcore)
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
  set(ICONVLIB libiconv.so)
  set(PTHREADLIB libpthread.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m64")
  set(CMAKE_CXX_FLAGS "-m64")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)
  
  set(TARGET tmlcore)
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
  set(ICONVLIB libiconv.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m32")
  set(CMAKE_CXX_FLAGS "-m32")
  set(PREPROCESSOR_DEF -DLINUX -DFREE_BSD -DGCC4)
  
  set(TARGET tmlcore)
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
  set(ICONVLIB libiconv.so)

  ADD_DEFINITIONS ("-std=c++0x")
  ADD_DEFINITIONS ("-m64")
  set(CMAKE_CXX_FLAGS "-m64")
  set(PREPROCESSOR_DEF -DLINUX -DFREE_BSD -DGCC4)
  
  set(TARGET tmlcore)
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

  ADD_DEFINITIONS ("-std=c++11")
  ADD_DEFINITIONS ("-arch i386")
  set(PREPROCESSOR_DEF -DLINUX -DOS_X)
  set(CMAKE_CXX_FLAGS "-arch i386")
  
  set(TARGET tmlcore)
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

  ADD_DEFINITIONS ("-std=c++11")
  ADD_DEFINITIONS ("-arch x86_64")
  set(PREPROCESSOR_DEF -DLINUX -DOS_X)
  set(CMAKE_CXX_FLAGS "-arch x86_64")
 
  set(TARGET tmlcore)
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

  ADD_DEFINITIONS ("-std=c++11")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4 -DANDROID)
  
  set(TARGET tmlcore)
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
  set(PTHREADLIB libpthread.so)

  ADD_DEFINITIONS ("-std=c++0x")
  set(PREPROCESSOR_DEF -DLINUX -DGCC4)
  
  set(TARGET tmlcore)
  set(IS64BIT false)
  
  set (INST_LIB_PATH /usr/lib)
  set (INST_INCLUDE_PATH /usr/include/tml)
endif (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)

                    
if (${IS64BIT} STREQUAL false)
  set(TMLLIBNAME_LOCAL  ${TARGET}${TML_API_VERSION})
  set(TMLLIBNAME ${TMLLIBNAME_LOCAL} PARENT_SCOPE)
endif (${IS64BIT} STREQUAL false)
if (${IS64BIT} STREQUAL true)
  set(TMLLIBNAME_LOCAL ${TARGET}${TML_API_VERSION}_64)
  set(TMLLIBNAME ${TMLLIBNAME_LOCAL} PARENT_SCOPE)
endif (${IS64BIT} STREQUAL true)

#######################################################
## The target is a shared library 
ADD_LIBRARY( ${TMLLIBNAME_LOCAL} SHARED ${TML_COMMON_SOURCES_HEADER} ${TML_INSTALL_SOURCES_HEADER} ${TML_SOURCES_HEADER} ${TML_COMMON_SOURCES} ${TML_SOURCES})

#######################################################
## On MinGW build target prefix don't have to be lib:
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
  set_target_properties(${TMLLIBNAME_LOCAL} PROPERTIES PREFIX "")
endif (${TML_TARGET_PLATFORM} STREQUAL win_x86-32_mingw)
if (${TML_TARGET_PLATFORM} STREQUAL win_x86-64_mingw)
  set_target_properties(${TMLLIBNAME_LOCAL} PROPERTIES PREFIX "")
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
INCLUDE_DIRECTORIES (${SIDEXINCLUDE})
INCLUDE_DIRECTORIES (${COMMON_INCLUDE_PATH})
INCLUDE_DIRECTORIES ( . )

#######################################################
## Preprocessor- definitions:
ADD_DEFINITIONS( ${PREPROCESSOR_DEF})
              
#######################################################
## libraries
TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${AXLLIB} )    
TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${VORTEXLIB} )    
TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${SIDEXLIBNAME} )    

#######################################################
## ICONV library in case of OSX
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${ICONVLIB} )                         
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${PTHREADLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${ICONVLIB} )                         
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${PTHREADLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL freeBSD_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-32)
if (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL osx_x86-64)
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${ICONVLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
if (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${ICONVLIB} )                         
  TARGET_LINK_LIBRARIES(${TMLLIBNAME_LOCAL} ${PTHREADLIB} )                         
endif (${TML_TARGET_PLATFORM} STREQUAL linux_arm32)

#######################################################
## Install
install (TARGETS ${TMLLIBNAME_LOCAL} DESTINATION ${INST_LIB_PATH})
install (FILES ${TML_INSTALL_SOURCES_HEADER} DESTINATION ${INST_INCLUDE_PATH})
