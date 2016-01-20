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
 * Contributors:
 *    wobe-systems GmbH
 */

#ifndef TMLCOREWRAPPER_H
#define TMLCOREWRAPPER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlUniversalHashTable.h"
#include "tmlStreamHandler.h"
#include "tmlCoreListener.h"
#include "tmlLogHandler.h"
#include "tmlCollectCall.h"
#include "tmlCriticalSectionObj.h"
#include "tmlProfileHandler.h"

class  tmlCoreWrapper
{

private:
    /* data */

    /**
     * @brief    Reference to tmlProfileHandler object
     */
    tmlProfileHandler*     m_pHandler;

    /**
     * @brief    Flag, if true the core is in shutdown process
     */
    bool                   m_bShutdown;

    tmlCriticalSectionObj* m_csObj;
    /**
     * @brief    handler for a command / message to multiple registered Vortex destinations
     */
    tmlCollectCall* m_sender;

    /**
     * @brief    reference to vortex execution context
     */
    VortexCtx* m_ctx;

    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;

    /**
     * @brief   Log- File index for explicit logs with closing the file
     */
    int m_iLogFileIndex;

    /**
     * @brief    The listener
     */
    TMLCoreListener* m_CoreListener;

    /**
     * @brief    Hash table for the registered dispatcher / profiles
     */
    TMLUniversalHashTable* m_dispatcherHashTable;

    /**
     * @brief    Handler to handle stream object hash tables
     */
    tmlStreamHandler* m_streamHandler;

    /**
     * @brief    Port number for the listener.
     */
    char* m_sListenerPort;

    /**
     * @brief    Port number for the listener / char16_t.
     */
    char16_t* m_sListenerPort_w;
    /**
     * @brief    Port number for the listener / w_chart.
     */
    wchar_t* m_sListenerPort_x;

    /**
     * @brief    IP number for the listener.
     */
    char* m_sListenerIP;

    /**
     * @brief    IP number for the listener / wchar_t.
     */
    char16_t* m_sListenerIP_w;

    /**
     * @brief    IP number for the listener / w_chart.
     */
    wchar_t* m_sListenerIP_x;

    /**
     * @brief    Flag for the listener enable status.
     */
    bool m_bListnerIsEnabled;

    /**
     * @brief    Window size for the outgoing data.
     */
    int m_iWindowSize;

    /**
     * @brief    Connection timeout for the outgoing data.
     */
    unsigned int m_iConnectionTimeout;

    /**
     * @brief   Value for the debug log.
     */
    int m_iLogValue;

    /**
     * @brief    The maximum load balanced connection fail counter for subscribed load balanced destinations.
     */
    unsigned int m_iMaxLoadBalancedConnectionFails;

    /**
     * @brief    The maximum event connection fail counter for subscribed load balanced destinations.
     */
    unsigned int m_iMaxEventConnectionFails;

    /**
     * @brief    Maximal length of queued event messages
     */
    unsigned int m_maxEventMsgQueueLength;

protected: 
    /* data */
    TCallback<tmlCoreWrapper> m_ListenerCallback;


    /* methods */


    /**
     * @brief   Callback will be called by the listener frame receive callback method of the TMLCoreListener.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the message from the listener.
     *
     * @returns false in case of TML_SUCCESS, true  in case of an error.
     *
     */
    bool listenerCallbackMethod(void* tmlhandle);


    /**
     * @brief   Send a Reply.
     *
     * @param   tmlhandle       Reference to an instance of TML_COMMAND_HANDLE that contains the reply data.
     * @param   iRetValue       The return value to add into the reply.
     * @param   bRawReply       It true we have raw data using stream interface
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<
     *
     * @see tmlErrors.h
     */
    int  tmlCoreWrapper_SendReply (TML_COMMAND_HANDLE tmlhandle, int iRetValue, bool bRawReply);


    /**
     * @brief    This method handles all command dispatch actions
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data / command.
     *
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of a read problem referring the command.
     *
     * @see tmlErrors.h
     */
    int cmdDispatch(TML_COMMAND_HANDLE tmlhandle);


    /**
    * @brief   Dispatch of registered dispatch commands.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the message from the listener.
     * @param   iCmd        The command.
     * @param   bEvent      Event identifier / TML_TRUE if it is an event.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance for the requested profile is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int registeredCmdDispatch(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE iCmd, TML_BOOL bEvent);


    /**
     * @brief    This method handles internal commands e.g. subscription requests
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data / command.
     * @param   iCmd        The command identification
     *
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of a read problem referring the command.
     *
     * @see tmlErrors.h
     */
    int internalCmdDispatch(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE iCmd);

    /**
     * @brief    This method handles internal commands e.g. subscription requests
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data / command.
     * @param   sHandle     Reference to DIDEX_HANDLE of the TML_COMMAND_HANDLE containing the data / command.
     * @param   iCmd        The command.
     *
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of a read problem referring the command.
     *
     * @see tmlErrors.h
     */
    int rawCmdDispatch(TML_COMMAND_HANDLE tmlhandle, SIDEX_HANDLE sHandle, TML_COMMAND_ID_TYPE iCmd);


