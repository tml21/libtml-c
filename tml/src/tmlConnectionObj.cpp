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

#include "tmlConnectionObj.h"
#include "tmlCoreSender.h"
#include <ctype.h>
#include "logValues.h"

/**
 * @brief    Constructor.
 */
tmlConnectionObj::tmlConnectionObj(tmlLogHandler* loghandler)
{
  m_sProfile = NULL;
  m_sHost = NULL;
  m_sPort = NULL;
  m_channel = NULL;
  m_channelPool = NULL;
  m_sender = NULL;
  m_connection = NULL;
  m_callbackData = new VORTEXSenderFrameReceivedCallbackData;
  m_bLocked = false;
  m_bRawViaVortexPayloadFeeder = false;
  m_log = loghandler;
  m_bPendingToBeRemoved = false;
  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlConnectionObj", "tmlConnectionObj", "Vortex CMD", "vortex_mutex_create");
}


/**
 * @brief    Destructor.
 */
tmlConnectionObj::~tmlConnectionObj()
{
  if (NULL != m_sProfile)
    delete[] m_sProfile;
  m_sProfile = NULL;
  if (NULL != m_sHost)
    delete[] m_sHost;
  m_sHost = NULL;
  if (NULL != m_sPort)
    delete[] m_sPort;
  m_sPort = NULL;
  m_channel = NULL;
  m_channelPool = NULL;
  m_sender = NULL;
  m_connection = NULL;
  delete m_callbackData;
  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlConnectionObj", "~tmlConnectionObj", "Vortex CMD", "vortex_mutex_destroy");
}


/**
 * @brief Set all connection relevant params.
 */
void tmlConnectionObj::setConnectionObj(const char* profile, const char* sHost, const char* sPort, TMLCoreSender* sender, VortexChannelPool* channelPool, VortexConnection* connection)
{
  int iSize;

  // To be removed flag:
  m_bPendingToBeRemoved = false;
  // Profile:
  if (NULL != m_sProfile)
    delete (m_sProfile);
  iSize = (int)strlen(profile)+1;
  m_sProfile = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
  strcpy (m_sProfile, profile);
#else
  strcpy_s (m_sProfile, iSize, profile);
#endif

  // Host:
  if (NULL != m_sHost)
    delete (m_sHost);
  iSize = (int)strlen(sHost)+1;
  m_sHost = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
  strcpy (m_sHost, sHost);
#else
  strcpy_s (m_sHost, iSize, sHost);
#endif

  // Port:
  if (NULL != m_sPort)
    delete (m_sPort);
  iSize = (int)strlen(sPort)+1;
  m_sPort = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
  strcpy (m_sPort, sPort);
#else
  strcpy_s (m_sPort, iSize, sPort);
#endif

  // Channel:
  m_channel = NULL;

  // ChannelPool:
  m_channelPool = channelPool;

  // Sender:
  m_sender = sender;

  // Connection:
  m_connection = connection;
}

/**
 * @brief Set locked flag
 */
void tmlConnectionObj::lock(bool bLock, bool bUnlock){
  if (bLock){
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlConnectionObj", "lock", "Vortex CMD", "vortex_mutex_lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a send command may not be possible during this method execution :
  }

  m_bLocked = true;

  if (bUnlock){
    /////////////////////////////////////////////////////////////////////////////////////////
    // End of critical section / a send command was not possible during this method execution
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlConnectionObj", "lock", "Vortex CMD", "vortex_mutex_unlock");
  }
}


/**
  * @brief Reset locked flag
  */
void tmlConnectionObj::unlock(){
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlConnectionObj", "unlock", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_bLocked = false;

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlConnectionObj", "unlock", "Vortex CMD", "vortex_mutex_unlock");
}


/**
  * @brief locked flag request
  */
bool tmlConnectionObj::isLocked(bool bLock, bool bkKeepLocked){
  bool bRet = false;
  if (bLock){
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlConnectionObj", "isLocked", "Vortex CMD", "vortex_mutex_lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a send command may not be possible during this method execution :
  }

  bRet = m_bLocked;

  if (bLock && (m_bLocked || !bkKeepLocked)){
    /////////////////////////////////////////////////////////////////////////////////////////
    // End of critical section / a send command was not possible during this method execution
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlConnectionObj", "isLocked", "Vortex CMD", "vortex_mutex_unlock");
  }

  return bRet;
}


/**
  * @brief Set Raw Via Vortex Payload Feeder
  */
void tmlConnectionObj::setRawViaVortexPayloadFeeder(bool bRaw){
  m_bRawViaVortexPayloadFeeder = bRaw;
}


