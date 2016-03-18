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

#ifdef LINUX
  #include <wchar.h>
  #include <stdbool.h>
  #include <string.h>
#else
  #include <stdio.h>
  #include <tchar.h>
  #include <string.h>
#endif
#include <iostream>
using namespace std;
#include <tmlCore.h>
#include <sidex.h>
#include "tmlrt_SendingCommands.h"
//#include "tmlrt_LoadBalancing.h"
#include "tmlrt_Utils.h"
#include "TestingProcess.h"
#include "tmlrt_MultipleListeners.h"


/** @brief Main function, accepts command line parameters
* @param int argc : contains amount of arguments in argv
* @param char* argv[] : contains input from user
* @returns int : 0 if everything went okay, -1 if a failure/error occurred or the options of the user were not recognized
*/
#ifdef LINUX
    int main(int argc, char* argv[])
#else //LINUX
    int _tmain(int argc, char* argv[])
#endif
{	
	//wenn datei mit defaultnamen da ist, benutzte es. wenn was übergeben wurde, nutze diesen namen. Wenn weder defaultname noch übergebener name da ist, dont execute that special section
	//defaultname in tmlrt_Utils
	if(2 == argc) {
		//text ungefähr: 
		//noch kein template: es wird ein template erzeugt in diesem verzeichnis. (funktionsaufruf) dann werden keine (?) tests ausgeführt.
		// template: wenn ein fehler auftritt, könnten die ip-adressen fehlerhaft sein(oder der name der file?)
	}



	//for testing purposes
	//testing to send messages
	simpleTestTmlMultiListenerSendSyncMessage();	
	simpleTestTmlMultiListener();
	testTmlCoreListenerClose();
	testTmlCoreListenerGetSetEnabled();
	testTmlCoreListenerCreateCloseErrorCodes();
	testTmlCoreGetListenerCountErrorCodes();
	testTmlCoreGetListenerErrorCodes();
	testTmlListenerGetSetEnabledForErrorCodes();

	
   /* testTmlSendSyncMessage();
    simpleTestTmlSendAsyncMessage();
    testTmlSendAsyncMessage();	

	//testing to send balanced messages
    simpleTestTmlSendBalancedSyncMessage();
	simpleTestTmlSendBalancedSyncMessageAndSendSubscribeRequest();
	testTmlSendBalancedSyncMessageAndBalancer();

	simpleTestTmlSendBalancedAsyncMessage();
	simpleTestTmlSendBalancedAsyncMessageAndSendSubscribeRequest();
	testTmlSendBalancedAsyncMessageAndBalancer();

	testTmlsendBalSyncMsgs();
	testTmlsendBalAsyncMsgs();

	//testing functions for expected error codes
	testTmlBal_Subscribe_MessageDestination();
	testTmlBal_Send_SubscriptionRequest();*/


    return 0;
}





//function for creating the bootstrap template
/*void createBootstrapTemplate() {
	SIDEX_INT32 iErr =  0;
	SIDEX_HANDLE bootstrapDocument = SIDEX_HANDLE_TYPE_NULL;

	//create a new sidex document
	iErr =  sidex_Create(DEFAULT_BOOTSTRAP, &bootstrapDocument);
	errLog(SIDEX_SUCCESS == iErr, "createBootstrapTemplate", "sidex_Create", 1);

	//add parameters



	//save the document as a file
	iErr = sidex_Save_Content(bootstrapDocument, DEFAULT_BOOTSTRAP);
	errLog(SIDEX_SUCCESS == iErr, "createBootstrapTemplate", "sidex_Save_Content", 1);
}*/

//function for checking whether the bootstrap file already exists









//function for loading variables from the bootstrap file