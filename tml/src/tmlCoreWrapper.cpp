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

#include "tmlObjWrapper.h"
#include "tmlCoreListener.h"
#include "tmlCoreWrapper.h"
#include "tmlDispatcher.h"
#include "tmlErrors.h"
#include "tmlCmdDefines.h"
#include "tmlCore.h"
#include "unicode.h"
#include "logValues.h"
#include "tmlConnectionManageObj.h"
#include "tmlListenerObj.h"


/**
 * @brief   Callback will be called by the listener frame receive callback method of the TMLCoreListener.
 */
bool tmlCoreWrapper::listenerCallbackMethod(void* tmlhandle)
{
  tmlObjWrapper* tmlCommandObj = (tmlObjWrapper*) tmlhandle;

  int iRet;
  ////////////////////////////////////
  // Set the core reference attribute:
  tmlCommandObj->tmlObjWrapper_Attr_Set_Core_Reference((TML_CORE_HANDLE)this);
  iRet = cmdDispatch((TML_COMMAND_HANDLE) tmlCommandObj);
  ////////////////////////////////////
  // Free the tml cmd Handle:
  TML_COMMAND_HANDLE handle = (TML_COMMAND_HANDLE)tmlhandle;
  tml_Cmd_Free(&handle);
  return TML_SUCCESS != iRet;
}

/**
 * @brief   Send a Reply.
 */
int tmlCoreWrapper::tmlCoreWrapper_SendReply (TML_COMMAND_HANDLE tmlhandle, int iRetValue, bool bRawReply){
  int iRet;

  tmlObjWrapper* tmlCommandObj = (tmlObjWrapper*) tmlhandle;
  /////////////////////////////////////////////////////////////////////////////////
  // Answere here in case of intern or extern dispatch but not in case of stream:
  //////////////////////////////////////////////////////
  // In case of TML_SUCCESS or if we have an internal cmd:
  if (TML_SUCCESS == iRetValue){
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Change Header / CMD execution  is now finished:
    tmlCommandObj->tmlObjWrapper_Header_setLogicalError(TML_SUCCESS, NULL);
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetState(TMLCOM_CSTATE_EXECUTED);
  }
  else{
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Change Header / CMD failed:
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetError(iRetValue);
    if(TML_SUCCESS == iRet){
      iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetState(TMLCOM_CSTATE_FAILED);
    }
    else{
      m_log->log ("tmlCoreWrapper", "tmlCoreWrapper_SendReply", "ERROR returned by tmlObjWrapper_Header_SetError", iRet);
    }
  }
  if (TML_SUCCESS == iRet){
    ///////////////////////////////////////////////////////////////////////////////////////////
    // The Reply:
    iRet = m_CoreListener->MessageAnswer((TML_COMMAND_HANDLE) tmlCommandObj, bRawReply, iRetValue);
    if (TML_SUCCESS != iRet){
      m_log->log ("tmlCoreWrapper", "tmlCoreWrapper_SendReply", "ERROR returned by tmlCoreWrapper_SendReply / maybe the channel is being closed by the client", iRet);
    }
  }
  else{
    m_log->log ("tmlCoreWrapper", "tmlCoreWrapper_SendReply", "ERROR returned by tmlObjWrapper_Header_SetState", iRet);
  }
  return iRet;
}


/**
 * @brief    Get the Vortex execution context
 */
VortexCtx* tmlCoreWrapper::getVortexCtx(){
  return m_ctx;
}



/**
  * @brief    Get the loghandler
  */
tmlLogHandler* tmlCoreWrapper::getLogHandler(){
  return m_log;
}

     
/**
 * @brief    The initialization called by the constructor
 */
void tmlCoreWrapper::initWrapper(int iLogValue, TML_INT32 iInitialThreadPoolSize, TML_INT32 iThreadPoolMaxSize, 
                                          TML_INT32 iThreadAddSteps, TML_INT32 iThreadPoolAddPeriod, 
                                          TML_INT32 iThreadRemoveSteps, TML_INT32 iThreadPoolRemovePeriod, 
                                          TML_BOOL bThreadAutoRemove, TML_BOOL bThreadPreemptive)
{
  ////////////////////////////////
  // list containing the connection manager objects
  m_connectionMgrObjs = sidex_Variant_New_List();
  ////////////////////////////////
  // The debug log handler
  m_log = new tmlLogHandler();
  ////////////////////////////////
  // Get the log handler's file index that can be used to log contiguous:
  m_iLogFileIndex= m_log->getLogFileIndex();
  m_log->setLoggingValue(iLogValue);

  
  m_csObj = new tmlCriticalSectionObj();
  //m_ctx = ctx;
  ///////////////////////////////////////////////////////////////////
  // The Vortex ececution context will be created in the DllMain now
  //
  ////////////////////////////////
  // create vortex execution context
  m_log->log (TML_LOG_VORTEX_CMD, "tmlCoreWrapper", "initWrapper", "Vortex CMD", "vortex_ctx_new");
  m_ctx = vortex_ctx_new ();

  ////////////////////////////////
  // use a greater default thread pool:
  m_log->log (TML_LOG_VORTEX_CMD, "tmlCoreWrapper", "initWrapper", "Vortex CMD:vortex_thread_pool_set_num", iInitialThreadPoolSize);

  vortex_thread_pool_set_num (iInitialThreadPoolSize);
  
  ////////////////////////////////
  // init vortex library
  axl_bool bSuccess;
  m_log->log (TML_LOG_VORTEX_CMD, "tmlCoreWrapper", "initWrapper", "Vortex CMD", "vortex_init_ctx");
  bSuccess = vortex_init_ctx (m_ctx);

  if (axl_true != bSuccess){
    m_log->log ("tmlCoreWrapper", "initWrapper", "vortex_init_ctx", "FAILLED");
  }

  /* enable automatic thread pool resize */
  m_log->log (TML_LOG_VORTEX_CMD, "tmlCoreWrapper", "initWrapper", "Vortex CMD", "vortex_thread_pool_setup");

  /////////////////////////////////////////
  // Maximal amount of threads is 40
  // Thread add step is 4 threads
  // Thread add period is 0 Seconds
  // Threads will not be freed automatically

  vortex_thread_pool_setup2 (m_ctx, iThreadPoolMaxSize, iThreadAddSteps, iThreadPoolAddPeriod, iThreadRemoveSteps, iThreadPoolRemovePeriod, TML_TRUE == bThreadAutoRemove, TML_TRUE == bThreadPreemptive);

  /* The next is to get rid of connection- creation timeouts if I start lots of senders immediately / stressTest */
  m_log->log (TML_LOG_VORTEX_CMD, "tmlCoreWrapper", "initWrapper", "Vortex CMD", "vortex_conf_set");
  vortex_conf_set (m_ctx, VORTEX_LISTENER_BACKLOG, 20, NULL);

  m_pHandler = new tmlProfileHandler(m_log);

  ////////////////////////////////
  // alloc tmlCollectCall
  m_sender = new tmlCollectCall(m_ctx, m_log, m_pHandler, this);
  ////////////////////////////////
  // The maximum connection fail counter for subscribed load balanced destinations:
  m_iMaxEventConnectionFails = DEFAULT_MAX_CONNECTION_FAILS;
  m_iMaxLoadBalancedConnectionFails = DEFAULT_MAX_CONNECTION_FAILS;
  m_maxEventMsgQueueLength = DEFAULT_MAX_EVENT_MSG_QUEUE_LENGTH;

  m_sender->loadBalancedSetMaxConnectionFailCount(m_iMaxEventConnectionFails);
  m_sender->eventSetMaxConnectionFailCount(m_iMaxLoadBalancedConnectionFails);

  /////////////////////////////////////////////////////////////////////////////
  // init the internal class callback method to handle a new message:
  m_ListenerCallback.SetCallback(this, &tmlCoreWrapper::listenerCallbackMethod);

  ////////////////////////////////
  // TMLCoreListener:
  m_CoreListener = new TMLCoreListener(m_log, m_ctx, m_pHandler);
  ////////////////////////////////
  // Dispatcher hash table:
  m_dispatcherHashTable = NULL;
  m_dispatcherHashTable = new TMLUniversalHashTable(m_log);
  m_dispatcherHashTable->createHashTable(true);
  ////////////////////////////////
  // sender stream objects hash table:
  m_streamHandler = new tmlStreamHandler(m_log);
  ////////////////////////////////
  // Defaults into the attributes:
  m_sListenerPort = new char[strlen(DEFAULT_LISTENER_PORT)+ 1];
  m_sListenerIP = new char[strlen(DEFAULT_LISTENER_HOST)+ 1];
  TML_INT32 iLength;
  m_sListenerPort_w = (char16_t*)UTF8toUTF16((char*)DEFAULT_LISTENER_PORT, &iLength);
  m_sListenerPort_x = UTF8toUTF32((char*)DEFAULT_LISTENER_PORT, &iLength);
  m_sListenerIP_w   = (char16_t*)UTF8toUTF16((char*)DEFAULT_LISTENER_HOST, &iLength);
  m_sListenerIP_x   = UTF8toUTF32((char*)DEFAULT_LISTENER_HOST, &iLength);
#if defined (LINUX) || defined (MINGW_BUILD)
  strcpy(m_sListenerPort, DEFAULT_LISTENER_PORT);
  strcpy(m_sListenerIP, DEFAULT_LISTENER_HOST);
#else
  strcpy_s(m_sListenerPort, strlen(DEFAULT_LISTENER_PORT)+ 1, DEFAULT_LISTENER_PORT);
  strcpy_s(m_sListenerIP, strlen(DEFAULT_LISTENER_HOST)+ 1, DEFAULT_LISTENER_HOST);
#endif
  m_bListnerIsEnabled = DEFAULT_LISTENER_IS_ENABLED;
  m_iWindowSize = DEFAULT_MSG_WINDOW_SIZE;
  m_iConnectionTimeout = DEFAULT_CONNECTION_TIMEOUT;
}

/**
 * @brief    This method handles internal commands e.g. subscription requeste
 */
int tmlCoreWrapper::cmdDispatch(TML_COMMAND_HANDLE tmlhandle){
  int iRet = TML_SUCCESS;

  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
  ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle, (char*)"tmlCoreWrapper::tmlCoreWrapper::cmdDispatch");

  TML_BOOL bCheckInternal  = TML_FALSE;
  TML_BOOL bEvent          = TML_FALSE;
  TML_COMMAND_ID_TYPE iCmd = 0;
  //////////////////////////////////////////////////////////////////////////////////
  // Check for an internal CMD / don't mind of the result because it may not be set:
  sidex_Boolean_Read (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, &bCheckInternal);
  sidex_Boolean_Read (sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_IS_EVENT, &bEvent);
  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::tmlCoreWrapper::cmdDispatch");
  ////////////////////////////////////////////////////////
  // Get the Command ID:
  iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetCommand(&iCmd);

  if (TML_SUCCESS == iRet){
    if (TML_FALSE != bCheckInternal){
      /////////////////////////////////////////////////////////////////
      // It's an internal cmd:
      iRet = internalCmdDispatch(tmlhandle, iCmd);
    }
    else{
      /////////////////////////////////////////////////////////////////
      // It's not an internal cmd so
      // let the dispatcher / registered callback change the content:
      iRet = registeredCmdDispatch(tmlhandle, iCmd, bEvent);
    }
  }
  return iRet;
}


