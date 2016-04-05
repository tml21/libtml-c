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

#include "TestingProcess.h"
#include "TmlCore.h"
#include <time.h>

TestingProcess::TestingProcess(SIDEX_TCHAR* testProcessName)
              : TmlTester(testProcessName), m_coreListenerSide(), m_coreSenderSide()
{
	m_cmdCodes = COMMAND_CODES_LIST;
	m_coreListenerSide = new TmlCore(testProcessName);
	m_coreSenderSide = new TmlCore(testProcessName);
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
	bool listenerSide = true;
	m_iErr = m_coreListenerSide->defaultInit(listenerSide);
	checkForSuccess();
	m_iErr = m_coreListenerSide->registerDefaultCmds(m_cmdCodes);
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
	bool listenerSide = false;
	m_iErr = m_coreSenderSide->defaultInit(listenerSide);
	checkForSuccess();
}

TML_INT32 TestingProcess::produceCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value, bool synchronous) {
	createCmd(cmd, value, synchronous);

    // Set cmdMessage parameter
    SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
    if (TML_SUCCESS == m_iErr)
        m_iErr = tml_Cmd_Acquire_Sidex_Handle(*cmd, &sHandle);
    if (TML_SUCCESS == m_iErr)
        sidex_Integer_Write(sHandle, S_GROUP, S_KEY, value);
    if (TML_SUCCESS == m_iErr)
        m_iErr = tml_Cmd_Release_Sidex_Handle(*cmd);

     return m_iErr;
}

TML_INT32 TestingProcess::createCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value, bool synchronous) {
	TML_COMMAND_HANDLE cmdMsg = TML_HANDLE_TYPE_NULL;
    m_iErr = tml_Cmd_Create (&cmdMsg);
    checkForSuccess();

    m_iErr = tml_Cmd_Header_SetCommand (cmdMsg, (TML_COMMAND_ID_TYPE) value);
	checkForSuccess();

	//register a cb function for after cmd reply was received, only if async
	if (!synchronous) {
		m_iErr = tml_Cmd_Register_CommandReady(cmdMsg, cbGenericCmdReplyReceived, NULL);
		checkForSuccess();
	}
	

	if (TML_SUCCESS == m_iErr)
        *cmd = cmdMsg;
	return m_iErr;
}

TML_INT32 TestingProcess::sendArbitraryCmd(TML_INT32 cmdIndex, TML_INT32 profileIndex, bool synchronous, int timeout) {
	TML_COMMAND_HANDLE cmdMsg = m_Cmds[cmdIndex];
	const int sizeOfProfilesArray = MAX_AMOUNT_OF_PROFILES;
	array<SIDEX_TCHAR*, sizeOfProfilesArray> listOfProfiles = m_coreListenerSide->getProfileNames();
	SIDEX_TCHAR* profileURL = listOfProfiles[profileIndex];
	SIDEX_TCHAR* listenerPort = LISTENERS_PORTS[cmdIndex];
	if (synchronous) {
		m_iErr = tml_Send_SyncMessage(m_coreSenderSide->getCore(), cmdMsg, profileURL, S_DESTINATION_HOST_IP, listenerPort, timeout);
	}
	else {
		m_iErr = tml_Send_AsyncMessage(m_coreSenderSide->getCore(), cmdMsg, profileURL, S_DESTINATION_HOST_IP, listenerPort, timeout);
	}
	return m_iErr;
}


void TestingProcess::sendArbitraryCmds(bool synchronous) {
	int numberOfCmdsToSend = AMOUNT_OF_CMDS;
	TML_INT32 indexOfWantedProfile = 0;
	createSetOfCmds(synchronous);
	for(int i = 0; i < numberOfCmdsToSend; i++) {
		m_iErr = sendArbitraryCmd(i, indexOfWantedProfile, synchronous, DEFAULT_TIMEOUT);
		checkForSuccess();
	}
}


void TestingProcess::freeCmds() {
	for( int i = 0; i < AMOUNT_OF_CMDS; i++) {
		m_iErr = freeCmd(&m_Cmds[i]);
		checkForSuccess();
	}
}

TML_INT32 TestingProcess::freeCmd(TML_COMMAND_HANDLE* cmd) {
	m_iErr = tml_Cmd_Free(cmd);
	return m_iErr;
}

