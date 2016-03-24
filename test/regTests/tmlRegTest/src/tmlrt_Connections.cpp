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

#include "tmlrt_Connections.h"
#include "TmlConnectionTester.h"

/** @ingroup Test_Connections
* @brief Test of the TML connection API
*
* A sender, a listener and a command are initialized and the command is synchronously(abbr. sync) send
* to the listener. It is checked at the listener if the data, that was send with the command,
* stayed the same and is then overwritten. After the reply of the listener, the data is checked again.
*
* @returns bool : false on failure of test, true on success.
*/
bool testTmlConnections()
{
  bool success = false;
	TmlConnectionTester* tester = new TmlConnectionTester(tmlrtT("TmlConnectionTester"));
  if(tester)
  {
    do
    {
      if(!tester->prepare()) break;

      if(!tester->testConnect()) break;
      if(!tester->testClose()) break;
      if(!tester->testGetAddress()) break;
      if(!tester->testGetRemoteProfiles()) break;
      if(!tester->testValidate()) break;
      if(!tester->testGetConnectionCount()) break;
      if(!tester->testGetConnection_Core()) break;
      if(!tester->testGetConnectionByAddress()) break;
      if(!tester->testSendSync()) break;
      if(!tester->testSendAsync()) break;
      if(!tester->testGetConnection_Cmd()) break;
      if(!tester->testSetOnConnect()) break;
      if(!tester->testSetOnDisconnect()) break;
    }
    while(false);

    tester->cleanup();
    success = tester->isTestOK();
	  DELETE_OBJ(tester);
  }
  return(success);
}
