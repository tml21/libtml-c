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

#include "TmlTester.h"

TmlTester::TmlTester(SIDEX_TCHAR* testProcessName)
         : TestingForReturns(testProcessName)
{
  for(int iMutex = 0; iMutex < MAX_MUTEXES; iMutex++)
  {
    if(!vortex_mutex_create_full(&m_mutex[iMutex], VORTEX_MUTEX_CONF_RECURSIVE))
    {
      errorOutput(tmlrtT("TmlTester - vortex_mutex_create()"), false, false);
    }
  }

  for(int iCore = 0; iCore < MAX_CORES; iCore++)
  {
    m_core[iCore] = TML_HANDLE_TYPE_NULL;

    for(int iListener = 0; iListener < MAX_LISTENERS; iListener++)
    {
      m_listener[iCore][iListener] = TML_HANDLE_TYPE_NULL;
    }
  }
}

TmlTester::~TmlTester()
{
  cleanup(true);

  for(int iMutex = 0; iMutex < MAX_MUTEXES; iMutex++)
  {
    vortex_mutex_destroy(&m_mutex[iMutex]);
  }
}

//------------------------------------------------------------------------------

bool TmlTester::enterMutex(int iMutex)
{
  bool result = false;
  if((iMutex >= 0) && (iMutex < MAX_MUTEXES))
  {
    vortex_mutex_lock(&m_mutex[iMutex]);
    result = true;
  }
  return(result);
}

bool TmlTester::leaveMutex(int iMutex)
{
  bool result = false;
  if((iMutex >= 0) && (iMutex < MAX_MUTEXES))
  {
    vortex_mutex_unlock(&m_mutex[iMutex]);
    result = true;
  }
  return(result);
}

//------------------------------------------------------------------------------

bool TmlTester::prepare()
{
  setTestSectionName(tmlrtT("prepare"));
  messageOutput(S_START);
  reset();

  _prepare();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

bool TmlTester::cleanup(bool silent)
{
  setTestSectionName(tmlrtT("cleanup"));
  if(!silent) messageOutput(S_START);
  reset();

  _cleanup();
  for(int iCore = 0; iCore < MAX_CORES; iCore++) deleteCore(iCore);

  if(!silent) messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  if(!silent) messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlTester::splitCoreAndListener(int& iCore, int& iListener)
{
  if(iListener > 10)
  {
    iCore = iListener / 10;
    iListener %= 10;
  }
  if(iCore < 0) iCore = 0;
  return((iCore     >= 0) && (iCore     < MAX_CORES) &&
         (iListener >= 0) && (iListener < MAX_LISTENERS));
}

//------------------------------------------------------------------------------

TML_CORE_HANDLE TmlTester::getCore(int iCore)
{
  TML_CORE_HANDLE hCore = TML_HANDLE_TYPE_NULL;
  if((iCore >= 0) && (iCore < MAX_CORES)) hCore = m_core[iCore];
  return(hCore);
}

bool TmlTester::createCore(int iCore)
{
  bool success = false;
  if((iCore >= 0) && (iCore < MAX_CORES))
  {
    if(m_core[iCore] == TML_HANDLE_TYPE_NULL)
    {
      m_iErr = tml_Core_Open(&m_core[iCore], 0);
	    success = checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Open(core"),
                                          tmlrt_itoa(iCore + 1),
                                          tmlrtT(")"), 2), true);
    }
    else success = true;
  }
  return(success);
}

bool TmlTester::deleteCore(int iCore)
{
  bool success = true;
  if((iCore >= 0) && (iCore < MAX_CORES))
  {
    if(m_core[iCore] != TML_HANDLE_TYPE_NULL)
    {
      for(int iListener = 0; iListener < MAX_LISTENERS; iListener++)
      {
        if(!deleteListener(iCore, iListener)) success = false;
      }

      m_iErr = tml_Core_GeneralDeregistration(m_core[iCore]);
	    if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_GeneralDeregistration(core"),
                                    tmlrt_itoa(iCore + 1),
                                    tmlrtT(")"), 2), true)) success = false;

      m_iErr = tml_Core_Close(&m_core[iCore]);
	    if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Close(core"),
                                    tmlrt_itoa(iCore + 1),
                                    tmlrtT(")"), 2), true)) success = false;

      m_core[iCore] = TML_HANDLE_TYPE_NULL;
    }
  }
  return(success);
}

