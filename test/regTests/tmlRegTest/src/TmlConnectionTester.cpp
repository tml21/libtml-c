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
#include "TestParams.h"

//------------------------------------------------------------------------------

SIDEX_TCHAR* cbt_Name[] =
{
  tmlrtT("OnConnect"), tmlrtT("OnDisconnect"),
  tmlrtT("OnCommand"),
  tmlrtT("")  // end of list marker
};

cbData_t cbData_Connection[MAX_CORES][cbt_TypeCount];

//------------------------------------------------------------------------------

cbData_t* prepareCallbackData(TmlConnectionTester* tester, int iCore, cbType_t type)
{
  cbData_t* result = NULL;
  if((iCore >= 0) && (iCore < MAX_CORES) && (type >= 0) && (type < cbt_TypeCount))
  {
    result = &cbData_Connection[iCore][type];
    result->tester        = tester;
    result->type          = type;
    result->iCore         = iCore;
    result->iValue        = 0;
    result->sValue        = NULL;
    result->bDeleteString = false;
  }
  return(result);
}

void CallbackHandler_DisConnection(TML_CONNECTION_HANDLE hConnection, TML_POINTER pCBData)
{
  if(enterGlobalMutex())
  {
    cbData_t* data = (cbData_t*)pCBData;
    if(data)
    {
      if(data->tester)
      {
        data->tester->OnDisConnectionCallback(hConnection, data);
      }
      else wcout << "CallbackHandler_DisConnection: no Tester!" << endl;

      if(data->bDeleteString) DELETE_STR(data->sValue);
    }
    else wcout << "CallbackHandler_DisConnection: no CB-Data!" << endl;
    leaveGlobalMutex();
  }
}

void CallbackHandler_Command(TML_COMMAND_HANDLE hCommand, TML_POINTER pCBData)
{
  if(enterGlobalMutex())
  {
    cbData_t* data = (cbData_t*)pCBData;
    if(data)
    {
      if(data->tester)
      {
        data->tester->OnCommandCallback(hCommand, data);
      }
      else wcout << "CallbackHandler_Command: no Tester!" << endl;

      if(data->bDeleteString) DELETE_STR(data->sValue);
    }
    else wcout << "CallbackHandler_Command: no CB-Data!" << endl;
    leaveGlobalMutex();
  }
}

//------------------------------------------------------------------------------

TmlConnectionTester::TmlConnectionTester(SIDEX_TCHAR* testProcessName)
                   : TmlTester(testProcessName)
{
  m_cbLog_Connection    = 0;
  m_cbLog_Disconnection = 0;
  m_cbLog_Command       = 0;
}

TmlConnectionTester::~TmlConnectionTester()
{
}

//------------------------------------------------------------------------------

