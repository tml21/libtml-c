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

#include <iostream>
#include <list>
#include <array>
using namespace std;
#include <sidex.h>
#include <tmlCore.h>
#include "TestingForReturns.h"
#include "TmlCore.h"
#include "TmlList.h"



class TestingProcess : public TestingForReturns{
public:
	TestingProcess(SIDEX_TCHAR* name);
	~TestingProcess();
	static const int amountOfCmds = 5;
	TmlCore* m_coreSenderSide;
	TmlCore* m_coreListenerSide;

	void defaultListenerInit();
	void freeTmlCores();
	void sendArbitraryCmds();
	void initSenderSide();
	void freeCmds();
	void checkListenerCount(int count);
	void checkListenerAdresses();
	void checkForDecreasedListenersAfterOneIsClosed();
	void disableListener(int indexOfListener);
	void checkIfListenerDisabled(int indexOfListener);

	//static bool[amountOfCmds] cmdsReceived = {true, true, true, true, true};

protected:
	TML_INT32 sendArbitraryCmd(TML_INT32 cmdIndex, TML_INT32 profilIndex);
	TML_INT32 produceCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value);
	TML_INT32 freeCmd(TML_COMMAND_HANDLE* cmd);
	TML_INT32 createSetOfCmds();

private:
	TML_INT32 createCmd(TML_COMMAND_HANDLE* cmd, TML_INT32 value);
	bool testOK;
	TML_COMMAND_HANDLE cmdMessage;
	array<TML_COMMAND_HANDLE,amountOfCmds> Cmds;

	//array of cmd-codes
	array<int, amountOfCmds> cmdCodes;
};



#endif	//TESTING_PROCESS_H