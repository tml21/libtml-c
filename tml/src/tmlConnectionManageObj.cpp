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

#include <stdio.h>
#include <string.h>
#include "tmlConnectionManageObj.h"
#include "tmlCore.h"
#include "unicode.h"

/**
 * @brief    Constructor.
 *
 */
tmlConnectionManageObj::tmlConnectionManageObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress)
{
  int iSize;
  TML_INT32 iLength;

  iSize = (int)strlen(sNetAddress)+1;
  m_sNetAddress = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
  strcpy (m_sProfile, profile);
#else
  strcpy_s (m_sNetAddress, iSize, sNetAddress);
#endif
  m_sNetAddress_w = (char16_t*)UTF8toUTF16(m_sNetAddress, &iLength);
  m_sNetAddress_x = UTF8toUTF32(m_sNetAddress, &iLength);

  m_coreHandle = coreHandle;

  // TODO:
  // vortex_conection_new();

   m_iRefCounter = 1;
}


/**
 * @brief    Destructor.
 */
tmlConnectionManageObj::~tmlConnectionManageObj()
{
  cleanUp();
}

/**
 * @brief    Cleans up refCounter dependent allocations.
 */
void tmlConnectionManageObj::cleanUp(){
  if (getRef())
    if (decRef() == 0){
        // TODO:
        // vortex_conection_close();

      if (TML_HANDLE_TYPE_NULL != m_sNetAddress){
        delete[] m_sNetAddress;
        delete[] m_sNetAddress_w;
        delete[] m_sNetAddress_x;
        m_sNetAddress = TML_HANDLE_TYPE_NULL;
      }
    }
}


/**
 * @brief Get the TML core handle.
 */
TML_CORE_HANDLE tmlConnectionManageObj::getCoreHandle(){
  return m_coreHandle;
}


/**
 * @brief Get the network address for connection binding.
 */
TML_INT32 tmlConnectionManageObj::getAddress_A(char** sAddress){
  *sAddress = m_sNetAddress;

  return TML_SUCCESS;
}


/**
  * @brief Get the network address for connection binding.
  *
  * @returns the network address.
  */
TML_INT32 tmlConnectionManageObj::getAddress_X(wchar_t** sAddress){
  *sAddress = m_sNetAddress_x;

  return TML_SUCCESS;
}


/**
  * @brief Get the network address for connection binding.
  *
  * @returns the network address.
  */
TML_INT32 tmlConnectionManageObj::getAddress_W(char16_t** sAddress){
  *sAddress = m_sNetAddress_w;

  return TML_SUCCESS;
}

/**
  * @brief Returns the remote peer supported profiles.
  *
  * @param   lProfiles   reference to profile list.<br>
  *                      The list has to be released with sidex_Variant_DecRef().
  *
  * @returns TML_SUCCESS
  */
TML_INT32 tmlConnectionManageObj::getRemoteProfiles(SIDEX_VARIANT* lProfiles){

  // TODO:
  // vortex_conection_get_remote_progiles();
  return TML_SUCCESS;
}


/**
  * @brief    Validate a connection.
  */
TML_INT32 tmlConnectionManageObj::validate(TML_BOOL bReconnect, TML_BOOL* bConnected){
  // TODO:
  // vortex_conection_is_ok();
  // TODO:
  // vortex_conection_new();
  return TML_SUCCESS;
}

/**
 * @brief   Decrement the reference counter value of this data object for the memory management.
 *
 */
int tmlConnectionManageObj::decRef(){
  return --m_iRefCounter;
}


/**
 * @brief   Increment the reference counter value of this data object for the memory management.
 */

int tmlConnectionManageObj::incRef(){
  return ++m_iRefCounter;
}


/**
     * @brief   Get the reference counter value of this data object for the memory management.
 */
int tmlConnectionManageObj::getRef(){
  return m_iRefCounter;
}
