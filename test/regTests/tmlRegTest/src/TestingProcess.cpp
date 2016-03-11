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
#include "TmlCore.h"
#include "TestingForReturns.h"
#include "TestingProcess.h"
#include "TmlList.h"


void TestingProcess::initSenderSide() {
	iErr = coreSenderSide.initCore();
	checkForSuccess();
}


TestingProcess::TestingProcess (SIDEX_TCHAR* name) {
	errorLocationOutput = name;
	testOK = true;
	cmdCodes = COMMAND_CODES_LIST;
}

void TestingProcess::defaultListenerInit() {
	coreListenerSide = TmlCore(errorLocationOutput);
	iErr = coreListenerSide.defaultInit();
	checkForSuccess();
	iErr = coreListenerSide.registerDefaultCmds(cmdCodes);
	checkForSuccess();
}

void TestingProcess::freeTmlCores() {
	if(coreListenerSide.isInitialized()) {
		iErr = coreListenerSide.freeTmlCore();
		checkForSuccess();
	}

	if(coreSenderSide.isInitialized()) {
		iErr = coreSenderSide.freeTmlCore();
		checkForSuccess();
	}
}

TML_INT32 TestingProcess::produceCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value) {
	createCmd(cmd, value);

    // Set cmdMessage parameter
    SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
    if (TML_SUCCESS == iErr)
        iErr = tml_Cmd_Acquire_Sidex_Handle(*cmd, &sHandle);
    if (TML_SUCCESS == iErr)
        sidex_Integer_Write(sHandle, GROUP, KEY, value);
    if (TML_SUCCESS == iErr)
        iErr = tml_Cmd_Release_Sidex_Handle(*cmd);

     return iErr;
}

TML_INT32 TestingProcess::createCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value) {
	TML_COMMAND_HANDLE cmdMsg = TML_HANDLE_TYPE_NULL;
    iErr = tml_Cmd_Create (&cmdMsg);
    checkForSuccess();

    iErr = tml_Cmd_Header_SetCommand (cmdMsg, (TML_COMMAND_ID_TYPE) value);
	checkForSuccess();

	if (TML_SUCCESS == iErr)
        *cmd = cmdMsg;
	return iErr;
}

TML_INT32 TestingProcess::sendArbitraryCmd(TML_INT32 cmdIndex, TML_INT32 profileIndex) {
	TML_COMMAND_HANDLE cmdMsg = Cmds[cmdIndex];
	TmlList listOfProfilenames = coreListenerSide.getProfileNames();
	SIDEX_TCHAR* profileURL = listOfProfilenames.getString(profileIndex);

	iErr = tml_Send_SyncMessage(coreSenderSide.getCore(), cmdMsg, profileURL, DESTINATION_HOST_IP, coreListenerSide.getPort(), 10000);
	return iErr;
}

void TestingProcess::sendArbitraryCmds() {
	int numberOfCmdsToSend = 5;
	TML_INT32 indexOfWantedProfile = 0;
	createSetOfCmds();
	for(int i = 0; i < numberOfCmdsToSend; i++) {
		iErr = sendArbitraryCmd(i, indexOfWantedProfile);
		checkForSuccess();
	}
}

void TestingProcess::freeCmds() {
	int sizeOfCmds = 5;
	for( int i = 0; i < sizeOfCmds; i++) {
		freeCmd(&Cmds[i]);
		checkForSuccess();
	}
}

TML_INT32 TestingProcess::freeCmd(TML_COMMAND_HANDLE* cmd) {
	iErr = tml_Cmd_Free(cmd);
	return iErr;
}

TML_INT32 TestingProcess::createSetOfCmds() {
	for(int i = 0; i < amountOfCmds; i++){
		Cmds[i] = TML_HANDLE_TYPE_NULL;
		iErr = produceCmd(&Cmds[i], cmdCodes[i]);
		checkForSuccess();
	}
	return iErr;
}