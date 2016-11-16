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

#include <stdio.h>
#ifdef LINUX
  #include <wchar.h> 
#endif // LINUX
#include <string.h>
#include <sidex.h>
#include <tmlCore.h>

#ifdef LINUX
  #include <unistd.h>
#else // LINUX
  #include <windows.h>
#endif // LINUX

#ifdef BUILD_TLS
#include "tml-tls.h"
#endif // BUILD_TLS

#ifdef TML_UNICODE
  #define IO_PROFILE                    (wchar_t*) L"http://wobe-team.com/profiles/plain_profile"
  #define IO_PROFILE2                   (wchar_t*) L"http://wobe-team.com/profiles/plain_profile2"
  #define LISTENER_NETWORK_INTERFACE_IP (wchar_t*) L"0.0.0.0"
  #define IO_PORT                       (wchar_t*) L"44100"
  #define DESTINATION_HOST_IP           (wchar_t*) L"127.0.0.1"
  #define DESTINATION_NETWORK_BINDING   (wchar_t*) L"127.0.0.1:44100"

  #define PARAMS                        (wchar_t*) L"PARAMS"
  #define SLEEP_MS                      (wchar_t*) L"SLEEP_MS"
  #define TLS_CRT_FILE                  (wchar_t*) L"TmlTestTls.crt"
  #define TLS_KEY_FILE                  (wchar_t*) L"TmlTestTls.key"

  const wchar_t* any_data1 = L"Any Data 1";
  const wchar_t* any_data2 = L"Any Data 2";
#else // TML_UNICODE
  #define IO_PROFILE                    (char*) "http://wobe-team.com/profiles/plain_profile"
  #define IO_PROFILE2                   (char*) "http://wobe-team.com/profiles/plain_profile2"
  #define LISTENER_NETWORK_INTERFACE_IP (char*) "0.0.0.0"
  #define IO_PORT                       (char*) "44100"
  #define DESTINATION_HOST_IP           (char*) "127.0.0.1"
  #define DESTINATION_NETWORK_BINDING   (char*) "127.0.0.1:44100"

  #define PARAMS                        (char*) "PARAMS"
  #define SLEEP_MS                      (char*) "SLEEP_MS"
  #define TLS_CRT_FILE                  (char*) "TmlTestTls.crt"
  #define TLS_KEY_FILE                  (char*) "TmlTestTls.key"

  const char* any_data1 = "Any Data 1";
  const char* any_data2 = "Any Data 2";
#endif // TML_UNICODE

TML_CORE_HANDLE m_coreHandle = TML_HANDLE_TYPE_NULL;
TML_CONNECTION_HANDLE m_connectionHandle = TML_HANDLE_TYPE_NULL;

/**
 * Helper method, loop until the console input of "exit"
 */
void waitUntilExitInput(char* name){
  char sInput[256];
  printf ("\nThe \"%s demo\" is running - enter \"exit\"<enter> to close the demo\n", name);
  do{
    int i = scanf("%s", sInput);
  }while (strcmp(sInput, "exit") != 0);
}

/**
 * Helper method, wait in milliseconds
 */
void wait(SIDEX_INT64 milliseconds){
    #ifdef LINUX
       printf ("  Sleep %lld milliseconds\n", (long long)milliseconds);
      usleep(milliseconds * 1000);
      printf ("  Sleep %lld milliseconds done\n", (long long)milliseconds);
    #else
      printf ("  Sleep %ld milliseconds\n", milliseconds);
      Sleep ((DWORD)milliseconds);
      printf ("  Sleep %ld milliseconds done\n", milliseconds);
    #endif
}

/**
 * Callback method invoked in case of incomming CMD- Code 4711
 */
void FUNC_C_DECL callbackMethodCmd4711(TML_COMMAND_HANDLE cmdMsg, TML_POINTER data){
  printf ("  envoke callbackMethodCmd4711\n");

  TML_INT32 iErr = TML_SUCCESS;
  TML_INT64 iSleep = 0;
  /////////////////////////////////////////////////////////////////////////
  // Set sleep parameter
  SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
  iErr = tml_Cmd_Acquire_Sidex_Handle(cmdMsg, &sHandle);
  if (TML_SUCCESS == iErr)
    iErr = sidex_Integer_Read(sHandle, PARAMS, SLEEP_MS, &iSleep);
  if (TML_SUCCESS == iErr)
    iErr = sidex_Integer_Write(sHandle, PARAMS, SLEEP_MS, iSleep / 2);
  if (TML_SUCCESS == iErr)
    iErr = tml_Cmd_Release_Sidex_Handle(cmdMsg);
  wait (iSleep);
}


