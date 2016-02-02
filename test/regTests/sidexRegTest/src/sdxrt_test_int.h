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
#ifndef SDXRT_TEST_INT_H
#define SDXRT_TEST_INT_H


#include <sidex.h>

/** @defgroup Sidex_Integer_Tests
  * @brief Test Reference for Sidex_Integer
  */

/* calls all tests */
bool test_sidex_int(int test, bool stop);
//test_int_01 - check min value for 64-bit through writing in as variant and reading out of a sidex document
bool test_int_01();
//test_int_02- check max value for 64-bit through writing in as variant and reading out of a sidex document
bool test_int_02();
//test_int_03 - check min value for 64-bit through writing in and reading out of a sidex document
bool test_int_03();
//test_int_04 - check max value for 64-bit through writing in and reading out of a sidex document
bool test_int_04();
//test_int_05 - check if function sidex_Variant_As_Integer works as expected
bool test_int_05();
//test_int_06 - checking in sidex_Integer_Read & sidex_Integer_Write with different special characters for group- and keynames
bool test_int_06();
//test_int_07 - check if the function sidex_Integer_Write throws appropriate errors
bool test_int_07();
//test_int_08 - check if the function sidex_Integer_Read throws appropriate Errors
bool test_int_08();
//calls sidex_Integer_Write and sidex_Integer_Read very often with random values
bool test_int_09();

#endif //SDXRT_TEST_INT_H