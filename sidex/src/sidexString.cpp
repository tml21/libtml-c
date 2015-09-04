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

#include <string.h>
#include "sidexErrors.h"
#include "sidexString.h"
#include "sidexUtils.h"
#include "unicode.h"

/**
 * @brief    Constructor.
 */
sidexString::sidexString()
{
   m_encoding = SIDEX_ENCODE_NONE;
   m_stringFormat = SIDEX_STRING_FORMAT_UNKNOWN;
   type = SIDEX_DATA_TYPE_STRING;
   size = 0;
   xValueLength = 0;
   wValueLength = 0;
   xValue = NULL;
   wValue = NULL;
   sValue = NULL;
   m_stringFormatX = SIDEX_HANDLE_TYPE_NULL;
   m_stringFormatW = SIDEX_HANDLE_TYPE_NULL;

   incRef();
}


/**
 * @brief    Constructor with initial value.
 */
sidexString::sidexString(char* newValue)
{
   m_encoding = SIDEX_ENCODE_NONE;
   m_stringFormat = SIDEX_STRING_FORMAT_UNKNOWN;
   type = SIDEX_DATA_TYPE_STRING;
   size = (int)strlen(newValue);
   xValueLength = 0;
   wValueLength = 0;
   xValue = NULL;
   wValue = NULL;
   m_stringFormatX = SIDEX_HANDLE_TYPE_NULL;
   m_stringFormatW = SIDEX_HANDLE_TYPE_NULL;

   allocEncodedDependentString(newValue, size, m_encoding);
   incRef();
}


/**
 * @brief    Constructor with initial value.
 */
sidexString::sidexString(char* newValue, SIDEX_ENCODE newEncoding)
{
   m_encoding = newEncoding;
   m_stringFormat = SIDEX_STRING_FORMAT_UNKNOWN;
   if(m_encoding == SIDEX_ENCODE_BASE64){
     type = SIDEX_DATA_TYPE_BINARY;
   }
   else{
     type = SIDEX_DATA_TYPE_STRING;
   }
   size = (int)strlen(newValue);
   xValueLength = 0;
   wValueLength = 0;
   xValue = NULL;
   wValue = NULL;
   m_stringFormatX = SIDEX_HANDLE_TYPE_NULL;
   m_stringFormatW = SIDEX_HANDLE_TYPE_NULL;

   allocEncodedDependentString(newValue, size, newEncoding);
   incRef();
}


/**
 * @brief    Constructor with initial value.
 */
sidexString::sidexString(char* newValue, int newSize, SIDEX_ENCODE newEncoding)
{
   m_encoding = newEncoding;
   m_stringFormat = SIDEX_STRING_FORMAT_UNKNOWN;
   if(m_encoding == SIDEX_ENCODE_BASE64){
     type = SIDEX_DATA_TYPE_BINARY;
   }
   else{
     type = SIDEX_DATA_TYPE_STRING;
   }
   allocEncodedDependentString(newValue, newSize, newEncoding);
   xValueLength = 0;
   wValueLength = 0;
   xValue = NULL;
   wValue = NULL;
   m_stringFormatX = SIDEX_HANDLE_TYPE_NULL;
   m_stringFormatW = SIDEX_HANDLE_TYPE_NULL;

   incRef();
}


/**
 * @brief    Destructor.
 */
sidexString::~sidexString()
{
  cleanUp();
}


/**
 * @brief    Cleans up refCounter dependent allocations
 */
void sidexString::cleanUp(){
  if (getRef())
    if (decRef() == 0){
      delete[] sValue;
      if (NULL != xValue){
        delete xValue;
        xValueLength = 0;
        xValue = NULL;
      }
      if (NULL != wValue){
        delete wValue;
        wValueLength = 0;
        wValue = NULL;
      }
      if (SIDEX_HANDLE_TYPE_NULL != m_stringFormatX){
        delete m_stringFormatX;
        m_stringFormatX = SIDEX_HANDLE_TYPE_NULL;
      }
      if (SIDEX_HANDLE_TYPE_NULL != m_stringFormatW){
        delete m_stringFormatW;
        m_stringFormatW = SIDEX_HANDLE_TYPE_NULL;
      }
    }
}


/**
 * @brief   Get type of this data object.
 */
SIDEX_DATA_TYPE sidexString::getType()
{
   return type;
}


/**
 * @brief   Get the string representation of this data object.
 */
char* sidexString::toString()
{
  if(m_encoding == SIDEX_ENCODE_BASE64){
    int iInSize = size;
    char* sTemp = sidexutil_encodeBase64(sValue, &iInSize);
    sReturnString = sTemp;
    return sTemp;
  }
  else{
     char* outbuf = new char[size+1];
#if defined(LINUX) || defined (MINGW_BUILD)
     memcpy (outbuf, sValue, size);
#else
     memcpy_s(outbuf, size, sValue, size);
#endif
     outbuf[size] = '\0';
     return outbuf;
  }
}


/**
 * @brief   Get the char string value of this object.
 */
char* sidexString::getValue_A()
{
  return sValue;
}


/**
 * @brief   Get the char string value of this object.
 */
wchar_t* sidexString::getValue_X(SIDEX_INT32* iLength)
{
  if (NULL == xValue){
    if (NULL != sValue){
      xValue = UTF8toUTF32(sValue, &xValueLength);
    }
    else{
      xValueLength = 0;
    }
  }
  *iLength = xValueLength;
  return xValue;
}


