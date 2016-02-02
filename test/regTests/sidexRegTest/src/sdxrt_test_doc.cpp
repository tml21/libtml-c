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
  #include <string.h>
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

#define I_VAL_A 1815
#define I_VAL_B 4711

SIDEX_INT32 docErr      = SIDEX_SUCCESS; // API return value

SIDEX_HANDLE docHandle = SIDEX_HANDLE_TYPE_NULL;

SIDEX_HANDLE docHandleNull = SIDEX_HANDLE_TYPE_NULL;

#ifdef SIDEX_UNICODE
    wchar_t* docContent = (wchar_t*) L"<?xml version='1.0' encoding='UTF-8'?><exampleCode><from>Jani</from><body>Don't forget me this weekend!</body></exampleCode>";
#else
    char* docContent = (char*) "<?xml version='1.0' encoding='UTF-8'?><exampleCode><from>Jani</from><body>Don't forget me this weekend!</body></exampleCode>";
#endif

/** 
* @ingroup Sidex_Document_Tests
* @brief Tests sidex_Create for errors
*
* First a sidex_document is normally created. Then errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_01() {
    bool testOkay = true;
    // -1 normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_01", "sidex_Create", 1);
    sidex_Free(&docHandle);

    // -2 testing a WRONG_DOC Error
    docErr = sidex_Create(wDOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_DOC == docErr, "test_doc_01", "WRONG_DOC", 1);

    // -3 testing a WRONG_PARAMETER error
    docErr = sidex_Create(NULL, &docHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == docErr, "test_doc_01", "WRONG_PARAMETER", 1);

    return testOkay;
}

/**  
* @ingroup Sidex_Document_Tests
* @brief Sidex_Set_DocumentName is tested to throw errors
*
* A sidex_document is created and the name is set with sidex_Set_DocumentName normally. 
* Errors are tested. 
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_02() {
    bool testOkay = true;
    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_02", "sidex_Create", 1);

    // -1 normal Set_DocumentName
    docErr = sidex_Set_DocumentName(docHandle, DOCUMENT);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_02", "sidex_Set_DocumentName", 1);

    // -2 check for wrong_doc error
    docErr = sidex_Set_DocumentName(docHandle, wDOCUMENT);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_DOC == docErr, "test_doc_02", "WRONG_DOC", 1);

    // -3 check for wrong_parameter error / handle in NULL
    docErr = sidex_Set_DocumentName(docHandleNull, DOCUMENT);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == docErr, "test_doc_02", "WRONG_PARAMETER", 1);

    sidex_Free(&docHandle);
    return testOkay;
}

/**  
* @ingroup Sidex_Document_Tests
* @brief The function sidex_Get_DocumentName is tested
*
* A sidex_document is created and sidex_Get_DocumentName is called, assigning the 
* documentname to a temporal variable. The variable is tested 
* if it is the same as param pname when the sidex_document was created.
* An error is tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_03() {
    bool testOkay = true;
    SIDEX_TCHAR* temp = NULL;

    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_03", "sidex_Create", 1);

    // -1 normal sidex_Get
    docErr = sidex_Get_DocumentName(docHandle, &temp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_03", "sidex_Get_DocumentName", 1);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(temp,DOCUMENT) == 0, "test_doc_03", "docName != name", 1);

    // -2 check for wrong_parameter error / handle in NULL
    docErr = sidex_Get_DocumentName(docHandleNull, &temp);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == docErr, "test_doc_03", "WRONG_PARAMETER", 1);

    sidex_Free(&docHandle);
    return testOkay;
}

/**  
* @ingroup Sidex_Document_Tests
* @brief Checks Save_Content
*
* A sidex_document is created. Then errors from Save_Content are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_04() {
    bool testOkay = true;
    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_04", "sidex_Create", 1);

    //-1 testing save_content to throw an error
    docErr = sidex_Save_Content(docHandle, NULL);
    testOkay = testOkay && errLog(SIDEX_ERR_UNICODE == docErr || SIDEX_ERR_WRONG_PARAMETER == docErr, "test_doc_04", "WRONG_PARAMETER or UNICODE", 1);

    //-2 testing save_content to throw dumpcontent error
    docErr = sidex_Save_Content(docHandle, wDOCUMENT);
    //in LINUX it is okay, to have special characters in the documentname ('<')
#ifndef LINUX 
    testOkay = testOkay && errLog(SIDEX_ERR_DUMPCONTENT == docErr, "test_doc_04", "DUMP_CONTENT", 1);
#else 
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_04", "sidex_Create", 1);

#endif
    //-3 normal save_content- SIDEX_SUCCESS
    docErr = sidex_Save_Content(docHandle, _sdxrtT("document.sdx"));
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_04", "sidex_Save_Content", 1);

    sidex_Free(&docHandle);
    return testOkay;
}

/**  
* @ingroup Sidex_Document_Tests
* @brief Checks Load_Content for appropriate errors
*
* A sidex_document is created, some entries are put inside and normally saved as file. 
* An error is tested. It is tried to load the content normally and afterwards 
* checked if the entries are still there.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_05() {
    bool testOkay = true;
    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_05", "sidex_Create", 1);

    // normal save_content- SIDEX_SUCCESS
    docErr = sidex_Save_Content(docHandle, _sdxrtT("document.xml"));
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_05", "sidex_Save_Content", 1);

    // load_content with handle as NULL -> DUMPCONTENT or UNICODE
    docErr = sidex_Load_Content(docHandleNull, DOCUMENT);
    testOkay = testOkay && errLog(SIDEX_ERR_UNICODE == docErr || SIDEX_ERR_DUMPCONTENT == docErr, 
                                  "test_doc_05", "DUMPCONTENT or UNICODE", 1);

    // normal load_content
    docErr = sidex_Load_Content(docHandle, _sdxrtT("document.xml"));
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_05", "sidex_Load_Content", 1);

    sidex_Free(&docHandle);
    return testOkay ;
}

/**  
* @ingroup Sidex_Document_Tests
* @brief Set_Content is tested for an error
*
* A sidex_document is created. XML-code is assigned to a variable, representing example-content of a sidex_document. 
* An errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_06() {
    bool testOkay = true;
    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_06", "sidex_Create", 1);

    //set_content with handle of NULL
    docErr = sidex_Set_Content(docHandleNull, docContent);
    testOkay = testOkay && errLog(SIDEX_ERR_DUMPCONTENT == docErr, "test_doc_06", "DUMPCONTENT", 1);

    //normal set_content, SIDEX_SUCCESS
    docErr = sidex_Set_Content(docHandle, docContent);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_06", "sidex_Set_Content", 1);

    sidex_Free(&docHandle);
    return testOkay;
}

/**  
* @ingroup Sidex_Document_Tests
* @brief Checks the functions Get_Content and sidex_Get_Content_Length, also testing for errors
* 
* A sidex_document is created and sample XML-code is set as content of the sidex_document. 
* The function Get_Content is called, saving the content and length of the content in 
* temporary variables to compare them with the original values, content and length of the XML-code. 
* Errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_07() {
    bool testOkay = true;
    SIDEX_INT32 contLength = sdxrt_len(docContent);
    SIDEX_TCHAR* contTemp = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 lengthTemp = -1;

    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_07", "sidex_Create", 1);

    //normal Set- and Get_Content
    docErr = sidex_Set_Content(docHandle, docContent);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_07", "sidex_Set_Content", 1);

    docErr = sidex_Get_Content(docHandle, &contTemp, &lengthTemp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_07", "sidex_Get_Content", 1);

    //lengthTemp and contLenght can be different and that is okay
    sidex_Free_Content(contTemp);

    //testing DUMPCONTENT error with param sHandle = NULL
    docErr = sidex_Get_Content(docHandleNull, &contTemp, &lengthTemp);
    testOkay = testOkay && errLog(SIDEX_ERR_DUMPCONTENT == docErr, "test_doc_07", "DUMPCONTENT", 1);

    //normal Get_Content_Lenght
    docErr = sidex_Get_Content_Length(docHandle, &contLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_07", "sidex_Get_Content_Length", 1);

    /*checking if there is a difference between the length the functions 
    sidex_get_content and sidex_get_content_length return */
    testOkay = testOkay && errLog(lengthTemp == contLength, "test_doc_07", "length =! length", 1);

    //Get_Content_Lenght testing a DUMPCONTENT error
    docErr = sidex_Get_Content_Length(SIDEX_HANDLE_TYPE_NULL, &contLength);
    testOkay = testOkay && errLog(SIDEX_ERR_DUMPCONTENT == docErr, "test_doc_07", "DUMPCONTENT", 2);

    sidex_Free(&docHandle);
    return testOkay;
}


