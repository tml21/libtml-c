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

#ifndef TML_TESTING_SENDING_COMMANDS_H
#define TML_TESTING_SENDING_COMMANDS_H

#include "TmlTester.h"
#include "TmlCore.h"

typedef struct 
{
    bool blockUntilAsyncReturn;
    int  sleepTimer; 
        
}AsyncCallbackData;

class TmlSendingCommandsTester : public TmlTester
{
private:
  AsyncCallbackData aData;
  	
protected:
  void _prepare();
  //virtual void _cleanup();


public:
  TmlSendingCommandsTester(SIDEX_TCHAR* testProcessName = NULL);
  ~TmlSendingCommandsTester(void);

  static void callBackAsyncCmd(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData);
  static void listenerCallBack(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData);
  static void asyncProgressDisplay(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData, TML_INT32 iProgress);
 
 	bool testSyncMessage();
  bool testAsyncMessage();

	
};

#endif 