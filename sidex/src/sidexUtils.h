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

#ifndef SIDEXUTILS_H
#define SIDEXUTILS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexStdTypes.h"


/**
 * @brief   Encodes a string using base64.
 *
 * @param   rawtext  String to be encoded.
 * @param   iSize    Reference to the length  of string / if -1 it will be computed by strlen.
 *
 * @returns Reference to the encoded string.<br>
 						Must be deallocated using delete().
 */
 char* sidexutil_encodeBase64(const char *rawtext,int* iSize);

/**
 * @brief   Decodes a base64 helper

 * @param   c char to calc
 *
 * @returns calculated value
 */
char ceCalc(char c);

  /**
 * @brief   Decodes a base64 encoded string.
 *
 * @param   encodedtext String to be decoded.
 * @param   iSize       Reference to the length of string / if -1 it will be internal computed by strlen.<br>
 *                      After operation it contains the length of the the encoded string.
 *
 * @returns Reference to the decoded string.<br>
 						Must be deallocated using delete().
 */
 char* sidexutil_decodeBase64(const char *encodedtext, SIDEX_INT32* iSize);


/**
 * @brief   Returns the length for a base64 encoded string decode operation.
 *
 * @param   encodedtext String to be decoded.
 *
 * @returns length for a base64 encoded string decode operation.
 */
 SIDEX_INT32 sidexutil_decodeBase64Length(const char *encodedtext);


/**
 * @brief   Get SIDEX_DATA_TYPE by string.
 *
 * @param   c_type string to be analyzed.
 *
 * @returns  The SIDEX_DATA_TYPE convenient to the string's value.<br>
 *           In case of a mismatch SIDEX_DATA_TYPE_UNKNOWN will be returned.
 *
 * @see sidexDefines.h, sidexStdTypes.h
 */
 SIDEX_DATA_TYPE sidexutil_strToTypeEnum(const char* c_type);

 /**
 * @brief   Converts SIDEX_DATA_TYPE to string
 *
 * @param   type type to be stringified.
 *
 * @returns The string convenient to the SIDEX_DATA_TYPE's value.
 *
 * @see sidexDefines.h, sidexStdTypes.h
 */
const char* sidexutil_TypeToString(SIDEX_DATA_TYPE type);

 /**
 * @brief   Converts SIDEX_ENCODE data to string.
 *
 * @param   encoding The Encoding
 *
 * @returns The string convenient to the SIDEX_ENCODE's value.
 *
 * @see sidexDefines.h, sidexStdTypes.h
 */
const char *sidexutil_sidexEncodingToString(SIDEX_ENCODE encoding);


 /**
 * @brief   Converts SIDEX_DATA_TYPE to string
 *
 * @param   type type to be stringified.
 *
 * @returns The string convenient to the SIDEX_STRING_ENCODING_TYPE's value.
 *
 * @see sidexDefines.h, sidexStdTypes.h
 */
const char* sidexutil_StringFormatToString(SIDEX_STRING_ENCODING_TYPE type);


 /**
 * @brief   Converts a string to a SIDEX_STRING_ENCODING_TYPE.
 *
 * @param   string The string to convert
 *
 * @returns The SIDEX_STRING_ENCODING_TYPE convenient to the string.
 *
 * @see sidexDefines.h, sidexStdTypes.h
 */
const SIDEX_STRING_ENCODING_TYPE sidexutil_StringToStringFormat(const char* string);


/**
 * @brief   Decrement the reference counter of the SIDEX_VARIANT object.
 *
 *          A reference counter value of 0 signals that there is no more reference to the object.<br>
 *          In this case the internal memory management will free the object.
 *
 * @param   variant Instance of a variant object to be managed.
 *
 * @see sidexDefines.h
 */
void sidexutil_Variant_DecRef(SIDEX_VARIANT variant);


/**
 * @brief   Increment the reference counter of the SIDEX_VARIANT object.
 *
 *          Every time you make a copy of the reference to a SIDEX_VARIANT object,<br>
 *          you must call this memory management method.<br>
 *          If the reference to the SIDEX_VARIANT object is no longer in use,<br>
 *          you must decrement the reference counter of the SIDEX_VARIANT object using sidexVariant_DecRef.
 *
 * @param   variant Instance of a variant object to be managed.
 *
 * @see sidexDefines.h
 */
void sidexutil_Variant_IncRef(SIDEX_VARIANT variant);