/**
 * @brief   Dispatch of registered dispatch commands.
 */
int tmlCoreWrapper::registeredCmdDispatch(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE iCmd, TML_BOOL bEvent){
  int iRet = TML_SUCCESS;

  const char* profile;
  iRet = ((tmlObjWrapper*) tmlhandle)->tmlObjWrapper_Attr_Get_Profile_A(&profile);
  if (TML_SUCCESS == iRet){
    tmlDispatcher* pDispatcher = NULL;
    iRet = m_dispatcherHashTable->getValue((char*)profile, (void**) &pDispatcher);
    if (TML_SUCCESS == iRet){
      if (NULL == pDispatcher){
        iRet = TML_ERR_DISPATCHER_NOT_CREATED;
      }
      else{
        iRet = pDispatcher->tml_Dispatcher_Dispatch_Cmd(tmlhandle, iCmd);
      }
    }
  }
  if (TML_SUCCESS != iRet){
    m_log->log ("tmlCoreWrapper", "registeredCmdDispatch", "ERROR returned by registeredCmdDispatch", iRet);
  }
  if (!bEvent){
    if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
      iRet = tmlCoreWrapper_SendReply (tmlhandle, iRet, false);
      if (TML_SUCCESS != iRet){
        m_log->log ("tmlCoreWrapper", "registeredCmdDispatch", "ERROR returned by tmlCoreWrapper_SendReply", iRet);
      }
    }
    else{
      m_log->log ("tmlCoreWrapper", "registeredCmdDispatch", "ERROR tmlCoreWrapper_IsAccessible", "false");
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////
  // Now it's time to free memory:
  if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
    iRet = m_CoreListener->MessageFinalize(tmlhandle);
    if (TML_SUCCESS != iRet){
      m_log->log ("tmlCoreWrapper", "registeredCmdDispatch", "ERROR returned by m_CoreListener->MessageFinalize", iRet);
    }
  }
  else{
    m_log->log ("tmlCoreWrapper", "registeredCmdDispatch", "ERROR tmlCoreWrapper_IsAccessible2", "false");
  }
  return iRet;
}


/**
 * @brief    This method handles internal commands e.g. subscription request
 */
int tmlCoreWrapper::internalCmdDispatch(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE iCmd){
  TML_INT32 iRet = TML_SUCCESS;

  char* sProfile = NULL;
  char* sHost = NULL;
  char* sPort = NULL;
  TML_INT32 iLength = 0;
  bool bRawDispatch = false;

  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  bool bAcquired = true;
  ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle, (char*)"tmlCoreWrapper::internalCmdDispatch");
  TML_BOOL bSubscribe = TML_TRUE;
  void* pCBFunc = NULL;
  TML_POINTER pCBData = NULL;
  switch (iCmd){
      case CMD_INTERNAL_STREAM_WRITE:
              m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_WRITE");
              m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_WRITE");
              iRet = m_streamHandler->handle_write(sHandle);
            break;
      case CMD_INTERNAL_STREAM_CLOSE:
            {
              m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_CLOSE");
              m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_CLOSE");
              iRet = m_streamHandler->handle_close(sHandle);
            }
            break;
      case CMD_INTERNAL_STREAM_GET_SIZE:
            {
              m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_GET_SIZE");
              m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_GET_SIZE");
              TML_INT64 iSize = 0;
              iRet = m_streamHandler->handle_getSize(sHandle, &iSize);
            }
            break;
      case CMD_INTERNAL_STREAM_SEEK:
            {
              m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_GET_SIZE");
              m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_GET_SIZE");
              TML_INT64 seekPosition = 0;
              if (TML_SUCCESS == iRet){
                iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_SEEK_POSITION, &seekPosition);
              }
              TML_INT64 seekOrigin = 0;
              if (TML_SUCCESS == iRet){
                iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_SEEK_ORIGIN, &seekOrigin);
              }
              if (TML_SUCCESS == iRet){
                iRet = m_streamHandler->handle_seek(sHandle, seekPosition, seekOrigin);
              }
            }
            break;
      case CMD_INTERNAL_STREAM_GET_POSITION:
            m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_GET_POSITION");
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_GET_POSITION");
            iRet = m_streamHandler->handle_getPosition(sHandle);
            break;
      case CMD_INTERNAL_STREAM_GET_LAST_ERR:
            m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_GET_LAST_ERR");
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_GET_LAST_ERR");
            iRet = m_streamHandler->handle_getLastErr(sHandle);
            break;
      case CMD_INTERNAL_STREAM_CANCEL_DOWNLOAD:
            m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_CANCEL_DOWNLOAD");
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_CANCEL_DOWNLOAD");
            iRet = m_streamHandler->handle_cancelStreamDownload(sHandle);
            break;
      case CMD_INTERNAL_LOAD_BALANCED_BUSY_STATE_REQUEST:
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_LOAD_BALANCED_BUSY_STATE_REQUEST");
            iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, &sProfile, &iLength);
            if (TML_SUCCESS == iRet){
              iRet = m_sender->getRegisteredBusyStatusCallback(sProfile, (TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC*)&pCBFunc, &pCBData);
              ///////////////////////////////////////////////
              // Now I can release the critical section use: 
              bAcquired = false;
              ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::internalCmdDispatch");
              if (NULL != pCBFunc){
                iRet = ((TML_INT32(FUNC_C_DECL *)(TML_COMMAND_HANDLE, TML_POINTER))pCBFunc)(tmlhandle, pCBData);
              }
            }
            break;
      case CMD_INTERNAL_EVENT_SUBSCRIPTION_REQUEST:
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_EVENT_SUBSCRIPTION_REQUEST");
            iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, &sProfile, &iLength);
            if (TML_SUCCESS == iRet){
              iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_HOST, &sHost, &iLength);
            }
            if (TML_SUCCESS == iRet){
              iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PORT, &sPort, &iLength);
            }
            if (TML_SUCCESS == iRet){
              // Don't mind of the return value because it may be 51 in case of a non registered callback:
              tmlUnicodeID iUnicode;
              m_sender->getRegisteredEventOnPeerCallback(sProfile, (TML_ON_PEER_REGISTRATION_CB_FUNC*)&pCBFunc, &pCBData, &iUnicode);
              ///////////////////////////////////////////////
              // Now I can release the critical section use: 
              bAcquired = false;
              ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::internalCmdDispatch");
              if (NULL != pCBFunc){
                switch (iUnicode){
                  case idUNICODE_ASCII:
                        bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, char*, char*, TML_POINTER))pCBFunc)(TML_TRUE, sHost, sPort, pCBData);
                        break;
                  case idUNICODE_WCHAR_T:
                        {
                          iRet = TML_ERR_UNICODE;
                          TML_INT32 iUTF16Length;
                          wchar_t* utf16host = UTF8toUTF32(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            wchar_t* utf16port = UTF8toUTF32(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, wchar_t*, wchar_t*, TML_POINTER))pCBFunc)(TML_TRUE, utf16host, utf16port, pCBData);
                              delete[] utf16port;
                              iRet = TML_SUCCESS;
                            }
                            delete[] utf16host;
                          }
                        }
                        break;
                  case idUNICODE_CHAR16_T:
                        {
                          iRet = TML_ERR_UNICODE;
                          TML_INT32 iUTF16Length;
                          char16_t* utf16host = (char16_t*)UTF8toUTF16(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            char16_t* utf16port = (char16_t*)UTF8toUTF16(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, char16_t*, char16_t*, TML_POINTER))pCBFunc)(TML_TRUE, utf16host, utf16port, pCBData);
                              delete[] utf16port;
                              iRet = TML_SUCCESS;
                            }
                            delete[] utf16host;
                          }
                        }
                        break;
                }
              }
            }
            if (TML_SUCCESS == iRet){
              if (TML_FALSE != bSubscribe){
                if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
                  iRet = tmlCoreWrapper_EventSubscribeMessageDestination(sProfile, sHost, sPort);
                }
              }
            }
            break;
      case CMD_INTERNAL_EVENT_UNSUBSCRIPTION_REQUEST:
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_EVENT_UNSUBSCRIPTION_REQUEST");
            iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, &sProfile, &iLength);
            if (TML_SUCCESS == iRet){
              iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_HOST, &sHost, &iLength);
            }
            if (TML_SUCCESS == iRet){
              iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PORT, &sPort, &iLength);
            }
            if (TML_SUCCESS == iRet){
              // Don't mind of the return value because it may be 51 in case of a non registered callback:
              tmlUnicodeID iUnicode;
              m_sender->getRegisteredEventOnPeerCallback(sProfile, (TML_ON_PEER_REGISTRATION_CB_FUNC*)&pCBFunc, &pCBData, &iUnicode);
              ///////////////////////////////////////////////
              // Now I can release the critical section use: 
              bAcquired = false;
              ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::internalCmdDispatch");
              if (NULL != pCBFunc){
                switch (iUnicode){
                  case idUNICODE_ASCII:
                        bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, char*, char*, TML_POINTER))pCBFunc)(TML_FALSE, sHost, sPort, pCBData);
                        break;
                  case idUNICODE_WCHAR_T:
                        {
                          iRet = TML_ERR_UNICODE;
                          TML_INT32 iUTF16Length;
                          wchar_t* utf16host = UTF8toUTF32(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            wchar_t* utf16port = UTF8toUTF32(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, wchar_t*, wchar_t*, TML_POINTER))pCBFunc)(TML_FALSE, utf16host, utf16port, pCBData);
                              delete[] utf16port;
                              iRet = TML_SUCCESS;
                            }
                            delete[] utf16host;
                          }
                        }
                        break;
                  case idUNICODE_CHAR16_T:
                        {
                          iRet = TML_ERR_UNICODE;
                          TML_INT32 iUTF16Length;
                          char16_t* utf16host = (char16_t*)UTF8toUTF16(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            char16_t* utf16port = (char16_t*)UTF8toUTF16(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, char16_t*, char16_t*, TML_POINTER))pCBFunc)(TML_FALSE, utf16host, utf16port, pCBData);
                              delete[] utf16port;
                              iRet = TML_SUCCESS;
                            }
                            delete[] utf16host;
                          }
                        }
                        break;
                }
              }
            }
            if (TML_SUCCESS == iRet){
              if (TML_FALSE != bSubscribe){
                if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
                  iRet = tmlCoreWrapper_EventUnsubscribeMessageDestination(sProfile, sHost, sPort);
                }
              }
            }
            break;
      case CMD_INTERNAL_LOAD_BALANCED_SUBSCRIPTION_REQUEST:
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_LOAD_BALANCED_SUBSCRIPTION_REQUEST");
            iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, &sProfile, &iLength);
            if (TML_SUCCESS == iRet){
              iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_HOST, &sHost, &iLength);
            }
            if (TML_SUCCESS == iRet){
              iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PORT, &sPort, &iLength);
            }
            if (TML_SUCCESS == iRet){
              // Don't mind of the return value because it may be 51 in case of a non registered callback:
              tmlUnicodeID iUnicode;
              m_sender->getRegisteredLoadBalancedOnPeerCallback(sProfile, (TML_ON_PEER_REGISTRATION_CB_FUNC*)&pCBFunc, &pCBData, &iUnicode);
              ///////////////////////////////////////////////
              // Now I can release the critical section use: 
              bAcquired = false;
              ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::internalCmdDispatch");
              if (NULL != pCBFunc){
                switch (iUnicode){
                  case idUNICODE_ASCII:
                        bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, char*, char*, TML_POINTER))pCBFunc)(TML_TRUE, sHost, sPort, pCBData);
                        break;
                  case idUNICODE_WCHAR_T:
                        {
                          iRet = TML_ERR_UNICODE;
                          TML_INT32 iUTF16Length;
                          wchar_t* utf16host = UTF8toUTF32(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            wchar_t* utf16port = UTF8toUTF32(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, wchar_t*, wchar_t*, TML_POINTER))pCBFunc)(TML_TRUE, utf16host, utf16port, pCBData);
                              iRet = TML_SUCCESS;
                              delete[] utf16port;
                            }
                            delete[] utf16host;
                          }
                        }
                        break;
                  case idUNICODE_CHAR16_T:
                        {
                          iRet = TML_ERR_UNICODE;
                          TML_INT32 iUTF16Length;
                          char16_t* utf16host = (char16_t*)UTF8toUTF16(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            char16_t* utf16port = (char16_t*)UTF8toUTF16(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, char16_t*, char16_t*, TML_POINTER))pCBFunc)(TML_TRUE, utf16host, utf16port, pCBData);
                              iRet = TML_SUCCESS;
                              delete[] utf16port;
                            }
                            delete[] utf16host;
                          }
                        }
                        break;
                }
              }
            }
            if (TML_SUCCESS == iRet){
              if (TML_FALSE != bSubscribe){
                if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
                  iRet = tmlCoreWrapper_LoadBalancedSubscribeMessageDestination(sProfile, sHost, sPort);
                }
              }
            }
            break;
      case CMD_INTERNAL_LOAD_BALANCED_UNSUBSCRIPTION_REQUEST:
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "internalCmdDispatch", "Got CMD", "CMD_INTERNAL_LOAD_BALANCED_UNSUBSCRIPTION_REQUEST");
            iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, &sProfile, &iLength);
            if (TML_SUCCESS == iRet){
              iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_HOST, &sHost, &iLength);
            }
            if (TML_SUCCESS == iRet){
              iRet = sidex_String_Read(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PORT, &sPort, &iLength);
            }
            if (TML_SUCCESS == iRet){
              // Don't mind of the return value because it may be 51 in case of a non registered callback:
              tmlUnicodeID iUnicode;
              m_sender->getRegisteredLoadBalancedOnPeerCallback(sProfile, (TML_ON_PEER_REGISTRATION_CB_FUNC*)&pCBFunc, &pCBData, &iUnicode);
              ///////////////////////////////////////////////
              // Now I can release the critical section use: 
              bAcquired = false;
              ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::internalCmdDispatch");
              if (NULL != pCBFunc){
                switch (iUnicode){
                  case idUNICODE_ASCII:
                        bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, char*, char*, TML_POINTER))pCBFunc)(TML_FALSE, sHost, sPort, pCBData);
                        break;
                  case idUNICODE_WCHAR_T:
                        {
                          iRet = TML_ERR_UNICODE;
                          TML_INT32 iUTF16Length;
                          wchar_t* utf16host = UTF8toUTF32(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            wchar_t* utf16port = UTF8toUTF32(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, wchar_t*, wchar_t*, TML_POINTER))pCBFunc)(TML_FALSE, utf16host, utf16port, pCBData);
                              delete[] utf16port;
                              iRet = TML_SUCCESS;
                            }
                            delete[] utf16host;
                          }
                        }
                        break;
                  case idUNICODE_CHAR16_T:
                        {
                          iRet = TML_ERR_UNICODE;
                          TML_INT32 iUTF16Length;
                          char16_t* utf16host = (char16_t*)UTF8toUTF16(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            char16_t* utf16port = (char16_t*)UTF8toUTF16(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              bSubscribe = ((TML_BOOL(FUNC_C_DECL *)(TML_BOOL, char16_t*, char16_t*, TML_POINTER))pCBFunc)(TML_FALSE, utf16host, utf16port, pCBData);
                              delete[] utf16port;
                              iRet = TML_SUCCESS;
                            }
                            delete[] utf16host;
                          }
                        }
                        break;
                }
              }
            }
            if (TML_SUCCESS == iRet){
              if (TML_FALSE != bSubscribe){
                if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
                  iRet = tmlCoreWrapper_LoadBalancedUnsubscribeMessageDestination(sProfile, sHost, sPort);
                }
              }
            }
            break;
      case CMD_INTERNAL_STREAM_READ:
      case CMD_INTERNAL_STREAM_DOWNLOAD:
            ///////////////////////////////////////////////
            // Now I can release the critical section use: 
            bAcquired = false;
            ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::internalCmdDispatch");
            iRet = rawCmdDispatch(tmlhandle, sHandle, iCmd);
            ///////////////////////////////////////////////////////////////////////////////////////////
            // Finish with command execution:
            bRawDispatch = true;
            break;
      default:
            m_log->log ("tmlCoreWrapper", "internalCmdDispatch", "ERROR Unexpected internal command", iCmd);
            break;
  }
  if (bAcquired){
    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    bAcquired = false;
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::internalCmdDispatch");
  }
  ///////////////////////////////////////////////////////////////////////////////////////////
  // If it's not a raw dispatch I have to send the reply:
  if (!bRawDispatch){
    if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
      iRet = tmlCoreWrapper_SendReply (tmlhandle, (int)iRet, false);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Now it's time to free memory:
    if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
      iRet = m_CoreListener->MessageFinalize(tmlhandle);
      if (TML_SUCCESS != iRet){
        m_log->log ("tmlCoreWrapper", "internalCmdDispatch", "ERROR returned by m_CoreListener->MessageFinalize", iRet);
      }
    }
  }
  /////////////////
  // Free Memory:
  if (NULL != sProfile)
    sidex_Free_ReadString(sProfile);
  if (NULL != sHost)
    sidex_Free_ReadString(sHost);
  if (NULL != sPort)
    sidex_Free_ReadString(sPort);
  return (int)iRet;
}


