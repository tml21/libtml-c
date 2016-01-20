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
 
#ifndef TMLCORESENDER_H
#define TMLCORESENDER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vortex.h>
#include "tmlCoreDefines.h"
#include "tmlCCallback.h"
#include "tmlLogHandler.h"
#include "tmlEventHandler.h"
#include "tmlMultiAsyncMsg.h"

#ifdef LINUX
    #define FUNC_STDCALL void*
#else // LINUX
  //The Windows definitions:
  #define FUNC_STDCALL unsigned __stdcall
#endif // LINUX

#ifdef LINUX
axl_bool  intern_thread_create (TMLThreadDef* threadInfo, VortexThreadFunc func, axlPointer user_data);
#else // LINUX
axl_bool  intern_thread_create (TMLThreadDef* threadInfo, FUNC_STDCALL func(void*), axlPointer user_data);
#endif // LINUX


////////////////////////////////////////////////////////////////////////////
// internal thread destruction method
axl_bool  intern_thread_destroy (TMLThreadDef* threadInfo);

//////////////////////////////////////////////////////////////////////////////
// C - declarations:
extern "C" {

int StreamContentHandling(SIDEX_HANDLE sHandle, 
                                          TML_INT32         iPayloadFrameSize, 
                                          char*             cPayload,
                                          TML_INT64         totalFrameSize,
                                          VortexFrame*      pFrame,
                                          TML_INT64         cmd,
                                          tmlLogHandler*    m_log,
                                          TML_INT32         iEmptyOnError);

/**
 * @brief  Timer thread to handle timeouts
 *
 * @param   pParam       Reference to TimerThreadData structure.
 */
FUNC_STDCALL TimerThread( void* pParam);

/**
 * @brief    Start the timer thread.
 *
 * @param   timerThreadData              Reference to TimerThreadData structure that contains parameter e.g. timer synchronisation events<br>
 *                                       It will be filled inside the method.
 * @param   eventHandler                 Reference to tmlEventHandler.
 * @param   hSynchronisationHandleArray  Array of timer synchonisation handles.
 * @param   dwTimeout                    The timeout value (in ms)
 * @param   pLogHandler                  Reference to tmlLogHandler for logging 
 * @param   threadInfo                   Reference to TMLThreadDef 
 *
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED in case of a problem.
 */
int StartTimerThread(TimerThreadData* timerThreadData, tmlEventHandler* eventHandler, const char** hSynchronisationHandleArray, DWORD dwTimeout, tmlLogHandler* pLogHandler, TMLThreadDef* threadInfo);


/**
 * @brief    Stop the timer thread.
 *
 * @param   hStopTimer       Handle that represents timer stop synchronisation event
 * @param   pThreadData      Reference to AsyncHandlingThreadData structure.
 * @param   threadInfo       Reference to TMLThreadDef 
 */
void StopTimerThread(const char* hStopTimer, AsyncHandlingThreadData* pThreadData, TMLThreadDef* threadInfo);


/**
 * @brief    Restart the timer thread / the timeout.
 *
 * @param   timerThreadData  Reference to TimerThreadData structure that contains parameter e.g. timer synchronisation events
 * @param   eventHandler     Reference to tmlEventHandler.
 * @param   hContinue        Handle to set the restart event.
 * @param   dwTimeout        The timeout value (in ms)
 * @param   pLogHandler      Reference to tmlLogHandler for logging 
 */
void RestartTimerThread(TimerThreadData* timerThreadData, tmlEventHandler* eventHandler, const char* hContinue, DWORD dwTimeout, tmlLogHandler* pLogHandler);


/**
 * @brief    Start the AsyncHandlingThread.
 *
 * @param   threadData         Reference to AsyncHandlingData structure that contains parameter e.g. timer synchronisation events
 * @param   eventHandler       Reference to tmlEventHandler.
 * @param   handlingEventArray Array of Handles for thread synchronisation
 * @param   payload            Reference to AsyncHandlingThreadData structure that contains parameter to handle events from the<br> 
 *                             "senderFrameReceivedCallback" ot "timerTread" e.g. synchronisation handles 
 *
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED in case of a problem.
 */
int StartAsyncHandlingThread(AsyncHandlingData* threadData, tmlEventHandler* eventHandler, const char** handlingEventArray, AsyncHandlingThreadData* payload);


/**
 * @brief    Method checks for and handles asynchronous progress and message replies
 *
 * This method will be called by AsyncHandlingThreadMethod().
 *
 * @param   tmlhandle             TML_COMMAND_HANDLE containing the data of the frame
 * @param   pLogHandler           Reference to tmlLogHandler for logging 
 * @param   pMutexCriticalSection Reference to mutex to handle critical sections
 */
bool HandleAsyncReply(TML_COMMAND_HANDLE tmlhandle, tmlLogHandler* pLogHandler, VortexMutex* pMutexCriticalSection);


/**
 * @brief    Method checks for and handles asynchronous steam replies
 *
 * This method will be called by AsyncHandlingThreadMethod().
 *
 * @param   msg             Reference to tmlMultiAsyncMsg class object
 * @param   iSize           Reference to size of the queue
 * @param   iEmptyOnError   Flag if != TML_SUCCESS do only free allocated memory
 */
TML_INT32 HandleAsyncReplyFromStream(tmlMultiAsyncMsg* msg, TML_INT32*  iSize, TML_INT32 iEmptyOnError);


/**
 * @brief  callback in a received frame (reply)
 *
 * @param   channel    Reference to VortexChannel of the received frame source
 * @param   connection Reference to VortexConnection of the received frame source
 * @param   frame      Reference to received VortexFrame
 * @param   user_data  Reference to user data registered on Vortex API vortex_channel_set_received_handler()
 */
void senderFrameReceivedCallback (VortexChannel* channel,
                           VortexConnection* connection,
                           VortexFrame* frame,
                           axlPointer user_data);


/**
 * @brief  callback in a received frame (reply) to consume latecomer after timeout.
 *
 * @param   channel    Reference to VortexChannel of the received frame source
 * @param   connection Reference to VortexConnection of the received frame source
 * @param   frame      Reference to received VortexFrame
 * @param   user_data  Reference to user data registered on Vortex API vortex_channel_set_received_handler()
 */
void senderFrameReceivedCallbackConsumeLatecomer (VortexChannel* channel,
                           VortexConnection* connection,
                           VortexFrame* frame,
                           axlPointer user_data);


/**
 * @brief  Async sender thread method
 *
 * @param   pParam      Reference to AsyncHandlingThreadData structure that contains parameter to handle events from the<br> 
 *                      "senderFrameReceivedCallback" ot "timerTread" e.g. synchronisation handles 
 */
void AsyncHandlingThreadMethod (axlPointer pParam);


}// extern "C"


