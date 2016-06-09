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
#ifndef TESTTMLCORE_H
#define TESTTMLCORE_H

#include "TestingForReturns.h"

class TmlCore : public TestingForReturns
{
public:
	TmlCore(SIDEX_TCHAR* testProcessName = NULL);
	~TmlCore();

	array<TML_LISTENER_HANDLE, AMOUNT_OF_LISTENERS> m_listenerHandles;

	array<SIDEX_TCHAR*, MAX_AMOUNT_OF_PROFILES> getProfileNames();
	TML_CORE_HANDLE getCore();
	SIDEX_TCHAR* getPort();
	SIDEX_TCHAR* getIP();

	TML_INT32 defaultInit(bool listenerSide);
	TML_INT32 initCore();
	TML_INT32 addProfileToCore(SIDEX_TCHAR* profile);
	TML_INT32 setDefaultProfile();
	TML_INT32 setDefaultPort();
	TML_INT32 setPort(SIDEX_TCHAR* port);
	TML_INT32 setDefaultIP();
	TML_INT32 startListeners(bool listenerSide);
	TML_INT32 freeTmlCore();
	TML_INT32 registerDefaultCmds(array<int, AMOUNT_OF_CMDS> cmdCodes);
	bool isInitialized();	
	TML_INT32 initListeners(bool listenerSide);
	
protected:
	TML_CORE_HANDLE m_core;
	int amountOfProfiles;
	array<SIDEX_TCHAR*, AMOUNT_OF_LISTENERS> m_listenerAddresses;
	array<SIDEX_TCHAR*, MAX_AMOUNT_OF_PROFILES> m_profileNames;
	SIDEX_TCHAR* m_ip;
	SIDEX_TCHAR* m_port;
	void initArrays();

	void appendProfileToList(SIDEX_TCHAR* profile);
	SIDEX_VARIANT formatToSidexString(SIDEX_TCHAR* profile);
};

#endif	//TESTTMLCORE_H
