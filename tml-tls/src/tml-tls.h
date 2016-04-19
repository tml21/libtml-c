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
 
 
//-----------------------------------------
// write here package with @ to get namespace TAB in the html- documentation
//-----------------------------------------

/** package  TML_TLS
  */


/** @file tml-tls.h
  * 
  * @brief Declarations and dll import file for the TML-TLS C API.
  */

/** @defgroup coreAPI TML API
  * @brief TML-TLS API reference
  */
  

/** @defgroup coreGeneral General
	* @ingroup coreAPI
  * @brief General library functions
  */
#ifndef TMLTLS_H
#define TMLTLS_H

//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/** @defgroup tmladvanced TMLCore expert use
	* @ingroup coreAPI
  * @brief Internal developer API calls.<br><br>
  * @note For develop use only / we recommend not to use this API calls as a standard user.
  */

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globalDefines.h"
#include "tmlStdTypes.h"

#ifndef DOXYGEN_GENERATION
#ifdef LINUX
  #define TLS_CORE_API __attribute__ ((__visibility__("default")))
#else
  #ifdef TML_EXPORTS
    /**
    * @brief    Definition for export of the dll
    */
    #define TLS_CORE_API __declspec(dllexport)
  #else
    /**
    * @brief    Definition for import of the dll
    */
    #define TLS_CORE_API __declspec(dllimport)
  #endif
#endif
#endif //DOXYGEN_GENERATION

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

void* m_pAcceptCB = NULL;
void* m_pCertReqCB = NULL;
void* m_pPrivateKeyReqCB = NULL;

/**
 * @brief   Callback function to accept TLS requests.

 * @param   serverName SIDEX_VARIANT containing the server name
 *
 * @returns TML_TRUE if TLS request is accepted
 */
typedef  TML_BOOL (*TML_ON_ACCEPT_TLS_REQUEST_CB_FUNC)(SIDEX_VARIANT vServerName);


/**
 * @brief   Callback function to get TLS private key file location.

 * @param   serverName SIDEX_VARIANT containing the server name
 *
 * @returns pathname of private key file as content of a SIDEX string Variant. It has to be freed by the caller.
 */
typedef  SIDEX_VARIANT (*TML_ON_CERTIFICATE_PRIVATE_KEY_LOCATION_CB_FUNC)(SIDEX_VARIANT vServerName);


/**
 * @brief   Callback function to get TLS certificate file location.

 * @param   serverName SIDEX_VARIANT containing the server name
 *
 * @returns pathname of certificate file as content of a SIDEX string Variant. It has to be freed by the caller.
 */
typedef  SIDEX_VARIANT (*TML_ON_CERTIFICATE_FILE_LOCATION_CB_FUNC)(SIDEX_VARIANT vServerName);


/**
 * @ingroup  coreGeneral
 * @brief    Accept tls negotiation on listener side
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 *
 * param    bAccept TML_TRUE if the current core instance could accept incoming TLS connections, otherwise TML_FALSE is returned. 
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_Accept_Negotiation(TML_CORE_HANDLE coreHandle, 
                                                                     TML_ON_ACCEPT_TLS_REQUEST_CB_FUNC pAcceptCB, 
                                                                     TML_ON_CERTIFICATE_FILE_LOCATION_CB_FUNC pCertReqCB,
                                                                     TML_ON_CERTIFICATE_PRIVATE_KEY_LOCATION_CB_FUNC pPrivateKeyReqCB,
                                                                     TML_BOOL* bAccept);


/**
 * @ingroup  coreGeneral
 * @brief    Start tls negotiation for the requested connection
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   bEncrypted       reference to encryption status.TML_TRUE if the current connection instance is encrypted, otherwise TML_FALSE
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid connection handle
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Start_Negotiation (TML_CONNECTION_HANDLE connectionHandle, TML_BOOL* bEncrypted);


/**
 * @ingroup  coreGeneral
 * @brief    Is encrption enabled for the requested connection
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   bEncrypted       reference to encryption status.TML_TRUE if the current connection instance is encrypted, otherwise TML_FALSE
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid connection handle
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Valid (TML_CONNECTION_HANDLE connectionHandle, TML_BOOL* bEncrypted);


/**
 * @ingroup  coreGeneral
 * @brief    Get the encrption status message
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   statusMsg        reference to string containing encryption status message.<br>
 *                           This is a borrowed reference and has not to be freed or modified.
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED if no status message exists<br>
 *          TML_ERR_MISSING_OBJ invalid connection handle
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage (TML_CONNECTION_HANDLE connectionHandle, SIDEX_CTSTR** statusMsg);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage_X (TML_CONNECTION_HANDLE connectionHandle, wchar_t** statusMsg);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage_W (TML_CONNECTION_HANDLE connectionHandle, char16_t** statusMsg);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage_A (TML_CONNECTION_HANDLE connectionHandle, char** statusMsg);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Tls_Connection_Encryption_Get_StatusMessage tml_Tls_Connection_Encryption_Get_StatusMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Tls_Connection_Encryption_Get_StatusMessage  tml_Tls_Connection_Encryption_Get_StatusMessage_W
    #else
      #define tml_Tls_Connection_Encryption_Get_StatusMessage  tml_Tls_Connection_Encryption_Get_StatusMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


#ifdef __cplusplus
}// extern "C"
#endif
#endif  // TMLTLS_H
