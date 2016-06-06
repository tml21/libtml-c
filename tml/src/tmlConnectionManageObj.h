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

#include <vortex.h>
#include "tmlStdTypes.h"
#include "tmlNetBinding.h"
#include "tmlCCallback.h"
#include "tmlConnectionManageObjBase.h"

//////////////////////////////////////////////////////////////////////////////
// C - declarations:
extern "C" {

 /**
 * @brief  callback in case of a close of the connection (initiated by the listener)
 *
 * @param   connection Reference to VortexConnection that has been closed
 * @param   user_data  Reference to user data registered on Vortex API vortex_connection_set_on_close_full()
 */
  void connectionCloseHandler(VortexConnection *connection, axlPointer user_data);

}// extern "C"

class tmlConnectionManageObj : public tmlConnectionManageObjBase //connection management object class
{
private:
    /* data */

protected: 
	  /* data */
    /**
     * @brief    reference counter
     */
    int               m_iRefCounter;

    /**
     * @brief    Betwork binding object
     */
    tmlNetBinding*    m_binding;

    /**
     * @brief    reference to TML error code during initialisation
     */
    TML_INT32         m_iErr;

    /**
     * @brief    reference to a callback method that will be called in case of a connection
     */
    void* m_onConnectCallback;

    /**
     * @brief    reference to a callback method that will be called in case of a disconnection
     */
    void* m_onDisconnectCallback;

    /**
     * @brief    reference to a programmable callback method that will be called in case of a disconnection
     */
    void* m_onProgrammableDisconnectCallback;

    /**
     * @brief    Reference to the class callback handling method for connection close
     */
    TCallback<tmlConnectionManageObj> m_internalConnectionCloseHandlerMethod;

    /**
     * @brief    true is the vortexConnection was allocated new
     */
    bool m_bIsOwner;

public:
	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @param   coreHandle       TML core handle (TML_CORE_HANDLE)
     * @param   sNetAddress network   address for connection binding.
     * @param   pOnConnectCallback    reference to callback method to be invoked on connection.
     * @param   pOnDisconnectCallback reference to callback method to be invoked on disconnection.
     * @param   vortexConnection      reference to a VortexConnection, NULL to create a new connection
     *
     * @returns an instance of tmlConnectionManageObj.
     */
    tmlConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress, void* pOnConnectCallback, void* pOnDisconnectCallback, VortexConnection* vortexConnection);



    /**
     * @brief    Constructor.
     *
     * @param   coreHandle       TML core handle (TML_CORE_HANDLE)
     * @param   sHost            network host / ip.
     * @param   sPort            port.
     * @param   pOnConnectCallback    reference to callback method to be invoked on connection.
     * @param   pOnDisconnectCallback reference to callback method to be invoked on disconnection.
     * @param   vortexConnection      reference to a VortexConnection, NULL to create a new connection
     *
     * @returns an instance of tmlConnectionManageObj.
     */
    tmlConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sHost, const char* sPort, void*  pOnConnectCallback, void*  pOnDisconnectCallback, VortexConnection* vortexConnection);


    /**
     * @brief    init the object
     *
     * @param   coreHandle       TML core handle (TML_CORE_HANDLE)
     * @param   sNetAddress network address for connection binding.
     * @param   pOnConnectCallback    reference to callback method to be invoked on connection.
     * @param   pOnDisconnectCallback reference to callback method to be invoked on disconnection.
     * @param   vortexConnection      reference to a VortexConnection, NULL to create a new connection
     */
    void initConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress, void*  pOnConnectCallback, void*  pOnDisconnectCallback, VortexConnection* vortexConnection);


    /**
     * @brief    Destructor.
     */
    virtual ~tmlConnectionManageObj();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief Returns the connection close handler
     */
    void* getConnectionCloseHandler();

    /**
     * @brief Get the network hostname / IP of the connection binding.
     *
     * @param   sHost         borrowed reference to hostname / IP
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getHost(char** sHost);


    /**
     * @brief Get the network hostname / IP of the connection binding.
     *
     * @param   sHost         borrowed reference to hostname / IP
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getHost(wchar_t** sHost);


    /**
     * @brief Get the network hostname / IP of the connection binding.
     *
     * @param   sHost         borrowed reference to hostname / IP
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getHost(char16_t** sHost);


    /**
     * @brief Get the network port of the connection binding.
     *
     * @param   sPort         borrowed reference to port
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getPort(char** sPort);


    /**
     * @brief Get the network port of the connection binding.
     *
     * @param   sPort         borrowed reference to port
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getPort(wchar_t** sPort);


    /**
     * @brief Get the network port of the connection binding.
     *
     * @param   sPort         borrowed reference to port
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getPort(char16_t** sPort);


    /**
     * @brief Get the network address of the connection binding.
     *
     * @param   sAddress         borrowed reference to network binding address
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getAddress(char** sAddress);


    /**
     * @brief Get the network address of the connection binding.
     *
     * @param   sAddress         borrowed reference to network binding address
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getAddress(wchar_t** sAddress);


    /**
     * @brief Get the network address of the connection binding.
     *
     * @param   sAddress         borrowed reference to network binding address
     *
     * @returns TML_SUCCESS in case of success<br>
     *          TML_ERR_NET_BINDING network binding syntax error
     */
    TML_INT32 getAddress(char16_t** sAddress);


    /**
     * @brief Check for equality of this connection with the requested parameter.
     *
     * @param   sAddress         network binding address
     *
     * @returns true if equal
     */
    bool isEqual(const char* sAddress);


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
     * @brief Check the connection
     *
     * @param   bConnected       reference to the connection status, TML_TRUE if the connection is valid
     */
    void checkConnection(TML_BOOL* bConnected);


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
     * @brief   Establish the Vortex connection 
     *
     * @returns TML_SUCCESS in case of success
     */
    TML_INT32 establishVortexConnection();


    /**
      * @brief   returns the last error code
      */
    TML_INT32 getLastErr();


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
     * @brief   Class callback method that will be called in case of a close of the connection
     *
     * @param   connection reference VortexConnection object
     *
     * @returns true on success.
     */
    bool SignalConnectionClose(void* connection);


    /**
     * @brief   Set callback method for disconnection
     *
     * @param   setOnDisconnectFullCB reference to callback method
     */
    void setOnDisconnectFull(void* setOnDisconnectFullCB);


    /**
     * @brief     register connectionLost callback
     *
     * @param   connection reference VortexConnection of the request
     */
    void registerConnnectionLost(VortexConnection* connection);


    /**
     * @brief     Deregister connectionLost callback
     */
    void deregisterConnnectionLost();
};

#endif // TMLCONNECTIONMANAGEOBJ_H
