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

#include "TestParams.h"

CTestParams* TestParams = NULL;

// document name
SIDEX_TCHAR* S_TN_TESTPARAMS      = tmlrtT("TestParams");

// group names
SIDEX_TCHAR* S_TG_NETWORK         = tmlrtT("Network");
SIDEX_TCHAR* S_TG_TEST            = tmlrtT("Test");
SIDEX_TCHAR* S_TG_TLS             = tmlrtT("TLS");

// parameter names
SIDEX_TCHAR* S_TP_CARDS           = tmlrtT("Cards");
SIDEX_TCHAR* S_TP_FIRSTPORTNUMBER = tmlrtT("FirstPortNumber");

SIDEX_TCHAR* S_TP_LOOPCOUNT       = tmlrtT("LoopCount");
SIDEX_TCHAR* S_TP_ISREPEATER      = tmlrtT("IsRepeater");
SIDEX_TCHAR* S_TP_MAXIDLETIME     = tmlrtT("MaxIdleTime");
SIDEX_TCHAR* S_TP_TLSREPEATERHOST = tmlrtT("TlsRepeaterHost");
SIDEX_TCHAR* S_TP_TLSREPEATERPORT = tmlrtT("TlsRepeaterPort");

SIDEX_TCHAR* S_TP_LOCATION        = tmlrtT("Location");
SIDEX_TCHAR* S_TP_CRT             = tmlrtT("CRT");
SIDEX_TCHAR* S_TP_CRT2            = tmlrtT("CRT2");
SIDEX_TCHAR* S_TP_KEY             = tmlrtT("KEY");
SIDEX_TCHAR* S_TP_KEY2            = tmlrtT("KEY2");
SIDEX_TCHAR* S_TP_FP_SHA1         = tmlrtT("Fingerprint_SHA1");
SIDEX_TCHAR* S_TP_FP_MD5          = tmlrtT("Fingerprint_MD1");

// default values
SIDEX_TCHAR* S_TD_127_0_0_1       = tmlrtT("127.0.0.1");

SIDEX_TCHAR* S_TD_TLS_LOCATION    = tmlrtT("../../../../../test/regTests/tmlRegTest/tls");
SIDEX_TCHAR* S_TD_TLS_CRT         = tmlrtT("TmlRegTestTls.crt");
SIDEX_TCHAR* S_TD_TLS_CRT2        = tmlrtT("TmlRegTestTls2.crt");
SIDEX_TCHAR* S_TD_TLS_KEY         = tmlrtT("TmlRegTestTls.key");
SIDEX_TCHAR* S_TD_TLS_KEY2        = tmlrtT("TmlRegTestTls2.key");
SIDEX_TCHAR* S_TD_TLS_FP_SHA1     = tmlrtT("86:57:56:05:DF:5E:D2:F2:EC:2E:91:39:3E:E6:B0:F5:CE:77:03:7D");
SIDEX_TCHAR* S_TD_TLS_FP_MD5      = tmlrtT("E5:D1:CB:99:26:5E:F8:F1:D0:94:6F:72:C9:23:D2:18");

#define DEFAULT_PORTNUMBER  42042
#define DEFAULT_MAXIDLETIME    60

CTestParams::CTestParams(SIDEX_TCHAR* testParamsFileName)
{
  m_ParamsFileName = NULL;
  m_sdxParams      = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iErr = sidex_Create(S_TN_TESTPARAMS, &m_sdxParams);
  if(iErr == SIDEX_SUCCESS)
  {
    if(testParamsFileName)
    {
      iErr = sidex_Load_Content(m_sdxParams, testParamsFileName);
      if(iErr == SIDEX_SUCCESS) m_ParamsFileName = tmlrt_cpy(testParamsFileName);
    }
    if(!ensureDefaultParams())
    {
      #ifndef LINUX
        sidex_Save_Content(m_sdxParams, tmlrtT("C:\\filledTestParams.sdx"));
      #endif
    }
  }
}

CTestParams::~CTestParams()
{
  if(m_sdxParams != SIDEX_HANDLE_TYPE_NULL)
  {
    sidex_Free(&m_sdxParams);
    m_sdxParams = SIDEX_HANDLE_TYPE_NULL;
  }
  DELETE_STR(m_ParamsFileName);
}

bool CTestParams::hasParams()
{
  return(m_sdxParams != SIDEX_HANDLE_TYPE_NULL);
}

SIDEX_TCHAR* CTestParams::getParamsFileName()
{
  return(m_ParamsFileName);
}

