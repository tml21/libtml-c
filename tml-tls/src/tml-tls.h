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

/** @defgroup tlsAPI TML TLS API
  * @brief TML TLS API reference
  */
  

/** @defgroup tlsGeneral TML TLS API functions
	* @ingroup tlsAPI
  * @brief TML TLS library functions
  */

#ifndef TMLTLS_H
#define TMLTLS_H

//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

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

  /** 
 * @ingroup tlsGeneral
 * @brief Digest method provided.
 */
typedef enum {
	/** 
	 * @brief Allows to especify the DIGEST method SHA-1.
	 */
	TML_TLS_SHA1 = 1,
	/** 
	 * @brief Allows to especify the DIGEST method MD5.
	 */
	TML_TLS_MD5 = 2,

  	/** 
	 * @internal Internal value. Do not modify.
	 */
	TML_DIGEST_NUM
} TmlTlsDigestMethod;


/**
 * @ingroup tlsGeneral
 * @brief   Callback function to accept TLS requests.
 *
 * @param   serverName SIDEX string Variant containing the server name
 *
 * @returns TML_TRUE if TLS request is accepted, otherwise TML_FALSE
 *
 * @see tml_Tls_Core_AcceptNegotiation()
 */
typedef  TML_BOOL (*TML_ON_ACCEPT_TLS_REQUEST_CB_FUNC)(SIDEX_VARIANT vServerName);


/**
 * @ingroup tlsGeneral
 * @brief   Callback function to get TLS certificate file location.
 *
 * @param   serverName SIDEX string Variant containing the server name
 *
 * @returns pathname of certificate file as content of a SIDEX string Variant allocated by the callback function.<br>It has to be freed by the caller using sidex_Variant_DecRef()
 *
 * @see tml_Tls_Core_AcceptNegotiation()
 */
typedef  SIDEX_VARIANT (*TML_ON_CERTIFICATE_FILE_LOCATION_CB_FUNC)(SIDEX_VARIANT vServerName);


/**
 * @ingroup tlsGeneral
 * @brief   Callback function to get TLS private key file location.
 *
 * @param   serverName SIDEX string Variant containing the server name
 *
 * @returns pathname of private key file as content of a SIDEX string Variant allocated by the callback function.<br>It has to be freed by the caller using sidex_Variant_DecRef()
 *
 * @see tml_Tls_Core_AcceptNegotiation()
 */
typedef  SIDEX_VARIANT (*TML_ON_CERTIFICATE_PRIVATE_KEY_LOCATION_CB_FUNC)(SIDEX_VARIANT vServerName);


/**
 * @ingroup tlsGeneral
 * @brief   Callback function called when an error is found during TLS handshake.
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   errorMessage     string Variant containing the errorMessage
 * @param   pErrorData       reference to optional user data configured in tml_Tls_Core_Set_OnError() 
 *
 * @see tml_Tls_Core_Set_OnError()
 */
typedef  void (*TML_ON_TLS_ERROR_CB_FUNC)(TML_CONNECTION_HANDLE connection, SIDEX_VARIANT errorMessage, void* pErrorData);


/**
 * @ingroup tlsGeneral
 * @brief   Allows to configure if the provided tml core will accept TLS incoming connections.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   pAcceptCB reference to callback handler method executed to notify user app level that a TLS request was received,<br>
 *          allowing to accept or deny it according to the return value of the handler.
 * @param   pCertReqCB reference to callback handler method executed to know where is located the certificate file to be used to cipher the session.
 * @param   pPrivateKeyReqCB reference to callback method executed to know where is located the private key file to be used to cipher the session.
 * @param   bAccept TML_TRUE if the current core instance could accept incoming TLS connections, otherwise TML_FALSE is returned
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid handle / reference parameter
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_AcceptNegotiation(TML_CORE_HANDLE coreHandle, 
                                                                     TML_ON_ACCEPT_TLS_REQUEST_CB_FUNC pAcceptCB, 
                                                                     TML_ON_CERTIFICATE_FILE_LOCATION_CB_FUNC pCertReqCB,
                                                                     TML_ON_CERTIFICATE_PRIVATE_KEY_LOCATION_CB_FUNC pPrivateKeyReqCB,
                                                                     TML_BOOL* bAccept);


/**
 * @ingroup tlsGeneral
 * @brief   Starts the TLS transport security negotiation on the given connection.
 *
 * @param   connectionHandle   TML connection handle (TML_CONNECTION_HANDLE)
 * @param   bAllowTlsFailures  configure how to handle errors produced while activating TLS negotiation. TML_TRUE - connection will be used unencrypted if possible.
 * @param   bEncrypted         reference to encryption status.TML_TRUE if the current connection instance is encrypted, otherwise TML_FALSE
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid handle / reference parameter
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_StartNegotiation (TML_CONNECTION_HANDLE connectionHandle, TML_BOOL bAllowTlsFailures, TML_BOOL* bEncrypted);


/**
 * @ingroup tlsGeneral
 * @brief   Allows to activate TLS profile automatic negotiation for every connection created.
 *
 * @param   coreHandle TML    core handle (TML_CORE_HANDLE)
 * @param   bEnabled          TML_TRUE to activate the automatic TLS profile negotiation for every connection created, TML_FALSE to disable it
 * @param   bAllowTlsFailures configure how to handle errors produced while activating automatic TLS negotiation
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid coreHandle handle
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_Set_AutoNegotiation (TML_CORE_HANDLE coreHandle, TML_BOOL bEnabled, TML_BOOL bAllowTlsFailures);


/**
 * @ingroup tlsGeneral
 * @brief   Allows to verify peer certificate after successfully establish TLS session. 
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   bVerifyOk        reference to verification result.TML_TRUE if certificate verification status is ok, otherwise TML_FALSE 
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid handle / reference parameter
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_VerifyCert (TML_CONNECTION_HANDLE connectionHandle, TML_BOOL* bVerifyOk);


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL
/**
 * @ingroup tlsGeneral
 * @brief   Is encrption enabled for the requested connection.
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   bEncrypted       reference to encryption status.TML_TRUE if the current connection instance is encrypted, otherwise TML_FALSE
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid handle / reference parameter
 */
  // TODO: 
  // The connectionHandle->isEncrpyted() flag cannot be set using tml_Tls_Core_SetAuto_Negation().
  // It can only be configured in the tml_Tls_Connection_StartNegotiation().
  // tml_Tls_Core_SetAuto_Negation has no handler to get this information.
  // 
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Valid (TML_CONNECTION_HANDLE connectionHandle, TML_BOOL* bEncrypted);

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/


