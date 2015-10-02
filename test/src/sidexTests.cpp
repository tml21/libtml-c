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

#ifdef SIDEX_UNICODE
  #define STR_DOCUMENT (wchar_t*) L"SIDEX_DEMO"
  #define STR_TEST_VALUES (wchar_t*) L"TestValues"
  #define STR_NUMNER (wchar_t*) L"Number"
  #define STR_TEXT (wchar_t*) L"Text"
  #define STR_THANKS (wchar_t*) L"thanks for the fish"
  #define STR_YES_NO (wchar_t*) L"YesOrNo"
  #define STR_PRICE (wchar_t*) L"Price"
  #define STR_BINARY (wchar_t*) L"Binary"
  #define STR_SIMPLE_CONTENT_SDX (wchar_t*) L"simpleContent_U.sdx"

  #define STAND_ALONE   (wchar_t*) L"Ein standalone string"
  #define ALONE         (wchar_t*) L"ALONE"
  #define N_STAND_ALONE (wchar_t*) L"NStandAlone"
  #define B_STAND_ALONE (wchar_t*) L"BStandAlone"
  #define I_STAND_ALONE (wchar_t*) L"IStandAlone"
  #define L_STAND_ALONE (wchar_t*) L"LStandAlone"
  #define F_STAND_ALONE (wchar_t*) L"FStandAlone"
  #define S_STAND_ALONE (wchar_t*) L"SStandAlone"
  #define D_STAND_ALONE (wchar_t*) L"DStandAlone"
  #define T_STAND_ALONE (wchar_t*) L"TStandAlone"

  #define AGROUP        (wchar_t*) L"AGroup"
  #define N_IN_A_GROUP  (wchar_t*) L"NInAGroup"
  #define B_IN_A_GROUP  (wchar_t*) L"BInAGroup"
  #define I_IN_A_GROUP  (wchar_t*) L"IInAGroup"
  #define L_IN_A_GROUP  (wchar_t*) L"LInAGroup"
  #define F_IN_A_GROUP  (wchar_t*) L"FInAGroup"
  #define S_IN_A_GROUP  (wchar_t*) L"SInAGroup"
  #define D_IN_A_GROUP  (wchar_t*) L"DInAGroup"
  #define T_IN_A_GROUP  (wchar_t*) L"TInAGroup"

  #define N_VAL_KEY     (wchar_t*) L"N_VAL_KEY"
  #define B_VAL_KEY     (wchar_t*) L"B_VAL_KEY"
  #define I_VAL_KEY     (wchar_t*) L"I_VAL_KEY"
  #define F_VAL_KEY     (wchar_t*) L"F_VAL_KEY"
  #define S_VAL_KEY     (wchar_t*) L"S_VAL_KEY"
  #define S2_VAL_KEY    (wchar_t*) L"S2_VAL_KEY"
  #define L_VAL_KEY     (wchar_t*) L"L_VAL_KEY"
  #define D_VAL_KEY     (wchar_t*) L"D_VAL_KEY"
  #define T_VAL_KEY     (wchar_t*) L"T_VAL_KEY"
  #define N_VAL_KEY_NESTED  (wchar_t*) L"N_VAL_KEY_NESTED"
  #define B_VAL_KEY_NESTED  (wchar_t*) L"B_VAL_KEY_NESTED"
  #define I_VAL_KEY_NESTED  (wchar_t*) L"I_VAL_KEY_NESTED"
  #define F_VAL_KEY_NESTED  (wchar_t*) L"F_VAL_KEY_NESTED"
  #define S_VAL_KEY_NESTED  (wchar_t*) L"S_VAL_KEY_NESTED"
  #define S2_VAL_KEY_NESTED (wchar_t*) L"S2_VAL_KEY_NESTED"
  #define N_VAL_KEY_IN_TABLE  (wchar_t*) L"N_VAL_KEY_IN_TABLE"
  #define B_VAL_KEY_IN_TABLE  (wchar_t*) L"B_VAL_KEY_IN_TABLE"
  #define I_VAL_KEY_IN_TABLE  (wchar_t*) L"I_VAL_KEY_IN_TABLE"
  #define F_VAL_KEY_IN_TABLE  (wchar_t*) L"F_VAL_KEY_IN_TABLE"
  #define S_VAL_KEY_IN_TABLE  (wchar_t*) L"S_VAL_KEY_IN_TABLE"
  #define S2_VAL_KEY_IN_TABLE (wchar_t*) L"S2_VAL_KEY_IN_TABLE"
  #define L2_VAL_KEY_IN_TABLE (wchar_t*) L"L2_VAL_KEY_IN_TABLE"
  #define D2_VAL_KEY_IN_TABLE (wchar_t*) L"D2_VAL_KEY_IN_TABLE"
  #define T2_VAL_KEY_IN_TABLE (wchar_t*) L"T2_VAL_KEY_IN_TABLE"
  #define STR_1_IN_ROW_1 (wchar_t*) L"Str 1 in table row 1"
  #define STR_2_IN_ROW_1 (wchar_t*) L"Str 2 in table row 1"
  #define STR_1_IN_ROW_2 (wchar_t*) L"Str 1 in table row 2"
  #define STR_2_IN_ROW_2 (wchar_t*) L"Str 2 in table row 2"
  #define REQUESTED_COL  (wchar_t*) L"REQUEST"
  #define STR_1_IN_LIST (wchar_t*) L"A string in a list"
  #define STR_2_IN_LIST (wchar_t*) L"A second string in a list"
  #define STR_IN_NESTED_LIST (wchar_t*) L"A string in a nested list"
  #define STR_1_IN_NESTED_DICT (wchar_t*) L"A string in a nested dict"
  #define STR_2_IN_NESTED_DICT (wchar_t*) L"A second string in a nested dict"

  #define TYPE_TEST_XML_FILE  (wchar_t*) L"typeTest_U.xml"

