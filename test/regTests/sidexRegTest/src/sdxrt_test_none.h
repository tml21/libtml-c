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
#ifndef SDXRT_TEST_NONE_H
#define SDXRT_TEST_NONE_H


#include <sidex.h>

/** @defgroup Sidex_None_Tests
  * @brief Test Reference for Sidex_None
  */

bool test_sidex_none(int test, bool stop);
//test_none_01 - check none value through writing in as variant and reading out of a sidex document
bool test_none_01();
//test_none_02 - check none value through writing in and reading out of a sidex document
bool test_none_02();
//test_none_03 - checking in sidex_Variant_Read & sidex_None_Write with different special characters for group- and keynames
bool test_none_03();
//test_none_04 - check if the function sidex_None_Write throws appropriate errors
bool test_none_04();

#endif      //SDXRT_TEST_NONE_H