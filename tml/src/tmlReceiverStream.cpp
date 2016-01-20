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

#include "tmlObjWrapper.h"
#include <sidex.h>
#include "tmlCmdDefines.h"
#include "tmlReceiverStream.h"
#include "tmlErrors.h"
#include "tmlCore.h"
#include "tmlCoreWrapper.h"
#include "logValues.h"

/**
 * @brief    Maximal buffer size of one portion for a write operation:
 */
#define MAX_STREAM_WRITE_BUFFER 0x10000

/*********************************************************************************************************************************
*                                             "C" / Global methods / Callbacks & Threads
*********************************************************************************************************************************/

/**
 * @brief    Call internal for the last error.
 */
int getLastErrorInternCB(comParamStruct* cp)
{
  tmlObjWrapper* command = new tmlObjWrapper();
  int iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_GET_LAST_ERR);
  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
  iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
  if (TML_SUCCESS == iRet)
    iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, cp->iStreamID);

  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  command->tmlObjWrapper_Release_Sidex_Handle();

  if (TML_SUCCESS == iRet){
    ////////////////////////////////////////////
    // Maybe someone has called the exit /close:
    iRet = ((tmlCoreWrapper*)cp->coreHandle)->tmlCoreWrapper_IsAccessible();
    if (TML_SUCCESS == iRet){
      iRet = cp->sender->sender_SendSyncMessage(cp->sProfile, cp->sHost, cp->sPort, cp->iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
      if (TML_SUCCESS == iRet){
        command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
        TML_INT64 errVal;
        iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_LAST_ERR, &errVal);
        ////////////// NO LAST ERROR //////////////
        //*(cp->iLastErr) = (TML_INT32) errVal;
        ///////////////////////////////////////////////
        // Now I can release the critical section use: 
        command->tmlObjWrapper_Release_Sidex_Handle();
      }
      else
      {
        ////////////// NO LAST ERROR //////////////
        /////////////////////////////////////////
        // Could not get the error from the other
        //*(cp->iLastErr) = iRet;
      }
    }
  }

  delete (command);

  return iRet;
}


/**
 * @brief    Cancel a download stream operation.
 */
int cancelDownload(comParamStruct* cp){
  tmlObjWrapper* command = new tmlObjWrapper();
  int iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_CANCEL_DOWNLOAD);
  ///////////////////////////////////////////////
  // Acquire critical section use: 
  SIDEX_HANDLE sHandle;
  command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
  iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
  if (TML_SUCCESS == iRet)
    iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, cp->iStreamID);

  ///////////////////////////////////////////////
  // Now I can release the critical section use: 
  command->tmlObjWrapper_Release_Sidex_Handle();

  if (TML_SUCCESS == iRet){
    cp->log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "cancelDownload", "SendSync CMD to source CMD_INTERNAL_STREAM_CANCEL_DOWNLOAD", cp->iStreamID);
    iRet = cp->sender->sender_SendSyncMessage(cp->sProfile, cp->sHost, cp->sPort, cp->iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
  }

  delete (command);

  return iRet;
}


/**
 * @brief  callback method for the end of a download
 */