// Prototying:
class tmlConnectionObj;

class TMLCoreSender
{
    /**
     * @brief    Definition for synchronisation event
     */
    #define SYNCHRONISATION_EVENT_HANDLE "synchronisationEventHandle"

    /**
     * @brief    Definition for close synchronisation event
     */
    #define CLOSE_SYNCHRONISATION_EVENT_HANDLE "closesynchronisationEventHandle"

    /**
     * @brief    Definition for timeout synchronisation event
     */
    #define TIMEOUT_EVENT_HANDLE "timeoutEventHandle"

    /**
     * @brief    Definition for stop timer synchronisation event
     */
    #define STOP_TIMER_EVENT_HANDLE "stopTimerEventHandle"

    /**
     * @brief    Definition for continue timer synchronisation event
     */
    #define CONTINUE_TIMER_EVENT_HANDLE "continueTimerEventHandle"

    /**
     * @brief    Definition for async synchronisation event
     */
    #define ASYNC_SYNCHRONISATION_EVENT_HANDLE "asyncSynchronisationEventHandle"

    /**
     * @brief    Definition for async stream event
     */
    #define ASYNC_STREAM_EVENT_HANDLE "asyncStreamEventHandle"

    /**
     * @brief    Definition for terminate synchronisation event
     */
    #define TERMINATE_EVENT_HANDLE "terminateEventHandle"

    /**
     * @brief    Definition for continue synchronisation event
     */
    #define CONTINUE_EVENT_HANDLE "continueEventHandle"

    /**
     * @brief    Definition for async list proceeded synchronisation event
     */
    #define SENDER_ASYNC_LIST_PROCEEDED_EVENT_HANDLE "senderAsyncListProceededHandle"

private:
    // Members:

    /**
     * @brief    Thread inforamtion structure 
     */
    //TMLThreadDef m_threadInfo;

