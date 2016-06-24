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
 
//-----------------------------------------
// write here package with @ to get namespace TAB in the html- documentation
//-----------------------------------------

/** package  TML
  */


/** @file tmlCore.h
  * 
  * @brief Declarations and dll import file for the TML C API.
  */

/** @defgroup coreAPI TML API
  * @brief TML API reference
  */
  

/** @defgroup coreGeneral General
	* @ingroup coreAPI
  * @brief General library functions
  */

/** @ingroup coreGeneral
  * @defgroup coreErrors TML return codes
  * @brief Return codes of TML API functions.
  */  

/** @ingroup coreGeneral
  * @defgroup tmlTypes Type definitions
  * @brief Type definitions used by TML API functions.
  */

/** @ingroup coreGeneral
  * @defgroup tmlConstants Constant definitions
  * @brief Constants used by TML API functions.
  */

/** @defgroup coreHandle TMLCore
	* @ingroup coreAPI
  * @brief TMLCore functions
  *
  * All communications in TML is related to a TMLCore object. At least one
  * TMLCore must exist before any TML communication is possible.
  */  

/** @ingroup coreHandle
  * @defgroup coreManagement Listener management
  * @brief TMLCore listener management
  *
  * A TMLCore listener must be started and initialized to enable incoming traffic.
  */  

/** @ingroup coreHandle
  * @defgroup multiListener Multiple listener management
  * @brief TMLCore multiple listener management
  *
  * A TMLCore connection must be initialized to send data.
  */  

/** @ingroup coreHandle
  * @defgroup connectionManagement Connection management
  * @brief TMLCore connection management
  *
  * A TMLCore connection must be initialized to send data.
  */  

/** @ingroup coreGeneral
  * @defgroup tmlCBFunctions Callback function reference
  * @brief Definitions of TML callback functions.
  */  

/** @ingroup coreHandle
  * @defgroup dispatching Profile management
  * @brief TMLCore profile
  *
  * A profile is linked to a TMLCore and diapatches incoming traffic to command handling functions.
  */  

/** @ingroup coreHandle
  * @defgroup dataIO Sending commands
  * @brief Using the TMLCore to send commands.
  */  

/** @ingroup coreHandle
  * @defgroup eventIO Event handling
  * @brief Sending of events to multiple destinations. 
  *
  * Sending event messages to multiple peers without a reply.
  */  

/** @ingroup coreHandle
  * @defgroup loadbalancedIO Load balancing
  * @brief Balance command calls.
  *
  * For backup and load balancing TML provides an API to call a group of peers instead of a specific peer to
  * process a command. If one peer fails the remaining will still handle the command.
  */  

/** @defgroup tmlHandle TML commands
	* @ingroup coreAPI
  * @brief Creating and handling of TML commands.
  *
  *  TML commands contain data and header information. The data can be set and read using
  *  the SIDEX API. All SIDEX datatypes can be used to be exchanged through a TML command call.
  */  

/** @ingroup tmlHandle
  * @defgroup tmlCallbacks Command message callbacks
  * @brief Callback methods for multiple answers and asynchronous reply handling.
  *
  * If commands are called asynchronously or status/progress replies are used, callbacks can
  * be registered for a command to handle this information.
  */  


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/** @defgroup tmlAttributes TMLCore listener communication attributes
	* @ingroup coreAPI
  * @brief Handling of TMLCore listner communication attributes as content of the data object.<br>
  * Getting the communication attributes may be interesting on callback method side (for example for statistical purposes).
  */  

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

/**
  * @ingroup tmlHandle
  * @defgroup tmlheader Accessing header information
  * @brief Functions to read/write TML command header data.
  */

/** @ingroup coreHandle
  * @defgroup streamIO Stream communication
  * @brief Using TML streams.
  *
  * The stream API enables TML to transfer files without using another port or protocol.
  */  

#ifndef TMLCORE_H
#define TMLCORE_H

//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/** @defgroup tmladvanced TMLCore expert use
	* @ingroup coreAPI
  * @brief Internal developer API calls.<br><br>
  * @note For develop use only / we recommend not to use this API calls as a standard user.
  */

 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globalDefines.h"
#include "tmlStdTypes.h"
#include "tmlErrors.h"

#ifndef DOXYGEN_GENERATION
#ifdef LINUX
  #define TML_CORE_API __attribute__ ((__visibility__("default")))
#else
  #ifdef TML_EXPORTS
    /**
    * @brief    Definition for export of the dll
    */
    #define TML_CORE_API __declspec(dllexport)
  #else
    /**
    * @brief    Definition for import of the dll
    */
    #define TML_CORE_API __declspec(dllimport)
  #endif
#endif
#endif //DOXYGEN_GENERATION

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