/**
 * @brief    This method handles internal commands dispatch actions with raw wata reply (stream)
 */
int tmlCoreWrapper::rawCmdDispatch(TML_COMMAND_HANDLE tmlhandle, SIDEX_HANDLE sHandle, TML_COMMAND_ID_TYPE iCmd){
  int iRet = TML_SUCCESS;
  TML_INT32  bytesRead = 0;
  TML_STREAM_ID_TYPE  iStreamID;
  switch (iCmd){
      case CMD_INTERNAL_STREAM_READ:
            m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "rawCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_READ");
            m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "rawCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_READ");
            iRet = m_streamHandler->handle_readPreparation(sHandle, &iStreamID);
            if (TML_SUCCESS == iRet){
              iRet = m_streamHandler->handle_read(sHandle, &bytesRead);
            }
            if (TML_SUCCESS == iRet && TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
              iRet = tmlCoreWrapper_SendReply (tmlhandle, iRet, true);
            }
            ///////////////////////////////////////////////////////////////////////////////////////////
            // Free allocated memory in the handle:
            m_streamHandler->handle_Free(sHandle);
            ///////////////////////////////////////////////////////////////////////////////////////////
            // Now it's time to free memory:
            if (TML_SUCCESS == iRet && TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
              iRet = m_CoreListener->MessageFinalize(tmlhandle);
            }
            if (TML_SUCCESS != iRet){
              m_log->log ("tmlCoreWrapper", "internalCmdDispatch", "ERROR returned by m_CoreListener->MessageFinalize", iRet);
            }
            break;
      case CMD_INTERNAL_STREAM_DOWNLOAD:
            {
              m_log->log (TML_LOG_STREAM_HANDLING, "tmlCoreWrapper", "rawCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_DOWNLOAD");
              m_log->log (TML_LOG_INTERNAL_DISPATCH, "tmlCoreWrapper", "rawCmdDispatch", "Got CMD", "CMD_INTERNAL_STREAM_DOWNLOAD");
              ///////////////////////////////
              // Init download & get the size:
              iRet = m_streamHandler->handle_readPreparation(sHandle, &iStreamID);
              VortexPayloadFeeder* feeder = NULL;
              if (TML_SUCCESS == iRet){
                VortexPayloadStreamFeederData* data = NULL;
                ////////////////////////////////////////
                // Communication data struct:
                data = new VortexPayloadStreamFeederData;
                iRet = m_streamHandler->handle_VortexFeederDownload (m_ctx, iStreamID, &feeder, data);
                if (TML_SUCCESS == iRet){
                  if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
                    iRet = m_CoreListener->SendRawAnswerReplyFromFeeder(tmlhandle, feeder);
                    if (TML_SUCCESS != iRet){
                      ///////////////////////////////////////////////////////
                      // I have to delete the structure in case of an error:
                      delete data;
                    }
                  }
                }
                else{
                  ///////////////////////////////////////////////////////
                  // I have to delete the structure in case of an error:
                  delete data;
                }
              }
              if (TML_SUCCESS != iRet){
                /////////////////////////////////////////////////////////////////////////////////////////
                // Error in getSize or getPosition / will be indicated by a bytesRead of 0:
                if (TML_SUCCESS == tmlCoreWrapper_IsAccessible ()){
                /*int iRet2 = */ m_CoreListener->MessageFinalize(tmlhandle);
                }
                ////////////////////////////////////////////////////////////
                // set everything to be ok afterwards:
                iRet = TML_SUCCESS;
              }
              if (TML_SUCCESS != iRet){
                m_log->log ("tmlCoreWrapper", "rawCmdDispatch", "ERROR returned by m_CoreListener->MessageFinalize", iRet);
              }
            }
            break;
      default:
            m_log->log ("tmlCoreWrapper", "rawCmdDispatch", "ERROR Unexpected internal command", iCmd);
            break;
  }
  return iRet;
}


