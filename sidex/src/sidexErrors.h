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
 
/** @file sidexErrors.h
  * @brief SIDEX error defines
  */

#ifndef SIDEXERRORS_H
#define SIDEXERRORS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
* @brief    operation successful
* @ingroup sidexReturncodes
*/
#define SIDEX_SUCCESS                              0

//-----------------------------------------
// SIDEX- specific errors:
//-----------------------------------------

/**
* @brief    error on copying a string from or to a SIDEX object
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_DUMPCONTENT                   1001

/**
* @brief    a sidex document group/key address has no content
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_NOCONTENT                     1002

/**
* @brief    value type in the SIDEX document does not match expected type of the read operation
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_WRONG_TYPE                    1003

/**
* @brief    Key contains forbidden XML tag characters (Syntax Error)
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_WRONG_KEY                     1004

/**
* @brief    Group contains forbidden XML tag characters (Syntax Error)
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_WRONG_GROUP                   1005

/**
* @brief    key parameter is NULL
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_MISSING_KEY                   1006

/**
* @brief    invalid parameter passed
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_WRONG_PARAMETER               1007

/**
* @brief    dictionary value definition syntax error in SIDEX document
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_WRONG_DICT_SYNTAX             1008

/**
* @brief    maximun number of dictionary entries reached
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_DICT_FULL                     1009

/**
* @brief    boolean value definition syntax error
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_INVALID_BOOLEAN               1010

/**
* @brief    integer value out of range
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_INVALID_INTEGER               1011

/**
* @brief    float value out of range
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_INVALID_FLOAT                 1012

/**
* @brief    unhandled error
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_COMMON                        1013

/**
* @brief    table column already exists
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_TABLE_COLUMN_ALREADY_EXISTS   1014

/**
* @brief    DateTime format error
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_INVALID_DATETIME              1015

/**
* @brief    interation not inititialized by calling first(...) before next(...)
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_FIRST_NEXT                    1017

/**
* @brief    unicode conversion error
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_UNICODE                       1018

/**
* @brief    group name is null
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_MISSING_GROUP                 1019

/**
* @brief    syntax error in document name
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_WRONG_DOC                     1020

/**
* @brief    string encoding format is NULL or unexpected
* @ingroup sidexReturncodes
*/
#define SIDEX_ERR_WRONG_ENCODING_FORMAT         1021

#endif // SIDEXERRORS_H
