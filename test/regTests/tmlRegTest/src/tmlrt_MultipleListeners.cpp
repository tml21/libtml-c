/* 
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2016 wobe-systems GmbH
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

#include <iostream>
using namespace std;
#include <sidex.h>
#include <tmlCore.h>
#include "tmlrt_Utils.h"
#include "TestingForReturns.h"
#include "TestingProcess.h"
#include "TmlCore.h"


/** @ingroup Test_Multiple_Listeners
* @brief Testing a simple communication between a sender and a listener
* 
* A TestingProcess is started and the Listener- and SenderSides are initialized. 
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then five Commands are initialized and send to the different Ports. Afterwards the Commands
* and TMLCores are freed.
*/
void simpleTestTmlMultiListenerSendSyncMessage() {
	TestingProcess multiListenerSendSyncMessage = TestingProcess(tmlrtT("MultiListenerSendSyncMessage"));
	multiListenerSendSyncMessage.defaultListenerInit();
	multiListenerSendSyncMessage.initSenderSide();

	multiListenerSendSyncMessage.sendArbitraryCmds(true);
	multiListenerSendSyncMessage.freeCmds();
	multiListenerSendSyncMessage.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing a simple async communication between a sender and listeners
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then five Commands are initialized and send asynchronously to the different Ports. Afterwards the Commands
* and TMLCores are freed.
*/
void simpleTestTmlMultiListenerSendAsyncMessage() {
	TestingProcess multiListenerSendAsyncMessage = TestingProcess(tmlrtT("multiListenerSendAsyncMessage"));
	multiListenerSendAsyncMessage.defaultListenerInit();
	multiListenerSendAsyncMessage.initSenderSide();

	multiListenerSendAsyncMessage.sendArbitraryCmds(false);

	multiListenerSendAsyncMessage.waitForAsyncReplies();
	multiListenerSendAsyncMessage.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing a simple Loadbalanced communication between a sender and listeners
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then some listeners are subscribed as loadbalanced listeners and some subscribe themselves
* as laodbalanced listeners. Afterwards five commands are created and send to the profile,
* some are send synchronously, some asynchronously.
* The commands and TMLCores are freed afterwards.
*/
void testTmlMultiListenerLoadBalancingMessages() {
	TestingProcess multiListenerLoadBalancingMessages = TestingProcess(tmlrtT("multiListenerLoadBalancingMessages"));
	multiListenerLoadBalancingMessages.defaultListenerInit();
	multiListenerLoadBalancingMessages.initSenderSide();

	bool loadbalancing = true;
	bool synchronous = true;
	//mixed Subscribing and SubscriptionRequests
	multiListenerLoadBalancingMessages.subscribeListenersForLoadBalancingOrEvents(loadbalancing);

	//send cmds
	multiListenerLoadBalancingMessages.sendArbitraryLoadBalancedCmds(!synchronous);
	multiListenerLoadBalancingMessages.sendArbitraryLoadBalancedCmds(synchronous);

	multiListenerLoadBalancingMessages.freeCmds();
	multiListenerLoadBalancingMessages.waitForAsyncReplies();
	multiListenerLoadBalancingMessages.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the amount and addresses of the Listeners are equal to the originals.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then it is tested, whether the amount of Listeners at the ListenerSides Core is really five.
* And it is checked whether the Listeners addresses (ListenerSide) is equal to the addresses, that 
* they were created with.
* Afterwards the TMLCores of both Sides are freed.
*/
void simpleTestTmlMultiListener() {
	TestingProcess simpleMultiListenerTest = TestingProcess(tmlrtT("simpleMultiListenerTest"));
	simpleMultiListenerTest.defaultListenerInit();
	simpleMultiListenerTest.initSenderSide();
	//hardcoded, there should be 5 Listeners now
	simpleMultiListenerTest.checkListenerCount(5);
	simpleMultiListenerTest.checkListenerAdresses();
	
	simpleMultiListenerTest.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the amount of Listeners decreases after one is closed.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then it is tested, whether the amount of Listeners at the ListenerSides Core decreases after
* one of them is closed.
* Afterwards the TMLCores of both Sides are freed.
*/
void testTmlCoreListenerClose() {
	TestingProcess testListenerClose = TestingProcess(tmlrtT("testListenerClose"));
	testListenerClose.defaultListenerInit();
	testListenerClose.initSenderSide();

	testListenerClose.checkForDecreasedListenersAfterOneIsClosed();
	//here lies a race condition, just happens on linux
	testListenerClose.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether a Listener is really disabled after being disabled.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then a Listener of the ListenerSide is disabled and it is checked, if it
* really is disabled. Also it is checked whether the amount of Listeners changed, because it should not.
* Then async and sync cmds are send, expecting errors. Afterwards the listener is enabled again and 
* sync and async cmds are send.
* Afterwards the TMLCores of both Sides are freed.
*/
void testTmlCoreListenerGetSetEnabled() {
	TML_BOOL enable = TML_FALSE;
	TestingProcess testListenerGetSetEnabled = TestingProcess(tmlrtT("testListenerGetSetEnabled"));
	testListenerGetSetEnabled.defaultListenerInit();
	testListenerGetSetEnabled.initSenderSide();

	//disable a listener
	testListenerGetSetEnabled.disEnableListener(3, enable);
	testListenerGetSetEnabled.checkIfListenerDisabled(3);

	testListenerGetSetEnabled.checkListenerCount(5);
	//send async and sync cmds
	testListenerGetSetEnabled.checkAsyncSyncMessagesForErrors(3);
	testListenerGetSetEnabled.freeCmds();
	testListenerGetSetEnabled.waitForAsyncReplies();
	//enable listener again
	enable = TML_TRUE;
	testListenerGetSetEnabled.disEnableListener(3, enable);

	//send another sync and async cmd
	bool synchronous = true;
	testListenerGetSetEnabled.sendArbitraryCmds(!synchronous);
	testListenerGetSetEnabled.sendArbitraryCmds(synchronous);
	testListenerGetSetEnabled.freeCmds();
	
	testListenerGetSetEnabled.waitForAsyncReplies();
	testListenerGetSetEnabled.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the functions tml_core_Listener_Create and _Listener_Close return expected error codes.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the function tml_Core_Listener_Create is called with param coreHandle = NULL, and it is checked whether
* the function returns the expected error code.
* The same is done for tml_Listener_Close. 
* Then, depending on the system (Unicode or not) function tml_Core_Listener_Create is called with param 
* address = NULL and the expected error code is checked, or nothing is tested.
* Afterwards the TMLCores of both Sides are freed.
*/
void testTmlCoreListenerCreateCloseErrorCodes() {
	SIDEX_TCHAR* address = tmlrtT("0.0.0.0:1234");
	TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
	TML_INT32 apiReturns = 0;
	TmlCore* tmlCore = TML_HANDLE_TYPE_NULL;

	TestingProcess testListenerCreateClose = TestingProcess(tmlrtT("testListenerCreateClose"));
	testListenerCreateClose.defaultListenerInit();
	testListenerCreateClose.initSenderSide();

	apiReturns = tml_Core_Listener_Create(TML_HANDLE_TYPE_NULL, address, &listenerHandle);
	testListenerCreateClose.m_iErr = apiReturns;
	testListenerCreateClose.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	tmlCore = testListenerCreateClose.m_coreListenerSide;
	apiReturns = tml_Core_Listener_Create(tmlCore->getCore(), tmlrtT("21"), &listenerHandle);
	testListenerCreateClose.m_iErr = apiReturns;
	testListenerCreateClose.checkForExpectedReturnCode(TML_ERR_NET_BINDING);

	apiReturns = tml_Listener_Close(&listenerHandle);
	testListenerCreateClose.m_iErr = apiReturns;
	testListenerCreateClose.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

#if defined SIDEX_UNICODE || defined TML_UNICODE
	tmlCore = testListenerCreateClose.m_coreSenderSide;
	apiReturns = tml_Core_Listener_Create(tmlCore->getCore(), NULL, &listenerHandle);
	testListenerCreateClose.m_iErr = apiReturns;
	testListenerCreateClose.checkForExpectedReturnCode(TML_ERR_UNICODE);
#endif

	testListenerCreateClose.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the function tml_Listener_Get_Address returns expected error codes.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the function tml_Listener_Get_Address is called with param coreHandle = NULL and it is tested
* whether it returns the expected error code.
* Afterwards the TMLCores of both Sides are freed.
*/
void testTmlListenerGetAddressErrorCodes() {
	SIDEX_TCHAR* address;
	TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
	TML_INT32 apiReturns = 0;

	TestingProcess testListenerGetAddress = TestingProcess(tmlrtT("testListenerGetAddress"));
	testListenerGetAddress.initSenderSide();
	testListenerGetAddress.defaultListenerInit();

	apiReturns = tml_Listener_Get_Address(TML_HANDLE_TYPE_NULL, &address);
	testListenerGetAddress.m_iErr = apiReturns;
	testListenerGetAddress.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	testListenerGetAddress.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the function tml_Core_Get_ListenerCount returns expected error codes.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the function tml_Core_Get_ListenerCount is called with param coreHandle = NULL and it is tested
* whether it returns the expected error code.
* Afterwards the TMLCores of both Sides are freed.
*/
void testTmlCoreGetListenerCountErrorCodes() {
	TML_INT32 apiReturns = 0;
	TML_UINT32 count = 0;

	TestingProcess testCoreGetListenerCount = TestingProcess(tmlrtT("testCoreGetListenerCount"));
	testCoreGetListenerCount.initSenderSide();
	testCoreGetListenerCount.defaultListenerInit();

	apiReturns = tml_Core_Get_ListenerCount(TML_HANDLE_TYPE_NULL, &count);
	testCoreGetListenerCount.m_iErr = apiReturns;
	testCoreGetListenerCount.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	testCoreGetListenerCount.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the function tml_Core_Get_Listener returns expected error codes.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the function tml_Core_Get_Listener is called with param coreHandle = NULL and it is tested
* whether it returns the expected error code.
* Then it is called again, with a valid coreHandle but param address = NULL and it is checked whether
* it returns the expected error code. It is done again with address being an index, that is out of bounds 
* for the listeners, and it is tested whether it returns the expected error code.
* Afterwards the TMLCores of both Sides are freed.
*/
void testTmlCoreGetListenerErrorCodes() {
	TML_INT32 apiReturns = 0;
	TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
	TML_UINT32 indexOfListenerHandle = 0;
	TmlCore* tmlCore = TML_HANDLE_TYPE_NULL;
	TML_UINT32 indexOutOfBounds = 7;

	TestingProcess testCoreGetListener = TestingProcess(tmlrtT("testCoreGetListener"));
	testCoreGetListener.defaultListenerInit();
	testCoreGetListener.initSenderSide();

	apiReturns = tml_Core_Get_Listener(TML_HANDLE_TYPE_NULL, indexOfListenerHandle, &listenerHandle);
	testCoreGetListener.m_iErr = apiReturns;
	testCoreGetListener.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	tmlCore = testCoreGetListener.m_coreSenderSide;

	apiReturns = tml_Core_Get_Listener(tmlCore->getCore(), indexOutOfBounds, &listenerHandle);
	testCoreGetListener.m_iErr = apiReturns;
	testCoreGetListener.checkForExpectedReturnCode(TML_ERR_INFORMATION_UNDEFINED);

	testCoreGetListener.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the functions tml_Listener_Get_Enabled and _Set_Enabled return expected error codes.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the function tml_Listener_Set_Enabled is called with param coreHandle = NULL and it is tested
* whether it returns the expected error code. The same is done with tml_Listener_Get_Enabled.
* Afterwards the TMLCores of both Sides are freed.
*/
void testTmlListenerGetSetEnabledForErrorCodes() {
	TML_INT32 apiReturns = 0;
	TML_BOOL listenerEnabled = true;

	TestingProcess testListenerGetListener = TestingProcess(tmlrtT("testListenerGetListener"));
	testListenerGetListener.defaultListenerInit();
	testListenerGetListener.initSenderSide();

	apiReturns = tml_Listener_Set_Enabled(TML_HANDLE_TYPE_NULL, listenerEnabled);
	testListenerGetListener.m_iErr = apiReturns;
	testListenerGetListener.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	apiReturns = tml_Listener_Get_Enabled(TML_HANDLE_TYPE_NULL, &listenerEnabled);
	testListenerGetListener.m_iErr = apiReturns;
	testListenerGetListener.checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	testListenerGetListener.freeTmlCores();
}

/** @ingroup Test_Multiple_Listeners
* @brief Testing a simple Event communication.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the listeners are subscribed for all cmd-events. This is followed by sending all cmds
* as events.
* Afterwards the TMLCores of both Sides are freed.
*/
void testTmlMultiListenerEventMessages() {
	bool loadbalancing = false;
	TestingProcess multiListenerEventMessages = TestingProcess(tmlrtT("multiListenerEventMessages"));
	multiListenerEventMessages.defaultListenerInit();
	multiListenerEventMessages.initSenderSide();

	//mixed Subscribing and SubscriptionRequests
	multiListenerEventMessages.subscribeListenersForLoadBalancingOrEvents(loadbalancing);	
	multiListenerEventMessages.sendEventMessages();

	TmlSleep(5000);	//important to let the listeners catch up their queue, otherwise error 45
	multiListenerEventMessages.freeCmds();
	multiListenerEventMessages.freeTmlCores();
}