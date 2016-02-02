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
  #include <string.h>
#endif // LINUX
#include <sidex.h>
#include "sdxrt_tchar.h"
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

SIDEX_INT32 listErr      = SIDEX_SUCCESS; // API return value

SIDEX_HANDLE docListHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_VARIANT listHandle = SIDEX_HANDLE_TYPE_NULL;

/** @ingroup Sidex_List_Tests
* @brief Checks sidex_Variant_New_List, writing it with entries to a sidex_document
*
* A new list is created and assigned to a sidex_variant and a sidex_binary and 
* a sidex_integer are added to the list. It is checked, whether it is a sidex_list 
* and written to a recently created sidex_document. The list is read again 
* and checked whether it stayed the same as before being written to the sidex_document.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_01() {
    bool testOkay = true;
    SIDEX_INT32 binLength = 3;
    SIDEX_VARIANT binHandle = SIDEX_HANDLE_TYPE_NULL;
    unsigned char maxChar = 255;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 binIndex = -1;
    SIDEX_VARIANT tempList = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = -1;
    SIDEX_VARIANT tempVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64 tempInt = -1;
    unsigned char* tempBin = NULL;

    //normal creating of a list
    listHandle = sidex_Variant_New_List();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_List_Check(listHandle), "test_list_01", "sidex_Variant_List_Check", 1);
    //normal sidex_Create
    listErr = sidex_Create(DOCUMENT, &docListHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_Create", 1);

    //normal creating of a sidex_binary
    listErr = sidex_Variant_New_Binary(&maxChar, binLength, &binHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "New_Binary", 1);

    //normal creating of a sidex_variant, handling an integer
    intHandle = sidex_Variant_New_Integer(-1234567);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Integer_Check(intHandle), "test_list_01", "sidex_Variant_Integer_Check", 1);

    //append the created sidex_variants to the list
    listErr = sidex_Variant_List_Append(listHandle, binHandle, &binIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_Variant_List_Append", 1);
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_Variant_List_Append", 2);

    //write the list to the sidex_document
    listErr = sidex_List_Write(docListHandle, GROUP, KEY, listHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_List_Write", 1);

    //read the list again
    listErr = sidex_List_Read(docListHandle, GROUP, KEY, &tempList);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_List_Read", 1);

    //compare the 'after'-list entries with the 'before'-list entries
    //starting with the sidex_integer
    listErr = sidex_Variant_List_Get(tempList, intIndex, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_List_Get", 1);
    listErr = sidex_Variant_As_Integer(tempVariant,&tempInt);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_Variant_As_Integer", 1);
    testOkay = testOkay && errLog(-1234567 == tempInt, "test_list_01", "tempInt != value", 1);  //compare the original to the value

    //next compare the sidex_binary
    listErr = sidex_Variant_List_Get(tempList, binIndex, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_List_Get", 2);
    listErr = sidex_Variant_As_Binary(tempVariant,&tempBin, &binLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_01", "sidex_Variant_As_Binary", 1);
    testOkay = testOkay && errLog(255 == *tempBin, "test_list_01", "tempBin != value", 2);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(binHandle);
    sidex_Variant_DecRef(listHandle);
    sidex_Free(&docListHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks expected behaviour of sidex_Variant_List_Size
*
* A sidex_variant is set to SIDEX_HANDLE_TYPE_NULL and its size is checked, testing an error. 
* A new list is created and assigned to the sidex_Variant. 
* A sidex_integer is appended and the size of the list is checked. An error is tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_02() {
    bool testOkay = true;
    listHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 listSize = -1;
    SIDEX_INT32 intIndex = -1;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;

    //testing a NOCONTENT error
    listErr = sidex_Variant_List_Size(listHandle, &listSize);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == listErr, "test_list_02", "NOCONTENT", 1);

    //normal new list
    listHandle = sidex_Variant_New_List();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_List_Check(listHandle), "test_list_02", "sidex_Variant_List_Check", 1);

    //creating a sidex_integer (in a variant) and appending it to the list
    intHandle = sidex_Variant_New_Integer(1234567);
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_02", "sidex_Variant_List_Append", 1);

    //normal sidex_Variant_List_Size
    listErr = sidex_Variant_List_Size(listHandle, &listSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_02", "sidex_Variant_List_Size", 1);
    testOkay = testOkay && errLog(1 == listSize, "test_list_02", "listSize != size", 1);

    //testing a WRONG_TYPE error
    listErr = sidex_Variant_List_Size(intHandle, &listSize);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_02", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(listHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks sidex_Variant_List_Set for expected behaviour, also testing for errors
*
* A sidex_variant containing a sidex_Integer is created and with it as param, an
* error is tested in sidex_Variant_List_Set. A list is created, an integer(a) is appended and
* another integer(b) is set at the position of a. Afterwards b is gotten from the list and compared whether it
* really is integer b. Then errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_03() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    listHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = 0;
    SIDEX_VARIANT tempHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64 intValue = -1;

    //creating a sidex_integer (in a variant)
    intHandle = sidex_Variant_New_Integer(1234567);

    //testing a WRONG_TYPE error
    listErr = sidex_Variant_List_Set(intHandle, listHandle, intIndex);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_03", "WRONG_TYPE", 1);

    //normal new list
    listHandle = sidex_Variant_New_List();
    
    //append the integer sidex_variant
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_03", "sidex_Variant_List_Append", 1);

    //set the entry anew with another sidex_integer
    sidex_Variant_DecRef(intHandle);
    intHandle = sidex_Variant_New_Integer(-1234567);
    listErr = sidex_Variant_List_Set(listHandle, intHandle, intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_03", "sidex_Variant_List_Set", 1);

    //get the just set entry and compare the value
    listErr = sidex_Variant_List_Get(listHandle, intIndex, &tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_03", "sidex_Variant_List_Get", 1);
    listErr = sidex_Variant_As_Integer(tempHandle, &intValue);  //get the integer value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_03", "sidex_Variant_As_Integer", 1); //output
    testOkay = testOkay && errLog(-1234567 == intValue, "test_list_03", "intValue != value", 1);  //compare to 'new' integer value, that was set

    //testing a WRONG_PARAMETER error
    listErr = sidex_Variant_List_Set(listHandle, intHandle, -1);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == listErr, "test_list_03", "WRONG_PARAMETER", 1);

    //testing a NOCONTENT error
    listErr = sidex_Variant_List_Set(listHandle, intHandle, 5);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == listErr, "test_list_03", "NOCONTENT", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(listHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks sidex_Variant_List_Insert for expected behaviour, also testing for errors
*
* A sidex_variant containing a sidex_integer is created and an error is tested.
* A new list is created and we insert the sidex_integer to the list of position 5.
* It is gotten back from the list, comparing its value to the original value.
* Another error is tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_04() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    listHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = 0;
    SIDEX_VARIANT tempEntry = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64 intValue = -1;

    //creating a sidex_integer, in a variant
    intHandle = sidex_Variant_New_Integer(1234567);

    //test a WRONG_TYPE error
    listErr = sidex_Variant_List_Insert(intHandle, listHandle, intIndex);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_04", "WRONG_TYPE", 1);

    //normal new list
    listHandle = sidex_Variant_New_List();

    //append two dummy float entries
    tempEntry = sidex_Variant_New_Float(12.34567);
    listErr = sidex_Variant_List_Append(listHandle, tempEntry, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_04", "sidex_Variant_List_Append", 1);

    listErr = sidex_Variant_List_Append(listHandle, tempEntry, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_04", "sidex_Variant_List_Append", 2);

    //insert a sidex_variant, handling an integer
    intIndex = 1;
    listErr = sidex_Variant_List_Insert(listHandle, intHandle, intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_04", "sidex_Variant_List_Insert", 1);

    //get the integer from the list and compare them
    sidex_Variant_DecRef(intHandle);
    listErr = sidex_Variant_List_Get(listHandle, intIndex, &intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_04", "sidex_Variant_List_Get", 1);

    listErr = sidex_Variant_As_Integer(intHandle, &intValue);  //get the integer value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_04", "sidex_Variant_As_Integer", 1);
    testOkay = testOkay && errLog(1234567 == intValue, "test_list_04", "intValue != value", 1);

    //test a WRONG_PARAMETER error
    listErr = sidex_Variant_List_Insert(listHandle, intHandle, -1);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == listErr, "test_list_04", "WRONG_PARAMETER", 1);

    sidex_Variant_DecRef(tempEntry);
    sidex_Variant_DecRef(listHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks sidex_Variant_List_Get for expected behavior, also testing for errors
*
* A sidex_variant containing a sidex_integer is created and given as param for sidex_Variant_List_Get, 
* testing an error. A new list is created and we append the sidex_integer to the list.
* The sidex_integer is gotten back from the list, comparing its value to the original value.
* An error is tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_05() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    listHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = 0;
    SIDEX_INT64 intValue = -1;

    //creating a sidex_integer, in a variant
    intHandle = sidex_Variant_New_Integer(1234567);

    //testing a WRONG_TYPE error
    listErr = sidex_Variant_List_Get(intHandle, intIndex, &listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_05", "WRONG_TYPE", 1);

    //normal new list
    listHandle = sidex_Variant_New_List();

    //append the integer to the list
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_05", "sidex_Variant_List_Append", 1);

    //get the sidex_integer entry from the list
    sidex_Variant_DecRef(intHandle);
    listErr = sidex_Variant_List_Get(listHandle, intIndex, &intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_05", "sidex_Variant_List_Get", 1);

    listErr = sidex_Variant_As_Integer(intHandle, &intValue);  //get the integer value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_05", "sidex_Variant_As_Integer", 1);
    testOkay = testOkay && errLog(1234567 == intValue, "test_list_05", "intValue != value", 1); //compare it to original value

    //testing NOCONTENT error
    listErr = sidex_Variant_List_Get(listHandle, -1, &intHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == listErr, "test_list_05", "NOCONTENT", 1);

    //testing NOCONTENT error
    listErr = sidex_Variant_List_Get(listHandle, 5, &intHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == listErr, "test_list_05", "NOCONTENT", 2);

    sidex_Variant_DecRef(listHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks sidex_Variant_List_DeleteItem for expected behavior, also testing errors
*
* A sidex_variant containing a sidex_integer is created and given as param to sidex_Variant_List_DeleteItem, 
* testing an error. A new list is created and we append the sidex_integer to the list.
* The sidex_integer is deleted with sidex_Variant_List_DeleteItem and checked whether the list really is empty.
* Errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_06() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    listHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = 0;
    SIDEX_INT32 listSize = -1;

    //creating a sidex_integer (in a variant)
    intHandle = sidex_Variant_New_Integer(1234567);

    //testing a WRONG_TYPE error
    listErr = sidex_Variant_List_DeleteItem(intHandle, intIndex);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_06", "WRONG_TYPE", 1);

    //normal new list
    listHandle = sidex_Variant_New_List();

    //append the sidex_Integer to the list
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_06", "sidex_Variant_List_Append", 1);

    //normale deleteItem
    listErr = sidex_Variant_List_DeleteItem(listHandle, intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_06", "sidex_Variant_List_Append", 1);

    //the size of the list should be 0 now (empty)
    listErr = sidex_Variant_List_Size(listHandle, &listSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_06", "sidex_Variant_List_Size", 1);
    testOkay = testOkay && errLog(0 == listSize, "test_list_06", "listSize != size", 1);    //compare size

    //testing a NOCONTENT error
    listErr = sidex_Variant_List_DeleteItem(listHandle, intIndex);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == listErr, "test_list_06", "NOCONTENT", 1);

    //testing a WRONG_PARAMETER error
    listErr = sidex_Variant_List_DeleteItem(listHandle, -1);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == listErr, "test_list_06", "WRONG_PARAMETER", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(listHandle);
    return testOkay;
}

/** @ingroup Sidex_List_Tests
* @brief Checks sidex_Variant_List_Clear for expected behavior, also testing an error
*
* A sidex_variant containing a sidex_integer is created and given as param to sidex_Variant_List_Clear, 
* testing an error. A new list is created and the sidex_integer is appended to the list.
* The list is cleared afterwards and checked whether the list really is empty.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_07() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    listHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = 0;
    SIDEX_INT32 listSize = -1;

    //creating a sidex_integer, in a variant
    intHandle = sidex_Variant_New_Integer(1234567);

    //testing a WRONG_TYPE error
    listErr = sidex_Variant_List_Clear(intHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_07", "WRONG_TYPE", 1);

    //normal new list
    listHandle = sidex_Variant_New_List();

    //append the sidex_Integer to the list
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_07", "sidex_Variant_List_Append", 1);

    //normale deleteItem
    listErr = sidex_Variant_List_Clear(listHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_07", "sidex_Variant_List_DeleteItem", 1);

    //the size of the list should be 0 now (empty)
    listErr = sidex_Variant_List_Size(listHandle, &listSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_07", "sidex_Variant_List_Size", 1);
    testOkay = testOkay && errLog(0 == listSize, "test_list_07", "listSize != size", 1);    //compare size

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(listHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks sidex_Variant_List_Check for expected behavior
*
* A sidex_variant containing a sidex_integer and a new list are created.
* It is checked whether they are lists or not.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_08() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;

    //creating a sidex_integer, in a variant
    intHandle = sidex_Variant_New_Integer(1234567);

    //normal new list
    listHandle = sidex_Variant_New_List();

    //normal sidex_Variant_List_Check with a list
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_List_Check(listHandle), "test_list_08", "sidex_Variant_List_Check", 1);

    //check for SIDEX_FALSE
    testOkay = testOkay && errLog(SIDEX_FALSE == sidex_Variant_List_Check(intHandle), "test_list_08", "sidex_Variant_List_Check", 2);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(listHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks sidex_Variant_List_Append for expected behavior, also testing an error
*
* A sidex_variant containing a sidex_integer is created and with it an error is tested.
* A new list is created and we append the sidex_integer to the list, and two dummy entries.
* Afterwards we get the sidex_integer back from the list, comparing its value to the original value.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_09() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    listHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = 0;
    SIDEX_VARIANT tempEntry = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 tempIndex = -1;
    SIDEX_INT64 intValue = -1;

    //creating a sidex_integer (in a variant)
    intHandle = sidex_Variant_New_Integer(1234567);

    //testing a WRONG_TYPE error
    listErr = sidex_Variant_List_Append(intHandle, listHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_09", "WRONG_TYPE", 1);

    //normal new list
    listHandle = sidex_Variant_New_List();

    //append the sidex_integer to the list
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_09", "sidex_Variant_List_Append", 1);

    //append two dummy float entries to the list
    tempEntry = sidex_Variant_New_Float(12.34567);
    listErr = sidex_Variant_List_Append(listHandle, tempEntry, &tempIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_09", "sidex_Variant_List_Append", 2);

    listErr = sidex_Variant_List_Append(listHandle, tempEntry, &tempIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_09", "sidex_Variant_List_Append", 3);

    //get the sidex_integer entry and compare the value with the original one
    sidex_Variant_DecRef(intHandle);
    listErr = sidex_Variant_List_Get(listHandle, intIndex, &intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_09", "sidex_Variant_List_Get", 1);

    listErr = sidex_Variant_As_Integer(intHandle, &intValue);  //get the integer value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_09", "sidex_Variant_As_Integer", 1);
    testOkay = testOkay && errLog(1234567 == intValue, "test_list_09", "intValue != value", 1);   //compare it to original value

    sidex_Variant_DecRef(tempEntry);
    sidex_Variant_DecRef(listHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks sidex_List_Write, mostly testing errors
*
* A sidex_document is created and a list with one sidex_integer entry is written to.
* Different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_10() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = -1;

    //normal sidex_Create
    listErr = sidex_Create(DOCUMENT, &docListHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_10", "sidex_Create", 1);

    //creating a sidex_integer, in a variant
    intHandle = sidex_Variant_New_Integer(1234567);

    //normal new list
    listHandle = sidex_Variant_New_List();

    //append the sidex_integer to the list
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_10", "sidex_Variant_List_Append", 1);

    //write the list to the sidex_document
    listErr = sidex_List_Write(docListHandle, GROUP, KEY, listHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_10", "sidex_List_Write", 1);

    //testing a MISSING_KEY error or UNICODE
    listErr = sidex_List_Write(docListHandle, GROUP, NULL, listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == listErr || SIDEX_ERR_UNICODE == listErr,
                                  "test_list_10", "MISSING_KEY or UNICODE", 1);

    //testing a MISSING_GROUP error or UNICODE
    listErr = sidex_List_Write(docListHandle, NULL, KEY, listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == listErr || SIDEX_ERR_UNICODE == listErr, 
                                  "test_list_10", "MISSING_GROUP or UNICODE", 1);

    //testing a WRONG_KEY error
    listErr = sidex_List_Write(docListHandle, GROUP, wKEY, listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == listErr, "test_list_10", "WRONG_KEY", 1);

    //testing a WRONG_GROUP error
    listErr = sidex_List_Write(docListHandle, wGROUP, KEY, listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == listErr, "test_list_10", "WRONG_GROUP", 1);

    //testing a WRONG_TYPE error
    listErr = sidex_List_Write(docListHandle, GROUP, KEY2, intHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_10", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(listHandle);
    sidex_Variant_DecRef(intHandle);
    sidex_Free(&docListHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Checks sidex_List_Read, mostly testing errors
*
* A sidex_document is created and a list, with one sidex_integer entry is written to it.
* The list is read again, comparing its entry to the original one.
* Different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_list_11() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 intIndex = -1;
    SIDEX_INT64 intValue = -1;

    //normal sidex_Create
    listErr = sidex_Create(DOCUMENT, &docListHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_11", "sidex_Create", 1);

    //creating a sidex_integer, in a variant
    intHandle = sidex_Variant_New_Integer(1234567);

    //normal new list
    listHandle = sidex_Variant_New_List();

    //append the sidex_integer to the list
    listErr = sidex_Variant_List_Append(listHandle, intHandle, &intIndex);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_11", "sidex_Variant_List_Append", 1);

    //write the list to the sidex_document
    listErr = sidex_List_Write(docListHandle, GROUP, KEY, listHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_11", "sidex_List_Write", 1);

    //normal List_Read
    listErr = sidex_List_Read(docListHandle, GROUP, KEY, &listHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_11", "sidex_List_Read", 1);

    //get the sidex_integer entry and compare the value with the original one
    sidex_Variant_DecRef(intHandle);
    listErr = sidex_Variant_List_Get(listHandle, intIndex, &intHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_11", "sidex_Variant_List_Get", 1);

    listErr = sidex_Variant_As_Integer(intHandle, &intValue);  //get the integer value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == listErr, "test_list_11", "sidex_Variant_As_Integer", 1);

    testOkay = testOkay && errLog(1234567 == intValue, "test_list_11", "intValue != value", 1);   //compare it to original value

//-------normal read is done, the following tests only for errors

    sidex_Variant_DecRef(listHandle);
    //testing a MISSING_KEY error or UNICODE
    listErr = sidex_List_Read(docListHandle, GROUP, NULL, &listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == listErr || SIDEX_ERR_UNICODE == listErr, 
                                  "test_list_11", "MISSING_KEY or UNICODE", 1);

    //testing a MISSING_GROUP error or UNICODE
    listErr = sidex_List_Read(docListHandle, NULL, KEY, &listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == listErr || SIDEX_ERR_UNICODE == listErr, 
                                  "test_list_11", "MISSING_GROUP or UNICODE", 1);

    //testing a WRONG_KEY error
    listErr = sidex_List_Read(docListHandle, GROUP, wKEY, &listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == listErr, "test_list_11", "WRONG_KEY", 1);

    //testing a WRONG_GROUP error
    listErr = sidex_List_Read(docListHandle, wGROUP, KEY, &listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == listErr, "test_list_11", "WRONG_GROUP", 1);

    //testing a WRONG_TYPE error
    //first write a non-list entry into the sidex_document
    listErr = sidex_Boolean_Write(docListHandle, GROUP, KEY2, SIDEX_FALSE);
    listErr = sidex_List_Read(docListHandle, GROUP, KEY2, &listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == listErr, "test_list_11", "WRONG_TYPE", 1);

    //testing a NOCONTENT error
    listErr = sidex_List_Read(docListHandle, GROUP2, KEY, &listHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == listErr, "test_list_11", "NOCONTENT", 1);

    sidex_Free(&docListHandle);
    return testOkay;
}


/** @ingroup Sidex_List_Tests
* @brief Calls all subtests
* 
* @param int test: If 0- all subtests are executed, if 1-11- that one subtest is executed, returns false otherwise.
* @param bool stop: If true- when a subtest fails, the test returns immediately. If false- all subtests are executed.
* Different functions are tested, see test_list01-test_list11 for more details.
*
* @returns bool testsOkay : false on failure of subtest, true on success.
*/
bool test_sidex_list(int test, bool stop) {
    bool testsOkay = true;

    switch(test) {
    case 0:
    case 1:
        //test_list_01
        testsOkay = errLog(test_list_01(), "test_list_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }
    
    case 2:
        //test_list_02
        testsOkay = errLog(test_list_02(), "test_list_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3:
        //test_list_03
        testsOkay = errLog(test_list_03(), "test_list_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }

    case 4:
        //test_list_04
        testsOkay = errLog(test_list_04(), "test_list_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }
    
    case 5:
        //test_list_05
        testsOkay = errLog(test_list_05(), "test_list_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }

    case 6:
        //test_list_06
        testsOkay = errLog(test_list_06(), "test_list_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_list_07
        testsOkay = errLog(test_list_07(), "test_list_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }

    case 8:
        //test_list_08
        testsOkay = errLog(test_list_08(), "test_list_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }
    case 9:
        //test_list_09
        testsOkay = errLog(test_list_09(), "test_list_09") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 9) {
            break;
        }
    case 10:
        //test_list_10
        testsOkay = errLog(test_list_10(), "test_list_10") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 10) {
            break;
        }
    case 11:
        //test_list_11
        testsOkay = errLog(test_list_11(), "test_list_11") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 11) {
            break;
        }
    }
    return testsOkay;
}
