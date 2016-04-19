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

#include "tmlConnectionManageObjBase.h"
#include "tmlCoreWrapper.h"
#include "tmlCore.h"
#include "sidex.h"

/**
 * @brief    Constructor.
 *
 */
tmlConnectionManageObjBase::tmlConnectionManageObjBase()
{
  m_coreHandle = NULL;
  m_vortexConnection = NULL;
  m_bEncrypted = TML_FALSE;
}

/**
 * @brief    Destructor.
 */
tmlConnectionManageObjBase::~tmlConnectionManageObjBase()
{
}


/**
 * @brief Get the TML core handle.
 */
TML_CORE_HANDLE tmlConnectionManageObjBase::getCoreHandle(){
  return m_coreHandle;
}


/**
  * @brief   Get Vortex connection 
  */
VortexConnection* tmlConnectionManageObjBase::getVortexConnection(){
  return m_vortexConnection;
}


/**
  * @brief   Set Vortex connection 
  */
void tmlConnectionManageObjBase::setVortexConnection(VortexConnection* connection){
  m_vortexConnection = connection;
}


/**
  * @brief   Get the connection TLS encryption status
  */
TML_BOOL tmlConnectionManageObjBase::isEncrpyted(){
  return m_bEncrypted;
}


/**
  * @brief   Set the connection TLS encryption status
  */
void tmlConnectionManageObjBase::setEncrypted(TML_BOOL bEncrypted){
  m_bEncrypted = bEncrypted;
}


/**
  * @brief   Set the connection TLS status message
  */
void tmlConnectionManageObjBase::setTlsStatusMsg(char* statusMsg){
  if (SIDEX_HANDLE_TYPE_NULL != m_tlsStatusMsg){
    sidex_Variant_DecRef(m_tlsStatusMsg);
  }
  SIDEX_VARIANT msg;
  sidex_Variant_New_String(statusMsg, &msg);
  m_tlsStatusMsg = msg;
}


/**
  * @brief   Get the connection TLS status message
  */
SIDEX_INT32 tmlConnectionManageObjBase::getTlsStatusMsg_A(char** statusMsg){
  SIDEX_INT32 iLength;
  return sidex_Variant_As_String_A(m_tlsStatusMsg, statusMsg, &iLength);
}


/**
  * @brief   Get the connection TLS status message
  */
SIDEX_INT32 tmlConnectionManageObjBase::getTlsStatusMsg_X(wchar_t** statusMsg){
  SIDEX_INT32 iLength;
  return sidex_Variant_As_String_X(m_tlsStatusMsg, statusMsg, &iLength);
}


/**
  * @brief   Get the connection TLS status message
  */
SIDEX_INT32 tmlConnectionManageObjBase::getTlsStatusMsg_W(char16_t** statusMsg){
  SIDEX_INT32 iLength;
  return sidex_Variant_As_String_W(m_tlsStatusMsg, statusMsg, &iLength);
}