void FUNC_C_DECL downloadReadyReturnMethod(TML_COMMAND_HANDLE tmlHandle, TML_POINTER data){
  comParamStruct* cp = (comParamStruct*) data;
  tmlObjWrapper* command = (tmlObjWrapper*) tmlHandle;

  ////////////////////////////////////////////
  // Maybe someone has called the exit /close:
  TML_INT32 iRet = ((tmlCoreWrapper*)cp->coreHandle)->tmlCoreWrapper_IsAccessible();
  if (TML_SUCCESS == iRet){
    cp->log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "downloadReadyReturnMethod", "CMD_INTERNAL_STREAM_DOWNLOAD finished", cp->iStreamID);
    ////////////// NO LAST ERROR //////////////
    //getLastErrorInternCB(cp);

    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    tml_Cmd_Header_GetError(tmlHandle, &iRet);
    if (TML_SUCCESS == iRet){
      ///////////////////////////////////////////////
      // Acquire critical section use: 
      SIDEX_HANDLE sHandle;
      command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
      TML_INT64 bytes = 0;
      TML_INT64 size = -1;
      ///////////////////////////////////////////////
      // Read the number of total bytes read:
      TML_INT32 iInternalRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_BYTRES_READ, &bytes);
      if (TML_SUCCESS != iInternalRet){
        //////////////////////////////////////////////////
        // Error will be indicated by a bytes length of 0:
        bytes = 0;
      }
      else{
        ///////////////////////////////////////////////////////////////
        // Read the size of the stream that should have been read read:
        iInternalRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_CONTENT_SIZE, &size);
      }
      ///////////////////////////////////////////////////////////////
      // Values must be identical on success:
      if (bytes != size)
      {
        iRet = TML_ERR_STREAM_DOWNLOAD_FAILED;
      }
      ///////////////////////////////////////////////
      // Now I can release the critical section use: 
      command->tmlObjWrapper_Release_Sidex_Handle();
      ////////////// NO LAST ERROR //////////////
      ///////////////////////////////////////////////////
      // If m_iLastErr not already set I can set it now:
      //if (TML_SUCCESS == *(cp->iLastErr)){
        //*(cp->iLastErr) = iRet;
      //}
    } 
    else{
      ///////////////////////////////////////////////////////////////////////////////////////
      // In case of an error it is necessary to cancel the download on the other one's side:
      if (TML_ERR_STREAM_DOWNLOAD_CANCELED == iRet  ||
                                        TML_ERR_STREAM_DOWNLOAD_WRITE_FAILED == iRet){
        ///////////////////////////////////////////////////////////////////////////////
        // Error or cancel happened during download - tell the other one to stop:
        if (TML_ERR_STREAM_DOWNLOAD_CANCELED == iRet){
          cp->log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "downloadReadyReturnMethod", "TML_ERR_STREAM_DOWNLOAD_CANCELED", "I have to call cancelDownload");
        }
        else{
          cp->log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "downloadReadyReturnMethod", "TML_ERR_STREAM_DOWNLOAD_WRITE_FAILED", "I have to call cancelDownload");
        }
        cancelDownload(cp);
        ////////////// NO LAST ERROR //////////////
        //*(cp->iLastErr) = iRet;
      }
    }
  }
  else{
    cp->log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "downloadReadyReturnMethod", "is not accessable anymore", "");
  }
  ////////////// NO LAST ERROR //////////////
  //////////////////////////////////////
  // Map return value to the last error:
//  if (TML_SUCCESS != iRet && TML_SUCCESS != *(cp->iLastErr)){
//    iRet = *(cp->iLastErr);
//  }
  ///////////////////////////////////////////////
  //  Now call the callback method for a clean up
   cp->log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "downloadReadyReturnMethod", "I have to call callbackmethod for Close", cp->iStreamID);
  ((void(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_INT32, TML_POINTER))cp->pCBFuncRet)(cp->iStreamID, iRet, cp->pCBDataRet);
   cp->log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "downloadReadyReturnMethod", "return from callbackmethod", cp->iStreamID);
  ///////////////////////////////////////////////
  //  Now call the callback method for a clean up
  delete command;
  ///////////////////////////////////////////////
  //  Delete the callback structure
  delete cp;
}


/**
 * @brief    Constructor.
 */
tmlReceiverStream::tmlReceiverStream(TML_CORE_HANDLE coreHandle, tmlCollectCall* sender, int iWindowSize, tmlLogHandler* loghandler)
{
  // Connection parameter:
  m_coreHandle = coreHandle;
  m_sProfile = NULL;
  m_sHost = NULL;
  m_sPort = NULL;

  m_sKey = NULL;

  m_iStreamID = 0;

  m_sender = sender;
  m_iWindowSize = iWindowSize;
  m_log = loghandler;
  m_iLastErr = TML_SUCCESS;
  m_bOpenState = CLOSED;
  m_cp = NULL;
}


/**
 * @brief    Destructor.
 */
