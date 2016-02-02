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

#include <stdio.h>
#ifdef LINUX
  #include <wchar.h> 
#endif // LINUX
#include <sidex.h>
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

SIDEX_INT32 noneErr      = SIDEX_SUCCESS; // API return value

SIDEX_VARIANT noneHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_HANDLE docNoneHandle = SIDEX_HANDLE_TYPE_NULL;

/** @ingroup Sidex_None_Tests
* @brief Checks if a none value stays none after writing it to and reading it from a sidex_document as sidex_variant
* 
* A sidex_document is created and a none is put in a sidex_variant. This sidex_variant is first checked, 
* if it is really handling a none value and written to the sidex_document. 
* Afterwards it is read as sidex_variant, a none value cannot be read, and checked if the sidex_variant is still handling a none value.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_none_01() {
    bool testOkay = true;
    SIDEX_VARIANT temp = SIDEX_HANDLE_TYPE_NULL;

    noneErr = sidex_Create(DOCUMENT, &docNoneHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_01", "sidex_Create", 1);
    noneHandle = sidex_Variant_New_None();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_None_Check(noneHandle), "test_none_01", "sidex_Create", 1);
    //write as variant to sidex_document
    noneErr = sidex_Variant_Write(docNoneHandle, GROUP, KEY, noneHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_01", "sidex_Variant_Write", 1);
    //read as variant
    noneErr = sidex_Variant_Read(docNoneHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_01", "sidex_Variant_Read", 1);
    //check if entry is a none
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_None_Check(temp), "test_none_01", "sidex_Variant_None_Check", 1);
    sidex_Variant_DecRef(temp);

    sidex_Variant_DecRef(noneHandle);
    sidex_Free(&docNoneHandle);
    return testOkay;
}

/** @ingroup Sidex_None_Tests
* @brief Checks if a none value stays none after writing it directly to a sidex_document and reading it as a sidex_variant
* 
* A sidex_document is created. A none value is written to the sidex_document. It is read as sidex_variant,
* a none value cannot be read, and checked if the sidex_variant is still handling a none value.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_none_02() {
    bool testOkay = true;
    SIDEX_VARIANT temp = SIDEX_HANDLE_TYPE_NULL;

    noneErr = sidex_Create(DOCUMENT, &docNoneHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_02", "sidex_Create", 1);
    //write to sidex_document
    noneErr = sidex_None_Write(docNoneHandle, GROUP, KEY);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_02", "sidex_None_Write", 1);

    //read as variant
    noneErr = sidex_Variant_Read(docNoneHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_02", "sidex_None_Read", 1);
    //check if entry is a none
    testOkay = testOkay && errLog(sidex_Variant_None_Check(temp), "test_none_02", "sidex_Variant_None_Check", 1);

    sidex_Free(&docNoneHandle);
    sidex_Variant_DecRef(temp);
    return testOkay;
}

/** @ingroup Sidex_None_Tests
* @brief Checks if sidex_Variant_Read, handling none-values, and sidex_None_Write accept different kinds of characters for params group and key.
*
* First a sidex_document is created. A none value is written to the sidex_document with unusual characters three times.
* Then the same values are read as sidex_variants.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_none_03() {
    bool testOkay = true;
    SIDEX_VARIANT temp = SIDEX_HANDLE_TYPE_NULL;

    noneErr = sidex_Create(DOCUMENT, &docNoneHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_03", "sidex_Create", 1);
    //write None
    noneErr = sidex_None_Write(docNoneHandle, GROUP2, KEY2);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_03", "sidex_None_Write", 1);
    noneErr = sidex_None_Write(docNoneHandle, GROUP3, KEY3);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_03", "sidex_None_Write", 2);
    noneErr = sidex_None_Write(docNoneHandle, GROUP4, KEY4);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_03", "sidex_None_Write", 3);
    //read Variant
    noneErr = sidex_Variant_Read(docNoneHandle, GROUP2, KEY2, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_03", "sidex_Variant_Read", 1);
    sidex_Variant_DecRef(temp);
    noneErr = sidex_Variant_Read(docNoneHandle, GROUP3, KEY3, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_03", "sidex_Variant_Read", 2);
    sidex_Variant_DecRef(temp);
    noneErr = sidex_Variant_Read(docNoneHandle, GROUP4, KEY4, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_03", "sidex_Variant_Read", 3);

    sidex_Variant_DecRef(temp);
    sidex_Free(&docNoneHandle);
    return testOkay;
}

/** @ingroup Sidex_None_Tests
* @brief Checks if the function sidex_None_Write throws expected errors
* 
* A sidex_document is created. Different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/ 
bool test_none_04() {
    bool testOkay = true;
    noneErr = sidex_Create(DOCUMENT, &docNoneHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == noneErr, "test_none_04", "sidex_Create", 1);
    //group is null
    noneErr = sidex_None_Write(docNoneHandle, NULL, KEY);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == noneErr || SIDEX_ERR_UNICODE == noneErr, 
                                  "test_none_04", "MISSING_GROUP or UNICODE", 1);
    //key is null
    noneErr = sidex_None_Write(docNoneHandle, GROUP, NULL);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == noneErr || SIDEX_ERR_UNICODE == noneErr, 
                                  "test_none_04", "MISSING_KEY or UNICODE", 1);
    //wrong group characters
    noneErr = sidex_None_Write(docNoneHandle, wGROUP, KEY);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == noneErr, "test_none_04", "WRONG_GROUP", 1);
    //wrong key characters
    noneErr = sidex_None_Write(docNoneHandle, GROUP, wKEY);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == noneErr, "test_none_04", "WRONG_KEY", 1);

    sidex_Free(&docNoneHandle);
    return testOkay;
}

/** @ingroup Sidex_None_Tests
* @brief Calls all subtests
* 
* @param int test: If 0- all subtests are executed. If 1-4, the particular subtest is executed.
* @param bool stop: If false- all subtests are executed. If true- it returns immediately after a subtest fails.
* 
* For more detailed information see documentation of subtests
*
* @returns bool testsOkay : false on failure of subtest, true on success.
*/
bool test_sidex_none(int test, bool stop) {
    bool testsOkay = true;
    switch(test) {
    case 0:
    case 1:
        //test_none_01
        testsOkay = errLog(test_none_01(), "test_none_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) break;

    case 2:
        //test_none_02
        testsOkay = errLog(test_none_02(), "test_none_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) break;

    case 3:
        //test_none_03
        testsOkay = errLog(test_none_03(), "test_none_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) break;

    case 4:
        //test_none_04
        testsOkay = errLog(test_none_04(), "test_none_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) break;

    }
    return testsOkay;
}