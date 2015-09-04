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

#ifndef TMLCOLLECTCALLDESTINATIONOBJHANDLER_H
#define TMLCOLLECTCALLDESTINATIONOBJHANDLER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlCoreDefines.h"
#include "tmlUniversalHashTable.h"
#include "tmlDestinationAddressHandlingHashValue.h"

class  tmlCollectCallDestinationObjHandler
{
private:
    /* data */

    /**
     * @brief    Flaf shows destruction is in process
     */
    bool  m_bInShutdown;

    /**
     * @brief    Hash table for the event message handling
     */
    TMLUniversalHashTable* m_eventDestinationAddressHashTable;

    /**
     * @brief    Hash table for the load balanced message handling
     */
    TMLUniversalHashTable* m_loadBalancedDestinationAddressHashTable;


    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;


    /**
     * @brief    A mutex to protect critial sections
     */
    VortexMutex       m_mutexCriticalSection;

    /**
     * @brief    Number of recursive critial section calls
     */
    int               m_iMutexCriticalSectionCount;


    /**
     * @brief    A mutex to protect critial sections
     */
    VortexMutex       m_mutexCriticalSectionLinkList;

    /**
     * @brief    Number of recursive critial section calls
     */
    int               m_iMutexCriticalSectionLinkListCount;


    /**
     * @brief    Link list of tmlCollectCallDestinationObj that have to be free on destructor
     */
    DestinationAddressHandlingListElement* m_unsubscribedObjLinkListLast;


    /* methods */

    /**
     * @brief    Empty and free the hashtable
     *
     * @param   hashTable     Reference to TMLUniversalHashTable for the operation
     */
    void freeMessageHandlingHashTable(TMLUniversalHashTable* hashTable);


    /**
     * @brief    Wait, until pending event messages are sent
     */
    void waitForPendingEventMessages();

    /**
     * @brief    Wait, until pending event messages are sent for the given profile index
     *
     * @param   hashTable     Reference to profile index / will be incremented inside
     *
     * @returns true, if more profiles are available
     */
    bool waitForPendingEventMessagesProfileIndex(int* index);


    /**
     * @brief    Empty and free the link list for unsubscribed load balancing destinations
     */
    void freeUnsubscribedObjLinkList();


    /**
     * @brief    Add unsubscribed load balancing destination object to be freed later on destructor
     *
     * @param   obj     Reference to load balancing destination object to add to link list
     */
    void addUnsubscribedObjToLinkList(tmlCollectCallDestinationObj* obj);


    /**
     * @brief    Subscribe a destination.
     *
     * @param   hashTable   Reference to TMLUniversalHashTable for the operation
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     * @param   log         Reference to loghandler
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int subscribeMessageDestination(TMLUniversalHashTable* hashTable, const char* profile, const char* sHost, const char* sPort, tmlLogHandler* log);


    /**
     * @brief    Unsubscribe a destination.
     *
     * @param   hashTable     Reference to TMLUniversalHashTable for the operation
     * @param   profile       URI of the profile for the requested operation.
     * @param   sHost         The host name / IP number.
     * @param   sPort         The Port.
     * @param   bLockCritical Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if the requested parameter are not found.
     *
     * @see tmlErrors.h
     */
    int unsubscribeMessageDestination(TMLUniversalHashTable* hashTable, const char* profile, const char* sHost, const char* sPort, bool bLockCritical);


    /**
     * @brief    Check whether destination is subsrcibed
     *
     * @param   hashTable   Reference to TMLUniversalHashTable for the operation
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns true if destination is subsrcibed
     */
    bool acquireMessageDestination(TMLUniversalHashTable* hashTable, const char* profile, const char* sHost, const char* sPort);


    /**
     * @brief    Get subscribed destinations for event / balanced messages for the provided profile.
     *
     * @param   hashTable         Reference to TMLUniversalHashTable for the operation
     * @param   requestedProfile  Profile name of the request.<br>
     *                            If empty or TML_HANDLE_TYPE_NULL the subscriptions for all known profiles will be returned.
     * @param   subscriptions     reference to a SIDEX_VARIANT / to be interpreted as a table containing the requested result.<br>
     *                            The table has three columns with the names "PROFILE", "HOST" and "PORT".<br>
     *                            The table cell contents are SIDEX_VARIANT values of the type SIDEX_DATA_TYPE_STRING.
     * @param   bLockCritical     Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see sidexStdTypes.h, tmlErrors.h
     */
    int GetSubscribedMessageDestinations(TMLUniversalHashTable* hashTable, const char* requestedProfile, SIDEX_VARIANT* subscriptions, bool bLockCritical);


