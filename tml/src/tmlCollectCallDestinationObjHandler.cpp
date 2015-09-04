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

#include "tmlCollectCallDestinationObjHandler.h"
#include "tmlCoreDefines.h"
#include "tmlErrors.h"
#include "tmlEventCall.h"
#include <sidex.h>
#include "systemBase.h"
#include "logValues.h"



/**
 * @brief    Constructor.
 */
tmlCollectCallDestinationObjHandler::tmlCollectCallDestinationObjHandler(tmlLogHandler* loghandler)
{
  m_log = loghandler;
  m_iMutexCriticalSectionCount = 0;
  m_eventDestinationAddressHashTable = new TMLUniversalHashTable(loghandler);
  m_eventDestinationAddressHashTable->createHashTable(true);
  m_loadBalancedDestinationAddressHashTable = new TMLUniversalHashTable(loghandler);
  m_loadBalancedDestinationAddressHashTable->createHashTable(true);
  ////////////////////////////////////////////////////////////////////////////////////////
  // Link list of tmlCollectCallDestinationObj that has to be freed on destructor
  m_unsubscribedObjLinkListLast = NULL;

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlCollectCallDestinationObjHandler", "tmlCollectCallDestinationObjHandler", "Vortex CMD", "vortex_mutex_create");
  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  createCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, "tmlCollectCallDestinationObjHandler", "tmlCollectCallDestinationObjHandler", "Vortex CMD", "vortex_mutex_create");
  m_iMutexCriticalSectionLinkListCount = 0;
  m_bInShutdown = false;
}


/**
 * @brief    Destructor.
 */
tmlCollectCallDestinationObjHandler::~tmlCollectCallDestinationObjHandler()
{
  ////////////////////////////////////////////////////////////////////////////////////////
  // Wait, until pending event messages are sent:
  waitForPendingEventMessages();

  ////////////////////////////////////////////////////////////////////////////////////////
  // Free link List of tmlCollectCallDestinationObj
  freeUnsubscribedObjLinkList();

  m_bInShutdown = true;
  ////////////////////////////////////////////////////////////////////////////////////////
  // Free hash tables
  freeMessageHandlingHashTable(m_eventDestinationAddressHashTable);
  m_eventDestinationAddressHashTable = NULL;
  freeMessageHandlingHashTable(m_loadBalancedDestinationAddressHashTable);
  m_loadBalancedDestinationAddressHashTable = NULL;

  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, "tmlCollectCallDestinationObjHandler", "~tmlCollectCallDestinationObjHandler", "Vortex CMD", "vortex_mutex_destroy");
  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, "tmlCollectCallDestinationObjHandler", "~tmlCollectCallDestinationObjHandler", "Vortex CMD", "vortex_mutex_destroy");
}


/**
 * @brief    Returns the referenceto the unsubscribed link list
 */
DestinationAddressHandlingListElement** tmlCollectCallDestinationObjHandler::acquireUnsubscribedLinkList(){
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "acquireUnsubscribedLinkList", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :
  return &m_unsubscribedObjLinkListLast;
}



/**
  * @brief    unlocks exlusive access to unsubscribed link list
  */
void tmlCollectCallDestinationObjHandler::unlockUnsubscribedLinkList(){
  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unlockUnsubscribedLinkList", "Vortex CMD", "vortex_mutex_unlock");
}


/**
  * @brief    Subscribe a destination / event.
  */
int tmlCollectCallDestinationObjHandler::subscribeEventMessageDestination(const char* profile, const char* sHost, const char* sPort, tmlLogHandler* log){
  return subscribeMessageDestination(m_eventDestinationAddressHashTable, profile, sHost, sPort, log);
}


/**
  * @brief    Subscribe a destination / loadBalanced.
  */
int tmlCollectCallDestinationObjHandler::subscribeLoadBalancedMessageDestination(const char* profile, const char* sHost, const char* sPort, tmlLogHandler* log){
  return subscribeMessageDestination(m_loadBalancedDestinationAddressHashTable, profile, sHost, sPort, log);
}


/**
 * @brief    Subscribe a destination
 */
