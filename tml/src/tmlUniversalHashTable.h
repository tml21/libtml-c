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
 *  Homepage:
 *    http://www.libtml.org
 *
 *  For professional support contact us:
 *
 *    wobe-systems GmbH
 *    support@libtml.org
 *
 * Contributors:
 *    wobe-systems GmbH
 */

#ifndef TMLUNIVERSALHASHTABL_H
#define TMLUNIVERSALHASHTABL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlLogHandler.h"
#include "tmlStdTypes.h"



//////////////////////////////////////////////////////////////////////////////
// C - declarations:
extern "C" {

}// extern "C"


class TMLUniversalHashTable
{
protected:
    /* data */

    /**
     * @brief    Vortex hash
     */
    SIDEX_VARIANT m_hash;
    /**
     * @brief    the kind of hashtable
     */
    bool        m_bHashIsString;

    /* methods */

public:
    /* methods */

    /**
     * @brief    Constructor.
     * @returns an instance of TMLUniversalHashTable.
     */
    TMLUniversalHashTable();
    

    /**
     * @brief    Constructor.
     * @returns an instance of TMLUniversalHashTable.
     */
    explicit TMLUniversalHashTable(tmlLogHandler* log);

    /**
     * @brief    Destructor.
     */
    ~TMLUniversalHashTable();


    /**
     * @brief   initialization of the hash.
     * @param   bHashIsString In case of true the keys are interpreted as strings otherwise as integers.
     */
    void createHashTable(bool bHashIsString);

    /**
     * @brief   Set a value into the hash table referring the key. Keys are interpreted as strings.
     * @param   key      referring key (string) for the hash table entry.
     * @param   pValue   reference to the data.<br>
     *                   If the key is already hashed the old value will be freed and the entry will be overwritten.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.<br>
     *          TML_ERR_HASH_WRONG_TYPE if the key type is wrong.
     *
     * @see tmlErrors
     */
     int setValue(char* key, void* pValue);
 
    
    /**
     * @brief   Get the hash table entry for the key. Keys are interpreted as strings.
     * @param   key      referring key (string) for the hash table entry.
     * @param   pValue   reference to store the reference to the data.<br>
     *                   If the key is invalid NULL will be returned.
     *              
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.<br>
     *          TML_ERR_HASH_WRONG_TYPE if the key type is wrong.
     *
     * @see tmlErrors
     */
     int getValue(char* key, void** pValue);


    /**
     * @brief   Remove the hash table entry for the key. Keys are interpreted as strings.
     * @param   key      referring key (string) for the hash table entry.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.<br>
     *          TML_ERR_HASH_WRONG_TYPE if the key type is wrong.
     *
     * @see tmlErrors
     */
    int removeEntry(char* key);


    /**
     * @brief   Get the keys of the hashtable. Keys are interpreted as strings.
     * @param   hashKeys  reference to the keys (strings). Must be deallocated using delete().
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.<br>
     *          TML_ERR_HASH_WRONG_TYPE if the key type is wrong.
     *
     * @see tmlErrors
     */
    int getKeys(char*** hashKeys);


    /**
     * @brief   Set a value into the hash table referring the key. Keys are interpreted as integers.
     * @param   key      referring key (integer) for the hash table entry.
     * @param   pValue   reference to the data.<br>
     *                   If the key is already hashed the old value will be freed and the entry will be overwritten.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.<br>
     *          TML_ERR_HASH_WRONG_TYPE if the key type is wrong.
     *
     * @see tmlErrors, tmlStdTypes.h
     */
     int setValue(TML_INT64 key, void* pValue);
 
    
    /**
     * @brief   Get the hash table entry for the key. Keys are interpreted as integers.
     * @param   key      referring key (integer) for the hash table entry.
     * @param   pValue   reference to store the reference to the data.<br>
     *                   If the key is invalid NULL will be returned.
     *              
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.<br>
     *          TML_ERR_HASH_WRONG_TYPE if the key type is wrong.
     *
     * @see tmlErrors, tmlStdTypes.h
     */
     int getValue(TML_INT64 key, void** pValue);


    /**
     * @brief   Remove the hash table entry for the key. Keys are interpreted as integers.
     * @param   key      referring key (integer) for the hash table entry.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.<br>
     *          TML_ERR_HASH_WRONG_TYPE if the key type is wrong.
     *
     * @see tmlErrors, tmlStdTypes.h
     */
    int removeEntry(TML_INT64 key);


    /**
     * @brief   Get the keys of the hashtable. Keys are interpreted as integers.
     * @param   hashKeys  reference to the keys (integer). Must be deallocated using delete().
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.<br>
     *          TML_ERR_HASH_WRONG_TYPE if the key type is wrong.
     *
     * @see tmlErrors, tmlStdTypes.h
     */
    int getKeys(TML_INT64** hashKeys);

    /**
     * @brief   Get the size of the hashtable.
     * @param   iSize  reference to the sizevalue.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.
     *
     * @see tmlErrors
     */
    int hashSize(int* iSize);
};
#endif // TMLUNIVERSALHASHTABL_H