#else /// SIDEX_UNICODE
  #define STR_DOCUMENT (char*) "SIDEX_DEMO"
  #define STR_TEST_VALUES (char*) "TestValues"
  #define STR_NUMNER (char*) "Number"
  #define STR_TEXT (char*) "Text"
  #define STR_THANKS (char*) "thanks for the fish"
  #define STR_YES_NO (char*) "YesOrNo"
  #define STR_PRICE (char*) "Price"
  #define STR_BINARY (char*) "Binary"
  #define STR_SIMPLE_CONTENT_SDX (char*) "simpleContent.sdx"

  #define STAND_ALONE   (char*) "Ein standalone string"
  #define ALONE         (char*) "ALONE"
  #define N_STAND_ALONE (char*) "NStandAlone"
  #define B_STAND_ALONE (char*) "BStandAlone"
  #define I_STAND_ALONE (char*) "IStandAlone"
  #define L_STAND_ALONE (char*) "LStandAlone"
  #define F_STAND_ALONE (char*) "FStandAlone"
  #define S_STAND_ALONE (char*) "SStandAlone"
  #define D_STAND_ALONE (char*) "DStandAlone"
  #define T_STAND_ALONE (char*) "TStandAlone"

  #define AGROUP        (char*) "AGroup"
  #define N_IN_A_GROUP  (char*) "NInAGroup"
  #define B_IN_A_GROUP  (char*) "BInAGroup"
  #define I_IN_A_GROUP  (char*) "IInAGroup"
  #define L_IN_A_GROUP  (char*) "LInAGroup"
  #define F_IN_A_GROUP  (char*) "FInAGroup"
  #define S_IN_A_GROUP  (char*) "SInAGroup"
  #define D_IN_A_GROUP  (char*) "DInAGroup"
  #define T_IN_A_GROUP  (char*) "TInAGroup"


  #define N_VAL_KEY     (char*) "N_VAL_KEY"
  #define B_VAL_KEY     (char*) "B_VAL_KEY"
  #define I_VAL_KEY     (char*) "I_VAL_KEY"
  #define F_VAL_KEY     (char*) "F_VAL_KEY"
  #define S_VAL_KEY     (char*) "S_VAL_KEY"
  #define S2_VAL_KEY    (char*) "S2_VAL_KEY"
  #define L_VAL_KEY     (char*) "L_VAL_KEY"
  #define D_VAL_KEY     (char*) "D_VAL_KEY"
  #define T_VAL_KEY     (char*) "T_VAL_KEY"
  #define N_VAL_KEY_NESTED  (char*) "N_VAL_KEY_NESTED"
  #define B_VAL_KEY_NESTED  (char*) "B_VAL_KEY_NESTED"
  #define I_VAL_KEY_NESTED  (char*) "I_VAL_KEY_NESTED"
  #define F_VAL_KEY_NESTED  (char*) "F_VAL_KEY_NESTED"
  #define S_VAL_KEY_NESTED  (char*) "S_VAL_KEY_NESTED"
  #define S2_VAL_KEY_NESTED (char*) "S2_VAL_KEY_NESTED"
  #define N_VAL_KEY_IN_TABLE  (char*) "N_VAL_KEY_IN_TABLE"
  #define B_VAL_KEY_IN_TABLE  (char*) "B_VAL_KEY_IN_TABLE"
  #define I_VAL_KEY_IN_TABLE  (char*) "I_VAL_KEY_IN_TABLE"
  #define F_VAL_KEY_IN_TABLE  (char*) "F_VAL_KEY_IN_TABLE"
  #define S_VAL_KEY_IN_TABLE  (char*) "S_VAL_KEY_IN_TABLE"
  #define S2_VAL_KEY_IN_TABLE (char*) "S2_VAL_KEY_IN_TABLE"
  #define L2_VAL_KEY_IN_TABLE (char*) "L2_VAL_KEY_IN_TABLE"
  #define D2_VAL_KEY_IN_TABLE (char*) "D2_VAL_KEY_IN_TABLE"
  #define T2_VAL_KEY_IN_TABLE (char*) "T2_VAL_KEY_IN_TABLE"
  #define STR_1_IN_ROW_1 (char*) "Str 1 in table row 1"
  #define STR_2_IN_ROW_1 (char*) "Str 2 in table row 1"
  #define STR_1_IN_ROW_2 (char*) "Str 1 in table row 2"
  #define STR_2_IN_ROW_2 (char*) "Str 2 in table row 2"
  #define REQUESTED_COL  (char*) "REQUEST"
  #define STR_1_IN_LIST (char*) "A string in a list"
  #define STR_2_IN_LIST (char*) "A second string in a list"
  #define STR_IN_NESTED_LIST (char*) "A string in a nested list"
  #define STR_1_IN_NESTED_DICT (char*) "A string in a nested dict"
  #define STR_2_IN_NESTED_DICT (char*) "A second string in a nested dict"

  #define TYPE_TEST_XML_FILE  (char*) "typeTest.xml"

#endif // SIDEX_UNICODE

/**
 * A simple SIDEX I/O example 
 */
