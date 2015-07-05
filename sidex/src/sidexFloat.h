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
 
#ifndef SIDEXTYPEFLOAT_H
#define SIDEXTYPEFLOAT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexTypeBase.h"

class sidexFloat : public sidexBase
{
   
private:
	  /* data */
    double value;
    char* m_strRepresentation;

protected: 
	  /* data */
    static const SIDEX_DATA_TYPE type = SIDEX_DATA_TYPE_FLOAT;

public:
	  /* methods */
    /**
     * @brief    Constructor.
     *
     * @returns an instance of sidexFloat.
     */
    sidexFloat();


    /**
     * @brief    Constructor with initial value.
     *
     * @param   newValue Initial content of the sidexFloat.
     *
     * @returns an instance of sidexFloat.
     */
    sidexFloat(double newValue);


    /**
     * @brief    Destructor.
     */
    ~sidexFloat();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief   Get type of this data object.
     *
     * @returns SIDEX_DATA_TYPE_FLOAT.
     *
     * @see sidexStdTypes.h
     */
    SIDEX_DATA_TYPE getType();


    /**
     * @brief   Get the string representation of this data object.
     *
     *          The return character array is owned by the sidexFloat.<br>
     *          Thus, the caller must not modify or free the memory !<br>
     *          In case of a following call the string has to be copied before.
     *
     * @returns the string representation.
     */
    char* toString();


    /**
     * @brief   Set the float value of this object.
     *
     * @param   newValue the value to set.
     */
    void setValue(double newValue);

    /**
     * @brief   Get the float value of this object.
     *
     * @returns the float value.
     */
    double getValue();
};

#endif // SIDEXTYPEFLOAT_H
