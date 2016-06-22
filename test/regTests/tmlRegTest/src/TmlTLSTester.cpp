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

#include "TmlTLSTester.h"
#include "TestParams.h"

//------------------------------------------------------------------------------

cbData_t_TLS cbData_TLS[MAX_CORES];

//------------------------------------------------------------------------------

volatile int  g_TlsErrorDetector = 0;     // bit field 1=Core0, 2=Core1, 4=Core2, ...
volatile int  g_TlsCertSwitch    = 0;     // 0=Crt1/Key1, 1=Crt1/Key2, 2=Crt2/Key1, 3=Crt2/Key2, 4=None
volatile bool g_TlsHideErrorMsg  = false;
volatile int  g_TlsIdleCounter   = 0;

//------------------------------------------------------------------------------

SIDEX_TCHAR* S_DEFAULT_DIGEST_SHA1 = tmlrtT("51:DE:5D:75:C5:D0:28:B5:53:86:EB:60:8A:FB:0A:68:34:66:93:B2");
SIDEX_TCHAR* S_DEFAULT_DIGEST_MD5  = tmlrtT("91:59:49:7A:61:8A:DB:6F:F1:90:FB:0E:9B:4B:74:89");
SIDEX_TCHAR* S_REGTEST_DIGEST_SHA1 = tmlrtT("F5:7F:2B:98:0D:60:DF:E0:49:A8:1F:A0:17:AB:C9:F3:E2:2B:32:4D");
SIDEX_TCHAR* S_REGTEST_DIGEST_MD5  = tmlrtT("FA:2B:20:DF:FE:BF:DF:15:71:E4:43:F1:41:2D:EE:45");

//------------------------------------------------------------------------------

cbData_t_TLS* prepareCallbackData_TLS(TmlTLSTester* tester, int iCore)
{
  cbData_t_TLS* result = NULL;
  if((iCore >= 0) && (iCore < MAX_CORES))
  {
    result = &cbData_TLS[iCore];
    result->tester        = tester;
    result->iCore         = iCore;
    result->iValue        = 0;
    result->sValue        = NULL;
    result->bDeleteString = false;
  }
  return(result);
}

//------------------------------------------------------------------------------

void CallbackHandler_TLS(TML_COMMAND_HANDLE hCommand, TML_POINTER pCBData)
{
  g_TlsIdleCounter = 0;
  cbData_t_TLS* data = (cbData_t_TLS*)pCBData;
  if(data)
  {
    if(data->tester)
    {
      data->tester->OnCommandCallback(hCommand, data);
    }
    else
    {
      if(enterGlobalMutex())
      {
        wcout << "CallbackHandler_Command: no Tester!" << endl;
        leaveGlobalMutex();
      }
    }
    if(data->bDeleteString) DELETE_STR(data->sValue);
  }
  else
  {
    if(enterGlobalMutex())
    {
      wcout << "CallbackHandler_Command: no CB-Data!" << endl;
      leaveGlobalMutex();
    }
  }
}

//------------------------------------------------------------------------------

TmlTLSTester::TmlTLSTester(SIDEX_TCHAR* testProcessName)
            : TmlTester(testProcessName)
{
  m_cbLog_Command = 0;
}

//------------------------------------------------------------------------------

TmlTLSTester::~TmlTLSTester()
{
}

//------------------------------------------------------------------------------

void TmlTLSTester::_prepare()
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

//------------------------------------------------------------------------------

void TmlTLSTester::_cleanup()
{

  TmlTester::_cleanup();
}

//------------------------------------------------------------------------------

void TmlTLSTester::OnCommandCallback(TML_COMMAND_HANDLE hCommand, cbData_t_TLS* cbData)
{
  g_TlsIdleCounter = 0;
  if(cbData)
  {
    SIDEX_INT32 id = getCommandInt(hCommand, S_GROUP_TEST, S_KEY_INDEX, cbData->iValue);

    if(enterGlobalMutex())
    {
      wcout << S_CALLBACK_PREFIX << "OnCommandCallback: (ID = " << id << S_PARENTHESIS_R;
      if(cbData->sValue) wcout << S_SPACE << cbData->sValue;
      wcout << endl;
      leaveGlobalMutex();
    }

    m_cbLog_Command |= id;
  }
}

//------------------------------------------------------------------------------

TML_BOOL CallbackHandler_OnAcceptTlsRequest(SIDEX_VARIANT vServerName)
{
  SIDEX_TCHAR* sServerName = NULL;
  SIDEX_INT32  iLength     = 0;

  g_TlsIdleCounter = 0;

  sidex_Variant_As_String(vServerName, &sServerName, &iLength);
  if(!sServerName || !sServerName[0]) sServerName = S_UNNAMED;

  TML_BOOL bAccept = (tmlrt_cmp(sServerName, S_DESTINATION_HOST_IP) ? TML_FALSE : TML_TRUE);

  if(enterGlobalMutex())
  {
    wcout << S_CALLBACK_PREFIX << "OnAcceptTlsRequest(" << S_QUOTE << sServerName << S_QUOTE << S_PARENTHESIS_R
          << " : Accept = " << (bAccept ? S_BOOL_TRUE : S_BOOL_FALSE) << endl;
    if(!bAccept)
    {
      g_TlsErrorDetector |= (1 << 3);
      wcout << "Wrong server name! Expected " << S_QUOTE << S_DESTINATION_HOST_IP << S_QUOTE
            << ", but got " << S_QUOTE << sServerName << S_QUOTE << endl;
    }
    leaveGlobalMutex();
  }

  return(TestParams->isActingAsRepeater() ? TML_TRUE : bAccept);
}

//------------------------------------------------------------------------------

