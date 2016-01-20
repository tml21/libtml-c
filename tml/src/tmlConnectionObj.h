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
 
#ifndef TMLCONNECTIONOBJ_H
#define TMLCONNECTIONOBJ_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlCoreSender.h"

static TML_INT64          m_iMagicNumber = 0;

class  tmlConnectionObj
{

private:
	  /* data */

    /**
     * @brief    char array to set the date arribute
     */
    bool               m_bPendingToBeRemoved;
    char*              m_sProfile;
    char*              m_sHost;
    char*              m_sPort;
    VortexChannelPool* m_channelPool;
    VortexChannel*     m_channel;
    TMLCoreSender*     m_sender;
    VortexConnection*  m_connection;
    bool               m_bLocked;
    bool               m_bRawViaVortexPayloadFeeder;

    /**
     * @brief  Datastructure contains the parameter for the tmlCoreSender's frame receive method
     */
    VORTEXSenderFrameReceivedCallbackData* m_callbackData;

    /**
     * @brief    Synchronisation mutex 
     */
    VortexMutex  m_mutexCriticalSection;

    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;

	  /* methods */
    /**
     * @brief    Create the mutex that protect critial section about communication data
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     * @returns axl_true on success.
     */
    axl_bool createCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    Destroy the mutex that protect critial section about communication data
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     * @returns axl_true on success.
     */
    axl_bool destroyCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    Enter a critical Section
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     */
    void enterCriticalSection(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    Leave a critical Section
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     */
    void leaveCriticalSection(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


public:
	  /* data */

	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @param   loghandler   Reference to a log handler.
     *
     * @returns an instance of tmlCoreWrapper
     */
    explicit tmlConnectionObj(tmlLogHandler* loghandler);


    /**
     * @brief    Destructor.
     */
    ~tmlConnectionObj();


    /**
     * @brief Set all connection relevant params.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     * @param   sender      Reference to an instance of tmlCoreSender.
     * @param   channelPool The channel pool
     * @param   connection  Reference to an instance of VortexConnection.
     */
    void setConnectionObj(const char* profile, const char* sHost, const char* sPort, TMLCoreSender* sender, VortexChannelPool* channelPool, VortexConnection* connection);


    /**
     * @brief Set locked flag
     *
     * @param   bLock     Use mutex lock mechanism at the beginning.
     * @param   bUnlock   Use mutex lock mechanism at the end.
     */
    void lock(bool bLock, bool bUnlock);


    /**
     * @brief Reset locked flag
     */
    void unlock();

    /**
     * @brief locked flag request
     *
     *
     * @param   bLock        Use mutex lock mechanism at the beginning.
     * @param   bkKeepLocked Use mutex lock mechanism only if locked at the end.
     * @returns true if locked
     */
    bool isLocked(bool bLock, bool bkKeepLocked);


    /**
     * @brief Set Raw Via Vortex Payload Feeder
     *
     * @param bRaw the raw flag
     */
    void setRawViaVortexPayloadFeeder(bool bRaw);

    /**
     * @brief Fet Raw Via Vortex Payload Feeder
     *
     * @returns true if flag is set
     */
    bool getRawViaVortexPayloadFeeder();


    /**
      * @brief Get a unique number
      *
      * @returns the number
      */
    TML_INT64 getMagicNumber();

    /**
     * @brief Get the profile.
     *
     * @param   sProfile  Reference to value for the profile.
     */
    void getProfile(char** sProfile);


    /**
     * @brief Get the port.
     *
     * @param   sPort  Reference to value for the port
     */
    void getPort(char** sPort);


    /**
     * @brief Get the host.
     *
     * @param   sHost  Reference to value for the host.
     */
    void getHost(char** sHost);


    /**
     * @brief Get the callback data
     */
    void getCallbackData(VORTEXSenderFrameReceivedCallbackData** callbackData);


    /**
     * @brief Get the tmlCoreSender object.
     *
     * @param   sender  Reference to value to write the sender object to.
     */
    void getSender(TMLCoreSender** sender);


    /**
     * @brief get the attribute that signals a to be removed.
     */
    bool isPendingToBeRemoved();


    /**
     * @brief set attribute to be removed.
     */
    void setToBeRemoved();


    /**
     * @brief Get the VortexConnection object.
     *
     * @param   connection  Reference to value to write the connection object to.
     */
    void getConnection(VortexConnection** connection);


    /**
     * @brief Get the VortexChannel object.
     */
    void getChannel(VortexChannel** channel);


    /**
     * @brief Set the VortexChannel object.
     */
    void setChannel(VortexChannel* channel);


    /**
     * @brief Get the VortexChannelPool object.
     *
     * @param   channelPool  Reference to VortexChannelPool object.
     */
    void getChannelPool(VortexChannelPool** channelPool);


    /**
     * @brief Set the VortexChannelPool object.
     */
    void setChannelPool(VortexChannelPool* channelPool);


    /**
     * @brief Check for equality of this connection with the requested parameter.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns true in case of equality, otherwise false.
     */
    bool isEqual(const char* profile, const char* sHost, const char* sPort);


    /**
     * @brief Check for equality of this connection with the requested parameter.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     * @param   channel     Reference to a VortexChannel
     *
     * @returns true in case of equality, otherwise false.
     */
    bool isEqual(const char* profile, const char* sHost, const char* sPort, VortexChannel* channel);
};


#endif //TMLCONNECTIONOBJ_H