/**
 * @brief   Get row of type SIDEX_VARIANT out of a sidexTable.
 *
 * @param   sTable   the table (will be checked to be an instance of sidexTable).
 * @param   rowIndex 0 based index of the row to get.
 * @param   variant  Reference to SIDEX_VARIANT object for the row to get.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sTable is not SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if there is no value at the requested index.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutil_TableGetRow(SIDEX_VARIANT sTable, int rowIndex, SIDEX_VARIANT* variant);


/**
 * @brief   Remove the row referring the rowIndex out of the table.
 *
 * @param   sTable   the table (will be checked to be an instance of sidexTable).
 * @param   rowIndex  0 based index of the row to be removed out of the table.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sTable is not SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if there is no row at the requested rowIndex.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutil_TableDeleteRow(SIDEX_VARIANT sTable, int rowIndex);


/**
 * @brief   Compare a variants type with an expected type.
 *
 * @param   variant The value to compare.
 * @param   expectedType The data type to compare with.
 *
 * @returns SIDEX_TRUE if the variant type is equals to the expectedType.
 *
 * @see sidexDefines.h, sidexStdTypes.h
 */
SIDEX_BOOL sidexutil_dataTypeCheck(SIDEX_VARIANT variant, SIDEX_DATA_TYPE expectedType);


/**
 * @brief   Compare a string variant encoding with an expected encoding.
 *
 * @param   variant The value to compare.
 * @param   expectedEncode The string encoding to compare with.
 *
 * @returns SIDEX_TRUE if the variant is a SIDEX_DATA_TYPE_STRING / SIDEX_DATA_TYPE_BINARY type and if it's encoding is equals to the expectedEncode.
 *
 * @see sidexDefines.h, sidexStdTypes.h
 */
SIDEX_BOOL sidexutil_stringDataEncodeCheck(SIDEX_VARIANT variant, SIDEX_ENCODE expectedEncode);


/**
 * @brief   Returns the type of a SIDEX_VARIANT value.
 *
 * @param   value the value to check for it's type.
 *
 * @returns The type of the value.<br> 
 * SIDEX_DATA_TYPE_UNKNOWN in case of an error / value of NULL.<br>
 * SIDEX_DATA_TYPE_NONE in case of a None object value.<br>
 * SIDEX_DATA_TYPE_BOOLEAN in case of a Boolean object value.<br>
 * SIDEX_DATA_TYPE_INTEGER in case of an Integer object value.<br>
 * SIDEX_DATA_TYPE_FLOAT in case of a Float object value.<br>
 * SIDEX_DATA_TYPE_DATETIME in case of a DateTime object value.<br>
 * SIDEX_DATA_TYPE_STRING in case of a String object value.<br>
 * SIDEX_DATA_TYPE_BINARY in case of a Binary String object value.<br>
 * SIDEX_DATA_TYPE_LIST in case of a List object value.<br>
 * SIDEX_DATA_TYPE_DICT in case of a Dict object value.<br>
 * SIDEX_DATA_TYPE_TABLE in case of a Table object value.
 *
 * @see sidexStdTypes.h
 */
SIDEX_DATA_TYPE sidexutil_getDataType(SIDEX_VARIANT value);


/**
 * @brief   Returns a copy of a SIDEX_VARIANT.
 *
 * @param   value SIDEX_VARIANT to copy.
 *
 * @returns  a copy of a SIDEX_VARIANT.
 */
SIDEX_VARIANT sidexutil_Variant_Copy (SIDEX_VARIANT value);


