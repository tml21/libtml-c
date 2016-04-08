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

#include "tmlrt_Utils.h"

bool linux = true;

#ifndef LINUX
  CRITICAL_SECTION notifyRepliesRecieved;
  static bool cmdRepliesReceived[5] = { { false } };
#else 
  pthread_mutex_t mutexRepliesRecieved = PTHREAD_MUTEX_INITIALIZER;
  static bool cmdRepliesReceived[5] = { false };
#endif

SIDEX_TCHAR* S_EMPTY                          = tmlrtT("");
SIDEX_TCHAR* S_UNNAMED                        = tmlrtT("<unnamed>");
SIDEX_TCHAR* S_START                          = tmlrtT("Start...");
SIDEX_TCHAR* S_FINISH_SUCCESS                 = tmlrtT("Finish. (Success)");
SIDEX_TCHAR* S_FINISH_FAILED                  = tmlrtT("Finish. (Failed)");
SIDEX_TCHAR* S_GROUP_TEST                     = tmlrtT("Test");
SIDEX_TCHAR* S_KEY_INDEX                      = tmlrtT("Index");

SIDEX_TCHAR* S_DOCUMENT                       = tmlrtT("Document");
SIDEX_TCHAR* S_wDOCUMENT                      = tmlrtT("Doc<ument");
SIDEX_TCHAR* S_DOCUMENT2                      = tmlrtT("AnotherDocument");
SIDEX_TCHAR* S_GROUP                          = tmlrtT("group");
SIDEX_TCHAR* S_KEY                            = tmlrtT("key");
SIDEX_TCHAR* S_GROUP2                         = tmlrtT("h\u00E4ll\u00F6W\u00F6rld");
SIDEX_TCHAR* S_KEY2                           = tmlrtT("????");
SIDEX_TCHAR* S_GROUP3                         = tmlrtT("g\u00F3\u00F0urdagur");
SIDEX_TCHAR* S_KEY3                           = tmlrtT("?????");
SIDEX_TCHAR* S_GROUP4                         = tmlrtT("????");
SIDEX_TCHAR* S_KEY4                           = tmlrtT("????");
SIDEX_TCHAR* S_wGROUP                         = tmlrtT("group&");
SIDEX_TCHAR* S_wKEY                           = tmlrtT("key]]>");
SIDEX_TCHAR* S_A_S_VAL                        = tmlrtT("systems");
SIDEX_TCHAR* S_B_S_VAL                        = tmlrtT("team");
SIDEX_TCHAR* S_COLUMN                         = tmlrtT("name");
SIDEX_TCHAR* S_COLUMN2                        = tmlrtT("g\u00F3\u00F0urdagur");
SIDEX_TCHAR* S_MINDATE                        = tmlrtT("0001-01-01 00:00:00:000");
SIDEX_TCHAR* S_MAXDATE                        = tmlrtT("9999-12-31 23:59:59:999");
SIDEX_TCHAR* S_wDATE                          = tmlrtT("0000-00-00 24:00:58:999");
SIDEX_TCHAR* S_uDATE                          = tmlrtT("0000-00-00 12:48:11:810");
SIDEX_TCHAR* S_HANDLEKEY                      = tmlrtT("handleKey");
SIDEX_TCHAR* S_FLOATKEY                       = tmlrtT("floatKey");
SIDEX_TCHAR* S_minStr                         = tmlrtT(" ");

SIDEX_TCHAR* S_IO_PROFILE                     = tmlrtT("http://wobe-team.com/profiles/plain_profile");
SIDEX_TCHAR* S_IO_PROFILE_TWO                 = tmlrtT("http://wobe-team.com/profiles/simple_profile");
SIDEX_TCHAR* S_IO_PROFILE_THREE               = tmlrtT("http://wobe-team.com/profiles/basic_profile");
SIDEX_TCHAR* S_LISTENER_NETWORK_INTERFACE_IP  = tmlrtT("0.0.0.0");
SIDEX_TCHAR* S_IO_PORT                        = tmlrtT("44100");
SIDEX_TCHAR* S_IO_PORT_TWO                    = tmlrtT("44102");
SIDEX_TCHAR* S_IO_PORT_THREE                  = tmlrtT("44103");
SIDEX_TCHAR* S_IO_PORT_FOUR                   = tmlrtT("44104");
SIDEX_TCHAR* S_IO_PORT_FIVE                   = tmlrtT("44105");
SIDEX_TCHAR* S_DESTINATION_HOST_IP            = tmlrtT("127.0.0.1");
SIDEX_TCHAR* S_LISTENER_ADDRESS               = tmlrtT("0.0.0.0:44100");

SIDEX_TCHAR* S_MEANING                        = tmlrtT("Meaning");
SIDEX_TCHAR* S_OF_LIFE                        = tmlrtT("ofLife");

SIDEX_TCHAR* S_DEFAULT_BOOTSTRAP              = tmlrtT("bootstrapTemplate.sdx");

VortexMutex g_mutex_handle;
bool        g_mutex_valid = false;

bool initGlobalMutex()
{
  if(!g_mutex_valid)
  {
    g_mutex_valid = (vortex_mutex_create(&g_mutex_handle) == axl_true);
    if(!g_mutex_valid) { wcout << "Failed: initGlobalMutex - vortex_mutex_create()" << endl; }
  }
  return(g_mutex_valid);
}

bool enterGlobalMutex()
{
  bool success = false;
  if(initGlobalMutex())
  {
    vortex_mutex_lock(&g_mutex_handle);
    success = true;
  }
  return(success);
}

void leaveGlobalMutex()
{
  if(g_mutex_valid) vortex_mutex_unlock(&g_mutex_handle);
}

