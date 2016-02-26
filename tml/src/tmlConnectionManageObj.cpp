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
#include "tmlConnectionManageObj.h"
#include "tmlCoreWrapper.h"
#include "tmlCore.h"
#include "unicode.h"
#include "logValues.h"
#include "sidex.h"

/**
 * @brief    Constructor.
 *
 */
tmlConnectionManageObj::tmlConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sHost, const char* sPort)
{
  int iLength = strlen(sHost) + strlen(sPort) + 2;

  char* sNetAddress = new char[iLength];
  #if defined(LINUX) || defined (MINGW_BUILD)
    sprintf(sNetAddress, "%s:%s", sHost, sPort);
  #else // LINUX
    sprintf_s(sNetAddress, iLength, "%s:%s", sHost, sPort);
  #endif // LINUX

  initConnectionManageObj(coreHandle, sNetAddress);

  delete[]sNetAddress;
}


/**
 * @brief    Constructor.
 */
tmlConnectionManageObj::tmlConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress)
{
  initConnectionManageObj(coreHandle, sNetAddress);
}


/**
 * @brief    Destructor.
 */
tmlConnectionManageObj::~tmlConnectionManageObj()
{
  cleanUp();
}

/**
 * @brief    init the object
 */
void tmlConnectionManageObj::initConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress)
{
  m_iErr = TML_SUCCESS;
  m_coreHandle = coreHandle;

  m_binding = new tmlNetBinding(sNetAddress);
  m_vortexConnection = NULL;

  m_iErr = establishVortexConnection();

  m_iRefCounter = 1;
}


/**
  * @brief   Establish the Vortex connection 
  */
TML_INT32 tmlConnectionManageObj::establishVortexConnection(){

  int iRet = TML_SUCCESS;

  VortexCtx* ctx = ((tmlCoreWrapper*)m_coreHandle)->getVortexCtx();
  tmlLogHandler* log =  ((tmlCoreWrapper*)m_coreHandle)->getLogHandler();

  // Is there a valid vortex execution context
  if (NULL == ctx){
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }
  else{
    VortexConnection* connection = m_vortexConnection;
    //////////////////////////////////////////////////////////
    // maybe we have a vortex connection and try to reconnect:
    if (NULL != connection){
      log->log (TML_LOG_VORTEX_CMD, "tmlConnectionManageObj", "establishVortexConnection", "Vortex CMD", "vortex_connection_reconnect");
      axl_bool bConnected;
      bConnected = vortex_connection_reconnect (connection, NULL, NULL);
      if (axl_false == bConnected){
        const char* msg = vortex_connection_get_message(connection);
        log->log ("tmlConnectionManageObj", "establishVortexConnection", "vortex_connection_get_message", msg);
        iRet = TML_ERR_SENDER_INVALID_PARAMS;
      }
    }
    else{
      ///////////////////////////////////////////////////
      // Set the connection timeout to 5 seconds:
      log->log (TML_LOG_VORTEX_CMD, "tmlConnectionManageObj", "establishVortexConnection", "Vortex CMD", "vortex_connection_connect_timeout");
      vortex_connection_connect_timeout (ctx, 5000000);

      char* sHost;
      char* sPort;
      TML_BOOL bIsIPV6 = TML_FALSE;
      iRet = m_binding->getHost(&sHost);
      if (TML_SUCCESS == iRet){
        iRet = m_binding->getPort(&sPort);
      }
      if (TML_SUCCESS == iRet){
        bIsIPV6 = m_binding->isIPV6();
      }
      if (TML_SUCCESS == iRet){
        if (bIsIPV6){
          log->log (TML_LOG_VORTEX_CMD, "tmlConnectionManageObj", "establishVortexConnection", "Vortex CMD", "vortex_connection_new");
          connection = vortex_connection_new6 (ctx, sHost, sPort, NULL, NULL);
        }
        else{
          log->log (TML_LOG_VORTEX_CMD, "tmlConnectionManageObj", "establishVortexConnection", "Vortex CMD", "vortex_connection_new");
          connection = vortex_connection_new (ctx, sHost, sPort, NULL, NULL);
        }
        log->log (TML_LOG_VORTEX_CMD, "tmlConnectionManageObj", "establishVortexConnection", "Vortex CMD", "vortex_connection_is_ok");
        if (!vortex_connection_is_ok (connection, axl_false))
        {
          const char* msg = vortex_connection_get_message(connection);
          log->log ("tmlConnectionManageObj", "establishVortexConnection", "vortex_connection_get_message", msg);
          iRet = TML_ERR_SENDER_INVALID_PARAMS;
        }
      }
    }
    m_vortexConnection = connection;
  }
  return iRet;
}


/**
  * @brief   returns the last error code
  */
TML_INT32 tmlConnectionManageObj::getLastErr(){
  return m_iErr;
}


/**
 * @brief    Cleans up refCounter dependent allocations.
 */
void tmlConnectionManageObj::cleanUp(){
  if (getRef())
    if (decRef() == 0){
      if (NULL != m_vortexConnection){
        tmlLogHandler* log =  ((tmlCoreWrapper*)m_coreHandle)->getLogHandler();
        ////////////////////////////////////////////////////////////////////////
        // shutdown connection:
        log->log (TML_LOG_VORTEX_CMD, "tmlConnectionManageObj", "cleanUp", "Vortex CMD", "vortex_connection_shutdown");
        vortex_connection_shutdown(m_vortexConnection);
        ////////////////////////////////////////////////////////////////////////
        // close connection to get rid of references:
        log->log (TML_LOG_VORTEX_CMD, "tmlConnectionManageObj", "cleanUp", "Vortex CMD", "vortex_connection_close");
        vortex_connection_close(m_vortexConnection);
        m_vortexConnection = NULL;
      }
      delete m_binding;
    }
}