/**
 * @brief   Return the number of items in the list.
 *
 * @param   variant The VARIANT_TYPE object interpreted as a List.
 * @param   iSize Reference to the size.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_LIST.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilList_Size(SIDEX_VARIANT variant, SIDEX_INT32* iSize);


/**
 * @brief   Return the number of items in the dictionary.
 *
 * @param   variant The VARIANT_TYPE object interpreted as a Dict.
 * @param   iSize Reference to the size.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Size(SIDEX_VARIANT variant, SIDEX_INT32* iSize);


/**
 * @brief   Return all the keys from the dictionary.
 *
 * @param   variant The VARIANT_TYPE object interpreted as a Dict.
 * @param   sKeys Reference to the keys list.<br>
 *          It's a List object and must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sDict is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Keys(SIDEX_VARIANT variant, SIDEX_VARIANT* sKeys);


/**
 * @brief   Resets pointer to the beginning of the dictionary. Use with sidex_Variant_Dict_Next().
 *
 * @param   variant The VARIANT_TYPE object interpreted as a Dict.
  * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_First(SIDEX_VARIANT variant);


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 *
 * @param   variant   The VARIANT_TYPE object interpreted as a Dict.
 * @param   sKey      Reference to the next key in the dict.
 * @param   nextValue Reference to the next variant in the dict.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Next_A(SIDEX_VARIANT variant, char** sKey, SIDEX_VARIANT* nextValue);


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 *
 * @param   variant   The VARIANT_TYPE object interpreted as a Dict.
 * @param   sKey      Reference to the next key in the dict.
 * @param   nextValue Reference to the next variant in the dict.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Next_X(SIDEX_VARIANT variant, wchar_t** sKey, SIDEX_VARIANT* nextValue);


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 *
 * @param   variant   The VARIANT_TYPE object interpreted as a Dict.
 * @param   sKey      Reference to the next key in the dict.
 * @param   nextValue Reference to the next variant in the dict.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Next_W(SIDEX_VARIANT variant, char16_t** sKey, SIDEX_VARIANT* nextValue);


/**
 * @brief   Check for the existence of the sKey in the dict.
 *
 * @param   variant The VARIANT_TYPE object interpreted as a Dict.
 * @param   sKey    name of the column.
 * @param   bRet    reference to the result of the request.<br>
                    SIDEX_TRUE if the key exists.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_PARAMETER if the sKey is not valid.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_HasKey(SIDEX_VARIANT variant, char* sKey, SIDEX_BOOL* bRet);
/**
 * @brief   Return the number of rows of the table.
 *
 * @param   variant The VARIANT_TYPE object interpreted as a Table.
 * @param   iRows   Reference to the count of rows.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_TABLE.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_Rows(SIDEX_VARIANT variant, SIDEX_INT32* iRows);


/**
 * @brief   Return the number of columns of the table.
 *
 * @param   variant  The VARIANT_TYPE object interpreted as a Table.
 * @param   iColumns Reference to the the count of columns.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_TABLE.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_Columns(SIDEX_VARIANT variant, SIDEX_INT32* iColumns);


/**
 * @brief   Return all the column names from the table.
 *
 * @param   variant The VARIANT_TYPE object interpreted as a Table.
 * @param   varNames Reference to the names list.<br>
 *          It's a List object and must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_TABLE.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_ColumnNames(SIDEX_VARIANT variant, SIDEX_VARIANT* varNames);


/**
 * @brief   Check for the existence of the sColumnName in the table.
 *
 * @param   variant     The VARIANT_TYPE object interpreted as a sidexTable.
 * @param   sColumnName name of the column.
 * @param   bRet        reference to the result of the request.<br>
                        SIDEX_TRUE if the column exists.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if the variant is NULL.<br>
 *          SIDEX_ERR_WRONG_PARAMETER if the sColumnName is not valid.
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_TABLE.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutil_TableHasColumn(SIDEX_VARIANT variant, char* sColumnName, SIDEX_BOOL* bRet);


/**
 * @brief   Interpreting a SIDEX_VARIANT as an boolean value. Returns the bool value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as an boolean.
 * @param   bValue  reference to a SIDEX_BOOL to hold the result.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of the variant is not SIDEX_DATA_TYPE_BOOLEAN.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilBoolean_Value(SIDEX_VARIANT variant, SIDEX_BOOL* bValue);


/**
 * @brief   Interpreting a SIDEX_VARIANT as an integer value. Returns the int value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as an integer.
 * @param   iValue  reference to an int object to hold the result.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of the variant is not SIDEX_DATA_TYPE_INTEGER.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilInteger_Value(SIDEX_VARIANT variant, SIDEX_INT64* iValue);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a float value. Returns the float value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a float.
 * @param   fValue  reference to a double object to hold the result.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of the variant is not SIDEX_DATA_TYPE_FLOAT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilFloat_Value(SIDEX_VARIANT variant, SIDEX_DOUBLE* fValue);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a DateTime value. Returns the DateTime value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a DateTime.
 * @param   dtValue reference to a char array object to hold the result.<br>
 *                  It is owned by the SIDEX_VARIANT and has not to be modified or deallocated.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of the variant is not SIDEX_DATA_TYPE_DATETIME.
 *
 * @see sidexErrors.h, sidexStdTypes.h
 */
int sidexutilDateTime_Value_A(SIDEX_VARIANT variant, char** dtValue);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a DateTime value. Returns the DateTime value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a DateTime.
 * @param   dtValue reference to a char array object to hold the result.<br>
 *                  It is owned by the SIDEX_VARIANT and has not to be modified or deallocated.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of the variant is not SIDEX_DATA_TYPE_DATETIME.
 *
 * @see sidexErrors.h, sidexStdTypes.h
 */