    /**
     * @brief    The initialization called by the constructor
     *
     * @param   iLogValue               The debug logging value.
     * @param   iInitialThreadPoolSize  Initial thread pool size.
     * @param   iThreadPoolMaxSize      Maximal thread pool size.
     * @param   iThreadAddSteps         Thread pool add step.
     * @param   iThreadPoolAddPeriod    Thread pool add period.
     * @param   iThreadRemoveSteps      Thread pool remove step.
     * @param   iThreadPoolRemovePeriod Thread pool remove period.
     * @param   bThreadAutoRemove       Auto remove thread pool.
     * @param   bThreadPreemptive       Auto remove thread pool.<br>
     *        - axl_true :  makes vortex to react faster to create threads<br>
     *                      by checking before running pool tasks.<br>
     *        - axl_false : makes vortex to react slower to create threads<br>
     *                      by checking after running pool tasks.
     */
    void initWrapper(int iLogValue, TML_INT32 iInitialThreadPoolSize, TML_INT32 iThreadPoolMaxSize, 
                                          TML_INT32 iThreadAddSteps, TML_INT32 iThreadPoolAddPeriod, 
                                          TML_INT32 iThreadRemoveSteps, TML_INT32 iThreadPoolRemovePeriod, 
                                          TML_BOOL bThreadAutoRemove, TML_BOOL bThreadPreemptive);


    /**
     * @brief   Helpermethod to unregister allregistered profiles.
     */
    void unregisterAll_Registered_Profiles();


public:
    /* data */
    /* methods */

    /**
     * @brief    Constructor.
     *
     * @param    ctx                      The VORTEX execution context.
     * @param    iLogValue                The debug logging value.
     * @param    iInitialThreadPoolSize   Initial thread pool size.
     * @param    iThreadPoolMaxSize       Maximal thread pool size.
     * @param    iThreadAddSteps          Thread pool add step.
     * @param    iThreadPoolAddPeriod     Thread pool add period.
     * @param    iThreadRemoveSteps       Thread pool remove step.
     * @param    iThreadPoolRemovePeriod  Thread pool remove period.
     * @param    bThreadAutoRemove        Auto remove thread pool.
     * @param    bThreadPreemptive        Auto remove thread pool.<br>
     *        - axl_true :  makes vortex to react faster to create threads<br>
     *                      by checking before running pool tasks<br>
     *        - axl_false : makes vortex to react slower to create threads<br>
     *                      by checking after running pool tasks<br>
     *
     * @returns an instance of tmlCoreWrapper
     */
    tmlCoreWrapper(VortexCtx* ctx, int iLogValue,
                         TML_INT32 iInitialThreadPoolSize, TML_INT32 iThreadPoolMaxSize, 
                        TML_INT32 iThreadAddSteps, TML_INT32 iThreadPoolAddPeriod,
                        TML_INT32 iThreadRemoveSteps, TML_INT32 iThreadPoolRemovePeriod, 
                        TML_BOOL bThreadAutoRemove, TML_BOOL bThreadPreemptive);




    /**
     * @brief    Destructor.
     */
    ~tmlCoreWrapper();


    /**
     * @brief    make all deregistrations / sometimes useful before calling the destructor.
     */
    void tmlCoreWrapper_General_Deregistration();


