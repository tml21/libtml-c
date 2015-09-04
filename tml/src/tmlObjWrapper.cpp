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
 
#include <time.h>
#include <string.h>
#include "tmlErrors.h"
#include "tmlObjWrapper.h"
#include "logValues.h"
#include "tmlCmdDefines.h"
#include "unicode.h"
#include <vortex.h>
#include "tmlCore.h"
#ifdef LINUX
#include <errno.h>
#endif // LINUX

/**
 * @brief    Definition of the length for the char array to set the date arribute
 */
#define DATE_STRING_LENGTH 256

/**
 * @brief    Constructor.
 */
tmlObjWrapper::tmlObjWrapper()
{
  ////////////////////////////////////////////////////
  // Instance of the SIDEX- XML data binding object
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Cmd_Create", "INTERNAL", (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
  m_coreHandle  = TML_HANDLE_TYPE_NULL;
  m_iSessionID = -1;
  m_objChannelID = -1;
  m_objChannel = NULL;
  m_iMessageID = -1;
  m_iUnicodeForStatusReply = idUNICODE_WCHAR_T;
  m_sidexHandle = SIDEX_HANDLE_TYPE_NULL;
  sidex_Create(TML_ROOT_NODE, &m_sidexHandle);
  m_bCoreHandleIsValid = false;
  m_bProfileIsValid = false;
  m_sProfile=NULL;
  m_sProfileUtf32=NULL;
  m_sProfileUtf16=NULL;
  m_UnicodeStatus = TML_SUCCESS;
  m_bSessionIDIsValid = false;
  m_bObjChannelIsValid = false;
  m_bObjChannelIDIsValid = false;
  m_bMessageIDIsValid = false;
  m_pProgressCallback = NULL;
  m_pProgressCallbackData = NULL;
  m_pSendStatusReplyCallback = NULL;
  m_pSendStatusReplyCallbackData = NULL;
  m_pCommandReadyCallback = NULL;
  m_pCommandReadyCallbackData = NULL;
  m_csObj = new tmlCriticalSectionObj();

  ////////////////////////////////
  // alloc memory for char array to set the date attribute
  m_date = new char[DATE_STRING_LENGTH];
  ////////////////////////////////
  // automatic set creation time:
  tmlObjWrapper_Header_SetCreationTime(getFormattedTime());
}

tmlObjWrapper::tmlObjWrapper(char* sID)
{
  ////////////////////////////////////////////////////
  // Instance of the SIDEX- XML data binding object
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Cmd_Create", sID, (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
  m_coreHandle  = TML_HANDLE_TYPE_NULL;
  m_iSessionID = -1;
  m_objChannelID = -1;
  m_objChannel = NULL;
  m_iMessageID = -1;
  m_iUnicodeForStatusReply = idUNICODE_WCHAR_T;
  m_sidexHandle = SIDEX_HANDLE_TYPE_NULL;
  sidex_Create(TML_ROOT_NODE, &m_sidexHandle);
  m_bCoreHandleIsValid = false;
  m_bProfileIsValid = false;
  m_sProfile=NULL;
  m_sProfileUtf32=NULL;
  m_sProfileUtf16=NULL;
  m_UnicodeStatus = TML_SUCCESS;
  m_bSessionIDIsValid = false;
  m_bObjChannelIsValid = false;
  m_bObjChannelIDIsValid = false;
  m_bMessageIDIsValid = false;
  m_pProgressCallback = NULL;
  m_pProgressCallbackData = NULL;
  m_pSendStatusReplyCallback = NULL;
  m_pSendStatusReplyCallbackData = NULL;
  m_pCommandReadyCallback = NULL;
  m_pCommandReadyCallbackData = NULL;
  m_csObj = new tmlCriticalSectionObj();
  ////////////////////////////////
  // alloc memory for char array to set the date attribute
  m_date = new char[DATE_STRING_LENGTH];
  ////////////////////////////////
  // automatic set creation time:
  tmlObjWrapper_Header_SetCreationTime(getFormattedTime());
}

