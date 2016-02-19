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

#ifndef TMLCONNECTIONMANAGEOBJ_H
#define TMLCONNECTIONMANAGEOBJ_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlStdTypes.h"
#include "tmlCoreWrapper.h"

class tmlConnectionManageObj //connection management object class
{
private:
    /* data */

    /**
     * @brief    connection binding address
     */
    char*     m_sHost;
    char*     m_sPort;
    char*     m_sNetAddress;
    char16_t* m_sNetAddress_w;
    wchar_t*  m_sNetAddress_x;

protected: 
	  /* data */
    int m_iRefCounter;

    TML_CORE_HANDLE m_coreHandle;

public:
	  /* methods */
    /**
     * @brief    Constructor.
     *
     * @param   coreHandle       TML core handle (TML_CORE_HANDLE)
     * @param   sNetAddress network address for connection binding.
     *
     * @returns an instance of tmlConnectionManageObj.
     */
    tmlConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress);


    /**
     * @brief    Destructor.
     */
    virtual ~tmlConnectionManageObj();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief Get the TML core handle.
     */
    TML_CORE_HANDLE getCoreHandle();


    /**
     * @brief Get the network address for connection binding.
     *
     * @param   sAddress       borrowed reference to network address for connection binding.

     * @returns TML_SUCCESS
     */
    TML_INT32 getAddress_A(char** sAddress);


    /**
     * @brief Get the network address for connection binding.
     *
     * @param   sAddress       borrowed reference to network address for connection binding.

     * @returns TML_SUCCESS
     */
    TML_INT32 getAddress_X(wchar_t** sAddress);


    /**
     * @brief Get the network address for connection binding.
     *
     * @param   sAddress       borrowed reference to network address for connection binding.

     * @returns TML_SUCCESS
     */
    TML_INT32 getAddress_W(char16_t** sAddress);


    /**
     * @brief Returns the remote peer supported profiles.
     *
     * @param   lProfiles   reference to profile list.<br>
     *                      The list has to be released with sidex_Variant_DecRef().
     *
     * @returns TML_SUCCESS
     */
    TML_INT32 getRemoteProfiles(SIDEX_VARIANT* lProfiles);


    /**
     * @brief    Validate a connection.
     *
     * @param   bReconnect       TML_TRUE = try to reconnect if disconnected / TML_FALSE = don't try to reconnect
     * @param   bConnected       reference to the connection status, TML_TRUE if the connection is valid
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_MISSING_OBJ invalid core handle
     */
    TML_INT32 validate(TML_BOOL bReconnect, TML_BOOL* bConnected);


    /**
     * @brief   Decrement the reference counter value of this data object for the memory management.
     *
     *          A reference counter value of 0 signals that there is no more reference to the object.<br>
     *          In this case the internal memory management will free allocated memory.
     *
     * @returns The reference counter value after the opperation.
     */
    int decRef();


    /**
     * @brief   Increment the reference counter value of this data object for the memory management.
     *
     * @returns The reference counter value after the opperation.
     */
    int incRef();


    /**
     * @brief   Get the reference counter value of this data object for the memory management.
     *
     * @returns The reference counter value.
     */
    int getRef();
};

#endif // TMLCONNECTIONMANAGEOBJ_H