void TmlConnectionTester::_prepare()
{
  TmlTester::_prepare();

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

void TmlConnectionTester::_cleanup()
{

  TmlTester::_cleanup();
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testConnect()
{
  setTestSectionName(tmlrtT("testConnect"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort     = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard     = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress1 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    SIDEX_TCHAR* sAddress2 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    if(createCore(0) && createCore(1))
    {
      TML_CONNECTION_HANDLE hConnection1, hConnection2;

      // The initial connection count has to be 0...
      checkConnectionCount(0, 0, tmlrtT(" (start)"));

      // Test with invalid core handle...
      hConnection1 = TML_HANDLE_TYPE_NULL;
      m_iErr = tml_Core_Connect(TML_HANDLE_TYPE_NULL, sAddress1, &hConnection1);
      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Connect(NoCoreHandle)"));

      checkConnectionCount(0, 0, tmlrtT(" (after NoCoreHandle)"));

      // Test with invalid connection handle parameter...
      m_iErr = tml_Core_Connect(getCore(0), sAddress1, NULL);
      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Connect(NoConnectionHandleParameter)"));

      checkConnectionCount(0, 0, tmlrtT(" (after NoConnectionHandleParameter)"));

      // Test with invalid address...
      hConnection1 = TML_HANDLE_TYPE_NULL;
      m_iErr = tml_Core_Connect(getCore(0), tmlrtT("This is an invalid address!"), &hConnection1);
      checkForExpectedReturnCode(TML_ERR_NET_BINDING, tmlrtT("tml_Core_Connect(InvalidAddress)"));

      checkConnectionCount(0, 0, tmlrtT(" (after InvalidAddress)"));

      // Test with invalid port...
      hConnection1 = TML_HANDLE_TYPE_NULL;
      m_iErr = tml_Core_Connect(getCore(0), tmlrtT("127.0.0.1:Wrong"), &hConnection1);
      checkForExpectedReturnCode(TML_ERR_NET_BINDING, tmlrtT("tml_Core_Connect(InvalidPort)"));

      checkConnectionCount(0, 0, tmlrtT(" (after InvalidAddress)"));

      // Test connection without listener...
      hConnection1 = TML_HANDLE_TYPE_NULL;
      m_iErr = tml_Core_Connect(getCore(0), sAddress1, &hConnection1);
      checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Core_Connect(NoListener)"));

      checkConnectionCount(0, 1, tmlrtT(" (after NoListener)"));

      if(hConnection1 != TML_HANDLE_TYPE_NULL)
      {
        m_iErr = tml_Connection_Close(&hConnection1);
        checkForSuccess(tmlrtT("tml_Connection_Close(Connection1)"));
        hConnection1 = TML_HANDLE_TYPE_NULL;
      }

      if(createListener(1, 0, sAddress1))
      {
        if(startListener(1, 0))
        {
          checkConnectionCount(0, 0, tmlrtT(" (after listener start)"));

          // Test connection...
          hConnection1 = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress1, &hConnection1);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress1, tmlrtT(")")), true))
          {
            // The connection count has to be 1 now...
            if(checkConnectionCount(0, 1, tmlrtT(" (after first connect)")))
            {
              // Test same connection again...
              hConnection2 = TML_HANDLE_TYPE_NULL;
              m_iErr = tml_Core_Connect(getCore(0), sAddress1, &hConnection2);
              checkForExpectedReturnCode(TML_ERR_CONNECTION_ALREADY_EXISTS, tmlrtT("tml_Core_Connect(SameAddressAgain)"));

              checkConnectionCount(0, 1, tmlrtT(" (after SameAddressAgain)"));

              if(createListener(1, 1, sAddress2))
              {
                if(startListener(1, 1))
                {
                  // Test second connection...
                  hConnection2 = TML_HANDLE_TYPE_NULL;
                  m_iErr = tml_Core_Connect(getCore(0), sAddress2, &hConnection2);
                  if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress2, tmlrtT(")")), true))
                  {
                    // The connection count has to be 2 now...
                    if(checkConnectionCount(0, 2, tmlrtT(" (after second connect)")))
                    {
                      messageOutput(tmlrtT("Test culmination reached!"));

                      m_iErr = tml_Connection_Close(&hConnection2);
                      checkForSuccess(tmlrtT("tml_Connection_Close(Connection2)"));
                      hConnection2 = TML_HANDLE_TYPE_NULL;
                    }
                  }

                  stopListener(1, 1);

                } // startListener 1, 1

                deleteListener(1, 1);

              } // createListener 1, 1, addr 2

              m_iErr = tml_Connection_Close(&hConnection1);
              checkForSuccess(tmlrtT("tml_Connection_Close(Connection1)"));
              hConnection1 = TML_HANDLE_TYPE_NULL;

            } // checkConnectionCount 0, 1
          } // connect 0, addr 1

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress2);
    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testClose()
{
  setTestSectionName(tmlrtT("testClose"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    SIDEX_TCHAR* sAddress = tmlrt_cat(TestParams->getNetworkCard(0),
                                      tmlrtT(":"),
                                      tmlrt_itoa(TestParams->getFirstPortNumber()), 5);
    if(createCore(0) && createCore(1))
    {
      TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;

      // The initial connection count has to be 0...
      checkConnectionCount(0, 0, tmlrtT(" (start)"));

      // Test with invalid connection handle...
      hConnection = TML_HANDLE_TYPE_NULL;
      m_iErr = tml_Connection_Close(&hConnection);
      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_Close(NoConnectionHandle)"));

      checkConnectionCount(0, 0, tmlrtT(" (after NoConnectionHandle)"));

      // Test with invalid connection handle parameter...
      m_iErr = tml_Connection_Close(NULL);
      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_Close(NoConnectionHandleParameter)"));

      checkConnectionCount(0, 0, tmlrtT(" (after NoConnectionHandleParameter)"));

      if(createListener(1, 0, sAddress))
      {
        if(startListener(1, 0))
        {
          checkConnectionCount(0, 0, tmlrtT(" (after listener start)"));

          hConnection = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress, tmlrtT(")")), true))
          {
            // The connection count has to be 1 now...
            if(checkConnectionCount(0, 1, tmlrtT(" (after connect)")))
            {
              // Test connection close...
              m_iErr = tml_Connection_Close(&hConnection);
              if(checkForSuccess(tmlrtT("tml_Connection_Close(Connection)")))
              {
                // The connection count has to be 0 now...
                if(checkConnectionCount(0, 0, tmlrtT(" (after close connection)")))
                {
                  messageOutput(tmlrtT("Test culmination reached!"));
                }
              }
            } // checkConnectionCount 0, 1
          } // connect 0, addr 1

          hConnection = TML_HANDLE_TYPE_NULL;

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testGetAddress()
{
  setTestSectionName(tmlrtT("testGetAddress"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    SIDEX_TCHAR* sAddress = tmlrt_cat(TestParams->getNetworkCard(0),
                                      tmlrtT(":"),
                                      tmlrt_itoa(TestParams->getFirstPortNumber()), 5);
    if(createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress))
      {
        if(startListener(1, 0))
        {
          TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress, tmlrtT(")")), true))
          {
            // The connection count has to be 1 now...
            if(checkConnectionCount(0, 1, tmlrtT(" (after connect)")))
            {
              SIDEX_TCHAR* pAddress = NULL;

              // Test with missing parameter...
              m_iErr = tml_Connection_Get_Address(hConnection, NULL);
              checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_Get_Address(NoAddressParameter)"));

              // Test with missing connection handle...
              pAddress = NULL;
              m_iErr = tml_Connection_Get_Address(TML_HANDLE_TYPE_NULL, &pAddress);
              checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_Get_Address(NoConnectionHandle)"));

              // Test getting address...
              pAddress = NULL;
              m_iErr = tml_Connection_Get_Address(hConnection, &pAddress);
              if(checkForSuccess(tmlrtT("tml_Connection_Get_Address(Connection)")))
              {
                if(tmlrt_cmp(sAddress, pAddress) == 0)
                {
                  messageOutput(tmlrtT("Test culmination reached!"));
                }
                else errorOutput(tmlrt_cat(sAddress,
                                            tmlrtT(" is different to "),
                                            pAddress), false, true);
              }

              pAddress = NULL;

              // Close connection...
              m_iErr = tml_Connection_Close(&hConnection);
              if(checkForSuccess(tmlrtT("tml_Connection_Close(Connection)")))
              {
                // The connection count has to be 0 now...
                checkConnectionCount(0, 0, tmlrtT(" (after close connection)"));
              }
            } // checkConnectionCount 0, 1
          } // connect 0, addr 1

          hConnection = TML_HANDLE_TYPE_NULL;

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testGetRemoteProfiles()
{
  setTestSectionName(tmlrtT("testGetRemoteProfiles"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort     = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard     = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress1 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    SIDEX_TCHAR* sAddress2 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    if(createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress1))
      {
        if(startListener(1, 0))
        {
          TML_CONNECTION_HANDLE hConnection1 = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress1, &hConnection1);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress1, tmlrtT(")")), true))
          {
            // Test with missing parameter...
            m_iErr = tml_Connection_Get_RemoteProfiles(hConnection1, NULL);
            checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_Get_RemoteProfiles(NoProfilesParameter)"));

            // Test with missing connection handle...
            SIDEX_VARIANT lProfiles = SIDEX_HANDLE_TYPE_NULL;
            m_iErr = tml_Connection_Get_RemoteProfiles(TML_HANDLE_TYPE_NULL, &lProfiles);
            checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_Get_RemoteProfiles(NoConnectionHandle)"));

            checkRemoteProfileCount(hConnection1, 0, tmlrtT("No profiles on connection1"));

            m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE);
            checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core1, "), S_IO_PROFILE, tmlrtT(")")), true);
//            checkRemoteProfileCount(hConnection1, 1, tmlrtT("One profile on connection1"));

            if(createListener(1, 1, sAddress2))
            {
              if(startListener(1, 1))
              {
                TML_CONNECTION_HANDLE hConnection2 = TML_HANDLE_TYPE_NULL;
                m_iErr = tml_Core_Connect(getCore(0), sAddress2, &hConnection2);
                if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress2, tmlrtT(")")), true))
                {
                  checkRemoteProfileCount(hConnection2, 1, tmlrtT("One profile on connection2"));

                  m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE_TWO);
                  checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core1, "), S_IO_PROFILE_TWO, tmlrtT(")")), true);
//                  checkRemoteProfileCount(hConnection2, 2, tmlrtT("Two profiles on connection2"));

                  m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE_THREE);
                  checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core1, "), S_IO_PROFILE_THREE, tmlrtT(")")), true);