/**  
* @ingroup Sidex_Document_Tests
* @brief Checks sidex_Clear
*
* A sidex_document is created and the content is set with a custom String containing XML-code. 
* The sidex_document gets cleared. It is checked whether the sidex_document 
* still contains an entry, that was set before with Set_Content.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_08() {
    bool testOkay = true;
    SIDEX_INT32 contLength = sdxrt_len(docContent);
    SIDEX_TCHAR* contTemp = NULL;
    SIDEX_INT32 lengthTemp = -1;
    SIDEX_TCHAR* tempFloat = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 tempLength = -1;

    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_08", "sidex_Create", 1);

    // normal Set_Content
    docErr = sidex_Set_Content(docHandle, docContent);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_08", "sidex_Set_Content", 1);

    // normal sidex_Clear
    sidex_Clear(docHandle);
    //check if content is really empty
    docErr = sidex_Get_Content(docHandle, &contTemp, &lengthTemp);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_08", "sidex_Get_Content", 1);

    //try to read a certain string from the sidex_document from before sidex_clear
    docErr = sidex_String_Read(docHandle, _sdxrtT("from"), KEY3, &tempFloat, &tempLength);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == docErr, "test_doc_08", "content != empty", 1);

    sidex_Free_Content(contTemp);
    sidex_Free(&docHandle);
    return testOkay;
}

/**  
* @ingroup sidex_Merge_Subtest
* @brief helper functions for testing sidex_merge
*/
bool createDoc(SIDEX_HANDLE* sHandle, SIDEX_TCHAR* pname, SIDEX_TCHAR* iGroup, SIDEX_TCHAR* sGroup, SIDEX_TCHAR* iKey, SIDEX_TCHAR* sKey, SIDEX_INT64 iValue, SIDEX_TCHAR* sValue) {
  bool testOkay = true;
  *sHandle = SIDEX_HANDLE_TYPE_NULL;
  docErr = sidex_Create(pname, sHandle);
  testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_09", "sidex_Create", 1);
  if (testOkay){
    docErr = sidex_Integer_Write(*sHandle, iGroup, iKey, iValue);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_09", "sidex_Integer_Write", 1);
  }
  if (testOkay){
    docErr = sidex_String_Write(*sHandle, sGroup, sKey, sValue);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_09", "sidex_String_Write", 1);
  }
  return testOkay;
}

