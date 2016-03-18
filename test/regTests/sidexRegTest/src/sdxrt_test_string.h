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
#ifndef SDXRT_TEST_STRING_H
#define SDXRT_TEST_STRING_H


#include <sidex.h>

/** @defgroup Sidex_String_Tests
  * @brief Test Reference for Sidex_String
  */

/* calls all tests */
bool test_sidex_string(int test, bool stop);
//test_string_01 - checks sidex_Variant_New_String with a small (" ") value of string
bool test_string_01();
//test_string_02 - checks a big value for a sidex_string
bool test_string_02();
//test_string_03 - checks sidex_Variant_New_String and sidex_Variant_String_Check, provoking errors
bool test_string_03();
//test_string_04 - checks sidex_String_Read, provoking errors
bool test_string_04();
//test_string_05 - checks sidex_String_Write- provoking errors
bool test_string_05();
//test_string_06 - checks sidex_String_Length- provoking errors
bool test_string_06();
//test_string_07 - checks sidex_Variant_As_String, - also provoking errors
bool test_string_07();
//test_string_08 - check sidex_Variant_As_String_Length, - also provoking an error
bool test_string_08();
//test_string_09 - check sidex_Variant_SetFormat and _GetFormat,- also provoking an error
bool test_string_09();

#endif      //SDXRT_TEST_STRING_H