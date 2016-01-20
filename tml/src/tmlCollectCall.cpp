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
#include "tmlErrors.h"
#include <sidex.h>
#include "tmlCore.h"
#include "tmlGlobalCallback.h"
#include "tmlCollectCall.h"
#include "tmlCmdDefines.h"
#include "tmlEventDataQueue.h"
#include "unicode.h"
#include "tmlEventData.h"
#include "tmlEventCall.h"
#include "tmlCoreWrapper.h"
#include "logValues.h"

/*********************************************************************************************************************************
*                                             "C" / Global methods / Callbacks & Threads
*********************************************************************************************************************************/


/**
 * @brief  callback method for event messages to free allocated memory
 */
void FUNC_C_DECL asyncEventMessageReturnMethod(TML_COMMAND_HANDLE tmlhandle, TML_POINTER data){
  if (NULL != data){
    EventMessageReturnDataExtension* extData = (EventMessageReturnDataExtension*) data;
    ////////////////////////////////////////////////////
    // Now put the TML_COMMAND_HANDLE into the structure:
    extData->tmlhandle = tmlhandle;
    ////////////////////////////////////////////////////
    // And call tmlCollectCall::InternalAsyncEventMessageReturnMethod()
    globalCallback(extData->pCBFunc, extData);
  }
}


/*********************************************************************************************************************************
*                                             Class definitions:
*********************************************************************************************************************************/


/**
 * @brief   Class callback method that will be called by asyncEventMessageReturnMethod when an async message is finish.
 */
bool tmlCollectCall::InternalAsyncEventMessageReturnMethod(void* callbackData)
{
  EventMessageReturnDataExtension* extData = (EventMessageReturnDataExtension*) callbackData;
  ///////////////////////////
  // Get the state:
  TML_INT32 iState;
  TML_INT32 iRet = tml_Cmd_Header_GetState(extData->tmlhandle, &iState);
  if (TML_SUCCESS == iRet){
    ///////////////////////////
    // Get the Error:
    TML_INT32 iErrorOutOfHeader = TML_SUCCESS;
    iRet = tml_Cmd_Header_GetError(extData->tmlhandle, &iErrorOutOfHeader);
    if (TML_SUCCESS != iRet || TML_SUCCESS != iErrorOutOfHeader || TMLCOM_CSTATE_FAILED == iState){
      /////////////////////////////////////////////////////////////////
      // Something went wrong during message processing, so I have an to 
      // return an error notification, if there is a regirstered Callback:
      char* sProfile;
      extData->connectionObj->getProfile(&sProfile);

      TML_ON_EVT_ERROR_CB_FUNC pCBFunc;
      TML_POINTER pCBData;
      tmlUnicodeID iUnicode;
      iRet = m_collectCallDestObjHandler->eventGetRegisteredCallbackOnError(sProfile, &pCBFunc, &pCBData, &iUnicode, true);
      if (TML_SUCCESS == iRet && NULL != pCBFunc){
        ///////////////////////////
        // Get the Command id:
        TML_COMMAND_ID_TYPE iCmd;
        TML_INT32 iRet = tml_Cmd_Header_GetCommand(extData->tmlhandle, &iCmd);
        if (TML_SUCCESS == iRet){
          char* sPort;
          extData->connectionObj->getPort(&sPort);
          char* sHost;
          extData->connectionObj->getHost(&sHost);
          ///////////////////////////////////////////////////////////////////////////////////////////////
          // Callback in the manner:
          //  void pCBFunc (char* sProfile, char* sHost, char* sPort, TML_COMMAND_ID_TYPE iCMD, TML_INT32 iErrorCode, TML_POINTER pCBData)
          switch (iUnicode){
            case idUNICODE_ASCII:
                //m_log->log ("tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                ((void(FUNC_C_DECL *)(char*, char*, char*, TML_COMMAND_ID_TYPE, TML_INT32, TML_POINTER))pCBFunc)(sProfile, sHost, sPort, iCmd, iErrorOutOfHeader, pCBData);
                //m_log->log ("tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                break;
            case idUNICODE_WCHAR_T:
                {
                  TML_INT32 iUTF16Length;
                  wchar_t* utf16profile = UTF8toUTF32(sProfile, &iUTF16Length);
                  if (NULL != utf16profile){
                    wchar_t* utf16host = UTF8toUTF32(sHost, &iUTF16Length);
                    if (NULL != utf16host){
                      wchar_t* utf16port = UTF8toUTF32(sPort, &iUTF16Length);
                      if (NULL != utf16port){
                        //m_log->log ("tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                        //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                        ((void(FUNC_C_DECL *)(wchar_t*, wchar_t*, wchar_t*, TML_COMMAND_ID_TYPE, TML_INT32, TML_POINTER))pCBFunc)(utf16profile, utf16host, utf16port, iCmd, iErrorOutOfHeader, pCBData);
                        //m_log->log ("tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                        //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                        delete (utf16port);
                      }
                      delete (utf16host);
                    }
                    delete (utf16profile);
                  }
                }
                break;
            case idUNICODE_CHAR16_T:
                {
                  TML_INT32 iUTF16Length;
                  char16_t* utf16profile = (char16_t*)UTF8toUTF16(sProfile, &iUTF16Length);
                  if (NULL != utf16profile){
                    char16_t* utf16host = (char16_t*)UTF8toUTF16(sHost, &iUTF16Length);
                    if (NULL != utf16host){
                      char16_t* utf16port = (char16_t*)UTF8toUTF16(sPort, &iUTF16Length);
                      if (NULL != utf16port){
                        //m_log->log ("tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                        //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                        ((void(FUNC_C_DECL *)(char16_t*, char16_t*, char16_t*, TML_COMMAND_ID_TYPE, TML_INT32, TML_POINTER))pCBFunc)(utf16profile, utf16host, utf16port, iCmd, iErrorOutOfHeader, pCBData);
                        //m_log->log ("tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                        //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:InternalAsyncEventMessageReturnMethod:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                        delete utf16port;
                      }
                      delete utf16host;
                    }
                    delete utf16profile;
                  }
                }
                 break;
          }
        }
      }
    }
  }
  return true;
}


/**
 * @brief    Constructor.
 */
tmlCollectCall::tmlCollectCall(VortexCtx* ctx, tmlLogHandler* loghandler, tmlProfileHandler* pHandler, tmlCoreWrapper* pCore): tmlSingleCall(ctx, loghandler, pHandler, pCore)
{
  m_log = loghandler;

  m_iEventQueueRemoveCounter = 0;
  m_iLastEventLog = 0;
  m_iEventLogPeriod = m_log->getEventQueueRemoveLogPeriod();


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  init the internal class callback method to handle event messages in background called by a thread of the tmlEventCallHandler object
  m_internalEventMessage1stStep.SetCallback(this, &tmlCollectCall::eventSendMessage1stStep);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  init the internal class callback method to handle event messages in background called by a thread of the tmlEventCall object
  m_internalEventMessage2ndStep.SetCallback(this, &tmlCollectCall::eventSendMessage2ndStep);

  //////////////////////////////////////////////////////////////////////////////////////////
  //  init the internal class callback method to handle the return value of an event message
  m_internalAsyncEventMessageReturnMethod.SetCallback(this, &tmlCollectCall::InternalAsyncEventMessageReturnMethod);
  m_iMaxEventConnectionFails = DEFAULT_MAX_CONNECTION_FAILS;
  m_maxEventMsgQueueLength = DEFAULT_MAX_EVENT_MSG_QUEUE_LENGTH;
  ////////////////////////////////
  // Event / Load balanced message handling:
  m_collectCallDestObjHandler = new tmlCollectCallDestinationObjHandler(loghandler);
  m_iMaxLoadBalancedConnectionFails = DEFAULT_MAX_CONNECTION_FAILS;
  //////////////////////////////////////////////////////////////////////////////
  //  Event message background handling:
  // m_internalEventMessage2ndStep will be the callback method,
  // that will be called by the tmlEventCallHandler's background thread:
  m_eventCallHandler = new tmlEventCallHandler(loghandler, &m_internalEventMessage1stStep, &m_internalEventMessage2ndStep);

  ////////////////////////////////////////////////////////////////////////////
  // create the queue containing the background event message data
  m_EventMessageHandlingParams.pDataQueue = new tmlEventDataQueue(loghandler);

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexBackgroundEventMsg, "tmlCollectCall", "tmlCollectCall", "Vortex CMD", "vortex_mutex_create");
  m_iMutexBackgroundEventMsgLockCount = 0;

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, "tmlCollectCall", "tmlCollectCall", "Vortex CMD", "vortex_mutex_create");
  m_iMutexCollectCallCriticalSectionLockCount = 0;
  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection1stStep, "tmlCollectCall", "tmlCollectCall", "Vortex CMD", "vortex_mutex_create");
  m_iMutexCollectCallCriticalSectionLockCount1stStep = 0;
  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection2ndStep, "tmlCollectCall", "tmlCollectCall", "Vortex CMD", "vortex_mutex_create");
  m_iMutexCollectCallCriticalSectionLockCount2ndStep = 0;

  m_iLogFileIndex = 0;
  m_bInShutdown = false;
}


