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

#include "tmlDestinationAddressHandlingHashValue.h"

/**
 * @brief    Constructor.
 */
tmlDestinationAddressHandlingHashValue::tmlDestinationAddressHandlingHashValue(const char* profile)
{
  m_sProfile = NULL;

  // Profile:
  int iSize;

  if (NULL != m_sProfile)
    delete (m_sProfile);
  iSize = (int)strlen(profile)+1;
  m_sProfile = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
  strcpy (m_sProfile, profile);
#else
  strcpy_s (m_sProfile, iSize, profile);
#endif

  m_linkList = NULL;
  m_nextLoadBalanced = NULL;
  m_pCBFuncOnError = NULL;
  m_pCBDataOnError = NULL;
  m_iUnicodeOnErrorCall = idUNICODE_WCHAR_T;
  m_pCBFuncOnQueueOverflow = NULL;
  m_pCBDataOnQueueOverflow = NULL;
  m_iUnicodeOnQueueOverflow = idUNICODE_WCHAR_T;

  m_pCBFuncBalPopulate = NULL;
  m_pCBDataBalPopulate = NULL;
  m_iUnicodeOnBalPopulate = idUNICODE_WCHAR_T;
  m_pCBFuncBalPeer = NULL;
  m_pCBDataBalPeer = NULL;

  m_pCBFuncEvtPopulate = NULL;
  m_pCBDataEvtPopulate = NULL;
  m_iUnicodeOnEvtPopulate = idUNICODE_WCHAR_T;
  m_pCBFuncEvtPeer = NULL;
  m_pCBDataEvtPeer = NULL;

  m_iUnicodeOnPeerBalRegistration = idUNICODE_WCHAR_T;
  m_iUnicodeOnPeerEvtRegistration = idUNICODE_WCHAR_T;
  m_pCBFuncBusyStatus = NULL;
  m_pCBDataBusyStatus = NULL;
  m_pCBFuncCalculateLoadBalancing = NULL;
  m_pCBDataCalculateLoadBalancing = NULL;
}


/**
 * @brief    Destructor.
 */
tmlDestinationAddressHandlingHashValue::~tmlDestinationAddressHandlingHashValue()
{
  if (NULL != m_sProfile)
    delete[] m_sProfile;
  m_sProfile = NULL;
}



/**
 * @brief Get the profile.
 */
char* tmlDestinationAddressHandlingHashValue::getProfile(){
  return m_sProfile;
}


/**
 * @brief    Set reference to the link list that holds the destination structures.
 */
void tmlDestinationAddressHandlingHashValue::setLinkList(DestinationAddressHandlingListElement* linkList){
  m_linkList = linkList;
}


/**
 * @brief    Get reference to the link list that holds the destination structures.
 */
DestinationAddressHandlingListElement* tmlDestinationAddressHandlingHashValue::getLinkList(){
  return m_linkList;
}


/**
 * @brief    Set reference to callback method to populate registration.
 */
void tmlDestinationAddressHandlingHashValue::setEventPopulateCB(TML_ON_POPULATE_CB_FUNC pCBFuncPopulate, TML_POINTER pCBDataPopulate, tmlUnicodeID iUnicode){
  m_pCBFuncEvtPopulate = pCBFuncPopulate;
  m_pCBDataEvtPopulate = pCBDataPopulate;
  m_iUnicodeOnEvtPopulate = iUnicode;
}


/**
 * @brief    Get reference to callback method to populate registration.
 */
void tmlDestinationAddressHandlingHashValue::getEventPopulateCB(TML_ON_POPULATE_CB_FUNC* pCBFuncPopulate, TML_POINTER* pCBDataPopulate, tmlUnicodeID* iUnicode){
  *pCBFuncPopulate = m_pCBFuncEvtPopulate;
  *pCBDataPopulate = m_pCBDataEvtPopulate;
   *iUnicode = m_iUnicodeOnEvtPopulate;
}


/**
 * @brief    Set reference to callback method to populate registration.
 */
void tmlDestinationAddressHandlingHashValue::setBalPopulateCB(TML_ON_POPULATE_CB_FUNC pCBFuncPopulate, TML_POINTER pCBDataPopulate, tmlUnicodeID iUnicode){
  m_pCBFuncBalPopulate = pCBFuncPopulate;
  m_pCBDataBalPopulate = pCBDataPopulate;
  m_iUnicodeOnBalPopulate = iUnicode;
}


/**
 * @brief    Get reference to callback method to populate registration.
 */
void tmlDestinationAddressHandlingHashValue::getLoadBalancedPopulateCB(TML_ON_POPULATE_CB_FUNC* pCBFuncPopulate, TML_POINTER* pCBDataPopulate, tmlUnicodeID* iUnicode){
  *pCBFuncPopulate = m_pCBFuncBalPopulate;
  *pCBDataPopulate = m_pCBDataBalPopulate;
   *iUnicode = m_iUnicodeOnBalPopulate;
}