int sidexutilDateTime_Value_W(SIDEX_VARIANT variant, char16_t** dtValue);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a DateTime value. Returns the DateTime value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a DateTime.
 * @param   dtValue reference to a char array object to hold the result.<br>
 *                  It is owned by the SIDEX_VARIANT and has not to be modified or deallocated.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of the variant is not SIDEX_DATA_TYPE_DATETIME.
 *
 * @see sidexErrors.h, sidexStdTypes.h
 */
int sidexutilDateTime_Value_X(SIDEX_VARIANT variant, wchar_t** dtValue);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the char array value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a character array.
 * @param   sValue  reference to a character array object to hold the result.<br>
 *                  It is owned by the SIDEX_VARIANT and has not to be modified or deallocated.
 * @param   iLength reference to the length of the character array object.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_STRING.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilString_Value_A(SIDEX_VARIANT variant, char** sValue, SIDEX_INT32* iLength);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the char array value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a character array.
 * @param   sValue  reference to a character array object to hold the result.<br>
 *                  It is owned by the SIDEX_VARIANT and has not to be modified or deallocated.
 * @param   iLength reference to the length of the character array object.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_STRING.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilString_Value_X(SIDEX_VARIANT variant, wchar_t** sValue, SIDEX_INT32* iLength);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the char array value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a character array.
 * @param   sValue  reference to a character array object to hold the result.<br>
 *                  It is owned by the SIDEX_VARIANT and has not to be modified or deallocated.
 * @param   iLength reference to the length of the character array object.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_STRING.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilString_Value_W(SIDEX_VARIANT variant, char16_t** sValue, SIDEX_INT32* iLength);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the length of the char array value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a character array.
 * @param   iLength reference to the length of the character array object.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_STRING.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilString_Value_Length_A(SIDEX_VARIANT variant, SIDEX_INT32* iLength);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the length of the char array value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a character array.
 * @param   iLength reference to the length of the character array object.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_STRING.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilString_Value_Length_X(SIDEX_VARIANT variant, SIDEX_INT32* iLength);

  
/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the length of the char array value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a character array.
 * @param   iLength reference to the length of the character array object.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_STRING.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilString_Value_Length_W(SIDEX_VARIANT variant, SIDEX_INT32* iLength);

  