//------------------------------------------------------------------------------

TML_LISTENER_HANDLE TmlTester::getListener(int iCore, int iListener)
{
  TML_LISTENER_HANDLE hListener = TML_HANDLE_TYPE_NULL;
  if(splitCoreAndListener(iCore, iListener)) hListener = m_listener[iCore][iListener];
  return(hListener);
}

bool TmlTester::createListener(int iCore, int iListener, SIDEX_TCHAR* sAddress)
{
  bool success = false;
  if(splitCoreAndListener(iCore, iListener))
  {
    if(createCore(iCore))
    {
      if(m_listener[iCore][iListener] == TML_HANDLE_TYPE_NULL)
      {
        m_iErr = tml_Core_Listener_Create(m_core[iCore], sAddress, &m_listener[iCore][iListener]);
        success = checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Listener_Create(core"),
                                            tmlrt_cat(tmlrt_cat(tmlrt_itoa(iCore + 1),
                                                                tmlrtT(", listener"),
                                                                tmlrt_itoa(iListener + 1), 5),
                                                      tmlrtT(", "),
                                                      sAddress, 1),
                                            tmlrtT(")"), 2), true);
      }
      else success = true;
    }
  }
  return(success);
}

bool TmlTester::startListener(int iCore, int iListener)
{
  bool success = false;
  if(splitCoreAndListener(iCore, iListener))
  {
    if(m_listener[iCore][iListener] != TML_HANDLE_TYPE_NULL)
    {
      m_iErr = tml_Listener_Set_Enabled(m_listener[iCore][iListener], TML_TRUE);
      success = checkForSuccess(tmlrt_cat(tmlrtT("tml_Listener_Set_Enabled(core"),
                                          tmlrt_cat(tmlrt_itoa(iCore + 1),
                                                    tmlrtT(", listener"),
                                                    tmlrt_itoa(iListener + 1), 5),
                                          tmlrtT(", true)"), 2), true);
    }
  }
  return(success);
}

bool TmlTester::stopListener(int iCore, int iListener)
{
  bool success = false;
  if(splitCoreAndListener(iCore, iListener))
  {
    if(m_listener[iCore][iListener] != TML_HANDLE_TYPE_NULL)
    {
      m_iErr = tml_Listener_Set_Enabled(m_listener[iCore][iListener], TML_FALSE);
      success = checkForSuccess(tmlrt_cat(tmlrtT("tml_Listener_Set_Enabled(core"),
                                          tmlrt_cat(tmlrt_itoa(iCore + 1),
                                                    tmlrtT(", listener"),
                                                    tmlrt_itoa(iListener + 1), 5),
                                          tmlrtT(", false)"), 2), true);
    }
  }
  return(success);
}

bool TmlTester::deleteListener(int iCore, int iListener)
{
  bool success = false;
  if(splitCoreAndListener(iCore, iListener))
  {
    if(m_listener[iCore][iListener] != TML_HANDLE_TYPE_NULL)
    {
      m_iErr = tml_Listener_Close(&m_listener[iCore][iListener]);
      success = checkForSuccess(tmlrt_cat(tmlrtT("tml_Listener_Close(core"),
                                          tmlrt_cat(tmlrt_itoa(iCore + 1),
                                                    tmlrtT(", listener"),
                                                    tmlrt_itoa(iListener + 1), 5),
                                          tmlrtT(")"), 2), true);
      m_listener[iCore][iListener] = TML_HANDLE_TYPE_NULL;
    }
    else success = true;
  }
  return(success);
}

