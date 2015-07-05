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

#include "tmlEventDataQueue.h"
#include <vortex.h>
#include "tmlCore.h"
#include <sidex.h>
#include "logValues.h"


/**
 * @brief    Constructor.
 */
tmlEventDataQueue::tmlEventDataQueue(tmlLogHandler* loghandler)
{
  m_log = loghandler;

  m_firstElement = NULL;
  m_lastElement = NULL;
  m_iQueueLength = 0;

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlEventDataQueue", "tmlEventDataQueue", "Vortex CMD", "vortex_mutex_create");
}


/**
 * @brief    Destructor.
 */
tmlEventDataQueue::~tmlEventDataQueue()
{
  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlEventDataQueue", "~tmlEventDataQueue", "Vortex CMD", "vortex_mutex_destroy");
}


/**
 * @brief    Add an event message data object into the queue.
 */
void tmlEventDataQueue::addData2Queue(EventMsgExecutionData* data){
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlCollectCall", "eventSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

   QueueElement *newElement = new QueueElement;
   newElement->data = data;
   if (NULL == m_firstElement)
   {
      m_firstElement = newElement;
      m_lastElement = newElement;
      newElement->next = NULL;
      newElement->prev = NULL;
   } 
   else
   {
      m_lastElement->next = newElement;
      newElement->prev = m_lastElement;
      newElement->next = NULL;
      m_lastElement = newElement;
   }
   ++m_iQueueLength;
  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlCollectCall", "eventSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");
}


/**
 * @brief    Get the next event message data object out of the queue(Chain element will be removed).
 */
bool tmlEventDataQueue::getDataQueueFrom(EventMsgExecutionData** data, bool bUseCriticalSection){
  bool bFound = false;
  if (bUseCriticalSection){
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlCollectCall", "eventSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

   QueueElement* first = m_firstElement;
   if (NULL != first){
      bFound = true;
      // get the params:
      *data = first->data;
      --m_iQueueLength;
      if (NULL != first->next){
        // Change the params for the next:
        first->next->prev = NULL;
        m_firstElement = first->next;
      }
      else{
        // It's only one CMD:
        m_firstElement = m_lastElement = NULL;
      }
      delete (first);
   }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  if (bUseCriticalSection){
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlCollectCall", "eventSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");
  }
  return bFound;
}


/**
 * @brief    Get the length of the message data object queue.
 */
int tmlEventDataQueue::getQueueLength(){
  return m_iQueueLength;
}


/**
 * @brief    Clear the message data object queue.
 */
void tmlEventDataQueue::clear(){

  bool bFound;
  EventMsgExecutionData* data;
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlCollectCall", "eventSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :
  do{
    bFound = getDataQueueFrom(&data, false);
    if (bFound){
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Now it's time to free all memory for this background event message request (content of EventMessageData);
      destroyDataContent(data);
    }
  }
  while (bFound);
  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlCollectCall", "eventSubscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");
}


/**
 * @brief    Destroy content of data object.
 */
void tmlEventDataQueue::destroyDataContent(EventMsgExecutionData* data){

  if (NULL != data){
    if (NULL != data->msgData){
      if (NULL != data->msgData->profile){
        delete(data->msgData->profile);
      }
      if (NULL != data->msgData->cmdStrContent){
        data->msgData->cmdStrContent->decRef();
        if (data->msgData->cmdStrContent->getRef() == 0){
          TML_COMMAND_HANDLE value = data->msgData->cmdStrContent->getValue();
          if (TML_HANDLE_TYPE_NULL != value){
            tml_Cmd_Free(&value);
          }
          delete (data->msgData->cmdStrContent);
          data->msgData->cmdStrContent = NULL;
        }
      }
      delete data->msgData;
    }
    delete (data);
  }
}


/**
 * @brief    Create the mutex that protect critial section about communication data
 */
axl_bool tmlEventDataQueue::createCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  axl_bool bSuccess = axl_true;
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  bSuccess = vortex_mutex_create (mutex);
  return bSuccess;
}


/**
 * @brief    Destroy the mutex that protect critial section about communication data
 */
axl_bool tmlEventDataQueue::destroyCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  axl_bool bSuccess = axl_true;
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  bSuccess = vortex_mutex_destroy (mutex);
  return bSuccess;
}


/**
 * @brief    Enter a critical Section
 */
void tmlEventDataQueue::enterCriticalSection(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  vortex_mutex_lock (mutex);
}


/**
 * @brief    Leave a critical Section
 */
void tmlEventDataQueue::leaveCriticalSection(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  vortex_mutex_unlock (mutex);
}
