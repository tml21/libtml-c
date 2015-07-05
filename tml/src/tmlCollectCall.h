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

#ifndef TMLCOLLECTCALL_H
#define TMLCOLLECTCALL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globalDefines.h"
#include "tmlSingleCall.h"
#include "tmlEventCallHandler.h"
#include "tmlCCallback.h"
#include "tmlCollectCallDestinationObjHandler.h"


//////////////////////////////////////////////////////////////////////////////
// C - declarations:
extern "C" {

/**
 * @brief  callback method for event messages to free allocated memory
 *
 * @param   tmlhandle       Reference to an instance of TML_COMMAND_HANDLE containing the data to analyse and free
 * @param   data            callback method's data.
 */
void FUNC_C_DECL asyncEventMessageReturnMethod(TML_COMMAND_HANDLE tmlhandle, TML_POINTER data);

}// extern "C"

class tmlCoreWrapper;

class  tmlCollectCall : public tmlSingleCall
{

private:
    /* data */

    /**
     * @brief    Actual event counter
     */
    TML_INT64   m_iEventQueueRemoveCounter;

    /**
     * @brief    Actual event counter
     */
    TML_INT64   m_iLastEventLog;

    /**
     * @brief    Period for event log
     */
    unsigned int   m_iEventLogPeriod;

    /**
     * @brief    Maximal length of queued event messages
     */
    unsigned int         m_maxEventMsgQueueLength;


    /**
     * @brief    Handling of event messages in background;
     */
    tmlEventCallHandler*        m_eventCallHandler;

    /**
     * @brief  Datastructure contains the parameter for the event messages
     */
    EventMsgHandlingParams m_EventMessageHandlingParams;

    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;

    /**
     * @brief    The maximum load balanced connection fail counter for subscribed load balanced destinations.
     */
    unsigned int m_iMaxLoadBalancedConnectionFails;

    /**
     * @brief    The maximum event connection fail counter for subscribed load balanced destinations.
     */
    unsigned int m_iMaxEventConnectionFails;

    /**
     * @brief    Flag, shows that destruction is in process
     */
    bool m_bInShutdown;
    
    /* methods */

    /**
     * @brief    Get the number of destinations for the requested profile / event
     *
     * @param   destinationObjHandler         Reference to the tmlCollectCallDestinationObjHandler that holds the list of destinations.
     * @param   profile                       URI of the profile for the requested operation.
     * @param   iDestinations                 Deference to the number of destinations.
     */
    void getEventDestinationCount(tmlCollectCallDestinationObjHandler* destinationObjHandler, const char* profile, int* iDestinations);


    /**
     * @brief    Get the number of destinations for the requested profile / load balanced
     *
     * @param   destinationObjHandler         Reference to the tmlCollectCallDestinationObjHandler that holds the list of destinations.
     * @param   profile                       URI of the profile for the requested operation.
     * @param   iDestinations                 Deference to the number of destinations.
     + @param   bCallPopulate                 If true, it will call a registered Onpopulate callback in case of a empty list
     */
    void getLoadBalancedDestinationCount(tmlCollectCallDestinationObjHandler* destinationObjHandler, const char* profile, int* iDestinations, bool bCallPopulate);


    /**
     * @brief    Get the BUSY STATE for the  destination with the requested index.
     *
     * @param   destinationObjHandler Reference to the tmlCollectCallDestinationObjHandler that holds the list of destinations.
     * @param   profile               URI of the profile for the requested operation.
     * @param   iWindowSize           The window size.
     * @param   index                 Index of destination.
     * @param   activityState         Reference TML_COMMAND_HANDLE witth the result of the request.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int activityStateRequest(tmlCollectCallDestinationObjHandler* destinationObjHandler, const char* profile, int iWindowSize, int index, ActivityStateRequestData* activityState);


    /**
     * @brief    Get the destinationObj for the next load balanced destination
     *
     * @param   destinationObjHandler   Reference to the tmlCollectCallDestinationObjHandler that holds the list of destinations.
     * @param   profile        URI of the profile for the requested operation.
     * @param   iWindowSize    The window size.
     * @param   iDestinations  Number of destinations.
     * @param   destinationObj Reference to the tmlCollectCallDestinationObj.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int getNextLoadBalancedDestination(tmlCollectCallDestinationObjHandler* destinationObjHandler, const char* profile, int iWindowSize, int iDestinations, bool* bAnyFailFound, tmlCollectCallDestinationObj** destinationObj);


    /**
     * @brief   Class callback method that will be called by asyncEventMessageReturnMethod when an async message is finish.
     *
     * @param   callbackData   Reference to a EventMessageReturnDataExtension object.
     *
     * @returns always true.
     *
     */
    bool InternalAsyncEventMessageReturnMethod(void* callbackData);


protected: 
    /* data */
    /**
     * @brief    Reference to the class callback handling method for event message return values
     */
    TCallback<tmlCollectCall> m_internalAsyncEventMessageReturnMethod;

