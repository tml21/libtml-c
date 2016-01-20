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

#include "sidexDict.h"
#include "sidexTypeBase.h"
#include "sidex.h"


/**
 * @brief    Constructor. The initial dict size is 100.
 */
sidexDict::sidexDict()
{
  m_hashTable = new sidexHashTable();
  incRef();
}
 

/**
 * @brief    Constructor with defined initialized dict size.
 */
sidexDict::sidexDict(long tableSize)
{
  m_hashTable = new sidexHashTable(tableSize);
   incRef();
}
 

/**
 * @brief    Destructor.
 */
sidexDict::~sidexDict()
{
	cleanUp();
}


/**
 * @brief    Cleans up refCounter dependent allocations
 */
void sidexDict::cleanUp(){
 if (decRef() == 0){
    if (NULL != m_hashTable)
      delete (m_hashTable);
    m_hashTable = NULL;
  }
}
 

/**
 * @brief   Get type of this data object.
  */
SIDEX_DATA_TYPE sidexDict::getType()
{
   return type;
}


/**
 * @brief   Set an SIDEX_VARIANT value into the dict referring the key.
 */
int sidexDict::setValue(char *key, SIDEX_VARIANT value)
{
  return m_hashTable->setValue(key, value);
}


/**
 * @brief   Remove the dict entry for the key.
 */
int sidexDict::removeEntry(char *key){
  return m_hashTable->removeEntry(key);
}


/**
 * @brief   Removes all Items out of a dict.
 */
void sidexDict::clear(){

  int iSize = getNumberOfDictEntrys();
  if (0 < iSize){
    char ** keyArray = NULL;
    int iRet = getKeys(&keyArray);
    if (SIDEX_SUCCESS == iRet){
      for (int i = iSize-1; i >= 0; --i){
        removeEntry(keyArray[i]);
      }
    }
    if (NULL != keyArray){
      delete []keyArray;
    }
  }
}


/**
 * @brief   Get the dict entry for the key
 */
int sidexDict::getValue(char *key, SIDEX_VARIANT* variant)
{
  return m_hashTable->getValue(key, variant);
}


/**
* @brief   Get number of dict entrys
*/
int sidexDict::getNumberOfDictEntrys(){
  return m_hashTable->getNumberOfEntrys();
}


/**
 * @brief   Get the dict keys
 */
int sidexDict::getKeys(char *** keyArray){
  return m_hashTable->getKeys(keyArray);
}


/**
 * @brief   Resets pointer to the beginning of the dictionary. Use with next(). 
 */
void sidexDict::first(){
  m_hashTable->first();
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexDict::next_A(char** sKey, SIDEX_VARIANT* variant)
{
  return m_hashTable->next_A(sKey, variant);
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexDict::next_X(wchar_t** sKey, SIDEX_VARIANT* variant)
{
  return m_hashTable->next_X(sKey, variant);
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexDict::next_W(char16_t** sKey, SIDEX_VARIANT* variant)
{
  return m_hashTable->next_W(sKey, variant);
}
