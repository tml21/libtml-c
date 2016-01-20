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
 
/** @file stdIntTypes.h
  * @brief standard integer type definitions (for windows platform). For details please open file directly.
  */

#include <stddef.h>
#include <limits.h>
#include <signal.h>


#ifndef _PSTDINT_H_INCLUDED
#define _PSTDINT_H_INCLUDED

#ifndef DOXYGEN_GENERATION

////////////////////////////////////////////////////////
//                   64 Bit
////////////////////////////////////////////////////////
#ifdef ANDROID
#ifndef __int64
    typedef signed long long int64_t;
#else
    typedef __int64 int64_t;
#endif

#ifndef uint64_t
    typedef unsigned long long uint64_t;
#else
    typedef unsigned __int64 uint64_t;
#endif
#else // ANDROID
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;
#endif

#define UINT64_C(v) v ## UI64
#define  INT64_C(v) v ## I64

#if !defined (INT64_MAX) && defined (INT64_C)
 #define INT64_MAX INT64_C (9223372036854775807)
#endif

#if !defined (INT64_MIN) && defined (INT64_C)
 #define INT64_MIN INT64_C (-9223372036854775808)
#endif

#if !defined (UINT64_MAX) && defined (INT64_C)
 #define UINT64_MAX UINT64_C (18446744073709551615)
#endif


////////////////////////////////////////////////////////
//                   32 Bit
////////////////////////////////////////////////////////
#ifndef UINT32_MAX
 #define UINT32_MAX (0xffffffffUL)
#endif

#ifndef uint32_t
  #if (ULONG_MAX == UINT32_MAX)
    typedef unsigned long uint32_t;
    #define UINT32_C(v) v ## UL
  #elif (UINT_MAX == UINT32_MAX)
    typedef unsigned int uint32_t;
    #define UINT32_C(v) v ## U
  #elif (USHRT_MAX == UINT32_MAX)
    typedef unsigned short uint32_t;
    #define UINT32_C(v) ((unsigned short) (v))
  #else
    #error "Unsupported Platform"
  #endif
#endif

#ifndef INT32_MAX
  #define INT32_MAX (0x7fffffffL)
#endif

#ifndef INT32_MIN
  #define INT32_MIN INT32_C(0x80000000)
#endif

#ifndef int32_t
  #if (LONG_MAX == INT32_MAX)
    typedef signed long int32_t;
    #define INT32_C(v) v ## L
  #elif (INT_MAX == INT32_MAX)
    typedef signed int int32_t;
    #define INT32_C(v) v
  #elif (SHRT_MAX == INT32_MAX)
    typedef signed short int32_t;
    #define INT32_C(v) ((short) (v))
  #else
    #error "Unsupported Platform"
  #endif
#endif


////////////////////////////////////////////////////////
//                   16 Bit
////////////////////////////////////////////////////////
#ifndef UINT16_MAX
  #define UINT16_MAX 0xffff
#endif

#ifndef uint16_t
  #if (UINT_MAX == UINT16_MAX)
    typedef unsigned int uint16_t;
    #define UINT16_C(v) ((uint16_t) (v))
  #elif (USHRT_MAX == UINT16_MAX)
    typedef unsigned short uint16_t;
    #define UINT16_C(v) ((uint16_t) (v))
  #else
    #error "Unsupported Platform"
  #endif
#endif

#ifndef INT16_MAX
  #define INT16_MAX 0x7fff
#endif

#ifndef INT16_MIN
  #define INT16_MIN INT16_C(0x8000)
#endif
#ifndef int16_t
  #if (INT_MAX == INT16_MAX)
    typedef signed int int16_t;
    #define INT16_C(v) ((int16_t) (v))
  #elif (SHRT_MAX == INT16_MAX)
    typedef signed short int16_t;
    #define INT16_C(v) ((int16_t) (v))
  #else
    #error "Unsupported Platform"
  #endif
#endif


////////////////////////////////////////////////////////
//                   8 Bit
////////////////////////////////////////////////////////
#ifndef UINT8_MAX
  #define UINT8_MAX 0xff
#endif

#ifndef uint8_t
  #if (UCHAR_MAX == UINT8_MAX)
    typedef unsigned char uint8_t;
    #define UINT8_C(v) ((uint8_t) v)
  #else
    #error "Platform not supported"
  #endif
#endif

#ifndef INT8_MAX
  #define INT8_MAX 0x7f
#endif

#ifndef INT8_MIN
  #define INT8_MIN INT8_C(0x80)
#endif

#ifndef int8_t
  #if (SCHAR_MAX == INT8_MAX)
    typedef signed char int8_t;
    #define INT8_C(v) ((int8_t) v)
  #else
    #error "Platform not supported"
  # endif
#endif    

#endif // DOXYGEN_GENERATION

#endif // _PSTDINT_H_INCLUDED
