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

#ifndef TML_TESTER_H
#define TML_TESTER_H

#include "TestingForReturns.h"

#define MAX_CORES     9
#define MAX_LISTENERS 9 // max. number of listeners per core
#define MAX_MUTEXES   4

class TmlTester : public TestingForReturns
{
protected:
  TML_CORE_HANDLE     m_core[MAX_CORES];
  TML_LISTENER_HANDLE m_listener[MAX_CORES][MAX_LISTENERS];
  VortexMutex         m_mutex[MAX_MUTEXES];

protected:
  bool createCore(int iCore);
  bool deleteCore(int iCore);
  bool createListener(int iCore, int iListener, SIDEX_TCHAR* sAddress);
  bool startListener(int iCore, int iListener);
  bool stopListener(int iCore, int iListener);
  bool deleteListener(int iCore, int iListener);

  bool splitCoreAndListener(int& iCore, int& iListener);

  virtual void _prepare() {};
  virtual void _cleanup() {};

  bool checkConnectionCount(int iCore, int nExpectedConnections, SIDEX_TCHAR* debugText = NULL);
  bool checkRemoteProfileCount(TML_CONNECTION_HANDLE hConnection, int nExpectedProfiles, SIDEX_TCHAR* debugText = NULL);

  SIDEX_INT32  getSidexListCount(SIDEX_VARIANT vList, SIDEX_TCHAR* debugText = NULL);
  SIDEX_TCHAR* getSidexListStringItem(SIDEX_VARIANT vList, SIDEX_INT32 index, SIDEX_TCHAR* debugText = NULL); // delete after use!

  bool         setCommandInt(TML_COMMAND_HANDLE hCommand, SIDEX_TCHAR* group, SIDEX_TCHAR* key, TML_INT32 iValue);
  TML_INT32    getCommandInt(TML_COMMAND_HANDLE hCommand, SIDEX_TCHAR* group, SIDEX_TCHAR* key, TML_INT32 iDefault = 0);

  bool         enterMutex(int iMutex);
  bool         leaveMutex(int iMutex);

public:
  TmlTester(SIDEX_TCHAR* testProcessName = NULL);
  ~TmlTester();

  bool prepare();
  bool cleanup(bool silent = false);

  TML_CORE_HANDLE     getCore(int iCore);
  TML_LISTENER_HANDLE getListener(int iCore, int iListener);
};

#endif  // TML_TESTER_H