    /**
     * @brief    Reference to the class callback handling method for background event messages called by tmlEventCall
     */
    TCallback<tmlCollectCall> m_internalEventMessage2ndStep;

    /**
     * @brief    Reference to the class callback handling method for background event messages called by tmlEventCallHandler
     */
    TCallback<tmlCollectCall> m_internalEventMessage1stStep;
    

    /**
     * @brief    A mutex to protect critial sections
     */
    VortexMutex       m_mutexBackgroundEventMsg;
    int               m_iMutexBackgroundEventMsgLockCount;


    /**
     * @brief    A mutex to protect critial sections
     */
    VortexMutex       m_mutexCollectCallCriticalSection;
    int               m_iMutexCollectCallCriticalSectionLockCount;
    VortexMutex       m_mutexCollectCallCriticalSection1stStep;
    int               m_iMutexCollectCallCriticalSectionLockCount1stStep;
    VortexMutex       m_mutexCollectCallCriticalSection2ndStep;
    int               m_iMutexCollectCallCriticalSectionLockCount2ndStep;



public:
    /* data */

    /* methods */

    /**
     * @brief    Constructor.
     *
     * @param   ctx          Reference Vortex execution context.
     * @param   loghandler   Reference to a log handler.
     * @param   pHandler     Reference to tmlProfileHandler.
     * @param   pCore        Reference to the tmlCore.

     * @returns an instance of tmlCollectCall
     */
    tmlCollectCall(VortexCtx* ctx, tmlLogHandler* loghandler, tmlProfileHandler* pHandler, tmlCoreWrapper* pCore);


    /**
     * @brief    Destructor.
     */
    ~tmlCollectCall();


    /**
     * @brief    Subscribe a destination for event messages.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventSubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort);


    /**
     * @brief    Get subscribed destinations for event messages for the provided profile.
     *
     * @param   profile       Profile name of the request.<br>
     *                        If empty or TML_HANDLE_TYPE_NULL the subscriptions for all known profiles will be returned.
     * @param   subscriptions reference to a SIDEX_VARIANT / to be interpreted as a table containing the requested result.<br>
     *                        The table has three columns with the names "PROFILE", "HOST" and "PORT".<br>
     *                        The table cell contents are SIDEX_VARIANT values of the type SIDEX_DATA_TYPE_STRING.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see sidexStdTypes.h, tmlErrors.h
     */
    int eventGetSubscribedMessageDestinations(const char* profile, SIDEX_VARIANT* subscriptions);