    /**
     * @brief    Get the number of registered destinations.
     *
     * @param   hashTable     Reference to TMLUniversalHashTable for the operation
     * @param   profile       URI of the profile for the requested operation.
     * @param   iCount        Reference to the number of registered destinations.
     * @param   bLockCritical Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     */
    int getSubscribedDestinationCount(TMLUniversalHashTable* hashTable, const char* profile, int* iCount, bool bLockCritical);


    /**
     * @brief    Get the registered destination for requested index.
     *
     * @param   hashTable       Reference to TMLUniversalHashTable for the operation
     * @param   profile         URI of the profile for the requested operation.
     * @param   iIndex          The index of the subscription list (base 0).
     * @param   destinationObj  Reference to the tmlCollectCallDestinationObj.
     */
    int getSubscribedDestinationByIndex(TMLUniversalHashTable* hashTable, const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj);


    /**
     * @brief    Get the registered destination parameter at the index for the profile.
     *
     * @param   hashTable       Reference to TMLUniversalHashTable for the operation
     * @param   profile          URI of the profile for the requested operation.
     * @param   iIndex           The index of the subscription list (base 0).
     * @param   destinationObj   Reference to the tmlCollectCallDestinationObj.
     * @param   bLockCritical    Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     */
    int getSubscribedMessageDestination(TMLUniversalHashTable* hashTable, const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj, bool bLockCritical);


    /**
     * @brief    Make a common deregistration
     *
     * @param   hashTable       Reference to TMLUniversalHashTable for the operation
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int deregisterAll(TMLUniversalHashTable* hashTable);


public:
    /* data */

    /* methods */


    /**
     * @brief    Constructor.
     *
     * @param   loghandler   Reference to a log handler.
     *
     * @returns an instance of tmlCollectCallDestinationObjHandler
     */
    explicit tmlCollectCallDestinationObjHandler(tmlLogHandler* loghandler);


    /**
     * @brief    Destructor.
     */
    ~tmlCollectCallDestinationObjHandler();


    /**
     * @brief    Make a console dump of the "unsubscribed" object link list
     *
     * @param   sInfo         Additional info that will be printed
     * @param   bLockCritical Flag, if true mutex will be used
     */
    void dumpUnsubscribedObjToLinkList(char* sInfo, bool bLockCritical);


    /**
     * @brief    Returns the reference to the unsubscribed link list / will lock it's access
     */
    DestinationAddressHandlingListElement** acquireUnsubscribedLinkList();


    /**
     * @brief    unlocks exlusive access to unsubscribed link list
     */
    void unlockUnsubscribedLinkList();


    /**
     * @brief    Subscribe a destination / event.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     * @param   log         Reference to loghandler
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int subscribeEventMessageDestination(const char* profile, const char* sHost, const char* sPort, tmlLogHandler* log);


    /**
     * @brief    Subscribe a destination / loadBalanced.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     * @param   log         Reference to loghandler
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int subscribeLoadBalancedMessageDestination(const char* profile, const char* sHost, const char* sPort, tmlLogHandler* log);


    /**
     * @brief    Check whether destination is subsrcibed
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns true if destination is subsrcibed
     */
    bool acquireMessageDestination(const char* profile, const char* sHost, const char* sPort);


    /**
     * @brief    Unsubscribe a destination / event.
     *
     * @param   profile       URI of the profile for the requested operation.
     * @param   sHost         The host name / IP number.
     * @param   sPort         The Port.
     * @param   bLockCritical Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if the requested parameter are not found.
     *
     * @see tmlErrors.h
     */
    int unsubscribeEventMessageDestination(const char* profile, const char* sHost, const char* sPort, bool bLockCritical);


    /**
     * @brief    Unsubscribe a destination / load balanced.
     *
     * @param   profile       URI of the profile for the requested operation.
     * @param   sHost         The host name / IP number.
     * @param   sPort         The Port.
     * @param   bLockCritical Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if the requested parameter are not found.
     *
     * @see tmlErrors.h
     */
    int unsubscribeLoadBalancedMessageDestination(const char* profile, const char* sHost, const char* sPort, bool bLockCritical);


