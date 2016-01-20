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

#include "sidex.h"
#include "tmlErrors.h"
#include "tmlUniversalHashTable.h"
#include <stdio.h>
#ifdef LINUX
#include <string.h>
#include <stdlib.h>
#endif //LINUX



/*********************************************************************************************************************************
*                                             "C" / Global methods / Callbacks & Threads
*********************************************************************************************************************************/

/*********************************************************************************************************************************
*                                             Class definitions:
*********************************************************************************************************************************/


/**
 * @brief    Constructor.
 */
TMLUniversalHashTable::TMLUniversalHashTable()
{
  m_hash = SIDEX_HANDLE_TYPE_NULL;
  m_bHashIsString = true;
}
 
TMLUniversalHashTable::TMLUniversalHashTable(tmlLogHandler* log){
  m_hash = SIDEX_HANDLE_TYPE_NULL;
  m_bHashIsString = true;
}

/**
 * @brief    Destructor.
 */
TMLUniversalHashTable::~TMLUniversalHashTable()
{
  if (SIDEX_HANDLE_TYPE_NULL != m_hash){
    int iSize = 0;
    SIDEX_INT32 iRet = hashSize(&iSize);
    if (SIDEX_SUCCESS == iRet && 0 < iSize){
      if (m_bHashIsString){
        char** sKeys;
        iRet = getKeys(&sKeys);
        for (SIDEX_INT32 i = 0; i < iSize && SIDEX_SUCCESS == iRet; ++i){
          iRet = removeEntry(sKeys[i]);
          delete sKeys[i];
        }
        delete[] sKeys;
      }
      else{
        SIDEX_INT64* iKeys;
        iRet = getKeys(&iKeys);
        for (SIDEX_INT32 i = 0; i < iSize && SIDEX_SUCCESS == iRet; ++i){
          iRet = removeEntry(iKeys[i]);
        }
        delete[] iKeys;
      }
    }

    sidex_Variant_DecRef(m_hash);
  }
  m_hash = SIDEX_HANDLE_TYPE_NULL;
}
 
 
/**
 * @brief   initialization of the hash.
 */
void TMLUniversalHashTable::createHashTable(bool bHashIsString){
  m_bHashIsString = bHashIsString;
  m_hash = sidex_Variant_New_Dict();
}

/**
 * @brief   Get the hash table entry for the key. Keys are interpreted as strings.
 */