/**
 * @brief    Constructor.
 */
tmlCoreWrapper::tmlCoreWrapper(int iLogValue,
                                TML_INT32 iInitialThreadPoolSize, TML_INT32 iThreadPoolMaxSize, 
                                                                  TML_INT32 iThreadAddSteps, TML_INT32 iThreadPoolAddPeriod,
                                                                  TML_INT32 iThreadRemoveSteps, TML_INT32 iThreadPoolRemovePeriod, 
                                                                  TML_BOOL bThreadAutoRemove, TML_BOOL bThreadPreemptive)
{
  m_bShutdown = false;
  initWrapper(iLogValue, iInitialThreadPoolSize, iThreadPoolMaxSize, iThreadAddSteps, iThreadPoolAddPeriod, 
                                                                     iThreadRemoveSteps, iThreadPoolRemovePeriod, 
                                                                     bThreadAutoRemove, bThreadPreemptive);

}


/**
 * @brief    make all deregistrations / sometimes useful before calling the destructor.
 */
void tmlCoreWrapper::tmlCoreWrapper_General_Deregistration()
{
  m_sender->DeregisterConnectionLost();
  ////////////////////////////////////////////////////////////////
  // Disable the listener:
  tmlCoreWrapper_Enable_Listener(false);

  ////////////////////////////////////////////////////////////////
  // Unregister all dispatch callback methods:
  TML_INT32 iSize = 0;
  TML_INT32 iRet = tmlCoreWrapper_Get_Registered_Profiles_Size(&iSize);

  SIDEX_VARIANT registeredProfiles = SIDEX_HANDLE_TYPE_NULL;
  iRet = tmlCoreWrapper_Get_Registered_Profiles(&registeredProfiles);

  if (TML_SUCCESS == iRet){
    for (int k = 0; k < iSize; ++k){
      SIDEX_VARIANT vSingleKey;
      iRet = sidex_Variant_List_Get(registeredProfiles, k, &vSingleKey);
      if (SIDEX_SUCCESS == iRet){
        SIDEX_INT32 iLength;
        char* sSingleKey;
        iRet = sidex_Variant_As_String(vSingleKey, &sSingleKey, &iLength);
        if (SIDEX_SUCCESS == iRet){
          iRet = tmlCoreWrapper_Unregister_Profile (sSingleKey);
        }
      }
    }
    if (SIDEX_HANDLE_TYPE_NULL != registeredProfiles){
      sidex_Variant_DecRef(registeredProfiles);
    }
  }
  // Finish with unregister all dispatch callback methods
  ////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////
  // Event Callbacks:
  m_sender->eventDeregisterAll();
  ////////////////////////////////////////////////////////////////
  // Load Balanced Callbacks:
  m_sender->loadBalancedDeregisterAll();
}


/**
 * @brief    Destructor.
 */
tmlCoreWrapper::~tmlCoreWrapper()
{
  tmlCoreWrapper_General_Deregistration();
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Set contiguous log file index to make logging the right file in the destructor of the sender possible:
  m_sender->setLogFileIndex(m_iLogFileIndex);
  ////////////////////////////////////////////////////////////////////////////////////////////
  // free tmlCollectCall / to get rid of ctx references before vortex_exit_ctx
  delete (m_sender);
  m_sender = NULL;
  ////////////////////////////////
  // unregister profiles:
  unregisterAll_Registered_Profiles();
  ////////////////////////////////////////////////////////////////////////////////////////////
  // free listener / to get rid of ctx references before vortex_exit_ctx
  delete (m_CoreListener);
  m_CoreListener = NULL;

  ////////////////////////////////
  // destruct the connection manager objects
  tmlCoreWrapper_Connection_CloseAll();
  sidex_Variant_DecRef(m_connectionMgrObjs);


#ifdef LINUX
  #ifdef OS_X
    m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "initSender", "Vortex CMD", "vortex_conf_set");
    vortex_conf_set (m_ctx, VORTEX_SKIP_THREAD_POOL_WAIT, axl_true, NULL);
  #else // OSX
  #endif // OSX
#else // LINUX
#endif // LINUX
  // now call to exit
  m_log->log (TML_LOG_VORTEX_CMD, "tmlCoreWrapper", "~tmlCoreWrapper", "Vortex CMD", "vortex_exit_ctx");
  vortex_exit_ctx (m_ctx, axl_true);


///////////////////////////////////////
// To debug m_ctx ref counting:
// print and release ctx
/*
  gettimeofday (&now, NULL);
printf ("Reference counting after exiting is: %d (stamp: %d.%d)\n", 
        vortex_ctx_ref_count (m_ctx), (int) now.tv_sec, (int) now.tv_usec); 
vortex_ctx_unref (&m_ctx);
*/
//
///////////////////////////////////////
  m_ctx = NULL;

  ////////////////////////////////
  // Dispatcher hash table:
  delete (m_dispatcherHashTable);
  m_dispatcherHashTable = NULL;
  
  ////////////////////////////////
  // attributes:
  delete[] m_sListenerIP;
  m_sListenerIP = NULL;
  delete[] m_sListenerIP_w;
  m_sListenerIP_w = NULL;
  delete[] m_sListenerIP_x;
  m_sListenerIP_x = NULL;
  delete[] m_sListenerPort;
  m_sListenerPort = NULL;
  delete[] m_sListenerPort_w;
  m_sListenerPort_w = NULL;
  delete[] m_sListenerPort_x;
  m_sListenerPort_x = NULL;

  ////////////////////////////////
  // Profile Handler:
  delete (m_pHandler);

  delete (m_streamHandler);
  ////////////////////////////////
  // The debug log handler
  delete (m_log);
  ////////////////////////////////
  // Critical section object
  delete (m_csObj);
}


/**
 * @brief   Set the callback method for thread creation.
 */
int tmlCoreWrapper::tmlCoreWrapper_Thread_Set_OnCreate(void *pCBCreate){
  vortex_thread_set_create ((VortexThreadCreateFunc)pCBCreate);
  return TML_SUCCESS;
}


/**
 * @brief   Set the callback method for thread destruction.
 */
int tmlCoreWrapper::tmlCoreWrapper_Thread_Set_OnDestroy(void *pCBDestroy){
  vortex_thread_set_destroy ((VortexThreadDestroyFunc)pCBDestroy);
  return TML_SUCCESS;
}


/**
 * @brief   Create an instance of a command dispatcher.
 */
int tmlCoreWrapper::tmlCoreWrapper_Register_Profile(const char* profile){
  int iRet = TML_SUCCESS;

  tmlDispatcher* pDispatcher = NULL;
  iRet = m_dispatcherHashTable->getValue((char*)profile, (void**) &pDispatcher);
  if (TML_SUCCESS == iRet){
    if (NULL != pDispatcher){
      iRet = TML_ERR_DISPATCHER_ALREADY_EXISTS;
    }
    else{
      if (TML_SUCCESS == iRet){
        pDispatcher = new tmlDispatcher(m_log);
        pDispatcher->tml_Dispatcher_Create(profile);
        ////////////////////////////////////////////////////////////
        // Work with the copy because the argument may be destroyed 
        // After the call:
        char* profileCopy;
        pDispatcher->tml_Dispatcher_Get_Profile(&profileCopy);
        m_dispatcherHashTable->setValue(profileCopy, pDispatcher);
        ////////////////////////////////////////////////////////////
        // Profile registration at the listener:
        if (m_bListnerIsEnabled){
          m_CoreListener->TMLCoreListener_RegisterProfile(profileCopy, (TML_CORE_HANDLE)this);
        }
      }
    }
  }
  return iRet;
}

/**
 * @brief   Dismiss an instance of a command dispatcher.
 */
int tmlCoreWrapper::tmlCoreWrapper_Unregister_Profile(const char* profile){
  int iRet = TML_SUCCESS;

  /////////////////////////////////////////////////////////////////////
  // It should not be good to deregister delete command callback here:
  //tmlCoreWrapper_Set_OnDeleteCmd(profile, NULL, NULL);

  tmlCoreWrapper_Set_OnCustomDispatch(profile, NULL, NULL);
  tmlCoreWrapper_Set_OnQueueOverflow(profile, NULL, NULL,idUNICODE_WCHAR_T);
  tmlDispatcher* pDispatcher = NULL;
  iRet = m_dispatcherHashTable->getValue((char*)profile, (void**) &pDispatcher);
  if (TML_SUCCESS == iRet){
    if (NULL == pDispatcher){
      iRet = TML_ERR_DISPATCHER_NOT_CREATED;
    }
    else{
      ////////////////////////////////////////////
      // Profile unregistration at the listener:
      if (m_bListnerIsEnabled){
        m_CoreListener->TMLCoreListener_UnregisterProfile(profile);
      }
      ////////////////////////////////////////////
      // now I don't need the profile anymore:
      iRet = pDispatcher->tml_Dispatcher_Free();
      ////////////////////////////////////////
      // Delete the value stored in the hash:
      delete (pDispatcher);
      pDispatcher = NULL;
      ////////////////////////////////////////
      m_dispatcherHashTable->removeEntry((char*)profile);
    }
  }
  return iRet;
}


/**
 * @brief   Get the number of registered profiles for the given coreHandle.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_Registered_Profiles_Size(TML_INT32* iSize)
{
  *iSize = 0;

  int hSize;
  int iRet = m_dispatcherHashTable->hashSize(&hSize);
  *iSize = hSize;
  return iRet;
}


/**
 * @brief   Get the registered profiles.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_Registered_Profiles(SIDEX_VARIANT* profiles)
{
  *profiles = TML_HANDLE_TYPE_NULL;
  char** retProfiles = NULL;
  int iSize = 0;
  int iRet = m_dispatcherHashTable->hashSize(&iSize);
  if (TML_SUCCESS == iRet){
    if (0 < iSize){
      TML_INT32 iRet = m_dispatcherHashTable->getKeys(&retProfiles);
      if (TML_SUCCESS == iRet && NULL != retProfiles){
        SIDEX_VARIANT list = sidex_Variant_New_List();
        for (int i = 0;i < iSize && TML_SUCCESS == iRet; ++i){
          SIDEX_VARIANT profileItem;
          iRet = sidex_Variant_New_String (retProfiles[i], &profileItem);
          ////////////////////////////////////////
          // Free memory allocated for keys array
          delete retProfiles[i];
          if (TML_SUCCESS == iRet){
            TML_INT32 pos;
            sidex_Variant_List_Append (list, profileItem, &pos);
            // Decrement of the refcounter:
            sidex_Variant_DecRef (profileItem);
          }
        }
        *profiles = list;
      }
      if (NULL != retProfiles)
        ////////////////////////////////////////
        // Free memory allocated for keys array
        delete retProfiles;
    }
    else{
      iRet = TML_ERR_NOPROFILES;
    }
  }
  return iRet;
}


/**
 * @brief   Helpermethod to unregister allregistered profiles.
 */
