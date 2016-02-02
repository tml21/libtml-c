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
#include <stdlib.h>
#ifdef LINUX
  #include <wchar.h>
  #include <stdbool.h>
  #include <limits>
#endif // LINUX
#include <sidex.h>
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

/** @brief max and min constants of Integer values
*/
const SIDEX_INT64 min64 = -9223372036854775807;
const SIDEX_INT64 max64 = 9223372036854775807;

SIDEX_INT32 intErr = SIDEX_SUCCESS; // API return value

SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_HANDLE docIntHandle = SIDEX_HANDLE_TYPE_NULL;

/** @ingroup Sidex_Integer_Tests
* @brief Checks min value for 64-bit sidex-integer in sidex_document- written with sidex_variant
*
* A sidex_variant is created with the min value for 64-bit integers. It is written to a newly created sidex_document and read again afterwards. 
* The returned value is checked against the min 64-bit integer value.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_01() {
    bool testOkay = true;
    SIDEX_INT64 temp = 0;

    intErr = sidex_Create(DOCUMENT, &docIntHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_01", "sidex_Create", 1);

    intHandle = sidex_Variant_New_Integer(min64);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Integer_Check(intHandle), "test_int_01", 
                                  "sidex_Variant_Integer_Check", 1);

    //write to sidex_document
    intErr = sidex_Variant_Write(docIntHandle, GROUP, KEY, intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_01", "sidex_Variant_Write", 1);

    //read as integer
    intErr = sidex_Integer_Read(docIntHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_01", "sidex_Integer_Read", 1);
    //compare with original
    testOkay = testOkay && errLog(temp == min64, "test_int_01", "value != intValue", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Free(&docIntHandle);
    return testOkay;
}

/** @ingroup Sidex_Integer_Tests
* @brief Checks max value for 64-bit sidex-integer in sidex_document- written with sidex_variant
*
* A sidex_variant is created with the max value for 64-bit integers. It is written to a newly created sidex_document and read again afterwards. 
* It is checked if the value from the sidex_document is the same as the max 64-bit integer value. 
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_02() {
    bool testOkay = true;
    SIDEX_INT64 temp = 0;

    intErr = sidex_Create(DOCUMENT, &docIntHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_02", "sidex_Create", 1);

    intHandle = sidex_Variant_New_Integer(max64);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Integer_Check(intHandle), "test_int_02", 
                                  "sidex_Variant_Integer_Check", 1);

    //write as variant to sidex_document
    intErr = sidex_Variant_Write(docIntHandle, GROUP, KEY, intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_02", "sidex_Variant_Write", 1);
    //read as integer
    intErr = sidex_Integer_Read(docIntHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_02", "sidex_Integer_Read", 1);
    //compare to original
    testOkay = testOkay && errLog(temp == max64, "test_int_02", "value != intValue", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Free(&docIntHandle);
    return testOkay;
}

/** @ingroup Sidex_Integer_Tests
* @brief Checks min value for 64-bit sidex-integer in sidex_document
*
* The min value for 64-bit integer is written directly to a newly created sidex_document.  
* It is checked if the value, after being read, is the same as the min 64-bit integer value. 
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_03() {
    bool testOkay = true;
    SIDEX_INT64 temp = 0;
    
    intErr = sidex_Create(DOCUMENT, &docIntHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_03", "sidex_Create", 1);
    //write to sidex_document
    intErr = sidex_Integer_Write(docIntHandle, GROUP, KEY, min64);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_03", "sidex_Integer_Write", 1);

    //read as integer
    intErr = sidex_Integer_Read(docIntHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_03", "sidex_Integer_Read", 1);
    //compare with original value
    testOkay = testOkay && errLog(temp == min64, "test_int_03", "value != intValue", 1);

    sidex_Free(&docIntHandle);

    return testOkay;
}

/** @ingroup Sidex_Integer_Tests
* @brief Checks max value for 64-bit sidex-integer in sidex_document
*
* The max value for 64-bit integer is written to a newly created sidex_document.  
* It is checked if the value, read from the sidex_document, is the same as the max 64-bit integer value. 
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_04() {
    bool testOkay = true;
    SIDEX_INT64 temp = 0;

    intErr = sidex_Create(DOCUMENT, &docIntHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_04", "sidex_Create", 1);

    //write as integer to sidex_document
    intErr = sidex_Integer_Write(docIntHandle, GROUP, KEY, max64);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_04", "sidex_Integer_Write", 1);
    //read as integer
    intErr = sidex_Integer_Read(docIntHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_04", "sidex_Integer_Read", 1);
    //compare to original value
    testOkay = testOkay && errLog(temp == max64, "test_int_04", "value != intValue", 1);

    sidex_Free(&docIntHandle);
    return testOkay;
}

/** @ingroup Sidex_Integer_Tests
* @brief Checks if function sidex_Variant_As_Integer works as expected
* 
* The function sidex_Variant_As_Integer is first tested with a wrong parameter type to throw an error. 
* It is tested with the min and max 64-bit integer values, after being created as sidex_variants 
* and converted by sidex_Variant_As_Integer. The returned error-code and value is checked.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_05() {
    bool testOkay = true;
    SIDEX_VARIANT variant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64 newInt = 0;

    variant = sidex_Variant_New_Float(1.23456);
    
    //tests a WRONG_TYPE error
    intErr = sidex_Variant_As_Integer(variant, &newInt);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == intErr, "test_int_05", "WRONG_TYPE", 1);
    sidex_Variant_DecRef(variant);

    //test function with min64
    variant = sidex_Variant_New_Integer(min64);
    intErr = sidex_Variant_As_Integer(variant, &newInt);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_05", "sidex_Variant_As_Integer", 1);
    //compare to original value
    testOkay = testOkay && errLog(newInt == min64, "test_int_05", "value != originalValue", 1);
    sidex_Variant_DecRef(variant);

    //test function with max64
    variant = sidex_Variant_New_Integer(max64);
    intErr = sidex_Variant_As_Integer(variant, &newInt);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_05", "sidex_Variant_As_Integer", 2);
    //compare to original value
    testOkay = testOkay && errLog(newInt == max64, "test_int_05", "value != originalValue", 2);
    sidex_Variant_DecRef(variant);

    return testOkay;
}


/** @ingroup Sidex_Integer_Tests
* @brief Checks sidex_Integer_Read & sidex_Integer_Write with different special characters for group- and keynames
*
* A sidex_document is created and the same value is written three times inside it. The groupnames and keynames during these three times are all unusual strings.
* Afterwards, the same group- and keynames are used with sidex_Integer_Read, to check if no errors are occurring.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_06() {
    bool testOkay = true;
    SIDEX_INT64 value = -42;
    intErr = sidex_Create(DOCUMENT, &docIntHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_06", "sidex_Create", 1);
    //write
    intErr = sidex_Integer_Write(docIntHandle, GROUP2, KEY2, value);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_06", "sidex_Integer_Write", 1);
    intErr = sidex_Integer_Write(docIntHandle, GROUP3, KEY3, value);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_06", "sidex_Integer_Write", 2);
    intErr = sidex_Integer_Write(docIntHandle, GROUP4, KEY4, value);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_06", "sidex_Integer_Write", 3);
    //read
    intErr = sidex_Integer_Read(docIntHandle, GROUP2, KEY2, &value);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_06", "sidex_Integer_Read", 1);
    intErr = sidex_Integer_Read(docIntHandle, GROUP3, KEY3, &value);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_06", "sidex_Integer_Read", 2);
    intErr = sidex_Integer_Read(docIntHandle, GROUP4, KEY4, &value);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_06", "sidex_Integer_Read", 3);

    sidex_Free(&docIntHandle);
    return testOkay;
}

/** @ingroup Sidex_Integer_Tests
* @brief Checks if the function sidex_Integer_Write throws appropriate errors
*
* A Sidex_document is created and the function sidex_Integer_Write is tested with different wrong parameters to throw expected errors.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_07() {
    bool testOkay = true;
    intErr = sidex_Create(DOCUMENT, &docIntHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_07", "sidex_Create", 1);
    //group is null
    intErr = sidex_Integer_Write(docIntHandle, NULL, KEY, min64);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == intErr || SIDEX_ERR_UNICODE == intErr, 
                                  "test_int_07", "MISSING_GROUP or UNICODE", 1);
    //key is null
    intErr = sidex_Integer_Write(docIntHandle, GROUP, NULL, min64);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == intErr || SIDEX_ERR_UNICODE == intErr, 
                                  "test_int_07", "MISSING_KEY or UNICODE", 1);
    //wrong group characters
    intErr = sidex_Integer_Write(docIntHandle, wGROUP, KEY, min64);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == intErr, "test_int_07", "WRONG_GROUP", 1);
    //wrong key characters
    intErr = sidex_Integer_Write(docIntHandle, GROUP, wKEY, min64);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == intErr, "test_int_07", "WRONG_KEY", 1);

    sidex_Free(&docIntHandle);
    return testOkay;
}

/** @ingroup Sidex_Integer_Tests
* @brief Checks if the function sidex_Integer_Read throws appropriate errors
*
* A sidex_document is created and the min value for 64-bit integers is written to it. 
* The function sidex_Integer_Read is tested with wrong parameters to throw errors.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_08() {
    bool testOkay = true;
    SIDEX_INT64 temp = 0;
    intErr = sidex_Create(DOCUMENT, &docIntHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_08", "sidex_Create", 1);
    //normal integer write
    intErr = sidex_Integer_Write(docIntHandle, GROUP, KEY, min64);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_08", "sidex_Integer_Write", 1);
    //group is null
    intErr = sidex_Integer_Read(docIntHandle, NULL, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == intErr || SIDEX_ERR_UNICODE == intErr, 
                                  "test_int_08", "MISSING_GROUP or UNICODE", 1);
    //key is null
    intErr = sidex_Integer_Read(docIntHandle, GROUP, NULL, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == intErr || SIDEX_ERR_UNICODE == intErr, 
                                  "test_int_08", "MISSING_KEY or UNICODE", 1);
    //wrong group characters
    intErr = sidex_Integer_Read(docIntHandle, wGROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == intErr, "test_int_08", "WRONG_GROUP", 1);
    //wrong key characters
    intErr = sidex_Integer_Read(docIntHandle, GROUP, wKEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == intErr, "test_int_08", "WRONG_KEY", 1);
    //there is no entry
    intErr = sidex_Integer_Read(docIntHandle, GROUP2, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == intErr, "test_int_08", "NOCONTENT", 1);
    //integer inside doc is not an integer, write & tested for read
    intErr = sidex_Float_Write(docIntHandle, GROUP, KEY2, 1.05);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_08", "sidex_Float_Write", 1);
    intErr = sidex_Integer_Read(docIntHandle, GROUP, KEY2, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == intErr, "test_int_08", "WRONG_TYPE", 1);

    sidex_Free(&docIntHandle);
    return testOkay;
}

/** @ingroup Sidex_Integer_Tests
* @brief Calls sidex_Integer_Write and sidex_Integer_Read in a loop with random values
*
* A sidex_document is created and in a loop, different kinds of integer values are written to the sidex_document
* and read again, as well as checked.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_int_09() {
    bool testOkay = true;
    SIDEX_INT64 temp = 0;
    SIDEX_INT64 nmbr = 0;
    intErr = sidex_Create(DOCUMENT, &docIntHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_09", "sidex_Create", 1);
    for(int i = 0; i < 1000; i++) {
        nmbr = rand();  //generates random number between 0 and at least 32767.
        //write
        intErr = sidex_Integer_Write(docIntHandle, GROUP, KEY, nmbr);
        testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_09", "sidex_Integer_Write", i);
        //read
        intErr = sidex_Integer_Read(docIntHandle, GROUP, KEY, &temp);
        testOkay = testOkay && errLog(SIDEX_SUCCESS == intErr, "test_int_09", "sidex_Integer_Read", i);
        //compare to original value
        testOkay = testOkay && errLog(temp == nmbr, "test_int_09", "value != docValue", i);
    }
    return testOkay;
}


/** @ingroup Sidex_Integer_Tests
* @brief Calls all subtests
* 
* @param int test: If 0- all subtests are executed, if 1-9- that one subtest is executed, returns false otherwise.
* @param bool stop: If true- when a subtest fails, the test returns immediately. If false- all subtests are executed.
* Different functions are tested, see test_int_01-test_int_09 for more details.
*
* @returns bool testOkay : false on failure of subtest, true on success.
*/
bool test_sidex_int(int test, bool stop) {
    bool testsOkay = true;

    switch(test) {
    case 0:
    case 1:
        //test_int_01
        testsOkay = errLog(test_int_01(), "test_int_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }
    
    case 2:
        //test_int_02
        testsOkay = errLog(test_int_02(), "test_int_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3:
        //test_int_03
        testsOkay = errLog(test_int_03(), "test_int_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }

    case 4:
        //test_int_04
        testsOkay = errLog(test_int_04(), "test_int_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }
    
    case 5:
        //test_int_05
        testsOkay = errLog(test_int_05(), "test_int_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }

    case 6:
        //test_int_06
        testsOkay = errLog(test_int_06(), "test_int_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_int_07
        testsOkay = errLog(test_int_07(), "test_int_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }

    case 8:
        //test_int_08
        testsOkay = errLog(test_int_08(), "test_int_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }
    case 9:
        //test_int_09
        testsOkay = errLog(test_int_09(), "test_int_09") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 9) {
            break;
        }
    }
        
    return testsOkay;
}