int TMLUniversalHashTable::getValue(char* key, void** pValue)
{
  int iRet = TML_SUCCESS;
  SIDEX_VARIANT value = SIDEX_HANDLE_TYPE_NULL;

  if (m_bHashIsString){
    if (SIDEX_HANDLE_TYPE_NULL != m_hash){
      iRet = sidex_Variant_Dict_Get_A (m_hash, key, &value);
      switch (iRet)
      {
        case SIDEX_SUCCESS:
          SIDEX_INT64 iVal;
          iRet = sidex_Variant_As_Integer (value, &iVal);
          if (SIDEX_SUCCESS == iRet){
            *pValue = (void*)iVal;
          }
          break;
        case SIDEX_ERR_NOCONTENT:
          *pValue = NULL;
          iRet = SIDEX_SUCCESS;
          break;
        default:
          break;
      }
    }
    else{
      iRet = TML_ERR_HASH;
    }
  }
  else{
      iRet = TML_ERR_HASH_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Set a value into the hash table referring the key. Keys are interpreted as strings.
 */
int TMLUniversalHashTable::setValue(char* key, void* pValue)
{
  int iRet = TML_SUCCESS;

  if (m_bHashIsString){
    if (SIDEX_HANDLE_TYPE_NULL != m_hash){
      SIDEX_INT64 iVal = (SIDEX_INT64) pValue;
      SIDEX_VARIANT variant = sidex_Variant_New_Integer (iVal);
      iRet = sidex_Variant_Dict_Set_A (m_hash, key, variant);
      sidex_Variant_DecRef (variant);
    }
    else{
      iRet = TML_ERR_HASH;
    }
  }
  else{
      iRet = TML_ERR_HASH_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Remove the hash table entry for the key. Keys are interpreted as strings.
 */
 int TMLUniversalHashTable::removeEntry(char* key){
  int iRet = TML_SUCCESS;
  
  if (m_bHashIsString){
    if (SIDEX_HANDLE_TYPE_NULL != m_hash){
      //////////////////////////
      // Get the value:
      void* pValue = NULL;
      getValue(key, &pValue);
      // No return value here because a SIDEX_ERR_NOCONTENT shall not be announced:
      sidex_Variant_Dict_Delete_A (m_hash, key);
      //////////////////////////
      // Free the value:
      // Now I have to remove the stream object outside of the hashtable:
      /*
      if (NULL != pValue){
        delete pValue;
      }
      */
    }
    else{
      iRet = TML_ERR_HASH;
    }
  }
  else{
      iRet = TML_ERR_HASH_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Get the keys of the hashtable. Keys are interpreted as strings.
 */
int TMLUniversalHashTable::getKeys(char*** hashKeys)
{
  int iRet = TML_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != m_hash){
    if (m_bHashIsString){
      SIDEX_INT32 iSize;
      iRet = sidex_Variant_Dict_Size (m_hash, &iSize);
      if (SIDEX_SUCCESS == iRet){
        if (0 < iSize){
          SIDEX_VARIANT vKeys;
          iRet = sidex_Variant_Dict_Keys 	(m_hash, &vKeys);
          if (SIDEX_SUCCESS == iRet){
            char** cKeys = new char*[iSize];
            for (SIDEX_INT32 i = 0;i < iSize && SIDEX_SUCCESS == iRet;++i){
              SIDEX_VARIANT vKey;
              iRet = sidex_Variant_List_Get(vKeys, i, &vKey);
              if (SIDEX_SUCCESS == iRet){
                char* sVal;
                SIDEX_INT32 iLength;
                sidex_Variant_As_String_A (vKey, &sVal, &iLength);
                if (SIDEX_SUCCESS == iRet){
                  cKeys[i] = new char[iLength+1];
  #ifdef LINUX
                  strcpy (cKeys[i], sVal);
  #else // LINUX
  #if _MSC_VER > 1500
                  strcpy_s (cKeys[i], iLength+1, sVal);
  #else
                  strcpy (cKeys[i], sVal);
  #endif
  #endif // LINUX
                }
              }
            }
            sidex_Variant_DecRef(vKeys);
            if (TML_SUCCESS == iRet){
              *hashKeys = cKeys; 
            }
          }
        }
        else{
          *hashKeys = SIDEX_HANDLE_TYPE_NULL;
        }
      }
    }
    else{
      iRet = TML_ERR_HASH_WRONG_TYPE;
    }
  }
  else{
    iRet = TML_ERR_HASH;
  }
  return iRet;
}


/**
 * @brief   Get the hash table entry for the key. Keys are interpreted as integers.
 */
int TMLUniversalHashTable::getValue(TML_INT64 key, void** pValue)
{
  int iRet = TML_SUCCESS;
  SIDEX_VARIANT value = SIDEX_HANDLE_TYPE_NULL;

  if (!m_bHashIsString){
    if (SIDEX_HANDLE_TYPE_NULL != m_hash){
      char sKey[128];
#if defined (LINUX) || defined (MINGW_BUILD)
      sprintf (sKey, "%lld", (long long)key);
#else // LINUX
  #if _MSC_VER > 1500
      sprintf_s (sKey, 128, "%lld", key);
  #else
      sprintf (sKey, "%lld", key);
  #endif
#endif // LINUX
      iRet = sidex_Variant_Dict_Get_A (m_hash, sKey, &value);
      switch (iRet)
      {
        case SIDEX_SUCCESS:
          SIDEX_INT64 iVal;
          iRet = sidex_Variant_As_Integer (value, &iVal);
          if (SIDEX_SUCCESS == iRet){
            *pValue = (void*)iVal;
          }
          break;
        case SIDEX_ERR_NOCONTENT:
          *pValue = NULL;
          iRet = SIDEX_SUCCESS;
          break;
        default:
          break;
      }
    }
    else{
      iRet = TML_ERR_HASH;
    }
  }
  else{
      iRet = TML_ERR_HASH_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Set a value into the hash table referring the key. Keys are interpreted as integers.
 */
int TMLUniversalHashTable::setValue(TML_INT64 key, void* pValue)
{
  int iRet = TML_SUCCESS;

  if (!m_bHashIsString){
    if (SIDEX_HANDLE_TYPE_NULL != m_hash){
      SIDEX_INT64 iVal = (SIDEX_INT64) pValue;
      SIDEX_VARIANT variant = sidex_Variant_New_Integer (iVal);
      char sKey[128];
#if defined (LINUX) || defined (MINGW_BUILD)
      sprintf (sKey, "%lld", (long long)key);
#else // LINUX
  #if _MSC_VER > 1500
      sprintf_s (sKey, 128, "%lld", key);
  #else
      sprintf (sKey, "%lld", key);
  #endif
#endif // LINUX
      iRet = sidex_Variant_Dict_Set_A (m_hash, sKey, variant);
      sidex_Variant_DecRef (variant);
    }
    else{
      iRet = TML_ERR_HASH;
    }
  }
  else{
      iRet = TML_ERR_HASH_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Remove the hash table entry for the key. Keys are interpreted as integers.
 */
 int TMLUniversalHashTable::removeEntry(TML_INT64 key){
  int iRet = TML_SUCCESS;
  
  if (!m_bHashIsString){
    if (SIDEX_HANDLE_TYPE_NULL != m_hash){
      char sKey[128];
#if defined (LINUX) || defined (MINGW_BUILD)
      sprintf (sKey, "%lld", (long long)key);
#else // LINUX
  #if _MSC_VER > 1500
      sprintf_s (sKey, 128, "%lld", key);
  #else
      sprintf (sKey, "%lld", key);
  #endif
#endif // LINUX
      //////////////////////////
      // Get the value:
      void* pValue = NULL;
      getValue(key, &pValue);
      // No return value here because a SIDEX_ERR_NOCONTENT shall not be announced:
      sidex_Variant_Dict_Delete_A (m_hash, sKey);
      //////////////////////////
      // Free the value:
      // Now I have to remove the stream object outside of the hashtable:
      /*
      if (NULL != pValue){
        delete pValue;
      }
      */
    }
    else{
      iRet = TML_ERR_HASH;
    }
  }
  else{
      iRet = TML_ERR_HASH_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Get the keys of the hashtable. Keys are interpreted as integers.
 */
int TMLUniversalHashTable::getKeys(TML_INT64** hashKeys)
{
  int iRet = TML_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != m_hash){
    if (!m_bHashIsString){
      SIDEX_INT32 iSize;
      iRet = sidex_Variant_Dict_Size (m_hash, &iSize);
      if (SIDEX_SUCCESS == iRet){
        if (0 < iSize){
          SIDEX_VARIANT vKeys;
          iRet = sidex_Variant_Dict_Keys 	(m_hash, &vKeys);
          if (SIDEX_SUCCESS == iRet){
            TML_INT64* iKeys = new TML_INT64[iSize];
            for (SIDEX_INT32 i = 0;i < iSize && SIDEX_SUCCESS == iRet;++i){
              SIDEX_VARIANT vKey;
              iRet = sidex_Variant_List_Get(vKeys, i, &vKey);
              if (SIDEX_SUCCESS == iRet){
                char* sVal;
                SIDEX_INT32 iLength;
                sidex_Variant_As_String_A (vKey, &sVal, &iLength);
                if (SIDEX_SUCCESS == iRet){
#ifdef LINUX
                iKeys[i] = atoll(sVal);
#else // LINUX
  #if _MSC_VER > 1500
                iKeys[i] = _strtoi64(sVal, NULL, 10);
  #else
                iKeys[i] = _atoi64(sVal);
  #endif
#endif // LINUX
                }
              }
            }
            sidex_Variant_DecRef(vKeys);
            if (TML_SUCCESS == iRet){
              *hashKeys = iKeys; 
            }
          }
        }
        else{
          *hashKeys = SIDEX_HANDLE_TYPE_NULL;
        }
      }
    }
    else{
      iRet = TML_ERR_HASH_WRONG_TYPE;
    }
  }
  else{
    iRet = TML_ERR_HASH;
  }
  return iRet;
}



/**
 * @brief   Get the size of the hashtable.
 */
int TMLUniversalHashTable::hashSize(int* iSize)
{
  int iRet = TML_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != m_hash){
    SIDEX_INT32 iSize32;
    iRet = sidex_Variant_Dict_Size (m_hash, &iSize32);
    if (SIDEX_SUCCESS == iRet){
      *iSize = iSize32;
    }
  }
  else{
    iRet = TML_ERR_HASH;
  }

  return iRet;
}