int tmlCollectCallDestinationObjHandler::subscribeMessageDestination(TMLUniversalHashTable* hashTable, const char* profile, const char* sHost, const char* sPort, tmlLogHandler* log)
{
  int iRet = TML_SUCCESS;

  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  DestinationAddressHandlingListElement* linkList = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "subscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  hashTable->getValue((char*)profile, &hashItem);
  if (NULL == hashItem){
    //////////////////////////////////////////////////
    // There is no hash entry:
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    hashTable->setValue(hashValue->getProfile(), hashValue);
  }
  else{
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  //////////////////////////////////////
  // The link to the destination list:
  linkList = hashValue->getLinkList();

  if (NULL == linkList){
    //////////////////////////////////////////////
    // The hash value exists but no destination entry is known:

    //////////////////////////////////////////////
    // create a link list entry:
    linkList = new DestinationAddressHandlingListElement;
    linkList->next = NULL;
    linkList->prev = NULL;
    tmlCollectCallDestinationObj* destinationObj = new tmlCollectCallDestinationObj(profile, log);

    /////////////////////////////////////////////////////////////
    // Set the NEXT destination for a possible load balancing:
    hashValue->setNextLoadBalancedDestination(destinationObj);

    //////////////////////////////////////////////////
    // write parameter to the tmlCollectCallDestinationObj:
    destinationObj->subscribeMessageDestination (sHost, sPort);
    linkList->destinationObj = destinationObj;
    hashValue->setLinkList(linkList);
  }
  else
  {
    ////////////////////////////////////////////////////////////
    // There is also a link list of subscribed destinations:
    bool bFound = false;
    DestinationAddressHandlingListElement* actualElement = NULL;
    //////////////////////////////////////////////
    // look if the destination entry is known:
    do{
      actualElement = linkList; // remember the actual entry
      tmlCollectCallDestinationObj* destinationObj;
      destinationObj = linkList->destinationObj;
      if (NULL!= destinationObj){
        //////////////////////////////////////////////
        // look if the destination entry is known:
        if (destinationObj->isSubscriptionEmpty()){
          //////////////////////////////////////////////////////////
          // There is a link list entry without a subscription,
          // so write parameter to the tmlCollectCallDestinationObj:
          destinationObj->subscribeMessageDestination (sHost, sPort);

          /////////////////////////////////////////////////////////////
          // Set the NEXT destination for a possible load balancing:
          hashValue->setNextLoadBalancedDestination(destinationObj);

          bFound = true;
        }
        else{
          //////////////////////////////////////////////////////////
          // There is a link list entry with any subscription
          if (destinationObj->isEqual(profile, sHost, sPort)){
            bFound = true;
          }
        }
      }
      linkList = linkList->next;
    }
    while (!bFound && NULL != linkList);

    if (!bFound){
      //////////////////////////////////////////////
      // The destination entry is unknown:

      //////////////////////////////////////////////
      // create a link list entry:
      linkList = new DestinationAddressHandlingListElement;
      linkList->next = NULL;
      linkList->prev = actualElement;
      actualElement->next = linkList; // Link from the last
      tmlCollectCallDestinationObj* destinationObj = new tmlCollectCallDestinationObj(profile, log);

      /////////////////////////////////////////////////////////////
      // Set the NEXT destination for a possible load balancing:
      hashValue->setNextLoadBalancedDestination(destinationObj);

      //////////////////////////////////////////////////
      // write parameter to the tmlCollectCallDestinationObj:
      destinationObj->subscribeMessageDestination (sHost, sPort);
      linkList->destinationObj = destinationObj;
    }
    else{
      ////////////////////////////////////////
      // Nothing to do because element exists
      ////////////////////////////////////////
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "subscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
  * @brief    Check whether destination is subsrcibed
  */
bool tmlCollectCallDestinationObjHandler::acquireMessageDestination(const char* profile, const char* sHost, const char* sPort){
  return acquireMessageDestination(m_eventDestinationAddressHashTable, profile, sHost, sPort);
}


/**
 * @brief    Check whether destination is subsrcibed
 */
bool tmlCollectCallDestinationObjHandler::acquireMessageDestination(TMLUniversalHashTable* hashTable, const char* profile, const char* sHost, const char* sPort)
{
  bool bFound = false;
  
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "acquireMessageDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  tmlDestinationAddressHandlingHashValue* hashValue;
  DestinationAddressHandlingListElement* linkList = NULL;
  void* hashItem;

  hashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    linkList = hashValue->getLinkList();
  }
  if (NULL != linkList){
    //////////////////////////////////////////////
    // look if the destination entry is known:
    do{
      tmlCollectCallDestinationObj* destinationObj = NULL;
      destinationObj = linkList->destinationObj;
      if (NULL!= destinationObj){
        if (destinationObj->isEqual(profile, sHost, sPort)){
          if (!destinationObj->isMarkedToBeRemoved()){
            bFound = true;
          }
          else{
            m_log->log ("tmlCollectCallDestinationObjHandler", "acquireMessageDestination", "found Object but", "it is Marked to be remove");
          }
        }
      }
      linkList = linkList->next;
    }
    while (!bFound && NULL != linkList);
  }
  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "acquireMessageDestination", "Vortex CMD", "vortex_mutex_unlock");
  return bFound;
}


/**
  * @brief    Unsubscribe a destination / event.
  */
int tmlCollectCallDestinationObjHandler::unsubscribeEventMessageDestination(const char* profile, const char* sHost, const char* sPort, bool bLockCritical){
  return unsubscribeMessageDestination(m_eventDestinationAddressHashTable, profile, sHost, sPort, bLockCritical);
}


/**
  * @brief    Unsubscribe a destination / load balanced.
  */
int tmlCollectCallDestinationObjHandler::unsubscribeLoadBalancedMessageDestination(const char* profile, const char* sHost, const char* sPort, bool bLockCritical){
  return unsubscribeMessageDestination(m_loadBalancedDestinationAddressHashTable, profile, sHost, sPort, bLockCritical);
}


/**
 * @brief    Unsubscribe a destination for event messages.
 */
int tmlCollectCallDestinationObjHandler::unsubscribeMessageDestination(TMLUniversalHashTable* hashTable, const char* profile, const char* sHost, const char* sPort, bool bLockCritical)
{
  int iRet = TML_SUCCESS;

  if (bLockCritical){
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unsubscribeMessageDestination", "Vortex CMD", "vortex_mutex_lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a send command may not be possible during this method execution :
  }

  tmlDestinationAddressHandlingHashValue* hashValue;
  DestinationAddressHandlingListElement* linkList = NULL;
  void* hashItem;

  hashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    linkList = hashValue->getLinkList();
  }
  if (NULL != linkList){
    bool bFound = false;
    DestinationAddressHandlingListElement* actualElement = NULL;
    //////////////////////////////////////////////
    // look if the destination entry is known:
    tmlCollectCallDestinationObj* destinationObj = NULL;
    do{
      destinationObj = linkList->destinationObj;
      if (NULL!= destinationObj){
        if (destinationObj->isEqual(profile, sHost, sPort)){
          actualElement = linkList; // remember the actual entry
          bFound = true;
        }
      }
      linkList = linkList->next;
    }
    while (!bFound && NULL != linkList);
    if (bFound){
      ////////////////////////////////////////////////////////////
      // profile exist in the hashtable and host - port match found:
      if (NULL == actualElement->prev){
        ////////////////////////////////////////////////////////////
        // I'm looking at the first element in the link list:
        tmlCollectCallDestinationObj* actualObj = actualElement->destinationObj;
        if (NULL != actualElement->next){
          /////////////////////////////////////////////////////////////
          // possible skip of the NEXT destination for a possible load balancing
          hashValue->skipLoadBalancedDestinationByMatch(actualObj);
          
          ////////////////////////////////////////////////////////////
          // The next one is getting the first one now:
          actualElement->next->prev = NULL;
          hashValue->setLinkList(actualElement->next);
          delete (actualElement);
          ////////////////////////////////////////////////////////////
          // I cannot remove the tmlCollectCallDestinationObj at the moment,
          // because  of a possible pending async cmds / so remember in a link list.
          // This will be done in tmlSingleCall::RemoveMarkedUnsubscriptions()
          // during thenect command
          actualObj->setMarkedToBeRemoved (true);
          addUnsubscribedObjToLinkList(actualObj);
        }
        else{
          ////////////////////////////////////////////////////////////
          // No link list / no destination for this hash key:
          hashValue->setLinkList(NULL);
          ////////////////////////////////////////////////////////////
          // No Load Balancing possible:
          hashValue->setNextLoadBalancedDestination(NULL);
          ////////////////////////////////////////////////////////////
          // I cannot remove the tmlCollectCallDestinationObj at the moment,
          // because  of a possible pending async cmds / so remember in a link list.
          // This will be done in tmlSingleCall::RemoveMarkedUnsubscriptions()
          // during thenect command
          addUnsubscribedObjToLinkList(actualObj);
          ////////////////////////////////////////////////////////////
          // free the list entry:
          delete (actualElement);
        }
      }
      else{
        destinationObj = actualElement->destinationObj;
        ////////////////////////////////////////////////////////////
        // I'm looking at any element in the link list:
        /////////////////////////////////////////////////////////////
        // possible skip of the NEXT destination for a possible load balancing
        hashValue->skipLoadBalancedDestinationByMatch(destinationObj);

        ////////////////////////////////////////////////////////////
        // I'm looking at any element in the link list,
        // so change the link list:
        actualElement->prev->next = actualElement->next;
        if (NULL != actualElement->next){
          actualElement->next->prev = actualElement->prev;
        }
        ////////////////////////////////////////////////////////////
        // I cannot remove the tmlCollectCallDestinationObj at the moment,
        // because  of a possible pending async cmds / so remember in a link list.
        // This will be done in tmlSingleCall::RemoveMarkedUnsubscriptions()
        // during thenect command
        addUnsubscribedObjToLinkList(destinationObj);
        ////////////////////////////////////////////////////////////
          // free the list entry:
        delete (actualElement);
      }
    }
    else{
      ////////////////////////////////////////////////////////////
      // profile exist in the hashtable but no host - port match:
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    ////////////////////////////////////////////////////
    // a link list entry doesn't exist in the hashtable:
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  if (bLockCritical){
    /////////////////////////////////////////////////////////////////////////////////////////
    // End of critical section / a send command was not possible during this method execution
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unsubscribeMessageDestination", "Vortex CMD", "vortex_mutex_unlock");
  }

  return iRet;
}




/**
  * @brief    Unsubscribe all destinations for the provided profile / event.
  */
int tmlCollectCallDestinationObjHandler::unsubscribeAllEventMessageDestinations(const char* profile){
  SIDEX_VARIANT subscriptions = SIDEX_HANDLE_TYPE_NULL;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unsubscribeAllMessageDestinations", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  TML_INT32 iRet = (TML_INT32) GetSubscribedEventMessageDestinations(profile, &subscriptions, false);
  if (TML_SUCCESS == iRet && SIDEX_HANDLE_TYPE_NULL != subscriptions){
    TML_INT32 iRows;
    iRet = sidex_Variant_Table_Rows (subscriptions, &iRows);
    for (TML_INT32 i = 0; i < iRows && TML_SUCCESS == iRet; ++i){
      SIDEX_VARIANT vProfile = SIDEX_HANDLE_TYPE_NULL;
      SIDEX_VARIANT vHost = SIDEX_HANDLE_TYPE_NULL;
      SIDEX_VARIANT vPort = SIDEX_HANDLE_TYPE_NULL;
      char* sProfile = NULL;
      char* sHost = NULL;
      char* sPort = NULL;
      TML_INT32 iLength;

      iRet = sidex_Variant_Table_GetField (subscriptions, i, (char*)"PROFILE", &vProfile);
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_As_String(vProfile, &sProfile, &iLength);
      }
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_Table_GetField (subscriptions, i, (char*)"HOST", &vHost);
      }
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_As_String(vHost, &sHost, &iLength);
      }
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_Table_GetField (subscriptions, i, (char*)"PORT", &vPort);
      }
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_As_String(vPort, &sPort, &iLength);
      }

      if (TML_SUCCESS == iRet){
        iRet = unsubscribeEventMessageDestination(sProfile, sHost, sPort, false);
      }
    }
    sidex_Variant_DecRef(subscriptions);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unsubscribeAllMessageDestinations", "Vortex CMD", "vortex_mutex_unlock");

  return (int)iRet;
}


