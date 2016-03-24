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

TmlConnectionTester::TmlConnectionTester(SIDEX_TCHAR* testProcessName)
                   : TmlTester(testProcessName)
{
}

TmlConnectionTester::~TmlConnectionTester()
{
}

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
    SIDEX_TCHAR* sAddress1 = tmlrt_cat_del(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    SIDEX_TCHAR* sAddress2 = tmlrt_cat_del(sCard, tmlrtT(":"), tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    if(createCore(0))
    {
      if(createCore(1))
      {
        TML_CONNECTION_HANDLE hConnection1, hConnection2;

        // The initial connection count has to be 0...
        checkConnectionCount(0, 0, tmlrtT(" (start)"));

        // Test with invalid core handle...
        hConnection1 = TML_HANDLE_TYPE_NULL;
        m_iErr = tml_Core_Connect(TML_HANDLE_TYPE_NULL, sAddress1, &hConnection1);
        checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Core_Connect(NoCoreHandle)"));

        checkConnectionCount(0, 0, tmlrtT(" (after NoCoreHandle)"));
/*
        // Test with invalid address...
        hConnection1 = TML_HANDLE_TYPE_NULL;
        m_iErr = tml_Core_Connect(getCore(0), tmlrtT("This is an invalid address!"), &hConnection1);
        checkForExpectedReturnCode(TML_ERR_NET_BINDING, tmlrtT("tml_Core_Connect(InvalidAddress)"));

        checkConnectionCount(0, 0, tmlrtT(" (after InvalidAddress)"));

        // Test with invalid connection handle parameter...
        m_iErr = tml_Core_Connect(getCore(0), sAddress1, NULL);
        checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Core_Connect(NoConnectionHandleParameter)"));

        checkConnectionCount(0, 0, tmlrtT(" (after NoConnectionHandleParameter)"));

        // Test connection without listener...
        hConnection1 = TML_HANDLE_TYPE_NULL;
        m_iErr = tml_Core_Connect(getCore(0), sAddress1, &hConnection1);
        checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS, tmlrtT("tml_Core_Connect(NoListener)"));

        checkConnectionCount(0, 0, tmlrtT(" (after NoListener)"));
*/
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

        deleteCore(1);

      } // createCore 1

      deleteCore(0);

    } // createCore 0

    DELETE_STR(sAddress2);
    DELETE_STR(sAddress1);

  } // network card count > 0

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

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    SIDEX_TCHAR* sAddress = tmlrt_cat_del(TestParams->getNetworkCard(0),
                                          tmlrtT(":"),
                                          tmlrt_itoa(TestParams->getFirstPortNumber()), 5);
    if(createCore(0))
    {
      if(createCore(1))
      {
        TML_CONNECTION_HANDLE hConnection;

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
                hConnection = TML_HANDLE_TYPE_NULL;

              } // checkConnectionCount 0, 1
            } // connect 0, addr 1

            stopListener(1, 0);

          } // startListener 1, 0

          deleteListener(1, 0);

        } // createListener 1, 0, addr 1

        deleteCore(1);

      } // createCore 1

      deleteCore(0);

    } // createCore 0

    DELETE_STR(sAddress);

  } // network card count > 0

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