TML_INT32 TestingProcess::createSetOfCmds(bool synchronous) {
	for(int i = 0; i < AMOUNT_OF_CMDS; i++){
		m_Cmds[i] = TML_HANDLE_TYPE_NULL;
		m_iErr = produceCmd(&m_Cmds[i], m_cmdCodes[i], synchronous);
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
	bool noFailures = true;
	TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
	for (int i = 0; i < AMOUNT_OF_LISTENERS; i++) {
		m_iErr = tml_Core_Get_Listener(m_coreListenerSide->getCore(), i, &listenerHandle);
		checkForSuccess();
		m_iErr = tml_Listener_Get_Address(listenerHandle, &listenerAddress);
		checkForSuccess();
		apiReturnCode = tmlrt_cmp(listenerAddress, LISTENERS_ADDRESS[i]);
		if (0 != apiReturnCode) {
			noFailures = false;
		}
	}
	if (!noFailures) {
		m_iErr = 4;
		errorOutput();
	}
}

void TestingProcess::checkForDecreasedListenersAfterOneIsClosed() {
	TML_LISTENER_HANDLE listenerHandleToBeClosed = TML_HANDLE_TYPE_NULL;
	int amountOfListenersAtCore = AMOUNT_OF_LISTENERS - 1;

	m_iErr = tml_Core_Get_Listener(m_coreListenerSide->getCore(), 0, &listenerHandleToBeClosed);
	checkForSuccess();
	m_iErr = tml_Listener_Close(&listenerHandleToBeClosed);
	checkForSuccess();
	
	checkListenerCount(amountOfListenersAtCore);
}

void TestingProcess::disEnableListener(int indexOfListener, TML_BOOL enable) {
	TML_LISTENER_HANDLE listenerToBeDisabled = TML_HANDLE_TYPE_NULL;
	m_iErr = tml_Core_Get_Listener(m_coreListenerSide->getCore(), indexOfListener, &listenerToBeDisabled);
	checkForSuccess();
	m_iErr = tml_Listener_Set_Enabled(listenerToBeDisabled, enable);
	checkForSuccess();
	checkListenerAdresses();
}

void TestingProcess::checkIfListenerDisabled(int indexOfListener) {
	TML_LISTENER_HANDLE listenerToBeChecked = TML_HANDLE_TYPE_NULL;
	m_iErr = tml_Core_Get_Listener(m_coreListenerSide->getCore(), indexOfListener, &listenerToBeChecked);
	checkForSuccess();
	TML_BOOL isListenerEnabled;
	m_iErr = tml_Listener_Get_Enabled(listenerToBeChecked, &isListenerEnabled);
	checkForSuccess();
	if (TML_FALSE != isListenerEnabled) {
		m_iErr = 4;
		errorOutput();
	}
}

void TestingProcess::sendArbitraryLoadBalancedCmds(bool synchronous) {
	int numberOfCmdsToSend = 5;
	TML_INT32 indexOfWantedProfile = 0;
	createSetOfCmds(synchronous);
	for (int i = 0; i < numberOfCmdsToSend; i++) {
		m_iErr = sendArbitraryLoadBalancedCmd(i, indexOfWantedProfile, synchronous);	
		checkForSuccess();
	}

}

void TestingProcess::subscribeListenersForLoadBalancingOrEvents(bool loadbalancing) {
	TML_HANDLE_TYPE senderCore = m_coreSenderSide->getCore();
	TML_HANDLE_TYPE listenerCore = m_coreListenerSide->getCore();
	SIDEX_TCHAR* destinationIP = S_DESTINATION_HOST_IP;
	const int sizeOfProfilesArray = MAX_AMOUNT_OF_PROFILES;
	array<SIDEX_TCHAR*, sizeOfProfilesArray> listOfProfiles = m_coreListenerSide->getProfileNames();
	array<SIDEX_TCHAR*, 5> listenersPorts = LISTENERS_PORTS;
	int profilesIndex = 0;
	//the first two Listeners are subscribed
	int listenersIndex = 0;
	for (listenersIndex; listenersIndex < 2; listenersIndex++) {
		if (loadbalancing) {
			m_iErr = tml_Bal_Subscribe_MessageDestination(senderCore, listOfProfiles[profilesIndex],
				destinationIP, listenersPorts[listenersIndex]);
		}
		else {
			m_iErr = tml_Evt_Subscribe_MessageDestination(senderCore, listOfProfiles[profilesIndex],
				destinationIP, listenersPorts[listenersIndex]);
		}
		
		checkForSuccess();
	}
	//the three remaining Listeners subscribe themselves
	for (listenersIndex; listenersIndex < 5; listenersIndex++) {
		if (loadbalancing) {
			m_iErr = tml_Bal_Send_SubscriptionRequest(listenerCore, listOfProfiles[profilesIndex],
				destinationIP, listenersPorts[listenersIndex], destinationIP, S_IO_PORT, DEFAULT_TIMEOUT);
		}
		else {
			m_iErr = tml_Evt_Send_SubscriptionRequest(listenerCore, listOfProfiles[profilesIndex],
				destinationIP, listenersPorts[listenersIndex], destinationIP, S_IO_PORT, DEFAULT_TIMEOUT);
		}
		
		checkForSuccess();
	}
}

TML_INT32 TestingProcess::sendArbitraryLoadBalancedCmd(TML_INT32 cmdIndex, TML_INT32 profileIndex, bool synchronous) {
	TML_COMMAND_HANDLE cmdMsg = m_Cmds[cmdIndex];
	const int sizeOfProfilesArray = MAX_AMOUNT_OF_PROFILES;
	array<SIDEX_TCHAR*, sizeOfProfilesArray> listOfProfiles = m_coreListenerSide->getProfileNames();
	SIDEX_TCHAR* profileURL = listOfProfiles[profileIndex];
	if (synchronous) {
		m_iErr = tml_Bal_Send_SyncMessage(m_coreSenderSide->getCore(), cmdMsg, profileURL, DEFAULT_TIMEOUT);
	}
	else {
		m_iErr = tml_Bal_Send_AsyncMessage(m_coreSenderSide->getCore(), cmdMsg, profileURL, DEFAULT_TIMEOUT);
	}
	return m_iErr;
}

void TestingProcess::checkAsyncSyncMessagesForErrors(int indexOfDisabledListener) {
	int numberOfCmdsToSend = 5;
	TML_INT32 indexOfWantedProfile = 0;
	initCmdRepliesReceived();
	int cmdIndex = COMMAND_CODES_LIST[indexOfDisabledListener];
	cmdIndex = cmdIndex / 10 - 1;
	setCmdRepliesReceivedToTrue(cmdIndex);
	bool synchronous = false;
	for (int j = 0; j < 2; j++) {
		createSetOfCmds(synchronous);
		for (int i = 0; i < numberOfCmdsToSend; i++) {
			m_iErr = sendArbitraryCmd(i, indexOfWantedProfile, synchronous, DEFAULT_TIMEOUT);
			if (indexOfDisabledListener == i) {
				checkForExpectedReturnCode(32);
			}
			else {
				checkForSuccess();
			}	
		}
		synchronous = true;
	}
	
}

void TestingProcess::checkSendMessageForErrors(TML_INT32 errorCode, bool synchronous, int timeout) {
	TML_INT32 indexOfWantedProfile = 0;
	TML_INT32 indexOfCmdToSend = 2;
	createSetOfCmds(synchronous);
	m_iErr = sendArbitraryCmd(indexOfCmdToSend, indexOfWantedProfile, synchronous, timeout);
	checkForExpectedReturnCode(errorCode);
}

void TestingProcess::checkSendMessageForUNICODEError(bool synchronous) {
#if defined TML_UNICODE || defined SIDEX_UNICODE
	int cmdIndex = 2;
	int profileIndex = 0;
	TML_COMMAND_HANDLE cmdMsg = m_Cmds[cmdIndex];
	const int sizeOfProfilesArray = MAX_AMOUNT_OF_PROFILES;
	array<SIDEX_TCHAR*, sizeOfProfilesArray> listOfProfiles = m_coreListenerSide->getProfileNames();
	SIDEX_TCHAR* profileURL = listOfProfiles[profileIndex];
	if (synchronous) {
		m_iErr = tml_Send_SyncMessage(m_coreSenderSide->getCore(), cmdMsg, profileURL, S_DESTINATION_HOST_IP, NULL, DEFAULT_TIMEOUT);
	}
	else {
		m_iErr = tml_Send_AsyncMessage(m_coreSenderSide->getCore(), cmdMsg, profileURL, S_DESTINATION_HOST_IP, NULL, DEFAULT_TIMEOUT);
	}
	checkForExpectedReturnCode(TML_ERR_UNICODE);
#endif
}

void TestingProcess::sendEventMessages() {
	TML_CORE_HANDLE senderCore = m_coreSenderSide->getCore();
	int numberOfCmdsToSend = AMOUNT_OF_CMDS;
	TML_INT32 indexOfWantedProfile = 0;
	const int sizeOfProfilesArray = MAX_AMOUNT_OF_PROFILES;
	array<SIDEX_TCHAR*, sizeOfProfilesArray> listOfProfiles = m_coreListenerSide->getProfileNames();
	SIDEX_TCHAR* profileURL = listOfProfiles[indexOfWantedProfile];
	bool synchronous = true; //with event messages it does not matter
	createSetOfCmds(synchronous);
	for (int i = 0; i < numberOfCmdsToSend; i++) {
		m_iErr = tml_Evt_Send_Message(senderCore, m_Cmds[i], profileURL);
		checkForSuccess();
	}
}

void TestingProcess::waitForAsyncReplies() {
	time_t timer = time(NULL);
	time_t diffTimer;

	while (!allCmdsFreed()) {
		diffTimer = time(NULL);	
		if (10 < difftime(diffTimer, timer)) {	
			//this function waits already for more than 10 seconds
			return;
		}
		TmlSleep(50);
	}
}