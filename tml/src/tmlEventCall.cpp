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
#include "tmlEventCall.h"
#include "tmlEventDataQueue.h"
#include "sidex.h"
#include "tmlCore.h"
#include "tmlCoreSender.h"
#include "systemBase.h"
#include "logValues.h"

/**
 * @brief  Thread to handle async messages / only one during lifetime
 */
FUNC_STDCALL EventMsgHandlingThread (axlPointer pParam)
{
  EventMsgHandlingParams* pThreadHandlingParams = (EventMsgHandlingParams*)pParam;
  pThreadHandlingParams->threadInfo->iThreadStatus = THREAD_IN_PROCESS;
  tmlLogHandler* pLog = pThreadHandlingParams->pLog;

  pLog->log (TML_LOG_CORE_IO, "tmlEventCall", "EventMsgHandlingThread", "EventMsgHandlingThread", "start");

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
          pLog->log (TML_LOG_CORE_IO, "tmlEventCall", "EventMsgHandlingThread", "EventMsgHandlingThread", "continue");
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
          pLog->log (TML_LOG_CORE_IO, "tmlEventCall", "EventMsgHandlingThread", "Handle", "abandoned");
          bTerminateThread = true;
          break;
      }
    }
    else{
      // abandoned - terminate:
      pLog->log (TML_LOG_CORE_IO, "tmlEventCall", "EventMsgHandlingThread", "Handle", "abandoned");
      bTerminateThread = true;
    }
  }while (!bTerminateThread);

  // Thread end
  pLog->log (TML_LOG_CORE_IO, "tmlEventCall", "EventMsgHandlingThread", "EventMsgHandlingThread", "terminated");

  pThreadHandlingParams->threadInfo->iThreadStatus = THREAD_STOPPED;
  return 0;
}


/**
 * @brief    Constructor.
 */
tmlEventCall::tmlEventCall(tmlLogHandler* loghandler, void* pCBFunc)
{
  m_log = loghandler;
  m_pCBFunc = pCBFunc;

  ////////////////////////////////////////////////////
  // Async handling event array has initial value NULL
  m_eventMessageHandlingEventArray = NULL;

  m_eventHandler = new tmlEventHandler(loghandler);

  ////////////////////////////////////////////////////////////////////////////
  // No thread started at this momemt
  m_threadInfo.iThreadStatus = THREAD_STOPPED;

  ////////////////////////////////////////////////////////////////////////////
  // create the queue containing the background event message data
  m_EventMessageHandlingParams.pDataQueue = new tmlEventDataQueue(loghandler);

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  axl_bool bSuccess = createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlEventCall", "tmlEventCall", "Vortex CMD", "vortex_mutex_create");
  m_iMutexCriticalSectionLockCount = 0;
  m_bInShutdown = false;
}


/**
 * @brief    Destructor.
 */
tmlEventCall::~tmlEventCall()
{
  m_bInShutdown = true;
  /////////////////////////////////////
  // stop and wait until the thread is finished:
  stopEventMessageHandlingThread();

  ////////////////////////////////////////////////////////////////////////////
  // Free the Handles for thread / callback synchronisation:
  freeEventMessageEventArray(&m_eventMessageHandlingEventArray, NO_OF_EVENT_MESSAGE_HANDLING_HANDLES);

  delete(m_eventMessageHandlingEventArray);
  delete(m_eventHandler);

  ////////////////////////////////////////////////////////////////////////////
  // Clear the queue containing the background event message data first,
  // to be sure, that no more event messages will be started in the backgound
  m_EventMessageHandlingParams.pDataQueue->clear();

  ////////////////////////////////////////////////////////////////////////////
  //  Now destroy the queue containing the background event message data:
  delete (m_EventMessageHandlingParams.pDataQueue);

  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlEventCall", "~tmlEventCall", "Vortex CMD", "vortex_mutex_destroy");
}


/**
 * @brief    Return the reference to the event data queue
 */
tmlEventDataQueue* tmlEventCall::getEventMessageQueue(){
  return m_EventMessageHandlingParams.pDataQueue;
}


/**
 * @brief    A new event message has to be operated in the background
 */