tmlReceiverStream::~tmlReceiverStream()
{
  reset(false);
}


/**
 * @brief    Returns the registered profile for this stram
 */
char* tmlReceiverStream::getProfile(){
  return m_sProfile;
}


/**
 * @brief    Returns the registered host for this stram
 */
char* tmlReceiverStream::getHost(){
  return m_sHost;
}


/**
 * @brief    Returns the registered port for this stram
 */
char* tmlReceiverStream::getPort(){
  return m_sPort;
}


/**
 * @brief    Open a receiver stream.
 */
int tmlReceiverStream::open(TML_STREAM_ID_TYPE iStreamID, const char* sPort, const char* sHost, const char* profile){
  int iRet = TML_ERR_STREAM_INVALID_ADDRESS;

  switch (m_bOpenState){
    case CLOSED:
        if (NULL != profile && 0 < strlen(profile) &&
            NULL != sHost && 0 < strlen(sHost) &&
            NULL != sPort && 0 < strlen(sPort)){
          int iSize;
          /////////////////////////////
          // Profile:
          if (NULL != m_sProfile)
            delete[] m_sProfile;
          iSize = (int)strlen(profile)+1;
          m_sProfile = new char[iSize];
      #if defined (LINUX) || defined (MINGW_BUILD)
          strcpy (m_sProfile, profile);
      #else
          strcpy_s (m_sProfile, iSize, profile);
      #endif
          /////////////////////////////
          // Host:
          if (NULL != m_sHost)
            delete[] m_sHost;
          iSize = (int)strlen(sHost)+1;
          m_sHost = new char[iSize];
      #if defined (LINUX) || defined (MINGW_BUILD)
          strcpy (m_sHost, sHost);
      #else
          strcpy_s (m_sHost, iSize, sHost);
      #endif
          /////////////////////////////
          // Port:
          if (NULL != m_sPort)
            delete[] m_sPort;
          iSize = (int)strlen(sPort)+1;
          m_sPort = new char[iSize];
      #if defined (LINUX) || defined (MINGW_BUILD)
          strcpy (m_sPort, sPort);
      #else
          strcpy_s (m_sPort, iSize, sPort);
      #endif


          m_iStreamID = iStreamID;

          /////////////////////////////
          // Key:
          if (NULL != m_sKey)
            delete[] m_sKey;
          iSize = (int)strlen(profile) + (int)strlen(sHost) + (int)strlen (sPort) + 32 + 3 + 1;
          m_sKey = new char[iSize];
      #if defined (LINUX) || defined (MINGW_BUILD)
          sprintf(m_sKey, "%s-%s-%s-%lld", profile, sHost, sPort, (long long)m_iStreamID);
      #else // LINUX
          sprintf_s(m_sKey, iSize, "%s-%s-%s-%d", profile, sHost, sPort, m_iStreamID);
      #endif // LINUX

          m_bOpenState = OPENED;

          iRet = TML_SUCCESS;
        }
        break;
    case RETAIN_OPENED:
        if (0 == strcmp(m_sProfile, profile)&& 0 == strcmp(m_sHost, sHost) && 0 == strcmp(m_sPort, sPort))
        {
          m_bOpenState = OPENED;
          iRet = TML_SUCCESS;
        }
        else{
          iRet = TML_ERR_STREAM_OPEN_ADDRESS_MISMATCH;
        }
        break;
    case OPENED:
    default:
        iRet = TML_ERR_STREAM_ALREADY_IN_USE;
        break;
  }
  return iRet;
}


/**
 * @brief    Close a receiver stream.
 */
