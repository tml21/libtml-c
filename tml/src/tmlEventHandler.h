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

#ifndef TMLEVENTHANDLER_H
#define TMLEVENTHANDLER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlUniversalHashTable.h"
#include "tmlLogHandler.h"


class tmlEventHandler{

    /**
     * @brief    An error occurred on wait for single / multiple events
     */
    #define WAIT_ERR       0

    /**
     * @brief    Wait for single / multiple events succeeded
     */
    #define WAIT_SUCCEEDED 1

    /**
     * @brief    A timeout occurred on wait for single / multiple events
     */
    #define WAIT_TOUT      2

private:
	  /* data */

    /**
     * @brief    instance of a hashTable
     */
    TMLUniversalHashTable* m_hashTable;

#ifdef LINUX

    /**
     * @brief    the semaphore identificarion
     */
    int m_semid;

    /**
     * @brief    the semaphore index for create
     */
    int m_iSemIdx;

#endif // LINUX

    /**
     * @brief    logging object
     */
    tmlLogHandler* m_log;

#ifdef LINUX
    /**
    * @brief  sleep for one millisecond and check for timeout condition
    *
    * @param   pTimeout reference to the amount of left milliseconds
    *
    * @returns true on timeout condition, otherwise false
    */
    bool sleepForOneMilliSecondAndCheckTimeout(DWORD* pTimeout);
#endif

public:

	  /* data */
	  /* Methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlEventHandler
     */
    explicit tmlEventHandler(tmlLogHandler* log);


    /**
     * @brief    Destructor.
     */
    ~tmlEventHandler();


    /**
     * @brief    Create a HANDLE to be able to signal a condition
     *
     * @param   cIdentification reference to the handle identification.
     *
     * @returns true on success, otherwise false
     */
    bool CreateEventHandle(const char* cIdentification);


    /**
     * @brief    Remove the HANDLE entry that has been created to be able to signal a condition
     *
     * @param   cIdentification reference to the handle identification.
     *
     * @returns true on success, otherwise false
     */
    bool RemoveEventHandle(const char* cIdentification);


    /**
     * @brief    Set an event to a handle
     *
     * @param   cIdentification reference to the handle identification.
     *
     * @returns true on success, otherwise false
     */
    bool SetEventOnHandle(const char* cIdentification);


    /**
     * @brief    Reset an event to a handle
     *
     * @param   cIdentification reference to the handle identification.
     *
     * @returns true on success, otherwise false
     */
    bool ResetEventOnHandle(const char* cIdentification);


    /**
     * @brief    Wait until a single object was signaled
     *
     * @param   cIdentification reference to the handle identification.
     * @param   timeout the timeout value in milliseconds / may be INFINITE.
     *
     * @returns WAIT_SUCCEEDED on success<br>
     *          WAIT_ERR on error<br>
     *          WAIT_TOUT on timeout
     */
    int WaitForSingleEvent(const char* cIdentification, DWORD timeout);


    /**
     * @brief    Wait until one of a couple of objects has been signaled
     *
     * @param   cIdentifications reference to the handle identifications.
     * @param   iCount           Number of handle identifications.
     * @param   timeout          The timeout value in milliseconds / may be INFINITE.
     * @param   iIndex           On success the index content is the index of the signaled identifier.
     *
     * @returns WAIT_SUCCEEDED on success<br>
     *          WAIT_ERR on error<br>
     *          WAIT_TOUT on timeout
     */
    int WaitForMultipleEvent(const char** cIdentifications, DWORD iCount, DWORD timeout, DWORD* iIndex);
};
#endif  // TMLEVENTHANDLER_H