bool CTestParams::ensureDefaultParams()
{
  bool result = true;
  if(hasParams())
  {
    SIDEX_INT32 nCards = getNetworkCardCount();
    if(nCards == 0)
    {
      result = false;
      SIDEX_VARIANT card = SIDEX_HANDLE_TYPE_NULL;
      SIDEX_INT32   iErr = sidex_Variant_New_String(S_TD_127_0_0_1, &card);
      if(iErr == SIDEX_SUCCESS)
      {
        SIDEX_VARIANT cards = sidex_Variant_New_List();
        if(cards != SIDEX_HANDLE_TYPE_NULL)
        {
          SIDEX_INT32 pos = 0;
          iErr = sidex_Variant_List_Append(cards, card, &pos);
          if(iErr == SIDEX_SUCCESS)
          {
            sidex_List_Write(m_sdxParams, S_TG_NETWORK, S_TP_CARDS, cards);
          }
          sidex_Variant_DecRef(cards);
          cards = SIDEX_HANDLE_TYPE_NULL;
        }
        sidex_Variant_DecRef(card);
        card = SIDEX_HANDLE_TYPE_NULL;
      }
    }

    if(getFirstPortNumber() <= 0)
    {
      result = false;
      sidex_Integer_Write(m_sdxParams, S_TG_NETWORK, S_TP_FIRSTPORTNUMBER, DEFAULT_PORTNUMBER);
    }

    if(getTestLoopCount() <= 0)
    {
      result = false;
      sidex_Integer_Write(m_sdxParams, S_TG_TEST, S_TP_LOOPCOUNT, 1);
    }

    if(!sidex_HasKey(m_sdxParams, S_TG_TEST, S_TP_ISREPEATER))
    {
      result = false;
      sidex_Boolean_Write(m_sdxParams, S_TG_TEST, S_TP_ISREPEATER, SIDEX_FALSE);
    }

    if(getMaxIdleTime() < 0)
    {
      result = false;
      sidex_Integer_Write(m_sdxParams, S_TG_TEST, S_TP_MAXIDLETIME, DEFAULT_MAXIDLETIME);
    }

    if(getTlsRepeaterPort() <= 0)
    {
      result = false;
      sidex_Integer_Write(m_sdxParams, S_TG_TEST, S_TP_TLSREPEATERPORT, DEFAULT_PORTNUMBER);
    }

    if(!sidex_HasKey(m_sdxParams, S_TG_TEST, S_TP_TLSREPEATERHOST))
    {
      result = false;
      sidex_String_Write(m_sdxParams, S_TG_TEST, S_TP_TLSREPEATERHOST, S_EMPTY);
    }

    SIDEX_TCHAR* s = getTlsLocation();
    if(s)
    {
      if(!s[0])
      {
        result = false;
        sidex_String_Write(m_sdxParams, S_TG_TLS, S_TP_LOCATION, S_TD_TLS_LOCATION);
      }
      DELETE_STR(s);
    }

    s = getTlsCrt();
    if(s)
    {
      if(!s[0])
      {
        result = false;
        sidex_String_Write(m_sdxParams, S_TG_TLS, S_TP_CRT, S_TD_TLS_CRT);
      }
      DELETE_STR(s);
    }

    s = getTlsCrt2();
    if(s)
    {
      if(!s[0])
      {
        result = false;
        sidex_String_Write(m_sdxParams, S_TG_TLS, S_TP_CRT2, S_TD_TLS_CRT2);
      }
      DELETE_STR(s);
    }

    s = getTlsKey();
    if(s)
    {
      if(!s[0])
      {
        result = false;
        sidex_String_Write(m_sdxParams, S_TG_TLS, S_TP_KEY, S_TD_TLS_KEY);
      }
      DELETE_STR(s);
    }

    s = getTlsKey2();
    if(s)
    {
      if(!s[0])
      {
        result = false;
        sidex_String_Write(m_sdxParams, S_TG_TLS, S_TP_KEY2, S_TD_TLS_KEY2);
      }
      DELETE_STR(s);
    }

    s = getTlsFingerprintSHA1();
    if(s)
    {
      if(!s[0])
      {
        result = false;
        sidex_String_Write(m_sdxParams, S_TG_TLS, S_TP_FP_SHA1, S_TD_TLS_FP_SHA1);
      }
      DELETE_STR(s);
    }

    s = getTlsFingerprintMD5();
    if(s)
    {
      if(!s[0])
      {
        result = false;
        sidex_String_Write(m_sdxParams, S_TG_TLS, S_TP_FP_MD5, S_TD_TLS_FP_MD5);
      }
      DELETE_STR(s);
    }
  }
  return(result);
}

