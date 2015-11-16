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
 
#include "unicode.h"
#ifdef LINUX
  #include <iconv.h>
  #include <wchar.h>
  #include <errno.h>
  #include <stdio.h>
  #include <string.h>
#else // LINUX
  #include <windows.h>
  #include <stdio.h>
#endif // LINUX
  #include "sidexUtils.h"
 

/**
 * Different type keywords for iconv_open
 */
#define COMMON_UTF8 "UTF-8"

#define WCHAR_32_LE "UTF-32LE"
#define WCHAR_32_BE "UTF-32BE"
#define WCHAR_16_LE "UTF-16LE"
#define WCHAR_16_BE "UTF-16BE"

/**
 * The actual type keywords for iconv_open
 */
static const char* mySystemsWCharType = NULL;

/**
 * @brief helper method returns the systems wchar type for iconv_open
 */
const char* systemWCharType(){
  if (NULL == mySystemsWCharType){
    int wCharSize = sizeof(wchar_t);
    int num = 1;
    if(*(char *)&num == 1)
    {
      if (4 == wCharSize){
        mySystemsWCharType = WCHAR_32_LE;
      }
      else{
        mySystemsWCharType = WCHAR_16_LE;
      }
    }
    else
    {
      if (4 == wCharSize){
        mySystemsWCharType = WCHAR_32_BE;
      }
      else{
        mySystemsWCharType = WCHAR_16_BE;
      }
    }
  }
  return mySystemsWCharType;
}

/**
 * @brief convert wchar_t to UTF8
 */
char* UTF32toUTF8(wchar_t* utf16, SIDEX_INT32* iLength)
{
  const char* fromcode;
  int size;
#ifdef LINUX
  fromcode = systemWCharType();
  size = sizeof(wchar_t);
#else // LINUX
  fromcode = "UTF-16LE";
  size = 2;
#endif // LINUX
  return UTF32toUTF8(utf16, iLength, fromcode, size);
}

/**
 * @brief convert wchar_t to UTF8
 */
char* UTF32toUTF8(wchar_t* utf16, SIDEX_INT32* iLength, const char *fromcode, int fromSize)
{
  char* utf8 = NULL;

  if (NULL == utf16){
    *iLength = 0;
    return NULL;
  }

#ifdef LINUX
   char* utf8Out = NULL;
   size_t inbytesleft;

   inbytesleft = wcslen(utf16);
   if (0 < inbytesleft){
     size_t outbytesleftIn;
     size_t outbytesleftOut;

     iconv_t cd = iconv_open (COMMON_UTF8, fromcode);

     outbytesleftIn = outbytesleftOut =(inbytesleft+1)*fromSize;
     utf8 = utf8Out = new char[(inbytesleft+1)*4];
     inbytesleft *= fromSize;
     long len = iconv(cd, (char**)&utf16, &inbytesleft, &utf8Out, &outbytesleftOut);
     if (-1 == len)
     { 
       printf ("UTF32toUTF8 / Error = %d\n", errno);
     }
     else{
       *iLength = outbytesleftIn - outbytesleftOut;
       // Termination;
       utf8[*iLength] = '\0';
     }
     iconv_close (cd);
   }
   else{
     *iLength = 0;
     if (NULL != utf16){
       utf8 = new char[1];
       // Termination;
       utf8[*iLength] = '\0';
     }
   }
#else // LINUX
    int len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0);
    if (len>0)
    {
       utf8 = new char[len];
       if (utf8){
         WideCharToMultiByte(CP_UTF8, 0, utf16, -1, utf8, len, 0, 0);
         if (NULL == utf8){
           DWORD dw = GetLastError();
           printf ("Error on UTF16toUTF8 %d\n", dw);
         }
         else{
           *iLength = len - 1;
         }
       }
    }
#endif // LINUX
   return utf8;
}


/**
 * @brief convert wchar_t to UTF8
 */
char* UTF16toUTF8(wchar_t* utf16, SIDEX_INT32* iLength)
{
  char* utf8 = NULL;

  if (NULL == utf16){
    *iLength = 0;
    return NULL;
  }

#ifdef LINUX
   int fromSize = 2;
   char* utf8Out = NULL;
   size_t inbytesleft;

   inbytesleft = strlen16((char16_t*)utf16);
   if (0 < inbytesleft){
     size_t outbytesleftIn;
     size_t outbytesleftOut;

     const char *fromcode = "UTF-16LE";

     iconv_t cd = iconv_open (COMMON_UTF8, fromcode);

     outbytesleftIn = outbytesleftOut =(inbytesleft+1)*fromSize;
     utf8 = utf8Out = new char[(inbytesleft+1)*4];
     inbytesleft *= fromSize;
     long len = iconv(cd, (char**)&utf16, &inbytesleft, &utf8Out, &outbytesleftOut);
     if (-1 == len)
     { 
       printf ("UTF16toUTF8 / Error = %d\n", errno);
     }
     else{
       *iLength = outbytesleftIn - outbytesleftOut;
       // Termination;
       utf8[*iLength] = '\0';
     }
     iconv_close (cd);
   }
   else{
     *iLength = 0;
     if (NULL != utf16){
       utf8 = new char[1];
       // Termination;
       utf8[*iLength] = '\0';
     }
   }
#else // LINUX
    int len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0);
    if (len>0)
    {
       utf8 = new char[len];
       if (utf8){
         WideCharToMultiByte(CP_UTF8, 0, utf16, -1, utf8, len, 0, 0);
         if (NULL == utf8){
           DWORD dw = GetLastError();
           printf ("Error on UTF16toUTF8 %d\n", dw);
         }
         else{
           *iLength = len - 1;
         }
       }
    }
#endif // LINUX
   return utf8;
}


