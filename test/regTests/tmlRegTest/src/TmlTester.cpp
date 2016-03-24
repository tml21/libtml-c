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
	    success = checkForSuccess(tmlrt_cat_del(tmlrtT("tml_Core_Open(core"),
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
	    if(!checkForSuccess(tmlrt_cat_del(tmlrtT("tml_Core_GeneralDeregistration(core"),
                                        tmlrt_itoa(iCore + 1),
                                        tmlrtT(")"), 2), true)) success = false;

      m_iErr = tml_Core_Close(&m_core[iCore]);
	    if(!checkForSuccess(tmlrt_cat_del(tmlrtT("tml_Core_Close(core"),
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
        success = checkForSuccess(tmlrt_cat_del(tmlrtT("tml_Core_Listener_Create(core"),
                                                tmlrt_cat_del(tmlrt_cat_del(tmlrt_itoa(iCore + 1),
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
      success = checkForSuccess(tmlrt_cat_del(tmlrtT("tml_Listener_Set_Enabled(core"),
                                              tmlrt_cat_del(tmlrt_itoa(iCore + 1),
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
      success = checkForSuccess(tmlrt_cat_del(tmlrtT("tml_Listener_Set_Enabled(core"),
                                              tmlrt_cat_del(tmlrt_itoa(iCore + 1),
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
      success = checkForSuccess(tmlrt_cat_del(tmlrtT("tml_Listener_Close(core"),
                                              tmlrt_cat_del(tmlrt_itoa(iCore + 1),
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
      if(checkForSuccess(tmlrt_cat_del(tmlrtT("tml_Core_Get_ConnectionCount(Core"),
                                       tmlrt_itoa(iCore),
                                       tmlrtT(")"), 2), true))
      {
        success = (iCount == nExpectedConnections);
        if(!success)
        {
          errorOutput(tmlrt_cat_del(tmlrt_cat_del(tmlrt_cat(debugText,
                                                            tmlrtT(" "),
                                                            tmlrtT("Connection count expected as ")),
                                                  tmlrt_itoa(nExpectedConnections),
                                                  tmlrtT(", but is "), 3),
                                    tmlrt_itoa(iCount),
                                    tmlrtT("!"), 3), false, true);
        }
      }
    }
  }
  return(success);
}
