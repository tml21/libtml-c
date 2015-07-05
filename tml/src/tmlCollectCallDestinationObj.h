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

#ifndef TMLCOLLECTCALLDESTINATIONOBJ_H
#define TMLCOLLECTCALLDESTINATIONOBJ_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlLogHandler.h"
#include <vortex.h>

// Prototype:
class tmlEventCall;

class  tmlCollectCallDestinationObj
{

private:
    /* data */

    /**
     * @brief    object that handels synchron event messages to the destination
     */
    tmlEventCall*      m_eventCall;

    /**
     * @brief    the profile / For event & load balanced
     */
    char*              m_sProfile;

    /**
     * @brief    the host / For event & load balanced
     */
    char*              m_sHost;

    /**
     * @brief    the port / For event & load balanced
     */
    char*              m_sPort;

    /**
     * @brief    counter for the number of trys to connect / send messages
     */
    unsigned int       m_iFailCounter;

    /**
     * @brief    reference to loghandler
     */
    tmlLogHandler*     m_log;

    /**
     * @brief    Flag shows the maked to be remove state
     */
    bool   m_bMarkedToBeRemoved;

    /**
     * @brief    Actual event counter
     */
    TML_INT64   m_iEventCounter;

    /**
     * @brief    Actual event counter
     */
    TML_INT64   m_iLastEventLog;

    /**
     * @brief    Period for event log
     */
    unsigned int   m_iEventLogPeriod;

    /**
     * @brief    Actual event counter
     */
    TML_INT64   m_iEventQueueRemoveCounter;

    /**
     * @brief    Last remove event counter
     */
    TML_INT64   m_iLastRemoveEventLog;

    /* methods */

public:
    /* data */

    /* methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlCollectCallDestinationObj
     */
    tmlCollectCallDestinationObj(const char* profile, tmlLogHandler* log);


    /**
     * @brief    Destructor.
     */
    ~tmlCollectCallDestinationObj();


    /**
     * @brief Set all connection relevant params.
     *
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     */
    void subscribeMessageDestination(const char* sHost, const char* sPort);


    /**
     * @brief Get the destination profile.
     *
     * @param   profile     Reference to URI of the profile for the requested operation.<br>
     *                      It is owened by the tmlCollectCallDestinationObj so it has not to be freed.<br>
     *                      In case of NULL the parameter is not defined.
     */
    void getMessageDestinationProfile(char** profile);


    /**
     * @brief Get the destination host.
     *
     * @param   sHost       Reference to the host name / IP number.<br>
     *                      It is owened by the tmlCollectCallDestinationObj so it has not to be freed.<br>
     *                      In case of NULL the parameter is not defined.
     */
    void getMessageDestinationHost(char**sHost);


    /**
     * @brief Get the destination port.
     *
     * @param   sPort       Reference to the Port.<br>
     *                      It is owened by the tmlCollectCallDestinationObj so it has not to be freed.<br>
     *                      In case of NULL the parameter is not defined.
     */
    void getMessageDestinationPort(char** sPort);


    /**
     * @brief Inform about start of an Event message
     *
     * @returns true if a message was printed
     */
    bool eventMessageStartInfo();


    /**
    * @brief Inform about end of an Event message
     *
     * @param bLogPrinted Flag, if true a log will be printed
     */
    void eventMessageDoneInfo(bool bLogPrinted);


    /**
     * @brief Check for equality of this destination with the requested parameter.
     *
     * @param   profile     URI of the profile for the requested operation.
     *
     * @returns true in case of equality, otherwise false.
     */
    bool isEqual(const char* profile);


    /**
     * @brief Check if message destination is subscribed
     *
     * @returns true in case of a missing subscription, otherwise false.
     */
    bool isSubscriptionEmpty();


    /**
     * @brief Check for equality of this destination with the requested parameter.
     *
     * @param   profile     URI of the profile for the requested operation.
     * @param   sHost       The host name / IP number.
     * @param   sPort       The Port.
     *
     * @returns true in case of equality, otherwise false.
     */
    bool isEqual(const char* profile, const char* sHost, const char* sPort);


    /**
     * @brief Increment the number of failed message requests to the receiver.
     */
    void incrementFailCounter();


    /**
     * @brief Resets the number of failed message requests to the receiver.
     */
    void resetFailCounter();


    /**
     * @brief Returns the number of failed message requests to the receiver.
     */
    unsigned int getFailCounter();


    /**
     * @brief    Returns the tmlEventCall object
     */
    tmlEventCall* getTmlEventCaller();


    /**
     * @brief    Returns markedToBeRemove flag
     */
    bool isMarkedToBeRemoved();


    /**
     * @brief    set markedToBeRemove flag
     */
    void setMarkedToBeRemoved(bool bMarkedToBeRemoved);


    /**
     * @brief    Get the Event Queue remove counter
     */
    TML_INT64 getEventQueueRemoveCounter();


    /**
     * @brief    Get the Last remove Event log counter
     */
    TML_INT64 getLastRemoveEventLogCounter();


    /**
     * @brief    Set the Last remove Event log counter
     */
    void setLastRemoveEventLogCounter(TML_INT64 counter);
};


#endif //TMLCOLLECTCALLDESTINATIONOBJ_H