/**
 * @ingroup tlsGeneral
 * @brief   Get the encrption status message created during the start of the TLS transport security negotiation.
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   statusMsg        reference to string containing encryption status message.<br>
 *                           This is a borrowed reference and has not to be freed or modified
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED if no status message exists<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle / reference parameter
 *
 * @see tml_Tls_Connection_StartNegotiation()
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage (TML_CONNECTION_HANDLE connectionHandle, TML_CTSTR** statusMsg);
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


/**
 * @ingroup tlsGeneral
 * @brief   Allows to create a digest from the provided string.
 *
 * @param   string	         the string to digest
 * @param   method           the digest method to use
 * @param   sDigest          reference to string with the hash value that represents the string provided.<br>
 *                           Must be deallocated using the data type API sidex_Free_ReadString()
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle / reference parameter
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Get_Digest (TML_CTSTR* string, TmlTlsDigestMethod method, TML_CTSTR** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Get_Digest_X (wchar_t* string, TmlTlsDigestMethod method, wchar_t** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Get_Digest_W (char16_t* string, TmlTlsDigestMethod method, char16_t** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Get_Digest_A (char* string, TmlTlsDigestMethod method, char** sDigest);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Tls_Get_Digest tml_Tls_Get_Digest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Tls_Get_Digest  tml_Tls_Get_Digest_W
    #else
      #define tml_Tls_Get_Digest  tml_Tls_Get_Digest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tlsGeneral
 * @brief   Allows to return the certificate digest from the remote peer given TLS session is activated (this is also called the certificate fingerprint).
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   method           the digest method to use
 * @param   sDigest          reference to string representing a newly allocated fingerprint or NULL if it fails. If NULL is returned there is a TLS error (certificate not provided) or the system is out of memory.<br>
 *                           Must be deallocated using the data type API sidex_Free_ReadString()
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle / reference parameter
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_PeerSSLDigest (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, TML_CTSTR** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_PeerSSLDigest_X (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, wchar_t** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_PeerSSLDigest_W (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, char16_t** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_PeerSSLDigest_A (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, char** sDigest);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Tls_Connection_Get_PeerSSLDigest tml_Tls_Connection_Get_PeerSSLDigest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Tls_Connection_Get_PeerSSLDigest  tml_Tls_Connection_Get_PeerSSLDigest_W
    #else
      #define tml_Tls_Connection_Get_PeerSSLDigest  tml_Tls_Connection_Get_PeerSSLDigest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tlsGeneral
 * @brief   Allows to return the certificate digest from a local stored certificate file (this is also called the certificate fingerprint).
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   method           the digest method to use
 * @param   sPathName        reference to pathname of the certificate file
 * @param   sDigest          reference to string representing a newly allocated fingerprint or NULL if it fails.<br>
 *                           Must be deallocated using the data type API sidex_Free_ReadString()
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle / reference parameter<br>
 *          TML_ERR_INFORMATION_UNDEFINED wrong sPathName or sPathName refers no valid certificate file
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_SSLDigest (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, TML_CTSTR* sPathName, TML_CTSTR** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_SSLDigest_X (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, wchar_t* sPathName, wchar_t** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_SSLDigest_W (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, char16_t* sPathName, char16_t** sDigest);
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_SSLDigest_A (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, char* sPathName, char** sDigest);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Tls_Connection_Get_SSLDigest tml_Tls_Connection_Get_SSLDigest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Tls_Connection_Get_SSLDigest  tml_Tls_Connection_Get_SSLDigest_W
    #else
      #define tml_Tls_Connection_Get_SSLDigest  tml_Tls_Connection_Get_SSLDigest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tlsGeneral
 * @brief   Allows to configure an error handler that will be called when a failure is found at SSL level or during the handshake with the particular function failing.
 *
 * @param   coreHandle TML    core handle (TML_CORE_HANDLE)
 * @param   pErrorCB          the error handler to be called when an error is found
 * @param   pErrorData        reference to optional user data to be passed into the function when the handler is called
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_Set_OnError(TML_CORE_HANDLE coreHandle, TML_ON_TLS_ERROR_CB_FUNC pErrorCB, void* pErrorData);
#ifdef __cplusplus
}// extern "C"
#endif
#endif  // TMLTLS_H