/**
 * @brief Get the TML core handle.
 */
TML_CORE_HANDLE tmlConnectionManageObj::getCoreHandle(){
  return m_coreHandle;
}


/**
 * @brief Get the network hostname / IP of the connection binding.
 */
TML_INT32 tmlConnectionManageObj::getHost(char** sHost){
  TML_INT32 iRet = m_binding->getHost(sHost);
  return iRet;
}


/**
  * @brief Get the network hostname / IP of the connection binding.
  */
TML_INT32 tmlConnectionManageObj::getHost(wchar_t** sHost){
  TML_INT32 iRet = m_binding->getHost(sHost);
  return iRet;
}


/**
  * @brief Get the network hostname / IP of the connection binding.
  */
TML_INT32 tmlConnectionManageObj::getHost(char16_t** sHost){
  TML_INT32 iRet = m_binding->getHost(sHost);
  return iRet;
}


/**
 * @brief Get the network port of the connection binding.
 */
TML_INT32 tmlConnectionManageObj::getPort(char** sPort){
  TML_INT32 iRet = m_binding->getPort(sPort);
  return iRet;
}


/**
  * @brief Get the network port of the connection binding.
  */
TML_INT32 tmlConnectionManageObj::getPort(wchar_t** sPort){
  TML_INT32 iRet = m_binding->getPort(sPort);
  return iRet;
}


/**
  * @brief Get the network port of the connection binding.
  */
TML_INT32 tmlConnectionManageObj::getPort(char16_t** sPort){
  TML_INT32 iRet = m_binding->getPort(sPort);
  return iRet;
}


/**
 * @brief Get the network address of the connection binding.
 */
TML_INT32 tmlConnectionManageObj::getAddress(char** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
  * @brief Get the network address of the connection binding.
  */
TML_INT32 tmlConnectionManageObj::getAddress(wchar_t** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
  * @brief Get the network address of the connection binding.
  */
TML_INT32 tmlConnectionManageObj::getAddress(char16_t** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
 * @brief Check for equality of this connection with the requested parameter.
 */
bool tmlConnectionManageObj::isEqual(const char* sAddress){
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
 * @brief Returns the remote peer supported profiles.
 */
TML_INT32 tmlConnectionManageObj::getRemoteProfiles(SIDEX_VARIANT* lProfiles){
  TML_INT32 iRet = TML_SUCCESS;

  SIDEX_VARIANT list = SIDEX_HANDLE_TYPE_NULL;

  if (NULL != m_vortexConnection){

    axlList* profiles = NULL;
    profiles = vortex_connection_get_remote_profiles(m_vortexConnection);
    int iAxlListLength = axl_list_length (profiles);
    if (-1 == iAxlListLength){
      //////////////////
      // No Connection:
      iRet = TML_ERR_SENDER_INVALID_PARAMS;
    }
    else
    {
      list = sidex_Variant_New_List();
      /////////////////////
      // for each item do:
      int iterator = 0;
      while (iterator < axl_list_length (profiles)) {
        SIDEX_INT32 iPos;
        SIDEX_VARIANT item;
        iRet = sidex_Variant_New_String((char *) axl_list_get_nth (profiles, iterator), &item);
        if (SIDEX_SUCCESS == iRet){
          sidex_Variant_List_Append(list, item, &iPos);
          sidex_Variant_DecRef(item);
        }
        iterator++;
      }
      ///////////////////////////////////////
      // free the list when no longer needed
      axl_list_free (profiles);
    }
  }
  else{
    iRet = TML_ERR_MISSING_OBJ;
  }

  *lProfiles = list;
  return iRet;
}


/**
 * @brief Check the connection
 */
void tmlConnectionManageObj::checkConnection(TML_BOOL* bConnected){
  TML_INT32 iStatus = TML_FALSE;

  VortexCtx* ctx = ((tmlCoreWrapper*)m_coreHandle)->getVortexCtx();
  tmlLogHandler* log =  ((tmlCoreWrapper*)m_coreHandle)->getLogHandler();

  if (NULL != m_vortexConnection){
    log->log (TML_LOG_VORTEX_CMD, "tmlConnectionManageObj", "checkConnection", "Vortex CMD", "vortex_connection_is_ok");
    if (vortex_connection_is_ok (m_vortexConnection, axl_false))
    {
      iStatus = TML_TRUE;
    }
  }
  *bConnected = iStatus;
}


/**
  * @brief    Validate a connection.
  */
TML_INT32 tmlConnectionManageObj::validate(TML_BOOL bReconnect, TML_BOOL* bConnected){
  TML_INT32 iRet = TML_SUCCESS;
  TML_INT32 iStatus = TML_FALSE;

  //////////////////////////////////
  // Check the connection:
  checkConnection(&iStatus);
  if (TML_FALSE == iStatus && bReconnect){
    //////////////////////////////////
    // Try to reconnect:
    establishVortexConnection();
    //////////////////////////////////
    // Check the connection once more:
    checkConnection(&iStatus);
  }
  *bConnected = iStatus;
  return iRet;
}

/**
  * @brief   Get Vortex connection 
  */
VortexConnection* tmlConnectionManageObj::getVortexConnection(){
  return m_vortexConnection;
}

/**
 * @brief   Decrement the reference counter value of this data object for the memory management.
 *
 */
int tmlConnectionManageObj::decRef(){
  return --m_iRefCounter;
}


/**
 * @brief   Increment the reference counter value of this data object for the memory management.
 */

int tmlConnectionManageObj::incRef(){
  return ++m_iRefCounter;
}


/**
     * @brief   Get the reference counter value of this data object for the memory management.
 */
int tmlConnectionManageObj::getRef(){
  return m_iRefCounter;
}