SIDEX_VARIANT CallbackHandler_OnCertificateLocation(SIDEX_VARIANT vServerName)
{
  SIDEX_TCHAR* sServerName = NULL;
  SIDEX_INT32  iLength     = 0;

  g_TlsIdleCounter = 0;

  sidex_Variant_As_String(vServerName, &sServerName, &iLength);
  if(!sServerName || !sServerName[0]) sServerName = S_UNNAMED;

  SIDEX_TCHAR* sResult = NULL;
  if(!(g_TlsCertSwitch & 4))
  {
    sResult = tmlrt_delimiters(tmlrt_cat(TestParams->getTlsLocation(), S_SLASH,
                                         (g_TlsCertSwitch & 2) ? TestParams->getTlsCrt2()
                                                               : TestParams->getTlsCrt(), 5), true);
  }

  if(enterGlobalMutex())
  {
    wcout << S_CALLBACK_PREFIX << "OnCertificateLocation(" << S_QUOTE << sServerName << S_QUOTE << S_PARENTHESIS_R
          << " : Path = " << S_QUOTE << sResult << S_QUOTE << endl;
    if(tmlrt_cmp(sServerName, S_DESTINATION_HOST_IP) != 0)
    {
      g_TlsErrorDetector |= (1 << 4);
      wcout << "Wrong server name! Expected " << S_QUOTE << S_DESTINATION_HOST_IP << S_QUOTE
            << ", but got " << S_QUOTE << sServerName << S_QUOTE << endl;
    }
    leaveGlobalMutex();
  }

  SIDEX_VARIANT vResult = SIDEX_HANDLE_TYPE_NULL;
  if(sResult)
  {
    sidex_Variant_New_String(sResult, &vResult);
    DELETE_STR(sResult);
  }
  return(vResult);
}

//------------------------------------------------------------------------------

SIDEX_VARIANT CallbackHandler_OnPrivateKeyLocation(SIDEX_VARIANT vServerName)
{
  SIDEX_TCHAR* sServerName = NULL;
  SIDEX_INT32  iLength     = 0;

  g_TlsIdleCounter = 0;

  sidex_Variant_As_String(vServerName, &sServerName, &iLength);
  if(!sServerName || !sServerName[0]) sServerName = S_UNNAMED;

  SIDEX_TCHAR* sResult = NULL;
  if(!(g_TlsCertSwitch & 4))
  {
    sResult = tmlrt_delimiters(tmlrt_cat(TestParams->getTlsLocation(), S_SLASH,
                                         (g_TlsCertSwitch & 1) ? TestParams->getTlsKey2()
                                                               : TestParams->getTlsKey(), 5), true);
  }

  if(enterGlobalMutex())
  {
    wcout << S_CALLBACK_PREFIX << "OnPrivateKeyLocation(" << S_QUOTE << sServerName << S_QUOTE << S_PARENTHESIS_R
          << " : Path = " << S_QUOTE << sResult << S_QUOTE << endl;
    if(tmlrt_cmp(sServerName, S_DESTINATION_HOST_IP) != 0)
    {
      g_TlsErrorDetector |= (1 << 5);
      wcout << "Wrong server name! Expected " << S_QUOTE << S_DESTINATION_HOST_IP << S_QUOTE
            << ", but got " << S_QUOTE << sServerName << S_QUOTE << endl;
    }
    leaveGlobalMutex();
  }

  SIDEX_VARIANT vResult = SIDEX_HANDLE_TYPE_NULL;
  if(sResult)
  {
    sidex_Variant_New_String(sResult, &vResult);
    DELETE_STR(sResult);
  }
  return(vResult);
}

//------------------------------------------------------------------------------

void CallbackHandler_OnTlsError(TML_CONNECTION_HANDLE connection, SIDEX_VARIANT errorMessage, void* pErrorData)
{
  cbData_t_TLS* data = (cbData_t_TLS*)pErrorData;
  if(data)
  {
    g_TlsErrorDetector |= (1 << data->iCore);

    SIDEX_TCHAR* sErrMsg = NULL;
    SIDEX_INT32  iLength = 0;
    sidex_Variant_As_String(errorMessage, &sErrMsg, &iLength);

    if(enterGlobalMutex())
    {
      wcout << S_CALLBACK_PREFIX << "CallbackHandler_OnTlsError: Core" << data->iCore;
      if(!g_TlsHideErrorMsg) wcout << S_SPACE << S_COLON << S_SPACE << sErrMsg;
      wcout << endl;
      leaveGlobalMutex();
    }

    if(data->bDeleteString) DELETE_STR(data->sValue);
  }
  else
  {
    if(enterGlobalMutex())
    {
      wcout << "CallbackHandler_OnTlsError: no data!" << endl;
      leaveGlobalMutex();
    }
  }
}

//------------------------------------------------------------------------------

