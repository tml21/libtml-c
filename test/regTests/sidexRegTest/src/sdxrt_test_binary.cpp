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
#include "sdxrt_tchar.h"
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

SIDEX_VARIANT binHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_HANDLE docBinHandle = SIDEX_HANDLE_TYPE_NULL;

unsigned char maxChar = 255;
unsigned char minChar = 0;
SIDEX_INT32 binLength = 0;

SIDEX_INT32 binErr = SIDEX_SUCCESS;

/** @ingroup Sidex_Binary_Tests
* @brief Checks sidex_Variant_New_Binary with maxChar
*
* A new binary is created and assigned to a sidex_variant. It is checked, 
* whether it is a sidex_binary and written to a recently created sidex_document. It is 
* read again and checked whether the binary stayed the same as before being written 
* to the sidex_document.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_binary_01() {
    bool testOkay = true;
    unsigned char* tempVal = NULL;

    //normal creating of a sidex_binary
    binLength = 3;
    binErr = sidex_Variant_New_Binary(&maxChar, binLength, &binHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_01", "New_Binary", 1);

    //normal sidex_Create
    binErr = sidex_Create(DOCUMENT, &docBinHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_01", "sidex_Create", 1);

    //write the binary-variant to the sidex_document
    binErr = sidex_Variant_Write(docBinHandle, GROUP, KEY, binHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_01", "sidex_Variant_Write", 1);

    //read the binary variant
    sidex_Variant_DecRef(binHandle);
    binErr = sidex_Variant_Read(docBinHandle, GROUP, KEY, &binHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_01", "sidex_Variant_Read", 1);

    //compare the value of the entry against the original one
    binErr = sidex_Variant_As_Binary(binHandle, &tempVal, &binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_01", "sidex_Variant_As_Binary", 1);
    testOkay = testOkay && errLog(maxChar == *tempVal, "test_binary_01", "value != entry", 1);

    sidex_Variant_DecRef(binHandle);
    sidex_Free(&docBinHandle);
    return testOkay;
}


/**  @ingroup Sidex_Binary_Tests
* @brief Checks sidex_Binary_Write and Binary_Read with minChar
*
* A sidex_document is created and a binary is written to it. Then it is 
* read again and checked whether the binary stayed the same as before being written 
* to the sidex_document.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_binary_02() {
    bool testOkay = true;
    unsigned char* binValue = NULL;

    //normal sidex_Create
    binErr = sidex_Create(DOCUMENT, &docBinHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_02", "sidex_Create", 1);

    //write the binary-variant to the sidex_document
    binLength = 1;
    binErr = sidex_Binary_Write(docBinHandle, GROUP, KEY, &minChar, binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_02", "sidex_Binary_Write", 1);

    //read the binary variant
    binErr = sidex_Binary_Read(docBinHandle, GROUP, KEY, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_02", "sidex_Binary_Read", 1);

    //compare the value of the entry with the original one
    testOkay = testOkay && errLog(minChar == *binValue, "test_binary_02", "value != entry", 1);

    sidex_Free_Binary_ReadString(binValue);
    sidex_Free(&docBinHandle);
    return testOkay;
}


/**  @ingroup Sidex_Binary_Tests
* @brief Checks sidex_Variant_Binary_Check
*
* A new binary is created and assigned to a sidex_variant. 
* Another side_variant is created, handling an integer.
* Afterwards both are checked with Binary_Check.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_binary_03() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a sidex_binary
    binLength = 3;
    binErr = sidex_Variant_New_Binary(&maxChar, binLength, &binHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_03", "New_Binary", 1);

    //normal creating of a sidex_variant, handling an integer
    intHandle = sidex_Variant_New_Integer(1234567);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Integer_Check(intHandle), "test_binary_03", "sidex_Variant_Integer_Check", 1);

    //check both sidex_variants
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Binary_Check(binHandle), "test_binary_03", "sidex_Variant_Binary_Check", 1);

    //expect a SIDEX_FALSE error
    testOkay = testOkay && errLog(SIDEX_FALSE == sidex_Variant_Binary_Check(intHandle), "test_binary_03", "sidex_Variant_Integer_Check", 2);

    sidex_Variant_DecRef(binHandle);
    sidex_Variant_DecRef(intHandle);
    return testOkay;
}


/**  @ingroup Sidex_Binary_Tests
* @brief Checks sidex_Variant_As_Binary for expected behaviour
*
* A sidex_binary is assigned to a sidex_variant. Another sidex_Variant is created,
* handling an integer. Afterwards it is tried to convert them to binaries. The converting of the
* sidex_variant handling the integer returns an expected WRONG_TYPE error.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_binary_04() {
    bool testOkay = true;
    unsigned char* binValue = NULL;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a sidex_binary
    binLength = 3;
    binErr = sidex_Variant_New_Binary(&maxChar, binLength, &binHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_04", "New_Binary", 1);

    //normal creating of a sidex_variant, handling an integer
    intHandle = sidex_Variant_New_Integer(1234567);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Integer_Check(intHandle), "test_binary_04", "sidex_Variant_Integer_Check", 1);

    //converting the 'binHandle' to a binary:
    binErr = sidex_Variant_As_Binary(binHandle, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_04", "sidex_Variant_As_Binary", 1);
    //and check whether the values are the same as the original
    testOkay = testOkay && errLog(*binValue == maxChar, "test_binary_04", "binValue != value", 1);
    testOkay = testOkay && errLog(binLength == 3, "test_binary_04", "binLength != length", 1);

    //expecting the error WRONG_TYPE
    binErr = sidex_Variant_As_Binary(intHandle, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == binErr, "test_binary_04", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(binHandle);
    return testOkay;
}


/**  @ingroup Sidex_Binary_Tests
* @brief Checks sidex_Variant_As_Binary_Length for expected behaviour
*
* A sidex_binary is assigned to a sidex_variant. Another sidex_Variant is created,
* handling an integer. Afterwards it is tried to get their length. The functioncall with
* sidex_variant handling the integer returns an expected WRONG_TYPE error.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_binary_05() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a sidex_binary
    binLength = 3;
    binErr = sidex_Variant_New_Binary(&maxChar, binLength, &binHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_05", "New_Binary", 1);

    //normal creating of a sidex_variant, handling an integer
    intHandle = sidex_Variant_New_Integer(1234567);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Integer_Check(intHandle), "test_binary_05", "sidex_Variant_Integer_Check", 1);

    //converting the 'binHandle' to a binary:
    binErr = sidex_Variant_As_Binary_Length(binHandle, &binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_05", "sidex_Variant_As_Binary", 1);
    testOkay = testOkay && errLog(binLength == 3, "test_binary_05", "binLength != length", 1);

    //expecting the error WRONG_TYPE
    binErr = sidex_Variant_As_Binary_Length(intHandle, &binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == binErr, "test_binary_05", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(binHandle);
    return testOkay;
}


/**  @ingroup Sidex_Binary_Tests
* @brief Checks sidex_Binary_Write, mostly testing error codes
*
* A sidex_document is created and then a sidex_binary is written to it.
* All expected error codes are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_binary_06() {
    bool testOkay = true;
    //normal sidex_Create
    binErr = sidex_Create(DOCUMENT, &docBinHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_06", "sidex_Create", 1);

    //write the binary-variant to the sidex_document
    binLength = 3;
    binErr = sidex_Binary_Write(docBinHandle, GROUP, KEY, &maxChar, binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_06", "sidex_Binary_Write", 1);

    //testing a MISSING_KEY error or UNICODE
    binErr = sidex_Binary_Write(docBinHandle, GROUP, NULL, &maxChar, binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == binErr || SIDEX_ERR_UNICODE == binErr, "test_binary_06", "MISSING_KEY or UNICODE", 1);

    //testing a MISSING_GROUP error or UNICODE
    binErr = sidex_Binary_Write(docBinHandle, NULL, KEY, &maxChar, binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == binErr || SIDEX_ERR_UNICODE == binErr, "test_binary_06", "MISSING_GROUP or UNICODE", 1);

    //testing a WRONG_KEY error
    binErr = sidex_Binary_Write(docBinHandle, GROUP, wKEY, &maxChar, binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == binErr, "test_binary_06", "WRONG_KEY", 1);

    //testing a WRONG_GROUP error
    binErr = sidex_Binary_Write(docBinHandle, wGROUP, KEY, &maxChar, binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == binErr, "test_binary_06", "WRONG_GROUP", 1);

    sidex_Free(&docBinHandle);
    return testOkay;
}


/**  @ingroup Sidex_Binary_Tests
* @brief Checks sidex_Binary_Read, testing for errors
*
* A sidex_document is created and a sidex_binary and a sidex_integer are written to it.
* All expected errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_binary_07() {
    bool testOkay = true;
    unsigned char* binValue = NULL;

    //normal sidex_Create
    binErr = sidex_Create(DOCUMENT, &docBinHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_07", "sidex_Create", 1);

    //write the binary-variant to the sidex_document
    binLength = 3;
    binErr = sidex_Binary_Write(docBinHandle, GROUP, KEY, &maxChar, binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_07", "sidex_Binary_Write", 1);

    //write a sidex_integer to the sidex_document
    binErr = sidex_Integer_Write(docBinHandle, GROUP, KEY2, -1234567);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_07", "sidex_Integer_Write", 1);

    //normal read
    binErr = sidex_Binary_Read(docBinHandle, GROUP, KEY, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == binErr, "test_binary_07", "sidex_Binary_Read", 1);
    
    //free the memory after Binary_Read
    sidex_Free_Binary_ReadString(binValue);

    //MISSING_KEY error or UNICODE
    binErr = sidex_Binary_Read(docBinHandle, GROUP2, NULL, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == binErr || SIDEX_ERR_UNICODE == binErr,
                                  "test_binary_07", "MISSING_KEY or UNICODE", 1);

    //MISSING_GROUP error or UNICODE
    binErr = sidex_Binary_Read(docBinHandle, NULL, KEY3, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == binErr || SIDEX_ERR_UNICODE == binErr,
                                  "test_binary_07", "MISSING_GROUP or UNICODE", 1);

    //WRONG_KEY error
    binErr = sidex_Binary_Read(docBinHandle, GROUP2, wKEY, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == binErr, "test_binary_07", "WRONG_KEY", 1);

    //WRONG_GROUP error
    binErr = sidex_Binary_Read(docBinHandle, wGROUP, KEY3, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == binErr, "test_binary_07", "WRONG_GROUP", 1);

    //WRONG_TYPE error
    binErr = sidex_Binary_Read(docBinHandle, GROUP, KEY2, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == binErr, "test_binary_07", "WRONG_TYPE", 1);

    //NOCONTENT error
    binErr = sidex_Binary_Read(docBinHandle, GROUP, KEY3, &binValue, &binLength);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == binErr, "test_binary_07", "NOCONTENT", 1);

    sidex_Free(&docBinHandle);
    return testOkay;
}


/**  
* @ingroup Sidex_Binary_Tests
* @brief Calls all subtests
* 
* @param int test: If 0- all subtests are executed, if 1-7- just that one subtest is executed, returns false otherwise
* @param bool stop: If true- it returns immediately when a subtest fails. If false- all subtests are executed.
*
* @returns bool testsOkay : false on failure of a subtest, true on success.
*/
bool test_sidex_binary(int test, bool stop) {
    bool testsOkay = true;

    switch(test) {
    case 0:
    case 1:
        //test_binary_01
        testsOkay = errLog(test_binary_01(), "test_binary_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }
    
    case 2:
        //test_binary_02
        testsOkay = errLog(test_binary_02(), "test_binary_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3:
        //test_binary_03
        testsOkay = errLog(test_binary_03(), "test_binary_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }

    case 4:
        //test_binary_04
        testsOkay = errLog(test_binary_04(), "test_binary_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }
    
    case 5:
        //test_binary_05
        testsOkay = errLog(test_binary_05(), "test_binary_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }

    case 6:
        //test_binary_06
        testsOkay = errLog(test_binary_06(), "test_binary_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_binary_07
        testsOkay = errLog(test_binary_07(), "test_binary_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }
    }
    return testsOkay;
}