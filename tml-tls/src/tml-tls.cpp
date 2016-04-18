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
#include <vortex_tls.h>

// return axl_true if we agree to accept the TLS negotiation
axl_bool      check_and_accept_tls_request (VortexConnection* connection, 
                                                       const char* serverName)
{
    // perform some special operations against the serverName
    // value or the received connection, return axl_false to deny the 
    // TLS request, or axl_true to allow it.
    
    return axl_true;  
}

char * certificate_file_location (VortexConnection* connection, 
                                                       const char* serverName)
{
	VortexCtx* ctx = vortex_connection_get_ctx (connection);
  // perform some special operation to choose which 
  // certificate file to be used, then return it:
   
  return vortex_support_find_data_file (ctx, "test-certificate.pem"); 
}

char * private_key_file_location (VortexConnection* connection, 
                                                       const char* serverName)
{
	VortexCtx* ctx = vortex_connection_get_ctx (connection);
  // perform some special operation to choose which 
  // private key file to be used, then return it:
   
  return vortex_support_find_data_file (ctx, "test-private-key.pem"); 
}


/**
 * @brief    Accept tls negotiation
 */
TLS_CORE_API TML_BOOL DLL_CALL_CONV tml_tls_accept_negotiation(TML_CORE_HANDLE coreHandle){
  TML_BOOL bRet = TML_FALSE;
  VortexCtx* ctx = ((tmlCoreWrapperBase*) coreHandle)->getVortexCtx();

  if (! vortex_tls_init (ctx)) {
    printf ("Unable to activate TLS, Vortex is not prepared\n");
    // TODO: fehler bekannt geben.
  }
  else{
    vortex_support_add_search_path (ctx, "C:\\ssl");    // activate TLS profile support using defaults

    if (! vortex_tls_accept_negotiation    (ctx,     // context to configure
                                           check_and_accept_tls_request,    // accept all TLS request received
                                           certificate_file_location,    // use default certificate file
                                           private_key_file_location)){  // use default private key file
      printf ("Cannot accept incoming TLS connections\n");
    }
    else{
      bRet = TML_TRUE;
    }
  }
  return bRet;
}


TML_BOOL tml_tls_start_negotiation (TML_CONNECTION_HANDLE connectionHandle) 
{
  TML_CORE_HANDLE  coreHandle = ((tmlConnectionManageObjBase*) connectionHandle)->getCoreHandle();
  VortexCtx* ctx = ((tmlCoreWrapperBase*) coreHandle)->getVortexCtx();
  VortexConnection* connection = ((tmlConnectionManageObjBase*) connectionHandle)->getVortexConnection();
  VortexConnection* retValue = connection;
  TML_BOOL bEncrypted = TML_FALSE;

  // initialize and check if current vortex library supports TLS

  if (! vortex_tls_init (ctx)) {
      printf ("Unable to activate TLS, Vortex is not prepared\n");
  }
  else{
    // start the TLS profile negotiation process
    VortexStatus status;
    char* status_message;
    VortexConnection* tls_connection = vortex_tls_start_negotiation_sync (connection, NULL, 
                                  &status, &status_message);
    switch (status) {
    case VortexOk:
        printf ("TLS negotiation OK! over the new connection %ld\n",
                  vortex_connection_get_id (connection));
        // use the new connection reference provided by this function.
        retValue = tls_connection;
        bEncrypted = TML_TRUE;
        break;
    case VortexError: 
        printf ("TLS negotiation have failed, message: %s\n",
                  status_message);
        // ok, TLS process have failed but, do we have a connection
        // still working?
        if (vortex_connection_is_ok (tls_connection, axl_false)) {
          // well we don't have TLS activated but the connection still works
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
  return bEncrypted;
}



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