void tmlCoreWrapper::unregisterAll_Registered_Profiles()
{
  char** retProfiles = NULL;
  int iSize = 0;
  int iRet = m_dispatcherHashTable->hashSize(&iSize);
  if (TML_SUCCESS == iRet){
    if (0 < iSize){
      TML_INT32 iRet = m_dispatcherHashTable->getKeys(&retProfiles);
      if (TML_SUCCESS == iRet && NULL != retProfiles){
        for (int i = 0;i < iSize && TML_SUCCESS == iRet; ++i){
          tmlCoreWrapper_Unregister_Profile((const char*)retProfiles[i]);
          ////////////////////////////////////////
          // Free memory allocated for keys array
          delete retProfiles[i];
        }
      }
      if (NULL != retProfiles)
        ////////////////////////////////////////
        // Free memory allocated for keys array
        delete retProfiles;
    }
  }
}


/**
 * @brief   Check if the  profile is registered.
 */
int tmlCoreWrapper::tmlCoreWrapper_Is_Profile_Registered(char* profile, bool* bRegistered){
  *bRegistered = false;
  char** retProfiles = NULL;
  int iSize = 0;
  int iRet = m_dispatcherHashTable->hashSize(&iSize);
  if (TML_SUCCESS == iRet){
    if (0 < iSize){
      int iRet = m_dispatcherHashTable->getKeys(&retProfiles);
      if (TML_SUCCESS == iRet && NULL != retProfiles){
        for (int i = 0;i < iSize; ++i){
          if (strcmp (profile, retProfiles[i]) == 0){
            *bRegistered = true;
          }
          delete retProfiles[i];
        }
      }
      if (NULL != retProfiles)
        delete retProfiles;
    }
    else{
      // Nothing to do in case of no registered profile
    }
  }
  return iRet;
}


/**
 * @brief   Register a callback method referring a command for the command dispatch process.
 */
int tmlCoreWrapper::tmlCoreWrapper_Register_Cmd(const char* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  tmlDispatcher* pDispatcher = NULL;
  iRet = m_dispatcherHashTable->getValue((char*)profile, (void**) &pDispatcher);
  if (TML_SUCCESS == iRet){
    if (NULL == pDispatcher){
      iRet = TML_ERR_DISPATCHER_NOT_CREATED;
    }
    else{
      if (TML_SUCCESS == iRet){
        if (NULL != pCBFunc){
          // Possible replace of an entry:
          iRet = pDispatcher->tml_Dispatcher_Register_Cmd(cmd, NULL, NULL);
          if (TML_SUCCESS == iRet || TML_ERR_DISPATCHER_CMD_NOT_REGISTERED == iRet)
          {
            // Now I can do my registration:
            iRet = pDispatcher->tml_Dispatcher_Register_Cmd(cmd, pCBFunc, pCBData);
          }
        }
        else{
          iRet = pDispatcher->tml_Dispatcher_Register_Cmd(cmd, NULL, NULL);
        }
      }
    }
  }
  return iRet;
}


/**
 * @brief   Register a callback method that will be invoked in the case of command deregistration out of the command dispatch process.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_OnDeleteCmd(const char* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  tmlDispatcher* pDispatcher = NULL;
  iRet = m_dispatcherHashTable->getValue((char*)profile, (void**) &pDispatcher);
  if (TML_SUCCESS == iRet){
    if (NULL == pDispatcher){
      iRet = TML_ERR_DISPATCHER_NOT_CREATED;
    }
    else{
      iRet = pDispatcher->tml_Dispatcher_Set_OnDeleteCmd(pCBFunc, pCBData);
    }
  }
  return iRet;
}


/**
 * @brief   Register a callback method for non registered dispatch commands.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_OnCustomDispatch(const char* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  tmlDispatcher* pDispatcher = NULL;
  iRet = m_dispatcherHashTable->getValue((char*)profile, (void**) &pDispatcher);
  if (TML_SUCCESS == iRet){
    if (NULL == pDispatcher){
      iRet = TML_ERR_DISPATCHER_NOT_CREATED;
    }
    else{
      iRet = pDispatcher->tml_Dispatcher_Set_OnCustomDispatch(pCBFunc, pCBData);
    }
  }
  return iRet;
}


/**
 * @brief   Set the Port number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_ListenerPort_A(char* sPort){
  int iRet = TML_SUCCESS;

  if (!m_bListnerIsEnabled){
    TML_INT32 iLength;

    delete[]  m_sListenerPort;
    delete[]  m_sListenerPort_w;
    delete[]  m_sListenerPort_x;
    m_sListenerPort_x = UTF8toUTF32(sPort, &iLength);
    m_sListenerPort = UTF32toUTF8(m_sListenerPort_x, &iLength);
    m_sListenerPort_w = (char16_t*)UTF8toUTF16(m_sListenerPort, &iLength);
  }
  else{
    iRet = TML_ERR_NOT_OPERABLE_AT_THE_MOMENT;
  }
  return iRet;
}


/**
 * @brief   Set the Port number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_ListenerPort_X(wchar_t* sPort){
  int iRet = TML_SUCCESS;

  if (!m_bListnerIsEnabled){
    TML_INT32 iLength;

    delete[]  m_sListenerPort;
    delete[]  m_sListenerPort_w;
    delete[]  m_sListenerPort_x;
    m_sListenerPort = UTF32toUTF8(sPort, &iLength);
    m_sListenerPort_w = (char16_t*)UTF8toUTF16(m_sListenerPort, &iLength);
    m_sListenerPort_x = UTF8toUTF32(m_sListenerPort, &iLength);
  }
  else{
    iRet = TML_ERR_NOT_OPERABLE_AT_THE_MOMENT;
  }
  return iRet;
}


/**
 * @brief   Set the Port number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_ListenerPort_W(char16_t* sPort){
  int iRet = TML_SUCCESS;

  if (!m_bListnerIsEnabled){
    TML_INT32 iLength;

    delete[]  m_sListenerPort;
    delete[]  m_sListenerPort_w;
    delete[]  m_sListenerPort_x;
    m_sListenerPort = UTF16toUTF8((wchar_t*)sPort, &iLength);
    m_sListenerPort_w = (char16_t*)UTF8toUTF16(m_sListenerPort, &iLength);
    m_sListenerPort_x = UTF8toUTF32(m_sListenerPort, &iLength);
  }
  else{
    iRet = TML_ERR_NOT_OPERABLE_AT_THE_MOMENT;
  }
  return iRet;
}


/**
 * @brief   Get the Port number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_ListenerPort_A(char** sPort){
  int iRet = TML_SUCCESS;

  *sPort = m_sListenerPort;
  return iRet;
}


/**
 * @brief   Get the Port number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_ListenerPort_X(wchar_t** sPort){
  int iRet = TML_SUCCESS;

  *sPort = m_sListenerPort_x;
  return iRet;
}


/**
 * @brief   Get the Port number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_ListenerPort_W(char16_t** sPort){
  int iRet = TML_SUCCESS;

  *sPort = m_sListenerPort_w;
  return iRet;
}


/**
 * @brief   Set the IP number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_ListenerIP_A(char* sIP){
  int iRet = TML_SUCCESS;

  if (!m_bListnerIsEnabled){
    TML_INT32 iLength;

    delete[] m_sListenerIP;
    delete[] m_sListenerIP_w;
    delete[] m_sListenerIP_x;
    m_sListenerIP_x = UTF8toUTF32(sIP, &iLength);
    m_sListenerIP = UTF32toUTF8(m_sListenerIP_x, &iLength);
    m_sListenerIP_w = (char16_t*)UTF8toUTF16(m_sListenerIP, &iLength);
  }
  else{
    iRet = TML_ERR_NOT_OPERABLE_AT_THE_MOMENT;
  }

  return iRet;
}


/**
 * @brief   Set the IP number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_ListenerIP_X(wchar_t* sIP){
  int iRet = TML_SUCCESS;

  if (!m_bListnerIsEnabled){
    TML_INT32 iLength;

    delete[] m_sListenerIP;
    delete[] m_sListenerIP_w;
    delete[] m_sListenerIP_x;
    m_sListenerIP = UTF32toUTF8(sIP, &iLength);
    m_sListenerIP_w = (char16_t*)UTF8toUTF16(m_sListenerIP, &iLength);
    m_sListenerIP_x = UTF8toUTF32(m_sListenerIP, &iLength);
  }
  else{
    iRet = TML_ERR_NOT_OPERABLE_AT_THE_MOMENT;
  }

  return iRet;
}


/**
 * @brief   Set the IP number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_ListenerIP_W(char16_t* sIP){
  int iRet = TML_SUCCESS;

  if (!m_bListnerIsEnabled){
    TML_INT32 iLength;

    delete[] m_sListenerIP;
    delete[] m_sListenerIP_w;
    delete[] m_sListenerIP_x;
    m_sListenerIP = UTF16toUTF8((wchar_t*)sIP, &iLength);
    m_sListenerIP_w = (char16_t*)UTF8toUTF16(m_sListenerIP, &iLength);
    m_sListenerIP_x = UTF8toUTF32(m_sListenerIP, &iLength);
  }
  else{
    iRet = TML_ERR_NOT_OPERABLE_AT_THE_MOMENT;
  }

  return iRet;
}


/**
 * @brief   Get the IP number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_ListenerIP_A(char** sIP){
  int iRet = TML_SUCCESS;

  *sIP = m_sListenerIP;
  return iRet;
}


/**
 * @brief   Get the IP number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_ListenerIP_X(wchar_t** sIP){
  int iRet = TML_SUCCESS;

  *sIP = m_sListenerIP_x;
  return iRet;
}


/**
 * @brief   Get the IP number for the listener.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_ListenerIP_W(char16_t** sIP){
  int iRet = TML_SUCCESS;

  *sIP = m_sListenerIP_w;
  return iRet;
}


/**
 * @brief   Enable / Disable the listener for incoming data.
 */
