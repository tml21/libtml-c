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
    aData.blockUntilAsyncReturn = false;
    aData.sleepTimer = 4000;
    aData.checkAsyncReturn = 0;
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

void TmlSendingCommandsTester::callBackSyncCmd(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData)
{
    cout << "Listener received cmd 1000" << endl;
    TmlSleep(2000);
}

bool TmlSendingCommandsTester::testSyncMessage()
{
  setTestSectionName(tmlrtT("tml_Send_SyncMessage"));
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
    TML_COMMAND_HANDLE tCommandSleep = TML_HANDLE_TYPE_NULL;
    TML_COMMAND_HANDLE tCommand      = TML_HANDLE_TYPE_NULL;

    tml_Cmd_Create(&tCommandSleep);
    setCommandID(tCommandSleep, 1000);

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

            m_iErr = tml_Profile_Register_Cmd(getCore(1), S_IO_PROFILE, (TML_COMMAND_ID_TYPE) 1000, &(TmlSendingCommandsTester::callBackSyncCmd), &(aData));
            checkForSuccess();

            /***********************************  START Test tml_Send_SyncMessage  *******************************/

            
            // tml_Send_SyncMessage timeout is 1000ms. The callBackSyncCmd waits 1500ms which results in a timeout error
            m_iErr = tml_Send_SyncMessage( getCore(0), tCommandSleep, S_IO_PROFILE , S_TP_127_0_0_1, tmlrt_itoa(iPort), 1000);
            checkForExpectedReturnCode(TML_ERR_TIMEOUT, tmlrtT("tml_Send_SyncMessage(Timeout)"));
            
            // A valid test call for tml_Send_SyncMessage
            m_iErr = tml_Send_SyncMessage( getCore(0), tCommand, S_IO_PROFILE , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
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

            #if defined(SIDEX_UNICODE) || defined(TML_UNICODE)
            // Check for invalid Unicode error
            m_iErr = tml_Send_SyncMessage(getCore(0), tCommand, NULL , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(SIDEX_ERR_UNICODE, tmlrtT("tml_Send_SyncMessage(Unicode error)"));
            #endif

            // Check for invalid host address
            m_iErr = tml_Send_SyncMessage(getCore(0), tCommand, S_IO_PROFILE , tmlrtT("127.0.5.1"), tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Send_SyncMessage(WrongHostAddress)"));

            // Check for invalid host port
            m_iErr = tml_Send_SyncMessage(getCore(0), tCommand, S_IO_PROFILE , S_TP_127_0_0_1, tmlrtT("5555"), 3000);
            checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Send_SyncMessage(WrongHostPort)"));           
            
            
            /***********************************  END Test tml_Send_SyncMessage  *******************************/
           
            stopListener(1, 0);
        } // startListener 1, 0
        deleteListener(1, 0);
      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    tml_Cmd_Free(&tCommand);
    tml_Cmd_Free(&tCommandSleep);

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

void TmlSendingCommandsTester::callBackAsyncCmd(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData)
{
    AsyncCallbackData* atest = (AsyncCallbackData*) pCBData;
    cout << "Sender command 1234 finished waiting time: " << atest->sleepTimer << "ms" << endl;
    if(atest->checkAsyncReturn == 0) atest->checkAsyncReturn = 1234;
    atest->blockUntilAsyncReturn = true;
}

void TmlSendingCommandsTester::callBackAsyncCmdInstant(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData)
{
    AsyncCallbackData* atest = (AsyncCallbackData *) pCBData;
    cout << "Sender command 1235 finished" << endl;
    //TmlSleep(3500);
    if(atest->checkAsyncReturn == 0) atest->checkAsyncReturn = 1235;
}

void TmlSendingCommandsTester::listenerCallBackReturnInstantly(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData)
{
    AsyncCallbackData * atest = (AsyncCallbackData *) pCBData;
    cout << "Listener received command 1235 and returns immediatly" << endl;
}

void TmlSendingCommandsTester::listenerCallBack(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData)
{
    AsyncCallbackData* atest = (AsyncCallbackData*) pCBData;
    TestingForReturns check;
    TML_COMMAND_HANDLE CommandHdl = TML_HANDLE_TYPE_NULL;
    tml_Cmd_Create(&CommandHdl);

    int aSleep = atest->sleepTimer / 4;

    cout << "Listener received command 1234 sleep time: " << atest->sleepTimer << "ms" << endl;

    /*********************************  START Test tml_Send_AsyncProgressReply  *****************************/
    // Check for invalid TML command handle
    check.m_iErr = tml_Send_AsyncProgressReply(TML_HANDLE_TYPE_NULL, 1);
    check.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Send_AsyncProgressReply(MissingObj)"));

    // Check if the progress count is out of range
    check.m_iErr = tml_Send_AsyncProgressReply(tmlhandle, -1);
    check.checkForExpectedReturnCode(TML_ERR_COMMAND_PROGRESS_RANGE, tmlrtT("tml_Send_AsyncProgressReply(CommandProgressOutOfRange)"));

    // Check if core handle is assigned to a command 
    check.m_iErr = tml_Send_AsyncProgressReply(CommandHdl, 1);
    check.checkForExpectedReturnCode(TML_ERR_ATTRIBUTE_NOT_SET, tmlrtT("tml_Send_AsyncProgressReply(NoCoreAssignedtoCmd)"));
 
    // A valid test call for tml_Send_AsyncProgressReply
    check.m_iErr = tml_Send_AsyncProgressReply(tmlhandle, 1);
    check.checkForSuccess();
    TmlSleep(aSleep);

    check.m_iErr = tml_Send_AsyncProgressReply(tmlhandle, 25);
    check.checkForSuccess();
    TmlSleep(aSleep);

    check.m_iErr = tml_Send_AsyncProgressReply(tmlhandle, 50);
    check.checkForSuccess();
    TmlSleep(aSleep);

    check.m_iErr = tml_Send_AsyncProgressReply(tmlhandle, 75);
    check.checkForSuccess();
    TmlSleep(aSleep);

    check.m_iErr = tml_Send_AsyncProgressReply(tmlhandle, 100);
    check.checkForSuccess();

    /*********************************  END   Test tml_Send_AsyncProgressReply  *****************************/

    /*********************************  START Test tml_Send_AsyncStatusReply  *****************************/

    // Check for invalid TML command handle
    check.m_iErr = tml_Send_AsyncStatusReply(TML_HANDLE_TYPE_NULL , 10, tmlrtT("Test"));
    check.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Send_AsyncStatusReply(MissingObj)"));

    // Check for not supported command handle
    check.m_iErr = tml_Send_AsyncStatusReply(CommandHdl, 10, tmlrtT("Test"));
    check.checkForExpectedReturnCode(TML_ERR_ATTRIBUTE_NOT_SET, tmlrtT("tml_Send_AsyncStatusReply(MissingObj)"));
    
    // Check for not undefined reply type
    check.m_iErr = tml_Send_AsyncStatusReply(tmlhandle, -1, tmlrtT("Test"));
    check.checkForExpectedReturnCode(TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED, tmlrtT("tml_Send_AsyncStatusReply(AttributeNotSet)"));

    #if defined(SIDEX_UNICODE) || defined(TML_UNICODE)
    // Check for Unicode error
    check.m_iErr = tml_Send_AsyncStatusReply(tmlhandle, -1, NULL);
    check.checkForExpectedReturnCode(SIDEX_ERR_UNICODE, tmlrtT("tml_Send_AsyncStatusReply(UnicodeError)"));
    #endif

    // A valid test call for tml_Send_AsyncStatusReply
    check.m_iErr = tml_Send_AsyncStatusReply(tmlhandle, 10, tmlrtT("Test10"));
    check.checkForSuccess();

    // A valid test call for tml_Send_AsyncStatusReply
    check.m_iErr = tml_Send_AsyncStatusReply(tmlhandle, 20, tmlrtT("Test20"));
    check.checkForSuccess();

    // A valid test call for tml_Send_AsyncStatusReply
    check.m_iErr = tml_Send_AsyncStatusReply(tmlhandle, 30, tmlrtT("Test30"));
    check.checkForSuccess();

    /*********************************  END   Test tml_Send_AsyncStatusReply  *****************************/
    
    tml_Cmd_Free(&CommandHdl);
}

void TmlSendingCommandsTester::asyncProgressDisplay(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData, TML_INT32 iProgress)
{
    cout << "Received Progress Reply: " << iProgress << "%" << endl;
}

void TmlSendingCommandsTester::asyncStatusReply(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData, TML_INT32 iType, TML_CTSTR *sMsg)
{
  cout << "iType: " << iType << " Message: ";
  
  #ifdef SIDEX_UNICODE
  fwprintf (stdout, L"%ls\n", sMsg);
  #else// SIDEX_UNICODE
  printf ("%s\n", sMsg);
  #endif// SIDEX_UNICODE
}

bool TmlSendingCommandsTester::testAsyncMessage()
{
  setTestSectionName(tmlrtT("tml_Send_AsyncMessage"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();

  if(n > 0)
  {  
    int          iPort     = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard     = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress1 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort), 4);
    DELETE_STR(sCard);

    TML_COMMAND_HANDLE tCommand = TML_HANDLE_TYPE_NULL;
    tml_Cmd_Create(&tCommand);
    setCommandID(tCommand, 1234);

    TML_COMMAND_HANDLE tCommand2 = TML_HANDLE_TYPE_NULL;
    tml_Cmd_Create(&tCommand2);
    setCommandID(tCommand2, 1235);

    if(createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress1))
      {
        if(startListener(1, 0))
        {
            m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE);
            checkForSuccess();

            /***********************************  START Test tml_Send_AsyncMessage  *******************************/

            // Check for missing core handle
            m_iErr = tml_Send_AsyncMessage(TML_HANDLE_TYPE_NULL, tCommand, S_IO_PROFILE , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Send_AyncMessage(NoCoreHandle)"));

            // Check for wrong profile
            m_iErr = tml_Send_AsyncMessage(getCore(0), tCommand, S_IO_PROFILE_TWO , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(TML_ERR_SENDER_PROFILE_NOT_SUPPORTED, tmlrtT("tml_Send_AsyncMessage(WrongProfile)")); 
    
            #if defined(SIDEX_UNICODE) || defined(TML_UNICODE)
            // Check for Unicode Error
            m_iErr = tml_Send_AsyncMessage(getCore(0), tCommand, NULL , S_TP_127_0_0_1, tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(SIDEX_ERR_UNICODE, tmlrtT("tml_Send_AsyncMessage(InvalidProfileType)"));
            #endif

            // Check for invalid host address
            m_iErr = tml_Send_AsyncMessage(getCore(0), tCommand, S_IO_PROFILE , tmlrtT("127.0.5.1"), tmlrt_itoa(iPort), 3000);
            checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Send_AsyncMessage(WrongHostAddress)"));

            // Check for invalid host port
            m_iErr = tml_Send_AsyncMessage(getCore(0), tCommand, S_IO_PROFILE , S_TP_127_0_0_1, tmlrtT("5555"), 3000);
            checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Send_AsyncMessage(WrongHostPort)"));

            /***********************************  END Test tml_Send_AsyncMessage  *******************************/

            /***************** Register all necessary callbacks to test tml_Send_AsyncMessage *******************/

            // Listener callback for command id 1234. This callback tests the tml_Send_AsyncProgressReply and tml_Send_AsyncStatusReply
            m_iErr = tml_Profile_Register_Cmd(getCore(1), S_IO_PROFILE, 1234, TmlSendingCommandsTester::listenerCallBack, &(aData));
            checkForSuccess();

            m_iErr = tml_Profile_Register_Cmd(getCore(1), S_IO_PROFILE, 1337, TmlSendingCommandsTester::listenerCallBack, &(aData));
            checkForSuccess();

            // Sender callback when listener returns
            m_iErr = tml_Cmd_Register_CommandReady(tCommand, TmlSendingCommandsTester::callBackAsyncCmd, &(aData));
            checkForSuccess();

            // Sender callback for incoming listener progress messages
            m_iErr = tml_Cmd_Register_Progress(tCommand, TmlSendingCommandsTester::asyncProgressDisplay, &(aData));
            checkForSuccess();

            // Sender callback for async status replies
            m_iErr = tml_Cmd_Register_StatusReply(tCommand, TmlSendingCommandsTester::asyncStatusReply, &(aData));
            checkForSuccess();

            // Listener command that returns immediatly
            m_iErr = tml_Profile_Register_Cmd(getCore(1), S_IO_PROFILE, 1235, TmlSendingCommandsTester::listenerCallBackReturnInstantly, &(aData));
            checkForSuccess();

            // Listener command that returns immediatly
            m_iErr = tml_Cmd_Register_CommandReady(tCommand2, TmlSendingCommandsTester::callBackAsyncCmdInstant, &(aData));
            checkForSuccess();

            /********************************** End of callback registrations ************************************/

            // A valid test call for tml_Send_AsyncMessage.  
            // This call also triggers the tml_Send_AsyncProgressReply and tml_Send_AsyncStatusReply regression tests.
            m_iErr = tml_Send_AsyncMessage(getCore(0), tCommand, S_IO_PROFILE , S_TP_127_0_0_1, tmlrt_itoa(iPort), 10000);
            checkForSuccess();       

            TmlSleep(1000);

            // This call tests the async behaviour of the libTML library
            m_iErr = tml_Send_AsyncMessage(getCore(0), tCommand2, S_IO_PROFILE , S_TP_127_0_0_1, tmlrt_itoa(iPort), 10000);
            checkForSuccess();       

            while(!(aData.blockUntilAsyncReturn)){
              TmlSleep(50);
            };
            aData.blockUntilAsyncReturn = false;                       

            if(aData.checkAsyncReturn == 1235)
            {
                m_iErr = 0;
            }
            else if(aData.checkAsyncReturn == 1234)
            {
                cout << "Command 1234 returned first. This should not happen." << endl;
                m_iErr = -1;
            }             
            checkForSuccess();

        } // startListener 1, 0
        deleteListener(1, 0);
      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    tml_Cmd_Free(&tCommand);
    tml_Cmd_Free(&tCommand2);
    deleteCore(1);
    deleteCore(0);    

    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}