bool reloadContent()
{
  SIDEX_INT32 iErr      = SIDEX_SUCCESS; // API return value

  printf ("reloadContent\n");

  /////////////////////////////////////////////////////////////////////////
  // Create SIDEX_HANDLE
  SIDEX_HANDLE sHandle  = SIDEX_HANDLE_TYPE_NULL;
  iErr = sidex_Create(STR_DOCUMENT, &sHandle);

  iErr = sidex_Load_Content(sHandle, STR_SIMPLE_CONTENT_SDX);
  // read variants
  if (SIDEX_SUCCESS == iErr){
    SIDEX_INT64 iValue = 0;
    iErr = sidex_Integer_Read (sHandle, STR_TEST_VALUES, STR_NUMNER, &iValue);
#ifdef LINUX
    printf ("iValue = %lld\n", (long long) iValue);
#else
    printf ("iValue = %ld\n", iValue);
#endif // LINUX
  }
  if (SIDEX_SUCCESS == iErr){
#ifdef SIDEX_UNICODE
    wchar_t* strValue;
    SIDEX_INT32 iLength;
    iErr = sidex_String_Read (sHandle, STR_TEST_VALUES, STR_TEXT, &strValue, &iLength);
    fwprintf (stderr, L"strVal = %ls\n", strValue);
#else
    char* strValue;
    SIDEX_INT32 iLength;
    iErr = sidex_String_Read (sHandle, STR_TEST_VALUES, STR_TEXT, &strValue, &iLength);
    printf ("strVal = %s\n", strValue);
#endif// SIDEX_UNICODE
  }
  /////////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (SIDEX_SUCCESS != iErr){
    printf ("sidexDemo / error happened - Code = %d\n", iErr);
  }
  /////////////////////////////////////////////////////////////////////////
  // Free SIDEX_HANDLE
  if (SIDEX_HANDLE_TYPE_NULL != sHandle)
    sidex_Free(&sHandle);
  return true;
}

/**
 * A simple SIDEX I/O example 
 */
bool sidexTest01()
{
  SIDEX_INT32 iErr      = SIDEX_SUCCESS; // API return value

  printf ("sidexTest01: Perform simple SIDEX data write action\n");

  SIDEX_INT32 apiVer   = 0;
  SIDEX_INT32 libVer   = 0;
  SIDEX_INT32 cLength   = 0;
  SIDEX_TCHAR* verStr  = NULL;
  SIDEX_TCHAR* copyright  = NULL;
  sidex_Get_Version (&apiVer, &libVer, &verStr);
  sidex_Get_Copyright (&copyright, &cLength);
#ifdef SIDEX_UNICODE
    fwprintf (stderr, L"Version: %d - %d - %ls\n", apiVer, libVer, verStr);
    fwprintf (stderr, L"Copyright: %ls\n", copyright);
#else// SIDEX_UNICODE
    printf ("Version %d - %d - %s\n", apiVer, libVer, verStr);
    printf ("Copyright: %s\n", copyright);
#endif// SIDEX_UNICODE

  SIDEX_TCHAR* pContent  = NULL;
  SIDEX_INT32 iLenght   = 0;
  /////////////////////////////////////////////////////////////////////////
  // Create SIDEX_HANDLE
  SIDEX_HANDLE sHandle  = SIDEX_HANDLE_TYPE_NULL;
  iErr = sidex_Create(STR_DOCUMENT, &sHandle);
  /////////////////////////////////////////////////////////////////////////
  // write variants
  if (SIDEX_SUCCESS == iErr)
    iErr = sidex_Integer_Write (sHandle, STR_TEST_VALUES, STR_NUMNER, 42);
  if (SIDEX_SUCCESS == iErr)
    iErr = sidex_String_Write (sHandle, STR_TEST_VALUES, STR_TEXT, STR_THANKS);
  if (SIDEX_SUCCESS == iErr)
    iErr = sidex_Boolean_Write (sHandle, STR_TEST_VALUES, STR_YES_NO, SIDEX_FALSE);
  if (SIDEX_SUCCESS == iErr)
    iErr = sidex_Float_Write (sHandle, STR_TEST_VALUES, STR_PRICE, 12.4);
  if (SIDEX_SUCCESS == iErr)
    iErr = sidex_Binary_Write (sHandle, STR_TEST_VALUES, STR_BINARY, (unsigned char*)"1\n2\n3\n4\0", 8);
  /////////////////////////////////////////////////////////////////////////
  // Content
  if (SIDEX_SUCCESS == iErr)
    iErr = sidex_Get_Content(sHandle, &pContent, &iLenght);
  /////////////////////////////////////////////////////////////////////////
  // Save content
  if (SIDEX_SUCCESS == iErr)
    iErr = sidex_Save_Content(sHandle, STR_SIMPLE_CONTENT_SDX);
  /////////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (SIDEX_SUCCESS == iErr){
#ifdef SIDEX_UNICODE
    fwprintf (stderr, L"\nThe following SIDEX data is written into file %ls :\n", STR_SIMPLE_CONTENT_SDX);
    fwprintf (stderr, L"=====================================================================\n\n");
    fwprintf (stderr, L"%ls\n", pContent);
#else// SIDEX_UNICODE
    printf ("\nThe following SIDEX data is written into file %s :\n", STR_SIMPLE_CONTENT_SDX);
    printf ("=====================================================================\n\n");
    printf ("%s\n", pContent);
#endif// SIDEX_UNICODE
  }
  else
    printf ("sidexDemo / error happened - Code = %d\n", iErr);
  /////////////////////////////////////////////////////////////////////////
  // Free SIDEX_HANDLE
  if (SIDEX_HANDLE_TYPE_NULL != sHandle)
    sidex_Free(&sHandle);
  /////////////////////////////////////////////////////////////////////////
  // Free Content
  if (NULL != pContent)
    sidex_Free_Content(pContent);

  return true;
}

/**
 * Helper method / returns a list
 */
