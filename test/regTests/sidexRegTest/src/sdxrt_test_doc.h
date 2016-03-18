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
#ifndef SDXRT_TEST_DOCUMENT_H
#define SDXRT_TEST_DOCUMENT_H

#include <sidex.h>

/** @defgroup Sidex_Document_Tests
  * @brief Test Reference for Sidex_Document
  */

/** @defgroup sidex_Merge_Subtest
* @ingroup Sidex_Document_Tests
* @brief Tests for checking the functionality of sidex_Merge
*/

bool test_sidex_doc(int test, bool stop);

//tests sidex_Create to throw errors
bool test_doc_01();

//sidex_Set_DocumentName is tested for throwing errors
bool test_doc_02();

//the function sidex_Get_DocumentName is tested
bool test_doc_03();

//checks Save_Content
bool test_doc_04();

//check Load_Content for appropriate errors
bool test_doc_05();

//Set_Content is tested for throwing an error
bool test_doc_06();

//checks the function Get_Content and sidex_Get_Content_Length, also provoking errors
bool test_doc_07();

//checks sidex_Clear
bool test_doc_08();

//checks sidex_merge, also provoking errors
bool test_doc_09();

//checks Group management functions, sidex_GetGroups, sidex_HasGroup, sidex_DeleteGroup
bool test_doc_10();

//check sidex_GetKeys, sidex_HasKey and sidex_DeleteKey
bool test_doc_11();


#endif  //SDXRT_TEST_DOCUMENT_H