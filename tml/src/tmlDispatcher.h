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
 * Contributors:
 *    wobe-systems GmbH
 */

#ifndef TMLDISPATCHER_H
#define TMLDISPATCHER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlUniversalHashTable.h"
#include "tmlLogHandler.h"

class  tmlDispatcher
{

private:
	  /* data */

    /**
     * @brief    The debug log handler
     */
    tmlLogHandler* m_log;

    /* Methods */

    /**
    * @brief   Get rid of all initialized attributes / set the objet to uninitialized state
     */
    void tml_DispatcherDismissAllAttributes();


protected: 
	  /* data */

    /**
    * @brief    Reference to instance of a hashtable that holds the references to callback methods and data
    */
    TMLUniversalHashTable*  m_hashTable;


    /**
    * @brief    The profile for this dispatcher object
    */
    char*          m_profile;


    /**
    * @brief    Reference to callback method that will be invoked in case of an deregistration of a callback method 
    */
    TML_ON_CMD_DELETION_CB_FUNC   m_pDeleteCmdCallback;


    /**
    * @brief    Reference to callback method parameter that will be invoked in case of an unregistered commans dispatch action 
    */
    TML_POINTER          m_pDeleteCmdData;


    /**
    * @brief    Reference to callback method that will be invoked in case of an unregistered commans dispatch action 
    */
   TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC  m_pCustomDispatchCallback;


    /**
    * @brief    Reference to callback method parameter that will be invoked in case of an unregistered commans dispatch action 
    */
    TML_POINTER          m_pCustomDispatchData;


public:
	  /* data */
	  /* Methods */

    /**
     * @brief    Constructor.
     *
     * @param   loghandler   Reference to a log handler.
     *
     * @returns an instance of tmlDispatcher
     */
    explicit tmlDispatcher(tmlLogHandler* loghandler);


    /**
     * @brief    Destructor.
     */
    ~tmlDispatcher();


    /**
     * @brief   Create an instance of a command dispatcher.
     *
     * @param   profile   The BEEP communication profile.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_ALREADY_EXISTS if a valid dispatcher already exists.
     *
     * @see tmlErrors.h
     */
    int tml_Dispatcher_Create(const char* profile);


    /**
     * @brief   Returns the dispatchers profile.
     *
     * @param   profile   The BEEP communication profile.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tml_Dispatcher_Get_Profile(char** profile);


    /**
     * @brief   Dismiss the instance of the dispatcher.
     *
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tml_Dispatcher_Free();


    /**
     * @brief   Register a callback method referring a command for the command dispatch process.
     *
     * @param   cmd       Command ID that refer the callback method.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method  has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_CMD_NOT_REGISTERED if the command is not registered<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tml_Dispatcher_Register_Cmd(TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief   Check if the cmd is registered
     *
     * @param   cmd       Command ID for the request.
     *
     * @returns true if requested command registered, otherwise false.
     */
    bool tml_Dispatcher_Is_Cmd_Registered(TML_COMMAND_ID_TYPE cmd);


    /**
     * @brief   Returns the numer of registered commands
     *
     * @param   iSize Reference to the return value.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_HASH if the hashtable is invalid.
     *
     * @see tmlErrors
     */
    int tml_Dispatcher_Get_Registered_Size(int* iSize);


    /**
     * @brief   Register a callback method that will be invoked in the case of command deregistration out of the command dispatch process.
     *
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered.
     * @param   pCBData   Reference to parameter of the callback method / may be TML_HANDLE_TYPE_NULL.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tml_Dispatcher_Set_OnDeleteCmd(TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief   Register a callback method for non registered dispatch commands.
     *
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method  has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tml_Dispatcher_Set_OnCustomDispatch(TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief   This will initiate the command dispatch action for the tmlhandle containing the requested command.
     *
     * @param   tmlhandle   Reference to an instance of TML_COMMAND_HANDLE that holds the command header and data.
     * @param   iCmd        The command identification.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_DISPATCHER_NOT_CREATED if the dispatcher instance is not initialized (created).
     *
     * @see tmlErrors.h
     */
    int tml_Dispatcher_Dispatch_Cmd(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE iCmd);
};

#endif  // TMLDISPATCHER_H