bool test_doc_Merge(SIDEX_BOOL bOverwrite, 
                   SIDEX_TCHAR* name1, 
                   SIDEX_TCHAR* iGroup1, SIDEX_TCHAR* sGroup1, SIDEX_TCHAR* iKey1, SIDEX_TCHAR* sKey1, SIDEX_INT64 iValue1, SIDEX_TCHAR* sValue1,
                   SIDEX_TCHAR* name2,
                   SIDEX_TCHAR* iGroup2, SIDEX_TCHAR* sGroup2, SIDEX_TCHAR* iKey2, SIDEX_TCHAR* sKey2, SIDEX_INT64 iValue2, SIDEX_TCHAR* sValue2,
                   SIDEX_TCHAR* sMergeGroup, SIDEX_TCHAR* sMergeKey,
                   SIDEX_HANDLE* mergeHandle) {
  bool testOkay = true;

  SIDEX_HANDLE aHandle = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_HANDLE bHandle = SIDEX_HANDLE_TYPE_NULL;
  testOkay = createDoc (&aHandle, name1, iGroup1, sGroup1, iKey1, sKey1, iValue1, sValue1);
  if (testOkay){
    testOkay = createDoc (&bHandle, name2, iGroup2, sGroup2, iKey2, sKey2, iValue2, sValue2);
  }
  SIDEX_TCHAR* aContent = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_TCHAR* bContent = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 aLength;
  SIDEX_INT32 bLength;
  docErr = sidex_Get_Content(aHandle, &aContent, &aLength);
  testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_09", "sidex_Get_Content", 1);
  docErr = sidex_Get_Content(bHandle, &bContent, &bLength);
  testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_09", "sidex_Get_Content", 2);
  if (testOkay){
    docErr = sidex_Merge(aHandle, bHandle, bOverwrite, sMergeGroup, sMergeKey);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_09", "sidex_Merge", 1);
  }
  if (testOkay){
    SIDEX_TCHAR* zContent = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 zLength;
    docErr = sidex_Get_Content(aHandle, &zContent, &zLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_09", "sidex_Get_Content", 3);
    if (SIDEX_HANDLE_TYPE_NULL != zContent){
      sidex_Free_Content(zContent);
    }
  }
  if (SIDEX_HANDLE_TYPE_NULL != bContent){
    sidex_Free_Content(bContent);
  }
  if (SIDEX_HANDLE_TYPE_NULL != aContent){
    sidex_Free_Content(aContent);
  }
  sidex_Free(&bHandle);
  *mergeHandle = aHandle;
  return testOkay;
}

//checks whether in sHandle exists a String in sGroup at point sKey.
bool checkStringExistence(SIDEX_HANDLE sHandle, SIDEX_TCHAR* sGroup, SIDEX_TCHAR* sKey, SIDEX_TCHAR* sCompare) {
  bool testOkay       = true;
  SIDEX_INT32 iLength = 0;
  SIDEX_TCHAR*      sResult = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet    = SIDEX_SUCCESS;


  iRet = sidex_String_Read (sHandle, sGroup, sKey, &sResult, &iLength);
  if (SIDEX_SUCCESS == iRet){
    if (0 != sdxrt_cmp(sResult, sCompare)){
      testOkay = false;
    }
    sidex_Free_ReadString(sResult);
  }
  else{
    testOkay = false;
  }
  return testOkay;
}

//it is checked whether at position sGroup, sKey exists a string. If yes, return false.
bool checkUnexpectedStringExistence(SIDEX_HANDLE sHandle, SIDEX_TCHAR* sGroup, SIDEX_TCHAR* sKey) {
  bool        testOkay = true;
  SIDEX_INT32 iLength  = 0;
  SIDEX_TCHAR*      sResult  = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet     = SIDEX_SUCCESS;

  iRet = sidex_String_Read (sHandle, sGroup, sKey, &sResult, &iLength);
  if (SIDEX_ERR_NOCONTENT != iRet && SIDEX_ERR_WRONG_TYPE != iRet){
    testOkay = false;
    sidex_Free_ReadString(sResult);
  }
  return testOkay;
}

//checks whether there exists an integer at location sGroup, sKey. And if yes, returns false.
bool checkUnexpectedIntegerExistence(SIDEX_HANDLE sHandle, SIDEX_TCHAR* sGroup, SIDEX_TCHAR* sKey) {
  bool        testOkay = true;
  SIDEX_INT64 tempInt  = 0;
  SIDEX_TCHAR*      sResult  = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet     = SIDEX_SUCCESS;

  iRet = sidex_Integer_Read (sHandle, sGroup, sKey, &tempInt);
  if (SIDEX_ERR_NOCONTENT != iRet && SIDEX_ERR_WRONG_TYPE != iRet){
    testOkay = false;
  }
  return testOkay;
}

//checks whether there exists an integer at location sGroup, sKey. And if yes, returns true.
bool checkIntegerExistence(SIDEX_HANDLE sHandle, SIDEX_TCHAR* sGroup, SIDEX_TCHAR* sKey, SIDEX_INT64 iCompare) {
  bool        testOkay = true;
  SIDEX_INT32 iRet     = SIDEX_SUCCESS;
  SIDEX_INT64 iResult = 0;

  iRet = sidex_Integer_Read (sHandle, GROUP, KEY3, &iResult);
  if (SIDEX_SUCCESS == iRet){
    if (I_VAL_A != iResult){
      testOkay = false;
    }
  }
  else{
    testOkay = false;
  }
  return testOkay;
}

/**  
* @ingroup sidex_Merge_Subtest
* @brief subtest of test_doc_09 - a
*
* This Test tests if sidex_Merge works as expected. No overwriting, same Groups and one time same Key- merging at this one same Key.
* @returns bool : false on failure of test, true on success.
*/
bool test_doc_9a() {
  bool testOkay = true;
  SIDEX_HANDLE mergeHandle = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  testOkay = test_doc_Merge(SIDEX_FALSE, 
                            DOCUMENT, 
                            GROUP, GROUP, KEY3, KEY, I_VAL_A, A_S_VAL, 
                            DOCUMENT2,
                            GROUP, GROUP, KEY, KEY2, I_VAL_B, B_S_VAL,
                            GROUP, KEY, &mergeHandle);
  ///////////////////////////////////////////////
  // Check the expected results:
  if (testOkay){
    testOkay = checkStringExistence(mergeHandle, GROUP, KEY, A_S_VAL);
  }
  if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP, KEY3, I_VAL_A);
  }
  ///////////////////////////////////////////////
  // Check the unexpected results:
  if (testOkay){
    testOkay = checkUnexpectedStringExistence(mergeHandle, GROUP, KEY2);
  }
  /////////////////////////
  // Now free the handle:
  if (SIDEX_HANDLE_TYPE_NULL != mergeHandle){
    sidex_Free(&mergeHandle);
  }
  return testOkay;
}