bool TmlTLSTester::testTLS()
{
  setTestSectionName(tmlrtT("testTLS"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort     = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard     = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress1 = tmlrt_cat(sCard, S_COLON, tmlrt_itoa(iPort), 4);
    SIDEX_TCHAR* sAddress2 = sAddress1;
    SIDEX_TCHAR* sRepeater = TestParams->getTlsRepeaterHost();
    if(sRepeater && sRepeater[0])
    {
      int iRepeaterPort = TestParams->getTlsRepeaterPort();
      if(iRepeaterPort <= 0) iRepeaterPort = iPort;
      sAddress2 = tmlrt_cat(sRepeater, S_COLON, tmlrt_itoa(iRepeaterPort), 4);
    }
    bool isRepeaterTest = (sAddress2 != sAddress1);
    DELETE_STR(sRepeater);
    DELETE_STR(sCard);

    if(isRepeaterTest)
    {
      messageOutput(tmlrt_cat(tmlrtT("Test with repeater: "), sAddress2), true, true, true);
    }

    if(createCore(0) && createCore(1))
    {
      TML_BOOL bAccept = TML_FALSE;
      if(!isRepeaterTest)
      {
        m_iErr = tml_Tls_Core_AcceptNegotiation(TML_HANDLE_TYPE_NULL, NULL, NULL, NULL, &bAccept);
        checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Core_AcceptNegotiation(NoCoreHandle)"));

        m_iErr = tml_Tls_Core_AcceptNegotiation(getCore(0), NULL, NULL, NULL, NULL);
        checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Core_AcceptNegotiation(Core0, NoAcceptParameter)"));

        m_iErr = tml_Tls_Core_Set_OnError(TML_HANDLE_TYPE_NULL, &CallbackHandler_OnTlsError, NULL);
        checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Core_Set_OnError(NoCoreHandle)"));
      }

      cbData_t_TLS* pData0 = prepareCallbackData_TLS(this, 0);
      cbData_t_TLS* pData1 = prepareCallbackData_TLS(this, 1);
      if(pData0 && pData1)
      {
        g_TlsErrorDetector = 0;

        m_iErr = tml_Tls_Core_Set_OnError(getCore(0), &CallbackHandler_OnTlsError, pData0);
        checkForSuccess(tmlrtT("tml_Tls_Core_Set_OnError(Core0)"));

        m_iErr = tml_Tls_Core_Set_OnError(getCore(1), &CallbackHandler_OnTlsError, pData1);
        checkForSuccess(tmlrtT("tml_Tls_Core_Set_OnError(Core1)"));

        // Register profile and command...
        m_iErr = tml_Profile_Register(getCore(0), S_IO_PROFILE);
        if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core0, "), S_IO_PROFILE, S_PARENTHESIS_R), true))
        {
          m_iErr = tml_Profile_Register_Cmd(getCore(0), S_IO_PROFILE, 1234, &CallbackHandler_TLS, pData0);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register_Cmd(Core0, "), S_IO_PROFILE, tmlrtT(", 1234)")), true))
          {
            if(!isRepeaterTest)
            {
              messageOutput(tmlrtT("Test with default certificate..."));
              bAccept = TML_FALSE;
              m_iErr = tml_Tls_Core_AcceptNegotiation(getCore(0), NULL, NULL, NULL, &bAccept);
              if(checkForSuccess(tmlrtT("tml_Tls_Core_AcceptNegotiation(Core0, Defaults)")) &&
                 checkForValue(tmlrtT("tml_Tls_Core_AcceptNegotiation : bAccept (Default)"),
                               SIDEX_INT32(TML_TRUE), SIDEX_INT32(bAccept)))
              {
                if(createListener(0, 0, sAddress1))
                {
                  if(startListener(0, 0))
                  {
                    TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                    m_iErr = tml_Core_Connect(getCore(1), sAddress2, &hConnection);
                    if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect(Core1, "), sAddress2, tmlrtT(") - Default")), true))
                    {
                      TML_BOOL bEncrypted = TML_FALSE;
                      TML_BOOL bVerifyOk  = TML_FALSE;

                      m_iErr = tml_Tls_Connection_StartNegotiation(TML_HANDLE_TYPE_NULL, TML_FALSE, &bEncrypted);
                      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Connection_StartNegotiation(NoConnectionHandle) - Default"));

                      m_iErr = tml_Tls_Connection_StartNegotiation(hConnection, TML_FALSE, NULL);
                      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Connection_StartNegotiation(NoEncryptionParameter) - Default"));

                      m_iErr = tml_Tls_Connection_VerifyCert(TML_HANDLE_TYPE_NULL, &bVerifyOk);
                      checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Connection_VerifyCert(NoConnectionHandle) - Default"));

                      bEncrypted = TML_FALSE;
                      m_iErr = tml_Tls_Connection_StartNegotiation(hConnection, TML_FALSE, &bEncrypted);
                      if(checkForSuccess(tmlrtT("tml_Tls_Connection_StartNegotiation(Connection) - Default")) &&
                         checkForValue(tmlrtT("tml_Tls_Connection_StartNegotiation : bEncrypted (Default)"),
                                       SIDEX_INT32(TML_TRUE), SIDEX_INT32(bEncrypted)))
                      {
                        bVerifyOk = TML_FALSE;
                        // Note: The default certificate wasn't signed by a certificate authority!
                        //       Therefore we always get TML_FALSE as the result for bVerifyOk!
                        m_iErr = tml_Tls_Connection_VerifyCert(hConnection, &bVerifyOk);
                        if(checkForSuccess(tmlrtT("tml_Tls_Connection_VerifyCert(Connection) - Default")) &&
                           checkForValue(tmlrtT("tml_Tls_Connection_VerifyCert : bVerifyOk (Default)"),
                                         SIDEX_INT32(TML_FALSE), SIDEX_INT32(bVerifyOk)))
                        {
                          SIDEX_TCHAR* sDigest = NULL;
                          m_iErr = tml_Tls_Connection_Get_PeerSSLDigest(hConnection, TML_TLS_SHA1, &sDigest);
                          checkForSuccess(tmlrtT("tml_Tls_Connection_Get_PeerSSLDigest(Connection, SHA1) - Default"));
                          checkForValue(tmlrtT("Equality of digest (Default, SHA1)"), S_DEFAULT_DIGEST_SHA1, sDigest);
                          if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                          m_iErr = tml_Tls_Connection_Get_PeerSSLDigest(hConnection, TML_TLS_MD5, &sDigest);
                          checkForSuccess(tmlrtT("tml_Tls_Connection_Get_PeerSSLDigest(Connection, MD5) - Default"));
                          checkForValue(tmlrtT("Equality of digest (Default, MD5)"), S_DEFAULT_DIGEST_MD5, sDigest);
                          if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                          // - - - - -

                          m_cbLog_Command = 0;
                          bool bStop = false;
                          // send 4 commands... (1, 2, 4, 8)
                          for(TML_INT32 iCommand = 1; iCommand < 16; iCommand <<= 1)
                          {
                            TML_COMMAND_HANDLE hCommand = TML_HANDLE_TYPE_NULL;
                            m_iErr = tml_Cmd_Create(&hCommand);
                            if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Create(Command"),
                                                          tmlrt_itoa(iCommand),
                                                          tmlrtT(") - Default"), 2), true))
                            {
                              if(setCommandID(hCommand, 1234))
                              {
                                if(setCommandInt(hCommand, S_GROUP_TEST, S_KEY_INDEX, iCommand))
                                {
                                  m_iErr = tml_Connection_SendSync(hConnection, S_IO_PROFILE, hCommand, 2000);
                                  if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Connection_SendSync(Connection, Command"),
                                                                tmlrt_itoa(iCommand),
                                                                tmlrtT(") - Default"), 2), true)) bStop = true;
                                }
                              }
                              else bStop = true;

                              // it was a sync call - we can delete the command now...
                              m_iErr = tml_Cmd_Free(&hCommand);
                              if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Free(Command"),
                                                            tmlrt_itoa(iCommand),
                                                            tmlrtT(") - Default"), 2), true)) bStop = true;
                              hCommand = TML_HANDLE_TYPE_NULL;
                            }
                            else bStop = true;
                            if(bStop) break;
                          }
                          checkForValue(tmlrtT("Received commands log - Default"), 15, m_cbLog_Command, false);
                        }
                      }
                      else
                      {
                        SIDEX_TCHAR* pStatusMsg = NULL;
                        tml_Tls_Connection_Encryption_Get_StatusMessage(hConnection, &pStatusMsg);
                        if(pStatusMsg) messageOutput(pStatusMsg);
                      }

                      // Close connection...
                      m_iErr = tml_Connection_Close(&hConnection);
                      checkForSuccess(tmlrtT("tml_Connection_Close(Connection) - Default"));
                      hConnection = TML_HANDLE_TYPE_NULL;
                    }
                    stopListener(0, 0);
                  } // startListener 0, 0
                  deleteListener(0, 0);
                } // createListener 0, 0, addr 1

                checkForValue(tmlrtT("TlsErrorDetector (Default)"), 0, g_TlsErrorDetector);
              }

              // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

              if(m_testOK)
              {
                messageOutput(tmlrtT("Test with own certificate, but wrong key..."));
                g_TlsErrorDetector = 0;
                g_TlsCertSwitch    = 1;
                bAccept            = TML_FALSE;
                m_iErr = tml_Tls_Core_AcceptNegotiation(getCore(0), &CallbackHandler_OnAcceptTlsRequest,
                                                                    &CallbackHandler_OnCertificateLocation,
                                                                    &CallbackHandler_OnPrivateKeyLocation, &bAccept);
                if(checkForSuccess(tmlrtT("tml_Tls_Core_AcceptNegotiation(Core0, OwnCallbacksWrongKey)")) &&
                   checkForValue(tmlrtT("tml_Tls_Core_AcceptNegotiation : bAccept (OwnWrongKey)"),
                                 SIDEX_INT32(TML_TRUE), SIDEX_INT32(bAccept)))
                {
                  if(createListener(0, 0, sAddress1))
                  {
                    if(startListener(0, 0))
                    {
                      TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                      m_iErr = tml_Core_Connect(getCore(1), sAddress2, &hConnection);
                      if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect(Core1, "), sAddress2, tmlrtT(") - OwnWrongKey")), true))
                      {
                        g_TlsHideErrorMsg   = true;
                        TML_BOOL bEncrypted = TML_FALSE;
                        m_iErr = tml_Tls_Connection_StartNegotiation(hConnection, TML_FALSE, &bEncrypted);
                        if(checkForSuccess(tmlrtT("tml_Tls_Connection_StartNegotiation(Connection) - OwnWrongKey")) &&
                           checkForValue(tmlrtT("tml_Tls_Connection_StartNegotiation : bEncrypted (OwnWrongKey)"),
                                         SIDEX_INT32(TML_FALSE), SIDEX_INT32(bEncrypted)))
                        {
                          TML_BOOL bVerifyOk = TML_FALSE;
                          m_iErr = tml_Tls_Connection_VerifyCert(hConnection, &bVerifyOk);
                          if(checkForSuccess(tmlrtT("tml_Tls_Connection_VerifyCert(Connection) - OwnWrongKey")) &&
                             checkForValue(tmlrtT("tml_Tls_Connection_VerifyCert : bVerifyOk (OwnWrongKey)"),
                                           SIDEX_INT32(TML_FALSE), SIDEX_INT32(bVerifyOk)))
                          {
                            checkForValue(tmlrtT("TlsErrorDetector (OwnWrongKey)"), 3, g_TlsErrorDetector);
                            g_TlsErrorDetector = 0;
                          }
                        }
                        else
                        {
                          SIDEX_TCHAR* pStatusMsg = NULL;
                          tml_Tls_Connection_Encryption_Get_StatusMessage(hConnection, &pStatusMsg);
                          if(pStatusMsg) messageOutput(pStatusMsg);
                        }
                        g_TlsHideErrorMsg = false;

                        // Close connection...
                        m_iErr = tml_Connection_Close(&hConnection);
                        checkForSuccess(tmlrtT("tml_Connection_Close(Connection) - OwnWrongKey"));
                        hConnection = TML_HANDLE_TYPE_NULL;
                      }
                      stopListener(0, 0);
                    } // startListener 0, 0
                    deleteListener(0, 0);
                  } // createListener 0, 0, addr 1
                }
              }
            } // !isRepeaterTest

            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

            if(m_testOK || isRepeaterTest)
            {
              messageOutput(tmlrtT("Test with own certificate..."));
              g_TlsErrorDetector = 0;
              g_TlsCertSwitch    = 0;
              bAccept            = TML_FALSE;
              m_iErr = tml_Tls_Core_AcceptNegotiation(getCore(0), &CallbackHandler_OnAcceptTlsRequest,
                                                                  &CallbackHandler_OnCertificateLocation,
                                                                  &CallbackHandler_OnPrivateKeyLocation, &bAccept);
              if(checkForSuccess(tmlrtT("tml_Tls_Core_AcceptNegotiation(Core0, OwnCallbacks)")) &&
                 checkForValue(tmlrtT("tml_Tls_Core_AcceptNegotiation : bAccept (Own)"),
                               SIDEX_INT32(TML_TRUE), SIDEX_INT32(bAccept)))
              {
                if(createListener(0, 0, sAddress1))
                {
                  if(startListener(0, 0))
                  {
                    TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                    m_iErr = tml_Core_Connect(getCore(1), sAddress2, &hConnection);
                    if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect(Core1, "), sAddress2, tmlrtT(") - Own")), true))
                    {
                      // The test will run the next part twice - unencrypted (iLoop=0) and encrypted (iLoop=1)...
                      int iLoop = 0;
                      do
                      {
                        messageOutput(iLoop ? tmlrtT("- - - Encrypted - - -")
                                            : tmlrtT("- - - Not encrypted - - -"));

                        TML_BOOL bEncrypted = iLoop ? TML_FALSE : TML_TRUE;
                        if(iLoop) m_iErr = tml_Tls_Connection_StartNegotiation(hConnection, TML_FALSE, &bEncrypted);
                        if(checkForSuccess(tmlrtT("tml_Tls_Connection_StartNegotiation(Connection) - Own")) &&
                           checkForValue(tmlrtT("tml_Tls_Connection_StartNegotiation : bEncrypted (Own)"),
                                         SIDEX_INT32(TML_TRUE), SIDEX_INT32(bEncrypted)))
                        {
                          checkForValue(tmlrtT("TlsErrorDetector (Own, StartNegotiation)"), 0, g_TlsErrorDetector);
                          g_TlsErrorDetector = 0;

                          TML_BOOL bVerifyOk = TML_FALSE;
                          // Note: Our certificate wasn't signed by a certificate authority!
                          //       Therefore we always get TML_FALSE as the result for bVerifyOk!
                          if(iLoop) m_iErr = tml_Tls_Connection_VerifyCert(hConnection, &bVerifyOk);
                          if(checkForSuccess(tmlrtT("tml_Tls_Connection_VerifyCert(Connection) - Own")) &&
                             checkForValue(tmlrtT("tml_Tls_Connection_VerifyCert : bVerifyOk (Own)"),
                                           SIDEX_INT32(TML_FALSE), SIDEX_INT32(bVerifyOk)))
                          {
                            m_cbLog_Command = 0;
                            bool bStop = false;
                            // send 4 commands... (1, 2, 4, 8)
                            for(TML_INT32 iCommand = 1; iCommand < 16; iCommand <<= 1)
                            {
                              TML_COMMAND_HANDLE hCommand = TML_HANDLE_TYPE_NULL;
                              m_iErr = tml_Cmd_Create(&hCommand);
                              if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Create(Command"),
                                                            tmlrt_itoa(iCommand),
                                                            tmlrtT(") - Own"), 2), true))
                              {
                                if(setCommandID(hCommand, 1234))
                                {
                                  if(setCommandInt(hCommand, S_GROUP_TEST, S_KEY_INDEX, iCommand))
                                  {
                                    if(isRepeaterTest)
                                    {
                                      messageOutput(tmlrt_cat(tmlrtT("Sending command: "),
                                                              tmlrt_itoa(iCommand),
                                                              NULL, 2), true, true, true);
                                    }

                                    m_iErr = tml_Connection_SendSync(hConnection, S_IO_PROFILE, hCommand, 2000);
                                    if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Connection_SendSync(Connection, Command"),
                                                                  tmlrt_itoa(iCommand),
                                                                  tmlrtT(") - Own"), 2), true)) bStop = true;
                                  }
                                }
                                else bStop = true;

                                // it was a sync call - we can delete the command now...
                                m_iErr = tml_Cmd_Free(&hCommand);
                                if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Free(Command"),
                                                              tmlrt_itoa(iCommand),
                                                              tmlrtT(") - Own"), 2), true)) bStop = true;
                                hCommand = TML_HANDLE_TYPE_NULL;
                              }
                              else bStop = true;
                              if(bStop) break;
                            }
                            if(iLoop)
                            {
                              if(isRepeaterTest) m_cbLog_Command = 15;
                              if(checkForValue(tmlrtT("Received commands log - Own"), 15, m_cbLog_Command, false))
                              {
                                messageOutput(tmlrtT("Test digest functions..."));

                                SIDEX_TCHAR* sDigest  = NULL;
                                SIDEX_TCHAR* sCrtPath = tmlrt_delimiters(tmlrt_cat(TestParams->getTlsLocation(),
                                                                                   S_SLASH,
                                                                                   TestParams->getTlsCrt(), 5), true);
                                // - - - - - String SHA1 - - - - -

                                m_iErr = tml_Tls_Get_Digest(NULL, TML_TLS_SHA1, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Get_Digest(NoInput, SHA1)"));
                                checkForValue(tmlrtT("Null test (NoInput, SHA1)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Get_Digest(S_EMPTY, TML_TLS_SHA1, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Get_Digest(EmptyInput, SHA1)"));
                                checkForValue(tmlrtT("Null test (EmptyInput, SHA1)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Get_Digest(S_REGTEST, TML_TLS_SHA1, NULL);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Get_Digest(NoOutput, SHA1)"));

                                m_iErr = tml_Tls_Get_Digest(S_REGTEST, TML_TLS_SHA1, &sDigest);
                                checkForSuccess(tmlrtT("tml_Tls_Get_Digest(Input, SHA1)"));
                                checkForValue(tmlrtT("Equality of digest (1, SHA1)"), S_REGTEST_DIGEST_SHA1, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                // - - - - - File SHA1 - - - - -

                                m_iErr = tml_Tls_Connection_Get_SSLDigest(TML_HANDLE_TYPE_NULL, TML_TLS_SHA1, sCrtPath, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Connection_Get_SSLDigest(NoConnection, SHA1)"));
                                checkForValue(tmlrtT("Null test (NoConnection, SHA1, File)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Connection_Get_SSLDigest(hConnection, TML_TLS_SHA1, NULL, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Connection_Get_SSLDigest(NoPath, SHA1)"));
                                checkForValue(tmlrtT("Null test (Connection, SHA1, NoFile)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Connection_Get_SSLDigest(hConnection, TML_TLS_SHA1, sCrtPath, NULL);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Connection_Get_SSLDigest(Connection, NoOutput, SHA1)"));

                                m_iErr = tml_Tls_Connection_Get_SSLDigest(hConnection, TML_TLS_SHA1, sCrtPath, &sDigest);
                                checkForSuccess(tmlrtT("tml_Tls_Connection_Get_SSLDigest(CertPath, SHA1)"));
                                checkForValue(tmlrtT("Equality of digest (2, SHA1)"), TestParams->getTlsFingerprintSHA1(), sDigest, false, true, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                // - - - - - Peer SHA1 - - - - -

                                m_iErr = tml_Tls_Connection_Get_PeerSSLDigest(TML_HANDLE_TYPE_NULL, TML_TLS_SHA1, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Connection_Get_PeerSSLDigest(NoConnection, SHA1)"));
                                checkForValue(tmlrtT("Null test (NoConnection, SHA1, Peer)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Connection_Get_PeerSSLDigest(hConnection, TML_TLS_SHA1, NULL);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Connection_Get_PeerSSLDigest(Connection, NoOutput, SHA1)"));

                                m_iErr = tml_Tls_Connection_Get_PeerSSLDigest(hConnection, TML_TLS_SHA1, &sDigest);
                                checkForSuccess(tmlrtT("tml_Tls_Connection_Get_PeerSSLDigest(Connection, SHA1)"));
                                checkForValue(tmlrtT("Equality of digest (3, SHA1)"), TestParams->getTlsFingerprintSHA1(), sDigest, false, true, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                // - - - - - String MD5 - - - - -

                                m_iErr = tml_Tls_Get_Digest(NULL, TML_TLS_MD5, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Get_Digest(NoInput, MD5)"));
                                checkForValue(tmlrtT("Null test (NoInput, MD5)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Get_Digest(S_EMPTY, TML_TLS_MD5, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Get_Digest(EmptyInput, MD5)"));
                                checkForValue(tmlrtT("Null test (EmptyInput, MD5)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Get_Digest(S_REGTEST, TML_TLS_MD5, NULL);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Get_Digest(NoOutput, MD5)"));

                                m_iErr = tml_Tls_Get_Digest(S_REGTEST, TML_TLS_MD5, &sDigest);
                                checkForSuccess(tmlrtT("tml_Tls_Get_Digest(Input, MD5)"));
                                checkForValue(tmlrtT("Equality of digest (1, MD5)"), S_REGTEST_DIGEST_MD5, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                // - - - - - File MD5 - - - - -

                                m_iErr = tml_Tls_Connection_Get_SSLDigest(TML_HANDLE_TYPE_NULL, TML_TLS_MD5, sCrtPath, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Connection_Get_SSLDigest(NoConnection, MD5)"));
                                checkForValue(tmlrtT("Null test (NoConnection, MD5, File)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Connection_Get_SSLDigest(hConnection, TML_TLS_MD5, NULL, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Connection_Get_SSLDigest(NoPath, MD5)"));
                                checkForValue(tmlrtT("Null test (Connection, MD5, NoFile)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Connection_Get_SSLDigest(hConnection, TML_TLS_MD5, sCrtPath, NULL);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Connection_Get_SSLDigest(Connection, NoOutput, MD5)"));

                                m_iErr = tml_Tls_Connection_Get_SSLDigest(hConnection, TML_TLS_MD5, sCrtPath, &sDigest);
                                checkForSuccess(tmlrtT("tml_Tls_Connection_Get_SSLDigest(CertPath, MD5)"));
                                checkForValue(tmlrtT("Equality of digest (2, MD5)"), TestParams->getTlsFingerprintMD5(), sDigest, false, true, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                // - - - - - Peer MD5 - - - - -

                                m_iErr = tml_Tls_Connection_Get_PeerSSLDigest(TML_HANDLE_TYPE_NULL, TML_TLS_MD5, &sDigest);
                                checkForExpectedReturnCode(TML_ERR_MISSING_OBJ, tmlrtT("tml_Tls_Connection_Get_PeerSSLDigest(NoConnection, MD5)"));
                                checkForValue(tmlrtT("Null test (NoConnection, MD5, Peer)"), NULL, sDigest, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                m_iErr = tml_Tls_Connection_Get_PeerSSLDigest(hConnection, TML_TLS_MD5, NULL);
                                checkForExpectedReturnCode(TML_ERR_UNICODE, tmlrtT("tml_Tls_Connection_Get_PeerSSLDigest(Connection, NoOutput, MD5)"));

                                m_iErr = tml_Tls_Connection_Get_PeerSSLDigest(hConnection, TML_TLS_MD5, &sDigest);
                                checkForSuccess(tmlrtT("tml_Tls_Connection_Get_PeerSSLDigest(Connection, MD5)"));
                                checkForValue(tmlrtT("Equality of digest (3, MD5)"), TestParams->getTlsFingerprintMD5(), sDigest, false, true, false);
                                if(sDigest) sidex_Free_ReadString(sDigest); sDigest = NULL;

                                if(sCrtPath) DELETE_STR(sCrtPath);
                              }
                            }
                          }
                        }
                        else
                        {
                          SIDEX_TCHAR* pStatusMsg = NULL;
                          tml_Tls_Connection_Encryption_Get_StatusMessage(hConnection, &pStatusMsg);
                          if(pStatusMsg) messageOutput(pStatusMsg);
                        }
                      }
                      while(++iLoop < 2);

                      // Close connection...
                      m_iErr = tml_Connection_Close(&hConnection);
                      checkForSuccess(tmlrtT("tml_Connection_Close(Connection) - Own"));
                      hConnection = TML_HANDLE_TYPE_NULL;
                    }

                    stopListener(0, 0);
                  } // startListener 0, 0
                  deleteListener(0, 0);
                } // createListener 0, 0, addr 1

                checkForValue(tmlrtT("TlsErrorDetector (Own)"), 0, g_TlsErrorDetector);
              }
            }

            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

            if(m_testOK && !isRepeaterTest)
            {
              messageOutput(tmlrtT("Test auto negotiation with wrong key..."));
              g_TlsErrorDetector = 0;
              g_TlsCertSwitch    = 1;
              bAccept            = TML_FALSE;
              m_iErr = tml_Tls_Core_AcceptNegotiation(getCore(0), &CallbackHandler_OnAcceptTlsRequest,
                                                                  &CallbackHandler_OnCertificateLocation,
                                                                  &CallbackHandler_OnPrivateKeyLocation, &bAccept);
              if(checkForSuccess(tmlrtT("tml_Tls_Core_AcceptNegotiation(Core0, AutoWrongKey)")) &&
                 checkForValue(tmlrtT("tml_Tls_Core_AcceptNegotiation : bAccept (AutoWrongKey)"),
                               SIDEX_INT32(TML_TRUE), SIDEX_INT32(bAccept)))
              {
                if(createListener(0, 0, sAddress1))
                {
                  if(startListener(0, 0))
                  {
                    m_iErr = tml_Tls_Core_Set_AutoNegotiation(getCore(1), TML_TRUE, TML_FALSE);
                    if(checkForSuccess(tmlrtT("tml_Tls_Core_Set_AutoNegotiation(Core1) - AutoWrongKey")))
                    {
                      g_TlsHideErrorMsg = true;
                      TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                      m_iErr = tml_Core_Connect(getCore(1), sAddress2, &hConnection);
                      checkForExpectedReturnCode(TML_ERR_SENDER_INVALID_PARAMS,
                                                 tmlrt_cat(tmlrtT("tml_Core_Connect(Core1, "),
                                                           sAddress2, tmlrtT(") - AutoWrongKey")), true);
                      if(hConnection != TML_HANDLE_TYPE_NULL)
                      {
                        errorOutput(tmlrtT("tml_Core_Connect returns error code, but connection is established!"), false, false);

                        // Close connection...
                        m_iErr = tml_Connection_Close(&hConnection);
                        checkForSuccess(tmlrtT("tml_Connection_Close(Connection) - AutoWrongKey"));
                        hConnection = TML_HANDLE_TYPE_NULL;
                      }
                      g_TlsHideErrorMsg = false;
                    }

                    stopListener(0, 0);
                  } // startListener 0, 0
                  deleteListener(0, 0);
                } // createListener 0, 0, addr 1

                checkForValue(tmlrtT("TlsErrorDetector (AutoWrongKey)"), 3, g_TlsErrorDetector);
              }
            }

            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

            if(m_testOK && !isRepeaterTest)
            {
              messageOutput(tmlrtT("Test auto negotiation..."));
              g_TlsErrorDetector = 0;
              g_TlsCertSwitch    = 0;
              bAccept            = TML_FALSE;
              m_iErr = tml_Tls_Core_AcceptNegotiation(getCore(0), &CallbackHandler_OnAcceptTlsRequest,
                                                                  &CallbackHandler_OnCertificateLocation,
                                                                  &CallbackHandler_OnPrivateKeyLocation, &bAccept);
              if(checkForSuccess(tmlrtT("tml_Tls_Core_AcceptNegotiation(Core0, Auto)")) &&
                 checkForValue(tmlrtT("tml_Tls_Core_AcceptNegotiation : bAccept (Auto)"),
                               SIDEX_INT32(TML_TRUE), SIDEX_INT32(bAccept)))
              {
                if(createListener(0, 0, sAddress1))
                {
                  if(startListener(0, 0))
                  {
                    m_iErr = tml_Tls_Core_Set_AutoNegotiation(getCore(1), TML_TRUE, TML_FALSE);
                    if(checkForSuccess(tmlrtT("tml_Tls_Core_Set_AutoNegotiation(Core1)")))
                    {
                      TML_CONNECTION_HANDLE hConnection = TML_HANDLE_TYPE_NULL;
                      m_iErr = tml_Core_Connect(getCore(1), sAddress2, &hConnection);
                      if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Core_Connect(Core1, "), sAddress2, tmlrtT(") - Auto")), true))
                      {
                        m_cbLog_Command = 0;
                        bool bStop = false;
                        // send 4 commands... (1, 2, 4, 8)
                        for(TML_INT32 iCommand = 1; iCommand < 16; iCommand <<= 1)
                        {
                          TML_COMMAND_HANDLE hCommand = TML_HANDLE_TYPE_NULL;
                          m_iErr = tml_Cmd_Create(&hCommand);
                          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Create(Command"),
                                                        tmlrt_itoa(iCommand),
                                                        tmlrtT(") - Auto"), 2), true))
                          {
                            if(setCommandID(hCommand, 1234))
                            {
                              if(setCommandInt(hCommand, S_GROUP_TEST, S_KEY_INDEX, iCommand))
                              {
                                m_iErr = tml_Connection_SendSync(hConnection, S_IO_PROFILE, hCommand, 2000);
                                if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Connection_SendSync(Connection, Command"),
                                                              tmlrt_itoa(iCommand),
                                                              tmlrtT(") - Auto"), 2), true)) bStop = true;
                              }
                            }
                            else bStop = true;

                            // it was a sync call - we can delete the command now...
                            m_iErr = tml_Cmd_Free(&hCommand);
                            if(!checkForSuccess(tmlrt_cat(tmlrtT("tml_Cmd_Free(Command"),
                                                          tmlrt_itoa(iCommand),
                                                          tmlrtT(") - Auto"), 2), true)) bStop = true;
                            hCommand = TML_HANDLE_TYPE_NULL;
                          }
                          else bStop = true;
                          if(bStop) break;
                        }
                        if(checkForValue(tmlrtT("Received commands log - Auto"), 15, m_cbLog_Command, false))
                        {
                          if(m_testOK) messageOutput(tmlrtT("Test culmination reached!"));
                        }

                        // Close connection...
                        m_iErr = tml_Connection_Close(&hConnection);
                        checkForSuccess(tmlrtT("tml_Connection_Close(Connection) - Auto"));
                        hConnection = TML_HANDLE_TYPE_NULL;
                      } // connect
                    } // auto negotiation

                    stopListener(0, 0);
                  } // startListener 0, 0
                  deleteListener(0, 0);
                } // createListener 0, 0, addr 1

                checkForValue(tmlrtT("TlsErrorDetector (Auto)"), 0, g_TlsErrorDetector);
              }
            }
          } // tml_Profile_Register_Cmd
        } // tml_Profile_Register
      }
    } // createCore 0 and 1

    deleteCore(1);
    deleteCore(0);

    if(sAddress2 != sAddress1) DELETE_STR(sAddress2);
    DELETE_STR(sAddress1);

  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