    /**
     * The error code that will be set during construction / initSender()
     */
    int                  m_iErrCode;

    /**
     * @brief   Log- File index for explicit logs with closing the file
     */
    int                  m_iLogFileIndex;

    /**
     * @brief    Flag signals that initialization is valid
     */
    bool                 m_bValidInitialized;


    /**
     * @brief    The handler to implement event handling
     */
    tmlEventHandler*     m_eventHandler;


    /**
     * @brief    Flag signals that an async cmd is pending
     */
    bool                 m_bAnyAsyncCmdPending;

    /**
     * @brief    Reference to the data structure of the first async cmd list entry
     */
    AsyncCMDListElement* m_firstAsyncCmdListElement;

    /**
     * @brief    Reference to the data structure of the last async cmd list entry
     */
    AsyncCMDListElement* m_lastAsyncCmdListElement;

    /**
     * @brief    Handles to signal async cmd list is processed / empty
     */
    const char*  m_handleSenderAsyncListProcessed;

    /**
     * @brief    A mutex to protect critial section referring async cmd list
     */
    VortexMutex  m_mutexAsyncCmdList;

    /**
     * @brief    Synchronisation mutex to be sure that the thread is closed
     */
    VortexMutex  m_mutexTimerThreadSync;

    /**
     * @brief    Synchronisation mutex to be sure that the thread is closed
     */
    VortexMutex  m_mutexAsyncThreadSync;

    /**
     * @brief  Datastructure contains the parameter for the tmlCoreSender's AsyncHandlingThreadMethod
     */
    AsyncHandlingThreadData m_AsyncHandlingThreadData;
    //int                m_iPayloadFrameSize;                 // actual received frame size
    //char*              m_cPayload;                          // actual received frame payload
    tmlMultiAsyncMsg*    m_multiAsyncMsg;

    /**
     * @brief  Datastructure contains the parameter for the tmlCoreSender's AsyncHandlingThreadMethod
     */
    AsyncHandlingData m_AsyncHandlingData;

    /**
     * @brief    Array of Handles for thread synchronisation
     */
    const char** m_asyncHandlingEventArray;

    /**
     * @brief    Array of Handles for thread / callback synchronisation
     */
    const char** m_senderSyncEventArray;

    /**
     * @brief    data stucture to the TimerThread
     */
    TimerThreadData m_timerThreadData;
 
    /**
     * @brief    parameter structure for the case of a connection close
     */
    VORTEXSenderConnectionCloseCallbackData* m_closeCallbackData;

    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;

    /**
     * @brief    Vortex execution context
     */
    VortexCtx*        m_ctx;

    /**
     * @brief    A mutex to protect critial sections
     */
    VortexMutex       m_mutexCriticalSection;

    /**
     * @brief    Reference to the class callback handling method.
     */
    TCallback<TMLCoreSender> m_internalFrameReceiveHandlerMethod;

    /**
     * @brief    Reference to the class callback handling method.
     */
    TCallback<TMLCoreSender> m_internalAsyncCmdCallbackHandlerMethod;
    

    // Methods:

    /**
     * @brief    Restart the AyncHandlingThread.
     *
     * @param   threadData         Reference to AsyncHandlingData structure that contains parameter e.g. timer synchronisation events
     * @param   handlingEventArray reference to synchronisation handle array.
     * @param   payload            Reference to AsyncHandlingThreadData structure that contains parameter to handle events from the<br> 
     *                             "senderFrameReceivedCallback" ot "timerTread" e.g. synchronisation handles 
     * @param   pLogHandler        Reference to a log handler.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the HANDLE is invalid.
     *
     * @see tmlErrors.h
     */
    int RestartAsyncHandlingThread(AsyncHandlingData* threadData, const char*** handlingEventArray, AsyncHandlingThreadData* payload, tmlLogHandler* pLogHandler);


    /**
     * @brief   Destroy the HANDLE to be able to signal the end of a frame receive operation via setEvent or connection close etc.
     *
     * @param   handle reference to the handle.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the HANDLE is invalid.
     *
     * @see tmlErrors.h
     */
    int DestroySynchronisationHandle(const char** handle);

    /**
     * @brief    The internal class frame receive callback handling method.
     *
     * @param   param   Reference to a VORTEXSenderFrameReceivedCallbackData structure.
     *
     * @returns allways true.
     */
    bool InternalFrameReceiveHandlerMethod(void *param);

