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

#ifndef SIDEXTYPEDATETIME_H
#define SIDEXTYPEDATETIME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexTypeBase.h"

class sidexDateTime : public sidexBase
{
   
private:
	  /* data */
    char* value;
    wchar_t*  xValue;
    char16_t* wValue;

protected: 
	  /* data */
    static const SIDEX_DATA_TYPE type = SIDEX_DATA_TYPE_DATETIME;

private:
	  /* methods */

    /**
     * @brief    Check the DateTime string for correct format
     *
     * @param   sDateTime The DateTime string to check.
     *
     * @returns true if string has correct format, otherwide false
     */
    bool sidexDateCheck(char* sDateTime);


public:
	  /* methods */
    /**
     * @brief    Constructor.
     *
     * @returns an instance of sidexDateTime.
     */
    sidexDateTime();


    /**
     * @brief    Constructor with initial value.
     *
     * @param   newValue Initial content of the sidexDateTime.
     *
     * @returns an instance of sidexDateTime.
     */
    explicit sidexDateTime(char* newValue);


    /**
     * @brief    Destructor.
     */
    ~sidexDateTime();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief   Get type of this data object.
     *
     * @returns SIDEX_DATA_TYPE_DATETIME.
     *
     * @see sidexStdTypes.h
     */
    SIDEX_DATA_TYPE getType();


    /**
     * @brief   Get the string representation of this data object.
     *
     *          The return character array is owned by the sidexDateTime.<br>
     *          Thus, the caller must not modify or free the memory !<br>
     *          In case of a following call the string has to be copied before.
     *
     * @returns the string representation.
     */
    char* toString();


    /**
     * @brief   Set the DateTime value of this object.
     *
     * @param   newValue the value to set.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_INVALID_DATETIME if newValue is in invalid format..
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int setValue(char* newValue);

    /**
     * @brief   Get the DateTime value of this object.
     *
     * @returns the DateTime value.
     */
    char* getValue_A();

    /**
     * @brief   Get the DateTime value of this object.
     *
     * @returns the DateTime value.
     */
    wchar_t* getValue_X();


    /**
     * @brief   Get the DateTime value of this object.
     *
     * @returns the DateTime value.
     */
    char16_t* getValue_W();
};

#endif // SIDEXTYPEDATETIME_H
