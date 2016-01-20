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

#ifndef TMLEVENTDATA_H
#define TMLEVENTDATA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlStdTypes.h"
#include <string>

using namespace std;

class tmlEventData //Event data class
{
private:
    /* data */
   TML_COMMAND_HANDLE wValue;

protected: 
	  /* data */
    string sReturnString;
    int m_iRefCounter;

public:
	  /* methods */
    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlEventData.
     */
    explicit tmlEventData(TML_COMMAND_HANDLE eValue);


    /**
     * @brief    Destructor.
     */
    virtual ~tmlEventData();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief   Get the value of this object.
     *
     * @returns the value.
     */
    TML_COMMAND_HANDLE getValue();


    /**
     * @brief   Get the string representation of this data object.
     *
     *          The return character array is owned by the base class tmlEventData.<br> 
     *          Thus, the caller must not modify or free the memory !<br>
     *          In case of a following call the string has to be copied before.
     *
     * @returns the string representation.
     */
    virtual char* toString();  


    /**
     * @brief   Decrement the reference counter value of this data object for the memory management.
     *
     *          A reference counter value of 0 signals that there is no more reference to the object.<br>
     *          In this case the internal memory management will free allocated memory.
     *
     * @returns The reference counter value after the opperation.
     */
    int decRef();


    /**
     * @brief   Increment the reference counter value of this data object for the memory management.
     *
     * @returns The reference counter value after the opperation.
     */
    int incRef();


    /**
     * @brief   Get the reference counter value of this data object for the memory management.
     *
     * @returns The reference counter value.
     */
    int getRef();
};

#endif // TMLEVENTDATA_H
