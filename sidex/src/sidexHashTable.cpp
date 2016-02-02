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
 
#include <string.h>
#include "unicode.h"
#include "sidexHashTable.h"
#include "sidexTypeBase.h"
#include "sidexUtils.h"
#include <sidexErrors.h>

using namespace std;
 
/**
 * @brief    Constructor. The initial table size is 100.
 */
sidexHashTable::sidexHashTable()
{
	m_tableSize = SIDEX_DEFAULT_AMOUNT_OF_DICT_ENTRYS;
  m_NumOfEntries = 0;
	m_data = new bucket[m_tableSize];
  for(int i=0; i<m_tableSize; i++)
  {
     m_data[i].used = false;
     m_data[i].single = true;
     m_data[i].key = NULL;
     m_data[i].xkey = NULL;
     m_data[i].wkey = NULL;
     m_data[i].value = SIDEX_HANDLE_TYPE_NULL;
     m_data[i].valid = false;
     m_data[i].ht = NULL;
  }
  m_walkerIndex = -1;
}
 

/**
 * @brief    Constructor with defined initialized table size.
 */
sidexHashTable::sidexHashTable(long tableSize)
{
	m_tableSize = tableSize;
  m_NumOfEntries = 0;
	m_data = new bucket[m_tableSize];
  for(int i=0; i<m_tableSize; i++)
  {
     m_data[i].used = false;
     m_data[i].single = true;
     m_data[i].key = NULL;
     m_data[i].xkey = NULL;
     m_data[i].wkey = NULL;
     m_data[i].value = SIDEX_HANDLE_TYPE_NULL;
     m_data[i].valid = false;
     m_data[i].ht = NULL;
  }
  m_walkerIndex = -1;
}
 

/**
 * @brief    Destructor.
 */
sidexHashTable::~sidexHashTable()
{

	for (int i = 0; i < m_tableSize; i++)
	{
    if (m_data[i].used){
      // decrement the objects reference counter:
      if (true == m_data[i].valid){
        sidexutil_Variant_DecRef(m_data[i].value);

        /////////////////////////////////////////////////////////////
        // Hashtable allocated internal memory for key / stringcompare:
        if (NULL != m_data[i].key){
          delete[] m_data[i].key;
          m_data[i].key = NULL;
        }
        if (NULL != m_data[i].xkey){
          delete[] m_data[i].xkey;
          m_data[i].xkey = NULL;
        }
        if (NULL != m_data[i].wkey){
          delete[] m_data[i].wkey;
          m_data[i].wkey = NULL;
        }
      }
      if (!m_data[i].single){
        // deallocate the next hashTable:
        delete[] m_data[i].ht;
        m_data[i].ht = NULL;

        /////////////////////////////////////////////////////////////
        // Hashtable allocated internal memory for key / stringcompare:
        if (NULL != m_data[i].key){
          delete[]  m_data[i].key;
          m_data[i].key = NULL;
        }
        if (NULL != m_data[i].xkey){
          delete[] m_data[i].xkey;
          m_data[i].xkey = NULL;
        }
        if (NULL != m_data[i].wkey){
          delete[] m_data[i].wkey;
          m_data[i].wkey = NULL;
        }
      }
    }
	}
  m_tableSize = 0;
  m_NumOfEntries = 0;

  if (NULL != m_data){
	  delete[] m_data;
    m_data = NULL;
  }
}
 

/**
 * @brief   Get the hash table entry for the key.
 */
int sidexHashTable::getValue(char *key, SIDEX_VARIANT* variant)
{
  int iRet = SIDEX_ERR_NOCONTENT;

  try{
    long nKey;

    // The hash- Value:
	  nKey = createHash(m_tableSize, key);
		if (m_data[nKey].used)
		{
      if (m_data[nKey].single){
        // there are NO following hash tables:
        if (strcmp(key, m_data[nKey].key) == 0)
        {
          // match to the key in this hash table:
          if (true == m_data[nKey].valid){
            *variant = m_data[nKey].value;
            iRet = SIDEX_SUCCESS;
          }
        }
			}
      else{
        // there ARE following hash tables:
        if (strcmp(key, m_data[nKey].key) == 0){
          if (true == m_data[nKey].valid){
            *variant = m_data[nKey].value;
            iRet = SIDEX_SUCCESS;
          }
        }
        else{
          // must habe a look in the following hash table:
				  return m_data[nKey].ht->getValue(key, variant);
        }
			}
		}
  }
  catch (int iException){
    // In case of an invalid key (NULL or length of 0)
    // createHash throws the sixex error SIDEX_ERR_NOCONTENT.
    iRet = iException;
  }
  return iRet;
}


/**
 * @brief   Get number of hash table entrys.
 */