/**
 * @brief    Destructor.
 */
tmlCollectCall::~tmlCollectCall()
{
  m_bInShutdown = true;
  ////////////////////////////////////////////////////////////////////////////
  // Clear the queue containing the background event message data first,
  // to be sure, that no more event messages will be started in the backgound
  //m_EventMessageHandlingParams.pDataQueue->clear();

  ///////////////////////////////////////////
  // Event message handling:
  m_eventCallHandler->stopEventMessageHandlingThread();
  delete (m_collectCallDestObjHandler);
  m_collectCallDestObjHandler = NULL;
  ////////////////////////////////////////////////////////////////////////////
  //  Afterwards destroy event message handling:
  delete (m_eventCallHandler);
  m_eventCallHandler = NULL;
  
  ////////////////////////////////////////
  // TMLCoreSender / drop all connections /
  // if there are async calls pendig it will wait:
  DropAllOpenConnections();

  ////////////////////////////////////////////////////////////////////////////
  //  Now destroy the queue containing the background event message data:
  delete (m_EventMessageHandlingParams.pDataQueue);

  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexBackgroundEventMsg, "tmlCollectCall", "~tmlCollectCall", "Vortex CMD", "vortex_mutex_destroy");

  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, "tmlCollectCall", "~tmlCollectCall", "Vortex CMD", "vortex_mutex_destroy");
  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection1stStep, "tmlCollectCall", "~tmlCollectCall", "Vortex CMD", "vortex_mutex_destroy");
  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection2ndStep, "tmlCollectCall", "~tmlCollectCall", "Vortex CMD", "vortex_mutex_destroy");
}


/**
 * @brief    Subscribe a destination for event messages.
 */
int tmlCollectCall::eventSubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort)
{
  int iRet;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  iRet = m_collectCallDestObjHandler->subscribeEventMessageDestination(profile, sHost, sPort, m_log);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get subscribed destinations for event messages for the provided profile.
 */
int tmlCollectCall::eventGetSubscribedMessageDestinations(const char* profile, SIDEX_VARIANT* subscriptions){
  int iRet;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventGetSubscribedMessageDestinations", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  iRet = m_collectCallDestObjHandler->GetSubscribedEventMessageDestinations(profile, subscriptions, true);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventGetSubscribedMessageDestinations", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Register a callback method for the case of a peer subscription / unsubscription request for event messages.
 */
int tmlCollectCall::eventRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_collectCallDestObjHandler->eventRegisterOnPeerCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Get the registered callback method for the case of a peer subscription / unsubscription request for event messages.
 */
int tmlCollectCall::getRegisteredEventOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode)
{
    return m_collectCallDestObjHandler->getRegisteredEventOnPeerCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Unsubscribe a destination for event messages.
 */
int tmlCollectCall::eventUnsubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort)
{
  int iRet;
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventUnsubscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  iRet = m_collectCallDestObjHandler->unsubscribeEventMessageDestination(profile, sHost, sPort, true);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventUnsubscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");

  //////////////////////////////////////////////////////////////////////////
  // It's a good idea to remove pending event / load balanced unsubscriptions here:
  RemoveMarkedUnsubscriptions(true);

  return iRet;
}


/**
 * @brief    Unsubscribe all destinations for event messages for the provided profile.
 */
int tmlCollectCall::eventUnsubscribeAllMessageDestinations(const char* profile)
{
  int iRet;
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventUnsubscribeAllMessageDestinations", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  iRet = m_collectCallDestObjHandler->unsubscribeAllEventMessageDestinations(profile);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventUnsubscribeAllMessageDestinations", "Vortex CMD", "vortex_mutex_unlock");

  //////////////////////////////////////////////////////////////////////////
  // It's a good idea to remove pending event / load balanced unsubscriptions here:
  RemoveMarkedUnsubscriptions(true);

  return iRet;
}


/**
 * @brief    Make a common deregistration / event
 */
int tmlCollectCall::eventDeregisterAll()
{
    return m_collectCallDestObjHandler->eventDeregisterAll();
}


/**
 * @brief    Make a common deregistration / load balanced
 */
int tmlCollectCall::loadBalancedDeregisterAll()
{
    return m_collectCallDestObjHandler->loadBalancedDeregisterAll();
}


/**
 * @brief    Register a callback method that will be called in case of an error during event message send execution.
 */