SIDEX_INT32 createNestedList(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sNone4List = sidex_Variant_New_None();
  SIDEX_VARIANT sBoolean4List = sidex_Variant_New_Boolean(SIDEX_FALSE);
  SIDEX_VARIANT sIntegern4List = sidex_Variant_New_Integer(4);
  SIDEX_VARIANT sFloat4List = sidex_Variant_New_Float((SIDEX_DOUBLE)4.4444);
  SIDEX_VARIANT sString4List;
  iRet = sidex_Variant_New_String(STR_IN_NESTED_LIST, &sString4List);

  /////////////////////////////////////////////
  // Variants into List
  SIDEX_VARIANT sList = sidex_Variant_New_List();
  SIDEX_INT32 pos;
  iRet = sidex_Variant_List_Append (sList, sNone4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sBoolean4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sIntegern4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sFloat4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sString4List, &pos);

  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sNone4List);
  sidex_Variant_DecRef(sBoolean4List);
  sidex_Variant_DecRef(sIntegern4List);
  sidex_Variant_DecRef(sFloat4List);
  sidex_Variant_DecRef(sString4List);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sList;
  }
  return iRet;
}

/**
 * Helper method / returns a dict
 */
SIDEX_INT32 createNestedDict(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sNone4Dict = sidex_Variant_New_None();
  SIDEX_VARIANT sBoolean4Dict = sidex_Variant_New_Boolean(SIDEX_FALSE);
  SIDEX_VARIANT sIntegern4Dict = sidex_Variant_New_Integer(5);
  SIDEX_VARIANT sFloat4Dict = sidex_Variant_New_Float((SIDEX_DOUBLE)-5.5);
  SIDEX_VARIANT sString4Dict;
  SIDEX_VARIANT sString4Dict2;
  iRet = sidex_Variant_New_String(STR_1_IN_NESTED_DICT, &sString4Dict);
  iRet = sidex_Variant_New_String(STR_2_IN_NESTED_DICT, &sString4Dict2);

  /////////////////////////////////////////////
  // Variants into List
  SIDEX_VARIANT sDict = sidex_Variant_New_Dict();
  iRet = sidex_Variant_Dict_Set(sDict, N_VAL_KEY_NESTED, sNone4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict, B_VAL_KEY_NESTED,  sBoolean4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict, I_VAL_KEY_NESTED,  sIntegern4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict, F_VAL_KEY_NESTED,  sFloat4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict, S_VAL_KEY_NESTED,  sString4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict, S2_VAL_KEY_NESTED,  sString4Dict2);

  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sNone4Dict);
  sidex_Variant_DecRef(sBoolean4Dict);
  sidex_Variant_DecRef(sIntegern4Dict);
  sidex_Variant_DecRef(sFloat4Dict);
  sidex_Variant_DecRef(sString4Dict);
  sidex_Variant_DecRef(sString4Dict2);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sDict;
  }
  return iRet;
}

/**
 * Helper method / returns one table row
 */
SIDEX_INT32 createANestedTableRow1(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sNone4TableRow1 = sidex_Variant_New_None();
  SIDEX_VARIANT sBoolean4TableRow1 = sidex_Variant_New_Boolean(SIDEX_FALSE);
  SIDEX_VARIANT sIntegern4TableRow1 = sidex_Variant_New_Integer(1);
  SIDEX_VARIANT sFloat4TableRow1 = sidex_Variant_New_Float((SIDEX_DOUBLE)1.111);
  SIDEX_VARIANT sString4TableRow1;
  SIDEX_VARIANT sString_2_4TableRow1;
  iRet = sidex_Variant_New_String(STR_1_IN_ROW_1, &sString4TableRow1);
  iRet = sidex_Variant_New_String(STR_2_IN_ROW_1, &sString_2_4TableRow1);

  /////////////////////////////////////////////
  // Variants into List
  SIDEX_VARIANT sDict = sidex_Variant_New_Dict();
  iRet = sidex_Variant_Dict_Set(sDict,N_VAL_KEY_IN_TABLE, sNone4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,B_VAL_KEY_IN_TABLE,  sBoolean4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,I_VAL_KEY_IN_TABLE,  sIntegern4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,F_VAL_KEY_IN_TABLE,  sFloat4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S_VAL_KEY_IN_TABLE,  sString4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S2_VAL_KEY_IN_TABLE,  sString_2_4TableRow1);

  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sNone4TableRow1);
  sidex_Variant_DecRef(sBoolean4TableRow1);
  sidex_Variant_DecRef(sIntegern4TableRow1);
  sidex_Variant_DecRef(sFloat4TableRow1);
  sidex_Variant_DecRef(sString4TableRow1);
  sidex_Variant_DecRef(sString_2_4TableRow1);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sDict;
  }
  return iRet;
}

/**
 * Helper method / returns a recond table row
 */
SIDEX_INT32 createANestedTableRow2(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sNone4TableRow2 = sidex_Variant_New_None();
  SIDEX_VARIANT sBoolean4TableRow2 = sidex_Variant_New_Boolean(SIDEX_FALSE);
  SIDEX_VARIANT sIntegern4TableRow2 = sidex_Variant_New_Integer(2);
  SIDEX_VARIANT sFloat4TableRow2 = sidex_Variant_New_Float((SIDEX_DOUBLE)2.222);
  SIDEX_VARIANT sString4TableRow2;
  iRet = sidex_Variant_New_String(STR_1_IN_ROW_2, &sString4TableRow2);
  SIDEX_VARIANT sString_2_4TableRow2;
  iRet = sidex_Variant_New_String(STR_2_IN_ROW_2, &sString_2_4TableRow2);

  /////////////////////////////////////////////
  // Variants into List
  SIDEX_VARIANT sDict = sidex_Variant_New_Dict();
  iRet = sidex_Variant_Dict_Set(sDict,N_VAL_KEY_IN_TABLE, sNone4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,B_VAL_KEY_IN_TABLE,  sBoolean4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,I_VAL_KEY_IN_TABLE,  sIntegern4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,F_VAL_KEY_IN_TABLE,  sFloat4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S_VAL_KEY_IN_TABLE,  sString4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S2_VAL_KEY_IN_TABLE,  sString_2_4TableRow2);

  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sNone4TableRow2);
  sidex_Variant_DecRef(sBoolean4TableRow2);
  sidex_Variant_DecRef(sIntegern4TableRow2);
  sidex_Variant_DecRef(sFloat4TableRow2);
  sidex_Variant_DecRef(sString4TableRow2);
  sidex_Variant_DecRef(sString_2_4TableRow2);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sDict;
  }
  return iRet;
}

