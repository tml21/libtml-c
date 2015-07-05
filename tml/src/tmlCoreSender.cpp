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
 *  For commercial support on build UJO enabled solutions contact us:
 *  
 * Contributors:
 *    wobe-systems GmbH
 */
 
#include "tmlGlobalCallback.h"
#include "tmlObjWrapper.h"
#include "tmlCoreSender.h"
#include "tmlCoreDefines.h"
#include "tmlCmdDefines.h"
#include "tmlConnectionObj.h"
#include "tmlReceiverStream.h"
#include "tmlCore.h"
#include "tmlErrors.h"
#include "unicode.h"
#include "vortex.h"
#include <errno.h>
#include <stddef.h>
#include "systemBase.h"
#include "logValues.h"


////////////////////////////////////////////////////////////////////////////
// This define will log inplausible ( plausible timeout values:
#define TIMEOUT_CHECK_PLAUSIBILITY_XX

////////////////////////////////////////////////////////////////////////////
// internal thread destruction method
axl_bool  intern_thread_destroy (TMLThreadDef* threadInfo)
{
  VortexThread* data = threadInfo->pThreadDef;
  axl_bool bRet = axl_true;
#ifndef LINUX
  DWORD err;
  err = WaitForSingleObject(data->handle, INFINITE);
  switch (err) {
  default:
  case WAIT_OBJECT_0:
    //printf ("thread %p stopped\n", data->handle);
    break;
  case WAIT_ABANDONED:
    //printf ("unable to stop thread %p, wait_abandoned\n", data->handle);
    break;

  case WAIT_TIMEOUT:
    //printf ("unable to stop thread %p, wait_timeout\n", data);
    break;
  }
  CloseHandle (data->handle);
  data->handle = NULL;

  axl_free (data);
  if (err != WAIT_OBJECT_0){
    printf ("## intern_thread_destroy / Handle = %d - Err = %d\n", data->handle, err);
  }
  bRet = err == WAIT_OBJECT_0;

#else // LINUX
  bRet = vortex_thread_destroy (data, axl_true);
#endif
  return bRet;
}


////////////////////////////////////////////////////////////////////////////
// internal thread creation method / 2nd level
#ifdef LINUX
axl_bool  intern_thread_create (TMLThreadDef* threadInfo, VortexThreadFunc func, axlPointer user_data)
#else // LINUX
axl_bool  intern_thread_create (TMLThreadDef* threadInfo, FUNC_STDCALL func(void*), axlPointer user_data)
#endif // LINUX
{
  axl_bool      result = axl_true;
#ifndef LINUX

  /* windows implementation */
  /* create the thread data to pass it to the proxy function */
  VortexThread* data = axl_new(VortexThread, 1);
  data->data     = user_data;
  data->handle   = (HANDLE) _beginthreadex (
    /* use default SECURITY ATTRIBUTES */
    NULL,
    /* use default stack size */
    0,  
    /* function to execute */
    func,
    /* data to be passed to the function */
    user_data,
    /* initial state is running */
    0,
    /* identifier */
    &data->id);

  if (0 == data->handle){
    printf ("intern_thread_create - Handle is 0 / Error on _beginthread\n");
  }
  if ((TML_INT64)-1 == (TML_INT64)data->handle){
      printf ("intern_thread_create - Handle is -1 / Error on _beginthread too many threads\n");
  }
  if (data->handle == NULL) {
    /* free data because thread wasn't created */
      printf ("intern_thread_create - Handle is Null\n");
    axl_free (data);
    result = axl_false;
  }
  else{
    threadInfo->pThreadDef = data;
  }
#else // LINUX
  VortexThread* data = axl_new(VortexThread, 1);
  /* create the thread */
  result = vortex_thread_create (data, (VortexThreadFunc)func, user_data, VORTEX_THREAD_CONF_END);
  //printf ("## __intern_thread_create / Handle = %d\n", data->handle);

  if (axl_false == result){
    axl_free(data);
  }
  else{
    threadInfo->pThreadDef = data;
  }
#endif
  return result;
}


/*********************************************************************************************************************************
*                                             "C" / Global methods / Callbacks & Threads
*********************************************************************************************************************************/
/**
 * @brief  Thread to handle async messages / only one during lifetime
 */
FUNC_STDCALL AsyncHandlingThread (axlPointer pParam)
{
  AsyncHandlingData* pThreadData = (AsyncHandlingData*)pParam;
  tmlLogHandler* pLog = pThreadData->payload->pLog;

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // First I lock the mutex to be sure that the parent process waits until I'm finished
  pLog->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "AsyncHandlingThread", "Vortex CMD", "vortex_mutex_lock");
  intern_mutex_lock (pThreadData->terminationMutex, pLog, "AsyncHandlingThread");

  AsyncHandlingThreadMethod(pThreadData->payload);

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Unlock the mutex to tell the parent process that I'm finished
  pLog->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "AsyncHandlingThread", "Vortex CMD", "vortex_mutex_unlock");
  intern_mutex_unlock (pThreadData->terminationMutex, pLog, "AsyncHandlingThread");
#ifndef LINUX
  _endthreadex( 0 );
#endif // LINUX
  return 0;
}


/**
 * @brief    Start the AsyncHandlingThread.
 */
int StartAsyncHandlingThread(AsyncHandlingData* threadData, tmlEventHandler* eventHandler, const char** handlingEventArray, AsyncHandlingThreadData* payload, VortexMutex* mutexAsyncThreadSync)
{
  int iRet = TML_SUCCESS;

  ///////////////////////////////////////////////////////////////
  // Delete Memory of the last call
  // Maybe we have had an async handling thread, so free memory:
  if (NULL != threadData->threadInfo.pThreadDef){
    intern_thread_destroy (&threadData->threadInfo);
    threadData->threadInfo.pThreadDef = NULL;
  }

  threadData->handlingEventArray = handlingEventArray;
  threadData->payload = payload;
  threadData->eventHandler = eventHandler;
  threadData->terminationMutex = mutexAsyncThreadSync;


  TMLThreadDef* threadInfo = &threadData->threadInfo;
  payload->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "StartAsyncHandlingThread", "Vortex CMD", "vortex_thread_create");
  // Thread- generation log
#ifdef LINUX
  #ifdef OS_X
    axl_bool bSuccess = intern_thread_create(threadInfo, AsyncHandlingThread, threadData);
  #else // OSX
    axl_bool bSuccess = intern_thread_create(threadInfo, AsyncHandlingThread, threadData);
  #endif // OSX
#else // LINUX
    axl_bool bSuccess = intern_thread_create(threadInfo, AsyncHandlingThread, threadData);
#endif // LINUX
  // Thread- generation log
  if (!bSuccess){
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }
  else{
    /////////////////////////////////////////////////////////////////////////////
    // Delete allocated memory here because we don't stop the thread explicitely
    // by using of intern_thread_destroy :

    // No - this will be done in the next call (see a few lines sooner)
    //delete threadInfo.pThreadDef;
  }
  return iRet;
}


/**
 * @brief    Stop the AsyncHandlingThread.
 */
void StopAsyncHandlingThread(const char** handlingEventArray, tmlEventHandler* eventHandler, tmlLogHandler* pLogHandler)
{
  if (NULL != handlingEventArray){
    pLogHandler->log (TML_LOG_CORE_IO, "TMLCoreSender", "StopAsyncHandlingThread", "AsyncHandlingThread", "Stop");
    if (! eventHandler->SetEventOnHandle(handlingEventArray[ASYNC_HANDLING_TERMINATE_EVENT])){
      pLogHandler->log ("TMLCoreSender", "StopAsyncHandlingThread", "SetEvent", "failed");
    }
  }
}


/**
 * @brief    Restart the AyncHandlingThread.
 */
int TMLCoreSender::RestartAsyncHandlingThread(AsyncHandlingData* threadData, const char*** handlingEventArray, AsyncHandlingThreadData* payload, tmlLogHandler* pLogHandler)
{
  int iRet = TML_SUCCESS;

  if (NULL == *handlingEventArray){
    /////////////////////////////////////////////////////
    // It's the first time of RestartAsyncHandlingThread
    // I have to allocate the attributes memory:
    iRet = allocAsyncThreadEventArray(handlingEventArray);
    ////////////////////////////////////////////////////////////////////////////
    // Start the async thread only once during sender's lifetime:
    if (TML_SUCCESS == iRet){
      iRet = resetSyncEventArray(handlingEventArray, NO_OF_ASYNC_HANDLING_HANDLES, true);
      if (TML_SUCCESS == iRet){
        payload->pLog = pLogHandler;
      }
    }
  }
  if (TML_SUCCESS == iRet){
    iRet = StartAsyncHandlingThread(threadData, m_eventHandler, *handlingEventArray, payload, &m_mutexAsyncThreadSync);
  }
  return iRet;
}


/**
 * @brief  Timer thread to handle timeouts
 */
FUNC_STDCALL TimerThread( void* pParam)
{
  TimerThreadData* pThreadData = (TimerThreadData*)pParam;

  bool bTerminateThread = false;

  const char** handleArray = new const char*[2];
  handleArray[0] = pThreadData->timerSyncEventArray[SEND_MSG_SYNC_HANDLES_CONTINUE_INDEX];
  handleArray[1] = pThreadData->timerSyncEventArray[SEND_MSG_SYNC_HANDLES_STOP_TIMER_INDEX];

  pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "TimerThread", "TimerThread", "start");
  do
  {
    bTerminateThread = false;
    DWORD dwIndex;
    int iRetValue = pThreadData->eventHandler->WaitForMultipleEvent(handleArray, 2, pThreadData->dwTimeoutValue, &dwIndex);
    if (WAIT_SUCCEEDED == iRetValue){
      switch (dwIndex){
        case 0:
          // Continue - New Timer
          pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "TimerThread", "TimerThread", "continue");
          break;
        case 1:
          // terminate:
          bTerminateThread = true;
          break;
        default:
          // abandoned - terminate:
          pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "TimerThread", "Handle", "abandoned");
          // I will map this error into a timeout:
          if (! pThreadData->eventHandler->SetEventOnHandle(pThreadData->timerSyncEventArray[SEND_MSG_SYNC_HANDLES_TIMEOUT_INDEX])){
            pThreadData->pLog->log ("TMLCoreSender", "TimerThread", "SetEvent", "failed");
          }
          /////////////////////////
          // Incase of lost od connection / close all there was a sticking if I don't close the thread here
          bTerminateThread = true;
          break;
      }
    }
    else{
      if (WAIT_TOUT == iRetValue){
        pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "TimerThread", "TimerThread", "timeout");
        // Timeout:
        if (! pThreadData->eventHandler->SetEventOnHandle(pThreadData->timerSyncEventArray[SEND_MSG_SYNC_HANDLES_TIMEOUT_INDEX])){
          pThreadData->pLog->log ("TMLCoreSender", "TimerThread", "SetEvent", "failed");
        }
      }
      else{
        // abandoned - terminate:
        pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "TimerThread", "Handle", "abandoned");
        // I will map this error into a timeout:
        if (! pThreadData->eventHandler->SetEventOnHandle(pThreadData->timerSyncEventArray[SEND_MSG_SYNC_HANDLES_TIMEOUT_INDEX])){
          pThreadData->pLog->log ("TMLCoreSender", "TimerThread", "SetEvent", "failed");
        }
          // Incase of lost od connection / close all there was a sticking if I don't close the thread here
        bTerminateThread = true;
      }
    }
  }while (!bTerminateThread);
  // Thread end
  pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "TimerThread", "TimerThread", "terminated");
  delete (handleArray);
#ifndef LINUX
  _endthreadex( 0 );
#endif // LINUX
  return 0;
}


/**
 * @brief    Start the timer thread.
 */
int StartTimerThread(TimerThreadData* timerThreadData, tmlEventHandler* eventHandler, const char** hSynchronisationHandleArray, DWORD dwTimeout, tmlLogHandler* pLogHandler, TMLThreadDef* threadInfo)
{
  int iRet = TML_SUCCESS;
  timerThreadData->timerSyncEventArray  = hSynchronisationHandleArray;
  timerThreadData->dwTimeoutValue       = dwTimeout;
  timerThreadData->pLog                 = pLogHandler;
  timerThreadData->eventHandler         = eventHandler;
  timerThreadData->terminationMutex     = NULL;

  pLogHandler->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "StartTimerThread", "Vortex CMD", "vortex_thread_create");