/**
  * @brief    Unsubscribe all destinations for the provided profile / load balanced.
  */
int tmlCollectCallDestinationObjHandler::unsubscribeAllLoadBalancedMessageDestinations(const char* profile){
  SIDEX_VARIANT subscriptions = SIDEX_HANDLE_TYPE_NULL;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unsubscribeAllMessageDestinations", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  TML_INT32 iRet = (TML_INT32) GetSubscribedLoadBalancedMessageDestinations(profile, &subscriptions, false);
  if (TML_SUCCESS == iRet && SIDEX_HANDLE_TYPE_NULL != subscriptions){
    TML_INT32 iRows;
    iRet = sidex_Variant_Table_Rows (subscriptions, &iRows);
    for (TML_INT32 i = 0; i < iRows && TML_SUCCESS == iRet; ++i){
      SIDEX_VARIANT vProfile = SIDEX_HANDLE_TYPE_NULL;
      SIDEX_VARIANT vHost = SIDEX_HANDLE_TYPE_NULL;
      SIDEX_VARIANT vPort = SIDEX_HANDLE_TYPE_NULL;
      char* sProfile = NULL;
      char* sHost = NULL;
      char* sPort = NULL;
      TML_INT32 iLength;

      iRet = sidex_Variant_Table_GetField (subscriptions, i, (char*)"PROFILE", &vProfile);
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_As_String(vProfile, &sProfile, &iLength);
      }
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_Table_GetField (subscriptions, i, (char*)"HOST", &vHost);
      }
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_As_String(vHost, &sHost, &iLength);
      }
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_Table_GetField (subscriptions, i, (char*)"PORT", &vPort);
      }
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_As_String(vPort, &sPort, &iLength);
      }

      if (TML_SUCCESS == iRet){
        iRet = unsubscribeLoadBalancedMessageDestination(sProfile, sHost, sPort, false);
      }
    }
    sidex_Variant_DecRef(subscriptions);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unsubscribeAllMessageDestinations", "Vortex CMD", "vortex_mutex_unlock");

  return (int)iRet;
}


/**
 * @brief    Empty and free the hashtable
 */
void tmlCollectCallDestinationObjHandler::freeMessageHandlingHashTable(TMLUniversalHashTable* hashTable){

  char** sKeys;
  int iRet;


  ///////////////////////////////////////////
  // Event message handling hash table:
  iRet = hashTable->getKeys(&sKeys);
  if (TML_SUCCESS == iRet){
    if (NULL != sKeys){
      int iSize;
      iRet = hashTable->hashSize(&iSize);
      if (TML_SUCCESS == iRet){
        ////////////////////////////////////
        // loop via all keys:
        for (int i = 0; i < iSize; ++i){
          void* hashItem;
          iRet = hashTable->getValue(sKeys[i], &hashItem);
          tmlDestinationAddressHandlingHashValue* hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
          DestinationAddressHandlingListElement* element = hashValue->getLinkList();
          if (NULL != element){
            do{
              DestinationAddressHandlingListElement*  dismiss = element;
              tmlCollectCallDestinationObj* destinationObj;
              destinationObj = element->destinationObj;
              if (NULL!= destinationObj){
                delete (destinationObj);
              }
              element = element->next;
              delete (dismiss);
            }
            while (NULL != element);
          }
          ////////////////////////////////////////////////////////////
          // I need to call the destructor:
          hashValue->~tmlDestinationAddressHandlingHashValue();
          ////////////////////////////////////////
          // Free memory allocated for keys array
          delete (sKeys[i]);
          ////////////////////////////////////////
          // Delete the value stored in the hash:
          if (NULL != hashItem){
            delete ((tmlDestinationAddressHandlingHashValue*) hashItem);
          }
          ////////////////////////////////////////
        }
      }
      /////////////////////////////////
      // Now I have to free the memory:
      delete sKeys;
    }
  }
  delete (hashTable);
}


/**
 * @brief    Wait, until pending event messages are sent
 */
void tmlCollectCallDestinationObjHandler::waitForPendingEventMessages(){
  bool bContinue = false;
  int index = 0;
  do{
    bContinue = waitForPendingEventMessagesProfileIndex(&index);
  }while (bContinue);
}


/**
 * @brief    Wait, until pending event messages are sent for the given profile index
 */
