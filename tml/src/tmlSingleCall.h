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
 
#ifndef TMLSINGLECALL_H
#define TMLSINGLECALL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlConnectionObj.h"
#include "tmlLogHandler.h"
#include "tmlCCallback.h"
#include <vortex.h>
#include "tmlProfileHandler.h"
#include "tmlCollectCallDestinationObjHandler.h"
#include "tmlConnectionManageObj.h"


class tmlCoreWrapper;

class  tmlSingleCall
{
private:
    /* data */

    /* methods */

    /**
     * @brief   Shutdown all open Connections / has to becalled before CloseAllOpenConnections
     */
    void ShutdownAllOpenConnections();


    /**
     * @brief   Close all open Connections / has to becalled after ShutdownAllOpenConnections
     */
    void CloseAllOpenConnections();


protected:
    /* data */

    /**
     * @brief    Flag, shows that the connection lost callback is deregistered
     */
    bool m_bDeregistered;

    /**
     * @brief    Reference to core handle
     */
    tmlCoreWrapper* m_tmlCoreHandle;

    /**
     * @brief    Reference to profileHandler object
     */
    tmlProfileHandler*     m_pHandler;

    /**
     * @brief    handler for the event message handling
     */
    tmlCollectCallDestinationObjHandler* m_collectCallDestObjHandler;

    /**
     * @brief   Value for the debug log.
     */
    int m_iLogValue;

    /**
     * @brief   Log- File index for explicit logs with closing the file
     */
    int m_iLogFileIndex;

    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;

    /**
     * @brief    Reference to the class callback handling method for connection close
     */
    TCallback<tmlSingleCall> m_internalConnectionCloseHandlerMethod;

    /**
     * @brief    A mutex to protect critial sections
     */
    VortexMutex       m_mutexCriticalSection;
    int               m_iMutexCriticalSectionLockCount;

    /* methods */


    /**
     * @brief   Remove all pending unsubscriptions.
     *
     * @param   bLockCritical  Flag shows if mutex for critical section shall be fetched.
     */
    void RemoveMarkedUnsubscriptions(bool bLockCritical);


    /**
     * @brief   Disconnect all open Connections.
     */
    void DropAllOpenConnections();


