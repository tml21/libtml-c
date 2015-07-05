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
 
/** @file globalDefines.h
  * @brief this file contains platform dependent function call definitions. For details please open file directly.
  */

#ifndef GLOBALDEFINES_H
#define GLOBALDEFINES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef DOXYGEN_GENERATION

#ifdef LINUX 
  ///////////////////////////////
  // The Linux and OS-X
  #ifdef OS_X
    #define FUNC_C_DECL __attribute__ (( ))
  #else //OS_X
    #define FUNC_C_DECL __attribute__ (( ))
    #ifndef GCC4
      #define FUNC_C_DECL __attribute__ ((cdecl))
    #endif // GCC4
  #endif // OS_X
#else // LINUX
  //The Windows definitions:
  #ifdef USE_STDCALL
    #define FUNC_C_DECL __stdcall
  #else // USE_STDCALL
    #ifdef USE_FASTCALL
      #define FUNC_C_DECL __fastcall
    #else // USE_FASTCALL
      #define FUNC_C_DECL __cdecl
    #endif // USE_FASTCALL
  #endif // USE_STDCALL
#endif // LINUX

#define DLL_CALL_CONV FUNC_C_DECL

#endif // DOXYGEN_GENERATION

#endif // GLOBALDEFINES_H
