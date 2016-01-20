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
 
/** @file tmlGlobalDefines.h
  * @brief TML communication and log definitions
  */

#ifndef TMLGLOBALDEFINES_H
#define TMLGLOBALDEFINES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef LINUX
typedef void *HANDLE;
typedef unsigned long DWORD;
typedef void *LPVOID;

#define INFINITE            0xFFFFFFFF  // Infinite timeout

#endif

/////////////////////////////////////////////////
// The command reply types:
/////////////////////////////////////////////////
/**
* @brief    undefined reply type
* @ingroup tmlConstants
*/
#define TMLCOM_RPY_UNDEFINED           -1

/**
* @brief    progress reply
* @ingroup tmlConstants
*/
#define TMLCOM_RPY_PROGRESS             0

/**
* @brief    warning message reply
* @ingroup tmlConstants
*/
#define TMLCOM_RPY_WARNING              10

/**
* @brief    error message reply
* @ingroup tmlConstants
*/
#define TMLCOM_RPY_ERROR                20

/**
* @brief    information message reply
* @ingroup tmlConstants
*/
#define TMLCOM_RPY_INFORMATION          30


/////////////////////////////////////////////////
// The command modes:
/////////////////////////////////////////////////
/**
* @brief    command mode undefined
* @ingroup tmlConstants
*/
#define TMLCOM_MODE_UNDEFINED            -1

/**
* @brief    asynchronous command
* @ingroup tmlConstants
*/
#define TMLCOM_MODE_ASYNC                 0

/**
* @brief    synchronous command
* @ingroup tmlConstants
*/
#define TMLCOM_MODE_SYNC                  1
/**
* @brief    command is event (no reply)
* @ingroup tmlConstants
*/
#define TMLCOM_MODE_EVT                   2


/////////////////////////////////////////////////
// The command states:
/////////////////////////////////////////////////
/**
* @brief    command state undefined
* @ingroup tmlConstants
*/
#define TMLCOM_CSTATE_UNDEFINED        -1

/**
* @brief    initial command state
* @ingroup tmlConstants
*/
#define TMLCOM_CSTATE_CREATED           0

/**
* @brief    command successfully executed
* @ingroup tmlConstants
*/
#define TMLCOM_CSTATE_EXECUTED          1

/**
* @brief    command execution failed
* @ingroup tmlConstants
*/
#define TMLCOM_CSTATE_FAILED            2

/**
* @brief    command execution is pending
* @ingroup tmlConstants
*/
#define TMLCOM_CSTATE_PENDING           3

#endif // TMLGLOBALDEFINES_H
