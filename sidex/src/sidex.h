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
 
//-----------------------------------------
// in case of an optional group (may be NULL) the following
// define has to be set to a value.
// If group is mandatory the define has to be set into comments.
//#define GROUP_IS_OPTIONAL 1

//-----------------------------------------
// write here package with @ to get namespace TAB in the html- documentation
//-----------------------------------------

/** package  SIDEX
  */

/** @file sidex.h
  * 
  * @brief import of SIDEX API
  *
  */

/** @defgroup sdxAPI SIDEX API
  * @brief SIDEX API reference
	*/
	
/** @defgroup general General
  * @ingroup sdxAPI
  * @brief General SIDEX API functions.
  */

/** @ingroup general
  * @defgroup sidexReturncodes SIDEX return codes
  * @brief Return codes of SIDEX API functions.
  */

/** @ingroup general
  * @defgroup sidexTypes Type definitions
  * @brief Type definitions used by SIDEX API functions.
  */

/** @ingroup general
  * @defgroup sidexConstants Constant definitions
  * @brief Constants used by SIDEX API functions.
  */

/** @defgroup sidexHandle SIDEX document
  * @ingroup sdxAPI
  * @brief Handling of SIDEX documents
  */

/** @ingroup sidexHandle
  * @defgroup navigationType Group/Key managment
  * @brief Managing groups and keys in SIDEX documents
  */

/** @ingroup sidexHandle
  * @defgroup sdxDocRWValues Read/Write values
  * @brief Read/Write values from/to the SIDEX document.
  */

/** @defgroup datatypes SIDEX datatypes
  * @ingroup sdxAPI
  * @brief SIDEX API for supported datatypes
  */

/** @ingroup datatypes
  * @defgroup varianttype General variant functions
  * @brief General functions to handle data objects.
  */

/** @ingroup datatypes
  * @defgroup simpletypes Simple data types
  * @brief Simple data objects.
  */

/** @ingroup datatypes
  * @defgroup complextypes container types
  * @brief Datatypes containing multiple variant values.
  */

/** @ingroup simpletypes
  * @defgroup inttype Integer
  * @brief Handling of integer values
  */

/** @ingroup simpletypes
  * @defgroup nonetype None
  * @brief Handling of None values
  */

/** @ingroup simpletypes
  * @defgroup booltype Boolean
  * @brief Handling of Boolean values
  */

/** @ingroup simpletypes
  * @defgroup floattype Float
  * @brief Handling of Float values
  */

/** @ingroup simpletypes
  * @defgroup stringtype String
  * @brief Handling of String values
  */

/** @ingroup simpletypes
  * @defgroup bintype Binary
  * @brief Handling of Binary data
  */

/** @ingroup simpletypes
  * @defgroup datetimetype DateTime
  * @brief Handling of DateTime values
  */

/** @ingroup complextypes
  * @defgroup listtype List
  * @brief A list of values.
  */

/** @ingroup complextypes
  * @defgroup dicttype Dictionary
  * @brief Values organized in a dictionary with a string key.
  */

/** @ingroup complextypes
  * @defgroup tabletype Table
  * @brief A table with rows and columns.
  */

  
#ifndef SIDEX_H
#define SIDEX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globalDefines.h"
#include "sidexStdTypes.h"
#include "sidexErrors.h"

#ifndef DOXYGEN_GENERATION
#ifdef LINUX
  #define SIDEX_API __attribute__ ((__visibility__("default")))
#else
  #ifdef SIDEX_EXPORTS
    /**
    * @brief    Definition for export of the dll
    */
    #define SIDEX_API __declspec(dllexport)
  #else
    /**
    * @brief    Definition for import of the dll
    */
    #define SIDEX_API __declspec(dllimport)
  #endif
#endif
#endif // DOXYGEN_GENERATION

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  general
 * @brief    Returns current interface api and library version.
 *
 * @param    apiVer api version
 * @param    libVer library version
 * @param    verStr library version as a string<br>
 *           This is a borrowed reference and has not to be freed or modified.
 *
 * @see sidex_Get_Copyright()
 */
SIDEX_API void DLL_CALL_CONV sidex_Get_Version(SIDEX_INT32* apiVer, SIDEX_INT32* libVer, SIDEX_CTSTR** verStr);
SIDEX_API void DLL_CALL_CONV sidex_Get_Version_X(SIDEX_INT32* apiVer, SIDEX_INT32* libVer, wchar_t** verStr);
SIDEX_API void DLL_CALL_CONV sidex_Get_Version_W(SIDEX_INT32* apiVer, SIDEX_INT32* libVer, char16_t** verStr);
SIDEX_API void DLL_CALL_CONV sidex_Get_Version_A(SIDEX_INT32* apiVer, SIDEX_INT32* libVer, char** verStr);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Get_Version sidex_Get_Version_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Get_Version sidex_Get_Version_W
    #else
      #define sidex_Get_Version sidex_Get_Version_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup general
 * @brief   Returns the copyright information.
 *
 * The copyright information contains all hints and links concerning the LGPL related
 * libraries used in this SDK.
 *
 * @param   sValue  copyright information<br>
 *          This is a borrowed reference and has not to be freed or modified.
 * @param   iLength length of the copyright information
 *
 * @see sidex_Get_Version()
 */