/**
 * @brief    Set reference to callback method to peer registration.
 */
void tmlDestinationAddressHandlingHashValue::setEventPeerCB(TML_ON_PEER_REGISTRATION_CB_FUNC pCBFuncPeer, TML_POINTER pCBDataPeer, tmlUnicodeID iUnicode){
  m_pCBFuncEvtPeer = pCBFuncPeer;
  m_pCBDataEvtPeer = pCBDataPeer;
  m_iUnicodeOnPeerEvtRegistration = iUnicode;
}


/**
 * @brief    Get reference to callback method to peer registration.
 */
void tmlDestinationAddressHandlingHashValue::getEventPeerCB(TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFuncPeer, TML_POINTER* pCBDataPeer, tmlUnicodeID* iUnicode){
  *pCBFuncPeer = m_pCBFuncEvtPeer;
  *pCBDataPeer = m_pCBDataEvtPeer;
  *iUnicode = m_iUnicodeOnPeerEvtRegistration;
}


/**
 * @brief    Set reference to callback method to peer registration.
 */
void tmlDestinationAddressHandlingHashValue::setLoadBalancedPeerCB(TML_ON_PEER_REGISTRATION_CB_FUNC pCBFuncPeer, TML_POINTER pCBDataPeer, tmlUnicodeID iUnicode){
  m_pCBFuncBalPeer = pCBFuncPeer;
  m_pCBDataBalPeer = pCBDataPeer;
  m_iUnicodeOnPeerBalRegistration = iUnicode;
}


/**
 * @brief    Get reference to callback method to peer registration.
 */
void tmlDestinationAddressHandlingHashValue::getLoadBalancedPeerCB(TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFuncPeer, TML_POINTER* pCBDataPeer, tmlUnicodeID* iUnicode){
  *pCBFuncPeer = m_pCBFuncBalPeer;
  *pCBDataPeer = m_pCBDataBalPeer;
  *iUnicode = m_iUnicodeOnPeerBalRegistration;
}


/**
 * @brief    Set reference to callback method for the BUSY STATUS requests.
 */
void tmlDestinationAddressHandlingHashValue::setBusyStatusCB(TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFuncBusyStatus, TML_POINTER pCBDataBusyStatus){
  m_pCBFuncBusyStatus = pCBFuncBusyStatus;
  m_pCBDataBusyStatus = pCBDataBusyStatus;
}


/**
 * @brief    get reference to callback method for the BUSY STATUS requests.
 */
void tmlDestinationAddressHandlingHashValue::getBusyStatusCB(TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC* pCBFuncBusyStatus, TML_POINTER* pCBDataBusyStatus){
  *pCBFuncBusyStatus = m_pCBFuncBusyStatus;
  *pCBDataBusyStatus = m_pCBDataBusyStatus;
}


/**
 * @brief    Set reference to callback method to calculate load balancing.
 */
void tmlDestinationAddressHandlingHashValue::setLoadBalanceCalculatorCB(TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFuncCalculateLoadBalancing, TML_POINTER pCBDataCalculateLoadBalancing){
  m_pCBFuncCalculateLoadBalancing = pCBFuncCalculateLoadBalancing;
  m_pCBDataCalculateLoadBalancing = pCBDataCalculateLoadBalancing;
}


/**
 * @brief    Get reference to callback method to calculate load balancing.
 */
void tmlDestinationAddressHandlingHashValue::getLoadBalanceCalculatorCB(TML_ON_BAL_CALCULATION_REQ_CB_FUNC* pCBFuncCalculateLoadBalancing, TML_POINTER* pCBDataCalculateLoadBalancing){
  *pCBFuncCalculateLoadBalancing = m_pCBFuncCalculateLoadBalancing;
  *pCBDataCalculateLoadBalancing = m_pCBDataCalculateLoadBalancing;
}


/**
 * @brief    Set reference to callback method on message problem
 */
void tmlDestinationAddressHandlingHashValue::setOnErrorCB(TML_ON_EVT_ERROR_CB_FUNC pCBFuncOnError, TML_POINTER pCBDataOnError, tmlUnicodeID iUnicode){
  m_pCBFuncOnError = pCBFuncOnError;
  m_pCBDataOnError = pCBDataOnError;
  m_iUnicodeOnErrorCall = iUnicode;
}


/**
 * @brief    Get reference to callback method on message problem
 */
void tmlDestinationAddressHandlingHashValue::getOnErrorCB(TML_ON_EVT_ERROR_CB_FUNC* pCBFuncOnError, TML_POINTER* pCBDataOnError, tmlUnicodeID* iUnicode){
  *pCBFuncOnError = m_pCBFuncOnError;
  *pCBDataOnError = m_pCBDataOnError;
  *iUnicode = m_iUnicodeOnErrorCall;
}


