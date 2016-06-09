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

#ifndef TMLCORELISTENER_H
#define TMLCORELISTENER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlCoreDefines.h"
#include "tmlStdTypes.h"
#include "tmlLogHandler.h"
#include "tmlStreamHandler.h"
#include "tmlProfileHandler.h"
#include "tmlListenerObj.h"
#include <vortex.h>


//////////////////////////////////////////////////////////////////////////////
// C - declarations:
extern "C" {

/**
 * @brief  a frame received Callback
 *
 * @param   channel    Reference to VortexChannel of the received frame source.
 * @param   connection Reference to VortexConnection of the received frame source.
 * @param   frame      Reference to received VortexFrame.
 * @param   user_data  Reference to user data registered on Vortex API vortex_profiles_register().
 */
void listenerFrameReceivedCallback (VortexChannel* channel,
                                     VortexConnection* connection,
                                     VortexFrame* frame,
                                     axlPointer user_data);


/**
 * @brief  Callback of a start of a channel
 *
 * @param   channel_num  Channel number of the started channel.
 * @param   connection   Reference to VortexConnection of the started channel.
 * @param   user_data    Reference to user data registered on Vortex API vortex_profiles_register().
 */
int startChannelCallback (int channel_num, 
                          VortexConnection* connection, 
                          axlPointer  user_data);

/**
 * @brief  Callback of a close of a channel
 *
 * @param   channel_num  Channel number of the closed channel.
 * @param   connection   Reference to VortexConnection of the closed channel.
 * @param   user_data    Reference to user data registered on Vortex API vortex_profiles_register().
 */
int closeChannelCallback (int channel_num, 
                          VortexConnection * connection, 
                          axlPointer user_data);


/**
 * @brief  callback in case of the accept the connection
 *
 * @param   connection   Reference to VortexConnection to accept.
 * @param   user_data    Reference to user data registered on Vortex API vortex_listener_set_on_connection_accepted().
 */
int onAcceptedCallback (VortexConnection * connection, axlPointer user_data);


}// extern "C"


class TMLCoreListener
{
  public:
    // Members:

    /**
     * @brief    Flag shows the start status
     */
    int m_iMultiSyncMessageCounter;

    /**
     * @brief   Log- File index for explicit logs with closing the file
     */
    int m_iLogFileIndex;

    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;

    // Methods:


    /**
     * @brief    Constructor.
     *
     * @param   tmlcorehandle Reference to the core handle.
     * @param   loghandler    Reference to a log handler.
     * @param   ctx           Reference Vortex execution context.
     * @param   pHandler      Reference to tmlProfileHandler.
     * @param   callback      Reference to callback method to handle the frame receive payload to.
     *
     * @returns an instance of TMLCoreListener
     */
    TMLCoreListener(TML_CORE_HANDLE tmlcorehandle, tmlLogHandler* loghandler, VortexCtx* ctx, tmlProfileHandler* pHandler, void* callback);


    /**
     * @brief    Destructor.
     */
    ~TMLCoreListener(void);


    /**
     * @brief    Send the anser referring a pending message.
     *
     * @param   tmlCommand   An instance of TML_COMMAND_HANDLE containing all necessary data and attributes for the answer.
     * @param   bRawReply    It true we have raw data using stream interface
     * @param   iErr         In case of bRawReply this is the error status. It leads into an empty reply if it's value is not TML_SUCCESS.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<br>
     *          If this happens the message communication should be terminated unsing sendError().
     *
     * @see tmlErrors.h
     */
    int MessageAnswer (TML_COMMAND_HANDLE tmlCommand, bool bRawReply, int iErr);


    /**
     * @brief    Send raw answer for a download via the Vortex payload feeder 
     *
     * @param   tmlCommand   An instance of TML_COMMAND_HANDLE containing all necessary data and attributes for the answer.
     * @param   feeder       Reference to the VortexPayloadFeeder.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<br>
     *          If this happens the message communication should be terminated unsing sendError().
     *
     * @see tmlErrors.h
     */
    int SendRawAnswerReplyFromFeeder (TML_COMMAND_HANDLE tmlCommand, VortexPayloadFeeder* feeder);


    /**
     * @brief    Send the final answer
     *
     * @param   tmlCommand   An instance of TML_COMMAND_HANDLE containing all necessary data and attributes to be freed.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<br>
     *          If this happens the message communication should be terminated unsing sendError().
     *
     * @see tmlErrors.h
     */
    int MessageFinalize (TML_COMMAND_HANDLE tmlCommand);


    /**
    * @brief    Register a profile
     *
     * @param   profile   The BEEP communication profile to register.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION if there is a problem during registration.
     *
     * @see tmlErrors.h
     */
    int TMLCoreListener_RegisterProfile(const char* profile);


    /**
    * @brief    Unregister a profile:
     *
     * @param   profile   The BEEP communication profile to unregister.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION if there is a problem during registration.
     *
     * @see tmlErrors.h
     */
    int TMLCoreListener_UnregisterProfile(const char* profile);