int tmlEventCall::performBackendEventMessage(void* pCBFunc, EventMsgExecutionData* data)
{
  const char*** handlingEventArray = &m_eventMessageHandlingEventArray;
  int iRet = TML_SUCCESS;

  ///////////////////////////////////////////////////////
  // START PREPARE THREAD- ATTRIBUTES FIRST TIME
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
        iRet = startEventMessageHandlingThread(&m_EventMessageHandlingParams, m_eventHandler, *handlingEventArray, m_log, pCBFunc, &m_threadInfo);
      }
    }
  }
  // END PREPARE THREAD- ATTRIBUTES FIRST TIME
  ///////////////////////////////////////////////////////



  EventMsgExecutionData* pEventMsgExecutionData = NULL;
  EventMsgData* pEventMsgData = NULL;

  char* profileCopy = NULL;
  if (TML_SUCCESS == iRet){
    pEventMsgData = new EventMsgData;
    pEventMsgExecutionData = new EventMsgExecutionData;
    ////////////////////////////////////////////
    // Make a copy of the data->msgData:
    int iSize = (int)strlen(data->msgData->profile)+1;
    profileCopy = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
    strcpy (profileCopy, data->msgData->profile);
#else
    strcpy_s (profileCopy, iSize, data->msgData->profile);
#endif
    pEventMsgData->profile                = profileCopy;
    pEventMsgData->cmdStrContent          = data->msgData->cmdStrContent;
    pEventMsgData->iWindowSize            = data->msgData->iWindowSize;
    pEventMsgData->iTimeout               = data->msgData->iTimeout;
    pEventMsgData->iCmd                   = data->msgData->iCmd;

    pEventMsgExecutionData->msgData = pEventMsgData;
    pEventMsgExecutionData->destinationObj = data->destinationObj;
    ////////////////////////////////////////////////////////////////////////////
    // The new data object for this background event message into the structure:
    m_EventMessageHandlingParams.pDataQueue->addData2Queue(pEventMsgExecutionData);
    ////////////////////////////////////////////////////////////////////////////
    // Set the event to signal new backend event message reqest:
    if (NULL != *handlingEventArray){
      if (! m_eventHandler->SetEventOnHandle((*handlingEventArray)[EVENT_HANDLING_MESSAGE_CONTINUE_EVENT])){
        m_log->log ("tmlEventCall", "performBackendEventMessage", "SetEvent", "failed");
      }
    }
    else{
      m_log->log ("tmlEventCall", "performBackendEventMessage", "", "NULL == *handlingEventArray");
    }
  }
  else{
    // Free allocated data in case of an error:
    if (TML_SUCCESS != iRet){
      if (NULL != profileCopy){
        delete[] profileCopy;
      }
      if (NULL != pEventMsgData){
        delete pEventMsgData;
      }
      if (NULL != pEventMsgExecutionData){
        delete pEventMsgExecutionData;
      }
    }
  }
  return iRet;
}


/**
 * @brief    Allocate event handles for process synchronisation.
 */
int tmlEventCall::allocEventMessageThreadEventArray(const char*** asyncThreadEventArray){
  int iRet = TML_ERR_SYSTEMRESOURCES;

  const char** evtArr = new const char*[NO_OF_EVENT_MESSAGE_HANDLING_HANDLES];
  *asyncThreadEventArray = evtArr;

  /////////////////////////////////////////////////////////////////////////////
  // HANDLE for event in signal a fullfilled reply:
  bool terminateEventHandle = m_eventHandler->CreateEventHandle(TERMINATE_EVENT_MESSAGE_HANDLE);
  if (terminateEventHandle){
    /////////////////////////////////////////////////////////////////////////////
    // HANDLE for event in the case of connection close of a server:
    bool continueEventHandle = m_eventHandler->CreateEventHandle(CONTINUE_EVENT_MESSAGE_HANDLE);
    if (continueEventHandle){
      /////////////////////////////////////////
      // handles into the array:
      evtArr[EVENT_HANDLING_MESSAGE_TERMINATE_EVENT] = TERMINATE_EVENT_MESSAGE_HANDLE;
      evtArr[EVENT_HANDLING_MESSAGE_CONTINUE_EVENT] = CONTINUE_EVENT_MESSAGE_HANDLE;
      iRet = TML_SUCCESS;
    }
  }
  return iRet;
}


