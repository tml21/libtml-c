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

#ifndef TML_CONNECTION_TESTER_H
#define TML_CONNECTION_TESTER_H

#include "TmlTester.h"

#define MAX_ADDRESSES 10

//------------------------------------------------------------------------------

class TmlConnectionTester;

//------------------------------------------------------------------------------

enum cbType_t
{
  cbtOnConnect = 0, cbtOnDisconnect,
  cbtOnCommand, cbtOnCmdConnectionCommand,
  cbtOnEvent, cbtOnBalancer,
  cbt_TypeCount // type count in enum
};
// Note: keep cbType_t and cbt_Name in sync! (same item count)
extern SIDEX_TCHAR* cbt_Name[];

struct cbData_t
{
  TmlConnectionTester* tester;
  cbType_t             type;
  int                  iCore;
  int                  iValue;
  SIDEX_TCHAR*         sValue;
};

//------------------------------------------------------------------------------

class TmlConnectionTester : public TmlTester
{
private:
  volatile int m_cbLog_Connection;
  volatile int m_cbLog_Disconnection;
  volatile int m_cbLog_Command;
  volatile int m_cbLog_Event;
  volatile int m_cbLog_Balancer;

protected:
  virtual void _prepare();
  virtual void _cleanup();

public:
  void OnDisConnectionCallback(TML_CONNECTION_HANDLE hConnection, cbData_t* cbData);
  void OnCommandCallback(TML_COMMAND_HANDLE hCommand, cbData_t* cbData);
  void OnCommandConnectionCallback(TML_COMMAND_HANDLE hCommand, cbData_t* cbData);

public:
  TmlConnectionTester(SIDEX_TCHAR* testProcessName = NULL);
  ~TmlConnectionTester();

  bool testConnect();
  bool testClose();
  bool testGetAddress();
  bool testGetRemoteProfiles();
  bool testValidate();
  bool testGetConnectionCount();
  bool testGetConnection_Core();
  bool testGetConnectionByAddress();
  bool testSetOnConnect();
  bool testSetOnDisconnect();
  bool testSendSync();
  bool testSendAsync();
  bool testGetConnection_Cmd();
  bool testConnectionEvents();
  bool testConnectionBalancers();
};

#endif  // TML_CONNECTION_TESTER_H
