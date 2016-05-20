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

/** @file tmlCore.cpp
  * 
  * @brief Definitions and dll import file for the TMLCore Object API
  *
  * To use the TMLCore interface library add the tmlCore.h, tmlGlobalDefines.h, tmlErrors.h and tmlCore.lib into your project and take
  * care to have the libvortex-1.1.dll, libaxl.dll and the tmlCore.dll in your path.
  */

#include "tmlCoreWrapperBase.h"
#include "tmlConnectionManageObjBase.h"
#include "tml-tls.h"
#include "tmlErrors.h"
#include "sidex.h"
#include <vortex_tls.h>
#include "unicode.h"

// return axl_true if we agree to accept the TLS negotiation
void tls_failure_handler (VortexConnection* connection, const char*error_message, axlPointer user_data)
{
  VortexCtx* ctx = vortex_connection_get_ctx(connection);
  TML_CORE_HANDLE coreHandle = (TML_CORE_HANDLE) vortex_ctx_get_data(ctx, "TML_CORE_HANDLE");

  void* pFailureCB = NULL;
  void* pFailureData = NULL;
  ((tmlCoreWrapperBase*) coreHandle)->getTlsFailureCB(&pFailureCB, &pFailureData);
  TML_CONNECTION_HANDLE connectionHandle = (TML_CONNECTION_HANDLE) vortex_ctx_get_data(ctx, "TML_CONNECTION_HANDLE");
  tmlConnectionManageObjBase* c = ((tmlConnectionManageObjBase*) connectionHandle);

  if (NULL != pFailureCB){
    SIDEX_VARIANT vErrorMessage = SIDEX_HANDLE_TYPE_NULL;
    if (NULL != error_message){
      sidex_Variant_New_String((char*)error_message, &vErrorMessage);
    }
    ((void(FUNC_C_DECL *)(TML_CONNECTION_HANDLE, SIDEX_VARIANT, void*))pFailureCB)(connectionHandle, vErrorMessage, pFailureData);
    if (SIDEX_HANDLE_TYPE_NULL != vErrorMessage){
      sidex_Variant_DecRef(vErrorMessage);
    }
  }
  return; 
}


// return axl_true if we agree to accept the TLS negotiation
axl_bool check_and_accept_tls_request (VortexConnection* connection, 
                                                       const char* serverName)
{
  // perform some special operations against the serverName
  // value or the received connection, return axl_false to deny the 
  // TLS request, or axl_true to allow it.

  axl_bool bRet = axl_true;

  VortexCtx* ctx = vortex_connection_get_ctx(connection);
  TML_CORE_HANDLE coreHandle = (TML_CORE_HANDLE) vortex_ctx_get_data(ctx, "TML_CORE_HANDLE");

  void* pAcceptCB = ((tmlCoreWrapperBase*) coreHandle)->getTlsAcceptCB();

  if (NULL != pAcceptCB){
    SIDEX_VARIANT vServerName;
    sidex_Variant_New_String((char*)serverName, &vServerName);
    TML_BOOL bRetVal = ((TML_BOOL(FUNC_C_DECL *)(SIDEX_VARIANT))pAcceptCB)(vServerName);
    sidex_Variant_DecRef(vServerName);
    if (TML_FALSE == bRetVal){
      bRet = axl_false;
    }
    else{
      bRet = axl_true;
    }
  }
  return bRet;  
}

// returns certificate file location
char* certificate_file_location (VortexConnection* connection, 
                                                       const char* serverName)
{
  VortexCtx* ctx = vortex_connection_get_ctx(connection);
  TML_CORE_HANDLE coreHandle = (TML_CORE_HANDLE) vortex_ctx_get_data(ctx, "TML_CORE_HANDLE");

  void* pCertReqCB = ((tmlCoreWrapperBase*) coreHandle)->getTlsCertReqCB();

  // perform some special operation to choose which 
  // certificate file to be used, then return it:
  char* fileLocation = NULL; 
  char* pathname = NULL;
  SIDEX_INT32 iLength;
  SIDEX_VARIANT vFileLocation = SIDEX_HANDLE_TYPE_NULL;
  if (NULL != pCertReqCB){
    SIDEX_VARIANT vServerName;
    sidex_Variant_New_String((char*)serverName, &vServerName);
    vFileLocation = ((SIDEX_VARIANT(FUNC_C_DECL *)(SIDEX_VARIANT))pCertReqCB)(vServerName);
    sidex_Variant_DecRef(vServerName);
  }
  if (SIDEX_HANDLE_TYPE_NULL != vFileLocation){
    sidex_Variant_As_String(vFileLocation, &fileLocation, &iLength);
    if (NULL != fileLocation && iLength){
      pathname = axl_strdup (fileLocation);
    }
    else{
      pathname = axl_strdup ("");
    }
    sidex_Variant_DecRef(vFileLocation);
  }
  return pathname;  
}