bool tmlCollectCallDestinationObjHandler::waitForPendingEventMessagesProfileIndex(int* iIndex){

    bool bContinue = false;
    int iSize = 0;

    char** sKeys = NULL;
    iSize = 0;
    int iRet;
    bool bHaveMutex;


    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "sendQueuedEventMessagesUntilQueueIsEmpty", "Vortex CMD", "vortex_mutex_lock");
    bHaveMutex = true;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a send command may not be possible during this method execution :

    ///////////////////////////////////////////
    // Event message handling hash table:
    iRet = m_eventDestinationAddressHashTable->getKeys(&sKeys);
    if (TML_SUCCESS == iRet){
      if (NULL != sKeys){
        iRet = m_eventDestinationAddressHashTable->hashSize(&iSize);
        if (TML_SUCCESS == iRet && iSize > *iIndex){
          ////////////////////////////////////
          // loop via all keys:
          void* hashItem;
          iRet = m_eventDestinationAddressHashTable->getValue(sKeys[*iIndex], &hashItem);
          if (TML_SUCCESS == iRet){
            /////////////////////////////////////////////////////////////////////////////////////////
            // End of critical section / a send command was not possible during this method execution
            leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "sendQueuedEventMessagesUntilQueueIsEmpty", "Vortex CMD", "vortex_mutex_unlock");
            bHaveMutex = false;

            tmlDestinationAddressHandlingHashValue* hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
            DestinationAddressHandlingListElement* element = hashValue->getLinkList();
            if (NULL != element){
              // SIDEX-223:API- "tml_Evt_Set_MaxConnectionFailCount" don't work
                tmlCollectCallDestinationObj* destinationObj;
                destinationObj = element->destinationObj;
                if (NULL!= destinationObj){
                  bContinue = true;
                  // Hold the thread / wait untilfinished:
                  destinationObj->getTmlEventCaller()->stopEventMessageHandlingThread();
                }
            }
          }
        }
        for (int i = 0; i < iSize; ++i){	
        // Free memory allocated for keys array
          delete (sKeys[i]);
        }
        /////////////////////////////////
        // Now I have to free the memory:
        delete sKeys;
      }
    }

    if (bHaveMutex){
      /////////////////////////////////////////////////////////////////////////////////////////
      // End of critical section / a send command was not possible during this method execution
      leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "sendQueuedEventMessagesUntilQueueIsEmpty", "Vortex CMD", "vortex_mutex_unlock");
    }
    (*iIndex)++;
    if (iSize > *iIndex){
      bContinue = true;
    }
    return bContinue;
}


/**
 * @brief    Empty and free the link list for unsubscribed load balancing destinations
 */
void tmlCollectCallDestinationObjHandler::freeUnsubscribedObjLinkList(){

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "freeUnsubscribedObjLinkList", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :
  DestinationAddressHandlingListElement* element = m_unsubscribedObjLinkListLast;
  while (NULL != element){
    /////////////////////////////////////////
    // Element to be deleted:
    DestinationAddressHandlingListElement* delElement;
    delElement = element;
    /////////////////////////////////////////
    // Delete the containing object:
    delete (element->destinationObj);
    /////////////////////////////////////////
    // Next element:
    element = element->prev;
    /////////////////////////////////////////
    // Delete the link list object:
    delete (delElement);
    if (NULL != element){
      element->next = NULL;
    }
    m_unsubscribedObjLinkListLast = element;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "freeUnsubscribedObjLinkList", "Vortex CMD", "vortex_mutex_unlock");
}

/**
  * @brief    Make a console dump of the "unsubscribed" object link list
  */
void tmlCollectCallDestinationObjHandler::dumpUnsubscribedObjToLinkList(char* sInfo, bool bLockCritical){
  if (bLockCritical){
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "freeUnsubscribedObjLinkList", "Vortex CMD", "vortex_mutex_lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a send command may not be possible during this method execution :
  }
  DestinationAddressHandlingListElement* element = m_unsubscribedObjLinkListLast;

  int iLoop = 0;

  printf ("########################         %s               ###################\n", sInfo);
  if (NULL == element){
    printf ("####### List is empty\n");
  }
  while (NULL != element){
    /////////////////////////////////////////
    // Element to be deleted:
    DestinationAddressHandlingListElement* delElement;
    delElement = element;
    char* sHost;
    element->destinationObj->getMessageDestinationHost(&sHost);
    char* sPort = NULL;
    element->destinationObj->getMessageDestinationPort(&sPort);
    printf ("#### LinkList -Host[%d] = %s / Port[%d] = %s ####\n", iLoop, sHost, iLoop, sPort);
    /////////////////////////////////////////
    // Next element:
    element = element->prev;
  }
  printf ("##########################################################################\n");

  if (bLockCritical){
    /////////////////////////////////////////////////////////////////////////////////////////
    // End of critical section / a send command was not possible during this method execution
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "freeUnsubscribedObjLinkList", "Vortex CMD", "vortex_mutex_unlock");
  }
}


/**
 * @brief    Add unsubscribed load balancing destination object to be freed later on destructor:
 */
void tmlCollectCallDestinationObjHandler::addUnsubscribedObjToLinkList(tmlCollectCallDestinationObj* obj){
  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "addUnsubscribedObjToLinkList", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  //dumpUnsubscribedObjToLinkList("addUnsubscribedObjToLinkList - BEGIN", false);
  if (NULL == m_unsubscribedObjLinkListLast){
    m_unsubscribedObjLinkListLast = new DestinationAddressHandlingListElement;
    m_unsubscribedObjLinkListLast->next = NULL;
    m_unsubscribedObjLinkListLast->prev = NULL;
    m_unsubscribedObjLinkListLast->destinationObj = obj;
  }
  else{
    DestinationAddressHandlingListElement* element;
    element = new DestinationAddressHandlingListElement;
    element->next = NULL;
    element->prev = m_unsubscribedObjLinkListLast;
    element->destinationObj = obj;
    m_unsubscribedObjLinkListLast->next = element;
    m_unsubscribedObjLinkListLast = element;
  }
  //dumpUnsubscribedObjToLinkList("addUnsubscribedObjToLinkList - END  ", false);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSectionLinkList, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unlockUnsubscribedLinkList", "Vortex CMD", "addUnsubscribedObjToLinkList");
}


/**
  * @brief    Get the number of registered destinations / event.
  *
  * @param   profile       URI of the profile for the requested operation.
  * @param   iCount        Reference to the number of registered destinations.
  * @param   bLockCritical Flag, if true mutex will be used
  *
  * @returns TML_SUCCESS in case of success.<br>
  *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
  */
int tmlCollectCallDestinationObjHandler::getSubscribedEventDestinationCount(const char* profile, int* iCount, bool bLockCritical){
  return getSubscribedDestinationCount(m_eventDestinationAddressHashTable, profile, iCount, bLockCritical);
}


/**
  * @brief    Get the number of registered destinations / load balanced.
  *
  * @param   profile       URI of the profile for the requested operation.
  * @param   iCount        Reference to the number of registered destinations.
  * @param   bLockCritical Flag, if true mutex will be used
  *
  * @returns TML_SUCCESS in case of success.<br>
  *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
  */
int tmlCollectCallDestinationObjHandler::getSubscribedLoadBalancedDestinationCount(const char* profile, int* iCount, bool bLockCritical){
  return getSubscribedDestinationCount(m_loadBalancedDestinationAddressHashTable, profile, iCount, bLockCritical);
}


/**
 * @brief    Get the number of registered destinations.
 */