/**
 * @brief   Interpreting a SIDEX_VARIANT as a binary char array value. Returns the binary char array value.
 *
 * @param   variant the SIDEX_VARIANT value that will be interpreted as a character array.
 * @param   sValue  reference to a character array object to hold the result.<br>
 *                  It is owned by the SIDEX_VARIANT and has not to be modified or deallocated.
 * @param   iLength reference to the length of the character array object.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_BINARY.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilBinaryString_Value(SIDEX_VARIANT variant, char** sValue, SIDEX_INT32* iLength);


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns it's encoding.
 *
 * @param   variant  the SIDEX_VARIANT value that will be interpreted as a character array.
 * @param   encoding The encoding of the string variant.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of variant is not SIDEX_DATA_TYPE_STRING / SIDEX_DATA_TYPE_BINARY.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilString_Encoding(SIDEX_VARIANT variant, SIDEX_ENCODE* encoding);


 /**
 * @brief   Add an element of type SIDEX_VARIANT to a List. (Subkeys will be created)
 *
 * @param   sList the list (will be checked to be an instance of List).
 * @param   value the value to add.
 * @param   iPos  reference to the position (0 based) of the added element in the list.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sList is not SIDEX_DATA_TYPE_LIST.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilList_Add(SIDEX_VARIANT sList, SIDEX_VARIANT value, SIDEX_INT32* iPos);


/**
 * @brief   Inserts an object of type SIDEX_VARIANT to the list before the given position. (Subkeys will be created)
 *
 * @param   sList the list (will be checked to be an instance of List).
 * @param   value the value to add.
 * @param   pos the position (base 0) in the list.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sList is not SIDEX_DATA_TYPE_LIST.<br>
 *          SIDEX_ERR_WRONG_PARAMETER in case of a negative pos value.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilList_Insert(SIDEX_VARIANT sList, SIDEX_VARIANT value, int pos);


/**
 * @brief   Overwrite an element of type SIDEX_VARIANT in a sidexList.
 *
 * @param   sList the list (will be checked to be an instance of List).
 * @param   value the new value for the overwrite operation.
 * @param   pos the position (base 0) in the list.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sList is not SIDEX_DATA_TYPE_LIST.<br>
 *          SIDEX_ERR_WRONG_PARAMETER in case of a negative pos value.<br>
 *          SIDEX_ERR_NOCONTENT if pos holds no content / pos is invalid.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilList_Set(SIDEX_VARIANT sList, SIDEX_VARIANT value, int pos);


/**
 * @brief   Remove the object out of the list at the given position.
 *
 * @param   sList the list (will be checked to be an instance of List).
 * @param   pos   the position (base 0) in the list.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sList is not SIDEX_DATA_TYPE_LIST.<br>
 *          SIDEX_ERR_WRONG_PARAMETER in case of a negative pos value.<br>
 *          SIDEX_ERR_NOCONTENT if pos holds no content.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilList_DeleteItem(SIDEX_VARIANT sList, int pos);


/**
 * @brief   Removes all objects out of a list.
 *
 * @param   sList the list (will be checked to be an instance of List).
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sList is not SIDEX_DATA_TYPE_LIST.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int  sidexutilList_Clear(SIDEX_VARIANT sList);


/**
 * @brief   Get an element of type SIDEX_VARIANT out of a List.
 *
 * @param   sList   the list (will be checked to be an instance of List).
 * @param   index   0 based index of the variant to get.
 * @param   variant Reference to SIDEX_VARIANT object to get.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sList is not SIDEX_DATA_TYPE_LIST.<br>
 *          SIDEX_ERR_NOCONTENT if there is no value at the requested index.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilList_Get(SIDEX_VARIANT sList, int index, SIDEX_VARIANT* variant);


/**
 * @brief   Set an object of type SIDEX_VARIANT into the Dict referring the sKey
 *
 * @param   sDict the Dict to write into (will be checked to be an instance of Dict).
 * @param   sKey  referring key for the dict entry.
 * @param   value the value to set.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sDict is not SIDEX_DATA_TYPE_DICT.<br>
 *          SIDEX_ERR_DICT_FULL Maximum of entrys reached.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Set(SIDEX_VARIANT sDict, char* sKey, SIDEX_VARIANT value);


/**
 * @brief   Remove the Item referring the sKey out of the dict.
 *
 * @param   sDict the Dict to operate (will be checked to be an instance of Dict).
 * @param   sKey  referring key for the entry to be removed out of the dict.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT if there is no item at the requested index.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sDict is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Delete(SIDEX_VARIANT sDict, char* sKey);


/**
 * @brief   Removes all Items out of a dict.
 *
 * @param   sDict the Dict to operate (will be checked to be an instance of Dict).
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sDict is not SIDEX_DATA_TYPE_DICT.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Clear(SIDEX_VARIANT sDict);


/**
 * @brief   Get an element of type SIDEX_VARIANT out of a Dict.
 *
 * @param   sDict   the dict (will be checked to be an instance of Dict).
 * @param   sKey    referring key of the dict entry to get.
 * @param   variant Reference to SIDEX_VARIANT object to get.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sDict is not SIDEX_DATA_TYPE_DICT.<br>
 *          SIDEX_ERR_NOCONTENT if there is no value at the requested sKey.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilDict_Get(SIDEX_VARIANT sDict, char* sKey, SIDEX_VARIANT* variant);


/**
 * @brief   Add a column to a Table.
 *
 * @param   sTable the table (will be checked to be an instance of Table).
 * @param   sColumn  reference to column name to add.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if there is a type mismatch of SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_WRONG_PARAMETER if sColumn is invalid.<br>
 *          SIDEX_ERR_TABLE_COLUMN_ALREADY_EXISTS if column exists.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_AddColumn(SIDEX_VARIANT sTable, char* sColumn);


/**
 * @brief   Remove a column of a Table.
 *
 * @param   sTable the table (will be checked to be an instance of Table).
 * @param   sColumn  reference to column name to add.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if there is a type mismatch of SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_WRONG_PARAMETER if sColumn is invalid.<br>
 *          SIDEX_ERR_NOCONTENT if column don't exist.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_DeleteColumn(SIDEX_VARIANT sTable, char* sColumn);


/**
 * @brief    Get the column name from a Table at the requested index.
 *
 * @param   sTable the table (will be checked to be an instance of Table).
 * @param   index requested column index  (base 0).
 * @param   sColumn reference to reference for the column name.<br>
 *          It is owned by the sTable and has not to be modified or deallocated.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if there is a type mismatch of SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if column don't exist.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_GetColumnName_A(SIDEX_VARIANT sTable, int index, char** sColumn);


/**
 * @brief    Get the column name from a Table at the requested index.
 *
 * @param   sTable the table (will be checked to be an instance of Table).
 * @param   index requested column index  (base 0).
 * @param   sColumn reference to reference for the column name.<br>
 *          It is owned by the sTable and has not to be modified or deallocated.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if there is a type mismatch of SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if column don't exist.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_GetColumnName_X(SIDEX_VARIANT sTable, int index, wchar_t** sColumn);


/**
 * @brief    Get the column name from a Table at the requested index.
 *
 * @param   sTable the table (will be checked to be an instance of Table).
 * @param   index requested column index  (base 0).
 * @param   sColumn reference to reference for the column name.<br>
 *          It is owned by the sTable and has not to be modified or deallocated.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if there is a type mismatch of SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if column don't exist.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_GetColumnName_W(SIDEX_VARIANT sTable, int index, char16_t** sColumn);


/**
 * @brief   Add a new empty row to a Table.
 *
 * It is possible to add empty rows before any column definition, but<br>
 * sidexutilTable_GetRow() will return SIDEX_ERR_NOCONTENT in case of a request referring that empty row(s),<br>
 * until the first column has been defined by sidexutilTable_AddColumn().<br>
 * sidexutilTable_Rows() will return the amount of empty rows, or the amount of defined rows, if the first column has been defined by sidexutilTable_AddColumn().
 *
 * @param   sTable the table (will be checked to be an instance of Table).
 * @param   iRowIdx reference to the index for the new added row.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if there is a type mismatch of SIDEX_DATA_TYPE_TABLE or value.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_AddRow(SIDEX_VARIANT sTable, SIDEX_INT32* iRowIdx);


/**
 * @brief   Remove    the row referring the rowIndex out of the table.
 *
 * @param   sTable    the table (will be checked to be an instance of Table).
 * @param   rowIndex  0 based index of the row to be removed out of the table.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sTable is not SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if there is no value at the requested rowIndex.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_DeleteRow(SIDEX_VARIANT sTable, int rowIndex);


/**
 * @brief   Remove all rows out of the table.
 *
 * Also empty rows will be removed - see sidexutilTable_AddRow().
 *
 * @param   sTable    the table (will be checked to be an instance of Table).
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sTable is not SIDEX_DATA_TYPE_TABLE.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_DeleteRows(SIDEX_VARIANT sTable);


/**
 * @brief   Get row of type SIDEX_VARIANT out of a Table.
 *
 * @param   sTable   the table (will be checked to be an instance of Table).
 * @param   rowIndex 0 based index of the row to get.
 * @param   variant  Reference to SIDEX_VARIANT object for the row to get.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sTable is not SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if there is no value at the requested index.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_GetRow(SIDEX_VARIANT sTable, int rowIndex, SIDEX_VARIANT* variant);


/**
 * @brief   Get a table field of type SIDEX_VARIANT out of a Table.
 *
 * @param   sTable      the table (will be checked to be an instance of Table).
 * @param   rowIndex    0 based index of the row for the requested field.
 * @param   sColumnName Column name  for the requested field.
 * @param   variant     Reference to SIDEX_VARIANT object for the field entry.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sTable is not SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if there is no value at the requested field.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_GetField(SIDEX_VARIANT sTable, int rowIndex, char* sColumnName, SIDEX_VARIANT* variant);


/**
 * @brief   Set a table field of type SIDEX_VARIANT in a Table.
 *
 * @param   sTable      the table (will be checked to be an instance of Table).
 * @param   rowIndex    0 based index of the row for the requested field.
 * @param   sColumnName Column name  for the requested field.
 * @param   variant     SIDEX_VARIANT object for the field entry.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE if the type of sTable is not SIDEX_DATA_TYPE_TABLE.<br>
 *          SIDEX_ERR_NOCONTENT if rowIndex / sColumnName does not exist.
 *
 * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
 */
int sidexutilTable_SetField(SIDEX_VARIANT sTable, int rowIndex, char* sColumnName, SIDEX_VARIANT variant);

/**
 * @brief   counting the characters (not code points) of a UTF-16 string
 *
 * @param   strarg      UTF-16 string
 *
 * @returns number of characters
 */
int strlen16(const char16_t* strarg);


#endif // SIDEXUTILS_H
