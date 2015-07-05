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

#ifndef SIDEXHASHTABLE_H
#define SIDEXHASHTABLE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexStdTypes.h"

class sidexHashTable
{
protected:
    /* data */
    long m_tableSize;
    SIDEX_INT32 m_NumOfEntries;
    struct bucket
    {
      bool used;
      bool valid;
      bool FirstNextPending;
      bool single;
      char *key;
      wchar_t  *xkey;
      char16_t *wkey;
      SIDEX_VARIANT value;
      sidexHashTable *ht;
    };
    bucket *m_data;

    int m_walkerIndex;
   
    /* methods */
    /**
     * @brief    Generate a hash value depended of the table size and the given key.
     *
     * @param   size the hash table size.
     * @param   key  key for the hash value.<br>
     *               In case of an invalid key (NULL or length of 0)<br>
     *               the method throws the sixex error SIDEX_ERR_NOCONTENT.
     *
     * @returns the hash value.
     *
     * @see sidexErrors.h
     */
    static long createHash(long size, char *key);


    /**
     * @brief   Add all valid keys of the hash table to a string array
     *
     * @param   keyArray reference to the string array.<br>
     *                   Only keys for entrys with a value != NULL will be returned.<br>
     *                   Memory allocation has been done outside depending of a getNumberOfEntrys() request.
     * @param   iPos     reference to the index in the string array.<br>
     *                   It will be set to the new position after the operation
     *
     * @returns the hash value.
     */
    void addKeysToArray(char *** keyArray, int* iPos);
 

    /**
     * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
     *
     * @param   sKey      Reference to key.
     * @param   variant   Reference to the next Variant of the dictionary.
     *
     * @returns SIDEX_SUCCESS in case of success.
     *          SIDEX_ERR_FIRST_NEXT if there was no first() or<br>
     *          a setValue() / removeEntry() / happened since the first().<br>
     *          SIDEX_ERR_NOCONTENT if the end of the dictionary was reached.
     *
     *
     * @see globalDefines.h, sidexErrors.h, sidexStdTypes.h
     */
    bool getNextVariant_A(char** sKey, SIDEX_VARIANT* variant);
 

    /**
     * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
     *
     * @param   sKey      Reference to key.
     * @param   variant   Reference to the next Variant of the dictionary.
     *
     * @returns SIDEX_SUCCESS in case of success.
     *          SIDEX_ERR_FIRST_NEXT if there was no first() or<br>
     *          a setValue() / removeEntry() / happened since the first().<br>
     *          SIDEX_ERR_NOCONTENT if the end of the dictionary was reached.
     *
     *
     * @see globalDefines.h, sidexErrors.h, sidexStdTypes.h
     */
    bool getNextVariant_X(wchar_t** sKey, SIDEX_VARIANT* variant);


    /**
     * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
     *
     * @param   sKey      Reference to key.
     * @param   variant   Reference to the next Variant of the dictionary.
     *
     * @returns SIDEX_SUCCESS in case of success.
     *          SIDEX_ERR_FIRST_NEXT if there was no first() or<br>
     *          a setValue() / removeEntry() / happened since the first().<br>
     *          SIDEX_ERR_NOCONTENT if the end of the dictionary was reached.
     *
     *
     * @see globalDefines.h, sidexErrors.h, sidexStdTypes.h
     */
    bool getNextVariant_W(char16_t** sKey, SIDEX_VARIANT* variant);


    /**
     * @brief   Resets pointer to the beginning of the dictionary.
     */
    int firstInit();


public:
    /* methods */
    /**
     * @brief    Constructor. The initial table size is 100.
     *
     * @returns an instance of sidexHashTable.
     */
    sidexHashTable();


    /**
     * @brief    Constructor with defined initialized table size.
     *
     * @param   tableSize the initial table size.
     *
     * @returns an instance of sidexHashTable.
     */
    sidexHashTable(long tableSize);

    
    /**
     * @brief    Destructor.
     */
    virtual ~sidexHashTable();


    /**
     * @brief   Set an SIDEX_VARIANT value into the hash table referring the key.
     * @param   key   Referring key for the hash table entry.
     * @param   value The value to set.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_DICT_FULL Maximum of entrys reached.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int setValue(char *key, SIDEX_VARIANT value);


    /**
     * @brief   Set an SIDEX_VARIANT value into the hash table referring the key.
     * @param   key   Referring key for the hash table entry.
     * @param   value The value to set.
     * @param   numOfEntries reference to num of entries of caller.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_DICT_FULL Maximum of entrys reached.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int setValue(char *key, SIDEX_VARIANT value, SIDEX_INT32* numOfEntries);
 
    
    /**
     * @brief   Get the hash table entry for the key.
     * @param   key     Referring key for the hash table entry.
     * @param   variant Reference to the SIDEX_VARIANT value to get.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no hash table entry for the given key.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
     int getValue(char *key, SIDEX_VARIANT* variant);


    /**
     * @brief   Remove the hash table entry for the key.
     * @param   key Referring key for the hash entry.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT If there is no entry referring the key.
     *
     * @see sidexErrors.h
     */
    int removeEntry(char *key);


    /**
     * @brief   Remove the hash table entry for the key.
     * @param   key Referring key for the hash entry.
     * @param   numOfEntries reference to num of entries of caller.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT If there is no entry referring the key.
     *
     * @see sidexErrors.h
     */
    int removeEntry(char *key, SIDEX_INT32* numOfEntries);


    /**
     * @brief   Get number of hash table entrys.
     * @returns the amount of hash table entrys with a value != NULL.
     */
    SIDEX_INT32 getNumberOfEntrys();


    /**
     * @brief   Get the valid hash table keys.
     * @param   keyArray  reference to key array for the hash table.<br>
     *          Only keys for entrys with a value != NULL will be returned.<br>
     *          After using the key array you must have to deallocate memory using delete()<br>
     *          for the keyArray (but not for it's content).
     *    
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no hash table entry.
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
     *          a setValue() / removeEntry() / happened since the first().<br>
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
     *          a setValue() / removeEntry() / happened since the first().<br>
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
     *          a setValue() / removeEntry() / happened since the first().<br>
     *          SIDEX_ERR_NOCONTENT if the end of the dictionary was reached.
     *
     *
     * @see globalDefines.h, sidexErrors.h, sidexStdTypes.h
     */
    int next_W(char16_t** sKey, SIDEX_VARIANT* variant);
};

#endif //SIDEXHASHTABLE_H
