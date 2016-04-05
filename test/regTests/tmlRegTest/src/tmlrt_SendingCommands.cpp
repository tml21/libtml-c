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
#include "TestingProcess.h"

/** @ingroup Test_Sending_Commands
* @brief Testing a simple communication between a sender and a listener
* 
* A sender, a listener and a command are initialized and the command is synchronously(abbr. sync) send 
* to the listener. It is checked at the listener if the data, that was send with the command,
* stayed the same and is then overwritten. After the reply of the listener, the data is checked again.
*
* @returns bool : false on failure of test, true on success.
*/
bool simpleTestTmlSendSyncMessage()
{
  bool success = false;
  TestingProcess* simpleSendSyncMessage = new TestingProcess(tmlrtT("simpleSendSyncMessage"));
  if(simpleSendSyncMessage)
  {
    simpleSendSyncMessage->messageOutput(S_START);

    do
    {
      simpleSendSyncMessage->defaultListenerInit();
      if(!simpleSendSyncMessage->isTestOK()) break;

      simpleSendSyncMessage->initSenderSide();
      if(!simpleSendSyncMessage->isTestOK()) break;

      simpleSendSyncMessage->sendArbitraryCmds();
      if(!simpleSendSyncMessage->isTestOK()) break;

      simpleSendSyncMessage->freeCmds();
      if(!simpleSendSyncMessage->isTestOK()) break;

      simpleSendSyncMessage->freeTmlCores();
      if(!simpleSendSyncMessage->isTestOK()) break;
    }
    while(false);

    success = simpleSendSyncMessage->isTestOK();
    simpleSendSyncMessage->messageOutput(success ? S_FINISH_SUCCESS : S_FINISH_FAILED);

    delete(simpleSendSyncMessage);
    simpleSendSyncMessage = NULL;
  }
  return(success);
}
