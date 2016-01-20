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

#include "tmlCollectCallDestinationObj.h"
#include "tmlCoreSender.h"
#include "tmlEventCall.h"
#include <string.h>
#include <vortex.h>
#include "logValues.h"

/**
 * @brief    Constructor.
 */
tmlCollectCallDestinationObj::tmlCollectCallDestinationObj(const char* profile, tmlLogHandler* log)
{
  m_bMarkedToBeRemoved = false;
  m_log = log;
  m_iFailCounter = 0;
  m_sProfile = NULL;
  m_sHost = NULL;
  m_sPort = NULL;
  m_iEventCounter = 0;
  m_iLastEventLog = 0;
  m_iEventLogPeriod = m_log->getEventLogPeriod();
  m_iEventQueueRemoveCounter = 0;
  m_iLastRemoveEventLog = 0;


  // Profile:
  int iSize;

  if (NULL != m_sProfile)
    delete[] m_sProfile;
  iSize = (int)strlen(profile)+1;
  m_sProfile = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
  strcpy (m_sProfile, profile);
#else
  strcpy_s (m_sProfile, iSize, profile);
#endif

 //////////////////////////////////////////////////////////////////////////////
  //  Event message background handling:
  // m_internalEventMessage2ndStep will be the callback method,
  // that will be called by the tmlEventCall's background thread:
  m_eventCall = new tmlEventCall(log, NULL);
}


/**
 * @brief    Destructor.
 */
tmlCollectCallDestinationObj::~tmlCollectCallDestinationObj()
{
  ///////////////////////////////////////////////////////////
  // First wait until the all event Messages have been done:
  m_eventCall->stopEventMessageHandlingThread();
  if (NULL != m_sProfile)
    delete[] m_sProfile;
  if (NULL != m_sHost)
    delete[] m_sHost;
  if (NULL != m_sPort)
    delete[] m_sPort;
  ////////////////////////////////////////////////////////////////////////////
  //  Afterwards destroy event message handling:
  delete (m_eventCall);

}


/**
* @brief Set all connection relevant params.
*/
void tmlCollectCallDestinationObj::subscribeMessageDestination(const char* sHost, const char* sPort)
{
  int iSize;

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
}


/**
 * @brief Get the destination profile.
 */
void tmlCollectCallDestinationObj::getMessageDestinationProfile(char** profile){
  *profile = m_sProfile;
}


/**
 * @brief Get the destination host.
 */
void tmlCollectCallDestinationObj::getMessageDestinationHost(char**sHost){
  *sHost = m_sHost;
}


/**
 * @brief Get the destination port.
 */
void tmlCollectCallDestinationObj::getMessageDestinationPort(char** sPort){
  *sPort = m_sPort;
}


/**
  * @brief Inform about start of an Event message
  */
bool tmlCollectCallDestinationObj::eventMessageStartInfo(){
  ++m_iEventCounter;
  bool bLogPrinted = false;

  if ((m_iEventCounter - m_iLastEventLog) >= m_iEventLogPeriod){
   // I have To log:
    m_iLastEventLog = m_iEventCounter;
    m_log->log (TML_LOG_EVENT, "Event Message", m_sHost, m_sPort, m_iEventCounter);
    bLogPrinted = true;
  }
  return bLogPrinted;
}


/**
  * @brief Inform about end of an Event message
  */
void tmlCollectCallDestinationObj::eventMessageDoneInfo(bool bLogPrinted){
  if (bLogPrinted){
   // I have To log:
    m_log->log (TML_LOG_EVENT, "Event Message", m_sHost, m_sPort,"Done");
  }
  return;
}


/**
 * @brief Check for equality of this connection with the requested parameter.
 */
bool tmlCollectCallDestinationObj::isEqual(const char* profile){
  bool bEqual = false;
  if (0 == strcmp(profile, m_sProfile)){
    bEqual = true;
  }
  return bEqual;
}


/**
 * @brief Check if message destination is subscribed
 */
bool tmlCollectCallDestinationObj::isSubscriptionEmpty(){
  if ((NULL == m_sHost) && (NULL == m_sPort)){
    printf ("-------------> Subscription Is Empty <---------------\n");
    return true;
  }
  else
    return false;
}


/**
 * @brief Check for equality of this connection with the requested parameter.
 */
bool tmlCollectCallDestinationObj::isEqual(const char* profile, const char* sHost, const char* sPort){
  bool bEqual = false;
  if (0 == strcmp(profile, m_sProfile)){
    // The hostname is case insensitive:
#ifdef LINUX
    if (0 == strcasecmp(sHost, m_sHost)){
#else
    if (0 == _strcmpi(sHost, m_sHost)){
#endif
      if (0 == strcmp(sPort, m_sPort)){
          bEqual = true;
      }
    }
  }
  return bEqual;
}

/**
 * @brief Increment the number of failed message requests to the receiver.
 */
void tmlCollectCallDestinationObj::incrementFailCounter(){
  ++m_iFailCounter;
}

/**
 * @brief Resets the number of failed message requests to the receiver.
 */
void tmlCollectCallDestinationObj::resetFailCounter(){
  m_iFailCounter = 0;
}

/**
 * @brief Returns the number of failed message requests to the receiver.
 */
unsigned int tmlCollectCallDestinationObj::getFailCounter(){
  return m_iFailCounter;
}


/**
 * @brief    Returns the tmlEventCall object
 */
tmlEventCall* tmlCollectCallDestinationObj::getTmlEventCaller(){
  return m_eventCall;
}


/**
 * @brief    Returns markedToBeRemove flag
 */
bool tmlCollectCallDestinationObj::isMarkedToBeRemoved(){
  return m_bMarkedToBeRemoved;
}


/**
 * @brief    set markedToBeRemove flag
 */
void tmlCollectCallDestinationObj::setMarkedToBeRemoved(bool bMarkedToBeRemoved){
  m_bMarkedToBeRemoved = bMarkedToBeRemoved;
}


/**
  * @brief    Get the Event Queue remove counter
  */
TML_INT64 tmlCollectCallDestinationObj::getEventQueueRemoveCounter(){
  return ++m_iEventQueueRemoveCounter;
}


/**
  * @brief    Get the Last remove Event log counter
  */
TML_INT64 tmlCollectCallDestinationObj::getLastRemoveEventLogCounter(){
  return m_iLastRemoveEventLog;
}


/**
  * @brief    Set the Last remove Event log counter
  */
void tmlCollectCallDestinationObj::setLastRemoveEventLogCounter(TML_INT64 counter){
  m_iLastRemoveEventLog = counter;
}

