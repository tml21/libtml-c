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
  if(!testProcessName) testProcessName = tmlrtT("<unnamed>");
  m_testProcessName = testProcessName;
  testOK = true;
  iErr   = TML_SUCCESS;
}

TestingForReturns::~TestingForReturns()
{
  m_testProcessName = NULL;
}

void TestingForReturns::messageOutput(SIDEX_TCHAR* messageTextOutput, bool withProcessName, SIDEX_TCHAR* testFunctionName)
{
  if(withProcessName) wcout << m_testProcessName;
  if(testFunctionName)
  {
    if(testFunctionName[0]) wcout << ":" << testFunctionName;
  }
  if(messageTextOutput)
  {
    if(messageTextOutput[0]) wcout << " - " << messageTextOutput;
  }
  wcout << endl;
}

void TestingForReturns::errorOutput(SIDEX_TCHAR* testFunctionName)
{
  //TODO only output, enough output for locating?
  wcout << "Test failed at " << m_testProcessName;
  if(testFunctionName)
  {
    if(testFunctionName[0]) wcout << ":" << testFunctionName;
  }
  wcout << " with ErrorCode " << iErr << endl;
  testOK = false;
}

void TestingForReturns::checkForExpectedReturnCode(TML_INT32 expectedReturnCode, SIDEX_TCHAR* testFunctionName)
{
  if(expectedReturnCode != iErr) errorOutput(testFunctionName);
}

void TestingForReturns::checkForSuccess(SIDEX_TCHAR* testFunctionName)
{
  checkForExpectedReturnCode(TML_SUCCESS, testFunctionName);
}
