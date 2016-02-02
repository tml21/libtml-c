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

#ifdef MINGW_BUILD
  // Do the following undef for swprintf
  #undef __STRICT_ANSI__ 
#endif // MINGW_BUILD
#include <sidex.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <iostream>
using namespace std;


/** @ingroup Wrapping_Sidex_TChar
* @brief Wrapper function for comparing two strings with each other
* @param const SIDEX_TCHAR* str1 : First string
* @param const SIDEX_TCHAR* str2 : Second string
* @returns int : Returns value of either wcscmp or strcmp, depending on definition of SIDEX_UNICODE
*/
int sdxrt_cmp( const SIDEX_TCHAR * str1, const SIDEX_TCHAR * str2) {
#ifdef SIDEX_UNICODE
    return wcscmp(str1, str2);
#else /// SIDEX_UNICODE
    return strcmp(str1, str2);
#endif // SIDEX_UNICODE
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Wrapper function for concatenating two strings
* @param SIDEX_TCHAR* destination : First string for concatening and the result will be returned in place
* @param const SIDEX_TCHAR* source : Second string
* @returns int : Returns value of either wcscat or strcat, depending on definition of SIDEX_UNICODE
*/
SIDEX_TCHAR * sdxrt_cat( SIDEX_TCHAR * destination, const SIDEX_TCHAR * source) {
#ifdef SIDEX_UNICODE
    return wcscat(destination, source);
#else /// SIDEX_UNICODE
    return strcat(destination, source);
#endif // SIDEX_UNICODE
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Wrapper function for formatting strings
* @param SIDEX_TCHAR* buffer : Buffer for returning formatted result
* @param const SIDEX_TCHAR* format : Param arg will be formatted to this
* @param SIDEX_INT32 arg : Argument that is to be formatted to the param format
* @returns int : Returns value of either _swprintf if Linux, swprintf if not Linux, or sprintf, depending on definition of SIDEX_UNICODE
*/
int sdxrt_printf( SIDEX_TCHAR * buffer, const SIDEX_TCHAR * format, SIDEX_INT32 arg) {
#ifdef SIDEX_UNICODE
    #ifndef LINUX
      #ifdef MINGW_BUILD
          return swprintf(buffer, format, arg);
      #else 
          return _swprintf(buffer, format, arg);
      #endif
    #else 
        return swprintf(buffer, 8, format,arg);
    #endif
#else /// SIDEX_UNICODE
    return sprintf(buffer, format, arg);
#endif // SIDEX_UNICODE
}

/** @ingroup Wrapping_Sidex_TChar
* @brief Wrapper function for returning string length
* @param const SIDEX_TCHAR* str : string to return length for
* @returns int : Returns value of either wcslen or strlen, depending on definition of SIDEX_UNICODE
*/
int sdxrt_len(const SIDEX_TCHAR* str) {
#ifdef SIDEX_UNICODE
    return wcslen(str);
#else  //SIDEX_UNICODE
    return strlen(str);
#endif  //SIDEX_UNICODE
}