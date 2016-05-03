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
 
#include <sidex.h>
#include "tmlCore.h"
#include "tmlCoreListener.h"
#include "tmlCmdDefines.h"
#include "tmlCoreDefines.h"
#include "tmlGlobalCallback.h"
#include "tmlErrors.h"
#include "logValues.h"
#include "tmlObjWrapper.h"
#include "tmlCoreWrapper.h"

/*********************************************************************************************************************************
*                                             "C" / Global methods / Callbacks & Threads
*********************************************************************************************************************************/

/**
 * @brief  a frame received Callback
 */
void listenerFrameReceivedCallback (VortexChannel* channel,
                                     VortexConnection* connection,
                                     VortexFrame* frame,
                                     axlPointer user_data)
{

  VORTEXListenerFrameReceivedCallbackData* callbackData = (VORTEXListenerFrameReceivedCallbackData*) user_data;

  TML_INT32 iRet = TML_SUCCESS;
  /////////////////////////////////////////////////////////////////////////////
  // Fetching all necessary attributes from Vortex:


  ///////////////////////////
  // Die Connection ID:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_connection_get_id");
  int iConnectionID = vortex_connection_get_id(connection);
  ///////////////////////////
  // Die Host IP:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_connection_get_host_ip");
  const char* sHostIP = vortex_connection_get_host_ip(connection);
  ///////////////////////////
  // Der Port:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_connection_get_local_port");
  const char* sPort = vortex_connection_get_local_port(connection);
  ///////////////////////////
  // Die Channel ID:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_channel_get_number");
  int iChannelID = vortex_channel_get_number (channel);
  if (-1 == iChannelID)
    iRet = TML_ERR_LISTENER_COMMUNICATION;
  ///////////////////////////
  // Die Message ID:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_msgno");
  int iMsgID = vortex_frame_get_msgno (frame);
  if (-1 == iMsgID)
    iRet = TML_ERR_LISTENER_COMMUNICATION;
  ///////////////////////////
  // Das Profil, mit dem der Sender conneted ist:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_channel_get_profile");
  const char* sProfile = vortex_channel_get_profile (channel);
  if (NULL == sProfile)
    iRet = TML_ERR_LISTENER_COMMUNICATION;
  ///////////////////////////
  // Das Payload:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_payload");
  char* cPayload = (char*)vortex_frame_get_payload (frame);
  if (NULL == cPayload)
    iRet = TML_ERR_LISTENER_COMMUNICATION;
  ///////////////////////////
  // Die Payloadsize:
  callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_frame_get_payload_size");
  int iPayloadSize = vortex_frame_get_payload_size (frame);
  if (-1 == iPayloadSize)
    iRet = TML_ERR_LISTENER_COMMUNICATION;


  /////////////////////////////////////////////////////////////////////////////
  // Set all necessary Vortex attributes into the TML_COMMAND_HANDLE Obj:
  if (TML_SUCCESS == iRet){
    // A new TML_COMMAND_HANDLE object out of the payload:
    TML_COMMAND_HANDLE cmdHandle;
    iRet = tml_Cmd_Create(&cmdHandle);
    if (TML_SUCCESS == iRet){
      SIDEX_HANDLE sHandle;
      ///////////////////////////////////////////////
      // Acquire critical section use: 
      tml_Cmd_Acquire_Sidex_Handle(cmdHandle, &sHandle);
      iRet = sidex_Set_Content(sHandle, cPayload);
      ///////////////////////////////////////////////
      // Now I can release the critical section use: 
      tml_Cmd_Release_Sidex_Handle(cmdHandle);
    }
    if (TML_SUCCESS == iRet){
      // Vortex- session ID into the TML_COMMAND_HANDLE object:
      tml_Cmd_Attr_Set_Session_ID(cmdHandle, iConnectionID); // Don't mind of return value
      // Vortex- channel ID into the TML_COMMAND_HANDLE object:
      tml_Cmd_Attr_Set_Channel_ID(cmdHandle, iChannelID); // Don't mind of return value
      callbackData->pLog->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "listenerFrameReceivedCallback", "Vortex CMD", "vortex_channel_ref");
      vortex_channel_ref(channel); // I will be shure to have a valid channel until the reply is send
      // Vortex- channel into the TML_COMMAND_HANDLE object:
      tml_Cmd_Attr_Set_Channel(cmdHandle, channel); // Don't mind of return value
      // Vortex- message ID into the TML_COMMAND_HANDLE object:
      tml_Cmd_Attr_Set_Message_ID(cmdHandle, iMsgID); // Don't mind of return value
      // Vortex- profile into the TML_COMMAND_HANDLE object:
      tml_Cmd_Attr_Set_Profile(cmdHandle, sProfile);
      // The core handle:
      tml_Cmd_Attr_Set_Core_Reference(cmdHandle, callbackData->tmlcorehandle); // Don't mind of return value

      TML_CONNECTION_HANDLE connectionHandle = TML_HANDLE_TYPE_NULL;
      //////////////////////////////////////////////////////////////////
      // Add the connection to the list:
      ((tmlCoreWrapper*)callbackData->tmlcorehandle)->tmlCoreWrapper_Connect(sHostIP, sPort, false, &connectionHandle, connection);
      ((tmlObjWrapper*)cmdHandle)->tmlObjWrapper_Set_Connection(connectionHandle);

      // Now call the callback method:
      globalCallback(callbackData->callback, (void*) cmdHandle);
    }
  }
  else{
    callbackData->pLog->log ("TMLCoreListener", "listenerFrameReceivedCallback", "ERROR", "Problem on getting some attributes.");
  }
  return;
}


