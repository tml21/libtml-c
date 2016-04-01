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
#ifndef TMLRT_UTILS_H
#define TMLRT_UTILS_H

#include <iostream>
#include <array>
using namespace std;
#include <sidex.h>
#include <tmlCore.h>

//constants for reuse
#ifdef SIDEX_UNICODE
#define DOCUMENT (wchar_t*) L"Document"
#define wDOCUMENT (wchar_t*) L"Doc<ument"
#define DOCUMENT2 (wchar_t*) L"AnotherDocument"
#define GROUP (wchar_t*) L"group"
#define KEY (wchar_t*) L"key"
#define GROUP2 (wchar_t*) L"hällöWörld"
#define KEY2 (wchar_t*) L"良い一日"
#define GROUP3 (wchar_t*) L"góðurdagur"
#define KEY3 (wchar_t*) L"গ্রুপ"
#define GROUP4 (wchar_t*) L"समूह"
#define KEY4 (wchar_t*) L"ກຸ່ມ"
#define wGROUP (wchar_t*) L"group&"
#define wKEY (wchar_t*) L"key]]>"
#define A_S_VAL (wchar_t*) L"systems"
#define B_S_VAL (wchar_t*) L"team"
#define COLUMN (wchar_t*) L"name"
#define COLUMN2 (wchar_t*) L"góðurdagur"
#define MINDATE (wchar_t*) L"0001-01-01 00:00:00:000"
#define MAXDATE (wchar_t*) L"9999-12-31 23:59:59:999"
#define wDATE (wchar_t*) L"0000-00-00 24:00:58:999"
#define uDATE (wchar_t*) L"0000-00-00 12:48:11:810"
#define HANDLEKEY (wchar_t*) L"handleKey"
#define FLOATKEY (wchar_t*) L"floatKey"
#define minStr (wchar_t*) L" "
#else
#define DOCUMENT (char*) "Document"
#define wDOCUMENT (char*) "Doc<ument"
#define DOCUMENT2 (char*) "AnotherDocument"
#define GROUP (char*) "group"
#define KEY (char*) "key"
#define GROUP2 (char*) "helloWorld"
#define KEY2 (char*) "anotherKey"
#define GROUP3 (char*) "anotherGroup"
#define KEY3 (char*) "KEY"
#define GROUP4 (char*) "GROUPFOUR"
#define KEY4 (char*) "keyfour"
#define wGROUP (char*) "group&"
#define wKEY (char*) "key]]>"
#define A_S_VAL (char*) "systems"
#define B_S_VAL (char*) "team"
#define COLUMN (char*) "name"
#define COLUMN2 (char*) "AnotherName"
#define MINDATE (char*) "0001-01-01 00:00:00:000"
#define MAXDATE (char*) "9999-12-31 23:59:59:999"
#define wDATE (char*) "0000-00-00 24:00:58:999"
#define uDATE (char*) "0000-00-00 12:48:11:810"
#define HANDLEKEY (char*) "handleKey"
#define FLOATKEY (char*) "floatKey"
#define minStr (char*) " "
#endif


#ifdef TML_UNICODE
  #define IO_PROFILE                    (wchar_t*) L"http://wobe-team.com/profiles/plain_profile"
  #define IO_PROFILE_TWO				(wchar_t*) L"http://wobe-team.com/profiles/simple_profile"
  #define IO_PROFILE_THREE				(wchar_t*) L"http://wobe-team.com/profiles/basic_profile"
  #define LISTENER_NETWORK_INTERFACE_IP (wchar_t*) L"0.0.0.0"
  #define IO_PORT                       (wchar_t*) L"44100"
  #define IO_PORT_TWO                   (wchar_t*) L"44102"
  #define IO_PORT_THREE                 (wchar_t*) L"44103"
  #define IO_PORT_FOUR	                (wchar_t*) L"44104"
  #define IO_PORT_FIVE					(wchar_t*) L"44105"
  #define DESTINATION_HOST_IP           (wchar_t*) L"127.0.0.1"
  #define LISTENER_ADDRESS				(wchar_t*) L"0.0.0.0:44100"

  #define MEANING                       (wchar_t*) L"Meaning"
  #define OF_LIFE                       (wchar_t*) L"ofLife"

  #define DEFAULT_BOOTSTRAP				(wchar_t*) L"bootstrapTemplate.sdx"
	

