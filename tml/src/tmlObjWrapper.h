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

#ifndef TMLOBJWRAPPER_H
#define TMLOBJWRAPPER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sidexCom.h>
#include "tmlCriticalSectionObj.h"
#include "tmlStdTypes.h"
#include "tmlCoreDefines.h"


class  tmlObjWrapper
{

private:
  /* data */
  SIDEX_HANDLE    m_sidexHandle;
  TML_CORE_HANDLE m_coreHandle;
  char*           m_sProfile;
  wchar_t*        m_sProfileUtf32;
  char16_t*       m_sProfileUtf16;
  TML_INT32       m_UnicodeStatus;
  int             m_iSessionID;
  int             m_objChannelID;
  void*           m_objChannel;
  int             m_iMessageID;
  bool            m_bCoreHandleIsValid;
  bool            m_bProfileIsValid;
  bool            m_bSessionIDIsValid;
  bool            m_bObjChannelIsValid;
  bool            m_bObjChannelIDIsValid;
  bool            m_bMessageIDIsValid;

  /**
    * @brief    char array to set the date arribute
    */
  char* m_date;

  /* methods */

  /**
  * @brief    Get the local time as a formatted string
  *
  * @returns reference to a time formatted string.
  */
char* getFormattedTime();



protected: 
  /* data */
  TML_ON_PROGRESS_REPLY_CB_FUNC   m_pProgressCallback;
  TML_POINTER                     m_pProgressCallbackData;
  TML_ON_STATUS_REPLY_CB_FUNC     m_pSendStatusReplyCallback;
  TML_POINTER                     m_pSendStatusReplyCallbackData;
  tmlUnicodeID                    m_iUnicodeForStatusReply;
  TML_ON_COMMAND_READY_CB_FUNC    m_pCommandReadyCallback;
  TML_POINTER                     m_pCommandReadyCallbackData;
  tmlCriticalSectionObj           *m_csObj;


public:
    /* data */
    /* methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlObjWrapper
     */
    tmlObjWrapper();


    /**
     * @brief    Constructor.
     *
     * @param   sID Identification (log information)
     *
     * @returns an instance of tmlObjWrapper
     */
    tmlObjWrapper(char* sID);


    /**
     * @brief    Destructor.
     */
    ~tmlObjWrapper();


    /**
     * @brief   Register a callback method to get progress information
     *
     * @param   pCBFunc Reference to callback method.
     * @param   pCBData Reference to callback method's data.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Register_Progress(TML_ON_PROGRESS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief   Get registered callback method to send progress infos
     *
     * @param   pCBFunc Reference to reference to callback method.
     * @param   pCBData Reference to reference to callback method's data.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Registered_Progress(TML_ON_PROGRESS_REPLY_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief   Register a callback method to get status reply information
     *
     * @param   pCBFunc  Reference to callback method.
     * @param   pCBData  Reference to callback method's data.
     * @param   iUnicode unicode identifier.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Register_StatusReply(TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);

    
    /**
     * @brief   Get registered callback method to get status reply information
     *
     * @param   pCBFunc Reference to reference to callback method.
     * @param   pCBData Reference to reference to callback method's data.
     * @param   bUnicode Reference unicode identifier.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Registered_StatusReply(TML_ON_STATUS_REPLY_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode);


    /**
     * @brief   Register a callback method to get the result of a command execution
     *
     * @param   pCBFunc Reference to callback method.
     * @param   pCBData Reference to callback method's data.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Register_CommandReady(TML_ON_COMMAND_READY_CB_FUNC pCBFunc, TML_POINTER pCBData);

    
    /**
     * @brief   Get registered callback method to set the result of a command execution
     *
     * @param   pCBFunc Reference to reference to callback method.
     * @param   pCBData Reference to reference to callback method's data.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Registered_CommandReady(TML_ON_COMMAND_READY_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Set the receiver core reference attribute. 
     *
     * @param   tmlcorehandle The TML_CORE_HANDLE.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Attr_Set_Core_Reference(TML_CORE_HANDLE tmlcorehandle);


    /**
     * @brief    Get the receiver core reference attribute. 
     *
     * @param   tmlcorehandle The TML_COMMAND_HANDLE to operate.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_ATTRIBUTE_NOT_SET in case of missing attribute.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Attr_Get_Core_Reference(TML_CORE_HANDLE* tmlcorehandle);


    /**
     * @brief    Set receiver command session ID. 
     *
     * @param   iSesionID receiver's message session id.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Attr_Set_Session_ID(int iSesionID);


    /**
     * @brief    Get receiver command session ID. 
     *
     * @param   iSesionID Reference to the receiver's message session id.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_ATTRIBUTE_NOT_SET in case of missing attribute.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Attr_Get_Session_ID(TML_INT32* iSesionID);


    /**
     * @brief    Set receiver command profile. 
     *
     * @param   profile receiver's message profile.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Attr_Set_Profile(const char* profile);


    /**
     * @brief    Get receiver command profile. 
     *
     * @param   profile Reference to the receiver's message profile.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_ATTRIBUTE_NOT_SET in case of missing attribute.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Attr_Get_Profile_A(const char** profile);


    /**
     * @brief    Get receiver command profile. 
     *
     * @param   profile Reference to the receiver's message profile.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_ATTRIBUTE_NOT_SET in case of missing attribute.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Attr_Get_Profile_X(const wchar_t** profile);


    /**
     * @brief    Get receiver command profile. 
     *
     * @param   profile Reference to the receiver's message profile.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_ATTRIBUTE_NOT_SET in case of missing attribute.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Attr_Get_Profile_W(const char16_t** profile);


    /**
     * @brief    Set receiver command channel ID. 
     *
     * @param   channel the receiver's channel.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Attr_Set_Channel(void* channel);


    /**
     * @brief    Get receiver command channel ID. 
     *
     * @param   channel Reference to the receiver's channel.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_ATTRIBUTE_NOT_SET in case of missing attribute.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Attr_Get_Channel(void** channel);


    /**
     * @brief    Set receiver command channel ID. 
     *
     * @param   channelID id of the receiver's channel.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Attr_Set_Channel_ID(int channelID);


    /**
     * @brief    Get receiver command channel ID. 
     *
     * @param   channelID Reference to the id of the receiver's channel.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_ATTRIBUTE_NOT_SET in case of missing attribute.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Attr_Get_Channel_ID(TML_INT32* channelID);


    /**
     * @brief    Set receiver command message ID. 
     *
     * @param   iMessageID id of the receiver's message.
     *
     * @see tmlStdTypes.h
     */
    void tmlObjWrapper_Attr_Set_Message_ID(int iMessageID);


