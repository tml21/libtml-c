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

#include "TestingForReturns.h"

TestingForReturns::TestingForReturns(SIDEX_TCHAR* testProcessName)
{
  if(testProcessName && (!testProcessName[0])) testProcessName = NULL;
  if(!testProcessName) testProcessName = S_UNNAMED;
  m_testProcessName = testProcessName;
  m_testSectionName = S_EMPTY;
  m_testOK_Overall  = true;
  m_testOK          = true;
  m_iErr            = TML_SUCCESS;
}

TestingForReturns::~TestingForReturns()
{
  m_testProcessName = NULL;
  m_testSectionName = NULL;
}

void TestingForReturns::reset(bool overall)
{
  m_testOK = true;
  m_iErr   = TML_SUCCESS;
  if(overall) m_testOK_Overall = true;
}

void TestingForReturns::setTestSectionName(SIDEX_TCHAR* testSectionName)
{
  if(testSectionName && (!testSectionName[0])) testSectionName = NULL;
  if(!testSectionName) testSectionName = S_EMPTY;
  m_testSectionName = testSectionName;
}

SIDEX_TCHAR* TestingForReturns::getTestProcessName()
{
  return(m_testProcessName);
}

SIDEX_TCHAR* TestingForReturns::getTestSectionName()
{
  return(m_testSectionName);
}

TML_INT32 TestingForReturns::getLastErrorCode()
{
  return(m_iErr);
}

bool TestingForReturns::isTestOK()
{
  return(m_testOK_Overall);
}

void TestingForReturns::messageOutput(SIDEX_TCHAR* messageText, bool withProcessName, bool withSectionName, bool deleteText)
{
  if(enterGlobalMutex())
  {
    if(messageText)
    {
      bool hasText = (withProcessName && m_testProcessName);
      if(hasText) wcout << m_testProcessName;
      if(withSectionName && m_testSectionName)
      {
        if(m_testSectionName[0])
        {
          if(hasText) wcout << ":";
          wcout << m_testSectionName;
          hasText = true;
        }
      }
      if(messageText[0])
      {
        if(hasText) wcout << " - ";
        wcout << messageText;
        hasText = true;
      }
      if(deleteText) DELETE_STR(messageText);
    }
    wcout << endl;
    leaveGlobalMutex();
  }
}

void TestingForReturns::contentOutput(SIDEX_TCHAR* name, SIDEX_TCHAR* content,
                                      bool withProcessName, bool withSectionName, bool deleteName, bool deleteContent)
{
  messageOutput(tmlrt_cat(name, tmlrtT(" = "), content, (deleteName ? 1 : 0) | (deleteContent ? 4 : 0)),
                withProcessName, withSectionName, true);
}

void TestingForReturns::numberOutput(SIDEX_TCHAR* name, SIDEX_INT32 number,
                                     bool withProcessName, bool withSectionName, bool deleteName)
{
  contentOutput(name, tmlrt_itoa(number), withProcessName, withSectionName, deleteName, true);
}

void TestingForReturns::indexOutput(SIDEX_TCHAR* arrayName, SIDEX_INT32 index, SIDEX_TCHAR* content,
                                    bool withProcessName, bool withSectionName, bool deleteArrayName, bool deleteContent)
{
  contentOutput(tmlrt_cat(arrayName,
                          tmlrt_cat(tmlrtT("["),
                                    tmlrt_itoa(index),
                                    tmlrtT("]"), 2),
                          NULL, deleteArrayName ? 1 : 0),
                content, withProcessName, withSectionName, true, deleteContent);
}

void TestingForReturns::errorOutput(SIDEX_TCHAR* messageText, bool withErrorCode, bool deleteText)
{
  if(enterGlobalMutex())
  {
    wcout << "Test failed at " << m_testProcessName;
    if(m_testSectionName)
    {
      if(m_testSectionName[0]) wcout << ":" << m_testSectionName;
    }
    if(messageText)
    {
      if(messageText[0]) wcout << ":" << messageText;
      if(deleteText) DELETE_STR(messageText);
    }
    if(withErrorCode) wcout << " with ErrorCode " << m_iErr;
    wcout << endl;
    leaveGlobalMutex();
  }
  m_testOK         = false;
  m_testOK_Overall = false;
}

bool TestingForReturns::checkForExpectedReturnCode(TML_INT32    expectedReturnCode,
                                                   SIDEX_TCHAR* messageText,
                                                   bool         deleteText)
{
  bool result = (expectedReturnCode == m_iErr);
  if(!result)
  {
    errorOutput(tmlrt_cat(messageText,
                          tmlrt_cat(tmlrtT(" Expected was "),
                                    tmlrt_itoa(expectedReturnCode),
                                    tmlrtT(", but returned"), 2)),
                true, true);
  }
  if(deleteText && messageText) DELETE_STR(messageText);
  return(result);
}

bool TestingForReturns::checkForSuccess(SIDEX_TCHAR* messageText, bool deleteText)
{
  return(checkForExpectedReturnCode(TML_SUCCESS, messageText, deleteText));
}

bool TestingForReturns::checkForValue(SIDEX_TCHAR* name, SIDEX_INT32 desiredValue, SIDEX_INT32 actualValue, bool deleteName)
{
  bool result = (actualValue == desiredValue);
  if(!result)
  {
    errorOutput(tmlrt_cat(tmlrt_cat(name,
                                    tmlrtT(" expected as "),
                                    tmlrt_itoa(desiredValue),
                                    deleteName ? 5 : 4),
                          tmlrtT(", but is "),
                          tmlrt_itoa(actualValue), 5), false, true);
  }
  return(result);
}