int tmlCoreWrapper::tmlCoreWrapper_Enable_Listener(bool bEnable){
  int iRet = TML_SUCCESS;
  const char* resPort;


  if (bEnable == m_bListnerIsEnabled){
    iRet = TML_SUCCESS;
  }
  else{
    //m_csObj->tmlCriticalSectionEnter();
    if (!m_bListnerIsEnabled){
      ////////////////////////////////////////
      // Enable logging:
      m_CoreListener->TMLCoreListener_Set_Vortex_Logging_Value(m_iLogValue);
      ////////////////////////////////////////////////////////////////////////////////////////////
      // Set contiguous log file index to make logging to the right file in the TMLCoreListener_Start of the sender possible:
      m_CoreListener->setLogFileIndex(m_iLogFileIndex);
      //////////////////////
      // Start the listener:
      iRet = m_CoreListener->TMLCoreListener_Start(m_sListenerIP, m_sListenerPort, &resPort, &m_ListenerCallback);

      if (TML_SUCCESS == iRet){
        //////////////////////////////////////////////////////////////
        // in case of m_sListenerPort equals 0 the vortex_listener_new will find
        // the next free port, so I want to save it's identification:
        tmlCoreWrapper_Set_ListenerPort_A((char*)resPort);
        ////////////////////////////////////////
        // Profile registration at the listener:
        int iSize = 0;
        iRet = m_dispatcherHashTable->hashSize(&iSize);
        if (TML_SUCCESS == iRet && 0 < iSize){
          char** retProfiles = NULL;
          int iRet = m_dispatcherHashTable->getKeys(&retProfiles);
          if (TML_SUCCESS == iRet && NULL != retProfiles){
            for (int i = 0;i < iSize; ++i){
              m_CoreListener->TMLCoreListener_RegisterProfile(retProfiles[i], (TML_CORE_HANDLE)this);
              delete (retProfiles[i]);
            }
            delete retProfiles;
          }
        }
      }
    }
    else{
      iRet = m_CoreListener->TMLCoreListener_Stop();
    }
    // In case of TML_SUCCESS:
    if (TML_SUCCESS == iRet)
      m_bListnerIsEnabled = bEnable;
    //m_csObj->tmlCriticalSectionLeave();
  }
  return iRet;
}


/**
 * @brief   Returns the listener enable status.
 */
int tmlCoreWrapper::tmlCoreWrapper_Is_Listener_Enabled(bool* bEnable){
  int iRet = TML_SUCCESS;

  *bEnable = m_bListnerIsEnabled;
  return iRet;
}


/**
 * @brief   Set the window size for the following outgoing data.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_WindowSize(int iWindowSize){
  int iRet = TML_SUCCESS;

  m_iWindowSize = iWindowSize;
  return iRet;
}


/**
 * @brief   Get the window size for the following outgoing data.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_WindowSize(TML_INT32* iWindowSize){
  int iRet = TML_SUCCESS;

  *iWindowSize = (TML_INT32)m_iWindowSize;
  return iRet;
}

/**
 * @brief   Enable / Disable debug logging.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_Logging_Value(int iLogValue){
  int iRet = TML_SUCCESS;

  m_log->setLoggingValue(iLogValue);
  // Maybe logging was enabled by File, so:
  iLogValue = m_log->getLoggingValue(); 
  m_iLogValue = iLogValue;

  // Log value for all instances of sender
  m_sender->setLoggingValue(iLogValue);

  if (m_bListnerIsEnabled){
    m_CoreListener->TMLCoreListener_Set_Vortex_Logging_Value(m_iLogValue);
  }
  return iRet;
}


/**
 * @brief   Returns the debug logging enable status.
 */
int tmlCoreWrapper::tmlCoreWrapper_Get_Logging_Value(TML_INT32* iLogValue){
  int iRet = TML_SUCCESS;

  *iLogValue = (TML_INT32)m_iLogValue;
  return iRet;
}


/**
 * @brief    Send a synchron Message.
 */
int tmlCoreWrapper::tmlCoreWrapper_SendSyncMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile, const char* sHost, const char* sPort, unsigned int iTimeout)
{
  int  iRet = TML_SUCCESS;
  iRet = m_sender->sender_SendSyncMessage(profile, sHost, sPort, m_iWindowSize, tmlhandle, iTimeout + m_log->getAdditionalTimeout(), NULL, true);
  return iRet;
}


/**
 * @brief    Send an asynchron Message.
 */

int tmlCoreWrapper::tmlCoreWrapper_SendAsyncMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile, const char* sHost, const char* sPort, unsigned int iTimeout, int iMode)
{
  int  iRet = TML_SUCCESS;
  iRet = m_sender->sender_SendAsyncMessage(profile, sHost, sPort, m_iWindowSize, tmlhandle, iTimeout + m_log->getAdditionalTimeout(), iMode, true, false);
  return iRet;
}


/**
 * @brief    Send an asynchronous progress reply.
 */
int tmlCoreWrapper::tmlCoreWrapper_SendAsyncProgressReply(TML_COMMAND_HANDLE tmlhandle,  TML_INT32 progress)
{
  int  iRet = TML_SUCCESS;

  m_log->log (TML_LOG_CORE_IO,  "TMLCoreWrapper", "tmlCoreWrapper_SendAsyncProgressReply", "Progress", progress);
  iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetProgress(progress);
  if (TML_SUCCESS == iRet){
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetState(TMLCOM_CSTATE_PENDING);
    if (TML_SUCCESS == iRet){
      iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetReplyType(TMLCOM_RPY_PROGRESS);
      if (TML_SUCCESS == iRet){
        iRet = m_CoreListener->MessageAnswer(tmlhandle, false, TML_SUCCESS);
      }
    }
  }
  return iRet;
}

/**
 * @brief    Send an asynchronous status reply.
 */
int tmlCoreWrapper::tmlCoreWrapper_SendAsyncStatusReply(TML_COMMAND_HANDLE tmlhandle, int iType, char* sStatus)
{
  int  iRet = TML_SUCCESS;

  ////////////////////////////
  // Check the reply type:
  switch (iType){
    case TMLCOM_RPY_WARNING:
    case TMLCOM_RPY_ERROR:
    case TMLCOM_RPY_INFORMATION:
         break;
    default:
         iRet = TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED;
         break;
  }
  m_log->log (TML_LOG_CORE_IO,  "TMLCoreWrapper", "tmlCoreWrapper_SendAsyncStatusReply", sStatus, iType);
  if (TML_SUCCESS == iRet){
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetReplyMessage(sStatus, (int)strlen(sStatus));
    if (TML_SUCCESS == iRet){
      iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetState(TMLCOM_CSTATE_PENDING);
      if (TML_SUCCESS == iRet){
        iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetReplyType(iType);
        if (TML_SUCCESS == iRet){
          iRet = m_CoreListener->MessageAnswer(tmlhandle, false, TML_SUCCESS);
        }
      }
    }
  }
  return iRet;
}


/**
 * @brief    Subscribe a destination for event messages.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventSubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort)
{
    return m_sender->eventSubscribeMessageDestination(profile, sHost, sPort);
}



/**
* @brief    Get subscribed destinations for event messages for the provided profile.
*/
int tmlCoreWrapper::tmlCoreWrapper_EventGetSubscribedMessageDestinations(const char* profile, SIDEX_VARIANT* subscriptions){
  return m_sender->eventGetSubscribedMessageDestinations(profile, subscriptions);
}


/**
 * @brief    Unsubscribe a destination for event messages.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventUnsubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort)
{
  TML_INT32 iRet = m_sender->eventUnsubscribeMessageDestination(profile, sHost, sPort);
  //////////////////////////////////////////////////
  // If Destination was not found it is not an error:
  if (TML_ERR_DESTINATION_OBJ_NOT_FOUND == iRet){
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Unsubscribe all destinations for event messages for the provided profile.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventUnsubscribeAllMessageDestinations(const char* profile)
{
  TML_INT32 iRet = m_sender->eventUnsubscribeAllMessageDestinations(profile);
  //////////////////////////////////////////////////
  // If Destination was not found it is not an error:
  if (TML_ERR_DESTINATION_OBJ_NOT_FOUND == iRet){
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Register a callback method that will be called in case of problem during event message send actions.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventRegisterCallbackOnError(const char* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_sender->eventRegisterCallbackOnError(profile, pCBFunc, pCBData, iUnicode);
}

/**
 * @brief    Register a callback method that will be called in case of event message queue overflow.
 */