/**
 * @ingroup  coreGeneral
 * @brief    Returns current interface api and library version.
 *
 * @param    apiVer api version
 * @param    libVer library version
 * @param    verStr library version as a string<br>
 *           This is a borrowed reference and has not to be freed or modified.
 */
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Version(TML_INT32* apiVer, TML_INT32* libVer, SIDEX_CTSTR** verStr);
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Version_X(TML_INT32* apiVer, TML_INT32* libVer, wchar_t** verStr);
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Version_W(TML_INT32* apiVer, TML_INT32* libVer, char16_t** verStr);
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Version_A(TML_INT32* apiVer, TML_INT32* libVer, char** verStr);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Get_Version tml_Core_Get_Version_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Get_Version  tml_Core_Get_Version_W
    #else
      #define tml_Core_Get_Version  tml_Core_Get_Version_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup coreGeneral
 * @brief   Returns the copyright information.
 *
 * @param   sValue  copyright information<br>
 *          This is a borrowed reference and has not to be freed or modified.
 * @param   iLength length of the copyright information string
 */
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Copyright(TML_CTSTR** sValue, TML_INT32* iLength);
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Copyright_X(wchar_t** sValue, TML_INT32* iLength);
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Copyright_W(char16_t** sValue, TML_INT32* iLength);
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Copyright_A(char** sValue, TML_INT32* iLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Get_Copyright  tml_Core_Get_Copyright_X
  #else
    #ifdef TML_UNICODE_16
    #define tml_Core_Get_Copyright  tml_Core_Get_Copyright_W
    #else
    #define tml_Core_Get_Copyright  tml_Core_Get_Copyright_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup coreGeneral
 * @brief    Apply the license to the library
 *
 * Before sdk functions can be used a license key must be applied.
 * @note Since TML/SIDEX is open source this API call is deprecated and isn't necessary. It returns allways TML_SUCCESS.<br>
 *       It has been left in the API to be downward compatible.
 *
 * @param    pUserName   user name (case sensitive)
 * @param    pPassWord   password (case insensitive)
 *
 * @returns  TML_SUCCESS in case of success<br>
 *           TML_ERR_UNICODE error in unicode conversion
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_Password(const TML_CTSTR* pUserName, const TML_CTSTR* pPassWord);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_Password_X(const wchar_t* pUserName, const wchar_t* pPassWord);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_Password_W(const char16_t* pUserName, const char16_t* pPassWord);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_Password_A(const char* pUserName, const char* pPassWord);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Set_Password  tml_Core_Set_Password_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Set_Password  tml_Core_Set_Password_W
    #else
      #define tml_Core_Set_Password  tml_Core_Set_Password_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
  * @ingroup  coreManagement
  * @brief    Configure global listener thread pool handling.
  *
  * @note For develop use only / we recommend not to use this API call as a standard user.
  * <br><br>
  * To change thread pool behaviour this call has to be called before tml_Core_Open().
  *
  * @param    iInitialThreadPoolSize   Initial thread pool size (Default is 10).<br>
  *                                    The listener may support iInitialThreadPoolSize parallel asynchron command / message requests.<br>
  *                                    For every additional parallel asynchron request, a new thread has to be created internal.
  * @param    iThreadPoolMaxSize       Maximal thread pool size (Default is 40).<br>
  *                                    If the maximal thread pool size is reached, asynchron command / message requests will be queued and processed, if a thread is getting idle.
  * @param    iThreadAddSteps          Thread pool add step (Default is 1).<br>
  *                                    This value is the number of threads, that will be created in case of an additional parallel asynchron request.
  * @param    iThreadPoolAddPeriod     Thread pool add period in seconds (Default 0 Seconds).
  *                                    To smooth the process of thread generation (Denial of Service)<br>
  *                                    you can configure a time for the additional thread gerneration.
  * @param    iThreadRemoveSteps       Thread pool remove step (Default is 1).<br>
  * @param    iThreadPoolRemovePeriod  Thread pool remove period in seconds (Default 1 Second).
  * @param    bThreadAutoRemove        Auto remove thread pool (Default TML_TRUE).<br>
  *                                    If additional created thread become idle, they will be removed if this attribute is TML_TRUE.
  * @param    bThreadPreemptive        Preemptive value. (Default TML_TRUE).<br>
  *        - axl_true :  makes vortex to react faster to create threads<br>
  *                      by checking before running pool tasks<br>
  *        - axl_false : makes vortex to react slower to create threads<br>
  *                      by checking after running pool tasks<br>
  */
TML_CORE_API void DLL_CALL_CONV tml_Configure_Thread_Pool_Handling(TML_INT32 iInitialThreadPoolSize, TML_INT32 iThreadPoolMaxSize,
                                                        TML_INT32 iThreadAddSteps, TML_INT32 iThreadPoolAddPeriod,
                                                        TML_INT32 iThreadRemoveSteps, TML_INT32 iThreadPoolRemovePeriod, 
                                                        TML_BOOL bThreadAutoRemove, TML_BOOL bThreadPreemptive);

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

/**
 * @ingroup  coreHandle
 * @brief   Open an instance of a TMLCore.
 *
 * @param   coreHandle  reference to TML core handle (TML_CORE_HANDLE)
 * @param   iLogValue   logging value (see tml_Core_Set_LoggingValue())
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INITIALIZATION initialization error<br>
 *          TML_ERR_LISTENER_NOT_INITIALIZED listener initialization error
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Open(TML_CORE_HANDLE* coreHandle, TML_INT32 iLogValue);

/**
 * @ingroup  coreHandle
 * @brief  Free a TML core.
 *
 * @param   coreHandle reference to TML core handle (TML_CORE_HANDLE)
 *
 * @returns TML_SUCCESS or error code
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Close(TML_CORE_HANDLE* coreHandle);

/**
 * @ingroup  coreHandle
 * @brief  Unregister all callback functions.
 *
 * Unregister all callbacks to a TML core can be done before closing it.
 * Doing this makes sure that no function is called while the status of the TML core is undefined.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 *
 * @returns TML_SUCCESS or error code
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_GeneralDeregistration(TML_CORE_HANDLE coreHandle);


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup  tmladvanced
 * @brief   Changes the callback method for thread creation.
 *
 * Note: For develop use only / we recommend not to use this API call as a standard user.<br><br>
 * See tml_Sys_Free().
 *
 * @param   pCBCreate   reference to callback method.
 */
TML_CORE_API void DLL_CALL_CONV tml_Core_Thread_Set_OnCreate(void* pCBCreate);


/**
 * @ingroup  tmladvanced
 * @brief   Changes the callback method for thread destruction.
 *
 * Note: For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   pCBDestroy  reference to callback method.
 */
TML_CORE_API void DLL_CALL_CONV tml_Core_Thread_Set_OnDestroy(void* pCBDestroy);


/**
 * @ingroup  tmladvanced
 * @brief Helper method referring the registered tml_Core_Thread_Set_OnCreate() callback method.
 *
 * Note: For develop use only / we recommend not to use this API call as a standard user.
 *
 * It is necessary to free memory, that has been allocated by the ASPL library
 *        entering the registered tml_Core_Thread_Set_OnCreate() callback method.
 *
 * @param   ptr  reference to be freed.
 */
TML_CORE_API void DLL_CALL_CONV tml_Sys_Free(TML_POINTER ptr);

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

/**
 * @ingroup  dispatching
 * @brief   Register a profile.
 *
 * Before any other operation referring to a profile is accepted by the TML core, the profile
 * has to be registered.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 *
 * @returns TML_SUCCESS or error code
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_W(TML_CORE_HANDLE coreHandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_A(TML_CORE_HANDLE coreHandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Profile_Register  tml_Profile_Register_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Profile_Register  tml_Profile_Register_W
    #else
      #define tml_Profile_Register  tml_Profile_Register_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup  dispatching
 * @brief   Unregister a profile.
 *
 * All command handlers registered for the profile will be removed from the TML core.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_DISPATCHER_NOT_CREATED profile not registered
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Unregister(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Unregister_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Unregister_W(TML_CORE_HANDLE coreHandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Unregister_A(TML_CORE_HANDLE coreHandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Profile_Unregister  tml_Profile_Unregister_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Profile_Unregister  tml_Profile_Unregister_W
    #else
      #define tml_Profile_Unregister  tml_Profile_Unregister_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  dispatching
 * @brief   Get the number of registered profiles.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iSize      reference to profile count
 *
 * @returns TML_SUCCESS or error code
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_Registered_Count(TML_CORE_HANDLE coreHandle, TML_INT32* iSize);


/**
 * @ingroup  dispatching
 * @brief   Get a list of registered profile names.
 *
 * A SIDEX list containing strings with profile names is returned.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profiles   reference to profile list.<br>
 *                     The list has to be released with sidex_Variant_DecRef().
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_HASH invalid hashtable<br>
 *          TML_ERR_NOPROFILES no profiles are registered<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_Registered(TML_CORE_HANDLE coreHandle, SIDEX_VARIANT* profiles);


/**
 * @ingroup  dispatching
 * @brief   Check if a profile name is already registered.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   bRegistered reference to a boolean<br>
 *                      TML_TRUE = profile is registered
 *                      TML_FALSE = profile not registered
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_HASH invalid hashtable<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_RegisterState(TML_CORE_HANDLE coreHandle, TML_CTSTR* profile, TML_BOOL* bRegistered);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_RegisterState_X(TML_CORE_HANDLE coreHandle, wchar_t* profile, TML_BOOL* bRegistered);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_RegisterState_W(TML_CORE_HANDLE coreHandle, char16_t* profile, TML_BOOL* bRegistered);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_RegisterState_A(TML_CORE_HANDLE coreHandle, char* profile, TML_BOOL* bRegistered);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Profile_Get_RegisterState  tml_Profile_Get_RegisterState_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Profile_Get_RegisterState  tml_Profile_Get_RegisterState_W
    #else
      #define tml_Profile_Get_RegisterState  tml_Profile_Get_RegisterState_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  dispatching
 * @brief   Register a command handler function.
 *
 * Read TML_ON_CMD_DISPATCH_CB_FUNC() for further reference.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   cmd        command id
 * @param   pCBFunc    callback callback address or NULL to unregister a previously registered function
 * @param   pCBData    user data or NULL<br>
 *                     Use tml_Profile_Set_OnDeleteCmd() callback registration to release memory allocated by pCBData.
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_DISPATCHER_CMD_NOT_REGISTERED try to unregister a non existing command handler<br>
 *          TML_ERR_DISPATCHER_NOT_CREATED invalid profile<br>
 *          TML_ERR_UNICODE unicode conversion error<br>
 *
 * @see tml_Profile_Set_OnDeleteCmd(), TML_ON_CMD_DISPATCH_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_Cmd(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_Cmd_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_Cmd_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_Cmd_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Profile_Register_Cmd  tml_Profile_Register_Cmd_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Profile_Register_Cmd  tml_Profile_Register_Cmd_W
    #else
      #define tml_Profile_Register_Cmd  tml_Profile_Register_Cmd_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  dispatching
 * @brief   Register a callback to release custom data assigned to a command handler
 *
 * Read TML_ON_CMD_DELETION_CB_FUNC() for further reference.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    callback address or NULL to unregister a previously registered function
 * @param   pCBData    user data or NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_DISPATCHER_NOT_CREATED invalid profile
 *
 * @see TML_ON_CMD_DELETION_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnDeleteCmd(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnDeleteCmd_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnDeleteCmd_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnDeleteCmd_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Profile_Set_OnDeleteCmd  tml_Profile_Set_OnDeleteCmd_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Profile_Set_OnDeleteCmd  tml_Profile_Set_OnDeleteCmd_W
    #else
      #define tml_Profile_Set_OnDeleteCmd  tml_Profile_Set_OnDeleteCmd_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  dispatching
 * @brief   Register a general command handler.
 *
 * If no command handler was registered for a command id, the profile will call the general command handler
 * if registered. This can be used to implement an own dispatching mechanism to handle incoming commands and events.<br>
 * Read TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC() for further reference.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    name of the profile
 * @param   pCBFunc    callback address or NULL to unregister a previously registered function
 * @param   pCBData    user data or NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_DISPATCHER_NOT_CREATED invalid profile
 *
 * @see TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnCustomDispatch(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnCustomDispatch_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnCustomDispatch_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnCustomDispatch_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Profile_Set_OnCustomDispatch  tml_Profile_Set_OnCustomDispatch_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Profile_Set_OnCustomDispatch  tml_Profile_Set_OnCustomDispatch_W
    #else
      #define tml_Profile_Set_OnCustomDispatch  tml_Profile_Set_OnCustomDispatch_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup  coreManagement
 * @brief  Set listener port.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   sPort      port number, 0 = random port number, default 44000
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_NOT_OPERABLE_AT_THE_MOMENT listener enable<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid port or core handle
 *
 * @see tml_Core_Get_ListenerPort()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerPort(TML_CORE_HANDLE coreHandle, TML_CTSTR* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerPort_X(TML_CORE_HANDLE coreHandle, wchar_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerPort_W(TML_CORE_HANDLE coreHandle, char16_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerPort_A(TML_CORE_HANDLE coreHandle, char* sPort);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Set_ListenerPort  tml_Core_Set_ListenerPort_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Set_ListenerPort  tml_Core_Set_ListenerPort_W
    #else
      #define tml_Core_Set_ListenerPort  tml_Core_Set_ListenerPort_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  coreManagement
 * @brief   Get the listener port number.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   sPort      borrowed reference to port number
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Core_Set_ListenerPort()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerPort(TML_CORE_HANDLE coreHandle, TML_CTSTR** sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerPort_X(TML_CORE_HANDLE coreHandle, wchar_t** sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerPort_W(TML_CORE_HANDLE coreHandle, char16_t** sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerPort_A(TML_CORE_HANDLE coreHandle, char** sPort);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Get_ListenerPort  tml_Core_Get_ListenerPort_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Get_ListenerPort  tml_Core_Get_ListenerPort_W
    #else
      #define tml_Core_Get_ListenerPort  tml_Core_Get_ListenerPort_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  coreManagement
 * @brief   Set listener interface (IP)
 *
 * The listener can be assigned to a specific network interface or to all network interfaces available ("0.0.0.0) 
 * which is the default.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   sIP        interface IP
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_NOT_OPERABLE_AT_THE_MOMENT listener is already enabled<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle or interface address
 *
 * @see tml_Core_Get_ListenerIP()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerIP(TML_CORE_HANDLE coreHandle, TML_CTSTR* sIP);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerIP_X(TML_CORE_HANDLE coreHandle, wchar_t* sIP);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerIP_W(TML_CORE_HANDLE coreHandle, char16_t* sIP);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerIP_A(TML_CORE_HANDLE coreHandle, char* sIP);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Set_ListenerIP  tml_Core_Set_ListenerIP_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Set_ListenerIP  tml_Core_Set_ListenerIP_W
    #else
      #define tml_Core_Set_ListenerIP  tml_Core_Set_ListenerIP_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  coreManagement
 * @brief   Get the listener interface (IP).
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   sIP        borrowed reference to interface IP
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Core_Set_ListenerIP()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerIP(TML_CORE_HANDLE coreHandle, TML_CTSTR** sIP);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerIP_X(TML_CORE_HANDLE coreHandle, wchar_t** sIP);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerIP_W(TML_CORE_HANDLE coreHandle, char16_t** sIP);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerIP_A(TML_CORE_HANDLE coreHandle, char** sIP);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Get_ListenerIP  tml_Core_Get_ListenerIP_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Get_ListenerIP  tml_Core_Get_ListenerIP_W
    #else
      #define tml_Core_Get_ListenerIP  tml_Core_Get_ListenerIP_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  coreManagement
 * @brief   Enable or disable the TML listener.
 *
 * @note The interface IP and port have to be configured or a listener has been created before enabling the listener
 *
 * Enabling of multiple listeners in a "tml_Core_Set_ListenerEnabled" call will fail with an error should any one of them fail to start.<br>
 * On this type of error all listeners will be disabled.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   bEnable      TML_TRUE to enable, TML_FALSE to disable the listener
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_LISTENER_ADDRESS_BINDING listener start failed - invalid port number or interface<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_LISTENER_NOT_INITIALIZED listener not initialized<br>
 *          TML_ERR_LISTENER_ALREADY_STARTED listener already enabled
 *
 * @see tml_Core_Get_ListenerEnabled(), tml_Core_Listener_Create()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerEnabled(TML_CORE_HANDLE coreHandle, TML_BOOL bEnable);


/**
 * @ingroup  coreManagement
 * @brief   Get listener status.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   bEnable    reference to listener status (TML_FALSE, TML_TRUE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Core_Set_ListenerEnabled()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerEnabled(TML_CORE_HANDLE coreHandle, TML_BOOL* bEnable);


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup  coreManagement
 * @brief   Set the window size for the following outgoing data.
 *
 * @note For develop use only / we recommend not to use this API call as a standard user.
 * By default the window size is 32768 byte.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iWindowSize The window size.
 *
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.
 *
 * @see tml_Core_Get_WindowSize()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_WindowSize(TML_CORE_HANDLE coreHandle, TML_INT32 iWindowSize);


/**
 * @ingroup  coreManagement
 * @brief   Get the window size for the following outgoing data.
 *
 * Note: For develop use only / we recommend not to use this API call as a standard user.<br><br>
 * By default the window size is 32768 byte.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iWindowSize Reference to the return value.
 *
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.
 *
 * @see tml_Core_Set_WindowSize()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_WindowSize(TML_CORE_HANDLE coreHandle, TML_INT32* iWindowSize);

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

/**
 * @ingroup  coreGeneral
 * @brief  Set the debug log value.
 *
 *  <TABLE>
 *   <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
 *   <TR><TD>TML_LOG_OFF                </TD><TD>0x0</TD><TD>no debug log</TD></TR>
 *   <TR><TD>TML_LOG_VORTEX_CMD         </TD><TD>0x4</TD><TD>log API commands to vortex library</TD></TR>
 *   <TR><TD>TML_LOG_CORE_IO            </TD><TD>0x8</TD><TD>core I/O logging</TD></TR>
 *   <TR><TD>TML_LOG_CORE_API           </TD><TD>0x10</TD><TD>log core API</TD></TR>
 *   <TR><TD>TML_LOG_VORTEX_FRAMES      </TD><TD>0x20</TD><TD>vortex frame logging</TD></TR>
 *   <TR><TD>TML_LOG_VORTEX_CH_POOL     </TD><TD>0x40</TD><TD>vortex channel pool logging</TD></TR>
 *   <TR><TD>TML_LOG_VORTEX_MUTEX       </TD><TD>0x80</TD><TD>vortex mutex logging</TD></TR>
 *   <TR><TD>TML_LOG_MULTI_SYNC_CMDS    </TD><TD>0x100</TD><TD>vortex multi sync cmds</TD></TR>
 *   <TR><TD>TML_LOG_INTERNAL_DISPATCH  </TD><TD>0x800</TD><TD>internal command dispatcher log</TD></TR>
 *   <TR><TD>TML_LOG_STREAM_HANDLING    </TD><TD>0x2000</TD><TD>stream handling log</TD></TR>
 *   <TR><TD>TML_LOG_EVENT              </TD><TD>0x8000</TD><TD>event logging</TD></TR>
 *  </TABLE>
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iLogValue  debug log value
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Core_Get_LoggingValue()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_LoggingValue(TML_CORE_HANDLE coreHandle, TML_INT32 iLogValue);


/**
 * @ingroup  coreGeneral
 * @brief   Get current log value.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iLogValue  debug log value 
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Core_Set_LoggingValue()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_LoggingValue(TML_CORE_HANDLE coreHandle, TML_INT32*  iLogValue);


/**
 * @ingroup  dataIO
 * @brief    Send a synchronous message / command.
 *
 * Sending a message synchronously means that the call returns after the result of the message call
 * was received or an error occurred.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   tmlhandle   TML command handle
 * @param   profile     profile name
 * @param   sHost       hostname or IP address
 * @param   sPort       port number
 * @param   iTimeout    timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid address (host,IP,port)<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Send_AsyncMessage(), tml_Bal_Send_AsyncMessage(), tml_Bal_Send_SyncMessage()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_SyncMessage(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_SyncMessage_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_SyncMessage_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_SyncMessage_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile, const char* sHost, const char* sPort, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Send_SyncMessage  tml_Send_SyncMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Send_SyncMessage  tml_Send_SyncMessage_W
    #else
      #define tml_Send_SyncMessage  tml_Send_SyncMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  dataIO
 * @brief    Send an asynchronous message / command.
 *
 * The call returns after sending the message without waiting for a reply. 
 * The result of the command execution and possible error needs to be handled in a callback function registered with tml_Cmd_Register_CommandReady().
 * In the callback function you also have to free the TML command handle using tml_Cmd_Free(). 
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   tmlhandle   TML command handle
 * @param   profile     profile name
 * @param   sHost       hostname or IP address
 * @param   sPort       port number
 * @param   iTimeout    timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid address (host,IP,port)<br>
 *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED profile not supported by receiver<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Send_SyncMessage(), tml_Bal_Send_SyncMessage(), tml_Bal_Send_AsyncMessage()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncMessage(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncMessage_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncMessage_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncMessage_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile, const char* sHost, const char* sPort, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Send_AsyncMessage  tml_Send_AsyncMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Send_AsyncMessage  tml_Send_AsyncMessage_W
    #else
      #define tml_Send_AsyncMessage  tml_Send_AsyncMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup  dataIO
 * @brief    Send progress reply.
 *
 * A progress reply resets the timeout at the sender side and allows progress monitoring. To read the progress value
 * at the sender a callback function has to be registered with tml_Cmd_Register_Progress().
 *
 * @param   tmlhandle   TML command handle
 * @param   progress    progress value (0 - 100)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid command handle<br>
 *          TML_ERR_ATTRIBUTE_NOT_SET no core handle assigned to command<br>
 *          TML_ERR_LISTENER_NOT_INITIALIZED listener initialization failed<br>
 *          TML_ERR_LISTENER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC internal timeout error<br>
 *          TML_ERR_COMMAND_PROGRESS_RANGE progress range error
 *
 * @see tml_Cmd_Register_Progress()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncProgressReply(TML_COMMAND_HANDLE tmlhandle, TML_INT32 progress);

/**
 * @ingroup  dataIO
 * @brief    Send status reply.
 *
 * A status reply sends back messages to the sender. To read the status information
 * at the sender a callback function has to be registered with tml_Cmd_Register_StatusReply().
 *
 * Status reply types:
 *  <TABLE>
 *   <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
 *   <TR><TD>TMLCOM_RPY_WARNING       </TD><TD>10</TD><TD>warning message reply</TD></TR>
 *   <TR><TD>TMLCOM_RPY_ERROR         </TD><TD>20</TD><TD>error message reply</TD></TR>
 *   <TR><TD>TMLCOM_RPY_INFORMATION   </TD><TD>30</TD><TD>information message reply</TD></TR>
 *  </TABLE> 
 *
 * @param   tmlhandle   TML command handle
 * @param   iType       status reply type
 * @param   sStatus     status reply message
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid command handle<br>
 *          TML_ERR_ATTRIBUTE_NOT_SET no core handle assigned to command<br>
 *          TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED reply type not allowed<br>
 *          TML_ERR_LISTENER_NOT_INITIALIZED listener initialization failed<br>
 *          TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC internal timeout error<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_LISTENER_COMMUNICATION communication error
 *
 * @see tml_Cmd_Register_StatusReply()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncStatusReply(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iType, TML_CTSTR* sStatus);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncStatusReply_X(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iType, wchar_t* sStatus);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncStatusReply_W(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iType, char16_t* sStatus);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncStatusReply_A(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iType, char* sStatus);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Send_AsyncStatusReply  tml_Send_AsyncStatusReply_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Send_AsyncStatusReply  tml_Send_AsyncStatusReply_W
    #else
      #define tml_Send_AsyncStatusReply  tml_Send_AsyncStatusReply_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Subscribe event receiver.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   sHost       host name or IP address
 * @param   sPort       port number
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Get_Subscribed_MessageDestinations(), tml_Evt_Unsubscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Subscribe_MessageDestination(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Subscribe_MessageDestination_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Subscribe_MessageDestination_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Subscribe_MessageDestination_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sHost, const char* sPort);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Subscribe_MessageDestination  tml_Evt_Subscribe_MessageDestination_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Subscribe_MessageDestination  tml_Evt_Subscribe_MessageDestination_W
    #else
      #define tml_Evt_Subscribe_MessageDestination  tml_Evt_Subscribe_MessageDestination_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Subscribe event receiver.
 *
 * @param   connectionHandle  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   sProfile          profile identification string
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_Evt_Unsubscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Subscribe_MessageDestination(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Subscribe_MessageDestination_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Subscribe_MessageDestination_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Subscribe_MessageDestination_A(TML_CONNECTION_HANDLE connectionHandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Evt_Subscribe_MessageDestination  tml_Connection_Evt_Subscribe_MessageDestination_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Evt_Subscribe_MessageDestination  tml_Connection_Evt_Subscribe_MessageDestination_W
    #else
      #define tml_Connection_Evt_Subscribe_MessageDestination  tml_Connection_Evt_Subscribe_MessageDestination_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Get event receiver list.
 *
 * @param   coreHandle    TML core handle (TML_CORE_HANDLE)
 * @param   profile       profile name<br>
 *                        An empty string or TML_HANDLE_TYPE_NULL will return event receivers for all registered profiles.
 * @param   subscriptions reference to a table with event receiver data<br>
 *                        The table has to be released with sidex_Variant_DecRef().<br>
 *                        Table columns are "PROFILE", "HOST" and "PORT".<br>
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Subscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_Subscribed_MessageDestinations(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, SIDEX_VARIANT* subscriptions);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_Subscribed_MessageDestinations_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, SIDEX_VARIANT* subscriptions);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_Subscribed_MessageDestinations_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, SIDEX_VARIANT* subscriptions);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_Subscribed_MessageDestinations_A(TML_CORE_HANDLE coreHandle, const char* profile, SIDEX_VARIANT* subscriptions);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Get_Subscribed_MessageDestinations  tml_Evt_Get_Subscribed_MessageDestinations_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Get_Subscribed_MessageDestinations  tml_Evt_Get_Subscribed_MessageDestinations_W
    #else
      #define tml_Evt_Get_Subscribed_MessageDestinations  tml_Evt_Get_Subscribed_MessageDestinations_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Unsubscribe event receiver.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   sHost       host name or IP address
 * @param   sPort       port number
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Subscribe_MessageDestination(), tml_Evt_Unsubscribe_All_MessageDestinations()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_MessageDestination(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_MessageDestination_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_MessageDestination_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_MessageDestination_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sHost, const char* sPort);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Unsubscribe_MessageDestination  tml_Evt_Unsubscribe_MessageDestination_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Unsubscribe_MessageDestination  tml_Evt_Unsubscribe_MessageDestination_W
    #else
      #define tml_Evt_Unsubscribe_MessageDestination  tml_Evt_Unsubscribe_MessageDestination_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Unsubscribe event receiver.
 *
 * @param   connectionHandle  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   sProfile          profile identification string
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_Evt_Subscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Unsubscribe_MessageDestination(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Unsubscribe_MessageDestination_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Unsubscribe_MessageDestination_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Unsubscribe_MessageDestination_A(TML_CONNECTION_HANDLE connectionHandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Evt_Unsubscribe_MessageDestination  tml_Connection_Evt_Unsubscribe_MessageDestination_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Evt_Unsubscribe_MessageDestination  tml_Connection_Evt_Unsubscribe_MessageDestination_W
    #else
      #define tml_Connection_Evt_Unsubscribe_MessageDestination  tml_Connection_Evt_Unsubscribe_MessageDestination_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Unsubscribe all event receivers.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name<br>
 *                      An empty string or TML_HANDLE_TYPE_NULL will unsubscribe all event receivers for any registered profile.
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Unsubscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_All_MessageDestinations(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_All_MessageDestinations_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_All_MessageDestinations_W(TML_CORE_HANDLE coreHandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_All_MessageDestinations_A(TML_CORE_HANDLE coreHandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Unsubscribe_All_MessageDestinations  tml_Evt_Unsubscribe_All_MessageDestinations_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Unsubscribe_All_MessageDestinations  tml_Evt_Unsubscribe_All_MessageDestinations_W
    #else
      #define tml_Evt_Unsubscribe_All_MessageDestinations  tml_Evt_Unsubscribe_All_MessageDestinations_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Register event error handler.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL.
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see TML_ON_EVT_ERROR_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnError(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnError_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnError_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnError_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Set_OnError  tml_Evt_Set_OnError_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Set_OnError  tml_Evt_Set_OnError_W
    #else
      #define tml_Evt_Set_OnError  tml_Evt_Set_OnError_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Register event queue overflow handler.
 *
 * Events are internally queued before they are sent. If the number of queue entries reaches it's maximum
 * the overflow handler is called.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL.
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Get_MaxQueuedEventMessages(), tml_Evt_Set_MaxQueuedEventMessages(), tml_Evt_Send_Message(), TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnQueueOverflow(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnQueueOverflow_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnQueueOverflow_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnQueueOverflow_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Set_OnQueueOverflow  tml_Evt_Set_OnQueueOverflow_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Set_OnQueueOverflow  tml_Evt_Set_OnQueueOverflow_W
    #else
      #define tml_Evt_Set_OnQueueOverflow  tml_Evt_Set_OnQueueOverflow_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup  eventIO
 * @brief    Register on populate handler.
 *
 * If no event receiver is registered and an event is added to the queue by calling tml_Evt_Send_Message(),
 * the OnPopulate handler can register or reregister event receivers.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see  tml_Evt_Send_Message(), TML_ON_POPULATE_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPopulate(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPopulate_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPopulate_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPopulate_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Set_OnPopulate  tml_Evt_Set_OnPopulate_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Set_OnPopulate  tml_Evt_Set_OnPopulate_W
    #else
      #define tml_Evt_Set_OnPopulate  tml_Evt_Set_OnPopulate_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Send an event to all registered receivers.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   tmlhandle   TML command handle has to be released after the call with tml_Cmd_Free()
 * @param   profile     profile name
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INITIALIZATION initialization error<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED sender initialization failed<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_UNICODE unicode conversion error<br>
 *
 * @see tml_Evt_Subscribe_MessageDestination(), tml_Evt_Set_OnPopulate(), tml_Evt_Get_MaxQueuedEventMessages(), tml_Evt_Set_MaxQueuedEventMessages(), tml_Evt_Set_OnQueueOverflow()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_Message(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_Message_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_Message_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_Message_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Send_Message  tml_Evt_Send_Message_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Send_Message  tml_Evt_Send_Message_W
    #else
      #define tml_Evt_Send_Message  tml_Evt_Send_Message_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Send an event subscription request.
 *
 * With this function a command receiver can subscribe itself to receive event messages.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   sSourceHost host name / IP addresse of the subscriber
 * @param   sSourcePort port number of the subscriber
 * @param   sDestHost   host name / IP address of the event producer
 * @param   sDestPort   port number of the event producer
 * @param   iTimeout    timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Evt_Send_UnsubscriptionRequest()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_SubscriptionRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sSourceHost, const TML_CTSTR* sSourcePort, const TML_CTSTR* sDestHost, const TML_CTSTR* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_SubscriptionRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sSourceHost, const wchar_t* sSourcePort, const wchar_t* sDestHost, const wchar_t* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_SubscriptionRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sSourceHost, const char16_t* sSourcePort, const char16_t* sDestHost, const char16_t* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_SubscriptionRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Send_SubscriptionRequest  tml_Evt_Send_SubscriptionRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Send_SubscriptionRequest  tml_Evt_Send_SubscriptionRequest_W
    #else
      #define tml_Evt_Send_SubscriptionRequest  tml_Evt_Send_SubscriptionRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Send an event subscription request.
 *
 * With this function a command receiver can subscribe itself to receive event messages.
 *
 * @param  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param  sProfile         profile identification string
 * @param  iTimeout         timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_Evt_Send_UnsubscriptionRequest()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Send_SubscriptionRequest(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Send_SubscriptionRequest_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Send_SubscriptionRequest_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Send_SubscriptionRequest_A(TML_CONNECTION_HANDLE connectionHandle, const char* sProfile, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Evt_Send_SubscriptionRequest  tml_Connection_Evt_Send_SubscriptionRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Evt_Send_SubscriptionRequest  tml_Connection_Evt_Send_SubscriptionRequest_W
    #else
      #define tml_Connection_Evt_Send_SubscriptionRequest  tml_Connection_Evt_Send_SubscriptionRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Register a function to handle an event subscription request.
 *
 * The handler function is called if an event subscription or unsubscription request is received.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Send_SubscriptionRequest(), tml_Evt_Send_UnsubscriptionRequest(), TML_ON_PEER_REGISTRATION_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPeerRegister(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPeerRegister_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPeerRegister_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPeerRegister_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Set_OnPeerRegister  tml_Evt_Set_OnPeerRegister_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Set_OnPeerRegister  tml_Evt_Set_OnPeerRegister_W
    #else
      #define tml_Evt_Set_OnPeerRegister  tml_Evt_Set_OnPeerRegister_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup  eventIO
 * @brief    Unregister an event message receiver.
 *
 * With this function a command receiver can unsubscribe itself to receive event messages.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   sSourceHost host name / IP addresse of the subscriber
 * @param   sSourcePort port number of the subscriber
 * @param   sDestHost   host name / IP address of the event producer
 * @param   sDestPort   port number of the event producer
 * @param   iTimeout    timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Evt_Send_SubscriptionRequest()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_UnsubscriptionRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sSourceHost, const TML_CTSTR* sSourcePort, const TML_CTSTR* sDestHost, const TML_CTSTR* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_UnsubscriptionRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sSourceHost, const wchar_t* sSourcePort, const wchar_t* sDestHost, const wchar_t* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_UnsubscriptionRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sSourceHost, const char16_t* sSourcePort, const char16_t* sDestHost, const char16_t* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_UnsubscriptionRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Evt_Send_UnsubscriptionRequest  tml_Evt_Send_UnsubscriptionRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Evt_Send_UnsubscriptionRequest  tml_Evt_Send_UnsubscriptionRequest_W
    #else
      #define tml_Evt_Send_UnsubscriptionRequest  tml_Evt_Send_UnsubscriptionRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  eventIO
 * @brief    Unregister an event message receiver.
 *
 * With this function a command receiver can unsubscribe itself to receive event messages.
 *
 * @param  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param  sProfile         profile identification string
 * @param  iTimeout         timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_Evt_Send_SubscriptionRequest()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Send_UnsubscriptionRequest(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Send_UnsubscriptionRequest_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Send_UnsubscriptionRequest_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Evt_Send_UnsubscriptionRequest_A(TML_CONNECTION_HANDLE connectionHandle, const char* sProfile, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Evt_Send_UnsubscriptionRequest  tml_Connection_Evt_Send_UnsubscriptionRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Evt_Send_UnsubscriptionRequest  tml_Connection_Evt_Send_UnsubscriptionRequest_W
    #else
      #define tml_Connection_Evt_Send_UnsubscriptionRequest  tml_Connection_Evt_Send_UnsubscriptionRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Add a command receiver for load balanced calls.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   sHost       host name / IP address
 * @param   sPort       port number
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Get_Subscribed_MessageDestinations(), tml_Bal_Unsubscribe_MessageDestination() 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Subscribe_MessageDestination(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Subscribe_MessageDestination_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Subscribe_MessageDestination_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Subscribe_MessageDestination_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sHost, const char* sPort);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Subscribe_MessageDestination  tml_Bal_Subscribe_MessageDestination_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Subscribe_MessageDestination  tml_Bal_Subscribe_MessageDestination_W
    #else
      #define tml_Bal_Subscribe_MessageDestination  tml_Bal_Subscribe_MessageDestination_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Add a command receiver for load balanced calls.
 *
 * @param   connectionHandle  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   sProfile          profile identification string
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_Bal_Unsubscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Subscribe_MessageDestination(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Subscribe_MessageDestination_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Subscribe_MessageDestination_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Subscribe_MessageDestination_A(TML_CONNECTION_HANDLE connectionHandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Bal_Subscribe_MessageDestination  tml_Connection_Bal_Subscribe_MessageDestination_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Bal_Subscribe_MessageDestination  tml_Connection_Bal_Subscribe_MessageDestination_W
    #else
      #define tml_Connection_Bal_Subscribe_MessageDestination  tml_Connection_Bal_Subscribe_MessageDestination_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Get load balanced command receiver list.
 *
 * @param   coreHandle    TML core handle (TML_CORE_HANDLE)
 * @param   profile       profile name<br>
 *                        An empty string or TML_HANDLE_TYPE_NULL will return receivers for all registered profiles.
 * @param   subscriptions reference to a table with command receiver data<br>
 *                        The table has to be released with sidex_Variant_DecRef().
 *                        Table columns are "PROFILE", "HOST" and "PORT".
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Subscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_Subscribed_MessageDestinations(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, SIDEX_VARIANT* subscriptions);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_Subscribed_MessageDestinations_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, SIDEX_VARIANT* subscriptions);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_Subscribed_MessageDestinations_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, SIDEX_VARIANT* subscriptions);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_Subscribed_MessageDestinations_A(TML_CORE_HANDLE coreHandle, const char* profile, SIDEX_VARIANT* subscriptions);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Get_Subscribed_MessageDestinations  tml_Bal_Get_Subscribed_MessageDestinations_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Get_Subscribed_MessageDestinations  tml_Bal_Get_Subscribed_MessageDestinations_W
    #else
      #define tml_Bal_Get_Subscribed_MessageDestinations  tml_Bal_Get_Subscribed_MessageDestinations_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Unsubscribe load balanced command receiver.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   sHost       host name / IP address
 * @param   sPort       port number
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Subscribe_MessageDestination, tml_Bal_Unsubscribe_All_MessageDestinations()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_MessageDestination(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_MessageDestination_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_MessageDestination_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_MessageDestination_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sHost, const char* sPort);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Unsubscribe_MessageDestination  tml_Bal_Unsubscribe_MessageDestination_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Unsubscribe_MessageDestination  tml_Bal_Unsubscribe_MessageDestination_W
    #else
      #define tml_Bal_Unsubscribe_MessageDestination  tml_Bal_Unsubscribe_MessageDestination_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Unsubscribe a command receiver for load balanced calls.
 *
 * @param   connectionHandle  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   sProfile          profile identification string
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_Bal_Subscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Unsubscribe_MessageDestination(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Unsubscribe_MessageDestination_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Unsubscribe_MessageDestination_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Unsubscribe_MessageDestination_A(TML_CONNECTION_HANDLE connectionHandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Bal_Unsubscribe_MessageDestination  tml_Connection_Bal_Unsubscribe_MessageDestination_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Bal_Unsubscribe_MessageDestination  tml_Connection_Bal_Unsubscribe_MessageDestination_W
    #else
      #define tml_Connection_Bal_Unsubscribe_MessageDestination  tml_Connection_Bal_Unsubscribe_MessageDestination_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Clear load balanced command receiver list.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile       profile name<br>
 *                        If empty or TML_HANDLE_TYPE_NULL all receiver list of all registered profiles are cleared.
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Unsubscribe_MessageDestination()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_All_MessageDestinations(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_All_MessageDestinations_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_All_MessageDestinations_W(TML_CORE_HANDLE coreHandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_All_MessageDestinations_A(TML_CORE_HANDLE coreHandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Unsubscribe_All_MessageDestinations  tml_Bal_Unsubscribe_All_MessageDestinations_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Unsubscribe_All_MessageDestinations  tml_Bal_Unsubscribe_All_MessageDestinations_W
    #else
      #define tml_Bal_Unsubscribe_All_MessageDestinations  tml_Bal_Unsubscribe_All_MessageDestinations_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Register on populate handler. 
 *
 * If no load balanced command receiver is registered and a load balanced command is requested by tml_Bal_Send_SyncMessage() or tml_Bal_Send_AsyncMessage(), 
 * the OnPopulate handler can register or reregister load balanced command receiver.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Send_AsyncMessage(), tml_Bal_Send_SyncMessage(), TML_ON_POPULATE_CB_FUNC() 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPopulate(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPopulate_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPopulate_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPopulate_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Set_OnPopulate  tml_Bal_Set_OnPopulate_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Set_OnPopulate  tml_Bal_Set_OnPopulate_W
    #else
      #define tml_Bal_Set_OnPopulate  tml_Bal_Set_OnPopulate_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup  loadbalancedIO
 * @brief    Register a busy status request handler.
 *
 * To implement a custom load balancing instead of a round robin the status request handler can
 * collect and return host specifc data. The data is added to the TML command passed to the handler function.
 * On sender side the data is collected and passed to a load balance calculation handler (see tml_Bal_Set_OnCalculation()) that
 * will return the index of the next registered peer to call. If either the busy status request handler or the load balancing
 * calculation handler is not implemented round robin will be used.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC(), tml_Bal_Set_OnCalculation()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnBusyStatusRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnBusyStatusRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnBusyStatusRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnBusyStatusRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Set_OnBusyStatusRequest  tml_Bal_Set_OnBusyStatusRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Set_OnBusyStatusRequest  tml_Bal_Set_OnBusyStatusRequest_W
    #else
      #define tml_Bal_Set_OnBusyStatusRequest  tml_Bal_Set_OnBusyStatusRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup  loadbalancedIO
 * @brief    Register load balancing calculation handler.
 *
 * Evaluate load information returned by the status request handlers of registered command receivers to calculate
 * the index of the next peer to receive a load balanced command. 
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Send_SyncMessage(), tml_Bal_Send_AsyncMessage(), tml_Bal_Set_OnBusyStatusRequest(), TML_ON_BAL_CALCULATION_REQ_CB_FUNC() 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnCalculation(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnCalculation_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnCalculation_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnCalculation_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Set_OnCalculation  tml_Bal_Set_OnCalculation_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Set_OnCalculation  tml_Bal_Set_OnCalculation_W
    #else
      #define tml_Bal_Set_OnCalculation  tml_Bal_Set_OnCalculation_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Send a load balanced synchronous message / command.
 *
 * This call works like tml_Send_SyncMessage() but sends the command not to specific peer but to
 * a group of registered command receivers utilizing the TML load balancing.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   tmlhandle   TML command handle (TML_COMMAND_HANDLE)
 * @param   profile     profile name
 * @param   iTimeout    timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_DESTINATION_OBJ_NOT_FOUND no command receiver registered<br>
 *          TML_ERR_TIMEOUT timeout before reply<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_LOAD_BALANCE_CALCULATION error in load balancing calculation<br>
 *
 * @see tml_Send_SyncMessage(), tml_Bal_Send_AsyncMessage()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SyncMessage(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SyncMessage_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SyncMessage_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SyncMessage_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Send_SyncMessage  tml_Bal_Send_SyncMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Send_SyncMessage  tml_Bal_Send_SyncMessage_W
    #else
      #define tml_Bal_Send_SyncMessage  tml_Bal_Send_SyncMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Send a load balanced asynchronous message / command.
 *
 * This call works like tml_Send_AsyncMessage() but sends the command not to specific peer but to
 * a group of registered command receivers utilizing the TML load balancing.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   tmlhandle   TML command handle (TML_COMMAND_HANDLE)
 * @param   profile     profile name
 * @param   iTimeout    timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_DESTINATION_OBJ_NOT_FOUND no command receiver registered<br>
 *          TML_ERR_TIMEOUT timeout before reply<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_LOAD_BALANCE_CALCULATION error in load balancing calculation<br>
 *
 * @see tml_Send_AsyncMessage(), tml_Bal_Send_SyncMessage()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_AsyncMessage(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_AsyncMessage_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_AsyncMessage_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_AsyncMessage_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Send_AsyncMessage  tml_Bal_Send_AsyncMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Send_AsyncMessage  tml_Bal_Send_AsyncMessage_W
    #else
      #define tml_Bal_Send_AsyncMessage  tml_Bal_Send_AsyncMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Send a load balancing subscription request.
 *
 * With this function a command receiver can subscribe itself to receive load balanced commands.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   sSourceHost host name / IP address of the subscriber
 * @param   sSourcePort port number of the subscriber
 * @param   sDestHost   host name / IP address of the destination
 * @param   sDestPort   port number of the destination
 * @param   iTimeout    timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Bal_Send_UnsubscriptionRequest()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SubscriptionRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sSourceHost, const TML_CTSTR* sSourcePort, const TML_CTSTR* sDestHost, const TML_CTSTR* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SubscriptionRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sSourceHost, const wchar_t* sSourcePort, const wchar_t* sDestHost, const wchar_t* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SubscriptionRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sSourceHost, const char16_t* sSourcePort, const char16_t* sDestHost, const char16_t* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SubscriptionRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Send_SubscriptionRequest  tml_Bal_Send_SubscriptionRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Send_SubscriptionRequest  tml_Bal_Send_SubscriptionRequest_W
    #else
      #define tml_Bal_Send_SubscriptionRequest  tml_Bal_Send_SubscriptionRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Send a load balancing subscription request.
 *
 * With this function a command receiver can subscribe itself to receive load balanced commands.
 *
 * @param  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param  sProfile         profile identification string
 * @param  iTimeout         timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_Bal_Send_UnsubscriptionRequest()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Send_SubscriptionRequest(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Send_SubscriptionRequest_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Send_SubscriptionRequest_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Send_SubscriptionRequest_A(TML_CONNECTION_HANDLE connectionHandle, const char* sProfile, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Bal_Send_SubscriptionRequest  tml_Connection_Bal_Send_SubscriptionRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Bal_Send_SubscriptionRequest  tml_Connection_Bal_Send_SubscriptionRequest_W
    #else
      #define tml_Connection_Bal_Send_SubscriptionRequest  tml_Connection_Bal_Send_SubscriptionRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Register a function to handle a load balancing subscription request.
 *
 * The handler function is called if a load balancing subscription or unsubscription request is received.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   profile    profile name
 * @param   pCBFunc    function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Send_SubscriptionRequest(), tml_Bal_Send_UnsubscriptionRequest(), TML_ON_PEER_REGISTRATION_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPeerRegister(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPeerRegister_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPeerRegister_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPeerRegister_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Set_OnPeerRegister  tml_Bal_Set_OnPeerRegister_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Set_OnPeerRegister  tml_Bal_Set_OnPeerRegister_W
    #else
      #define tml_Bal_Set_OnPeerRegister  tml_Bal_Set_OnPeerRegister_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup  loadbalancedIO
 * @brief    Unregister a load balancing command receiver.
 *
 * With this function a command receiver can unsubscribe itself to receive load balanced commands.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   profile     profile name
 * @param   sSourceHost host name / IP address of the subscriber
 * @param   sSourcePort port number of the subscriber
 * @param   sDestHost   host name / IP address of the destination
 * @param   sDestPort   port number of the destination
 * @param   iTimeout    timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Bal_Send_SubscriptionRequest()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_UnsubscriptionRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sSourceHost, const TML_CTSTR* sSourcePort, const TML_CTSTR* sDestHost, const TML_CTSTR* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_UnsubscriptionRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sSourceHost, const wchar_t* sSourcePort, const wchar_t* sDestHost, const wchar_t* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_UnsubscriptionRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sSourceHost, const char16_t* sSourcePort, const char16_t* sDestHost, const char16_t* sDestPort, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_UnsubscriptionRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Bal_Send_UnsubscriptionRequest  tml_Bal_Send_UnsubscriptionRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Bal_Send_UnsubscriptionRequest  tml_Bal_Send_UnsubscriptionRequest_W
    #else
      #define tml_Bal_Send_UnsubscriptionRequest  tml_Bal_Send_UnsubscriptionRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Unregister a load balancing command receiver.
 *
 * With this function a command receiver can unsubscribe itself to receive load balanced commands.
 *
 * @param  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param  sProfile         profile identification string
 * @param  iTimeout         timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_Bal_Send_SubscriptionRequest()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Send_UnsubscriptionRequest(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Send_UnsubscriptionRequest_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Send_UnsubscriptionRequest_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* sProfile, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Bal_Send_UnsubscriptionRequest_A(TML_CONNECTION_HANDLE connectionHandle, const char* sProfile, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Bal_Send_UnsubscriptionRequest  tml_Connection_Bal_Send_UnsubscriptionRequest_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Bal_Send_UnsubscriptionRequest  tml_Connection_Bal_Send_UnsubscriptionRequest_W
    #else
      #define tml_Connection_Bal_Send_UnsubscriptionRequest  tml_Connection_Bal_Send_UnsubscriptionRequest_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  loadbalancedIO
 * @brief    Set maximum load balancing connection fail count.
 *
 * The maximum connection fail count defines after how many unsuccessful connections a command receiver is
 * automatically removed from the list. A value of 0 means it is never removed. The default value is 1.
 * 
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iCount      maximum fail count
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Get_MaxConnectionFailCount()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_MaxConnectionFailCount(TML_CORE_HANDLE coreHandle, TML_UINT32 iCount);


/**
 * @ingroup  loadbalancedIO
 * @brief    Get maximum load balancing connection fail count.
 *
 * The maximum connection fail count defines after how many unsuccessful connections a command receiver is
 * automatically removed from the list. A value of 0 means it is never removed. The default value is 1.
 * 
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iCount      reference to maximum fail count
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Bal_Set_MaxConnectionFailCount()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_MaxConnectionFailCount(TML_CORE_HANDLE coreHandle, TML_UINT32* iCount);


/**
 * @ingroup  eventIO
 * @brief    Set the maximum event connection fail count.
 *
 * The maximum connection fail count defines after how many unsuccessful connections an event consumer is
 * automatically removed from the list. A value of 0 means it is never removed. The default value is 1.
 * 
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iCount      maximum fail count
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Get_MaxConnectionFailCount()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_MaxConnectionFailCount(TML_CORE_HANDLE coreHandle, TML_UINT32 iCount);


/**
 * @ingroup  eventIO
 * @brief    Get the maximum event connection fail count.
 *
 * The maximum connection fail count defines after how many unsuccessful connections an event consumer is
 * automatically removed from the list. A value of 0 means it is never removed. The default value is 1.
 * 
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iCount      reference to maximum fail count
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Set_MaxConnectionFailCount()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_MaxConnectionFailCount(TML_CORE_HANDLE coreHandle, TML_UINT32* iCount);


/**
 * @ingroup  eventIO
 * @brief    Set the maximum queued events.
 *
 * If the maximum number of queued events for an event consumer is reached, the oldest event will be deleted.
 * The default value is 1000.
 * 
 * @param   coreHandle    TML core handle (TML_CORE_HANDLE)
 * @param   iMaximum      maximum queue count
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Send_Message(), tml_Evt_Get_MaxQueuedEventMessages(), tml_Evt_Set_OnQueueOverflow()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_MaxQueuedEventMessages(TML_CORE_HANDLE coreHandle, TML_UINT32 iMaximum);


/**
 * @ingroup  eventIO
 * @brief    Get the maximum value of queued event messages
 *
 * If the maximum number of queued events for an event consumer is reached, the oldest event will be deleted.
 * The default value is 1000.
 * 
 * @param   coreHandle    TML core handle (TML_CORE_HANDLE)
 * @param   iMaximum      reference to maximum queue count
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Evt_Send_Message(), tml_Evt_Set_MaxQueuedEventMessages(), tml_Evt_Set_OnQueueOverflow()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_MaxQueuedEventMessages(TML_CORE_HANDLE coreHandle, TML_UINT32* iMaximum);


/**
 * @ingroup tmlHandle
 * @brief    Create a TML command.
 *
 * @param    tmlhandle reference to a TML commandhandle (TML_COMMAND_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INITIALIZATION initialization error
 *
 * @see tml_Cmd_Free()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Create(TML_COMMAND_HANDLE* tmlhandle);


/**
 * @ingroup tmlHandle
 * @brief    Free TML command handle and release data.
 *
 * @param   tmlhandle reference to a TML command handle (TML_COMMAND_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid TML command handle
 *
 * @see tml_Cmd_Create()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Free(TML_COMMAND_HANDLE* tmlhandle);


/**
 * @ingroup tmlCallbacks
 * @brief   Register a progress reply handler.
 *
 * The progress reply handler is called if a progress reply was received for a command.
 *
 * @param   tmlhandle  TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBFunc    function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns  TML_SUCCESS in case of success.
 *
 * @see tml_Send_AsyncProgressReply(), TML_ON_PROGRESS_REPLY_CB_FUNC(), tml_Cmd_Registered_Progress()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_Progress(TML_COMMAND_HANDLE tmlhandle, TML_ON_PROGRESS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData);


/**
 * @ingroup tmlCallbacks
 * @brief   Get registered progress reply handler and custom data.
 *
 * The progress reply handler is called if a progress reply was received for a command.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBFunc    reference to handler function or NULL
 * @param   pCBData    reference to custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns  TML_SUCCESS in case of success
 *
 * @see tml_Cmd_Register_Progress()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Registered_Progress(TML_COMMAND_HANDLE tmlhandle, TML_ON_PROGRESS_REPLY_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


/**
 * @ingroup tmlCallbacks
 * @brief   Register a status reply handler.
 *
 * The status reply handler is called if a status reply was received for a command.
 *
 * @param   tmlhandle  TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBFunc    handler function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns  TML_SUCCESS in case of success.
 *
 * @see tml_Send_AsyncStatusReply(), TML_ON_STATUS_REPLY_CB_FUNC(), tml_Cmd_Registered_StatusReply()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_StatusReply(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_StatusReply_X(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_StatusReply_W(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_StatusReply_A(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Register_StatusReply  tml_Cmd_Register_StatusReply_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Register_StatusReply  tml_Cmd_Register_StatusReply_W
    #else
      #define tml_Cmd_Register_StatusReply  tml_Cmd_Register_StatusReply_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

/**
 * @ingroup tmlCallbacks
 * @brief   Get registered status reply handler and custom data.
 *
 * The status reply handler is called if a status reply was received for a command.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBFunc    reference to handler function or NULL
 * @param   pCBData    reference to custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns  TML_SUCCESS in case of success 
 *
 * @see tml_Send_AsyncStatusReply(), tml_Cmd_Register_StatusReply()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Registered_StatusReply(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC* pCBFunc, TML_POINTER* pCBData);

/**
 * @ingroup tmlCallbacks
 * @brief   Register command ready handler.
 *
 * The command ready handler is called if a command receives its final reply or if an error occurred.
 *
 * @param   tmlhandle  TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBFunc    handler function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns  TML_SUCCESS in case of success
 *
 * @see tml_Send_AsyncMessage(), tml_Connection_SendAsync(), tml_Bal_Send_AsyncMessage(), TML_ON_COMMAND_READY_CB_FUNC(), tml_Cmd_Registered_CommandReady()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_CommandReady(TML_COMMAND_HANDLE tmlhandle, TML_ON_COMMAND_READY_CB_FUNC pCBFunc, TML_POINTER pCBData);

/**
 * @ingroup tmlCallbacks
 * @brief   Get registered command ready handler and custom data.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   pCBFunc    reference to handler function or NULL
 * @param   pCBData    reference to custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns  TML_SUCCESS in case of success 
 *
 * @see TML_ON_COMMAND_READY_CB_FUNC(), tml_Cmd_Register_CommandReady() 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Registered_CommandReady(TML_COMMAND_HANDLE tmlhandle, TML_ON_COMMAND_READY_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup tmlAttributes
 * @brief    Set listener core reference.
 *
 * This function will be used on every incoming data by the internal listener process.<br>
 * @note For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   tmlhandle      TML command handle (TML_COMMAND_HANDLE)
 * @param   tmlcorehandle  TML core handle (TML_CORE_HANDLE)
 *
 * @returns  TML_SUCCESS in case of success
 *
 * @see tml_Cmd_Attr_Get_Core_Reference()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Core_Reference(TML_COMMAND_HANDLE tmlhandle, TML_CORE_HANDLE tmlcorehandle);


/**
 * @ingroup tmlAttributes
 * @brief    Get listener core reference. 
 *
 * @param   tmlhandle      TML command handle (TML_COMMAND_HANDLE)
 * @param   tmlcorehandle  TML core handle (TML_CORE_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_ATTRIBUTE_NOT_SET missing attribute
 *
 * @see tml_Cmd_Attr_Set_Core_Reference()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Core_Reference(TML_COMMAND_HANDLE tmlhandle, TML_CORE_HANDLE* tmlcorehandle);


/**
 * @ingroup tmlAttributes
 * @brief    Set listener communication profile. 
 *
 * This API call will be used on every incoming data by the internal listener process.<br>
 * @node For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   profile   profile name
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion
 *
 * @see tml_Cmd_Attr_Get_Profile()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Profile(TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Profile_X(TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Profile_W(TML_COMMAND_HANDLE tmlhandle, const char16_t* profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Profile_A(TML_COMMAND_HANDLE tmlhandle, const char* profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Attr_Set_Profile  tml_Cmd_Attr_Set_Profile_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Attr_Set_Profile  tml_Cmd_Attr_Set_Profile_W
    #else
      #define tml_Cmd_Attr_Set_Profile  tml_Cmd_Attr_Set_Profile_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tmlAttributes
 * @brief    Get listener communication profile. 
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   profile   borrowed reference to profile name
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_ATTRIBUTE_NOT_SET missing attribute
 *
 * @see tml_Cmd_Attr_Set_Profile()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Profile(TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR** profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Profile_X(TML_COMMAND_HANDLE tmlhandle, const wchar_t** profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Profile_W(TML_COMMAND_HANDLE tmlhandle, const char16_t** profile);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Profile_A(TML_COMMAND_HANDLE tmlhandle, const char** profile);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Attr_Get_Profile  tml_Cmd_Attr_Get_Profile_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Attr_Get_Profile  tml_Cmd_Attr_Get_Profile_W
    #else
      #define tml_Cmd_Attr_Get_Profile  tml_Cmd_Attr_Get_Profile_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tmlAttributes
 * @brief    Set listener communication session ID. 
 *
 * This API call will be used on every incoming data by the internal listener process.<br>
 * @node For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   iSesionID session id
 *
 * @returns  TML_SUCCESS in case of success
 *
 * @see tml_Cmd_Attr_Get_Session_ID()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Session_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iSesionID);


/**
 * @ingroup tmlAttributes
 * @brief    Get session ID. 
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   iSesionID reference to session id
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_ATTRIBUTE_NOT_SET missing attribute
 *
 * @see tml_Cmd_Attr_Set_Session_ID()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Session_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32* iSesionID);


/**
 * @ingroup tmlAttributes
 * @brief    Set listener channel. 
 *
 * This API call will be used on every incoming data by the internal listener process.<br>
 * @note For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   channel TML channel
 *
 * @returns  TML_SUCCESS in case of success
 *
 * @see tml_Cmd_Attr_Get_Channel()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Channel(TML_COMMAND_HANDLE tmlhandle, TML_POINTER channel);


/**
 * @ingroup tmlAttributes
 * @brief    Get listener channel. 
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   channel reference to TML channel
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_ATTRIBUTE_NOT_SET missing attribute
 *
 * @see tml_Cmd_Attr_Set_Channel()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Channel(TML_COMMAND_HANDLE tmlhandle, TML_POINTER* channel);


/**
 * @ingroup tmlAttributes
 * @brief    Set channel ID. 
 *
 * This API call will be used on every incoming data by the internal listener process.<br>
 * @node For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   channelID channel id
 *
 * @returns  TML_SUCCESS in case of success
 *
 * @see tml_Cmd_Attr_Get_Channel_ID()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Channel_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32 channelID);


/**
 * @ingroup tmlAttributes
 * @brief    Get channel ID. 
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   channelID reference to channel id
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_ATTRIBUTE_NOT_SET missing attribute
 *
 * @see tml_Cmd_Attr_Set_Channel_ID()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Channel_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32* channelID);


/**
 * @ingroup tmlAttributes
 * @brief    Set message ID. 
 *
 * This API call will be used on every incoming data by the internal listener process.<br>
 * @node For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   iMessageID message id
 *
 * @returns  TML_SUCCESS in case of success
 *
 * @see tml_Cmd_Attr_Get_Message_ID()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Message_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iMessageID);


/**
 * @ingroup tmlAttributes
 * @brief    Get message ID. 
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   iMessageID reference to message id
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_ATTRIBUTE_NOT_SET missing attribute.
 *
 * @see tml_Cmd_Attr_Set_Message_ID()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Message_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32* iMessageID);

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

/**
 * @ingroup tmlheader
 * @brief Get command creation time.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param time reference to the command creation time (string)<br>
 *             ISO formated: YYYYMMDDhhmmssttt<br>
 *             YYYY = year, MM = month, DD= day, hh = hour, mm = minute, ss = second, ttt = miliseconds
 *             Must be deallocated using the data type API sidex_Free_ReadString().
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_INFORMATION_UNDEFINED information is missing
 *
 * @see sidex_Free_ReadString()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCreationTime(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR** time);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCreationTime_X(TML_COMMAND_HANDLE tmlhandle, wchar_t** time);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCreationTime_W(TML_COMMAND_HANDLE tmlhandle, char16_t** time);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCreationTime_A(TML_COMMAND_HANDLE tmlhandle, char** time);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Header_GetCreationTime  tml_Cmd_Header_GetCreationTime_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Header_GetCreationTime  tml_Cmd_Header_GetCreationTime_W
    #else
      #define tml_Cmd_Header_GetCreationTime  tml_Cmd_Header_GetCreationTime_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup tmlheader
 * @brief Set the command creation time.
 *
 * @note For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param time command creation time (string)<br>
 *             ISO formated: YYYYMMDDhhmmssttt<br>
 *             YYYY = year, MM = month, DD= day, hh = hour, mm = minute, ss = second, ttt = miliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INITIALIZATION initialization error<br>
 *          TML_ERR_UNICODE error in unicode conversion
 *
 * @see tml_Cmd_Header_GetCreationTime()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCreationTime(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR* time);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCreationTime_X(TML_COMMAND_HANDLE tmlhandle, wchar_t* time);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCreationTime_W(TML_COMMAND_HANDLE tmlhandle, char16_t* time);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCreationTime_A(TML_COMMAND_HANDLE tmlhandle, char* time);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Header_SetCreationTime  tml_Cmd_Header_SetCreationTime_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Header_SetCreationTime  tml_Cmd_Header_SetCreationTime_W
    #else
      #define tml_Cmd_Header_SetCreationTime  tml_Cmd_Header_SetCreationTime_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

/**
 * @ingroup tmlheader
 * @brief Get the command id.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param cmd       reference to command id
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED information is missing
 *
 * @see tml_Cmd_Header_SetCommand()
 */
 TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCommand(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE* cmd);


/**
 * @ingroup tmlheader
 * @brief Set the command id.
 *
 * A command ID identifies the command and is necessary to process the command a the receiver.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param cmd       command id
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INITIALIZATION initialization error
 *
 * @see tml_Cmd_Header_GetCommand()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCommand(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE cmd);


/**
 * @ingroup tmlheader
 * @brief Get the error code.
 *
 * There are a number of predefined error codes that are set automatically. Besides that a
 * custom error code can be set while processing a command to inform the caller about problems related
 * to implementation specific problems.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param error reference to the error code
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED information is missing
 *
 * @see tml_Cmd_Header_SetError()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetError(TML_COMMAND_HANDLE tmlhandle, TML_INT32* error);


/**
 * @ingroup tmlheader
 * @brief Set the error code.
 *
 * If a custom error code is set while command processing it will not be overwritten by TML specific standard
 * error codes. 
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param error     error code
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INITIALIZATION initialization error<br>
 *
 * @see tml_Cmd_Header_GetError()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetError(TML_COMMAND_HANDLE tmlhandle, TML_INT32 error);


/**
 * @ingroup tmlheader
 * @brief Get error message.
 *
 * @param tmlhandle  TML command handle (TML_COMMAND_HANDLE)
 * @param msg        reference to error message (must be deallocated with sidex_Free_ReadString())
 * @param iMsgLength reference to message length
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_INFORMATION_UNDEFINED information is missing
 *
 * @see tml_Cmd_Header_SetErrorMessage(), sidex_Free_ReadString()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetErrorMessage(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR** msg, TML_INT32* iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetErrorMessage_X(TML_COMMAND_HANDLE tmlhandle, wchar_t** msg, TML_INT32* iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetErrorMessage_W(TML_COMMAND_HANDLE tmlhandle, char16_t** msg, TML_INT32* iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetErrorMessage_A(TML_COMMAND_HANDLE tmlhandle, char** msg, TML_INT32* iMsgLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Header_GetErrorMessage  tml_Cmd_Header_GetErrorMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Header_GetErrorMessage  tml_Cmd_Header_GetErrorMessage_W
    #else
      #define tml_Cmd_Header_GetErrorMessage  tml_Cmd_Header_GetErrorMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tmlheader
 * @brief Set error message.
 *
 * If a custom error message is set while command processing it will not be overwritten by TML specific standard
 * error messages. 
 *
 * @param tmlhandle  TML command handle (TML_COMMAND_HANDLE)
 * @param msg        error message
 * @param iMsgLength message length
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INITIALIZATION initialization error<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *
 * @see tml_Cmd_Header_GetErrorMessage()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetErrorMessage(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR* msg, TML_INT32 iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetErrorMessage_X(TML_COMMAND_HANDLE tmlhandle, wchar_t* msg, TML_INT32 iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetErrorMessage_W(TML_COMMAND_HANDLE tmlhandle, char16_t* msg, TML_INT32 iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetErrorMessage_A(TML_COMMAND_HANDLE tmlhandle, char* msg, TML_INT32 iMsgLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Header_SetErrorMessage  tml_Cmd_Header_SetErrorMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Header_SetErrorMessage  tml_Cmd_Header_SetErrorMessage_W
    #else
      #define tml_Cmd_Header_SetErrorMessage  tml_Cmd_Header_SetErrorMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tmlheader
 * @brief Get execution state.
 *
 *  <TABLE>
 *   <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
 *   <TR><TD>TMLCOM_CSTATE_CREATED   </TD><TD>0</TD><TD>initial command state</TD></TR>
 *   <TR><TD>TMLCOM_CSTATE_EXECUTED   </TD><TD>1</TD><TD>command successfully executed</TD></TR>
 *   <TR><TD>TMLCOM_CSTATE_FAILED   </TD><TD>2</TD><TD>command execution failed</TD></TR>
 *   <TR><TD>TMLCOM_CSTATE_PENDING   </TD><TD>3</TD><TD>command execution pending</TD></TR>
 *   <TR><TD>TMLCOM_CSTATE_UNDEFINED  </TD><TD>-1</TD><TD>undefined command state</TD></TR>
 *  </TABLE>
 * 
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param state reference to execution state
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED information is missing
 *
 * @see tml_Cmd_Header_SetState()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetState(TML_COMMAND_HANDLE tmlhandle, TML_INT32* state);

/**
 * @ingroup tmlheader
 * @brief Set execution state.
 *
 * It is not recommended to change the command state. This values is used internally. Changing the command state
 * might result in unpredictable results.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param state execution state
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_COMMAND_STATE_UNDEFINED unknown command state<br>
 *          TML_ERR_INITIALIZATION initialization error<br>
 *
 * @see tml_Cmd_Header_GetState()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetState(TML_COMMAND_HANDLE tmlhandle, TML_INT32 state);


/**
 * @ingroup tmlheader
 * @brief Get execution mode.
 *
 *  <TABLE>
 *   <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
 *   <TR><TD>TMLCOM_MODE_ASYNC   </TD><TD>0</TD><TD>asynchronous command call</TD></TR>
 *   <TR><TD>TMLCOM_MODE_SYNC   </TD><TD>1</TD><TD>synchronous command call</TD></TR>
 *   <TR><TD>TMLCOM_MODE_EVT   </TD><TD>2</TD><TD>event call</TD></TR>
 *  </TABLE>
 * 
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param mode      reference to execution mode
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED information is missing
 *
 * @see tml_Cmd_Header_SetMode()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetMode(TML_COMMAND_HANDLE tmlhandle, TML_INT32* mode);


/**
 * @ingroup tmlheader
 * @brief Set execution mode.
 *
 * It is not recommended to change the execution mode. This values is used internally. Changing the command state
 * might result in unpredictable results.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param mode      execution mode
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_COMMAND_MODE_UNDEFINED unknown execution mode<br>
 *          TML_ERR_INITIALIZATION initialization error
 *
 * @see tml_Cmd_Header_GetMode()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetMode(TML_COMMAND_HANDLE tmlhandle, TML_INT32 mode);


/**
 * @ingroup tmlheader
 * @brief Get reply type.
 *
 * If a progress or status reply is received, the command execution state is TMLCOM_CSTATE_PENDING and the
 * reply type can be used to determine the kind of reply.
 *
 *  <TABLE>
 *   <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
 *   <TR><TD>TMLCOM_RPY_PROGRESS   </TD><TD>0</TD><TD>progress reply</TD></TR>
 *   <TR><TD>TMLCOM_RPY_WARNING   </TD><TD>10</TD><TD>warning status reply</TD></TR>
 *   <TR><TD>TMLCOM_RPY_ERROR   </TD><TD>20</TD><TD>error status reply</TD></TR>
 *   <TR><TD>TMLCOM_RPY_INFORMATION   </TD><TD>30</TD><TD>information status reply</TD></TR>
 *   <TR><TD>TMLCOM_CSTATE_UNDEFINED   </TD><TD>-1</TD><TD>undefined reply type</TD></TR>
 *  </TABLE>
 * 
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param type      reference to reply type
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED information is missing
 *
 * @see tml_Cmd_Header_SetReplyType(), tml_Send_AsyncStatusReply()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyType(TML_COMMAND_HANDLE tmlhandle, TML_INT32* type);


/**
 * @ingroup tmlheader
 * @brief Set reply type.
 *
 * It is not recommended to change the reply type. This values is used internally. Changing the reply type
 * might result in unpredictable results.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param type      reply type
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED unknown reply type<br>
 *          TML_ERR_INITIALIZATION initialization error
 *
 * @see tml_Cmd_Header_GetReplyType()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyType(TML_COMMAND_HANDLE tmlhandle, TML_INT32 type);


/**
 * @ingroup tmlheader
 * @brief Get progress.
 *
 * If a progress is processed, the progress value can be read by this function.
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param progress Reference to the command execution progress (in percent).
 *
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_INFORMATION_UNDEFINED if the information is missing.
 *
 * @see tml_Cmd_Header_SetProgress(), tml_Send_AsyncProgressReply()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetProgress(TML_COMMAND_HANDLE tmlhandle, TML_INT32* progress);


/**
 * @ingroup tmlheader
 * @brief Set progress.
 *
 * The progress is usually set by tml_Send_AsyncProgressReply(). 
 *
 * @param tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param progress  progress value (0-100)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_COMMAND_PROGRESS_RANGE progress not in range (0-100)
 *
 * @see tml_Cmd_Header_GetProgress(), tml_Send_AsyncProgressReply() 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetProgress(TML_COMMAND_HANDLE tmlhandle, TML_INT32 progress);

/**
 * @ingroup tmlheader
 * @brief Get status reply message.
 *
 * @param tmlhandle  TML command handle (TML_COMMAND_HANDLE)
 * @param msg        reference to reply message (has to be deallocated with sidex_Free_ReadString()) 
 * @param iMsgLength reference to message length
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_INFORMATION_UNDEFINED information is missing
 *
 * @see tml_Send_AsyncStatusReply(), tml_Cmd_Header_SetReplyMessage(), sidex_Free_ReadString()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyMessage(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR** msg, TML_INT32* iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyMessage_X(TML_COMMAND_HANDLE tmlhandle, wchar_t** msg, TML_INT32* iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyMessage_W(TML_COMMAND_HANDLE tmlhandle, char16_t** msg, TML_INT32* iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyMessage_A(TML_COMMAND_HANDLE tmlhandle, char** msg, TML_INT32* iMsgLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Header_GetReplyMessage  tml_Cmd_Header_GetReplyMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Header_GetReplyMessage  tml_Cmd_Header_GetReplyMessage_W
    #else
      #define tml_Cmd_Header_GetReplyMessage  tml_Cmd_Header_GetReplyMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tmlheader
 * @brief Set status reply message.
 *
 * @param tmlhandle  TML command handle (TML_COMMAND_HANDLE)
 * @param msg        reply message
 * @param iMsgLength message length
 *
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_INITIALIZATION initialization error<br>
 *          TML_ERR_UNICODE error in unicode conversion
 *
 * @see tml_Send_AsyncStatusReply(), tml_Cmd_Header_GetReplyMessage()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyMessage(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR* msg, TML_INT32 iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyMessage_X(TML_COMMAND_HANDLE tmlhandle, wchar_t* msg, TML_INT32 iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyMessage_W(TML_COMMAND_HANDLE tmlhandle, char16_t* msg, TML_INT32 iMsgLength);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyMessage_A(TML_COMMAND_HANDLE tmlhandle, char* msg, TML_INT32 iMsgLength);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Cmd_Header_SetReplyMessage  tml_Cmd_Header_SetReplyMessage_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Cmd_Header_SetReplyMessage  tml_Cmd_Header_SetReplyMessage_W
    #else
      #define tml_Cmd_Header_SetReplyMessage  tml_Cmd_Header_SetReplyMessage_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup tmlHandle
 * @brief   Acquire command data (SIDEX_HANDLE)
 *
 * The TML command has it's own SIDEX document to transport data that has to be locked before 
 * safely accessed and/or changed (see SIDEX documentation) by a specific thread. 
 * The lock has to be removed after accessing the data
 * with tml_Cmd_Release_Sidex_Handle(). 
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @param   shandle   reference to SIDEX_HANDLE
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_CRITICAL_SECTION unsuccessful acquire
 *
 * @see tml_Cmd_Release_Sidex_Handle()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Acquire_Sidex_Handle(TML_COMMAND_HANDLE tmlhandle, SIDEX_HANDLE* shandle);


/**
 * @ingroup tmlHandle
 * @brief   Release command data (SIDEX_HANDLE)
 *
 * The command data handle has to be released by the same thread, that aquired the data for access.
 * Trying to release from another thread results in an error.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_CRITICAL_SECTION unsuccessful release
 *
 * @see tml_Cmd_Acquire_Sidex_Handle()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Release_Sidex_Handle(TML_COMMAND_HANDLE tmlhandle);


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup tmlHandle
 * @brief   Debug output.
 *
 * @note For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   tmlhandle TML command handle (TML_COMMAND_HANDLE)
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_MISSING_OBJ in case of an invalid tmlhandle.
 *
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Debug(TML_COMMAND_HANDLE tmlhandle);

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/

/**
 * @ingroup  streamIO
 * @brief    Open a receiver stream.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identification
 * @param   profile     name of the profile
 * @param   sHost       The destination host name / IP number.
 * @param   sPort       The destination Port.
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_STREAM_ALREADY_IN_USE stream already in use - can't be opened for a second time<br>
 *          TML_ERR_STREAM_OPEN_ADDRESS_MISMATCH retain open error / profile,host or port don't match withID<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_STREAM_INVALID_ADDRESS invalid destination address
 *
 * @see tml_SndStream_Open(), tml_RecStream_Close()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Open(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Open_X(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Open_W(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const char16_t* profile, const char16_t* sHost, const char16_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Open_A(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const char* profile, const char* sHost, const char* sPort);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_RecStream_Open  tml_RecStream_Open_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_RecStream_Open  tml_RecStream_Open_W
    #else
      #define tml_RecStream_Open  tml_RecStream_Open_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  streamIO
 * @brief    Close a receiver stream.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identifiation.
 * @param   bRetainOpen If this value is TML_TRUE the stream can be opened again with a call to tml_RecStream_Open().
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_Open()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Close(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_BOOL bRetainOpen);


/**
 * @ingroup  streamIO
 * @brief    Get the size of a stream.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identifiation
 * @param   rsize       reference to stream size
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_STREAM_INVALID_IDENTIFIER invalid stream ID<br>
 *          TML_ERR_STREAM_INVALID_CALLBACK callback for size calculation on sender side not registered<br>
 *          TML_ERR_STREAM_SIZE_NOT_AVAILABLE size is not available<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_SndStream_Register_GetSize()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_GetSize(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_INT64* rsize);


/**
 * @ingroup  streamIO
 * @brief    Move to stream position (seek).
 *
 * Offset origin:
 *  <TABLE>
 *   <TR><TD><B>Name / Enumeration </B></TD><TD><B>Description  </B></TD><TD><B>seekPos  </B></TD></TR>
 *   <TR><TD>soFromBeginning (0)   </TD><TD>from start of stream</TD><TD>>0</TD></TR>
 *   <TR><TD>soFromCurrent   (1)   </TD><TD>from current position</TD><TD></TD></TR>
 *   <TR><TD>soFromEnd       (2)   </TD><TD>from end of stream</TD><TD><0</TD></TR>
 *  </TABLE> 
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identifiation
 * @param   seekPos     position in stream
 * @param   origin      offset origin
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_STREAM_INVALID_IDENTIFIER invalid stream identification<br>
 *          TML_ERR_STREAM_INVALID_CALLBACK seek callback on sender side not registered<br>
 *          TML_ERR_STREAM_SEEK_NOT_OPERABLE error on seek operation<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_SndStream_Register_Seek(), tml_RecStream_GetPosition()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Seek(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_INT64 seekPos, tmlSeekOrigin origin);


/**
 * @ingroup  streamIO
 * @brief    Get stream position.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identifiation
 * @param   rposition   reference to the position
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ in invalid core handle<br>
 *          TML_ERR_STREAM_INVALID_IDENTIFIER invalid stream identification<br>
 *          TML_ERR_STREAM_INVALID_CALLBACK get postion callback on sender side not registered<br>
 *          TML_ERR_STREAM_POSITION_NOT_AVAILABLE error on position request<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_Seek()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_GetPosition(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_INT64* rposition);


/**
 * @ingroup  streamIO
 * @brief    Write data to a stream.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identifiation
 * @param   buffer      reference to buffer
 * @param   count       buffer size
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_STREAM_INVALID_CALLBACK stream write handler not registered at sender side<br>
 *          TML_ERR_MISSING_OBJ invalid core handler<br>
 *          TML_ERR_STREAM_WRITE_FAILED error writing to stream<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_Read(), tml_SndStream_Register_Write()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Write(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count);


/**
 * @ingroup  streamIO
 * @brief    Read data from a stream.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identifiation
 * @param   buffer      reference to buffer
 * @param   count       byte count to read
 * @param   bytesRead   reference to read byte count (can be smaller than count if stream ends)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_STREAM_INVALID_CALLBACK stream read handler not registered at sender side<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_STREAM_READ_FAILED error reading from stream<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_Write(), tml_RecStream_ReadBuffer(), tml_SndStream_Register_Read(), tml_RecStream_DownloadData()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Read(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count, TML_INT32* bytesRead);


/**
 * @ingroup  streamIO
 * @brief    Read data of a stream.
 *
 * This function does not return the number of bytes read like tml_RecStream_Read(). If the 
 * stream ends before all bytes are read, an error will be returned.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identifiation
 * @param   buffer      reference to buffer
 * @param   count       byte count to read
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_STREAM_INVALID_CALLBACK stream read handler not registered at sender side<br>
 *          TML_ERR_STREAM_READ_FAILED error reading from stream<br>
 *          TML_ERR_STREAM_READBUFFER_EOF end of stream before reading count bytes<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID 
 *
 * @see tml_RecStream_Read(), tml_SndStream_Register_Read()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_ReadBuffer(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count);


/**
 * @ingroup  streamIO
 * @brief    Start a full download of a stream.
 *
 * To download a full stream two handler functions are used. One to handle the data buffer by buffer and
 * one to finish the download. Data can be passed for both handlers referring to object instances or records
 * used by the download. Depending on buffer and stream size the handlers are automatically called to
 * handle the download. A call to this function returns after the download is finished or an error occurred.
 *
 * @param   coreHandle        TML core handle (TML_CORE_HANDLE)
 * @param   iID               clear stream identifiation
 * @param   buffersize        size of download buffer
 * @param   pCBFuncDld        buffer reception handler
 * @param   pCBDataDld        buffer reception handler data
 * @param   pCBFuncDldFinish  download finished handler
 * @param   pCBDataDldFinish  download finished handler data
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_STREAM_DOWNLOAD_CANCELED download canceled<br>
 *          TML_ERR_STREAM_DOWNLOAD_FAILED error while downloading<br>
 *          TML_ERR_STREAM_DOWNLOAD_WRITE_FAILED write operation failed<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC(), TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC(), tml_RecStream_Read()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_DownloadData(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_UINT32 buffersize, TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC pCBFuncDld, TML_POINTER pCBDataDld, TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncDldFinish, TML_POINTER pCBDataDldFinish);

/**
 * @ingroup  streamIO
 * @brief    Open a sender stream.
 *
 * Open a stream and acquire a clear stream id to be used in all stream operations.
 *
 * @param   coreHandle  The TML_CORE_HANDLE to operate.
 * @param   iID         reference to a clear stream identifiation
 * @param   profile     profile name
 * @param   sHost       destination host name or IP address
 * @param   sPort       destination port number
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_STREAM_INVALID_ADDRESS invalid destination address
 *
 * @see tml_SndStream_Close()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Open(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE* iID, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Open_X(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE* iID, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Open_W(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE* iID, const char16_t* profile, const char16_t* sHost, const char16_t* sPort);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Open_A(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE* iID, const char* profile, const char* sHost, const char* sPort);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_SndStream_Open  tml_SndStream_Open_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_SndStream_Open  tml_SndStream_Open_W
    #else
      #define tml_SndStream_Open  tml_SndStream_Open_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  streamIO
 * @brief    Close a sender stream.
 *
 * @param   coreHandle  TML core handle (TML_CORE_HANDLE)
 * @param   iID         clear stream identifiation.
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_SndStream_Open(), tml_SndStream_Register_Close()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Close(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID);


/**
 * @ingroup  streamIO
 * @brief    Register get stream size handler.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iID        clear stream identifiation
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_GetSize(), TML_ON_SND_STRM_GET_SIZE_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_GetSize (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc, TML_POINTER pCBData);


/**
 * @ingroup  streamIO
 * @brief    Register seek handler.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iID        clear stream identifiation
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_Seek(), TML_ON_SND_STRM_SEEK_CB_FUNC() 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_Seek (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc, TML_POINTER pCBData);


/**
 * @ingroup  streamIO
 * @brief    Register get position handler.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iID        clear stream identifiation
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_GetPosition(), TML_ON_SND_STRM_GET_POSITION_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_GetPosition (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc, TML_POINTER pCBData);


/**
 * @ingroup  streamIO
 * @brief    Register read stream handler.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iID        clear stream identifiation
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_Read(), TML_ON_SND_STRM_READ_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_Read (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_READ_CB_FUNC pCBFunc, TML_POINTER pCBData);


/**
 * @ingroup  streamIO
 * @brief    Register write stream handler.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iID        clear stream identifiation
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_RecStream_Write(), TML_ON_SND_STRM_WRITE_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_Write (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc, TML_POINTER pCBData);


/**
 * @ingroup  streamIO
 * @brief    Register close stream handler.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iID        clear stream identifiation
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see tml_SndStream_Close(), TML_ON_SND_STRM_CLOSE_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_Close (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc, TML_POINTER pCBData);


/**
 * @ingroup  streamIO
 * @brief    Register streaming error handler.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   iID        clear stream ID
 * @param   pCBFunc    callback function or NULL (deregistration of previously registered handler)
 * @param   pCBData    custom data or TML_HANDLE_TYPE_NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED invalid iID
 *
 * @see TML_ON_SND_STRM_COM_FAULT_CB_FUNC(), tml_SndStream_Open()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_OnError(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc, TML_POINTER pCBData);


//-----------------------------------------
// SUPRESS as API description via doxygen:
//-----------------------------------------
// @cond INTERNAL

/**
 * @ingroup  tmladvanced
 * @brief  make debug logs 
 *
 * @note For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   iLogMask   Logging mask / 0xFFFFFFFF will log immediately
 * @param   sClass     First reference to log
 * @param   sMethod     Second reference to log
 * @param   sFormatLog     Third reference to log
 * @param   sLog     Fourth reference to log
 *
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_UNICODE if an error happened in the unicode string conversion.
 *
 * @see globalDefines.h, tmlStdTypes.h, tmlErrors.h
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_log(TML_INT32 iLogMask, const TML_CTSTR* sClass, const TML_CTSTR* sMethod, const TML_CTSTR* sFormatLog, const TML_CTSTR* sLog);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_log_X(TML_INT32 iLogMask, const wchar_t* sClass, const wchar_t* sMethod, const wchar_t* sFormatLog, const wchar_t* sLog);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_log_W(TML_INT32 iLogMask, const char16_t* sClass, const char16_t* sMethod, const char16_t* sFormatLog, const char16_t* sLog);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_log_A(TML_INT32 iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_log  tml_log_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_log  tml_log_W
    #else
      #define tml_log  tml_log_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION 

/**
 * @ingroup  tmladvanced
 * @brief  make debug logs 
 *
 * Note: For develop use only / we recommend not to use this API call as a standard user.
 *
 * @param   iLogMask   Logging mask / 0xFFFFFFFF will log immediately
 * @param   sClass     First reference to log
 * @param   sMethod     Second reference to log
 * @param   sFormatLog     Third reference to log
 * @param   iVal     Fourth reference to log
 *
 * @returns TML_SUCCESS in case of success.<br>
 *          TML_ERR_UNICODE if an error happened in the unicode string conversion.
 *
 * @see globalDefines.h, tmlStdTypes.h, tmlErrors.h
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_logI(TML_INT32 iLogMask, const TML_CTSTR* sClass, const TML_CTSTR* sMethod, const TML_CTSTR* sFormatLog, const TML_INT64 iVal);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_logI_X(TML_INT32 iLogMask, const wchar_t* sClass, const wchar_t* sMethod, const wchar_t* sFormatLog, const TML_INT64 iVal);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_logI_W(TML_INT32 iLogMask, const char16_t* sClass, const char16_t* sMethod, const char16_t* sFormatLog, const TML_INT64 iVal);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_logI_A(TML_INT32 iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const TML_INT64 iVal);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_logI  tml_logI_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_logI  tml_logI_W
    #else
      #define tml_logI  tml_logI_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION 

//-----------------------------------------
// end of SUPRESS as API description via doxygen.
//-----------------------------------------
/**
// @endcond
**/


/**
 * @ingroup  multiListener
 * @brief    Create a new listener.
 *
 * @param   coreHandle       TML core handle (TML_CORE_HANDLE)
 * @param   sAddress         network address for listener binding.<br><br>
 *                           <host>:<port><br>
 *                           <IPv4Addr>:<port><br>
 *                           \[<IPv6Addr>\]:<port><br><br>
 *                           A network address port number 0 results into a random calculated free port number during tml_Listener_Set_Enabled() execution.
 * @param   listenerHandle   reference to a new TML listener handle (TML_LISTENER_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_LISTENER_ALREADY_EXISTS a listener for the requested network address already exists<br> 
 *          TML_ERR_MISSING_OBJ invalid core handle
 * @see tml_Listener_Set_Enabled(), tml_Listener_Get_Address()
*/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Listener_Create(TML_CORE_HANDLE coreHandle, const TML_CTSTR* sAddress, TML_LISTENER_HANDLE* listenerHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Listener_Create_X(TML_CORE_HANDLE coreHandle, const wchar_t* sAddress, TML_LISTENER_HANDLE* listenerHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Listener_Create_W(TML_CORE_HANDLE coreHandle, const char16_t* sAddress, TML_LISTENER_HANDLE* listenerHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Listener_Create_A(TML_CORE_HANDLE coreHandle, const char* sAddress, TML_LISTENER_HANDLE* listenerHandle);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Listener_Create  tml_Core_Listener_Create_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Listener_Create  tml_Core_Listener_Create_W
    #else
      #define tml_Core_Listener_Create  tml_Core_Listener_Create_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  multiListener
 * @brief    Close a listener and release resources.
 *
 * @param   listenerHandle reference to TML listener handle (TML_LISTENER_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Listener_Close(TML_LISTENER_HANDLE* listenerHandle);


/**
 * @ingroup  multiListener
 * @brief    Returns the listener's network binding address.
 *
 * @param   listenerHandle   TML listener handle (TML_LISTENER_HANDLE)
 * @param   sAddress         borrowed reference to network binding address
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_NET_BINDING network binding syntax error<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Listener_Get_Address(TML_LISTENER_HANDLE listenerHandle, TML_CTSTR** sAddress);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Listener_Get_Address_X(TML_LISTENER_HANDLE listenerHandle, wchar_t** sAddress);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Listener_Get_Address_W(TML_LISTENER_HANDLE listenerHandle, char16_t** sAddress);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Listener_Get_Address_A(TML_LISTENER_HANDLE listenerHandle, char** sAddress);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Listener_Get_Address  tml_Listener_Get_Address_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Listener_Get_Address  tml_Listener_Get_Address_W
    #else
      #define tml_Listener_Get_Address  tml_Listener_Get_Address_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  multiListener
 * @brief    Get the number of listeners.
 *
 * @param   coreHandle TML core handle
 * @param   iCount     reference to the number of listeners
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerCount(TML_CORE_HANDLE coreHandle, TML_UINT32* iCount);


/**
 * @ingroup  multiListener
 * @brief    Get listener's handle from a TML core.
 *
 * @param   coreHandle TML core handle
 * @param   index index of listener
 * @param   listenerHandle reference to TML listener handle (TML_LISTENER_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED  a listener for the requested index don't exist
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_Listener(TML_CORE_HANDLE coreHandle, TML_UINT32 index, TML_LISTENER_HANDLE* listenerHandle);


/**
 * @ingroup  multiListener
 * @brief    Get listener's handle from a TML core.
 *
 * @param   coreHandle TML core handle
 * @param   sAddress network binding address
 * @param   listenerHandle reference to TML listener handle (TML_LISTENER_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_INFORMATION_UNDEFINED  a listener for the requested network binding address don't exist
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerByAddress(TML_CORE_HANDLE coreHandle, TML_CTSTR* sAddress, TML_LISTENER_HANDLE* listenerHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerByAddress_X(TML_CORE_HANDLE coreHandle, wchar_t* sAddress, TML_LISTENER_HANDLE* listenerHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerByAddress_W(TML_CORE_HANDLE coreHandle, char16_t* sAddress, TML_LISTENER_HANDLE* listenerHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerByAddress_A(TML_CORE_HANDLE coreHandle, char* sAddress, TML_LISTENER_HANDLE* listenerHandle);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Get_ListenerByAddress  tml_Core_Get_ListenerByAddress_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Get_ListenerByAddress  tml_Core_Get_ListenerByAddress_W
    #else
      #define tml_Core_Get_ListenerByAddress  tml_Core_Get_ListenerByAddress_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  multiListener
 * @brief    Enable/disable a listener.
 *
 * To enable / disable all listeners, use tml_Core_Set_ListenerEnabled().
 *
 * @param   listenerHandle TML listener handle
 * @param   bEnable        TML_TRUE to enable, TML_FALSE to disable the listener
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Core_Set_ListenerEnabled()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Listener_Set_Enabled(TML_LISTENER_HANDLE listenerHandle, TML_BOOL bEnable);


/**
 * @ingroup  multiListener
 * @brief    Get enable status of a listener.
 *
 * @param   listenerHandle TML listener handle
 * @param   bEnable        reference to listener enable status
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid core handle
 *
 * @see tml_Listener_Set_Enabled()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Listener_Get_Enabled(TML_LISTENER_HANDLE listenerHandle, TML_BOOL* bEnable);


/**
 * @ingroup  connectionManagement
 * @brief    Create a new connection.
 *
 * @param   coreHandle       TML core handle (TML_CORE_HANDLE)
 * @param   sAddress         network address.<br><br>
 *                           <host>:<port><br>
 *                           <IPv4Addr>:<port><br>
 *                           \[<IPv6Addr>\]:<port><br>
 * @param   connectionHandle reference to a new TML connection handle (TML_CONNECTION_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED  error initializing sender<br> 
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_NET_BINDING network binding syntax error<br>
 *          TML_ERR_MISSING_OBJ invalid handle<br>
 *          TML_ERR_CONNECTION_ALREADY_EXISTS a connection for the requested network address already exists
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Connect(TML_CORE_HANDLE coreHandle, const TML_CTSTR* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Connect_X(TML_CORE_HANDLE coreHandle, const wchar_t* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Connect_W(TML_CORE_HANDLE coreHandle, const char16_t* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Connect_A(TML_CORE_HANDLE coreHandle, const char* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Connect  tml_Core_Connect_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Connect  tml_Core_Connect_W
    #else
      #define tml_Core_Connect  tml_Core_Connect_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  connectionManagement
 * @brief    Close a connection and release resources.
 *
 * @param   connectionHandle reference to a TML connection handle (TML_CONNECTION_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Close(TML_CONNECTION_HANDLE* connectionHandle);


/**
 * @ingroup  connectionManagement
 * @brief    Returns the network address of remote peer.
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   sAddress         borrowed reference to network address
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_NET_BINDING network binding syntax error<br>
 *          TML_ERR_MISSING_OBJ invalid handle<br>
 *          TML_ERR_INFORMATION_UNDEFINED  information is missing
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Get_Address(TML_CONNECTION_HANDLE connectionHandle, TML_CTSTR** sAddress);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Get_Address_X(TML_CONNECTION_HANDLE connectionHandle, wchar_t** sAddress);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Get_Address_W(TML_CONNECTION_HANDLE connectionHandle, char16_t** sAddress);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Get_Address_A(TML_CONNECTION_HANDLE connectionHandle, char** sAddress);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_Get_Address  tml_Connection_Get_Address_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_Get_Address  tml_Connection_Get_Address_W
    #else
      #define tml_Connection_Get_Address  tml_Connection_Get_Address_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  connectionManagement
 * @brief    Returns the remote peer supported profiles.
 *
 * A SIDEX list containing SIDEX strings with profile names is returned.
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   lProfiles   reference to profile list.<br>
 *                      The list has to be released with sidex_Variant_DecRef()
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Get_RemoteProfiles(TML_CONNECTION_HANDLE connectionHandle, SIDEX_VARIANT* lProfiles);


/**
 * @ingroup  connectionManagement
 * @brief    Validate a connection.
 *
 * @param   connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param   bReconnect       TML_TRUE = try to reconnect if disconnected / TML_FALSE = don't try to reconnect
 * @param   bConnected       reference to the connection status, TML_TRUE if the connection is valid
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED  error initializing sender<br> 
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_NET_BINDING network binding syntax error<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_Validate(TML_CONNECTION_HANDLE connectionHandle, TML_BOOL bReconnect, TML_BOOL* bConnected);


/**
 * @ingroup  connectionManagement
 * @brief    Returns the number of connections.
 *
 * @param   coreHandle TML core handle
 * @param   iCount     reference to the number of connections
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED  information is missing<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ConnectionCount(TML_CORE_HANDLE coreHandle, TML_UINT32* iCount);


/**
 * @ingroup  connectionManagement
 * @brief    Get connection handle from a TML core.
 *
 * @param   coreHandle TML core handle
 * @param   index index of connection
 * @param   connectionHandle reference to the TML connection handle (TML_CONNECTION_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED a connection for the requested index don't exist<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_Connection(TML_CORE_HANDLE coreHandle, TML_UINT32 index, TML_CONNECTION_HANDLE* connectionHandle);


/**
 * @ingroup  connectionManagement
 * @brief    Get connection handle from a TML core.
 *
 * @param   coreHandle TML core handle
 * @param   sAddress network address
 * @param   connectionHandle reference to the TML connection handle (TML_CONNECTION_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_INFORMATION_UNDEFINED  a connection for the requested network address don't exist<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ConnectionByAddress(TML_CORE_HANDLE coreHandle, TML_CTSTR* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ConnectionByAddress_X(TML_CORE_HANDLE coreHandle, wchar_t* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ConnectionByAddress_W(TML_CORE_HANDLE coreHandle, char16_t* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ConnectionByAddress_A(TML_CORE_HANDLE coreHandle, char* sAddress, TML_CONNECTION_HANDLE* connectionHandle);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Core_Get_ConnectionByAddress  tml_Core_Get_ConnectionByAddress_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Core_Get_ConnectionByAddress  tml_Core_Get_ConnectionByAddress_W
    #else
      #define tml_Core_Get_ConnectionByAddress  tml_Core_Get_ConnectionByAddress_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  connectionManagement
 * @brief    Send async command on existing connection.
 *
 * The call returns after sending the message without waiting for a reply. 
 * The result of the command execution and possible error needs to be handled in a callback function registered with tml_Cmd_Register_CommandReady().
 * In the callback function you also have to free the TML command handle using tml_Cmd_Free(). 
 *
 * @param  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param  sProfile         profile identification string
 * @param  tmlhandle        TML command handle
 * @param  iTimeout         timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_SENDER_PROFILE_NOT_SUPPORTED profile not supported by receiver<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_SendSync()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendAsync(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendAsync_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendAsync_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendAsync_A(TML_CONNECTION_HANDLE connectionHandle, const char* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_SendAsync  tml_Connection_SendAsync_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_SendAsync  tml_Connection_SendAsync_W
    #else
      #define tml_Connection_SendAsync  tml_Connection_SendAsync_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  connectionManagement
 * @brief    Send sync command on existing connection.
 *
 * Sending a message synchronously means that the call returns after the result of the message call
 * was received or an error occurred.
 *
 * @param  connectionHandle TML connection handle (TML_CONNECTION_HANDLE)
 * @param  sProfile         profile identification string
 * @param  tmlhandle        TML command handle
 * @param  iTimeout         timeout in milliseconds
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_UNICODE error in unicode conversion<br>
 *          TML_ERR_SENDER_NOT_INITIALIZED error initializing sender<br>
 *          TML_ERR_SYSTEMRESOURCES system resource error<br>
 *          TML_ERR_SENDER_PROFILE_REGISTRATION profile not registered<br>
 *          TML_ERR_SENDER_INVALID_PARAMS invalid network address<br>
 *          TML_ERR_CHANNEL_NOT_INITIALIZED sender channel not initialized<br>
 *          ERR_DUMPCONTENT internal data format error<br>
 *          TML_ERR_SENDER_COMMUNICATION communication error<br>
 *          TML_ERR_TIMEOUT timeout before receiving reply<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see tml_Connection_SendAsync()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendSync(TML_CONNECTION_HANDLE connectionHandle, const TML_CTSTR* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendSync_X(TML_CONNECTION_HANDLE connectionHandle, const wchar_t* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendSync_W(TML_CONNECTION_HANDLE connectionHandle, const char16_t* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Connection_SendSync_A(TML_CONNECTION_HANDLE connectionHandle, const char* sProfile, TML_COMMAND_HANDLE tmlhandle, TML_UINT32 iTimeout);
#if !defined (DOXYGEN_GENERATION)
  #ifdef TML_UNICODE
    #define tml_Connection_SendSync  tml_Connection_SendSync_X
  #else
    #ifdef TML_UNICODE_16
      #define tml_Connection_SendSync  tml_Connection_SendSync_W
    #else
      #define tml_Connection_SendSync  tml_Connection_SendSync_A
    #endif // TML_UNICODE_16
  #endif // TML_UNICODE
#endif // DOXYGEN_GENERATION


/**
 * @ingroup  connectionManagement
 * @brief    Get the connection on which the command was previously sent/received.
 *
 * @param   cmdHandle TML command handle (TML_COMMAND_HANDLE)
 * @param   connectionHandle reference to the TML connection handle (TML_CONNECTION_HANDLE)
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Get_Connection(TML_COMMAND_HANDLE cmdHandle, TML_CONNECTION_HANDLE* connectionHandle);


/**
 * @ingroup  connectionManagement
 * @brief    Set callback function to signal a new connection.
 *
 * Read TML_ON_CONNECT_CB_FUNC() for further reference.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   pCBFunc    callback function or NULL to remove previously registered function
 * @param   pCBData    user data or NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid handle<
 *
 * @see TML_ON_CONNECT_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_OnConnect(TML_CORE_HANDLE coreHandle, TML_ON_CONNECT_CB_FUNC pCBFunc, TML_POINTER pCBData);


/**
 * @ingroup  connectionManagement
 * @brief    Callback function to signal a closed connection.
 *
 * Read TML_ON_DISCONNECT_CB_FUNC() for further reference.
 *
 * @param   coreHandle TML core handle (TML_CORE_HANDLE)
 * @param   pCBFunc    callback function or NULL to remove previously registered function
 * @param   pCBData    user data or NULL
 *
 * @returns TML_SUCCESS in case of success<br>
 *          TML_ERR_MISSING_OBJ invalid handle
 *
 * @see TML_ON_DISCONNECT_CB_FUNC()
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_OnDisconnect(TML_CORE_HANDLE coreHandle, TML_ON_DISCONNECT_CB_FUNC pCBFunc, TML_POINTER pCBData);

#ifdef __cplusplus
}// extern "C"
#endif
#endif  // TMLCORE_H
