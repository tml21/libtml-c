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
 
/** @file tmlStdTypes.h
  * @brief TML standard defines for data types and callback functions (handler)
  *
  * See also sidexStdTypes.h
  */

#ifndef TMLSTDTYPES_H
#define TMLSTDTYPES_H

#include "sidexStdTypes.h"

/**
* @brief  TML seek origin
*
* See tml_RecStream_Seek()
*/
typedef enum TML_SEEK_ENUM
{
   soFromBeginning,
   soFromCurrent,
   soFromEnd
}tmlSeekOrigin;

/**
* @brief    definition of string types (wchar_t* / char*) to support ANSI or UNICODE strings.<br>
*           If TML_UNICODE is defined wchar_t is used as TML_TCHAR.
* @ingroup tmlTypes
*/
#ifdef TML_UNICODE
  typedef wchar_t              TML_TCHAR;
#else
  typedef char                 TML_TCHAR;
#endif // TML_UNICODE

/**
* @brief    TML string type
* @ingroup tmlTypes
*/
typedef SIDEX_CTSTR            TML_CTSTR;

/**
* @brief    TML handle type / 64 bit signed integer
* @ingroup tmlTypes
*/
typedef SIDEX_HANDLE_TYPE     TML_HANDLE_TYPE;


/**
* @brief    TML core handle
* @ingroup tmlTypes
*/
typedef TML_HANDLE_TYPE       TML_CORE_HANDLE;


/**
* @brief    TML command handle
* @ingroup tmlTypes
*/
typedef TML_HANDLE_TYPE       TML_COMMAND_HANDLE;


/**
* @brief    TML connection handle
* @ingroup tmlTypes
*/
typedef TML_HANDLE_TYPE       TML_CONNECTION_HANDLE;


/**
* @brief    TML null handle
*/
#define TML_HANDLE_TYPE_NULL    SIDEX_HANDLE_TYPE_NULL


/**
* @brief    TML command id  / 32 bit unsigned integer
* @ingroup tmlTypes
*/
typedef SIDEX_UINT32          TML_COMMAND_ID_TYPE;


/**
* @brief    TML stream ID  / 64 bit integer
* @ingroup tmlTypes
*/
typedef SIDEX_INT64         TML_STREAM_ID_TYPE;

/**
* @brief    TML stream id null definition
* @ingroup tmlConstants
*/
#define TML_STREAM_ID_NULL             0

/**
* @brief    TML boolean type / 32bit (legal values are 0 or 1)
* @ingroup tmlTypes
*/
typedef SIDEX_BOOL            TML_BOOL;


/** 
 * @brief TML false definition (0)
 * @ingroup tmlConstants
 */
#define TML_FALSE             SIDEX_FALSE


/** 
 * @brief TML true definition (1)
 * @ingroup tmlConstants
 */
#define TML_TRUE              SIDEX_TRUE


/**
* @brief    TML signed 32 bit int type
* @ingroup tmlTypes
*/
typedef SIDEX_INT32           TML_INT32;


/**
* @brief    TML unsigned 32 bit int type
* @ingroup tmlTypes
*/
typedef SIDEX_UINT32          TML_UINT32;


/**
* @brief    TML signed 64 bit int type
* @ingroup tmlTypes
*/
typedef SIDEX_INT64           TML_INT64;


/**
* @brief    TML unsigned 64 bit int type
* @ingroup tmlTypes
*/
typedef SIDEX_UINT64          TML_UINT64;


/**
* @brief    TML double type (float)
* @ingroup tmlTypes
*/
typedef SIDEX_DOUBLE            TML_DOUBLE;


/**
* @brief    TML pointer type
* @ingroup tmlTypes
*/
typedef void*                   TML_POINTER;

