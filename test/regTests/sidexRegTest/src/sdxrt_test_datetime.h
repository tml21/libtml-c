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
#ifndef SDXRT_TEST_DATETIME_H
#define SDXRT_TEST_DATETIME_H

#include <sidex.h>

/** @defgroup Sidex_DateTime_Tests
  * @brief Test Reference for Sidex_DateTime
  */

//calls all other subtests
bool test_sidex_date(int test, bool stop);
//checks sidex_Variant_New_DateTime with min value of dateTime
bool test_date_01();
//checks sidex_Variant_New_DateTime with max value of dateTime
bool test_date_02();
//checks sidex_DateTime_Write, provoking errors
bool test_date_03();
//checks sidex_DateTime_Read, provoking errors
bool test_date_04();
//check sidex_Variant_DateTime_Check and sidex_Variant_New_DateTime
bool test_date_05();
// checking sidex_DateTime_Write and sidex_DateTime_Read with unusual characters for param group and key
bool test_date_06();
//checks sidex_Variant_As_DateTime, provoking errors
bool test_date_07();
// A loop, that writes and reads out lots of dtaeTime-values in a sidex_document
bool test_date_08();

#endif //SDXRT_TEST_DATETIME_H