/**
 * @brief convert UTF8 to wchar_t
 */
wchar_t* UTF8toUTF32(char* utf8, SIDEX_INT32* iLength)
{
  const char* tocode;
  int size;
#ifdef LINUX
  tocode = systemWCharType();
  size = sizeof(wchar_t);
#else // LINUX
  tocode = "UTF-16LE";
  size = 2;
#endif // LINUX
  return UTF8toUTF32(utf8, iLength, tocode, size);
}

/**
 * @brief convert UTF8 to wchar_t
 */
wchar_t* UTF8toUTF32(char* utf8, SIDEX_INT32* iLength, const char *tocode, int toSize)
{
  wchar_t* utf16 = NULL;

#ifdef LINUX
   wchar_t* utf16Out = NULL;

   iconv_t cd = iconv_open (tocode, COMMON_UTF8);

   size_t inbytesleft;
   size_t outbytesleftIn;
   size_t outbytesleftOut;
   size_t outWcharCout;
   inbytesleft = strlen(utf8) + 1;
   outWcharCout = inbytesleft;
   outbytesleftIn = outbytesleftOut = outWcharCout * toSize;
   utf16 = utf16Out = new wchar_t[outWcharCout];

   long len = iconv(cd, (char**)&utf8, &inbytesleft, (char**)&utf16Out, &outbytesleftOut);
   if (-1 == len)
   {
     printf ("UTF8toUTF32 / Error = %d\n", errno);
   }
   else{
     *iLength = ((outbytesleftIn - outbytesleftOut) / toSize) - 1;
   }
   iconv_close (cd);
#else // LINUX
   int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
   if (len>0)
   { 
      utf16 = new wchar_t[len];
      if (utf16){
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, utf16, len);
        if (NULL == utf16){
          DWORD dw = GetLastError();
          printf ("Error on UTF8toUTF16 %d\n", dw);
        }
        else{
          *iLength = len - 1;
        }
      }
   }
   else{
     *iLength = 0;
   }
#endif // LINUX
   return utf16;
}

/**
 * @brief convert UTF8 to wchar_t
 */
wchar_t* UTF8toUTF16(char* utf8, SIDEX_INT32* iLength)
{
  wchar_t* utf16 = NULL;

#ifdef LINUX
   const char *tocode = "UTF-16LE";
   int toSize = 2;
   wchar_t* utf16Out = NULL;

   iconv_t cd = iconv_open (tocode, COMMON_UTF8);

   size_t inbytesleft;
   size_t outbytesleftIn;
   size_t outbytesleftOut;
   size_t outWcharCout;
   inbytesleft = strlen(utf8) + 1;
   outWcharCout = inbytesleft;
   outbytesleftIn = outbytesleftOut = outWcharCout * toSize;
   utf16 = utf16Out = new wchar_t[outWcharCout];

   long len = iconv(cd, (char**)&utf8, &inbytesleft, (char**)&utf16Out, &outbytesleftOut);
   if (-1 == len)
   {
     printf ("UTF8toUTF16 / Error = %d\n", errno);
   }
   else{
     *iLength = ((outbytesleftIn - outbytesleftOut) / toSize) - 1;
   }
   iconv_close (cd);
#else // LINUX
   int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
   if (len>0)
   { 
      utf16 = new wchar_t[len];
      if (utf16){
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, utf16, len);
        if (NULL == utf16){
          DWORD dw = GetLastError();
          printf ("Error on UTF8toUTF16 %d\n", dw);
        }
        else{
          *iLength = len - 1;
        }
      }
   }
   else{
     *iLength = 0;
   }
#endif // LINUX
   return utf16;
}

/**
 * @brief convert UTF8 to wchar_t
 */
SIDEX_INT32 UTF8toUTF32_length_request(char* utf8)
{
  const char* tocode;
  int size;
#ifdef LINUX
  tocode = systemWCharType();
  size = sizeof(wchar_t);
#else // LINUX
  tocode = "UTF-16LE";
  size = 2;
#endif // LINUX
  return UTF8toUTF_X_length_request(utf8, tocode, size);
}

/**
 * @brief convert UTF8 to wchar_t
 */
SIDEX_INT32 UTF8toUTF16_length_request(char* utf8)
{
  const char* tocode;
  int size;
  tocode = "UTF-16LE";
  size = 2;
  return UTF8toUTF_X_length_request(utf8, tocode, size);
}

/**
 * @brief returns the necessary length for a UTF8 to wchar_t convert operation
 */
SIDEX_INT32 UTF8toUTF_X_length_request(char* utf8, const char *tocode, int toSize){
  SIDEX_INT32 iLength = 0;

#ifdef LINUX
   wchar_t* utf16 = NULL;
   wchar_t* utf16Out = NULL;

   iconv_t cd = iconv_open (tocode, COMMON_UTF8);

   size_t inbytesleft;
   size_t outbytesleftIn;
   size_t outbytesleftOut;
   size_t outWcharCout;
   inbytesleft = strlen(utf8) + 1;
   outWcharCout = inbytesleft;
   outbytesleftIn = outbytesleftOut = outWcharCout * toSize;
   utf16 = utf16Out = new wchar_t[outWcharCout];

   long len = iconv(cd, (char**)&utf8, &inbytesleft, (char**)&utf16Out, &outbytesleftOut);
   if (-1 == len)
   {
     printf ("UTF8toUTF16_length_requestUNI / Error = %d\n", errno);
   }
   else{
     iLength = ((outbytesleftIn - outbytesleftOut) / toSize) - 1;
   }
   iconv_close (cd);
   delete[] utf16;
#else // LINUX
   int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
   if (len>1)
   { 
     iLength = len - 1;
   }
   else{
     iLength = 0;
   }
#endif // LINUX
   return iLength;
}