/**
 * @brief   Callback function to handle TML status replies.
 * @ingroup tmlCBFunctions
 *
 * @param   tmlhandle TML core handle (TML_COMMAND_HANDLE)
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 * @param   iType     reply type
 * @param   sMsg      reply message
 *
 * @see tml_Cmd_Register_StatusReply(), tml_Send_AsyncStatusReply()
 */
typedef  void (*TML_ON_STATUS_REPLY_CB_FUNC)(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData, TML_INT32 iType, TML_CTSTR*  sMsg);


/**
 * @brief   Callback function to handle TML progress replies.
 * @ingroup tmlCBFunctions
 *
 * @param   tmlhandle TML core handle (TML_COMMAND_HANDLE)
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 * @param   iProgress progress value (0-100)
 *
 * @see tml_Send_AsyncProgressReply(), tml_Cmd_Register_Progress()
 */
typedef  void (*TML_ON_PROGRESS_REPLY_CB_FUNC)(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData, TML_INT32 iProgress);


/**
 * @brief   TML command error handler.
 * @ingroup tmlCBFunctions
 *
 * @param   sProfile   profile name
 * @param   sHost      host name or IP address
 * @param   sPort      port number
 * @param   iCMD       command id
 * @param   iErrorCode error code
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @see tml_Evt_Set_OnError()
 */
typedef  void (*TML_ON_EVT_ERROR_CB_FUNC)(TML_CTSTR* sProfile, TML_CTSTR* sHost, TML_CTSTR* sPort, TML_COMMAND_ID_TYPE iCMD, TML_INT32 iErrorCode, TML_POINTER pCBData);


/**
 * @brief   Event queue overflow handler.
 * @ingroup tmlCBFunctions
 *
 * @param   sProfile  profile name
 * @param   iCMD      command/event id
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 *
 * @see tml_Evt_Set_OnQueueOverflow()
 */
typedef  void (*TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC)(TML_CTSTR* sProfile, TML_COMMAND_ID_TYPE iCMD, TML_POINTER pCBData);


/**
 * @brief   Callback function to populate event consumer or command receiver lists.
 * @ingroup tmlCBFunctions
 *
 * @param   sProfile  profile name
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS on success or error
 *
 * @see tml_Bal_Set_OnPopulate(), tml_Evt_Set_OnPopulate()
 */
typedef  TML_INT32 (*TML_ON_POPULATE_CB_FUNC)(TML_CTSTR* sProfile, TML_POINTER pCBData);


/**
 * @brief   Subscription / unsubscription request handler
 * @ingroup tmlCBFunctions
 *
 * @param   bSubscribe TML_TRUE = subscribe, TML_FALSE = unsubscribe
 * @param   sHost      host name or ip address
 * @param   sPort      port number
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_TRUE on subscription / unsubscription or TML_FALSE
 *
 * @see tml_Evt_Send_SubscriptionRequest(), tml_Evt_Send_UnsubscriptionRequest(), tml_Evt_Set_OnPeerRegister(),tml_Bal_Send_SubscriptionRequest(), tml_Bal_Send_UnsubscriptionRequest(), tml_Bal_Set_OnPeerRegister()
 */
typedef  TML_BOOL (*TML_ON_PEER_REGISTRATION_CB_FUNC)(TML_BOOL bSubscribe, TML_CTSTR* sHost, TML_CTSTR* sPort, TML_POINTER pCBData);


/**
 * @brief   TML command handler.
 * @ingroup tmlCBFunctions
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 *
 * @see tml_Profile_Register_Cmd()
 */
typedef  void (*TML_ON_CMD_DISPATCH_CB_FUNC)(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData);


/**
 * @brief   Callback function to signal a new connection.
 * @ingroup tmlCBFunctions
 *
 * @param   connectionHandle  TML connection handle (TML_CONNECTION_HANDLE)
 * @param   pCBData   custom data from handler registration (tml_Core_Set_OnConnect()) or TML_HANDLE_TYPE_NULL
 *
 * @see tml_Core_Set_OnConnect()
 */