/**
 * Callback method invoked in case of incomming CMD- Code 4711
 */
void FUNC_C_DECL callbackMethodCmd4711P2(TML_COMMAND_HANDLE cmdMsg, TML_POINTER data){
  printf ("  envoke callbackMethodCmd4711 (PROFILE2)\n");

  TML_INT32 iErr = TML_SUCCESS;
  TML_INT64 iSleep = 0;
  /////////////////////////////////////////////////////////////////////////
  // Set sleep parameter
  SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
  iErr = tml_Cmd_Acquire_Sidex_Handle(cmdMsg, &sHandle);
  if (TML_SUCCESS == iErr)
    iErr = sidex_Integer_Read(sHandle, PARAMS, SLEEP_MS, &iSleep);
  if (TML_SUCCESS == iErr)
    iErr = sidex_Integer_Write(sHandle, PARAMS, SLEEP_MS, iSleep / 2);
  if (TML_SUCCESS == iErr)
    iErr = tml_Cmd_Release_Sidex_Handle(cmdMsg);
  wait (iSleep);
}


/**
 * Callback method invoked in case of command ready
 */
void FUNC_C_DECL callbackMethodCommandReady(TML_COMMAND_HANDLE cmdMsg, TML_POINTER data){
  printf ("  callbackMethodCommandReady\n");

  TML_INT32 iErr = TML_SUCCESS;
  ///////////////////////////////////////////////////////////////////////
  // Free the instance of Command / message HANDLE:
  if (TML_HANDLE_TYPE_NULL != cmdMsg)
    tml_Cmd_Free(&cmdMsg);
}

/**
 * Listener initialisation
 */
TML_INT32 initListener(TML_CORE_HANDLE* listenerCore)
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to receive commands / messages 
  TML_CORE_HANDLE coreHandle = TML_HANDLE_TYPE_NULL;
  iErr = tml_Core_Open(&coreHandle, 0);
  /////////////////////////////////////////////////////////////////////////
  // Declare the IP of the listener network device
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Set_ListenerIP (coreHandle, LISTENER_NETWORK_INTERFACE_IP);
  /////////////////////////////////////////////////////////////////////////
  // Declare the listener port
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Set_ListenerPort (coreHandle, IO_PORT);
  /////////////////////////////////////////////////////////////////////////
  // Register profile (unique profile name) for command dispatch action
  if (TML_SUCCESS == iErr)
    iErr = tml_Profile_Register (coreHandle, IO_PROFILE);
  /////////////////////////////////////////////////////////////////////////
  // Register profile (unique profile name) for command dispatch action
  if (TML_SUCCESS == iErr)
    iErr = tml_Profile_Register (coreHandle, IO_PROFILE2);
  /////////////////////////////////////////////////////////////////////////
  // Register a callbackmethod for command dispatching of command 4711
  if (TML_SUCCESS == iErr)
    iErr = tml_Profile_Register_Cmd (coreHandle, IO_PROFILE, (TML_COMMAND_ID_TYPE) 4711, callbackMethodCmd4711, TML_HANDLE_TYPE_NULL);
  /////////////////////////////////////////////////////////////////////////
  // Register a callbackmethod for command dispatching of command 4711
  if (TML_SUCCESS == iErr)
    iErr = tml_Profile_Register_Cmd (coreHandle, IO_PROFILE2, (TML_COMMAND_ID_TYPE) 4711, callbackMethodCmd4711P2, TML_HANDLE_TYPE_NULL);
  /////////////////////////////////////////////////////////////////////////
  // Enable listener
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Set_ListenerEnabled (coreHandle, TML_TRUE);
  ///////////////////////////////////////////////////////////////////////
  // Returnvalue
  if (TML_SUCCESS == iErr)
    *listenerCore = coreHandle;
  return iErr;
}

