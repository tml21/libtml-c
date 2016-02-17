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
#ifndef SDXRT_TEST_BINARY_H
#define SDXRT_TEST_BINARY_H


#include <sidex.h>

/** @defgroup Sidex_Binary_Tests
  * @brief Test Reference for class Sidex_Binary
  */

/* calls all tests */
bool test_sidex_binary(int test, bool stop);

//test_binary_01 - checks sidex_Variant_New_Binary with a big char
bool test_binary_01();

//test_binary_02 - checks sidex_Binary_Write and Binary_Read with minChar
bool test_binary_02();

//test_binary_03 - checks sidex_Variant_Binary_Check
bool test_binary_03();

//test_binary_04 - checks sidex_Variant_As_Binary for expected behaviour
bool test_binary_04();

//test_binary_05 - checks sidex_Variant_As_Binary_Length for expected behaviour
bool test_binary_05();

//test_binary_06 - checks sidex_Binary_Write, mostly provoking errors
bool test_binary_06();

//test_binary_07 - checks sidex_Binary_Read, provoking errors
bool test_binary_07();

#endif  //SDXRT_TEST_BINARY_H