typedef  void (*TML_ON_CONNECT_CB_FUNC)(TML_CONNECTION_HANDLE connectionHandle, TML_POINTER pCBData);


/**
 * @brief   Callback function to signal a closed connection.
 * @ingroup tmlCBFunctions
 *
 * @param   connectionHandle  TML connection handle (TML_CONNECTION_HANDLE)
 * @param   pCBData   custom data from handler registration (tml_Core_Set_OnDisconnect()) or TML_HANDLE_TYPE_NULL
 *
 * @see tml_Core_Set_OnDisconnect()
 */
typedef  void (*TML_ON_DISCONNECT_CB_FUNC)(TML_CONNECTION_HANDLE connectionHandle, TML_POINTER pCBData);


/**
 * @brief   Handler for command deregistration.
 * @ingroup tmlCBFunctions
 *
 * @param   iCMD      command id
 * @param   pCmdData  custom data from command registration (tml_Profile_Register_Cmd()) or TML_HANDLE_TYPE_NULL
 * @param   pCBData   custom data from handler registration (tml_Profile_Set_OnDeleteCmd()) or TML_HANDLE_TYPE_NULL
 *
 * @see tml_Profile_Set_OnDeleteCmd(), tml_Profile_Register_Cmd()
 */
typedef  void (*TML_ON_CMD_DELETION_CB_FUNC)(TML_COMMAND_ID_TYPE iCMD, TML_POINTER pCmdData, TML_POINTER pCBData);


/**
 * @brief   General command handler.
 * @ingroup tmlCBFunctions
 *
 * A general command handler is called for any command id that has no specific command handler.
 *
 * @param   iCMD      command id
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 *
 * @see tml_Profile_Set_OnCustomDispatch()
 */
typedef  void (*TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC)(TML_COMMAND_ID_TYPE iCMD, TML_COMMAND_HANDLE cmdhandle, TML_POINTER pCBData);


/**
 * @brief   Load balancing busy request handler.
 * @ingroup tmlCBFunctions
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS or error code
 *
 * @see tml_Bal_Set_OnBusyStatusRequest()
 */
typedef  TML_INT32 (*TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC)(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData);


/**
 * @brief   Calculate next load balancing command receiver.
 * @ingroup tmlCBFunctions
 *
 * @param   iCountOfDestinations  command receiver count
 * @param   listenerBusyStateArray  array of TML commands (TML_COMMAND_HANDLE) with load (busy) requests from each command receiver
 * @param   pCBData                 custom data or TML_HANDLE_TYPE_NULL
 * @param   iNextListenerIndex      reference to next command receiver index
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED cannot calculate index (fallback to round robin)<br>
 *          or any other error code
 *
 * @see tml_Bal_Set_OnCalculation(), tml_Bal_Send_SyncMessage(), tml_Bal_Send_AsyncMessage()
 */
typedef  TML_INT32 (*TML_ON_BAL_CALCULATION_REQ_CB_FUNC)(TML_INT32 iCountOfDestinations, TML_COMMAND_HANDLE* listenerBusyStateArray, TML_POINTER pCBData, TML_INT32* iNextListenerIndex);


/**
 * @brief   Command ready handler.
 * @ingroup tmlCBFunctions
 *
 * The command ready handler is called to handle the return value of asynchronous commands.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 *
 * @see tml_Bal_Send_AsyncMessage(), tml_Send_AsyncMessage(), tml_Cmd_Register_CommandReady()
 */
typedef  void (*TML_ON_COMMAND_READY_CB_FUNC)(TML_COMMAND_HANDLE tmlhandle, TML_POINTER pCBData);


/**
 * @brief   Download stream processing handler.
 * @ingroup tmlCBFunctions
 *
 * @param   iID            clear stream identifiation
 * @param   pCBDataDld     buffer reception handler data
 * @param   buffer         data buffer
 * @param   bytesRead      number of bytes to read
 * @param   totalBytesRead number of bytes really read
 * @param   streamSize     size of the stream
 *
 * @returns  0 on success / -1 if not operable, / -2 if operation was canceled
 *
 * @see tml_RecStream_DownloadData()
 */
