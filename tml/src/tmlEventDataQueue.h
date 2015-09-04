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

#ifndef TMLEVENTDATAQUEUE_H
#define TMLEVENTDATAQUEUE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlEventHandler.h"
#include "tmlCoreDefines.h"

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the async command link list:
struct QueueElement {
       QueueElement* next;                       // the next chain element
       QueueElement* prev;                       // the prev chain element
       EventMsgExecutionData* data;              // The data
};


class  tmlEventDataQueue
{

private:
    /* data */
    int  m_iQueueLength;

  /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;
    
    /**
     * @brief    A mutex to protect critial sections
     */
    VortexMutex       m_mutexCriticalSection;

    /**
     * @brief    Reference to the data structure of the first async cmd list entry
     */
    QueueElement* m_firstElement;

    /**
     * @brief    Reference to the data structure of the last async cmd list entry
     */
    QueueElement* m_lastElement;

public:
    /**
     * @brief    Constructor.
     *
     * @param   loghandler   Reference to a log handler.
     *
     * @returns an instance of tmlEventDataQueue
     */
    explicit tmlEventDataQueue(tmlLogHandler* loghandler);

    /**
     * @brief    Destructor.
     */
    ~tmlEventDataQueue();


    /**
     * @brief    Add an event message data object into the queue.
     *
     * @param   data     Reference to an event message data object into the queue.
     */
    void addData2Queue(EventMsgExecutionData* data);


    /**
     * @brief    Get the next event message data object out of the queue(Chain element will be removed).
     *
     * @param   data     Reference to reference to the an event message data object result.
     * @param   bUseCriticalSection  Flag, if true the critical section mechanism shall be used.
     *
     * @returns true on success, otherwise false (the data reference will be NULL in that case.
     */
    bool getDataQueueFrom(EventMsgExecutionData** data, bool bUseCriticalSection);


    /**
     * @brief    Get the length of the message data object queue.
     *
     * @returns the length of the message data object queue.
     */
    int getQueueLength();
    

    /**
     * @brief    Clear the message data object queue.
     */
    void clear();


    /**
     * @brief    Destroy content of data object.
     *
     * @param   data     Reference to an event message data object.
     */
    void destroyDataContent(EventMsgExecutionData* data);


private:


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
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     */
    void enterCriticalSection(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    Leave a critical Section
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     */
    void leaveCriticalSection(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);

};
#endif //TMLEVENTDATAQUEUE_H

