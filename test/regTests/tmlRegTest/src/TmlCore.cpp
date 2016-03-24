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
#include <iostream>
using namespace std;
#include <sidex.h>
#include <tmlCore.h>
#include "TestingForReturns.h"
#include "TmlList.h"
#include "tmlrt_Utils.h"
#include "TmlCore.h"

TmlCore::TmlCore(SIDEX_TCHAR* testProcessName)
       : TestingForReturns(testProcessName)
{
// RK: Commented because ist did not build
//	m_errorLocationOutput = new char[50]();
//	tmlrt_cat(m_errorLocationOutput, name);
//	tmlrt_cat(m_errorLocationOutput, tmlrtT(" - TmlCore"));
	core = TML_HANDLE_TYPE_NULL;
	profileNames = TmlList(m_testProcessName);
}

TmlCore::~TmlCore() {
	//delete[] m_errorLocationOutput;
}

TML_INT32 TmlCore::defaultInit()
{
  do
  {
	  m_iErr = initCore();
	  if(!checkForSuccess(tmlrtT("initCore"))) break;

    m_iErr = setDefaultIP();
    if(!checkForSuccess(tmlrtT("setDefaultIP"))) break;

    m_iErr = setDefaultPort();
    if(!checkForSuccess(tmlrtT("setDefaultPort"))) break;

    m_iErr = setDefaultProfile();
    if(!checkForSuccess(tmlrtT("setDefaultProfile"))) break;

    m_iErr = startListener();
    if(!checkForSuccess(tmlrtT("startListener"))) break;
  }
  while(false);
	return m_iErr;
}

TML_INT32 TmlCore::initCore() {
	m_iErr = tml_Core_Open(&core, 0);
	checkForSuccess(tmlrtT("tml_Core_Open"));
	return m_iErr;
}

TML_INT32 TmlCore::addProfileToCore(SIDEX_TCHAR* profile) {
	appendProfileToList(profile);
	m_iErr = tml_Profile_Register (core, profile);	
	checkForSuccess(tmlrtT("tml_Profile_Register"));
	return m_iErr;
}

SIDEX_VARIANT TmlCore::formatToSidexString(SIDEX_TCHAR* profile) {
	SIDEX_VARIANT formattedString = SIDEX_HANDLE_TYPE_NULL;
	m_iErr = sidex_Variant_New_String(profile, &formattedString);
	checkForSuccess(tmlrtT("sidex_Variant_New_String"));
	return formattedString;
}

void TmlCore::appendProfileToList(SIDEX_TCHAR* profile) {
  m_iErr = profileNames.append(profile);
  if(checkForSuccess(tmlrtT("profileNames.append")))
  {
    m_iErr = tml_Profile_Register(core,profile);
    checkForSuccess(tmlrtT("tml_Profile_Register"));
  }
}

TML_INT32 TmlCore::setDefaultProfile() {
	appendProfileToList(IO_PROFILE);
	checkForSuccess(tmlrtT("appendProfileToList"));
	return m_iErr;
}

TML_INT32 TmlCore::setDefaultPort() {
	m_iErr = setPort(IO_PORT);
	checkForSuccess(tmlrtT("setPort"));
	return m_iErr;
}

TML_INT32 TmlCore::setPort(SIDEX_TCHAR* port) {
	this->port = port;
	m_iErr = tml_Core_Set_ListenerPort(core, port);
	checkForSuccess(tmlrtT("tml_Core_Set_ListenerPort"));
	return m_iErr;
}

TML_INT32 TmlCore::setDefaultIP() {
	ip = LISTENER_NETWORK_INTERFACE_IP;
	m_iErr = tml_Core_Set_ListenerIP (core, LISTENER_NETWORK_INTERFACE_IP);
	checkForSuccess(tmlrtT("tml_Core_Set_ListenerIP"));
	return m_iErr;
}

TML_INT32 TmlCore::startListener() {
	m_iErr = tml_Core_Set_ListenerEnabled (core, TML_TRUE);
	checkForSuccess(tmlrtT("tml_Core_Set_ListenerEnabled"));
	return m_iErr;
}

TML_INT32 TmlCore::freeTmlCore() {
	m_iErr = tml_Core_Close(&core);
	checkForSuccess(tmlrtT("tml_Core_Close"));
	return m_iErr;
}

bool TmlCore::isInitialized() {
	bool coreInitialized = true;
	if(core == TML_HANDLE_TYPE_NULL) {
		coreInitialized = false;
	}
	return coreInitialized;
}

TML_CORE_HANDLE TmlCore::getCore() {
	return core;
}

TmlList TmlCore::getProfileNames() {
	return profileNames;
}

SIDEX_TCHAR* TmlCore::getPort() {
	return port;
}

SIDEX_TCHAR* TmlCore::getIP() {
	return ip;
}

TML_INT32 TmlCore::registerDefaultCmds(array<int, 5> cmdCodes) {
  bool stopTest = false;
	TML_INT32 amountOfProfiles = profileNames.size();
	TML_INT32 amountOfCmdCodes = cmdCodes.size();
	for(int i = 0; i < amountOfProfiles; i++) {
		for(int j = 0; j < amountOfCmdCodes; j++) {
			m_iErr = tml_Profile_Register_Cmd(core, profileNames.getString(i), (TML_COMMAND_ID_TYPE) cmdCodes.at(j), cbgenericCmd, TML_HANDLE_TYPE_NULL);
			stopTest = !checkForSuccess(tmlrtT("tml_Profile_Register_Cmd"));
      if(stopTest) break;
		}
    if(stopTest) break;
	}
	return m_iErr;
}