int tmlCollectCallDestinationObjHandler::getSubscribedDestinationCount(TMLUniversalHashTable* hashTable, const char* profile, int* iCount, bool bLockCritical){
  int iRet = TML_SUCCESS;

  if (bLockCritical){
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getSubscribedDestinationCount", "Vortex CMD", "vortex_mutex_lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a send command may not be possible during this method execution :
  }

  int iInternalCount = 0;
  tmlDestinationAddressHandlingHashValue* hashValue;
  void* hashItem;

  hashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    iInternalCount = hashValue->getSubscribedDestinationCount();
  }
  if (0 == iInternalCount){
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }
  *iCount = iInternalCount;

  if (bLockCritical){
    /////////////////////////////////////////////////////////////////////////////////////////
    // End of critical section / a send command was not possible during this method execution
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getSubscribedDestinationCount", "Vortex CMD", "vortex_mutex_unlock");
  }

  return iRet;
}


/**
  * @brief    Get subscribed destinations for event messages for the provided profile.
  */
int tmlCollectCallDestinationObjHandler::GetSubscribedEventMessageDestinations(const char* requestedProfile, SIDEX_VARIANT* subscriptions, bool bLockCritical){
  return GetSubscribedMessageDestinations(m_eventDestinationAddressHashTable, requestedProfile, subscriptions, bLockCritical);
}


/**
  * @brief    Get subscribed destinations for load balanced messages for the provided profile.
  */
int tmlCollectCallDestinationObjHandler::GetSubscribedLoadBalancedMessageDestinations(const char* requestedProfile, SIDEX_VARIANT* subscriptions, bool bLockCritical){
  return GetSubscribedMessageDestinations(m_loadBalancedDestinationAddressHashTable, requestedProfile, subscriptions, bLockCritical);
}


/**
  * @brief    Get subscribed destinations for event / balanced messages for the provided profile.
  */
int tmlCollectCallDestinationObjHandler::GetSubscribedMessageDestinations(TMLUniversalHashTable* hashTable, const char* requestedProfile, SIDEX_VARIANT* subscriptions, bool bLockCritical){
  TML_INT32 iRet = TML_SUCCESS;

  if (bLockCritical){
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "GetSubscribedMessageDestinations", "Vortex CMD", "vortex_mutex_lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a send command may not be possible during this method execution :
  }

  SIDEX_VARIANT sTable = sidex_Variant_New_Table();

  int iProfiles;
  char** sProfiles = NULL;
  char* actualProfile = NULL;
  
  iRet = hashTable->hashSize(&iProfiles);
  if (TML_SUCCESS == iRet){
    iRet = sidex_Variant_Table_AddColumn (sTable, (char*)"PROFILE");
    if (TML_SUCCESS == iRet){
      iRet = sidex_Variant_Table_AddColumn (sTable, (char*)"HOST");
    }
    if (TML_SUCCESS == iRet){
      iRet = sidex_Variant_Table_AddColumn (sTable, (char*)"PORT");
    }
    if (TML_SUCCESS == iRet){
      iRet = hashTable->getKeys(&sProfiles);
    }
  }

  for (int i = 0; TML_SUCCESS == iRet && i < iProfiles; ++i){
    if (NULL == requestedProfile || 0 == strlen(requestedProfile)){
      actualProfile = sProfiles[i];
    }
    else{
      if (0 == strcmp (requestedProfile, sProfiles[i])){
        actualProfile = sProfiles[i];
      }
      else{
        actualProfile = NULL;
      }
    }
    int iSubscriptionCount = 0;
    if (NULL != actualProfile){
      iRet = getSubscribedDestinationCount(hashTable, actualProfile, &iSubscriptionCount, false);
      if (TML_ERR_DESTINATION_OBJ_NOT_FOUND == iRet){
        ///////////////////////////////////////////////////////////////////////////////////////////////
        // TML_ERR_DESTINATION_OBJ_NOT_FOUND is not an error / maybe ist was unregistered in the past:
        iRet = TML_SUCCESS;
      }
    }
    for (int j = 0; iRet == TML_SUCCESS && j < iSubscriptionCount; ++j){
      tmlCollectCallDestinationObj* obj = NULL;
      iRet = getSubscribedMessageDestination(hashTable, actualProfile, j, &obj, false);
      if (TML_SUCCESS == iRet){
        TML_INT32 iRowIdx;
        iRet = sidex_Variant_Table_AddRow (sTable, &iRowIdx);
        SIDEX_VARIANT sVal;
        if (TML_SUCCESS == iRet){
          iRet = sidex_Variant_New_String (actualProfile, &sVal);
        }
        if (TML_SUCCESS == iRet){
          iRet = sidex_Variant_Table_SetField (sTable, iRowIdx, (char*)"PROFILE", sVal);
          sidex_Variant_DecRef(sVal);
        }
        char* cVal;
        obj->getMessageDestinationHost(&cVal);
        if (TML_SUCCESS == iRet){
          iRet = sidex_Variant_New_String (cVal, &sVal);
        }
        if (TML_SUCCESS == iRet){
          iRet = sidex_Variant_Table_SetField (sTable, iRowIdx, (char*)"HOST", sVal);
          sidex_Variant_DecRef(sVal);
        }
        obj->getMessageDestinationPort(&cVal);
        if (TML_SUCCESS == iRet){
          iRet = sidex_Variant_New_String (cVal, &sVal);
        }
        if (TML_SUCCESS == iRet){
          iRet = sidex_Variant_Table_SetField (sTable, iRowIdx, (char*)"PORT", sVal);
          sidex_Variant_DecRef(sVal);
        }
      }
    }
  }

  /////////////////////////////////
  // Now I have to free the memory:
  if (NULL != sProfiles){
    for (int i = 0; i < iProfiles; ++i){
      delete (sProfiles[i]);
    }
    delete sProfiles;
  }
  if (TML_SUCCESS == iRet){
    *subscriptions = sTable;
  }
  else{
    sidex_Variant_DecRef(sTable);
  }

  if (bLockCritical){
    /////////////////////////////////////////////////////////////////////////////////////////
    // End of critical section / a send command was not possible during this method execution
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "GetSubscribedMessageDestinations", "Vortex CMD", "vortex_mutex_unlock");
  }

  return (int)iRet;
}


/**
  * @brief    Get the registered destination for requested index / event.
  */
int tmlCollectCallDestinationObjHandler::getSubscribedEventDestinationByIndex(const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj){
  return getSubscribedDestinationByIndex(m_eventDestinationAddressHashTable, profile, iIndex, destinationObj);
}


/**
  * @brief    Get the registered destination for requested index / load balanced.
  */
int tmlCollectCallDestinationObjHandler::getSubscribedLoadBalancedDestinationByIndex(const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj){
  return getSubscribedDestinationByIndex(m_loadBalancedDestinationAddressHashTable, profile, iIndex, destinationObj);
}


/**
 * @brief    Get the registered destination for requested index.
 */
