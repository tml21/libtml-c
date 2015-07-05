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

#ifndef SIDEXTYPELIST_H
#define SIDEXTYPELIST_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexTypeBase.h"

class sidexList : public sidexBase
{
protected: 
	  /* data */
    static const SIDEX_DATA_TYPE type = SIDEX_DATA_TYPE_LIST;

public:
	  /* data */
    struct listElement {
       listElement* next;
       listElement* prev;
       SIDEX_VARIANT value;
    };

	  /* methods */
    /**
     * @brief    Constructor.
     *
     * @returns an instance of sidexList.
     */
    sidexList();


    /**
     * @brief    Destructor.
     */
    ~sidexList();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief   Get type of this data object.
     *
     * @returns SIDEX_DATA_TYPE_LIST.
     *
     * @see sidexStdTypes.h
     */
    SIDEX_DATA_TYPE getType();


    /**
     * @brief   Get the first listElement object.
     *
     * @returns the first listElement object.
     */
    listElement* getFirst();

    /**
     * @brief   Get the last listElement object.
     *
     * @returns the last listElement object.
     */
    listElement* getLast();


    /**
     * @brief   Add an object of type SIDEX_VARIANT to the list.
     *
     * @param   val the value to add.
     *
     * @see sidexDefines.h
     */
    void addElement(SIDEX_VARIANT val);


    /**
     * @brief   Remove the object out of the list at the given position.
     *
     * @param   pos the position (base 0) in the list.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if pos holds no content.<br>
     *          SIDEX_ERR_WRONG_PARAMETER in case of a negative pos value.
     * @see sidexErrors.h
     */
    int DeleteItem(int pos);


    /**
     * @brief   Removes all objects out of a list.
     */
    void Clear();


    /**
     * @brief   Remove an SIDEX_VARIANT object out of the list.
     *
     * @param   refElement the SIDEX_VARIANT to be removed out of the list.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if refElement is not an item of the list.<br>
     *          SIDEX_ERR_WRONG_PARAMETER if refElement is NULL.
     * @see sidexErrors.h
     */
    int deleteVariant(SIDEX_VARIANT refElement);


    /**
     * @brief   Add an object of type SIDEX_VARIANT to the list before the given position.
     *
     *          If pos is more than the list size it will be added at the end of the list.
     *
     * @param   val the value to add.
     * @param   pos the position (base 0) in the list.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_WRONG_PARAMETER in case of a negative pos value.
     * @see sidexDefines.h
     */
    int addElement(SIDEX_VARIANT val, int pos);


    /**
     * @brief   Get the object of type SIDEX_VARIANT  at index pos (base 0).
     *
     * @param   val reference to a SIDEX_VARIANT with the return object / NULL if ist don't exist.
     * @param   pos the index (base 0) of the value in the list.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no content at the given pos.
     *
     * @see sidexDefines.h
     */
    int getElement(SIDEX_VARIANT *val, int pos);


    /**
     * @brief   Get the number of list entrys.
     *
     * @returns the number of list entrys.
     *
     */
    int length();

private:
	  /* data */
    listElement *first;
    listElement *last;

};  

#endif // SIDEXTYPELIST_H
