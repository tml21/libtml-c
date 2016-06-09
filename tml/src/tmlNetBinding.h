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

#ifndef TMLNETBINDING_H
#define TMLNETBINDING_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlStdTypes.h"

class tmlNetBinding //listener object class
{
private:
    /* data */

protected: 
    /* data */
    SIDEX_VARIANT m_addresses;
    SIDEX_VARIANT m_addresses_w;
    SIDEX_VARIANT m_addresses_x;

    SIDEX_VARIANT m_hosts;
    SIDEX_VARIANT m_hosts_w;
    SIDEX_VARIANT m_hosts_x;

    SIDEX_VARIANT m_ports;
    SIDEX_VARIANT m_ports_w;
    SIDEX_VARIANT m_ports_x;

    SIDEX_VARIANT m_isIPV6;
    int           m_iBindings;
    TML_BOOL      m_bValid;

public:
	  /* methods */
    /**
     * @brief    Constructor.
     *
     * @param   sNetAddress network address for listener binding.
     *
     * @returns an instance of tmlNetBinding.
     */
    explicit tmlNetBinding(const char* sNetAddress);


    /**
     * @brief    Destructor.
     */
    virtual ~tmlNetBinding();

    /**
     * @brief Get the network host / ip.
     *
     * @param   sHost  borrowed reference to host / ip at index 0
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getHost(char** sHost);


    /**
     * @brief Get the network host / ip.
     *
     * @param   sHost  borrowed reference to host / ip at index 0
     *
     * @returns TML_SUCCESS on success
     */
     SIDEX_INT32 getHost(char16_t** sHost);


    /**
     * @brief Get the network host / ip.
     *
     * @param   sHost  borrowed reference to host / ip at index 0
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getHost(wchar_t** sHost);


    /**
     * @brief Get the network host / ip.
     *
     * @param   index  network binding index
     * @param   sHost  borrowed reference to host / ip
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getHost(TML_INT32 index, char** sHost);


    /**
     * @brief Get the network host / ip.
     *
     * @param   index  network binding index
     * @param   sHost  borrowed reference to host / ip
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getHost(TML_INT32 index, char16_t** sHost);


    /**
     * @brief Get the network host / ip.
     *
     * @param   index  network binding index
     * @param   sHost  borrowed reference to host / ip
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getHost(TML_INT32 index, wchar_t** sHost);


    /**
     * @brief Get the network port.
     *
     * @param   sPort  borrowed reference to port name at index 0
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getPort(char** sPort);


    /**
     * @brief Get the network port.
     *
     * @param   sPort  borrowed reference to port name at index 0
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getPort(char16_t** sPort);


    /**
     * @brief Get the network port.
     *
     * @param   sPort  borrowed reference to port name at index 0
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getPort(wchar_t** sPort);


    /**
     * @brief Get the network port.
     *
     * @param   index  network binding index
     * @param   sPort  borrowed reference to port name
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getPort(TML_INT32 index, char** sPort);


    /**
     * @brief Get the network port.
     *
     * @param   index  network binding index
     * @param   sPort  borrowed reference to port name
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getPort(TML_INT32 index, char16_t** sPort);


    /**
     * @brief Get the network port.
     *
     * @param   index  network binding index
     * @param   sPort  borrowed reference to port name
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getPort(TML_INT32 index, wchar_t** sPort);


    /**
     * @brief Get the network address.
     *
     * @param   sAddress  borrowed reference to network binding address at index 0
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getAddress(char** sAddress);


    /**
     * @brief Get the network address.
     *
     * @param   sAddress  borrowed reference to network binding address at index 0
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getAddress(char16_t** sAddress);


    /**
     * @brief Get the network address.
     *
     * @param   sAddress  borrowed reference to network binding address at index 0
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getAddress(wchar_t** sAddress);


    /**
     * @brief Get the network address.
     *
     * @param   index  network binding index
     * @param   sAddress  borrowed reference to network binding address
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getAddress(TML_INT32 index, char** sAddress);


    /**
     * @brief Get the network address.
     *
     * @param   index  network binding index
     * @param   sAddress  borrowed reference to network binding address
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getAddress(TML_INT32 index, char16_t** sAddress);


    /**
     * @brief Get the network address.
     *
     * @param   index  network binding index
     * @param   sAddress  borrowed reference to network binding address
     *
     * @returns TML_SUCCESS on success
     */
    SIDEX_INT32 getAddress(TML_INT32 index, wchar_t** sAddress);


    /**
     * @brief check the network host type.
     *
     * @returns TML_TRUE if host type is IPV6
     */
    TML_BOOL isIPV6();


    /**
     * @brief check the network host type.
     *
     * @returns TML_TRUE if host type is IPV6
     */
    TML_BOOL isIPV6(TML_INT32 index);


    /**
     * @brief check if binding is valid.
     *
     * @returns TML_TRUE if binding is valid.
     */
    TML_BOOL isValid();

protected:
	  /* methods */
    /**
     * @brief    add a IP attribute to SIDEX list.
     */
    int addIPAttrItem(int iBindings, SIDEX_BOOL bIsIPV6);


    /**
     * @brief    add a host item to SIDEX list.
     */
    int addHostItem(int iBindings, const char* sHostStartAddr, int iLength);


    /**
     * @brief    add a port item to SIDEX list.
     */
    int addPortItem(int iBindings, const char* sPortStartAddr, int iLength);

    /**
     * @brief    add an address item to SIDEX list.
     */
    int addAddressItem(int iBindings, const char* sAddr, int iLength);

};
#endif // TMLNETBINDING_H