/**
 * @brief    Free event handles for process synchronisation.
 */
 void tmlEventCall::freeEventMessageEventArray(const char*** senderSyncEventArray, int iLength){
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
int tmlEventCall::destroySynchronisationHandle(const char** handle)
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
 int tmlEventCall::resetSyncEventArray(const char*** senderSyncEventArray, int iLength){
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
int tmlEventCall::startEventMessageHandlingThread(EventMsgHandlingParams* threadData, tmlEventHandler* eventHandler, const char** handlingEventArray, tmlLogHandler* pLog, void* pCBFunc, TMLThreadDef* threadInfo)
{
  int iRet = TML_SUCCESS;

  if (m_bInShutdown){
    m_log->log ("tmlEventCall", "startEventMessageHandlingThread", "Called while I'm", "in shutdown");
    return TML_ERR_EVENT_HANDLER_IN_DESTRUCTION;
  }
  ///////////////////////////////////////////////////////////////////////////
  // Begin of critical section
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionLockCount, "tmlEventCall", "startEventMessageHandlingThread", "Vortex CMD", "vortex_mutex_lock");

  threadData->handlingEventArray = handlingEventArray;
  threadData->pLog = pLog;
  threadData->eventHandler = eventHandler;
  threadData->terminationMutex = NULL;
  threadData->pCBFunc = pCBFunc;
  threadData->threadInfo = threadInfo;
  threadData->threadInfo->iThreadStatus = THREAD_PENDING_TO_START;

  pLog->log (TML_LOG_VORTEX_CMD, "tmlEventCall", "StartEventMessageHandlingThread", "Vortex CMD", "vortex_thread_create");
    axl_bool bSuccess = intern_thread_create(threadInfo, EventMsgHandlingThread, threadData);
  // Thread- generation log
  if (!bSuccess){
    iRet = TML_ERR_SENDER_NOT_INITIALIZED;
  }

  ///////////////////////////////////////////////////////////////////////////
  // End of critical section
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionLockCount, "tmlEventCall", "startEventMessageHandlingThread", "Vortex CMD", "vortex_mutex_unlock");
  return iRet;
}


/**
 * @brief    Stop the EventMessageHandlingThread.
 */
void tmlEventCall::stopEventMessageHandlingThread()
{
  ///////////////////////////////////////////////////////////////////////////
  // Begin of critical section
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionLockCount, "tmlEventCall", "stopEventMessageHandlingThread", "Vortex CMD", "vortex_mutex_lock");

  if (m_threadInfo.iThreadStatus){
    m_log->log (TML_LOG_CORE_IO, "tmlEventCall", "StopEventMessageHandlingThread", "EventMsgHandlingThread", "Stop");
    if (! m_eventHandler->SetEventOnHandle(m_eventMessageHandlingEventArray[EVENT_HANDLING_MESSAGE_TERMINATE_EVENT])){
      m_log->log ("tmlEventCall", "StopEventMessageHandlingThread", "SetEvent", "failed");
    }
    else{
      /////////////////////////////////////
      // Wait until the thread is finished:
      m_log->log (TML_LOG_VORTEX_MUTEX, "tmlEventCall", "stopEventMessageHandlingThread", "Vortex CMD", "vortex_thread_destroy");
      //  printf ("stopEventMessageHandlingThread -1-\n");
      axl_bool bRet = intern_thread_destroy(&m_threadInfo);
      //  printf ("stopEventMessageHandlingThread -2-\n");
      if (AXL_FALSE == bRet){
        printf ("#### tmlEventCall - stopEventMessageHandlingThread - Error in \"vortex_thread_destroy\" ####\n");
      }
      m_log->log (TML_LOG_VORTEX_MUTEX, "tmlEventCall", "stopEventMessageHandlingThread", "Vortex CMD", "vortex_thread_destroy done");
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  // End of critical section
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionLockCount, "tmlEventCall", "stopEventMessageHandlingThread", "Vortex CMD", "vortex_mutex_unlock");
}


/**
 * @brief    Create the mutex that protect critial section about communication data
 */
axl_bool tmlEventCall::createCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  axl_bool bSuccess = axl_true;
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  bSuccess = intern_mutex_create (mutex);
  return bSuccess;
}


/**
 * @brief    Destroy the mutex that protect critial section about communication data
 */
axl_bool tmlEventCall::destroyCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  axl_bool bSuccess = intern_mutex_destroy (mutex, (char*)"tmlEventCall::destroyCriticalSectionObject");
  return bSuccess;
}


/**
 * @brief    Enter a critical Section
 */
void tmlEventCall::enterCriticalSection(int iLogMask, VortexMutex* mutex, int* iLockCount, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  intern_mutex_lock (mutex, m_log, sMethod);
  return;
}


/**
 * @brief    Leave a critical Section
 */
void tmlEventCall::leaveCriticalSection(int iLogMask, VortexMutex* mutex, int* iLockCount, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  intern_mutex_unlock (mutex, m_log, sMethod);
  return;
}