    /**
     * @brief    Unsubscribe all destinations for the provided profile / event.
     *
     * @param   profile       Profile name of the request.<br>
     *                        If empty or TML_HANDLE_TYPE_NULL all known profiles will be unsubscribed.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int unsubscribeAllEventMessageDestinations(const char* profile);


    /**
     * @brief    Unsubscribe all destinations for the provided profile / load balanced.
     *
     * @param   profile       Profile name of the request.<br>
     *                        If empty or TML_HANDLE_TYPE_NULL all known profiles will be unsubscribed.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int unsubscribeAllLoadBalancedMessageDestinations(const char* profile);


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
    int getSubscribedEventDestinationCount(const char* profile, int* iCount, bool bLockCritical);


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
    int getSubscribedLoadBalancedDestinationCount(const char* profile, int* iCount, bool bLockCritical);


    /**
     * @brief    Get subscribed destinations for event messages for the provided profile.
     *
     * @param   requestedProfile  Profile name of the request.<br>
     *                            If empty or TML_HANDLE_TYPE_NULL the subscriptions for all known profiles will be returned.
     * @param   subscriptions     reference to a SIDEX_VARIANT / to be interpreted as a table containing the requested result.<br>
     *                            The table has three columns with the names "PROFILE", "HOST" and "PORT".<br>
     *                            The table cell contents are SIDEX_VARIANT values of the type SIDEX_DATA_TYPE_STRING.
     * @param   bLockCritical     Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see sidexStdTypes.h, tmlErrors.h
     */
    int GetSubscribedEventMessageDestinations(const char* requestedProfile, SIDEX_VARIANT* subscriptions, bool bLockCritical);


    /**
     * @brief    Get subscribed destinations for load balanced messages for the provided profile.
     *
     * @param   requestedProfile  Profile name of the request.<br>
     *                            If empty or TML_HANDLE_TYPE_NULL the subscriptions for all known profiles will be returned.
     * @param   subscriptions     reference to a SIDEX_VARIANT / to be interpreted as a table containing the requested result.<br>
     *                            The table has three columns with the names "PROFILE", "HOST" and "PORT".<br>
     *                            The table cell contents are SIDEX_VARIANT values of the type SIDEX_DATA_TYPE_STRING.
     * @param   bLockCritical     Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see sidexStdTypes.h, tmlErrors.h
     */
    int GetSubscribedLoadBalancedMessageDestinations(const char* requestedProfile, SIDEX_VARIANT* subscriptions, bool bLockCritical);


    /**
     * @brief    Get the registered destination for requested index / event.
     *
     * @param   profile         URI of the profile for the requested operation.
     * @param   iIndex          The index of the subscription list (base 0).
     * @param   destinationObj  Reference to the tmlCollectCallDestinationObj.
     */
    int getSubscribedEventDestinationByIndex(const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj);


    /**
     * @brief    Get the registered destination for requested index / load balanced.
     *
     * @param   profile         URI of the profile for the requested operation.
     * @param   iIndex          The index of the subscription list (base 0).
     * @param   destinationObj  Reference to the tmlCollectCallDestinationObj.
     */
    int getSubscribedLoadBalancedDestinationByIndex(const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj);


    /**
     * @brief    Get the registered destination parameter at the index for the profile.
     *
     * @param   profile          URI of the profile for the requested operation.
     * @param   iIndex           The index of the subscription list (base 0).
     * @param   destinationObj   Reference to the tmlCollectCallDestinationObj.
     * @param   bLockCritical    Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     */
    int getSubscribedEventMessageDestination(const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj, bool bLockCritical);


    /**
     * @brief    Get the registered destination parameter at the index for the profile.
     *
     * @param   profile          URI of the profile for the requested operation.
     * @param   iIndex           The index of the subscription list (base 0).
     * @param   destinationObj   Reference to the tmlCollectCallDestinationObj.
     * @param   bLockCritical    Flag, if true mutex will be used
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     */
    int getSubscribedLoadBalancedMessageDestination(const char* profile, int iIndex, tmlCollectCallDestinationObj** destinationObj, bool bLockCritical);


