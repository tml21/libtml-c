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
 * @brief    init the object
 */
void tmlConnectionManageObj::initConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress)
{
  m_iErr = TML_SUCCESS;
  m_coreHandle = coreHandle;

  m_binding = new tmlNetBinding(sNetAddress);
  m_iErr = establishVortexConnection();

  m_iRefCounter = 1;

  //////////////////////////////////////////////////
  // Add handle to core list
  if (TML_HANDLE_TYPE_NULL != m_coreHandle){
    ((tmlCoreWrapper*)m_coreHandle)->tmlCoreWrapper_Add_ConnectionItem((TML_CONNECTION_HANDLE) this);
  }
}


/**
  * @brief   Establish the Vortex connection 
  */
TML_INT32 tmlConnectionManageObj::establishVortexConnection(){

  int iRet = TML_SUCCESS;

  VortexCtx* ctx = ((tmlCoreWrapper*)m_coreHandle)->getVortexCtx();
  tmlLogHandler* log =  ((tmlCoreWrapper*)m_coreHandle)->getLogHandler();

  VortexConnection* connection = NULL;

  // Is there a valid vortex execution context
  if (NULL == ctx){
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
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
        connection = NULL;
      }
    }
  }
  m_vortexConnection = connection;
  return iRet;
}


/**
  * @brief   returns the last error code
  */
TML_INT32 tmlConnectionManageObj::getLastErr(){
  return m_iErr;
}


/**
 * @brief    Destructor.
 */
tmlConnectionManageObj::~tmlConnectionManageObj()
{
  cleanUp();
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
 * @brief Get the network address for connection binding.
 */
TML_INT32 tmlConnectionManageObj::getAddress(char** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
  * @brief Get the network address for connection binding.
  */
TML_INT32 tmlConnectionManageObj::getAddress(wchar_t** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
  * @brief Get the network address for connection binding.
  */
TML_INT32 tmlConnectionManageObj::getAddress(char16_t** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
  * @brief Returns the remote peer supported profiles.
  */
TML_INT32 tmlConnectionManageObj::getRemoteProfiles(SIDEX_VARIANT* lProfiles){

  // TODO:
  // vortex_conection_get_remote_progiles();
  return TML_SUCCESS;
}


/**
  * @brief    Validate a connection.
  */
TML_INT32 tmlConnectionManageObj::validate(TML_BOOL bReconnect, TML_BOOL* bConnected){
  // TODO:
  // vortex_conection_is_ok();
  // TODO:
  // vortex_conection_new();
  return TML_SUCCESS;
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
