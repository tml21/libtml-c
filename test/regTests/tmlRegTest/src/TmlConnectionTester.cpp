/*
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2016 wobe-systems GmbH
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

#include "TmlConnectionTester.h"

TmlConnectionTester::TmlConnectionTester(SIDEX_TCHAR* testProcessName)
                   : TestingForReturns(testProcessName)
{
}

TmlConnectionTester::~TmlConnectionTester()
{
}

/*
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Connect(TML_CORE_HANDLE coreHandle, const TML_CTSTR* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Close(TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Get_Address(TML_CONNECTION_HANDLE connectionHandle, TML_CTSTR** sAddress);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Get_RemoteProfiles(TML_CONNECTION_HANDLE connectionHandle, SIDEX_VARIANT* lProfiles);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Validate(TML_CONNECTION_HANDLE connectionHandle, TML_BOOL bReconnect, TML_BOOL* bConnected);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ConnectionCount(TML_CORE_HANDLE coreHandle, TML_UINT32* iCount);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_Connection(TML_CORE_HANDLE coreHandle, TML_UINT32 index, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ConnectionByAddress(TML_CORE_HANDLE coreHandle, TML_CTSTR* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendAsync(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendSync(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Get_Connection(TML_COMMAND_HANDLE cmdHandle, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_OnConnect(TML_CORE_HANDLE coreHandle, TML_ON_CONNECT_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_OnDisconnect(TML_CORE_HANDLE coreHandle, TML_ON_DISCONNECT_CB_FUNC pCBFunc, TML_POINTER pCBData);
*/

bool TmlConnectionTester::testConnect()
{
  setTestSectionName(tmlrtT("testConnect"));
  messageOutput(S_START);
  reset();

  m_iErr = 0;
  if(checkForSuccess(tmlrtT("xyz")))
  {
  }

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testClose()
{
  setTestSectionName(tmlrtT("testClose"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testGetAddress()
{
  setTestSectionName(tmlrtT("testGetAddress"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testGetRemoteProfiles()
{
  setTestSectionName(tmlrtT("testGetRemoteProfiles"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testValidate()
{
  setTestSectionName(tmlrtT("testValidate"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testGetConnectionCount()
{
  setTestSectionName(tmlrtT("testGetConnectionCount"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testGetConnection_Core()
{
  setTestSectionName(tmlrtT("testGetConnection_Core"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testGetConnectionByAddress()
{
  setTestSectionName(tmlrtT("testGetConnectionByAddress"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testSendSync()
{
  setTestSectionName(tmlrtT("testSendSync"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testSendAsync()
{
  setTestSectionName(tmlrtT("testSendAsync"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testGetConnection_Cmd()
{
  setTestSectionName(tmlrtT("testGetConnection_Cmd"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testSetOnConnect()
{
  setTestSectionName(tmlrtT("testSetOnConnect"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlConnectionTester::testSetOnDisconnect()
{
  setTestSectionName(tmlrtT("testSetOnDisconnect"));
  messageOutput(S_START);
  reset();



  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}
