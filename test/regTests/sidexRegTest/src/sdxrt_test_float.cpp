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
  #include <limits>
#endif // LINUX
#include <sidex.h>
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

SIDEX_INT32 floatErr      = SIDEX_SUCCESS; // API return value

SIDEX_DOUBLE minFl = numeric_limits<float>::min();
SIDEX_DOUBLE maxFl = numeric_limits<float>::max();
SIDEX_DOUBLE lowFl = numeric_limits<float>::lowest();

SIDEX_VARIANT floatHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_HANDLE docFloatHandle = SIDEX_HANDLE_TYPE_NULL;

/** @ingroup Sidex_Float_Tests
* @brief Checks if the min value of float can be written as a sidex_variant to a sidex_document, read and stays the same value
*
* A sidex_document and a sidex_variant are created. The sidex_variant gets the value of minFl- the min value for floats.
* The sidex_variant is first checked for being a sidex_variant handling a float and then written to the sidex_document. 
* Then the entry is read as float and compared to the minFl. The min value is the number that is closest to null, but not null.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_float_01() {
    bool testOkay = true;
    SIDEX_DOUBLE temp = 0.00;

    floatErr = sidex_Create(DOCUMENT, &docFloatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_01", "sidex_Create", 1);

    floatHandle = sidex_Variant_New_Float(minFl);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Float_Check(floatHandle), "test_float_01", 
        "sidex_Variant_Float_Check", 1);
    //write as variant to sidex_document
    floatErr = sidex_Variant_Write(docFloatHandle, GROUP, KEY, floatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_01", "sidex_Variant_Write", 1);
    //read entry as float again
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_01", "sidex_Float_Read", 1);
    //compare to original
    testOkay = testOkay && errLog(temp == minFl, "test_float_01", "value != Document", 1);

    sidex_Variant_DecRef(floatHandle);
    sidex_Free(&docFloatHandle);
    return testOkay;
}

/** @ingroup Sidex_Float_Tests
* @brief Checks if the max value of float can be written as sidex_variant to a sidex_document, read and stay the same value
* 
* A sidex_document and a sidex_variant are created. The sidex_variant gets the value of minFl- the min value for floats.
* The sidex_variant is first checked for being a sidex_variant handling a float and then written to the sidex_document. 
* The entry is read as float and compared to the minFl.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_float_02() {
    bool testOkay = true;
    SIDEX_DOUBLE temp = 0;

    floatErr = sidex_Create(DOCUMENT, &docFloatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_02", "sidex_Create", 1);
    
    floatHandle = sidex_Variant_New_Float(maxFl);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Float_Check(floatHandle), "test_float_02", "sidex_Variant_Float_Check", 1);
    //write the variant to the sidex_document
    floatErr = sidex_Variant_Write(docFloatHandle, GROUP, KEY, floatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_02", "sidex_Variant_Write", 1);
    //read the the entry again
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_02", "sidex_Float_Read", 1);
    //compare to the original
    testOkay = testOkay && errLog(temp == maxFl, "test_float_02", "value != Document", 1);

    sidex_Variant_DecRef(floatHandle);
    sidex_Free(&docFloatHandle);
    return testOkay;
}

/** @ingroup Sidex_Float_Tests
* @brief Checks if the min value of floats can be written directly to a sidex_document and stay the same after reading it again
* 
* A sidex_document is created and the min value for floats is written to the sidex_document. 
* The entry is read from the document and checked if the value from the document is the same as the min value for floats.
* The min value is the number that is closest to null, but not null.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_float_03() { 
    bool testOkay = true;
    SIDEX_DOUBLE temp = 0;

    floatErr = sidex_Create(DOCUMENT, &docFloatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_03", "sidex_Create", 1);
    //write a float to the sidex_document
    floatErr = sidex_Float_Write(docFloatHandle, GROUP, KEY, minFl);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_02", "sidex_Float_Write", 1);
    //read the float again
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_02", "sidex_Float_Read", 1);
    //compare to original
    testOkay = testOkay && errLog(temp == minFl, "test_float_02", "value != Document", 1);

    sidex_Free(&docFloatHandle);
    return testOkay;
}

/** @ingroup Sidex_Float_Tests
* @brief Checks if the max value of floats can be written directly to a sidex_document and stay the same after reading it again.
* 
* A sidex_document is created and the max value for floats is written to the sidex_document. 
* The entry is read from the document and checked if the value from the document is the same as the max value for floats.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_float_04() {
    bool testOkay = true;
    SIDEX_DOUBLE temp = 0;
    floatErr = sidex_Create(DOCUMENT, &docFloatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_04", "sidex_Create", 1);
    //write a float to the sidex_document
    floatErr = sidex_Float_Write(docFloatHandle, GROUP, KEY, maxFl);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_04", "sidex_Float_Write", 1);
    //read the float again
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_04", "sidex_Float_Read", 1);
    //compare to original
    testOkay = testOkay && errLog(temp == numeric_limits<float>::max(), "test_float_04", "value != Document", 1);

    sidex_Free(&docFloatHandle);
    return testOkay;
}

/** @ingroup Sidex_Float_Tests
* @brief Checks if the lowest value for floats stays the same after writing it to a sidex_document as variant or directly.
*
* A sidex_document and a sidex_variant are created and the sidex_variant contains the lowest value for floats.
* The sidex_variant and the lowest value for floats are written to the sidex_document with different keys.
* Both entries are read and compared to the original, the lowest value of floats. 
* The lowest value of floats is the highest minus value of floats.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_float_05() {
    bool testOkay = true;
    SIDEX_DOUBLE temp = 0;
    floatErr = sidex_Create(DOCUMENT, &docFloatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_05", "sidex_Create", 1);
    floatHandle = sidex_Variant_New_Float(lowFl);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Float_Check(floatHandle), 
        "test_float_05", "sidex_Variant_Float_Check", 1);
    //write a variant to the sidex_document
    floatErr = sidex_Variant_Write(docFloatHandle, GROUP, HANDLEKEY, floatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_05", "sidex_Variant_Write", 1);
    //write a float to the sidex_document
    floatErr = sidex_Float_Write(docFloatHandle, GROUP, FLOATKEY, lowFl);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_05", "sidex_Float_Write", 1);
    //read the variant-entry as float again
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, HANDLEKEY, &temp);	
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_05", "sidex_Float_Read", 1);
    //compare to original
    testOkay = testOkay && errLog(temp == lowFl, "test_float_05", "value != Document", 1);
    //read the float as float
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, FLOATKEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_05", "sidex_Float_Read", 2);
    //compare to original
    testOkay = testOkay && errLog(temp == lowFl, "test_float_05", "value != Document", 2);

    sidex_Variant_DecRef(floatHandle);
    sidex_Free(&docFloatHandle);
    return testOkay;
}

/** @ingroup Sidex_Float_Tests
* @brief Checks if sidex_Float_Write is throwing appropriate errors
*
* A sidex_document is created. Different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_float_06() {
    bool testOkay = true;
    floatErr = sidex_Create(DOCUMENT, &docFloatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_06", "sidex_Create", 1);

    //group is null
    floatErr = sidex_Float_Write(docFloatHandle, NULL, KEY, lowFl);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == floatErr || SIDEX_ERR_UNICODE == floatErr, 
        "test_float_06", "MISSING_GROUP or UNICODE", 1);

    //key is null
    floatErr = sidex_Float_Write(docFloatHandle, GROUP, NULL, lowFl);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == floatErr || SIDEX_ERR_UNICODE == floatErr, 
        "test_float_06", "MISSING_KEY or UNICODE", 1);

    //wrong group characters
    floatErr = sidex_Float_Write(docFloatHandle, wGROUP, KEY, lowFl);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == floatErr, "test_float_06", "WRONG_GROUP", 1);

    //wrong key characters
    floatErr = sidex_Float_Write(docFloatHandle, GROUP, wKEY, lowFl);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == floatErr, "test_float_06", "WRONG_KEY", 1);

    sidex_Free(&docFloatHandle);
    return testOkay;
}

/** @ingroup Sidex_Float_Tests
* @brief Checks if sidex_Float_Read is throwing appropriate errors
*
* A sidex_document is created and a float is written in. Then different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_float_07() {
    bool testOkay = true;
    SIDEX_DOUBLE temp = 0;

    //normal sidex_create
    floatErr = sidex_Create(DOCUMENT, &docFloatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_07", "sidex_Create", 1);

    //write a float to the sidex_document
    floatErr = sidex_Float_Write(docFloatHandle, GROUP, KEY, lowFl);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_07", "sidex_Float_Write", 1);

    //group is null 
    floatErr = sidex_Float_Read(docFloatHandle, NULL, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == floatErr || SIDEX_ERR_UNICODE == floatErr, 
        "test_float_07", "MISSING_GROUP or UNICODE", 1);

    //key is null 
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, NULL, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == floatErr || SIDEX_ERR_UNICODE == floatErr, 
        "test_float_07", "MISSING_KEY or UNICODE", 1);

    // group-name has invalid characters 
    floatErr = sidex_Float_Read(docFloatHandle, wGROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == floatErr, "test_float_07", "WRONG_GROUP", 1);

    // key-name has invalid characters 
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, wKEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == floatErr, "test_float_07", "WRONG_KEY", 1);

    //doc has no entry there 
    floatErr = sidex_Float_Read(docFloatHandle, GROUP2, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == floatErr, "test_float_07", "NOCONTENT", 1);

    // value is not float, wrong type 
    floatErr = sidex_String_Write(docFloatHandle, GROUP, KEY2, KEY3);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_07", "sidex_String_Write", 1);
    floatErr = sidex_Float_Read(docFloatHandle, GROUP, KEY2, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == floatErr, "test_float_07", "WRONG_TYPE", 1);
    
    sidex_Free(&docFloatHandle);
    return testOkay;
}

/** @ingroup Sidex_Float_Tests
* @brief Calls sidex_Float_Write and sidex_Float_Read very often with random values.
*
* A sidex_document is created and in a loop, different kinds of float values are written to the sidex_document, read and checked. 
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_float_08() {
    bool testOkay = true;
    SIDEX_DOUBLE temp = 0;
    SIDEX_DOUBLE nmbr = 0;
    //normal sidex_create
    floatErr = sidex_Create(DOCUMENT, &docFloatHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_08", "sidex_Create", 1);

    for(int i = 0; i < 1000; i++) {
        nmbr = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);	//generates number between 0.0 to 1.0
        //write to sidex_document
        floatErr = sidex_Float_Write(docFloatHandle, GROUP, KEY, nmbr);
        testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_08", "sidex_Float_Write", i);
        //read again
        floatErr = sidex_Float_Read(docFloatHandle, GROUP, KEY, &temp);
        testOkay = testOkay && errLog(SIDEX_SUCCESS == floatErr, "test_float_08", "sidex_Float_Read", i);
        //compare to original
        testOkay = testOkay && errLog(temp == nmbr, "test_float_08", "one value != doc", i);
    }
    return testOkay;
}

/** @ingroup Sidex_Float_Tests
* @brief Calls all subtests
*
* @param int test: If 0- do all tests, if 1-8- execute the particular subtest.
* @param bool stop: If false- execute tests normally, if true- return after a subtest failed.
* 
* For more information see documentation of the particular subtests.
*
* @returns bool testsOkay : false on failure of subtest, true on success.
*/
bool test_sidex_float(int test, bool stop) {
    bool testsOkay = true;

    switch(test) {
    case 0:
    case 1:
        //test_float_01
        testsOkay = errLog(test_float_01(), "test_float_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }

    case 2:
        //test_float_02
        testsOkay = errLog(test_float_02(), "test_float_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3:
        //test_float_03
        testsOkay = errLog(test_float_03(), "test_float_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }

    case 4:
        //test_float_04
        testsOkay = errLog(test_float_04(), "test_float_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }

    case 5:
        //test_float_05
        testsOkay = errLog(test_float_05(), "test_float_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }

    case 6:
        //test_float_06
        testsOkay = errLog(test_float_06(), "test_float_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_float_07
        testsOkay = errLog(test_float_07(), "test_float_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }

    case 8:
        //test_float_08
        testsOkay = errLog(test_float_08(), "test_float_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }
    }
    return testsOkay;
}