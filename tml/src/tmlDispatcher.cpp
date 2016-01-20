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

#include "tmlDispatcher.h"
#include "tmlErrors.h"
#include "tmlCore.h"
#include "tmlCoreDefines.h"

/**
 * @brief    Constructor.
 */
tmlDispatcher::tmlDispatcher(tmlLogHandler* loghandler){
  m_log = loghandler;
  m_hashTable = TML_HANDLE_TYPE_NULL;
  m_profile = TML_HANDLE_TYPE_NULL;
  m_pDeleteCmdCallback = TML_HANDLE_TYPE_NULL;
  m_pDeleteCmdData = TML_HANDLE_TYPE_NULL;
  m_pCustomDispatchCallback = TML_HANDLE_TYPE_NULL;
  m_pCustomDispatchData = TML_HANDLE_TYPE_NULL;
}


/**
 * @brief    Destructor.
 */
tmlDispatcher::~tmlDispatcher(){
  tml_DispatcherDismissAllAttributes();
}

/**
    * @brief   Get rid of all initialized attributes / set the objet to uninitialized state
 */
void tmlDispatcher::tml_DispatcherDismissAllAttributes(){
  if (NULL != m_hashTable){
    ///////////////////////////////////////////
    // Unregister possible registered keys:
    TML_INT64* iKeys;
    int iRet = m_hashTable->getKeys(&iKeys);
    if (TML_SUCCESS == iRet){
      if (NULL != iKeys){
        int iSize;
        iRet = m_hashTable->hashSize(&iSize);
        if (TML_SUCCESS == iRet){
          for (int i = 0; i < iSize; ++i)
            tml_Dispatcher_Register_Cmd((int)iKeys[i], NULL, NULL);
        }
        /////////////////////////////////
        // Now I have to free the memory:
        delete iKeys;
        iKeys = NULL;
      }
    }

    delete (m_hashTable);
    m_hashTable = NULL;
    if (NULL != m_profile){
      delete[] m_profile;
      m_profile = NULL;
    }
    m_pDeleteCmdCallback = TML_HANDLE_TYPE_NULL;
    m_pDeleteCmdData = TML_HANDLE_TYPE_NULL;
    m_pCustomDispatchCallback = TML_HANDLE_TYPE_NULL;
    m_pCustomDispatchData = TML_HANDLE_TYPE_NULL;
  }
}


/**
 * @brief   Returns the dispatchers profile.
 */
int tmlDispatcher::tml_Dispatcher_Get_Profile(char** profile){
  int iRet = TML_SUCCESS;
  if (NULL == m_hashTable){
    iRet = TML_ERR_DISPATCHER_NOT_CREATED;
  }
  else{
    *profile = m_profile;
  }
  return iRet;
}


/**
 * @brief   Create an instance of a command dispatcher.
 */
int tmlDispatcher::tml_Dispatcher_Create(const char* profile){
  int iRet = TML_SUCCESS;

  if (NULL == m_hashTable){
    m_hashTable = new TMLUniversalHashTable(m_log);
    m_hashTable->createHashTable(false); // integer hash

    size_t size = strlen(profile) + 1;
    m_profile = new char[size];
#if defined (LINUX) || defined (MINGW_BUILD)
    strcpy(m_profile, profile);
#else
    strcpy_s(m_profile, size, profile);
#endif
  }
  else{
    iRet = TML_ERR_DISPATCHER_ALREADY_EXISTS;
  }
  return iRet;
}


/**
 * @brief   Dismiss the instance of the dispatcher.
 */
int tmlDispatcher::tml_Dispatcher_Free(){
  int iRet = TML_SUCCESS;
  if (NULL == m_hashTable){
    iRet = TML_ERR_DISPATCHER_NOT_CREATED;
  }
  else{
    tml_DispatcherDismissAllAttributes();
  }
  return iRet;
}


/**
 * @brief   Register a callback method referring a command for the command dispatch process.
 */
int tmlDispatcher::tml_Dispatcher_Register_Cmd(TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet;

  if (NULL == m_hashTable){
    iRet = TML_ERR_DISPATCHER_NOT_CREATED;
  }
  else{
    if (NULL != pCBFunc){
      ///////////////////////////////////////
      // Put the values into the hash table:
      dispatcherCallbackData* data = new dispatcherCallbackData;
      data->pCBFunc = pCBFunc;
      data->pCBData = pCBData;
      iRet = m_hashTable->setValue(cmd, data);
    }
    else{
      dispatcherCallbackData* data = NULL;
      TML_INT64 hasKey = cmd;
      iRet = m_hashTable->getValue(hasKey, (void**)&data);

      if (TML_SUCCESS == iRet && NULL != data){
        //////////////////////////////////////////////////////////////////////////////////////////////
        // Possible delete callback:
        if (NULL != m_pDeleteCmdCallback && NULL != data->pCBFunc){
          ((void(FUNC_C_DECL *)(TML_COMMAND_ID_TYPE, TML_POINTER, TML_POINTER))m_pDeleteCmdCallback)(cmd, data->pCBData, m_pDeleteCmdData);
        }
        ////////////////////////////////////////
        // Delete the value stored in the hash:
        if (NULL != data){
          delete (data);
        }
        ////////////////////////////////////////
        m_hashTable->removeEntry(cmd);
      }
      else{
        iRet = TML_ERR_DISPATCHER_CMD_NOT_REGISTERED;
      }
    }
  }
  return iRet;
}