/**
 * @brief    Set reference to callback method on event message queue overflow
 */
void tmlDestinationAddressHandlingHashValue::setOnQueueOverflowCB(TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFuncOnQueueOverflow, TML_POINTER pCBDataOnQueueOverflow, tmlUnicodeID iUnicode){
  m_pCBFuncOnQueueOverflow = pCBFuncOnQueueOverflow;
  m_pCBDataOnQueueOverflow = pCBDataOnQueueOverflow;
  m_iUnicodeOnQueueOverflow = iUnicode;
}


/**
 * @brief    Get reference to callback method on message problem
 */
void tmlDestinationAddressHandlingHashValue::getOnQueueOverflowCB(TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC* pCBFuncOnQueueOverflow, TML_POINTER* pCBDataOnQueueOverflow, tmlUnicodeID* iUnicode){
  *pCBFuncOnQueueOverflow = m_pCBFuncOnQueueOverflow;
  *pCBDataOnQueueOverflow = m_pCBDataOnQueueOverflow;
  *iUnicode = m_iUnicodeOnQueueOverflow;
}

    
/**
 * @brief    Set the attribute for the next load balances destination
 */
void tmlDestinationAddressHandlingHashValue::setNextLoadBalancedDestination(tmlCollectCallDestinationObj* setElement)
{
  m_nextLoadBalanced = setElement;
}

    
/**
 * @brief    Get the attribute for the next load balances destination
 */
tmlCollectCallDestinationObj* tmlDestinationAddressHandlingHashValue::getNextLoadBalancedDestination()
{
  return m_nextLoadBalanced;
}

    
/**
     * @brief    skip load balanced destination if it matches to the requested destination.
 */
void tmlDestinationAddressHandlingHashValue::skipLoadBalancedDestinationByMatch(tmlCollectCallDestinationObj* actObj){
 if (actObj == m_nextLoadBalanced){
   // The one that will be removed is the next load balanced destination,
   // so skip:
   skipNextLoadBalancedDestination();
   // In case of only one element the load balance ref has to be reset:
   if (m_nextLoadBalanced == actObj){
     m_nextLoadBalanced = NULL;
   }
  }
}

    
/**
 * @brief    Get the number of registered destinations.
 */
int tmlDestinationAddressHandlingHashValue::getSubscribedDestinationCount(){
  int iInternalCount = 0;
  DestinationAddressHandlingListElement* nextElement;

  //////////////////////////////////////////////
  // entry exists in the hashtable:
  nextElement = m_linkList;
  if (NULL != nextElement){
    //////////////////////////////////////////////
    // now count the entrys:
    tmlCollectCallDestinationObj* destinationObj = NULL;
    do{
      destinationObj = nextElement->destinationObj;
      if (NULL != destinationObj){
        if (!destinationObj->isSubscriptionEmpty()){
          ++iInternalCount;
        }
      }
      nextElement = nextElement->next;
    }
    while (NULL != nextElement);
  }
  return iInternalCount;
}


/**
 * @brief    skip to the next load balanced destination (round robin)
 */
void tmlDestinationAddressHandlingHashValue::skipNextLoadBalancedDestination(){
  int iCount;
  iCount = getSubscribedDestinationCount();
  if (iCount > 1){
    DestinationAddressHandlingListElement* actElement = m_linkList;
    bool bFound = false;
    do{
      if (actElement->destinationObj == m_nextLoadBalanced){
        bFound = true;
        if (NULL == actElement->next){
          // It's the last, so I must point to the first:
          m_nextLoadBalanced = m_linkList->destinationObj;
        }
        else{
          m_nextLoadBalanced = actElement->next->destinationObj;
        }
      }
      actElement = actElement->next;
    } while (!bFound && NULL != actElement);
  }
}

    
/**
 * @brief    Get the next connection with a failcounter more or equals iMaxFail.
 */
bool tmlDestinationAddressHandlingHashValue::getNextConnectionFailedDestination(tmlCollectCallDestinationObj** failObject, unsigned int iMaxFail){
  DestinationAddressHandlingListElement* nextElement;
  tmlCollectCallDestinationObj* destinationObj = NULL;
  bool bFound = false;

  //////////////////////////////////////////////
  // entry exists in the hashtable:
  nextElement = m_linkList;
  if (NULL != nextElement){
    do{
      destinationObj = nextElement->destinationObj;
      if (NULL != destinationObj){
        if (!destinationObj->isSubscriptionEmpty()){
          if (iMaxFail <= destinationObj->getFailCounter()){
            bFound = true;
          }
        }
      }
      nextElement = nextElement->next;
    }
    while (!bFound && NULL != nextElement);
    if (bFound){
      *failObject = destinationObj;
    }
  }
  return bFound;
}