// returns private key file location
char* private_key_file_location (VortexConnection* connection, 
                                                       const char* serverName)
{
  VortexCtx* ctx = vortex_connection_get_ctx(connection);
  TML_CORE_HANDLE coreHandle = (TML_CORE_HANDLE) vortex_ctx_get_data(ctx, "TML_CORE_HANDLE");

  void* pPrivateKeyReqCB = ((tmlCoreWrapperBase*) coreHandle)->getTlsPrivateKeyReqCB();

  // perform some special operation to choose which 
  // private key file to be used, then return it:
   
  char* fileLocation = NULL; 
  char* pathname = NULL;
  SIDEX_INT32 iLength;
  SIDEX_VARIANT vFileLocation = SIDEX_HANDLE_TYPE_NULL;
  if (NULL != pPrivateKeyReqCB){
    SIDEX_VARIANT vServerName;
    sidex_Variant_New_String((char*)serverName, &vServerName);
    vFileLocation = ((SIDEX_VARIANT(FUNC_C_DECL *)(SIDEX_VARIANT))pPrivateKeyReqCB)(vServerName);
    sidex_Variant_DecRef(vServerName);
  }
  if (SIDEX_HANDLE_TYPE_NULL != vFileLocation){
    sidex_Variant_As_String(vFileLocation, &fileLocation, &iLength);
    if (NULL != fileLocation && iLength){
      pathname = axl_strdup (fileLocation);
    }
    else{
      pathname = axl_strdup ("");
    }
    sidex_Variant_DecRef(vFileLocation);
  }
  return pathname;  
}