    /**
     * @brief    Register a callback method for the case of non subscribed destinations.
     *
     * This will register a callback method that will be called in case of a message to a profile without a subscribed destination.<br>
     * The callback returns the result of the destination registration and will be called in the manner:<br>
     * TML_INT32 pCBFunc (char* sProfile, TML_INT32 pCBData)     
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     * @param   iUnicode   Unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Get the registered callback method that will be called in case of a message to a profile without a subscribed destination.
     *
     * In case of an undefined destination for the requested profile the callback will be called in the following manner:
     * pCBFunc (char* sProfile, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     * @param   iUnicode   Reference to unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getEventRegisteredPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode);


    /**
     * @brief    Register a callback method for the case of non subscribed destinations.
     *
     * This will register a callback method that will be called in case of a message to a profile without a subscribed destination.<br>
     * The callback returns the result of the destination registration and will be called in the manner:<br>
     * TML_INT32 pCBFunc (char* sProfile, TML_INT32 pCBData)     
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     * @param   iUnicode   Unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedRegisterPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Get the registered callback method that will be called in case of a message to a profile without a subscribed destination.
     *
     * In case of an undefined destination for the requested profile the callback will be called in the following manner:
     * pCBFunc (char* sProfile, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     * @param   iUnicode   Reference to unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getLoadBalancedRegisteredPopulateCallback(const char* profile, TML_ON_POPULATE_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode);


    /**
     * @brief    Register a callback method for the case of a peer subscription / unsubscription request for event / load balanced messages.
     *
     * This will register a callback method that will be called in case of a peer subscription request for event / load balanced messages.<br>
     * The callback will be called in the manner:<br>
     * TML_BOOL pCBFunc (TML_BOOL bSubscribe, char* sHost, char* sPort, TML_POINTER pCBData)<br>
     * -bSubscribe  TML_TRUE in case of subscription / TML_FALSE in case of unsubscription.<br>
     * -sHost       host identification.<br>
     * -sPort       port identification.<br>
     * -pCBData     Reference to additional parameter of the callback method /may be NULL.<br>
     * Returns TML_TRUE if the subscription / unsubscription should be performed, otherwise TML_FALSE.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     * @param   iUnicode   Unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.
     *
     * @see tmlErrors.h
     */
    int loadBalancedRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Get the registered callback method that will be called in case of a peer subscription / unsubscription request for event / load balanced messages.
     *
     * In case of a peer subscription / unsubscription request for event / load balanced messages for the requested profile the callback will be called in the following manner:
     * pCBFunc (char* sProfile, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     * @param   iUnicode   Reference to unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getRegisteredLoadBalancedOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode);