int tmlCollectCall::eventRegisterCallbackOnError(const char* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_collectCallDestObjHandler->eventRegisterCallbackOnError(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Register a callback method that will be called in case of event message queue overflow.
 */
int tmlCollectCall::eventRegisterCallbackOnQueueOverflow(const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc,TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_collectCallDestObjHandler->eventRegisterCallbackOnQueueOverflow(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Register a callback method for the case of non subscribed destinations for event messages.
 */
int tmlCollectCall::eventRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_collectCallDestObjHandler->eventRegisterPopulateCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Remove all queue entrys more than m_maxEventMsgQueueLength out of the event message queue
 */
void tmlCollectCall::eventQueueOverflowHandling(tmlEventDataQueue* pDataQueue, TML_COMMAND_ID_TYPE iCmd, const char* profile, tmlCollectCallDestinationObj* destinationObj){
  unsigned int iLength;
  EventMsgExecutionData* tempData;
  do
  {
    iLength = pDataQueue->getQueueLength();
    if (m_maxEventMsgQueueLength <= iLength){
      --iLength;
      bool bFound = pDataQueue->getDataQueueFrom(&tempData, true);
      if (bFound){
        //////////////
        // Remove log:
        eventMessageRemoveInfo(destinationObj);
        // Callback to Inform about the remove:
        
        /////////////////////////////////////////////////////////////////
        // Something went wrong during message processing, so I have an to 
        // return an error notification, if there is a registered Callback:
        TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc;
        TML_POINTER pCBData;
        tmlUnicodeID iUnicode;
        ////////////////////
        // Siehe TmlSidexDemo - Close All
        int iRet = m_collectCallDestObjHandler->eventGetRegisteredCallbackOnQueueOverflow(profile, &pCBFunc, &pCBData, &iUnicode);

        if (TML_SUCCESS == iRet && NULL != pCBFunc){
          ///////////////////////////////////////////////////////////////////////////////////////////////
          // Callback in the manner:
          //  void pCBFunc (char* sProfile, TML_COMMAND_ID_TYPE iCMD, TML_POINTER pCBData)
          switch (iUnicode){
                case idUNICODE_ASCII:
                /*
                char*sHost = "Input";
                char*sPort = "Queue";
                if (NULL != destinationObj){
                  destinationObj->getMessageDestinationHost(&sHost);
                  destinationObj->getMessageDestinationPort(&sPort);
                }
                destinationObj->getMessageDestinationPort(&sPort);
                m_log->log ("tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "CALL");
                tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "CALL");
                */
                ((void(FUNC_C_DECL *)(char*, TML_COMMAND_ID_TYPE, TML_POINTER))pCBFunc)((char*) profile, iCmd, pCBData);
                //m_log->log ("tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "BACK");
                //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "BACK");
                break;
            case idUNICODE_WCHAR_T:
                {
                  TML_INT32 iUTF16Length;
                  wchar_t* utf16profile = UTF8toUTF32((char*)profile, &iUTF16Length);
                  if (NULL != utf16profile){
                    /*
                    char*sHost = "Input";
                    char*sPort = "Queue";
                    if (NULL != destinationObj){
                      destinationObj->getMessageDestinationHost(&sHost);
                      destinationObj->getMessageDestinationPort(&sPort);
                    }
                    m_log->log ("tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "CALL");
                    tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "CALL");
                    */
                    ((void(FUNC_C_DECL *)(wchar_t*, TML_COMMAND_ID_TYPE, TML_POINTER))pCBFunc)(utf16profile, iCmd, pCBData);
                    //m_log->log ("tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "BACK");
                    //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "BACK");
                    delete (utf16profile);
                  }
                }
                break;
            case idUNICODE_CHAR16_T:
                {
                  TML_INT32 iUTF16Length;
                  char16_t* utf16profile = (char16_t*)UTF8toUTF16((char*)profile, &iUTF16Length);
                  if (NULL != utf16profile){
                    /*
                    char*sHost = "Input";
                    char*sPort = "Queue";
                    if (NULL != destinationObj){
                      destinationObj->getMessageDestinationHost(&sHost);
                      destinationObj->getMessageDestinationPort(&sPort);
                    }
                    m_log->log ("tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "CALL");
                    tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "CALL");
                    */
                    ((void(FUNC_C_DECL *)(char16_t*, TML_COMMAND_ID_TYPE, TML_POINTER))pCBFunc)(utf16profile, iCmd, pCBData);
                    //m_log->log ("tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "BACK");
                    //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventQueueOverflowHandling:eventGetRegisteredCallbackOnQueueOverflow", sHost, sPort, "BACK");
                    delete utf16profile;
                  }
                }
                break;
          }
        }
        // Now destroy it:
        m_EventMessageHandlingParams.pDataQueue->destroyDataContent(tempData);
      }
    }
  }
  while (m_maxEventMsgQueueLength <= iLength);
}


/**
 * @brief    Send an event message.
 */
int tmlCollectCall::eventSendMessage(int iWindowSize, TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout){
  TML_INT32 iRet = TML_SUCCESS;
  TML_INT32 iContentLength = 0;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexBackgroundEventMsg, &m_iMutexBackgroundEventMsgLockCount, "tmlCollectCall", "eventSendMessage", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  /////////////////////////////////////////////////////////////
  // Make a copy of all TML_COMMAND_HANDLE relevant content:
  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  char* sContent = NULL;
  TML_COMMAND_ID_TYPE iCmd = -1;
  iRet = tml_Cmd_Acquire_Sidex_Handle(tmlhandle, &sHandle);
  if (TML_SUCCESS == iRet){
    iRet = sidex_Boolean_Write (sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_IS_EVENT, TML_TRUE);
    if (TML_SUCCESS == iRet){
      iRet = sidex_Get_Content(sHandle, &sContent, &iContentLength);
    }
    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    tml_Cmd_Release_Sidex_Handle(tmlhandle);
    if (TML_SUCCESS == iRet){
      iRet = tml_Cmd_Header_GetCommand(tmlhandle, &iCmd);
    }
  }
  EventMsgData* pEventMsgData = NULL;
  char* profileCopy = NULL;

  if (TML_SUCCESS == iRet){
    EventMsgExecutionData* pEventMsgExecutionData = NULL;

    ///////////////////////////////////////////////////////////////
    // Datastructure contains the parameter for the event messages
    pEventMsgData = new EventMsgData;
    pEventMsgExecutionData = new EventMsgExecutionData;

    ////////////////////////////////////////////
    // Make a copy of the profile:
    int iSize = (int)strlen(profile)+1;
    profileCopy = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
    strcpy (profileCopy, profile);
#else
    strcpy_s (profileCopy, iSize, profile);
#endif
    //////////////////////////////////////////////////////
    // Create a command handle for the event call:
    TML_COMMAND_HANDLE cmdHandle = TML_HANDLE_TYPE_NULL;

    iRet = tml_Cmd_Create (&cmdHandle);
    if (TML_SUCCESS == iRet){
      ///////////////////////////////////////////////
      // Acquire critical section use: 
      SIDEX_HANDLE sHandle;
      iRet = tml_Cmd_Acquire_Sidex_Handle(cmdHandle, &sHandle);
      if (TML_SUCCESS == iRet){
        //////////////////////////////////////////////////////////////////
        // Set the content
        iRet = sidex_Set_Content(sHandle, sContent);
        ///////////////////////////////////////////////
        // Now I can release the critical section use: 
        tml_Cmd_Release_Sidex_Handle(cmdHandle);
      }
    }


    if (TML_SUCCESS == iRet){
      tmlEventData* data = new tmlEventData(cmdHandle);
      pEventMsgData->profile                = profileCopy;
      pEventMsgData->cmdStrContent          = data;
      pEventMsgData->iWindowSize            = iWindowSize;
      pEventMsgData->iTimeout               = iTimeout;
      pEventMsgData->iCmd                   = iCmd;

      pEventMsgExecutionData->msgData = pEventMsgData;
      pEventMsgExecutionData->destinationObj = NULL;

      /////////////////////////////////////////////////////////////////////
      // I handle the   Queue- Overflow on the first queue now:
      eventQueueOverflowHandling(m_EventMessageHandlingParams.pDataQueue, iCmd, profile, NULL);
    
      //  20-09-12 / This will be done now her
      //////////////////////////////////////////////////////////////////////////////////////
      // Set the new data object for this background event message into the structure here:

      // 25-09-12 NO
      //m_EventMessageHandlingParams.pDataQueue->addData2Queue(pEventMsgExecutionData);
      iRet = m_eventCallHandler->performBackendEventMessage(&m_EventMessageHandlingParams, pEventMsgExecutionData);
    }
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
    ////////////////////////////////////////////
    // Now I can free the sidex content
    if (NULL != sContent){
      sidex_Free_Content(sContent);
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexBackgroundEventMsg, &m_iMutexBackgroundEventMsgLockCount, "tmlCollectCall", "eventSendMessage", "Vortex CMD", "vortex_mutex_unlock");
  return (int)iRet;
}


/**
 * @brief    Send an event message (in the background).
 */
bool tmlCollectCall::eventSendMessage2ndStep(void* callbackData)
{
  TML_INT32 iRet = TML_SUCCESS;

  EventMsgExecutionData* data = (EventMsgExecutionData*) callbackData; 
  EventMsgData* eventMsgData = data->msgData;
  
  int iWindowSize = eventMsgData->iWindowSize;

  TML_COMMAND_HANDLE cmdHandle = eventMsgData->cmdStrContent->getValue();

  const char* profile = eventMsgData->profile;
  unsigned int iTimeout = eventMsgData->iTimeout;

  bool bInCriticalSection = true;
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection2ndStep, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventSendMessage2ndStep", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a subscribe or unsubscribe may not be possible during this method execution :

  TML_COMMAND_ID_TYPE iCmd = eventMsgData->iCmd;

  char* sHost = NULL;
  char* sPort = NULL;

  //////////////////////////////////////////////////
  // The destination attributes:
  data->destinationObj->getMessageDestinationHost(&sHost);
  data->destinationObj->getMessageDestinationPort(&sPort);

  ///////////////////////////////////////////////////////////////////////////////////
  // It may possible that a subscribed destination had a fail during event messages
  // in that case it is no longer valid so check the validity:
  bool bSubscribed = m_collectCallDestObjHandler->acquireMessageDestination(profile, sHost, sPort);
  if (bSubscribed){
      ////////////////////////////////////////////////////////////////////////////////
      // Send the async message don't mind of the return value (use it only internal):
      EventMessageReturnDataExtension* ext = new EventMessageReturnDataExtension;
      ext->pCBFunc = &m_internalAsyncEventMessageReturnMethod;
      tmlConnectionObj* connectionObj = new tmlConnectionObj(m_log);
      connectionObj->setConnectionObj(profile, sHost, sPort, NULL, NULL, NULL);
      ext->connectionObj = connectionObj;
      ////////////////////////////////
      // Synchron Event-Message:
      leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection2ndStep, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventSendMessage2ndStep", "Vortex CMD", "vortex_mutex_unlock");
      bInCriticalSection = false;
      /////////////////////////////////////////////////////////////////////////////
      // Maybe we are in a shutdown process / in that case don't do anything here:

      // Changed: Since event message queues will be emptied in the destruction
      // I don't have to check for SHUTDOWN:
      //if (1 == 1 || TML_SUCCESS == m_tmlCoreHandle->tmlCoreWrapper_IsAccessible()){
      bool bAnyConnectionFailed = false;
      {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Event - Logging:
        bool bLogPrinted = data->destinationObj->eventMessageStartInfo();
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Send the message but flag, it has it's source in an event handling / there must not be an emptying of pending unsubscription objects:
        int iErrorOutOfSendAsyncMessage = sender_SendSyncMessage(profile, sHost, sPort, iWindowSize, cmdHandle, iTimeout, NULL, false);
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Event Done - Logging:
        data->destinationObj->eventMessageDoneInfo(bLogPrinted);

        switch (iErrorOutOfSendAsyncMessage){
          case TML_SUCCESS:
            data->destinationObj->resetFailCounter();
            break;
          case TML_ERR_SENDER_INVALID_PARAMS:
          case TML_ERR_COM_LAYER_CONNECTION_CLOSE:
            m_log->log (TML_LOG_EVENT, "tmlCollectCall", "eventSendMessage2ndStep / incrementFailCounter", sHost, sPort);
            tml_log_A(TML_LOG_EVENT, "tmlCollectCall", "eventSendMessage2ndStep / incrementFailCounter", sHost, sPort);
            data->destinationObj->incrementFailCounter();
            bAnyConnectionFailed = true;
            // No Break on here:
          default:
            if (TML_ERR_TIMEOUT != iErrorOutOfSendAsyncMessage){
              m_log->log ("tmlCollectCall:eventSendMessage2ndStep:ERROR returned by eventSendMessage2ndStep", sHost, sPort, iErrorOutOfSendAsyncMessage);
              tml_logI_A(TML_LOG_EVENT, "tmlCollectCall:eventSendMessage2ndStep:ERROR returned by eventSendMessage2ndStep", sHost, sPort, iErrorOutOfSendAsyncMessage);
            }
            else{
              m_log->log ("tmlCollectCall:eventSendMessage2ndStep:TIMEOUT ERROR returned by eventSendMessage2ndStep", sHost, sPort, iTimeout);
              tml_logI_A(TML_LOG_EVENT, "tmlCollectCall:eventSendMessage2ndStep:TIMEOUT ERROR returned by eventSendMessage2ndStep", sHost, sPort, iTimeout);
            }
            /////////////////////////////////////////////////////////////////////////////
            // Maybe we are in a shutdown process / in that case don't do anything here:

            // Changed: Since event message queues will be emptied in the destruction
            // I don't have to check for SHUTDOWN:
            //if (1 == 1 || TML_SUCCESS == m_tmlCoreHandle->tmlCoreWrapper_IsAccessible()){
            {
              /////////////////////////////////////////////////////////////////
              // Something went wrong during message processing, so I have an to 
              // return an error notification, if there is a regirstered Callback:
              TML_ON_EVT_ERROR_CB_FUNC pCBFunc = NULL;
              TML_POINTER pCBData = NULL;
              tmlUnicodeID iUnicode;
              int iInternalRet2 = m_collectCallDestObjHandler->eventGetRegisteredCallbackOnError(profile, &pCBFunc, &pCBData, &iUnicode, false);
              if (TML_SUCCESS == iInternalRet2 && NULL != pCBFunc){
                ///////////////////////////////////////////////////////////////////////////////////////////////
                // Callback in the manner:
                //  void pCBFunc (char* sProfile, char* sHost, char* sPort, TML_COMMAND_ID_TYPE iCMD, TML_INT32 iErrorCode, TML_POINTER pCBData)
                switch (iUnicode){
                  case idUNICODE_ASCII:
                      //m_log->log ("tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                      //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                      ((void(FUNC_C_DECL *)(char*, char*, char*, TML_COMMAND_ID_TYPE, TML_INT32, TML_POINTER))pCBFunc)((char*) profile, sHost, sPort, iCmd, iErrorOutOfSendAsyncMessage, pCBData);
                      //m_log->log ("tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                      //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                      break;
                  case idUNICODE_WCHAR_T:
                      {
                        TML_INT32 iUTF16Length;
                        wchar_t* utf16profile = UTF8toUTF32((char*)profile, &iUTF16Length);
                        if (NULL != utf16profile){
                          wchar_t* utf16host = UTF8toUTF32(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            wchar_t* utf16port = UTF8toUTF32(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              //m_log->log ("tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                              //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                              ((void(FUNC_C_DECL *)(wchar_t*, wchar_t*, wchar_t*, TML_COMMAND_ID_TYPE, TML_INT32, TML_POINTER))pCBFunc)(utf16profile, utf16host, utf16port, iCmd, iErrorOutOfSendAsyncMessage, pCBData);
                              //m_log->log ("tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                              //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                              delete (utf16port);
                            }
                            delete (utf16host);
                          }
                          delete (utf16profile);
                        }
                      }
                      break;
                  case idUNICODE_CHAR16_T:
                      {
                        TML_INT32 iUTF16Length;
                        char16_t* utf16profile = (char16_t*)UTF8toUTF16((char*)profile, &iUTF16Length);
                        if (NULL != utf16profile){
                          char16_t* utf16host = (char16_t*)UTF8toUTF16(sHost, &iUTF16Length);
                          if (NULL != utf16host){
                            char16_t* utf16port = (char16_t*)UTF8toUTF16(sPort, &iUTF16Length);
                            if (NULL != utf16port){
                              //m_log->log ("tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                              //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","CALL", sHost, sPort);
                              ((void(FUNC_C_DECL *)(char16_t*, char16_t*, char16_t*, TML_COMMAND_ID_TYPE, TML_INT32, TML_POINTER))pCBFunc)(utf16profile, utf16host, utf16port, iCmd, iErrorOutOfSendAsyncMessage, pCBData);
                              //m_log->log ("tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                              //tml_log_A(TML_LOG_EVENT, "tmlCollectCall:eventSendMessage2ndStep:eventGetRegisteredCallbackOnError","BACK", sHost, sPort);
                              delete utf16port;
                            }
                            delete utf16host;
                          }
                          delete utf16profile;
                        }
                      }
                      break;
                }
              }
            }
            break;
        }
        // SIDEX-223:API- "tml_Evt_Set_MaxConnectionFailCount" don't work
        m_collectCallDestObjHandler->unregisterFailedEventConnectionDestinations(profile, m_iMaxEventConnectionFails, false);
      }
      ///////////////////////////////////////////////////////
      // Free memory that holds the listener's identification:
      delete (connectionObj);
      ///////////////////////////////////////////////////////
      // Free EventMessageReturnDataExtension structure:
      delete (ext);

      /////////////////////////////////////////////////////////////////////////////
      // Maybe we are in a shutdown process / in that case don't do anything here:

      // Changed: Since event message queues will be emptied in the destruction
      // I don't have to check for SHUTDOWN:
      //if (1 == 1 || TML_SUCCESS == m_tmlCoreHandle->tmlCoreWrapper_IsAccessible()){
      {
        //////////////////////////////////////
        // No unregistration during shutdown !
        if (bAnyConnectionFailed && !m_bInShutdown){
          ////////////////////////////////////////////////////////////////////////////
          // Now it is time to remove destinations with a positive fail couner:
          if (NULL != m_collectCallDestObjHandler){
            m_collectCallDestObjHandler->unregisterFailedEventConnectionDestinations(profile, m_iMaxEventConnectionFails, false);
          }
          else{
          }
        }
      }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Now it's time to free all memory for this background event message request (content of EventMessageData);
  ///////////////////////////////////////////////
  // Now I can delete the content
  eventMsgData->cmdStrContent->decRef();
  if (eventMsgData->cmdStrContent->getRef() == 0){
    TML_COMMAND_HANDLE value = eventMsgData->cmdStrContent->getValue();
    if (TML_HANDLE_TYPE_NULL != value){
      tml_Cmd_Free(&value);
    }
    if (NULL != eventMsgData->cmdStrContent){
      delete (eventMsgData->cmdStrContent);
      eventMsgData->cmdStrContent = NULL;
    }
  }

  delete[] data->msgData->profile;
  delete data->msgData;
  delete data;

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a subscribe or unsubscribe was not possible during this method execution
  if (bInCriticalSection){
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection2ndStep, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventSendMessage2ndStep", "Vortex CMD", "vortex_mutex_unlock");
  }

  return (TML_SUCCESS == iRet);
}

bool tmlCollectCall::eventSendMessage1stStep(void* callbackData)
{
  TML_INT32 iRet = TML_SUCCESS;

  EventMsgExecutionData* eventMsgData = (EventMsgExecutionData*) callbackData;
  const char* profile = eventMsgData->msgData->profile;
  int   iDestinations;
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Ask for the number of registered destinatation:
  getEventDestinationCount(m_collectCallDestObjHandler, profile, &iDestinations);
  if (iDestinations){
    TML_COMMAND_ID_TYPE iCmd = eventMsgData->msgData->iCmd;
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection1stStep, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventSendMessage1stStep", "Vortex CMD", "vortex_mutex_lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a subscribe or unsubscribe may not be possible during this method execution :
    for (int i = 0; TML_SUCCESS == iRet && i < iDestinations; ++i){
      tmlCollectCallDestinationObj* destinationObj = NULL;
      ///////////////////////////////////////////////////////////////////
      //  Get the destination for this loop:
      iRet = m_collectCallDestObjHandler->getSubscribedEventMessageDestination(profile, i, &destinationObj, true);
      if (TML_SUCCESS == iRet){
        eventMsgData->msgData->cmdStrContent->incRef();

        tmlEventCall* caller = destinationObj->getTmlEventCaller();

        ///////////////////////////////////////////////////////////////
        // Before we put a new object into the queue, maybe the queue is full:
        tmlEventDataQueue* queue = caller->getEventMessageQueue();

        eventQueueOverflowHandling(queue, iCmd, profile, destinationObj);

        eventMsgData->destinationObj  = destinationObj;
        caller->performBackendEventMessage(&m_internalEventMessage2ndStep, eventMsgData);
      }
    }
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection1stStep, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "eventSendMessage1stStep", "Vortex CMD", "vortex_mutex_unlock");
  }
  // Free allocated data:
  m_EventMessageHandlingParams.pDataQueue->destroyDataContent(eventMsgData);
  return (TML_SUCCESS == iRet);
}


/**
 * @brief    Subscribe a destination for load balanced messages.
 */
int tmlCollectCall::loadBalancedSubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort)
{
  int iRet;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  iRet = m_collectCallDestObjHandler->subscribeLoadBalancedMessageDestination(profile, sHost, sPort, m_log);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get subscribed destinations for load balanced messages for the provided profile.
 */
int tmlCollectCall::loadBalancedGetSubscribedMessageDestinations(const char* profile, SIDEX_VARIANT* subscriptions){
  int iRet;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedGetSubscribedMessageDestinations", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  iRet = m_collectCallDestObjHandler->GetSubscribedLoadBalancedMessageDestinations(profile, subscriptions, true);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedGetSubscribedMessageDestinations", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Register a callback method for the case of a peer subscription / unsubscription request for load balanced messages.
 */
int tmlCollectCall::loadBalancedRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_collectCallDestObjHandler->loadBalancedRegisterOnPeerCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Get the registered callback method for the case of a peer subscription / unsubscription request for load balanced messages.
 */
int tmlCollectCall::getRegisteredLoadBalancedOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode)
{
    return m_collectCallDestObjHandler->getRegisteredLoadBalancedOnPeerCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Unsubscribe a destination for load balanced messages.
 */
int tmlCollectCall::loadBalancedUnsubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort)
{
  int iRet;

  enterCriticalSection(TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedUnsubscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  iRet = m_collectCallDestObjHandler->unsubscribeLoadBalancedMessageDestination(profile, sHost, sPort, true);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedUnsubscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");

  //////////////////////////////////////////////////////////////////////////
  // It's a good idea to remove pending event / load balanced unsubscriptions here:
  RemoveMarkedUnsubscriptions(true);

  return iRet;
}


/**
 * @brief    Unsubscribe all destinations for load balanced messages for the provided profile.
 */
int tmlCollectCall::loadBalancedUnsubscribeAllMessageDestinations(const char* profile)
{
  int iRet;

  enterCriticalSection(TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedUnsubscribeAllMessageDestinations", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  iRet = m_collectCallDestObjHandler->unsubscribeAllLoadBalancedMessageDestinations(profile);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedUnsubscribeAllMessageDestinations", "Vortex CMD", "vortex_mutex_unlock");

  //////////////////////////////////////////////////////////////////////////
  // It's a good idea to remove pending event / load balanced unsubscriptions here:
  RemoveMarkedUnsubscriptions(true);

  return iRet;
}


/**
 * @brief    Register a callback method for the case of non subscribed destinations for load balanced messages.
 */
int tmlCollectCall::loadBalancedRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
    return m_collectCallDestObjHandler->loadBalancedRegisterPopulateCallback(profile, pCBFunc, pCBData, iUnicode);
}


/**
 * @brief    Register a callback method for the BUSY STATUS requests.
 */
int tmlCollectCall::loadBalancedRegisterBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc,TML_POINTER pCBData)
{
    return m_collectCallDestObjHandler->loadBalancedRegisterBusyStatusCallback(profile, pCBFunc, pCBData);
}


/**
 * @brief    Get the registered callback method that will be called in case of BUSY STATUS requests.
 */
int tmlCollectCall::getRegisteredBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC* pCBFunc, TML_POINTER* pCBData)
{
    return m_collectCallDestObjHandler->getRegisteredBusyStatusCallback(profile, pCBFunc, pCBData);
}


/**
 * @brief    Register a callback method for the load balance calculation.
 */
int tmlCollectCall::loadBalancedRegisterCalculator(const char* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
    return m_collectCallDestObjHandler->registerLoadBalanceCalculator(profile, pCBFunc, pCBData);
}


/**
 * @brief    Send a synchron load ballanced message.
 */
int tmlCollectCall::loadBalancedSendSyncMessage(int iWindowSize, TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout)
{
  int   iRet = TML_SUCCESS;
  int   iDestinations;
  TML_INT32 iDebug = 0;
  try{
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Ask for the number of registered destinatation:
  getLoadBalancedDestinationCount(m_collectCallDestObjHandler, profile, &iDestinations, true);
  ++iDebug;
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // we have a critical section because multi task calls of this method may not overwrite the next destination attributes:
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendSyncMessage", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a subscribe or unsubscribe may not be possible during this method execution :

  /////////////////////////////////////////
  // If there is an existing destination:
  if (0 < iDestinations){
    char* sHost = NULL;
    char* sPort = NULL;
    tmlCollectCallDestinationObj* destinationObj = NULL;
    bool bAnyFailFound;
    do{
      bAnyFailFound = false;
      ////////////////////////////////////////////////////////////////////////////////
      //  Get the destination for this loop / NO inclusive lock / NO critical section:
      iRet = getNextLoadBalancedDestination(m_collectCallDestObjHandler, profile, iWindowSize, iDestinations, &bAnyFailFound, &destinationObj);
      ++iDebug;
      if (TML_SUCCESS == iRet){
        //////////////////////////////////////////////////
        // The destination attributes:
        destinationObj->getMessageDestinationHost(&sHost);
        ++iDebug;
        destinationObj->getMessageDestinationPort(&sPort);
        //////////////////////////////////////////////////////////////////
        // Send SyncMessage
        // I pass the mutex to the sender because it knows better the 
        // time to unlock - if the data / addresses may be save to be
        // overwritten by a nother thred instance:
        iRet = sender_SendSyncMessage(profile, sHost, sPort, iWindowSize, tmlhandle, iTimeout, &m_mutexCollectCallCriticalSection, true);
        ++iDebug;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 	 of course I have to fetch the critical Section again afterward:
        enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendSyncMessage", "Vortex CMD", "vortex_mutex_lock");
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Check if the destinationObj is valid:
        switch (iRet){
          case TML_ERR_SENDER_INVALID_PARAMS:
          case TML_ERR_COM_LAYER_CONNECTION_CLOSE:
            m_log->log ("tmlCollectCall", "loadBalancedSendSyncMessage", "Connection problem ERROR / try another one", iRet);
            destinationObj->incrementFailCounter();
            ++iDebug;
            bAnyFailFound = true;
            break;
          case TML_SUCCESS:
            destinationObj->resetFailCounter();
            ++iDebug;
            break;
          default:
            ++iDebug;
            m_log->log ("tmlCollectCall", "loadBalancedSendSyncMessage", "ERROR returned by sender_SendSyncMessage", iRet);
            break;
        }
        if (bAnyFailFound){
          ////////////////////////////////////////////////////////////////////////////
          // There has been a destination request fail,
          // so it is time to remove destinations with a positive fail couner:
          m_collectCallDestObjHandler->unregisterFailedLoadBalancedConnectionDestinations(profile, m_iMaxLoadBalancedConnectionFails);
          ++iDebug;
          /////////////////////////////////////////////////////////////////////////////////////////
          // End of critical section / a subscribe or unsubscribe was not possible during this method execution
          leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendSyncMessage", "Vortex CMD", "vortex_mutex_unlock");
          ////////////////////////////////////////////////////////////////////////////
          // The number of destinations may have changed:
          getLoadBalancedDestinationCount(m_collectCallDestObjHandler, profile, &iDestinations, false);
          enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendSyncMessage", "Vortex CMD", "vortex_mutex_lock");
          ////////////////////////////////////////////////////////////////////////////////////////////////
          // Begin of critical section / a subscribe or unsubscribe may not be possible during this method execution :
        }
      }
    }
    ////////////////////////////////////////////////////////////////////////////
    // loop again on TML_ERR_SENDER_INVALID_PARAMS (connection fail)
    // loop again on TML_ERR_COM_LAYER_CONNECTION_CLOSE (communicationbroken)
    // if another destination is registered:
    while ((TML_ERR_SENDER_INVALID_PARAMS == iRet || TML_ERR_COM_LAYER_CONNECTION_CLOSE == iRet) && 0 < iDestinations);
    ////////////////////////////////////////////////////////////////////////////
    // It may be possible that no more destinations are valid:
    if (0 == iDestinations){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a subscribe or unsubscribe was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendAsyncMessage", "Vortex CMD", "vortex_mutex_unlock");
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "tmlCollectCall", "loadBalancedSendSyncMessage", "EXCEPTION / DebugVal", iDebug);
  }

  return iRet;
}


/**
 * @brief    Send an asynchron load ballanced message.
 */
int tmlCollectCall::loadBalancedSendAsyncMessage(int iWindowSize, TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout)
{
  int   iRet = TML_SUCCESS;
  int   iDestinations;
  TML_INT32 iDebug = 0;
  try{

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Ask for the number of registered destinatation:
  getLoadBalancedDestinationCount(m_collectCallDestObjHandler, profile, &iDestinations, true);
  ++iDebug;
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // we have a critical section because multi task calls of this method may not overwrite the next destination attributes:
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendAsyncMessage", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a subscribe or unsubscribe may not be possible during this method execution :

  /////////////////////////////////////////
  // If there is an existing destination:
  if (0 < iDestinations){
    char* sHost = NULL;
    char* sPort = NULL;
    tmlCollectCallDestinationObj* destinationObj = NULL;
    bool bAnyFailFound;

    do{
      bAnyFailFound = false;
      ////////////////////////////////////////////////////////////////////////////////
      //  Get the destination for this loop / NO inclusive lock / NO critical section:
      iRet = getNextLoadBalancedDestination(m_collectCallDestObjHandler, profile, iWindowSize, iDestinations, &bAnyFailFound, &destinationObj);
      ++iDebug;
      if (TML_SUCCESS == iRet){
        //////////////////////////////////////////////////
        // The destination attributes:
        destinationObj->getMessageDestinationHost(&sHost);
        ++iDebug;
        destinationObj->getMessageDestinationPort(&sPort);
        //////////////////////////////////////////////////////////////////
        // Send AsyncMessage
        iRet = sender_SendAsyncMessage(profile, sHost, sPort, iWindowSize, tmlhandle, iTimeout, TMLCOM_MODE_ASYNC, false, false);
        ++iDebug;
        switch (iRet){
          case TML_ERR_SENDER_INVALID_PARAMS:
          case TML_ERR_COM_LAYER_CONNECTION_CLOSE:
            m_log->log ("tmlCollectCall", "loadBalancedSendAsyncMessage", "Connection problem ERROR / try another one", iRet);
            destinationObj->incrementFailCounter();
            ++iDebug;
            bAnyFailFound = true;
            break;
          case TML_SUCCESS:
            destinationObj->resetFailCounter();
            ++iDebug;
            break;
          default:
            ++iDebug;
            m_log->log ("tmlCollectCall", "loadBalancedSendAsyncMessage", "ERROR returned by sender_SendAsyncMessage", iRet);
            break;
        }
        if (bAnyFailFound){
          ////////////////////////////////////////////////////////////////////////////
          // There has been a destination request fail,
          // so it is time to remove destinations with a positive fail couner:
          m_collectCallDestObjHandler->unregisterFailedLoadBalancedConnectionDestinations(profile, m_iMaxLoadBalancedConnectionFails);
          ++iDebug;
          /////////////////////////////////////////////////////////////////////////////////////////
          // End of critical section / a subscribe or unsubscribe was not possible during this method execution
          leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendAsyncMessage", "Vortex CMD", "vortex_mutex_unlock");
          ////////////////////////////////////////////////////////////////////////////
          // The number of destinations may have changed:
          getLoadBalancedDestinationCount(m_collectCallDestObjHandler, profile, &iDestinations, false);
          enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendAsyncMessage", "Vortex CMD", "vortex_mutex_lock");
          ////////////////////////////////////////////////////////////////////////////////////////////////
          // Begin of critical section / a subscribe or unsubscribe may not be possible during this method execution :
        }
      }
    }
    ////////////////////////////////////////////////////////////////////////////
    // loop again on TML_ERR_SENDER_INVALID_PARAMS (connection fail)
    // loop again on TML_ERR_COM_LAYER_CONNECTION_CLOSE (communicationbroken)
    // if another destination is registered:
    while ((TML_ERR_SENDER_INVALID_PARAMS == iRet || TML_ERR_COM_LAYER_CONNECTION_CLOSE == iRet) && 0 < iDestinations);
    ////////////////////////////////////////////////////////////////////////////
    // It may be possible that no more destinations are valid:
    if (0 == iDestinations){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a subscribe or unsubscribe was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCollectCallCriticalSection, &m_iMutexCollectCallCriticalSectionLockCount, "tmlCollectCall", "loadBalancedSendAsyncMessage", "Vortex CMD", "vortex_mutex_unlock");
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "tmlCollectCall", "loadBalancedSendAsyncMessage", "EXCEPTION / DebugVal", iDebug);
  }

  return iRet;
}


/**
 * @brief    Set the maximum load balanced connection fail count value.
 */
void tmlCollectCall::loadBalancedSetMaxConnectionFailCount(unsigned int iCount){
  m_iMaxLoadBalancedConnectionFails = iCount;
}


/**
 * @brief    Get the maximum load balanced connection fail count value.
 */
void  tmlCollectCall::loadBalancedGetMaxConnectionFailCount(unsigned int* iCount){
  *iCount = m_iMaxLoadBalancedConnectionFails;
}


/**
 * @brief    Set the maximum event connection fail count value.
 */
void tmlCollectCall::eventSetMaxConnectionFailCount(unsigned int iCount){
  m_iMaxEventConnectionFails = iCount;
}


/**
 * @brief    Set the maximum value of queued event messages
 */
void tmlCollectCall::eventSetMaxQueuedEventMessages(unsigned int iMaximum){
  m_maxEventMsgQueueLength = iMaximum;
}


/**
 * @brief    Get the maximum event connection fail count value.
 */
void  tmlCollectCall::eventGetMaxConnectionFailCount(unsigned int* iCount){
  *iCount = m_iMaxEventConnectionFails;
}


/**
 * @brief    Get the number of destinations for the requested profile / event
 */
void tmlCollectCall::getEventDestinationCount(tmlCollectCallDestinationObjHandler* destinationObjHandler, const char* profile, int* iDestinations){
  TML_INT32 iRet = TML_SUCCESS;
  TML_ON_POPULATE_CB_FUNC pCBFunc;
  TML_POINTER pCBData;

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Ask for the number of registered destinatation:
  iRet = destinationObjHandler->getSubscribedEventDestinationCount(profile, iDestinations, true);
  if (TML_ERR_DESTINATION_OBJ_NOT_FOUND == iRet){
    /////////////////////////////////////////////////////////////////
    // We don't have any registered destination, so try to call
    // the populate callback:
    tmlUnicodeID iUnicode;
    iRet = destinationObjHandler->getEventRegisteredPopulateCallback(profile, &pCBFunc, &pCBData, &iUnicode);
    if (TML_SUCCESS == iRet && NULL != pCBFunc){
      ///////////////////////////////////////////////////////////////////////////////////////////////
      // Callback is valid, so try to call the callback in the manner:
      //  TML_INT32 pCBFunc (char* sProfile, TML_POINTER pCBData)
      switch (iUnicode){
        case idUNICODE_ASCII:
              iRet = ((TML_INT32(FUNC_C_DECL *)(char*, TML_POINTER))pCBFunc)((char*) profile, pCBData);
              break;
        case idUNICODE_WCHAR_T:
              {
                TML_INT32 iUTF16Length;
                wchar_t* utf16profile = UTF8toUTF32((char*)profile, &iUTF16Length);
                if (NULL != utf16profile){
                    iRet = ((TML_INT32(FUNC_C_DECL *)(wchar_t*, TML_POINTER))pCBFunc)(utf16profile, pCBData);
                  delete (utf16profile);
                }
                else{
                  iRet = TML_ERR_UNICODE;
                }
              }
              break;
        case idUNICODE_CHAR16_T:
              {
                TML_INT32 iUTF16Length;
                char16_t* utf16profile = (char16_t*)UTF8toUTF16((char*)profile, &iUTF16Length);
                if (NULL != utf16profile){
                    iRet = ((TML_INT32(FUNC_C_DECL *)(char16_t*, TML_POINTER))pCBFunc)(utf16profile, pCBData);
                  delete utf16profile;
                }
                else{
                  iRet = TML_ERR_UNICODE;
                }
              }
              break;
      }

      if (TML_SUCCESS == iRet){
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // Ask for the number of registered destinatation again:
        iRet = destinationObjHandler->getSubscribedEventDestinationCount(profile, iDestinations, true);
      }
    }
  }
}


/**
  * @brief    Get the number of destinations for the requested profile / load balanced
  */
void tmlCollectCall::getLoadBalancedDestinationCount(tmlCollectCallDestinationObjHandler* destinationObjHandler, const char* profile, int* iDestinations, bool bCallPopulate)
{
  TML_INT32 iRet = TML_SUCCESS;
  TML_ON_POPULATE_CB_FUNC pCBFunc;
  TML_POINTER pCBData;

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Ask for the number of registered destinatation:
  iRet = destinationObjHandler->getSubscribedLoadBalancedDestinationCount(profile, iDestinations, true);
  if (TML_ERR_DESTINATION_OBJ_NOT_FOUND == iRet && bCallPopulate){
    /////////////////////////////////////////////////////////////////
    // We don't have any registered destination, so try to call
    // the populate callback:
    tmlUnicodeID iUnicode;
    iRet = destinationObjHandler->getLoadBalancedRegisteredPopulateCallback(profile, &pCBFunc, &pCBData, &iUnicode);
    if (TML_SUCCESS == iRet && NULL != pCBFunc){
      ///////////////////////////////////////////////////////////////////////////////////////////////
      // Callback is valid, so try to call the callback in the manner:
      //  TML_INT32 pCBFunc (char* sProfile, TML_POINTER pCBData)
      switch (iUnicode){
        case idUNICODE_ASCII:
              iRet = ((TML_INT32(FUNC_C_DECL *)(char*, TML_POINTER))pCBFunc)((char*) profile, pCBData);
              break;
        case idUNICODE_WCHAR_T:
              {
                TML_INT32 iUTF16Length;
                wchar_t* utf16profile = UTF8toUTF32((char*)profile, &iUTF16Length);
                if (NULL != utf16profile){
                    iRet = ((TML_INT32(FUNC_C_DECL *)(wchar_t*, TML_POINTER))pCBFunc)(utf16profile, pCBData);
                  delete (utf16profile);
                }
                else{
                  iRet = TML_ERR_UNICODE;
                }
              }
              break;
        case idUNICODE_CHAR16_T:
              {
                TML_INT32 iUTF16Length;
                char16_t* utf16profile = (char16_t*)UTF8toUTF16((char*)profile, &iUTF16Length);
                if (NULL != utf16profile){
                    iRet = ((TML_INT32(FUNC_C_DECL *)(char16_t*, TML_POINTER))pCBFunc)(utf16profile, pCBData);
                  delete utf16profile;
                }
                else{
                  iRet = TML_ERR_UNICODE;
                }
              }
              break;
      }

      if (TML_SUCCESS == iRet){
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // Ask for the number of registered destinatation again:
        iRet = destinationObjHandler->getSubscribedLoadBalancedDestinationCount(profile, iDestinations, true);
      }
    }
  }
}


int tmlCollectCall::activityStateRequest(tmlCollectCallDestinationObjHandler* destinationObjHandler, const char* profile, int iWindowSize, int index, ActivityStateRequestData* activityState){
  TML_INT32 iRet = TML_SUCCESS;

  tmlCollectCallDestinationObj* destinationObj = NULL;

  ////////////////////////////////////////////////
  // Fetch the destination object:
  iRet = destinationObjHandler->getSubscribedLoadBalancedDestinationByIndex(profile, index, &destinationObj);
  if (TML_SUCCESS == iRet){
    activityState->destinationObj = destinationObj;
    char* sHost = NULL;
    char* sPort = NULL;
    destinationObj->getMessageDestinationHost(&sHost);
    destinationObj->getMessageDestinationPort(&sPort);

    ////////////////////////////////////////////////
    // Command object:
    TML_COMMAND_HANDLE cmdHandle;
    iRet = tml_Cmd_Create(&cmdHandle);
    SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
    if (TML_SUCCESS == iRet){
      iRet = tml_Cmd_Header_SetCommand(cmdHandle, CMD_INTERNAL_LOAD_BALANCED_BUSY_STATE_REQUEST);
    }
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    tml_Cmd_Acquire_Sidex_Handle(cmdHandle, &sHandle);
    if (TML_SUCCESS == iRet){
      //////////////////////////////////////////////////////////////////////////////////////////////////////
      // Flag for internal command:
      iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    }
    if (TML_SUCCESS == iRet){
      iRet = sidex_String_Write(sHandle, TML_CMD_INTERNAL_GROUP, TML_CMD_INTERNAL_KEY_PROFILE, profile);
    }
    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    tml_Cmd_Release_Sidex_Handle(cmdHandle);
    if (TML_SUCCESS == iRet){
      ////////////////////////////////////////////////
      // Send Command for the request
      iRet = sender_SendSyncMessage(profile, sHost, sPort, iWindowSize, cmdHandle, DEFAULT_CONNECTION_TIMEOUT, NULL, true);
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Note: If iRet == TML_ERR_INFORMATION_UNDEFINED, the callback function on the other side isn't implemented.
    }
    if (TML_SUCCESS == iRet){
      ////////////////////////////////////////////////
      // Set the return value:
      activityState->activityState = cmdHandle;
    }
    else{
      ////////////////////////////////////////////////
      // Free memory on error:
      tml_Cmd_Free(&cmdHandle);
      ////////////////////////////////////////////////////////////////////////////////////////////////////////
      // TML_ERR_INFORMATION_UNDEFINED tells that destination is reachable but cannot calculate busy value,
      // so don't do "incrementFailCounter" in that case
      if (TML_ERR_INFORMATION_UNDEFINED != iRet){
        ////////////////////////////////////////////////
        // Set flag for a failed request:
        destinationObj->incrementFailCounter();
      }
    }
  }
  if (TML_SUCCESS != iRet){
    ////////////////////////////////////////////////
    // Return NULL on error:
    activityState->destinationObj = NULL;
    activityState->activityState = TML_HANDLE_TYPE_NULL;
  }
  return (int)iRet;
}


/**
 * @brief    Get the destinationObj for the next load balances destination
 */
int tmlCollectCall::getNextLoadBalancedDestination(tmlCollectCallDestinationObjHandler* destinationObjHandler, const char* profile, int iWindowSize, int iDestinations, bool* bAnyFailFound, tmlCollectCallDestinationObj** destinationObj)
{
  TML_INT32 iRet = TML_SUCCESS;

  TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc = NULL;
  TML_POINTER pCBData = NULL;
  tmlCollectCallDestinationObj* tempDestinationObj;

  bool bGetNextRoundRobin = true;

  destinationObjHandler->getRegisteredLoadBalanceCalculator(profile, &pCBFunc, &pCBData);
  if (NULL != pCBFunc){
    /////////////////////////////////////////////////////
    // There is a registered load balance calculator :

    ActivityStateRequestData* activityState = new ActivityStateRequestData[iDestinations];

    /////////////////////////////////////////////////////
    // Ask all destinations for the BUSY STATE:
    for (int i = 0; i < iDestinations; ++i){
      activityState[i].activityState = TML_HANDLE_TYPE_NULL;
      activityState[i].destinationObj = NULL;
      // Activity state request / don't mind of the result:
      activityState[i].iRequestRet = activityStateRequest(destinationObjHandler, profile, iWindowSize, i, &(activityState[i]));
    }

    if (TML_SUCCESS == iRet){
      ////////////////////////////////////////////////////
      // Let's have a loop how many valid requests we have:
      TML_INT32 iCount = 0;
      bool bAnyUndefinedFound = false;

      for (int i = 0; i < iDestinations; ++i){
        if (activityState[i].activityState != TML_HANDLE_TYPE_NULL){
          activityState[i].iIndex = iCount++;
        }
        else{
          if (activityState[i].iRequestRet == TML_SUCCESS || activityState[i].iRequestRet == TML_ERR_INFORMATION_UNDEFINED){
            activityState[i].iIndex = iCount++;
            bAnyUndefinedFound = true;
          }
          else{
            activityState[i].iIndex = -1;
          }
        }
      }
      //////////////////////////////////////////
      // Any fail found ?
      *bAnyFailFound = (iCount != iDestinations);

      //////////////////////////////////////////
      // Is there one or more valid destination ?
      if (iCount && !bAnyUndefinedFound){
        ////////////////////////////////////////////////////
        // Copy only he valid return data:
        TML_COMMAND_HANDLE* activityStateCmd = new TML_COMMAND_HANDLE[(int)iCount];
        int iIdx = 0;
        for (int i = 0; i < iDestinations; ++i){
          if (activityState[i].activityState != TML_HANDLE_TYPE_NULL)
            activityStateCmd[iIdx++] = activityState[i].activityState;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        // try to call the callback for the calculation in the manner:
        //  TML_INT32 pCBFunc (TML_INT32 iNumberOfListener, TML_COMMAND_HANDLE* listenerBusyStateArray, TML_POINTER pCBData, TML_INT32* iNextListenerIndex)<br>
        TML_INT32 iNextDestinationIndex = 0;
        iRet = ((TML_INT32(FUNC_C_DECL *)(TML_INT32,TML_COMMAND_HANDLE*, TML_POINTER, TML_INT32*))pCBFunc)(iCount, activityStateCmd, pCBData, &iNextDestinationIndex);
        if (TML_SUCCESS == iRet){
          bGetNextRoundRobin = false;
          /////////////////////////////////////////////////////////////////////
          // Calculate the destination object out of the iNextDestinationIndex:
          bool bFound = false;
          for (int i = 0; i < iDestinations; ++i){
            if (activityState[i].activityState != TML_HANDLE_TYPE_NULL){
              if (activityState[i].iIndex == iNextDestinationIndex){
                tempDestinationObj = activityState[i].destinationObj;
                bFound = true;
              }
            }
          }
          if (!bFound){
            iRet = TML_ERR_LOAD_BALANCE_CALCULATION;
          }
        }
        else{
          /////////////////////////////////////////////////////////////////////////////////////////////////
          // If iRet == TML_ERR_INFORMATION_UNDEFINED, the callback function want us to do the round robin.
          if (TML_ERR_INFORMATION_UNDEFINED != iRet){
            bGetNextRoundRobin = false;
            iRet = TML_ERR_LOAD_BALANCE_CALCULATION;
          }
        }
        /////////////////////////////////////////////
        // I don't need the activityStateCmd anymore:
        delete[] activityStateCmd;
      }
    }
    //////////////////////////////////////////////////////////////
    //  Free allocated memory:
    for (int i = 0; i < iDestinations; ++i){
      if (NULL != activityState[i].destinationObj){
        tml_Cmd_Free(&(activityState[i].activityState));
      }
    }
    delete[] activityState;
  }
  if (bGetNextRoundRobin){
    ///////////////////////////////////////////////////////////////////////////
    // There is no registered load balance calculator / I do make round robin:

    ///////////////////////////////////////////////////////////////////////////////
    // Skip to the next load balanced destination / don't mind of the return value
    m_collectCallDestObjHandler->skipNextLoadBalancedDestination(profile);

    ///////////////////////////////////////////////////////////////////
    //  Get the destination for this loop:
    iRet = m_collectCallDestObjHandler->getNextLoadBalancedDestination(profile, &tempDestinationObj);
  }
  if (TML_SUCCESS == iRet){
    *destinationObj = tempDestinationObj;
  }

  return (int)iRet;
}

/**
  * @brief Inform about remove of an Event message out of the queue
  */
bool tmlCollectCall::eventMessageRemoveInfo(tmlCollectCallDestinationObj* destinationObj){
  bool bLogPrinted = false;

  if (NULL != destinationObj){
    TML_INT64 iActCounter = destinationObj->getEventQueueRemoveCounter();
    TML_INT64 iLastLog = destinationObj->getLastRemoveEventLogCounter();
    if ((iActCounter - iLastLog) >= m_iEventLogPeriod){
      // I have To log:
      destinationObj->setLastRemoveEventLogCounter(iActCounter);

      char* sHost;
      destinationObj->getMessageDestinationHost(&sHost);
      char* sPort;
      destinationObj->getMessageDestinationPort(&sPort);
      m_log->log (TML_LOG_EVENT, "Event Removement", sHost, sPort, iActCounter);
      bLogPrinted = true;
    }
  }
  else{
    ++m_iEventQueueRemoveCounter;

    if ((m_iEventQueueRemoveCounter - m_iLastEventLog) >= m_iEventLogPeriod){
     // I have To log:
      m_iLastEventLog = m_iEventQueueRemoveCounter;

      m_log->log (TML_LOG_EVENT, "Event Removement", "Input", "Queue", m_iEventQueueRemoveCounter);
      bLogPrinted = true;
    }
  }
  return bLogPrinted;
}
