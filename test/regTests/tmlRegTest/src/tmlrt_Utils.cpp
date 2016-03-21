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

SIDEX_TCHAR* S_EMPTY                        = tmlrtT("");
SIDEX_TCHAR* S_UNNAMED                      = tmlrtT("<unnamed>");
SIDEX_TCHAR* S_START                        = tmlrtT("Start...");
SIDEX_TCHAR* S_FINISH_SUCCESS               = tmlrtT("Finish. (Success)");
SIDEX_TCHAR* S_FINISH_FAILED                = tmlrtT("Finish. (Failed)");

SIDEX_TCHAR* DOCUMENT                       = tmlrtT("Document");
SIDEX_TCHAR* wDOCUMENT                      = tmlrtT("Doc<ument");
SIDEX_TCHAR* DOCUMENT2                      = tmlrtT("AnotherDocument");
SIDEX_TCHAR* GROUP                          = tmlrtT("group");
SIDEX_TCHAR* KEY                            = tmlrtT("key");
SIDEX_TCHAR* GROUP2                         = tmlrtT("hällöWörld");
SIDEX_TCHAR* KEY2                           = tmlrtT("良い一日");
SIDEX_TCHAR* GROUP3                         = tmlrtT("góðurdagur");
SIDEX_TCHAR* KEY3                           = tmlrtT("গ্রুপ");
SIDEX_TCHAR* GROUP4                         = tmlrtT("समूह");
SIDEX_TCHAR* KEY4                           = tmlrtT("ກຸ່ມ");
SIDEX_TCHAR* wGROUP                         = tmlrtT("group&");
SIDEX_TCHAR* wKEY                           = tmlrtT("key]]>");
SIDEX_TCHAR* A_S_VAL                        = tmlrtT("systems");
SIDEX_TCHAR* B_S_VAL                        = tmlrtT("team");
SIDEX_TCHAR* COLUMN                         = tmlrtT("name");
SIDEX_TCHAR* COLUMN2                        = tmlrtT("góðurdagur");
SIDEX_TCHAR* MINDATE                        = tmlrtT("0001-01-01 00:00:00:000");
SIDEX_TCHAR* MAXDATE                        = tmlrtT("9999-12-31 23:59:59:999");
SIDEX_TCHAR* wDATE                          = tmlrtT("0000-00-00 24:00:58:999");
SIDEX_TCHAR* uDATE                          = tmlrtT("0000-00-00 12:48:11:810");
SIDEX_TCHAR* HANDLEKEY                      = tmlrtT("handleKey");
SIDEX_TCHAR* FLOATKEY                       = tmlrtT("floatKey");
SIDEX_TCHAR* minStr                         = tmlrtT(" ");

SIDEX_TCHAR* IO_PROFILE                     = tmlrtT("http://wobe-team.com/profiles/plain_profile");
SIDEX_TCHAR* IO_PROFILE_TWO				          = tmlrtT("http://wobe-team.com/profiles/simple_profile");
SIDEX_TCHAR* IO_PROFILE_THREE				        = tmlrtT("http://wobe-team.com/profiles/basic_profile");
SIDEX_TCHAR* LISTENER_NETWORK_INTERFACE_IP  = tmlrtT("0.0.0.0");
SIDEX_TCHAR* IO_PORT                        = tmlrtT("44100");
SIDEX_TCHAR* IO_PORT_TWO                    = tmlrtT("44102");
SIDEX_TCHAR* IO_PORT_THREE                  = tmlrtT("44103");
SIDEX_TCHAR* IO_PORT_FOUR	                  = tmlrtT("44104");
SIDEX_TCHAR* IO_PORT_FIVE					          = tmlrtT("44105");
SIDEX_TCHAR* DESTINATION_HOST_IP            = tmlrtT("127.0.0.1");

SIDEX_TCHAR* MEANING                        = tmlrtT("Meaning");
SIDEX_TCHAR* OF_LIFE                        = tmlrtT("ofLife");

SIDEX_TCHAR* DEFAULT_BOOTSTRAP				      = tmlrtT("bootstrapTemplate.sdx");

/** @ingroup Wrapping_Sidex_TChar
* @brief Helper function for concatenating two strings
* @param const SIDEX_TCHAR* first : First string
* @param const SIDEX_TCHAR* second : Second string
* @returns SIDEX_TCHAR* : Returns concatenated string (has to be deleted after use)
*/
SIDEX_TCHAR* tmlrt_cat(const SIDEX_TCHAR* first, const SIDEX_TCHAR* second)
{
  int i, l1 = 0, l2 = 0;
  if(first)  { i = 0; while(first[i])  { i++; l1++; } }
  if(second) { i = 0; while(second[i]) { i++; l2++; } }
  SIDEX_TCHAR* result = new SIDEX_TCHAR[l1 + l2 + 1];
  if(result)
  {
    for(i = 0; i < l1; i++) { result[i     ] = first[i];  }
    for(i = 0; i < l2; i++) { result[i + l1] = second[i]; }
    result[l1 + l2] = 0;
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


//cb function
/** @brief Callback method invoked in case of incoming CMD- Code 20
 * @param   cmdMsg  Handle on cmd
 * @param   data    Pointer on data in cmd
 */
void FUNC_C_DECL cbgenericCmd(TML_COMMAND_HANDLE cmdMsg, TML_POINTER data)
{
  wcout << "received cmd" << endl;
}
