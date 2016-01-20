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

#ifndef SIDEXTYPESTRING_H
#define SIDEXTYPESTRING_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexTypeBase.h"

class sidexString : public sidexBase
{
private:
    /* data */
    SIDEX_ENCODE m_encoding;
    SIDEX_ENCODE m_stringFormat;
    char* sValue;
    wchar_t*  xValue;
    char16_t* wValue;
    int size;
    SIDEX_INT32 xValueLength;
    SIDEX_INT32 wValueLength;
    // global attribute to store a converted version request
    wchar_t*     m_stringFormatX;
    char16_t*    m_stringFormatW;

protected: 
    /* data */
    SIDEX_DATA_TYPE type;
    /* methods */


    
    /**
     * @brief   Allocates the input string value dependent of the encoding.
     *
     *          The value will be stored to the sValue menber.<br>
     *          The length of the allocated string will be to the size menber.
     *
     * @param   sInputString  The input string to operate.
     * @param   iSize         Size of the sInputString.
     * @param   iEncoding     Encoding of the sInputString.<br>
     *                        'SIDEX_ENCODE_NONE' (raw) or 'SIDEX_ENCODE_BASE64' (base64).
     *
     * @see sidexStdTypes.h
     */
    void allocEncodedDependentString(char* sInputString, int iSize, SIDEX_ENCODE iEncoding);

public:
    /* methods */
    /**
     * @brief    Constructor.
     *
     * @returns an instance of sidexString.
     */
    sidexString();


    /**
     * @brief    Constructor with initial value.
     *
     * @param   newValue Initial content of the sidexString.
     *
     * @returns an instance of sidexString.
     */
    explicit sidexString(char* newValue);


    /**
     * @brief    Constructor with initial value.
     *
     * @param   newValue    Initial content of the sidexString.
     * @param   newEncoding Encoding of the initial content.<br>
     *                      'SIDEX_ENCODE_NONE' (raw) or 'SIDEX_ENCODE_BASE64' (base64).
     *
     * @returns an instance of sidexString.
     */
    sidexString(char* newValue, SIDEX_ENCODE newEncoding);


    /**
     * @brief    Constructor with initial value.
     *
     * @param   newValue    Initial content of the sidexString.
     * @param   newSize     Size if the initial content string.
     * @param   newEncoding Encoding of the initial content.<br>
     *                      'SIDEX_ENCODE_NONE' (raw) or 'SIDEX_ENCODE_BASE64' (base64).
     *
     * @returns an instance of sidexString.
     */
    sidexString(char* newValue, int newSize, SIDEX_ENCODE newEncoding);


    /**
     * @brief    Destructor.
     */
    ~sidexString();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief   Get type of this data object.
     *
     * @returns SIDEX_DATA_TYPE_STRING / SIDEX_DATA_TYPE_BINARY.
     *
     * @see sidexStdTypes.h
     */
    SIDEX_DATA_TYPE getType();


    /**
     * @brief   Get the string representation of this data object.
     *
     *          It must be deallocated using delete().
     *
     * @returns the string representation.
     */
    char* toString();


    /**
     * @brief   Get the char string value of this object.
     *
     * @returns the char string value.
     */
    char* getValue_A();


    /**
     * @brief   Get the char string value of this object.
     *
     * @param   iLength reference to the length of the result.
     *
     * @returns the char string value.
     */
    wchar_t* getValue_X(SIDEX_INT32* iLength);

    
    /**
     * @brief   Get the char string value of this object.
     *
     * @param   iLength reference to the length of the result.
     *
     * @returns the char string value.
     */
    char16_t* getValue_W(SIDEX_INT32* iLength);

    
    /**
     * @brief   Get the length of the char string value (Unicode) of this object
     *
     * @returns the length of the char string value.
     */
    SIDEX_INT32 getLength_X();

      
    /**
     * @brief   Get the length of the char string value (Unicode) of this object
     *
     * @returns the length of the char string value.
     */
    SIDEX_INT32 getLength_W();

      
    /**
     * @brief    Set the encoding of the char string value.
     *
     * @param   newEncoding Encoding of the char string value.<br>
     *                      'SIDEX_ENCODE_NONE' (raw) or 'SIDEX_ENCODE_BASE64' (base64).
     *
     * @see sidexDefines.h, sidexStdTypes.h
     */
    void  setEncoding(SIDEX_ENCODE newEncoding);


    /**
     * @brief    Get the encoding of the char string value.
     *
     * @returns Encoding of the char string value.<br>
     *          'SIDEX_ENCODE_NONE' (raw) or 'SIDEX_ENCODE_BASE64' (base64).
     *
     * @see sidexDefines.h, sidexStdTypes.h
     */
    SIDEX_ENCODE  getEncoding();


    /**
     * @brief    Set the string data format of the char string value.

     * @param   format String data format of the char string value.
     */
    SIDEX_INT32 setStringFormat(SIDEX_STRING_ENCODING_TYPE format);


    /**
     * @brief    Set the string data format of the char string value.

     * @param   format String data format of the char string value.
     */
    SIDEX_INT32 setStringFormat(const char* format);


    /**
     * @brief    Get the string data format of the char string value.
     *
     * @returns the string data format of the char string value.
     */
    SIDEX_STRING_ENCODING_TYPE getStringFormat();


    /**
     * @brief    Get the string data format of the char string value.
     *
     * @param format   reference to the string format.
     */
    void getStringFormat_A(const char** format);


    /**
     * @brief    Get the string data format of the char string value.
     *
     * @param format   reference to the string format.
     */
    void getStringFormat_X(const wchar_t** format);


    /**
     * @brief    Get the string data format of the char string value.
     *
     * @param format   reference to the string format.
     */
    void getStringFormat_W(const char16_t** format);


    /**
     * @brief    Set the size of the char string value.
     *
     * @param   newSize size of the char string value.
     */
    void setSize(int newSize);


    /**
     * @brief    Get the size of the char string value.
     *
     * @returns the size of the char string value.
     */
    int getSize();
};

#endif // SIDEXTYPESTRING_H