/*********************************************************************************************************************************
*                                             Class definitions:
*********************************************************************************************************************************/


/*********************************************************************************************************************************
*                                                    Private methods
*********************************************************************************************************************************/

/**
 * @brief    Send the anser referring a pending message.
 */
int TMLCoreListener::SendRawAnswerReplyFromFeeder (TML_COMMAND_HANDLE tmlCommand, VortexPayloadFeeder* feeder)
{
  TML_INT32 iRet = TML_SUCCESS;
  /////////////////////////////////////////
  // No input error, so try to make the work:
  VortexChannel* channel = NULL;
  iRet = tml_Cmd_Attr_Get_Channel(tmlCommand, (void**)&channel);
  if (NULL != channel){
    /////////////////////////////////////////////////
    // The channel must not be referenced any longer:
    m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "SendRawAnswerReplyFromFeeder", "Vortex CMD", "vortex_channel_unref");
    vortex_channel_unref(channel);
  }
  if (TML_SUCCESS == iRet){
    TML_INT32 iMsgID;
    iRet = tml_Cmd_Attr_Get_Message_ID(tmlCommand, &iMsgID);
    if (TML_SUCCESS == iRet){
      m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "SendRawAnswerReplyFromFeeder", "Vortex CMD", "vortex_channel_send_rpy_from_feeder");
      if (axl_false == vortex_channel_send_rpy_from_feeder (channel, feeder, iMsgID)){
        iRet = TML_ERR_LISTENER_COMMUNICATION;
      }
    }
    else{
      iRet = TML_ERR_LISTENER_COMMUNICATION;
    }
  }
  return (int)iRet;
}


/**
 * @brief    Send a single reply to a pending message.
 */
int TMLCoreListener::SendReply (VortexChannel* channel, int iMsgID, char* data, int iDataLength)
{
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "SendReply", "Vortex CMD", "vortex_channel_send_rpy");
  if (axl_false == vortex_channel_send_rpy (channel, data, iDataLength, iMsgID)){
    iRet = TML_ERR_LISTENER_COMMUNICATION;
  }
  return iRet;
}


/**
 * @brief    Send one of multiple replys to a pending message.
 */
int TMLCoreListener::SendAnsReply(VortexChannel* channel, int iMsgID, char* data, int iDataLength)
{
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "SendAnsReply", "Vortex CMD", "vortex_channel_send_ans_rpy");
  if (axl_false == vortex_channel_send_ans_rpy (channel, data, iDataLength, iMsgID)){
    iRet = TML_ERR_LISTENER_COMMUNICATION;
    m_log->log (TML_LOG_MULTI_SYNC_CMDS, "TMLCoreListener", "SendAnsReply", "Communication error using", "vortex_channel_send_ans_rpy");
  }
  return iRet;
}


