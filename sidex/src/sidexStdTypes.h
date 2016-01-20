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
 * Contributors:
 *    wobe-systems GmbH
 */
 
/** @file sidexStdTypes.h
  * @brief SIDEX standard defines and data type definitions
  */

#ifndef SIDEXSTDTYPES_H
#define SIDEXSTDTYPES_H

#ifdef LINUX
#include <stdint.h>
#else // LINUX
#ifdef MINGW_BUILD
  #include <stdint.h>
#else // MINGW_BUILD
  #include <stdIntTypes.h>
#endif // MINGW_BUILD
#endif // LINUX


////////////////////////////////////////////////////////
//                   UNICODE
////////////////////////////////////////////////////////
#ifndef LINUX
  #ifndef char16_t
      typedef uint16_t char16_t;
  #endif
#endif // LINUX

/**
* @brief    Common typedefinition for string types (wchar_t* / char*) to support ANSI or UNICODE usage.<br>
*           In case of SIDEX_UNICODE prepocessor is set, wchar_t* is mapped, otherwise char*.
*/
#ifdef SIDEX_UNICODE
  typedef wchar_t              SIDEX_TCHAR;
#else
  #ifdef SIDEX_UNICODE_16
    typedef char16_t           SIDEX_TCHAR;
  #else
    typedef char               SIDEX_TCHAR;
  #endif// SIDEX_UNICODE_16
#endif // SIDEX_UNICODE

/**
* @brief    Common typedefinition for the SIDEX- API interface to be able to use ANSI or UNICODE.<br>
* @ingroup sidexTypes
*/
typedef void*                 SIDEX_CTSTR;

/**
* @brief    Typedefinition of a handle / 64bit signed
* @ingroup sidexTypes
*/
typedef int64_t               SIDEX_HANDLE_TYPE;


/**
* @brief    Common definition to check SIDEX_HANDLE_TYPE value against 0 / NULL
* @ingroup sidexConstants
*/
#define SIDEX_HANDLE_TYPE_NULL 0


/**
* @brief    Typedefinition of a SIDEX_HANDLE / 64bit signed
* @ingroup sidexTypes
*/
typedef SIDEX_HANDLE_TYPE     SIDEX_HANDLE;


/**
* @brief    Typedefinition of a SIDEX_VARIANT / 64bit signed
* @ingroup sidexTypes
*/
typedef SIDEX_HANDLE_TYPE     SIDEX_VARIANT;


/**
* @brief    Typedefinition of a SIDEX_BOOL / 32bit (legal values are 0 or 1)
* @ingroup sidexTypes
*/
typedef int32_t               SIDEX_BOOL;


/** 
 * @brief Common definition to have false value (which is defined to 0 / 32bit signed value).
 * @ingroup sidexConstants
 */
#define SIDEX_FALSE           ((int32_t)0)


/** 
 * @brief Common definition to have true value (which is defined to 1 / 32bit signed value).
 * @ingroup sidexConstants
 */
#define SIDEX_TRUE            ((int32_t)1)


/**
* @brief    Typedefinition of a SIDEX_INT32 / 32bit signed
* @ingroup sidexTypes
*/
typedef int32_t               SIDEX_INT32;


/**
* @brief    Typedefinition of a SIDEX_UINT32 / 32bit unsigned
* @ingroup sidexTypes
*/
typedef uint32_t              SIDEX_UINT32;


/**
* @brief    Typedefinition of a SIDEX_INT64 / 64bit signed
* @ingroup sidexTypes
*/
typedef int64_t               SIDEX_INT64;


/**
* @brief    Typedefinition of a SIDEX_UINT64 / 64bit unsigned
* @ingroup sidexTypes
*/
typedef uint64_t              SIDEX_UINT64;


/**
* @brief    Typedefinition of a SIDEX_DOUBLE / double
* @ingroup sidexTypes
*/
typedef double                SIDEX_DOUBLE;


/**
* @brief    Typedefinition of a SIDEX_DATA_TYPE / 32bit signed
* @ingroup sidexTypes
*/
typedef int32_t               SIDEX_DATA_TYPE;


/**
* @brief  Datatype is unknown
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_UNKNOWN     ((int32_t)0)


/**
* @brief  Datatype is None
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_NONE        ((int32_t)1)


/**
* @brief  Datatype is Boolean
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_BOOLEAN     ((int32_t)2)


/**
* @brief  Datatype is Integer
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_INTEGER     ((int32_t)3)


/**
* @brief  Datatype is Float
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_FLOAT       ((int32_t)4)


/**
* @brief  Datatype is String
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_STRING      ((int32_t)5)


/**
* @brief  Datatype is List
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_LIST        ((int32_t)6)


/**
* @brief  Datatype is Dict
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_DICT        ((int32_t)7)


/**
* @brief  Datatype is Table
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_TABLE       ((int32_t)8)


/**
* @brief  Datatype is DateTime
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_DATETIME    ((int32_t)9)


/**
* @brief  Datatype is Binary String
* @ingroup sidexConstants
*/
#define SIDEX_DATA_TYPE_BINARY      ((int32_t)10)


/**
* @brief    Typedefinition of a SIDEX_ENCODE / 32bit signed
* @ingroup sidexTypes
*/
typedef int32_t               SIDEX_ENCODE;


/**
* @brief  encoding / none
* @ingroup sidexConstants
*/
#define SIDEX_CONV_NOT_FOUND       ((int32_t)-1)


/**
* @brief  encoding / none
* @ingroup sidexConstants
*/
#define SIDEX_ENCODE_NONE           ((int32_t)0)


/**
* @brief  encoding / cdata section
* @ingroup sidexConstants
*/
#define SIDEX_ENCODE_CDATA          ((int32_t)1)


/**
* @brief  encoding / base64
* @ingroup sidexConstants
*/
#define SIDEX_ENCODE_BASE64         ((int32_t)2)


/**
* @brief  unknown encoding
* @ingroup sidexConstants
*/
#define SIDEX_ENCODE_UNKNOWN        ((int32_t)3)


/**
* @brief    Typedefinition of a SIDEX_STRING_ENCODING_TYPE / 32bit signed
* @ingroup sidexTypes
*/
typedef int32_t               SIDEX_STRING_ENCODING_TYPE;


/**
* @brief  encoded string Format is unknown
* @ingroup sidexConstants
*/
#define SIDEX_STRING_FORMAT_UNKNOWN ((int32_t)10)

/**
* @brief  encoded string Format is SIDEX
* @ingroup sidexConstants
*/
#define SIDEX_STRING_FORMAT_SIDEX ((int32_t)11)

/**
* @brief  encoded string Format is XML
* @ingroup sidexConstants
*/
#define SIDEX_STRING_FORMAT_XML ((int32_t)12)

/**
* @brief  encoded string Format is JSON
* @ingroup sidexConstants
*/
#define SIDEX_STRING_FORMAT_JSON ((int32_t)13)

/**
* @brief  encoded string Format is CVS
* @ingroup sidexConstants
*/
#define SIDEX_STRING_FORMAT_CVS ((int32_t)14)

/**
* @brief  encoded string Format is INI
* @ingroup sidexConstants
*/
#define SIDEX_STRING_FORMAT_INI ((int32_t)15)


#endif // SIDEXSTDTYPES_H