/**
 * @brief    Destructor.
 */
tmlObjWrapper::~tmlObjWrapper()
{
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Cmd_Free", "begin", (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
   ////////////////////////////////////////////////////
   // Instance of the SIDEX- XML data binding object
  if (SIDEX_HANDLE_TYPE_NULL != m_sidexHandle){
    sidex_Free (&m_sidexHandle) ;
    m_sidexHandle = SIDEX_HANDLE_TYPE_NULL;
  }
  m_bCoreHandleIsValid = false;
  m_bProfileIsValid = false;
  if (NULL != m_sProfile){
    delete[] m_sProfile;
  }
  m_sProfile=NULL;
  if (NULL != m_sProfileUtf32){
    delete (m_sProfileUtf32);
  }
  m_sProfileUtf32=NULL;
  if (NULL != m_sProfileUtf16){
    delete (m_sProfileUtf16);
  }
  m_sProfileUtf16=NULL;
  m_UnicodeStatus = TML_SUCCESS;
  m_bSessionIDIsValid = false;
  m_bObjChannelIsValid = false;
  m_bObjChannelIDIsValid = false;
  m_bMessageIDIsValid = false;
  m_pProgressCallback = NULL;
  m_pSendStatusReplyCallback = NULL;
  m_iUnicodeForStatusReply = idUNICODE_WCHAR_T;
  delete (m_csObj);

  ////////////////////////////////
  // free memory for char array to set the date attribute
  delete[] m_date;
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Cmd_Free", "end", (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
}


/**
 * @brief    Get the local time as a formatted string
 */
char* tmlObjWrapper::getFormattedTime(){
  // Set the CMD creation date:
#ifdef LINUX
  time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  struct timeval detail_time;
  gettimeofday(&detail_time,NULL);
  int iMilliSeconds = detail_time.tv_usec % 1000;

  sprintf(m_date, "%4d%2d%2d%2d%2d%2d%3d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,
                                                                      timeinfo->tm_mday,
                                                                       timeinfo->tm_hour,
                                                                        timeinfo->tm_min,
                                                                         timeinfo->tm_sec,
                                                                          iMilliSeconds);
#else // LINUX
  SYSTEMTIME lpSystemTime;
  GetLocalTime(&lpSystemTime);
  
#ifdef MINGW_BUILD
  sprintf(m_date, "%4d%2d%2d%2d%2d%2d%3d", lpSystemTime.wYear,
                                                                    lpSystemTime.wMonth,
                                                                      lpSystemTime.wDay,
                                                                        lpSystemTime.wHour,
                                                                          lpSystemTime.wMinute,
                                                                            lpSystemTime.wSecond,
                                                                              lpSystemTime.wMilliseconds);
#else //MINGW_BUILD
  sprintf_s(m_date, DATE_STRING_LENGTH, "%4d%2d%2d%2d%2d%2d%3d", lpSystemTime.wYear,
                                                                    lpSystemTime.wMonth,
                                                                      lpSystemTime.wDay,
                                                                        lpSystemTime.wHour,
                                                                          lpSystemTime.wMinute,
                                                                            lpSystemTime.wSecond,
                                                                              lpSystemTime.wMilliseconds);
#endif //MINGW_BUILD
#endif // LINUX
  // Replace each blank with the '0' character:
  for (unsigned int i = 0; i < strlen(m_date);++i){
    if (' ' == m_date[i]){
      m_date[i] = '0';
    }
  }
  return m_date;
}


/**
 * @brief   Register a callback method to get progress information
 */
void tmlObjWrapper::tmlObjWrapper_Register_Progress(TML_ON_PROGRESS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData){
  m_pProgressCallback = pCBFunc;
  m_pProgressCallbackData = pCBData;
}


/**
 * @brief   Get registered callback method to send progress infos
 */
void tmlObjWrapper::tmlObjWrapper_Registered_Progress(TML_ON_PROGRESS_REPLY_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  *pCBFunc = m_pProgressCallback;
  *pCBData = m_pProgressCallbackData;
}