//------------------------------------------------------------------------------

bool TmlTester::checkConnectionCount(int iCore, int nExpectedConnections, SIDEX_TCHAR* debugText)
{
  bool success = false;
  if((iCore >= 0) && (iCore < MAX_CORES))
  {
    if(m_core[iCore] != TML_HANDLE_TYPE_NULL)
    {
      TML_UINT32 iCount = 0;
      m_iErr = tml_Core_Get_ConnectionCount(m_core[iCore], &iCount);
      if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Get_ConnectionCount(Core"),
                                   tmlrt_itoa(iCore),
                                   tmlrtT(")"), 2), true))
      {
        success = checkForValue(tmlrt_cat(debugText, tmlrtT(" Connection count")),
                                nExpectedConnections, iCount, true);
      }
    }
  }
  return(success);
}

bool TmlTester::checkRemoteProfileCount(TML_CONNECTION_HANDLE hConnection, int nExpectedProfiles, SIDEX_TCHAR* debugText)
{
  bool success = false;
  if(hConnection != TML_HANDLE_TYPE_NULL)
  {
    SIDEX_VARIANT lProfiles = SIDEX_HANDLE_TYPE_NULL;
    m_iErr = tml_Connection_Get_RemoteProfiles(hConnection, &lProfiles);
    if(checkForSuccess(tmlrt_cat(debugText,
                                 debugText ? tmlrtT(" - ") : NULL,
                                 tmlrtT("tml_Connection_Get_RemoteProfiles()")), true))
    {
      SIDEX_INT32 n = getSidexListCount(lProfiles, debugText);
      success = checkForValue(tmlrt_cat(debugText, tmlrtT(" Profile count")), nExpectedProfiles, n, true);
    }
    if(lProfiles != SIDEX_HANDLE_TYPE_NULL)
    {
      sidex_Variant_DecRef(lProfiles);
      lProfiles = SIDEX_HANDLE_TYPE_NULL;
    }
  }
  return(success);
}

//------------------------------------------------------------------------------

SIDEX_INT32 TmlTester::getSidexListCount(SIDEX_VARIANT vList, SIDEX_TCHAR* debugText)
{
  SIDEX_INT32 n = 0;
  if(sidex_Variant_List_Check(vList) == SIDEX_TRUE)
  {
    m_iErr = sidex_Variant_List_Size(vList, &n);
    checkForSuccess(tmlrt_cat(debugText,
                              debugText ? tmlrtT(" - ") : NULL,
                              tmlrtT("sidex_Variant_List_Size()")), true);
  }
  else errorOutput(tmlrt_cat(debugText,
                             debugText ? tmlrtT(" - ") : NULL,
                             tmlrtT("SidexVariant is not a list!")), false, true);
  return(n);
}

SIDEX_TCHAR* TmlTester::getSidexListStringItem(SIDEX_VARIANT vList, SIDEX_INT32 index, SIDEX_TCHAR* debugText)
{
  SIDEX_TCHAR* s = NULL;
  if(sidex_Variant_List_Check(vList) == SIDEX_TRUE)
  {
    SIDEX_VARIANT vString = SIDEX_HANDLE_TYPE_NULL;
    m_iErr = sidex_Variant_List_Get(vList, index, &vString);
    if(checkForSuccess(tmlrt_cat(debugText,
                                 debugText ? tmlrtT(" - ") : NULL,
                                 tmlrt_cat(tmlrtT("sidex_Variant_List_Get("),
                                           tmlrt_itoa(index),
                                           tmlrtT(")"), 2), 4), true))
    {
      if(sidex_Variant_String_Check(vString) == SIDEX_TRUE)
      {
        SIDEX_TCHAR* sBorrowed = NULL;
        SIDEX_INT32  len       = 0;
        m_iErr = sidex_Variant_As_String(vString, &sBorrowed, &len);
        checkForSuccess(tmlrt_cat(debugText,
                                  debugText ? tmlrtT(" - ") : NULL,
                                  tmlrtT("sidex_Variant_As_String()")), true);
        s = tmlrt_cpy(sBorrowed);
        // sBorrowed is borrowed - do not free!
      }
      else errorOutput(tmlrt_cat(debugText,
                                 debugText ? tmlrtT(" - ") : NULL,
                                 tmlrt_cat(tmlrtT("Item #"),
                                           tmlrt_itoa(index),
                                           tmlrtT(" of SidexList is not a string!"), 2), 4), false, true);
    }
  }
  else errorOutput(tmlrt_cat(debugText,
                             debugText ? tmlrtT(" - ") : NULL,
                             tmlrtT("SidexVariant is not a list!")), false, true);
  return(s);
}