//                  checkRemoteProfileCount(hConnection2, 3, tmlrtT("Three profiles on connection2"));

                  // !!! workaraound !!!
                  // Remote profile list doesn't update, if already connected!
                  m_iErr = tml_Connection_Close(&hConnection2);
                  checkForSuccess(tmlrtT("tml_Connection_Close(Connection1)"));
                  hConnection2 = TML_HANDLE_TYPE_NULL;
                  m_iErr = tml_Core_Connect(getCore(0), sAddress2, &hConnection2);
                  checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress2, tmlrtT(")")), true);
/*
                  stopListener(1, 1);
                  TmlSleep(100);  // <-- Bug: won't reconnect without sleep!
                  startListener(1, 1);
                  TML_BOOL bConnected = TML_FALSE;
                  m_iErr = tml_Connection_Validate(hConnection2, TML_TRUE, &bConnected);
                  checkForSuccess(tmlrtT("tml_Connection_Validate(Connection2)"));
                  checkForValue(tmlrtT("tml_Connection_Validate(Connection2)"), TML_TRUE, bConnected, false);
*/
                  // !!! workaraound !!!

                  // Test getting remote profiles...
                  lProfiles = SIDEX_HANDLE_TYPE_NULL;
                  m_iErr = tml_Connection_Get_RemoteProfiles(hConnection2, &lProfiles);
                  if(checkForSuccess(tmlrtT("tml_Connection_Get_RemoteProfiles(Connection2)")))
                  {
                    if(sidex_Variant_List_Check(lProfiles) == SIDEX_TRUE)
                    {
                      SIDEX_INT32 n = getSidexListCount(lProfiles, tmlrtT("(lProfiles)"));
                      if(checkForValue(tmlrtT("Profile count"), 3, n, false))
                      {
                        for(SIDEX_INT32 i = 0; i < n; i++)
                        {
                          SIDEX_TCHAR* sProfile = getSidexListStringItem(lProfiles, i, tmlrtT("(lProfiles)"));
                          if(sProfile)
                          {
                            if(tmlrt_cmp(sProfile, S_IO_PROFILE) != 0)
                            {
                              if(tmlrt_cmp(sProfile, S_IO_PROFILE_TWO) != 0)
                              {
                                if(tmlrt_cmp(sProfile, S_IO_PROFILE_THREE) != 0)
                                {
                                  errorOutput(tmlrt_cat(tmlrtT("Unexpected profile: "), sProfile), false, true);
                                }
                              }
                            }
                            DELETE_STR(sProfile);
                          }
                        }

                        messageOutput(tmlrtT("Test culmination reached!"));
                      }
                      else
                      {
                        for(SIDEX_INT32 i = 0; i < n; i++)
                        {
                          SIDEX_TCHAR* sProfile = getSidexListStringItem(lProfiles, i, tmlrtT("(lProfiles)"));
                          if(sProfile)
                          {
                            indexOutput(tmlrtT("Profile"), i, sProfile);
                            DELETE_STR(sProfile);
                          }
                        }
                      }
                    }
                    else errorOutput(tmlrtT("SidexVariant isn't a list! (Profiles)"), false, false);
                  }

                  if(lProfiles != SIDEX_HANDLE_TYPE_NULL)
                  {
                    sidex_Variant_DecRef(lProfiles);
                    lProfiles = SIDEX_HANDLE_TYPE_NULL;
                  }

                  // Close connection 2...
                  m_iErr = tml_Connection_Close(&hConnection2);
                  checkForSuccess(tmlrtT("tml_Connection_Close(Connection2)"));
                } // connect 0, addr 2

                hConnection2 = TML_HANDLE_TYPE_NULL;

                stopListener(1, 1);

              } // startListener 1, 1

              deleteListener(1, 1);

            } // createListener 1, 1, addr 2

            // Close connection 1...
            m_iErr = tml_Connection_Close(&hConnection1);
            checkForSuccess(tmlrtT("tml_Connection_Close(Connection1)"));
          } // connect 0, addr 1

          hConnection1 = TML_HANDLE_TYPE_NULL;

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress2);
    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testValidate()
{
  setTestSectionName(tmlrtT("testValidate"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort    = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard    = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    if(createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress))
      {
        if(startListener(1, 0))
        {
          TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
          TML_BOOL              bConnected  = TML_FALSE;

          // Test with missing connection handle...
          m_iErr = tml_Connection_Validate(TML_HANDLE_TYPE_NULL, TML_FALSE, &bConnected);
          checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_Validate(NoConnectionHandle)"));

          // connect
          m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress, tmlrtT(")")), true))
          {
            // Test with missing parameter...
            m_iErr = tml_Connection_Validate(hConnection, TML_FALSE, NULL);
            checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_Validate(NoConnectedParameter)"));

            // Test for established connection...
            bConnected = TML_FALSE;
            m_iErr = tml_Connection_Validate(hConnection, TML_FALSE, &bConnected);
            checkForSuccess(tmlrtT("tml_Connection_Validate(Connection, NoReconnect, True)"));
            checkForValue(tmlrtT("tml_Connection_Validate(Connection, NoReconnect, True)"), TML_TRUE, bConnected, false);

            // Test for lost connection...
            stopListener(1, 0);
            TmlSleep(100);  // wait a little bit for disconnection
            bConnected = TML_TRUE;
            m_iErr = tml_Connection_Validate(hConnection, TML_FALSE, &bConnected);
            checkForSuccess(tmlrtT("tml_Connection_Validate(Connection, NoReconnect, False)"));
            checkForValue(tmlrtT("tml_Connection_Validate(Connection, NoReconnect, False)"), TML_FALSE, bConnected, false);

            // Test for reconnection...
            startListener(1, 0);
            bConnected = TML_FALSE;
            m_iErr = tml_Connection_Validate(hConnection, TML_TRUE, &bConnected);
            if(checkForSuccess(tmlrtT("tml_Connection_Validate(Connection, Connect, True)")))
            {
              if(checkForValue(tmlrtT("tml_Connection_Validate(Connection, Connect, True)"), TML_TRUE, bConnected, false))
              {
                messageOutput(tmlrtT("Test culmination reached!"));
              }
            }
          }

          if(hConnection != TML_HANDLE_TYPE_NULL)
          {
            // Close connection...
            m_iErr = tml_Connection_Close(&hConnection);
            checkForSuccess(tmlrtT("tml_Connection_Close(Connection)"));
            hConnection = TML_HANDLE_TYPE_NULL;
          } // connect 0, addr

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testGetConnectionCount()
{
  setTestSectionName(tmlrtT("testGetConnectionCount"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort     = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard     = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress1 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    SIDEX_TCHAR* sAddress2 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    if(createCore(0) && createCore(1))
    {
      // Test with missing core handle...
      TML_UINT32 iCount = 0;
      m_iErr = tml_Core_Get_ConnectionCount(TML_HANDLE_TYPE_NULL, &iCount);
      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Get_ConnectionCount(NoCoreHandle)"));

      // Test with missing core handle...
      m_iErr = tml_Core_Get_ConnectionCount(getCore(0), NULL);
      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Get_ConnectionCount(Core0, NoCountParameter)"));

      // The initial connection count has to be 0...
      checkConnectionCount(0, 0, tmlrtT(" (start)"));

      if(createListener(1, 0, sAddress1))
      {
        if(startListener(1, 0))
        {
          // initial there has to be no connection...
          checkConnectionCount(0, 0, tmlrtT(" (after listener start)"));

          // make first connection...
          TML_CONNECTION_HANDLE hConnection1 = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress1, &hConnection1);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress1, tmlrtT(")")), true))
          {
            // The connection count has to be 1 now...
            if(checkConnectionCount(0, 1, tmlrtT(" (after first connect)")))
            {
              if(createListener(1, 1, sAddress2))
              {
                if(startListener(1, 1))
                {
                  // make second connection...
                  TML_CONNECTION_HANDLE hConnection2 = TML_HANDLE_TYPE_NULL;
                  m_iErr = tml_Core_Connect(getCore(0), sAddress2, &hConnection2);
                  if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress2, tmlrtT(")")), true))
                  {
                    // The connection count has to be 2 now...
                    if(checkConnectionCount(0, 2, tmlrtT(" (after second connect)")))
                    {
                      messageOutput(tmlrtT("Test culmination reached!"));

                      m_iErr = tml_Connection_Close(&hConnection2);
                      checkForSuccess(tmlrtT("tml_Connection_Close(Connection2)"));
                      hConnection2 = TML_HANDLE_TYPE_NULL;
                    }
                  }

                  stopListener(1, 1);

                } // startListener 1, 1

                deleteListener(1, 1);

              } // createListener 1, 1, addr 2

              m_iErr = tml_Connection_Close(&hConnection1);
              checkForSuccess(tmlrtT("tml_Connection_Close(Connection1)"));
              hConnection1 = TML_HANDLE_TYPE_NULL;

            } // checkConnectionCount 0, 1
          } // connect 0, addr 1

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress2);
    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testGetConnection_Core()
{
  setTestSectionName(tmlrtT("testGetConnection_Core"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort     = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard     = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress1 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    SIDEX_TCHAR* sAddress2 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    if(createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress1))
      {
        if(startListener(1, 0))
        {
          // make first connection...
          TML_CONNECTION_HANDLE hConnection1 = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress1, &hConnection1);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress1, tmlrtT(")")), true))
          {
            if(createListener(1, 1, sAddress2))
            {
              if(startListener(1, 1))
              {
                // make second connection...
                TML_CONNECTION_HANDLE hConnection2 = TML_HANDLE_TYPE_NULL;
                m_iErr = tml_Core_Connect(getCore(0), sAddress2, &hConnection2);
                if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress2, tmlrtT(")")), true))
                {
                  // The connection count has to be 2 now...
                  if(checkConnectionCount(0, 2, tmlrtT(" (two connections)")))
                  {
                    // Test with missing core handle...
                    TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                    m_iErr = tml_Core_Get_Connection(TML_HANDLE_TYPE_NULL, 0, &hConnection);
                    checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Get_Connection(NoCoreHandle)"));

                    // Test with missing connection parameter...
                    m_iErr = tml_Core_Get_Connection(getCore(0), 0, NULL);
                    checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Get_Connection(Core0, NoConnectionParameter)"));

                    // Test with wrong index...
                    m_iErr = tml_Core_Get_Connection(getCore(0), 99, &hConnection);
                    checkForExpectedReturnCode(TML_ERR_INFORMATION_UNDEFINED, tmlrtT("tml_Core_Get_Connection(IndexOutOfRange)"));

                    // Test the connection handles...
                    hConnection = TML_HANDLE_TYPE_NULL;
                    m_iErr = tml_Core_Get_Connection(getCore(0), 0, &hConnection);
                    if(checkForSuccess(tmlrtT("tml_Core_Get_Connection(0)")))
                    {
                      bool bFound1 = (hConnection == hConnection1);
                      bool bFound2 = (hConnection == hConnection2);
                      if(bFound1 || bFound2)
                      {
                        hConnection = TML_HANDLE_TYPE_NULL;
                        m_iErr = tml_Core_Get_Connection(getCore(0), 1, &hConnection);
                        if(checkForSuccess(tmlrtT("tml_Core_Get_Connection(1)")))
                        {
                          if(bFound1) bFound2 = (hConnection == hConnection2);
                          else        bFound1 = (hConnection == hConnection1);

                          if(bFound1 && bFound2) messageOutput(tmlrtT("Test culmination reached!"));
                          else errorOutput(tmlrtT("Second connection not found!"), false, false);
                        }
                      }
                      else errorOutput(tmlrtT("First connection not found!"), false, false);
                    }
                  }

                  m_iErr = tml_Connection_Close(&hConnection2);
                  checkForSuccess(tmlrtT("tml_Connection_Close(Connection2)"));
                  hConnection2 = TML_HANDLE_TYPE_NULL;
                }

                stopListener(1, 1);

              } // startListener 1, 1

              deleteListener(1, 1);

            } // createListener 1, 1, addr 2

            m_iErr = tml_Connection_Close(&hConnection1);
            checkForSuccess(tmlrtT("tml_Connection_Close(Connection1)"));
            hConnection1 = TML_HANDLE_TYPE_NULL;
          } // connect 0, addr 1

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress2);
    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testGetConnectionByAddress()
{
  setTestSectionName(tmlrtT("testGetConnectionByAddress"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort     = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard     = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress1 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    SIDEX_TCHAR* sAddress2 = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    if(createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress1))
      {
        if(startListener(1, 0))
        {
          // make first connection...
          TML_CONNECTION_HANDLE hConnection1 = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress1, &hConnection1);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress1, tmlrtT(")")), true))
          {
            if(createListener(1, 1, sAddress2))
            {
              if(startListener(1, 1))
              {
                // make second connection...
                TML_CONNECTION_HANDLE hConnection2 = TML_HANDLE_TYPE_NULL;
                m_iErr = tml_Core_Connect(getCore(0), sAddress2, &hConnection2);
                if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress2, tmlrtT(")")), true))
                {
                  // The connection count has to be 2 now...
                  if(checkConnectionCount(0, 2, tmlrtT(" (two connections)")))
                  {
                    // Test with missing core handle...
                    TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                    m_iErr = tml_Core_Get_ConnectionByAddress(TML_HANDLE_TYPE_NULL, sAddress1, &hConnection);
                    checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Get_ConnectionByAddress(NoCoreHandle)"));

                    // Test with missing address parameter...
                    m_iErr = tml_Core_Get_ConnectionByAddress(getCore(0), NULL, &hConnection);
                    checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Core_Get_ConnectionByAddress(NoAddressParameter)"));

                    // Test with missing connection parameter...
                    m_iErr = tml_Core_Get_ConnectionByAddress(getCore(0), sAddress1, NULL);
                    checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Get_ConnectionByAddress(NoConnectionParameter)"));

                    // Test with wrong address...
                    m_iErr = tml_Core_Get_ConnectionByAddress(getCore(0), tmlrtT("This is an invalid address!"), &hConnection);
                    checkForExpectedReturnCode(TML_ERR_INFORMATION_UNDEFINED, tmlrtT("tml_Core_Get_ConnectionByAddress(WrongAddress)"));

                    // Test the connection handles...
                    hConnection = TML_HANDLE_TYPE_NULL;
                    m_iErr = tml_Core_Get_ConnectionByAddress(getCore(0), sAddress1, &hConnection);
                    if(checkForSuccess(tmlrtT("tml_Core_Get_Connection(Address1)")))
                    {
                      if(hConnection == hConnection1)
                      {
                        hConnection = TML_HANDLE_TYPE_NULL;
                        m_iErr = tml_Core_Get_ConnectionByAddress(getCore(0), sAddress2, &hConnection);
                        if(checkForSuccess(tmlrtT("tml_Core_Get_Connection(Address2)")))
                        {
                          if(hConnection == hConnection2) messageOutput(tmlrtT("Test culmination reached!"));
                          else errorOutput(tmlrtT("Connection2 not correct!"), false, false);
                        }
                      }
                      else errorOutput(tmlrtT("Connection1 not correct!"), false, false);
                    }
                  }

                  m_iErr = tml_Connection_Close(&hConnection2);
                  checkForSuccess(tmlrtT("tml_Connection_Close(Connection2)"));
                  hConnection2 = TML_HANDLE_TYPE_NULL;
                }

                stopListener(1, 1);

              } // startListener 1, 1

              deleteListener(1, 1);

            } // createListener 1, 1, addr 2

            m_iErr = tml_Connection_Close(&hConnection1);
            checkForSuccess(tmlrtT("tml_Connection_Close(Connection1)"));
            hConnection1 = TML_HANDLE_TYPE_NULL;
          } // connect 0, addr 1

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress2);
    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