/**
 * @brief   Register a callback method to get status reply information
 */
void tmlObjWrapper::tmlObjWrapper_Register_StatusReply(TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode){
  m_pSendStatusReplyCallback = pCBFunc;
  m_pSendStatusReplyCallbackData = pCBData;
  m_iUnicodeForStatusReply = iUnicode;
}

    
/**
 * @brief   Get registered callback method to get status reply information
 */
void tmlObjWrapper::tmlObjWrapper_Registered_StatusReply(TML_ON_STATUS_REPLY_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode){
  *pCBFunc = m_pSendStatusReplyCallback;
  *pCBData = m_pSendStatusReplyCallbackData;
  *iUnicode = m_iUnicodeForStatusReply;
}


/**
 * @brief   Register a callback method to get the result of a command execution
 */
void tmlObjWrapper::tmlObjWrapper_Register_CommandReady(TML_ON_COMMAND_READY_CB_FUNC pCBFunc, TML_POINTER pCBData){
  m_pCommandReadyCallback = pCBFunc;
  m_pCommandReadyCallbackData = pCBData;
}


/**
 * @brief   Get registered callback method to set the result of a command execution
 */
void tmlObjWrapper::tmlObjWrapper_Registered_CommandReady(TML_ON_COMMAND_READY_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  *pCBFunc = m_pCommandReadyCallback;
  *pCBData = m_pCommandReadyCallbackData;
}

    
/**
 * @brief Get the command creation time.
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetCreationTime(char** time){

  TML_INT32 iContentSize;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetCreationTime");
  TML_INT32 iRet = sidex_String_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_TIMESTAMP, time, &iContentSize);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetCreationTime");
  if (TML_SUCCESS != iRet)
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  return (int)iRet;
}


/**
 * @brief    Set the receiver core reference attribute. 
 */
void tmlObjWrapper::tmlObjWrapper_Attr_Set_Core_Reference(TML_CORE_HANDLE tmlcorehandle){
  m_bCoreHandleIsValid = true;
  m_coreHandle = tmlcorehandle;
}


/**
 * @brief    Get the receiver core reference attribute. 
 */