/**
 * A simple listener example 
 */
bool listenerTest01()
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to receive commands / messages 
  TML_CORE_HANDLE coreHandle = TML_HANDLE_TYPE_NULL;

  iErr = initListener(&coreHandle);
  /////////////////////////////////////////////////////////////////////////
  // loop until the input of "exit"
  if (TML_SUCCESS == iErr)
    waitUntilExitInput((char*)"listenerTest01");
  ///////////////////////////////////////////////////////////////////////
  // Free the instance of TMLCore:
  if (TML_HANDLE_TYPE_NULL != coreHandle)
    tml_Core_Close(&coreHandle);
  if (TML_SUCCESS != iErr)
    printf ("listenerTest01 / error happened - Code = %d\n", iErr);
  return true;
}

/**
 * Command initialisation
 */
TML_INT32 createCmd4711(TML_COMMAND_HANDLE* cmd, bool bRegisterReady)
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  /////////////////////////////////////////////////////////////////////////
  // Command / message HANDLE (internal represented by SIDEX data)
  TML_COMMAND_HANDLE cmdMsg = TML_HANDLE_TYPE_NULL;
  iErr = tml_Cmd_Create (&cmdMsg);
  if (TML_SUCCESS == iErr)
    iErr = tml_Cmd_Header_SetCommand (cmdMsg, (TML_COMMAND_ID_TYPE)4711);
  /////////////////////////////////////////////////////////////////////////
  // Set sleep parameter
  SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
  if (TML_SUCCESS == iErr)
    iErr = tml_Cmd_Acquire_Sidex_Handle(cmdMsg, &sHandle);
  if (TML_SUCCESS == iErr)
    sidex_Integer_Write(sHandle, PARAMS, SLEEP_MS, 7000);
  if (TML_SUCCESS == iErr)
    iErr = tml_Cmd_Release_Sidex_Handle(cmdMsg);
  if (TML_SUCCESS == iErr && bRegisterReady)
      iErr = tml_Cmd_Register_CommandReady (cmdMsg, callbackMethodCommandReady, TML_HANDLE_TYPE_NULL);
  ///////////////////////////////////////////////////////////////////////
  // Returnvalue
  if (TML_SUCCESS == iErr)
    *cmd = cmdMsg;
  return iErr;
}



/**
 * @brief   Class callback method that will be called by establishment of a connection
 */
void connectionEstablishHandler(TML_CONNECTION_HANDLE connectionHandle, TML_POINTER pCBData)
{
  TML_INT32 iRet;
  TML_BOOL bConnected;
  iRet = tml_Connection_Validate(connectionHandle, TML_FALSE, &bConnected);
  printf ("==========================================\n");
  printf ("connectionEstablishHandler / %d / %s\n", bConnected, (char*)pCBData);
  printf ("==========================================\n\n");
}


/**
 * @brief   Class callback method that will be called by establishment of a connection
 */
void connectionCloseHandler(TML_CONNECTION_HANDLE connectionHandle, TML_POINTER pCBData)
{

  TML_INT32 iRet;
  TML_BOOL bConnected;
  iRet = tml_Connection_Validate(connectionHandle, TML_FALSE, &bConnected);
  printf ("==========================================\n");
  printf ("connectionCloseHandler / %d / %s\n", bConnected, (char*)pCBData);
  printf ("==========================================\n\n");
}


/**
 * A simple sender example 
 */