void TmlConnectionTester::OnDisConnectionCallback(TML_CONNECTION_HANDLE hConnection, cbData_t* cbData)
{
  if(cbData)
  {
    SIDEX_INT32 id = cbData->iValue;

    switch(cbData->type)
    {
      case cbtOnConnect:    m_cbLog_Connection    |= id; break;
      case cbtOnDisconnect: m_cbLog_Disconnection |= id; break;
    }

    if(enterGlobalMutex())
    {
      wcout << "OnDisConnectionCallback: Callback " << cbt_Name[cbData->type] << " (ID = " << id << ") ";
      if(cbData->sValue) wcout << cbData->sValue;
      wcout << endl;
      leaveGlobalMutex();
    }
  }
}

bool TmlConnectionTester::testSetOnConnect()
{
  setTestSectionName(tmlrtT("testSetOnConnect"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort    = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard    = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    cbData_t* pData1 = prepareCallbackData(this, 0, cbtOnConnect);
    cbData_t* pData2 = prepareCallbackData(this, 1, cbtOnConnect);

    if(pData1 && pData2 && createCore(0) && createCore(1))
    {
      // Test with invalid core handle...
      m_iErr = tml_Core_Set_OnConnect(TML_HANDLE_TYPE_NULL, &CallbackHandler_DisConnection, pData1);
      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Set_OnConnect(NoCoreHandle)"));

      m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE);
      if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core1, "), S_IO_PROFILE, tmlrtT(")")), true))
      {
        if(createListener(1, 0, sAddress))
        {
          if(startListener(1, 0))
          {
            // Register OnConnection callbacks...
            m_iErr = tml_Core_Set_OnConnect(getCore(0), &CallbackHandler_DisConnection, pData1);
            if(checkForSuccess(tmlrtT("tml_Core_Set_OnConnect(Core0, CB, Data1)")))
            {
              m_iErr = tml_Core_Set_OnConnect(getCore(1), &CallbackHandler_DisConnection, pData2);
              if(checkForSuccess(tmlrtT("tml_Core_Set_OnConnect(Core1, CB, Data2)")))
              {
                m_cbLog_Connection = 0;

                pData1->iValue = 1; pData1->sValue = tmlrtT("Data1");
                pData2->iValue = 2; pData2->sValue = tmlrtT("Data2");

                // Test with connection...
                TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
                if(checkForSuccess(tmlrtT("tml_Core_Connect(Core0, Address)")))
                {
                  // send a command to get receiver triggered...
                  TML_COMMAND_HANDLE hCommand = TML_HANDLE_TYPE_NULL;
                  m_iErr = tml_Cmd_Create(&hCommand);
                  checkForSuccess(tmlrtT("tml_Cmd_Create()"));
                  m_iErr = tml_Cmd_Header_SetCommand(hCommand, 1234);
                  checkForSuccess(tmlrtT("tml_Cmd_Header_SetCommand(Command, 1234)"));
                  m_iErr = tml_Connection_SendSync(hConnection, S_IO_PROFILE, hCommand, 2000);
                  checkForSuccess(tmlrt_cat(tmlrtT("tml_Connection_SendSync(Connection, "), S_IO_PROFILE, tmlrtT(")")), true);
                  m_iErr = tml_Cmd_Free(&hCommand);
                  checkForSuccess(tmlrtT("tml_Cmd_Free()"));
                  hCommand = TML_HANDLE_TYPE_NULL;

                  // Wait a little bit to get callbacks fired...
                  int msWait = 20;
                  for(int i = 0; i < 1000; i += msWait)
                  {
                    if(m_cbLog_Connection == 3) break;
                    TmlSleep(msWait);
                  }
                  if(checkForValue(tmlrtT("Received callback log"), 3, m_cbLog_Connection, false))
                  {
                    messageOutput(tmlrtT("Test culmination reached!"));
                  }

                  // Close connection...
                  m_iErr = tml_Connection_Close(&hConnection);
                  checkForSuccess(tmlrtT("tml_Connection_Close(Connection)"));
                }

                // Unregister callbacks...
                m_iErr = tml_Core_Set_OnConnect(getCore(1), NULL, NULL);
                checkForSuccess(tmlrtT("tml_Core_Set_OnConnect(Core1, NULL, NULL)"));
              }
              m_iErr = tml_Core_Set_OnConnect(getCore(0), NULL, NULL);
              checkForSuccess(tmlrtT("tml_Core_Set_OnConnect(Core0, NULL, NULL)"));
            }

            stopListener(1, 0);

          } // startListener 1, 0

          deleteListener(1, 0);

        } // createListener 1, 0, addr 1
      } // register profile on core 0
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

bool TmlConnectionTester::testSetOnDisconnect()
{
  setTestSectionName(tmlrtT("testSetOnDisconnect"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort    = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard    = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress = tmlrt_cat(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    cbData_t* pData1 = prepareCallbackData(this, 0, cbtOnDisconnect);
    cbData_t* pData2 = prepareCallbackData(this, 1, cbtOnDisconnect);

    if(pData1 && pData2 && createCore(0) && createCore(1))
    {
      // Test with invalid core handle...
      m_iErr = tml_Core_Set_OnDisconnect(TML_HANDLE_TYPE_NULL, &CallbackHandler_DisConnection, pData1);
      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Set_OnDisconnect(NoCoreHandle)"));

      m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE);
      if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core1, "), S_IO_PROFILE, tmlrtT(")")), true))
      {
        if(createListener(1, 0, sAddress))
        {
          if(startListener(1, 0))
          {
            // Register OnDisconnection callbacks...
            m_iErr = tml_Core_Set_OnDisconnect(getCore(0), &CallbackHandler_DisConnection, pData1);
            if(checkForSuccess(tmlrtT("tml_Core_Set_OnDisconnect(Core0, CB, Data1)")))
            {
              m_iErr = tml_Core_Set_OnDisconnect(getCore(1), &CallbackHandler_DisConnection, pData2);
              if(checkForSuccess(tmlrtT("tml_Core_Set_OnDisconnect(Core1, CB, Data2)")))
              {
                m_cbLog_Disconnection = 0;

                pData1->iValue = 1; pData1->sValue = tmlrtT("Data1");
                pData2->iValue = 2; pData2->sValue = tmlrtT("Data2");

                // Test with connection...
                TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
                if(checkForSuccess(tmlrtT("tml_Core_Connect(Core0, Address)")))
                {
                  // send a command to get receiver triggered...
                  TML_COMMAND_HANDLE hCommand = TML_HANDLE_TYPE_NULL;
                  m_iErr = tml_Cmd_Create(&hCommand);
                  checkForSuccess(tmlrtT("tml_Cmd_Create()"));
                  m_iErr = tml_Cmd_Header_SetCommand(hCommand, 1234);
                  checkForSuccess(tmlrtT("tml_Cmd_Header_SetCommand(Command, 1234)"));
                  m_iErr = tml_Connection_SendSync(hConnection, S_IO_PROFILE, hCommand, 2000);
                  checkForSuccess(tmlrt_cat(tmlrtT("tml_Connection_SendSync(Connection, "), S_IO_PROFILE, tmlrtT(")")), true);
                  m_iErr = tml_Cmd_Free(&hCommand);
                  checkForSuccess(tmlrtT("tml_Cmd_Free()"));
                  hCommand = TML_HANDLE_TYPE_NULL;

                  // Close connection...
                  m_iErr = tml_Connection_Close(&hConnection);
                  checkForSuccess(tmlrtT("tml_Connection_Close(Connection)"));

                  // Wait a little bit to get callbacks fired...
                  int msWait = 20;
                  for(int i = 0; i < 1000; i += msWait)
                  {
                    if(m_cbLog_Disconnection == 3) break;
                    TmlSleep(msWait);
                  }
                  if(checkForValue(tmlrtT("Received callback log"), 3, m_cbLog_Disconnection, false))
                  {
                    messageOutput(tmlrtT("Test culmination reached!"));
                  }
                }

                // Unregister callbacks...
                m_iErr = tml_Core_Set_OnDisconnect(getCore(1), NULL, NULL);
                checkForSuccess(tmlrtT("tml_Core_Set_OnDisconnect(Core1, NULL, NULL)"));
              }
              m_iErr = tml_Core_Set_OnDisconnect(getCore(0), NULL, NULL);
              checkForSuccess(tmlrtT("tml_Core_Set_OnDisconnect(Core0, NULL, NULL)"));
            }

            stopListener(1, 0);

          } // startListener 1, 0

          deleteListener(1, 0);

        } // createListener 1, 0, addr 1
      } // register profile on core 0
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

void TmlConnectionTester::OnCommandCallback(TML_COMMAND_HANDLE hCommand, cbData_t* cbData)
{
  if(cbData)
  {
    SIDEX_INT32 id = cbData->iValue;

    switch(cbData->type)
    {
      case cbtOnCommand:
      {
        id = getCommandInt(hCommand, S_GROUP_TEST, S_KEY_INDEX, 0);
        m_cbLog_Command |= id;
        break;
      }
    }

    if(enterGlobalMutex())
    {
      wcout << "OnCommandCallback: Callback " << cbt_Name[cbData->type] << " (ID = " << id << ") ";
      if(cbData->sValue) wcout << cbData->sValue;
      wcout << endl;
      leaveGlobalMutex();
    }
  }
}

bool TmlConnectionTester::testSendSync()
{
  setTestSectionName(tmlrtT("testSendSync"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    SIDEX_TCHAR* sAddress = tmlrt_cat(TestParams->getNetworkCard(0),
                                      tmlrtT(":"),
                                      tmlrt_itoa(TestParams->getFirstPortNumber()), 5);
    cbData_t* pData = prepareCallbackData(this, 1, cbtOnCommand);
    if(pData && createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress))
      {
        if(startListener(1, 0))
        {
          TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress, tmlrtT(")")), true))
          {
            // Test without profile registration...

            TML_COMMAND_HANDLE hCmdDummy = TML_HANDLE_TYPE_NULL;
            m_iErr = tml_Cmd_Create(&hCmdDummy);
            checkForSuccess(tmlrtT("tml_Cmd_Create(DummyCommand)"));
            m_iErr = tml_Cmd_Header_SetCommand(hCmdDummy, 1234);
            checkForSuccess(tmlrtT("tml_Cmd_Header_SetCommand(DummyCommand, 1234)"));

            // Test with invalid connection handle...
            m_iErr = tml_Connection_SendSync(TML_HANDLE_TYPE_NULL, S_IO_PROFILE, hCmdDummy, 2000);
            checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_SendSync(NoConnectionHandle)"));

            // Test without profile...
            m_iErr = tml_Connection_SendSync(hConnection, NULL, hCmdDummy, 2000);
            checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Connection_SendSync(NoProfileParameter)"));

            // Test with a not registered profile...
            m_iErr = tml_Connection_SendSync(hConnection, S_IO_PROFILE, hCmdDummy, 2000);
            checkForExpectedReturnCode(TML_ERR_SENDER_PROFILE_NOT_SUPPORTED, tmlrtT("tml_Connection_SendSync(ProfileNotExists)"));

            m_iErr = tml_Cmd_Free(&hCmdDummy);
            checkForSuccess(tmlrtT("tml_Cmd_Free(DummyCommand)"));
            hCmdDummy = TML_HANDLE_TYPE_NULL;
            
            // Register profile and command...
            m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE);
            if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core1, "), S_IO_PROFILE, tmlrtT(")")), true))
            {
              m_iErr = tml_Profile_Register_Cmd(getCore(1), S_IO_PROFILE, 1234, &CallbackHandler_Command, pData);
              if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register_Cmd("), S_IO_PROFILE, tmlrtT(", 1234)")), true))
              {
                // !!! workaraound !!!
                // Reconnect - Remote profile list doesn't update, if already connected!
                m_iErr = tml_Connection_Close(&hConnection);
                checkForSuccess(tmlrtT("tml_Connection_Close(NoProfileConnection)"));
                hConnection = TML_HANDLE_TYPE_NULL;
                m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
                checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress, tmlrtT(")")), true);
                // !!! workaraound !!!
            
                // Test without command...
//                m_iErr = tml_Connection_SendSync(hConnection, IO_PROFILE, TML_HANDLE_TYPE_NULL, 2000);
//                checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Connection_SendSync(NoCommand)"));

                // Test sending sync commands...
                m_cbLog_Command = 0;

                bool bStop = false;
                // send 4 commands... (1, 2, 4, 8)
                for(TML_INT32 iCommand = 1; iCommand < 16; iCommand <<= 1)
                {
                  TML_COMMAND_HANDLE hCommand = TML_HANDLE_TYPE_NULL;
                  m_iErr = tml_Cmd_Create(&hCommand);
                  if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Create(Command"),
                                               tmlrt_itoa(iCommand),
                                               tmlrtT(")"), 2), true))
                  {
                    m_iErr = tml_Cmd_Header_SetCommand(hCommand, 1234);
                    if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Header_SetCommand(Command"),
                                                 tmlrt_itoa(iCommand),
                                                 tmlrtT(", 1234)"), 2), true))
                    {
                      if(setCommandInt(hCommand, S_GROUP_TEST, S_KEY_INDEX, iCommand))
                      {
                        m_iErr = tml_Connection_SendSync(hConnection, S_IO_PROFILE, hCommand, 2000);
                        if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Connection_SendSync(Command"),
                                                      tmlrt_itoa(iCommand),
                                                      tmlrtT(")"), 2), true)) bStop = true;
                      }
                    }
                    else bStop = true;

                    // it was a sync call - we can delete the command now...
                    m_iErr = tml_Cmd_Free(&hCommand);
                    if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Free(Command"),
                                                  tmlrt_itoa(iCommand),
                                                  tmlrtT(")"), 2), true)) bStop = true;
                    hCommand = TML_HANDLE_TYPE_NULL;
                  }
                  else bStop = true;
                  if(bStop) break;
                }
            
                if(checkForValue(tmlrtT("Received commands log"), 15, m_cbLog_Command, false))
                {
                  messageOutput(tmlrtT("Test culmination reached!"));
                }

                // Close connection...
                m_iErr = tml_Connection_Close(&hConnection);
                checkForSuccess(tmlrtT("tml_Connection_Close(Connection)"));
              }

              // unregister commands...
              m_iErr = tml_Profile_Unregister(getCore(1), S_IO_PROFILE);
              checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Unregister(Core1, "), S_IO_PROFILE, tmlrtT(")")), true);

            } // register profile, core 1
          } // connect 0, addr 1

          hConnection = TML_HANDLE_TYPE_NULL;

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