#ifdef LINUX
  #ifdef OS_X
  axl_bool bSuccess = intern_thread_create(threadInfo, TimerThread, timerThreadData);
  #else // OSX
    axl_bool bSuccess = intern_thread_create(threadInfo, TimerThread, timerThreadData);
  #endif // OSX
#else // LINUX
    axl_bool bSuccess = intern_thread_create(threadInfo, TimerThread, timerThreadData);
#endif // LINUX
  if (!bSuccess){
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }
  else{
    threadInfo->bThreadStarted = true;
  }
  return iRet;
}


/**
 * @brief    Stop the timer thread.
 */
void StopTimerThread(const char* hStopTimer, AsyncHandlingThreadData* pThreadData, TMLThreadDef* threadInfo)
{
  if (threadInfo->bThreadStarted){
    pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "StopTimerThread", "Timer Thread", "Stop");
    if (! pThreadData->eventHandler->SetEventOnHandle(hStopTimer)){
      pThreadData->pLog->log ("TMLCoreSender", "StopTimerThread", "SetEvent", "failed");
    }
    else{
      /////////////////////////////////////
      // Wait until the thread is finished:
      pThreadData->pLog->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "StopTimerThread", "Vortex CMD", "vortex_thread_destroy");
#ifdef LINUX
      axl_bool bRet = intern_thread_destroy(threadInfo);
#else
      axl_bool bRet = intern_thread_destroy(threadInfo);
#endif // LINUX
      if (AXL_FALSE == bRet){
        printf ("#### StopTimerThread - Error in \"vortex_thread_destroy\" ####\n");
      }
      threadInfo->bThreadStarted = false;
      pThreadData->pLog->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "StopTimerThread", "Vortex CMD", "vortex_thread_destroy done");
    }
  }
}


/**
 * @brief    Restart the timer thread / the timeout.
 */
void RestartTimerThread(TimerThreadData* timerThreadData, tmlEventHandler* eventHandler, const char* hContinue, DWORD dwTimeout, tmlLogHandler* pLogHandler)
{
  if (NULL != hContinue){
    pLogHandler->log (TML_LOG_CORE_IO, "TMLCoreSender", "RestartTimerThread", "Timer Thread", "Restart");
    timerThreadData->dwTimeoutValue = dwTimeout;
    if (! eventHandler->SetEventOnHandle(hContinue)){
      pLogHandler->log ("TMLCoreSender", "RestartTimerThread", "SetEvent", "failed");
    }
  }
}


/**
 * @brief    Method checks for and handles asynchronous steam replies
 */
TML_INT32 HandleAsyncReplyFromStream(tmlMultiAsyncMsg* msg, TML_INT32*  iSize, TML_INT32 iEmptyOnError){
  TML_INT32 iRet = TML_SUCCESS;

  streamDataStructure* data;
  msg->GetNextAsyncStreamMsg(&data, iSize);
  msg->ReleaseNextAsyncMsgStream();
  if (*iSize >= 1){
    //////////////////////////////////////
    // Hier findet das Wegschreiben statt:
    iRet = StreamContentHandling(data->sHandle, 
                          data->iPayloadFrameSize,
                          data->cPayload,
                          data->totalFrameSize,
                          data->pFrame,
                          data->cmd,
                          data->log,
                          iEmptyOnError);
    delete data;
  }
  return iRet;
}

/**
 * @brief    Method checks for and handles asynchronous progress and message replies.
 */
bool HandleAsyncReply(TML_COMMAND_HANDLE tmlhandle, tmlLogHandler* pLogHandler, VortexMutex* pMutexCriticalSection){
  TML_ON_STATUS_REPLY_CB_FUNC pFuncStatus = NULL;
  TML_ON_PROGRESS_REPLY_CB_FUNC pFuncProgress = NULL;
  TML_POINTER pData      = NULL;
  TML_INT32 iProgress    = -1;
  TML_INT32 iReplyType   = TMLCOM_RPY_UNDEFINED;
  TML_INT32 iReplyMsgLen = -1;
  char* sReplyMsg        = NULL;
  bool  bContinue        = true;
  TML_INT32   iRet;


  ///////////////////////////////////////////////////////////////////////////
  // Begin of critical section / possible undefinded m_SidexObj during next operation:
  pLogHandler->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "HandleAsyncReply", "Vortex CMD", "vortex_mutex_lock");
  intern_mutex_lock (pMutexCriticalSection, pLogHandler, "HandleAsyncReply");
  iRet = tml_Cmd_Header_GetReplyType(tmlhandle, &iReplyType);
  if (TML_SUCCESS == iRet){
    if (TMLCOM_RPY_PROGRESS == iReplyType){
      pLogHandler->log (TML_LOG_CORE_IO, "TMLCoreSender", "HandleAsyncReply", "ReplyType", "TMLCOM_RPY_PROGRESS");
      ///////////////////////////////////////
      // We have an async. Progress reply:
      iRet = tml_Cmd_Header_GetProgress(tmlhandle, &iProgress);
      if (TML_SUCCESS == iRet){
        tml_Cmd_Registered_Progress(tmlhandle, &pFuncProgress, &pData); // Don't mind of return value
        ///////////////////////////////////////////////////////////////////////////
        // End of critical section / possible undefinded m_SidexObj during next operation:
        pLogHandler->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "HandleAsyncReply", "Vortex CMD", "vortex_mutex_unlock");
        intern_mutex_unlock (pMutexCriticalSection, pLogHandler, "HandleAsyncReply");
        if (NULL != pFuncProgress){
          ///////////////////////////////////////
          // Async Progress Callback with data:
          ((void(FUNC_C_DECL *)(TML_COMMAND_HANDLE, TML_POINTER, TML_INT32))pFuncProgress)(tmlhandle, pData, iProgress);
        }
        else{
          // In the past I didn't expect progress replies without a registered callback, but this possible:
          // pLogHandler->log ("TMLCoreSender", "HandleAsyncReply", "WARNING", "No registered progress callback method");
        }
      }
      else{
        ///////////////////////////////////////////////////////////////////////////
        // End of critical section / possible undefinded m_SidexObj during next operation:
        pLogHandler->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "HandleAsyncReply", "Vortex CMD", "vortex_mutex_unlock");
        intern_mutex_unlock (pMutexCriticalSection, pLogHandler, "HandleAsyncReply");
        pLogHandler->log ("TMLCoreSender", "HandleAsyncReply", "ERROR", "Can't detect progress attribute to handle async progress reply");
      }
    }
    else{
      ///////////////////////////////////////
      // We have an async. status reply:
      pLogHandler->log (TML_LOG_CORE_IO, "TMLCoreSender", "HandleAsyncReply", "ReplyType", "WARNING-ERROR-INFORMATION");
      iRet = tml_Cmd_Header_GetReplyMessage(tmlhandle, &sReplyMsg, &iReplyMsgLen);
      if (TML_SUCCESS == iRet){
        char* sStatus = NULL;
        if (iReplyMsgLen > 0){
          sStatus = new char[(int)iReplyMsgLen+1];
#if defined (LINUX) || defined (MINGW_BUILD)
          strcpy (sStatus, sReplyMsg);
#else
          strcpy_s (sStatus, (int)iReplyMsgLen+1,  sReplyMsg);
#endif
        }
        sidex_Free_ReadString(sReplyMsg);
        tmlUnicodeID iUnicode;
        ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Registered_StatusReply(&pFuncStatus, &pData, &iUnicode);
        ///////////////////////////////////////////////////////////////////////////
        // End of critical section / possible undefinded m_SidexObj during next operation:
        pLogHandler->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "HandleAsyncReply", "Vortex CMD", "vortex_mutex_unlock");
        intern_mutex_unlock (pMutexCriticalSection, pLogHandler, "HandleAsyncReply");
        if (NULL != pFuncStatus){
          ///////////////////////////////////////
          // Async Status Reply Callback with data:
          switch (iUnicode){
            case idUNICODE_ASCII:
                  ((void(FUNC_C_DECL *)(TML_COMMAND_HANDLE, TML_POINTER, TML_INT32, char*))pFuncStatus)(tmlhandle, pData, iReplyType, sStatus);
                  break;
            case idUNICODE_WCHAR_T:
                  {
                    TML_INT32 iStatusLength;
                    wchar_t* utf16status = UTF8toUTF32(sStatus, &iStatusLength);
                    if (NULL != utf16status){
                      ((void(FUNC_C_DECL *)(TML_COMMAND_HANDLE, TML_POINTER, TML_INT32, wchar_t*))pFuncStatus)(tmlhandle, pData, iReplyType, utf16status);
                      delete utf16status;
                    }
                  }
                  break;
            case idUNICODE_CHAR16_T:
                  {
                    TML_INT32 iStatusLength;
                    char16_t* utf16status = (char16_t*)UTF8toUTF16(sStatus, &iStatusLength);
                    if (NULL != utf16status){
                      ((void(FUNC_C_DECL *)(TML_COMMAND_HANDLE, TML_POINTER, TML_INT32, char16_t*))pFuncStatus)(tmlhandle, pData, iReplyType, utf16status);
                      delete utf16status;
                    }
                  }
                  break;
          }
        }
        else{
          // In the past I didn't expect status replies without a registered callback, but this possible:
          // pLogHandler->log ("TMLCoreSender", "HandleAsyncReply", "WARNING", "No registered status callback method");
        }
        if (NULL != sStatus)
          delete (sStatus);
      }
      else{
        ///////////////////////////////////////////////////////////////////////////
        // End of critical section / possible undefinded m_SidexObj during next operation:
        pLogHandler->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "HandleAsyncReply", "Vortex CMD", "vortex_mutex_unlock");
        intern_mutex_unlock (pMutexCriticalSection, pLogHandler, "HandleAsyncReply");
        pLogHandler->log ("TMLCoreSender", "HandleAsyncReply", "ERROR", "Can't detect status attribute to handle async progress reply");
      }
    }
    bContinue = false;
  }
  else{
    ///////////////////////////////////////////////////////////////////////////
    // End of critical section / possible undefinded m_SidexObj during next operation:
    pLogHandler->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "HandleAsyncReply", "Vortex CMD", "vortex_mutex_unlock");
    intern_mutex_unlock (pMutexCriticalSection, pLogHandler, "HandleAsyncReply");
    pLogHandler->log ("TMLCoreSender", "HandleAsyncReply", "ERROR", "Can't detect reply type to handle async status / progress reply");
  }
  return bContinue;
}

int m_iMultiSyncMessageCounter = 1;

/**
 * @brief  callback in a received frame (Raw reply)
 */