    /**
     * @brief    Register a callback method for the case of a peer subscription / unsubscription request for event / load balanced messages.
     *
     * This will register a callback method that will be called in case of a peer subscription request for event / load balanced messages.<br>
     * The callback will be called in the manner:<br>
     * TML_BOOL pCBFunc (TML_BOOL bSubscribe, char* sHost, char* sPort, TML_POINTER pCBData)<br>
     * -bSubscribe  TML_TRUE in case of subscription / TML_FALSE in case of unsubscription.<br>
     * -sHost       host identification.<br>
     * -sPort       port identification.<br>
     * -pCBData     Reference to additional parameter of the callback method /may be NULL.<br>
     * Returns TML_TRUE if the subscription / unsubscription should be performed, otherwise TML_FALSE.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     * @param   iUnicode   Unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.
     *
     * @see tmlErrors.h
     */
    int eventRegisterOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Get the registered callback method that will be called in case of a peer subscription / unsubscription request for event / load balanced messages.
     *
     * In case of a peer subscription / unsubscription request for event / load balanced messages for the requested profile the callback will be called in the following manner:
     * pCBFunc (char* sProfile, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     * @param   iUnicode   Reference to unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getRegisteredEventOnPeerCallback(const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode);


    /**
     * @brief    Register a callback method for the BUSY STATUS requests.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedRegisterBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Get the registered callback method that will be called in case of BUSY STATUS requests.
     *
     * In case of an undefined destination for the requested profile the callback will be called in the following manner:
     * pCBFunc (char* sProfile, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getRegisteredBusyStatusCallback(const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Register a callback method for the load balance calculation.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int registerLoadBalanceCalculator(const char* profile,TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Get the registered callback method for the load balance calculation.
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int getRegisteredLoadBalanceCalculator(const char* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Register a callback method that will be called in case of an error during event message send actions.
     *
     * In case of an error during message processing the callback will be called in the following manner:
     * TML_INT32 pCBFunc (char* sProfile, char* sHost, char* sPort, TML_COMMAND_ID_TYPE iCMD, TML_INT32 iErrorCode, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.
     * @param   iUnicode   Unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventRegisterCallbackOnError(const char* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Get the register callback method that will be called in case of an error during event message send actions.
     *
     * In case of an error during message processing the callback will be called in the following manner:
     * pCBFunc (char* sProfile, char* sHost, char* sPort, TML_INT32 iCMD, TML_INT32 iErrorCode, TML_POINTER pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     * @param   iUnicode   Reference to unicode identifier.
     * @param   bLock      Flag, if true mutex will be used.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int eventGetRegisteredCallbackOnError(const char* profile, TML_ON_EVT_ERROR_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode, bool bLock);


    /**
     * @brief    Make a common deregistration / event
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventDeregisterAll();


    /**
     * @brief    Make a common deregistration / load balanced
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int loadBalancedDeregisterAll();


    /**
     * @brief    Register a callback method that will be called in case of event message queue overflow.
     *
     * If the event message processing is slower than the time of incoming event message requests,<br>
     * they will be queued internal. If the number of queue elements is more than a maximum, the oldest queue<br>
     * entry will be removed. In that case a registered callback will be called.
     *
     * The callback will be called in the manner:<br>
     * void pCBFunc (char* sProfile, TML_COMMAND_ID_TYPE iCMD, TML_INT32 pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method  has no parameter.
     * @param   iUnicode   Unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int eventRegisterCallbackOnQueueOverflow(const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData, tmlUnicodeID iUnicode);


    /**
     * @brief    Get the registered callback method that will be called in case of event message queue overflow.
     *
     * If the event message processing is slower than the time of incoming event message requests,<br>
     * they will be queued internal. If the number of queue elements is more than a maximum, the oldest queue<br>
     * entry will be removed. In that case a registered callback will be called.
     *
     * The callback will be called in the manner:<br>
     * void pCBFunc (char* sProfile, TML_COMMAND_ID_TYPE iCMD, TML_INT32 pCBData)
     *
     * @param   profile    URI of the profile for the requested operation.
     * @param   pCBFunc    Reference to store the reference to the callback method.
     * @param   pCBData    Reference to store the reference to parameter of the callback method.
     * @param   iUnicode   Reference to unicode identifier.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DESTINATION_OBJ_NOT_FOUND if any of the registrations has missing parameter.
     *
     * @see tmlErrors.h
     */
    int eventGetRegisteredCallbackOnQueueOverflow(const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC* pCBFunc, TML_POINTER* pCBData, tmlUnicodeID* iUnicode);

    /**
     * @brief    skip to the next load balanced destination (round robin)
     *
     * @param   profile    URI of the profile for the requested operation.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int skipNextLoadBalancedDestination(const char* profile);


    /**
     * @brief    Get the destinationObj for the next load balances destination
     *
     * @param   profile        URI of the profile for the requested operation.
     * @param   destinationObj Reference to the tmlCollectCallDestinationObj.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int getNextLoadBalancedDestination(const char* profile, tmlCollectCallDestinationObj** destinationObj);


    /**
     * @brief    Unregister failed connection destinations.
     *
     * This call will unregister all connection destinations with an internal failcounter more or equals than iMaxFail.
     * If iMaxFail is 0 nothing will be done.
     *
     * @param   profile        URI of the profile for the requested operation.
     * @param   iMaxFail       The number of failed connections / if 0 nothing will be done.
     * @param   bLock          Flag, if true mutex will be used.
     */
    void unregisterFailedEventConnectionDestinations(const char* profile, unsigned int iMaxFail, bool bLock);


    /**
     * @brief    Unregister failed connection destinations.
     *
     * This call will unregister all connection destinations with an internal failcounter more or equals than iMaxFail.
     * If iMaxFail is 0 nothing will be done.
     *
     * @param   profile        URI of the profile for the requested operation.
     * @param   iMaxFail       The number of failed connections / if 0 nothing will be done.
     */
    void unregisterFailedLoadBalancedConnectionDestinations(const char* profile, unsigned int iMaxFail);

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
     * @param   iLockCount  Reference to number of locks
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     */
    void enterCriticalSection(int iLogMask, VortexMutex* mutex, int* iLockCount, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    Leave a critical Section
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   iLockCount  Reference to number of locks
     * @param   mutex       Reference to mutex for the critical section
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     */
    void leaveCriticalSection(int iLogMask, VortexMutex* mutex, int* iLockCount, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);

};
#endif //TMLCOLLECTCALLDESTINATIONOBJHANDLER_H