int tmlCoreWrapper::tmlCoreWrapper_Set_OnQueueOverflow(const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_sender->eventRegisterCallbackOnQueueOverflow(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Register a callback method for the case of non subscribed destinations for event messages.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_sender->eventRegisterPopulateCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Send an event message.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventSendMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile)
{
  int  iRet = TML_SUCCESS;

  // I do use a default timeout to be sure that the message return anyway
  iRet = m_sender->eventSendMessage(m_iWindowSize, tmlhandle, profile, DEFAULT_EVT_CONNECTION_TIMEOUT + m_log->getAdditionalTimeout());
  return iRet;
}


/**
 * @brief    Send an event subscription request.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest(const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, unsigned int iTimeout){
  TML_INT32 iRet = TML_SUCCESS;

  tmlObjWrapper* command = new tmlObjWrapper();
 ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle, (char*)"tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest");
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Flag for internal command:
  iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, profile);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_HOST, sSourceHost);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PORT, sSourcePort);
  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  command->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest");
  if (TML_SUCCESS == iRet)
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_EVENT_SUBSCRIPTION_REQUEST);
  if (TML_SUCCESS == iRet)
    iRet = m_sender->sender_SendSyncMessage(profile, sDestHost, sDestPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, iTimeout, NULL, true);

  //////////////////////////////////////////////////////////////////////////////
  // in case of an error I have to free for the TML_COMMAND_HANDLE:
  delete (command);

  return (int)iRet;
}


/**
 * @brief    Register a callback method for the case of a peer subscription / unsubscription request for event messages.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode){
    return m_sender->eventRegisterOnPeerCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Send an event unsubscription request.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventSendUnsubscriptionRequest(const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, unsigned int iTimeout){
 TML_INT32 iRet = TML_SUCCESS;

  tmlObjWrapper* command = new tmlObjWrapper();
  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle, (char*)"tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest");
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Flag for internal command:
  iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, profile);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_HOST, sSourceHost);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PORT, sSourcePort);

  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  command->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest");
  if (TML_SUCCESS == iRet)
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_EVENT_UNSUBSCRIPTION_REQUEST);

  if (TML_SUCCESS == iRet)
    iRet = m_sender->sender_SendSyncMessage(profile, sDestHost, sDestPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, iTimeout, NULL, true);

  //////////////////////////////////////////////////////////////////////////////
  // in case of an error I have to free for the TML_COMMAND_HANDLE:
  delete (command);

  return (int)iRet;
}


/**
 * @brief    Subscribe a destination for load balanced messages.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedSubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort)
{
  return m_sender->loadBalancedSubscribeMessageDestination(profile, sHost, sPort);
}


/**
 * @brief    Get subscribed destinations for load balanced messages for the provided profile.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedGetSubscribedMessageDestinations(const char* profile, SIDEX_VARIANT* subscriptions){
  return m_sender->loadBalancedGetSubscribedMessageDestinations(profile, subscriptions);
}


/**
 * @brief    Unsubscribe a destination for load balanced messages.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedUnsubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort)
{
  TML_INT32 iRet = m_sender->loadBalancedUnsubscribeMessageDestination(profile, sHost, sPort);
  //////////////////////////////////////////////////
  // If Destination was not found it is not an error:
  if (TML_ERR_DESTINATION_OBJ_NOT_FOUND == iRet){
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Unsubscribe all destinations for load balanced messages for the provided profile.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedUnsubscribeAllMessageDestinations(const char* profile)
{
  TML_INT32 iRet = m_sender->loadBalancedUnsubscribeAllMessageDestinations(profile);
  //////////////////////////////////////////////////
  // If Destination was not found it is not an error:
  if (TML_ERR_DESTINATION_OBJ_NOT_FOUND == iRet){
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Register a callback method for the case of non subscribed destinations for load balanced messages.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_sender->loadBalancedRegisterPopulateCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Register a callback method for the BUSY STATUS requests.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedRegisterBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
    return m_sender->loadBalancedRegisterBusyStatusCallback(profile, pCBFunc, pCBData);
}


/**
 * @brief    Register a callback method for the load balance calculation.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedRegisterCalculator(const char* profile,TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
    return m_sender->loadBalancedRegisterCalculator(profile, pCBFunc, pCBData);
}


/**
 * @brief    Send a load balanced message.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedSendSyncMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout)
{
  int  iRet = TML_SUCCESS;

  iRet = m_sender->loadBalancedSendSyncMessage(m_iWindowSize, tmlhandle, profile, iTimeout + m_log->getAdditionalTimeout());
  return iRet;
}


/**
 * @brief    Send a load balanced async message.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedSendAsyncMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout)
{
  int  iRet = TML_SUCCESS;

  iRet = m_sender->loadBalancedSendAsyncMessage(m_iWindowSize, tmlhandle, profile, iTimeout + m_log->getAdditionalTimeout());
  return iRet;
}


/**
 * @brief    Send a load balanced subscription request.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedSendSubscriptionRequest(const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, unsigned int iTimeout)
{
 TML_INT32 iRet = TML_SUCCESS;

  tmlObjWrapper* command = new tmlObjWrapper();
  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle, (char*)"tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest");
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Flag for internal command:
  iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, profile);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_HOST, sSourceHost);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PORT, sSourcePort);

  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  command->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest");
  if (TML_SUCCESS == iRet)
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_LOAD_BALANCED_SUBSCRIPTION_REQUEST);

  if (TML_SUCCESS == iRet)
    iRet = m_sender->sender_SendSyncMessage(profile, sDestHost, sDestPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, iTimeout, NULL, true);

  //////////////////////////////////////////////////////////////////////////////
  // in case of an error I have to free for the TML_COMMAND_HANDLE:
  delete (command);

  return (int)iRet;
}


/**
 * @brief    Register a callback method for the case of a peer subscription / unsubscription request for load balanced messages.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode){
    return m_sender->loadBalancedRegisterOnPeerCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Send a load balanced unsubscription request.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedSendUnsubscriptionRequest(const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, unsigned int iTimeout)
{
 TML_INT32 iRet = TML_SUCCESS;

  tmlObjWrapper* command = new tmlObjWrapper();
  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle, (char*)"tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest");
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Flag for internal command:
  iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, profile);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_HOST, sSourceHost);
  if (TML_SUCCESS == iRet)
    iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PORT, sSourcePort);

  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  command->tmlObjWrapper_Release_Sidex_Handle((char*)"tmlCoreWrapper::tmlCoreWrapper_EventSendSubscriptionRequest");
  if (TML_SUCCESS == iRet)
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_LOAD_BALANCED_UNSUBSCRIPTION_REQUEST);

  if (TML_SUCCESS == iRet)
    iRet = m_sender->sender_SendSyncMessage(profile, sDestHost, sDestPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, iTimeout, NULL, true);

  //////////////////////////////////////////////////////////////////////////////
  // in case of an error I have to free for the TML_COMMAND_HANDLE:
  delete (command);

  return (int)iRet;
}


/**
 * @brief    Set the maximum load balanced connection fail count value.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedSetMaxConnectionFailCount(unsigned int iCount){
  m_iMaxLoadBalancedConnectionFails = iCount;
  m_sender->loadBalancedSetMaxConnectionFailCount(iCount);
  return TML_SUCCESS;
}


/**
 * @brief    Get the maximum load balanced connection fail count value.
 */
int tmlCoreWrapper::tmlCoreWrapper_LoadBalancedGetMaxConnectionFailCount(TML_UINT32* iCount){
  *iCount = m_iMaxLoadBalancedConnectionFails;
  return TML_SUCCESS;
}



/**
 * @brief    Set the maximum event connection fail count value.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventSetMaxConnectionFailCount(unsigned int iCount){
  m_iMaxEventConnectionFails = iCount;

  m_sender->eventSetMaxConnectionFailCount(iCount);
  return TML_SUCCESS;
}


/**
 * @brief    Get the maximum event connection fail count value.
 */
int tmlCoreWrapper::tmlCoreWrapper_EventGetMaxConnectionFailCount(TML_UINT32* iCount){
  *iCount = (TML_UINT32)m_iMaxEventConnectionFails;
  return TML_SUCCESS;
}


/**
 * @brief    Set the maximum value of queued event messages
 */
int tmlCoreWrapper::tmlCoreWrapper_EventSetMaxQueuedEventMessages(unsigned int iMaximum){
  m_maxEventMsgQueueLength = iMaximum;
  m_sender->eventSetMaxQueuedEventMessages(iMaximum);
  return TML_SUCCESS;
}


/**
 * @brief    Get the maximum value of queued event messages
 */
int tmlCoreWrapper::tmlCoreWrapper_EventGetMaxQueuedEventMessages(unsigned int* iMaximum){
  *iMaximum = m_maxEventMsgQueueLength;
  return TML_SUCCESS;
}


/**
 * @brief    write the debug log to the stdout
 */
void tmlCoreWrapper::log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog){
#ifndef DISABLE_TML_LOGGING
  m_log->log (iLogMask, sClass, sMethod,sFormatLog, sLog);
#endif
}


/**
 * @brief    write the debug log to the stdout
 */
void tmlCoreWrapper::log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const wchar_t* sLog){
#ifndef DISABLE_TML_LOGGING
  SIDEX_INT32 iLengthUtf8;
  char* utf8Name = UTF32toUTF8((wchar_t*)sLog, &iLengthUtf8);
  log (iLogMask, sClass, sMethod, sFormatLog, utf8Name);
  delete[] utf8Name;
#endif
}


/**
 * @brief    write the debug log to the stdout
 */
void tmlCoreWrapper::log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char16_t* sLog){
#ifndef DISABLE_TML_LOGGING
  SIDEX_INT32 iLengthUtf8;
  char* utf8Name = UTF16toUTF8((wchar_t*)sLog, &iLengthUtf8);
  log (iLogMask, sClass, sMethod, sFormatLog, utf8Name);
  delete[] utf8Name;
#endif
}


/**
 * @brief    Open a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_Open(TML_STREAM_ID_TYPE iID, const char* sPort, const char* sHost, const char* profile){
  return m_streamHandler->recStream_Open((TML_CORE_HANDLE)this, iID, sPort, sHost, profile, m_sender, m_iWindowSize);
}


/**
 * @brief    Close a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_Close(TML_STREAM_ID_TYPE iID, TML_BOOL bRetainOpen){
  return m_streamHandler->recStream_Close(iID, bRetainOpen, true);
}


/**
 * @brief    Get the size of a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_GetSize(TML_STREAM_ID_TYPE iID, TML_INT64* rsize){
  return m_streamHandler->recStream_GetSize(iID, rsize);
}


/**
 * @brief    Perform a seek operation of a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_Seek(TML_STREAM_ID_TYPE iID, TML_INT64 seekPos, tmlSeekOrigin origin){
  return m_streamHandler->recStream_Seek(iID, seekPos, origin);
}


/**
 * @brief    Get the position of a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_GetPosition(TML_STREAM_ID_TYPE iID, TML_INT64* rposition){
  return m_streamHandler->recStream_GetPosition(iID, rposition);
}


/**
 * @brief    Write data to a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_Write(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count){
  return m_streamHandler->recStream_Write(iID, buffer, count);
}

/**
 * @brief    Read data of a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_Read(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count, TML_INT32* bytesRead){
  return m_streamHandler->recStream_Read(iID, buffer, count, bytesRead);
}


/**
 * @brief    Read data of a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_ReadBuffer(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count){
  return m_streamHandler->recStream_ReadBuffer(iID, buffer, count);
}


/**
 * @brief    Initiates a download of data of a receiver stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_RecStream_DownloadData(TML_STREAM_ID_TYPE iID, TML_UINT32 buffersize, TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC pCBFuncDld, TML_POINTER pCBDataDld, TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncDldFinish, TML_POINTER pCBDataDldFinish){
  return m_streamHandler->recStream_DownloadData(iID, buffersize, pCBFuncDld, pCBDataDld, pCBFuncDldFinish, pCBDataDldFinish);
}


/**
 * @brief    Open a sender stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Open(TML_STREAM_ID_TYPE* iStreamID, const char* sPort, const char* sHost, const char* profile){
  return m_streamHandler->sndStream_Open(iStreamID, sPort, sHost, profile);
}


/**
 * @brief    Close a sender stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Close(TML_STREAM_ID_TYPE iID){
  return m_streamHandler->sndStream_Close(iID);
}


/**
 * @brief    Register a callback method to get the sender stream size.
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Register_GetSize (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  return m_streamHandler->sndStream_Register_GetSize (iID, pCBFunc, pCBData);
}


/**
 * @brief    Register a callback method to perform stream seek operations.
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Register_Seek (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc, TML_POINTER pCBData){
  return m_streamHandler->sndStream_Register_Seek (iID, pCBFunc, pCBData);
}


/**
 * @brief    Register a callback method to get the stream position.
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Register_GetPosition (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  return m_streamHandler->sndStream_Register_GetPosition (iID, pCBFunc, pCBData);
}


/**
 * @brief    Register a callback method to read data of the sender stream .
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Register_Read (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_READ_CB_FUNC pCBFunc, TML_POINTER pCBData){
  return m_streamHandler->sndStream_Register_Read (iID, pCBFunc, pCBData);
}


/**
 * @brief    Register a callback method to write data to a sender stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Register_Write (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  return m_streamHandler->sndStream_Register_Write (iID, pCBFunc, pCBData);
}

/**
 * @brief    Register a callback method that will be called in case of closing the sender stream.
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Register_Close (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  return m_streamHandler->sndStream_Register_Close (iID, pCBFunc, pCBData);
}


/**
     * @brief    Register a callback method that will be called on communication fault during a request.
 */