void senderRawFrameReceivedCallback (VortexChannel* channel,
                           VortexConnection* connection,
                           VortexFrame* frame,
                           axlPointer user_data)
{
  VORTEXSenderFrameReceivedCallbackData* callbackData = (VORTEXSenderFrameReceivedCallbackData*) user_data;
  // CMD sticking during stream download
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderRawFrameReceivedCallback", "Vortex CMD", "vortex_channel_get_number");
  callbackData->iChannelNo = vortex_channel_get_number (channel);
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderRawFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_type");
  callbackData->type = vortex_frame_get_type(frame);
  /////////////////////////////////////////////////////////////////////////////////
  // Every type expect ANS will be followed by vortex_channel_pool_release_channel
  if (VORTEX_FRAME_TYPE_ANS != callbackData->type){
    int iLog = callbackData->pLog->getLoggingValue();
    if (iLog & TML_LOG_MULTI_SYNC_CMDS){
      callbackData->pLog->log (TML_LOG_MULTI_SYNC_CMDS, "TMLCoreSender", "senderRawFrameReceivedCallback", "MessageCounter", m_iMultiSyncMessageCounter);
      ++m_iMultiSyncMessageCounter;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // Notice:
    // 17-05-10 The release of the channel will not be done in the senderRawFrameReceivedCallback() but
    // will be done now later in the "InternalAsyncCmdCallbackHandlerMethod" because 
    // it MUST be the last thing to do because getting the channel out of the pool signals an idle sender.
    //////////////////////////////////////////////////////////////////////////////////////////////////
  }


  // Increment refCounter to hold the data:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderRawFrameReceivedCallback", "Vortex CMD", "vortex_frame_ref");
  vortex_frame_ref(frame);  
  callbackData->pFrame = frame;
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderRawFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_payload");
  callbackData->cPayload = (char*) vortex_frame_get_payload (frame);
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderRawFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_payload_size");
  callbackData->iPayloadFrameSize = vortex_frame_get_payload_size (frame);
  callbackData->pLog->log (TML_LOG_STREAM_HANDLING,  "TMLCoreSender", "senderRawFrameReceivedCallback", "vortex_frame_get_payload_size", callbackData->iPayloadFrameSize);
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderRawFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_msgno");
  callbackData->iMsgNo = vortex_frame_get_msgno(frame);
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderRawFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_more_flag");
  callbackData->bMore = vortex_frame_get_more_flag(frame);
  // Call the class callback handling method:
  globalCallback(callbackData->callback, callbackData);

  return;
}


/**
 * @brief  callback in a received frame (reply)
 */
void senderFrameReceivedCallback (VortexChannel* channel,
                           VortexConnection* connection,
                           VortexFrame* frame,
                           axlPointer user_data)
{
  VORTEXSenderFrameReceivedCallbackData* callbackData = (VORTEXSenderFrameReceivedCallbackData*) user_data;
  // CMD sticking during stream download
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderFrameReceivedCallback", "Vortex CMD", "vortex_channel_get_number");
  callbackData->iChannelNo = vortex_channel_get_number (channel);
  //callbackData->pLog->log ("senderFrameReceivedCallback", "#######   BEGIN   #######", "Channel", callbackData->iChannelNo);
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_type");
  callbackData->type = vortex_frame_get_type(frame);
  /////////////////////////////////////////////////////////////////////////////////
  // Every type expect ANS will be followed by vortex_channel_pool_release_channel
  if (VORTEX_FRAME_TYPE_ANS != callbackData->type){
    int iLog = callbackData->pLog->getLoggingValue();
    if (iLog & TML_LOG_MULTI_SYNC_CMDS){
      callbackData->pLog->log (TML_LOG_MULTI_SYNC_CMDS, "TMLCoreSender", "senderFrameReceivedCallback", "MessageCounter", m_iMultiSyncMessageCounter);
      ++m_iMultiSyncMessageCounter;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // Notice:
    // 17-05-10 The release of the channel will not be done in the senderFrameReceivedCallback() but
    // will be done now later in the "InternalAsyncCmdCallbackHandlerMethod" because 
    // it MUST be the last thing to do because getting the channel out of the pool signals an idle sender.
    //////////////////////////////////////////////////////////////////////////////////////////////////
  }


  // Increment refCounter to hold the data:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderFrameReceivedCallback", "Vortex CMD", "vortex_frame_ref");
  vortex_frame_ref(frame);  
  callbackData->pFrame = frame;
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_payload");
  callbackData->cPayload = (char*) vortex_frame_get_payload (frame);
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_payload_size");
  callbackData->iPayloadFrameSize = vortex_frame_get_payload_size (frame);
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_msgno");
  callbackData->iMsgNo = vortex_frame_get_msgno(frame);
  callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_more_flag");
  callbackData->bMore = vortex_frame_get_more_flag(frame);
  // Call the class callback handling method:
  globalCallback(callbackData->callback, callbackData);

  return;
}


/**
 * @brief  callback in a received frame (reply) to consume latecomer after timeout.
 */
void senderFrameReceivedCallbackConsumeLatecomer (VortexChannel* channel,
                           VortexConnection* connection,
                           VortexFrame* frame,
                           axlPointer pParam)
{
  AsyncHandlingThreadData* pThreadData = (AsyncHandlingThreadData*)pParam;

  pThreadData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "senderFrameReceivedCallbackConsumeLatecomer", "Vortex CMD", "vortex_frame_get_type");
  VortexFrameType type = vortex_frame_get_type(frame);

  /////////////////////////////////////////////////////////////////////////////////
  // Every type expect ANS will be followed by vortex_channel_pool_release_channel
  bool bFreePool = false;
  axl_bool bMore = axl_false;
  switch (type){
    case VORTEX_FRAME_TYPE_ANS:
      break;
    case VORTEX_FRAME_TYPE_RPY:
      /////////////////////////////////
      // It may be a raw data transfer:
      bMore = vortex_frame_get_more_flag(frame);
      if (axl_false == bMore){
        bFreePool = true;
      }
      break;
    default:
      bFreePool = true;
      break;
  }
  // New behaviour I do release the channel pool here and do also unlock the connection object
  // "senderFrameReceivedCallbackConsumeLatecomer" will not do this things for me now.
  // So set the flag to false here hardcoded:
  bFreePool = false;


  if (bFreePool){
    VortexChannelPool* channelPool;
    pThreadData->pLog->log (TML_LOG_MULTI_SYNC_CMDS,  "TMLCoreSender", "senderFrameReceivedCallbackConsumeLatecomer", "Vortex CMD", "vortex_channel_get_pool");
    channelPool = vortex_channel_get_pool(channel);
    if (NULL != channelPool){
  #ifndef DISABLE_TML_LOGGING
      pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "senderFrameReceivedCallbackConsumeLatecomer", "Vortex CMD", "vortex_channel_get_number");
      int iNumber = vortex_channel_get_number(channel);
      pThreadData->pLog->log (TML_LOG_VORTEX_CH_POOL, "TMLCoreSender", "senderFrameReceivedCallbackConsumeLatecomer", "Release channel", iNumber);
  #endif
      pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "senderFrameReceivedCallbackConsumeLatecomer", "Vortex CMD", "vortex_channel_pool_release_channel");
      vortex_channel_pool_release_channel(channelPool, channel);
      //////////////////////////////////////////////////////////////
      // Unlock the connection object to get it usable for others:
      pThreadData->connectionObj->unlock();
    }
  }
  //pLog->log ("senderFrameReceivedCallbackConsumeLatecomer", "#######    END    #######", "", "");
  return;
}


/**
 * @brief  Async sender thread method
 */