    /**
     * @brief    The internal class callback method to handle pending async cmds
     *
     * @param   param   Reference to parameter structure (tbd)
     *
     * @returns allways true.
     */
    bool InternalAsyncCmdCallbackHandlerMethod(void *param);


    /**
     * @brief    Makes a copy of the tmlhandle's SIDEX_HANDLE to have it for queuing
     *
     * @param   tmlhandle   Reference TML_COMMAND_HANDLE with the SIDEX_HANDLE to copy.
     * @param   retHandle   Reference the new SIDEX_HANDLE.
     *
     * @returns TML_SUCCESS in case of success.
     */
    int getSidexForStream(TML_COMMAND_HANDLE tmlhandle, SIDEX_HANDLE* retHandle);


    /**
     * @brief    Write the stream content into a queue to fetch it later in the thread
     *
     * @param   callbackData      Reference to a VORTEXSenderFrameReceivedCallbackData structure.
     * @param   cmd               The parsed stream command.
     *
     * @returns TML_SUCCESS in case of success
     */
    int StreamContentIntoQueue(VORTEXSenderFrameReceivedCallbackData* callbackData, TML_INT64 cmd);


    /**
     * @brief    Write the payload content into a queue to fetch it later in the thread
     *
     * @param   callbackData      Reference to a VORTEXSenderFrameReceivedCallbackData structure.
     * @param   sHandle           Reference to sidex handle.
     * @param   iState            Reference to state that will be set internal.
     * @param   iPayloadFrameSize Size of the payload
     * @param   cPayload          Reference to the payload
     *
     * @returns TML_SUCCESS in case of success
     */
    int PayloadContentIntoQueue(VORTEXSenderFrameReceivedCallbackData* callbackData, SIDEX_HANDLE sHandle, TML_INT64* iState, TML_INT32 iPayloadFrameSize, char* cPayload);


    /**
     * @brief  Write the total waw payloadSize into the command handle
     *
     * @param   callbackData   Reference to a VORTEXSenderFrameReceivedCallbackData structure.
     */
    void totalRawPayloadSizeToHandle(VORTEXSenderFrameReceivedCallbackData* callbackData);


    /**
     * @brief    Allocate event handles for process synchronisation.
     *
     * @param   senderSyncEventArray reference to event handle array.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the HANDLE  creation failed.
     */
    int allocSyncEventArray(const char*** senderSyncEventArray);


    /**
     * @brief    Allocate event handles for process synchronisation.
     *
     * @param   asyncThreadEventArray reference to event handle array.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the HANDLE  creation failed.
     */
    int allocAsyncThreadEventArray(const char*** asyncThreadEventArray);


    /**
     * @brief    Reset event handles for process synchronisation.
     *
     * @param   senderSyncEventArray reference to event handle array.
     * @param   iLength              Length of the array.
     * @param   bInitializeState     Flag, the sender initialize state.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the sender is not initialized.
     */
     int resetSyncEventArray(const char*** senderSyncEventArray, int iLength, bool bInitializeState);

    /**
     * @brief    Free event handles for process synchronisation.
     *
     * @param   senderSyncEventArray reference to event handle array.
     * @param   iLength              Length of the array.
     */
    void freeSyncEventArray(const char*** senderSyncEventArray, int iLength);


    /**
     * @brief    The initialization called by the constructor
     *
     * @param   iLogValue    The debug logging value.
     *
     */
    void initSender(int iLogValue);

    
    /**
     * @brief Set the state of async cmd processing.
     *
     * @param   bSet The state of an active async cmd processing.
     */
    void SetAsyncCmdProcessing(bool bSet);


    /**
     * @brief    Add an async command element to the link list.
     *
     * @param   tmlhandle                   An instance of TML_COMMAND_HANDLE that holds the command.
     * @param   iTimeout                    Timeout for the command operation in milliseconds.
     * @param   firstConnectionListElement  Reference to the first async command list element.
     * @param   LastConnectionListElement   Reference to the last async command list element.
     */
    void AddAsyncCmdElementIntoQueue(TML_COMMAND_HANDLE tmlhandle, int iTimeout, AsyncCMDListElement** firstAsyncCmdListElement, AsyncCMDListElement** lastAsyncCmdListElement);


