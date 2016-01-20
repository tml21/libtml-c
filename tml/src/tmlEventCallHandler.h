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

#ifndef TMLEVENTCALLHANDLER_H
#define TMLEVENTCALLHANDLER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlEventHandler.h"
#include "tmlCoreDefines.h"


/**
 * @brief    Definition for index to terminate the background thread
 */
#define EVENT_HANDLING_MESSAGE_CONTINUE_EVENT  0

/**
 * @brief    Definition for index to continue the background thread
 */
#define EVENT_HANDLING_MESSAGE_TERMINATE_EVENT 1

/**
 * @brief    Number of event for the thread handling
 */
#define NO_OF_EVENT_MESSAGE_HANDLING_HANDLES   2

/**
 * @brief    Definition for event to terminate the background thread
 */
#define TERMINATE_EVENT_HANDLER_MESSAGE_HANDLE "terminateEventHandlerMessageHandle"

/**
* @brief    Definition for event to continue the background thread
  */
#define CONTINUE_EVENT_HANDLER_MESSAGE_HANDLE "continueEventHandlerMessageHandle"


class  tmlEventCallHandler
{

private:
    /* data */
    /**
     * @brief    The handler to implement event handling
     */
    tmlEventHandler*     m_eventHandler;

    /**
     * @brief    Thread inforamtion structure 
     */
    TMLThreadDef m_threadInfo;

    /**
     * @brief    Array of Handles for thread synchronisation
     */
    const char** m_eventMessageHandlingEventArray;
    
    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;

    /**
     * @brief    Reference to a callers class callback method
     */
    void*          m_pCBFunc;

    
public:
    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlEventCallHandler
     */
    tmlEventCallHandler(tmlLogHandler* loghandler, void* pCBFuncHandler, void* pCBFuncExec);


    /**
     * @brief    Destructor.
     */
    ~tmlEventCallHandler();
    
    /**
     * @brief    A new event message has to be operated in the background
     */
    int performBackendEventMessage(EventMsgHandlingParams* threadHandlingParams, EventMsgExecutionData* data);

    void stopEventMessageHandlingThread();

private:

    /**
     * @brief    Allocate event handles for process synchronisation.
     *
     * @param   asyncThreadEventArray reference to event handle array.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the HANDLE  creation failed.
     */
    int allocEventMessageThreadEventArray(const char*** asyncThreadEventArray);


    /**
     * @brief    Free event handles for process synchronisation.
     *
     * @param   senderSyncEventArray reference to event handle array.
     * @param   iLength              Length of the array.
     */
    void freeEventMessageEventArray(const char*** senderSyncEventArray, int iLength);


    /**
     * @brief   Destroy a process synchronisation event handle.
     *
     * @param   handle reference to the handle.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the HANDLE is invalid.
     *
     * @see tmlErrors.h
     */
    int destroySynchronisationHandle(const char** handle);


    /**
     * @brief    Reset event handles for process synchronisation.
     *
     * @param   senderSyncEventArray reference to event handle array.
     * @param   iLength              Length of the array.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_SENDER_NOT_INITIALIZED if the HANDLE  is NULL.
     */
    int resetSyncEventArray(const char*** senderSyncEventArray, int iLength);

    /**
     * @brief    Start the EventMessageHandlingThread.
     */
    int startEventMessageHandlingThread(EventMsgHandlingParams* threadData, tmlEventHandler* eventHandler, const char** handlingEventArray, tmlLogHandler* pLog, void* pCBFunc, TMLThreadDef* threadInfo);
};
#endif //TMLEVENTCALLHANDLER_H

