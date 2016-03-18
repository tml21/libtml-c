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
#ifndef SDXRT_TEST_LIST_H
#define SDXRT_TEST_LIST_H


#include <sidex.h>

/** @defgroup Sidex_List_Tests
  * @brief Test Reference for Sidex_List
  */

/* calls all tests */
bool test_sidex_list(int test, bool stop);
//test_list_01 - checks sidex_Variant_New_List, writing it with entries into a sidex_document
bool test_list_01();
//test_list_02 -checks expected behaviour of sidex_Variant_List_Size
bool test_list_02();
//test_list_03 - checks sidex_Variant_List_Set for expected behaviour, also provoking errors
bool test_list_03();
//test_list_04 - checks sidex_Variant_List_Insert for expected behaviour, also provoking errors
bool test_list_04();
//test_list_05 - checks sidex_Variant_List_Get for expected behaviour, also provoking errors
bool test_list_05();
//test_list_06 - checks sidex_Variant_List_DeleteItem for expected behaviour, also provoking errors
bool test_list_06();
//test_list_07 - checks sidex_Variant_List_Clear for expected behaviour, also provoking an error
bool test_list_07();
//test_list_08 - checks sidex_Variant_List_Check for expected behaviour
bool test_list_08();
//test_list_09 - checks sidex_Variant_List_Append for expected behaviour, also provoking an error
bool test_list_09();
//test_list_10 - checks sidex_List_Write, mostly provoking errors
bool test_list_10();
//test_list_11 - checks sidex_List_Read, mostly provoking errors
bool test_list_11();

#endif  //SDXRT_TEST_LIST_H