int tmlCoreWrapper::tmlCoreWrapper_SndStream_Register_OnError (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc, TML_POINTER pCBData){
  return m_streamHandler->sndStream_Register_OnError (iID, pCBFunc, pCBData);
}


/**
 * @brief    Set Shutdown
 */
void tmlCoreWrapper::tmlCoreWrapper_SetShutdown (bool bShutdown){
  m_csObj->tmlCriticalSectionEnter("tmlCoreWrapper::tmlCoreWrapper_SetShutdown");
  m_bShutdown = bShutdown;
  m_csObj->tmlCriticalSectionLeave("tmlCoreWrapper::tmlCoreWrapper_SetShutdown");
}


/**
 * @brief    Is core accessable
 */
int tmlCoreWrapper::tmlCoreWrapper_IsAccessible (){
  int iRet = TML_SUCCESS;
  m_csObj->tmlCriticalSectionEnter("tmlCoreWrapper::tmlCoreWrapper_IsAccessible");
  if (m_bShutdown){
    // m_log->log ("tmlCoreWrapper", "tmlCoreWrapper_IsAccessible", "TML_ERR_SHUTDOWN", "");
    iRet = TML_ERR_SHUTDOWN;
  }
  m_csObj->tmlCriticalSectionLeave("tmlCoreWrapper::tmlCoreWrapper_IsAccessible");
  return iRet;
}


/**
 */
int tmlCoreWrapper::getLogFileIndex(){
  return m_iLogFileIndex;
}


/**
  * @brief   Create a new listener.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Listener_Create(const char* sAddress, TML_LISTENER_HANDLE* listenerHandle){
  TML_INT32 iRet = TML_SUCCESS;

  tmlListenerObj* wrapper = new tmlListenerObj((TML_CORE_HANDLE)this, sAddress);
  *listenerHandle = (TML_LISTENER_HANDLE) wrapper;

  // TODO: - Add listener to list

  return iRet;
}


/**
  * @brief   Close a listener and release resources.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Listener_Close(TML_LISTENER_HANDLE* listenerHandle){
  TML_INT32 iRet = TML_SUCCESS;

  // Do make the cast to (tmlListenerObj*) / In that case the delete will call the destructor automatically via the scalar destructor:
  delete (tmlListenerObj*)*listenerHandle;
  *listenerHandle = TML_HANDLE_TYPE_NULL;

  // TODO: - remove listener from list

  return iRet;
}


/**
  * @brief   Get the number of listeners.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Get_ListenerCount(TML_UINT32* iCount){
  TML_INT32 iRet = TML_SUCCESS;

  // TODO: - listener count

  return iRet;
}

/**
  * @brief   Get listener's handle from a TML core.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Get_Listener(TML_UINT32 index, TML_LISTENER_HANDLE* listenerHandle){
  TML_INT32 iRet = TML_SUCCESS;

  // TODO: TML_LISTENER_HANDLE
  *listenerHandle = TML_HANDLE_TYPE_NULL;

  return iRet;
}


/**
  * @brief    Enable/disable a listener. 
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Listener_Set_Enabled(TML_LISTENER_HANDLE listenerHandle, TML_BOOL bEnable){
  TML_INT32 iRet = TML_SUCCESS;

  // TODO: enable / disable the listener

  return iRet;
}


/**
  * @brief    Get enable status of a listener.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Listener_Get_Enabled(TML_LISTENER_HANDLE listenerHandle, TML_BOOL* bEnable){
  TML_INT32 iRet = TML_SUCCESS;

  // TODO: Get enable status of a listener

  return iRet;
}


/**
  * @brief   Create a new connection.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Connect(const char* sHost, const char* sPort, bool bUseExisting, TML_CONNECTION_HANDLE* connectionHandle){
  TML_INT32 iRet = TML_SUCCESS;
  int iLength = strlen(sHost) + strlen(sPort) + 2;

  char* sNetAddress = new char[iLength];
  #if defined(LINUX) || defined (MINGW_BUILD)
    sprintf(sNetAddress, "%s:%s", sHost, sPort);
  #else // LINUX
    sprintf_s(sNetAddress, iLength, "%s:%s", sHost, sPort);
  #endif // LINUX

  iRet = tmlCoreWrapper_Connect (sNetAddress, bUseExisting, connectionHandle);

  delete[]sNetAddress;

  return iRet;
}


/**
  * @brief   Create a new connection.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Connect(const char* sAddress, bool bUseExisting, TML_CONNECTION_HANDLE* connectionHandle){
  TML_INT32 iRet = TML_SUCCESS;
  tmlConnectionManageObj* wrapper = TML_HANDLE_TYPE_NULL;

  TML_UINT32 iCount = 0;
  bool bFound = false;
  tmlCoreWrapper_Get_ConnectionCount(&iCount);
  for (TML_UINT32 i = 0; i < iCount && !bFound; ++i){
    TML_CONNECTION_HANDLE connection = TML_HANDLE_TYPE_NULL;
    tmlCoreWrapper_Get_Connection (i, &connection);
    if (connection){
      wrapper = (tmlConnectionManageObj*)connection;
      if (wrapper->isEqual(sAddress)){
        bFound = true;
      }
    }
  }
  if (!bFound){
    wrapper = new tmlConnectionManageObj((TML_CORE_HANDLE)this, sAddress);
    tmlCoreWrapper_Add_ConnectionItem((TML_CONNECTION_HANDLE) wrapper);
  }

  *connectionHandle = (TML_CONNECTION_HANDLE) wrapper;

  return iRet = wrapper->getLastErr();
}


/**
  * @brief   Close a connection and release resources.
  */
TML_INT32  tmlCoreWrapper::tmlCoreWrapper_Connection_Close(TML_CONNECTION_HANDLE* connectionHandle){
  TML_INT32 iRet = TML_SUCCESS;

  // Delete a TML connection handle from the connection list:
  tmlCoreWrapper_Delete_ConnectionItem(*connectionHandle);

  // Do make the cast to (tmlConnectionManageObj*) / In that case the delete will call the destructor automatically via the scalar destructor:
  delete (tmlConnectionManageObj*)*connectionHandle;
  *connectionHandle = TML_HANDLE_TYPE_NULL;

  return iRet;
}


/**
  * @brief     Close al connections and release resources.
  */
void tmlCoreWrapper::tmlCoreWrapper_Connection_CloseAll(){

  TML_UINT32 iCount = 0;
  tmlCoreWrapper_Get_ConnectionCount(&iCount);
  for (TML_UINT32 i = 0; i < iCount; ++i){
    TML_CONNECTION_HANDLE connection = TML_HANDLE_TYPE_NULL;
    tmlCoreWrapper_Get_Connection (i, &connection);
    if (connection){
      tmlCoreWrapper_Connection_Close(&connection);
    }
  }
}

     
/**
  * @brief    Delete a TML connection handle from the connection list
  */
void tmlCoreWrapper::tmlCoreWrapper_Delete_ConnectionItem(TML_CONNECTION_HANDLE handle){

  TML_UINT32 iCount = 0;
  tmlCoreWrapper_Get_ConnectionCount(&iCount);
  bool bFound = false;
  for (TML_UINT32 i = 0; i < iCount && !bFound; ++i){
    TML_CONNECTION_HANDLE tmpConnection = TML_HANDLE_TYPE_NULL;
    tmlCoreWrapper_Get_Connection (i, &tmpConnection);
    if (handle == tmpConnection){
      bFound = true;
      sidex_Variant_List_DeleteItem (m_connectionMgrObjs, i);
    }
  }
}

     
/**
  * @brief    Add a TML connection handle to the connection list
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Add_ConnectionItem(TML_CONNECTION_HANDLE handle){
  SIDEX_INT32 iRet;
  SIDEX_INT32 iPos;
  SIDEX_VARIANT vObj = sidex_Variant_New_Integer(handle);
  iRet = sidex_Variant_List_Append(m_connectionMgrObjs, vObj, &iPos);
  sidex_Variant_DecRef(vObj);
  return iRet;
}

     
/**
  * @brief   Returns the number of connections.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Get_ConnectionCount(TML_UINT32* iCount){
  TML_INT32 iRet = TML_SUCCESS;

  TML_INT32 iSize = 0;
  iRet = sidex_Variant_List_Size (m_connectionMgrObjs, &iSize);
  *iCount = (TML_UINT32)iSize;

  if (SIDEX_SUCCESS != iRet){
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  return iRet;
}

/**
  * @brief   Get connection handle from a TML core.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Get_Connection(TML_UINT32 index, TML_CONNECTION_HANDLE* connectionHandle){
  TML_INT32 iRet = TML_SUCCESS;

  SIDEX_VARIANT vObj;
  iRet = sidex_Variant_List_Get(m_connectionMgrObjs, index, &vObj);
  if (SIDEX_SUCCESS == iRet){
    SIDEX_INT64 iHandle = 0;
    iRet = sidex_Variant_As_Integer (vObj, &iHandle);
    if (SIDEX_SUCCESS == iRet){
      *connectionHandle = (TML_CONNECTION_HANDLE) iHandle;
    }
  }
  if (SIDEX_SUCCESS != iRet){
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  return iRet;
}


/**
  * @brief   Send async command on existing connection.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Connection_SendAsyncMessage(TML_CONNECTION_HANDLE connectionHandle, const char* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_SUCCESS;

  // TODO: Send async message

  return iRet;
}


/**
  * @brief   Send sync command on existing connection.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Connection_SendSyncMessage(TML_CONNECTION_HANDLE connectionHandle, const char* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout){
  int  iRet = TML_SUCCESS;
  iRet = m_sender->sender_SendSyncMessage(sProfile, connectionHandle, m_iWindowSize, tmlhandle, iTimeout + m_log->getAdditionalTimeout(), NULL, true);
  return iRet;
}

/**
  * @brief   Send sync command on existing connection.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Set_OnConnect(TML_ON_CONNECT_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;

  // TODO: Set_OnConnect

  return iRet;
}


/**
  * @brief    Callback function to signal a closed connection.
  */
TML_INT32 tmlCoreWrapper::tmlCoreWrapper_Set_OnDisconnect(TML_ON_DISCONNECT_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;

  // TODO: Set_OnDisconnect

  return iRet;
}
