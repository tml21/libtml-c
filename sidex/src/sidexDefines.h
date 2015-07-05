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

/** @file sidexDefines.h
  * @brief SIDEX (Simple Data Exchange Format) defines and data type definitions
  */

#ifndef SIDEXDEFINES_H
#define SIDEXDEFINES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
* @brief    API version number
*/
#define API_VERSION_NUMBER 101

/**
* @brief    Library version number
*/
#define LIB_VERSION_NUMBER  10105

/**
* @brief    Library version string
*/
#define LIB_VERSION_STRING_A  (char*) "1.1.5"

/**
* @brief    Sub version number 2
*/
#define SUB_VERSION_NUMBER2 1

/**
* @brief    Typedefinition of SIDEX_DOC
*/
typedef void* SIDEX_DOC;

/**
* @brief    Typedefinition of SIDEX_NODE
*/
typedef void* SIDEX_NODE;

/*
 * Attributes for datatype of value entries
 * each node containing a data entry is typed. Before reading a node the type
 * has to be checked and if not matching the expected type an error should be reported.
 * Example: type=int
 */
/**
* @brief    Name of the attribute containing the type identifier
*/
#define SIDEX_TYPE_ATTR         "type"
/**
* @brief    Data type is none
*/
#define SIDEX_TYPE_ATTR_NONE "none" 
/**
* @brief    Data type is boolean
*/
#define SIDEX_TYPE_ATTR_BOOLEAN "bool" 
/**
* @brief    Data type is integer
*/
#define SIDEX_TYPE_ATTR_INTEGER "int" 
/**
* @brief    Data type is float
*/
#define SIDEX_TYPE_ATTR_FLOAT   "float"
/**
* @brief    Data type is DateTime
*/
#define SIDEX_TYPE_ATTR_DATETIME   "datetime"
/**
* @brief    Data type is string
*/
#define SIDEX_TYPE_ATTR_STRING  "string"
/**
* @brief    Data type is binary string
*/
#define SIDEX_TYPE_ATTR_BINARY  "binary"
/**
* @brief    Data type is  a structure containing a List
*/
#define SIDEX_TYPE_ATTR_LIST    "list"
/**
* @brief    Data type is a structure containing a Dict
*/
#define SIDEX_TYPE_ATTR_DICT    "dict"
/**
* @brief    Data type is a structure containing a Table
*/
#define SIDEX_TYPE_ATTR_TABLE   "table"
/**
* @brief    Data type is unknown
*/
#define SIDEX_TYPE_ATTR_UNKNOWN "unknown"

/**
* @brief    Definition of the encode attribute
*/
#define SIDEX_ENC_ATTR         "encode" // name of the encoding attribute

/**
* @brief    Definition of the base64 encoding
*/
#define SIDEX_ENC_ATTR_BASE64  "base64"

/**
* @brief    Definition of the raw / no encoding
*/
#define SIDEX_ENC_ATTR_NONE    "raw"    // no encoding

/**
* @brief    Definition of the cdata / write explicit into CDATA section 
*/
#define SIDEX_ENC_ATTR_CDATA   "cdata"    // data into CDATA section

/**
* @brief    Definition of SIDEX_LIST_ITEM
*/
#define SIDEX_LIST_ITEM  "Item"

/**
* @brief    Definition of SIDEX_DICT_ITEM
*/
#define SIDEX_DICT_ITEM  "Item"

/**
* @brief    Definition of SIDEX_TABLE_ITEM
*/
#define SIDEX_TABLE_ITEM "Row"

/**
* @brief    Definition of the coded string format attribute
*/
#define SIDEX_STRING_FORMAT_ATTR			"format"

/**
* @brief    Coded string format is unknown (default)
*/
#define SIDEX_STRING_FORMAT_ATTR_UNKNOWN_A	"unknown"

/**
* @brief    Coded string format is defined as sidex
*/
#define SIDEX_STRING_FORMAT_ATTR_SIDEX_A		"sidex"

/**
* @brief    Coded string format is defined as xml
*/
#define SIDEX_STRING_FORMAT_ATTR_XML_A		"xml"

/**
* @brief    Coded string format is defined as json
*/
#define SIDEX_STRING_FORMAT_ATTR_JSON_A		"json"

/**
* @brief    Coded string format is defined as cvs
*/
#define SIDEX_STRING_FORMAT_ATTR_CVS_A		"cvs"

/**
* @brief    Coded string format is defined as ini
*/
#define SIDEX_STRING_FORMAT_ATTR_INI_A		"ini"


/**
* @brief    Definition of SIDEX_TABLE_COLUMNS
*/
#define SIDEX_TABLE_COLUMNS "ColumnNames"

/**
* @brief    Max- amount of dict entrys
*/
#define SIDEX_DEFAULT_AMOUNT_OF_DICT_ENTRYS 100

/**
* @brief    Definition of the string representation of none
*/
#define SIDEX_NONE_STRING ""

/**
* @brief    Coded string for copyright
*/
#define SIDEX_STRING_COPYRIGHT_A	(char*) "Copyright \xC2\xA9 2010-2015 wobe-system GmbH\n\
All rights reserved."

/**
* @brief  TML UNICODE data identifier
*/
typedef enum UNCIODE_ID_ENUM
{
   idUNICODE_ASCII,
   idUNICODE_WCHAR_T,
   idUNICODE_CHAR16_T,
}tmlUnicodeID;

#endif // SIDEXDEFINES_H
