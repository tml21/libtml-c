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
#include "tmlCoreWrapper.h"
#include "logValues.h"

/**
 * @brief  callback in case of new connection request
 */
axl_bool listenerObj_connection_accept_handler (VortexConnection * conn, axlPointer ptr) {
  VORTEXLimitCheckDataCallbackData* limitCheckData = (VORTEXLimitCheckDataCallbackData*) ptr;
  /* check if connection limit was reached */

  limitCheckData->pLog->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "listenerObj_connection_accept_handler", "Vortex CMD", "vortex_connection_get_ctx");
  VortexCtx* ctx = vortex_connection_get_ctx (conn);
  limitCheckData->pLog->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "listenerObj_connection_accept_handler", "Vortex CMD", "vortex_reader_connections_watched");
  int iConnectionsWartched = vortex_reader_connections_watched (ctx);
  if (-1 != limitCheckData->iMax && iConnectionsWartched > limitCheckData->iMax)
  {
    return axl_false;
  }
  /* accept connection */
  return axl_true;
}


/**
 * @brief    Constructor.
 */
tmlListenerObj::tmlListenerObj(TML_CORE_HANDLE coreHandle, VortexCtx* ctx, const char* sNetAddress)
{
  initListenerObj(coreHandle, ctx, sNetAddress);
}


/**
 * @brief    Constructor.
 */
tmlListenerObj::tmlListenerObj(TML_CORE_HANDLE coreHandle, VortexCtx* ctx, const char* sHost, const char* sPort)
{
  int iLength = strlen(sHost) + strlen(sPort) + 2;

  char* sNetAddress = new char[iLength];
  #if defined(LINUX) || defined (MINGW_BUILD)
    sprintf(sNetAddress, "%s:%s", sHost, sPort);
  #else // LINUX
    sprintf_s(sNetAddress, iLength, "%s:%s", sHost, sPort);
  #endif // LINUX

  initListenerObj(coreHandle, ctx, sNetAddress);

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
void tmlListenerObj::initListenerObj(TML_CORE_HANDLE coreHandle, VortexCtx* ctx, const char* sNetAddress){
  m_ctx = ctx;

  m_bListnerIsEnabled = TML_FALSE;

  m_coreHandle = coreHandle;

  m_vortexConnection = NULL;

  m_binding = new tmlNetBinding(sNetAddress);

  tmlLogHandler* log =  ((tmlCoreWrapper*)m_coreHandle)->getLogHandler();
  m_connectionsLimitCheckData.iMax = -1;
  m_connectionsLimitCheckData.pLog = log;

  m_iRefCounter = 1;

  m_iErr = TML_SUCCESS;
}


/**
 * @brief    Cleans up refCounter dependent allocations.
 */
void tmlListenerObj::cleanUp(){
  if (getRef()){
    if (decRef() == 0){
      set_Enabled(TML_FALSE);
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
    tmlLogHandler* log =  ((tmlCoreWrapper*)m_coreHandle)->getLogHandler();
    if (!m_bListnerIsEnabled){
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
        // create a vortex listener:
        if (bIsIPV6){
          log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_listener_new");
          m_vortexConnection = vortex_listener_new6 (m_ctx, sHost, sPort, NULL, NULL);
        }
        else{
          log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_listener_new");
          m_vortexConnection = vortex_listener_new (m_ctx, sHost, sPort, NULL, NULL);
        }
        // Check the vortex listener:
        log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_connection_is_ok");
        if (! vortex_connection_is_ok (m_vortexConnection, axl_false)) {
          log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_connection_get_status");
          VortexStatus status = vortex_connection_get_status (m_vortexConnection);
          log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_connection_get_message");
          const char* msg = vortex_connection_get_message (m_vortexConnection);
          log->log ("tmlListenerObj", "tmlListenerObj:set_Enabled", "ERROR: failed to start listener, error msg", msg);
          // Error / unable to create the listener:
          log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_listener_shutdown");
          vortex_listener_shutdown(m_vortexConnection, axl_true);
          m_vortexConnection = NULL;
          if (VortexBindError == status){
            iRet = TML_ERR_LISTENER_ADDRESS_BINDING;
          }
          else{
            iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
          }
        }
        else{
          //////////////////////////////////////////////////////////////
          // in case of port equals 0 the vortex_listener_new will find
          // the next free port, so I want to know it's identification:
          log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_connection_get_port");
          const char* resPort = vortex_connection_get_port(m_vortexConnection);
          if (NULL == resPort){
            vortex_listener_shutdown(m_vortexConnection, axl_true);
            m_vortexConnection = NULL;
            iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
          }
          else{
            if (0 != strcmp(resPort, sPort)){
              char* sHost;
              TML_BOOL bIsIPV6 = TML_FALSE;
              iRet = m_binding->getHost(&sHost);
              if (TML_SUCCESS == iRet){
                bIsIPV6 = m_binding->isIPV6();
              }
              if (TML_SUCCESS == iRet){
                delete m_binding;
                char buffer[256];
                if (bIsIPV6){
                  sprintf_s(buffer,256, "[%s]:%s", sHost, resPort);
                }
                else{
                  sprintf_s(buffer,256, "%s:%s", sHost, resPort);
                }
                m_binding = new tmlNetBinding(buffer);
              }
            }
          }
        }
        if (TML_SUCCESS == iRet){
          ///////////////////////////////////////////////////////////////////////////
          // configure connection notification callback:
          log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_listener_set_on_connection_accepted");
          vortex_listener_set_on_connection_accepted (m_ctx, listenerObj_connection_accept_handler, &m_connectionsLimitCheckData);
        }
      }
    }
    else{
      if (NULL != m_vortexConnection){
        log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_listener_shutdown");
        vortex_listener_shutdown(m_vortexConnection, axl_true);
        ///////////////////////////////////////////////////////////////////////////
        // deregister connection notification callback:
        log->log (TML_LOG_VORTEX_CMD, "tmlListenerObj", "set_Enabled", "Vortex CMD", "vortex_listener_set_on_connection_accepted");
        vortex_listener_set_on_connection_accepted (m_ctx, NULL, NULL);
         m_vortexConnection = NULL;
      }
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


/**
  * @brief   returns the last error code
  */
TML_INT32 tmlListenerObj::getLastErr(){
  return m_iErr;
}


/**
  * @brief   Get Vortex connection 
  */
VortexConnection* tmlListenerObj::getVortexConnection(){
  return m_vortexConnection;
}

