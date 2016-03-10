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

TmlCore::TmlCore(SIDEX_TCHAR* name) {
	errorLocationOutput = new char[50]();
	tmlrt_cat(errorLocationOutput, name);
	tmlrt_cat(errorLocationOutput, tmlrtT(" - TmlCore"));
	core = TML_HANDLE_TYPE_NULL;
	profileNames = TmlList(errorLocationOutput);
}

TmlCore::~TmlCore() {
	//delete[] errorLocationOutput;
}

TmlCore::TmlCore () {
	core = TML_HANDLE_TYPE_NULL;
}

TML_INT32 TmlCore::defaultInit() { 
	iErr = initCore();
	checkForSuccess();
	iErr = setDefaultIP();
	checkForSuccess();
	iErr = setDefaultPort();
	checkForSuccess();
	iErr = setDefaultProfile();
	checkForSuccess();
	iErr = startListener();
	checkForSuccess();
	return iErr;
}

TML_INT32 TmlCore::initCore() {
	iErr = tml_Core_Open(&core, 0);
	checkForSuccess();
	return iErr;
}

TML_INT32 TmlCore::addProfileToCore(SIDEX_TCHAR* profile) {
	appendProfileToList(profile);
	iErr = tml_Profile_Register (core, profile);	
	checkForSuccess();
	return iErr;
}

SIDEX_VARIANT TmlCore::formatToSidexString(SIDEX_TCHAR* profile) {
	SIDEX_VARIANT formattedString = SIDEX_HANDLE_TYPE_NULL;
	iErr = sidex_Variant_New_String(profile, &formattedString);
	checkForSuccess();
	return formattedString;
}

void TmlCore::appendProfileToList(SIDEX_TCHAR* profile) {
	iErr = profileNames.append(profile);
	iErr = tml_Profile_Register(core,profile);
	checkForSuccess();
}

TML_INT32 TmlCore::setDefaultProfile() {
	appendProfileToList(IO_PROFILE);
	checkForSuccess();
	return iErr;
}

TML_INT32 TmlCore::setDefaultPort() {
	iErr = setPort(IO_PORT);
	checkForSuccess();
	return iErr;
}

TML_INT32 TmlCore::setPort(SIDEX_TCHAR* port) {
	this->port = port;
	iErr = tml_Core_Set_ListenerPort(core, port);
	checkForSuccess();
	return iErr;
}

TML_INT32 TmlCore::setDefaultIP() {
	ip = LISTENER_NETWORK_INTERFACE_IP;
	iErr = tml_Core_Set_ListenerIP (core, LISTENER_NETWORK_INTERFACE_IP);
	checkForSuccess();
	return iErr;
}

TML_INT32 TmlCore::startListener() {
	iErr = tml_Core_Set_ListenerEnabled (core, TML_TRUE);
	checkForSuccess();
	return iErr;
}

TML_INT32 TmlCore::freeTmlCore() {
	iErr = tml_Core_Close(&core);
	checkForSuccess();
	return iErr;
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
	TML_INT32 amountOfProfiles = profileNames.size();
	TML_INT32 amountOfCmdCodes = cmdCodes.size();
	for(int i = 0; i < amountOfProfiles; i++) {
		for(int j = 0; j < amountOfCmdCodes; j++) {
			iErr = tml_Profile_Register_Cmd(core, profileNames.getString(i), (TML_COMMAND_ID_TYPE) cmdCodes.at(j), cbgenericCmd, TML_HANDLE_TYPE_NULL);
			checkForSuccess();
		}
	}
	return iErr;
}