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

#ifndef TMLLISTENEROBJ_H
#define TMLLISTENEROBJ_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlStdTypes.h"
#include "tmlNetBinding.h"
#include "tmlCoreDefines.h"
#include <vortex.h>

class tmlListenerObj //listener object class
{
private:
    /* data */

    /**
     * @brief    Flag for the listener enable status.
     */
    TML_BOOL m_bListnerIsEnabled;

protected: 
	  /* data */
    /**
     * @brief    reference counter
     */
    int m_iRefCounter;

    /**
     * @brief    TML core handle
     */
    TML_CORE_HANDLE m_coreHandle;

    /**
     * @brief    Betwork binding object
     */
    tmlNetBinding *m_binding;

    /**
     * @brief    reference VortexConnection object
     */
    VortexConnection* m_vortexConnection;

    /**
     * @brief    reference to VORTEX instance
     */
    VortexCtx* m_ctx;

    /**
     * @brief    reference to TML error code during initialisation
     */
    TML_INT32 m_iErr;
public:
	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
     * @param   sNetAddress network address of the listener binding.
     *
     * @returns an instance of tmlListenerObj.
     */
    tmlListenerObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress);



    /**
     * @brief    Constructor.
     *
     * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
     * @param   sHost       network host / ip.
     * @param   sPort       port.
     *
     * @returns an instance of tmlListenerObj.
     */
    tmlListenerObj(TML_CORE_HANDLE coreHandle, const char* sHost, const char* sPort);


    /**
     * @brief    Destructor.
     */
    virtual ~tmlListenerObj();


    /**
     * @brief    init the object
     *
     * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
     * @param   sNetAddress network address for connection binding.
     */
    void initListenerObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress);


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief Get the TML core handle.
     */
    TML_CORE_HANDLE getCoreHandle();


    /**
     * @brief Get the network hostname / IP of the listener binding.
     *
     * @param   sHost         borrowed reference to hostname / IP
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getHost(char** sHost);


    /**
     * @brief Get the network hostname / IP of the listener binding.
     *
     * @param   sHost         borrowed reference to hostname / IP
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getHost(wchar_t** sHost);


    /**
     * @brief Get the network hostname / IP of the listener binding.
     *
     * @param   sHost         borrowed reference to hostname / IP
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getHost(char16_t** sHost);


    /**
     * @brief Get the network port of the listener binding.
     *
     * @param   sPort         borrowed reference to port
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getPort(char** sPort);


    /**
     * @brief Get the network port of the listener binding.
     *
     * @param   sPort         borrowed reference to port
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getPort(wchar_t** sPort);


    /**
     * @brief Get the network port of the listener binding.
     *
     * @param   sPort         borrowed reference to port
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getPort(char16_t** sPort);


    /**
     * @brief Get the network address of the listener binding.
     *
     * @param   sAddress         borrowed reference to network binding address
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getAddress(char** sAddress);


    /**
     * @brief Get the network address of the listener binding.
     *
     * @param   sAddress         borrowed reference to network binding address
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getAddress(wchar_t** sAddress);


    /**
     * @brief Check for equality of this listener with the requested parameter.
     *
     * @param   sAddress         network binding address
     *
     * @returns true if equal
     */
    bool isEqual(const char* sAddress);


    /**
     * @brief Get the network address of the listener binding.
     *
     * @param   sAddress         borrowed reference to network binding address
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getAddress(char16_t** sAddress);


    /**
     * @brief   Enable or disable the listener.
     *
     * @param   bEnable      TML_TRUE to enable, TML_FALSE to disable the listener
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_LISTENER_ADDRESS_BINDING listener start failed - invalid port number or interface<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED listener not initialized<br>
     *          TML_ERR_LISTENER_ALREADY_STARTED listener already enabled
     */
    TML_INT32 set_Enabled(TML_BOOL bEnable);


    /**
     * @brief   Get listener status.
     *
     * @returns listener status (TML_FALSE, TML_TRUE)
     */
    TML_BOOL get_Enabled();


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


    /**
      * @brief   returns the last error code
      */
    TML_INT32 getLastErr();


    /**
     * @brief   Get Vortex connection 
     *
     * @returns the Vortex connection / NULL if invalid
     */
    VortexConnection* getVortexConnection();
};

#endif // TMLLISTENEROBJ_H
