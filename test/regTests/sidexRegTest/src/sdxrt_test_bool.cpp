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


SIDEX_BOOL sTrue = true;
SIDEX_BOOL sFalse = false;

SIDEX_INT32 boolErr = SIDEX_SUCCESS; // API return value

SIDEX_VARIANT boolHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_HANDLE docBoolHandle = SIDEX_HANDLE_TYPE_NULL;


/** @ingroup Sidex_Bool_Tests
* @brief Check if value bool true can be written to a sidex_document as variant and stays the same.
*
* First a sidex_document is created. A sidex_boolean is put into a sidex_variant. Afterwards it is checked, if the sidex_variant really handles a boolean
* and it is written to the sidex_document. Afterwards the entry is read as boolean and checked, if the boolean stayed true.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_bool_01() {
    bool testOkay = true;
    SIDEX_BOOL temp;

    //normal sidex_create
    boolErr = sidex_Create(DOCUMENT, &docBoolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_01", "sidex_Create", 1);

    //create a variant handling a boolean
    boolHandle = sidex_Variant_New_Boolean(sTrue);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Boolean_Check(boolHandle), "test_bool_01", "sidex_Variant_Boolean_Check", 1);

    //write the variant to the sidex_document
    boolErr = sidex_Variant_Write(docBoolHandle, GROUP, KEY, boolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_01", "sidex_Variant_Write", 1);

    //read the value as boolean
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_01", "sidex_Boolean_Read", 1);
    //compare the boolean to the original
    testOkay = testOkay && errLog(temp == sTrue, "test_bool_01", "val != doc", 1);

    sidex_Variant_DecRef(boolHandle);
    sidex_Free(&docBoolHandle);
    return testOkay;
}

/** @ingroup Sidex_Bool_Tests
*  @brief Check if the bool false value is still the same after writing it as sidex_variant to a sidex_document and reading it again afterwards as bool
*
* A sidex_document is created and the boolean value false is assigned to a sidex_variant. After checking if the sidex_variant is really handling a boolean,
* it is written to the sidex_document. Then the entry is read as boolean and it gets checked, if it is still the boolean value false.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_bool_02() {
    bool testOkay = true;
    SIDEX_BOOL temp;

    boolErr = sidex_Create(DOCUMENT, &docBoolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_02", "sidex_Create", 1);
    boolHandle = sidex_Variant_New_Boolean(sFalse);

    //check whether it really is a sidex_boolean
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Boolean_Check(boolHandle), "test_bool_02", "sidex_Variant_Boolean_Check", 1);

    //write the variant to the sidex_document
    boolErr = sidex_Variant_Write(docBoolHandle, GROUP, KEY, boolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_02", "sidex_Variant_Write", 1);

    //read the value from the sidex_document
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_02", "sidex_Boolean_Read", 1);

    //compare the value to the original
    testOkay = testOkay && errLog(temp == sFalse, "test_bool_02", "val != doc", 1);

    sidex_Variant_DecRef(boolHandle);
    sidex_Free(&docBoolHandle);
    return testOkay;
}

/**  @ingroup Sidex_Bool_Tests
* @brief Check if bool value true can be written to a sidex_document and after being read, stay the same.
*
* First a sidex_document is created. The boolean value true is written to the sidex_document. 
* Afterwards the entry is read as boolean and tested if it stayed the same, true.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_bool_03() {
    bool testOkay = true;
    SIDEX_BOOL temp = NULL;

    //normal sidex_create
    boolErr = sidex_Create(DOCUMENT, &docBoolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_03", "sidex_Create", 1);

    //write a boolean to a sidex_document
    boolErr = sidex_Boolean_Write(docBoolHandle, GROUP, KEY, sTrue);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_03", "sidex_Boolean_Write", 1);

    //read the boolean
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_03", "sidex_Boolean_Read", 1);

    //comapre to original
    testOkay = testOkay && errLog(temp == sTrue, "test_bool_03", "value != entry", 1);

    sidex_Free(&docBoolHandle);

    return testOkay;
}

/**  @ingroup Sidex_Bool_Tests
* @brief Check if bool value false can be written to a sidex_document and read again, and stay the same.
*
* First a sidex_document is created. The boolean value false is written to the sidex_document. 
* Afterwards the entry is read as boolean and checked, if the boolean stayed the same, false.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_bool_04() {
    bool testOkay = true;
    SIDEX_BOOL temp = NULL;

    boolErr = sidex_Create(DOCUMENT, &docBoolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_04", "sidex_Create", 1);

    //create sidex_variant handling a boolean
    boolHandle = sidex_Variant_New_Boolean(sFalse);
    
    //check whether the variant really handles a boolean
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Boolean_Check(boolHandle), 
                                  "test_bool_04", "sidex_Variant_Boolean_Check", 1);

    //write a boolean to a sidex_document
    boolErr = sidex_Boolean_Write(docBoolHandle, GROUP, KEY, sFalse);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_04", "sidex_Boolean_Write", 1);

    //read the entry of the sidex_document
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_04", "sidex_Boolean_Read", 1);

    //compare to original
    testOkay = testOkay && errLog(temp == sFalse, "test_bool_04", "value != entry", 1);

    sidex_Variant_DecRef(boolHandle);
    sidex_Free(&docBoolHandle);
    return testOkay;
}

/**  @ingroup Sidex_Bool_Tests
* @brief Checks if function sidex_Variant_As_Boolean works as expected
*
* A sidex_variant is created and a float value is assigned to it. Different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_bool_05() {
    bool testOkay = true;
    SIDEX_VARIANT variant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_BOOL newBool = NULL;

    //test a WRONG_TYPE error
    variant = sidex_Variant_New_Float(1.23456);
    boolErr = sidex_Variant_As_Boolean(variant, &newBool);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == boolErr, "test_bool_05", "WRONG_TYPE", 1);
    sidex_Variant_DecRef(variant);

    //test with sTrue
    variant = sidex_Variant_New_Boolean(sTrue);
    boolErr = sidex_Variant_As_Boolean(variant, &newBool);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_05", "sidex_Variant_As_Boolean", 1);
    //compare value to the original
    testOkay = testOkay && errLog(newBool == sTrue, "test_bool_05", "boolean != value", 1);
    sidex_Variant_DecRef(variant);

    //test with sFalse
    variant = sidex_Variant_New_Boolean(sFalse);
    boolErr = sidex_Variant_As_Boolean(variant, &newBool);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_05", "sidex_Variant_As_Boolean", 2);
    //compare to original
    testOkay = testOkay && errLog(newBool == sFalse, "test_bool_05", "boolean != value", 2);
    sidex_Variant_DecRef(variant);

    return testOkay;
}

/**  @ingroup Sidex_Bool_Tests
* @brief sidex_Boolean_Write and sidex_Boolean_Read are used with unusual characters for params group and key
*
* A sidex_document is created and the function sidex_Boolean_Write is called three times 
* with different unusual characters for the parameters group and key.
* The same entries are read and checked, if there are no errors while reading.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_bool_06() {
    bool testOkay = true;
    SIDEX_BOOL temp = true;

    boolErr = sidex_Create(DOCUMENT, &docBoolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_06", "sidex_Create", 1);

    //write
    boolErr = sidex_Boolean_Write(docBoolHandle, GROUP2, KEY2, true);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_06", "sidex_Boolean_Write", 1);

    boolErr = sidex_Boolean_Write(docBoolHandle, GROUP3, KEY3, false);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_06", "sidex_Boolean_Write", 2);

    boolErr = sidex_Boolean_Write(docBoolHandle, GROUP4, KEY4, true);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_06", "sidex_Boolean_Write", 3);

    //read
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP2, KEY2, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_06", "sidex_Boolean_Read", 1);

    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP3, KEY3, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_06", "sidex_Boolean_Read", 2);

    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP4, KEY4, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_06", "sidex_Boolean_Read", 3);

    sidex_Free(&docBoolHandle);
    return testOkay;
}

/**  @ingroup Sidex_Bool_Tests
* @brief Check if the function sidex_Boolean_Write throws appropriate errors
*
* A sidex_document is created. Different errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/ 
bool test_bool_07() {
    bool testOkay = true;
    boolErr = sidex_Create(DOCUMENT, &docBoolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_07", "sidex_Create", 1);

    //group is null
    boolErr = sidex_Boolean_Write(docBoolHandle, NULL, KEY, sTrue);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == boolErr || SIDEX_ERR_UNICODE == boolErr,
                                  "test_bool_07", "MISSING_GROUP or UNICODE", 1);

    //key is null
    boolErr = sidex_Boolean_Write(docBoolHandle, GROUP, NULL, sTrue);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == boolErr || SIDEX_ERR_UNICODE == boolErr, 
                                  "test_bool_07", "MISSING_KEY or UNICODE", 1);

    //wrong group characters
    boolErr = sidex_Boolean_Write(docBoolHandle, wGROUP, KEY, sTrue);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == boolErr, "test_bool_07", "WRONG_GROUP", 1);

    //wrong key characters
    boolErr = sidex_Boolean_Write(docBoolHandle, GROUP, wKEY, sTrue);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == boolErr, "test_bool_07", "WRONG_KEY", 1);

    sidex_Free(&docBoolHandle);
    return testOkay;
}

/**  @ingroup Sidex_Bool_Tests
* @brief Check if the function sidex_Boolean_Read throws appropriate errors
*
* A sidex_document is created and a sidex_boolean with value true is written to it. 
* Different errors for sidex_Boolean_Read are tested. 
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_bool_08() {
    bool testOkay = true;
    SIDEX_BOOL temp;
    //normal sidex_create
    boolErr = sidex_Create(DOCUMENT, &docBoolHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_08", "sidex_Create", 1);

    //write a boolean to the sidex_document
    boolErr = sidex_Boolean_Write(docBoolHandle, GROUP, KEY, sTrue);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_08", "sidex_Boolean_Write", 1);

    //group is null
    boolErr = sidex_Boolean_Read(docBoolHandle, NULL, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == boolErr || SIDEX_ERR_UNICODE == boolErr, 
        "test_bool_08", "MISSING_GROUP or UNICODE", 1);

    //key is null
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP, NULL, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == boolErr || SIDEX_ERR_UNICODE == boolErr,
        "test_bool_08", "MISSING_KEY or UNICODE", 1);

    //wrong group characters
    boolErr = sidex_Boolean_Read(docBoolHandle, wGROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == boolErr, "test_bool_08", "WRONG_GROUP", 1);

    //wrong key characters
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP, wKEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == boolErr, "test_bool_08", "WRONG_KEY", 1);

    //there is no entry
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP2, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == boolErr, "test_bool_08", "NOCONTENT", 1);

    //integer inside doc is not a boolean, testing a WRONG_TYPE error
    boolErr = sidex_Float_Write(docBoolHandle, GROUP, KEY2, 1.05);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == boolErr, "test_bool_08", "sidex_Float_Write", 1);
    boolErr = sidex_Boolean_Read(docBoolHandle, GROUP, KEY2, &temp);    //tests the error
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == boolErr, "test_bool_08", "WRONG_TYPE", 1);

    sidex_Free(&docBoolHandle);
    return testOkay;
}

/**  @ingroup Sidex_Bool_Tests
* @brief Can call all subtests
* @param int test, If 0- all tests are executed. If 1-8- just the subtest is executed. 
* @param bool stop: If false- all subtests are executed. If true- all subtests are executed, until a subtest fails. It returns immediately after that.
* 
* For more details see documentation of each subtest.
*
* @returns bool testsOkay : false on failure of test, true on success.
*/
bool test_sidex_bool(int test, bool stop) {
    bool testsOkay = true;

    switch(test) {
    case 0:
    case 1:
        //test_bool_01
        testsOkay = errLog(test_bool_01(), "test_bool_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }

    case 2:
        //test_bool_02
        testsOkay = errLog(test_bool_02(), "test_bool_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3:
        //test_bool_03
        testsOkay = errLog(test_bool_03(), "test_bool_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }

    case 4:
        //test_bool_04
        testsOkay = errLog(test_bool_04(), "test_bool_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }
    
    case 5:
        //test_bool_05
        testsOkay = errLog(test_bool_05(), "test_bool_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }
    
    case 6:
        //test_bool_06
        testsOkay = errLog(test_bool_06(), "test_bool_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_bool_07
        testsOkay = errLog(test_bool_07(), "test_bool_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }

    case 8:
        //test_bool_08
        testsOkay = errLog(test_bool_08(), "test_bool_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }
    }
    return testsOkay;
}