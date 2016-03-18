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


TestingProcess::TestingProcess (SIDEX_TCHAR* name)
	:TestingForReturns()
{
	m_errorLocationOutput = name;
	cmdCodes = COMMAND_CODES_LIST;
}

TestingProcess::~TestingProcess () {
  if (NULL != m_coreListenerSide){
    delete m_coreListenerSide;
  }
  if (NULL != m_coreSenderSide){
    delete m_coreSenderSide;
  }
}

void TestingProcess::defaultListenerInit() {
	m_coreListenerSide = new TmlCore(m_errorLocationOutput);
	m_iErr = m_coreListenerSide->defaultInit();
	checkForSuccess();
	m_iErr = m_coreListenerSide->registerDefaultCmds(cmdCodes);
	checkForSuccess();
}

void TestingProcess::freeTmlCores() {
	if(m_coreListenerSide->isInitialized()) {
		m_iErr = m_coreListenerSide->freeTmlCore();
		checkForSuccess();
	}

	if(m_coreSenderSide->isInitialized()) {
		m_iErr = m_coreSenderSide->freeTmlCore();
		checkForSuccess();
	}
}

void TestingProcess::initSenderSide() {
	m_coreSenderSide = new TmlCore(m_errorLocationOutput);
	m_iErr = m_coreSenderSide->initCore();
	checkForSuccess();
}

TML_INT32 TestingProcess::produceCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value) {
	createCmd(cmd, value);

    // Set cmdMessage parameter
    SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
    if (TML_SUCCESS == m_iErr)
        m_iErr = tml_Cmd_Acquire_Sidex_Handle(*cmd, &sHandle);
    if (TML_SUCCESS == m_iErr)
        sidex_Integer_Write(sHandle, GROUP, KEY, value);
    if (TML_SUCCESS == m_iErr)
        m_iErr = tml_Cmd_Release_Sidex_Handle(*cmd);

     return m_iErr;
}

TML_INT32 TestingProcess::createCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value) {
	TML_COMMAND_HANDLE cmdMsg = TML_HANDLE_TYPE_NULL;
    m_iErr = tml_Cmd_Create (&cmdMsg);
    checkForSuccess();

    m_iErr = tml_Cmd_Header_SetCommand (cmdMsg, (TML_COMMAND_ID_TYPE) value);
	checkForSuccess();

	if (TML_SUCCESS == m_iErr)
        *cmd = cmdMsg;
	return m_iErr;
}

TML_INT32 TestingProcess::sendArbitraryCmd(TML_INT32 cmdIndex, TML_INT32 profileIndex) {
	TML_COMMAND_HANDLE cmdMsg = Cmds[cmdIndex];
	TmlList* listOfProfilenames = m_coreListenerSide->getProfileNames();
	SIDEX_TCHAR* profileURL = listOfProfilenames->getString(profileIndex);
	SIDEX_TCHAR* listenerPort = LISTENERS_PORTS[cmdIndex];

	m_iErr = tml_Send_SyncMessage(m_coreSenderSide->getCore(), cmdMsg, profileURL, DESTINATION_HOST_IP, listenerPort, 10000);
	return m_iErr;
}

void TestingProcess::sendArbitraryCmds() {
	int numberOfCmdsToSend = 5;
	TML_INT32 indexOfWantedProfile = 0;
	createSetOfCmds();
	for(int i = 0; i < numberOfCmdsToSend; i++) {
		m_iErr = sendArbitraryCmd(i, indexOfWantedProfile);
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
	m_iErr = tml_Cmd_Free(cmd);
	return m_iErr;
}

TML_INT32 TestingProcess::createSetOfCmds() {
	for(int i = 0; i < amountOfCmds; i++){
		Cmds[i] = TML_HANDLE_TYPE_NULL;
		m_iErr = produceCmd(&Cmds[i], cmdCodes[i]);
		checkForSuccess();
	}
	return m_iErr;
}

void TestingProcess::checkListenerCount(int count) {
	TML_UINT32 listenerCount;
	m_iErr = tml_Core_Get_ListenerCount(m_coreListenerSide->getCore(), &listenerCount);
	checkForSuccess();
	if (count != listenerCount) {
		m_iErr = 4;
		errorOutput();
	}
}

void TestingProcess::checkListenerAdresses() {
	SIDEX_TCHAR* listenerAddress;
	int apiReturnCode;
	bool testOK = true;
	TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
	for (int i = 0; i < m_coreListenerSide->amountOfListeners; i++) {
		m_iErr = tml_Core_Get_Listener(m_coreListenerSide->getCore(), i, &listenerHandle);
		checkForSuccess();
		m_iErr = tml_Listener_Get_Address(listenerHandle, &listenerAddress);
		checkForSuccess();
		apiReturnCode = tmlrt_cmp(listenerAddress, LISTENERS_ADDRESS[i]);
		if (0 != apiReturnCode) {
			testOK = false;
		}
	}
	if (!testOK) {
		m_iErr = 4;			//TODO or rather 0?
		errorOutput();
	}
}

void TestingProcess::checkForDecreasedListenersAfterOneIsClosed() {
	TML_LISTENER_HANDLE listenerHandleToBeClosed = TML_HANDLE_TYPE_NULL;
	m_iErr = tml_Core_Get_Listener(m_coreListenerSide->getCore(), 0, &listenerHandleToBeClosed);
	checkForSuccess();
	m_iErr = tml_Listener_Close(&listenerHandleToBeClosed);
	checkForSuccess();
	//hardcoded, before closing the count should have been 5
	checkListenerCount(4);
}

void TestingProcess::disableListener(int indexOfListener) {
	TML_LISTENER_HANDLE listenerToBeDisabled = TML_HANDLE_TYPE_NULL;
	m_iErr = tml_Core_Get_Listener(m_coreListenerSide->getCore(), indexOfListener, &listenerToBeDisabled);
	checkForSuccess();
	m_iErr = tml_Listener_Set_Enabled(listenerToBeDisabled, TML_FALSE);
	checkForSuccess();
}

void TestingProcess::checkIfListenerDisabled(int indexOfListener) {
	TML_LISTENER_HANDLE listenerToBeChecked = TML_HANDLE_TYPE_NULL;
	m_iErr = tml_Core_Get_Listener(m_coreListenerSide->getCore(), indexOfListener, &listenerToBeChecked);
	checkForSuccess();
	TML_BOOL isListenerEnabled;
	m_iErr = tml_Listener_Get_Enabled(listenerToBeChecked, &isListenerEnabled);
	checkForSuccess();
	if (TML_FALSE != isListenerEnabled) {
		m_iErr = 4;					//TODO or rather 0?
		errorOutput();
	}
}
