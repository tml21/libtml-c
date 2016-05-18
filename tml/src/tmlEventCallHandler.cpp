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

#include "tmlErrors.h"
#include "tmlGlobalCallback.h"
#include "tmlEventCallHandler.h"
#include "tmlEventDataQueue.h"
#include "tmlCoreSender.h"
#include "logValues.h"

/**
 * @brief  Thread to handle async messages / only one during lifetime
 */
FUNC_STDCALL EventHandlingThread (axlPointer pParam)
{
  EventMsgHandlingParams* pThreadHandlingParams = (EventMsgHandlingParams*)pParam;
  pThreadHandlingParams->threadInfo->bThreadStarted = true;
  tmlLogHandler* pLog = pThreadHandlingParams->pLog;

  pLog->log (TML_LOG_CORE_IO, "tmlEventCallHandler", "EventHandlingThread", "EventHandlingThread", "start");

  bool bTerminateThread = false;

  do
  {
    DWORD dwIndex;
    int iRetValue = pThreadHandlingParams->eventHandler->WaitForMultipleEvent(pThreadHandlingParams->handlingEventArray, 2, INFINITE, &dwIndex);

  if (WAIT_SUCCEEDED == iRetValue){
      bool bRet = true;
      switch (dwIndex){
        case EVENT_HANDLING_MESSAGE_CONTINUE_EVENT:
          // Continue - New Event Message
          pLog->log (TML_LOG_CORE_IO, "tmlEventCallHandler", "EventHandlingThread", "EventHandlingThread", "continue");
          ////////////////////////////////////////////////////////////
          // Do the work in a loop until there is no more queue entry:
          do{
            EventMsgExecutionData* data;
            bRet = pThreadHandlingParams->pDataQueue->getDataQueueFrom(&data, true);
            if (bRet){
              globalCallback(pThreadHandlingParams->pCBFunc, data);
            }
          }
          while (bRet);
          break;
        case EVENT_HANDLING_MESSAGE_TERMINATE_EVENT:
          pLog->log (TML_LOG_CORE_IO, "tmlEventCallHandler", "EventHandlingThread", "Handle", "EVENT_HANDLING_MESSAGE_TERMINATE_EVENT");
          ////////////////////////////////////////////////////////////
          // Do the work in a loop until there is no more queue entry:
          do{
            EventMsgExecutionData* data;
            bRet = pThreadHandlingParams->pDataQueue->getDataQueueFrom(&data, true);
            if (bRet){
              globalCallback(pThreadHandlingParams->pCBFunc, data);
            }
          }
          while (bRet);
          // Termination:
          bTerminateThread = true;
          break;
        default:
          // abandoned - terminate:
          pLog->log (TML_LOG_CORE_IO, "tmlEventCallHandler", "EventHandlingThread", "Handle", "abandoned");
          bTerminateThread = false;
          break;
      }
    }
    else{
      // abandoned - terminate:
      pLog->log (TML_LOG_CORE_IO, "tmlEventCallHandler", "EventHandlingThread", "Handle", "abandoned");
      bTerminateThread = false;
    }
  }while (!bTerminateThread);

  // Thread end
  pLog->log (TML_LOG_CORE_IO, "tmlEventCallHandler", "EventHandlingThread", "EventHandlingThread", "terminated");

  pThreadHandlingParams->threadInfo->bThreadStarted = false;
  return 0;
}


/**
 * @brief    Constructor.
 */
tmlEventCallHandler::tmlEventCallHandler(tmlLogHandler* loghandler, void* pCBFuncHandler, void* pCBFuncExec)
{
  m_log = loghandler;
  m_pCBFunc = pCBFuncHandler;

  ////////////////////////////////////////////////////
  // Async handling event array has initial value NULL
  m_eventMessageHandlingEventArray = NULL;

  m_eventHandler = new tmlEventHandler(loghandler);

  ////////////////////////////////////////////////////////////////////////////
  // No thread started at this momemt
  m_threadInfo.bThreadStarted = false;
}


