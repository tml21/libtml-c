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
	m_core = TML_HANDLE_TYPE_NULL;
	m_errorLocationOutput = NULL;

	setListenerAddresses();
	setErrorLocationOutput(name);
	m_profileNames = new TmlList (m_errorLocationOutput);
	m_ip = tmlrtT("");
	m_port = tmlrtT("");
}

TmlCore::~TmlCore() {
	if(NULL != m_errorLocationOutput) {
		delete[] m_errorLocationOutput;
	}

	if (TML_HANDLE_TYPE_NULL != m_profileNames){
		delete m_profileNames;
	}
}

TML_INT32 TmlCore::defaultInit() { 
	m_iErr = initCore();
	checkForSuccess();
	m_iErr = initListeners();
	checkForSuccess();
	m_iErr = setDefaultProfile();
	checkForSuccess();
	m_iErr = startListeners();
	checkForSuccess();
	return m_iErr;
}

TML_INT32 TmlCore::initCore() {
	m_iErr = tml_Core_Open(&m_core, 0);
	checkForSuccess();
	return m_iErr;
}

TML_INT32 TmlCore::addProfileToCore(SIDEX_TCHAR* profile) {
	appendProfileToList(profile);
	m_iErr = tml_Profile_Register (m_core, profile);	
	checkForSuccess();
	return m_iErr;
}

SIDEX_VARIANT TmlCore::formatToSidexString(SIDEX_TCHAR* profile) {
	SIDEX_VARIANT formattedString = SIDEX_HANDLE_TYPE_NULL;
	m_iErr = sidex_Variant_New_String(profile, &formattedString);
	checkForSuccess();
	return formattedString;
}

void TmlCore::appendProfileToList(SIDEX_TCHAR* profile) {
	m_iErr = m_profileNames->append(profile);
	m_iErr = tml_Profile_Register(m_core,profile);
	checkForSuccess();
}

TML_INT32 TmlCore::setDefaultProfile() {
	appendProfileToList(IO_PROFILE);
	checkForSuccess();
	return m_iErr;
}

TML_INT32 TmlCore::setDefaultPort() {
	m_iErr = setPort(IO_PORT);
	checkForSuccess();
	return m_iErr;
}

TML_INT32 TmlCore::setPort(SIDEX_TCHAR* port) {
	m_port = port;
	m_iErr = tml_Core_Set_ListenerPort(m_core, m_port);
	checkForSuccess();
	return m_iErr;
}

TML_INT32 TmlCore::setDefaultIP() {
	m_ip = LISTENER_NETWORK_INTERFACE_IP;
	m_iErr = tml_Core_Set_ListenerIP (m_core, LISTENER_NETWORK_INTERFACE_IP);
	checkForSuccess();
	return m_iErr;
}

TML_INT32 TmlCore::startListeners() {
	for ( int i = 0; i < m_listenerHandles.size(); i++) {
		m_iErr = tml_Listener_Set_Enabled (m_listenerHandles[i], TML_TRUE);
		checkForSuccess();
	}
	return m_iErr;
}

TML_INT32 TmlCore::freeTmlCore() {
	m_iErr = tml_Core_Close(&m_core);	//listeners are getting closed here automatically
	checkForSuccess();
	return m_iErr;
}

bool TmlCore::isInitialized() {
	bool coreInitialized = true;
	if(TML_HANDLE_TYPE_NULL == m_core) {
		coreInitialized = false;
	}
	return coreInitialized;
}

TML_CORE_HANDLE TmlCore::getCore() {
	return m_core;
}

TmlList* TmlCore::getProfileNames() {
	return m_profileNames;
}

SIDEX_TCHAR* TmlCore::getPort() {
	return m_port;
}

SIDEX_TCHAR* TmlCore::getIP() {
	return m_ip;
}

TML_INT32 TmlCore::registerDefaultCmds(array<int, 5> cmdCodes) {
	TML_INT32 amountOfProfiles = m_profileNames->size();
	TML_INT32 amountOfCmdCodes = cmdCodes.size();
	SIDEX_TCHAR* nameFromProfiles;
	TML_INT32 cmdCodeFromList;
	for(int i = 0; i < amountOfProfiles; i++) {
		for(int j = 0; j < amountOfCmdCodes; j++) {
			nameFromProfiles = m_profileNames->getString(i);
			cmdCodeFromList = cmdCodes.at(j);
			m_iErr = tml_Profile_Register_Cmd(m_core, nameFromProfiles, (TML_COMMAND_ID_TYPE) cmdCodes.at(j), cbgenericCmd, TML_HANDLE_TYPE_NULL);
			checkForSuccess();
		}
	}
	return m_iErr;
}

void TmlCore::setErrorLocationOutput(SIDEX_TCHAR* outputInCaseOfError) {
#if defined SIDEX_UNICODE || defined TML_UNICODE
	m_errorLocationOutput = new wchar_t[50]();
#else
	m_errorLocationOutput = new char[50]();
#endif
	tmlrt_cat(m_errorLocationOutput, outputInCaseOfError);
	tmlrt_cat(m_errorLocationOutput, tmlrtT(" - TmlCore"));
}

TML_INT32 TmlCore::initListeners() {
	for (int i = 0; i < m_listenerHandles.size(); i++) {
		m_iErr = tml_Core_Listener_Create( m_core, m_listenerAddresses[i], &m_listenerHandles[i]);
		checkForSuccess();
	}
	return m_iErr;
}

void TmlCore::setListenerAddresses() {
	m_listenerAddresses = LISTENERS_ADDRESS;
}


