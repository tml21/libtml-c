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
#ifndef SDXRT_TEST_BOOL_H
#define SDXRT_TEST_BOOL_H

/** @defgroup Sidex_Bool_Tests
  * @brief Test Reference for Sidex_Bool
  */

#include <sidex.h>

bool test_sidex_bool(int test, bool stop);
//test_bool_01 - check true value for Bool through writing in as variant and reading out of a sidex document
bool test_bool_01();
//test_bool_02 - check false value for Bool through writing in as variant and reading out of a sidex document
bool test_bool_02();
//test_bool_03 - check true value for Bool through writing in and reading out of a sidex document
bool test_bool_03();
//test_bool_04 - check false value for Bool through writing in and reading out of a sidex document
bool test_bool_04();
//test_bool_05 - check lowest value for Bool through writing in as variant (& as SIDEX_bool) and reading out of a sidex document
bool test_bool_05();
//test_bool_06 - check if the function sidex_Bool_Write throws appropriate errors
bool test_bool_06();
//test_bool_07 - check if the function sidex_Bool_Read throws appropriate Errors
bool test_bool_07();
//test_bool_08 - check if the function sidex_Boolean_Read throws appropriate Errors
bool test_bool_08();

#endif  //SDXRT_TEST_BOOL_H