/**
 * Helper method / adds a row to a table
 */
SIDEX_INT32 AddRow2Table(SIDEX_VARIANT sTable, SIDEX_VARIANT sRow){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  SIDEX_VARIANT sKeys = SIDEX_HANDLE_TYPE_NULL;
  iRet = sidex_Variant_Dict_Keys(sRow, &sKeys);
  SIDEX_INT32 iRowIdx;

#ifdef SIDEX_UNICODE
  wchar_t* sCol;
  wchar_t* sColName;
#else /// SIDEX_UNICODE
  char* sCol;
  char* sColName;
#endif // SIDEX_UNICODE
  if (SIDEX_SUCCESS == iRet)
  {
    SIDEX_INT32 iSize;
    iRet = sidex_Variant_Dict_Size (sRow, &iSize);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_Table_Rows(sTable, &iRowIdx);
    }

    //////////////////////////////////////////////////
    // First set the column names:
    for (int i = 0; SIDEX_SUCCESS == iRet && i < iSize; ++i){
      SIDEX_VARIANT varCol;
      iRet = sidex_Variant_List_Get (sKeys, i, &varCol);
      if (SIDEX_SUCCESS == iRet){
        SIDEX_INT32 iLength;
        iRet = sidex_Variant_As_String (varCol, &sCol, &iLength);
        if (SIDEX_SUCCESS == iRet){
          // Don't mind of the return value in next instruction:
          sidex_Variant_Table_AddColumn (sTable, sCol);
        }
      }
    }

    //////////////////////////////////////////////////
    // Add an empty row:
    if (SIDEX_SUCCESS == iRet){
      SIDEX_INT32 iRowIdx;
      iRet = sidex_Variant_Table_AddRow (sTable, &iRowIdx);
    }


    for (int i = 0; SIDEX_SUCCESS == iRet && i < iSize; ++i){
      iRet = sidex_Variant_Table_GetColumnName (sTable, i, &sColName);
      if (SIDEX_SUCCESS == iRet){
        SIDEX_VARIANT varCol;
        iRet = sidex_Variant_Dict_Get(sRow, sColName, &varCol);
        if (SIDEX_SUCCESS == iRet){
          iRet = sidex_Variant_Table_SetField (sTable, iRowIdx, sColName, varCol);
        }
      }
    }
  }
  if (SIDEX_HANDLE_TYPE_NULL != sKeys){
    sidex_Variant_DecRef(sKeys);
  }

  return iRet;
}

/**
 * Helper method / returns a table
 */
SIDEX_INT32 createNestedTable(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  

  SIDEX_VARIANT sRow1 = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_VARIANT sRow2 = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_VARIANT sTable = SIDEX_HANDLE_TYPE_NULL;


  /////////////////////////////////////////////
  // create row1
  iRet = createANestedTableRow1(&sRow1);
  /////////////////////////////////////////////
  // create row2
  if (SIDEX_SUCCESS == iRet)
    iRet = createANestedTableRow2(&sRow2);
  /////////////////////////////////////////////
  // table
  if (SIDEX_SUCCESS == iRet)
    sTable = sidex_Variant_New_Table ();

  SIDEX_INT32 iColumns;
  iRet = sidex_Variant_Table_Columns(sTable, &iColumns);
  if (SIDEX_SUCCESS == iRet){
     if (0 != iColumns){
       printf ("Unexpected amount of columns / %d\n", iColumns);
     }
  }

  SIDEX_VARIANT columnNames;
  iRet = sidex_Variant_Table_ColumnNames(sTable, &columnNames);
  if (SIDEX_SUCCESS == iRet){
    SIDEX_BOOL bHasCol;
    iRet = sidex_Variant_Table_HasColumn(sTable, REQUESTED_COL, &bHasCol);
    sidex_Variant_DecRef (columnNames);
  }
  
  iRet = SIDEX_SUCCESS;

  if (SIDEX_SUCCESS == iRet)
    iRet = AddRow2Table(sTable, sRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = AddRow2Table(sTable, sRow2);
  
  /////////////////////////////////////////////
  // decrement references to Variants:
  if (SIDEX_HANDLE_TYPE_NULL != sRow1)
    sidex_Variant_DecRef(sRow1);
  if (SIDEX_HANDLE_TYPE_NULL != sRow2)
    sidex_Variant_DecRef(sRow2);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sTable;
  }
  return iRet;
}

/**
 * Helper method / returns a list
 */
