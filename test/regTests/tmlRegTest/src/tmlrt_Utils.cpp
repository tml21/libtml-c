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
#include "tmlrt_Utils.h"
#include "TestingProcess.h"

bool linux = true;

#ifndef LINUX
  #include <Windows.h>
  CRITICAL_SECTION notifyRepliesRecieved;
  static bool cmdRepliesReceived[5] = { { false } };
#else 
  #include <unistd.h>
  #include <string.h>
  pthread_mutex_t mutexRepliesRecieved = PTHREAD_MUTEX_INITIALIZER;
  static bool cmdRepliesReceived[5] = { false };
#endif




void initializeMutex() {
  #ifndef LINUX
	InitializeCriticalSection(&notifyRepliesRecieved);	//only windows
  #endif
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Wrapper function for concatenating two strings
* @param SIDEX_TCHAR* destination : First string for concatening and the result will be returned in place
* @param const SIDEX_TCHAR* source : Second string
* @returns int : Returns value of either wcscat or strcat, depending on definition of SIDEX_UNICODE
*/
SIDEX_TCHAR * tmlrt_cat( SIDEX_TCHAR * destination, const SIDEX_TCHAR * source) {
#ifdef SIDEX_UNICODE
    return wcscat(destination, source);
#else /// SIDEX_UNICODE
    return strcat(destination, source);
#endif // SIDEX_UNICODE
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Wrapper function for comparing two strings with each other.
* @param const SIDEX_TCHAR* str1 : First string
* @param const SIDEX_TCHAR* str2 : Second string
* @returns int : Returns value of either wcscmp or strcmp, depending on definition of SIDEX_UNICODE
*/
int tmlrt_cmp(const SIDEX_TCHAR * str1, const SIDEX_TCHAR * str2) {
#ifdef SIDEX_UNICODE
	return wcscmp(str1, str2);
#else /// SIDEX_UNICODE
	return strcmp(str1, str2);
#endif // SIDEX_UNICODE
}

/** @ingroup 
* @brief Wrapper function for letting the program sleep for some time.
* @param int milliseconds : Amount of time in milliseconds the program should sleep
*/
void TmlSleep(int milliseconds) {
#ifdef LINUX
    usleep(milliseconds*1000);
#else
	Sleep(milliseconds); 
#endif
}


/** @ingroup 
 * @brief Callback method invoked in case of incoming CMD- Code.
 * @param   cmdMsg  Handle on cmd
 * @param   data    Pointer on data in cmd
 */
void FUNC_C_DECL cbgenericCmd(TML_COMMAND_HANDLE cmdMsg, TML_POINTER data){
	TML_INT32 iErr = 0;
	TML_INT64 value = 0;
	SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;

	iErr = tml_Cmd_Acquire_Sidex_Handle(cmdMsg, &sHandle);
	if (TML_SUCCESS == iErr)
		sidex_Integer_Read(sHandle, GROUP, KEY, &value);
	if (TML_SUCCESS == iErr)
		iErr = tml_Cmd_Release_Sidex_Handle(cmdMsg);

	TmlSleep(value * 10);
	wcout << "received cmd " << value << endl;
}

TML_BOOL FUNC_C_DECL onPeerCB(TML_BOOL bSubscribe, TML_CTSTR *sHost, TML_CTSTR *sPort, TML_POINTER pCBData) {
	wcout << "on Peer" << endl;
	return true;
}

void FUNC_C_DECL cbGenericCmdReplyReceived(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData) {
	TML_INT32 iErr = 0;
	TML_INT64 value = 0;
	SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
	int index = 0;

	iErr = tml_Cmd_Acquire_Sidex_Handle(tmlhandle, &sHandle);
	if (TML_SUCCESS == iErr)
		sidex_Integer_Read(sHandle, GROUP, KEY, &value);
	if (TML_SUCCESS == iErr)
		iErr = tml_Cmd_Release_Sidex_Handle(tmlhandle);

	iErr = tml_Cmd_Free(&tmlhandle);

	if (TML_SUCCESS != iErr) {
		wcout << "Test failed at async callback cmd reply received function with " << iErr << endl;
	}
	index = (value / 10) - 1;
	lockMutex();
	cmdRepliesReceived[index] = true;
	unlockMutex();
	wcout << "received async cmd callback" << endl;
}

void initCmdRepliesReceived() {
	lockMutex();
	for (int i = 0; i < AMOUNT_OF_CMDS; i++) {
		cmdRepliesReceived[i] = false;
	}
	unlockMutex();
}

bool allCmdsFreed() {
	bool returnValue = true;
	lockMutex();
	for (int i = 0; i < AMOUNT_OF_CMDS; i++) {
		returnValue = returnValue && cmdRepliesReceived[i];
	}
	unlockMutex();
	return returnValue;
}

void lockMutex() {
#ifndef LINUX	//Windows
	EnterCriticalSection(&notifyRepliesRecieved);
#else	//Linux
	pthread_mutex_lock(&mutexRepliesRecieved);
#endif
}

void unlockMutex() {
#ifndef LINUX	//Windows
	LeaveCriticalSection(&notifyRepliesRecieved);
#else	//Linux
	pthread_mutex_unlock(&mutexRepliesRecieved);
#endif
}


void setCmdRepliesReceivedToTrue(int indexOfDisabledListener) {
	lockMutex();
	cmdRepliesReceived[indexOfDisabledListener] = true;
	unlockMutex();
}