int tmlCollectCallDestinationObjHandler::getSubscribedDestinationByIndex(TMLUniversalHashTable* hashTable, const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj){
  int iRet = TML_ERR_LOAD_BALANCE_CALCULATION;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getSubscribedDestinationByIndex", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  tmlDestinationAddressHandlingHashValue* hashValue;
  DestinationAddressHandlingListElement* nextLinkElement = NULL;
  void* hashItem;

  hashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    nextLinkElement = hashValue->getLinkList();
  }
  if (NULL != nextLinkElement){
    tmlCollectCallDestinationObj* actualDestObj = NULL;
   
    int iIndexCounter = 0;
    bool bFound = false;
    do{
      actualDestObj = nextLinkElement->destinationObj;
      if (!actualDestObj->isSubscriptionEmpty()){
        if (iIndexCounter == iIndex){
          bFound = true;
        }
        ++iIndexCounter;
      }
      nextLinkElement = nextLinkElement->next;
    }
    while (NULL != nextLinkElement && iIndexCounter <= iIndex);
    if (bFound){
      *destinationObj = actualDestObj;
      iRet = TML_SUCCESS;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getSubscribedDestinationByIndex", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
  * @brief    Get the registered destination parameter at the index for the profile.
  */
int tmlCollectCallDestinationObjHandler::getSubscribedEventMessageDestination(const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj, bool bLockCritical){
  return getSubscribedMessageDestination(m_eventDestinationAddressHashTable, profile, iIndex, destinationObj, bLockCritical);
}


/**
  * @brief    Get the registered destination parameter at the index for the profile.
  */
int tmlCollectCallDestinationObjHandler::getSubscribedLoadBalancedMessageDestination(const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj, bool bLockCritical){
  return getSubscribedMessageDestination(m_loadBalancedDestinationAddressHashTable, profile, iIndex, destinationObj, bLockCritical);
}


/**
 * @brief    Get the registered destination parameter at the index for the profile.
 */
int tmlCollectCallDestinationObjHandler::getSubscribedMessageDestination(TMLUniversalHashTable* hashTable, const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj, bool bLockCritical){

  if (bLockCritical){
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getSubscribedMessageDestination", "Vortex CMD", "vortex_mutex_lock");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin of critical section / a send command may not be possible during this method execution :
  }

  int iRet = TML_SUCCESS;
  tmlCollectCallDestinationObj* actualDestObj = NULL;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  DestinationAddressHandlingListElement* nextLinkElement = NULL;
  void* hashItem;

  hashTable->getValue((char*)profile, &hashItem);
  bool bFound = false;
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    nextLinkElement = hashValue->getLinkList();
  }
  if (NULL != nextLinkElement){
    //////////////////////////////////////////////
    // now count the entrys:
    int iInternalCount = 0;
    do{
      actualDestObj = nextLinkElement->destinationObj;
      if (NULL!= actualDestObj){
        if (actualDestObj->isSubscriptionEmpty()){
          iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
        }
        else{
          if (iInternalCount == iIndex){
            bFound = true;
          }
          else{
            ++iInternalCount;
          }
        }
      }
      else{
        iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
      }
      nextLinkElement = nextLinkElement->next;
    }
    while (TML_SUCCESS == iRet && NULL != nextLinkElement && !bFound);
  }
  if (TML_SUCCESS == iRet && bFound){
    *destinationObj = actualDestObj;
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  if (bLockCritical){
    /////////////////////////////////////////////////////////////////////////////////////////
    // End of critical section / a send command was not possible during this method execution
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getSubscribedMessageDestination", "Vortex CMD", "vortex_mutex_unlock");
  }

  return iRet;
}


/**
 * @brief    Register a callback method for the case of non subscribed destinations.
 */
int tmlCollectCallDestinationObjHandler::loadBalancedRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
  int iRet = TML_SUCCESS;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerPopulateCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  else{
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    m_loadBalancedDestinationAddressHashTable->setValue(hashValue->getProfile(), hashValue);
  }
  //////////////////////////////////////////////////
  // write parameter to the tmlCollectCallDestinationObj:
  hashValue->setBalPopulateCB(pCBFunc, pCBData, iUnicode);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerPopulateCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the registered callback method that will be called in case of an event message to a profile without a subscribed destination.
 */
int tmlCollectCallDestinationObjHandler::getLoadBalancedRegisteredPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode){
  int iRet = TML_SUCCESS;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredPopulateCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  void* hashItem;

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    //////////////////////////////////////////////
    // the data is stored in the first list element so:
    hashValue->getLoadBalancedPopulateCB(pCBFunc, pCBData, iUnicode);
    if (NULL == *pCBFunc){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredPopulateCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Register a callback method for the case of non subscribed destinations.
 */
int tmlCollectCallDestinationObjHandler::eventRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
  int iRet = TML_SUCCESS;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerPopulateCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  else{
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    m_eventDestinationAddressHashTable->setValue(hashValue->getProfile(), hashValue);
  }
  //////////////////////////////////////////////////
  // write parameter to the tmlCollectCallDestinationObj:
  hashValue->setEventPopulateCB(pCBFunc, pCBData, iUnicode);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerPopulateCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the registered callback method that will be called in case of an event message to a profile without a subscribed destination.
 */
int tmlCollectCallDestinationObjHandler::getEventRegisteredPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode){
  int iRet = TML_SUCCESS;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredPopulateCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  void* hashItem;

  m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    //////////////////////////////////////////////
    // the data is stored in the first list element so:
    hashValue->getEventPopulateCB(pCBFunc, pCBData, iUnicode);
    if (NULL == *pCBFunc){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredPopulateCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Register a callback method for the case of a peer subscription / unsubscription request for event / load balanced messages.
 */
int tmlCollectCallDestinationObjHandler::loadBalancedRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode){
  int iRet = TML_SUCCESS;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerOnPeerCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  else{
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    m_loadBalancedDestinationAddressHashTable->setValue(hashValue->getProfile(), hashValue);
  }
  //////////////////////////////////////////////////
  // write parameter to the tmlCollectCallDestinationObj:
  hashValue->setLoadBalancedPeerCB(pCBFunc, pCBData, iUnicode);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerOnPeerCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the registered callback method that will be called in case of a peer subscription / unsubscription request for event / load balanced messages.
 */
int tmlCollectCallDestinationObjHandler::getRegisteredLoadBalancedOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFunc,TML_POINTER* pCBData, tmlUnicodeID* iUnicode){
  int iRet = TML_SUCCESS;

  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredOnPeerCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    //////////////////////////////////////////////
    // the data is stored in the first list element so:
    hashValue->getLoadBalancedPeerCB(pCBFunc, pCBData, iUnicode);
    if (NULL == *pCBFunc){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredOnPeerCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Register a callback method for the case of a peer subscription / unsubscription request for event / load balanced messages.
 */
int tmlCollectCallDestinationObjHandler::eventRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode){
  int iRet = TML_SUCCESS;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerOnPeerCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  else{
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    m_eventDestinationAddressHashTable->setValue(hashValue->getProfile(), hashValue);
  }
  //////////////////////////////////////////////////
  // write parameter to the tmlCollectCallDestinationObj:
  hashValue->setEventPeerCB(pCBFunc, pCBData, iUnicode);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerOnPeerCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the registered callback method that will be called in case of a peer subscription / unsubscription request for event / load balanced messages.
 */
int tmlCollectCallDestinationObjHandler::getRegisteredEventOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFunc,TML_POINTER* pCBData, tmlUnicodeID* iUnicode){
  int iRet = TML_SUCCESS;

  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredOnPeerCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    //////////////////////////////////////////////
    // the data is stored in the first list element so:
    hashValue->getEventPeerCB(pCBFunc, pCBData, iUnicode);
    if (NULL == *pCBFunc){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredOnPeerCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Register a callback method for the BUSY STATUS requests.
 */
int tmlCollectCallDestinationObjHandler::loadBalancedRegisterBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
  int iRet = TML_SUCCESS;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "loadBalancedRegisterBusyStatusCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  else{
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    m_loadBalancedDestinationAddressHashTable->setValue(hashValue->getProfile(), hashValue);
  }
  //////////////////////////////////////////////////
  // write parameter to the tmlCollectCallDestinationObj:
  hashValue->setBusyStatusCB(pCBFunc, pCBData);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "loadBalancedRegisterBusyStatusCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the registered callback method that will be called in case of BUSY STATUS requests.
 */
int tmlCollectCallDestinationObjHandler::getRegisteredBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredBusyStatusCallback", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    //////////////////////////////////////////////
    // the data is stored in the first list element so:
    hashValue->getBusyStatusCB(pCBFunc, pCBData);
    if (NULL == *pCBFunc){
      //////////////////////////////////////////////////////////////////////////////////////////////
      // If there is no callback 
      // map the error to TML_ERR_INFORMATION_UNDEFINED to get round robin:
      // (( This may happen if the "listener" is a C implementatin and the "server" is CPP- wrapper implementatin ))
      iRet = TML_ERR_INFORMATION_UNDEFINED;      
    }
  }
  else{
    //////////////////////////////////////////////////////////////////////////////////////////////
    // If there is no callback 
    // map the error to TML_ERR_INFORMATION_UNDEFINED to get round robin:
    // (( This may happen if the "listener" is a C implementatin and the "server" is CPP- wrapper implementatin ))
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredBusyStatusCallback", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Register a callback method for the load balance calculation.
 */
int tmlCollectCallDestinationObjHandler::registerLoadBalanceCalculator(const char* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc,TML_POINTER pCBData)
{
  int iRet = TML_SUCCESS;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerLoadBalanceCalculator", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  else{
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    m_loadBalancedDestinationAddressHashTable->setValue(hashValue->getProfile(), hashValue);
  }
  //////////////////////////////////////////////////
  // write parameter to the tmlCollectCallDestinationObj:
  hashValue->setLoadBalanceCalculatorCB(pCBFunc, pCBData);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "registerLoadBalanceCalculator", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the registered callback method for the load balance calculation.
 */
int tmlCollectCallDestinationObjHandler::getRegisteredLoadBalanceCalculator(const char* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredLoadBalanceCalculator", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    //////////////////////////////////////////////
    // the data is stored in the first list element so:
    hashValue->getLoadBalanceCalculatorCB(pCBFunc, pCBData);
    if (NULL == *pCBFunc){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getRegisteredLoadBalanceCalculator", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Register a callback method that will be called in case of an error during event message send actions.
 */
int tmlCollectCallDestinationObjHandler::eventRegisterCallbackOnError(const char* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
  int iRet = TML_SUCCESS;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventRegisterCallbackOnError", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);

  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  else{
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    m_eventDestinationAddressHashTable->setValue(hashValue->getProfile(), hashValue);
  }
  ///////////////////////////////////////////////////////////////
  // write parameter to the tmlCollectCallDestinationObj / maybe they are different:
  hashValue->setOnErrorCB(pCBFunc, pCBData, iUnicode);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventRegisterCallbackOnError", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the registered callback method that will be called in case of an error during event message send actions.
 */
int tmlCollectCallDestinationObjHandler::eventGetRegisteredCallbackOnError(const char* profile, TML_ON_EVT_ERROR_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode, bool bLock){
  int iRet = TML_SUCCESS;

  if (m_bInShutdown){
    printf ("#### is shutdown ####\n");
    return TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }
  void* hashItem;

  if (bLock)
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventGetRegisteredCallbackOnError", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    //////////////////////////////////////////////
    // the data is stored in the first list element so:
    hashValue->getOnErrorCB(pCBFunc, pCBData, iUnicode);
    if (NULL == *pCBFunc){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  if (bLock)
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventGetRegisteredCallbackOnError", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
  * @brief    Make a common deregistration / event
  */
int tmlCollectCallDestinationObjHandler::eventDeregisterAll(){
  return deregisterAll(m_eventDestinationAddressHashTable);
}


/**
  * @brief    Make a common deregistration / load balanced
  */
int tmlCollectCallDestinationObjHandler::loadBalancedDeregisterAll(){
  return deregisterAll(m_loadBalancedDestinationAddressHashTable);
}


/**
 * @brief    Make a common deregistration
 */
int tmlCollectCallDestinationObjHandler::deregisterAll(TMLUniversalHashTable* hashTable)
{
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventRegisterCallbackOnQueueOverflow", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :
  char** sKeys;
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  ///////////////////////////////////////////
  // Event message handling hash table:
  int iRet = hashTable->getKeys(&sKeys);
  if (TML_SUCCESS == iRet){
    if (NULL != sKeys){
      int iSize;
      iRet = hashTable->hashSize(&iSize);
      if (TML_SUCCESS == iRet){
        ////////////////////////////////////
        // loop via all keys:
        for (int i = 0; i < iSize; ++i){
          hashTable->getValue(sKeys[i], &hashItem);
          delete (sKeys[i]);
          if (NULL != hashItem){
            tmlDestinationAddressHandlingHashValue* hashValue = NULL;
            //////////////////////////////////////////////
            // a link list entry exists in the hashtable:
            hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
            ///////////////////////////////////////////////////////////////
            // overwrite MrsProblem parameter:
            hashValue->setBalPopulateCB(NULL, NULL, idUNICODE_WCHAR_T);
            hashValue->setBusyStatusCB(NULL, NULL);
            hashValue->setEventPeerCB(NULL, NULL, idUNICODE_WCHAR_T);
            hashValue->setEventPopulateCB(NULL, NULL, idUNICODE_WCHAR_T);
            hashValue->setLoadBalanceCalculatorCB(NULL, NULL);
            hashValue->setLoadBalancedPeerCB(NULL, NULL, idUNICODE_WCHAR_T);
            hashValue->setOnErrorCB(NULL, NULL, idUNICODE_WCHAR_T);
            hashValue->setOnQueueOverflowCB(NULL, NULL, idUNICODE_WCHAR_T);
          }
        }
      }
    }
    delete sKeys;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventRegisterCallbackOnQueueOverflow", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}

/**
 * @brief    Register a callback method that will be called in case of event message queue overflow.
 */
int tmlCollectCallDestinationObjHandler::eventRegisterCallbackOnQueueOverflow(const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode)
{
  int iRet = TML_SUCCESS;
  tmlDestinationAddressHandlingHashValue* hashValue = NULL;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventRegisterCallbackOnQueueOverflow", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);

  if (NULL != hashItem){
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
  }
  else{
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    //////////////////////////////////////////////////
    // create a new link list entry for the hashtable:
    hashValue = new tmlDestinationAddressHandlingHashValue(profile);
    //////////////////////////////////////////////////
    // New Entry for the hashtable:
    m_eventDestinationAddressHashTable->setValue(hashValue->getProfile(), hashValue);
  }
  ///////////////////////////////////////////////////////////////
  // write parameter to the tmlCollectCallDestinationObj / maybe they are different:
  hashValue->setOnQueueOverflowCB(pCBFunc, pCBData, iUnicode);

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventRegisterCallbackOnQueueOverflow", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the registered callback method that will be called in case of event message queue overflow.
 */
int tmlCollectCallDestinationObjHandler::eventGetRegisteredCallbackOnQueueOverflow(const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode){
  int iRet = TML_SUCCESS;

  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventGetRegisteredCallbackOnQueueOverflow", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    //////////////////////////////////////////////
    // the data is stored in the first list element so:
    hashValue->getOnQueueOverflowCB(pCBFunc, pCBData, iUnicode);
    if (NULL == *pCBFunc){
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "eventGetRegisteredCallbackOnQueueOverflow", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    skip to the next load balanced destination (round robin)
 */
int tmlCollectCallDestinationObjHandler::skipNextLoadBalancedDestination(const char* profile){

  int iRet = TML_SUCCESS;

  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "skipNextLoadBalancedDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    tmlDestinationAddressHandlingHashValue* hashValue;
    //////////////////////////////////////////////
    // a link list entry exists in the hashtable:
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    hashValue->skipNextLoadBalancedDestination();
  }
  else{
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "skipNextLoadBalancedDestination", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Get the destinationObj for the next load balances destination
 */
int tmlCollectCallDestinationObjHandler::getNextLoadBalancedDestination(const char* profile, tmlCollectCallDestinationObj** destinationObj)
{
  int iRet = TML_SUCCESS;

  tmlDestinationAddressHandlingHashValue* hashValue;
  void* hashItem;

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getNextLoadBalancedDestination", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
  if (NULL != hashItem){
    hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
    tmlCollectCallDestinationObj* destination = hashValue->getNextLoadBalancedDestination();
    if (NULL != destination){
      *destinationObj = destination;
    }
    else{
      ////////////////////////////////////////////////////////////
      // profile exist in the hashtable but no host - port match:
      iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
    }
  }
  else{
    ////////////////////////////////////////////////////////////
    // No Hash entry:
    iRet = TML_ERR_DESTINATION_OBJ_NOT_FOUND;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "getNextLoadBalancedDestination", "Vortex CMD", "vortex_mutex_unlock");

  return iRet;
}


/**
 * @brief    Unregister failed connection destinations.
 */
void tmlCollectCallDestinationObjHandler::unregisterFailedEventConnectionDestinations(const char* profile, unsigned int iMaxFail, bool bLock){

  if (m_bInShutdown){
    return;
  }
  if (bLock)
    enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unregisterFailedConnectionDestinations", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  if (iMaxFail){
    int iDestinations = 0;
    int iRet = getSubscribedEventDestinationCount(profile, &iDestinations, false);
    if (TML_SUCCESS == iRet){
      tmlDestinationAddressHandlingHashValue* hashValue;
      void* hashItem;

      m_eventDestinationAddressHashTable->getValue((char*)profile, &hashItem);
      if (NULL != hashItem){
        //////////////////////////////////////////////
        // a link list entry exists in the hashtable:
        hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
        tmlCollectCallDestinationObj* failObject;
        bool bFound = false;
        do{
          bFound = hashValue->getNextConnectionFailedDestination(&failObject, iMaxFail);
          if (bFound){
            char* sHost = NULL;
            char* sPort = NULL;
            failObject->getMessageDestinationHost(&sHost);
            failObject->getMessageDestinationPort(&sPort);
            unsubscribeEventMessageDestination(profile, sHost, sPort, false);
          }
        }
        while (bFound);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  if (bLock)
    leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unregisterFailedConnectionDestinations", "Vortex CMD", "vortex_mutex_unlock");
}


/**
 * @brief    Unregister failed connection destinations.
 */
void tmlCollectCallDestinationObjHandler::unregisterFailedLoadBalancedConnectionDestinations(const char* profile, unsigned int iMaxFail){

  enterCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unregisterFailedConnectionDestinations", "Vortex CMD", "vortex_mutex_lock");
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Begin of critical section / a send command may not be possible during this method execution :

  if (iMaxFail){
    int iDestinations = 0;
    int iRet = getSubscribedLoadBalancedDestinationCount(profile, &iDestinations, false);
    if (TML_SUCCESS == iRet){
      tmlDestinationAddressHandlingHashValue* hashValue;
      void* hashItem;

      m_loadBalancedDestinationAddressHashTable->getValue((char*)profile, &hashItem);
      if (NULL != hashItem){
        //////////////////////////////////////////////
        // a link list entry exists in the hashtable:
        hashValue = (tmlDestinationAddressHandlingHashValue*) hashItem;
        tmlCollectCallDestinationObj* failObject;
        bool bFound = false;
        do{
          bFound = hashValue->getNextConnectionFailedDestination(&failObject, iMaxFail);
          if (bFound){
            char* sHost = NULL;
            char* sPort = NULL;
            failObject->getMessageDestinationHost(&sHost);
            failObject->getMessageDestinationPort(&sPort);
            unsubscribeLoadBalancedMessageDestination(profile, sHost, sPort, false);
          }
        }
        while (bFound);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // End of critical section / a send command was not possible during this method execution
  leaveCriticalSection (TML_LOG_VORTEX_MUTEX, &m_mutexCriticalSection, &m_iMutexCriticalSectionCount, "tmlCollectCallDestinationObjHandler", "unregisterFailedConnectionDestinations", "Vortex CMD", "vortex_mutex_unlock");
}


/**
 * @brief    Create the mutex that protect critial section about communication data
 */
axl_bool tmlCollectCallDestinationObjHandler::createCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  axl_bool bSuccess = intern_mutex_create (mutex);
  return bSuccess;
}


/**
 * @brief    Destroy the mutex that protect critial section about communication data
 */
axl_bool tmlCollectCallDestinationObjHandler::destroyCriticalSectionObject(int iLogMask, VortexMutex* mutex, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  axl_bool bSuccess = intern_mutex_destroy (mutex, (char*)"tmlCollectCallDestinationObjHandler::destroyCriticalSectionObject");
  return bSuccess;
}


/**
 * @brief    Enter a critical Section
 */
void tmlCollectCallDestinationObjHandler::enterCriticalSection(int iLogMask, VortexMutex* mutex, int* iLockCount, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  //printf ("#####   tmlCollectCallDestinationObjHandler / enterCriticalSection   #####\n");
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  intern_mutex_lock (mutex, m_log, sMethod);
  return;
}


/**
 * @brief    Leave a critical Section
 */
void tmlCollectCallDestinationObjHandler::leaveCriticalSection(int iLogMask, VortexMutex* mutex, int* iLockCount, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
  m_log->log (iLogMask, sClass, sMethod, sFormatLog, sLog);
  //printf ("#####   tmlCollectCallDestinationObjHandler / leaveCriticalSection   #####\n");
  intern_mutex_unlock (mutex, m_log, sMethod);
  return;
}