int tmlReceiverStream::close(TML_BOOL bRetainOpen, bool bSendCancel){
  int iRet = TML_SUCCESS;

  if (TML_FALSE == bRetainOpen){
    reset(bSendCancel);
  }
  else{
    if (OPENED == m_bOpenState){
      m_bOpenState = RETAIN_OPENED;
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }
  return iRet;
}


/**
 * @brief    Helper method to get the key for hash computing.
 */
void tmlReceiverStream::getKey(char** sKey){
  *sKey = m_sKey;
}


/**
 * @brief    Helper method to get the streamID for hash computing.
 */
void tmlReceiverStream::getID(TML_STREAM_ID_TYPE* iID){
  *iID = m_iStreamID;
}


/**
 * @brief    Helper method resets all parameter.
 */
void tmlReceiverStream::reset(bool bSendCancel){

  if (CLOSED != m_bOpenState && bSendCancel){
    m_bOpenState = CLOSED;
    ////////////////////////////////////////////////////////
    // Inform the sender stream that I'm closing:
    tmlObjWrapper* command = new tmlObjWrapper();
    TML_INT32 iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_CLOSE);
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Flag for internal command:
    iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, m_iStreamID);

    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    if (TML_SUCCESS == iRet){
      m_log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "reset", "SendSync CMD to source", "CMD_INTERNAL_STREAM_CLOSE");
      iRet = m_sender->sender_SendSyncMessage(m_sProfile, m_sHost, m_sPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
      m_log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "reset", "CMD_INTERNAL_STREAM_CLOSE to source", "finished");
    }
    delete (command);
  }
  if (NULL != m_sProfile)
    delete[] m_sProfile;
  m_sProfile = NULL;
  if (NULL != m_sHost)
    delete[] m_sHost;
  m_sHost = NULL;
  if (NULL != m_sPort)
    delete[] m_sPort;
  m_sPort = NULL;

  if (NULL != m_sKey)
    delete[] m_sKey;
  m_sKey = NULL;

  m_iStreamID = 0;
  m_iLastErr = TML_SUCCESS;
}


/**
 * @brief    Get the size of a receiver stream.
 */
int tmlReceiverStream::getSize (TML_INT64* rsize){
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;

  if (OPENED == m_bOpenState){
    tmlObjWrapper* command = new tmlObjWrapper();
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_GET_SIZE);
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Flag for internal command:
    iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, m_iStreamID);

    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    if (TML_SUCCESS == iRet){
      iRet = m_sender->sender_SendSyncMessage(m_sProfile, m_sHost, m_sPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
      getLastErrorIntern(&m_iLastErr);
    }

    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_SIZE, rsize);

    delete (command);
  }
  return (int)iRet;
}


/**
 * @brief    Perform a seek operation of a receiver stream.
 */
int tmlReceiverStream::seek (TML_INT64 seekPos, tmlSeekOrigin origin){
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;

  if (OPENED == m_bOpenState){
    tmlObjWrapper* command = new tmlObjWrapper();
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_SEEK);
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Flag for internal command:
    iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, m_iStreamID);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_SEEK_POSITION, seekPos);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_SEEK_ORIGIN, (TML_INT64)origin);

    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    if (TML_SUCCESS == iRet){
      iRet = m_sender->sender_SendSyncMessage(m_sProfile, m_sHost, m_sPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
      getLastErrorIntern(&m_iLastErr);
    }

    delete (command);
  }
  return (int)iRet;
}


/**
 * @brief    Get the position of a receiver stream.
 */
int tmlReceiverStream::getPosition (TML_INT64* rposition){
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;

  if (OPENED == m_bOpenState){
    tmlObjWrapper* command = new tmlObjWrapper();
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_GET_POSITION);
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Flag for internal command:
    iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, m_iStreamID);

    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    if (TML_SUCCESS == iRet){
      iRet = m_sender->sender_SendSyncMessage(m_sProfile, m_sHost, m_sPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
      getLastErrorIntern(&m_iLastErr);
    }
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_POSITION, rposition);

    delete (command);
  }
  return (int)iRet;
}


/**
 * @brief    Call internal for the last error.
 */
