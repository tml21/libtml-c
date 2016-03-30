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

#ifndef TESTINGFORRETURNS_H
#define TESTINGFORRETURNS_H

#include "tmlrt_Utils.h"

class TestingForReturns
{ //TODO auﬂer mit namen sehr zufrieden
protected:
  SIDEX_TCHAR* m_testProcessName;
  SIDEX_TCHAR* m_testSectionName;
  TML_INT32    m_iErr;
  bool         m_testOK, m_testOK_Overall;

public:
  TestingForReturns(SIDEX_TCHAR* testProcessName = NULL);
  ~TestingForReturns();

  void         reset(bool overall = false);
  void         setTestSectionName(SIDEX_TCHAR* testSectionName = NULL);

  SIDEX_TCHAR* getTestProcessName();
  SIDEX_TCHAR* getTestSectionName();
  TML_INT32    getLastErrorCode();
  bool         isTestOK();

  void messageOutput(SIDEX_TCHAR* messageText = NULL,
                     bool withProcessName = true, bool withSectionName = true, bool deleteText = false);
  void contentOutput(SIDEX_TCHAR* name, SIDEX_TCHAR* content,
                     bool withProcessName = true, bool withSectionName = true, bool deleteName = false, bool deleteContent = false);
  void numberOutput(SIDEX_TCHAR* name, SIDEX_INT32 number,
                     bool withProcessName = true, bool withSectionName = true, bool deleteName = false);
  void indexOutput(SIDEX_TCHAR* arrayName, SIDEX_INT32 index, SIDEX_TCHAR* content,
                     bool withProcessName = true, bool withSectionName = true, bool deleteArrayName = false, bool deleteContent = false);

protected:
  void errorOutput(SIDEX_TCHAR* messageText = NULL, bool withErrorCode = true, bool deleteText = false);

  bool checkForExpectedReturnCode(TML_INT32 expectedReturnCode, SIDEX_TCHAR* messageText = NULL, bool deleteText = false);
  bool checkForSuccess(SIDEX_TCHAR* messageText = NULL, bool deleteText = false);

  bool checkForValue(SIDEX_TCHAR* name, SIDEX_INT32 desiredValue, SIDEX_INT32 actualValue, bool deleteName = false);
};

#endif  //TESTINGFORRETURNS_H
