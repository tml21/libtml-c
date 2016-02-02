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

#ifndef SDXRT_TCHAR_H
#define SDXRT_TCHAR_H

#include <sidex.h>
#include <stdio.h>

/** @defgroup Wrapping_Sidex_TChar
* @brief Helper functions, which act as Wrappers for Sidex_TChar specific function calls
*/

//transfer function for comparing
int sdxrt_cmp( const SIDEX_TCHAR * str1, const SIDEX_TCHAR * str2);

//wrapper function for concatenating strings
SIDEX_TCHAR * sdxrt_cat( SIDEX_TCHAR * destination, const SIDEX_TCHAR * source);

//wrapper function for formatting an integer to string
int sdxrt_printf( SIDEX_TCHAR * buffer, const SIDEX_TCHAR * format, SIDEX_INT32 arg);

//wrapper function for getting the length of a string
int sdxrt_len(const SIDEX_TCHAR* str);


//makro for creating strings in the right format
#ifdef SIDEX_UNICODE
#define _sdxrtT(a) (wchar_t*) L ## a
#else // SIDEX_UNICODE
#define _sdxrtT(a) (char*) a
#endif // SIDEX_UNICODE


#endif //SDXRT_TCHAR_H