/**
 * @brief    Send the end of multiple replys to a pending message.
 */
int TMLCoreListener::SendFinalAnsReply(VortexChannel* channel, int iMsgID)
{
  int iRet = TML_SUCCESS;

  // DEBUG_QVENT_QUEUE */ m_log->log ("TMLCoreListener", "SendFinalAnsReply", "Vortex CMD vortex_channel_finalize_ans_rpy No ", iMsgID);
  m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "SendFinalAnsReply", "Vortex CMD", "vortex_channel_finalize_ans_rpy");
  if (m_log->getLoggingValue() & TML_LOG_MULTI_SYNC_CMDS){
    m_log->log (TML_LOG_MULTI_SYNC_CMDS, "TMLCoreListener", "SendFinalAnsReply", "MessageCounter", m_iMultiSyncMessageCounter);
    ++m_iMultiSyncMessageCounter;
  }
  if (axl_false == vortex_channel_finalize_ans_rpy (channel, iMsgID)){
    iRet = TML_ERR_LISTENER_COMMUNICATION;
    m_log->log (TML_LOG_MULTI_SYNC_CMDS, "TMLCoreListener", "SendAnsReply", "Communication error using", "vortex_channel_finalize_ans_rpy");
  }
  return iRet;
}


/**
 * @brief    Send an error reply to a pending message to terminate the message communication.
 */
int TMLCoreListener::SendError(VortexChannel* channel, int iMsgID, char* data, int iDataLength)
{
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "SendError", "Vortex CMD", "vortex_channel_send_err");
  if (axl_false == vortex_channel_send_err (channel, data, iDataLength, iMsgID)){
    iRet = TML_ERR_LISTENER_COMMUNICATION;
  }
  return iRet;
}


/*********************************************************************************************************************************
*                                                     Public methods
*********************************************************************************************************************************/


/**
 * @brief    Constructor.
 */
TMLCoreListener::TMLCoreListener(TML_CORE_HANDLE tmlcorehandle, tmlLogHandler* loghandler, VortexCtx* ctx, tmlProfileHandler* pHandler, void* callback)
{
  m_coreHandle = tmlcorehandle;
  m_log = loghandler;

  m_pHandler = pHandler;

  /////////////////////////////////////
  // Only one Vortex execution context:
  m_ctx = ctx;
  m_hValidListenerThread = false;
  m_iMultiSyncMessageCounter = 0;
  m_iLogFileIndex = 0;
  m_channelsPerConnectionLimitCheckData.iMax = -1;
  m_channelsPerConnectionLimitCheckData.pLog = loghandler;
  m_callbackData.callback = callback;
  m_callbackData.pLog = m_log;
  m_callbackData.tmlcorehandle = tmlcorehandle;
}


/**
 * @brief    Destructor.
 */
TMLCoreListener::~TMLCoreListener(void)
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Stop the listener first:
  TMLCoreListener_Stop();
  /////////////////////////////////////
  // Only one Vortex execution context:
  // (( so put the following into comments ))

/*  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Then exit the execution context:
  if (NULL != m_ctx){
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // end vortex function / this will unblock the vortex_listener_wait(..) in the LisenerThread to end the thread:
    m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "StopVortexListenerThread", "Vortex CMD", "vortex_exit_ctx");
    vortex_exit_ctx (m_ctx, axl_true);
    m_ctx = NULL;
  }
*/  
}


/**
 * @brief    Send the anser referring a pending message.
 */
