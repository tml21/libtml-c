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
#ifndef SDXRT_TEST_FLOAT_H
#define SDXRT_TEST_FLOAT_H


#include <sidex.h>

/** @defgroup Sidex_Float_Tests
  * @brief Test Reference for Sidex_Float
  */

bool test_sidex_float(int test, bool stop);
//floatTest01 - check min value for FLOAT through writing in as variant and reading out of a sidex document
bool test_float_01();
//floatTest02 - check max value for FLOAT through writing in as variant and reading out of a sidex document
bool test_float_02();
//floatTest03 - check min value for FLOAT through writing in and reading out of a sidex document
bool test_float_03();
//floatTest04 - check max value for FLOAT through writing in and reading out of a sidex document
bool test_float_04();
//floatTest05 - check lowest value for FLOAT through writing in as variant (& as SIDEX_FLOAT) and reading out of a sidex document
bool test_float_05();
//floatTest06 - check if the function sidex_Float_Write throws appropriate errors
bool test_float_06();
//floatTest07 - check if the function sidex_Float_Read throws appropriate Errors
bool test_float_07();
// test_float_08 - calls sidex_Float_Write and sidex_Float_Read very often with random values 
bool test_float_08();

#endif  //SDXRT_TEST_FLOAT_H