void AsyncHandlingThreadMethod (LPVOID pParam)
{
  TML_INT32 iRet = TML_SUCCESS;
  VortexChannel* channelAttr = NULL;
  VORTEXSenderFrameReceivedCallbackData* callbackData = NULL;
  AsyncHandlingThreadData* pThreadData = (AsyncHandlingThreadData*)pParam;
  bool bTimerStarted = false;
  TMLThreadDef threadInfo;
  threadInfo.bThreadStarted = false;

  pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "AsyncHandlingThreadMethod", "Start", "");

  ////////////////////////////////
  // The channelPool :
  VortexChannelPool* channelPoolAttr = NULL;
  pThreadData->connectionObj->getChannelPool(&channelPoolAttr);
  if (NULL == channelPoolAttr){
    //////////////////////////////////////////////
    // The first time allocation of a thread pool:
    VortexConnection* connectionAttr = NULL;
    pThreadData->connectionObj->getConnection(&connectionAttr);
    char* profile = NULL;
    pThreadData->connectionObj->getProfile(&profile);
    if (NULL != connectionAttr && NULL != profile){
      ////////////////////////////////////////
      // now create  a new channel pool:
      pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_pool_new");
      // Thread- generation log
      channelPoolAttr = vortex_channel_pool_new(connectionAttr, profile, 1, NULL, NULL, NULL, NULL, NULL, NULL);
      // Thread- generation log
      ////////////////////////////////////////////////////////////////////////////
      // And now it's time to set the channelPool attribute in the connectionObj:
      pThreadData->connectionObj->setChannelPool(channelPoolAttr);
    }
  }

  ////////////////////////////////
  // The channel :
  int iLog = pThreadData->pLog->getLoggingValue();
  if (!(iLog & TML_LOG_VORTEX_CMD)){
    pThreadData->pLog->log (TML_LOG_VORTEX_CH_POOL, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_pool_get_next_ready");
  }
  pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_pool_get_next_ready");
  if (NULL != channelPoolAttr){
    channelAttr = vortex_channel_pool_get_next_ready(channelPoolAttr, true);
    if (NULL != channelAttr){
      pThreadData->pLog->log (TML_LOG_VORTEX_CH_POOL, "TMLCoreSender", "AsyncHandlingThreadMethod", "channel", "returned");
      pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_set_serialize");
      vortex_channel_set_serialize(channelAttr, true);
#ifndef DISABLE_TML_LOGGING
      pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_get_number");
      int iNumber = vortex_channel_get_number (channelAttr);
      pThreadData->pLog->log (TML_LOG_VORTEX_CH_POOL, "TMLCoreSender", "AsyncHandlingThreadMethod", "VortexChannelNumber", iNumber);
#endif
      ///////////////////////////////////////////////////
      // And now it's time to set the channel attribute:
      pThreadData->connectionObj->setChannel(channelAttr);
    }
    else{
      iRet = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }
  else{
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }

  ////////////////////////////////
  // Raw or not raw :
  if (TML_SUCCESS == iRet){
    if (pThreadData->connectionObj->getRawViaVortexPayloadFeeder()){
      pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_set_complete_flag - false");
      vortex_channel_set_complete_flag(channelAttr, axl_false);
    }
    else{
      pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_set_complete_flag - true");
      vortex_channel_set_complete_flag(channelAttr, axl_true);
    }
  }

  ////////////////////////////////
  // send the message :
  if (TML_SUCCESS == iRet){
          pThreadData->connectionObj->getCallbackData(&callbackData);


          // Acquire critical section use: 
          SIDEX_HANDLE sHandle;
          iRet = tml_Cmd_Acquire_Sidex_Handle(callbackData->tmlhandle, &sHandle);
          //////////////////////////////////////////////////////////////////
          // Get the content:
          char* content;
          TML_INT32 iContentLength;
          iRet = sidex_Get_Content(sHandle, &content, &iContentLength);

          // register the frame receive callback method:
          pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_set_received_handler");
          if (CMD_INTERNAL_STREAM_READ == callbackData->cmdValue || CMD_INTERNAL_STREAM_DOWNLOAD == callbackData->cmdValue){
            vortex_channel_set_received_handler (channelAttr, senderRawFrameReceivedCallback , callbackData);
          }
          else{
            vortex_channel_set_received_handler (channelAttr, senderFrameReceivedCallback , callbackData);
          }

          // now send the message:
          pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_get_number");
          int iCh = vortex_channel_get_number(channelAttr);
          
          int iLog = pThreadData->pLog->getLoggingValue();
          if (!(iLog & TML_LOG_VORTEX_CMD)){
            pThreadData->pLog->log (TML_LOG_VORTEX_CH_POOL, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD:vortex_channel_send_msg - Channel", iCh);
          }
          pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD:vortex_channel_send_msg - Channel", iCh);
          int  iMsgNo = 0;
          if (! vortex_channel_send_msg (channelAttr, content, (int)iContentLength, &iMsgNo)) {
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Any more receive messges shall only be consumed - a channel may be released to the channel pool:
            pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThreadMethod", "Vortex CMD", "vortex_channel_set_received_handler");
            vortex_channel_set_received_handler (channelAttr, senderFrameReceivedCallbackConsumeLatecomer , pThreadData->pLog);

            iRet = TML_ERR_SENDER_COMMUNICATION;
          }
          else{
            /* Start timer now */
            // Timeout is INFINITE at initialization time:
            iRet = StartTimerThread(pThreadData->timerThreadData, pThreadData->eventHandler, pThreadData->senderSyncEventArray, INFINITE, pThreadData->pLog, &threadInfo);
            if (TML_SUCCESS != iRet){
              printf ("#####   StartTimerThread FAILED #####\n");
              iRet = TML_ERR_SENDER_NOT_INITIALIZED;
            }
            else{
              bTimerStarted = true;
            }
          }
          ////////////////////////////
          // free memory for content:
          sidex_Free_Content(content);
          ///////////////////////////////////////////////
          // Now I can release the critical section use: 
          tml_Cmd_Release_Sidex_Handle(callbackData->tmlhandle);
  }
  ////////////////////////////////
  // The return value loop:
  if (TML_SUCCESS == iRet){
          bool bContinue;

#ifdef TIMEOUT_CHECK_PLAUSIBILITY
          char sStartTime[256];
          char sStartDate[256];
          TML_INT64 iStartTime = 0;
#endif // TIMEOUT_CHECK_PLAUSIBILITY
          do{
            bContinue = true;
            // set new Timerthread timeout and restart it:
#ifdef TIMEOUT_CHECK_PLAUSIBILITY
            iStartTime = tmlLogHandler::getTime((char*) sStartDate,256, (char*)sStartTime, 256);
#endif // TIMEOUT_CHECK_PLAUSIBILITY
            RestartTimerThread(pThreadData->timerThreadData, pThreadData->eventHandler, pThreadData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_CONTINUE_INDEX], pThreadData->dwTimeoutValue, pThreadData->pLog);

            // Wait for the first three events or a timeout:
            DWORD dwIndex;
            int iRetValue = pThreadData->eventHandler->WaitForMultipleEvent(pThreadData->senderSyncEventArray, SEND_MSG_SYNC_HANDLES_TIMEOUT_INDEX + 1, INFINITE, &dwIndex);
            if (WAIT_SUCCEEDED == iRetValue){
              TML_INT32 iRetSize;
              switch (dwIndex){
                    break;
                case SEND_MSG_SYNC_HANDLES_STREAM_FRAME_RECEIVE_INDEX:
                    //////////////////////////////////////////////
                    // A stream reply / perform a loop until the queue is empty:
                    do{
                      //////////////////////////////////////////////
                      // This loop will not end in case of an error
                      // because the queue must be emptied / freed
                      // this will be signaled via the last parameter
                      // in HandleAsyncReplyFromStream:
                      iRet = HandleAsyncReplyFromStream (pThreadData->multiAsyncMsg, &iRetSize, iRet);
                      if (TML_SUCCESS == iRet){
                        if (iRetSize > 1){
                          // set new Timerthread timeout and restart it:
#ifdef TIMEOUT_CHECK_PLAUSIBILITY
                          iStartTime = tmlLogHandler::getTime((char*) sStartDate,256, (char*)sStartTime, 256);
#endif // TIMEOUT_CHECK_PLAUSIBILITY
                          RestartTimerThread(pThreadData->timerThreadData, pThreadData->eventHandler, pThreadData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_CONTINUE_INDEX], pThreadData->dwTimeoutValue, pThreadData->pLog);
                        }
                      }
                    }while (1 <= iRetSize);
                    if (TML_SUCCESS == iRet){
                      bContinue = false;// Stay in the loop until command is finished
                    }
                    else{
                      pThreadData->multiAsyncMsg->clearStreamList(true);
                      bContinue = true;// step out
                    }
                    break;
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    // The two following events will both be handled together / no break
                    // It may happen, that we have entrys in the async message queue in case of SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX
                case SEND_MSG_SYNC_HANDLES_ASYNC_FRAME_RECEIVE_INDEX:
                    //////////////////////////////////////////////
                    // In case of a stream reply / perform a loop until the queue is empty:
                    do{
                      iRet = HandleAsyncReplyFromStream (pThreadData->multiAsyncMsg, &iRetSize, iRet);
                      // Her we don't need to restart the timer / think it will be fast enough:
                    }while (1 <= iRetSize);
                    // No break here:
                case SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX:
                    //////////////////////////////////////////////
                    // A message reply / perform a loop until the queue is empty:
                    do{
                      TML_INT64 iState;
                      iRet = pThreadData->multiAsyncMsg->GetNextAsyncMsg(pThreadData->tmlhandle, &iState, &iRetSize);
                      if (iRetSize){
                        if (TMLCOM_CSTATE_PENDING == iState){
                          bContinue = false;// Stay in the loop until command is finished
                          //////////////////////////////////////////////
                          // An async reply:
                          HandleAsyncReply(pThreadData->tmlhandle, pThreadData->pLog, pThreadData->mutexCriticalSection);
                        }
                        else{
                          bContinue = true;// finished
                        }
                        if (iRetSize > 1){
                          // set new Timerthread timeout and restart it:
#ifdef TIMEOUT_CHECK_PLAUSIBILITY
                          iStartTime = tmlLogHandler::getTime((char*) sStartDate,256, (char*)sStartTime, 256);
#endif // TIMEOUT_CHECK_PLAUSIBILITY
                          RestartTimerThread(pThreadData->timerThreadData, pThreadData->eventHandler, pThreadData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_CONTINUE_INDEX], pThreadData->dwTimeoutValue, pThreadData->pLog);
                        }
                      }
                      else{
                        if (SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX == dwIndex){
                          bContinue = false;// Stay in the loop until command is finished
                        }
                      }
                      pThreadData->multiAsyncMsg->ReleaseNextAsyncMsg();
                    }
                    while (iRetSize > 1);
                    break;
                case SEND_MSG_SYNC_HANDLES_CONNECTION_CLOSE_INDEX:
                    ///////////////////////////////////////////////////
                    // In case of Stream I have to clear the queue:
                    pThreadData->connectionObj->getCallbackData(&callbackData);
                    if (CMD_INTERNAL_STREAM_READ == callbackData->cmdValue || CMD_INTERNAL_STREAM_DOWNLOAD == callbackData->cmdValue){
                      pThreadData->multiAsyncMsg->clearStreamList(true);
                    }
                    //////////////////////////////////////////////
                    // The Server has closed the connection:
                    pThreadData->pLog->log ("TMLCoreSender", "AsyncHandlingThread", "Communication", "broken");
                    iRet = TML_ERR_COM_LAYER_CONNECTION_CLOSE;
                    break;
                case SEND_MSG_SYNC_HANDLES_TIMEOUT_INDEX:
                    //////////////////////////////////////////////
                    // Timeout
                    ///////////////////////////////////////////////////
                    // In case of Stream I have to clear the queue:
                    pThreadData->connectionObj->getCallbackData(&callbackData);
                    if (CMD_INTERNAL_STREAM_READ == callbackData->cmdValue || CMD_INTERNAL_STREAM_DOWNLOAD == callbackData->cmdValue){
                      pThreadData->multiAsyncMsg->clearStreamList(true);
                    }
                    pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "AsyncHandlingThread", "ERROR", "Timeout");
                    iRet = TML_ERR_TIMEOUT;
                    break;
                default:
                    ///////////////////////////////////////////////////
                    // In case of Stream I have to clear the queue:
                    pThreadData->connectionObj->getCallbackData(&callbackData);
                    if (CMD_INTERNAL_STREAM_READ == callbackData->cmdValue || CMD_INTERNAL_STREAM_DOWNLOAD == callbackData->cmdValue){
                      pThreadData->multiAsyncMsg->clearStreamList(true);
                    }
                    pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "AsyncHandlingThread", "ERROR", "Undefined dwIndex on WaitForMultipleObjects");
                    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
              }
            }
            else{
              pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "AsyncHandlingThread", "ERROR", "Undefined return value on WaitForMultipleObjects");
              iRet = TML_ERR_SENDER_NOT_INITIALIZED;
            }
          }while (!bContinue);

          ///////////////////////////////////////////////////////////////////////////////////////////////////
          // Any more receive messges shall only be consumed - a channel may be released to the channel pool:
          pThreadData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "AsyncHandlingThread", "Vortex CMD", "vortex_channel_set_received_handler");
          vortex_channel_set_received_handler (channelAttr, senderFrameReceivedCallbackConsumeLatecomer,  pThreadData);
#ifdef TIMEOUT_CHECK_PLAUSIBILITY
          if (TML_ERR_TIMEOUT == iRet){
            char sStopTime[256];
            char sStopDate[256];
            TML_INT64 iStopTime = tmlLogHandler::getTime((char*) sStopDate, 256, (char*)sStopTime, 256);

            tml_logI(0xFFFFFFFF, "tmlCoreSender", "TMLCoreSender_SendMessage", "ERROR", iRet);

            SIDEX_INT64 iTimeout = pThreadData->dwTimeoutValue;
            if (iTimeout <= (iStopTime-iStartTime)){
              tml_logI(0xFFFFFFFF, "tmlCoreSender", "TMLCoreSender_SendMessage:Timeout", "REACHED", iTimeout);
            }
            else{
              tml_log(0xFFFFFFFF, "tmlCoreSender", "TMLCoreSender_SendMessage", "Start", sStartTime);
              tml_log(0xFFFFFFFF, "tmlCoreSender", "TMLCoreSender_SendMessage", "Stop", sStopTime);
              tml_logI(0xFFFFFFFF, "tmlCoreSender", "TMLCoreSender_SendMessage", "Timeoutvalue", iTimeout);
              tml_logI(0xFFFFFFFF, "tmlCoreSender", "TMLCoreSender_SendMessage", "Passed time / not plausible", iStopTime-iStartTime);
            }
          }
#endif // TIMEOUT_CHECK_PLAUSIBILITY
  }

  // I have to set the timeout to INFINITE:
  pThreadData->iRet = iRet;

  TML_ON_COMMAND_READY_CB_FUNC pCBFunc = NULL;
  TML_POINTER pCBData = NULL;
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Get the callback method for the return data:
  if (TML_HANDLE_TYPE_NULL != pThreadData->tmlhandle){
    tml_Cmd_Registered_CommandReady(pThreadData->tmlhandle, &pCBFunc, &pCBData); // Don't mind of return value
    if (NULL != pCBFunc){
      // Map possible error into TML_COMMAND_HANDLE:
      ((tmlObjWrapper*)pThreadData->tmlhandle)->tmlObjWrapper_Header_setLogicalError(iRet, DEFAULT_ERROR_MSG);
      //////////////////////////////////////////////////////
      // Return data to the Async Callback method with data:
      ((void(FUNC_C_DECL *)(TML_COMMAND_HANDLE, TML_POINTER))pCBFunc)(pThreadData->tmlhandle, pCBData);
    }
  }
  /////////////////////////////////////////////////////////////
  // Set flag to stop the timer in the callback, if necessary:
  pThreadData->bStopTimerThread = bTimerStarted;
  pThreadData->threadInfo = &threadInfo;
  //////////////////////////////////////////////////////////////
  // Call the class callback method to handle pending async cmds
  globalCallback(pThreadData->asyncCmdCallbackHandlerMethod,  pThreadData);

  pThreadData->pLog->log (TML_LOG_CORE_IO, "TMLCoreSender", "AsyncHandlingThreadMethod", "End", "");
}


/*********************************************************************************************************************************
*                                             Class definitions:
*********************************************************************************************************************************/


/**
 * @brief    Constructor.
 */
TMLCoreSender::TMLCoreSender(VortexCtx* ctx, tmlLogHandler* loghandler)
{
  m_iErrCode = TML_SUCCESS;
  m_bValidInitialized = false;
  ////////////////////////////////////////////////////
  // Remember the vortex execution context reference:
  m_ctx = ctx;

  m_log = loghandler;

  m_bAnyAsyncCmdPending = false;

  m_eventHandler = new tmlEventHandler(loghandler);
  initSender (loghandler->getLoggingValue());

  m_multiAsyncMsg = NULL;

  m_iLogFileIndex = 0;
  m_AsyncHandlingData.threadInfo.pThreadDef = NULL;
}


/**
 * @brief    Destructor.
 */
