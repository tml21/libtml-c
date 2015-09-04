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
 
#ifndef TMLDESTINATIONADDRESSHANDLINGHASHVALUE_H
#define TMLDESTINATIONADDRESSHANDLINGHASHVALUE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexDefines.h"
#include "tmlCoreDefines.h"

class  tmlDestinationAddressHandlingHashValue
{

private:
    /* data */

    /**
     * @brief    the profile / For event & load balanced
     */
    char*                                           m_sProfile;

    /**
     * @brief    the first element of chain
     */
    DestinationAddressHandlingListElement*          m_linkList;

    /**
     * @brief    the tmlCollectCallDestinationObj in case of a load balanced request
     */
    tmlCollectCallDestinationObj*                   m_nextLoadBalanced;

    /**
     * @brief    reference to callback on populate / For event
     */
    TML_ON_POPULATE_CB_FUNC                         m_pCBFuncEvtPopulate;

    /**
     * @brief    Flag identifies unicode use for TML_ON_POPULATE_CB_FUNC
     */
    tmlUnicodeID                                    m_iUnicodeOnEvtPopulate;

    /**
     * @brief    reference to callback data on populate / For event
     */
    TML_POINTER                                     m_pCBDataEvtPopulate;

    /**
     * @brief    reference to callback on populate / For load balanced
     */
    TML_ON_POPULATE_CB_FUNC                         m_pCBFuncBalPopulate;

    /**
     * @brief    Flag identifies unicode use for TML_ON_POPULATE_CB_FUNC
     */
    tmlUnicodeID                                    m_iUnicodeOnBalPopulate;

    /**
     * @brief    reference to callback data on populate / For load balanced
     */
    TML_POINTER                                     m_pCBDataBalPopulate;

    /**
     * @brief    reference to callback on peer / For event
     */
    TML_ON_PEER_REGISTRATION_CB_FUNC                m_pCBFuncEvtPeer;

    /**
     * @brief    Flag identifies unicode use for TML_ON_PEER_REGISTRATION_CB_FUNC
     */
    tmlUnicodeID                                    m_iUnicodeOnPeerEvtRegistration;

    /**
     * @brief    reference to callback data on peer / For event
     */
    TML_POINTER                                     m_pCBDataEvtPeer;

    /**
     * @brief    reference to callback on peer / For load balanced
     */
    TML_ON_PEER_REGISTRATION_CB_FUNC                m_pCBFuncBalPeer;

    /**
     * @brief    Flag identifies unicode use for TML_ON_PEER_REGISTRATION_CB_FUNC
     */
    tmlUnicodeID                                    m_iUnicodeOnPeerBalRegistration;

    /**
     * @brief    reference to callback data on peer / For load balanced
     */
    TML_POINTER                                     m_pCBDataBalPeer;

    /**
     * @brief    reference to callback for BUSY STATUS requests
     */
    TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC               m_pCBFuncBusyStatus;

    /**
     * @brief    reference to callback data for BUSY STATUS requests
     */
    TML_POINTER                                     m_pCBDataBusyStatus;

    /**
     * @brief    reference to callback to calculate load balancing
     */
    TML_ON_BAL_CALCULATION_REQ_CB_FUNC              m_pCBFuncCalculateLoadBalancing;

    /**
     * @brief    reference to callback data  to calculate load balancing
     */
    TML_POINTER                                     m_pCBDataCalculateLoadBalancing;

    /**
     * @brief    reference to callback on message problem / For event
     */
    TML_ON_EVT_ERROR_CB_FUNC                        m_pCBFuncOnError;

    /**
     * @brief    Flag identifies unicode use for TML_ON_EVT_ERROR_CB_FUNC
     */
    tmlUnicodeID                                    m_iUnicodeOnErrorCall;

    /**
     * @brief    reference to callback data on message problem / For event
     */
    TML_POINTER                                     m_pCBDataOnError;

    /**
     * @brief    reference to callback on queue overflow / For event
     */
    TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC               m_pCBFuncOnQueueOverflow;

    /**
     * @brief    Flag identifies unicode use for TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC
     */
    tmlUnicodeID                                    m_iUnicodeOnQueueOverflow;

    /**
     * @brief    reference to callback data on queue overflow / For event
     */
    TML_POINTER                                     m_pCBDataOnQueueOverflow;

    /* methods */

public:
    /* data */

    /* methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlCoreWrapper
     */
    explicit tmlDestinationAddressHandlingHashValue(const char* profile);


    /**
     * @brief    Destructor.
     */
    ~tmlDestinationAddressHandlingHashValue();