SIDEX_API void DLL_CALL_CONV sidex_Get_Copyright(SIDEX_CTSTR** sValue, SIDEX_INT32* iLength);
SIDEX_API void DLL_CALL_CONV sidex_Get_Copyright_X(wchar_t** sValue, SIDEX_INT32* iLength);
SIDEX_API void DLL_CALL_CONV sidex_Get_Copyright_W(char16_t** sValue, SIDEX_INT32* iLength);
SIDEX_API void DLL_CALL_CONV sidex_Get_Copyright_A(char** sValue, SIDEX_INT32* iLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Get_Copyright  sidex_Get_Copyright_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Get_Copyright  sidex_Get_Copyright_W
    #else
      #define sidex_Get_Copyright  sidex_Get_Copyright_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup general
 * @brief    Set the license key.
 *
 * Before the sdk functions can be used a license key must be applied.
 * @note Since TML/SIDEX is open source this API call is deprecated and isn't necessary. It returns allways SIDEX_SUCCESS.<br>
 *       It has been left in the API to be downward compatible.
 *
 * @param    pUserName   user name (case sensitive)
 * @param    pPassWord   password (case insensitive)
 *
 * @returns  SIDEX_SUCCESS in case of success<br>
 *           SIDEX_ERR_UNICODE error in unicode conversion
 */

SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Password(const SIDEX_CTSTR* pUserName, const SIDEX_CTSTR* pPassWord);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Password_X(const wchar_t* pUserName, const wchar_t* pPassWord);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Password_W(const char16_t* pUserName, const char16_t* pPassWord);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Password_A(const char* pUserName, const char* pPassWord);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Set_Password  sidex_Set_Password_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Set_Password  sidex_Set_Password_W
    #else
      #define sidex_Set_Password  sidex_Set_Password_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/


/**
 * @ingroup sidexHandle
 * @brief    Create a SIDEX document handle.
 *
 * SIDEX documents contain data based on SIDEX data objects organized in groups containing a number of keys.
 * Groups and keys are names to find a specific value in the document. The document can be exported to/imported from an
 * xml string or file.
 *
 * @param    pname   name of the document/xml root node
 * @param    shandle SIDEX document handle (SIDEX_HANDLE)<br>
 *           Use sidex_Free() to deallocate handle.  
 *
 * @returns SIDEX_SUCCESS  in case of success<br>
 *          SIDEX_ERR_WRONG_DOC pname is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty)<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_WRONG_PARAMETER pname is NULL
 *
 * @see sidex_Free()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Create(const SIDEX_CTSTR* pname, SIDEX_HANDLE* shandle);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Create_X(const wchar_t* pname, SIDEX_HANDLE* shandle);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Create_W(const char16_t* pname, SIDEX_HANDLE* shandle);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Create_A(const char* pname, SIDEX_HANDLE* shandle);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Create  sidex_Create_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Create  sidex_Create_W
    #else
      #define sidex_Create  sidex_Create_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sidexHandle
 * @brief    Change the document name / XML root node.
 *
 * @param    shandle SIDEX document handle (SIDEX_HANDLE)
 * @param    pname   name of the document/xml root node
 *
 * @returns  SIDEX_SUCCESS in case of success<br>
 *           SIDEX_ERR_WRONG_DOC pname is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty)<br>
 *           SIDEX_ERR_UNICODE error in unicode conversion<br>
 *           SIDEX_ERR_WRONG_PARAMETER invalid parameter passed
 *
 * @see sidex_Get_DocumentName()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_DocumentName(SIDEX_HANDLE shandle, const SIDEX_CTSTR pname);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_DocumentName_X(SIDEX_HANDLE shandle, const wchar_t* pname);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_DocumentName_W(SIDEX_HANDLE shandle, const char16_t* pname);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_DocumentName_A(SIDEX_HANDLE shandle, const char* pname);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Set_DocumentName  sidex_Set_DocumentName_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Set_DocumentName  sidex_Set_DocumentName_W
    #else
      #define sidex_Set_DocumentName  sidex_Set_DocumentName_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sidexHandle
 * @brief    Get the document name / XML root node name.
 *
 * @param    shandle SIDEX document handle (SIDEX_HANDLE)
 * @param    pname   name of the document/xml root node<br>
 *           Borrowed reference (owned by the SIDEX document object) and has not to be freed or modified.
 *           
 * @returns  SIDEX_SUCCESS in case of success<br>
 *           SIDEX_ERR_UNICODE error in unicode conversion<br>
 *           SIDEX_ERR_WRONG_PARAMETER invalid parameter passed
 *
 * @see sidex_Set_DocumentName(), sidex_Load_Content(), sidex_Set_Content()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_DocumentName(SIDEX_HANDLE shandle, SIDEX_CTSTR* pname);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_DocumentName_X(SIDEX_HANDLE shandle, wchar_t** pname);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_DocumentName_W(SIDEX_HANDLE shandle, char16_t** pname);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_DocumentName_A(SIDEX_HANDLE shandle, char** pname);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Get_DocumentName  sidex_Get_DocumentName_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Get_DocumentName  sidex_Get_DocumentName_W
    #else
      #define sidex_Get_DocumentName  sidex_Get_DocumentName_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sidexHandle
 * @brief    Remove all values from the sidex document.
 *
 * @param    shandle SIDEX document handle (SIDEX_HANDLE)
 */
SIDEX_API void DLL_CALL_CONV sidex_Clear(SIDEX_HANDLE shandle);


/**
 * @ingroup sidexHandle
 * @brief    Free a SIDEX document object.
 *
 * @param    shandle reference to SIDEX document handle (SIDEX_HANDLE)
 *
 * @see sidex_Create()
 */
SIDEX_API void DLL_CALL_CONV sidex_Free(SIDEX_HANDLE* shandle);


/**
 * @ingroup sidexHandle
 * @brief Read a SIDEX document from a file.
 *
 * @note If the document handle already contains data it will be deleted.
 *
 * @param shandle SIDEX document handle (SIDEX_HANDLE)
 * @param path    path to SIDEX file
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_DUMPCONTENT I/O error<br>
 *          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion
 *
 * @see sidex_Set_Content(), sidex_Save_Content()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Load_Content(SIDEX_HANDLE shandle, const SIDEX_CTSTR *path);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Load_Content_X(SIDEX_HANDLE shandle, const wchar_t *path);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Load_Content_W(SIDEX_HANDLE shandle, const char16_t *path);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Load_Content_A(SIDEX_HANDLE shandle, const char *path);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Load_Content  sidex_Load_Content_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Load_Content  sidex_Load_Content_W
    #else
      #define sidex_Load_Content  sidex_Load_Content_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sidexHandle
 * @brief Read a SIDEX document from a string.
 *
 * @note If the document handle already contains data it will be deleted.
 *
 * @param shandle SIDEX document handle (SIDEX_HANDLE)
 * @param content SIDEX document data
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_DUMPCONTENT I/O error<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion
 *
 * @see sidex_Load_Content(), sidex_Get_Content()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Content(SIDEX_HANDLE shandle, SIDEX_CTSTR *content);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Content_X(SIDEX_HANDLE shandle, wchar_t *content);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Content_W(SIDEX_HANDLE shandle, char16_t *content);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Content_A(SIDEX_HANDLE shandle, char *content);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Set_Content  sidex_Set_Content_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Set_Content  sidex_Set_Content_W
    #else
      #define sidex_Set_Content  sidex_Set_Content_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sidexHandle
 * @brief Write a SIDEX document to a file.
 *
 * @note If the file exits, it will be overwritten.
 *
 * @param shandle SIDEX document handle (SIDEX_HANDLE)
 * @param path    file path and name
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_DUMPCONTENT I/O error
 *
 * @see sidex_Load_Content()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Save_Content(SIDEX_HANDLE shandle, const SIDEX_CTSTR *path);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Save_Content_X(SIDEX_HANDLE shandle, const wchar_t *path);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Save_Content_W(SIDEX_HANDLE shandle, const char16_t *path);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Save_Content_A(SIDEX_HANDLE shandle, const char *path);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Save_Content  sidex_Save_Content_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Save_Content  sidex_Save_Content_W
    #else
      #define sidex_Save_Content  sidex_Save_Content_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sidexHandle
 * @brief Get the SIDEX document data as string.
 *
 * @param    shandle        SIDEX document handle (SIDEX_HANDLE)
 * @param    content        reference to the SIDEX document data as string<br>
 *                          It has to be freed using sidex_Free_Content().
 * @param    iContentLength reference to the content string length
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_DUMPCONTENT I/O error
 *
 * @see sidex_Set_Content()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content(SIDEX_HANDLE shandle, SIDEX_CTSTR** content, SIDEX_INT32* iContentLength); 
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content_X(SIDEX_HANDLE shandle, wchar_t** content, SIDEX_INT32* iContentLength); 
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content_W(SIDEX_HANDLE shandle, char16_t** content, SIDEX_INT32* iContentLength); 
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content_A(SIDEX_HANDLE shandle, char** content, SIDEX_INT32* iContentLength); 
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Get_Content  sidex_Get_Content_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Get_Content  sidex_Get_Content_W
    #else
      #define sidex_Get_Content  sidex_Get_Content_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sidexHandle
 * @brief Get the buffer length of the document content returned by sidex_Get_Content().
 *
 * @param    shandle        SIDEX document handle (SIDEX_HANDLE)
 * @param    iContentLength Reference to the length for a sidex_Get_Content().
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_DUMPCONTENT I/O error
 *
 * @see sidex_Get_Content()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content_Length(SIDEX_HANDLE shandle, SIDEX_INT32* iContentLength); 


/**
 * @ingroup sidexHandle
 * @brief Free an allocated string returnd by sidex_Get_Content()
 *
 * @param    string         reference to the SIDEX document data as string
 *
 * @see sidex_Get_Content_Length(), sidex_Get_Content()
 */
SIDEX_API void DLL_CALL_CONV sidex_Free_Content(SIDEX_CTSTR *string); 
SIDEX_API void DLL_CALL_CONV sidex_Free_Content_X(wchar_t *string); 
SIDEX_API void DLL_CALL_CONV sidex_Free_Content_W(char16_t *string); 
SIDEX_API void DLL_CALL_CONV sidex_Free_Content_A(char *string); 
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Free_Content  sidex_Free_Content_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Free_Content  sidex_Free_Content_W
    #else
      #define sidex_Free_Content  sidex_Free_Content_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup varianttype
 * @brief   Decrement the reference counter of a SIDEX_VARIANT data object.
 *
 *          A reference counter value of 0 signals that there is no more reference to the object.<br>
 *          In this case the SIDEX memory management will free the object.<br>
 *
 * @param   variant SIDEX_VARIANT handle
 *
 * @returns SIDEX_SUCCESS on success<br>
 *          SIDEX_ERR_COMMON unknown exception
 *
 * @see sidex_Variant_IncRef()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_DecRef(SIDEX_VARIANT variant);


/**
 * @ingroup varianttype
 * @brief   Increment the reference counter of a SIDEX_VARIANT data object.
 *
 *          If a new reference to the SIDEX_VARIANT handle is created,
 *          this function must be called. Many of the SIDEX API functions increment or
 *          decrement the reference counter automatically. Please refer the function description.
 *
 * @param   variant SIDEX_VARIANT handle
 *
 * @see sidex_Variant_DecRef()
 */
SIDEX_API void DLL_CALL_CONV sidex_Variant_IncRef(SIDEX_VARIANT variant);


/// @ingroup sdxDocRWValues
/// @brief   Write a SIDEX_VARIANT to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / optional NULL if you don't want to specify a group
/// @param   nkey    name of the key
/// @param   variant SIDEX_VARIANT handle
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   variant SIDEX_VARIANT handle
///
/// @returns SIDEX_SUCCESS in case of success.<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY invalid key name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty)<br>
///          SIDEX_ERR_WRONG_GROUP invalid group name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty)<br> 
///          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
///          SIDEX_ERR_UNICODE error in unicode conversion
///
/// @see sidex_Variant_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT variant);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Write  sidex_Variant_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Write  sidex_Variant_Write_W
    #else
      #define sidex_Variant_Write  sidex_Variant_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Read a value from a SIDEX document and return a SIDEX_VARIANT.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / optional NULL if you don't want to specify a group
/// @param   nkey    name of the key
/// @param   variant SIDEX_VARIANT handle reference<br>
///                  This call increments the reference counter of the SIDEX_VARIANT.<br>
///                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   variant SIDEX_VARIANT handle reference<br>
///                  This call increments the reference counter of the SIDEX_VARIANT.<br>
///                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
///
/// @returns SIDEX_SUCCESS in case of success.<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY invalid key name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty)<br>
///          SIDEX_ERR_WRONG_GROUP invalid group name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty)<br> 
///          SIDEX_ERR_INVALID_BOOLEAN boolean value error<br>
///          SIDEX_ERR_INVALID_INTEGER integer out of range<br>
///          SIDEX_ERR_INVALID_FLOAT float out of range<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT nkey / ngroup has no reference to a value
///
/// @see sidex_Variant_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Read(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Read_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Read_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Read_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT* variant);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Read  sidex_Variant_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Read  sidex_Variant_Read_W
    #else
      #define sidex_Variant_Read  sidex_Variant_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup nonetype
 * @brief   Create a SIDEX_VARIANT of type none.
 *
 * @returns SIDEX_VARIANT handle<br>
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_None();


/**
 * @ingroup booltype
 * @brief   Create a SIDEX_VARIANT of type boolean.
 *
 * @param   value boolean value (0,1)
 *
 * @returns SIDEX_VARIANT handle<br>
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Boolean(SIDEX_BOOL value);


/**
 * @ingroup inttype
 * @brief   Create a SIDEX_VARIANT of type integer.
 *
 * @param   value integer value
 *
 * @returns SIDEX_VARIANT handle<br>
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Integer(SIDEX_INT64 value);


/**
 * @ingroup floattype
 * @brief   Create a SIDEX_VARIANT of type float.
 *
 * @param   value float value
 *
 * @returns SIDEX_VARIANT handle<br>
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Float(SIDEX_DOUBLE value);


/**
 * @ingroup datetimetype
 * @brief   Create a SIDEX_VARIANT of type datetime from a string.
 *
 *          The value must be formatted according to the specification:<br>
 *          YYYY-MM-DD hh:mm:ss:ttt <br><br>
 *          YYYY year (0001-9999 / 0000 for undefined)<br>
 *          MM monst (01-12 / 00 for undefined)<br>
 *          DD day of month (01-31  / 00 for undefined)<br>
 *          hh hour (00-23)<br>
 *          mm minutes (00-59)<br>
 *          ss seconds (00-59)<br>
 *          ttt milliseseconds (000-999)<br>
 *          The date attributes may also be undefined. In that case year, month and day must be set to 0.
 *
 * @param   sDateTime Formatted string for the DateTime value<br>
 * @param   variant SIDEX_VARIANT handle reference<br> 
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_INVALID_DATETIME invalid date/time format
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_DateTime(char* sDateTime, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_DateTime_X(wchar_t* sDateTime, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_DateTime_W(char16_t* sDateTime, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_DateTime_A(char* sDateTime, SIDEX_VARIANT* variant);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_New_DateTime  sidex_Variant_New_DateTime_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_New_DateTime  sidex_Variant_New_DateTime_W
    #else
      #define sidex_Variant_New_DateTime  sidex_Variant_New_DateTime_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup stringtype
 * @brief   Set a string format attribute.
 *
 * A string can be formatted in a special way. The format attribute allows to
 * store this format information with the string.
 *
 * Valid formats: 
 *
 *  <TABLE>
 *   <TR><TD><B>Name       </B></TD><TD><B>Description  </B></TD></TR>
 *   <TR><TD>unknown           </TD><TD>no special format (default)</TD></TR>
 *   <TR><TD>sidex           </TD><TD>SIDEX formatted string</TD></TR>
 *   <TR><TD>xml             </TD><TD>XML formatted string</TD></TR>
 *   <TR><TD>json            </TD><TD>JSON formatted string</TD></TR>
 *   <TR><TD>cvs             </TD><TD>comma seperated fields</TD></TR>
 *   <TR><TD>ini             </TD><TD>window ini format</TD></TR>
 *  </TABLE>
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   format format of the string<br>
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE SIDEX variant is not a string<br>
 *          SIDEX_ERR_WRONG_ENCODING_FORMAT the format attribute is not supported<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion
 *
 * @see sidex_Variant_String_GetFormat()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_SetFormat(SIDEX_VARIANT variant, const SIDEX_CTSTR* format);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_SetFormat_X(SIDEX_VARIANT variant, const wchar_t* format);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_SetFormat_W(SIDEX_VARIANT variant, const char16_t* format);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_SetFormat_A(SIDEX_VARIANT variant, const char* format);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_String_SetFormat  sidex_Variant_String_SetFormat_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_String_SetFormat  sidex_Variant_String_SetFormat_W
    #else
      #define sidex_Variant_String_SetFormat  sidex_Variant_String_SetFormat_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup stringtype
 * @brief   Get a string format attribute.
 *
 * See sidex_Variant_String_SetFormat() for further reference.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   format reference to the format of the string<br>
 *          This is a borrowed reference (owned by the string) and has not to be freed or modified.
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE SIDEX variant is not a string<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion
 *
 * @see sidex_Variant_String_SetFormat()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_GetFormat(SIDEX_VARIANT variant, const SIDEX_CTSTR** format);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_GetFormat_X(SIDEX_VARIANT variant, const wchar_t** format);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_GetFormat_W(SIDEX_VARIANT variant, const char16_t** format);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_GetFormat_A(SIDEX_VARIANT variant, const char** format);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_String_GetFormat  sidex_Variant_String_GetFormat_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_String_GetFormat  sidex_Variant_String_GetFormat_W
    #else
      #define sidex_Variant_String_GetFormat  sidex_Variant_String_GetFormat_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup stringtype
 * @brief   Create a SIDEX_VARIANT of type string.
 *
 * @param   value       string value or SIDEX_HANDLE_TYPE_NULL for an empty string
 * @param   variant SIDEX_VARIANT handle reference<br> 
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_String(SIDEX_CTSTR* value, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_String_X(wchar_t* value, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_String_W(char16_t* value, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_String_A(char* value, SIDEX_VARIANT* variant);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_New_String  sidex_Variant_New_String_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_New_String  sidex_Variant_New_String_W
    #else
      #define sidex_Variant_New_String  sidex_Variant_New_String_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup bintype
 * @brief   Create a SIDEX_VARIANT of type binary from a char array.
 *
 * @param   value       binary value or SIDEX_HANDLE_TYPE_NULL for an empty string
 * @param   iStrLength  Length of the binary char array 
 * @param   variant SIDEX_VARIANT handle reference<br> 
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_Binary(unsigned char* value, SIDEX_INT32 iStrLength, SIDEX_VARIANT* variant);


/**
 * @ingroup listtype
 * @brief   Create a SIDEX_VARIANT of type list.
 *
 * @returns   variant SIDEX_VARIANT handle<br> 
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_List();


/**
 * @ingroup dicttype
 * @brief   Create a SIDEX_VARIANT of type dictionary.
 *
 * @returns   variant SIDEX_VARIANT handle<br> 
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 * @see sidex_Variant_New_DictBySize()
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Dict();


/**
 * @ingroup dicttype
 * @brief   Create a SIDEX_VARIANT of type dictionary with a predefined size.
 *
 * @param   iSize  number of dictionary entries
 *
 * @returns   variant SIDEX_VARIANT handle<br> 
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 *
 * @see sidex_Variant_New_Dict()
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_DictBySize(SIDEX_INT32 iSize);


/**
 * @ingroup tabletype
 * @brief   Create a SIDEX_VARIANT of type table.
 *
 * @returns   variant SIDEX_VARIANT handle<br> 
 *                  This call increments the reference counter of the SIDEX_VARIANT.<br>
 *                  It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Table();


/**
 * @ingroup varianttype
 * @brief   Returns the type of a SIDEX_VARIANT value.
 *
 *  <TABLE>
 *   <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_UNKNOWN </TD><TD>0 </TD><TD>error or value of SIDEX_HANDLE_TYPE_NULL</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_NONE    </TD><TD>1 </TD><TD>None type</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_BOOLEAN </TD><TD>2 </TD><TD>boolean value</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_INTEGER </TD><TD>3 </TD><TD>integer value</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_FLOAT   </TD><TD>4 </TD><TD>float value</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_STRING  </TD><TD>5 </TD><TD>string value</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_LIST    </TD><TD>6 </TD><TD>a list of values</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_DICT    </TD><TD>7 </TD><TD>a dictionary</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_TABLE   </TD><TD>8 </TD><TD>a table</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_DATETIME</TD><TD>9 </TD><TD>date and time</TD></TR>
 *   <TR><TD>SIDEX_DATA_TYPE_BINARY  </TD><TD>10 </TD><TD>binary string</TD></TR>
 *  </TABLE>
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns type of the value<br> 
 */
SIDEX_API SIDEX_DATA_TYPE DLL_CALL_CONV sidex_Variant_GetType (SIDEX_VARIANT value);


/**
 * @ingroup varianttype
 * @brief   Returns a copy of a SIDEX_VARIANT.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns  a copy of a SIDEX_VARIANT handle on success or SIDEX_HANDLE_TYPE_NULL on error
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_Copy (SIDEX_VARIANT value);


/**
 * @ingroup nonetype
 * @brief   Check if SIDEX_VARIANT type is none.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_None_Check(SIDEX_VARIANT value);


/**
 * @ingroup booltype
 * @brief   Check if SIDEX_VARIANT type is boolean.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Boolean_Check(SIDEX_VARIANT value);


/**
 * @ingroup inttype
 * @brief   Check if SIDEX_VARIANT type is integer.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Integer_Check(SIDEX_VARIANT value);


/**
 * @ingroup floattype
 * @brief   Check if SIDEX_VARIANT type is float.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Float_Check(SIDEX_VARIANT value);


/**
 * @ingroup datetimetype
 * @brief   Check if SIDEX_VARIANT type is datetime.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_DateTime_Check(SIDEX_VARIANT value);


/**
 * @ingroup stringtype
 * @brief   Check if SIDEX_VARIANT type is string.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_String_Check(SIDEX_VARIANT value);


/**
 * @ingroup bintype
 * @brief   Check if SIDEX_VARIANT type is binary.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Binary_Check(SIDEX_VARIANT value);


/**
 * @ingroup listtype
 * @brief   Check if SIDEX_VARIANT type is list.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_List_Check(SIDEX_VARIANT value);


/**
 * @ingroup dicttype
 * @brief   Check if SIDEX_VARIANT type is dictionary.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Dict_Check(SIDEX_VARIANT value);


/**
 * @ingroup tabletype
 * @brief   Check if SIDEX_VARIANT type is table.
 *
 * @param   value SIDEX_VARIANT handle
 *
 * @returns SIDEX_TRUE/SIDEX_FALSE
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Table_Check(SIDEX_VARIANT value);


/**
 * @ingroup listtype
 * @brief   Get the number of items in a List.
 *
 * @param   sList SIDEX_VARIANT handle
 * @param   iSize reference to item count
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_TYPE sList is not a list
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Size(SIDEX_VARIANT sList, SIDEX_INT32* iSize);


/**
 * @ingroup dicttype
 * @brief   Get the number of entries in a dictionary.
 *
 * @param   sDict SIDEX_VARIANT handle
 * @param   iSize reference to entry count
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT sDict is SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Size(SIDEX_VARIANT sDict, SIDEX_INT32* iSize);


/**
 * @ingroup dicttype
 * @brief   Get all key names of a dictionary.
 *
 * @param   sDict SIDEX_VARIANT handle
 * @param   sKeys reference to a SIDEX_VARIANT handle<br>
 *          The list contains all keys in the dictionary<br>
 *          and must be released by sidex_Variant_DecRef().
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT sDict is SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary
 *
 * @see sidex_Variant_Dict_HasKey()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Keys(SIDEX_VARIANT sDict, SIDEX_VARIANT* sKeys);


/**
 * @ingroup dicttype
 * @brief   Set the cursor to the first element of a dictionary.
 *
 * @param   sDict SIDEX_VARIANT handle
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT sDict is SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary
 *
 * @see sidex_Variant_Dict_Next()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_First(SIDEX_VARIANT sDict);


/**
 * @ingroup dicttype
 * @brief   Get the value from a dictionary at the current cursor position and move the cursor to the next value.
 *
 * @param   sDict   SIDEX_VARIANT handle
 * @param   sKey    Reference to the next key in the dictionary.<br>
 *                  This is a borrowed reference (owned by the dictionary) and has not to be freed or modified.
 * @param   vNext   Borrowed reference to the next value of the dictionary<br>
 *                  SIDEX_HANDLE_TYPE_NULL if the end of the dictionary was reached.
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary<br>
 *          SIDEX_ERR_FIRST_NEXT missing sidex_Variant_Dict_First() or<br>
 *                               if sidex_Variant_Dict_Set() / sidex_Variant_Dict_Delete() / sidex_Variant_Dict_Clear() was called after sidex_Variant_Dict_First().<br>
 *          SIDEX_ERR_NOCONTENT SIDEX_HANDLE_TYPE_NULL or the end of the dictionary was reached.
 *
 * @see sidex_Variant_Dict_First()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Next(SIDEX_VARIANT sDict, SIDEX_CTSTR** sKey, SIDEX_VARIANT* vNext);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Next_X(SIDEX_VARIANT sDict, wchar_t** sKey, SIDEX_VARIANT* vNext);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Next_W(SIDEX_VARIANT sDict, char16_t** sKey, SIDEX_VARIANT* vNext);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Next_A(SIDEX_VARIANT sDict, char** sKey, SIDEX_VARIANT* vNext);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Dict_Next sidex_Variant_Dict_Next_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Dict_Next sidex_Variant_Dict_Next_W
    #else
      #define sidex_Variant_Dict_Next sidex_Variant_Dict_Next_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup dicttype
 * @brief   Check if a key exists in a dictionary.
 *
 * @param   sDict   SIDEX_VARIANT handle
 * @param   sKey    key name
 * @param   bRet    reference to the result<br>
 *                  SIDEX_TRUE if the key exists
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_NOCONTENT sDict is SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary
 *
 * @see sidex_Variant_Dict_Keys()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_HasKey(SIDEX_VARIANT sDict, SIDEX_CTSTR* sKey, SIDEX_BOOL* bRet);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_HasKey_X(SIDEX_VARIANT sDict, wchar_t* sKey, SIDEX_BOOL* bRet);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_HasKey_W(SIDEX_VARIANT sDict, char16_t* sKey, SIDEX_BOOL* bRet);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_HasKey_A(SIDEX_VARIANT sDict, char* sKey, SIDEX_BOOL* bRet);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Dict_HasKey  sidex_Variant_Dict_HasKey_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Dict_HasKey  sidex_Variant_Dict_HasKey_W
    #else
      #define sidex_Variant_Dict_HasKey  sidex_Variant_Dict_HasKey_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tabletype
 * @brief   Get the number of rows of a table.
 *
 * @param   sTable  SIDEX_VARIANT handle
 * @param   iRows reference to row count
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_NOCONTENT sTable is SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table
 *
 * @see sidex_Variant_Table_Columns()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_Rows(SIDEX_VARIANT sTable, SIDEX_INT32* iRows);


/**
 * @ingroup tabletype
 * @brief   Get the number of columns of a table.
 *
 * @param   sTable  SIDEX_VARIANT handle
 * @param   iColumns reference to column count
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_NOCONTENT sTable is SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table
 *
 * @see sidex_Variant_Table_Rows(), sidex_Variant_Table_ColumnNames()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_Columns(SIDEX_VARIANT sTable, SIDEX_INT32* iColumns);


/**
 * @ingroup tabletype
 * @brief   Get column names of a table.
 *
 * @param   sTable  SIDEX_VARIANT handle
 * @param   varNames reference to a SIDEX_VARIANT handle
 *          The list contains all column names of the table<br>
 *          and must be released by sidex_Variant_DecRef().
 *
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_NOCONTENT sTable is SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table
 *
 * @see sidex_Variant_Table_Columns(), sidex_Variant_Table_HasColumn()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_ColumnNames(SIDEX_VARIANT sTable, SIDEX_VARIANT* varNames);


/**
 * @ingroup tabletype
 * @brief   Check if a column name exists.
 *
 * @param   sTable  SIDEX_VARIANT handle
 * @param   sColumnName name of the column
 * @param   bRet        reference to SIDEX_VARIANT handle (boolean)<br>
                        SIDEX_TRUE if the column exists
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT sTable is SIDEX_HANDLE_TYPE_NULL<br>
 *          SIDEX_ERR_WRONG_PARAMETER sColumnName does not exist<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table
 *
 * @see sidex_Variant_Table_ColumnNames()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_HasColumn(SIDEX_VARIANT sTable, SIDEX_CTSTR* sColumnName, SIDEX_BOOL* bRet);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_HasColumn_X(SIDEX_VARIANT sTable, wchar_t* sColumnName, SIDEX_BOOL* bRet);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_HasColumn_W(SIDEX_VARIANT sTable, char16_t* sColumnName, SIDEX_BOOL* bRet);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_HasColumn_A(SIDEX_VARIANT sTable, char* sColumnName, SIDEX_BOOL* bRet);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Table_HasColumn  sidex_Variant_Table_HasColumn_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Table_HasColumn  sidex_Variant_Table_HasColumn_W
    #else
      #define sidex_Variant_Table_HasColumn  sidex_Variant_Table_HasColumn_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup booltype
 * @brief   Get the value from a boolean SIDEX_VARIANT.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   bValue  reference to a SIDEX_BOOL
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE variant is not a boolean
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Boolean(SIDEX_VARIANT variant, SIDEX_BOOL* bValue);


/**
 * @ingroup inttype
 * @brief   Get the value from an integer SIDEX_VARIANT.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   iValue  reference to a SIDEX_INT64
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE variant is not an integer
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Integer(SIDEX_VARIANT variant, SIDEX_INT64* iValue);


/**
 * @ingroup floattype
 * @brief   Get the value from a float SIDEX_VARIANT.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   fValue  reference to a SIDEX_DOUBLE
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE variant is not a float
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Float(SIDEX_VARIANT variant, SIDEX_DOUBLE* fValue);


/**
 * @ingroup datetimetype
 * @brief   Get the value from a datetime SIDEX_VARIANT. Returns a formatted string.
 *
 * refer to sidex_Variant_New_DateTime() for information about the string format.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   dtValue borrowed reference to a datetime string
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_WRONG_TYPE variant is not datetime
 *
 * @see sidex_Variant_New_DateTime()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_DateTime(SIDEX_VARIANT variant, SIDEX_CTSTR** dtValue);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_DateTime_X(SIDEX_VARIANT variant, wchar_t** dtValue);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_DateTime_W(SIDEX_VARIANT variant, char16_t** dtValue);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_DateTime_A(SIDEX_VARIANT variant, char** dtValue);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_As_DateTime  sidex_Variant_As_DateTime_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_As_DateTime  sidex_Variant_As_DateTime_W
    #else
      #define sidex_Variant_As_DateTime  sidex_Variant_As_DateTime_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup stringtype
 * @brief   Get value and length from a string SIDEX_VARIANT.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   sValue  borrowed reference to string
 * @param   iLength reference to the length of the string
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_WRONG_TYPE variant is not a string
 *
 * @see sidex_Variant_As_String_Length()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String(SIDEX_VARIANT variant, SIDEX_CTSTR** sValue, SIDEX_INT32* iLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_X(SIDEX_VARIANT variant, wchar_t** sValue, SIDEX_INT32* iLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_W(SIDEX_VARIANT variant, char16_t** sValue, SIDEX_INT32* iLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_A(SIDEX_VARIANT variant, char** sValue, SIDEX_INT32* iLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_As_String  sidex_Variant_As_String_X
  #else
    #ifdef SIDEX_UNICODE_16
    #define sidex_Variant_As_String  sidex_Variant_As_String_W
    #else
    #define sidex_Variant_As_String  sidex_Variant_As_String_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION

  
/**
 * @ingroup stringtype
 * @brief   Get the length of a string SIDEX_VARIANT.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   iLength reference to the length of the string
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_WRONG_TYPE variant is not a string
 *
 * @see sidex_Variant_As_String()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_Length(SIDEX_VARIANT variant, SIDEX_INT32* iLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_Length_X(SIDEX_VARIANT variant, SIDEX_INT32* iLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_Length_W(SIDEX_VARIANT variant, SIDEX_INT32* iLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_Length_A(SIDEX_VARIANT variant, SIDEX_INT32* iLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_As_String_Length  sidex_Variant_As_String_Length_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_As_String_Length  sidex_Variant_As_String_Length_W
    #else
      #define sidex_Variant_As_String_Length  sidex_Variant_As_String_Length_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION

  
/**
 * @ingroup bintype
 * @brief   Get the length from a binary SIDEX_VARIANT.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   iLength reference to the length
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE variant is not binary
 *
 * @see sidex_Variant_As_Binary()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Binary_Length(SIDEX_VARIANT variant, SIDEX_INT32* iLength);

  
/**
 * @ingroup bintype
 * @brief   Get value and length from a binary SIDEX_VARIANT.
 *
 * @param   variant SIDEX_VARIANT handle
 * @param   sValue  borrowed reference to a binary char array<br>
 * @param   iLength reference to the length
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE variant is not binary
 *
 * @see sidex_Variant_As_Binary_Length()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Binary(SIDEX_VARIANT variant, unsigned char** sValue, SIDEX_INT32* iLength);

  
 /**
 * @ingroup listtype
 * @brief   Append a value to a list.
 *
 * @param   sList SIDEX_VARIANT handle of the list
 * @param   value SIDEX_VARIANT handle of the value
 * @param   pos   reference to the position of the added element in a list
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE sList is not a list
 *
 * @see sidex_Variant_List_Insert(), sidex_Variant_List_Set()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Append(SIDEX_VARIANT sList, SIDEX_VARIANT value, SIDEX_INT32* pos);


/**
 * @ingroup listtype
 * @brief   Insert a value into a list.
 *
 * @param   sList SIDEX_VARIANT handle of the list
 * @param   value SIDEX_VARIANT handle of the value
 * @param   pos the position in the list
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE sList is not a list<br>
 *          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed
 *
 * @see sidex_Variant_List_Append(), sidex_Variant_List_Set()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Insert(SIDEX_VARIANT sList, SIDEX_VARIANT value, SIDEX_INT32 pos);


/**
 * @ingroup listtype
 * @brief   Overwrite a value in a list.
 *
 * @param   sList SIDEX_VARIANT handle of the list
 * @param   value SIDEX_VARIANT handle of the new value
 * @param   pos the position in the list
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sList is not a list<br>
 *          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
 *          SIDEX_ERR_NOCONTENT invalid or not existing position passed
 *
 * @see sidex_Variant_List_Append(), sidex_Variant_List_Insert()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Set(SIDEX_VARIANT sList, SIDEX_VARIANT value, SIDEX_INT32 pos);


/**
 * @ingroup listtype
 * @brief   Remove a value from a list.
 *
 * @param   sList SIDEX_VARIANT handle of the list
 * @param   pos the position in the list
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sList is not a list<br>
 *          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
 *          SIDEX_ERR_NOCONTENT invalid or not existing position passed
 *
 * @see sidex_Variant_List_Clear()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_DeleteItem(SIDEX_VARIANT sList, SIDEX_INT32 pos);


/**
 * @ingroup listtype
 * @brief   Remove all values from a list.
 *
 * @param   sList SIDEX_VARIANT handle of the list
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sList is not a list
 *
 * @see sidex_Variant_List_DeleteItem()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Clear(SIDEX_VARIANT sList);


/**
 * @ingroup listtype
 * @brief   Get a value from a list.
 *
 * @param   sList SIDEX_VARIANT handle of the list
 * @param   index position of the value
 * @param   variant borrowed reference to a SIDEX_VARIANT<br>
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sList is not a list<br>
 *          SIDEX_ERR_NOCONTENT invalid index
 *
 * @see sidex_Variant_List_Set()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Get(SIDEX_VARIANT sList, SIDEX_INT32 index, SIDEX_VARIANT* variant);


/**
 * @ingroup dicttype
 * @brief   Write a value to a dictionary.
 *
 * @param   sDict   SIDEX_VARIANT handle of a dictionary
 * @param   nKey    name of the key
 * @param   value   SIDEX_VARIANT handle of the value
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary<br>
 *          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
 *          SIDEX_ERR_WRONG_KEY nkey is an invalid<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion
 *
 * @see sidex_Variant_Dict_Get()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Set(SIDEX_VARIANT sDict, SIDEX_CTSTR* nKey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Set_X(SIDEX_VARIANT sDict, wchar_t* nKey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Set_W(SIDEX_VARIANT sDict, char16_t* nKey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Set_A(SIDEX_VARIANT sDict, char* nKey, SIDEX_VARIANT value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Dict_Set  sidex_Variant_Dict_Set_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Dict_Set  sidex_Variant_Dict_Set_W
    #else
      #define sidex_Variant_Dict_Set  sidex_Variant_Dict_Set_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup dicttype
 * @brief   Delete a value from a dictionary.
 *
 * @param   sDict   SIDEX_VARIANT handle of a dictionary
 * @param   sKey    name of the key
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_NOCONTENT sKey does not exist<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary
 *
 * @see sidex_Variant_Dict_Clear()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Delete(SIDEX_VARIANT sDict, SIDEX_CTSTR* sKey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Delete_X(SIDEX_VARIANT sDict, wchar_t* sKey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Delete_W(SIDEX_VARIANT sDict, char16_t* sKey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Delete_A(SIDEX_VARIANT sDict, char* sKey);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Dict_Delete  sidex_Variant_Dict_Delete_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Dict_Delete  sidex_Variant_Dict_Delete_W
    #else
      #define sidex_Variant_Dict_Delete  sidex_Variant_Dict_Delete_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup dicttype
 * @brief   Remove all entries of from a dictionary.
 *
 * @param   sDict   SIDEX_VARIANT handle of a dictionary
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary
 *
 * @see sidex_Variant_Dict_Delete()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Clear(SIDEX_VARIANT sDict);


/**
 * @ingroup dicttype
 * @brief   Get a value from a dictionary.
 *
 * @param   sDict   SIDEX_VARIANT handle of a dictionary
 * @param   sKey    name of the key
 * @param   variant borrowed reference to a SIDEX_VARIANT handle
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE sDict is not a dictionary<br>
 *          SIDEX_ERR_WRONG_DICT_SYNTAX internal data error<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_NOCONTENT sKey does not exist
 *
 * @see sidex_Variant_Dict_Set()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Get(SIDEX_VARIANT sDict, SIDEX_CTSTR* sKey, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Get_X(SIDEX_VARIANT sDict, wchar_t* sKey, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Get_W(SIDEX_VARIANT sDict, char16_t* sKey, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Get_A(SIDEX_VARIANT sDict, char* sKey, SIDEX_VARIANT* variant);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Dict_Get  sidex_Variant_Dict_Get_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Dict_Get  sidex_Variant_Dict_Get_W
    #else
      #define sidex_Variant_Dict_Get  sidex_Variant_Dict_Get_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tabletype
 * @brief   Add a column to a Table.
 *
 * For column fields in existing rows the value will be set to None.<br>
 *
 * @param   sTable  SIDEX_VARIANT handle of a table
 * @param   sColumn column name
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table<br>
 *          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
 *          SIDEX_ERR_WRONG_KEY sColumn contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_TABLE_COLUMN_ALREADY_EXISTS column already exists
 *
 * @see sidex_Variant_Table_AddRow(), sidex_Variant_Table_DeleteColumn
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddColumn(SIDEX_VARIANT sTable, SIDEX_CTSTR* sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddColumn_X(SIDEX_VARIANT sTable, wchar_t* sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddColumn_W(SIDEX_VARIANT sTable, char16_t* sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddColumn_A(SIDEX_VARIANT sTable, char* sColumn);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Table_AddColumn  sidex_Variant_Table_AddColumn_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Table_AddColumn  sidex_Variant_Table_AddColumn_W
    #else
      #define sidex_Variant_Table_AddColumn  sidex_Variant_Table_AddColumn_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tabletype
 * @brief   Remove a column from a Table.
 *
 * @param   sTable  SIDEX_VARIANT handle of a table
 * @param   sColumn column name
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table<br>
 *          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_NOCONTENT column does not exist
 *
 * @see sidex_Variant_Table_AddColumn()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteColumn(SIDEX_VARIANT sTable, SIDEX_CTSTR* sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteColumn_X(SIDEX_VARIANT sTable, wchar_t* sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteColumn_W(SIDEX_VARIANT sTable, char16_t* sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteColumn_A(SIDEX_VARIANT sTable, char* sColumn);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Table_DeleteColumn  sidex_Variant_Table_DeleteColumn_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Table_DeleteColumn  sidex_Variant_Table_DeleteColumn_W
    #else
      #define sidex_Variant_Table_DeleteColumn  sidex_Variant_Table_DeleteColumn_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tabletype
 * @brief    Get the name of a table column.
 *
 * @param   sTable  SIDEX_VARIANT handle of a table
 * @param   index   column index
 * @param   sColumn borrowed reference to column name
 *
 * @returns SIDEX_SUCCESS in case of success.<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_NOCONTENT column does not exist
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetColumnName(SIDEX_VARIANT sTable, SIDEX_INT32 index, SIDEX_CTSTR** sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetColumnName_X(SIDEX_VARIANT sTable, SIDEX_INT32 index, wchar_t** sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetColumnName_W(SIDEX_VARIANT sTable, SIDEX_INT32 index, char16_t** sColumn);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetColumnName_A(SIDEX_VARIANT sTable, SIDEX_INT32 index, char** sColumn);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Table_GetColumnName  sidex_Variant_Table_GetColumnName_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Table_GetColumnName  sidex_Variant_Table_GetColumnName_W
    #else
      #define sidex_Variant_Table_GetColumnName  sidex_Variant_Table_GetColumnName_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tabletype
 * @brief   Add a row to a table.
 *
 * Fields of the new row will be set to None.
 *
 * @note Rows can be added before columns but
 * sidex_Variant_Table_GetRow() will return SIDEX_ERR_NOCONTENT.
 *
 * @param   sTable  SIDEX_VARIANT handle of a table
 * @param   iRowIdx reference to the row index
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table
 *
 * @see sidex_Variant_Table_AddColumn()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddRow(SIDEX_VARIANT sTable, SIDEX_INT32* iRowIdx);


/**
 * @ingroup tabletype
 * @brief   Delete a row from a table.
 *
 * @param   sTable    SIDEX_VARIANT handle of a table
 * @param   rowIndex  row index
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table<br>
 *          SIDEX_ERR_NOCONTENT row does not exist
 *
 * @see sidex_Variant_Table_AddRow(), sidex_Variant_Table_DeleteRows()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteRow(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex);


/**
 * @ingroup tabletype
 * @brief   Delete all rows from a table.
 *
 * @param   sTable  SIDEX_VARIANT handle of a table
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table
 *
 * @see sidex_Variant_Table_DeleteRow()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteRows(SIDEX_VARIANT sTable);


/**
 * @ingroup tabletype
 * @brief   Get row from a Table.
 *
 * @param   sTable  SIDEX_VARIANT handle of a table
 * @param   rowIndex row index
 * @param   variant  borrowed reference to SIDEX_VARIANT handle with a dictionary of values
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table<br>
 *          SIDEX_ERR_NOCONTENT row does not exist
 *
 * @see sidex_Variant_Table_AddRow()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetRow(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, SIDEX_VARIANT* variant);


/**
 * @ingroup tabletype
 * @brief   Get field from a Table.
 *
 * @param   sTable      SIDEX_VARIANT handle of a table
 * @param   rowIndex    row index
 * @param   sColumnName column name
 * @param   variant     borrowed reference to SIDEX_VARIANT handle
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_NOCONTENT field does not exist
 *
 * @see sidex_Variant_Table_SetField()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetField(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, SIDEX_CTSTR* sColumnName, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetField_X(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, wchar_t* sColumnName, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetField_W(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, char16_t* sColumnName, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetField_A(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, char* sColumnName, SIDEX_VARIANT* variant);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Table_GetField  sidex_Variant_Table_GetField_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Table_GetField  sidex_Variant_Table_GetField_W
    #else
      #define sidex_Variant_Table_GetField  sidex_Variant_Table_GetField_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tabletype
 * @brief   Set a field in a table.
 *
 * @note If the row index does not exist, the missing rows will automatically be added to the table.
 *
 * @param   sTable      SIDEX_VARIANT handle of a table
 * @param   rowIndex    row index
 * @param   sColumnName column name
 * @param   variant     SIDEX_VARIANT handle
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_TYPE sTable is not a table<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_NOCONTENT sColumnName does not exist
 *
 * @see sidex_Variant_Table_GetField()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_SetField(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, SIDEX_CTSTR* sColumnName, SIDEX_VARIANT variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_SetField_X(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, wchar_t* sColumnName, SIDEX_VARIANT variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_SetField_W(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, char16_t* sColumnName, SIDEX_VARIANT variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_SetField_A(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, char* sColumnName, SIDEX_VARIANT variant);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Variant_Table_SetField  sidex_Variant_Table_SetField_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Variant_Table_SetField  sidex_Variant_Table_SetField_W
    #else
      #define sidex_Variant_Table_SetField  sidex_Variant_Table_SetField_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write a none value (empty value) to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_UNICODE error in unicode conversion
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_None_Write(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_None_Write_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_None_Write_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_None_Write_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_None_Write  sidex_None_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_None_Write  sidex_None_Write_W
    #else
      #define sidex_None_Write  sidex_None_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get a boolean value from a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   reference boolean value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   reference boolean value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_INVALID_BOOLEAN internal format error<br>
///          SIDEX_ERR_UNICODE unicode conversion error<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_Boolean_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_BOOL* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_BOOL* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_BOOL* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_BOOL* value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Boolean_Read  sidex_Boolean_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Boolean_Read  sidex_Boolean_Read_W
    #else
      #define sidex_Boolean_Read  sidex_Boolean_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write a boolean value (SIDEX_TRUE, SIDEX_FALSE) to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   boolean value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   boolean value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_UNICODE unicode conversion error
///
///
/// @see sidex_Boolean_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Write(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_BOOL value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Write_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_BOOL value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Write_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_BOOL value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Write_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_BOOL value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Boolean_Write  sidex_Boolean_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Boolean_Write  sidex_Boolean_Write_W
    #else
      #define sidex_Boolean_Write  sidex_Boolean_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get an integer value from a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   reference an integer value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL.<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   reference an integer value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_INVALID_INTEGER integer out of range<br>
///          SIDEX_ERR_UNICODE unicode conversion error<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_Integer_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_INT64 *value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_INT64 *value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_INT64 *value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_INT64 *value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Integer_Read  sidex_Integer_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Integer_Read  sidex_Integer_Read_W
    #else
      #define sidex_Integer_Read  sidex_Integer_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write an integer value to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   integer value
///
/// @returns SIDEX_SUCCESS in case of success.<br>
///          SIDEX_ERR_MISSING_KEY if nkey is NULL.<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   integer value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_UNICODE error in unicode conversion
///
///
/// @see sidex_Integer_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Write(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_INT64 value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Write_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_INT64 value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Write_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_INT64 value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Write_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_INT64 value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Integer_Write  sidex_Integer_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Integer_Write  sidex_Integer_Write_W
    #else
      #define sidex_Integer_Write  sidex_Integer_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get a float value from a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   reference to a float value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   reference to a float value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_Float_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_DOUBLE *value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_DOUBLE *value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_DOUBLE *value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_DOUBLE *value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Float_Read  sidex_Float_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Float_Read  sidex_Float_Read_W
    #else
      #define sidex_Float_Read  sidex_Float_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write a float value to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   float value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   float value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_UNICODE error in unicode conversion
///
///
/// @see sidex_Float_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_DOUBLE value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_DOUBLE value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_DOUBLE value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_DOUBLE value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Float_Write  sidex_Float_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Float_Write  sidex_Float_Write_W
    #else
      #define sidex_Float_Write  sidex_Float_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get a datetime value from a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   reference to a SIDEX_VARIANT handle
///
/// @returns SIDEX_SUCCESS in case of success.<br>
///          SIDEX_ERR_MISSING_KEY if nkey is NULL.<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   reference to a SIDEX_VARIANT handle
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_INVALID_DATETIME invalid format<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_DateTime_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Read(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Read_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Read_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Read_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT* value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_DateTime_Read  sidex_DateTime_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_DateTime_Read  sidex_DateTime_Read_W
    #else
      #define sidex_DateTime_Read  sidex_DateTime_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write a datetime value to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   SIDEX_VARIANT handle
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   SIDEX_VARIANT handle
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_UNICODE error in unicode conversion
///
///
/// @see sidex_DateTime_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_DateTime_Write  sidex_DateTime_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_DateTime_Write  sidex_DateTime_Write_W
    #else
      #define sidex_DateTime_Write  sidex_DateTime_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get a string value from a SIDEX document.
///
///          It must be deallocated using sidex_Free_ReadString().
///
/// @param   shandle    SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value      reference to a string
/// @param   iStrLength reference to the string length
///
/// @returns SIDEX_SUCCESS in case of success.<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value      reference to a string
/// @param   iStrLength reference to the string length
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_String_Length(), sidex_Free_ReadString(), sidex_String_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_CTSTR** value, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, wchar_t** value, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, char16_t** value, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, char** value, SIDEX_INT32* iStrLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_String_Read  sidex_String_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_String_Read  sidex_String_Read_W
    #else
      #define sidex_String_Read  sidex_String_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get the length of a string value in a SIDEX document.
///
/// @param   shandle    SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   iStrLength reference to the string length
///
/// @returns SIDEX_SUCCESS in case of success.<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   iStrLength reference to the string length
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_String_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Length(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Length_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Length_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Length_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_INT32* iStrLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_String_Length  sidex_String_Length_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_String_Length  sidex_String_Length_W
    #else
      #define sidex_String_Length  sidex_String_Length_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get a binary value from a SIDEX document.
///
/// @param   shandle    SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value      reference to the binary data
/// @param   iStrLength reference to the byte count
///
/// @returns SIDEX_SUCCESS in case of success<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value      reference to the binary data
/// @param   iStrLength reference to the byte count
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_Free_Binary_ReadString(), sidex_Binary_Length()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, unsigned ** value, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, unsigned char** value, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, unsigned char** value, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, unsigned char** value, SIDEX_INT32* iStrLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Binary_Read  sidex_Binary_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Binary_Read  sidex_Binary_Read_W
    #else
      #define sidex_Binary_Read  sidex_Binary_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get the number of bytes of a binary value in a SIDEX document.
///
/// @param   shandle    SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   iStrLength reference to the byte count
///
/// @returns SIDEX_SUCCESS in case of success<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   iStrLength reference to the byte count
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_Binary_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Length(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Length_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Length_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_INT32* iStrLength);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Length_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_INT32* iStrLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Binary_Length  sidex_Binary_Length_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Binary_Length  sidex_Binary_Length_W
    #else
      #define sidex_Binary_Length  sidex_Binary_Length_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sdxDocRWValues
 * @brief   Free the memory for the string returned by sidex_String_Read().
 *
 * @param   string the reference to the string
 *
 * @see sidex_String_Read()
 */
SIDEX_API void DLL_CALL_CONV sidex_Free_ReadString(SIDEX_CTSTR* string);
SIDEX_API void DLL_CALL_CONV sidex_Free_ReadString_X(wchar_t* string);
SIDEX_API void DLL_CALL_CONV sidex_Free_ReadString_W(char16_t* string);
SIDEX_API void DLL_CALL_CONV sidex_Free_ReadString_A(char* string);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Free_ReadString  sidex_Free_ReadString_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Free_ReadString  sidex_Free_ReadString_W
    #else
      #define sidex_Free_ReadString  sidex_Free_ReadString_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup sdxDocRWValues
 * @brief   Free the memory for the binary data returned by sidex_Binary_Read().
 *
 * @param   string the reference of binary data
 *
 * @see sidex_Binary_Read()
 */
SIDEX_API void DLL_CALL_CONV sidex_Free_Binary_ReadString(unsigned char* string);


/// @ingroup sdxDocRWValues
/// @brief   Write a string value to a SIDEX document.
///
/// @param   shandle  SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   NULL terminated string or NULL for an empty string.
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   NULL terminated string or NULL for an empty string.
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_UNICODE error in unicode conversion
///
/// @see sidex_String_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, const SIDEX_CTSTR* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, const wchar_t* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, const char16_t* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, const char* value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_String_Write  sidex_String_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_String_Write  sidex_String_Write_W
    #else
      #define sidex_String_Write  sidex_String_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write a binary value to a SIDEX document.
///
/// @param   shandle  SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value    pointer to binary data or NULL
/// @param   size     length of binary data
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value    pointer to binary data or NULL
/// @param   size     length of binary data
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_PARAMETER value is NULL but the iStrLength is not 0<br>
///          SIDEX_ERR_UNICODE error in unicode conversion
///
/// @see sidex_Binary_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, const unsigned char* value, SIDEX_INT32 size);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, const unsigned char* value, SIDEX_INT32 size);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, const unsigned char* value, SIDEX_INT32 size);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, const unsigned char* value, SIDEX_INT32 size);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Binary_Write  sidex_Binary_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Binary_Write  sidex_Binary_Write_W
    #else
      #define sidex_Binary_Write  sidex_Binary_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get a list from a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   reference to the SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   reference to the SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_List_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_VARIANT* value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_List_Read  sidex_List_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_List_Read  sidex_List_Read_W
    #else
      #define sidex_List_Read  sidex_List_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write a list to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success.<br>
///          SIDEX_ERR_MISSING_KEY if nkey is NULL.<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
///          SIDEX_ERR_UNICODE error in unicode conversion
///
/// @see sidex_List_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_List_Write  sidex_List_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_List_Write  sidex_List_Write_W
    #else
      #define sidex_List_Write  sidex_List_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get a dictionary from a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   reference to the SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   reference to the SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_Dict_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_VARIANT* value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Dict_Read  sidex_Dict_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Dict_Read  sidex_Dict_Read_W
    #else
      #define sidex_Dict_Read  sidex_Dict_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write a dictionary to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
///          SIDEX_ERR_UNICODE error in unicode conversion
///
/// @see sidex_Dict_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Dict_Write  sidex_Dict_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Dict_Write  sidex_Dict_Write_W
    #else
      #define sidex_Dict_Write  sidex_Dict_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Get a table from a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   reference to the SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success.<br>
///          SIDEX_ERR_MISSING_KEY if nkey is NULL.<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   reference to the SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_Table_Write()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_VARIANT* value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Table_Read  sidex_Table_Read_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Table_Read  sidex_Table_Read_W
    #else
      #define sidex_Table_Read  sidex_Table_Read_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup sdxDocRWValues
/// @brief   Write a table to a SIDEX document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group / NULL if no group is required
/// @param   nkey    name of the key
/// @param   value   SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success.<br>
///          SIDEX_ERR_MISSING_KEY if nkey is NULL.<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
/// @param   value   SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br> 
///          SIDEX_ERR_WRONG_TYPE value is of wrong type<br>
///          SIDEX_ERR_WRONG_PARAMETER invalid parameter passed<br>
///          SIDEX_ERR_UNICODE error in unicode conversion
///
/// @see sidex_Table_Read()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT value);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT value);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Table_Write  sidex_Table_Write_X
  #else
    #ifdef SIDEX_UNICODE_16
     #define sidex_Table_Write  sidex_Table_Write_W
    #else
     #define sidex_Table_Write  sidex_Table_Write_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup navigationType
 * @brief   Get a list of group names.
 *
 * @param   shandle SIDEX document handle (SIDEX_HANDLE)
 * @param   variant reference to a SIDEX_VARIANT value.<br>
 *          The list contains all group names of the SIDEX document<br>
 *          and must be released by sidex_Variant_DecRef().
 *
 * @returns SIDEX_SUCCESS in case of success
 *
* @see sidex_HasGroup()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetGroups(SIDEX_HANDLE shandle, SIDEX_VARIANT* variant);


/**
 * @ingroup navigationType
 * @brief   Check if a group exists.
 *
 * @param   shandle SIDEX document handle (SIDEX_HANDLE)
 * @param   ngroup  Name of the group
 *
 * @returns SIDEX_TRUE if group exists
 *
* @see sidex_GetGroups()
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasGroup(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup);
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasGroup_X(SIDEX_HANDLE shandle, wchar_t* ngroup);
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasGroup_W(SIDEX_HANDLE shandle, char16_t* ngroup);
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasGroup_A(SIDEX_HANDLE shandle, char* ngroup);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_HasGroup  sidex_HasGroup_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_HasGroup  sidex_HasGroup_W
    #else
      #define sidex_HasGroup  sidex_HasGroup_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup navigationType
/// @brief   Get a list of key names in a group
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group or null
/// @param   variant reference to the SIDEX_VARIANT value
///
/// @returns SIDEX_SUCCESS in case of success<br>
#else //GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   variant reference to the SIDEX_VARIANT value<br>
///          The list contains all key names of the ngroup of the SIDEX document<br>
///          and must be released by sidex_Variant_DecRef().
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty
///
/// @see sidex_HasKey()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetKeys(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetKeys_X(SIDEX_HANDLE shandle, wchar_t* ngroup, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetKeys_W(SIDEX_HANDLE shandle, char16_t* ngroup, SIDEX_VARIANT* variant);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetKeys_A(SIDEX_HANDLE shandle, char* ngroup, SIDEX_VARIANT* variant);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_GetKeys  sidex_GetKeys_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_GetKeys  sidex_GetKeys_W
    #else
      #define sidex_GetKeys  sidex_GetKeys_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup navigationType
 * @brief   Check if a key exists in a group.
 *
 * @param   shandle SIDEX document handle (SIDEX_HANDLE)
 * @param   ngroup  name of the group
 * @param   nkey    name of the key
 *
 * @returns SIDEX_TRUE if the key exists in the group
 *
 * @see sidex_GetKeys()
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasKey(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey);
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasKey_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey);
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasKey_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey);
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasKey_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_HasKey  sidex_HasKey_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_HasKey  sidex_HasKey_W
    #else
      #define sidex_HasKey  sidex_HasKey_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup navigationType
 * @brief   Delete a group from the document.
 *
 * @param   shandle SIDEX document handle (SIDEX_HANDLE)
 * @param   ngroup  name of the group
 *
 * @returns SIDEX_SUCCESS in case of success<br>
 *          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
 *          SIDEX_ERR_UNICODE error in unicode conversion<br>
 *          SIDEX_ERR_NOCONTENT group does not exist
 *
* @see sidex_DeleteKey()
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteGroup(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteGroup_X(SIDEX_HANDLE shandle, wchar_t* ngroup);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteGroup_W(SIDEX_HANDLE shandle, char16_t* ngroup);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteGroup_A(SIDEX_HANDLE shandle, char* ngroup);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_DeleteGroup  sidex_DeleteGroup_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_DeleteGroup  sidex_DeleteGroup_W
    #else
      #define sidex_DeleteGroup  sidex_DeleteGroup_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


/// @ingroup navigationType
/// @brief   Delete a key, value pair from the document.
///
/// @param   shandle SIDEX document handle (SIDEX_HANDLE)
#ifdef GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group or NULL
/// @param   nkey    name of the key
///
/// @returns SIDEX_SUCCESS in case of success<br>
#else // GROUP_IS_OPTIONAL
/// @param   ngroup  name of the group
/// @param   nkey    name of the key
///
/// @returns SIDEX_SUCCESS in case of success<br>
///          SIDEX_ERR_MISSING_GROUP ngroup is NULL<br>
#endif // GROUP_IS_OPTIONAL
///          SIDEX_ERR_MISSING_KEY nkey is NULL<br>
///          SIDEX_ERR_WRONG_KEY nkey contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_WRONG_GROUP ngroup contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty<br>
///          SIDEX_ERR_UNICODE error in unicode conversion<br>
///          SIDEX_ERR_NOCONTENT value does not exist
///
/// @see sidex_DeleteGroup()
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteKey(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteKey_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteKey_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteKey_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_DeleteKey  sidex_DeleteKey_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_DeleteKey  sidex_DeleteKey_W
    #else
      #define sidex_DeleteKey  sidex_DeleteKey_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup  tmladvanced
 * @brief  Write debug logs.
 *
 * @note For develop use only / do not use this function in a productive environment.
 *
 * @param   iLogMask      logging mask / 0xFFFFFFFF will log immediately
 * @param   sClass        first reference to log
 * @param   sMethod       second reference to log
 * @param   sFormatLog    third reference to log
 * @param   sLog          fourth reference to log
 * @param   iFileIndex    index of contiguous logfile
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_log(SIDEX_INT32 iLogMask, const SIDEX_CTSTR* sClass, const SIDEX_CTSTR* sMethod, const SIDEX_CTSTR* sFormatLog, const SIDEX_CTSTR* sLog, SIDEX_INT32 iFileIndex);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_log_X(SIDEX_INT32 iLogMask, const wchar_t* sClass, const wchar_t* sMethod, const wchar_t* sFormatLog, const wchar_t* sLog, SIDEX_INT32 iFileIndex);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_log_W(SIDEX_INT32 iLogMask, const char16_t* sClass, const char16_t* sMethod, const char16_t* sFormatLog, const char16_t* sLog, SIDEX_INT32 iFileIndex);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_log_A(SIDEX_INT32 iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog, SIDEX_INT32 iFileIndex);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_log  sidex_log_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_log  sidex_log_W
    #else
      #define sidex_log  sidex_log_A
    #endif // SIDEX_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION 

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/


/**
 * @ingroup sidexHandle
 * @brief   Merge SIDEX document data.
 *
 * @param   sBaseHandle   SIDEX document handle (SIDEX_HANDLE)
 * @param   sMergeHandle  SIDEX document handle (SIDEX_HANDLE) to be merged
 * @param   bOverwrite    true = existing group,key pairs will be overwritten
 * @param   nGroup  name of the group
 * @param   nKey    name of the key or SIDEX_HANDLETYPE_NULL (complete group)
 *
 * @returns  SIDEX_SUCCESS in case of success
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Merge(SIDEX_HANDLE sBaseHandle, SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, SIDEX_CTSTR* nGroup, SIDEX_CTSTR* nKey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Merge_X(SIDEX_HANDLE sBaseHandle, SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, wchar_t* nGroup, wchar_t* nKey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Merge_W(SIDEX_HANDLE sBaseHandle, SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, char16_t* nGroup, char16_t* nKey);
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Merge_A(SIDEX_HANDLE sBaseHandle, SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, char* nGroup, char* nKey);
#if !defined (DOXYGEN_GENERATION)
  #ifdef SIDEX_UNICODE
    #define sidex_Merge  sidex_Merge_X
  #else
    #ifdef SIDEX_UNICODE_16
      #define sidex_Merge  sidex_Merge_W
    #else
      #define sidex_Merge  sidex_Merge_A
    #endif // SIDEX_UNICODE_16
  #endif // SIDEX_UNICODE
#endif // DOXYGEN_GENERATION


#ifdef __cplusplus
}// extern "C"
#endif

#endif  // SIDEX_H