/** 
* @ingroup sidex_Merge_Subtest
*  @brief subtest of test_doc_09 - b
*
* This Test tests if sidex_Merge works as expected. With overwriting (true), same Groups and one time same Key- merging at this one same Key.
* @returns bool : false on failure of test, true on success.
*/
bool test_doc_9b() {
  bool testOkay = true;
  SIDEX_HANDLE mergeHandle = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  testOkay = test_doc_Merge(SIDEX_TRUE, 
                            DOCUMENT, 
                            GROUP, GROUP, KEY3, KEY, I_VAL_A, A_S_VAL, 
                            DOCUMENT2,
                            GROUP, GROUP, KEY, KEY2, I_VAL_B, B_S_VAL,
                            GROUP, KEY, &mergeHandle);
  ///////////////////////////////////////////////
  // Check the expected results:
  if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP, KEY, I_VAL_B);
  }
  if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP, KEY3, I_VAL_A);
  }
  ///////////////////////////////////////////////
  // Check the unexpected results:
  if (testOkay){
    testOkay = checkUnexpectedStringExistence(mergeHandle, GROUP, KEY);
  }
  /////////////////////////
  // Now free the handle:
  if (SIDEX_HANDLE_TYPE_NULL != mergeHandle){
    sidex_Free(&mergeHandle);
  }
  return testOkay;
}