SIDEX_INT32 createStandAloneList(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sNone4List = sidex_Variant_New_None();
  SIDEX_VARIANT sBoolean4List = sidex_Variant_New_Boolean(SIDEX_FALSE);
  SIDEX_VARIANT sIntegern4List = sidex_Variant_New_Integer(4);
  SIDEX_VARIANT sFloat4List = sidex_Variant_New_Float((SIDEX_DOUBLE)4.4444);
  SIDEX_VARIANT sString4List;
  SIDEX_VARIANT sString4List2;
  iRet = sidex_Variant_New_String(STR_1_IN_LIST, &sString4List);
  iRet = sidex_Variant_New_String(STR_2_IN_LIST, &sString4List2);
  SIDEX_VARIANT sList4List = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedList(&sList4List);
  }
  SIDEX_VARIANT sDict4List = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedDict(&sDict4List);
  }
  SIDEX_VARIANT sTable4List = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedTable(&sTable4List);
  }

  /////////////////////////////////////////////
  // Variants into List
  SIDEX_VARIANT sList = sidex_Variant_New_List();
  SIDEX_INT32 pos;
  iRet = sidex_Variant_List_Append (sList, sNone4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sBoolean4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sIntegern4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sFloat4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sString4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sString4List2, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sList4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sDict4List, &pos);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_List_Append (sList, sTable4List, &pos);

  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sNone4List);
  sidex_Variant_DecRef(sBoolean4List);
  sidex_Variant_DecRef(sIntegern4List);
  sidex_Variant_DecRef(sFloat4List);
  sidex_Variant_DecRef(sString4List);
  sidex_Variant_DecRef(sString4List2);
  if (SIDEX_HANDLE_TYPE_NULL != sList4List)
    sidex_Variant_DecRef(sList4List);
  if (SIDEX_HANDLE_TYPE_NULL != sDict4List)
    sidex_Variant_DecRef(sDict4List);
  if (SIDEX_HANDLE_TYPE_NULL != sTable4List)
    sidex_Variant_DecRef(sTable4List);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sList;
  }
  return iRet;
}

/**
 * Helper method / returns a dict
 */
SIDEX_INT32 createStandAloneDict(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sNone4Dict = sidex_Variant_New_None();
  SIDEX_VARIANT sBoolean4Dict = sidex_Variant_New_Boolean(SIDEX_FALSE);
  SIDEX_VARIANT sIntegern4Dict = sidex_Variant_New_Integer(7);
  SIDEX_VARIANT sFloat4Dict = sidex_Variant_New_Float((SIDEX_DOUBLE)7.7);
  SIDEX_VARIANT sString4Dict;
  SIDEX_VARIANT sString4Dict2;
#ifdef SIDEX_UNICODE
  iRet = sidex_Variant_New_String((wchar_t*) L"Str 1 in dict", &sString4Dict);
  iRet = sidex_Variant_New_String((wchar_t*) L"Str 2 in dict", &sString4Dict2);
#else /// SIDEX_UNICODE
  iRet = sidex_Variant_New_String((char*)"Str 1 in dict", &sString4Dict);
  iRet = sidex_Variant_New_String((char*)"Str 2 in dict", &sString4Dict2);
#endif // SIDEX_UNICODE
  SIDEX_VARIANT sList4Dict = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedList(&sList4Dict);
  }
  SIDEX_VARIANT sDict4Dict = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedDict(&sDict4Dict);
  }
  SIDEX_VARIANT sTable4Dict = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedTable(&sTable4Dict);
  }

  /////////////////////////////////////////////
  // Variants into List
  SIDEX_VARIANT sDict = sidex_Variant_New_Dict();
  iRet = sidex_Variant_Dict_Set(sDict,N_VAL_KEY, sNone4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,B_VAL_KEY,  sBoolean4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,I_VAL_KEY,  sIntegern4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,F_VAL_KEY,  sFloat4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S_VAL_KEY,  sString4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S2_VAL_KEY,  sString4Dict2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,L_VAL_KEY,  sList4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,D_VAL_KEY,  sDict4Dict);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,T_VAL_KEY,  sTable4Dict);

  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sNone4Dict);
  sidex_Variant_DecRef(sBoolean4Dict);
  sidex_Variant_DecRef(sIntegern4Dict);
  sidex_Variant_DecRef(sFloat4Dict);
  sidex_Variant_DecRef(sString4Dict);
  sidex_Variant_DecRef(sString4Dict2);
  if (SIDEX_HANDLE_TYPE_NULL != sList4Dict)
    sidex_Variant_DecRef(sList4Dict);
  if (SIDEX_HANDLE_TYPE_NULL != sDict4Dict)
    sidex_Variant_DecRef(sDict4Dict);
  if (SIDEX_HANDLE_TYPE_NULL != sTable4Dict)
    sidex_Variant_DecRef(sTable4Dict);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sDict;
  }
  return iRet;
}

/**
 * Helper method / returns one table row
 */
SIDEX_INT32 createATableRow1(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sNone4TableRow1 = sidex_Variant_New_None();
  SIDEX_VARIANT sBoolean4TableRow1 = sidex_Variant_New_Boolean(SIDEX_FALSE);
  SIDEX_VARIANT sIntegern4TableRow1 = sidex_Variant_New_Integer(1);
  SIDEX_VARIANT sFloat4TableRow1 = sidex_Variant_New_Float((SIDEX_DOUBLE)1.111);
  SIDEX_VARIANT sString4TableRow1;
  SIDEX_VARIANT sString_2_4TableRow1;
  iRet = sidex_Variant_New_String(STR_1_IN_ROW_1, &sString4TableRow1);
  iRet = sidex_Variant_New_String(STR_2_IN_ROW_1, &sString_2_4TableRow1);


  SIDEX_VARIANT sList4TableRow1 = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedList(&sList4TableRow1);
  }
  SIDEX_VARIANT sDict4TableRow1 = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedDict(&sDict4TableRow1);
  }
  SIDEX_VARIANT sTable4TableRow1 = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedTable(&sTable4TableRow1);
  }

  /////////////////////////////////////////////
  // Variants into List
  SIDEX_VARIANT sDict = sidex_Variant_New_Dict();
  iRet = sidex_Variant_Dict_Set(sDict,N_VAL_KEY_IN_TABLE, sNone4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,B_VAL_KEY_IN_TABLE,  sBoolean4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,I_VAL_KEY_IN_TABLE,  sIntegern4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,F_VAL_KEY_IN_TABLE,  sFloat4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S_VAL_KEY_IN_TABLE,  sString4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S2_VAL_KEY_IN_TABLE,  sString_2_4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,L2_VAL_KEY_IN_TABLE,  sList4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,D2_VAL_KEY_IN_TABLE,  sDict4TableRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,T2_VAL_KEY_IN_TABLE,  sTable4TableRow1);

  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sNone4TableRow1);
  sidex_Variant_DecRef(sBoolean4TableRow1);
  sidex_Variant_DecRef(sIntegern4TableRow1);
  sidex_Variant_DecRef(sFloat4TableRow1);
  sidex_Variant_DecRef(sString4TableRow1);
  sidex_Variant_DecRef(sString_2_4TableRow1);
  if (SIDEX_HANDLE_TYPE_NULL != sList4TableRow1)
    sidex_Variant_DecRef(sList4TableRow1);
  if (SIDEX_HANDLE_TYPE_NULL != sDict4TableRow1)
    sidex_Variant_DecRef(sDict4TableRow1);
  if (SIDEX_HANDLE_TYPE_NULL != sTable4TableRow1)
    sidex_Variant_DecRef(sTable4TableRow1);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sDict;
  }
  return iRet;
}

