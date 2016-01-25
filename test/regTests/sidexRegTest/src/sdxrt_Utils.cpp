/* 
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2015 wobe-systems GmbH
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
 *  For commercial support on build UJO enabled solutions contact us:
 *  
 * Contributors:
 *    wobe-systems GmbH
 */

#include <iostream>
using namespace std;


/** @ingroup Test_Utils
* @brief Outputs a failure message if param test is false
* 
* @param bool test: Decides about output of failure message
* @param char* arg1 : Name of the test performed. Used in the output message.
* @param char* arg2 : Additional information for locating test failure.
* @param int no : Contains an integer value. Additional information for locating test failure.
* @returns bool : Param test.
*/
bool errLog(bool test, const  char* arg1, const char* arg2, int no){
    if (!test){
        cout << arg1 << " " << arg2 << " (" << no << ") FAILED" << endl;
    }
    return test;
}

/** @ingroup Test_Utils
* @brief Outputs a message at every call, the message varies, depending on param test
* 
* @param bool test : If true, output message containing 'OK' will be produced and if false,
* output message will contain 'FAILED'.
* @param char* functionName : Name of the test performed. Used in the output message.
* @returns bool : Param test.
*/
bool errLog(bool test, const char* functionName) {
    char* status = (char*)" OK";

    if(!test) {
        status = (char*)" FAILED";
    }

    cout << functionName << status << endl;

    return test;
}