SIDEX_INT32 sidexHashTable::getNumberOfEntrys(){
  return m_NumOfEntries;
}


/**
 * @brief   Get the valid hash table keys.
 */
int sidexHashTable::getKeys(char *** keyArray){
  int iRet;

  int iKeys = getNumberOfEntrys();
  if (iKeys > 0){
    char** arr;
    *keyArray = arr = new char*[iKeys];
    int iArrIdx = 0;
    addKeysToArray(&arr, &iArrIdx);
    iRet = SIDEX_SUCCESS;
  }
  else{
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief   Set an SIDEX_VARIANT value into the hash table referring the key.
 */
int sidexHashTable::setValue(char *key, SIDEX_VARIANT value)
{
  return setValue(key, value, &m_NumOfEntries);
}


/**
 * @brief   Set an SIDEX_VARIANT value into the hash table referring the key.
 */
int sidexHashTable::setValue(char *key, SIDEX_VARIANT value, SIDEX_INT32* numOfEntries)
{
  int iRet = SIDEX_SUCCESS;

  try{
    long nKey;

    // The hash- Value:
	  nKey = createHash(m_tableSize, key);
		if (m_data[nKey].used)
		{
      if (strcmp(key, m_data[nKey].key) == 0)
      {
        /////////////////////////////////////////////////////////
        // Overwrite existing Value / set new in case of removed:
        /////////////////////////////////////////////////////////

        // decrement of refCounter of old object in case of existence:
        if (true == m_data[nKey].valid){
          sidexutil_Variant_DecRef (m_data[nKey].value);
          --*numOfEntries;
        }
        // increment of refCounter of new object:
        ((sidexBase*)value)->incRef();
        m_data[nKey].value = value;
        m_data[nKey].valid = true;
        ++*numOfEntries;
      }
      else{
        //////////////////
        // Collision:
        //////////////////
			  if (m_data[nKey].single)
			  {
          // I have to create a following hash table:
				  m_data[nKey].ht = new sidexHashTable(m_tableSize + 1);
  		    m_data[nKey].single = false;
          // value into the following hash table:
				  iRet = m_data[nKey].ht->setValue(key, value, numOfEntries);
			  }
        else{
          // value into the following hash table:
				  iRet = m_data[nKey].ht->setValue(key, value, numOfEntries);
        }
      }
		}
		else
		{
      /////////////////////////////////////////////////////////////
      // Hashtable allocs internal memory for key / stringcompare:
      int iLength = (int) strlen(key)+1;
      char* sKey = new char[iLength];
#if defined(LINUX) || defined (MINGW_BUILD)
      strcpy(sKey, key);
#else
      strcpy_s(sKey, iLength, key);
#endif
      // It's a brand new entry:
			m_data[nKey].used = true;
			m_data[nKey].single = true;
      m_data[nKey].key = sKey;
      // increment of refCounter of new object:
      ((sidexBase*)value)->incRef();
			m_data[nKey].value = value;
      m_data[nKey].valid = true;
      ++*numOfEntries;
		}
	}
  catch (int iException){
    // In case of an invalid key (NULL or length of 0)
    // createHash throws the sixex error SIDEX_ERR_NOCONTENT.
    iRet = iException;
  }
  if (SIDEX_SUCCESS == iRet){
     m_walkerIndex = -1;
  }
  return iRet;
}


/**
 * @brief   Remove the hash table entry for the key.
 */
 int sidexHashTable::removeEntry(char *key){
   return removeEntry(key, &m_NumOfEntries);
 }


/**
 * @brief   Remove the hash table entry for the key.
 */
 int sidexHashTable::removeEntry(char *key, SIDEX_INT32* numOfEntries){
  int iRet = SIDEX_ERR_NOCONTENT;

  try{
    long nKey;

    // The hash- Value:
	  nKey = createHash(m_tableSize, key);
		if (m_data[nKey].used)
		{
      // the key is in use:
      if (m_data[nKey].single){
        // there are NO following hashTables:
        if (strcmp(key, m_data[nKey].key) == 0)
        {
          // match to the key in this hash table:
          if (true == m_data[nKey].valid)
          {
            // possible free of the SIDEX_VARIANT objext:
            sidexutil_Variant_DecRef (m_data[nKey].value);
            --*numOfEntries;
            m_data[nKey].value = SIDEX_HANDLE_TYPE_NULL;
            m_data[nKey].valid = false;
            // I can reset the flag and key because there are NO following hashTables:
            m_data[nKey].used = false;

            /////////////////////////////////////////////////////////////
            // Hashtable allocated internal memory for key / stringcompare:
            if (NULL != m_data[nKey].key){
              delete[] m_data[nKey].key;
              m_data[nKey].key = NULL;
            }
            if (NULL != m_data[nKey].xkey){
              delete[] m_data[nKey].xkey;
              m_data[nKey].xkey = NULL;
            }
            if (NULL != m_data[nKey].wkey){
              delete[] m_data[nKey].wkey;
              m_data[nKey].wkey = NULL;
            }
            iRet = SIDEX_SUCCESS;
          }
        }
      }
      else{
        // there ARE following hashTables:
        if (strcmp(key, m_data[nKey].key) == 0){
          // but match to the key in this hash table:
          if (true == m_data[nKey].valid)
          {
            // possible free of the SIDEX_VARIANT objext:
            sidexutil_Variant_DecRef (m_data[nKey].value);
            --*numOfEntries;
            m_data[nKey].value = SIDEX_HANDLE_TYPE_NULL;
            m_data[nKey].valid = false;
            iRet = SIDEX_SUCCESS;
            // but I can not reset the flag and key because there are NO following hashTables
          }
        }
        else{
          // must habe a look in the following hash table:
          iRet = m_data[nKey].ht->removeEntry(key, numOfEntries);
        }
      }
		}
	}
  catch (int iException){
    // In case of an invalid key (NULL or length of 0)
    // createHash throws the sixex error SIDEX_ERR_NOCONTENT.
    iRet = iException;
  }
  if (SIDEX_SUCCESS == iRet){
     m_walkerIndex = -1;
  }
  return iRet;
}


/**
 * @brief    Generate a hash value depended of the table size and the given key.
 */
long sidexHashTable::createHash(long size, char *key)
{
	long m = size;
 
  size_t keylen;
  
  // In case of an invalid key (NULL or length of 0)
  // the method throws the sixex error SIDEX_ERR_NOCONTENT.
  if (NULL == key || 0 == (keylen = strlen(key))) 
    throw SIDEX_ERR_NOCONTENT;
 
  // Use unsigend char* for calculations:
  unsigned char* calcKey = (unsigned char*)key;
	long h = (calcKey[1] % m);
	for (unsigned int i = 0; i < keylen; i++)
	{
		h = ((h << 7) + calcKey[i]) % m;
	}
	return h;
 }


/**
 * @brief   Add all valid keys of the hash table to a string array
 */
void sidexHashTable::addKeysToArray(char *** keyArray, int* iPos){

  for (int i = 0; i < m_tableSize; ++i){
    if (m_data[i].used){
      // Add key to array only in case of a valid value:
      if (true == m_data[i].valid){
        (*(keyArray))[(*iPos)] = m_data[i].key;
        *iPos = *iPos + 1;
      }
      if (!m_data[i].single){
        // in case of following hash tables ask them too:
        m_data[i].ht->addKeysToArray(keyArray, iPos);
      }
    }
  }
}


/**
 * @brief   Resets pointer to the beginning of the dictionary. Use with next(). 
 */
void sidexHashTable::first(){
  firstInit();
}


/**
 * @brief   Resets pointer to the beginning of the dictionary.
 */
int sidexHashTable::firstInit(){
  int iKeys = 0;
  m_walkerIndex = 0;

  for (int i = 0; i < m_tableSize; ++i){
    if (m_data[i].used){
      if (true == m_data[i].valid){
        // Add key to array only in case of a valid value:
        m_data[i].FirstNextPending = true;
        ++iKeys;
      }
      if (!m_data[i].single){
        // in case of following hash tables ask them too:
        iKeys += m_data[i].ht->firstInit();
      }
    }
  }
  return iKeys;
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexHashTable::next_A(char** sKey, SIDEX_VARIANT* variant)
{
  int iRet = SIDEX_SUCCESS;
  SIDEX_VARIANT value = SIDEX_HANDLE_TYPE_NULL;

  if (0 > m_walkerIndex){
    *variant = SIDEX_HANDLE_TYPE_NULL;
    iRet = SIDEX_ERR_FIRST_NEXT;
  }
  else{
    bool bFound = getNextVariant_A(sKey, &value);
    if (!bFound){
      *variant = SIDEX_HANDLE_TYPE_NULL;
      iRet = SIDEX_ERR_NOCONTENT;
    }
    else{
      *variant = value;
    }
  }
  return iRet;
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexHashTable::next_X(wchar_t** sKey, SIDEX_VARIANT* variant)
{
  int iRet = SIDEX_SUCCESS;
  SIDEX_VARIANT value = SIDEX_HANDLE_TYPE_NULL;

  if (0 > m_walkerIndex){
    *variant = SIDEX_HANDLE_TYPE_NULL;
    iRet = SIDEX_ERR_FIRST_NEXT;
  }
  else{
    bool bFound = getNextVariant_X(sKey, &value);
    if (!bFound){
      *variant = SIDEX_HANDLE_TYPE_NULL;
      iRet = SIDEX_ERR_NOCONTENT;
    }
    else{
      *variant = value;
    }
  }
  return iRet;
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexHashTable::next_W(char16_t** sKey, SIDEX_VARIANT* variant)
{
  int iRet = SIDEX_SUCCESS;
  SIDEX_VARIANT value = SIDEX_HANDLE_TYPE_NULL;

  if (0 > m_walkerIndex){
    *variant = SIDEX_HANDLE_TYPE_NULL;
    iRet = SIDEX_ERR_FIRST_NEXT;
  }
  else{
    bool bFound = getNextVariant_W(sKey, &value);
    if (!bFound){
      *variant = SIDEX_HANDLE_TYPE_NULL;
      iRet = SIDEX_ERR_NOCONTENT;
    }
    else{
      *variant = value;
    }
  }
  return iRet;
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
bool sidexHashTable::getNextVariant_A(char** sKey, SIDEX_VARIANT* variant){

  bool bFoundInThisTable = false;
  bool bFoundInThatTable = false;
  bool bAnyVariantFound = false;
  int i = m_walkerIndex;
  for (  ; i < m_tableSize && !bAnyVariantFound; ++i){
    if (m_data[i].used){
      ///////////////////////////////////////
      // Look into this table:
      if (true == m_data[i].valid){
        if (m_data[i].FirstNextPending){
          *variant = m_data[i].value;
          *sKey = m_data[i].key;
          bFoundInThisTable = true;
          m_data[i].FirstNextPending = false;
        }
      }
      if (!bFoundInThisTable && !m_data[i].single){
        // in case of following hash tables ask them too:
        bFoundInThatTable = m_data[i].ht->getNextVariant_A(sKey, variant);
      }
      bAnyVariantFound = bFoundInThisTable | bFoundInThatTable;
      if (bAnyVariantFound){
        m_walkerIndex = i;
      }
    }
  }
  if (!bAnyVariantFound){
    m_walkerIndex = m_tableSize;
  }
  return bAnyVariantFound;
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
bool sidexHashTable::getNextVariant_X(wchar_t** sKey, SIDEX_VARIANT* variant){

  bool bFoundInThisTable = false;
  bool bFoundInThatTable = false;
  bool bAnyVariantFound = false;
  int i = m_walkerIndex;
  for (  ; i < m_tableSize && !bAnyVariantFound; ++i){
    if (m_data[i].used){
      ///////////////////////////////////////
      // Look into this table:
      if (true == m_data[i].valid){
        if (m_data[i].FirstNextPending){
          *variant = m_data[i].value;
          if (NULL == m_data[i].xkey){
           SIDEX_INT32 iLengthUtf16;
            m_data[i].xkey = UTF8toUTF32(m_data[i].key, &iLengthUtf16);
          }
          *sKey = m_data[i].xkey;
          bFoundInThisTable = true;
          m_data[i].FirstNextPending = false;
        }
      }
      if (!bFoundInThisTable && !m_data[i].single){
        // in case of following hash tables ask them too:
        bFoundInThatTable = m_data[i].ht->getNextVariant_X(sKey, variant);
      }
      bAnyVariantFound = bFoundInThisTable | bFoundInThatTable;
      if (bAnyVariantFound){
        m_walkerIndex = i;
      }
    }
  }
  if (!bAnyVariantFound){
    m_walkerIndex = m_tableSize;
  }
  return bAnyVariantFound;


}/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
bool sidexHashTable::getNextVariant_W(char16_t** sKey, SIDEX_VARIANT* variant){

  bool bFoundInThisTable = false;
  bool bFoundInThatTable = false;
  bool bAnyVariantFound = false;
  int i = m_walkerIndex;
  for (  ; i < m_tableSize && !bAnyVariantFound; ++i){
    if (m_data[i].used){
      ///////////////////////////////////////
      // Look into this table:
      if (true == m_data[i].valid){
        if (m_data[i].FirstNextPending){
          *variant = m_data[i].value;
          if (NULL == m_data[i].wkey){
           SIDEX_INT32 iLengthUtf16;
            m_data[i].wkey = (char16_t*)UTF8toUTF16(m_data[i].key, &iLengthUtf16);
          }
          *sKey = m_data[i].wkey;
          bFoundInThisTable = true;
          m_data[i].FirstNextPending = false;
        }
      }
      if (!bFoundInThisTable && !m_data[i].single){
        // in case of following hash tables ask them too:
        bFoundInThatTable = m_data[i].ht->getNextVariant_W(sKey, variant);
      }
      bAnyVariantFound = bFoundInThisTable | bFoundInThatTable;
      if (bAnyVariantFound){
        m_walkerIndex = i;
      }
    }
  }
  if (!bAnyVariantFound){
    m_walkerIndex = m_tableSize;
  }
  return bAnyVariantFound;
}