int tmlReceiverStream::getLastErrorIntern(TML_INT32* iErr){
  ////////////// NO LAST ERROR //////////////
  /*
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;

  if (OPENED == m_bOpenState){
    tmlObjWrapper* command = new tmlObjWrapper();
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_GET_LAST_ERR);
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
    iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, m_iStreamID);

    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    if (TML_SUCCESS == iRet){
      iRet = m_sender->sender_SendSyncMessage(m_sProfile, m_sHost, m_sPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
      if (TML_SUCCESS == iRet){
        command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);
        TML_INT64 errVal;
        iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_LAST_ERR, &errVal);
        *iErr = (TML_INT32) errVal;
        ///////////////////////////////////////////////
        // Now I can release the critical section use: 
        command->tmlObjWrapper_Release_Sidex_Handle();
      }
      else
      {
        /////////////////////////////////////////
        // Could not get the error from the other
        *iErr = iRet;
      }
    }

    delete (command);
  }
  return iRet;
  */
  return TML_SUCCESS;
}

/**
 * @brief    Write a portion of data to a receiver stream.
 */
int tmlReceiverStream::writeOneBuffer(void* buffer, TML_INT32 count){
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;

  if (OPENED == m_bOpenState){
    tmlObjWrapper* command = new tmlObjWrapper();
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_WRITE);
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Flag for internal command:
    iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, m_iStreamID);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Binary_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_WRITE_DATA, (const unsigned char*)buffer, count);

    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    if (TML_SUCCESS == iRet){

      m_log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "write", "SendSync CMD", "CMD_INTERNAL_STREAM_READ");
      iRet = m_sender->sender_SendSyncMessage(m_sProfile, m_sHost, m_sPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
      m_log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "write", "CMD_INTERNAL_STREAM_READ", "finished");
    }
    delete (command);
  }
  return (int)iRet;
}


/**
 * @brief    Write data to a receiver stream.
 */
int tmlReceiverStream::write(void* buffer, TML_INT32 count){
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;

  if (OPENED == m_bOpenState){
    bool bAllBytesWritten = false;
    const char* writeAddr = (const char*) buffer;
    TML_INT32 iBytesToWrite = 0;
    do{
      writeAddr += iBytesToWrite;
      count -= iBytesToWrite;
      if (count > MAX_STREAM_WRITE_BUFFER){
        iBytesToWrite = MAX_STREAM_WRITE_BUFFER;
      }
      else{
        iBytesToWrite = count;
        bAllBytesWritten = true;
      }
      iRet = writeOneBuffer((void*)writeAddr, iBytesToWrite);
    }
    while (TML_SUCCESS == iRet && !bAllBytesWritten);
  }
  return (int)iRet;
}


/**
 * @brief    Read data of a receiver stream.
 */
int tmlReceiverStream::read (void* buffer, TML_INT32 count, TML_INT32* bytesRead){
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;

  if (OPENED == m_bOpenState){
    tmlObjWrapper* command = new tmlObjWrapper();
    iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_READ);
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Flag for internal command:
    iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, m_iStreamID);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_SIZE, count);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_DEST_BUFFER_ADDR, (TML_INT64)buffer);

    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    if (TML_SUCCESS == iRet){

      m_log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "read", "SendSync CMD", "CMD_INTERNAL_STREAM_READ");
      iRet = m_sender->sender_SendSyncMessage(m_sProfile, m_sHost, m_sPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, 6000, NULL, true);
      getLastErrorIntern(&m_iLastErr);

      m_log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "read", "CMD_INTERNAL_STREAM_READ", "finished");
    }
    if (TML_SUCCESS == iRet){
      TML_INT64 bytes = 0;
      TML_INT32 iInternalRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_BYTRES_READ, &bytes);
      if (TML_SUCCESS == iInternalRet){
        ////////////////////////////////////////////
        // Return has a length and is valid:
        *bytesRead = (TML_INT32) bytes;
      }
      else{
        ////////////////////////////////////////////
        // Return has length of 0 
        *bytesRead = 0;
      }
    } 

    delete (command);

    //////////////////////////////////////////////////////////////////////////////
    // Error in case of 0 bytes:
    if (0 == *bytesRead){
      iRet = TML_ERR_STREAM_READ_FAILED;
    }
  }
  return (int)iRet;
}


/**
 * @brief    Read data of a receiver stream.
 */