    /**
     * @brief    Get the next async cmd element.
     *
     * @param   tmlhandle                   Reference to an instance of TML_COMMAND_HANDLE that holds the command.
     * @param   iTimeout                    Reference to timeout for the command operation in milliseconds.
     * @param   firstConnectionListElement  Reference to the first async command list element.
     * @param   LastConnectionListElement   Reference to the last async command list element.
     */
    bool GetNextAsyncCmdElementOutOfQueue(TML_COMMAND_HANDLE* tmlhandle, int* iTimeout, AsyncCMDListElement** firstAsyncCmdListElement, AsyncCMDListElement** lastAsyncCmdListElement);


    /**
     * @brief   Clear pending async cmd queue.
     *
     * @param   firstConnectionListElement  Reference to the first async command list element.
     * @param   LastConnectionListElement   Reference to the last async command list element.
     */
    void ClearPendingAsyncCmdQueue(AsyncCMDListElement** firstAsyncCmdListElement, AsyncCMDListElement** lastAsyncCmdListElement);


    /**
     * @brief    Check for pending commands and send an async. message .
     *
     * @param   tmlhandle    An instance of TML_COMMAND_HANDLE that holds the command.
     * @param   iTimeout     Timeout for the command operation in milliseconds.
     * @param   bCheck       If true it will check for busy sender thread and will queue the message in in case of a running thread.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the communication context is not initialized.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
    int TMLCoreSender_SendAsyncMessage(tmlConnectionObj* pConnectionObj, TML_COMMAND_HANDLE tmlhandle, int iTimeout, bool bCheck);

public:
    // Methods:


    /**
     * @brief    Constructor.
     *
     * @param   ctx          Reference Vortex execution context.
     * @param   loghandler   Reference to a log handler.
     *
     * @returns an instance of TMLCoreSender
     */
    TMLCoreSender(VortexCtx* ctx, tmlLogHandler* loghandler);


    /**
     * @brief    Destructor.
     */
    ~TMLCoreSender();

    /**
     * @brief    Returns the error code that will be set during construction / initSender()
     *
     * @returns the error code that will be set during construction / initSender()
     */
    int getInitError();


    /**
     * @brief   Set the debug logging value.
     *
     * @param   iLogValue    The debug logging value.
     *
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if there was no running listener.<br>
     *
     * @see tmlErrors.h
     */
    int TMLCoreSender_Set_Logging_Value(int iLogValue);


    /**
     * @brief    Send a sync. message.
     *
     * @param   pConnectionObj Reference to an instance of tmlConnectionElement.
     * @param   tmlhandle      An instance of TML_COMMAND_HANDLE that holds the command.
     * @param   iTimeout       Timeout for the command operation in milliseconds.
     * @param   iMsgNo         reference to the identification number that will be given to the message.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the communication context is not initialized.<br>
     *          TML_ERR_SYSTEMRESOURCES if communication is not possible because of missing system resources.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
    int TMLCoreSender_SendMessage (tmlConnectionObj* pConnectionObj, TML_COMMAND_HANDLE tmlhandle, int iTimeout, int* iMsgNo);


    /**
     * @brief    Send an async. message.
     *
     * @param   pConnectionObj Reference to an instance of tmlConnectionElement.
     * @param   tmlhandle      An instance of TML_COMMAND_HANDLE that holds the command.
     * @param   iTimeout       Timeout for the command operation in milliseconds.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the communication context is not initialized.<br>
     *          TML_ERR_SYSTEMRESOURCES if communication is not possible because of missing system resources.<br>
     *          ERR_DUMPCONTENT if there is a problem converting the tmlhandle into a string.<br>
     *          TML_ERR_SENDER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
    int TMLCoreSender_SendAsyncMessage (tmlConnectionObj* pConnectionObj, TML_COMMAND_HANDLE tmlhandle, int iTimeout);


    /**
     * @brief    Wait until pending async commands are finished
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC if async command didn't finish after expected time.
     *
     * @see tmlErrors.h
     */
    int TMLCoreSender_WaitForPendingAsyncCmd ();


    /**
     * @brief  tmlCoreWrapper will call this method in case of a close of the connection (initiated by the listener)
     */
    void TMLCoreSender_ConnectionClose();


    /**
     * @brief   Set the log- file index for explicit logs with closing the file
     * @param   iFileLogIndex       The index.
     */
     void setLogFileIndex(int iFileLogIndex);
};


#endif // TMLCORESENDER_H 
