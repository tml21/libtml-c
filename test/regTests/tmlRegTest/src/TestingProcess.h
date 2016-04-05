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

#ifndef TESTING_PROCESS_H
#define TESTING_PROCESS_H

#include "TmlTester.h"
#include "TmlCore.h"

class TestingProcess : public TmlTester
{
public:
	TestingProcess(SIDEX_TCHAR* testProcessName = NULL);
	~TestingProcess();

	TmlCore* m_coreSenderSide;
	TmlCore* m_coreListenerSide;

	void defaultListenerInit();
	void freeTmlCores();
	void sendArbitraryCmds(bool synchronous);
	void initSenderSide();
	void freeCmds();
	void checkListenerCount(int count);
	void checkListenerAdresses();
	void checkForDecreasedListenersAfterOneIsClosed();
	void disEnableListener(int indexOfListener, TML_BOOL enable);
	void checkIfListenerDisabled(int indexOfListener);
	void subscribeListenersForLoadBalancingOrEvents(bool loadbalancing);
	void sendArbitraryLoadBalancedCmds(bool synchronous);
	void sendEventMessages();
	void checkAsyncSyncMessagesForErrors(int indexOfDisabledListener);
	void checkSendMessageForErrors(TML_INT32 errorCode, bool synchronous, int timeout);
	void checkSendMessageForUNICODEError(bool synchronous);
	void waitForAsyncReplies();

protected:
	TML_INT32 sendArbitraryCmd(TML_INT32 cmdIndex, TML_INT32 profilIndex, bool synchronous, int timeout);
	TML_INT32 produceCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value, bool synchronous);
	TML_INT32 freeCmd(TML_COMMAND_HANDLE* cmd);
	TML_INT32 createSetOfCmds(bool synchronous);
	TML_INT32 sendArbitraryLoadBalancedCmd(TML_INT32 cmdIndex, TML_INT32 profileIndex, bool synchronous);

private:
	TML_INT32 createCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value, bool synchronous);
	array<TML_COMMAND_HANDLE,AMOUNT_OF_CMDS> m_Cmds;

	//array of cmd-codes
	array<int, AMOUNT_OF_CMDS> m_cmdCodes;
};

#endif	//TESTING_PROCESS_H