void deleteGlobalMutex()
{
  if(g_mutex_valid)
  {
    vortex_mutex_destroy(&g_mutex_handle);
    g_mutex_valid = false;
  }
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Helper function for concatenating strings, with delete option
* @param SIDEX_TCHAR* first : First string
* @param SIDEX_TCHAR* second : Second string (optional, can be NULL)
* @param SIDEX_TCHAR* third : Third string (optional, can be NULL)
* @param int delStr : (optional) bit field : 0 = none, 1 = first, 2 = second, 4 = third, ... , 7 = all
* @returns SIDEX_TCHAR* : Returns concatenated string (has to be deleted after use)
*/
SIDEX_TCHAR* tmlrt_cat(SIDEX_TCHAR* first, SIDEX_TCHAR* second, SIDEX_TCHAR* third, int delStr)
{
  int i, l1 = 0, l2 = 0, l3 = 0;
  if(first)  { i = 0; while(first[i])  { i++; l1++; } }
  if(second) { i = 0; while(second[i]) { i++; l2++; } }
  if(third)  { i = 0; while(third[i])  { i++; l3++; } }
  SIDEX_TCHAR* result = new SIDEX_TCHAR[l1 + l2 + l3 + 1];
  if(result)
  {
    for(i = 0; i < l1; i++) { result[i          ] = first[i];  }
    for(i = 0; i < l2; i++) { result[i + l1     ] = second[i]; }
    for(i = 0; i < l3; i++) { result[i + l1 + l2] = third[i];  }
    result[l1 + l2 + l3] = 0;
  }
  if(delStr & 1) DELETE_STR(first);
  if(delStr & 2) DELETE_STR(second);
  if(delStr & 4) DELETE_STR(third);
  return(result);
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Helper function for copying a string
* @param const SIDEX_TCHAR* source : String to copy
* @returns SIDEX_TCHAR* : Returns copied string (has to be deleted after use)
*/
SIDEX_TCHAR* tmlrt_cpy(SIDEX_TCHAR* source)
{
  return(tmlrt_cat(source));
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Helper function for comparing two strings
* @param const SIDEX_TCHAR* first : First string
* @param const SIDEX_TCHAR* second : Second string
* @returns int : 0 = first equals second, >0 = first higher second, <0 = first lower second
*/
int tmlrt_cmp(SIDEX_TCHAR* first, SIDEX_TCHAR* second)
{
  int result = 0;
  SIDEX_TCHAR* s1 = (SIDEX_TCHAR*)first;
  SIDEX_TCHAR* s2 = (SIDEX_TCHAR*)second;
  if(s1 != s2)
  {
         if(!s1) result = -1;
    else if(!s2) result =  1;
    else
    {
      while(s1[0] && s2[0])
      {
             if(s1[0] < s2[0]) result = -1;
        else if(s1[0] > s2[0]) result =  1;
        if(result) break;
        s1++;
        s2++;
      }
           if(s1[0]) result =  1;
      else if(s2[0]) result = -1;
    }
  }
  return(result);
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Helper function for converting an integer into a string
* @param int value : integer value to be converted
* @returns SIDEX_TCHAR* : Returns a string (has to be deleted after use)
*/
SIDEX_TCHAR* tmlrt_itoa(int value)
{
  SIDEX_TCHAR* numbers = tmlrtT("0123456789-");
  int             sign = (value < 0) ? 1 : 0;
  if(sign > 0)   value = -value;
  SIDEX_TCHAR*  result = NULL;
  SIDEX_TCHAR*  buffer = new SIDEX_TCHAR[20];
  if(buffer)
  {
    int n = 0;
    do
    {
      buffer[n] = numbers[value % 10];
      value /= 10;
      n++;
    }
    while(value != 0);
    result = new SIDEX_TCHAR[n + 1 + sign];
    if(result)
    {
      for(int i = 0; i < n; i++) result[sign + i] = buffer[n - i - 1];
      if(sign > 0) result[0] = numbers[10];
      result[sign + n] = 0;
    }
    DELETE_STR(buffer);
  }
  return(result);
}


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
		sidex_Integer_Read(sHandle, S_GROUP, S_KEY, &value);
	if (TML_SUCCESS == iErr)
		iErr = tml_Cmd_Release_Sidex_Handle(cmdMsg);

	TmlSleep((int)value * 10);
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
		sidex_Integer_Read(sHandle, S_GROUP, S_KEY, &value);
	if (TML_SUCCESS == iErr)
		iErr = tml_Cmd_Release_Sidex_Handle(tmlhandle);

	iErr = tml_Cmd_Free(&tmlhandle);

	if (TML_SUCCESS != iErr) {
		wcout << "Test failed at async callback cmd reply received function with " << iErr << endl;
	}
	index = (int)(value / 10) - 1;
	enterGlobalMutex();
	cmdRepliesReceived[index] = true;
	leaveGlobalMutex();
	wcout << "received async cmd callback" << endl;
}

void initCmdRepliesReceived() {
	enterGlobalMutex();
	for (int i = 0; i < AMOUNT_OF_CMDS; i++) {
		cmdRepliesReceived[i] = false;
	}
	leaveGlobalMutex();
}

bool allCmdsFreed() {
	bool returnValue = true;
	enterGlobalMutex();
	for (int i = 0; i < AMOUNT_OF_CMDS; i++) {
		returnValue = returnValue && cmdRepliesReceived[i];
	}
	leaveGlobalMutex();
	return returnValue;
}

void setCmdRepliesReceivedToTrue(int indexOfDisabledListener) {
	enterGlobalMutex();
	cmdRepliesReceived[indexOfDisabledListener] = true;
	leaveGlobalMutex();
}