    /**
     * @brief    Get receiver command message ID. 
     *
     * @param   iMessageID Reference to the id of the receiver's message.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_ATTRIBUTE_NOT_SET in case of missing attribute.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Attr_Get_Message_ID(TML_INT32* iMessageID);


    /**
     * @brief Get the command creation time.
     *
     *        After use it must be deallocated using sidex_Free_ReadString().
     *
     * @param time Reference to the command creation time.<br>
     *             ISO formated: YYYYMMDDhhmmssttt<br>
     *             YYYY = year, MM = month, DD= day, hh = hour, mm = minute, ss = second, ttt = miliseconds
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of missing information.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_GetCreationTime(char** time);


    /**
     * @brief Set the command creation time / mandatory command.
     *
     *        This API call is mandatory for a successful tml data transfer.
     *
     * @param time The command creation time.<br>
     *             ISO formated: YYYYMMDDhhmmssttt<br>
     *             YYYY = year, MM = month, DD= day, hh = hour, mm = minute, ss = second, ttt = miliseconds
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetCreationTime(char* time);


    /**
     * @brief Get the command key.    
     *
     * @param cmd Reference to the command key. For the command set see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of missing information.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
     int tmlObjWrapper_Header_GetCommand(TML_COMMAND_ID_TYPE* cmd);


    /**
     * @brief Set the command key / mandatory command.
     *
     *        This API call is mandatory for a successful tml data transfer.
     *
     * @param cmd The command key. For the command set see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetCommand(TML_COMMAND_ID_TYPE cmd);


    /**
     * @brief Get the error key of the command execution.
     *
     * @param error Reference to the error key. For the error keys see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of missing information.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_GetError(TML_INT32* error);


    /**
     * @brief Set the error key of the command execution / mandatory command.
     *
     *        This API call is mandatory for a successful tml data transfer.
     *
     * @param error The error key. For the error keys see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetError(TML_INT32 error);


    /**
     * @brief Get the error message of the command execution.
     *
     *        After use it must be deallocated using sidex_Free_ReadString().
     *
     * @param msg        Reference to the error message.
     * @param iMsgLength Reference to the length of the error message.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED if no message is present.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_GetErrorMessage(char** msg, TML_INT32* iMsgLength);


    /**
     * @brief Set the error message of the command execution / optional command.
     *
     *        This API call is optional for a successful tml data transfer.
     *
     * @param msg        The error message.
     * @param iMsgLength The length of the error message.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetErrorMessage(const char* msg, int iMsgLength);


    /**
     * @brief Get the command execution state.
     *
     * @param state Reference to the command execution state. For the state description see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of missing information.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_GetState(TML_INT32* state);


    /**
     * @brief Set the command execution state / mandatory command.
     *
     *        This API call is mandatory for a successful tml data transfer.
     *
     * @param state The command execution state. For the state description see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_COMMAND_STATE_UNDEFINED the state is unknown.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetState(TML_INT32 state);


    /**
     * @brief Get the command execution mode.
     *
     * @param mode Reference to the command execution mode. For the mode description see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of missing information.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_GetMode(TML_INT32* mode);


    /**
     * @brief Set the command execution mode / mandatory command.
     *
     *        This API call is mandatory for a successful tml data transfer.
     *
     * @param mode The command execution mode. For the mode description see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_COMMAND_MODE_UNDEFINED the mode is unknown.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetMode(TML_INT32 mode);


    /**
     * @brief Get the command reply type.
     *
     * @param type Reference to the command reply type. For the reply type description see the "TML Specification".
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of missing information.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_GetReplyType(TML_INT32* type);


    /**
     * @brief Get the command reply type / optional command.
     *
     *        This API call is optional for a successful tml data transfer.
     *
     * @param type The command reply type. For the reply type description see the "TML Specification".<br>
     *              Possible Values:<br>
     *              TMLCOM_RPY_PROGRESS<br>
     *              TMLCOM_RPY_WARNING<br>
     *              TMLCOM_RPY_ERROR<br>
     *              TMLCOM_RPY_INFORMATION
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED the mode is unknown.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetReplyType(TML_INT32 type);


    /**
     * @brief Get the command execution progress.
     *
     * @param progress Reference to the command execution progress (in percent).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED in case of missing information.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_GetProgress(TML_INT32* progress);


    /**
     * @brief Set the command execution progress / optional command.
     *
     *        This API call is optional for a successful tml data transfer.
     *
     * @param progress The command execution progress value (in percent).
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INVALID_PROGRESS if 0 > progress > 100.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetProgress(TML_INT32 progress);


    /**
     * @brief Get the reply message of the command.
     *
     *        After use it must be deallocated using sidex_Free_ReadString().
     *
     * @param msg        Reference to the reply message.
     * @param iMsgLength Reference to the length of the error message.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_INFORMATION_UNDEFINED if no message is present.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_GetReplyMessage(char** msg, TML_INT32* iMsgLength);


    /**
     * @brief Set the reply message of the command / optional command.
     *
     *        This API call is optional for a successful tml data transfer.
     *
     * @param msg        The reply message.
     * @param iMsgLength The length of the reply message.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Header_SetReplyMessage(char* msg, int iMsgLength);

    /**
     * @brief Set all the error relevant attributes in the header if they are not already set.
     *
     * @param iError     The error id.
     * @param sErrMsg    The error message.
     *
     */
    void tmlObjWrapper_Header_setLogicalError(int iError, const char* sErrMsg);

