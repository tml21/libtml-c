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
#ifndef SDXRT_TEST_DICT_H
#define SDXRT_TEST_DICT_H


#include <sidex.h>

/** @defgroup Sidex_Dictionary_Tests
  * @brief Test Reference for Sidex_Dictionary
  */

//calls all other subtests
bool test_sidex_dict(int test, bool stop);
//checks sidex_Variant_NewDict, sidex_Variant_New_DictBySize(with sidex_Variant_Dict_Size) and sidex_Variant_Dict_Check
bool test_dict_01();
//check sidex_Variant_Dict_Size, also provoking errors
bool test_dict_02();
//check sidex_Variant_Dict_Set, also provoking errors
bool test_dict_03();
//checks sidex_Variant_Dict_First, also provoking errors
bool test_dict_04();
//checks sidex_Variant_Dict_Keys- also provoking errors
bool test_dict_05();
//checks sidex_Variant_Dict_HasKey- also provoking errors
bool test_dict_06();
//checks sidex_Variant_Dict_Delete- also provoking errors
bool test_dict_07();
//checks sidex_Variant_Dict_Clear- also provoking an error
bool test_dict_08();
//checks a normal sidex_Dict_Write and sidex_Dict_Read
bool test_dict_09();
//checks sidex_Dict_Write, only provoking errors
bool test_dict_10();
//checks sidex_Dict_Read, only provoking errors
bool test_dict_11();



#endif //SDXRT_TEST_DICT_H