TMLCoreSender::~TMLCoreSender()
{
  if (m_bValidInitialized){
    /////////////////////////////////////////////////////////////////////
    // Set connection close to get out of a asyncHandlingThreadMethod:
    TMLCoreSender_ConnectionClose();
    ///////////////////////////////////////////////////////////////
    // Delete Memory of the last call
    // Maybe we have had an async handling thread, so free memory:
    if (NULL != m_AsyncHandlingData.threadInfo.pThreadDef){
      intern_thread_destroy (&m_AsyncHandlingData.threadInfo);
      m_AsyncHandlingData.threadInfo.pThreadDef = NULL;
    }
    /////////////////////////////////////////////////
    // Wait until pending async comd is finished:
    intern_mutex_lock (&m_mutexAsyncThreadSync, m_log, "~TMLCoreSender");
    /////////////////////////////////////////////////
    // Unlock after lock
    intern_mutex_unlock (&m_mutexAsyncThreadSync, m_log, "~TMLCoreSender");

    ////////////////////////////////////////////
    // Free the Handle for async sender list:
    DestroySynchronisationHandle(&m_handleSenderAsyncListProcessed);
  
    ////////////////////////////////////////////////////////////////////////////
    // Free the Handles for thread / callback synchronisation:
    freeSyncEventArray(&m_senderSyncEventArray, NO_OF_SEND_MSG_SYNC_HANDLES);
    delete(m_senderSyncEventArray);

    ////////////////////////////////////////////////////////////////////////////
    // Free the Handles for thread / callback synchronisation:
    freeSyncEventArray(&m_asyncHandlingEventArray, NO_OF_ASYNC_HANDLING_HANDLES);
    delete(m_asyncHandlingEventArray);

    ////////////////////////////////////////////////////////////////////////////
    // Free the reference to a structure to handle connection close of a server:
    delete (m_closeCallbackData);

    ////////////////////////////////////////////////////////////////////////////
    // Free event handler
    delete (m_eventHandler);

    if (NULL != m_ctx){
      ////////////////////////////////////////////////////////////////////////////
      // Destroy the mutex that protect critial section about communication data:
      m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "~TMLCoreSender", "Vortex CMD", "vortex_mutex_destroy");
      intern_mutex_destroy(&m_mutexCriticalSection, (char*)"TMLCoreSender::~TMLCoreSender - m_mutexCriticalSection");
      ////////////////////////////////////////////////////////////////////////////
      // Destroy the mutex that protect critial section referring async cmd list:
      m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "~TMLCoreSender", "Vortex CMD", "vortex_mutex_destroy");
      intern_mutex_destroy(&m_mutexAsyncCmdList, (char*)"TMLCoreSender::~TMLCoreSender - m_mutexAsyncCmdList");
      ////////////////////////////////////////////////////////////////////////////
      // Destroy the mutex for thread termination synchronisation:
      m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "~TMLCoreSender", "Vortex CMD", "vortex_mutex_destroy");
      intern_mutex_destroy(&m_mutexTimerThreadSync, (char*)"TMLCoreSender::~TMLCoreSender - m_mutexTimerThreadSync");
      ////////////////////////////////////////////////////////////////////////////
      // Destroy the mutex for thread termination synchronisation:
      m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "~TMLCoreSender", "Vortex CMD", "vortex_mutex_destroy");
      intern_mutex_destroy(&m_mutexAsyncThreadSync, (char*)"TMLCoreSender::~TMLCoreSender - m_mutexAsyncThreadSync");

      ////////////////////////////////
      // vortex execution context
      m_ctx = NULL;
    }
    if (NULL != m_multiAsyncMsg)
      delete (m_multiAsyncMsg);
    m_bValidInitialized = false;
  }
}


/**
 * @brief    Destroy the HANDLE to be able to signal the end of a frame receive operation via setEvent or connection close etc.
 */
int TMLCoreSender::DestroySynchronisationHandle(const char** handle)
{
  int iRet = TML_SUCCESS;

  if (NULL != *handle){
    if (!m_eventHandler->RemoveEventHandle(*handle)){
      iRet = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }
  else{
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }
  return iRet;
}


/**
 * @brief    The internal class callback method to handle pending async cmds
 */
bool TMLCoreSender::InternalAsyncCmdCallbackHandlerMethod(void *param)
{
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // vortex_channel_pool_release_channel
  AsyncHandlingThreadData* callbackData = (AsyncHandlingThreadData*)param;

  switch (callbackData->iRet){
    case TML_ERR_COM_LAYER_CONNECTION_CLOSE:
          break;
    case TML_ERR_TIMEOUT:
          // New behaviour I do release the channel pool here and do also unlock the connection object
          // "senderFrameReceivedCallbackConsumeLatecomer" will not do this things for me now.
          // No break here:
    default:
          VortexChannelPool* channelPool;
          int iLog = callbackData->pLog->getLoggingValue();

          VortexChannel* channel;
          callbackData->connectionObj->getChannel(&channel);

          callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "InternalAsyncCmdCallbackHandlerMethod", "Vortex CMD", "vortex_channel_get_pool");
          channelPool = vortex_channel_get_pool(channel);
          if (NULL != channelPool){
            callbackData->pLog->log (TML_LOG_VORTEX_CMD,  "TMLCoreSender", "InternalAsyncCmdCallbackHandlerMethod", "Vortex CMD", "vortex_channel_get_number");
            int iChannelNo = vortex_channel_get_number (channel);
            if (!(iLog & TML_LOG_VORTEX_CMD)){
              callbackData->pLog->log (TML_LOG_VORTEX_CH_POOL, "TMLCoreSender", "InternalAsyncCmdCallbackHandlerMethod", "Vortex CMD:vortex_channel_pool_release_channel", iChannelNo);
            }
            callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "InternalAsyncCmdCallbackHandlerMethod", "Vortex CMD:vortex_channel_pool_release_channel", iChannelNo);
            vortex_channel_pool_release_channel(channelPool, channel);
          }
          break;
  }

  /* Stop the timer */
  if (callbackData->bStopTimerThread){
    StopTimerThread(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_STOP_TIMER_INDEX], callbackData, callbackData->threadInfo);
  }
  //////////////////////////////////////////////////////////////////////
  // It was the last async cmd in the queue - signal it:
  SetAsyncCmdProcessing(false);

  //////////////////////////////////////////////////////////////
  // Unlock the connection object to get it usable for others:
  callbackData->connectionObj->unlock();

  return true;
}


/**
 * @brief    The internal class frame receive callback handling method.
 */
bool TMLCoreSender::InternalFrameReceiveHandlerMethod(void *param)
{
  TML_INT64 iState;
  int iRet;
  bool bStreamContent = false;
  TML_INT64 iCmd;
  bool bFrameUnref = true;

  VORTEXSenderFrameReceivedCallbackData* callbackData = (VORTEXSenderFrameReceivedCallbackData*) param;

  if (!callbackData->bValidChain){
    // New data chain (the first reply):
    callbackData->bValidChain = true;
    callbackData->totalFrameSize = 0;
    callbackData->firstFrameListElement = NULL;
    callbackData->lastFrameListElement = NULL;
  }

  TML_INT32 iRetSize = callbackData->iPayloadFrameSize;

  VortexFrameType type = callbackData->type;
  SIDEX_HANDLE sHandle;

  switch (type){
    case VORTEX_FRAME_TYPE_RPY:
      callbackData->pLog->log (TML_LOG_VORTEX_FRAMES, "TMLCoreSender", "InternalFrameReceiveHandlerMethod", "VortexFrameType", "VORTEX_FRAME_TYPE_RPY");
      ///////////////////////////////////////////////
      // One of multiple VortexPayloadFeeder reply's:
      callbackData->pLog->log (TML_LOG_STREAM_HANDLING, "TMLCoreSender", "InternalFrameReceiveHandlerMethod", "Payload size ", iRetSize);
      callbackData->totalFrameSize += iRetSize;
      /////////////////////////////////////////////////
      // Get information about cmd and streaming flag:
      iRet = callbackData->multiAsyncMsg->getCmdFromCallbackData(callbackData, &bStreamContent, &iCmd);
      if (TML_SUCCESS == iRet){
        if (bStreamContent){
          /////////////////////////////////////////////////
          // Put streaming content into a queue:
          iRet = StreamContentIntoQueue(callbackData, iCmd);
          if (TML_SUCCESS == iRet){
            // The frame will be unrefed later in the thread method after data write operation:
            bFrameUnref = false;
            /////////////////////////////////
            // event / the reply is asynchron:
            if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_STREAM_FRAME_RECEIVE_INDEX])){
              // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
              callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent (Stream)");
            }

            if (axl_false == callbackData->bMore){
              ///////////////////////////////////////////////////////////////
              // It was the last one of the download / the same like a NULL:

              //////////////////////////////////////////////////////////////////////////////////
              // In case of raw data totalFrameSize may not match the download content size so:
              totalRawPayloadSizeToHandle(callbackData);

              // Now I have the trigger for the end of a series of replys
              /////////////////////////////////
              // event / the reply is finished:
              if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_ASYNC_FRAME_RECEIVE_INDEX])){
                // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
                callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent");
              }
            }
          }
          else{
            if (TML_ERR_STREAM_DOES_NOT_EXIST != iRet){
              // Set the Error:
              ((tmlObjWrapper*)callbackData->tmlhandle)->tmlObjWrapper_Header_setLogicalError(TML_ERR_COM_LAYER_READ_STATE, DEFAULT_ERROR_MSG);
              /////////////////////////////////
              // event / the reply is finished:
              if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_ASYNC_FRAME_RECEIVE_INDEX])){
                // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
                callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent");
              }
            }
          }
        }
        else{
          if (TML_ERR_STREAM_DOES_NOT_EXIST != iRet){
            callbackData->pLog->log (TML_LOG_STREAM_HANDLING, "InternalFrameReceiveHandlerMethod", "No stream content flag", "shouldn't happpen", "");
            //printf ("InternalFrameReceiveHandlerMethod / No strea content flag - shouldn't happpen CMD = %d!!\n", iCmd);
          }
        }
      }
      else{
        // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
        callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on getCmdFromHandle");
      }
      break;
    case VORTEX_FRAME_TYPE_ANS:
      callbackData->pLog->log (TML_LOG_VORTEX_FRAMES, "TMLCoreSender", "InternalFrameReceiveHandlerMethod", "VortexFrameType", "VORTEX_FRAME_TYPE_ANS");
      ///////////////////////////////////
      // One of multiple / async reply's:
      callbackData->totalFrameSize += iRetSize;
      /////////////////////////////////////////////////
      // Get information about cmd and streaming flag:
      iRet = callbackData->multiAsyncMsg->getCmdFromCallbackData(callbackData, &bStreamContent, &iCmd);
      if (TML_SUCCESS == iRet){
        if (bStreamContent){
          /////////////////////////////////////////////////
          // Put streaming content into a queue:
          iRet = StreamContentIntoQueue(callbackData, iCmd);
          if (TML_SUCCESS == iRet){
            // The frame will be unrefed later in the thread method after data write operation:
            bFrameUnref = false;
            /////////////////////////////////
            // event / the reply is asynchron:
            if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_STREAM_FRAME_RECEIVE_INDEX])){
              // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
              callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent (Stream)");
            }
          }
          else{
            if (TML_ERR_STREAM_DOES_NOT_EXIST != iRet){
              // Set the Error:
              ((tmlObjWrapper*)callbackData->tmlhandle)->tmlObjWrapper_Header_setLogicalError(TML_ERR_COM_LAYER_READ_STATE, DEFAULT_ERROR_MSG);
              /////////////////////////////////
              // event / the reply is finished:
              if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_ASYNC_FRAME_RECEIVE_INDEX])){
                // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
                callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent");
              }
            }
          }
        }
        else{
          ///////////////////////////
          // Get the payload:
          iRet = callbackData->multiAsyncMsg->getSidexFromPayoad(&sHandle, (int)callbackData->iPayloadFrameSize, callbackData->cPayload);
          if (TML_SUCCESS == iRet){
            ///////////////////////////
            // Queue the payload:
            iRet =PayloadContentIntoQueue(callbackData, sHandle, &iState, callbackData->iPayloadFrameSize, callbackData->cPayload);
            if (TML_SUCCESS == iRet){
              if (TMLCOM_CSTATE_PENDING != iState){
                // It must be the last one of a series of replys.
                // In this case no event till the VORTEX_FRAME_TYPE_NUL.
              }
              else{
                // It's an asynchron reply:
                /////////////////////////////////
                // event / the reply is asynchron:
                if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX])){
                  // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
                  callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent (Async)");
                }
              }
            }
            else{
              // Set the Error:
              ((tmlObjWrapper*)callbackData->tmlhandle)->tmlObjWrapper_Header_setLogicalError(TML_ERR_COM_LAYER_READ_STATE, DEFAULT_ERROR_MSG);
              /////////////////////////////////
              // event / the reply is finished:
              if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX])){
                // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
                callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent");
              }
            }
          }
          else{
            // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
            callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on getSidexFromPayoad");
          }
        }
      }
      else{
        // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
        callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on getCmdFromHandle");
      }
      break;
    case VORTEX_FRAME_TYPE_NUL:
      callbackData->pLog->log (TML_LOG_VORTEX_FRAMES, "TMLCoreSender", "InternalFrameReceiveHandlerMethod", "VortexFrameType", "VORTEX_FRAME_TYPE_NUL");
      /////////////////////////////////////////////////
      // Get information about cmd and streaming flag:
      iRet = callbackData->multiAsyncMsg->getCmdFromCallbackData(callbackData, &bStreamContent, &iCmd);
      if (TML_SUCCESS == iRet){
        if (bStreamContent){
          //////////////////////////////////////////////////////////////////////////////////
          // In case of raw data totalFrameSize may not match the download content size so:
          totalRawPayloadSizeToHandle(callbackData);
        }
        // Now I have the trigger for the end of a series of replys
        /////////////////////////////////
        // event / the reply is finished:
        if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_ASYNC_FRAME_RECEIVE_INDEX])){
          // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
          callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent");
        }
      }
      else{
        // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
        callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on getCmdFromHandle");
      }
      break;
    case VORTEX_FRAME_TYPE_ERR:
      callbackData->pLog->log (TML_LOG_VORTEX_FRAMES, "TMLCoreSender", "InternalFrameReceiveHandlerMethod", "VortexFrameType", "VORTEX_FRAME_TYPE_ERR");
      // Set the Error:
      ((tmlObjWrapper*)callbackData->tmlhandle)->tmlObjWrapper_Header_setLogicalError(TML_ERR_COM_LAYER_FRAME_TYPE_IS_ERR, DEFAULT_ERROR_MSG);
      /////////////////////////////////
      // event / the reply is finished:
      if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX])){
        // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
        callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent");
      }
      break;
    default:
      callbackData->pLog->log (TML_LOG_VORTEX_FRAMES, "TMLCoreSender", "InternalFrameReceiveHandlerMethod", "VortexFrameType", "UNKNOWN");
      // Set the Error:
      ((tmlObjWrapper*)callbackData->tmlhandle)->tmlObjWrapper_Header_setLogicalError(TML_ERR_COM_LAYER_FRAME_UNEXP_TYPE, DEFAULT_ERROR_MSG);
      /////////////////////////////////
      // event / the reply is finished:
      if (! m_eventHandler->SetEventOnHandle(callbackData->senderSyncEventArray[SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX])){
        // There is no possibility to inform the sendMessage initiator - I can only write a debug log:
        callbackData->pLog->log ("TMLCoreSender", "InternalFrameReceiveHandlerMethod", "ERROR", "Problem on SetEvent");
      }
      break;
  }

  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "InternalFrameReceiveHandlerMethod", "Vortex CMD", "vortex_frame_unref");
  // Unref the frame:
  if (bFrameUnref){
    vortex_frame_unref (callbackData->pFrame);
  }

  // ATTENTION: async. message reply (progress or status) - see also class tmlCoreListener
  // The VORTEX_FRAME_TYPE_ANS type will be done only once for a command
  // and multiple for async. message reply (progress or status) will also send it's SendFinalAnsReply()
  // So the synchronisation event will be set for each of them:
  return true;
}