/**
 * Helper method / returns a recond table row
 */
SIDEX_INT32 createATableRow2(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sNone4TableRow2 = sidex_Variant_New_None();
  SIDEX_VARIANT sBoolean4TableRow2 = sidex_Variant_New_Boolean(SIDEX_FALSE);
  SIDEX_VARIANT sIntegern4TableRow2 = sidex_Variant_New_Integer(2);
  SIDEX_VARIANT sFloat4TableRow2 = sidex_Variant_New_Float((SIDEX_DOUBLE)2.222);
  SIDEX_VARIANT sString4TableRow2;
  iRet = sidex_Variant_New_String(STR_1_IN_ROW_2, &sString4TableRow2);
  SIDEX_VARIANT sString_2_4TableRow2;
  iRet = sidex_Variant_New_String(STR_2_IN_ROW_2, &sString_2_4TableRow2);
  SIDEX_VARIANT sList4TableRow2 = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedList(&sList4TableRow2);
  }
  SIDEX_VARIANT sDict4TableRow2 = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedDict(&sDict4TableRow2);
  }
  SIDEX_VARIANT sTable4TableRow2 = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createNestedTable(&sTable4TableRow2);
  }

  /////////////////////////////////////////////
  // Variants into List
  SIDEX_VARIANT sDict = sidex_Variant_New_Dict();
  iRet = sidex_Variant_Dict_Set(sDict,N_VAL_KEY_IN_TABLE, sNone4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,B_VAL_KEY_IN_TABLE,  sBoolean4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,I_VAL_KEY_IN_TABLE,  sIntegern4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,F_VAL_KEY_IN_TABLE,  sFloat4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S_VAL_KEY_IN_TABLE,  sString4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,S2_VAL_KEY_IN_TABLE,  sString_2_4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,L2_VAL_KEY_IN_TABLE,  sList4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,D2_VAL_KEY_IN_TABLE,  sDict4TableRow2);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Dict_Set (sDict,T2_VAL_KEY_IN_TABLE,  sTable4TableRow2);

  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sNone4TableRow2);
  sidex_Variant_DecRef(sBoolean4TableRow2);
  sidex_Variant_DecRef(sIntegern4TableRow2);
  sidex_Variant_DecRef(sFloat4TableRow2);
  sidex_Variant_DecRef(sString4TableRow2);
  sidex_Variant_DecRef(sString_2_4TableRow2);
  if (SIDEX_HANDLE_TYPE_NULL != sList4TableRow2)
    sidex_Variant_DecRef(sList4TableRow2);
  if (SIDEX_HANDLE_TYPE_NULL != sDict4TableRow2)
    sidex_Variant_DecRef(sDict4TableRow2);
  if (SIDEX_HANDLE_TYPE_NULL != sTable4TableRow2)
    sidex_Variant_DecRef(sTable4TableRow2);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sDict;
  }
  return iRet;
}

/**
 * Helper method / returns a table
 */
SIDEX_INT32 createStandAloneTable(SIDEX_VARIANT* pVariant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  

  SIDEX_VARIANT sRow1 = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_VARIANT sRow2 = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_VARIANT sTable = SIDEX_HANDLE_TYPE_NULL;


  /////////////////////////////////////////////
  // create row1
  iRet = createATableRow1(&sRow1);
  /////////////////////////////////////////////
  // create row2
  if (SIDEX_SUCCESS == iRet)
    iRet = createATableRow2(&sRow2);
  /////////////////////////////////////////////
  // table
  if (SIDEX_SUCCESS == iRet)
    sTable = sidex_Variant_New_Table ();
  if (SIDEX_SUCCESS == iRet)
    iRet = AddRow2Table(sTable, sRow1);
  if (SIDEX_SUCCESS == iRet)
    iRet = AddRow2Table(sTable, sRow2);
  
  /////////////////////////////////////////////
  // decrement references to Variants:
  if (SIDEX_HANDLE_TYPE_NULL != sRow1)
    sidex_Variant_DecRef(sRow1);
  if (SIDEX_HANDLE_TYPE_NULL != sRow2)
    sidex_Variant_DecRef(sRow2);

  /////////////////////////////////////////////
  // Return value:
  if (SIDEX_SUCCESS == iRet){
    *pVariant = sTable;
  }
  return iRet;
}

/**
 * A data type test
 */
bool sidexTest02()
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;  
  /////////////////////////////////////////////////////////////////////////
  // CreateHandle
  printf("sidexTest02:  data type test:\n"); 
  printf("=============================\n"); 

  SIDEX_HANDLE shandle = SIDEX_HANDLE_TYPE_NULL;