int CTestParams::getNetworkCardCount()
{
  SIDEX_INT32 nCards = 0;
  if(hasParams())
  {
    SIDEX_VARIANT cards = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 iErr = sidex_List_Read(m_sdxParams, S_TG_NETWORK, S_TP_CARDS, &cards);
    if(iErr == SIDEX_SUCCESS)
    {
      iErr = sidex_Variant_List_Size(cards, &nCards);
      if(iErr != SIDEX_SUCCESS) nCards = 0;
      sidex_Variant_DecRef(cards);
    }
  }
  return(nCards);
}

SIDEX_TCHAR* CTestParams::getNetworkCard(SIDEX_INT32 index)
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_INT32 nCards = getNetworkCardCount();
    if((index >= 0) && (index < nCards))
    {
      SIDEX_VARIANT cards = SIDEX_HANDLE_TYPE_NULL;
      SIDEX_INT32 iErr = sidex_List_Read(m_sdxParams, S_TG_NETWORK, S_TP_CARDS, &cards);
      if(iErr == SIDEX_SUCCESS)
      {
        SIDEX_VARIANT card = SIDEX_HANDLE_TYPE_NULL;
        iErr = sidex_Variant_List_Get(cards, index, &card);
        if(iErr == SIDEX_SUCCESS)
        {
          SIDEX_TCHAR* pCard = NULL;
          SIDEX_INT32  len   = 0;
          iErr = sidex_Variant_As_String(card, &pCard, &len);
          if(iErr == SIDEX_SUCCESS)
          {
            result = tmlrt_cpy(pCard);
            // pCard = borrowed reference -> do not delete!
            pCard = NULL;
          }
          // card = borrowed reference -> do not DecRef!
          card = SIDEX_HANDLE_TYPE_NULL;
        }
        sidex_Variant_DecRef(cards);
        cards = SIDEX_HANDLE_TYPE_NULL;
      }
    }
  }
  return(result);
}

int CTestParams::getFirstPortNumber()
{
  int firstPortNumber = -1;
  if(hasParams())
  {
    SIDEX_INT64 value = 0;
    SIDEX_INT32 iErr = sidex_Integer_Read(m_sdxParams, S_TG_NETWORK, S_TP_FIRSTPORTNUMBER, &value);
    if(iErr == SIDEX_SUCCESS) firstPortNumber = (int)value;
  }
  return(firstPortNumber);
}

int CTestParams::getTestLoopCount()
{
  int nLoops = -1;
  if(hasParams())
  {
    SIDEX_INT64 value = 0;
    SIDEX_INT32 iErr = sidex_Integer_Read(m_sdxParams, S_TG_TEST, S_TP_LOOPCOUNT, &value);
    if(iErr == SIDEX_SUCCESS) nLoops = (int)value;
  }
  return(nLoops);
}

bool CTestParams::isActingAsRepeater()
{
  bool isRepeater = false;
  if(hasParams())
  {
    SIDEX_BOOL value = 0;
    SIDEX_INT32 iErr = sidex_Boolean_Read(m_sdxParams, S_TG_TEST, S_TP_ISREPEATER, &value);
    if(iErr == SIDEX_SUCCESS) isRepeater = (value != SIDEX_FALSE);
  }
  return(isRepeater);
}

int CTestParams::getMaxIdleTime()
{
  int nSeconds = -1;
  if(hasParams())
  {
    SIDEX_INT64 value = 0;
    SIDEX_INT32 iErr = sidex_Integer_Read(m_sdxParams, S_TG_TEST, S_TP_MAXIDLETIME, &value);
    if(iErr == SIDEX_SUCCESS) nSeconds = (int)value;
  }
  return(nSeconds);
}

SIDEX_TCHAR* CTestParams::getTlsRepeaterHost()
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_TCHAR* pStr = NULL;
    SIDEX_INT32  nStr = 0;
    SIDEX_INT32  iErr = sidex_String_Read(m_sdxParams, S_TG_TEST, S_TP_TLSREPEATERHOST, &pStr, &nStr);
    if(iErr == SIDEX_SUCCESS)
    {
      result = tmlrt_cpy(pStr);
      sidex_Free_ReadString(pStr);
      pStr = NULL;
    }
    else result = tmlrt_cpy(S_EMPTY);
  }
  return(result);
}