/**
 * @brief    Destructor.
 */
tmlEventCallHandler::~tmlEventCallHandler()
{
  ////////////////////////////////////////////////////////////////////////////
  // Stop the async sender thread and wait until it is finished:
  stopEventMessageHandlingThread();

  ////////////////////////////////////////////////////////////////////////////
  // Free the Handles for thread / callback synchronisation:
  freeEventMessageEventArray(&m_eventMessageHandlingEventArray, NO_OF_EVENT_MESSAGE_HANDLING_HANDLES);
  delete(m_eventMessageHandlingEventArray);
  delete(m_eventHandler);
}


/**
 * @brief    A new event message has to be operated in the background
 */
int tmlEventCallHandler::performBackendEventMessage(EventMsgHandlingParams* threadHandlingParams, EventMsgExecutionData* data)
{
  const char*** handlingEventArray = &m_eventMessageHandlingEventArray;
  int iRet = TML_SUCCESS;
  if (NULL == *handlingEventArray){
    ////////////////////////////////////////////////////////////////////////////
    // It's the first time of a backend message, so I  have to allocate the attributes memory:
    iRet = allocEventMessageThreadEventArray(handlingEventArray);
    if (TML_SUCCESS == iRet){
      ////////////////////////////////////////////////////////////////////////////
      // Reset the event array:
      iRet = resetSyncEventArray(handlingEventArray, NO_OF_EVENT_MESSAGE_HANDLING_HANDLES);
      if (TML_SUCCESS == iRet){
        ////////////////////////////////////////////////////////////////////////////
        // Start the async thread only once during sender's lifetime:
        iRet = startEventMessageHandlingThread(threadHandlingParams, m_eventHandler, *handlingEventArray, m_log, m_pCBFunc, &m_threadInfo);
      }
    }
  }
  if (TML_SUCCESS == iRet){
    //  20-09-12 / This will be done now in tmlCollectCall::eventSendMessage
    // 25-09-12 YES
    threadHandlingParams->pDataQueue->addData2Queue(data);
    ////////////////////////////////////////////////////////////////////////////
    // Set the event to signal new backend event message reqest:
    m_log->log (TML_LOG_CORE_IO, "tmlEventCallHandler", "backendEventMessageHandling", "EventHandlingThread", "New event message request");
    if (! m_eventHandler->SetEventOnHandle((*handlingEventArray)[EVENT_HANDLING_MESSAGE_CONTINUE_EVENT])){
      m_log->log ("tmlEventCallHandler", "backendEventMessageHandling", "SetEvent", "failed");
    }
  }
  return iRet;
}


/**
 * @brief    Allocate event handles for process synchronisation.
 */
int tmlEventCallHandler::allocEventMessageThreadEventArray(const char*** asyncThreadEventArray){
  int iRet = TML_ERR_SYSTEMRESOURCES;

  const char** evtArr = new const char*[NO_OF_EVENT_MESSAGE_HANDLING_HANDLES];
  *asyncThreadEventArray = evtArr;

  /////////////////////////////////////////////////////////////////////////////
  // HANDLE for event in signal a fullfilled reply:
  bool terminateEventHandle = m_eventHandler->CreateEventHandle(TERMINATE_EVENT_HANDLER_MESSAGE_HANDLE);
  if (terminateEventHandle){
    /////////////////////////////////////////////////////////////////////////////
    // HANDLE for event in the case of connection close of a server:
    bool continueEventHandle = m_eventHandler->CreateEventHandle(CONTINUE_EVENT_HANDLER_MESSAGE_HANDLE);
    if (continueEventHandle){
      /////////////////////////////////////////
      // handles into the array:
      evtArr[EVENT_HANDLING_MESSAGE_TERMINATE_EVENT] = TERMINATE_EVENT_HANDLER_MESSAGE_HANDLE;
      evtArr[EVENT_HANDLING_MESSAGE_CONTINUE_EVENT] = CONTINUE_EVENT_HANDLER_MESSAGE_HANDLE;
      iRet = TML_SUCCESS;
    }
  }
  return iRet;
}