/**
     * @brief  In case of raw data the total payload size must match to the stream size
 */
void TMLCoreSender::totalRawPayloadSizeToHandle(VORTEXSenderFrameReceivedCallbackData* callbackData){
  TML_INT32 iRet = TML_SUCCESS;

  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  tml_Cmd_Acquire_Sidex_Handle(callbackData->tmlhandle, &sHandle);
  iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_BYTRES_READ, callbackData->totalFrameSize);
  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  tml_Cmd_Release_Sidex_Handle(callbackData->tmlhandle);
}


/**
 * @brief    Check if we have stream content, in that case do it's work
 */
int StreamContentHandling(SIDEX_HANDLE sHandle, 
                                          TML_INT32         iPayloadFrameSize, 
                                          char*             cPayload,
                                          TML_INT64         totalFrameSize,
                                          VortexFrame*      pFrame,
                                          TML_INT64         cmd,
                                          tmlLogHandler*    m_log,
                                          TML_INT32         iEmptyOnError){
  // if iEmptyOnError != TML_SUCCESS this method will only free allocated memory:                                              
  TML_INT32 iRet = iEmptyOnError;
  if (0 < iPayloadFrameSize && TML_SUCCESS == iEmptyOnError){
    switch (cmd){
      case CMD_INTERNAL_STREAM_READ:
            // Acquire critical section use: 
            //tml_Cmd_Acquire_Sidex_Handle(tmlhandle, &sHandle);
            ////////////////////////////////////////////////
            //it's a raw data transmission read command so:
            iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_BYTRES_READ, iPayloadFrameSize);
            if (TML_SUCCESS == iRet){
              TML_INT64 buffer;
              iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_DEST_BUFFER_ADDR, &buffer);
              if (TML_SUCCESS == iRet){
                char* addr = (char*)buffer;

                m_log->log (TML_LOG_STREAM_HANDLING, "TMLCoreSender", "StreamContentHandling", "Strat copying", "Payload");

                /////////////////////////////////////////////////////////
                // Copy the payload into the buffer:
#if defined (LINUX) || defined (MINGW_BUILD)
                memcpy (addr, cPayload, iPayloadFrameSize);
#else
                memcpy_s(addr, iPayloadFrameSize, cPayload, iPayloadFrameSize);
#endif
                m_log->log (TML_LOG_STREAM_HANDLING, "TMLCoreSender", "StreamContentHandling", "Payload", "copied");
              }
            }
            ///////////////////////////////////////////////
            // Now I can release the critical section use: 
            //tml_Cmd_Release_Sidex_Handle(tmlhandle);
            break;
      case CMD_INTERNAL_STREAM_DOWNLOAD:
            // Acquire critical section use: 
            //tml_Cmd_Acquire_Sidex_Handle(tmlhandle, &sHandle);
            ///////////////////////////////////////////////////
            //it's a raw data transmission download command so:
            iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_BYTRES_READ, iPayloadFrameSize);
            TML_INT64 criticalSectionReceiver;
            TML_INT64 receiverStreamObjs;
            TML_INT64 pCBFunc;
            TML_INT64 pCBData;
            TML_INT64 streamSize;
            TML_INT64 streamID;
            if (TML_SUCCESS == iRet){
              iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, &streamID);
            }
            if (TML_SUCCESS == iRet){
              iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_RECEIVER_HASH, &receiverStreamObjs);
            }
            if (TML_SUCCESS == iRet){
              iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_CRITICAL_SECTION, &criticalSectionReceiver);
            }
            if (TML_SUCCESS == iRet){
              iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_DLD_CALLBACK_FUNC, &pCBFunc);
            }
            if (TML_SUCCESS == iRet){
              iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_DLD_CALLBACK_FUNC_DATA, &pCBData);
            }
            if (TML_SUCCESS == iRet)
              iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_CONTENT_SIZE, &streamSize);

            if (TML_SUCCESS == iRet && 0 != pCBFunc){
              tmlReceiverStream* stream = NULL;
              iRet = ((TMLUniversalHashTable*)receiverStreamObjs)->getValue(streamID, (void**) &stream);
              if (TML_SUCCESS != iRet || NULL == stream){
                m_log->log (TML_LOG_STREAM_HANDLING, "TMLCoreSender", "StreamContentHandling", "CMD_INTERNAL_STREAM_DOWNLOAD", "stream not longer valid");
                iRet = TML_ERR_STREAM_DOES_NOT_EXIST;
              }
              else{
                ///////////////////////////////////////////////////////////////////////////////////////////////
                // Callback in the manner:
                //  int pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER pCBoData, TML_POINTER buffer, TML_INT32 bytresRead, TML_INT64 totalBytesRead, TML_INT64 streamSize)
                //m_log->log (TML_LOG_STREAM_HANDLING, "TMLCoreSender", "StreamContentHandling", "download", "Calling callback");
                //////////////////////////////////////////////////////////
                // Write operation on receiver side is a critical section ;
                ((tmlCriticalSectionObj*)criticalSectionReceiver)->tmlCriticalSectionEnter("StreamContentHandling");
                //printf ("%ld   -   %lld    -   %lld\n", iPayloadFrameSize, totalFrameSize, streamSize);
                iRet = ((TML_INT32(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER, TML_POINTER, TML_INT32, TML_INT64, TML_INT64))(void*)pCBFunc)((TML_STREAM_ID_TYPE)streamID, (TML_POINTER)pCBData, cPayload, iPayloadFrameSize, totalFrameSize, streamSize);
                //printf ("Written %d\n", iRet);
                ((tmlCriticalSectionObj*)criticalSectionReceiver)->tmlCriticalSectionLeave("StreamContentHandling");
                // Write operation on receiver side was a critical section ;
                //////////////////////////////////////////////////////////
                //m_log->log (TML_LOG_STREAM_HANDLING, "TMLCoreSender", "StreamContentHandling", "download", "callback finished");
                switch (iRet){
                  case -1:
                    // -1 fuer stream konnte nicht geschrieben werden
                    iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_DLD_CALLBACK_FUNC, 0);
                    iRet = TML_ERR_STREAM_DOWNLOAD_WRITE_FAILED;
                    break;
                  case -2:
                    // -2 fuer cancel
                    iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_DLD_CALLBACK_FUNC, 0);
                    iRet = TML_ERR_STREAM_DOWNLOAD_CANCELED;
                    break;
                  default:
                    break;
                }
              }
            }
            break;
      default:
           ////////////////////////////////////////////////////////
           //it's a SIDEX data transmission command so do nothing:
           break;
    }
  }
  vortex_frame_unref (pFrame);

  if (SIDEX_HANDLE_TYPE_NULL != sHandle){
    sidex_Free(&sHandle);
  }
  return iRet;
}


/**
 * @brief    Makes a copy of the tmlhandle's SIDEX_HANDLE to have it for queuing
 */
int TMLCoreSender::getSidexForStream(TML_COMMAND_HANDLE tmlhandle, SIDEX_HANDLE* retHandle){
  TML_INT32 iRet = TML_SUCCESS;

  SIDEX_HANDLE sHandleIn = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_HANDLE sHandleOut = SIDEX_HANDLE_TYPE_NULL;
  // Acquire critical section use: 
  tml_Cmd_Acquire_Sidex_Handle(tmlhandle, &sHandleIn);

  iRet = sidex_Create_A("DUMMY", &sHandleOut);
  if (SIDEX_SUCCESS == iRet){
    char* 	content;
    SIDEX_INT32 iContentLength;
    iRet = sidex_Get_Content_A 	(sHandleIn, &content, &iContentLength);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Set_Content(sHandleOut, content);
      sidex_Free_Content(content);
    }
  }
  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  tml_Cmd_Release_Sidex_Handle(tmlhandle);

  *retHandle = sHandleOut;
  return (int)iRet;
}


/**
 * @brief    Write the payload content into a queue to fetch it later in the thread
 */
int TMLCoreSender::PayloadContentIntoQueue(VORTEXSenderFrameReceivedCallbackData* callbackData, SIDEX_HANDLE sHandle, TML_INT64* iState, TML_INT32 iPayloadFrameSize, char* cPayload){
  payloadDataStructure* pStruct = new payloadDataStructure;
  pStruct->sHandle = sHandle;
  pStruct->iPayloadFrameSize = iPayloadFrameSize;
  pStruct->cPayload = cPayload;
  TML_INT32 iRet = callbackData->multiAsyncMsg->DataIntoQueue(pStruct);

  if (TML_SUCCESS == iRet){
    iRet = sidex_Integer_Read (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_STATE, iState);
    if (TML_SUCCESS != iRet){
      // DOWNLAOAD:
      *iState = TMLCOM_CSTATE_UNDEFINED;
      iRet = TML_SUCCESS;
    }
  }

  return iRet;
}


/**
 * @brief    Write the stream content into a queue to fetch it later in the thread
 */
int TMLCoreSender::StreamContentIntoQueue(VORTEXSenderFrameReceivedCallbackData* callbackData, TML_INT64 cmd){
  TML_INT32 iRet = TML_SUCCESS;

  SIDEX_HANDLE sHandle;
  iRet = getSidexForStream(callbackData->tmlhandle, &sHandle);

  if (TML_SUCCESS == iRet){
    streamDataStructure* pStruct = new streamDataStructure;

    pStruct->sHandle = sHandle;
    pStruct->iPayloadFrameSize = callbackData->iPayloadFrameSize;
    pStruct->cPayload = callbackData->cPayload;
    pStruct->totalFrameSize = callbackData->totalFrameSize;
    pStruct->pFrame = callbackData->pFrame;
    pStruct->cmd = cmd;
    pStruct->log = callbackData->pLog;

    callbackData->multiAsyncMsg->DataIntoStreamQueue(pStruct);
  }

  return iRet;
}


/**
 * @brief    Allocate event handles for process synchronisation.
 */