//------------------------------------------------------------------------------

bool TmlTester::setCommandID(TML_COMMAND_HANDLE hCommand, TML_COMMAND_ID_TYPE cmdID)
{
  m_iErr = tml_Cmd_Header_SetCommand(hCommand, cmdID);
  return(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Header_SetCommand(ID = "),
                                   tmlrt_itoa(cmdID),
                                   tmlrtT(")"), 2), true));
}

TML_COMMAND_ID_TYPE TmlTester::getCommandID(TML_COMMAND_HANDLE hCommand, TML_COMMAND_ID_TYPE iDefault)
{
  TML_COMMAND_ID_TYPE cmdID = iDefault;
  m_iErr = tml_Cmd_Header_GetCommand(hCommand, &cmdID);
  checkForSuccess(tmlrtT("tml_Cmd_Header_GetCommand()"));
  return(cmdID);
}

//------------------------------------------------------------------------------

bool TmlTester::setCommandInt(TML_COMMAND_HANDLE hCommand, SIDEX_TCHAR* group, SIDEX_TCHAR* key, TML_INT32 iValue)
{
  bool success = false;
  SIDEX_HANDLE hSdx = SIDEX_HANDLE_TYPE_NULL;
  m_iErr = tml_Cmd_Acquire_Sidex_Handle(hCommand, &hSdx);
  if(checkForSuccess(tmlrtT("setCommandInt::tml_Cmd_Acquire_Sidex_Handle()")))
  {
    m_iErr = sidex_Integer_Write(hSdx, group, key, iValue);
    success = checkForSuccess(tmlrtT("setCommandInt::sidex_Integer_Write()"));

    m_iErr = tml_Cmd_Release_Sidex_Handle(hCommand);
    checkForSuccess(tmlrtT("setCommandInt::tml_Cmd_Release_Sidex_Handle()"));
  }
  return(success);
}

TML_INT32 TmlTester::getCommandInt(TML_COMMAND_HANDLE hCommand, SIDEX_TCHAR* group, SIDEX_TCHAR* key, TML_INT32 iDefault)
{
  TML_INT32 iValue = iDefault;
  SIDEX_HANDLE hSdx = SIDEX_HANDLE_TYPE_NULL;
  m_iErr = tml_Cmd_Acquire_Sidex_Handle(hCommand, &hSdx);
  if(checkForSuccess(tmlrtT("getCommandInt::tml_Cmd_Acquire_Sidex_Handle()")))
  {
    SIDEX_INT64 lValue = 0;
    m_iErr = sidex_Integer_Read(hSdx, group, key, &lValue);
    if(m_iErr != SIDEX_ERR_NOCONTENT)
    {
      if(checkForSuccess(tmlrtT("getCommandInt::sidex_Integer_Read()")))
      {
        iValue = (TML_INT32)lValue;
      }
    }
    m_iErr = tml_Cmd_Release_Sidex_Handle(hCommand);
    checkForSuccess(tmlrtT("getCommandInt::tml_Cmd_Release_Sidex_Handle()"));
  }
  return(iValue);
}