/**
 * @brief   Get the char string value of this object.
 */
char16_t* sidexString::getValue_W(SIDEX_INT32* iLength)
{
  if (NULL == wValue){
    if (NULL != sValue){
      wValue = (char16_t*)UTF8toUTF16(sValue, &wValueLength);
    }
    else{
      wValueLength = 0;
    }
  }
  *iLength = wValueLength;
  return wValue;
}


/**
 * @brief   Get the length of the char string value (Unicode) of this object
 */
SIDEX_INT32 sidexString::getLength_X()
{
  SIDEX_INT32 iLengthUtf16;

  getValue_X(&iLengthUtf16);
  return iLengthUtf16;
}


/**
 * @brief   Get the length of the char string value (Unicode) of this object
 */
SIDEX_INT32 sidexString::getLength_W()
{
  SIDEX_INT32 iLengthUtf16;

  getValue_W(&iLengthUtf16);
  return iLengthUtf16;
}


/**
 * @brief    Set the encoding of the char string value.
 */
void sidexString::setEncoding(SIDEX_ENCODE newEncoding)
{
   m_encoding = newEncoding;
   if(m_encoding == SIDEX_ENCODE_BASE64){
     type = SIDEX_DATA_TYPE_BINARY;
   }
   else{
     type = SIDEX_DATA_TYPE_STRING;
   }
}


/**
 * @brief    Get the encoding of the char string value.
 */
SIDEX_ENCODE sidexString::getEncoding()
{
   return m_encoding;
}


/**
 * @brief    Set the string data format of the char string value.
 */
SIDEX_INT32 sidexString::setStringFormat(const char* format)
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  SIDEX_STRING_ENCODING_TYPE iFormat = sidexutil_StringToStringFormat(format);
  if (SIDEX_CONV_NOT_FOUND == iFormat){
    iRet = SIDEX_ERR_WRONG_ENCODING_FORMAT;
  }
  else{
    if (SIDEX_HANDLE_TYPE_NULL != m_stringFormatW)
      delete (m_stringFormatW);
    m_stringFormatW = SIDEX_HANDLE_TYPE_NULL;
    if (SIDEX_HANDLE_TYPE_NULL != m_stringFormatX)
      delete (m_stringFormatX);
    m_stringFormatX = SIDEX_HANDLE_TYPE_NULL;

    m_stringFormat = iFormat;
  }
  return iRet;
}




/**
 * @brief    Set the string data format of the char string value.
 */
SIDEX_INT32 sidexString::setStringFormat(SIDEX_STRING_ENCODING_TYPE format)
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  const char* sFormat = sidexutil_StringFormatToString(format);
  if (NULL == sFormat){
    iRet = SIDEX_ERR_WRONG_ENCODING_FORMAT;
  }
  else{
    if (SIDEX_HANDLE_TYPE_NULL != m_stringFormatW)
      delete (m_stringFormatW);
    m_stringFormatW = SIDEX_HANDLE_TYPE_NULL;
    if (SIDEX_HANDLE_TYPE_NULL != m_stringFormatX)
      delete (m_stringFormatX);
    m_stringFormatX = SIDEX_HANDLE_TYPE_NULL;

    m_stringFormat = format;
  }
  return iRet;
}


/**
  * @brief    Get the string data format of the char string value.
  */
void sidexString::getStringFormat_A(const char** format){
  *format = sidexutil_StringFormatToString(m_stringFormat);
}


/**
  * @brief    Get the string data format of the char string value.
  */
void sidexString::getStringFormat_X(const wchar_t** format){
   if (SIDEX_HANDLE_TYPE_NULL != m_stringFormatX){
    *format = m_stringFormatX;
  }
  else{
    SIDEX_INT32 iLength;
    m_stringFormatX = UTF8toUTF32((char*)(sidexutil_StringFormatToString(m_stringFormat)), &iLength);
    *format = m_stringFormatX;
  }
}


/**
  * @brief    Get the string data format of the char string value.
  */
void sidexString::getStringFormat_W(const char16_t** format){
   if (SIDEX_HANDLE_TYPE_NULL != m_stringFormatW){
    *format = m_stringFormatW;
  }
  else{
    SIDEX_INT32 iLength;
    m_stringFormatW = (char16_t*)UTF8toUTF16((char*)(sidexutil_StringFormatToString(m_stringFormat)), &iLength);
    *format = m_stringFormatW;
  }
}


/**
 * @brief    Get the string data format of the char string value.
 */
SIDEX_STRING_ENCODING_TYPE sidexString::getStringFormat()
{
   return m_stringFormat;
}


/**
 * @brief    Set the size of the char string value.
 */
void sidexString::setSize(int newSize)
{
   size = newSize;
}


/**
 * @brief    Get the size of the char string value.
 */
int sidexString::getSize()
{
   return size;
}

/**
 * @brief    Allocates the input string value Dependent of the encoding.
 */
void sidexString::allocEncodedDependentString(char* sInputString, int iSize, SIDEX_ENCODE iEncoding)
{
    char* outbuf = new char[iSize+1];
#if defined(LINUX) || defined (MINGW_BUILD)
    memcpy (outbuf, sInputString, iSize);
#else
    memcpy_s(outbuf, iSize, sInputString, iSize);
#endif
    outbuf[iSize] = '\0';
    sValue = outbuf; // New allocated string
    size = iSize; // size will be the same
}