/**
 * @brief   Check if the cmd is registered
 */
bool tmlDispatcher::tml_Dispatcher_Is_Cmd_Registered(TML_COMMAND_ID_TYPE cmd){
  bool bRegistered = false;
  dispatcherCallbackData* data = NULL;
  int iRet = m_hashTable->getValue((TML_INT64)cmd, (void**)&data);

  if (TML_SUCCESS == iRet && NULL != data){
     bRegistered = true;
  }
  return  bRegistered;
}


/**
 * @brief   Returns the numer of registered commands
 */
int tmlDispatcher::tml_Dispatcher_Get_Registered_Size(int* iSize){

  int iRet = m_hashTable->hashSize(iSize);

  return iRet;
}


/**
 * @brief   Register a callback method that will be invoked in the case of command deregistration out of the command dispatch process.
 */
int tmlDispatcher::tml_Dispatcher_Set_OnDeleteCmd(TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet;
  if (NULL == m_hashTable){
    iRet = TML_ERR_DISPATCHER_NOT_CREATED;
  }
  else{
    m_pDeleteCmdCallback = pCBFunc;
    m_pDeleteCmdData = pCBData;
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief   Register a callback method for non registered dispatch commands.
 */
int tmlDispatcher::tml_Dispatcher_Set_OnCustomDispatch(TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet;

  if (NULL == m_hashTable){
    iRet = TML_ERR_DISPATCHER_NOT_CREATED;
  }
  else{
    m_pCustomDispatchCallback = pCBFunc;
    m_pCustomDispatchData = pCBData;
    iRet = TML_SUCCESS;
  }
  
  return iRet;
}


/**
 * @brief   This will initiate the command dispatch action for the tmlhandle containing the requested command.
 */
int tmlDispatcher::tml_Dispatcher_Dispatch_Cmd(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE iCmd){
  int iRet = TML_SUCCESS;

  if (NULL == m_hashTable){
    iRet = TML_ERR_DISPATCHER_NOT_CREATED;
  }
  else{
    ///////////////////////////////////////
    // Get the values out of the hash table:
    dispatcherCallbackData* data = NULL;
    iRet = m_hashTable->getValue((TML_INT64)iCmd, (void**)&data);

    if (TML_SUCCESS == iRet){
      if (NULL != data){
        ///////////////////////////////////////
        // there is an entry in the hash table:
        if (NULL != data->pCBFunc){
          // Uncomment to debug error 47
          //m_log->log (TML_LOG_SPECIAL, "tmlDispatcher", "tml_Dispatcher_Dispatch_Cmd", "iCmd", iCmd);
          if (NULL == data->pCBData){
            ///////////////////////////////////////
            // Callback without data:
            ((void(FUNC_C_DECL *)(TML_COMMAND_HANDLE))data->pCBFunc)(tmlhandle);
          }
          else{
            ///////////////////////////////////////
            // Callback with data:
            ((void(FUNC_C_DECL *)(TML_COMMAND_HANDLE, TML_POINTER))data->pCBFunc)(tmlhandle, data->pCBData);
          }
          // Uncomment to debug error 47
          //m_log->log (TML_LOG_SPECIAL, "tmlDispatcher", "done", "iCmd", iCmd);
        }
      }
      else{
        ////////////////////////////////////////////////////////////////////////////////
        // there is no entry in the hash table, use custom dispatch callback if valid:
        if (NULL != m_pCustomDispatchCallback){
          // Uncomment to debug error 47
          /*
          char* sTime = "";
          tml_Cmd_Header_GetCreationTime (tmlhandle, &sTime);
          m_log->log (TML_LOG_SPECIAL, "#############", "#############", "#############", "#############");
          m_log->log (TML_LOG_SPECIAL, "tmlDispatcher", "BEFORE - tml_Dispatcher_Dispatch_Cmd", "cmd", iCmd);
          m_log->log (TML_LOG_SPECIAL, "tmlDispatcher", "BEFORE - tml_Dispatcher_Dispatch_Cmd", "Time", sTime);
          */
          ((void(FUNC_C_DECL *)(TML_COMMAND_ID_TYPE, TML_COMMAND_HANDLE, TML_POINTER))m_pCustomDispatchCallback)(iCmd, tmlhandle, m_pCustomDispatchData);
          /*
          m_log->log (TML_LOG_SPECIAL, "tmlDispatcher", "AFTER  - tml_Dispatcher_Dispatch_Cmd", "cmd", iCmd);
          m_log->log (TML_LOG_SPECIAL, "tmlDispatcher", "AFTER  - tml_Dispatcher_Dispatch_Cmd", "Time", sTime);
          */
        }
      }
      }
  }
  return iRet;
}
