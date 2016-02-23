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
#include "tmlListenerObj.h"
#include "tmlCore.h"

/**
 * @brief    Constructor.
 *
 */
tmlListenerObj::tmlListenerObj(TML_CORE_HANDLE coreHandle, const char* sNetAddress)
{
  m_bListnerIsEnabled = TML_FALSE;

  m_coreHandle = coreHandle;

  // TODO: new listener
  m_binding = new tmlNetBinding(sNetAddress);

   m_iRefCounter = 1;
}


/**
 * @brief    Destructor.
 */
tmlListenerObj::~tmlListenerObj()
{
  cleanUp();
}

/**
 * @brief    Cleans up refCounter dependent allocations.
 */
void tmlListenerObj::cleanUp(){
  if (getRef()){
    if (decRef() == 0){
      delete m_binding;
    }
  }
}


/**
 * @brief Get the TML core handle.
 */
TML_CORE_HANDLE tmlListenerObj::getCoreHandle(){
  return m_coreHandle;
}


/**
 * @brief Get the network address for listener binding.
 */
TML_INT32 tmlListenerObj::getAddress(char** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
 * @brief Get the network address for listener binding.
  */
TML_INT32 tmlListenerObj::getAddress(wchar_t** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
 * @brief Get the network address for listener binding.
  */
TML_INT32 tmlListenerObj::getAddress(char16_t** sAddress){
  TML_INT32 iRet = m_binding->getAddress(sAddress);
  return iRet;
}


/**
  * @brief   Enable or disable the listener.
  */
TML_INT32 tmlListenerObj::set_Enabled(TML_BOOL bEnable){
  TML_INT32 iRet = TML_SUCCESS;
  if (bEnable == m_bListnerIsEnabled){
    iRet = TML_SUCCESS;
  }
  else{
    if (!m_bListnerIsEnabled){
      //////////////////////
      // Start the listener:
    }
    else{
      //////////////////////
      // Stop the listener:
    }
    // In case of TML_SUCCESS:
    if (TML_SUCCESS == iRet)
      m_bListnerIsEnabled = bEnable;
  }
  return iRet;
}


/**
  * @brief   Get listener status.
  */
TML_BOOL tmlListenerObj::get_Enabled(){
  return m_bListnerIsEnabled;
}


/**
 * @brief   Decrement the reference counter value of this data object for the memory management.
 *
 */
int tmlListenerObj::decRef(){
  return --m_iRefCounter;
}


/**
 * @brief   Increment the reference counter value of this data object for the memory management.
 */

int tmlListenerObj::incRef(){
  return ++m_iRefCounter;
}


/**
     * @brief   Get the reference counter value of this data object for the memory management.
 */
int tmlListenerObj::getRef(){
  return m_iRefCounter;
}
