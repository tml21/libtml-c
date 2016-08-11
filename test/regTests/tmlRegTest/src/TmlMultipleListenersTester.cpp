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

#include "TmlMultipleListenersTester.h"
#include "TestParams.h"

//------------------------------------------------------------------------------

TmlMultipleListenersTester::TmlMultipleListenersTester(SIDEX_TCHAR* testProcessName)
                          : TestingProcess(testProcessName)
{
}

TmlMultipleListenersTester::~TmlMultipleListenersTester()
{
}

//------------------------------------------------------------------------------

void TmlMultipleListenersTester::_prepare()
{
  TestingProcess::_prepare();

  if(!TestParams || !TestParams->hasParams())
  {
    errorOutput(tmlrtT("TestParams doesn't exist!"));
  }
  else
  {
    if(TestParams->getNetworkCardCount() == 0)
    {
      errorOutput(tmlrtT("No network cards specified!"));
    }
  }
}

void TmlMultipleListenersTester::_cleanup()
{

  TestingProcess::_cleanup();
}

//------------------------------------------------------------------------------

/** @ingroup Test_Multiple_Listeners
* @brief Testing a simple communication between a sender and a listener
* 
* A TestingProcess is started and the Listener- and SenderSides are initialized. 
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then five Commands are initialized and send to the different Ports. Afterwards the Commands
* and TMLCores are freed.
*/
bool TmlMultipleListenersTester::simpleTestTmlMultiListenerSendSyncMessage()
{
  setTestSectionName(tmlrtT("simpleTestTmlMultiListenerSendSyncMessage"));
  messageOutput(S_START);
  reset();

	defaultListenerInit();
	initSenderSide();
	
  sendArbitraryCmds(true);
	
	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

/** @ingroup Test_Multiple_Listeners
* @brief Testing a simple async communication between a sender and listeners
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then five Commands are initialized and send asynchronously to the different Ports. Afterwards the Commands
* and TMLCores are freed.
*/
bool TmlMultipleListenersTester::simpleTestTmlMultiListenerSendAsyncMessage()
{
  setTestSectionName(tmlrtT("simpleTestTmlMultiListenerSendAsyncMessage"));
  messageOutput(S_START);
  reset();

	defaultListenerInit();
	initSenderSide();

  sendArbitraryCmds(false);

  waitForAsyncReplies();
  freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

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
bool TmlMultipleListenersTester::testTmlMultiListenerLoadBalancingMessages()
{
  setTestSectionName(tmlrtT("testTmlMultiListenerLoadBalancingMessages"));
  messageOutput(S_START);
  reset();

	defaultListenerInit();
	initSenderSide();

	//mixed Subscribing and SubscriptionRequests
	bool loadbalancing = true;
	subscribeListenersForLoadBalancingOrEvents(loadbalancing);

	//send cmds
	bool synchronous = true;
	sendArbitraryLoadBalancedCmds(!synchronous);
	sendArbitraryLoadBalancedCmds(synchronous);

	freeCmds();
	waitForAsyncReplies();
	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

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
bool TmlMultipleListenersTester::simpleTestTmlMultiListener()
{
  setTestSectionName(tmlrtT("simpleTestTmlMultiListener"));
  messageOutput(S_START);
  reset();

	defaultListenerInit();
	initSenderSide();
	//hardcoded, there should be 5 Listeners now
	checkListenerCount(5);
	checkListenerAdresses();

	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the amount of Listeners decreases after one is closed.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then it is tested, whether the amount of Listeners at the ListenerSides Core decreases after
* one of them is closed.
* Afterwards the TMLCores of both Sides are freed.
*/
bool TmlMultipleListenersTester::testTmlCoreListenerClose()
{
  setTestSectionName(tmlrtT("testTmlCoreListenerClose"));
  messageOutput(S_START);
  reset();

	defaultListenerInit();
	initSenderSide();

	checkForDecreasedListenersAfterOneIsClosed();
	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

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
bool TmlMultipleListenersTester::testTmlCoreListenerGetSetEnabled()
{
  setTestSectionName(tmlrtT("testTmlCoreListenerGetSetEnabled"));
  messageOutput(S_START);
  reset();

	defaultListenerInit();
	initSenderSide();

	//disable a listener
	TML_BOOL enable = TML_FALSE;
	disEnableListener(3, enable);
	checkIfListenerDisabled(3);

	checkListenerCount(5);

  //send async and sync cmds
	checkAsyncSyncMessagesForErrors(3);
	waitForAsyncReplies();

  //enable listener again
	enable = TML_TRUE;
	disEnableListener(3, enable);

	//send another sync and async cmd
	bool synchronous = true;
	sendArbitraryCmds(!synchronous);
	sendArbitraryCmds(synchronous);

	waitForAsyncReplies();
	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

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
bool TmlMultipleListenersTester::testTmlCoreListenerCreateCloseErrorCodes()
{
  setTestSectionName(tmlrtT("testTmlCoreListenerCreateCloseErrorCodes"));
  messageOutput(S_START);
  reset();

	SIDEX_TCHAR* address = tmlrtT("0.0.0.0:1234");
	TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
	TML_INT32 apiReturns = 0;
	TmlCore* tmlCore = TML_HANDLE_TYPE_NULL;

	defaultListenerInit();
	initSenderSide();

	//invalid core handle
	apiReturns = tml_Core_Listener_Create(TML_HANDLE_TYPE_NULL, address, &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	//NULL as core handle TODO
	/*apiReturns = tml_Core_Listener_Create(NULL, address, &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	//invalid address
	tmlCore = m_coreListenerSide;
	apiReturns = tml_Core_Listener_Create(tmlCore->getCore(), tmlrtT("21"), &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_NET_BINDING);

	//NULL as listener handle
	apiReturns = tml_Core_Listener_Create(tmlCore->getCore(), address, NULL);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	//invalid listener handle
	apiReturns = tml_Listener_Close(&listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	//NULL as listener handle
	apiReturns = tml_Listener_Close(NULL);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

#if defined SIDEX_UNICODE || defined TML_UNICODE
	tmlCore = m_coreSenderSide;
	apiReturns = tml_Core_Listener_Create(tmlCore->getCore(), NULL, &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_UNICODE);
#else 
	//NULL as address
	apiReturns = tml_Core_Listener_Create(tmlCore->getCore(), NULL, &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);
#endif

	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the function tml_Core_Get_ListenerCount returns expected error codes.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the function tml_Core_Get_ListenerCount is called with param coreHandle = NULL and it is tested
* whether it returns the expected error code.
* Afterwards the TMLCores of both Sides are freed.
*/
bool TmlMultipleListenersTester::testTmlCoreGetListenerCountErrorCodes()
{
  setTestSectionName(tmlrtT("testTmlCoreGetListenerCountErrorCodes"));
  messageOutput(S_START);
  reset();

	TML_INT32 apiReturns = 0;
	TML_UINT32 count = 0;

	initSenderSide();
	defaultListenerInit();

	//invalid listener handle
	apiReturns = tml_Core_Get_ListenerCount(TML_HANDLE_TYPE_NULL, &count);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	//NULL as listener handle
	/*apiReturns = tml_Core_Get_ListenerCount(NULL, &count);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	//NULL as count parameter
	apiReturns = tml_Core_Get_ListenerCount(m_coreListenerSide->getCore(), NULL);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_UNICODE);

	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

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
bool TmlMultipleListenersTester::testTmlCoreGetListenerErrorCodes()
{
  setTestSectionName(tmlrtT("testTmlCoreGetListenerErrorCodes"));
  messageOutput(S_START);
  reset();

	TML_INT32 apiReturns = 0;
	TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
	TML_UINT32 indexOfListenerHandle = 0;
	TmlCore* tmlCore = TML_HANDLE_TYPE_NULL;
	TML_UINT32 indexOutOfBounds = 7;

	defaultListenerInit();
	initSenderSide();

	//invalid core handle
	apiReturns = tml_Core_Get_Listener(TML_HANDLE_TYPE_NULL, indexOfListenerHandle, &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	//NULL as core handle
	/*apiReturns = tml_Core_Get_Listener(NULL, indexOfListenerHandle, &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	//NULL as index
	/*apiReturns = tml_Core_Get_Listener(tmlCore->getCore(), NULL, &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	//NULL as listener handle
	/*apiReturns = tml_Core_Get_Listener(tmlCore->getCore(), indexOfListenerHandle, NULL);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	tmlCore = m_coreSenderSide;

	apiReturns = tml_Core_Get_Listener(tmlCore->getCore(), indexOutOfBounds, &listenerHandle);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_INFORMATION_UNDEFINED);

	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the functions tml_Listener_Get_Enabled and _Set_Enabled return expected error codes.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the function tml_Listener_Set_Enabled is called with param coreHandle = NULL and it is tested
* whether it returns the expected error code. The same is done with tml_Listener_Get_Enabled.
* Afterwards the TMLCores of both Sides are freed.
*/
bool TmlMultipleListenersTester::testTmlListenerGetSetEnabledForErrorCodes()
{
  setTestSectionName(tmlrtT("testTmlListenerGetSetEnabledForErrorCodes"));
  messageOutput(S_START);
  reset();

	TML_INT32 apiReturns = 0;
	TML_BOOL listenerEnabled = true;

	defaultListenerInit();
	initSenderSide();

	//invalid listener parameter
	apiReturns = tml_Listener_Set_Enabled(TML_HANDLE_TYPE_NULL, listenerEnabled);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	//NULL as listener handle
	/*apiReturns = tml_Listener_Set_Enabled(NULL, listenerEnabled);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	//NULL as bool parameter
	/*apiReturns = tml_Listener_Set_Enabled(m_coreListenerSide->getCore(), NULL);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	//invalid listener handle
	apiReturns = tml_Listener_Get_Enabled(TML_HANDLE_TYPE_NULL, &listenerEnabled);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	//NULL as listener handle
	/*apiReturns = tml_Listener_Get_Enabled(NULL, &listenerEnabled);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	//NULL as bool parameter
	apiReturns = tml_Listener_Get_Enabled(m_coreListenerSide->getCore(), NULL);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

/** @ingroup Test_Multiple_Listeners
* @brief Testing whether the function tml_Listener_Get_Address returns expected error codes.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the function tml_Listener_Get_Address is called with param coreHandle = NULL and it is tested
* whether it returns the expected error code.
* Afterwards the TMLCores of both Sides are freed.
*/
bool TmlMultipleListenersTester::testTmlListenerGetAddressErrorCodes()
{
  setTestSectionName(tmlrtT("testTmlListenerGetAddressErrorCodes"));
  messageOutput(S_START);
  reset();

	SIDEX_TCHAR* address;
	TML_LISTENER_HANDLE listenerHandle = TML_HANDLE_TYPE_NULL;
	TML_INT32 apiReturns = 0;
	TmlCore* tmlcore = TML_HANDLE_TYPE_NULL;

	initSenderSide();
	defaultListenerInit();

	//invalid core handle
	apiReturns = tml_Listener_Get_Address(TML_HANDLE_TYPE_NULL, &address);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	//NULL as core handle
	/*apiReturns = tml_Listener_Get_Address(NULL, &address);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);*/

	//NULL as address
	apiReturns = tml_Listener_Get_Address(m_coreListenerSide->getCore(), NULL);
	m_iErr = apiReturns;
	checkForExpectedReturnCode(TML_ERR_MISSING_OBJ);

	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}

//------------------------------------------------------------------------------

/** @ingroup Test_Multiple_Listeners
* @brief Testing a simple Event communication.
*
* A TestingProcess is started and the Listener- and SenderSides are initialized.
* The ListenerSide has five Listeners listening to different Ports with one Profile.
* Then the listeners are subscribed for all cmd-events. This is followed by sending all cmds
* as events.
* Afterwards the TMLCores of both Sides are freed.
*/
bool TmlMultipleListenersTester::testTmlMultiListenerEventMessages()
{
  setTestSectionName(tmlrtT("testTmlMultiListenerEventMessages"));
  messageOutput(S_START);
  reset();

	bool loadbalancing = false;
	defaultListenerInit();
	initSenderSide();

	//mixed Subscribing and SubscriptionRequests
	subscribeListenersForLoadBalancingOrEvents(loadbalancing);	
	sendEventMessages();

	TmlSleep(5000);	//important to let the listeners catch up their queue, otherwise error 45
	freeCmds();
	freeTmlCores();

  messageOutput(CHOICE_FINISH_MESSAGE(m_testOK));
  messageOutput();
  setTestSectionName();
  return(m_testOK);
}