    /**
     * @brief   Set the callback method for thread creation.
     *
     * @param   pCBCreate   reference to callback method.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Thread_Set_OnCreate(void *pCBCreate);


    /**
     * @brief   Set the callback method for thread destruction.
     *
     * @param   pCBDestroy  reference to callback method.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Thread_Set_OnDestroy(void *pCBDestroy);


    /**
     * @brief   Create an instance of a command dispatcher.
     *
     * @param   profile   The BEEP communication profile.<br>
     *                       If there is already an enabled listener / see tmlCoreWrapper_Enable_Listener(),<br>
     *                       the profile will be registered for the listener.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_ALREADY_EXISTS if a valid dispatcher for the given profile already exists.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Register_Profile(const char* profile);

    /**
     * @brief   Dismiss an instance of a command dispatcher.
     *
     * @param   profile   The BEEP communication profile.<br>
     *                       If there is already an enabled listener / see tmlCoreWrapper_Enable_Listener(),<br>
     *                       the profile will be unregistered for the listener.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance for the requested profile is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Unregister_Profile(const char* profile);


    /**
     * @brief   Get the number of registered profiles for the given coreHandle.
     *
     * @param   iSize      The reference to number of registered profiles.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_Registered_Profiles_Size(TML_INT32* iSize);


    /**
     * @brief   Get the registered profiles.
     *
     * @param   profiles  Reference to the profiles list.<br>
     *                    It's a List object and must be decremented using sidex_Variant_DecRef, if the object is not longer in use.<br>
     *                    Null in case of no registered profile.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_NOPROFILES if the profiles are NULL / no profiles are registered.<br>
     *          TML_ERR_HASH if the hashtable is invalid.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_Registered_Profiles(SIDEX_VARIANT* profiles);


    /**
     * @brief   Check if the profile is registered.
     *
     * @param   profile     URI of the profile to be requested.
     * @param   bRegistered Reference to result. True if profile is registered.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Is_Profile_Registered(char* profile, bool* bRegistered);


    /**
     * @brief   Register a callback method referring a command for the command dispatch process.
     *
     * @param   profile   URI of the profile to be requested.
     * @param   cmd       Command ID that refer the callback method.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method  has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_CMD_NOT_REGISTERED in case of the deregistration of a non registered cmd.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance for the requested profile is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Register_Cmd(const char* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief   Register a callback method that will be invoked in the case of command deregistration out of the command dispatch process.
     *
     * @param   profile   URI of the profile to be requested.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method / may be TML_HANDLE_TYPE_NULL.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance for the requested profile is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_OnDeleteCmd(const char* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief   Register a callback method for non registered dispatch commands.
     *
     * @param   profile   URI of the profile to be requested.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method  has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance for the requested profile is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_OnCustomDispatch(const char* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief   Set the Port number for the listener.
     *
     * Disable the listener before setting this parameter. If the listener is enabled, the parameter will be ignored.<br>
     * See also tmlCoreWrapper_Enable_Listener(), tmlCoreWrapper_Is_Listener_Enabled().
     *
     * @param   sPort      Port number.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_NOT_OPERABLE_AT_THE_MOMENT if the listener is already enabled.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_ListenerPort_A(char* sPort);


    /**
     * @brief   Set the Port number for the listener.
     *
     * Disable the listener before setting this parameter. If the listener is enabled, the parameter will be ignored.<br>
     * See also tmlCoreWrapper_Enable_Listener(), tmlCoreWrapper_Is_Listener_Enabled().
     *
     * @param   sPort      Port number.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_NOT_OPERABLE_AT_THE_MOMENT if the listener is already enabled.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_ListenerPort_X(wchar_t* sPort);


    /**
     * @brief   Set the Port number for the listener.
     *
     * Disable the listener before setting this parameter. If the listener is enabled, the parameter will be ignored.<br>
     * See also tmlCoreWrapper_Enable_Listener(), tmlCoreWrapper_Is_Listener_Enabled().
     *
     * @param   sPort      Port number.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_NOT_OPERABLE_AT_THE_MOMENT if the listener is already enabled.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_ListenerPort_W(char16_t* sPort);


    /**
     * @brief   Get the Port number for the listener.
     *
     * @param   sPort      Reference to store the port number.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_ListenerPort_A(char** sPort);


    /**
     * @brief   Get the Port number for the listener.
     *
     * @param   sPort      Reference to store the port number.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_ListenerPort_X(wchar_t** sPort);


    /**
     * @brief   Get the Port number for the listener.
     *
     * @param   sPort      Reference to store the port number.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_ListenerPort_W(char16_t** sPort);


    /**
     * @brief   Set the IP number for the listener.
     *
     * Disable the listener before setting this parameter. If the listener is enabled, the parameter will be ignored.<br>
     * See also tmlCoreWrapper_Enable_Listener(), tmlCoreWrapper_Is_Listener_Enabled().<br>
     * The IP number 0.0.0.0 will be used by default. In this case all installed network adaper will be used parallel for the communication.
     *
     * @param   sIP        IP number.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_NOT_OPERABLE_AT_THE_MOMENT if the listener is already enabled.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_ListenerIP_A(char* sIP);


    /**
     * @brief   Set the IP number for the listener.
     *
     * Disable the listener before setting this parameter. If the listener is enabled, the parameter will be ignored.<br>
     * See also tmlCoreWrapper_Enable_Listener(), tmlCoreWrapper_Is_Listener_Enabled().<br>
     * The IP number 0.0.0.0 will be used by default. In this case all installed network adaper will be used parallel for the communication.
     *
     * @param   sIP        IP number.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_NOT_OPERABLE_AT_THE_MOMENT if the listener is already enabled.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_ListenerIP_X(wchar_t* sIP);


    /**
     * @brief   Set the IP number for the listener.
     *
     * Disable the listener before setting this parameter. If the listener is enabled, the parameter will be ignored.<br>
     * See also tmlCoreWrapper_Enable_Listener(), tmlCoreWrapper_Is_Listener_Enabled().<br>
     * The IP number 0.0.0.0 will be used by default. In this case all installed network adaper will be used parallel for the communication.
     *
     * @param   sIP        IP number.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_NOT_OPERABLE_AT_THE_MOMENT if the listener is already enabled.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_ListenerIP_W(char16_t* sIP);


    /**
     * @brief   Get the IP number for the listener.
     *
     * The IP number 0.0.0.0 will be used by default. In this case all installed network adaper will be used parallel for the communication.
     *
     * @param   sIP        Reference to store the IP number. It's owned by the tmlCoreWrapper and shouldn't be free by the caller.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_ListenerIP_A(char** sIP);


    /**
     * @brief   Get the IP number for the listener.
     *
     * The IP number 0.0.0.0 will be used by default. In this case all installed network adaper will be used parallel for the communication.
     *
     * @param   sIP        Reference to store the IP number. It's owned by the tmlCoreWrapper and shouldn't be free by the caller.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_ListenerIP_X(wchar_t** sIP);


    /**
     * @brief   Get the IP number for the listener.
     *
     * The IP number 0.0.0.0 will be used by default. In this case all installed network adaper will be used parallel for the communication.
     *
     * @param   sIP        Reference to store the IP number. It's owned by the tmlCoreWrapper and shouldn't be free by the caller.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_ListenerIP_W(char16_t** sIP);


    /**
     * @brief   Enable / Disable the listener for incoming data.
     *
     * @param   bEnable      True to enable, false to disable the listener.<br>
     *                       If there are already registered profiles / see tmlCoreWrapper_Register_Profile(),<br>
     *                       they will be registered for the listener.<br>
     *                       In case of a configured port of 0 /see tmlCoreWrapper_Set_ListenerPort() a random free port will be detected<br>
     *                       and it's identification may be requested with tmlCoreWrapper_Get_ListenerPort().
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_LISTENER_ADDRESS_BINDING unable to bind listener address / maybe address already in use or insufficient permission<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_UNICODE if an error happened in the unicode string conversion.<br>
     *          TML_ERR_LISTENER_ALREADY_STARTED if the instance is already started.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Enable_Listener(bool bEnable);


    /**
     * @brief   Returns the listener enable status.
     *
     * @param   bEnable    Reference to the return value. True if the listener is enabled, false if the listener is disabled.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Is_Listener_Enabled(bool* bEnable);


    /**
     * @brief   Set the window size for the following outgoing data.
     *
     * We recommend not to use this call on customer side.<br>
     * By default the window size is 32768 byte.
     *
     * @param   iWindowSize  The window size.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_WindowSize(int iWindowSize);


    /**
     * @brief   Get the window size for the following outgoing data.
     *
     * By default the window size is 32768 byte.
     *
     * @param   iWindowSize  Reference to the return value.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_WindowSize(TML_INT32* iWindowSize);


    /**
     * @brief   Set the debug logging value.
     *
     * @param   iLogValue    The debug logging value.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_Logging_Value(int iLogValue);


    /**
     * @brief   Returns the debug logging value.
     *
     * @param   iLogValue    Reference to the debug logging value.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Get_Logging_Value(TML_INT32* iLogValue);


    /**
     * @brief    Send a synchron Message.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     * @param   iTimeout    Timeout for the command execution (in ms).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_CHANNEL_NOT_INITIALIZED if the channel initialization failled.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC timeout during wait for end of async cmd execution.<br>
     *          TML_ERR_TIMEOUT in case of a timeout.
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SendSyncMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile, const char* sHost, const char* sPort, unsigned int iTimeout);


    /**
     * @brief    Send an asynchron Message.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     * @param   iTimeout    Timeout for the command execution (in ms).
     * @param   iMode       The command mode (TMLCOM_MODE_ASYNC or TMLCOM_MODE_EVT).
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
    int tmlCoreWrapper_SendAsyncMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile, const char* sHost, const char* sPort, unsigned int iTimeout, int iMode);


    /**
     * @brief    Send an asynchronous progress reply.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the TML_CORE_HANDLE to send the reply to.
     * @param   progress    The progress value (in percent).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<br>
     *          TML_ERR_INVALID_PROGRESS if 0 > progress > 100.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SendAsyncProgressReply(TML_COMMAND_HANDLE tmlhandle,  TML_INT32 progress);


    /**
     * @brief    Send an asynchronous status reply.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the TML_CORE_HANDLE to send the reply to.
     * @param   iType       The command reply type. For the reply type description see the "TML Specification".<br>
     *                      Possible Values:<br>
     *                      TMLCOM_RPY_WARNING<br>
     *                      TMLCOM_RPY_ERROR<br>
     *                      TMLCOM_RPY_INFORMATION
     * @param   sStatus     Reference to the reply content.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED the reply type is not allowed for this operation.
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SendAsyncStatusReply(TML_COMMAND_HANDLE tmlhandle, int iType, char* sStatus);


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
    int tmlCoreWrapper_EventSubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort);


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
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_EventGetSubscribedMessageDestinations(const char* profile, SIDEX_VARIANT* subscriptions);


    /**
     * @brief    Unsubscribe a destination for event messages.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if a destination with the the requested parameter was not found.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_EventUnsubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort);


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
    int tmlCoreWrapper_EventUnsubscribeAllMessageDestinations(const char* profile);


    /**
     * @brief    Register a callback method that will be called in case of problem during event message send actions.
     *
     * The callback will be called in the manner:<br>
     * void pCBFunc (char* sProfile, char* sHost, char* sPort, TML_COMMAND_ID_TYPE iCMD, TML_INT32 iErrorCode, TML_POINTER pCBData)<br>
     * See TML_ON_EVT_ERROR_CB_FUNC()
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
    int tmlCoreWrapper_EventRegisterCallbackOnError(const char* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Register a callback method that will be called in case of event message queue overflow.
     *
     * If the event message processing is slower than the time of incoming event message requests,<br>
     * they will be queued internal. If the number of queue elements is more than a maximum, the oldest queue<br>
     * entry will be removed. In that case a registered callback will be called.
     *
     * The callback will be called in the manner:<br>
     * void pCBFunc (TML_CTSTR sProfile, TML_INT32 iCMD, TML_POINTER pCBData)<br>
     * See TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC()
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the tml_Evt_Set_MaxConnectionFailCount method.<br>
     *                     In case of NULL the callback method  has no parameter.
     * @param   iUnicode   unicode identifier
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_Set_OnQueueOverflow(const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Register a callback method for the case of non subscribed destinations for event messages.
     *
     * This will register a callback method that will be called in case of an event message to a profile without a subscribed destination.<br>
     * See tmlCoreWrapper_EventSendMessage()<br>
     * The callback returns the result of the destination registration and will be called in the manner:<br>
     * TML_INT32 pCBFunc (TML_CTSTR* sProfile, TML_POINTER pCBData)<br>
     * See TML_ON_POPULATE_CB_FUNC()
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
    int tmlCoreWrapper_EventRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Send an event message.
     *
     * This will send the TML_COMMAND_HANDLE to all registered destinations for the requested profile.<br>
     * See  tmlCoreWrapper_EventSubscribeMessageDestination().
     * If there is no registered destination, a callback method, if registered, will be called to perform the destination registration.<br>
     * See  tmlCoreWrapper_EventRegisterPopulateCallback().
     * If this succeeds the TML_COMMAND_HANDLE will be send to all registered destinations afterwards.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data to send.<br>
     *                      An intenal copy will be made, so it can be freed afterwards.
     * @param   profile     URI of the profile for the requested operation.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INITIALIZATION in case of a common initialization problem.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED if the profile is not supported by the server.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registered destination has missing parameter.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_EventSendMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile);


    /**
     * @brief    Send an event subscription request.
     *
     * This request will be send by a server to auto subscribe event commands.

     * @param   profile     URI of the profile for the requested operation.
     * @param   sSourceHost Reference to the host name for the subscribe request.
     * @param   sSourcePort Reference to the host port name for the subscribe request.
     * @param   sDestHost   Reference to the host name to send the request to.
     * @param   sDestPort   Reference of the Port to send the request to.
     * @param   iTimeout    Timeout for the command execution (in ms).
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
    int tmlCoreWrapper_EventSendSubscriptionRequest(const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, unsigned int iTimeout);


    /**
     * @brief    Register a callback method for the case of a peer subscription / unsubscription request for event messages.
     *
     * This will register a callback method that will be called in case of a peer subscription request for event messages.<br>
     * See tmlCoreWrapper_EventSendSubscriptionRequest() / tmlCoreWrapper_EventSendUnsubscriptionRequest().<br>
     * The callback will be called in the manner:<br>
     * TML_BOOL pCBFunc (TML_BOOL bSubscribe, TML_CTSTR* sHost, TML_CTSTR* sPort, TML_POINTER pCBData)<br>
     * See TML_ON_PEER_REGISTRATION_CB_FUNC()
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
    int tmlCoreWrapper_EventRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Send an event unsubscription request.
     *
     * This request will be send by a server to auto unsubscribe event commands.

     * @param   profile     URI of the profile for the requested operation.
     * @param   sSourceHost Reference to the host name for the unsubscribe request.
     * @param   sSourcePort Reference to the host port name for the unsubscribe request.
     * @param   sDestHost   Reference to the host name to send the request to.
     * @param   sDestPort   Reference of the Port to send the request to.
     * @param   iTimeout    Timeout for the command execution (in ms).
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
    int tmlCoreWrapper_EventSendUnsubscriptionRequest(const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, unsigned int iTimeout);


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
    int tmlCoreWrapper_LoadBalancedSubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort);


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
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_LoadBalancedGetSubscribedMessageDestinations(const char* profile, SIDEX_VARIANT* subscriptions);


    /**
     * @brief    Unsubscribe a destination for load balanced messages.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if a destination with the the requested parameter was not found.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_LoadBalancedUnsubscribeMessageDestination(const char* profile, const char* sHost, const char* sPort);


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
    int tmlCoreWrapper_LoadBalancedUnsubscribeAllMessageDestinations(const char* profile);


    /**
     * @brief    Register a callback method for the case of non subscribed destinations for load balanced messages.
     *
     * This will register a callback method that will be called in case of an event message to a profile without a subscribed destination.<br>
     * See tmlCoreWrapper_LoadBalancedSendSyncMessage() of tmlCoreWrapper_LoadBalancedAsendSyncMessage<br>
     * The callback returns the result of the destination registration and will be called in the manner:<br>
     * TML_INT32 pCBFunc (TML_CTSTR*  sProfile, TML_POINTER pCBData)<br>
     * See TML_ON_POPULATE_CB_FUNC()
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
    int tmlCoreWrapper_LoadBalancedRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Register a callback method for the BUSY STATUS requests.
     *
     * This will register a callback method that will be called in case of a load balanced message to get the BUSY STATUS information of the subscribed destination.<br>
     * See tml_Bal_Send_SyncMessage() or see tml_Bal_Send_AsyncMessage().<br>
     * The callback returns the result the BUSY STATE coded in a TML_COMMAND_HANDLE. It will be called in the manner:<br>
     * TML_INT32 pCBFunc (TML_CAMMAND_HANDLE cmdHandle, TML_POINTER pCBData)<br>
     * See TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC()
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
    int tmlCoreWrapper_LoadBalancedRegisterBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method for the load balance calculation.
     *
     * This will register a callback method that will be called on every load balance message send.<br>
     * See tml_Bal_Send_SyncMessage() or see tml_Bal_Send_AsyncMessage().<br>
     * The callback calculates the next message destination index and will be called in the manner:<br>
     * TML_INT32 pCBFunc (TML_INT32 iNumberOfListener, TML_COMMAND_HANDLE* listenerBusyStateArray, TML_POINTER pCBData, TML_INT32* iNextListenerIndex)<br>
     * See TML_ON_BAL_CALCULATION_REQ_CB_FUNC()
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
    int tmlCoreWrapper_LoadBalancedRegisterCalculator(const char* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Send a load balanced message.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   profile     URI of the profile for the requested operation.
     * @param   iTimeout    Timeout for the command execution (in ms).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.<br>
     *          TML_ERR_CHANNEL_NOT_INITIALIZED if the channel initialization failled.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registered destination has missing parameter<br>
     *          TML_ERR_TIMEOUT in case of a timeout.
     *          TML_ERR_LOAD_BALANCE_CALCULATION in case of an load balance calculation error.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_LoadBalancedSendSyncMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout);


    /**
     * @brief    Send a load balanced async message.
     *
     * API call will return immediately. The communication will be done in background.<br>
     * The communication result will be returned to the callback method pCBFunc.<br>
     * Allocated object(s) have to be freed in that callback method.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE containing the data to send.
     * @param   profile     URI of the profile for the requested operation.
     * @param   iTimeout    Timeout for the command execution (in ms).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INITIALIZATION in case of a common initialization problem.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the connection initialization failed.<br>
     *          TML_ERR_SENDER_PROFILE_REGISTRATION if the connection can't register the profile.<br>
     *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED if the profile is not supported by the server.<br>
     *          TML_ERR_SENDER_INVALID_PARAMS if the host and/or port to connect is invalid.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registered destination has missing parameter<br>
     *          TML_ERR_LOAD_BALANCE_CALCULATION in case of an load balance calculation error.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_LoadBalancedSendAsyncMessage(TML_COMMAND_HANDLE tmlhandle, const char* profile, unsigned int iTimeout);


    /**
     * @brief    Send a load balanced subscription request.
     *
     * This request will be send by a server to auto subscribe load balanced commands.

     * @param   profile     URI of the profile for the requested operation.
     * @param   sSourceHost Reference to the host name for the unsubscribe request.
     * @param   sSourcePort Reference to the host port name for the unsubscribe request.
     * @param   sDestHost   Reference to the host name to send the request to.
     * @param   sDestPort   Reference of the Port to send the request to.
     * @param   iTimeout    Timeout for the command execution (in ms).
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
    int tmlCoreWrapper_LoadBalancedSendSubscriptionRequest(const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, unsigned int iTimeout);


    /**
     * @brief    Register a callback method for the case of a peer subscription / unsubscription request for load balanced messages.
     *
     * This will register a callback method that will be called in case of a peer subscription request for load balanced messages.<br>
     * See tmlCoreWrapper_LoadBalancedSendSubscriptionRequest() / tmlCoreWrapper_LoadBalancedSendUnsubscriptionRequest().<br>
     * The callback will be called in the manner:<br>
     * TML_BOOL pCBFunc (TML_BOOL bSubscribe, TML_CTSTR* sHost, TML_CTSTR* sPort, TML_POINTER pCBData)<br>
     * See TML_ON_PEER_REGISTRATION_CB_FUNC()
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
    int tmlCoreWrapper_LoadBalancedRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Send a load balanced unsubscription request.
     *
     * This request will be send by a server to auto unsubscribe load balanced commands.

     * @param   profile     URI of the profile for the requested operation.
     * @param   sSourceHost Reference to the host name for the unsubscribe request.
     * @param   sSourcePort Reference to the host port name for the unsubscribe request.
     * @param   sDestHost   Reference to the host name to send the request to.
     * @param   sDestPort   Reference of the Port to send the request to.
     * @param   iTimeout    Timeout for the command execution (in ms).
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
    int tmlCoreWrapper_LoadBalancedSendUnsubscriptionRequest(const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, unsigned int iTimeout);


    /**
     * @brief    Set the maximum load balanced connection fail count value.
     *
     * On every fail of a connection of a subscribed message destination an internal counter will be incremented.<br>
     * If this counter is equals or more than the requested value, the destination will be unsubscribed.<br>
     * In case of a successful connection / message transfer the internal counter will be reset.
     * 
     * @param   iCount      The number of fail connections / if 0 the destination will not be unsubscribed anytime.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_LoadBalancedSetMaxConnectionFailCount(unsigned int iCount);


    /**
     * @brief    Get the maximum load balanced connection fail count value.
     *
     * On every fail of a connection of a subscribed message destination an internal counter will be incremented.<br>
     * If this counter is equals or more than the requested value, the destination will be unsubscribed.<br>
     * In case of a successful connection / message transfer the internal counter will be reset.
     * 
     * @param   iCount      Reference to the number of fail connections / if 0 the destination will not be unsubscribed anytime.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_LoadBalancedGetMaxConnectionFailCount(TML_UINT32* iCount);


    /**
     * @brief    Set the maximum event connection fail count value.
     *
     * On every fail of a connection of a subscribed message destination an internal counter will be incremented.<br>
     * If this counter is equals or more than the requested value, the destination will be unsubscribed.<br>
     * In case of a successful connection / message transfer the internal counter will be reset.
     * 
     * @param   iCount      The number of fail connections / if 0 the destination will not be unsubscribed anytime.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_EventSetMaxConnectionFailCount(unsigned int iCount);


    /**
     * @brief    Get the maximum event connection fail count value.
     *
     * On every fail of a connection of a subscribed message destination an internal counter will be incremented.<br>
     * If this counter is equals or more than the requested value, the destination will be unsubscribed.<br>
     * In case of a successful connection / message transfer the internal counter will be reset.
     * 
     * @param   iCount      Reference to the number of fail connections / if 0 the destination will not be unsubscribed anytime.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_EventGetMaxConnectionFailCount(TML_UINT32* iCount);


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
    int tmlCoreWrapper_EventSetMaxQueuedEventMessages(unsigned int iMaximum);


    /**
     * @brief    Get the maximum value of queued event messages
     *
     * If the event message processing is slower than the time of incoming event message requests,
     * they will be queued internal. If the number of queue elements is more than a maximum, the oldest queue
     * entry will be removed. This API call changes this maximum value.
     * 
     * @param   iMaximum      Reference to the maximum value of queued event messages
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_EventGetMaxQueuedEventMessages(unsigned int* iMaximum);


    /**
     * @brief    write the debug log to the stdout
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     */
    void log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    write the debug log to the stdout
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     */
    void log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const wchar_t* sLog);


    /**
     * @brief    write the debug log to the stdout
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     */
    void log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char16_t* sLog);


    /**
     * @brief    Open a receiver stream.
     *
     * @param   iID         Clear stream identifiation / returned by tml_SndStream_Open().
     * @param   sPort       The destination Port.
     * @param   sHost       The destination host name / IP number.
     * @param   profile     URI of the destination profile for the requested operation.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_ALREADY_IN_USE The stream is already in use - can't be opened for a second time.<br>
     *          TML_ERR_STREAM_OPEN_ADDRESS_MISMATCH Retain open problem / profile & host & port don't match with the iID.<br>
     *          TML_ERR_STREAM_INVALID_ADDRESS if one of the destination parameter is invalid.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_RecStream_Open(TML_STREAM_ID_TYPE iID, const char* sPort, const char* sHost, const char* profile);


    /**
     * @brief    Close a receiver stream.
     *
     * @param   iID         Clear stream identifiation.
     * @param   bRetainOpen Flag. If TML_TRUE the  receiver stream will be retained open / the close is only logically(Stream still stand open). A recStream_Open is allowed afterwards.
     *
     * @returns TML_SUCCESS in case of success<br>.
     *          TML_ERR_INFORMATION_UNDEFINED in case of an invalid iID / maybe of a missing tml_RecStream_Open().
     *
     */
    int tmlCoreWrapper_RecStream_Close(TML_STREAM_ID_TYPE iID, TML_BOOL bRetainOpen);


    /**
     * @brief    Get the size of a receiver stream.
     *
     * @param   iID         Clear stream identifiation.
     * @param   rsize       Reference to the sstream size.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_SIZE_NOT_AVAILABLE if the callback returns a value of -1.     
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_RecStream_GetSize(TML_STREAM_ID_TYPE iID, TML_INT64* rsize);


    /**
     * @brief    Perform a seek operation of a receiver stream.
     *
     * @param   iID     Clear stream identifiation.
     * @param   seekPos The position for the requested operation.
     * @param   origin  The origin for the requested operation.<br>
     *                  soFromBeginning: Offest relative to the beginning of the resource (Must be more than 0).
     *                  soFromCurrent: Offest relative to the current position of the resource.
     *                  soFromEnd: Offest relative to the end of the resource (Must be less than 0).
     *
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_SEEK_NOT_OPERABLE if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_RecStream_Seek(TML_STREAM_ID_TYPE iID, TML_INT64 seekPos, tmlSeekOrigin origin);


    /**
     * @brief    Get the position of a receiver stream.
     *
     * @param   iID       Clear stream identifiation.
     * @param   rposition Reference to the position.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_POSITION_NOT_AVAILABLE if the callback returns a value of -1.     
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_RecStream_GetPosition(TML_STREAM_ID_TYPE iID, TML_INT64* rposition);


    /**
     * @brief    Write data to a receiver stream.
     *
     * @param   iID         Clear stream identifiation.
     * @param   buffer      Reference to buffer for the write operation.
     * @param   count       Number of bytes to write.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.<br>
     *          TML_ERR_STREAM_WRITE_FAILED if the callback returns an error.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int tmlCoreWrapper_RecStream_Write(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count);


    /**
     * @brief    Read data of a receiver stream.
     *
     * @param   iID         Clear stream identifiation.
     * @param   buffer      Reference to the buffer to write the result into.
     * @param   count       Size of the buffer / number of bytes read.
     * @param   bytesRead   Reference to the number of bytes read.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_RecStream_Read(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count, TML_INT32* bytesRead);


    /**
     * @brief    Read data of a receiver stream.
     *
     * If the end of file is reached before the count of bytes were read, an error will be returned.
     *
     * @param   iID         Clear stream identifiation.
     * @param   buffer      Reference to the buffer to write the result into.
     * @param   count       Size of the buffer / number of bytes read.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns an error.<br>
     *          TML_ERR_STREAM_READBUFFER_EOF End of file is reached before the count of bytes were read.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_RecStream_ReadBuffer(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count);


    /**
     * @brief    Initiates a download of data of a receiver stream.
     *
     * The download is asynchronous. It's result will be returned to the pCBFuncRet function.
     *
     * @param   iID               Clear stream identifiation.
     * @param   buffersize        buffersize for the download.
     * @param   pCBFuncDld        Reference to the callback method that handles the download.
     * @param   pCBDataDld        Reference to parameter of the callback method that handles the download.<br>
     * @param   pCBFuncDldFinish  Reference to the callback method that will be called at the end of the download.<br>
     * @param   pCBDataDldFinish  Reference to parameter of the callback that will be called at the end of the download.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_DOWNLOAD_FAILED if the download failed.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_RecStream_DownloadData(TML_STREAM_ID_TYPE iID, TML_UINT32 buffersize, TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC pCBFuncDld, TML_POINTER pCBDataDld, TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncDldFinish, TML_POINTER pCBDataDldFinish);


    /**
     * @brief    Open a sender stream.
     *
     * @param   iID         reference to a clear identifiation.
     * @param   sPort       The destination Port.
     * @param   sHost       The destination host name / IP number.
     * @param   profile     URI of the destination profile for the requested operation.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_ADDRESS if one of the destination parameter is invalid.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Open(TML_STREAM_ID_TYPE* iID, const char* sPort, const char* sHost, const char* profile);


    /**
     * @brief    Close a sender stream.
     *
     * @param   iID        Clear stream identifiation.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Close(TML_STREAM_ID_TYPE iID);


    /**
     * @brief    Register a callback method to get the sender stream size.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Register_GetSize (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_SIZE_CB_FUNC, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to perform stream seek operations.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Register_Seek (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to get the stream position.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Register_GetPosition (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to read data of the sender stream.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Register_Read (TML_STREAM_ID_TYPE iID,TML_ON_SND_STRM_READ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to write data to a sender stream.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Register_Write (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method that will be called in case of closing the sender stream.
     *
     * The callback will be called in the manner:<br>
     *  void pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER pCBData)<br>
     *  -Parameter iID        Clear stream identifiation.<br>
     *  -Parameter pCBData    Reference to parameter of the callback method.
     *
     * @param   iID        Clear stream identifiation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle of sHandle.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Register_Close (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method that will be called on communication fault during a request.
     *
     * The callback will be called in the manner:<br>
     *  void pCBFunc (TML_STREAM_ID_TYPE iID, TML_INT32 iError, TML_POINTER pCBData)<br>
     *  -Parameter iID        Clear stream identifiation.<br>
     *  -Parameter iError     The detailed error code of the communication fault.<br>
     *  -Parameter pCBData    Reference to parameter of the callback method.
     *
     * @param   iID        Clear stream identifiation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle of sHandle.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int tmlCoreWrapper_SndStream_Register_OnError (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Set the flag to identifier a shutdown 
     *
     * @param   bShutdown  True, if shutdown is pending.
     */
    void tmlCoreWrapper_SetShutdown (bool bShutdown);


    /**
     * @brief    Ask if core access is possible.
     *
     * @returns TML_SUCCESS if core access is possible, otherwise TML_ERR_SHUTDOWN.
     */
    int tmlCoreWrapper_IsAccessible ();
  
    
    /**
     * @brief    returns the index of the contiguous log file
     */
    int getLogFileIndex();
};
#endif  // TMLCOREWRAPPER_H