int TMLCoreListener::MessageAnswer (TML_COMMAND_HANDLE tmlCommand, bool bRawReply, int iErr)
{
  TML_INT32 iRet = TML_SUCCESS;

  TML_INT32 iDataLength = 0;
  char* cData = NULL;

  VortexChannel* channel;
  iRet = tml_Cmd_Attr_Get_Channel(tmlCommand, (void**)&channel);
  if (TML_SUCCESS == iRet){
    TML_INT32 iMsgID;
    iRet = tml_Cmd_Attr_Get_Message_ID(tmlCommand, &iMsgID);
    if (TML_SUCCESS == iRet){
      if (bRawReply){
        ////////////////////////////////////////////////////////////////////
        // If iErr is TML_SUCCESS there is valid data:
        if (TML_SUCCESS == iErr){
          //////////////////////////////////////////////////////////////////
          // I have to write raw data:
          ///////////////////////////////////////////////
          // Acquire critical section use: 
          SIDEX_HANDLE sHandle;
          tml_Cmd_Acquire_Sidex_Handle(tmlCommand, &sHandle);
          TML_INT64 bytesRead;
          TML_INT64 addr;
          //////////////////////////////////////////////////////////////////
          // The number of bytes to send:
          if (TML_SUCCESS == iRet){
            iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_BYTRES_READ, &bytesRead);
          }
          //////////////////////////////////////////////////////////////////
          // The address with the data:
          if (TML_SUCCESS == iRet){
            iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_READ_BUFFER_ADDR, &addr);
          }
          ///////////////////////////////////////////////
          // Now I can release the critical section use: 
          tml_Cmd_Release_Sidex_Handle(tmlCommand);
          if (TML_SUCCESS == iRet){
            cData = (char*)addr;
            iDataLength = (int) bytesRead;
          }
        }
        else{
          /////////////////////////////////////////////////////////////////////////////
          // If iErr isn't TML_SUCCESS the iDataLength must be 0 to signal the problem:
          cData = NULL;
          iDataLength = 0;
        }
        iRet = (TML_INT32)MessageAnswer (channel, (int)iMsgID, cData, (int)iDataLength);
      }
      else{
        //////////////////////////////////////////////////////////////////
        // I have to write a SIDEX content:
        ///////////////////////////////////////////////
        // Acquire critical section use: 
        SIDEX_HANDLE sHandle;
        tml_Cmd_Acquire_Sidex_Handle(tmlCommand, &sHandle);
        iRet = sidex_Get_Content(sHandle, &cData, &iDataLength);
        //if (TML_SUCCESS == iRet){
          /////////////////////////////
          // To test the SIDEX content:
          //printf ("\n\n\n%s\n\n\n", cData);
          //sidex_Save_Content  (sHandle, "c:\\temp\\answer.xml");
        //}
        ///////////////////////////////////////////////
        // Now I can release the critical section use: 
        tml_Cmd_Release_Sidex_Handle(tmlCommand);
        if (TML_SUCCESS == iRet){
          iRet = (TML_INT32)MessageAnswer (channel, (int)iMsgID, cData, (int)iDataLength);
          sidex_Free_Content(cData);
        }
        else{
          iRet = TML_ERR_LISTENER_COMMUNICATION;
        }
      }
    }
    else{
      iRet = TML_ERR_LISTENER_COMMUNICATION;
    }
  }
  return (int)iRet;
}


/**
 * @brief    Send the anser referring a pending message.
 */
int TMLCoreListener::MessageAnswer (VortexChannel* channel, int iMsgID, char* cData, int iDataLength)
{
  int iRet = TML_SUCCESS;
  // It is necessary to send only one "SendAnsReply" that contains the whole data.
  iRet = SendAnsReply (channel, iMsgID, cData, iDataLength);
  if (TML_SUCCESS == iRet){
    // ATTENTION: async. message reply (progress or status) - see also class tmlCoreSender
    // The "SendFinalAnsReply" will be done only once in the "MessageFinalize()"
    // In other case there are Vortex problems of synchronisation when each
    // async. message reply (progress or status) will also send it's SendFinalAnsReply():

    //iRet = SendFinalAnsReply (channel, iMsgID);
  }
  if (TML_SUCCESS != iRet){
    m_log->log ("TMLCoreListener", "MessageAnswer", "INFO", "Communication error / maybe the channel is being closed by the client");
  }
  return iRet;
}


/**
 * @brief    Send the final answer
 */