int tmlReceiverStream::readBuffer (void* buffer, TML_INT32 count){
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;

  if (OPENED == m_bOpenState){
    TML_INT32 bytesRead;
    iRet = read (buffer, count, &bytesRead);


    if (TML_SUCCESS == iRet){
      if (bytesRead != count){
        iRet = TML_ERR_STREAM_READBUFFER_EOF;
      }
    }
  }
  return iRet;
}


/**
 * @brief    Download data of a receiver stream.
 */
int tmlReceiverStream::downloadData (tmlCriticalSectionObj* criticalSectionReceiver, TMLUniversalHashTable* recStreamObjects, TML_UINT32 buffersize, TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC pCBFuncDld, TML_POINTER pCBDataDld, TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncDldFinish, TML_POINTER pCBDataDldFinish){
  TML_INT32 iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
  if (OPENED == m_bOpenState){
    tmlObjWrapper* command = new tmlObjWrapper();

    TML_INT64 size = 0;
    ///////////////////////////////////////////////////
    // First get the size of the download:
    iRet = getSize (&size);
    if (TML_SUCCESS == iRet){
      /////////////////////////////////
      // Seek to the beginning:
      iRet = seek (0, soFromBeginning);
    }
    if (TML_SUCCESS == iRet){
      iRet = command->tmlObjWrapper_Header_SetCommand (CMD_INTERNAL_STREAM_DOWNLOAD);
    }
    ///////////////////////////////////////////////
    // Acquire critical section use: 
    SIDEX_HANDLE sHandle;
    command->tmlObjWrapper_Acquire_Sidex_Handle(&sHandle);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Flag for internal command:
    if (TML_SUCCESS == iRet)
      iRet = sidex_Boolean_Write (sHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_INTERNAL_COMMAND, true);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, m_iStreamID);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_CONTENT_SIZE, size);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_SIZE, buffersize);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_RECEIVER_HASH, (TML_INT64)recStreamObjects);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_CRITICAL_SECTION, (TML_INT64)criticalSectionReceiver);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_DLD_CALLBACK_FUNC, (TML_INT64)pCBFuncDld);
    if (TML_SUCCESS == iRet)
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_DLD_CALLBACK_FUNC_DATA, (TML_INT64)pCBDataDld);

    ///////////////////////////////////////////////
    // Now I can release the critical section use: 
    command->tmlObjWrapper_Release_Sidex_Handle();

    if (TML_SUCCESS == iRet){
      m_cp = new comParamStruct;
      m_cp->sProfile   = m_sProfile;
      m_cp->sHost      = m_sHost;
      m_cp->sPort      = m_sPort;
      m_cp->sKey       = m_sKey;
      m_cp->iWindowSize = m_iWindowSize;
      m_cp->iStreamID  = m_iStreamID;
      m_cp->sender     = m_sender;
      m_cp->iLastErr   = &m_iLastErr;
      m_cp->log        = m_log;
      m_cp->pCBFuncRet = pCBFuncDldFinish;
      m_cp->pCBDataRet = pCBDataDldFinish;
      m_cp->coreHandle = m_coreHandle;

      tml_Cmd_Register_CommandReady((TML_COMMAND_HANDLE) command, &downloadReadyReturnMethod, m_cp);
    }

    if (TML_SUCCESS == iRet){

      m_log->log (TML_LOG_STREAM_HANDLING, "tmlReceiverStream", "downloadData", "SendAsync CMD", "CMD_INTERNAL_STREAM_DOWNLOAD");
      iRet = m_sender->sender_SendAsyncMessage(m_sProfile, m_sHost, m_sPort, m_iWindowSize, (TML_COMMAND_HANDLE) command, 6000, TMLCOM_MODE_ASYNC, true, true);
    }
    if (TML_SUCCESS != iRet){
      ///////////////////////////////////////////////
      //  In case of an error I have to delete the command
      //  On SUCCESS the "downloadReadyReturnMethod" will do this
      delete (command);
    }
  }
  return (int)iRet;
}
