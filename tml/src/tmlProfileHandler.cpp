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

#include "tmlProfileHandler.h"
#include "tmlErrors.h"

/**
 * @brief    Constructor.
 */
tmlProfileHandler::tmlProfileHandler(tmlLogHandler* loghandler)
{
  m_profileHashTable = new TMLUniversalHashTable(loghandler);
  m_profileHashTable->createHashTable(true);
}


/**
 * @brief    Destructor.
 */
tmlProfileHandler::~tmlProfileHandler()
{
  ///////////////////////////////////////////////////
  // First delete the hash table content:
  int iSize;
  TML_INT32 iRet = m_profileHashTable->hashSize(&iSize);

  if (TML_SUCCESS == iRet && 0 < iSize){
    char** cKeys; // Temporary list of keys
    iRet = m_profileHashTable->getKeys(&cKeys);
    if (TML_SUCCESS == iRet){
      ProfileData* profileData;
      for (int i = 0; i < iSize && TML_SUCCESS == iRet;++i){
        iRet = m_profileHashTable->getValue(cKeys[i], (void**) &profileData);
        if (TML_SUCCESS == iRet){
          if (NULL != profileData){
            delete (profileData); // The hash table entry by itself
          }
          delete (cKeys[i]);      // The key in the temporary list at the requested index must be removed now
        }
      }
      delete[] cKeys;             // At last I must the temporary array holder 
    }
  }


  delete (m_profileHashTable);    // Now I can delete the hash table
  m_profileHashTable = NULL;
}


/**
 * @brief    Enter the critical section
 */
void tmlProfileHandler::tmlProfileRegister(const char* sProfile, bool blistener, void* lfCBFunc, void* lfCBData, bool* bRegisterProfile, bool* bRegisterCB){
  void* pdata = NULL;

  int iRet = m_profileHashTable->getValue((char*)sProfile, &pdata);
  if (TML_SUCCESS == iRet && NULL != pdata){
    ////////////////
    // Entry exists:
    if (blistener){
      ((ProfileData*) pdata)->bValidListener = true;
      ////////////////////////////////////////////////////////////////
      // I don't need to register the profile again:
      *bRegisterProfile = false;

      if (lfCBFunc != ((ProfileData*) pdata)->pCBFunc
                         ||
                         lfCBData != ((ProfileData*) pdata)->pCBData){
        ////////////////////////////////////////////////////////////////
        // I need to register the callback:
        ((ProfileData*) pdata)->pCBFunc = lfCBFunc;
        ((ProfileData*) pdata)->pCBData = lfCBData;

        *bRegisterCB = true;
      }
      else{
        ////////////////////////////////////////////////////////////////
        // I don't need to register the callback again:
        *bRegisterCB = false;
      }
    }
    else{
      ((ProfileData*) pdata)->bValidSender = true;
      /////////////////////////////////////////////////////////
      // In case of sender I don't need to do anything on here:
      *bRegisterProfile = false;
      *bRegisterCB = false;
    }
  }
  else{
    pdata = new ProfileData;
    ((ProfileData*) pdata)->bValidListener = blistener;
    ((ProfileData*) pdata)->bValidSender = !blistener;
    ((ProfileData*) pdata)->pCBFunc = lfCBFunc;
    ((ProfileData*) pdata)->pCBData = lfCBData;

    /*int iRet = */m_profileHashTable->setValue((char*)sProfile, pdata);

    *bRegisterProfile = true;
    *bRegisterCB = true;
  }
}


/**
 * @brief    Leave the critical section
 */
void tmlProfileHandler::tmlProfileUnregister(const char* sProfile){
  void* pdata = NULL;

  int iRet = m_profileHashTable->getValue((char*)sProfile, &pdata);
  if (TML_SUCCESS == iRet && NULL != pdata){
    m_profileHashTable->removeEntry((char*)sProfile);
    ////////////////////////////////////////
    // Delete the value stored in the hash:
    delete ((ProfileData*)pdata);
    ////////////////////////////////////////
  }
  else{
    printf ("Error on tmlProfileUnregister / profile not found !\n");
  }
}

