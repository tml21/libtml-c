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

#include <stdio.h>
#ifdef LINUX
  #include <wchar.h> 
  #include <limits>
#else 
  #ifdef SIDEX_UNICODE
    #define _UNICODE
    
  #endif
  #include <TCHAR.H>
#endif // LINUX
#include <sidex.h>
#include "sdxrt_tchar.h"
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

SIDEX_INT32 dictErr = SIDEX_SUCCESS; // API return value 

#ifdef LINUX    //depends on physical memory
    SIDEX_INT32 max32 = 33554;
#else 
    SIDEX_INT32 max32 = 3355443;
#endif

SIDEX_VARIANT dictHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_HANDLE docDictHandle = SIDEX_HANDLE_TYPE_NULL;

/** @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Variant_NewDict, sidex_Variant_New_DictBySize and sidex_Variant_Dict_Check
*
* A new dictionary is assigned to a sidex_variant. An integer gets assigned to the sidex_variant and it is checked, 
* whether the function sidex_Variant_Dict_Check really returns a false if the param sidex_variant does not handle a dict-object.
* A dict is assigned to the sidex_variant, with a predefined size. It is again checked if the 
* sidex_Variant really handles a valid dict and the size of the dict is compared. The same is done with another predefined size, 0.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_01() {
    bool testOkay = true;
    SIDEX_INT32 tempSize = -1;

    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();

    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_01", "sidex_Variant_Dict_Check", 1);

    //assigns an integer to handle
    sidex_Variant_DecRef(dictHandle);
    dictHandle = sidex_Variant_New_Integer(123456789);
    //testing for a SIDEX_FALSE
    testOkay = testOkay && errLog(SIDEX_FALSE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_01", "sidex_Variant_Dict_Check", 2);

    //assign dict with max size to sidex_variant
    sidex_Variant_DecRef(dictHandle);
    dictHandle = sidex_Variant_New_DictBySize(max32);
    //check if it is a valid dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_01", "sidex_Variant_Dict_Check", 3);

    //check size of the dict, its empty
    dictErr = sidex_Variant_Dict_Size(dictHandle,&tempSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_01", "sidex_Variant_Dict_Size", 1);
    //compare to 0
    testOkay = testOkay && errLog(tempSize == 0, "test_dict_01", "size != size", 1);

    //assign dict with min size to sidex_variant
    sidex_Variant_DecRef(dictHandle);
    dictHandle = sidex_Variant_New_DictBySize(0);
    //check if it is a valid dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_01", "sidex_Variant_Dict_Check", 4);
    //check size of the dict (with max size)
    dictErr = sidex_Variant_Dict_Size(dictHandle,&tempSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_01", "sidex_Variant_Dict_Size", 2);
    //compare to 0
    testOkay = testOkay && errLog(tempSize == 0, "test_dict_01", "size != size", 2);

    sidex_Variant_DecRef(dictHandle);
    return testOkay;
}


/**  @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Variant_Dict_Size, testing for errors
*
* A new dict is assigned to a sidex_variant and checked through sidex_Variant_Dict_Check. 
* The size of the dict, containing 0 keys, is checked. 
* An integer is put inside the dict and the size is checked again, now containing 1 key. 
* Different errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_02() {
    bool testOkay = true;
    SIDEX_VARIANT tempHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 tempSize = -1;

    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_02", "sidex_Variant_Dict_Check", 1);
    
    //check sidex_Variant_Dict_Size normally (1) (
    dictErr = sidex_Variant_Dict_Size(dictHandle, &tempSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_02", "sidex_Variant_Dict_Size", 1);
    //compare to 0
    testOkay = testOkay && errLog(tempSize == 0, "test_dict_02", "size != obj-size", 1);

    //normal writing to the dict
    tempHandle = sidex_Variant_New_Integer(-42);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY, tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_02", "sidex_Variant_Dict_Set", 1);

    //check sidex_Variant_Dict_Size normally (2) (
    dictErr = sidex_Variant_Dict_Size(dictHandle, &tempSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_02", "sidex_Variant_Dict_Size", 2);
    //compare to 1
    testOkay = testOkay && errLog(tempSize == 1, "test_dict_02", "size != obj-size", 2);

    //test for a NOCONTENT error
    sidex_Variant_DecRef(dictHandle);
    dictHandle = SIDEX_HANDLE_TYPE_NULL;
    dictErr = sidex_Variant_Dict_Size(dictHandle, &tempSize);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == dictErr, "test_dict_02", "NOCONTENT", 1);

    //tests for a WRONG_TYPE error, since tempHandle still handles the integer from before
    dictErr = sidex_Variant_Dict_Size(tempHandle, &tempSize);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dictErr, "test_dict_02", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(tempHandle);
    return testOkay;
}

/**  @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Variant_Dict_Set, also testing for errors
*
* A new dict is assigned to a sidex_variant and checked through sidex_Variant_Dict_Check. 
* An integer is assigned to a new sidex_variant and set into the dict. 
* Errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_03() {
    bool testOkay = true;
    SIDEX_VARIANT tempHandle = SIDEX_HANDLE_TYPE_NULL;

    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_03", "sidex_Variant_Dict_Check", 1);

    //normal writing to the dict(1)
    tempHandle = sidex_Variant_New_Integer(-42);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY, tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_03", "sidex_Variant_Dict_Set", 1);

    //testing for a WRONG_TYPE error(2)
    dictErr = sidex_Variant_Dict_Set(tempHandle, KEY, tempHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dictErr, "test_dict_03", "WRONG_TYPE", 1);

    //testing for a MISSING_KEY or UNICODE error (3)
    dictErr = sidex_Variant_Dict_Set(dictHandle, NULL, tempHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == dictErr || SIDEX_ERR_UNICODE == dictErr, 
                                  "test_dict_03", "MISSING_KEY or UNICODE", 1);

    sidex_Variant_DecRef(tempHandle);
    sidex_Variant_DecRef(dictHandle);
    return testOkay;
}


/**  @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Variant_Dict_First, also testing for errors
*
* The Errors NOCONTENT and WRONG_TYPE are tested. A new dict is assigned to a sidex_variant and checked 
* through sidex_Variant_Dict_Check.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_04() {
    bool testOkay = true;
    SIDEX_TCHAR* tempKey = NULL;
    SIDEX_VARIANT tempHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 tempSize = -1;

    //testing for a NOCONTENT error with an empty dict(1)
    dictHandle = SIDEX_HANDLE_TYPE_NULL;
    dictErr = sidex_Variant_Dict_First(dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == dictErr, "test_dict_04", "NOCONTENT", 1);

    //testing for a WRONG_TYPE error(2)
    dictHandle = sidex_Variant_New_Integer(-42);
    dictErr = sidex_Variant_Dict_First(dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dictErr, "test_dict_04", "WRONG_TYPE", 1);

    //assign dict to sidex_variant
    sidex_Variant_DecRef(dictHandle);
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_04", "sidex_Variant_Dict_Check", 1);

    //'initiliasizing' of the cursor, setting it to the first elem
    dictErr = sidex_Variant_Dict_First(dictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_First", 1);

    //write two elements to the dict
    tempHandle = sidex_Variant_New_Integer(-42);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY, tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_Set", 1);

    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY2, tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_Set", 2);

    //check if there are really two elements inside
    dictErr = sidex_Variant_Dict_Size(dictHandle, &tempSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_Size", 1);
    //compare to 2
    testOkay = testOkay && errLog(tempSize == 2, "test_dict_04", "size != obj-size", 1);

    sidex_Variant_DecRef(tempHandle);
    //first setting cursor on first
    dictErr = sidex_Variant_Dict_First(dictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_First", 2);

    //change cursor position with sidex_Variant_Dict_Next
    dictErr = sidex_Variant_Dict_Next(dictHandle, &tempKey, &tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_Next", 1);
    if(testOkay) {
        testOkay = testOkay && errLog(sdxrt_cmp(tempKey, KEY2) == 0, "test_dict_04", "key != tempKey", 1);
    }
    //checking the next element
    dictErr = sidex_Variant_Dict_Next(dictHandle, &tempKey, &tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_Next", 2);
    if(testOkay) {
        testOkay = testOkay && errLog(sdxrt_cmp(tempKey, KEY) == 0, "test_dict_04", "key != tempKey", 2);
    }

    //setting back to the first element
    dictErr = sidex_Variant_Dict_First(dictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_First", 2);

    //checking if it really is the first element
    dictErr = sidex_Variant_Dict_Next(dictHandle, &tempKey, &tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_04", "sidex_Variant_Dict_Next", 3);
    if(testOkay) {
        testOkay = testOkay && errLog(sdxrt_cmp(tempKey, KEY2) == 0, "test_dict_04", "key != tempKey", 3);
    }

    sidex_Variant_DecRef(dictHandle);
    return testOkay;
}

/**  @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Variant_Dict_Keys- also testing for errors.
*
* Two kinds of errors are tested, one before and one after a dictionary is created and assigned to dictHandle,
* also checking if it really is a sidex_dict.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_05() {
    bool testOkay = true;
    SIDEX_TCHAR* keyname = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 strLength = 0;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_VARIANT keysHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_HANDLE keynameHandle = SIDEX_HANDLE_TYPE_NULL;

    //testing for a NOCONTENT error
    dictHandle = SIDEX_HANDLE_TYPE_NULL;
    dictErr = sidex_Variant_Dict_Keys(dictHandle, &keysHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == dictErr, "test_dict_05", "NOCONTENT", 1);

    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), "test_dict_05", "sidex_Variant_Dict_Check", 1);

    //normal writing to the dict(1)
    intHandle = sidex_Variant_New_Integer(-42);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY, intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_05", "sidex_Variant_Dict_Set", 1);

    //normal dict_keys
    dictErr = sidex_Variant_Dict_Keys(dictHandle, &keysHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_05", "sidex_Variant_Dict_Keys", 1);

    dictErr = sidex_Variant_List_Get(keysHandle, 0, &keynameHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_05", "sidex_Variant_List_Get", 1);

    dictErr = sidex_Variant_As_String(keynameHandle, &keyname, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_05", "sidex_Variant_As_String", 1);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(KEY, keyname) == 0, "test_dict_05", "keyname != string", 1);

    //testing for a WRONG_TYPE error
    sidex_Variant_DecRef(keysHandle);
    dictErr = sidex_Variant_Dict_Keys(intHandle, &keysHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dictErr, "test_dict_05", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(dictHandle);
    return testOkay;
}


/** @ingroup Sidex_Dictionary_Tests
*  @brief Checks sidex_Variant_Dict_HasKey- also testing for errors
*
* Different kinds of errors are tested before and after a dictionary is created and assigned to dictHandle and 
* also checking if it really is a sidex_dict.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_06() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;

    // testing for a NOCONTENT error
    dictHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_BOOL hasKeyResult = SIDEX_TRUE;
    dictErr = sidex_Variant_Dict_HasKey(dictHandle, KEY, &hasKeyResult);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == dictErr, "test_dict_06", "sidex_Variant_Dict_HasKey", 1);
    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), "test_dict_06", 
                                  "sidex_Variant_Dict_Check", 1);

    //testing for a WRONG_PARAMETER or UNICODE error
    dictErr = sidex_Variant_Dict_HasKey(dictHandle, NULL, &hasKeyResult);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == dictErr || SIDEX_ERR_UNICODE == dictErr, 
                                  "test_dict_06", "WRONG_PARAMETER or UNICODE", 1);

    //normal writing to the dict(1)
    intHandle = sidex_Variant_New_Integer(-42);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY, intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_06", "sidex_Variant_Dict_Set", 1);

    //testing for a WRONG_TYPE error
    dictErr = sidex_Variant_Dict_HasKey(intHandle, KEY, &hasKeyResult);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dictErr, "test_dict_06", "WRONG_TYPE", 1);

    //normal dict_hasKey
    dictErr = sidex_Variant_Dict_HasKey(dictHandle, KEY, &hasKeyResult);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_06", "sidex_Variant_Dict_HasKey", 1);
    //compare 'return'-param
    testOkay = testOkay && errLog(SIDEX_TRUE == hasKeyResult, "test_dict_06", "sidex_Variant_Dict_HasKey-boolResult", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(dictHandle);
    return testOkay;
}

/**  @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Variant_Dict_Delete- also testing for errors
*
* A sidex_dict is created and assigned to dictHandle and checked whether it really is a sidex_dict.
* Different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_07() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;

    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_06", "sidex_Variant_Dict_Check", 1);

    //testing for the NOCONTENT error
    dictErr = sidex_Variant_Dict_Delete(dictHandle, KEY);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == dictErr, "test_dict_07", "NOCONTENT", 1);

    //testing for a NOCONTENT or UNICODE error
    dictErr = sidex_Variant_Dict_Delete(dictHandle, NULL);
    testOkay = testOkay && errLog(SIDEX_ERR_UNICODE == dictErr || SIDEX_ERR_NOCONTENT == dictErr, 
                                  "test_dict_07", "NOCONTENT or UNICODE", 1);

    //normal writing to the dict
    intHandle = sidex_Variant_New_Integer(-42);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY, intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_07", "sidex_Variant_Dict_Set", 1);

    //normal dict_delete
    dictErr = sidex_Variant_Dict_Delete(dictHandle, KEY);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_07", "sidex_Variant_Dict_Delete", 1);

    //checking whether the entry wiht key = KEY in sidex_dict was deleted
    sidex_Variant_DecRef(intHandle);
    dictErr = sidex_Variant_Dict_Get(dictHandle, KEY, &intHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == dictErr, "test_dict_07", "NOCONTENT", 2);

    sidex_Variant_DecRef(dictHandle);
    return testOkay;
}


/**  @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Variant_Dict_Clear- also testing for an error
*
* A new sidex_dict is created and two key/sidex_int pairs are set. The size of the dict is checked.
* Afterwards an error is tested with param sDict as sidex_variant handling an integer.
* The dict is cleared and it is checked whether it really is empty.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_08() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 dictSize = 0;

    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), "test_dict_08", "sidex_Variant_Dict_Check", 1);

    //normal writing to the dict(1)
    intHandle = sidex_Variant_New_Integer(-42);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY, intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_08", "sidex_Variant_Dict_Set", 1);

    //normal writing to the dict(2)
    sidex_Variant_DecRef(intHandle);
    intHandle = sidex_Variant_New_Integer(1234567);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY2, intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_08", "sidex_Variant_Dict_Set", 2);

    //checking whether the size of the sidex_dict is 2
    dictErr = sidex_Variant_Dict_Size(dictHandle, &dictSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_08", "sidex_Variant_Dict_Size", 1);
    //compare size to 2 
    testOkay = testOkay && errLog(2 == dictSize, "test_dict_08", "dictSize != size", 1);

    //provoking the error WRONG_TYPE
    sidex_Variant_DecRef(intHandle);
    dictErr = sidex_Variant_Dict_Clear(intHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dictErr, "test_dict_08", "WRONG_TYPE", 1);

    //normal dict_clear
    dictErr = sidex_Variant_Dict_Clear(dictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_08", "sidex_Variant_Dict_Clear", 1);

    //checking whether the sidex_dict is empty
    dictErr = sidex_Variant_Dict_Size(dictHandle, &dictSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_08", "sidex_Variant_Dict_Size", 2);
    //compare to 0
    testOkay = testOkay && errLog(0 == dictSize, "test_dict_08", "dictSize != size", 2);

    sidex_Variant_DecRef(dictHandle);
    return testOkay;
}


/**  @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Dict_Write and sidex_Dict_Read
*
* A sidex_document is created, as well as a sidex_dict with two entries: a string and a float.
* They are written to the sidex_document and read right afterwards,
* and compared to their values before being written to the sidex_document.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_09() {
    bool testOkay = true;
    SIDEX_VARIANT strHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_VARIANT floatHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_TCHAR* entryVal = NULL;
    SIDEX_INT32 strLength = 0;
    SIDEX_DOUBLE floatEntry = 0;

    //normal sidex_Create
    dictErr = sidex_Create(DOCUMENT, &docDictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Create", 1);

    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), "test_dict_09", "sidex_Variant_Dict_Check", 1);

    //normal writing to the dict(1)
    dictErr = sidex_Variant_New_String(KEY, &strHandle);    //KEY =  value of the first entry
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Variant_New_String", 1);

    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY, strHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Variant_Dict_Set", 1);

    //normal writing to the dict(2)
    floatHandle = sidex_Variant_New_Float(123.4567);
    dictErr = sidex_Variant_Dict_Set(dictHandle, KEY2, floatHandle);  //123.4567 = value of the float entry
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Variant_Dict_Set", 2);

    //writing the sidex_dict into the sidex_doc
    dictErr = sidex_Dict_Write(docDictHandle, GROUP, KEY, dictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Dict_Write", 1);

    //reading the sidex_dict again
    sidex_Variant_DecRef(dictHandle);
    dictErr = sidex_Dict_Read(docDictHandle, GROUP, KEY, &dictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Dict_Read", 1);

    //comparing the dict-entries with the original ones, starting with the string entry
    SIDEX_VARIANT tempHandle = SIDEX_HANDLE_TYPE_NULL;
    dictErr = sidex_Variant_Dict_Get(dictHandle, KEY, &tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Dict_Get", 1);

    dictErr = sidex_Variant_As_String(tempHandle, &entryVal, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Variant_As_String", 1);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(KEY, entryVal) == 0, "test_dict_09", "entryVal != value", 1);

    //second entry, the float
    dictErr = sidex_Variant_Dict_Get(dictHandle, KEY2, &tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Dict_Get", 2);

    dictErr = sidex_Variant_As_Float(tempHandle, &floatEntry);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_09", "sidex_Variant_As_Float", 1);
    //compare to original
    testOkay = testOkay && errLog(123.4567 == floatEntry, "test_dict_09", "floatEntry != float", 1);

    sidex_Variant_DecRef(floatHandle);
    sidex_Variant_DecRef(strHandle);
    sidex_Variant_DecRef(dictHandle);
    sidex_Free(&docDictHandle);
    return testOkay;
}


/**  @ingroup Sidex_Dictionary_Tests
* @brief Checks sidex_Dict_Write, only testing for errors
*
* A sidex_document is created and a new sidex_dict is assigned to a sidex_variant.
* Then errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_10() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;

    //normal sidex_Create
    dictErr = sidex_Create(DOCUMENT, &docDictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_10", "sidex_Create", 1);

    //assign dict to sidex_variant
    dictHandle = sidex_Variant_New_Dict();
    //check whether it really handles a dict
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Dict_Check(dictHandle), 
                                  "test_dict_10", "sidex_Variant_Dict_Check", 1);

    //testing for error MISSING_KEY or UNICODE with param nkey = NULL
    dictErr = sidex_Dict_Write(docDictHandle, GROUP, NULL, dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == dictErr || SIDEX_ERR_UNICODE == dictErr, 
                                  "test_dict_10", "MISSING_KEY or UNICODE", 1);

    //testing for error MISSING_GROUP or UNICODE with param ngroup = NULL
    dictErr = sidex_Dict_Write(docDictHandle, NULL, KEY, dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == dictErr|| SIDEX_ERR_UNICODE == dictErr, 
                                  "test_dict_10", "MISSING_GROUP or UNICODE", 1);

    //testing for error WRONG_KEY with param nkey containing prohibited characters
    dictErr = sidex_Dict_Write(docDictHandle, GROUP, wKEY, dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == dictErr, "test_dict_10", "WRONG_KEY", 1);

    //testing for error WRONG_GROUP with param ngroup containing prohibited characters
    dictErr = sidex_Dict_Write(docDictHandle, wGROUP, KEY, dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == dictErr, "test_dict_10", "WRONG_GROUP", 1);

    //testing for error WRONG_TYPE with param value not handling a sidex_dict
    intHandle = sidex_Variant_New_Integer(1234567);
    dictErr = sidex_Dict_Write(docDictHandle, GROUP, KEY, intHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dictErr, "test_dict_10", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(dictHandle);
    sidex_Free(&docDictHandle);
    return testOkay;
}

/** @ingroup Sidex_Dictionary_Tests
*  @brief Checks sidex_Dict_Read, only testing for errors
*
* A sidex_document is created.
* Then a number of error return codes are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_dict_11() {
    bool testOkay = true;
    //normal sidex_Create
    dictErr = sidex_Create(DOCUMENT, &docDictHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dictErr, "test_dict_11", "sidex_Create", 1);

    //testing for error MISSING_KEY or UNICODE with param nkey = NULL
    dictErr = sidex_Dict_Read(docDictHandle, GROUP, NULL, &dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == dictErr || SIDEX_ERR_UNICODE == dictErr, 
                                  "test_dict_11", "MISSING_KEY or UNICODE", 1);

    //testing for error MISSING_GROUP or UNICODE with param ngroup = NULL
    dictErr = sidex_Dict_Read(docDictHandle, NULL, KEY, &dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == dictErr|| SIDEX_ERR_UNICODE == dictErr, 
                                  "test_dict_11", "MISSING_GROUP or UNICODE", 1);

    //testing for error WRONG_KEY with param nkey containing prohibited characters
    dictErr = sidex_Dict_Read(docDictHandle, GROUP, wKEY, &dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == dictErr, "test_dict_11", "WRONG_KEY", 1);

    //testing for error WRONG_GROUP with param ngroup containing prohibited characters
    dictErr = sidex_Dict_Read(docDictHandle, wGROUP, KEY, &dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == dictErr, "test_dict_11", "WRONG_GROUP", 1);

    //testing for error WRONG_TYPE with param value not handling a sidex_dict
    dictErr = sidex_Integer_Write(docDictHandle, GROUP, KEY, 1234567);
    dictErr = sidex_Dict_Read(docDictHandle, GROUP, KEY, &dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dictErr, "test_dict_11", "WRONG_TYPE", 1);

    //testing for error NOCONTENT, with ngroup and nkey pointing to a nonexistent entry
    dictErr = sidex_Dict_Read(docDictHandle, GROUP, KEY2, &dictHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == dictErr, "test_dict_11", "NOCONTENT", 1);

    sidex_Free(&docDictHandle);
    return testOkay;
}

/**  @ingroup Sidex_Dictionary_Tests
* @brief Calls all other subtests
* @param int test: If 0- execute all subtests. if 1-11- execute particular subtest.
* @param bool stop: If false- execute all subtests. If true- return immediately after a subtest fails.
* 
* For more details see documentation of subtests
* @returns bool : false on failure of test, true on success.
*/
bool test_sidex_dict(int test, bool stop) {
    bool testsOkay = true;
    switch(test) {
    case 0:
    case 1: 
        //test_dict_01
        testsOkay = errLog(test_dict_01(), "test_dict_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }

    case 2: 
        //test_dict_02
        testsOkay = errLog(test_dict_02(), "test_dict_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3: 
        //test_dict_03
        testsOkay = errLog(test_dict_03(), "test_dict_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }

    case 4: 
        //test_dict_04
        testsOkay = errLog(test_dict_04(), "test_dict_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }
    case 5: 
        //test_dict_05
        testsOkay = errLog(test_dict_05(), "test_dict_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }
    case 6: 
        //test_dict_06
        testsOkay = errLog(test_dict_06(), "test_dict_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }
    case 7: 
        //test_dict_07
        testsOkay = errLog(test_dict_07(), "test_dict_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }
    case 8: 
        //test_dict_08
        testsOkay = errLog(test_dict_08(), "test_dict_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }
    case 9: 
        //test_dict_09
        testsOkay = errLog(test_dict_09(), "test_dict_09") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 9) {
            break;
        }
    case 10: 
        //test_dict_10
        testsOkay = errLog(test_dict_10(), "test_dict_10") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 10) {
            break;
        }
    case 11: 
        //test_dict_11
        testsOkay = errLog(test_dict_11(), "test_dict_11") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 11) {
            break;
        }
    }
    return testsOkay;
}