int TMLCoreListener::MessageFinalize (TML_COMMAND_HANDLE tmlCommand)
{
  TML_INT32 iRet;
  VortexChannel* channel = NULL;
  TML_INT32 iMsgID;

  iRet = tml_Cmd_Attr_Get_Channel(tmlCommand, (void**)&channel);
  if (TML_SUCCESS == iRet){
    iRet = tml_Cmd_Attr_Get_Message_ID(tmlCommand, &iMsgID);
  }
  if (TML_SUCCESS == iRet){
    // ATTENTION: async. message reply (progress or status) - see also class tmlCoreSender
    // The "SendFinalAnsReply" will be done only once here - see MessageAnswer():
    iRet = SendFinalAnsReply (channel, (int)iMsgID);
  }
  if (TML_SUCCESS == iRet){
    if (NULL != channel){
      /////////////////////////////////////////////////
      // The channel must not be referenced any longer:
      m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "MessageFinalize", "Vortex CMD", "vortex_channel_unref");
      vortex_channel_unref(channel);
    }
  }
  else{
    iRet = TML_ERR_LISTENER_COMMUNICATION;
    m_log->log ("TMLCoreListener", "MessageFinalize", "INFO", "Communication error / maybe the channel is being closed");
  }
  return (int)iRet;
}


/**
 * @brief    Register a profile
 */
int TMLCoreListener::TMLCoreListener_RegisterProfile(const char* profile) 
{
  int iRet = TML_SUCCESS;
  if (NULL != m_ctx){
        bool bRegisterProfile = false;
        bool bRegisterCB = false;
        m_pHandler->tmlProfileRegister(profile, true, (void*)listenerFrameReceivedCallback, &m_callbackData, &bRegisterProfile, &bRegisterCB);

        int bOk;
        if (bRegisterProfile){
          m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "TMLCoreListener_RegisterProfile", "Vortex CMD", "vortex_profiles_register");
          bOk = vortex_profiles_register (m_ctx, profile, 
                                          NULL, NULL,
                                          NULL, NULL,
                                          NULL, NULL);

          if (axl_false == bOk){
            m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "Profile registration of profile", profile, " failed !");
            iRet = TML_ERR_LISTENER_COMMUNICATION;
          }
          else{
            m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "Profile registration of profile", profile, " succeeded");
          }
        }
        if (TML_SUCCESS == iRet){
          if (bRegisterCB){
            m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "TMLCoreListener_RegisterProfile", "Vortex CMD", "vortex_profiles_set_received_handler");
            bOk = vortex_profiles_set_received_handler (m_ctx, profile, 
                                          listenerFrameReceivedCallback, &m_callbackData);
            if (axl_false == bOk){
              iRet = TML_ERR_LISTENER_COMMUNICATION;
            }
          }
        }
  }
  else
  {
    iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
  }
  return iRet;
}


/**
 * @brief    Unregister a profile
 */
int TMLCoreListener::TMLCoreListener_UnregisterProfile(const char* profile) 
{
  int iRet = TML_SUCCESS;
  if (NULL != m_ctx){
    bool bUnegisterProfile = true;
    bool bUnegisterCallback = true;
    m_pHandler->tmlProfileUnregister(profile);
    int bOk;
    if (bUnegisterCallback)
    {
        m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "TMLCoreListener_UnregisterProfile", "Vortex CMD", "vortex_profiles_set_received_handler");
        bOk = vortex_profiles_set_received_handler (m_ctx, profile, NULL, NULL);
        if (axl_false == bOk){
          iRet = TML_ERR_LISTENER_COMMUNICATION;
        }
    }
    if (bUnegisterProfile)
    {
        m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "TMLCoreListener_UnregisterProfile", "Vortex CMD", "vortex_profiles_unregister");
        bOk = vortex_profiles_unregister (m_ctx, profile);
        if (axl_true == bOk){
           m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "Profile deregistration of profile", profile, " succeeded");
        }
        else{
          m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "Profile deregistration of profile", profile, " failed !");
          iRet = TML_ERR_LISTENER_COMMUNICATION;
        }
    }
  }
  else
  {
    iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
  }
  return iRet;
}

/**
 * @brief   Set the vortex debug logging value.
 */
int TMLCoreListener::TMLCoreListener_Set_Vortex_Logging_Value(int iLogValue)
{
  int iRet = TML_SUCCESS;

  if (NULL == m_ctx){
    iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
  }
  return iRet;
}


/**
 * @brief    Start the TMLCoreListener.
 */