int TMLCoreSender::allocSyncEventArray(const char*** senderSyncEventArray){
  int iRet = TML_ERR_SYSTEMRESOURCES;

  const char** evtArr = new const char*[NO_OF_SEND_MSG_SYNC_HANDLES];
  *senderSyncEventArray = evtArr;

  /////////////////////////////////////////////////////////////////////////////
  // HANDLE for event in signal a fullfilled reply:
  bool synchronisationEventHandle = m_eventHandler->CreateEventHandle(SYNCHRONISATION_EVENT_HANDLE);
  if (synchronisationEventHandle){
    /////////////////////////////////////////////////////////////////////////////
    // HANDLE for event in the case of connection close of a server:
    bool closeSynchronisationEventHandle = m_eventHandler->CreateEventHandle(CLOSE_SYNCHRONISATION_EVENT_HANDLE);
    if (closeSynchronisationEventHandle){
      /////////////////////////////////////////////////////////////////////////////
      // HANDLE for timeout:
      bool timeoutEventHandle = m_eventHandler->CreateEventHandle(TIMEOUT_EVENT_HANDLE);
      if (timeoutEventHandle){
        /////////////////////////////////////////////////////////////////////////////
        // HANDLE stop timeout- Task:
        bool stopTimerEventHandle = m_eventHandler->CreateEventHandle(STOP_TIMER_EVENT_HANDLE);
        if (stopTimerEventHandle){
          /////////////////////////////////////////////////////////////////////////////
          // HANDLE stop timeout- Task:
          bool continueTimerEventHandle = m_eventHandler->CreateEventHandle(CONTINUE_TIMER_EVENT_HANDLE);
          if (continueTimerEventHandle){
            /////////////////////////////////////////////////////////////////////////////
            // HANDLE for event in signal an async reply:
            bool asyncSynchronisationEventHandle = m_eventHandler->CreateEventHandle(ASYNC_SYNCHRONISATION_EVENT_HANDLE);
            if (asyncSynchronisationEventHandle){
              /////////////////////////////////////////////////////////////////////////////
              // HANDLE for event in signal an async reply:
              bool asyncStreamEventHandle = m_eventHandler->CreateEventHandle(ASYNC_STREAM_EVENT_HANDLE);
              if (asyncStreamEventHandle){
                /////////////////////////////////////////
                // handles into the array:
                evtArr[SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX] = SYNCHRONISATION_EVENT_HANDLE;
                evtArr[SEND_MSG_SYNC_HANDLES_ASYNC_FRAME_RECEIVE_INDEX] = ASYNC_SYNCHRONISATION_EVENT_HANDLE;
                evtArr[SEND_MSG_SYNC_HANDLES_CONNECTION_CLOSE_INDEX] = CLOSE_SYNCHRONISATION_EVENT_HANDLE;
                evtArr[SEND_MSG_SYNC_HANDLES_TIMEOUT_INDEX] = TIMEOUT_EVENT_HANDLE;
                evtArr[SEND_MSG_SYNC_HANDLES_STOP_TIMER_INDEX] = STOP_TIMER_EVENT_HANDLE;
                evtArr[SEND_MSG_SYNC_HANDLES_CONTINUE_INDEX] = CONTINUE_TIMER_EVENT_HANDLE;
                evtArr[SEND_MSG_SYNC_HANDLES_STREAM_FRAME_RECEIVE_INDEX] = ASYNC_STREAM_EVENT_HANDLE;

                iRet = TML_SUCCESS;
              }
            }
          }
        }
      }
    }
  }
  return iRet;
}


/**
 * @brief    Allocate event handles for process synchronisation.
 */
int TMLCoreSender::allocAsyncThreadEventArray(const char*** asyncThreadEventArray){
  int iRet = TML_ERR_SENDER_NOT_INITIALIZED;

  const char** evtArr = new const char*[NO_OF_ASYNC_HANDLING_HANDLES];
  *asyncThreadEventArray = evtArr;

  /////////////////////////////////////////////////////////////////////////////
  // HANDLE for event in signal a fullfilled reply:
  bool terminateEventHandle = m_eventHandler->CreateEventHandle(TERMINATE_EVENT_HANDLE);
  if (terminateEventHandle){
    /////////////////////////////////////////////////////////////////////////////
    // HANDLE for event in the case of connection close of a server:
    bool continueEventHandle = m_eventHandler->CreateEventHandle(CONTINUE_EVENT_HANDLE);
    if (continueEventHandle){
      /////////////////////////////////////////
      // handles into the array:
      evtArr[ASYNC_HANDLING_TERMINATE_EVENT] = TERMINATE_EVENT_HANDLE;
      evtArr[ASYNC_HANDLING_CONTINUE_EVENT] = CONTINUE_EVENT_HANDLE;
      iRet = TML_SUCCESS;
    }
  }
  return iRet;
}


/**
 * @brief    Free event handles for process synchronisation.
 */
 void TMLCoreSender::freeSyncEventArray(const char*** senderSyncEventArray, int iLength){
  const char** evtArr = *senderSyncEventArray;
  if (NULL != evtArr && 0 < iLength){
    for (int i = 0; i < iLength; ++i){
      // free the synchronisation handles:
      DestroySynchronisationHandle(&(evtArr[i]));
    }
    ////////////////////////////////////////////////////////////////////////////
    // Free sync event array to handle send message:
    delete (evtArr);
    *senderSyncEventArray = NULL;
  }
}