typedef  TML_INT32 (*TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_POINTER pCBDataDld, TML_POINTER buffer, TML_INT32 bytesRead, TML_INT64 totalBytesRead, TML_INT64 streamSize);


/**
 * @brief   Download stream finished handler.
 * @ingroup tmlCBFunctions
 *
 * @param   iID              clear stream identifiation
 * @param   errCode          TML_SUCCESS on success or error code
 * @param   pCBDataDldFinish download finished handler data
 *
 * @see tmlErrors.h, tml_RecStream_DownloadData()
 */
typedef  void (*TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_INT32 errCode, TML_POINTER pCBDataDldFinish);


/**
 * @brief   Get stream size handler.
 * @ingroup tmlCBFunctions
 *
 * @param   iID       clear stream identifiation
 * @param   pCBData   custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns size of the stream
 *
 * @see tml_SndStream_Register_GetSize() 
 */
typedef  TML_INT64 (*TML_ON_SND_STRM_GET_SIZE_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_POINTER pCBData);


/**
 * @brief   Seek stream handler.
 * @ingroup tmlCBFunctions
 *
 * @param iID          clear stream identifiation
 * @param pCBData      custom data or TML_HANDLE_TYPE_NULL
 * @param seekPosition seek position
 * @param seekOrigin   requested seek origin
 *
 * @returns  0 on success / -1 on error
 *
 * @see tml_SndStream_Register_Seek() 
 */
typedef  TML_INT32 (*TML_ON_SND_STRM_SEEK_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_POINTER pCBData, TML_INT64 seekPosition, tmlSeekOrigin seekOrigin);


/**
 * @brief   Get stream position handler.
 * @ingroup tmlCBFunctions
 *
 * @param   iID        clear stream identifiation
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns current stream position
 *
 * @see tml_SndStream_Register_GetPosition()
 */
typedef  TML_INT64 (*TML_ON_SND_STRM_GET_POSITION_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_POINTER pCBData);


/**
 * @brief   Stream read handler.
 * @ingroup tmlCBFunctions
 *
 * @param   iID        clear stream identifiation
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 * @param   buffer     data buffer
 * @param   count      number of bytes to read
 * @param   bytesRead  reference to the number of read bytes
 *
 * @returns  0 on success / -1 on error
 *
 * @see tml_SndStream_Register_Read()
 */
typedef  TML_INT32 (*TML_ON_SND_STRM_READ_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_POINTER pCBData, TML_POINTER buffer, TML_INT32 count, TML_INT32* bytesRead);


/**
 * @brief   Stream write handler.
 * @ingroup tmlCBFunctions
 *
 * @param   iID        clear stream identifiation
 * @param   buffer     data buffer
 * @param   count      number of bytes to write
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns  0 on success / -1 on error
 *
 * @see tml_SndStream_Register_Write()
 */
typedef  TML_INT32 (*TML_ON_SND_STRM_WRITE_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count, TML_POINTER pCBData);


/**
 * @brief   Stream close handler.
 * @ingroup tmlCBFunctions
 *
 * @param   iID        clear stream identifiation
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @see tml_SndStream_Register_Close() 
 */
typedef  void (*TML_ON_SND_STRM_CLOSE_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_POINTER pCBData);


/**
 * @brief   Stream error handler.
 * @ingroup tmlCBFunctions
 *
 * @param   iID        clear stream identifiation
 * @param   iError     error code
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @see tml_SndStream_Register_OnError()
 */
typedef  void (*TML_ON_SND_STRM_COM_FAULT_CB_FUNC)(TML_STREAM_ID_TYPE iID, TML_INT32 iError, TML_POINTER pCBData);


#endif // TMLSTDTYPES_H