    /**
     * @brief   Set the vortex debug logging value.
     *
     * @param   iLogValue    The debug logging value.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if there was no running listener.<br>
     *
     * @see tmlErrors.h
     */
    int TMLCoreListener_Set_Vortex_Logging_Value(int iLogValue);


    /**
     * @brief    Start the TMLCoreListener.
     *
     * @param   host      The host name.
     * @param   port      The requested communication port.
     *                    
     * @param   resPort   If port is equals to "0" a random free port will be detected and the reference to it's identification returned.<br>
     *                    Otherwise it is equals to port.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_ADDRESS_BINDING unable to bind listener address. Maybe the IP/port is already in use or insufficient permission.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_ALREADY_STARTED if the instance is already started.
     *
     * @see tmlErrors.h
     */
    int TMLCoreListener_Start(const char* host, const char*port, const char** resPort);


    /**
     * @brief    Stop the internal TMLCoreListener.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if there was no running listener thread to stop.<br>
     *          TML_ERR_LISTENER_BUSY if the listener is in a pending reply action.<br>
     *          TML_ERR_LISTENER_VORTEX_NOT_INITIALIZED if the TMLListener's Vortex instance was not initialized.
     *
     * @see tmlErrors.h
     */
    int TMLCoreListener_Stop();


    /**
     * @brief   Set the log- file index for explicit logs with closing the file
     * @param   iFileLogIndex       The index.
     */
     void setLogFileIndex(int iFileLogIndex);


  private:
    // Members:

    /**
     * @brief    TML core handle
     */
    TML_CORE_HANDLE m_coreHandle;

    /**
     * @brief    reference to instance of tmlProfileHandler
     */
    tmlProfileHandler* m_pHandler;

    /**
     * @brief    Maximum of accepted connections / in case of iMax = -1 it is unlimited
     */
    VORTEXLimitCheckDataCallbackData m_connectionsLimitCheckData;


    /**
     * @brief    Maximum of accepted channels per connection / in case of iMax = -1 it is unlimited:
     */
    VORTEXLimitCheckDataCallbackData m_channelsPerConnectionLimitCheckData;


    /**
     * @brief    Frame received callback data structure:
     */
    VORTEXListenerFrameReceivedCallbackData m_callbackData;

    /**
     * @brief    Flag of a valid the ListenerThread
     */
    bool m_hValidListenerThread;

    /**
     * @brief    reference to VORTEX instance
     */
    VortexCtx* m_ctx;

    // Methods:

    /**
     * @brief    Send the anser referring a pending message.
     *
     * @param   channel     The channel to write the answer to.
     * @param   iMsgID      The message identification.
     * @param   iDataLength The length of data to send.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<br>
     *          If this happens the message communication should be terminated unsing sendError().
     *
     * @see tmlErrors.h
     */
    int MessageAnswer (VortexChannel* channel, int iMsgID, char* cData, int iDataLength);


    /**
     * @brief    Send a single reply to a pending message.
     *
     * @param   channel      The reference to the VortexChannel to send with.
     * @param   iMsgID       The referring message id.
     * @param   data         reference to the data of the reply.
     * @param   iDataLength  length of the data of the reply.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<br>
     *          If this happens the message communication should be terminated unsing sendError().
     *
     * @see tmlErrors.h
     */
    int SendReply (VortexChannel* channel, int iMsgID, char* data, int iDataLength);


    /**
     * @brief    Send one of multiple replys to a pending message.
     *
     * @param   channel      The reference to the VortexChannel to send with.
     * @param   iMsgID       The referring message id.
     * @param   data         reference to the data of the reply.
     * @param   iDataLength  length of the data of the reply.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<br>
     *          If this happens the message communication should be terminated unsing sendError().
     *
     * @see tmlErrors.h
     */
    int SendAnsReply (VortexChannel* channel, int iMsgID, char* data, int iDataLength);


    /**
     * @brief    Send the end of multiple replys to a pending message.
     *
     * @param   channel      The reference to the VortexChannel to send with.
     * @param   iMsgID       The referring message id.
     * @param   data         reference to the data of the reply.
     * @param   iDataLength  length of the data of the reply.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.<br>
     *          If this happens the message communication should be terminated unsing sendError().
     *
     * @see tmlErrors.h
     */
    int SendFinalAnsReply(VortexChannel* channel, int iMsgID);


    /**
     * @brief    Send an error reply to a pending message to terminate the message communication.
     *
     * @param   channel      The reference to the VortexChannel to send with.
     * @param   iMsgID       The referring message id.
     * @param   data         reference to the data of the reply.
     * @param   iDataLength  length of the data of the reply.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_LISTENER_NOT_INITIALIZED if the start / initialization failed.<br>
     *          TML_ERR_LISTENER_COMMUNICATION in case of a communication error.
     *
     * @see tmlErrors.h
     */
    int SendError (VortexChannel* channel, int iMsgID, char* data, int iDataLength);

};

#endif // TMLCORELISTENER_H 
