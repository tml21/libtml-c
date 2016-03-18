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
#include <tchar.h>
#endif // LINUX
#include <sidex.h>
#include "sdxrt_tchar.h"
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

SIDEX_INT32 strErr = SIDEX_SUCCESS; // API return value

//is used in different functions
#ifdef SIDEX_UNICODE
    SIDEX_TCHAR* bigStr = new wchar_t[131072]();
#else
    SIDEX_TCHAR* bigStr = new char[131072]();
#endif

SIDEX_INT64 bigSize = 131071;

SIDEX_VARIANT strHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_HANDLE docStrHandle = SIDEX_HANDLE_TYPE_NULL;


/** @ingroup Sidex_String_Tests
* @brief Checks sidex_Variant_New_String with a small (" ") value of string
*
* A sidex_document is created and the small string-value (" ") is assigned to a sidex_variant. The sidex_variant is written to the sidex_document with sidex_Variant_Write.
* The sidex_variant is read again with sidex_Variant_Read and checked, if it is a string and if it contains the same value as before (" ").
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_01() {
    bool testOkay = true;
    SIDEX_TCHAR* tempString = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 tempLength = -1; 

    //normal sidex_Create
    strErr = sidex_Create(DOCUMENT, &docStrHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_01", "sidex_Create", 1);
    //write into sidex_document
    strErr = sidex_String_Write(docStrHandle, GROUP, KEY, minStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_01", "sidex_String_Write", 1);
    //read again
    strErr = sidex_String_Read(docStrHandle, GROUP, KEY, &tempString, &tempLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_01", "sidex_String_Read", 1);
    //compare string to original string
    testOkay = testOkay && errLog(sdxrt_cmp(minStr, tempString) == 0, "test_string_01", "stringvalue != stringvalue", 1);
    sidex_Free_ReadString(tempString);
    sidex_Free(&docStrHandle);
    return testOkay;
}

/** @ingroup Sidex_String_Tests
* @brief Checks sidex_Variant_New_String for a big string-value
*
* A sidex_variant is created and assigned to the max size of a sidex_string. It is written to a newly created sidex_document and read again afterwards. 
* It is checked if the value from the sidex_document is the same as before- the big size string value.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_02() {
    bool testOkay = true;
    SIDEX_INT64 strSize = 0;
    SIDEX_VARIANT tempHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_TCHAR* tempString = NULL;
    SIDEX_INT32 tempLength = -1;

    //normal sidex_Create
    strErr = sidex_Create(DOCUMENT, &docStrHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_02", "sidex_Create", 1);
    //loop for generating a max-size string
    while (strSize < bigSize-8) {
        sdxrt_cat(bigStr, _sdxrtT("wwwwwwww")); //bigSize = 8*16384, so there are 8 'w's as a'shortcut' (one less, because its a string with '\0')
        strSize = strSize + 8;
    }
    sdxrt_cat(bigStr, _sdxrtT("wwwwwww"));

    strErr = sidex_Variant_New_String(bigStr, &strHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_02", "sidex_Variant_New_String", 1);
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_String_Check(strHandle), 
                                  "test_string_02", "sidex_Variant_String_Check", 1);
    //write as variant to sidex_document
    strErr = sidex_Variant_Write(docStrHandle, GROUP, KEY, strHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_02", "sidex_Variant_Write", 1);
    //read again as variant
    strErr = sidex_Variant_Read(docStrHandle, GROUP, KEY, &tempHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_02", "sidex_Variant_Read", 1);
    //helper variables
    //transform variant to string
    strErr = sidex_Variant_As_String(tempHandle, &tempString, &tempLength); 
    //compare string to original string
    testOkay = testOkay && errLog(sdxrt_cmp(bigStr, tempString) == 0, "test_string_02", "stringvalue != stringvalue", 1);
    //compare the size of the string to the original
    testOkay = testOkay && errLog(tempLength == bigSize, "test_string_02", "length != stringlength", 1);
    sidex_Variant_DecRef(tempHandle);

    sidex_Variant_DecRef(strHandle);
    sidex_Free(&docStrHandle);
    return testOkay;
}


/** @ingroup Sidex_String_Tests
*  @brief Checks sidex_Variant_New_String and sidex_Variant_String_Check, testing for errors
*
* First a new string variant is created with param value = NULL, throwing no error. The function sidex_Variant_String_Check is
* called with param value as a variant handling a sidex_int64- and returning a Sidex_False.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_03() {
    bool testOkay = true;
    SIDEX_VARIANT tempInt = SIDEX_HANDLE_TYPE_NULL;

    //check variant_new_String with param value as NULL
    strErr = sidex_Variant_New_String(NULL, &strHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_03", "sidex_Variant_New_String", 1);

    //check variant_String_Check with param value as integer-variant: so it has to return SIDEX_FALSE
    tempInt = sidex_Variant_New_Integer(12345);
    testOkay = testOkay && errLog(SIDEX_FALSE == sidex_Variant_String_Check(tempInt), "test_string_03", "sidex_Variant_String_Check", 1);

    sidex_Variant_DecRef(tempInt);
    sidex_Variant_DecRef(strHandle);
    return testOkay;
}


/** @ingroup Sidex_String_Tests
* @brief Checks sidex_String_Read, testing for errors
*
* A sidex_document is created and a valid string value and a non-string value (sidex_int) are written to it.
* Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_04() {
    bool testOkay = true;
    SIDEX_TCHAR* tempStr = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 strLength = 0;

    //normal sidex_create
    strErr = sidex_Create(DOCUMENT, &docStrHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_04", "sidex_Create", 1);
    //write a string entry to the sidex_document
    strErr = sidex_String_Write(docStrHandle, GROUP, KEY, KEY4);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_04", "sidex_String_Write", 1);
    //write an integer entry to the sidex_document
    strErr = sidex_Integer_Write(docStrHandle, GROUP, KEY2, 12345);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_04", "sidex_Integer_Write", 1);

    //check MISSING_GROUP- or UNICODE-error
    strErr = sidex_String_Read(docStrHandle, NULL, KEY, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == strErr || SIDEX_ERR_UNICODE == strErr, 
                                  "test_string_04", "MISSING_GROUP or UNICODE", 1);
    //check MISSING_KEY- or UNICODE-error
    strErr = sidex_String_Read(docStrHandle, GROUP, NULL, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == strErr || SIDEX_ERR_UNICODE == strErr, 
                                  "test_string_04", "MISSING_KEY or UNICODE", 1);
    //check WRONG_GROUP-error
    strErr = sidex_String_Read(docStrHandle, wGROUP, KEY, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == strErr, "test_string_04", "WRONG_GROUP", 1);
    //check WRONG_KEY-error
    strErr = sidex_String_Read(docStrHandle, GROUP, wKEY, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == strErr, "test_string_04", "WRONG_KEY", 1);
    //check WRONG_TYPE-error
    strErr = sidex_String_Read(docStrHandle, GROUP, KEY2, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == strErr, "test_string_04", "WRONG_TYPE", 1);
    //check NOCONTENT error
    strErr = sidex_String_Read(docStrHandle, GROUP2, KEY2, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == strErr, "test_string_04", "NOCONTENT", 1);

    sidex_Free(&docStrHandle);
    return testOkay;
}


/** @ingroup Sidex_String_Tests
* @brief Checks sidex_String_Write- testing for errors
*
* A sidex_document is created. Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_05() {
    bool testOkay = true;
    //normal sidex_create
    strErr = sidex_Create(DOCUMENT, &docStrHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_05", "sidex_Create", 1);
    //testing MISSING_KEY error or UNICODE
    strErr = sidex_String_Write(docStrHandle, GROUP, NULL, minStr);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == strErr || SIDEX_ERR_UNICODE == strErr, 
                                  "test_string_05", "MISSING_KEY or UNICODE", 1);
    //testing MISSING_GROUP error or UNICODE
    strErr = sidex_String_Write(docStrHandle, NULL, KEY, minStr);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == strErr || SIDEX_ERR_UNICODE == strErr, 
                                  "test_string_05", "MISSING_GROUP or UNICODE", 1);
    //testing WRONG_KEY error
    strErr = sidex_String_Write(docStrHandle, GROUP, wKEY, minStr);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == strErr, "test_string_05", "WRONG_KEY", 1);
    //testing WRONG_GROUP error
    strErr = sidex_String_Write(docStrHandle, wGROUP, KEY, minStr);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == strErr, "test_string_05", "WRONG_GROUP", 1);

    sidex_Free(&docStrHandle);
    return testOkay;
}

/** @ingroup Sidex_String_Tests
* @brief Checks sidex_String_Length- testing for errors
*
* A sidex_Document is created and a small string-, a 'normal-big' string- and a big-string-entry, as well as an integer-entry are written to the sidex_document.
* The length of the string entries (small, normal big and big) are read and compared to the original length of the strings.
* Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_06() {
    bool testOkay = true;
    SIDEX_TCHAR* normStr = _sdxrtT("One morning, when Gregor Samsa woke from troubled dreams, he found himself transformed in his bed into a horrible vermin.");
    SIDEX_INT32 strLength = 0;

    //normal sidex_create
    strErr = sidex_Create(DOCUMENT, &docStrHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_06", "sidex_Create", 1);
    //write three string entries to the sidex_document
    strErr = sidex_String_Write(docStrHandle, GROUP, KEY, minStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_06", "sidex_String_Write", 1);
    strErr = sidex_String_Write(docStrHandle, GROUP, KEY2, bigStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_06", "sidex_String_Write", 2);
    strErr = sidex_String_Write(docStrHandle, GROUP, KEY3, normStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_06", "sidex_String_Write", 3);

    //write an integer entry to the sidex_document
    strErr = sidex_Integer_Write(docStrHandle, GROUP2, KEY, 12345);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_06", "sidex_Integer_Write", 1);

    //check if the length is right(1) for minStr
    strErr = sidex_String_Length(docStrHandle, GROUP, KEY, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_06", "sidex_String_Length", 1);
    testOkay = testOkay && errLog(strLength == sdxrt_len(minStr), "test_string_06", "Length != Length", 1);
    //check if the length is right(2) for bigStr
    strErr = sidex_String_Length(docStrHandle, GROUP, KEY2, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_06", "sidex_String_Length", 2);
    testOkay = testOkay && errLog(bigSize == strLength, "test_string_06", "Length != Length", 2);
    //check if the length is right(3) for normStr
    strErr = sidex_String_Length(docStrHandle, GROUP, KEY3, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_06", "sidex_String_Length", 3);
    testOkay = testOkay && errLog(strLength == sdxrt_len(normStr), "test_string_06", "Length != Length", 3);

    //tests MISSING_GROUP or UNICODE
    strErr = sidex_String_Length(docStrHandle, NULL, KEY, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == strErr || SIDEX_ERR_UNICODE == strErr, 
                                  "test_string_06", "MISSING_GROUP or UNICODE", 1);
    //tests MISSING_KEY or UNICODE
    strErr = sidex_String_Length(docStrHandle, GROUP, NULL, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == strErr || SIDEX_ERR_UNICODE == strErr, 
                                  "test_string_06", "MISSING_KEY or UNICODE", 1);
    //tests WRONG_GROUP
    strErr = sidex_String_Length(docStrHandle, wGROUP, KEY, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == strErr, "test_string_06", "WRONG_GROUP", 1);
    //tests WRONG_KEY
    strErr = sidex_String_Length(docStrHandle, GROUP, wKEY, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == strErr, "test_string_06", "WRONG_KEY", 1);
    //tests WRONG_TYPE
    strErr = sidex_String_Length(docStrHandle, GROUP2, KEY, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == strErr, "test_string_06", "WRONG_TYPE", 1);
    //tests NOCONTENT
    strErr = sidex_String_Length(docStrHandle, GROUP2, KEY4, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == strErr, "test_string_06", "NOCONTENT", 1);

    sidex_Free(&docStrHandle);
    return testOkay;
}

/** @ingroup Sidex_String_Tests
* @brief Checks sidex_Variant_As_String, - also testing for an error
*
* Two string variants are created, as well as an integer variant- for testing an error later. 
* The string variants are transformed to strings and compared to their original strings.
* The function sidex_Variant_As_String is called with the integer-variant- testing a
* WRONG_TYPE error afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_07() {
    bool testOkay = true;
    SIDEX_VARIANT bigHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_TCHAR* tempStr = NULL;
    SIDEX_INT32 strLength = -1;

    //create string variants
    strErr = sidex_Variant_New_String(minStr, &strHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_07", "sidex_Variant_New_String", 1);
    strErr = sidex_Variant_New_String(bigStr, &bigHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_07", "sidex_Variant_New_String", 2);
    //create another variant, handling an integer
    intHandle = sidex_Variant_New_Integer(12345);

    //use sidex_Variant_As_String to translate the entries to strings, starting with minStr(strHandle)
    strErr = sidex_Variant_As_String(strHandle, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_07", "sidex_Variant_As_String", 1);
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, minStr) == 0, "test_string_07", "string != string", 1);
    strErr = sidex_Variant_As_String(bigHandle, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_07", "sidex_Variant_As_String", 2);
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, bigStr) == 0, "test_string_07", "string != string", 2);
    //WRONG_TYPE error is tested
    strErr = sidex_Variant_As_String(intHandle, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == strErr, "test_string_07", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(strHandle);
    sidex_Variant_DecRef(bigHandle);
    return testOkay;
}

/** @ingroup Sidex_String_Tests
* @brief Checks sidex_Variant_As_String_Length, - also testing for an error
*
* Two string variants are created, as well as an integer variant. 
* The function sidex_Variant_As_String_Length is called with the two string variants to get their lengths 
* and they are compared to their original length. The function sidex_Variant_As_String is called 
* with the integer-variant- testing a WRONG_TYPE error afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_08() {
    bool testOkay = true;
    SIDEX_VARIANT bigHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 strLength = -1;

    //create string variants
    strErr = sidex_Variant_New_String(minStr, &strHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_08", "sidex_Variant_New_String", 1);
    strErr = sidex_Variant_New_String(bigStr, &bigHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_08", "sidex_Variant_New_String", 2);
    //create another variant, handling an integer
    intHandle = sidex_Variant_New_Integer(12345);

    //use sidex_Variant_As_String_Length to check string length of the string variants, starting with minStr(strHandle)
    strErr = sidex_Variant_As_String_Length(strHandle, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_08", "sidex_Variant_As_String_Length", 1);
    testOkay = testOkay && errLog(sdxrt_len(minStr) == strLength, "test_string_08", "length != length", 1);

    strErr = sidex_Variant_As_String_Length(bigHandle, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_08", "sidex_Variant_As_String_Length", 2);
    testOkay = testOkay && errLog(bigSize == strLength, "test_string_08", "length != length", 2);

    //test for WRONG_TYPE error
    strErr = sidex_Variant_As_String_Length(intHandle, &strLength);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == strErr, "test_string_08", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(strHandle);
    sidex_Variant_DecRef(bigHandle);
    return testOkay;
}

/** @ingroup Sidex_String_Tests
* @brief Checks sidex_Variant_SetFormat and _GetFormat,- also testing for an error
*
* A string variant and an integer variant are created. The string format of the string variant is fetched
* and compared to the format it should have, which is 'unknown' by default. Afterwards the format is set and fetched again 
* and the formats are compared with each other (in order: sidex, xml, json, cvs, ini). The functions
* _SetFormat and _GetFormat are tested for errors: WRONG_TYPE and only _SetFormat for WRONG_ENCODING_FORMAT.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_string_09() {
    bool testOkay = true;
    SIDEX_VARIANT intHandle = SIDEX_HANDLE_TYPE_NULL;
    const SIDEX_TCHAR* tempFormat = SIDEX_HANDLE_TYPE_NULL;

    //creating a string variant
    strErr = sidex_Variant_New_String(bigStr, &strHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_New_String", 1);
    //create another variant, handling an integer
    intHandle = sidex_Variant_New_Integer(12345);

    //getFormat of string variant, it should be by default "unknown"
    strErr = sidex_Variant_String_GetFormat(strHandle, &tempFormat);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_String_GetFormat", 1);
    testOkay = testOkay && errLog(sdxrt_cmp(_sdxrtT("unknown"), tempFormat) == 0, "test_string_09", "format != format", 1);
    //-----setting format of the string variant to 'sidex'
    strErr = sidex_Variant_String_SetFormat(strHandle, _sdxrtT("sidex"));
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_SetFormat", 2);
    //compare formats
    strErr = sidex_Variant_String_GetFormat( strHandle, &tempFormat);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_String_GetFormat", 2);
    testOkay = testOkay && errLog(sdxrt_cmp(_sdxrtT("sidex"), tempFormat) == 0, "test_string_09", "format != format", 2);

    //-----change the format to 'xml'
    strErr = sidex_Variant_String_SetFormat(strHandle, _sdxrtT("xml"));
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_SetFormat", 3);
    //compare formats
    strErr = sidex_Variant_String_GetFormat( strHandle, &tempFormat);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_String_GetFormat", 3);
    testOkay = testOkay && errLog(sdxrt_cmp(_sdxrtT("xml"), tempFormat) == 0, "test_string_09", "format != format", 3);

    //------change the format to 'json'
    strErr = sidex_Variant_String_SetFormat(strHandle, _sdxrtT("json"));
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_SetFormat", 4);
    //compare formats
    strErr = sidex_Variant_String_GetFormat( strHandle, &tempFormat);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_String_GetFormat", 4);
    testOkay = testOkay && errLog(sdxrt_cmp(_sdxrtT("json"), tempFormat) == 0, "test_string_09", "format != format", 4);

    //------change the format to 'cvs'
    strErr = sidex_Variant_String_SetFormat(strHandle, _sdxrtT("cvs"));
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_SetFormat", 5);
    //compare formats
    strErr = sidex_Variant_String_GetFormat( strHandle, &tempFormat);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_String_GetFormat", 5);
    testOkay = testOkay && errLog(sdxrt_cmp(_sdxrtT("cvs"), tempFormat) == 0, "test_string_09", "format != format", 5);

    //------change the format to 'ini'
    strErr = sidex_Variant_String_SetFormat(strHandle, _sdxrtT("ini"));
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_SetFormat", 6);
    //compare formats
    strErr = sidex_Variant_String_GetFormat( strHandle, &tempFormat);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == strErr, "test_string_09", "sidex_Variant_String_GetFormat", 6);
    testOkay = testOkay && errLog(sdxrt_cmp(_sdxrtT("ini"), tempFormat) == 0, "test_string_09", "format != format", 6);
    //--------testing for errors
    //setFormat of an integer variant- testing for WRONG_TYPE error
    strErr = sidex_Variant_String_SetFormat(intHandle, _sdxrtT("unknown"));
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == strErr, "test_string_09", "WRONG_TYPE", 1);

    //setFormat of the string variant with an invalid format
    strErr = sidex_Variant_String_SetFormat(strHandle, _sdxrtT("tml"));
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_ENCODING_FORMAT == strErr, "test_string_09", "WRONG_ENCODING_FORMAT", 1);

    //getFormat of an integer variant- testing for WRONG_TYPE error
    strErr = sidex_Variant_String_GetFormat(intHandle, &tempFormat);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == strErr, "test_string_09", "WRONG_TYPE", 2);

    sidex_Variant_DecRef(intHandle);
    sidex_Variant_DecRef(strHandle);
    return testOkay;
}



/** @ingroup Sidex_String_Tests
* @brief Calls all subtests
* 
* @param int test: If 0- all subtests are executed, if 1-9- that one subtest is executed, returns false otherwise
* @param bool stop: if true- when a subtest fails, the test returns immediately. If false- all subtests are executed.
*
* Different functions are tested, see test_string_01-test_string_09 for more details.
*
* @returns bool testsOkay : false on failure of subtest, true on success.
*/
bool test_sidex_string(int test, bool stop) {
    bool testsOkay = true;

    switch(test) {
    case 0:
    case 1:
        //test_string_01
        testsOkay = errLog(test_string_01(), "test_string_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }
    
    case 2:
        //test_string_02
        testsOkay = errLog(test_string_02(), "test_string_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3:
        //test_string_03
        testsOkay = errLog(test_string_03(), "test_string_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }

    case 4:
        //test_string_04
        testsOkay = errLog(test_string_04(), "test_string_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }
    
    case 5:
        //test_string_05
        testsOkay = errLog(test_string_05(), "test_string_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }

    case 6:
        //test_string_06
        testsOkay = errLog(test_string_06(), "test_string_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_string_07
        testsOkay = errLog(test_string_07(), "test_string_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }

    case 8:
        //test_string_08
        testsOkay = errLog(test_string_08(), "test_string_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }
    case 9:
        //test_string_09
        testsOkay = errLog(test_string_09(), "test_string_09") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 9) {
            break;
        }
    }
    delete[] bigStr;
    return testsOkay;
}
