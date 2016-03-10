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
//for function sleep
#ifndef LINUX
  #include <Windows.h>
#else 
  #include <unistd.h>
#endif


/** @ingroup Wrapping_Sidex_TChar
* @brief Wrapper function for concatenating two strings
* @param SIDEX_TCHAR* destination : First string for concatening and the result will be returned in place
* @param const SIDEX_TCHAR* source : Second string
* @returns int : Returns value of either wcscat or strcat, depending on definition of SIDEX_UNICODE
*/
SIDEX_TCHAR * tmlrt_cat( SIDEX_TCHAR * destination, const SIDEX_TCHAR * source) {
#ifdef SIDEX_UNICODE
    return wcscat_s(destination, source);
#else /// SIDEX_UNICODE
    return strcat(destination, source);
#endif // SIDEX_UNICODE
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
void FUNC_C_DECL cbgenericCmd(TML_COMMAND_HANDLE cmdMsg, TML_POINTER data){

    cout << "received cmd" << endl;
}
