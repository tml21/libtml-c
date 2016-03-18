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
  #ifdef FREE_BSD
    #include <stdlib.h>
  #endif // FREE_BSD
#else 
  #ifdef SIDEX_UNICODE
    #define _UNICODE
  #endif // SIDEX_UNICODE
  #ifdef MINGW_BUILD
    #include <stdlib.h>
  #endif // MINGW_BUILD
#include <tchar.h>
#endif // LINUX
#include <sidex.h>
#include "sdxrt_tchar.h"
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

SIDEX_INT32 dateErr      = SIDEX_SUCCESS; // API return value 

SIDEX_VARIANT dateHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_HANDLE docDateHandle = SIDEX_HANDLE_TYPE_NULL;

/** @ingroup Sidex_DateTime_Tests
* @brief Helper function for generating a random dateTime in a valid format
*
* @returns SIDEX_TCHAR* date : contains a random dateTime in string-format.
*/
SIDEX_TCHAR* generate_DateTime() {
    SIDEX_INT32 nmbr = rand();//generates random number between 0 and at least 32767.
#ifdef SIDEX_UNICODE
    SIDEX_TCHAR* temp = new wchar_t[6]();
    SIDEX_TCHAR* date = new wchar_t[25]();
#else
    SIDEX_TCHAR* temp = new char[6]();
    SIDEX_TCHAR* date = new char[25]();
#endif
    bool dateOkay = true;

    //check whether one of the date-values (year, month or day) are 0
    if(nmbr%1000 == 0 || nmbr%13 == 0 || nmbr%32 == 0) {
        dateOkay = false;   //if one of them is 0- all of them must be 0 (in the right format)
    }
        //concatenate a dateTime in a valid format
    if(!dateOkay) {
        sdxrt_cat(date, _sdxrtT("0000"));
    } else if(999 < nmbr%10000) {
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%10000);
        sdxrt_cat(date, temp);
    } else if(99 < nmbr%10000 ) {
        sdxrt_cat(date, _sdxrtT("0"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%10000);
        sdxrt_cat(date, temp);
    } else if(9 < nmbr%10000) {
        sdxrt_cat(date, _sdxrtT("00"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%10000);
        sdxrt_cat(date, temp);
    } else {
        sdxrt_cat(date, _sdxrtT("000"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%10000);
        sdxrt_cat(date, temp);
    }
    sdxrt_cat(date, _sdxrtT("-"));
    if(!dateOkay) {
        sdxrt_cat(date, _sdxrtT("00"));
    } else if(9 < nmbr%13) {
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%13);
        sdxrt_cat(date, temp);
    } else {
        sdxrt_cat(date, _sdxrtT("0"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%13);
        sdxrt_cat(date, temp);
    }
    sdxrt_cat(date, _sdxrtT("-"));
    if(!dateOkay) {
        sdxrt_cat(date, _sdxrtT("00"));
    } else if( 9 < nmbr%32) {
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%32);
        sdxrt_cat(date, temp);
    } else {
        sdxrt_cat(date, _sdxrtT("0"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%32);
        sdxrt_cat(date, temp);
    }
    sdxrt_cat(date, _sdxrtT(" "));
    if(9 < nmbr%24) {
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%24);
        sdxrt_cat(date, temp);
    } else if( 0 < nmbr%24) {
        sdxrt_cat(date, _sdxrtT("0"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%24);
        sdxrt_cat(date, temp);
    } else {
        sdxrt_cat(date, _sdxrtT("00"));
    }
    sdxrt_cat(date, _sdxrtT(":"));
    if(9 < nmbr%60) {
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%60);
        sdxrt_cat(date, temp);
    } else if( 0 < nmbr%60) {
        sdxrt_cat(date, _sdxrtT("0"));
        sdxrt_printf(temp,_sdxrtT("%d"),nmbr%60);
        sdxrt_cat(date, temp);
    } else {
        sdxrt_cat(date, _sdxrtT("00"));
    } 
    sdxrt_cat(date, _sdxrtT(":"));
    if(9 < nmbr%60) {
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%60);
        sdxrt_cat(date, temp);
    } else if( 0 < nmbr%60) {
        sdxrt_cat(date, _sdxrtT("0"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%60);
        sdxrt_cat(date, temp);
    } else {
        sdxrt_cat(date, _sdxrtT("00"));
    }
    sdxrt_cat(date, _sdxrtT(":"));
    if(99 < nmbr%1000 ) {
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%1000);
        sdxrt_cat(date, temp);
    } else if(9 < nmbr%1000) {
        sdxrt_cat(date, _sdxrtT("0"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%1000);
        sdxrt_cat(date, temp);
    } else if(0 < nmbr%1000) {
        sdxrt_cat(date, _sdxrtT("00"));
        sdxrt_printf(temp, _sdxrtT("%d"),nmbr%1000);
        sdxrt_cat(date, temp);
    } else {
        sdxrt_cat(date, _sdxrtT("000"));
    }
    delete[] temp;
    return date;
}

/**  @ingroup Sidex_DateTime_Tests
* @brief Checks sidex_Variant_New_DateTime with min value of dateTime
*
* A sidex_document is created and the min datetime-value is assigned to a sidex_variant. 
* The sidex_variant is written to the sidex_document two times, with sidex_Variant_Write and sidex_DateTime_Write.
* The sidex_variant and dateTime-entry are read with sidex_Variant_Read and sidex_DateTime_Read
* and checked, whether they stayed the same.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_date_01() {
    bool testOkay = true;
    SIDEX_TCHAR* tempStr = NULL;
    SIDEX_VARIANT temp = SIDEX_HANDLE_TYPE_NULL;

    //normal sidex_create
    dateErr = sidex_Create(DOCUMENT, &docDateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_01", "sidex_Create", 1);

    //assign sidex_variant
    dateErr = sidex_Variant_New_DateTime(MINDATE, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_01", "sidex_Variant_New_DateTime", 1);

    //check if sidex_variant is really a dateTime
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_DateTime_Check(dateHandle),
                                  "test_date_01", "sidex_Variant_DateTime_Check", 1);

    //write sidex_variant to sidex_document
    dateErr = sidex_Variant_Write(docDateHandle, GROUP, KEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_01", "sidex_Variant_Write", 1);
    //read again
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_01", "sidex_DateTime_Read", 1);

    //check whether the entry really is a datetime
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_DateTime_Check(temp), 
                                  "test_date_01", "sidex_Variant_DateTime_Check", 2);
    dateErr = sidex_Variant_As_DateTime(temp, &tempStr);
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, MINDATE) == 0, "test_date_01", "entry != value", 1);


    //write dateTime to sidex_document
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP, KEY2, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_01", "sidex_DateTime_Write", 1);

    //read again
    sidex_Variant_DecRef(temp);
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP, KEY2, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_01", "sidex_DateTime_Read", 2);

    //check whether it really is a datetime
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_DateTime_Check(temp),
                                  "test_date_01", "sidex_Variant_DateTime_Check", 3);

    dateErr = sidex_Variant_As_DateTime(temp, &tempStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_01", "sidex_Variant_As_DateTime", 2);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, MINDATE) == 0, "test_date_01", "entry != value", 2);

    sidex_Variant_DecRef(temp);
    sidex_Variant_DecRef(dateHandle);
    sidex_Free(&docDateHandle);
    return testOkay;
}


/**  @ingroup Sidex_DateTime_Tests
* @brief Checks sidex_Variant_New_DateTime with max value of dateTime
*
* A sidex_document is created and the max datetime-value is assigned to a sidex_variant. 
* The sidex_variant is written to the sidex_document two times, with sidex_Variant_Write and sidex_DateTime_Write.
* The sidex_variant and dateTime-entry are read again with sidex_Variant_Read and sidex_DateTime_Read 
* and checked, if they stayed the same values.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_date_02() {
    bool testOkay = true;
    SIDEX_VARIANT temp = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_TCHAR* tempStr = NULL;

    dateErr = sidex_Create(DOCUMENT, &docDateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_02", "sidex_Create", 1);

    //assign sidex_variant
    dateErr = sidex_Variant_New_DateTime(MAXDATE, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_02", "sidex_Variant_New_DateTime", 1);

    //check if sidex_variant is really a dateTime
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_DateTime_Check(dateHandle), 
                                  "test_date_02", "sidex_Variant_DateTime_Check", 1);

    //write sidex_variant to sidex_document
    dateErr = sidex_Variant_Write(docDateHandle, GROUP, KEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_02", "sidex_Variant_Write", 1);
    
    //read the dateTime
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP, KEY, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_02", "sidex_DateTime_Read", 1);

    //check whether it really is a dateTime
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_DateTime_Check(temp), "test_date_02",
                                  "sidex_Variant_DateTime_Check", 2);

    dateErr = sidex_Variant_As_DateTime(temp, &tempStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_02", "sidex_Variant_As_DateTime", 1);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, MAXDATE) == 0, "test_date_01", "entry != value", 1);

    //write dateTime to sidex_document
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP, KEY2, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_02", "sidex_DateTime_Write", 1);
    
    sidex_Variant_DecRef(temp);
    //read the dateTime
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP, KEY2, &temp);

    //check whether the variant is a dateTime
    testOkay = testOkay && errLog(SIDEX_TRUE ==  sidex_Variant_DateTime_Check(temp), "test_date_02", 
                                  "sidex_Variant_New_DateTime", 1);

    dateErr = sidex_Variant_As_DateTime(temp, &tempStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_02", "sidex_Variant_As_DateTime", 2);
    testOkay = testOkay && errLog(0 == sdxrt_cmp(tempStr, MAXDATE), "test_date_02", "value != entry", 2);

    sidex_Variant_DecRef(temp);
    sidex_Variant_DecRef(dateHandle);
    sidex_Free(&docDateHandle);
    return testOkay;
}

/**  @ingroup Sidex_DateTime_Tests
* @brief Checks sidex_DateTime_Write, testing for errors
*
* A sidex_document is created and different inputs are tried to write in the sidex_document. First a normal call is done, throwing no error.
* Different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_date_03() {
    bool testOkay = true;
    //normal sidex_create
    dateErr = sidex_Create(DOCUMENT, &docDateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_03", "sidex_Create", 1);

    //assign sidex_variants
    dateErr = sidex_Variant_New_DateTime(MINDATE, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_03", "sidex_Variant_New_DateTime", 1);

    //write to sidex_document (normal)
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP, KEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_03", "sidex_DateTime_Write", 1);

    //DateTime_Write MISSING_KEY or UNICODE(2)
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP, NULL, dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == dateErr || SIDEX_ERR_UNICODE == dateErr, 
                                  "test_date_03", "MISSING_KEY or UNICODE", 1);

    //DateTime_Write MISSING_GROUP or UNICODE(3)
    dateErr = sidex_DateTime_Write(docDateHandle, NULL, KEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == dateErr || SIDEX_ERR_UNICODE == dateErr, 
                                  "test_date_03", "MISSING_GROUP or UNICODE", 1);

    //DateTime_Write WRONG_KEY(4)
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP, wKEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == dateErr, "test_date_03", "WRONG_KEY", 1);

    //DateTime_Write WRONG_GROUP(5)
    dateErr = sidex_DateTime_Write(docDateHandle, wGROUP, KEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == dateErr, "test_date_03", "WRONG_GROUP", 1);

    sidex_Variant_DecRef(dateHandle);
    sidex_Free(&docDateHandle);
    return testOkay;
}

/**  @ingroup Sidex_DateTime_Tests
* @brief Checks sidex_DateTime_Read, testing for errors
*
* A sidex_document is created and a dateTime entry is written to the sidex_document. First a normal sidex_DateTime_Read call is done, throwing no error.
* Different kinds of errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_date_04() {
    bool testOkay = true;
    //normal sidex_create
    dateErr = sidex_Create(DOCUMENT, &docDateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_04", "sidex_Create", 1);

    //assign sidex_variants
    dateErr = sidex_Variant_New_DateTime(MINDATE, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_04", "sidex_Variant_New_DateTime", 1);

    //write to sidex_document (normal)
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP, KEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_04", "sidex_DateTime_Write", 1);

    //normal read
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP, KEY, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_04", "sidex_DateTime_Read", 1);

    //DateTime_Write MISSING_KEY or UNICODE(2)
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP, NULL, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == dateErr || SIDEX_ERR_UNICODE == dateErr, 
                                  "test_date_04", "MISSING_KEY or UNICODE", 1);

    //DateTime_Write MISSING_GROUP or UNICODE(3)
    dateErr = sidex_DateTime_Read(docDateHandle, NULL, KEY, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == dateErr || SIDEX_ERR_UNICODE == dateErr, 
                                  "test_date_04", "MISSING_GROUP or UNICODE", 1);

    //DateTime_Write WRONG_KEY(4)
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP, wKEY, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == dateErr, "test_date_04", "WRONG_KEY", 1);

    //DateTime_Write WRONG_GROUP(5)
    dateErr = sidex_DateTime_Read(docDateHandle, wGROUP, KEY, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == dateErr, "test_date_04", "WRONG_GROUP", 1);


    //integer write- tests a WRONG_TYPE error
    dateHandle = sidex_Variant_New_Integer(-5236987);
    dateErr = sidex_Integer_Write(docDateHandle, GROUP2, KEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_04", "sidex_Integer_Write", 2);

    //read wrong dateTime/integer entry
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP2, KEY, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dateErr, "test_date_04", "WRONG_TYPE", 1);


    //trying to read a nonexistent entry - NOCONTENT
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP3, KEY, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == dateErr, "test_date_04", "NOCONTENT", 1);


    sidex_Free(&docDateHandle);
    return testOkay;
}

/**  @ingroup Sidex_DateTime_Tests
* @brief Checks sidex_Variant_DateTime_Check and sidex_Variant_New_DateTime for expected behavior
*
* A normal dateTime-value is assigned to a sidex_variant. It is tested with sidex_Variant_DateTime_Check, if it really handles a dateTime value.
* An integer is assigned to the sidex_variant and checked, if sidex_Variant_DateTime_Check returns SIDEX_FALSE.
* Errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_date_05() {
    bool testOkay = true;
    //assign sidex_variants
    dateErr = sidex_Variant_New_DateTime(MINDATE, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_05", "sidex_Variant_New_DateTime", 1);
    //check whether it really handles a datetime
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_DateTime_Check(dateHandle), "test_date_05", "sidex_Variant_DateTime_Check", 1);

    //test for a SIDEX_FALSE with a wrong sidex_variant type
    sidex_Variant_DecRef(dateHandle);
    dateHandle = sidex_Variant_New_Integer(-25);
    testOkay = testOkay && errLog(SIDEX_FALSE == sidex_Variant_DateTime_Check(dateHandle), 
                                  "test_date_05", "sidex_Variant_DateTime_Check", 1);

    //test for an INVALID_DATETIME error in sidex_Variant_New_DateTime
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_Variant_New_DateTime(wDATE, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_INVALID_DATETIME == dateErr, "test_date_05", "INVALID_DATETIME", 1);

    return testOkay;
}
 

/**  @ingroup Sidex_DateTime_Tests
* @brief Checks sidex_DateTime_Write and sidex_DateTime_Read with unusual characters for param group and key
*
* A sidex_document is created and three dateTime values are written to it, after assigning them to sidex_Variants. 
* They are read again and checked, whether they stayed type dateTime.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_date_06() {
    bool testOkay = true;
    //normal sidex_create
    dateErr = sidex_Create(DOCUMENT, &docDateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_06", "sidex_Create", 1);

    //assign sidex_variants
    dateErr = sidex_Variant_New_DateTime(MINDATE, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_06", "sidex_Variant_New_DateTime", 1);

    //write (1)
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP2, KEY2, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_06", "sidex_DateTime_Write", 1);

    //write (2)
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP3, KEY3, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_06", "sidex_DateTime_Write", 2);

    //write (3)
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP4, KEY4, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_06", "sidex_DateTime_Write", 3);

    //read (1)
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP2, KEY2, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_06", "sidex_DateTime_Read", 1);

    //read (2)
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP3, KEY3, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_06", "sidex_DateTime_Read", 2);

    //read (3)
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP4, KEY4, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_06", "sidex_DateTime_Read", 3);

    sidex_Variant_DecRef(dateHandle);
    sidex_Free(&docDateHandle);
    return testOkay;
}

/**  @ingroup Sidex_DateTime_Tests
* @brief Checks sidex_Variant_As_DateTime, testing for errors
*
* A sidex_document is created and an integer value is assigned to a sidex_variant. 
* It is checked, whether the function sidex_Variant_As_DateTime returns a WRONG_TYPE error.
* Next a dateTime is assigned to the sidex_variant and it is checked, whether the value stayed the same as before. 
* The sidex_variant is written as DateTime to the sidex_document and it is again checked,
* after reading the dateTime, if the value stayed the same.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_date_07() {
    bool testOkay = true;
    SIDEX_TCHAR* tempStr = NULL;

    //normal sidex_create
    dateErr = sidex_Create(DOCUMENT, &docDateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_07", "sidex_Create", 1);

    //assign an integer to a sidex_variant
    dateHandle = sidex_Variant_New_Integer(-123456);

    //sidex_Variant_As_DateTime-call, testing for a WRONG_TYPE error
    dateErr = sidex_Variant_As_DateTime(dateHandle, &tempStr);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == dateErr, "test_date_07", "WRONG_TYPE", 1);

    //assign a dateTime to a sidex_variant
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_Variant_New_DateTime(uDATE, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_07", "sidex_Variant_New_DateTime", 1);

    //check if the sidex_variant stayed the same value
    dateErr = sidex_Variant_As_DateTime(dateHandle, &tempStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_07", "sidex_Variant_As_DateTime", 1);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, uDATE) == 0, "test_date_07", "string != dateString", 1);

    //write the sidex_variant to a sidex_document
    dateErr = sidex_DateTime_Write(docDateHandle, GROUP, KEY, dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_07", "sidex_DateTime_Write", 1);

    //read the sidex_variant again
    sidex_Variant_DecRef(dateHandle);
    dateErr = sidex_DateTime_Read(docDateHandle, GROUP, KEY, &dateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_07", "sidex_DateTime_Read", 1);

    //check if the sidex_variant stayed the same value
    dateErr = sidex_Variant_As_DateTime(dateHandle, &tempStr);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_07", "sidex_Variant_As_DateTime", 2);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, uDATE) == 0, "test_date_07", "string != dateString", 2);

    sidex_Variant_DecRef(dateHandle);
    sidex_Free(&docDateHandle);
    return testOkay;
}

/**  @ingroup Sidex_DateTime_Tests
* @brief A loop, that writes and reads lots of dateTime-values in a sidex_document
*
* A sidex_document is created and a thousand times a dateTime is generated with a helper function, 
* assigning it to a sidex_variant. The sidex_variant is written to the sidex_document and read afterwards.
* The sidex_variant that was read is compared to the value the dateTime had before writing it to the sidex_document.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_date_08() {
    bool testOkay = true;
    SIDEX_TCHAR* temp;
    SIDEX_TCHAR* date;

    dateErr = sidex_Create(DOCUMENT, &docDateHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_08", "sidex_Create", 1);

    for(int i = 0; i < 1000; i++) {
        //call helper function for generating a dateTime with a valid format
        date = generate_DateTime();

        //create new DateTime
        dateErr = sidex_Variant_New_DateTime(date, &dateHandle);
        testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_08", "sidex_Variant_New_DateTime", i);
        if(!testOkay) {
            break;
        }

        //write dateTime into sidex_document
        dateErr = sidex_DateTime_Write(docDateHandle, GROUP, KEY, dateHandle);
        testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_08", "sidex_DateTime_Write", i);

        //read dateTime out of document
        sidex_Variant_DecRef(dateHandle);
        dateErr = sidex_DateTime_Read(docDateHandle, GROUP, KEY, &dateHandle);
        testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_08", "sidex_DateTime_Read", i);

        //get Value of dateTime as string
        dateErr = sidex_Variant_As_DateTime(dateHandle, &temp);
        testOkay = testOkay && errLog(SIDEX_SUCCESS == dateErr, "test_date_08", "sidex_Variant_As_DateTime", i);
        //compare to the original value
        testOkay = testOkay && errLog(sdxrt_cmp(temp, date) == 0, "test_date_08", "one value != doc", i);

        sidex_Variant_DecRef(dateHandle);
        delete[]date;
    }
    
    sidex_Free(&docDateHandle);
    return testOkay;
}

/**  @ingroup Sidex_DateTime_Tests
* @brief Calls all other subtests
* @param int test: If 0- all subtests are executed. If 1-8- particular subtest is executed.
* @param bool stop: If false- all subtests are executed 'normally'. If true- after a subtest fails, the function returns immediately.
* 
* For more details see documentation of the subtests
* 
* @returns bool : false on failure of test, true on success.
*/
bool test_sidex_date(int test, bool stop) {
    bool testsOkay = true;
    switch(test) {
    case 0:
    case 1:
        //test_date_01
        testsOkay = errLog(test_date_01(), "test_date_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }

    case 2:
        //test_date_02
        testsOkay = errLog(test_date_02(), "test_date_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }
    case 3:
        //test_date_03
        testsOkay = errLog(test_date_03(), "test_date_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }
    case 4:
        //test_date_04
        testsOkay = errLog(test_date_04(), "test_date_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }
    case 5:
        //test_date_05
        testsOkay = errLog(test_date_05(), "test_date_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }

    case 6:
        //test_date_06
        testsOkay = errLog(test_date_06(), "test_date_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_date_07
        testsOkay = errLog(test_date_07(), "test_date_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }

    case 8:
        //test_date_08
        testsOkay = errLog(test_date_08(), "test_date_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }
    }
    return testsOkay;
}