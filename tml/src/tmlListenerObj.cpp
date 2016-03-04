/* 
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2015 wobe-systems GmbH
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 *
 *  Homepage:
 *    http://www.libtml.org
 *
 *  For professional support contact us:
 *
 *    wobe-systems GmbH
 *    support@libtml.org
 *
 * Contributors:
 *    wobe-systems GmbH
 */

#include <stdio.h>
#include <string.h>
#include "tmlListenerObj.h"
#include "tmlCore.h"

/**
 * @brief    Constructor.
 */
tmlListenerObj::tmlListenerObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress)
{
  initListenerObj(coreHandle, sNetAddress);
}


/**
 * @brief    Constructor.
 */
tmlListenerObj::tmlListenerObj(TML_CORE_HANDLE coreHandle, const char* sHost, const char* sPort)
{
  int iLength = strlen(sHost) + strlen(sPort) + 2;

  char* sNetAddress = new char[iLength];
  #if defined(LINUX) || defined (MINGW_BUILD)
    sprintf(sNetAddress, "%s:%s", sHost, sPort);
  #else // LINUX
    sprintf_s(sNetAddress, iLength, "%s:%s", sHost, sPort);
  #endif // LINUX

  initListenerObj(coreHandle, sNetAddress);

  delete[]sNetAddress;
}


/**
 * @brief    Destructor.
 */
tmlListenerObj::~tmlListenerObj()
{
  cleanUp();
}



/**
  * @brief    init the object
  */
void tmlListenerObj::initListenerObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress){
  m_bListnerIsEnabled = TML_FALSE;

  m_coreHandle = coreHandle;

  m_binding = new tmlNetBinding(sNetAddress);

   m_iRefCounter = 1;
}


/**
 * @brief    Cleans up refCounter dependent allocations.
 */
void tmlListenerObj::cleanUp(){
  if (getRef()){
    if (decRef() == 0){
      delete m_binding;
    }
  }
}


/**
 * @brief Get the TML core handle.
 */
TML_CORE_HANDLE tmlListenerObj::getCoreHandle(){
  return m_coreHandle;
}


/**
 * @brief Get the network hostname / IP of the listener binding.
 */
TML_INT32 tmlListenerObj::getHost(char** sHost){
  TML_INT32 iRet = m_binding->getHost(sHost);
  return iRet;
}


/**
  * @brief Get the network hostname / IP of the listener binding.
  */
TML_INT32 tmlListenerObj::getHost(wchar_t** sHost){
  TML_INT32 iRet = m_binding->getHost(sHost);
  return iRet;
}


/**
  * @brief Get the network hostname / IP of the listener binding.
  */
TML_INT32 tmlListenerObj::getHost(char16_t** sHost){
  TML_INT32 iRet = m_binding->getHost(sHost);
  return iRet;
}


/**
 * @brief Get the network port of the listener binding.
 */
TML_INT32 tmlListenerObj::getPort(char** sPort){
  TML_INT32 iRet = m_binding->getPort(sPort);
  return iRet;
}


/**
  * @brief Get the network port of the listener binding.
  */
TML_INT32 tmlListenerObj::getPort(wchar_t** sPort){
  TML_INT32 iRet = m_binding->getPort(sPort);
  return iRet;
}


/**
  * @brief Get the network port of the listener binding.
  */
TML_INT32 tmlListenerObj::getPort(char16_t** sPort){
  TML_INT32 iRet = m_binding->getPort(sPort);
  return iRet;
}


/**
 * @brief Get the network address of the listener binding.
 */
TML_INT32 tmlListenerObj::getAddress(char** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
 * @brief Get the network address of the listener binding.
  */
TML_INT32 tmlListenerObj::getAddress(wchar_t** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
 * @brief Get the network address of the listener binding.
  */
TML_INT32 tmlListenerObj::getAddress(char16_t** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
 * @brief Check for equality of this listener with the requested parameter.
 */
bool tmlListenerObj::isEqual(const char* sAddress){
  bool bEqual = false;
  char* sRefAddress;
  TML_INT32 iRet = getAddress(&sRefAddress);
  if (TML_SUCCESS == iRet){
    if (0 == strcmp(sRefAddress, sAddress)){
      bEqual = true;
    }
  }
  return bEqual;
}


/**
  * @brief   Enable or disable the listener.
  */
TML_INT32 tmlListenerObj::set_Enabled(TML_BOOL bEnable){
  TML_INT32 iRet = TML_SUCCESS;
  if (bEnable == m_bListnerIsEnabled){
    iRet = TML_SUCCESS;
  }
  else{
    if (!m_bListnerIsEnabled){
      ////////////////////////////////////////
      // Enable logging:
      //m_CoreListener->TMLCoreListener_Set_Vortex_Logging_Value(m_iLogValue);
      ////////////////////////////////////////////////////////////////////////////////////////////
      // Set contiguous log file index to make logging to the right file in the TMLCoreListener_Start of the sender possible:
      //m_CoreListener->setLogFileIndex(m_iLogFileIndex);
      //////////////////////
      // Start the listener:
      //iRet = m_CoreListener->TMLCoreListener_Start(m_sListenerIP, m_sListenerPort, &resPort, &m_ListenerCallback);

      if (TML_SUCCESS == iRet){
        //////////////////////////////////////////////////////////////
        // in case of m_sListenerPort equals 0 the vortex_listener_new will find
        // the next free port, so I want to save it's identification:
        //tmlCoreWrapper_Set_ListenerPort_A((char*)resPort);
        ////////////////////////////////////////
        // Profile registration at the listener:
        //int iSize = 0;
        //iRet = m_dispatcherHashTable->hashSize(&iSize);
        //if (TML_SUCCESS == iRet && 0 < iSize){
          //char** retProfiles = NULL;
          //int iRet = m_dispatcherHashTable->getKeys(&retProfiles);
          //if (TML_SUCCESS == iRet && NULL != retProfiles){
            //for (int i = 0;i < iSize; ++i){
              //m_CoreListener->TMLCoreListener_RegisterProfile(retProfiles[i], (TML_CORE_HANDLE)this);
              //delete (retProfiles[i]);
            //}
            //delete retProfiles;
          //}
        //}
      }
    }
    else{
      //iRet = m_CoreListener->TMLCoreListener_Stop();
    }
    // In case of TML_SUCCESS:
    if (TML_SUCCESS == iRet)
      m_bListnerIsEnabled = bEnable;
  }
  return iRet;
}


/**
  * @brief   Get listener status.
  */
TML_BOOL tmlListenerObj::get_Enabled(){
  return m_bListnerIsEnabled;
}


/**
 * @brief   Decrement the reference counter value of this data object for the memory management.
 *
 */
int tmlListenerObj::decRef(){
  return --m_iRefCounter;
}


/**
 * @brief   Increment the reference counter value of this data object for the memory management.
 */

int tmlListenerObj::incRef(){
  return ++m_iRefCounter;
}


/**
     * @brief   Get the reference counter value of this data object for the memory management.
 */
int tmlListenerObj::getRef(){
  return m_iRefCounter;
}
