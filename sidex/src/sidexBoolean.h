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

#ifndef SIDEXTYPEBOOL_H
#define SIDEXTYPEBOOL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexTypeBase.h"


class sidexBoolean : public sidexBase
{

private:
	  /* data */
    bool value;

protected: 
	  /* data */
    static const SIDEX_DATA_TYPE type = SIDEX_DATA_TYPE_BOOLEAN;

public:
	  /* methods */
    /**
     * @brief    Constructor.
     *
     * @returns an instance of sidexBoolean
     */
    sidexBoolean();


    /**
     * @brief    Constructor with initial value.
     *
     * @param   newValue Initial content of the sidexBoolean.
     *
     * @returns an instance of sidexBoolean.
     */
    explicit sidexBoolean(bool newValue);


    /**
     * @brief    Destructor.
     */
    ~sidexBoolean();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief   Get type of this data object.
     *
     * @returns SIDEX_DATA_TYPE_DICT_BOOLEAN.
     *
     * @see sidexStdTypes.h
     */
    SIDEX_DATA_TYPE getType();


    /**
     * @brief   Get the string representation of this data object.
     *
     *          The return character array is owned by the sidexBoolean.<br>
     *          Thus, the caller must not modify or free the memory !<br>
     *          In case of a following call the string has to be copied before.
     *
     * @returns the string representation.
     */
    char* toString();


    /**
     * @brief   Set the boolean value of this object.
     *
     * @param   newValue the value to set.
     */
    void setValue(bool newValue);


    /**
     * @brief   Get the boolean value of this object.
     *
     * @returns the boolean value.
     */
    bool getValue();
};

#endif // SIDEXTYPEBOOL_H