/**
 * @brief   Allows to configure if the provided tml core will accept TLS incoming connections
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_AcceptNegotiation(TML_CORE_HANDLE coreHandle, 
                                                                     TML_ON_ACCEPT_TLS_REQUEST_CB_FUNC pAcceptCB,
                                                                     TML_ON_CERTIFICATE_FILE_LOCATION_CB_FUNC pCertReqCB,
                                                                     TML_ON_CERTIFICATE_PRIVATE_KEY_LOCATION_CB_FUNC pPrivateKeyReqCB,
                                                                     TML_BOOL* bAccept){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  TML_BOOL bRet = TML_FALSE;

  if (TML_HANDLE_TYPE_NULL != coreHandle){
    iRet = TML_SUCCESS;

    ((tmlCoreWrapperBase*) coreHandle)->setTlsAcceptCB((void*)pAcceptCB);
    ((tmlCoreWrapperBase*) coreHandle)->setTlsCertReqCB((void*)pCertReqCB);
    ((tmlCoreWrapperBase*) coreHandle)->setTlsPrivateKeyReqCB((void*)pPrivateKeyReqCB);

    VortexCtx* ctx = ((tmlCoreWrapperBase*) coreHandle)->getVortexCtx();
    vortex_ctx_set_data(ctx, "TML_CORE_HANDLE", (axlPointer)coreHandle);
    
    if (! vortex_tls_init (ctx)) {
      //printf ("Unable to activate TLS, Vortex is not prepared\n");
    }
    else{
      if (! vortex_tls_accept_negotiation    (ctx,     // context to configure
                                             (NULL != pAcceptCB) ? check_and_accept_tls_request : NULL,        // accept all TLS request received
                                             (NULL != pCertReqCB) ? certificate_file_location : NULL,          // use default certificate file
                                             (NULL != pPrivateKeyReqCB) ? private_key_file_location : NULL)){  // use default private key file
        //printf ("Cannot accept incoming TLS connections\n");
      }
      else{
        bRet = TML_TRUE;
      }
    }
  }
  *bAccept = bRet;
  return iRet;
}


/**
 * @brief   Allows to configure an error handler that will be called when a failure is found at SSL level or during the handshake with the particular function failing.
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_Set_OnError(TML_CORE_HANDLE coreHandle, 
                                                                     TML_ON_TLS_ERROR_CB_FUNC pErrorCB, void* pErrorData){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  TML_BOOL bRet = TML_FALSE;

  if (TML_HANDLE_TYPE_NULL != coreHandle){
    iRet = TML_SUCCESS;

    ((tmlCoreWrapperBase*) coreHandle)->setTlsFailureCB((void*)pErrorCB, (void*)pErrorData);

    VortexCtx* ctx = ((tmlCoreWrapperBase*) coreHandle)->getVortexCtx();
    vortex_ctx_set_data(ctx, "TML_CORE_HANDLE", (axlPointer)coreHandle);
    
    if (! vortex_tls_init (ctx)) {
      //printf ("Unable to activate TLS, Vortex is not prepared\n");
    }
    else{
      vortex_tls_set_failure_handler (ctx,     // context to configure
                                     (NULL != pErrorCB) ? tls_failure_handler : NULL,
                                     NULL);
    }
  }
  return iRet;
}


/**
 * @brief    Start tls negotiation for the requested connection
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_StartNegotiation (TML_CONNECTION_HANDLE connectionHandle, TML_CTSTR* serverName, TML_BOOL bAllowTlsFailures, TML_BOOL* bEncrypted);
/**
 * char* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_StartNegotiation_A (TML_CONNECTION_HANDLE connectionHandle, char* serverName, TML_BOOL bAllowTlsFailures, TML_BOOL* bEncrypted){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  TML_BOOL bEncryptedVal = TML_FALSE;

  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = TML_SUCCESS;
    TML_CORE_HANDLE  coreHandle = ((tmlConnectionManageObjBase*) connectionHandle)->getCoreHandle();
    VortexCtx* ctx = ((tmlCoreWrapperBase*) coreHandle)->getVortexCtx();
    VortexConnection* connection = ((tmlConnectionManageObjBase*) connectionHandle)->getVortexConnection();
    VortexConnection* retValue = connection;

    // initialize and check if current vortex library supports TLS

    if (! vortex_tls_init (ctx)) {
      ((tmlConnectionManageObjBase*) connectionHandle)->setTlsStatusMsg((char*)"Unable to activate TLS, Vortex is not prepared");
    }
    else{
      // start the TLS profile negotiation process
      VortexStatus status;
      char* status_message;
      VortexConnection* tls_connection = vortex_tls_start_negotiation_sync(connection, serverName, 
                                    &status, &status_message);

      ((tmlConnectionManageObjBase*) connectionHandle)->setTlsStatusMsg(status_message);

      switch (status) {
      case VortexOk:
          /*
          printf ("TLS negotiation OK! over the new connection %ld\n",
                    vortex_connection_get_id (tls_connection));
          printf ("TLS negotiation message: %s\n",
                    status_message);
          */
          // use the new connection reference provided by this function.
          retValue = tls_connection;
          bEncryptedVal = TML_TRUE;
          break;
      case VortexError: 
      default:
          // printf ("TLS negotiation have failed, message: %s\n", status_message);
          // ok, TLS process have failed but, do we have a connection
          // still working?
          if (bAllowTlsFailures && vortex_connection_is_ok (tls_connection, axl_false)) {
            // well we don't have TLS activated but the connection still works / use unencrypted
            retValue = tls_connection;
          } 
          else{
            // Negotiation fail:
            retValue = NULL;
          }
          break;
      }
    }
    ((tmlConnectionManageObjBase*) connectionHandle)->setVortexConnection(retValue);
    ((tmlConnectionManageObjBase*) connectionHandle)->setEncrypted(bEncryptedVal);
  }
  else{
    ((tmlConnectionManageObjBase*) connectionHandle)->setTlsStatusMsg((char*)"Invalid TML_CONNECTION_HANDLE");
  }

  *bEncrypted = bEncryptedVal;
  return iRet;
};
/**
 * wchar_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_StartNegotiation_X (TML_CONNECTION_HANDLE connectionHandle, wchar_t* serverName, TML_BOOL bAllowTlsFailures, TML_BOOL* bEncrypted){
  TML_INT32 iRet = TML_SUCCESS;

  TML_INT32 iLengthUtf8;
  char* utf8Str = NULL;
  try{
    if (NULL == serverName){
      iRet = tml_Tls_Connection_StartNegotiation_A (connectionHandle, utf8Str, bAllowTlsFailures, bEncrypted);
    }
    else{
      utf8Str = UTF32toUTF8((wchar_t*)serverName, &iLengthUtf8);
      if (NULL != utf8Str){
        iRet = tml_Tls_Connection_StartNegotiation_A (connectionHandle, utf8Str, bAllowTlsFailures, bEncrypted);

        delete[] utf8Str;
      }
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
};
/**
 * char16_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_StartNegotiation_W (TML_CONNECTION_HANDLE connectionHandle, char16_t* serverName, TML_BOOL bAllowTlsFailures, TML_BOOL* bEncrypted){
  TML_INT32 iRet = TML_SUCCESS;

  TML_INT32 iLengthUtf8;
  char* utf8Str = NULL;
  try{
    if (NULL == serverName){
      iRet = tml_Tls_Connection_StartNegotiation_A (connectionHandle, utf8Str, bAllowTlsFailures, bEncrypted);
    }
    else{
      char* utf8Str = UTF16toUTF8((wchar_t*)serverName, &iLengthUtf8);
      if (NULL != utf8Str){
        iRet = tml_Tls_Connection_StartNegotiation_A (connectionHandle, utf8Str, bAllowTlsFailures, bEncrypted);

        delete[] utf8Str;
      }
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
};

/**
 * @brief   Allows to activate TLS profile automatic negotiation for every connection created.
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_Set_AutoNegotiation (TML_CORE_HANDLE coreHandle, TML_BOOL bEnabled, TML_BOOL bAllowTlsFailures, TML_CTSTR* serverName);
/**
 * char* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_Set_AutoNegotiation_A (TML_CORE_HANDLE coreHandle, TML_BOOL bEnabled, TML_BOOL bAllowTlsFailures, char* serverName){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  TML_BOOL bEncryptedVal = TML_FALSE;

  if (TML_HANDLE_TYPE_NULL != coreHandle){
    iRet = TML_SUCCESS;
    VortexCtx* ctx = ((tmlCoreWrapperBase*) coreHandle)->getVortexCtx();
    if (! vortex_tls_init (ctx)) {
      //printf ("Unable to activate TLS, Vortex is not prepared");
    }
    else{
      vortex_tls_set_auto_tls(ctx, 
                              (TML_TRUE==bEnabled)? axl_true : axl_false, 
                              (TML_TRUE==bAllowTlsFailures)? axl_true : axl_false,
                              serverName);
    }
  }
  return iRet;
};
/**
 * wchar_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_Set_AutoNegotiation_X (TML_CORE_HANDLE coreHandle, TML_BOOL bEnabled, TML_BOOL bAllowTlsFailures, wchar_t* serverName){
  TML_INT32 iRet = TML_SUCCESS;

  TML_INT32 iLengthUtf8;
  char* utf8Str = NULL;
  try{
    if (NULL == serverName){
      iRet = tml_Tls_Core_Set_AutoNegotiation_A (coreHandle, bEnabled, bAllowTlsFailures, utf8Str);
    }
    else{
      utf8Str = UTF32toUTF8((wchar_t*)serverName, &iLengthUtf8);
      if (NULL != utf8Str){
        iRet = tml_Tls_Core_Set_AutoNegotiation_A (coreHandle, bEnabled, bAllowTlsFailures, utf8Str);

        delete[] utf8Str;
      }
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
};
/**
 * char16_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Core_Set_AutoNegotiation_W (TML_CORE_HANDLE coreHandle, TML_BOOL bEnabled, TML_BOOL bAllowTlsFailures, char16_t* serverName){
  TML_INT32 iRet = TML_SUCCESS;

  TML_INT32 iLengthUtf8;
  char* utf8Str = NULL;
  try{
    if (NULL == serverName){
      iRet = tml_Tls_Core_Set_AutoNegotiation_A (coreHandle, bEnabled, bAllowTlsFailures, utf8Str);
    }
    else{
      char* utf8Str = UTF16toUTF8((wchar_t*)serverName, &iLengthUtf8);
      if (NULL != utf8Str){
        iRet = tml_Tls_Core_Set_AutoNegotiation_A (coreHandle, bEnabled, bAllowTlsFailures, utf8Str);

        delete[] utf8Str;
      }
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
};


/**
 * @brief    Allows to verify peer certificate after successfully establish TLS session. 
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_VerifyCert (TML_CONNECTION_HANDLE connectionHandle, TML_BOOL* bVerifyOk) 
{
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  axl_bool bVerified = axl_false;

  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = TML_SUCCESS;
    VortexConnection* connection = ((tmlConnectionManageObjBase*) connectionHandle)->getVortexConnection();
    bVerified = vortex_tls_verify_cert(connection);
  }

  *bVerifyOk = (axl_true == bVerified);
  return iRet;
}


/**
 * @brief    Is encrption enabled for the requested connection
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Valid (TML_CONNECTION_HANDLE connectionHandle, TML_BOOL* bEncrypted){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  TML_BOOL bEncryptedVal = TML_FALSE;

  // TODO: 
  // The connectionHandle->isEncrpyted() flag cannot be set using tml_Tls_Core_SetAuto_Negation().
  // It can only be configured in the tml_Tls_Connection_StartNegotiation().
  // tml_Tls_Core_SetAuto_Negation has no handler to get this information.
  // 
  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = TML_SUCCESS;
    *bEncrypted = ((tmlConnectionManageObjBase*) connectionHandle)->isEncrpyted();
  }
  return iRet;
}


/**
 * @brief    Get the encrption status message
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage (TML_CONNECTION_HANDLE connectionHandle, TML_CTSTR** statusMsg);
/**
 * char* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage_A (TML_CONNECTION_HANDLE connectionHandle, char** statusMsg){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  TML_BOOL bEncryptedVal = TML_FALSE;

  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = ((tmlConnectionManageObjBase*) connectionHandle)->getTlsStatusMsg_A(statusMsg);
    if(TML_SUCCESS != iRet){
      iRet = TML_ERR_INFORMATION_UNDEFINED;
    }
  }
  return iRet;
};
/**
 * wchar_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage_X (TML_CONNECTION_HANDLE connectionHandle, wchar_t** statusMsg){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  TML_BOOL bEncryptedVal = TML_FALSE;

  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = ((tmlConnectionManageObjBase*) connectionHandle)->getTlsStatusMsg_X(statusMsg);
    if(TML_SUCCESS != iRet){
      iRet = TML_ERR_INFORMATION_UNDEFINED;
    }
  }
  return iRet;
};
/**
 * char16_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Encryption_Get_StatusMessage_W (TML_CONNECTION_HANDLE connectionHandle, char16_t** statusMsg){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;
  TML_BOOL bEncryptedVal = TML_FALSE;

  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = ((tmlConnectionManageObjBase*) connectionHandle)->getTlsStatusMsg_W(statusMsg);
    if(TML_SUCCESS != iRet){
      iRet = TML_ERR_INFORMATION_UNDEFINED;
    }
  }
  return iRet;
};


/**
 * @brief    Allows to create a digest from the provided string
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Get_Digest (TML_CTSTR* string, TmlTlsDigestMethod method, TML_CTSTR** sDigest);
/**
 * char* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Get_Digest_A (char* string, TmlTlsDigestMethod method, char** sDigest){
  TML_INT32 iRet = TML_SUCCESS;

  char* sAxlRet = vortex_tls_get_digest ((VortexDigestMethod)method, string); 

  int iLength = strlen(sAxlRet);
  char* sRet = new char[iLength+1];

#if defined (LINUX) || defined (MINGW_BUILD)
  strncpy(sRet, sAxlRet, iLength);
#else
  strncpy_s(sRet, iLength+1, sAxlRet, iLength);
#endif
  sRet[iLength] = '\0';

  *sDigest = sRet;
  axl_free(sAxlRet);
  return iRet;
};
/**
 * wchar_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Get_Digest_X (wchar_t* string, TmlTlsDigestMethod method, wchar_t** sDigest){
  TML_INT32 iRet = TML_SUCCESS;

  TML_INT32 iLengthUtf8;
  TML_INT32 iLengthUtf32;
  try{
    char* utf8Str = UTF32toUTF8((wchar_t*)string, &iLengthUtf8);
    if (NULL != utf8Str){
      char* sRet = vortex_tls_get_digest ((VortexDigestMethod)method, utf8Str); 
      wchar_t* utf32Str = UTF8toUTF32(sRet, &iLengthUtf32);
      if (NULL != utf32Str){
        *sDigest = utf32Str;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      delete[] utf8Str;
      axl_free(sRet);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
};
/**
 * char16_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Get_Digest_W (char16_t* string, TmlTlsDigestMethod method, char16_t** sDigest){
  TML_INT32 iRet = TML_SUCCESS;

  TML_INT32 iLengthUtf8;
  TML_INT32 iLengthUtf16;
  try{
    char* utf8Str = UTF16toUTF8((wchar_t*)string, &iLengthUtf8);
    if (NULL != utf8Str){
      char* sRet = vortex_tls_get_digest ((VortexDigestMethod)method, utf8Str); 
      char16_t* utf16Str = (char16_t*)UTF8toUTF16(sRet, &iLengthUtf16);
      if (NULL != utf16Str){
        *sDigest = utf16Str;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      delete[] utf8Str;
      axl_free(sRet);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
};




/**
 * @brief    Allows to create a digest from the provided string
 */
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_PeerSSLDigest (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, TML_CTSTR** sDigest);
/**
 * char* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_PeerSSLDigest_A (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, char** sDigest){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;

  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = TML_SUCCESS;
    VortexConnection* connection = ((tmlConnectionManageObjBase*) connectionHandle)->getVortexConnection();

    char* sAxlRet = vortex_tls_get_peer_ssl_digest (connection, (VortexDigestMethod)method); 


    int iLength = strlen(sAxlRet);
    char* sRet = new char[iLength+1];

  #if defined (LINUX) || defined (MINGW_BUILD)
    strncpy(sRet, sAxlRet, iLength);
  #else
    strncpy_s(sRet, iLength+1, sAxlRet, iLength);
  #endif
    sRet[iLength] = '\0';

    *sDigest = sRet;
    axl_free(sAxlRet);
  }
  return iRet;
};
/**
 * wchar_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_PeerSSLDigest_X (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, wchar_t** sDigest){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;

  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = TML_SUCCESS;

    TML_INT32 iLengthUtf32;
    try{
      VortexConnection* connection = ((tmlConnectionManageObjBase*) connectionHandle)->getVortexConnection();

      char* sRet = vortex_tls_get_peer_ssl_digest (connection, (VortexDigestMethod)method); 
      wchar_t* utf32Str = UTF8toUTF32(sRet, &iLengthUtf32);
      if (NULL != utf32Str){
        *sDigest = utf32Str;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      axl_free(sRet);
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
};
/**
 * char16_t* API
**/
TLS_CORE_API TML_INT32 DLL_CALL_CONV tml_Tls_Connection_Get_PeerSSLDigest_W (TML_CONNECTION_HANDLE connectionHandle, TmlTlsDigestMethod method, char16_t** sDigest){
  TML_INT32 iRet = TML_ERR_MISSING_OBJ;

  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    iRet = TML_SUCCESS;

    TML_INT32 iLengthUtf16;
    try{
      VortexConnection* connection = ((tmlConnectionManageObjBase*) connectionHandle)->getVortexConnection();

      char* sRet = vortex_tls_get_peer_ssl_digest (connection, (VortexDigestMethod)method); 
      char16_t* utf16Str = (char16_t*)UTF8toUTF16(sRet, &iLengthUtf16);
      if (NULL != utf16Str){
        *sDigest = utf16Str;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      axl_free(sRet);
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
};


#ifdef LINUX
void __attribute__ ((constructor)) my_load(void);
void __attribute__ ((destructor)) my_unload(void);

// Called when the library is loaded and before dlopen() returns
void my_load(void)
{
    // BEIM LADEN DER DLL:
    // Add initialization code
}

// Called when the library is unloaded and before dlclose()
// returns
void my_unload(void)
{
    // BEIM FREIGEBEN DER DLL:
    // Add clean-up code
}
#else
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{

    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
         break;

        case DLL_THREAD_ATTACH:
         break;

        case DLL_THREAD_DETACH:
         break;

        case DLL_PROCESS_DETACH:
         break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#endif