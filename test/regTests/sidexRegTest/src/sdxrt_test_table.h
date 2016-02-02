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
#ifndef SDXRT_TEST_TABLE_H
#define SDXRT_TEST_TABLE_H


#include <sidex.h>

/** @defgroup Sidex_Table_Tests
  * @brief Test Reference for Sidex_Table
  */

/* calls all tests */
bool test_sidex_table(int test, bool stop);
//test_list_01 - checks sidex_Variant_New_Table, writing it (with empty rows) into a sidex_document
bool test_list_01();
//test_list_02 - checks sidex_Variant_Table_AddColumn for expected behaviour, also provoking errors
bool test_list_02();
//test_list_03 - checks sidex_Variant_Table_AddRow for expected behaviour, also provoking an error
bool test_list_03();
//test_list_04 - checks function sidex_Variant_Table_Check
bool test_list_04();
//test_list_05 - checks sidex_Variant_Table_ColumnNames, also provoking errors
bool test_list_05();
//test_list_06 - checks sidex_Variant_Table_Columns, also provoking errors
bool test_list_06();
//test_list_07 - checks sidex_Variant_Table_DeleteColumn for expected behaviour, also provoking errors
bool test_list_07();
//test_list_08 - checks sidex_Variant_Table_DeleteRow for expected behaviour, also provoking errors
bool test_list_08();
//test_list_09 - checks sidex_Variant_Table_DeleteRows for expected behaviour, also provoking an error
bool test_list_09();
//test_list_10 - checks sidex_Variant_Table_GetColumnName for expected behaviour, also provoking errors
bool test_list_10();
//test_list_11 - checks sidex_Variant_Table_GetField for expected behaviour, also provoking errors
bool test_list_11();
//test_list_12 - checks sidex_Variant_Table_GetRow for expected behaviour, also provoking errors
bool test_list_12();
//test_list_13 - checks sidex_Variant_Table_HasColumn for expected behaviour, also provoking errors
bool test_list_13();
//test_list_14 - checks sidex_Variant_Table_Rows, also provoking errors
bool test_list_14();
//test_list_15 - checks sidex_Variant_Table_SetField for expected behaviour, also provoking errors
bool test_list_15();
//test_list_16 - checks sidex_Table_Write for expected behaviour, also provoking errors
bool test_list_16();
//test_list_17 - checks sidex_Table_Read for epxected behaviour, also provoking errors
bool test_list_17();

#endif  //SDXRT_TEST_TABLE_H