/**
 * @brief    Free event handles for process synchronisation.
 */
 void tmlEventCallHandler::freeEventMessageEventArray(const char*** senderSyncEventArray, int iLength){
  const char** evtArr = *senderSyncEventArray;
  if (NULL != evtArr && 0 < iLength){
    for (int i = 0; i < iLength; ++i){
      // free the synchronisation handles:
      destroySynchronisationHandle(&(evtArr[i]));
    }
    ////////////////////////////////////////////////////////////////////////////
    // Free sync event array to handle send message:
    delete[] evtArr;
    *senderSyncEventArray = NULL;
  }
}

 
/**
 * @brief    Destroy the HANDLE to be able to signal the end of a frame receive operation via setEvent or connection close etc.
 */
int tmlEventCallHandler::destroySynchronisationHandle(const char** handle)
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
 * @brief    Reset event handles for process synchronisation.
 */
 int tmlEventCallHandler::resetSyncEventArray(const char*** senderSyncEventArray, int iLength){
  int iRet = TML_SUCCESS;

  const char** evtArr = *senderSyncEventArray;
  if (NULL != evtArr && 0 < iLength){
    for (int i = 0; i < iLength; ++i){
      m_eventHandler->ResetEventOnHandle(evtArr[i]);
    }
  }
  else{
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }
  return iRet;
}


/**
 * @brief    Start the EventMessageHandlingThread.
 */
int tmlEventCallHandler::startEventMessageHandlingThread(EventMsgHandlingParams* threadData, tmlEventHandler* eventHandler, const char** handlingEventArray, tmlLogHandler* pLog, void* pCBFunc, TMLThreadDef* threadInfo)
{
  int iRet = TML_SUCCESS;

  threadData->handlingEventArray = handlingEventArray;
  threadData->pLog = pLog;
  threadData->eventHandler = eventHandler;
  threadData->terminationMutex = NULL;
  threadData->pCBFunc = pCBFunc;
  threadData->threadInfo = threadInfo;

  pLog->log (TML_LOG_VORTEX_CMD, "tmlEventCallHandler", "StartEventMessageHandlingThread", "Vortex CMD", "vortex_thread_create");
    axl_bool bSuccess = intern_thread_create(threadInfo, EventHandlingThread, threadData);
  // Thread- generation log
  if (!bSuccess){
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }
  return iRet;
}


/**
 * @brief    Stop the EventMessageHandlingThread.
 */
void tmlEventCallHandler::stopEventMessageHandlingThread()
{
  if (m_threadInfo.bThreadStarted){
    m_log->log (TML_LOG_CORE_IO, "tmlEventCallHandler", "StopEventMessageHandlingThread", "EventHandlingThread", "Stop");
    if (! m_eventHandler->SetEventOnHandle(m_eventMessageHandlingEventArray[EVENT_HANDLING_MESSAGE_TERMINATE_EVENT])){
      m_log->log ("tmlEventCallHandler", "StopEventMessageHandlingThread", "SetEvent", "failed");
    }
    else{
      /////////////////////////////////////
      // Wait until the thread is finished:
      m_log->log (TML_LOG_VORTEX_MUTEX, "tmlEventCallHandler", "stopEventMessageHandlingThread", "Vortex CMD", "vortex_thread_destroy");
      axl_bool bRet = intern_thread_destroy(&m_threadInfo);
      if (AXL_FALSE == bRet){
        printf ("#### tmlEventCallhandler - stopEventMessageHandlingThread - Error in \"vortex_thread_destroy\" ####\n");
      }
      m_log->log (TML_LOG_VORTEX_MUTEX, "tmlEventCallHandler", "stopEventMessageHandlingThread", "Vortex CMD", "vortex_thread_destroy done");
    }
  }
}

