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

SIDEX_TCHAR* S_TP_TESTPARAMS      = tmlrtT("TestParams");
SIDEX_TCHAR* S_TP_NETWORK         = tmlrtT("Network");
SIDEX_TCHAR* S_TP_CARDS           = tmlrtT("Cards");
SIDEX_TCHAR* S_TP_127_0_0_1       = tmlrtT("127.0.0.1");
SIDEX_TCHAR* S_TP_FIRSTPORTNUMBER = tmlrtT("FirstPortNumber");
SIDEX_TCHAR* S_TP_TEST            = tmlrtT("Test");
SIDEX_TCHAR* S_TP_LOOPCOUNT       = tmlrtT("LoopCount");

CTestParams::CTestParams(SIDEX_TCHAR* testParamsFileName)
{
  m_ParamsFileName = NULL;
  m_sdxParams      = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iErr = sidex_Create(S_TP_TESTPARAMS, &m_sdxParams);
  if(iErr == SIDEX_SUCCESS)
  {
    if(testParamsFileName)
    {
      iErr = sidex_Load_Content(m_sdxParams, testParamsFileName);
      if(iErr == SIDEX_SUCCESS) m_ParamsFileName = tmlrt_cpy(testParamsFileName);
    }
    if(!ensureDefaultParams())
    {
      sidex_Save_Content(m_sdxParams, tmlrtT("C:\\filledTestParams.sdx"));
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
      SIDEX_INT32   iErr = sidex_Variant_New_String(S_TP_127_0_0_1, &card);
      if(iErr == SIDEX_SUCCESS)
      {
        SIDEX_VARIANT cards = sidex_Variant_New_List();
        if(cards != SIDEX_HANDLE_TYPE_NULL)
        {
          SIDEX_INT32 pos = 0;
          iErr = sidex_Variant_List_Append(cards, card, &pos);
          if(iErr == SIDEX_SUCCESS)
          {
            sidex_List_Write(m_sdxParams, S_TP_NETWORK, S_TP_CARDS, cards);
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
      sidex_Integer_Write(m_sdxParams, S_TP_NETWORK, S_TP_FIRSTPORTNUMBER, DEFAULT_PORTNUMBER);
    }

    if(getTestLoopCount() <= 0)
    {
      result = false;
      sidex_Integer_Write(m_sdxParams, S_TP_TEST, S_TP_LOOPCOUNT, 1);
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
    SIDEX_INT32 iErr = sidex_List_Read(m_sdxParams, S_TP_NETWORK, S_TP_CARDS, &cards);
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
      SIDEX_INT32 iErr = sidex_List_Read(m_sdxParams, S_TP_NETWORK, S_TP_CARDS, &cards);
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
  int firstPortNumber = 0;
  if(hasParams())
  {
    SIDEX_INT64 value = 0;
    SIDEX_INT32 iErr = sidex_Integer_Read(m_sdxParams, S_TP_NETWORK, S_TP_FIRSTPORTNUMBER, &value);
    if(iErr == SIDEX_SUCCESS) firstPortNumber = (int)value;
  }
  return(firstPortNumber);
}

int CTestParams::getTestLoopCount()
{
  int nLoops = 0;
  if(hasParams())
  {
    SIDEX_INT64 value = 0;
    SIDEX_INT32 iErr = sidex_Integer_Read(m_sdxParams, S_TP_TEST, S_TP_LOOPCOUNT, &value);
    if(iErr == SIDEX_SUCCESS) nLoops = (int)value;
  }
  return(nLoops);
}