int TMLCoreListener::TMLCoreListener_Start(const char* host, const char*port, const char** resPort) 
{
  int iRet = TML_SUCCESS;

//tml_log(0xFFFFFFFF, "tmlCoreWrapper", "~tmlCoreWrapper", "POS", "Start-a");

  if (!m_hValidListenerThread){
    axl_bool bSuccess = axl_true;
    if (NULL == m_ctx){
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // create vortex execution context because the listener needs it's own
      // A reference from the tmlCoreWrapper don't work because the listener and the sender must work on the same platform
      m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "TMLCoreListener_Start", "Vortex CMD", "vortex_ctx_new");
      m_ctx = vortex_ctx_new ();
      /////////////////////////////////////////////////////////////
      // init vortex library /  SIDEX-72 has only to be done once
      m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "TMLCoreListener_Start", "Vortex CMD", "vortex_init_ctx");
      bSuccess = vortex_init_ctx (m_ctx);
      if (axl_true != bSuccess){
        m_ctx = NULL;
        m_log->log ("TMLCoreListener", "TMLCoreListener;TMLCoreListener_Start", "vortex_init_ctx", "FAILLED");
        iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
      }
    }
    tmlListenerObj* listenerMngObj = NULL;
    TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
    TML_BOOL bHasAnyListener = TML_FALSE;
    if (TML_SUCCESS == iRet){
      bHasAnyListener = ((tmlCoreWrapper*)m_coreHandle)->tmlCoreWrapper_Has_Any_Listener();
      if (!bHasAnyListener){
        iRet = ((tmlCoreWrapper*)m_coreHandle)->tmlCoreWrapper_Listener_Create(host, port, &listenerHandle);
      }
      else{
        iRet = ((tmlCoreWrapper*)m_coreHandle)->tmlCoreWrapper_Get_Listener(0, &listenerHandle);
      }
      if (TML_SUCCESS == iRet){
        listenerMngObj = (tmlListenerObj*) listenerHandle;
        iRet = listenerMngObj->set_Enabled(TML_TRUE);
        if (TML_SUCCESS != iRet && !bHasAnyListener){
          ((tmlCoreWrapper*)m_coreHandle)->tmlCoreWrapper_Listener_Close(&listenerHandle);
        }
      }
    }
    if (TML_SUCCESS == iRet){
      //////////////////////////////////////////////////////////////
      // in case of port equals 0 the vortex_listener_new will find
      // the next free port, so I want to know it's identification:
      m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "TMLCoreListener_Start", "Vortex CMD", "vortex_connection_get_port");
      iRet = listenerMngObj->getPort((char**)resPort);
      if (TML_SUCCESS != iRet){
        if (!bHasAnyListener){
          ((tmlCoreWrapper*)m_coreHandle)->tmlCoreWrapper_Listener_Close(&listenerHandle);
        }
        iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
      }
    }
    if (TML_SUCCESS == iRet){
      m_hValidListenerThread = true;
    }
  }
  else{
    // Error / context already exists
    iRet = TML_ERR_LISTENER_ALREADY_STARTED;
  }
  return iRet;
}


/**
 * @brief    Stop the internal TMLCoreListener.
 */
int TMLCoreListener::TMLCoreListener_Stop() 
{
  int iRet = TML_SUCCESS;

  if (m_hValidListenerThread)
  {
/*
  if (NULL != m_ctx){
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // end vortex function / this will unblock the vortex_listener_wait(..) in the LisenerThread to end the thread:
    m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "StopVortexListenerThread", "Vortex CMD", "vortex_exit_ctx");
    vortex_exit_ctx (m_ctx, axl_true);
    m_ctx = NULL;
  }
*/
    m_log->log (TML_LOG_VORTEX_CMD, "TMLCoreListener", "TMLCoreListener_Stop", "Vortex CMD", "vortex_listener_set_on_connection_accepted");
    vortex_listener_set_on_connection_accepted (m_ctx, NULL, NULL);

    /////////////////////////////////////
    // Destroy allocated handle:
    m_hValidListenerThread = false;
  }
  else{
     iRet = TML_ERR_LISTENER_NOT_INITIALIZED;
  }
  return iRet;
}


/**
 * @brief   Set the log- file index for explicit logs with closing the file
 */
void TMLCoreListener::setLogFileIndex(int iLogFileIndex){
  m_iLogFileIndex = iLogFileIndex;
}

