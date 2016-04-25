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

#include "TmlSendingCommandsTester.h"
#include "TestParams.h"

TmlSendingCommandsTester::TmlSendingCommandsTester(SIDEX_TCHAR* testProcessName) : TmlTester(testProcessName)
{
    
}


TmlSendingCommandsTester::~TmlSendingCommandsTester(void)
{
}

void TmlSendingCommandsTester::_prepare()
{
  if(!TestParams || !TestParams->hasParams())
  {
    errorOutput(tmlrtT("TestParams doesn't exist!"));
  }
  else
  {
    if(TestParams->getNetworkCardCount() == 0)
    {
      errorOutput(tmlrtT("No network cards specified!"));
    }
  }
}

bool TmlSendingCommandsTester::testSyncMessage()
{
  setTestSectionName(tmlrtT("testConnect"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();

  if(n > 0)
  {  
    int          iPort     = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard     = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress1 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort), 4);
    DELETE_STR(sCard);

    TML_COMMAND_HANDLE tCommandEmpty = TML_HANDLE_TYPE_NULL;
    TML_COMMAND_HANDLE tCommand = TML_HANDLE_TYPE_NULL;
    tml_Cmd_Create(&tCommand);
    setCommandID(tCommand, 1234);
    
    if(createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress1))
      {
        if(startListener(1, 0))
        {
            m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE);
            checkForSuccess();

            // A valid test call for tml_Send_SyncMessage
            m_iErr = tml_Send_SyncMessage(getCore(0), tCommand, S_IO_PROFILE , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            checkForSuccess();

            // Check for missing core handle
            m_iErr = tml_Send_SyncMessage(TML_HANDLE_TYPE_NULL, tCommand, S_IO_PROFILE , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Send_SyncMessage(NoCoreHandle)"));
            
            // to be implemented
            //m_iErr = tml_Send_SyncMessage(getCore(0), tCommandEmpty, S_IO_PROFILE , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            //checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Send_SyncMessage(NoCoreHandle)"));

            // Check for wrong profile
            m_iErr = tml_Send_SyncMessage(getCore(0), tCommand, S_IO_PROFILE_TWO , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(TML_ERR_SENDER_PROFILE_NOT_SUPPORTED, tmlrtT("tml_Send_SyncMessage(WrongProfile)"));            

            // Check for invalid profile type
            m_iErr = tml_Send_SyncMessage(getCore(0), tCommand, NULL , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(SIDEX_ERR_UNICODE, tmlrtT("tml_Send_SyncMessage(InvalidProfileType)"));

            // Check for invalid host address
            m_iErr = tml_Send_SyncMessage(getCore(0), tCommand, S_IO_PROFILE , tmlrtT("127.0.5.1"), tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Send_SyncMessage(WrongHostAddress)"));

            // Check for invalid host port
            m_iErr = tml_Send_SyncMessage(getCore(0), tCommand, S_IO_PROFILE , S_TP_127_0_0_1, tmlrtT("5555"), 3000);
            checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Send_SyncMessage(WrongHostPort)"));
            
           
            stopListener(1, 0);
        } // startListener 1, 0
        deleteListener(1, 0);
      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}