/**
  * @brief Get Raw Via Vortex Payload Feeder
  */
bool tmlConnectionObj::getRawViaVortexPayloadFeeder(){
  return m_bRawViaVortexPayloadFeeder;
}


/**
  * @brief Get a unique number
  */
TML_INT64 tmlConnectionObj::getMagicNumber(){
  return ++m_iMagicNumber;
}

/**
 * @brief Get the profile.
 */
void tmlConnectionObj::getProfile(char** sProfile){
  *sProfile = m_sProfile;
}


/**
 * @brief Get the port.
 */
void tmlConnectionObj::getPort(char** sPort){
  *sPort = m_sPort;
}


/**
 * @brief Get the host.
 */
void tmlConnectionObj::getHost(char** sHost){
  *sHost = m_sHost;
}


/**
 * @brief Get the callback data
 */
void tmlConnectionObj::getCallbackData(VORTEXSenderFrameReceivedCallbackData** callbackData){
  *callbackData = m_callbackData;
}

/**
 * @brief Get the tmlCoreSender object.
 */
void tmlConnectionObj::getSender(TMLCoreSender** sender){
  *sender = m_sender;
}


/**
 * @brief set attribute to be removed.
 */
void tmlConnectionObj::setToBeRemoved(){
  m_bPendingToBeRemoved = true;
}


/**
 * @brief get the attribute that signals a to be removed.
 */
bool tmlConnectionObj::isPendingToBeRemoved(){
  return m_bPendingToBeRemoved;
}


/**
 * @brief Get the VortexConnection object.
 */
void tmlConnectionObj::getConnection(VortexConnection** connection){
  *connection = m_connection;
}


/**
 * @brief Get the VortexChannel object.
 */
void tmlConnectionObj::getChannel(VortexChannel** channel){
  *channel = m_channel;
}


/**
 * @brief Set the VortexChannel object.
 */
void tmlConnectionObj::setChannel(VortexChannel* channel){
  m_channel = channel;
}


/**
 * @brief Get the VortexChannelPool object.
 */
void tmlConnectionObj::getChannelPool(VortexChannelPool** channelPool){
  *channelPool = m_channelPool;
}


/**
 * @brief Set the VortexChannelPool object.
 */
void tmlConnectionObj::setChannelPool(VortexChannelPool* channelPool){
  m_channelPool = channelPool;
}


/**
 * @brief Check for equality of this connection with the requested parameter.
 */
bool tmlConnectionObj::isEqual(const char* profile, const char* sHost, const char* sPort){
  bool bEqual = false;
  if ((NULL != m_sProfile) && (NULL != m_sHost) && (NULL != m_sPort)){
    if (0 == strcmp(profile, m_sProfile)){
      // The hostname is case insensitive:
#ifdef LINUX
      if (0 == strcasecmp(sHost, m_sHost)){
#else
      if (0 == _strcmpi(sHost, m_sHost)){
#endif
        if (0 == strcmp(sPort, m_sPort)){
          if (false == m_bPendingToBeRemoved){
            bEqual = true;
          }
        }
      }
    }
  }
  return bEqual;
}


/**
 * @brief Check for equality of this connection with the requested parameter.
 */
bool tmlConnectionObj::isEqual(const char* profile, const char* sHost, const char* sPort, VortexChannel* channel){
  bool bEqual = false;
  if ((NULL != m_sProfile) && (NULL != m_sHost) && (NULL != m_sPort)){
    if (0 == strcmp(profile, m_sProfile)){
      // The hostname is case insensitive:
#ifdef LINUX
      if (0 == strcasecmp(sHost, m_sHost)){
#else
      if (0 == _strcmpi(sHost, m_sHost)){
#endif
        if (0 == strcmp(sPort, m_sPort)){
          if (channel == m_channel){
            if (false == m_bPendingToBeRemoved){
              bEqual = true;
            }
          }
        }
      }
    }
  }
  return bEqual;
}
/**
 * @brief    Create the mutex that protect critial section about communication data
 */
axl_bool tmlConnectionObj::createCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  axl_bool bSuccess = vortex_mutex_create (mutex);
  return bSuccess;
}


/**
 * @brief    Destroy the mutex that protect critial section about communication data
 */
axl_bool tmlConnectionObj::destroyCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  axl_bool bSuccess = vortex_mutex_destroy (mutex);
  return bSuccess;
}


/**
 * @brief    Enter a critical Section
 */
void tmlConnectionObj::enterCriticalSection(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  vortex_mutex_lock (mutex);
}


/**
 * @brief    Leave a critical Section
 */
void tmlConnectionObj::leaveCriticalSection(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  vortex_mutex_unlock (mutex);
}