    /**
     * @brief   Returns the handle to the data object.
     *
     *          The handle is locked for private use (critical section) and<br>
     *          must be unlocked by using tmlObjWrapper_Release_Sidex_Handle().
     *
     * @param   shandle Reference to write the reference to the SIDEX_HANDLE.
     *
     * @see tmlErrors.h
     */
    int tmlObjWrapper_Acquire_Sidex_Handle(SIDEX_HANDLE* shandle);


    /**
     * @brief   Returns the handle to the data object.
     *
     *          The handle is locked for private use (critical section) and<br>
     *          must be unlocked by using tmlObjWrapper_Release_Sidex_Handle().
     *
     * @param   shandle Reference to write the reference to the SIDEX_HANDLE.
     * @param   sID Identification (log information)
     *
     * @see tmlErrors.h
     */
    int tmlObjWrapper_Acquire_Sidex_Handle(SIDEX_HANDLE* shandle, char* sID);


    /**
     * @brief   Release the private use of the data object (critical section).
     *
     *          see tmlObjWrapper_Acquire_Sidex_Handle().
     */
    int tmlObjWrapper_Release_Sidex_Handle();


    /**
     * @brief   Release the private use of the data object (critical section).
     *
     * @param   sID Identification (log information)
     *
     *          see tmlObjWrapper_Acquire_Sidex_Handle().
     */
    int tmlObjWrapper_Release_Sidex_Handle(char* sID);


    /**
     * @brief    Debug output for internal use only.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of missing objects. Maybe tml_Connection_New() failed or the connection has been closed.
     *
     * @see tmlErrors.h,tmlStdTypes.h
     */
    int tmlObjWrapper_Debug();

};

#endif  // TMLOBJWRAPPER_H