int tmlObjWrapper::tmlObjWrapper_Attr_Get_Core_Reference(TML_CORE_HANDLE* tmlcorehandle){
  int iRet = TML_ERR_ATTRIBUTE_NOT_SET;
  if (m_bCoreHandleIsValid){
    *tmlcorehandle = m_coreHandle;
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Set receiver command session ID. 
 */
void tmlObjWrapper::tmlObjWrapper_Attr_Set_Session_ID(int iSesionID){
  m_bSessionIDIsValid = true;
  m_iSessionID = iSesionID;
}


/**
 * @brief    Get receiver command session ID. 
 */
int tmlObjWrapper::tmlObjWrapper_Attr_Get_Session_ID(TML_INT32* iSesionID){
  int iRet = TML_ERR_ATTRIBUTE_NOT_SET;
  if (m_bSessionIDIsValid){
    *iSesionID = (TML_INT32) m_iSessionID;
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Set receiver command profile. 
 */
void tmlObjWrapper::tmlObjWrapper_Attr_Set_Profile(const char* profile){

  if (NULL != m_sProfile){
    delete[] m_sProfile;
  }
  if (NULL != m_sProfileUtf32){
    delete (m_sProfileUtf32);
  }
  if (NULL != m_sProfileUtf16){
    delete (m_sProfileUtf16);
  }
  if (NULL != profile){
    m_bProfileIsValid = true;
    m_sProfile = new char[strlen(profile)+ 1];
#if defined (LINUX) || defined (MINGW_BUILD)
    strcpy(m_sProfile, profile);
#else
    strcpy_s(m_sProfile, strlen(profile)+ 1, profile);
#endif
    TML_INT32 iLength;
    m_sProfileUtf32 = UTF8toUTF32(m_sProfile, &iLength);
    if (NULL == m_sProfileUtf32){
      m_UnicodeStatus = TML_ERR_UNICODE;
    }
    else{
      m_sProfileUtf16 = (char16_t*)UTF8toUTF16(m_sProfile, &iLength);
      if (NULL == m_sProfileUtf16){
        m_UnicodeStatus = TML_ERR_UNICODE;
      }
      else{
        m_UnicodeStatus = TML_SUCCESS;
      }
    }
  }
  else{
    m_bProfileIsValid = false;
    m_sProfile=NULL;
    m_sProfileUtf32=NULL;
    m_sProfileUtf16=NULL;
    m_UnicodeStatus = TML_SUCCESS;
  }
}


/**
 * @brief    Get receiver command profile. 
 */
int tmlObjWrapper::tmlObjWrapper_Attr_Get_Profile_A(const char** profile){
  int iRet = TML_ERR_ATTRIBUTE_NOT_SET;
  if (m_bProfileIsValid){
    *profile = m_sProfile;
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Get receiver command profile. 
 */
int tmlObjWrapper::tmlObjWrapper_Attr_Get_Profile_X(const wchar_t** profile){
  int iRet = TML_ERR_ATTRIBUTE_NOT_SET;
  if (m_bProfileIsValid){
    if (TML_SUCCESS == m_UnicodeStatus){
      *profile = m_sProfileUtf32;
      iRet = TML_SUCCESS;
    }
    else{
      iRet = TML_ERR_UNICODE;
    }
  }
  return iRet;
}


/**
 * @brief    Get receiver command profile. 
 */
int tmlObjWrapper::tmlObjWrapper_Attr_Get_Profile_W(const char16_t** profile){
  int iRet = TML_ERR_ATTRIBUTE_NOT_SET;
  if (m_bProfileIsValid){
    if (TML_SUCCESS == m_UnicodeStatus){
      *profile = m_sProfileUtf16;
      iRet = TML_SUCCESS;
    }
    else{
      iRet = TML_ERR_UNICODE;
    }
  }
  return iRet;
}


/**
 * @brief    Set receiver command channel. 
 */
void tmlObjWrapper::tmlObjWrapper_Attr_Set_Channel(void* channel){
  m_bObjChannelIsValid = true;
  m_objChannel = channel;
}


/**
 * @brief    Get receiver command channel. 
 */
int tmlObjWrapper::tmlObjWrapper_Attr_Get_Channel(void** channel){
  int iRet = TML_ERR_ATTRIBUTE_NOT_SET;
  if (m_bObjChannelIsValid){
    *channel = m_objChannel;
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Set receiver command channel ID. 
 */
void tmlObjWrapper::tmlObjWrapper_Attr_Set_Channel_ID(int channelID){
  m_bObjChannelIDIsValid = true;
  m_objChannelID = channelID;
}


/**
 * @brief    Get receiver command channel ID. 
 */
int tmlObjWrapper::tmlObjWrapper_Attr_Get_Channel_ID(TML_INT32* channelID){
  int iRet = TML_ERR_ATTRIBUTE_NOT_SET;
  if (m_bObjChannelIDIsValid){
    *channelID = (TML_INT32)m_objChannelID;
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief    Set receiver command message ID. 
 */
void tmlObjWrapper::tmlObjWrapper_Attr_Set_Message_ID(int iMessageID){
  m_bMessageIDIsValid = true;
  m_iMessageID = iMessageID;
}


/**
 * @brief    Get receiver command message ID. 
 */
int tmlObjWrapper::tmlObjWrapper_Attr_Get_Message_ID(TML_INT32* iMessageID){
  int iRet = TML_ERR_ATTRIBUTE_NOT_SET;
  if (m_bMessageIDIsValid){
    *iMessageID = (TML_INT32)m_iMessageID;
    iRet = TML_SUCCESS;
  }
  return iRet;
}


/**
 * @brief Set the command creation time / mandatory command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetCreationTime(char* time){
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetCreationTime");
  TML_INT32 iRet = sidex_String_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_TIMESTAMP, time);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetCreationTime");
  return (int)iRet;
}


/**
 * @brief Get the command key.    
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetCommand(TML_COMMAND_ID_TYPE* cmd){
  TML_INT32 iRet;
  TML_INT64 cmdValue;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetCommand");
  iRet = sidex_Integer_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_COMMAND, &cmdValue);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetCommand");
  if (TML_SUCCESS == iRet){
    *cmd = (TML_COMMAND_ID_TYPE) cmdValue;
  }
  else{
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  return (int)iRet;
}


/**
 * @brief Set the command key / mandatory command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetCommand(TML_COMMAND_ID_TYPE cmd){
  TML_INT32 iRet;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetCommand");
  iRet = sidex_Integer_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_COMMAND, cmd);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetCommand");
  return (int)iRet;
}


/**
 * @brief Get the error key of the command execution.
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetError(TML_INT32* error){
  TML_INT32 iRet;
  TML_INT64 errValue;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetError");
  iRet = sidex_Integer_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_ERRID, &errValue);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetError");
  if (TML_SUCCESS == iRet){
    *error = (TML_INT32) errValue;
  }
  else{
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  return (int)iRet;
}


/**
 * @brief Set the error key of the command execution / mandatory command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetError(TML_INT32 error){
  TML_INT32 iRet;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetError");
  iRet = sidex_Integer_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_ERRID, error);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetError");
  return (int)iRet;
}


/**
 * @brief Get the error message of the command execution.
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetErrorMessage(char** msg, TML_INT32* iMsgLength){
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetErrorMessage");
  TML_INT32 iVal;
  TML_INT32 iRet = sidex_String_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_ERRMSG, msg, &iVal);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetErrorMessage");
  if (TML_SUCCESS != iRet){
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  else{
    *iMsgLength = (TML_INT32) iVal;
  }
  return (int)iRet;
}


/**
 * @brief Set the error message of the command execution / optional command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetErrorMessage(const char* msg, int iMsgLength){
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetErrorMessage");
  TML_INT32 iRet = sidex_String_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_ERRMSG, msg);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetErrorMessage");
  return (int)iRet;
}


/**
 * @brief Get the command execution state.
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetState(TML_INT32* state){
  TML_INT32 iRet;
  TML_INT64 stateValue;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetState");
  iRet = sidex_Integer_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_STATE, &stateValue);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetState");
  if (TML_SUCCESS == iRet){
    *state = (TML_INT32) stateValue;
  }
  else{
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  return (int)iRet;
}


/**
 * @brief Set the command execution state / mandatory command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetState(TML_INT32 state){
  TML_INT32 iRet = TML_ERR_COMMAND_STATE_UNDEFINED;
  switch (state){
    case TMLCOM_CSTATE_CREATED:
    case TMLCOM_CSTATE_EXECUTED:
    case TMLCOM_CSTATE_FAILED:
    case TMLCOM_CSTATE_PENDING:
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetState");
         iRet = sidex_Integer_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_STATE, state);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetState");
         break;
    default:
         break;
  }
  return (int)iRet;
}


/**
 * @brief Get the command execution mode.
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetMode(TML_INT32* mode){
  TML_INT32 iRet;
  TML_INT64 modeValue;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetMode");
  iRet = sidex_Integer_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_MODE, &modeValue);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetMode");
  if (TML_SUCCESS == iRet){
    *mode = (TML_INT32) modeValue;
  }
  else{
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  return (int)iRet;
}


/**
 * @brief Set the command execution mode / mandatory command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetMode(TML_INT32 mode){
  TML_INT32 iRet = TML_ERR_COMMAND_MODE_UNDEFINED;
  switch (mode){
    case TMLCOM_MODE_ASYNC:
    case TMLCOM_MODE_SYNC:
    case TMLCOM_MODE_EVT:
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetMode");
         iRet = sidex_Integer_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_MODE, mode);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetMode");
         break;
    default:
         break;
  }
  return (int)iRet;
}


/**
 * @brief Get the command reply type.
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetReplyType(TML_INT32* type){
  TML_INT32 iRet;
  TML_INT64 typeValue;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetReplyType");
  iRet = sidex_Integer_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_REPLYTYPE, &typeValue);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetReplyType");
  if (TML_SUCCESS == iRet){
    *type = (TML_INT32) typeValue;
  }
  else{
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  return (int)iRet;
}


/**
 * @brief Get the command reply type / optional command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetReplyType(TML_INT32 type){
  TML_INT32 iRet = TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED;
  switch (type){
    case TMLCOM_RPY_PROGRESS:
    case TMLCOM_RPY_WARNING:
    case TMLCOM_RPY_ERROR:
    case TMLCOM_RPY_INFORMATION:
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetReplyType");
         iRet =  sidex_Integer_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_REPLYTYPE, type);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetReplyType");
         break;
    default:
         break;
  }
  return (int)iRet;
}


/**
 * @brief Get the command execution progress.
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetProgress(TML_INT32* progress){
  TML_INT32 iRet;
  TML_INT64 progrValue;
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetProgress");
  iRet = sidex_Integer_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_PROGRESS, &progrValue);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetProgress");
  if (TML_SUCCESS == iRet){
    *progress = (TML_INT32) progrValue;
  }
  else{
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  return (int)iRet;
}


/**
 * @brief Set the command execution progress / optional command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetProgress(TML_INT32 progress){
  TML_INT32 iRet;

  if (progress < 0 || progress > 100)
    iRet = TML_ERR_COMMAND_PROGRESS_RANGE;
  else{
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetProgress");
    iRet = sidex_Integer_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_PROGRESS, progress);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetProgress");
  }
  return (int)iRet;
}


/**
 * @brief Get the reply message of the command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_GetReplyMessage(char** msg, TML_INT32* iMsgLength){
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_GetReplyMessage");
  TML_INT32 iVal;
  TML_INT32 iRet = sidex_String_Read (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_REPLYMSG, msg, &iVal);
  *iMsgLength = (TML_INT32) iVal;
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_GetReplyMessage");
  if (TML_SUCCESS != iRet)
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  return (int)iRet;
}


/**
 * @brief Set the reply message of the command / optional command.
 */
int tmlObjWrapper::tmlObjWrapper_Header_SetReplyMessage(char* msg, int iMsgLength){
  SIDEX_HANDLE shandle;
  tmlObjWrapper_Acquire_Sidex_Handle(&shandle, (char*)"tmlObjWrapper::tmlObjWrapper_Header_SetReplyMessage");
  TML_INT32 iRet = sidex_String_Write (m_sidexHandle, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_REPLYMSG, msg);
  tmlObjWrapper_Release_Sidex_Handle((char*)"tmlObjWrapper::tmlObjWrapper_Header_SetReplyMessage");
  return (int)iRet;
}


/**
 * @brief Set all the error relevant attributes in the header if they are not already set.
 */
void tmlObjWrapper::tmlObjWrapper_Header_setLogicalError(int iError, const char* sErrMsg){
  int iRet; //Return value - don't pay attention to it !

  TML_INT32 iReturnedErr = TML_SUCCESS;

  ////////////////////////////////////
  // Read error ID:
  iRet = tmlObjWrapper_Header_GetError(&iReturnedErr);
  if (TML_SUCCESS == iReturnedErr){
    bool bAllowedToOverwrite = false;
    char* msg = NULL;
    TML_INT32 iMsgLen = 0;
    ////////////////////////////////////
    // Read error Message:
    iRet = tmlObjWrapper_Header_GetErrorMessage(&msg, &iMsgLen);
    if (0 >= iMsgLen){
      ///////////////////////////////////////////////////
      // no returnd error or message / so can overwrite:
      bAllowedToOverwrite = true;
    }
    else{
      //////////////////////////////////////////////////////////////////
      // Set the error ID to TML_ERR_ERROR_MSG_RECEIVED because I've got an message
      iRet = tmlObjWrapper_Header_SetError(TML_ERR_ERROR_MSG_RECEIVED);
    }
    if (NULL != msg){
      //////////////////////////////////////////////////////////////////
      // Free memory
      sidex_Free_ReadString(msg);
    }
    if (bAllowedToOverwrite){
      if(TML_SUCCESS != iError){
        //////////////////////////////////////////////////////////////////
        // Set failed state:
        iRet = tmlObjWrapper_Header_SetState(TMLCOM_CSTATE_FAILED);
        //////////////////////////////////////////////////////////////////
        // set error msg:
        iRet = tmlObjWrapper_Header_SetErrorMessage(sErrMsg, (int)strlen(sErrMsg));
      }
      //////////////////////////////////////////////////////////////////
      // Set the error ID:
      iRet = tmlObjWrapper_Header_SetError(iError);
    }
  }
}


/**
 * @brief   Returns the handle to the data object.
 */
int tmlObjWrapper::tmlObjWrapper_Acquire_Sidex_Handle(SIDEX_HANDLE* shandle){
  TML_INT32 iRet = TML_SUCCESS;
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore:tml_Cmd_Acquire_Sidex_Handle", "begin", "INTERNAL", (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
  bool bRet = m_csObj->tmlCriticalSectionEnter("tmlObjWrapper::tmlObjWrapper_Acquire_Sidex_Handle");
  if (!bRet){
    iRet = TML_ERR_CRITICAL_SECTION;
  }
  else{
    *shandle = m_sidexHandle;
  }
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore:tml_Cmd_Acquire_Sidex_Handle", "end", "INTERNAL", (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
  return (int)iRet;
}


/**
 * @brief   Returns the handle to the data object.
 */
int tmlObjWrapper::tmlObjWrapper_Acquire_Sidex_Handle(SIDEX_HANDLE* shandle, char* sID){
  TML_INT32 iRet = TML_SUCCESS;
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore:tml_Cmd_Acquire_Sidex_Handle", "begin", sID, (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
  bool bRet = m_csObj->tmlCriticalSectionEnter("tmlObjWrapper::tmlObjWrapper_Acquire_Sidex_Handle");
  if (!bRet){
    iRet = TML_ERR_CRITICAL_SECTION;
  }
  else{
    *shandle = m_sidexHandle;
  }
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore:tml_Cmd_Acquire_Sidex_Handle", "end", sID, (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
  return (int)iRet;
}

/**
 * @brief   Returns the handle to the data object.
 */
int tmlObjWrapper::tmlObjWrapper_Release_Sidex_Handle(){
  TML_INT32 iRet = TML_SUCCESS;
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Cmd_Release_Sidex_Handle", "INTERNAL", (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
  bool bRet = m_csObj->tmlCriticalSectionLeave("tmlObjWrapper::tmlObjWrapper_Release_Sidex_Handle");
  if (!bRet){
    iRet = TML_ERR_CRITICAL_SECTION;
  }
  return (int)iRet;
}


/**
 * @brief   Returns the handle to the data object.
 */
int tmlObjWrapper::tmlObjWrapper_Release_Sidex_Handle(char* sID){
  TML_INT32 iRet = TML_SUCCESS;
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Cmd_Release_Sidex_Handle", sID, (TML_COMMAND_HANDLE)this);
#endif //TIMEOUT_LOGGING
  bool bRet = m_csObj->tmlCriticalSectionLeave("tmlObjWrapper::tmlObjWrapper_Release_Sidex_Handle");
  if (!bRet){
    iRet = TML_ERR_CRITICAL_SECTION;
  }
  return (int)iRet;
}


/**
 * @brief    Debug output for internal use only.
 */
int tmlObjWrapper::tmlObjWrapper_Debug()
{ 
  TML_INT32 iRet = TML_SUCCESS;  

  iRet = sidex_Save_Content (m_sidexHandle, "test.xml");
  if (TML_SUCCESS != iRet)
  {
    iRet = TML_ERR_MISSING_OBJ;
  }
  return (int)iRet;
}