    /**
     * @brief Get the profile.
     *
     * @returns             Reference to URI of the profile for the requested operation.<br>
     *                      It is owened by the tmlDestinationAddressHandlingHashValue so it has not to be freed.<br>
     *                      In case of NULL the parameter is not defined.
     */
    char* getProfile();


    /**
     * @brief    Get reference to the link list that holds the destination structures.
     *
     * @returns   reference to the link list.
     */
    DestinationAddressHandlingListElement* getLinkList();

    /**
     * @brief    Set reference to the link list that holds the destination structures.
     *
     * @param   linkList  reference to the link list.
     */
    void setLinkList(DestinationAddressHandlingListElement* linkList);


    /**
     * @brief    Set reference to callback method to peer registration / event.
     *
     * @param   pCBFuncPeer  reference to the callback method.
     * @param   pCBDataPeer  reference to the callback methods data.
     * @param   iUnicode     unicode identifier.
     */
    void setEventPeerCB(TML_ON_PEER_REGISTRATION_CB_FUNC pCBFuncPeer, TML_POINTER pCBDataPeer, tmlUnicodeID iUnicode);


    /**
     * @brief    Get reference to callback method to peer registration / event.
     *
     * @param   pCBFuncPeer  reference to the callback method.
     * @param   pCBDataPeer  reference to the callback methods data.
     * @param   iUnicode     reference to unicode identifier.
     */
    void getEventPeerCB(TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFuncPeer, TML_POINTER* pCBDataPeer, tmlUnicodeID* iUnicode);


    /**
     * @brief    Set reference to callback method to peer registration / load balanced.
     *
     * @param   pCBFuncPeer  reference to the callback method.
     * @param   pCBDataPeer  reference to the callback methods data.
     * @param   iUnicode     unicode identifier.
     */
    void setLoadBalancedPeerCB(TML_ON_PEER_REGISTRATION_CB_FUNC pCBFuncPeer, TML_POINTER pCBDataPeer, tmlUnicodeID iUnicode);


    /**
     * @brief    Get reference to callback method to peer registration / load balanced.
     *
     * @param   pCBFuncPeer  reference to the callback method.
     * @param   pCBDataPeer  reference to the callback methods data.
     * @param   iUnicode     reference to unicode identifier.
     */
    void getLoadBalancedPeerCB(TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFuncPeer, TML_POINTER* pCBDataPeer, tmlUnicodeID* iUnicode);


    /**
     * @brief    Set reference to callback method to populate registration / event.
     *
     * @param   pCBFuncPopulate  reference to the callback method.
     * @param   pCBDataPopulate  reference to the callback methods data.
     * @param   iUnicode         reference to unicode identifier.
     */
    void setEventPopulateCB(TML_ON_POPULATE_CB_FUNC pCBFuncPopulate, TML_POINTER pCBDataPopulate, tmlUnicodeID iUnicode);


    /**
     * @brief    Get reference to callback method to populate registration / event.
     *
     * @param   pCBFuncPopulate  reference to the callback method.
     * @param   pCBDataPopulate  reference to the callback methods data.
     * @param   iUnicode         reference to unicode identifier.
     */
    void getEventPopulateCB(TML_ON_POPULATE_CB_FUNC* pCBFuncPopulate, TML_POINTER* pCBDataPopulate, tmlUnicodeID* iUnicode);


    /**
     * @brief    Set reference to callback method to populate registration / load balanced.
     *
     * @param   pCBFuncPopulate  reference to the callback method.
     * @param   pCBDataPopulate  reference to the callback methods data.
     * @param   iUnicode         unicode identifier.
     */
    void setBalPopulateCB(TML_ON_POPULATE_CB_FUNC pCBFuncPopulate, TML_POINTER pCBDataPopulate, tmlUnicodeID iUnicode);


    /**
     * @brief    Get reference to callback method to populate registration / load balanced.
     *
     * @param   pCBFuncPopulate  reference to the callback method.
     * @param   pCBDataPopulate  reference to the callback methods data.
     * @param   iUnicode         reference to unicode identifier.
     */
    void getLoadBalancedPopulateCB(TML_ON_POPULATE_CB_FUNC* pCBFuncPopulate, TML_POINTER* pCBDataPopulate, tmlUnicodeID* iUnicode);


    /**
     * @brief    Set reference to callback method for the BUSY STATUS requests.
     *
     * @param   pCBFuncBusyStatus  reference to the callback method.
     * @param   pCBDataBusyStatus  reference to the callback methods data.
     */
    void setBusyStatusCB(TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFuncBusyStatus, TML_POINTER pCBDataBusyStatus);