bool senderTest01()
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  TML_COMMAND_HANDLE cmdMsg  = TML_HANDLE_TYPE_NULL;
  TML_CORE_HANDLE coreHandle = TML_HANDLE_TYPE_NULL;

  /////////////////////////////////////////////////////////////////////////
  // Command creation:
  iErr = createCmd4711(&cmdMsg, false);
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Open(&coreHandle, 0);

  /////////////////////////////////////////////////////////////////////////
  // send command / message 
  if (TML_SUCCESS == iErr){
#ifdef TML_UNICODE
    fwprintf (stderr, L"Sending command 4711 to %ls -->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
    printf ("Sending command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE


    iErr = tml_Send_SyncMessage(coreHandle, cmdMsg, IO_PROFILE, DESTINATION_HOST_IP, IO_PORT, 10000);
  }

  ///////////////////////////////////////////////////////////////////////
  // Free the instance of Command / message HANDLE:
  if (TML_HANDLE_TYPE_NULL != cmdMsg)
    tml_Cmd_Free(&cmdMsg);
  ///////////////////////////////////////////////////////////////////////
  // Free the instance of TMLCore:
  if (TML_HANDLE_TYPE_NULL != coreHandle)
    tml_Core_Close(&coreHandle);
  ///////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (TML_SUCCESS != iErr)
    printf ("senderTest01 / error happened - Code = %d\n", iErr);
  return true;
}

/**
 * A simple sender example 
 */
bool senderTest02()
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  TML_COMMAND_HANDLE cmdMsg  = TML_HANDLE_TYPE_NULL;

  /////////////////////////////////////////////////////////////////////////
  // Command creation:
  iErr = createCmd4711(&cmdMsg, false);
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  if (TML_SUCCESS == iErr){
    if (TML_HANDLE_TYPE_NULL == m_coreHandle){
      iErr = tml_Core_Open(&m_coreHandle, 0);
    }
  }

  tml_Core_Set_OnConnect(m_coreHandle, &connectionEstablishHandler, (TML_POINTER)any_data1);
  tml_Core_Set_OnDisconnect(m_coreHandle, &connectionCloseHandler, (TML_POINTER)any_data2);

  /////////////////////////////////////////////////////////////////////////
  // send command / message 
  if (TML_SUCCESS == iErr){
#ifdef TML_UNICODE
    fwprintf (stderr, L"Sending 4711 commands to %ls (different profiles)-->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
    printf ("Sending 4711 commands to %s  (different profiles)-->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE

    if (TML_HANDLE_TYPE_NULL == m_connectionHandle){
      iErr = tml_Core_Connect(m_coreHandle, DESTINATION_NETWORK_BINDING, &m_connectionHandle);
    }
    if (TML_SUCCESS == iErr){
      iErr = tml_Connection_SendSync(m_connectionHandle, IO_PROFILE, cmdMsg, 10000);
	    if (TML_SUCCESS == iErr){
	      iErr = tml_Connection_SendSync(m_connectionHandle, IO_PROFILE2, cmdMsg, 10000);
	    }
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Free the instance of Command / message HANDLE:
  if (TML_HANDLE_TYPE_NULL != cmdMsg)
    tml_Cmd_Free(&cmdMsg);
  ///////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (TML_SUCCESS != iErr)
    printf ("senderTest02 / error happened - Code = %d\n", iErr);
  return false;
}

/**
 * Close core if it exists
 */
void closePossibleExistingCoreHandle(){
  if (TML_HANDLE_TYPE_NULL != m_coreHandle){
    tml_Core_Close(&m_coreHandle);
  }
}

/**
 * A simple sync send / listen example 
 */
bool sendAndListenTest01()
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to receive commands / messages 
  TML_CORE_HANDLE coreListenerHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  TML_CORE_HANDLE coreSenderHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_COMMAND_HANDLE Command / message HANDLE (internal represented by SIDEX data)
  TML_COMMAND_HANDLE cmdMsg  = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // Listener initialisation:
  iErr = initListener(&coreListenerHandle);
  /////////////////////////////////////////////////////////////////////////
  // Command / message creation:
  if (TML_SUCCESS == iErr)
    iErr = createCmd4711(&cmdMsg, false);
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Open(&coreSenderHandle, 0);

  /////////////////////////////////////////////////////////////////////////
  // send a loop of command / messages
  for (int i = 0; i < 10 && TML_SUCCESS == iErr; ++i){
#ifdef TML_UNICODE
    fwprintf (stderr, L"Sending sync command 4711 to %ls -->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
    printf ("Sending sync command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE
    iErr = tml_Send_SyncMessage(coreSenderHandle, cmdMsg, IO_PROFILE, DESTINATION_HOST_IP, IO_PORT, 10000);
    if (TML_SUCCESS == iErr){
#ifdef TML_UNICODE
      fwprintf (stderr, L"Sending sync command 4711 (PROFILE2) to %ls -->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
      printf ("Sending sync command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE
    iErr = tml_Send_SyncMessage(coreSenderHandle, cmdMsg, IO_PROFILE2, DESTINATION_HOST_IP, IO_PORT, 10000);
      }
  }
  ///////////////////////////////////////////////////////////////////////
  // Free the instance of Command / message HANDLE:
  if (TML_HANDLE_TYPE_NULL != cmdMsg)
    tml_Cmd_Free(&cmdMsg);
  ///////////////////////////////////////////////////////////////////////
  // Free the instances of TMLCore:
  if (TML_HANDLE_TYPE_NULL != coreSenderHandle)
    tml_Core_Close(&coreSenderHandle);
  if (TML_HANDLE_TYPE_NULL != coreListenerHandle)
    tml_Core_Close(&coreListenerHandle);

  ///////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (TML_SUCCESS != iErr)
    printf ("sendAndListenTest01 / error happened - Code = %d\n", iErr);
  return true;
}

/**
 * A simple async send / listen example 
 */
bool sendAndListenTest02()
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to receive commands / messages 
  TML_CORE_HANDLE coreListenerHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  TML_CORE_HANDLE coreSenderHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_COMMAND_HANDLE Command / message HANDLE (internal represented by SIDEX data)
  TML_COMMAND_HANDLE cmdMsg  = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // Listener initialisation:
  iErr = initListener(&coreListenerHandle);
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Open(&coreSenderHandle, 0);

  /////////////////////////////////////////////////////////////////////////
  // send a loop of command / messages
  for (int i = 10; i > 0 && TML_SUCCESS == iErr; --i){
    /////////////////////////////////////////////////////////////////////////
    // Command / message creation:
    if (TML_SUCCESS == iErr)
      iErr = createCmd4711(&cmdMsg, true);
    if (TML_SUCCESS == iErr){
#ifdef TML_UNICODE
      fwprintf (stderr, L"Sending async command 4711 to %ls -->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
      printf ("Sending async command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE
      iErr = tml_Send_AsyncMessage(coreSenderHandle, cmdMsg, IO_PROFILE, DESTINATION_HOST_IP, IO_PORT, 500);

      if (TML_SUCCESS == iErr)
        iErr = createCmd4711(&cmdMsg, true);
      if (TML_SUCCESS == iErr){
  #ifdef TML_UNICODE
        fwprintf (stderr, L"Sending async command 4711 (PROFILE2) to %ls -->\n", DESTINATION_HOST_IP);
  #else// TML_UNICODE
        printf ("Sending async command 4711 to %s -->\n", DESTINATION_HOST_IP);
  #endif// TML_UNICODE
        iErr = tml_Send_AsyncMessage(coreSenderHandle, cmdMsg, IO_PROFILE2, DESTINATION_HOST_IP, IO_PORT, 500);
      }
    }
  }
  ///////////////////////////////////////////////////////////////////////
  // Wait for a while to be shure to be synchron with the asynchron callback
  // FOR THIS TEST to avoid a broken communication by closing the core handles
  wait (10000);
  ///////////////////////////////////////////////////////////////////////
  // Free the instances of TMLCore:
  if (TML_HANDLE_TYPE_NULL != coreSenderHandle)
    tml_Core_Close(&coreSenderHandle);
  if (TML_HANDLE_TYPE_NULL != coreListenerHandle)
    tml_Core_Close(&coreListenerHandle);

  ///////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (TML_SUCCESS != iErr)
    printf ("sendAndListenTest02 / error happened - Code = %d\n", iErr);
  return true;
}

/**
 * A simple sync send / listen example with connection manange object
 */
bool sendAndListenTest03()
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to receive commands / messages 
  TML_CORE_HANDLE coreListenerHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  TML_CORE_HANDLE coreSenderHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_COMMAND_HANDLE Command / message HANDLE (internal represented by SIDEX data)
  TML_COMMAND_HANDLE cmdMsg  = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // Listener initialisation:
  iErr = initListener(&coreListenerHandle);
  /////////////////////////////////////////////////////////////////////////
  // Command / message creation:
  if (TML_SUCCESS == iErr)
    iErr = createCmd4711(&cmdMsg, false);
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Open(&coreSenderHandle, 0);

  TML_CONNECTION_HANDLE connectionHandle = TML_HANDLE_TYPE_NULL;
  iErr = tml_Core_Connect(coreSenderHandle, DESTINATION_NETWORK_BINDING, &connectionHandle);

  /////////////////////////////////////////////////////////////////////////
  // send a loop of command / messages
  for (int i = 0; i < 10 && TML_SUCCESS == iErr; ++i){
#ifdef TML_UNICODE
    fwprintf (stderr, L"Sending sync command 4711 to %ls -->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
    printf ("Sending sync command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE
    iErr = tml_Connection_SendSync(connectionHandle, IO_PROFILE, cmdMsg, 10000);
    if (TML_SUCCESS == iErr){
#ifdef TML_UNICODE
      fwprintf (stderr, L"Sending sync command 4711 (PROFILE2) to %ls -->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
      printf ("Sending sync command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE
    iErr = tml_Connection_SendSync(connectionHandle, IO_PROFILE2, cmdMsg, 10000);
      }
  }
  if (TML_HANDLE_TYPE_NULL != connectionHandle){
    tml_Connection_Close(&connectionHandle);
  }
  ///////////////////////////////////////////////////////////////////////
  // Free the instance of Command / message HANDLE:
  if (TML_HANDLE_TYPE_NULL != cmdMsg)
    tml_Cmd_Free(&cmdMsg);
  ///////////////////////////////////////////////////////////////////////
  // Free the instances of TMLCore:
  if (TML_HANDLE_TYPE_NULL != coreSenderHandle)
    tml_Core_Close(&coreSenderHandle);
  if (TML_HANDLE_TYPE_NULL != coreListenerHandle)
    tml_Core_Close(&coreListenerHandle);

  ///////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (TML_SUCCESS != iErr)
    printf ("sendAndListenTest03 / error happened - Code = %d\n", iErr);
  return true;
}

/**
 * A simple async send / listen example with connection manange object
 */
bool sendAndListenTest04()
{
  TML_INT32 iErr = TML_SUCCESS; // API return value
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to receive commands / messages 
  TML_CORE_HANDLE coreListenerHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  TML_CORE_HANDLE coreSenderHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_COMMAND_HANDLE Command / message HANDLE (internal represented by SIDEX data)
  TML_COMMAND_HANDLE cmdMsg  = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // Listener initialisation:
  iErr = initListener(&coreListenerHandle);
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  if (TML_SUCCESS == iErr)
    iErr = tml_Core_Open(&coreSenderHandle, 0);

  TML_CONNECTION_HANDLE connectionHandle = TML_HANDLE_TYPE_NULL;
  iErr = tml_Core_Connect(coreSenderHandle, DESTINATION_NETWORK_BINDING, &connectionHandle);

  /////////////////////////////////////////////////////////////////////////
  // send a loop of command / messages
  for (int i = 10; i > 0 && TML_SUCCESS == iErr; --i){
    /////////////////////////////////////////////////////////////////////////
    // Command / message creation:
    if (TML_SUCCESS == iErr)
      iErr = createCmd4711(&cmdMsg, true);
    if (TML_SUCCESS == iErr){
#ifdef TML_UNICODE
      fwprintf (stderr, L"Sending async command 4711 to %ls -->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
      printf ("Sending async command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE
      iErr = tml_Connection_SendAsync(connectionHandle, IO_PROFILE, cmdMsg, 500);

      if (TML_SUCCESS == iErr)
        iErr = createCmd4711(&cmdMsg, true);
      if (TML_SUCCESS == iErr){
  #ifdef TML_UNICODE
        fwprintf (stderr, L"Sending async command 4711 (PROFILE2) to %ls -->\n", DESTINATION_HOST_IP);
  #else// TML_UNICODE
        printf ("Sending async command 4711 to %s -->\n", DESTINATION_HOST_IP);
  #endif// TML_UNICODE
        iErr = tml_Connection_SendAsync(connectionHandle, IO_PROFILE2, cmdMsg, 500);
      }
    }
  }
  ///////////////////////////////////////////////////////////////////////
  // Wait for a while to be shure to be synchron with the asynchron callback
  // FOR THIS TEST to avoid a broken communication by closing the core handles
  wait (10000);
  ///////////////////////////////////////////////////////////////////////
  // Free the instances of TMLCore:
  if (TML_HANDLE_TYPE_NULL != coreSenderHandle)
    tml_Core_Close(&coreSenderHandle);
  if (TML_HANDLE_TYPE_NULL != coreListenerHandle)
    tml_Core_Close(&coreListenerHandle);

  ///////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (TML_SUCCESS != iErr)
    printf ("sendAndListenTest04 / error happened - Code = %d\n", iErr);
  return true;
}

bool validateConnection(){
  TML_INT32 iRet;
  TML_BOOL bConnected;
  SIDEX_VARIANT profiles = SIDEX_HANDLE_TYPE_NULL;
  iRet = tml_Connection_Get_RemoteProfiles(m_connectionHandle, &profiles);
  if (TML_SUCCESS != iRet){
    printf ("validateConnection / error iRet = %d on tml_Connection_Get_RemoteProfiles\n", iRet);
  }

  SIDEX_INT32 iSize;
  iRet = sidex_Variant_List_Size (profiles, &iSize);
  if (TML_SUCCESS == iRet){
    printf ("validateConnection / number of profiles = %d\n", iSize);
    for (TML_INT32 i = 0; i < iSize && TML_SUCCESS == iRet; ++i){
      SIDEX_VARIANT item;
      iRet = sidex_Variant_List_Get (profiles, i, &item);
      if (TML_SUCCESS == iRet){
        SIDEX_INT32 iItemLength;
#ifdef SIDEX_UNICODE
        wchar_t* profileName;
        iRet = sidex_Variant_As_String (item, &profileName, &iItemLength);
        if (TML_SUCCESS == iRet){
          fwprintf (stderr, L"profile(%d): %ls\n", i, profileName);
        }
#else // SIDEX_UNICODE
        char* profileName;
        iRet = sidex_Variant_As_String (item, &profileName, &iItemLength);
        if (TML_SUCCESS == iRet){
          printf ("profile(%d): %s\n", i, profileName);
        }
#endif // SIDEX_UNICODE
      }
    }
    sidex_Variant_DecRef(profiles);
  }
  printf ("validateConnection - tml_Connection_Get_RemoteProfiles / iRet = %d\n", iRet);

  iRet = tml_Connection_Validate(m_connectionHandle, TML_TRUE, &bConnected);
  printf ("validateConnection / iRet = %d - bConnected = %d\n", iRet, bConnected);
  return false;
}

#ifdef BUILD_TLS

TML_BOOL check_and_accept_tls_request (SIDEX_VARIANT vServerName){

  // perform some special operations against the serverName value,
  // return TML_FALSE to deny the TLS request, or TML_TRUE to allow it
    
  return TML_TRUE;
}

SIDEX_VARIANT certificate_file_location (SIDEX_VARIANT vServerName)
{
  SIDEX_VARIANT var;

  // perform some special operation to choose which 
  // certificate file to be used, then return it

  sidex_Variant_New_String(TLS_CRT_FILE, &var);
  return var; 
}

SIDEX_VARIANT private_key_file_location (SIDEX_VARIANT vServerName)
{
  SIDEX_VARIANT var;

  // perform some special operation to choose which 
  // private key file to be used, then return it

  sidex_Variant_New_String(TLS_KEY_FILE, &var);
  return var; 
}

bool tlsTest()
{
  TML_BOOL bEncrypted = TML_FALSE;
  TML_BOOL bAccepted = TML_FALSE;
#ifdef TML_UNICODE
  wchar_t* sErrormessage;
#else// TML_UNICODE
  char* sErrormessage;
#endif// TML_UNICODE  

  TML_INT32 iErr = TML_SUCCESS; // API return value
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to receive commands / messages 
  TML_CORE_HANDLE coreListenerHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  TML_CORE_HANDLE coreSenderHandle = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // TML_COMMAND_HANDLE Command / message HANDLE (internal represented by SIDEX data)
  TML_COMMAND_HANDLE cmdMsg  = TML_HANDLE_TYPE_NULL;
  /////////////////////////////////////////////////////////////////////////
  // Listener initialisation:
  iErr = initListener(&coreListenerHandle);
  /////////////////////////////////////////////////////////////////////////
  // Allow to configure if the provided tml core will accept TLS incoming connections
  if (TML_SUCCESS == iErr)
  {
    iErr = tml_Tls_Core_AcceptNegotiation(coreListenerHandle,
                                          check_and_accept_tls_request,
                                          certificate_file_location,
                                          private_key_file_location,
                                          &bAccepted);
  }
  /////////////////////////////////////////////////////////////////////////
  // TML_CORE_HANDLE to send commands / messages 
  if (TML_SUCCESS == iErr)
  {
    iErr = tml_Core_Open(&coreSenderHandle, 0);
  }

  TML_CONNECTION_HANDLE connectionHandle = TML_HANDLE_TYPE_NULL;
  iErr = tml_Core_Connect(coreSenderHandle, DESTINATION_NETWORK_BINDING, &connectionHandle);
  /////////////////////////////////////////////////////////////////////////
  // Start tls negotiation for the requested connection
  if (TML_SUCCESS == iErr)
  {
    iErr = tml_Tls_Connection_StartNegotiation(connectionHandle, TML_FALSE, &bEncrypted);
  }

  if (!bEncrypted)
  {
#ifdef TML_UNICODE
      fwprintf (stderr, L"Cannot negotiate TLS-connection on listener side. Please insert valid \"%ls\" and certificate \"%ls\" file into executable working directory\n", TLS_KEY_FILE, TLS_CRT_FILE);
#else// TML_UNICODE
      printf ("Cannot negotiate TLS-connection on listener side. Please insert valid \"%s\" and certificate \"%s\" file into executable working directory\n", TLS_KEY_FILE, TLS_CRT_FILE);
#endif// TML_UNICODE
  }
  else
  {  
    /////////////////////////////////////////////////////////////////////////
    // Command / message creation:
    if (TML_SUCCESS == iErr)
    {
      iErr = createCmd4711(&cmdMsg, false);
    }

    /////////////////////////////////////////////////////////////////////////
    // send a loop of command / messages
    for (int i = 0; i < 10 && TML_SUCCESS == iErr; ++i)
    {
#ifdef TML_UNICODE
      fwprintf (stderr, L"Sending sync command 4711 to %ls -->\n", DESTINATION_HOST_IP);
#else// TML_UNICODE
      printf ("Sending sync command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE
      iErr = tml_Connection_SendSync(connectionHandle, IO_PROFILE, cmdMsg, 10000);
      if (TML_SUCCESS == iErr)
      {
#ifdef TML_UNICODE
        fwprintf (stderr, L"Sending sync command 4711 (PROFILE2) to %ls -->\n", DESTINATION_HOST_IP);
#else // TML_UNICODE
        printf ("Sending sync command 4711 to %s -->\n", DESTINATION_HOST_IP);
#endif// TML_UNICODE
        iErr = tml_Connection_SendSync(connectionHandle, IO_PROFILE2, cmdMsg, 10000);
      }
    }
    ///////////////////////////////////////////////////////////////////////
    // Free the instance of Command / message HANDLE:
    if (TML_HANDLE_TYPE_NULL != cmdMsg)
    {
      tml_Cmd_Free(&cmdMsg);
    }
  }

  if (TML_HANDLE_TYPE_NULL != connectionHandle)
  {
    tml_Connection_Close(&connectionHandle);
  }
  ///////////////////////////////////////////////////////////////////////
  // Free the instances of TMLCore:
  if (TML_HANDLE_TYPE_NULL != coreSenderHandle)
  {
    tml_Core_Close(&coreSenderHandle);
  }

  if (TML_HANDLE_TYPE_NULL != coreListenerHandle)
  {
    tml_Core_Close(&coreListenerHandle);
  }

  ///////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (TML_SUCCESS != iErr)
  {
    printf ("TLS-Test / error happened - Code = %d\n", iErr);
  }

  return false;
}
#endif // BUILD_TLS