/**
 * @brief    Reset event handles for process synchronisation.
 */
 int TMLCoreSender::resetSyncEventArray(const char*** senderSyncEventArray, int iLength, bool bInitializeState){
  int iRet = TML_SUCCESS;

  if (bInitializeState){
    const char** evtArr = *senderSyncEventArray;
    if (NULL != evtArr && 0 < iLength){
      for (int i = 0; i < iLength; ++i){
        m_eventHandler->ResetEventOnHandle(evtArr[i]);
      }
    }
    else{
      iRet = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }
  else{
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }
  return iRet;
}


/**
 * @brief    The initialization called by the constructor
 */
void TMLCoreSender::initSender(int iLogValue){

  axl_bool bSuccess = axl_true;
  int iRet;

  ////////////////////////////////////////////////////
  // Async handling event array has initial value NULL
  m_asyncHandlingEventArray = NULL;
  m_timerThreadData.timerSyncEventArray = NULL;

  ////////////////////////////////////////////
  // Alloc the Handle for async sender list:
  bool bValid = m_eventHandler->CreateEventHandle(SENDER_ASYNC_LIST_PROCEEDED_EVENT_HANDLE);
  if (bValid){
    m_handleSenderAsyncListProcessed = SENDER_ASYNC_LIST_PROCEEDED_EVENT_HANDLE;
  }
  else{
    m_handleSenderAsyncListProcessed = NULL;
    bSuccess = axl_false;
    m_iErrCode = TML_ERR_SENDER_NOT_INITIALIZED;
  }
  ////////////////////////////////////////////////////////////////////////////
  // Alloc the Handles for thread / callback synchronisation:
  if (axl_true == bSuccess){
    iRet = allocSyncEventArray(&m_senderSyncEventArray);
    bSuccess = TML_SUCCESS == iRet;
    if (axl_true != bSuccess){
      m_iErrCode = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }
  if (axl_true == bSuccess){
    // Enable Logging:
    iRet = TMLCoreSender_Set_Logging_Value(iLogValue);
    bSuccess = TML_SUCCESS == iRet;
    if (axl_true != bSuccess){
      m_iErrCode = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  if (axl_true == bSuccess){
    m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "initSender", "Vortex CMD", "vortex_mutex_create");
    bSuccess = intern_mutex_create (&m_mutexCriticalSection);
    if (axl_true != bSuccess){
      m_iErrCode = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex to protect critial section  referring async cmd list:
  if (axl_true == bSuccess){
    m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "initSender", "Vortex CMD", "vortex_mutex_create");
    bSuccess = intern_mutex_create (&m_mutexAsyncCmdList);
    if (axl_true != bSuccess){
      m_iErrCode = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex for thread termination synchronisation:
  if (axl_true == bSuccess){
    m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "initSender", "Vortex CMD", "vortex_mutex_create");
    bSuccess = intern_mutex_create (&m_mutexTimerThreadSync);
    if (axl_true != bSuccess){
      m_iErrCode = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex for thread termination synchronisation:
  if (axl_true == bSuccess){
    m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "initSender", "Vortex CMD", "vortex_mutex_create");
    bSuccess = intern_mutex_create (&m_mutexAsyncThreadSync);
    if (axl_true != bSuccess){
      m_iErrCode = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // Start the timer thread only once during sender's lifetime:
  if (axl_true == bSuccess){
    iRet = resetSyncEventArray(&m_senderSyncEventArray, NO_OF_SEND_MSG_SYNC_HANDLES, true);
    if (TML_SUCCESS != iRet){
      m_iErrCode = TML_ERR_SENDER_NOT_INITIALIZED;
    }
    if (TML_SUCCESS != iRet){
      bSuccess = axl_false;
    }
  }

  if (axl_true == bSuccess){
    /////////////////////////////////////////////////////////////////////////////
    //  init the internal class callback method to handle frame receive
    m_internalFrameReceiveHandlerMethod.SetCallback(this, &TMLCoreSender::InternalFrameReceiveHandlerMethod);
    /////////////////////////////////////////////////////////////////////////////
    //  init the internal class callback method to handle pending async cmds
    m_internalAsyncCmdCallbackHandlerMethod.SetCallback(this, &TMLCoreSender::InternalAsyncCmdCallbackHandlerMethod);
    /////////////////////////////////////////////////////////////////////////////
    // Init the reference to a structure to handle connection close of a server:
    m_closeCallbackData = new VORTEXSenderConnectionCloseCallbackData;
    m_closeCallbackData->senderSyncEventArray = m_senderSyncEventArray;
    m_closeCallbackData->pLog = m_log;
    m_bValidInitialized = true;
  }
  else{
    m_bValidInitialized = false;
  }
}


/**
 * @brief    Returns the error code that will be set during construction / initSender()
 */
int TMLCoreSender::getInitError(){
  return m_iErrCode;
}


/**
 * @brief Set the state of async cmd processing.
 */
void TMLCoreSender::SetAsyncCmdProcessing(bool bSet){
  m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "SetAsyncCmdProcessing", "Vortex CMD", "vortex_mutex_lock");
  intern_mutex_lock (&m_mutexAsyncCmdList, m_log, "SetAsyncCmdProcessing");
  if (bSet){
    m_bAnyAsyncCmdPending = true;
    if (! m_eventHandler->ResetEventOnHandle(m_handleSenderAsyncListProcessed)){
       m_log->log ("TMLCoreSender", "SetAsyncCmdProcessing", "ResetEvent", "failed");
    }
  }
  else{
     m_bAnyAsyncCmdPending = false;
     if (! m_eventHandler->SetEventOnHandle(m_handleSenderAsyncListProcessed)){
       m_log->log ("TMLCoreSender", "SetAsyncCmdProcessing", "SetEvent", "failed");
     }
  }
  m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "SetAsyncCmdProcessing", "Vortex CMD", "vortex_mutex_unlock");
  intern_mutex_unlock (&m_mutexAsyncCmdList, m_log, "SetAsyncCmdProcessing");
}


/**
 * @brief    Wait until pending async commands are finished
 */
int TMLCoreSender::TMLCoreSender_WaitForPendingAsyncCmd(){
  int iRet = TML_SUCCESS;
  bool bCommandIsPending;
  
  if (m_bValidInitialized){
    m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "WaitForPendingAsyncCmdsAreFinished", "Vortex CMD", "vortex_mutex_lock");
    intern_mutex_lock (&m_mutexAsyncCmdList, m_log, "WaitForPendingAsyncCmdsAreFinished");

    bCommandIsPending = m_bAnyAsyncCmdPending;

    m_log->log (TML_LOG_VORTEX_MUTEX, "TMLCoreSender", "WaitForPendingAsyncCmdsAreFinished", "Vortex CMD", "vortex_mutex_unlock");
    intern_mutex_unlock (&m_mutexAsyncCmdList, m_log, "WaitForPendingAsyncCmdsAreFinished");

    if (bCommandIsPending){
      m_log->log (TML_LOG_CORE_IO, "TMLCoreSender", "WaitForPendingAsyncCmdsAreFinished", "Wait", "until all async cmd's are finished");
      int iResult = m_eventHandler->WaitForSingleEvent(m_handleSenderAsyncListProcessed, 1000);
      if (WAIT_TOUT == iResult){
        iRet = TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC;
      }
      m_log->log (TML_LOG_CORE_IO, "TMLCoreSender", "WaitForPendingAsyncCmdsAreFinished", "Now", "all async cmd's are finished");
    }
  }
  return iRet;
}
 

/**
 * @brief   Set the debug logging value.
 */
int TMLCoreSender::TMLCoreSender_Set_Logging_Value(int iLogValue)
{
  int iRet = TML_SUCCESS;

  if (NULL != m_ctx){
    // enable log to see whats going on on VORTEX:
    // Map Vortex logging into my own
    //m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "TMLCoreSender_EnableLogging", "vortex_log_set_handler");
    //vortex_log_set_handler (m_ctx,	myVortexLoghandler);
    // VORTEX- Logging will be made via Environment setting "set VORTEX_DEBUG=1"
    //m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "TMLCoreSender_EnableLogging", "Vortex CMD", "vortex_log_enable");
    //bool bEnable;
    //bEnable = (TML_LOG_VORTEX_INTERN & iLogValue) != 0;
    // VORTEX- Logging will be made via Environment setting "set VORTEX_DEBUG=1"
    //vortex_log_enable (m_ctx, bEnable);
    // VORTEX- Logging will be made via Environment setting "set VORTEX_DEBUG=1"
    //m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "TMLCoreSender_EnableLogging", "Vortex CMD", "vortex_log2_enable");
    //bEnable = (TML_LOG_VORTEX_VERBOSE & iLogValue) != 0;
    // VORTEX- Logging will be made via Environment setting "set VORTEX_DEBUG=1"
    //vortex_log2_enable (m_ctx, bEnable);
    //m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreSender", "TMLCoreSender_EnableLogging", "Vortex CMD", "vortex_log_filter_level");
    //vortex_log_filter_level (m_ctx, "debug, warning");
  }
  else
  {
    iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
  }
  return iRet;
}


/**
 * @brief    Send a sync. message.
 */
int TMLCoreSender::TMLCoreSender_SendMessage(tmlConnectionObj* pConnectionObj, TML_COMMAND_HANDLE tmlhandle, int iTimeout, int* iMsgNo)
{
  TML_INT32  iRet = TML_SUCCESS;

  ////////////////////////////////////////////
  // Wait until async sender thread is finish:
  TMLCoreSender_WaitForPendingAsyncCmd();

  SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
  if (TML_SUCCESS == iRet){
    // Is there a valid vortex execution context and a valid channel:
    if (NULL != m_ctx){
      ////////////////////////////////////////////////////////////////////////////////////////////////
      // Begin of critical section / a send command may not be possible during this method execution :
      ///////////////////////////////////////////////
      // Acquire critical section use: 
      tml_Cmd_Acquire_Sidex_Handle(tmlhandle, &sHandle);
      /////////////////////////////////////////////////////////
      // The command identification:
      TML_INT64 cmdValue;
      if (TML_SUCCESS == iRet){
        iRet = sidex_Integer_Read (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_COMMAND, &cmdValue);
      }
      /////////////////////////////////////////////////////////
      // Is it an internal command:
      TML_BOOL bInternal = TML_FALSE;
      /////////////////////////////////////////////////////////
      // Don't mind of the return value here:
      sidex_Boolean_Read (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, &bInternal);
      ///////////////////////////////////////////////
      // Now I can release the critical section use: 
      tml_Cmd_Release_Sidex_Handle(tmlhandle);
      if (TML_SUCCESS == iRet){
        iRet = resetSyncEventArray(&m_senderSyncEventArray, NO_OF_SEND_MSG_SYNC_HANDLES, m_bValidInitialized);
        if (TML_SUCCESS == iRet){
          // alloc the callback data struct:
          VORTEXSenderFrameReceivedCallbackData* callbackData;
          pConnectionObj->getCallbackData(&callbackData);
          if (NULL != m_multiAsyncMsg)
            delete m_multiAsyncMsg;
          m_multiAsyncMsg = new tmlMultiAsyncMsg();
          callbackData->multiAsyncMsg = m_multiAsyncMsg;
          callbackData->cmdValue = cmdValue;
          callbackData->bInternal = bInternal;
          callbackData->pLog = m_log; // logging
          // no return data at this moment / chain is invalid:
          callbackData->bValidChain = false;
          callbackData->senderSyncEventArray = m_senderSyncEventArray;
          // Pointer to the class callback handling method:
          callbackData->callback = &m_internalFrameReceiveHandlerMethod;
          // the object to return:
          callbackData->tmlhandle = tmlhandle;
          // fill m_AsyncHandlingThreadData and call AsyncHandlingThreadMethod:
          m_AsyncHandlingThreadData.pLog = m_log;
          m_AsyncHandlingThreadData.senderSyncEventArray = m_senderSyncEventArray;
          m_AsyncHandlingThreadData.dwTimeoutValue = iTimeout;
          m_AsyncHandlingThreadData.timerThreadData = &m_timerThreadData;
          m_AsyncHandlingThreadData.tmlhandle = tmlhandle;
          m_AsyncHandlingThreadData.mutexCriticalSection = &m_mutexCriticalSection;
          m_AsyncHandlingThreadData.connectionObj = pConnectionObj;
          m_AsyncHandlingThreadData.asyncCmdCallbackHandlerMethod = &m_internalAsyncCmdCallbackHandlerMethod;
          m_AsyncHandlingThreadData.eventHandler = m_eventHandler;
          m_AsyncHandlingThreadData.multiAsyncMsg = m_multiAsyncMsg;
          m_AsyncHandlingThreadData.timerTerminationMutex = &m_mutexTimerThreadSync;
          ////////////////////////////////////////////
          // there is an active async command processing:
          SetAsyncCmdProcessing(true);
          ////////////////////////////////////////////
          // AsyncHandlingThreadMethod as method:
          AsyncHandlingThreadMethod(&m_AsyncHandlingThreadData);
          // Return value:
          iRet = m_AsyncHandlingThreadData.iRet;
        }
      }
      else{
        iRet = TML_ERR_SENDER_NOT_INITIALIZED;
      }
    }
    else{
      iRet = TML_ERR_SENDER_NOT_INITIALIZED;
    }
  }
   
  ///////////////////////////////////////////////////////////////////////////////////////////
  // fetch possible communication errors out of the return XML (set by setErrorOnFrameReceive)
  TML_INT32 iErrCode = TML_SUCCESS;
  if (TML_SUCCESS == iRet){
    TML_INT32 iState;
    if (TML_SUCCESS == tml_Cmd_Header_GetState(tmlhandle, &iState)){
      if (TMLCOM_CSTATE_FAILED == iState){
        if (TML_SUCCESS == tml_Cmd_Header_GetError(tmlhandle, &iErrCode)){
          if (TML_SUCCESS != iErrCode){
            // Map internal communication error code:
            iRet = iErrCode;
          }
        }
      }
    }
  }
  // channel pool has been released in InternalAsyncCmdCallbackHandlerMethod() - also for sync messages - , so not necessary on here
  return (int)iRet;
}


/**
 * @brief    Send an async. message.
 */
int TMLCoreSender::TMLCoreSender_SendAsyncMessage(tmlConnectionObj* pConnectionObj, TML_COMMAND_HANDLE tmlhandle, int iTimeout){
  return (int)TMLCoreSender_SendAsyncMessage(pConnectionObj, tmlhandle, iTimeout, true);
}


/**
 * @brief    Send an async. message.
 */
int TMLCoreSender::TMLCoreSender_SendAsyncMessage(tmlConnectionObj* pConnectionObj, TML_COMMAND_HANDLE tmlhandle, int iTimeout, bool bCheck)
{
   TML_INT32 iRet = TML_SUCCESS;

    // Is there a valid vortex execution context and a valid channel:
    if (NULL != m_ctx){
      ////////////////////////////////////////////////////////////////////////////////////////////////
      // Begin of critical section / a send command may not be possible during this method execution :
      ///////////////////////////////////////////////
      // Acquire critical section use: 
      SIDEX_HANDLE sHandle;
      tml_Cmd_Acquire_Sidex_Handle(tmlhandle, &sHandle);
      /////////////////////////////////////////////////////////
      // The command identification:
      TML_INT64 cmdValue;
      if (TML_SUCCESS == iRet){
        iRet = sidex_Integer_Read (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_COMMAND, &cmdValue);
      }
      /////////////////////////////////////////////////////////
      // Is it an internal command:
      TML_BOOL bInternal = TML_FALSE;
      /////////////////////////////////////////////////////////
      // Don't mind of the return value here:
      sidex_Boolean_Read (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, &bInternal);
      ///////////////////////////////////////////////
      // Now I can release the critical section use: 
      tml_Cmd_Release_Sidex_Handle(tmlhandle);
      if (TML_SUCCESS == iRet){
        iRet = resetSyncEventArray(&m_senderSyncEventArray, NO_OF_SEND_MSG_SYNC_HANDLES, m_bValidInitialized);
        if (TML_SUCCESS == iRet){
          // alloc the callback data struct:
          VORTEXSenderFrameReceivedCallbackData* callbackData;
          pConnectionObj->getCallbackData(&callbackData);
          if (NULL != m_multiAsyncMsg)
            delete m_multiAsyncMsg;
          m_multiAsyncMsg = new tmlMultiAsyncMsg();
          callbackData->multiAsyncMsg = m_multiAsyncMsg;
          callbackData->cmdValue = cmdValue;
          callbackData->bInternal = bInternal;
          callbackData->pLog = m_log; // logging
          // no return data at this moment / chain is invalid:
          callbackData->bValidChain = false;
          callbackData->senderSyncEventArray = m_senderSyncEventArray;
          // Pointer to the class callback handling method:
          callbackData->callback = &m_internalFrameReceiveHandlerMethod;
          // the object to return:
          callbackData->tmlhandle = tmlhandle;
          // fill m_AsyncHandlingThreadData and call AsyncHandlingThreadMethod:
          m_AsyncHandlingThreadData.pLog = m_log;
          m_AsyncHandlingThreadData.senderSyncEventArray = m_senderSyncEventArray;
          m_AsyncHandlingThreadData.dwTimeoutValue = iTimeout;
          m_AsyncHandlingThreadData.timerThreadData = &m_timerThreadData;
          m_AsyncHandlingThreadData.tmlhandle = tmlhandle;
          m_AsyncHandlingThreadData.mutexCriticalSection = &m_mutexCriticalSection;
          m_AsyncHandlingThreadData.connectionObj = pConnectionObj;
          m_AsyncHandlingThreadData.asyncCmdCallbackHandlerMethod = &m_internalAsyncCmdCallbackHandlerMethod;
          m_AsyncHandlingThreadData.eventHandler = m_eventHandler;
          m_AsyncHandlingThreadData.multiAsyncMsg = m_multiAsyncMsg;
          m_AsyncHandlingThreadData.timerTerminationMutex = &m_mutexTimerThreadSync;
          ////////////////////////////////////////////
          // there is an active async command processing:
          SetAsyncCmdProcessing(true);
          ////////////////////////////////////////////
          // AsyncHandlingThreadMethod as thread:
          ////////////////////////////////////////////////
          // Use the one and only thread that life's once:
          iRet = RestartAsyncHandlingThread(&m_AsyncHandlingData, &m_asyncHandlingEventArray, &m_AsyncHandlingThreadData, m_log);
        }
      }
      else{
        iRet = TML_ERR_SENDER_NOT_INITIALIZED;
      }
    }
    else{
      iRet = TML_ERR_SENDER_NOT_INITIALIZED;
    }
    return (int)iRet;
}


/**
 * @brief  tmlCoreWrapper will call this method in case of a close of the connection (initiated by the listener)
 */
void TMLCoreSender::TMLCoreSender_ConnectionClose()
{
  if (NULL != m_senderSyncEventArray){
      m_log->log (TML_LOG_CORE_IO, "TMLCoreSender", "TMLCoreSender_ConnectionClose", "Connection closed by listener", "terminate a pending data upload !");
      m_eventHandler->SetEventOnHandle(m_senderSyncEventArray[SEND_MSG_SYNC_HANDLES_CONNECTION_CLOSE_INDEX]);
  }
}


/**
 * @brief   Set the log- file index for explicit logs with closing the file
 */
void TMLCoreSender::setLogFileIndex(int iLogFileIndex){
  m_iLogFileIndex = iLogFileIndex;
}
