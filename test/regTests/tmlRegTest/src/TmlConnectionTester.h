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
  cbtOnCommand,
  cbt_TypeCount // type count in enum
};
SIDEX_TCHAR* cbt_Name[];

struct cbData_t
{
  TmlConnectionTester* tester;
  cbType_t             type;
  int                  iCore;
  int                  iValue;
  SIDEX_TCHAR*         sValue;
  bool                 bDeleteString;
};

//------------------------------------------------------------------------------

class TmlConnectionTester : public TmlTester
{
private:
  int  m_cbLog_Connection;
  int  m_cbLog_Disconnection;
  int  m_cbLog_Command;

protected:
  virtual void _prepare();
  virtual void _cleanup();

public:
  void OnConnectionCallback(TML_CONNECTION_HANDLE hConnection, cbData_t* cbData);
  void OnCommandCallback(TML_COMMAND_HANDLE hCommand, cbData_t* cbData);

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
};

#endif  // TML_CONNECTION_TESTER_H