int CTestParams::getTlsRepeaterPort()
{
  int portNumber = -1;
  if(hasParams())
  {
    SIDEX_INT64 value = 0;
    SIDEX_INT32 iErr = sidex_Integer_Read(m_sdxParams, S_TG_TEST, S_TP_TLSREPEATERPORT, &value);
    if(iErr == SIDEX_SUCCESS) portNumber = (int)value;
  }
  return(portNumber);
}

SIDEX_TCHAR* CTestParams::getTlsLocation()
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_TCHAR* pStr = NULL;
    SIDEX_INT32  nStr = 0;
    SIDEX_INT32  iErr = sidex_String_Read(m_sdxParams, S_TG_TLS, S_TP_LOCATION, &pStr, &nStr);
    if(iErr == SIDEX_SUCCESS)
    {
      result = tmlrt_cpy(pStr);
      sidex_Free_ReadString(pStr);
      pStr = NULL;
    }
    else result = tmlrt_cpy(S_EMPTY);
  }
  return(result);
}

SIDEX_TCHAR* CTestParams::getTlsCrt()
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_TCHAR* pStr = NULL;
    SIDEX_INT32  nStr = 0;
    SIDEX_INT32  iErr = sidex_String_Read(m_sdxParams, S_TG_TLS, S_TP_CRT, &pStr, &nStr);
    if(iErr == SIDEX_SUCCESS)
    {
      result = tmlrt_cpy(pStr);
      sidex_Free_ReadString(pStr);
      pStr = NULL;
    }
    else result = tmlrt_cpy(S_EMPTY);
  }
  return(result);
}

SIDEX_TCHAR* CTestParams::getTlsCrt2()
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_TCHAR* pStr = NULL;
    SIDEX_INT32  nStr = 0;
    SIDEX_INT32  iErr = sidex_String_Read(m_sdxParams, S_TG_TLS, S_TP_CRT2, &pStr, &nStr);
    if(iErr == SIDEX_SUCCESS)
    {
      result = tmlrt_cpy(pStr);
      sidex_Free_ReadString(pStr);
      pStr = NULL;
    }
    else result = tmlrt_cpy(S_EMPTY);
  }
  return(result);
}

SIDEX_TCHAR* CTestParams::getTlsKey()
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_TCHAR* pStr = NULL;
    SIDEX_INT32  nStr = 0;
    SIDEX_INT32  iErr = sidex_String_Read(m_sdxParams, S_TG_TLS, S_TP_KEY, &pStr, &nStr);
    if(iErr == SIDEX_SUCCESS)
    {
      result = tmlrt_cpy(pStr);
      sidex_Free_ReadString(pStr);
      pStr = NULL;
    }
    else result = tmlrt_cpy(S_EMPTY);
  }
  return(result);
}

SIDEX_TCHAR* CTestParams::getTlsKey2()
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_TCHAR* pStr = NULL;
    SIDEX_INT32  nStr = 0;
    SIDEX_INT32  iErr = sidex_String_Read(m_sdxParams, S_TG_TLS, S_TP_KEY2, &pStr, &nStr);
    if(iErr == SIDEX_SUCCESS)
    {
      result = tmlrt_cpy(pStr);
      sidex_Free_ReadString(pStr);
      pStr = NULL;
    }
    else result = tmlrt_cpy(S_EMPTY);
  }
  return(result);
}

SIDEX_TCHAR* CTestParams::getTlsFingerprintSHA1()
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_TCHAR* pStr = NULL;
    SIDEX_INT32  nStr = 0;
    SIDEX_INT32  iErr = sidex_String_Read(m_sdxParams, S_TG_TLS, S_TP_FP_SHA1, &pStr, &nStr);
    if(iErr == SIDEX_SUCCESS)
    {
      result = tmlrt_cpy(pStr);
      sidex_Free_ReadString(pStr);
      pStr = NULL;
    }
    else result = tmlrt_cpy(S_EMPTY);
  }
  return(result);
}

SIDEX_TCHAR* CTestParams::getTlsFingerprintMD5()
{
  SIDEX_TCHAR* result = NULL;
  if(hasParams())
  {
    SIDEX_TCHAR* pStr = NULL;
    SIDEX_INT32  nStr = 0;
    SIDEX_INT32  iErr = sidex_String_Read(m_sdxParams, S_TG_TLS, S_TP_FP_MD5, &pStr, &nStr);
    if(iErr == SIDEX_SUCCESS)
    {
      result = tmlrt_cpy(pStr);
      sidex_Free_ReadString(pStr);
      pStr = NULL;
    }
    else result = tmlrt_cpy(S_EMPTY);
  }
  return(result);
}