void TmlTLSTester::startRepeater()
{
  setTestSectionName(tmlrtT("TlsRepeater"));
  messageOutput(S_START);
  reset();

  int n = TestParams->getNetworkCardCount();
  if(n > 0)
  {
    int          iPort    = TestParams->getFirstPortNumber();
    SIDEX_TCHAR* sCard    = TestParams->getNetworkCard(0);
    SIDEX_TCHAR* sAddress = tmlrt_cat(sCard, S_COLON, tmlrt_itoa(iPort++), 4);
    DELETE_STR(sCard);

    if(createCore(0))
    {
      cbData_t_TLS* pData0 = prepareCallbackData_TLS(this, 0);
      if(pData0)
      {
        g_TlsErrorDetector = 0;

        m_iErr = tml_Tls_Core_Set_OnError(getCore(0), &CallbackHandler_OnTlsError, pData0);
        checkForSuccess(tmlrtT("tml_Tls_Core_Set_OnError(Core0)"));

        // Register profile and command...
        m_iErr = tml_Profile_Register(getCore(0), S_IO_PROFILE);
        if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register(Core0, "), S_IO_PROFILE, S_PARENTHESIS_R), true))
        {
          m_iErr = tml_Profile_Register_Cmd(getCore(0), S_IO_PROFILE, 1234, &CallbackHandler_TLS, pData0);
          if(checkForSuccess(tmlrt_cat(tmlrtT("tml_Profile_Register_Cmd(Core0, "), S_IO_PROFILE, tmlrtT(", 1234)")), true))
          {
            g_TlsErrorDetector = 0;
            g_TlsCertSwitch    = 0;
            TML_BOOL bAccept   = TML_FALSE;
            m_iErr = tml_Tls_Core_AcceptNegotiation(getCore(0), &CallbackHandler_OnAcceptTlsRequest,
                                                                &CallbackHandler_OnCertificateLocation,
                                                                &CallbackHandler_OnPrivateKeyLocation, &bAccept);
            if(checkForSuccess(tmlrtT("tml_Tls_Core_AcceptNegotiation(Core0, OwnCallbacks)")) &&
                checkForValue(tmlrtT("tml_Tls_Core_AcceptNegotiation : bAccept (OwnCallbacks)"),
                              SIDEX_INT32(TML_TRUE), SIDEX_INT32(bAccept)))
            {
              if(createListener(0, 0, sAddress))
              {
                if(startListener(0, 0))
                {
                  messageOutput(tmlrtT("Repeater is running..."));

                  // wait for given idle time...
                  int maxIdleCounter = TestParams->getMaxIdleTime();
                  g_TlsIdleCounter = 0;
                  do
                  {
                    TmlSleep(1000);
                    if(maxIdleCounter > 0)
                    {
                      if(enterGlobalMutex())
                      {
                        wcout << (maxIdleCounter - g_TlsIdleCounter) << endl;
                        leaveGlobalMutex();
                      }
                      g_TlsIdleCounter++;
                    }
                  }
                  while(g_TlsIdleCounter < maxIdleCounter);

                  stopListener(0, 0);
                } // startListener 0, 0
                deleteListener(0, 0);
              } // createListener 0, 0, addr 1
            } // tml_Tls_Core_AcceptNegotiation
          } // tml_Profile_Register_Cmd
        } // tml_Profile_Register
      }
    } // createCore 0
    deleteCore(0);
    DELETE_STR(sAddress);
  } // network card count > 0

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
}

//------------------------------------------------------------------------------