    /**
     * @brief    Create the mutex that protect critial section about communication data
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     * @returns axl_true on success.
     */
    axl_bool createCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    Destroy the mutex that protect critial section about communication data
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     * @returns axl_true on success.
     */
    axl_bool destroyCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    Enter a critical Section
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   mutex       Reference to mutex for the critical section
     * @param   iLockCount  Reference to number of locks
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     */
    void enterCriticalSection(int iLogMask, VortexMutex* mutex, int* iLockCount, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    Leave a critical Section
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   iLockCount  Reference to number of locks
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     */
    void leaveCriticalSection(int iLogMask, VortexMutex* mutex, int* iLockCount, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


private:
    /* data */

    /**
     * @brief    hashtable for connectionElements
     */
    TMLUniversalHashTable* m_ConnectionElementHT;


    /**
     * @brief    reference to vortex execution context
     */
    VortexCtx* m_ctx;

    /* methods */


    /**
     * @brief   Class callback method that will be called by the registered callback method in case of a close of the connection (initiated by the listener).
     *
     * @param   connection   Reference to the Vortex connection that has been closed.
     *
     * @returns always true.
     *
     */
     bool SignalConnectionCloseToSender(void* connection);


    /**
     * @brief    Create a Vortex channel for the requested parameters.
     *
     * @param   profile                     URI of the profile for the requested operation.
     * @param   iChannel                    The requested channel ID.
     * @param   iWindowSize                 The window size.
     * @param   connection                  Reference to the destination to create a channel to.
     * @param   channel                     Reference to save the result into.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_CHANNEL_NOT_INITIALIZED if the channel initialization failed.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.
     *
     * @see tmlErrors.h
     */
    int CreateChannel(const char*  profile, int iChannel, int iWindowSize, VortexConnection* m_connection, VortexChannel** channel);


    /**
     * @brief    Check for valid connection
     *
     * @param   connection   Reference to the VortexConnection.
     * @param   channel      Reference to the VortexChannel.
     *
     * @returns true in case of a valid connection, otherwise false,
     */
    bool IsConnectionValid(VortexConnection * connection, VortexChannel* channel);


    /**
     * @brief    Add a Vortex connection element to the connection link list.
     *
     * @param   connectionObj               Reference to the tmlConnectionObj object.
     */
    void AddConnectionElement(tmlConnectionObj* connectionObj, bool bLock);


    /**
     * @brief   Wait until the last sender is idle.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC if async command of any sender didn't finish after expected time.
     *
     * @see tmlErrors.h
     */
    int WaitUntilLastSenderIsIdle();


    /**
     * @brief   Remove all sender marked to be removed.
     *
     * @param   bLockCritical  Flag shows if mutex for critical section shall be fetched.
     */
    void RemoveMarkedSenderOutOfConnectionList(bool bLockCritical);


     /**
      * @brief   Ask if any connection with the requested parameter is locked
      *
      * @param   bLockCritical  Flag shows if mutex for critical section shall be fetched.
      * @param   sRefPort                       The Port.
      * @param   sRefHost                       The host name / IP number.
      * @param   sRefProfile                    URI of the profile for the requested operation.
      */
    bool AnyConnectionLocked(bool bLockCritical, char* sRefPort, char* sRefHost, char* sRefProfile);


    /**
     * @brief    Get a Vortex connection for the requested parameters.
     *
     * @param   profile                     URI of the profile for the requested operation.
     * @param   sHost                       The host name / IP number.
     * @param   sPort                       The Port.
     * @param   connection                  Reference to the destination to save the result.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED if the profile is not supported by the server.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.
     *
     * @see tmlErrors.h
     */
    int GetConnection(const char* profile, const char* sHost, const char* sPort, tmlConnectionManageObj** connectionMgr);


    /**
     * @brief    Search for an existing tmlConnectionObj in a hashtable for the requested parameter
     *
     * @param   profile                     URI of the profile for the requested operation.
     * @param   sHost                       The host name / IP number.
     * @param   sPort                       The Port.
     * @param   connectionObj               Reference to an instance of tmlConnectionObj.
     * @param   bFoundRet                   Reference to foud flag / true if the connection element was found, otherwise false.
     * @param   bRemoveMarkedObjs           Flag if true, pending unsubscription lists will be emptied, if possible
     *
     * @returns TML_SUCCESS on success
     */
    int SearchForConnectionObjInHT(const char* profile, const char* sHost, const char* sPort, tmlConnectionObj** connectionObj, VortexChannelPool** channelPool, bool* bFoundRet, bool bRemoveMarkedObjs);


    /**
     * @brief    Get the Vortex connection element for the requested parameter.
     *
     * @param   profile                     URI of the profile for the requested operation.
     * @param   sHost                       The host name / IP number.
     * @param   sPort                       The Port.
     * @param   pConnectionObj              Reference to an instance of tmlConnectionObj.
     * @param   bRawViaVortexPayloadFeeder  True in case of using the Vortex Payload Feeder API.
     * @param   bRemoveMarkedObjs           Flag if true, pending unsubscription lists will be emptied, if possible
     * @param   connectionMgrObj            An instance of tmlConnectionManageObj
     *
     * @returns true if the connection element was found, otherwise false.
     */
    int GetConnectionElement(const char* profile, const char* sHost, const char* sPort, tmlConnectionObj** pConnectionObj, bool bRawViaVortexPayloadFeeder, bool bRemoveMarkedObjs, tmlConnectionManageObj* connectionMgrObj);


    /**
     * @brief   Set the window size for the following outgoing data.
     *
     * By default the window size is 32768 byte.
     *
     * @param   pConnectionObj   Reference to an instance of tmlConnectionObj.
     * @param   iWindowSize      The window size.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int setChannelWindowSize(tmlConnectionObj* pConnectionObj, int iWindowSize);


public:
    /* data */

    /* methods */

    /**
     * @brief   Deregister connection lost callback messages
     */
    void DeregisterConnectionLost();

    /**
     * @brief   Remove the tmlConnectionManageObj out of the connection object hashtable
     */
    void RemoveConnectionFromHT(tmlConnectionManageObj* tmlConnectionMngObj);

    /**
     * @brief   Deregister connection lost callback messages
     */
    void DeregisterConnectionLost(tmlConnectionManageObj* connectionMgr);

    /**
     * @brief    Deregister connection lost callback messages free resources of connection from sender
     *
     * @param   connectionMgrObj  reference to an ionstance of tmlConnectionManageObj
     */
     void DeregisterConnectionLostAndFree(tmlConnectionManageObj* connectionMgrObj);

     
     /**
     * @brief    Constructor.
     *
     * @param   ctx          Reference Vortex execution context.
     * @param   loghandler   Reference to a log handler.
     * @param   pHandler     Reference to tmlProfileHandler.
     * @param   pCore        Reference to core handle
     *
     * @returns an instance of tmlSingleCall
     */
    tmlSingleCall(VortexCtx* ctx, tmlLogHandler* loghandler, tmlProfileHandler* pHandler, tmlCoreWrapper* pCore);


    /**
     * @brief    Destructor.
     */
    ~tmlSingleCall();


    /**
     * @brief   Set log value to all open Connections.
     *
     * @param   iLog                        The log value.
     */
    void setLoggingValue(int iLog);


    /**
     * @brief    Send a synchron Message.
     *
     * @param   profile        URI of the profile for the requested operation.
     * @param   sHost          The host name / IP number.
     * @param   sPort          The Port.
     * @param   iWindowSize    The window size.
     * @param   tmlhandle      Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   iTimeout       Timeout for the command execution (in ms).
     * @param   mutexCriticalSection  If not NULL the mutex will be unlocked just before sending the messaghe
     * @param   bRemoveMarkedObjs Flag if true, pending unsubscription lists will be emptied, if possible
     * @param   iMode          The command mode (TMLCOM_MODE_SYNC or TMLCOM_MODE_EVT).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_CHANNEL_NOT_INITIALIZED if the channel initialization failled.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC timeout during wait for end of async cmd execution.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
     int sender_SendSyncMessage(const char* profile, const char* sHost, const char* sPort, int iWindowSize, TML_COMMAND_HANDLE tmlhandle, unsigned int iTimeout, VortexMutex* mutexCriticalSection, bool bRemoveMarkedObjs, int iMode);


    /**
     * @brief    Send a synchron Message.
     *
     * @param   profile          URI of the profile for the requested operation.
     * @param   connectionHandle TML connection handle (TML_CORE_HANDLE)
     * @param   iWindowSize      The window size.
     * @param   tmlhandle        Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   iTimeout         Timeout for the command execution (in ms).
     * @param   mutexCriticalSection  If not NULL the mutex will be unlocked just before sending the messaghe
     * @param   bRemoveMarkedObjs Flag if true, pending unsubscription lists will be emptied, if possible
     * @param   iMode          The command mode (TMLCOM_MODE_SYNC or TMLCOM_MODE_EVT).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_CHANNEL_NOT_INITIALIZED if the channel initialization failled.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC timeout during wait for end of async cmd execution.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
     int sender_SendSyncMessage(const char* profile, TML_CONNECTION_HANDLE connectionHandle, int iWindowSize, TML_COMMAND_HANDLE tmlhandle, unsigned int iTimeout, VortexMutex* mutexCriticalSection, bool bRemoveMarkedObjs, int iMode);


    /**
     * @brief    Perform the sending of a synchron Message.
     *
     * @param   connectionObj  reference to an ionstance of tmlConnectionObj
     * @param   iWindowSize    The window size.
     * @param   tmlhandle      Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   iTimeout       Timeout for the command execution (in ms).
     * @param   mutexCriticalSection  If not NULL the mutex will be unlocked just before sending the message
     * @param   iMode       The command mode (TMLCOM_MODE_ASYNC or TMLCOM_MODE_EVT).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_CHANNEL_NOT_INITIALIZED if the channel initialization failled.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC timeout during wait for end of async cmd execution.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
    int perform_SendSyncMessage(tmlConnectionObj* connectionObj, int iWindowSize, TML_COMMAND_HANDLE tmlhandle, unsigned int iTimeout, VortexMutex* mutexCriticalSection, int iMode);


    /**
     * @brief    Send an asynchron Message.
     *
     * @param   profile        URI of the profile for the requested operation.
     * @param   sHost          The host name / IP number.
     * @param   sPort          The Port.
     * @param   iWindowSize    The window size.
     * @param   tmlhandle      Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   iTimeout       Timeout for the command execution (in ms).
     * @param   bLockCritical  Flag shows if mutex for critical section shall be fetched.
     * @param   bRawViaVortexPayloadFeeder True in case of using the Vortex Payload Feeder API.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INITIALIZATION in case of a common initialization problem.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED if the profile is not supported by the server.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.<br>
     *          TML_ERR_COMMAND_MODE_UNDEFINED the mode is unknown.
     *
     * @see tmlErrors.h
     */
     int sender_SendAsyncMessage(const char* profile, const char* sHost, const char* sPort, int iWindowSize, TML_COMMAND_HANDLE tmlhandle, unsigned int iTimeout, bool bLockCritical, bool bRawViaVortexPayloadFeeder);

    /**
     * @brief    Send an asynchron Message.
     *
     * @param   profile          URI of the profile for the requested operation.
     * @param   connectionHandle TML connection handle (TML_CORE_HANDLE)
     * @param   iWindowSize      The window size.
     * @param   tmlhandle        Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   iTimeout         Timeout for the command execution (in ms).
     * @param   bLockCritical    Flag shows if mutex for critical section shall be fetched.
     * @param   bRawViaVortexPayloadFeeder True in case of using the Vortex Payload Feeder API.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INITIALIZATION in case of a common initialization problem.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED if the profile is not supported by the server.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.<br>
     *          TML_ERR_COMMAND_MODE_UNDEFINED the mode is unknown.
     *
     * @see tmlErrors.h
     */
     int sender_SendAsyncMessage(const char* profile, TML_CONNECTION_HANDLE connectionHandle, int iWindowSize, TML_COMMAND_HANDLE tmlhandle, unsigned int iTimeout, bool bLockCritical, bool bRawViaVortexPayloadFeeder);

    /**
     * @brief    Perform the sending of an asynchron Message.
     *
     * @param   connectionObj  reference to an ionstance of tmlConnectionObj
     * @param   iWindowSize    The window size.
     * @param   tmlhandle      Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   iTimeout       Timeout for the command execution (in ms).
     * @param   bLockCritical  Flag shows if mutex for critical section shall be fetched.
     * @param   bRawViaVortexPayloadFeeder  True in case of using the Vortex Payload Feeder API.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_CHANNEL_NOT_INITIALIZED if the channel initialization failled.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC timeout during wait for end of async cmd execution.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
     int perform_SendAsyncMessage(tmlConnectionObj* connectionObj, int iWindowSize, TML_COMMAND_HANDLE tmlhandle, unsigned int iTimeout, bool bLockCritical, bool bRawViaVortexPayloadFeeder);

     
    /**
     * @brief   Set the log- file index for explicit logs with closing the file
     * @param   iFileLogIndex       The index.
     */
     void setLogFileIndex(int iFileLogIndex);
};


#endif //TMLSINGLECALL_H

