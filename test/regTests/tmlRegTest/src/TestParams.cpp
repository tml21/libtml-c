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

CTestParams::CTestParams(SIDEX_TCHAR* testParamsFileName)
{
  bool success     = false;
  m_ParamsFileName = testParamsFileName;
  m_stringBuffer   = NULL;
  m_sdxParams      = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iErr = sidex_Create(tmlrtT("TestParams"), &m_sdxParams);
  if(iErr == SIDEX_SUCCESS)
  {
    if(m_ParamsFileName)
    {
      iErr = sidex_Load_Content(m_sdxParams, m_ParamsFileName);
      success = (iErr == SIDEX_SUCCESS);
    }
  }
  else if(hasParams())
  {
    sidex_Free(&m_sdxParams);
    m_sdxParams = SIDEX_HANDLE_TYPE_NULL;
  }

  if(!success) m_ParamsFileName = NULL;
  ensureDefaultParams();
}

CTestParams::~CTestParams()
{
  m_ParamsFileName = NULL;
  if(m_sdxParams != SIDEX_HANDLE_TYPE_NULL)
  {
    sidex_Free(&m_sdxParams);
    m_sdxParams = SIDEX_HANDLE_TYPE_NULL;
  }
  if(m_stringBuffer)
  {
    delete[](m_stringBuffer);
    m_stringBuffer = NULL;
  }
}

bool CTestParams::hasParams()
{
  return(m_sdxParams != SIDEX_HANDLE_TYPE_NULL);
}

SIDEX_TCHAR* CTestParams::getParamsFileName()
{
  return(m_ParamsFileName);
}

void CTestParams::ensureDefaultParams()
{
  if(hasParams())
  {
    SIDEX_INT32 nCards = getNetworkCardCount();
    if(nCards == 0)
    {
      SIDEX_VARIANT card = SIDEX_HANDLE_TYPE_NULL;
      SIDEX_INT32   iErr = sidex_Variant_New_String(tmlrtT("127.0.0.1"), &card);
      if(iErr == SIDEX_SUCCESS)
      {
        SIDEX_VARIANT cards = sidex_Variant_New_List();
        if(cards != SIDEX_HANDLE_TYPE_NULL)
        {
          SIDEX_INT32 pos = 0;
          iErr = sidex_Variant_List_Append(cards, card, &pos);
          if(iErr == SIDEX_SUCCESS)
          {
            sidex_List_Write(m_sdxParams, tmlrtT("Network"), tmlrtT("Cards"), cards);
          }
          sidex_Variant_DecRef(cards);
          cards = SIDEX_HANDLE_TYPE_NULL;
        }
        sidex_Variant_DecRef(card);
        card = SIDEX_HANDLE_TYPE_NULL;
      }
    }
  }
}

int CTestParams::getNetworkCardCount()
{
  SIDEX_INT32 nCards = 0;
  if(hasParams())
  {
    SIDEX_VARIANT cards = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 iErr = sidex_List_Read(m_sdxParams, tmlrtT("Network"), tmlrtT("Cards"), &cards);
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
  SIDEX_TCHAR* name = NULL;
  if(hasParams())
  {
    SIDEX_INT32 nCards = getNetworkCardCount();
    if((index >= 0) && (index < nCards))
    {
      SIDEX_VARIANT cards = SIDEX_HANDLE_TYPE_NULL;
      SIDEX_INT32 iErr = sidex_List_Read(m_sdxParams, tmlrtT("Network"), tmlrtT("Cards"), &cards);
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
            if(m_stringBuffer) delete[](m_stringBuffer);
            m_stringBuffer = new SIDEX_TCHAR[len + 1];
            if(m_stringBuffer)
            {
              for(SIDEX_INT32 i = 0; i <= len; i++) m_stringBuffer[i] = pCard[i];
              name = m_stringBuffer;
            }
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
  return(name);
}
