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
	TmlConnectionTester* conTester = new TmlConnectionTester(tmlrtT("TmlConnectionTester"));
  if(conTester)
  {
    do
    {
      if(!conTester->testConnect()) break;
      if(!conTester->testClose()) break;
      if(!conTester->testGetAddress()) break;
      if(!conTester->testGetRemoteProfiles()) break;
      if(!conTester->testValidate()) break;
      if(!conTester->testGetConnectionCount()) break;
      if(!conTester->testGetConnection_Core()) break;
      if(!conTester->testGetConnectionByAddress()) break;
      if(!conTester->testSendSync()) break;
      if(!conTester->testSendAsync()) break;
      if(!conTester->testGetConnection_Cmd()) break;
      if(!conTester->testSetOnConnect()) break;
      if(!conTester->testSetOnDisconnect()) break;
    }
    while(false);

    success = conTester->isTestOK();

	  delete(conTester);
    conTester = NULL;
  }
  return(success);
}