#else // TML_UNICODE
  #define IO_PROFILE                    (char*) "http://wobe-team.com/profiles/plain_profile"
  #define IO_PROFILE_TWO                (char*) "http://wobe-team.com/profiles/simple_profile"
  #define IO_PROFILE_THREE              (char*) "http://wobe-team.com/profiles/basic_profile"
  #define LISTENER_NETWORK_INTERFACE_IP (char*) "0.0.0.0"
  #define IO_PORT                       (char*) "44100"
  #define IO_PORT_TWO                   (char*) "44102"
  #define IO_PORT_THREE                 (char*) "44103"
  #define IO_PORT_FOUR	                (char*) "44104"
  #define IO_PORT_FIVE					(char*) "44105"
  #define DESTINATION_HOST_IP           (char*) "127.0.0.1"
  #define LISTENER_ADDRESS				(char*) "0.0.0.0:44100"

  #define MEANING                       (char*) "Meaning"
  #define OF_LIFE                       (char*) "ofLife"

  #define DEFAULT_BOOTSTRAP				(char*) "bootstrapTemplate.sdx"


#endif // TML_UNICODE

const int DEFAULT_TIMEOUT = 5000;
const int AMOUNT_OF_CMDS = 5;
const int AMOUNT_OF_LISTENERS = 5;
const int MAX_AMOUNT_OF_PROFILES = 5;
const array<int, AMOUNT_OF_CMDS> COMMAND_CODES_LIST = {10,50,42,30,20};

//makro for creating strings in the right format
#ifdef SIDEX_UNICODE
#define tmlrtT(a) (wchar_t*) L ## a
#else // SIDEX_UNICODE
#define tmlrtT(a) (char*) a
#endif // SIDEX_UNICODE

const array<SIDEX_TCHAR*, 5> LISTENERS_PORTS = { tmlrtT("4711"), tmlrtT("4712"), tmlrtT("1815"), tmlrtT("1420"), tmlrtT("1234") };
const array<SIDEX_TCHAR*, 5> LISTENERS_ADDRESS = { tmlrtT("0.0.0.0:4711"), tmlrtT("0.0.0.0:4712"), tmlrtT("0.0.0.0:1815"), tmlrtT("0.0.0.0:1420"), tmlrtT("0.0.0.0:1234") };

//transfer function for comparing
int tmlrt_cmp(const SIDEX_TCHAR * str1, const SIDEX_TCHAR * str2);

//Wrapper function for concatenating two strings
SIDEX_TCHAR * tmlrt_cat( SIDEX_TCHAR * destination, const SIDEX_TCHAR * source);

void TmlSleep(int milliseconds);

//cb for cmd 
void FUNC_C_DECL cbgenericCmd(TML_COMMAND_HANDLE cmdMsg, TML_POINTER data);

//cb for onPeerRegister
TML_BOOL FUNC_C_DECL onPeerCB(TML_BOOL bSubscribe, TML_CTSTR *sHost, TML_CTSTR *sPort, TML_POINTER pCBData);

//cb if cmd reply was received from listener
void FUNC_C_DECL cbGenericCmdReplyReceived(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData);

//set the bool array to false
void initCmdRepliesReceived();

//check whether all cmds were freed
bool allCmdsFreed();

//initalize windowsmutex
void initializeMutex();


void lockMutex();

void unlockMutex();

void setCmdRepliesReceivedToTrue(int indexOfDisabledListener);

#endif //TMLRT_UTILS_H