#ifdef SIDEX_UNICODE
  iRet = sidex_Create(L"ROOT", &shandle);
  wchar_t* s;
  iRet = sidex_Get_DocumentName(shandle, &s);
  iRet = sidex_Set_DocumentName(shandle, (wchar_t*) L"TYPETEST");
  iRet = sidex_Get_DocumentName(shandle, &s);
#else // SIDEX_UNICODE
  iRet = sidex_Create("ROOT", &shandle);
  char* s;
  iRet = sidex_Get_DocumentName(shandle, &s);
  iRet = sidex_Set_DocumentName(shandle,(char*) "TYPETEST");
  iRet = sidex_Get_DocumentName(shandle, &s);
#endif //SIDEX_UNICODE

  /////////////////////////////////////////////
  // Variants:
  SIDEX_VARIANT sSingleNoneStandAlone = sidex_Variant_New_None();
  SIDEX_VARIANT sSingleBooleanStandAlone = sidex_Variant_New_Boolean(SIDEX_TRUE);
  SIDEX_VARIANT sSingleIntegernStandAlone = sidex_Variant_New_Integer(8);
  SIDEX_VARIANT sSingleFloatStandAlone = sidex_Variant_New_Float((SIDEX_DOUBLE)8.1);
  SIDEX_VARIANT sSingleStringStandAlone;
  iRet = sidex_Variant_New_String(STAND_ALONE, &sSingleStringStandAlone);
  SIDEX_VARIANT sSingleListStandAlone = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createStandAloneList(&sSingleListStandAlone);
  }
  SIDEX_VARIANT sSingleDictStandAlone = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createStandAloneDict(&sSingleDictStandAlone);
  }
  SIDEX_VARIANT sSingleTableStandAlone = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_SUCCESS == iRet){
    iRet = createStandAloneTable(&sSingleTableStandAlone);
  }

  /////////////////////////////////////////////
  // Variants into handle / Single - no group:
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,ALONE,N_STAND_ALONE, sSingleNoneStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,ALONE,B_STAND_ALONE, sSingleBooleanStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,ALONE,I_STAND_ALONE, sSingleIntegernStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,ALONE,F_STAND_ALONE, sSingleFloatStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,ALONE,S_STAND_ALONE, sSingleStringStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,ALONE,L_STAND_ALONE, sSingleListStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,ALONE,D_STAND_ALONE, sSingleDictStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,ALONE,T_STAND_ALONE, sSingleTableStandAlone);


  /////////////////////////////////////////////
  // Variants into handle under AGroup
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,AGROUP,N_IN_A_GROUP, sSingleNoneStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,AGROUP,B_IN_A_GROUP, sSingleBooleanStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,AGROUP,I_IN_A_GROUP, sSingleIntegernStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,AGROUP,F_IN_A_GROUP, sSingleFloatStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,AGROUP,S_IN_A_GROUP, sSingleStringStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,AGROUP,L_IN_A_GROUP, sSingleListStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,AGROUP,D_IN_A_GROUP, sSingleDictStandAlone);
  if (SIDEX_SUCCESS == iRet)
    iRet = sidex_Variant_Write(shandle,AGROUP,T_IN_A_GROUP, sSingleTableStandAlone);


  /////////////////////////////////////////////
  // decrement references to Variants:
  sidex_Variant_DecRef(sSingleNoneStandAlone);
  sidex_Variant_DecRef(sSingleBooleanStandAlone);
  sidex_Variant_DecRef(sSingleIntegernStandAlone);
  sidex_Variant_DecRef(sSingleFloatStandAlone);
  sidex_Variant_DecRef(sSingleStringStandAlone);
  if (SIDEX_HANDLE_TYPE_NULL != sSingleListStandAlone)
    sidex_Variant_DecRef(sSingleListStandAlone);
  if (SIDEX_HANDLE_TYPE_NULL != sSingleDictStandAlone)
    sidex_Variant_DecRef(sSingleDictStandAlone);
  if (SIDEX_HANDLE_TYPE_NULL != sSingleTableStandAlone)
    sidex_Variant_DecRef(sSingleTableStandAlone);

  /////////////////////////////////////////////
  // Handle to file:
  if (SIDEX_SUCCESS == iRet){
    iRet = sidex_Save_Content(shandle,TYPE_TEST_XML_FILE);
  }

  SIDEX_TCHAR*   pContent  = SIDEX_HANDLE_TYPE_NULL;
  SIDEX_INT32    iLenght   = 0;
   /////////////////////////////////////////////////////////////////////////
  // Content
  if (SIDEX_SUCCESS == iRet){
    iRet = sidex_Get_Content(shandle, &pContent, &iLenght);
  }
  if (SIDEX_HANDLE_TYPE_NULL != pContent){
#ifdef SIDEX_UNICODE
    fwprintf (stderr, L"\nThe following SIDEX data is written into file %ls :\n", TYPE_TEST_XML_FILE);
    fwprintf (stderr, L"=====================================================================\n\n");
    fwprintf (stderr, L"%ls\n", pContent);
#else// SIDEX_UNICODE
    printf ("\nThe following SIDEX data is written into file %s :\n", TYPE_TEST_XML_FILE);
    printf ("=====================================================================\n\n");
    printf ("%s\n", pContent);
#endif// SIDEX_UNICODE
    sidex_Free_Content(pContent);
  }
  else{
    printf ("Unexpected content\n");
  }
  /////////////////////////////////////////////////////////////////////////
  // FreeHandle
  if  (SIDEX_HANDLE_TYPE_NULL != shandle){
    sidex_Free(&shandle);
  }
  /////////////////////////////////////////////////////////////////////////
  // Errorhandling:
  if (iRet != SIDEX_SUCCESS)
  {
    printf ("Error %d", iRet);
  }
  return true;
}