    /**
     * @brief    Register a callback method for the case of a peer subscription / unsubscription request for event messages.
     *
     * This will register a callback method that will be called in case of a peer subscription request for event messages.<br>
     * See eventSubscribeMessageDestination() / eventUnsubscribeMessageDestination().<br>
     * The callback will be called in the manner:<br>
     * TML_BOOL pCBFunc (TML_BOOL bSubscribe, char* sHost, char* sPort, TML_POINTER pCBData)<br>
     * -bSubscribe  TML_TRUE in case of subscription / TML_FALSE in case of unsubscription.<br>
     * -sHost       host identification.<br>
     * -sPort       port identification.<br>
     * -pCBData     Reference to additional parameter of the callback method /may be NULL.<br>
     * Returns TML_TRUE if the subscription / unsubscription should be performed, otherwise TML_FALSE.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     * @param   iUnicode   unicode identifier
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.
     *
     * @see tmlErrors.h
     */
    int eventRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Get the registered callback method for the case of a peer subscription / unsubscription request for event messages.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     * @param   iUnicode   Reference to unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getRegisteredEventOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode);


    /**
     * @brief    Unsubscribe a destination for event messages.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if the requested parameter are not found.
     *
     * @see tmlErrors.h
     */
    int eventUnsubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort);


    /**
     * @brief    Unsubscribe all destinations for event messages for the provided profile.
     *
     * @param   profile       Profile name of the request.<br>
     *                        If empty or TML_HANDLE_TYPE_NULL all known profiles will be unsubscribed.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventUnsubscribeAllMessageDestinations(const char* profile);


    /**
     * @brief    Register a callback method that will be called in case of an error during event message send execution.
     *
     * The callback will be called in the manner:<br>
     * void pCBFunc (char* sProfile, char* sHost, char* sPort, TML_COMMAND_ID_TYPE iCMD, TML_INT32 iErrorCode, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     * @param   iUnicode   unicode identifier
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventRegisterCallbackOnError(const char* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Make a common deregistration / event
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventDeregisterAll();


    /**
     * @brief    Make a common deregistration / load balanced
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedDeregisterAll();


    /**
     * @brief    Register a callback method that will be called in case of event message queue overflow.
     *
     * If the event message processing is slower than the time of incoming event message requests,<br>
     * they will be queued internal. If the number of queue elements is more than a maximum, the oldest queue<br>
     * entry will be removed. In that case a registered callback will be called.
     *
     * The callback will be called in the manner:<br>
     * void pCBFunc (char* sProfile, TML_COMMAND_ID_TYPE iCMD, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     * @param   iUnicode   unicode identifier
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventRegisterCallbackOnQueueOverflow(const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Register a callback method for the case of non subscribed destinations for event messages.
     *
     * This will register a callback method that will be called in case of an event message to a profile without a subscribed destination.<br>
     * See eventSendMessage()<br>
     * The callback returns the result of the destination registration and will be called in the manner:<br>
     * int pCBFunc (char* sProfile, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     * @param   iUnicode   unicode identifier
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Remove all queue entrys more than m_maxEventMsgQueueLength out of the event message queue
     */
    void eventQueueOverflowHandling(tmlEventDataQueue* pDataQueue, TML_COMMAND_ID_TYPE iCmd, const char* profile, tmlCollectCallDestinationObj* destinationObj);

    /**
     * @brief    Send an event message.
     *
     * This will send the TML_COMMAND_HANDLE to all registered destinations for the requested profile.<br>
     * See  eventSubscribeMessageDestination().
     * If there is no registered destination, a callback method, if registered, will be called to perform the destination registration.<br>
     * See  eventRegisterPopulateCallback().
     * If this succeeds the TML_COMMAND_HANDLE will be send to all registered destinations afterwards.
     *
     * @param   iWindowSize The window size.
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data to send.<br>
     *                      An intenal copy will be made, so it can be freed afterwards.
     * @param   profile     URI of the profile for the requested operation.
     * @param   iTimeout    Timeout for the command execution (in ms).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INITIALIZATION in case of a common initialisation problem.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SYSTEMRESOURCES if communication is not possible because of missing system resources.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED if the profile is not supported by the server.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registered destination has missing parameter.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int eventSendMessage(int iWindowSize, TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout);


    /**
     * @brief    Send an event message (in the background) 1st step called by tmlEventCallHandler
     *
     * @param   callbackData  Reference to the callback data structure.
     *
     * @returns true in case of success, otherwise false.
     *
     * @see tmlErrors.h
     */
    bool eventSendMessage1stStep(void* callbackData);


    /**
     * @brief    Send an event message (in the background) 2nd step called by tmlEventCall
     *
     * @param   callbackData  Reference to the callback data structure.
     *
     * @returns true in case of success, otherwise false.
     *
     * @see tmlErrors.h
     */
    bool eventSendMessage2ndStep(void* callbackData);


    /**
     * @brief    Subscribe a destination for load balanced messages.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedSubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort);


    /**
     * @brief    Get subscribed destinations for load balanced messages for the provided profile.
     *
     * @param   profile       Profile name of the request.<br>
     *                        If empty or TML_HANDLE_TYPE_NULL the subscriptions for all known profiles will be returned.
     * @param   subscriptions reference to a SIDEX_VARIANT / to be interpreted as a table containing the requested result.<br>
     *                        The table has three columns with the names "PROFILE", "HOST" and "PORT".<br>
     *                        The table cell contents are SIDEX_VARIANT values of the type SIDEX_DATA_TYPE_STRING.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see sidexStdTypes.h, tmlErrors.h
     */
    int loadBalancedGetSubscribedMessageDestinations(const char* profile, SIDEX_VARIANT* subscriptions);


    /**
     * @brief    Register a callback method for the case of a peer subscription / unsubscription request for load balanced messages.
     *
     * This will register a callback method that will be called in case of a peer subscription request fo load balanced messages.<br>
     * See loadBalancedSubscribeMessageDestination() / loadBalancedUnsubscribeMessageDestination().<br>
     * The callback will be called in the manner:<br>
     * TML_BOOL pCBFunc (TML_BOOL bSubscribe, char* sHost, char* sPort, TML_POINTER pCBData)<br>
     * -bSubscribe  TML_TRUE in case of subscription / TML_FALSE in case of unsubscription.<br>
     * -sHost       host identification.<br>
     * -sPort       port identification.<br>
     * -pCBData     Reference to additional parameter of the callback method /may be NULL.<br>
     * Returns TML_TRUE if the subscription / unsubscription should be performed, otherwise TML_FALSE.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     * @param   iUnicode   unicode identifier
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.
     *
     * @see tmlErrors.h
     */
    int loadBalancedRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Get the registered callback method for the case of a peer subscription / unsubscription request for load balanced messages.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     * @param   iUnicode   Reference to unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getRegisteredLoadBalancedOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode);


    /**
     * @brief    Unsubscribe a destination for load balanced messages.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if the requested parameter are not found.
     *
     * @see tmlErrors.h
     */
    int loadBalancedUnsubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort);


    /**
     * @brief    Unsubscribe all destinations for load balanced messages for the provided profile.
     *
     * @param   profile       Profile name of the request.<br>
     *                        If empty or TML_HANDLE_TYPE_NULL all known profiles will be unsubscribed.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedUnsubscribeAllMessageDestinations(const char* profile);


    /**
     * @brief    Register a callback method for the case of non subscribed destinations for load balanced messages.
     *
     * This will register a callback method that will be called in case of an event message to a profile without a subscribed destination.<br>
     * See eventSendMessage()<br>
     * The callback returns the result of the destination registration and will be called in the manner:<br>
     * int pCBFunc (char* sProfile, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     * @param   iUnicode   unicode identifier
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc,TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Register a callback method for the BUSY STATUS requests.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedRegisterBusyStatusCallback(const char* profile,TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Get the registered callback method that will be called in case of BUSY STATUS requests.
     *
     * In case of an undefined destination for the requested profile the callback will be called in the following manner:
     * pCBFunc (char* sProfile, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getRegisteredBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Register a callback method for the load balance calculation.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedRegisterCalculator(const char* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Send a synchron load ballanced message.
     *
     * This will send the TML_COMMAND_HANDLE synchron to one of the registered destinations for the  requested profile.<br>
     * See  loadBalancedSubscribeMessageDestination().
     * If there is no registered destination, a callback method, if registered, will be called to perform the destination registration.<br>
     * See  loadBalancedRegisterPopulateCallback().
     * If this succeeds the TML_COMMAND_HANDLE will be send to all registered destination afterwards.
     *
     * @param   iWindowSize The window size.
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data to send.<br>
     *                      An intenal copy will be made, so it can be freed afterwards.
     * @param   profile     URI of the profile for the requested operation.
     * @param   iTimeout    Timeout for the command execution (in ms).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SYSTEMRESOURCES if communication is not possible because of missing system resources.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.<br>
     *          TML_ERR_CHANNEL_NOT_INITIALIZED if the channel initialization failled.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registered destination has missing parameter<br>
     *          TML_ERR_LOAD_BALANCE_CALCULATION in case of an load balance calculation error.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int loadBalancedSendSyncMessage(int iWindowSize, TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout);


    /**
     * @brief    Send an asynchron load ballanced message.
     *
     * This will send the TML_COMMAND_HANDLE synchron to one of the registered destinations for the  requested profile.<br>
     * See  loadBalancedSubscribeMessageDestination().
     * If there is no registered destination, a callback method, if registered, will be called to perform the destination registration.<br>
     * See  loadBalancedRegisterPopulateCallback().
     * If this succeeds the TML_COMMAND_HANDLE will be send to all registered destination afterwards.
     *
     * @param   iWindowSize The window size.
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data to send.<br>
     *                      An intenal copy will be made, so it can be freed afterwards.
     * @param   profile     URI of the profile for the requested operation.
     * @param   iTimeout    Timeout for the command execution (in ms).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INITIALIZATION in case of a common initialisation problem.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SYSTEMRESOURCES if communication is not possible because of missing system resources.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED if the profile is not supported by the server.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registered destination has missing parameter<br>
     *          TML_ERR_LOAD_BALANCE_CALCULATION in case of an load balance calculation error.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int loadBalancedSendAsyncMessage(int iWindowSize, TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout);


    /**
     * @brief    Set the maximum load balanced connection fail count value.
     *
     * On every fail of a connection of a subscribed message destination an internal counter will be incremented.<br>
     * If this counter is equals or more than the requested value, the destination will be unsubscribed.
     * In case of a successful connection / message transfer the internal counter will be reset.
     * 
     * @param   iCount      The number of fail connections / if 0 the destination will not be unsubscribed anytime.
     */
    void loadBalancedSetMaxConnectionFailCount(unsigned int iCount);


    /**
     * @brief    Get the maximum load balanced connection fail count value.
     *
     * On every fail of a connection of a subscribed message destination an internal counter will be incremented.<br>
     * If this counter is equals or more than the requested value, the destination will be unsubscribed.<br>
     * In case of a successful connection / message transfer the internal counter will be reset.
     * 
     * @param   iCount      Reference to the number of fail connections / if 0 the destination will not be unsubscribed anytime.
     */
    void loadBalancedGetMaxConnectionFailCount(unsigned int* iCount);


    /**
     * @brief    Set the maximum event connection fail count value.
     *
     * On every fail of a connection of a subscribed message destination an internal counter will be incremented.<br>
     * If this counter is equals or more than the requested value, the destination will be unsubscribed.
     * In case of a successful connection / message transfer the internal counter will be reset.
     * 
     * @param   iCount      The number of fail connections / if 0 the destination will not be unsubscribed anytime.
     */
    void eventSetMaxConnectionFailCount(unsigned int iCount);


    /**
     * @brief    Set the maximum value of queued event messages
     *
     * If the event message processing is slower than the time of incoming event message requests,
     * they will be queued internal. If the number of queue elements is more than a maximum, the oldest queue
     * entry will be removed. This API call changes this maximum value.
     * 
     * @param   iMaximum      The maximum value of queued event messages
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    void eventSetMaxQueuedEventMessages(unsigned int iMaximum);


/**
     * @brief    Get the maximum event connection fail count value.
     *
     * On every fail of a connection of a subscribed message destination an internal counter will be incremented.<br>
     * If this counter is equals or more than the requested value, the destination will be unsubscribed.<br>
     * In case of a successful connection / message transfer the internal counter will be reset.
     * 
     * @param   iCount      Reference to the number of fail connections / if 0 the destination will not be unsubscribed anytime.
     */
    void eventGetMaxConnectionFailCount(unsigned int* iCount);


    /**
     * @brief Inform about remove of an Event message out of the queue
     *
     * @returns true if a message was printed
     */
    bool eventMessageRemoveInfo(tmlCollectCallDestinationObj* destinationObj);

};
#endif //TMLCOLLECTCALL_H