/**  
* @ingroup sidex_Merge_Subtest
* @brief subtest of test_doc_09 - c
*
* This Test tests if sidex_Merge works as expected. No overwriting, same Groups and one time same Key- merging the whole Group.
* @returns bool : false on failure of test, true on success.
*/
bool test_doc_9c() {
  bool testOkay = true;
  SIDEX_HANDLE mergeHandle = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  testOkay = test_doc_Merge(SIDEX_FALSE, 
                            DOCUMENT, 
                            GROUP, GROUP, KEY3, KEY, I_VAL_A, A_S_VAL, 
                            DOCUMENT2,
                            GROUP, GROUP, KEY, KEY2, I_VAL_B, B_S_VAL,
                            GROUP, NULL, &mergeHandle);
  ///////////////////////////////////////////////
  // Check the expected results:
  if (testOkay){
    testOkay = checkStringExistence(mergeHandle, GROUP, KEY, A_S_VAL);
  }
  if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP, KEY3, I_VAL_A);
  }
  if (testOkay){
    testOkay = checkStringExistence(mergeHandle, GROUP, KEY2, B_S_VAL);
  }

  ///////////////////////////////////////////////
  // Check the unexpected results:
  if (testOkay){
    testOkay = checkUnexpectedIntegerExistence(mergeHandle, GROUP, KEY);
  }
  /////////////////////////
  // Now free the handle:
  if (SIDEX_HANDLE_TYPE_NULL != mergeHandle){
    sidex_Free(&mergeHandle);
  }
  return testOkay;
}

/** 
*  @brief subtest of test_doc_09 - d
*
* This Test tests if sidex_Merge works as expected. With overwriting, same Groups and one time same Key- merging the whole Group.
* @returns bool : false on failure of test, true on success.
*/
bool test_doc_9d() {
  bool testOkay = true;
  SIDEX_HANDLE mergeHandle = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  testOkay = test_doc_Merge(SIDEX_TRUE, 
                            DOCUMENT, 
                            GROUP, GROUP, KEY3, KEY, I_VAL_A, A_S_VAL, 
                            DOCUMENT2,
                            GROUP, GROUP, KEY, KEY2, I_VAL_B, B_S_VAL,
                            GROUP, NULL, &mergeHandle);
  ///////////////////////////////////////////////
  // Check the expected results:
  if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP, KEY, I_VAL_B);
  }
  if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP, KEY3, I_VAL_B);
  }
  if (testOkay){
    testOkay = checkStringExistence(mergeHandle, GROUP, KEY2, B_S_VAL);
  }
  ///////////////////////////////////////////////
  // Check the unexpected results:
  if (testOkay){
    testOkay = checkUnexpectedStringExistence(mergeHandle, GROUP, KEY);
  }
  /////////////////////////
  // Now free the handle:
  if (SIDEX_HANDLE_TYPE_NULL != mergeHandle){
    sidex_Free(&mergeHandle);
  }
  return testOkay;
}

/** 
* @ingroup sidex_Merge_Subtest
*  @brief subtest of test_doc_09 - e
*
* This Test tests if sidex_Merge works as expected. No overwriting, different Groups and one time same Key- merging the whole Group.
* @returns bool : false on failure of test, true on success.
*/
bool test_doc_9e() {
  bool testOkay = true;
  SIDEX_HANDLE mergeHandle = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  testOkay = test_doc_Merge(SIDEX_FALSE, 
                            DOCUMENT, 
                            GROUP, GROUP, KEY3, KEY, I_VAL_A, A_S_VAL, 
                            DOCUMENT2,
                            GROUP2, GROUP2, KEY, KEY2, I_VAL_B, B_S_VAL,
                            GROUP2, NULL, &mergeHandle);
  ///////////////////////////////////////////////
  // Check the expected results:
  if (testOkay){
    testOkay = checkStringExistence(mergeHandle, GROUP, KEY, A_S_VAL);
  }
  if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP, KEY3, I_VAL_A);
  }
  if (testOkay){
    testOkay = checkStringExistence(mergeHandle, GROUP2, KEY2, B_S_VAL);
  }
    if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP2, KEY, I_VAL_B);
  }
  ///////////////////////////////////////////////
  // No unexpected results:
  /////////////////////////
  // Now free the handle:
  if (SIDEX_HANDLE_TYPE_NULL != mergeHandle){
    sidex_Free(&mergeHandle);
  }
  return testOkay;
}


/**  
* @ingroup sidex_Merge_Subtest
* @brief subtest of test_doc_09 - f
*
* This Test tests if sidex_Merge works as expected. No overwriting, different Groups and one time same Key- merging the whole Group.
* @returns bool: false on failure of test, true on success.
*/
bool test_doc_9f() {
  bool testOkay = true;
  SIDEX_HANDLE mergeHandle = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  testOkay = test_doc_Merge(SIDEX_TRUE, 
                            DOCUMENT, 
                            GROUP, GROUP, KEY3, KEY, I_VAL_A, A_S_VAL, 
                            DOCUMENT2,
                            GROUP2, GROUP2, KEY, KEY2, I_VAL_B, B_S_VAL,
                            GROUP2, NULL, &mergeHandle);
  ///////////////////////////////////////////////
  // Check the expected results:
  if (testOkay){
    testOkay = checkStringExistence(mergeHandle, GROUP, KEY, A_S_VAL);
  }
  if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP, KEY3, I_VAL_A);
  }
  if (testOkay){
    testOkay = checkStringExistence(mergeHandle, GROUP2, KEY2, B_S_VAL);
  }
    if (testOkay){
    testOkay = checkIntegerExistence(mergeHandle, GROUP2, KEY, I_VAL_B);
  }
  ///////////////////////////////////////////////
  // No unexpected results:
  /////////////////////////
  // Now free the handle:
  if (SIDEX_HANDLE_TYPE_NULL != mergeHandle){
    sidex_Free(&mergeHandle);
  }
  return testOkay;
}


