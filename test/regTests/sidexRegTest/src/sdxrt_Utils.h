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



/** @defgroup Test_Utils
* @brief Helper functions which get used often
*/


//see .cpp-file for more details
bool errLog(bool test, char* arg1, char* arg2, int no);

//overloaded errLog function
bool errLog(bool test, char* functionName);