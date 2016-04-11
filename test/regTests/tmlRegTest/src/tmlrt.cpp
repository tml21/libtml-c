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

#include "tmlrt_Utils.h"
#include "TestParams.h"
#include "tmlrt_Connections.h"
#include "tmlrt_MultipleListeners.h"

/** @brief Main function, accepts command line parameters
* @param int argc : contains amount of arguments in argv
* @param char* argv[] : contains input from user
* @returns int : 0 if everything went okay, -1 if a failure/error occurred or the options of the user were not recognized
*/
#ifdef LINUX
  int main(int argc, char* argv[])
#else //LINUX
  int _tmain(int argc, char* argv[])
#endif
{
  int result = -1;

  wcout << "=======================" << endl;
  wcout << "  TML regression test  " << endl;
  wcout << "=======================" << endl;
  wcout << endl;

  wcout << (8 * sizeof(int)) << "bit" << endl;
  wcout << "UTF" << (8 * sizeof(SIDEX_TCHAR)) << endl;
  wcout << endl;

  wcout << "Programm is called with " << (argc - 1) << " parameter" << ((argc > 2) ? "s." : ".") << endl;
  for(int i = 1; i < argc; i++) { wcout << "Param" << i << " = " << argv[i] << endl; }
  wcout << endl;

  SIDEX_TCHAR* pArg = NULL;
  SIDEX_TCHAR* pBuf = NULL;
  if(argc > 1)
  {
    SIDEX_INT32 iLength = 0;
    switch(sizeof(SIDEX_TCHAR))
    {
      case 1:
      {
        pArg = (SIDEX_TCHAR*)argv[1];
        break;
      }
      case 2:
      {
        pBuf = (SIDEX_TCHAR*)UTF8toUTF16(argv[1], &iLength);
        pArg = pBuf;
        break;
      }
      case 4:
      {
        pBuf = (SIDEX_TCHAR*)UTF8toUTF32(argv[1], &iLength);
        pArg = pBuf;
        break;
      }
    }
  }

  TestParams = new CTestParams(pArg);
  if(TestParams)
  {
    initGlobalMutex();
    SIDEX_TCHAR* pfn = TestParams->getParamsFileName();
    if(pfn) { wcout << "Using params file: " << pfn << endl; }
    else    { wcout << "Running without params file!" << endl; }
    wcout << endl;

    int i = 0, n = TestParams->getTestLoopCount();
    wcout << "Tests - Start... ( " << n << " loop" << ((n > 1) ? "s )" : " )") << endl;
    wcout << endl;
    do
    {
      wcout << "----------------------------------------" << endl;
      wcout << "  Loop #" << (i + 1) << endl;
      wcout << "----------------------------------------" << endl;
      wcout << endl;

      if(!testTmlConnections()) break;            // test the connection API
      if(!testTmlMultiListeners()) break;         // test the multi listener API

      i++;
    }
    while(i < n);
    if(i == n) result = 0;

    wcout << endl;
    wcout << "Tests - " << (result ? S_FINISH_FAILED : S_FINISH_SUCCESS) << endl;
    wcout << endl;

    DELETE_OBJ(TestParams);
    deleteGlobalMutex();
  }

  DELETE_STR(pBuf);

  wcout << "========================================" << endl;
  wcout << endl;
  return(result);
}