/**  
* @ingroup Sidex_Document_Tests
* @brief checks sidex_merge, also testing an error
*
* Two sidex_documents are created and an integer-entry is added in both, under the same group/key-pair, but with different values. 
* Then they are merged in different circumstances (overwrite true/false, whole Group,etc). An error is tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_09() {
    bool testsOkay = true;
    bool testOkay = true;

    testOkay = test_doc_9a();
    if (!testOkay) {
        cout << "test_doc_09a FAILED" << endl;
        testsOkay = false;
    }

    testOkay = test_doc_9b();
    if (!testOkay) {
        cout << "test_doc_09b FAILED" << endl;
        testsOkay = false;
    }

    testOkay = test_doc_9c();
    if (!testOkay) {
        cout << "test_doc_09c FAILED" << endl;
        testsOkay = false;
    }

    testOkay = test_doc_9d();
    if (!testOkay) {
        cout << "test_doc_09d FAILED" << endl;
        testsOkay = false;
    }

    testOkay = test_doc_9e();
    if (!testOkay) {
        cout << "test_doc_09e FAILED" << endl;
        testsOkay = false;
    }

    testOkay = test_doc_9f();
    if (!testOkay) {
        cout << "test_doc_09f FAILED" << endl;
        testsOkay = false;
    }

    //testing for an error - 9g
    docErr = sidex_Merge(docHandle, docHandle, false, NULL, NULL);
    testsOkay = testOkay && errLog(SIDEX_ERR_UNICODE == docErr || SIDEX_ERR_MISSING_GROUP == docErr, 
                                   "test_doc_09", "sidex_Merge(g) UNICODE or MISSING_GROUP", 1);

    return testsOkay;
}


/** 
* @ingroup Sidex_Document_Tests
* @brief Checks Group management functions, sidex_GetGroups, sidex_HasGroup, sidex_DeleteGroup
*
* A sidex_document is created and two sidex-Integer- and two sidex-Boolean-entries are added, Integer and booleans are both in their own groups.
* It is checked, whether sidex_HasGroups returns not SIDEX_TRUE if called with the group-names of the just created entries.
* Sidex_HasGroup is called with param group=NULL, and it returns SIDEX_FALSE.
* Sidex_GetGroups is called and the Variant is checked, if it contains the two group-names that were earlier created.
* Sidex_DeleteGroup is called, two times, with the group-names from before and it is checked whether the groups are still in the sidex_document with sidex_HasGroups.
* Afterwards sidex_DeleteGroup is tested for errors.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_10() {
    bool testOkay = true;
    SIDEX_VARIANT listVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 tempSize = -1;
    SIDEX_VARIANT groupVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_TCHAR* tempStr = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 strLength = -1;

    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Create", 1);

    //adding some entries, integer-entry:
    docErr = sidex_Integer_Write(docHandle, GROUP, KEY, -9223372036854775807);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Integer_Write", 1);

    //another integer-entry
    docErr = sidex_Integer_Write(docHandle, GROUP, KEY2, 9223372036854775807);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Integer_Write", 2);

    //boolean-entry 
    docErr = sidex_Boolean_Write(docHandle, GROUP2, KEY, true);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Boolean_Write", 1);

    //boolean-entry
    docErr = sidex_Boolean_Write(docHandle, GROUP2, KEY2, false);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Boolean_Write", 2);

    //check sidex_HasGroup(1)
    testOkay = testOkay && errLog(SIDEX_TRUE == (sidex_HasGroup(docHandle, GROUP)), "test_doc_10", "sidex_HasGroup", 1);
    //check sidex_HasGroup(2)
    testOkay = testOkay && errLog(SIDEX_TRUE == (sidex_HasGroup(docHandle, GROUP2)), "test_doc_10", "sidex_HasGroup", 2);

    //test for FALSE(3)
    testOkay = testOkay && errLog(SIDEX_FALSE == (sidex_HasGroup(docHandle, NULL)), "test_doc_10", "sidex_HasGroup", 3);

    //check sidex_GetGroups (1)
    docErr = sidex_GetGroups(docHandle, &listVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_GetGroups", 1);

    //checking listSize = 2
    docErr = sidex_Variant_List_Size(listVariant, &tempSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Variant_List_Size", 1);
    //compare size to 2
    testOkay = testOkay && errLog(2 == tempSize, "test_doc_10", "Variant_List_Size != groupSize", 1);

    //checking the groupnames in list
    docErr = sidex_Variant_List_Get(listVariant, 0, &groupVariant);     //groupVariant contains borrowed reference
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Variant_List_Get", 1);

    docErr = sidex_Variant_As_String(groupVariant, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Variant_As_String", 1);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, GROUP) == 0 , "test_doc_10", "group != groupname", 1);

    //check second groupname (2)
    docErr = sidex_Variant_List_Get(listVariant, 1, &groupVariant);     //groupVariant contains borrowed reference
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Variant_List_Get", 2);

    docErr = sidex_Variant_As_String(groupVariant, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_Variant_As_String", 2);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, GROUP2) == 0, "test_doc_10", "group != groupname", 2);

    //check sidex_DeleteGroup
    docErr = sidex_DeleteGroup(docHandle, GROUP);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_DeleteGroup", 1);
    //verify that the group was deleted
    testOkay = testOkay && errLog(SIDEX_FALSE == (sidex_HasGroup(docHandle, GROUP)), "test_doc_10", "sidex_HasGroup", 4);

    docErr = sidex_DeleteGroup(docHandle, GROUP2);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_10", "sidex_DeleteGroup", 2);
    //verify that the group was deleted
    testOkay = testOkay && errLog(SIDEX_FALSE == (sidex_HasGroup(docHandle, GROUP)), "test_doc_10", "sidex_HasGroup", 5);

    //sidex_DeleteGroups - testing error(3) WRONG_GROUP
    docErr = sidex_DeleteGroup(docHandle, wGROUP); 
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == docErr, "test_doc_10", "WRONG_GROUP", 1);

    //(4)NOCONTENT error
    docErr = sidex_DeleteGroup(docHandle, GROUP);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == docErr, "test_doc_10", "NOCONTENT", 1);

    //(5) UNICODE or WRONG_GROUP
    docErr = sidex_DeleteGroup(docHandle, NULL);
    testOkay = testOkay && errLog(SIDEX_ERR_UNICODE == docErr || SIDEX_ERR_WRONG_GROUP == docErr, "test_doc_10", "UNICODE or WRONG_GROUP", 1);

    sidex_Variant_DecRef(listVariant);
    sidex_Free(&docHandle);
    return testOkay;
}


/** 
* @ingroup Sidex_Document_Tests
* @brief Checks sidex_GetKeys, sidex_HasKey and sidex_DeleteKey
*
* A sidex_document is created and two sidex-Integer- and two sidex-Boolean-entries are added, Integer and booleans are both in their own groups.
* It is checked, whether sidex_HasKey returns not SIDEX_TRUE if called with the key-names of just created entries.
* Sidex_HasKey is called with param key=NULL, and it returns SIDEX_FALSE.
* Sidex_GetKeys is called and the Variant is checked, if it contains the key-names that were earlier created.
* Sidex_DeleteKey is called, two times with the key-names from before and it is checked whether the keys are still in the sidex_document with sidex_HasKeys.
* Afterwards sidex_DeleteKey is tested for errors.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_doc_11() {
    bool testOkay = true;
    SIDEX_VARIANT listVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 tempSize = -1;
    SIDEX_VARIANT keyVariant= SIDEX_HANDLE_TYPE_NULL;
    SIDEX_TCHAR* tempStr = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 strLength = -1;

    // normal sidex_Create
    docErr = sidex_Create(DOCUMENT, &docHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Create", 1);

    //adding some entries, integer-entry:
    docErr = sidex_Integer_Write(docHandle, GROUP, KEY, -9223372036854775807);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Integer_Write", 1);

    //another integer-entry
    docErr = sidex_Integer_Write(docHandle, GROUP, KEY2, 9223372036854775807);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Integer_Write", 2);

    //boolean-entry 
    docErr = sidex_Boolean_Write(docHandle, GROUP2, KEY, true);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Boolean_Write", 1);

    //check sidex_HasKey(1)
    testOkay = testOkay && errLog(SIDEX_TRUE == (sidex_HasKey(docHandle, GROUP, KEY)), "test_doc_11", "sidex_HasKey", 1);

    //check sidex_HasKey(2)
    testOkay = testOkay && errLog(SIDEX_TRUE == (sidex_HasKey(docHandle, GROUP, KEY2)), "test_doc_11", "sidex_HasKey", 2);

    //check sidex_HasKey(3)
    testOkay = testOkay && errLog(SIDEX_TRUE == (sidex_HasKey(docHandle, GROUP2, KEY)), "test_doc_11", "sidex_HasKey", 3);

    //testing for FALSE(3)
    testOkay = testOkay && errLog(SIDEX_FALSE == (sidex_HasKey(docHandle, GROUP, NULL)), "test_doc_11", "sidex_HasKey", 4);

    //check sidex_GetKeys (1)
    docErr = sidex_GetKeys(docHandle, GROUP, &listVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_GetKeys", 1);

    //checking listSize = 2
    docErr = sidex_Variant_List_Size(listVariant, &tempSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Variant_List_Size", 1);
    //compare size to 2
    testOkay = testOkay && errLog(tempSize == 2, "test_doc_11", "Variant_List_Size != groupSize", 1);

    //checking the keynames in list
    docErr = sidex_Variant_List_Get(listVariant, 0, &keyVariant);   //keyVariant contains borrowed reference
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Variant_List_Get", 1);

    docErr = sidex_Variant_As_String(keyVariant, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Variant_As_String", 1);
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, KEY) == 0, "test_doc_11", "key != keyname", 1);

    //check second keyname (2)
    docErr = sidex_Variant_List_Get(listVariant, 1, &keyVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Variant_List_Get", 2);

    docErr = sidex_Variant_As_String(keyVariant, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Variant_As_String", 2);
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, KEY2) == 0, "test_doc_11", "key != keyname", 2);

    //check sidex_GetKeys from second Group (2)
    sidex_Variant_DecRef(listVariant);
    docErr = sidex_GetKeys(docHandle, GROUP2, &listVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_GetKeys", 2);

    //checking listSize = 1, (2)
    docErr = sidex_Variant_List_Size(listVariant, &tempSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Variant_List_Size", 2);
    testOkay = testOkay && errLog(tempSize == 1, "test_doc_11", "Variant_List_Size != groupSize", 2);

    //checking the key-names in list
    docErr = sidex_Variant_List_Get(listVariant, 0, &keyVariant);    //keyVariant contains borrowed reference
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Variant_List_Get", 3);

    docErr = sidex_Variant_As_String(keyVariant, &tempStr, &strLength);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_Variant_As_String", 3);
    //compare to original
    testOkay = testOkay && errLog(sdxrt_cmp(tempStr, KEY) == 0, "test_doc_11", "key != keyname", 3);

    //check sidex_DeleteKey
    docErr = sidex_DeleteKey(docHandle, GROUP, KEY);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_DeleteKey", 1);
    testOkay = testOkay && errLog(SIDEX_FALSE == (sidex_HasKey(docHandle, GROUP, KEY)), "test_doc_11", "sidex_HasKey", 4);

    docErr = sidex_DeleteKey(docHandle, GROUP, KEY2);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == docErr, "test_doc_11", "sidex_DeleteKey", 2);
    testOkay = testOkay && errLog(SIDEX_FALSE == (sidex_HasKey(docHandle, GROUP, KEY2)), "test_doc_11", "sidex_Haskey", 5);

    //sidex_Deletekeys - testing for error(3): WRONG_KEY
    docErr = sidex_DeleteKey(docHandle, GROUP2 ,wKEY);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == docErr, "test_doc_11", "WRONG_KEY", 1);

    //(4) NOCONTENT error
    docErr = sidex_DeleteKey(docHandle, GROUP, KEY);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == docErr, "test_doc_11", "NOCONTENT", 1);

    //(5) UNICODE or MISSING_KEY error
    docErr = sidex_DeleteKey(docHandle, GROUP2, NULL);
    testOkay = testOkay && errLog(SIDEX_ERR_UNICODE == docErr || SIDEX_ERR_MISSING_KEY == docErr, "test_doc_11", "UNICODE or MISSING_KEY", 1);

    sidex_Variant_DecRef(listVariant);
    sidex_Free(&docHandle);
    return testOkay;
    return testOkay;
}

/** 
* @ingroup Sidex_Document_Tests
* @brief Calls all other subtests
* @param int test: If 0- all subtests are executed. If 1-X- the particular test is executed.
* @param bool stop: If false- subtests are normally executed. If true- immediately returns after a subtest fails.
*
* @returns bool testOkay : false on failure of subtest, true on success.
*/
bool test_sidex_doc(int test, bool stop) {
    bool testsOkay = true;

    switch(test) {
    case 0:
    case 1:
        //test_doc_01
        testsOkay = errLog(test_doc_01(), "test_doc_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }
    
    case 2:
        //test_doc_02
        testsOkay = errLog(test_doc_02(), "test_doc_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3:
        //test_doc_03
        testsOkay = errLog(test_doc_03(), "test_doc_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }
    
    case 4:
        //test_doc_04
        testsOkay = errLog(test_doc_04(), "test_doc_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }

    case 5:
        //test_doc_05
        testsOkay = errLog(test_doc_05(), "test_doc_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }

    case 6:
        //test_doc_06
        testsOkay = errLog(test_doc_06(), "test_doc_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_doc_07
        testsOkay = errLog(test_doc_07(), "test_doc_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }

    case 8:
        //test_doc_08
        testsOkay = errLog(test_doc_08(), "test_doc_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }

    case 9:
        //test_doc_9
        testsOkay = errLog(test_doc_09(), "test_doc_09") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 9) {
            break;
        }

    case 10:
        //test_doc_10
        testsOkay = errLog(test_doc_10(), "test_doc_10") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 10) {
            break;
        }

    case 11:
        //test_doc_11
        testsOkay = errLog(test_doc_11(), "test_doc_11") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 11) {
            break;
        }
    }
    return testsOkay;
}
