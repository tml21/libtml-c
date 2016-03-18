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

#ifndef TMLRT_SENDING_COMMANDS_H
#define TMLRT_SENDING_COMMANDS_H


/** @defgroup Test_Sending_Commands
* @brief Test Reference for functions in Module Sending Commands
*/

//Testing a simple communication between a sender and a listener
bool simpleTestTmlSendSyncMessage();

//Various Errors are produced with tml_Send_SyncMessage
bool testTmlSendSyncMessage();

//A simple Test for tml_Send_AsyncMessage
bool simpleTestTmlSendAsyncMessage();

//Various Errors are produced with tml_Send_AsyncMessage
bool testTmlSendAsyncMessage();


#endif  //TMLRT_SENDING_COMMANDS_H
