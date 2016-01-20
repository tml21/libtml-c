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

#ifndef SIDEXTYPEDICT_H
#define SIDEXTYPEDICT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexTypeBase.h"
#include "sidexHashTable.h"

class sidexDict : public sidexBase
{

private:
   
protected: 
    /* data */
    static const SIDEX_DATA_TYPE type = SIDEX_DATA_TYPE_DICT;
    sidexHashTable*  m_hashTable;
   
public:
    /* data */
    /* methods */
    /**
     * @brief    Constructor. The initial dict size is 100.
     *
     * @returns an instance of sidexDict.
     */
    sidexDict();


    /**
     * @brief    Constructor with defined initialized dict size.
     *
     * @param   dictSize the initial dictSize.
     *
     * @returns an instance of sidexDict.
     */
    explicit  sidexDict(long dictSize);

    
    /**
     * @brief    Destructor.
     */
    virtual ~sidexDict();   


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief   Get type of this data object.
     *
     * @returns SIDEX_DATA_TYPE_DICT.
     *
     * @see sidexStdTypes.h
     */
    SIDEX_DATA_TYPE getType();


    /**
     * @brief   Set an SIDEX_VARIANT value into the dict referring the key.
     * @param   key   Referring key for the dict entry.
     * @param   value the value to set.
     *
     * @returns SIDEX_SUCCESS in case of success.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int setValue(char *key, SIDEX_VARIANT value);


    /**
     * @brief   Remove the dict entry for the key.
     * @param   key Referring key for the dict entry.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT If there is no entry referring the key.
     *
     * @see sidexErrors.h
     */
    int removeEntry(char *key);


    /**
     * @brief   Removes all Items out of a dict.
     */
    void clear();


    /**
     * @brief   Get the dict entry for the key.
     * @param   key     Referring key for the dict entry.
     * @param   variant Reference to the SIDEX_VARIANT value to get.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no dict entry for the given key.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int getValue(char *key, SIDEX_VARIANT* variant);


    /**
     * @brief   Get number of dict entrys.
     * @returns the amount of dict entrys with a value != SIDEX_HANDLE_TYPE_NULL.
     */
    int getNumberOfDictEntrys();


    /**
     * @brief   Get the dict keys.
     * @param   keyArray  reference to key array for the dict.<br>
     *          Only keys for entrys with a value != NULL will be returned.<br>
     *          After using the key array you must have to deallocate memory using delete()<br>
     *          for the keyArray (but not for it's content).
     *    
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no dict entry.
     *
     * @see sidexErrors.h
     */
    int getKeys(char *** keyArray);

    
    /**
     * @brief   Resets pointer to the beginning of the dictionary. Use with next(). 
     */
    void first();


    /**
     * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
     *
     * Use with first(). 
     *
     * @param   sKey      Reference to key.
     * @param   variant   Reference to the next Variant of the dictionary.
     *
     * @returns SIDEX_SUCCESS in case of success.
     *          SIDEX_ERR_FIRST_NEXT if there was no first() or<br>
     *          a setValue() / removeEntry() / cleanUp() happened since the first().<br>
     *          SIDEX_ERR_NOCONTENT if the end of the dictionary was reached.
     *
     *
     * @see globalDefines.h, sidexErrors.h, sidexStdTypes.h
     */
    int next_A(char** sKey, SIDEX_VARIANT* variant);


    /**
     * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
     *
     * Use with first(). 
     *
     * @param   sKey      Reference to key.
     * @param   variant   Reference to the next Variant of the dictionary.
     *
     * @returns SIDEX_SUCCESS in case of success.
     *          SIDEX_ERR_FIRST_NEXT if there was no first() or<br>
     *          a setValue() / removeEntry() / cleanUp() happened since the first().<br>
     *          SIDEX_ERR_NOCONTENT if the end of the dictionary was reached.
     *
     *
     * @see globalDefines.h, sidexErrors.h, sidexStdTypes.h
     */
    int next_X(wchar_t** sKey, SIDEX_VARIANT* variant);


    /**
     * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
     *
     * Use with first(). 
     *
     * @param   sKey      Reference to key.
     * @param   variant   Reference to the next Variant of the dictionary.
     *
     * @returns SIDEX_SUCCESS in case of success.
     *          SIDEX_ERR_FIRST_NEXT if there was no first() or<br>
     *          a setValue() / removeEntry() / cleanUp() happened since the first().<br>
     *          SIDEX_ERR_NOCONTENT if the end of the dictionary was reached.
     *
     *
     * @see globalDefines.h, sidexErrors.h, sidexStdTypes.h
     */
    int next_W(char16_t** sKey, SIDEX_VARIANT* variant);

};


#endif // SIDEXTYPEDICT_H
