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
 
#ifndef UNICODE_H
#define UNICODE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexStdTypes.h"

/**
 * @brief helper method returns the systems wchar type for iconv_open
 */
const char* systemWCharType();


/**
 * @brief convert wchar_t to UTF8
 */
char* UTF16toUTF8(wchar_t* utf16, SIDEX_INT32* iLength);

char* UTF32toUTF8(wchar_t* utf16, SIDEX_INT32* iLength);
char* UTF32toUTF8(wchar_t* utf16, SIDEX_INT32* iLength, const char *fromcode, int fromSize);


/**
 * @brief convert UTF8 to wchar_t
 */

wchar_t* UTF8toUTF16(char* utf8, SIDEX_INT32* iLength);

wchar_t* UTF8toUTF32(char* utf8, SIDEX_INT32* iLength);
wchar_t* UTF8toUTF32(char* utf8, SIDEX_INT32* iLength, const char *tocode, int toSize);


/**
 * @brief returns the necessary length for a UTF8 to wchar_t convert operation
 */
SIDEX_INT32 UTF8toUTF16_length_request(char* utf8);
SIDEX_INT32 UTF8toUTF32_length_request(char* utf8);
SIDEX_INT32 UTF8toUTF_X_length_request(char* utf8, const char *tocode, int toSize);


#endif // UNICODE_H