void OnAsyncCommandReady(TML_COMMAND_HANDLE hCommand, TML_POINTER pCBData)
{
  tml_Cmd_Free(&hCommand);
  hCommand = TML_HANDLE_TYPE_NULL;
}

bool TmlConnectionTester::testSendAsync()
{
  setTestSectionName(tmlrtT("testSendAsync"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    SIDEX_TCHAR* sAddress = tmlrt_cat(TestParams->getNetworkCard(0),
                                      tmlrtT(":"),
                                      tmlrt_itoa(TestParams->getFirstPortNumber()), 5);
    cbData_t* pData = prepareCallbackData(this, 1, cbtOnCommand);
    if(pData && createCore(0) && createCore(1))
    {
      if(createListener(1, 0, sAddress))
      {
        if(startListener(1, 0))
        {
          TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
          m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress, tmlrtT(")")), true))
          {
            // Test without profile registration...

            TML_COMMAND_HANDLE hCmdDummy = TML_HANDLE_TYPE_NULL;
            m_iErr = tml_Cmd_Create(&hCmdDummy);
            checkForSuccess(tmlrtT("tml_Cmd_Create(DummyCommand)"));
            m_iErr = tml_Cmd_Header_SetCommand(hCmdDummy, 1234);
            checkForSuccess(tmlrtT("tml_Cmd_Header_SetCommand(DummyCommand, 1234)"));

            // Test with invalid connection handle...
            m_iErr = tml_Connection_SendAsync(TML_HANDLE_TYPE_NULL, S_IO_PROFILE, hCmdDummy, 2000);
            checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Connection_SendAsync(NoConnectionHandle)"));

            // Test without profile...
            m_iErr = tml_Connection_SendAsync(hConnection, NULL, hCmdDummy, 2000);
            checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Connection_SendAsync(NoProfileParameter)"));

            // Test with a not registered profile...
            m_iErr = tml_Connection_SendAsync(hConnection, S_IO_PROFILE, hCmdDummy, 2000);
            checkForExpectedReturnCode(TML_ERR_SENDER_PROFILE_NOT_SUPPORTED, tmlrtT("tml_Connection_SendAsync(ProfileNotExists)"));

            m_iErr = tml_Cmd_Free(&hCmdDummy);
            checkForSuccess(tmlrtT("tml_Cmd_Free(DummyCommand)"));
            hCmdDummy = TML_HANDLE_TYPE_NULL;
            
            // Register profile and command...
            m_iErr = tml_Profile_Register(getCore(1), S_IO_PROFILE);
            if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core1, "), S_IO_PROFILE, tmlrtT(")")), true))
            {
              m_iErr = tml_Profile_Register_Cmd(getCore(1), S_IO_PROFILE, 1234, &CallbackHandler_Command, pData);
              if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register_Cmd("), S_IO_PROFILE, tmlrtT(", 1234)")), true))
              {
                // !!! workaraound !!!
                // Reconnect - Remote profile list doesn't update, if already connected!
                m_iErr = tml_Connection_Close(&hConnection);
                checkForSuccess(tmlrtT("tml_Connection_Close(NoProfileConnection)"));
                hConnection = TML_HANDLE_TYPE_NULL;
                m_iErr = tml_Core_Connect(getCore(0), sAddress, &hConnection);
                checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect("), sAddress, tmlrtT(")")), true);
                // !!! workaraound !!!
            
                // Test without command...
//                m_iErr = tml_Connection_SendAsync(hConnection, IO_PROFILE, TML_HANDLE_TYPE_NULL, 2000);
//                checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Connection_SendAsync(NoCommand)"));

                // Test sending async commands...
                m_cbLog_Command = 0;

                bool bStop = false;
                // send 4 commands... (1, 2, 4, 8)
                for(TML_INT32 iCommand = 1; iCommand < 16; iCommand <<= 1)
                {
                  TML_COMMAND_HANDLE hCommand = TML_HANDLE_TYPE_NULL;
                  m_iErr = tml_Cmd_Create(&hCommand);
                  if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Create(Command"),
                                               tmlrt_itoa(iCommand),
                                               tmlrtT(")"), 2), true))
                  {
                    m_iErr = tml_Cmd_Register_CommandReady(hCommand, &OnAsyncCommandReady, NULL);
                    if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Register_CommandReady(Command"),
                                                 tmlrt_itoa(iCommand),
                                                 tmlrtT(")"), 2), true))
                    {
                      m_iErr = tml_Cmd_Header_SetCommand(hCommand, 1234);
                      if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Header_SetCommand(Command"),
                                                   tmlrt_itoa(iCommand),
                                                   tmlrtT(", 1234)"), 2), true))
                      {
                        if(setCommandInt(hCommand, S_GROUP_TEST, S_KEY_INDEX, iCommand))
                        {
                          m_iErr = tml_Connection_SendAsync(hConnection, S_IO_PROFILE, hCommand, 2000);
                          if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Connection_SendAsync(Command"),
                                                        tmlrt_itoa(iCommand),
                                                        tmlrtT(")"), 2), true)) bStop = true;
                        }
                      }
                      else bStop = true;
                    }
                    else bStop = true;

                    // it was an async call - we mustn't delete the command here!
                    hCommand = TML_HANDLE_TYPE_NULL;
                  }
                  else bStop = true;
                  if(bStop) break;
                }

                // wait a little bit for processing of async commands...
                int msWait = 20;
                for(int i = 0; i < 1000; i += msWait)
                {
                  if(m_cbLog_Command == 15) break;
                  TmlSleep(msWait);
                }

                if(checkForValue(tmlrtT("Received commands log"), 15, m_cbLog_Command, false))
                {
                  messageOutput(tmlrtT("Test culmination reached!"));
                }

                // Close connection...
                m_iErr = tml_Connection_Close(&hConnection);
                checkForSuccess(tmlrtT("tml_Connection_Close(Connection)"));
              }

              // unregister commands...
              m_iErr = tml_Profile_Unregister(getCore(1), S_IO_PROFILE);
              checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Unregister(Core1, "), S_IO_PROFILE, tmlrtT(")")), true);

            } // register profile, core 1
          } // connect 0, addr 1

          hConnection = TML_HANDLE_TYPE_NULL;

          stopListener(1, 0);

        } // startListener 1, 0

        deleteListener(1, 0);

      } // createListener 1, 0, addr 1
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    DELETE_STR(sAddress);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

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