    /**
     * @brief    get reference to callback method for the BUSY STATUS requests.
     *
     * @param   pCBFuncBusyStatus  reference to the callback method.
     * @param   pCBDataBusyStatus  reference to the callback methods data.
     */
    void getBusyStatusCB(TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC* pCBFuncBusyStatus, TML_POINTER* pCBDataBusyStatus);


    /**
     * @brief    Set reference to callback method to calculate load balancing.
     *
     * @param   pCBFuncCalculateLoadBalancing  reference to the callback method.
     * @param   pCBDataCalculateLoadBalancing  reference to the callback methods data.
     */
    void setLoadBalanceCalculatorCB(TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFuncCalculateLoadBalancing, TML_POINTER pCBDataCalculateLoadBalancing);


    /**
     * @brief    Get reference to callback method to calculate load balancing.
     *
     * @param   pCBFuncCalculateLoadBalancing  reference to the callback method.
     * @param   pCBDataCalculateLoadBalancing  reference to the callback methods data.
     */
    void getLoadBalanceCalculatorCB(TML_ON_BAL_CALCULATION_REQ_CB_FUNC* pCBFuncCalculateLoadBalancing, TML_POINTER* pCBDataCalculateLoadBalancing);


    /**
     * @brief    Set reference to callback method on message problem
     *
     * @param   pCBFuncOnError  reference to the callback method.
     * @param   pCBDataOnError  reference to the callback methods data.
     * @param   iUnicode        unicode identifier.
     */
    void setOnErrorCB(TML_ON_EVT_ERROR_CB_FUNC pCBFuncOnError, TML_POINTER pCBDataOnError, tmlUnicodeID iUnicode);


    /**
     * @brief    Get reference to callback method on message problem
     *
     * @param   pCBFuncOnError  reference to the callback method.
     * @param   pCBDataOnError  reference to the callback methods data.
     * @param   iUnicode        reference to unicode identifier.
     */
    void getOnErrorCB(TML_ON_EVT_ERROR_CB_FUNC* pCBFuncOnError, TML_POINTER* pCBDataOnError, tmlUnicodeID* iUnicode);


    /**
     * @brief    Set reference to callback method on event message queue overflow
     *
     * @param   pCBFuncOnQueueOverflow  reference to the callback method.
     * @param   pCBDataOnQueueOverflow  reference to the callback methods data.
     * @param   iUnicode                unicode identifier.
     */
    void setOnQueueOverflowCB(TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFuncOnQueueOverflow, TML_POINTER pCBDataOnQueueOverflow, tmlUnicodeID iUnicode);


    /**
     * @brief    Get reference to callback method on message problem
     *
     * @param   pCBFuncOnQueueOverflow  reference to the callback method.
     * @param   pCBDataOnQueueOverflow  reference to the callback methods data.
     * @param   iUnicode                reference to unicode identifier.
     */
    void getOnQueueOverflowCB(TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC* pCBFuncOnQueueOverflow, TML_POINTER* pCBDataOnQueueOverflow, tmlUnicodeID* iUnicode);


    /**
     * @brief    Set the attribute for the next load balances destination
     *
     * @param   setElement reference to the destination object.
     */
    void setNextLoadBalancedDestination(tmlCollectCallDestinationObj* setElement);

    /**
     * @brief    Get the attribute for the next load balances destination
     *
     * @returns  the destination object.
     */
    tmlCollectCallDestinationObj* getNextLoadBalancedDestination();


    /**
     * @brief    skip load balanced destination if it matches to the requested destination.
     *
     * @param   actObj     Reference to the destination that has to be checkedfor a match.
     */
    void skipLoadBalancedDestinationByMatch(tmlCollectCallDestinationObj* actObj);


    /**
     * @brief    Get the number of registered destinations for the profile.
     *
     * @returns the number of registered destinations.
     */
    int getSubscribedDestinationCount();


    /**
     * @brief    skip to the next load balanced destination (round robin)
     */
    void skipNextLoadBalancedDestination();


    /**
     * @brief    Get the next connection with a failcounter more or equals iMaxFail.
     *
     * This call will unregister all connection destinations with a failCounter equals or more than iMaxFail.
     * If iMaxFail is 0 nothing will be done.
     *
     * @param   failObject  Reference to the next destination object for the requested iMaxFail.
     * @param   iMaxFail    The number of failed connections / if 0 nothing will be done.
     *
     * @returns true if a destination for the requested iMaxFail was found, otherwise false.
     */
    bool getNextConnectionFailedDestination(tmlCollectCallDestinationObj** failObject, unsigned int iMaxFail);
};


#endif //TMLDESTINATIONADDRESSHANDLINGHASHVALUE_H

