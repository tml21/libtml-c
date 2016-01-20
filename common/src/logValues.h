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

#ifndef LOGVALUES_H
#define LOGVALUES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////
// The logging masks:
/////////////////////////////////////////////////
/**
* @brief    no debug log
* @ingroup tmlConstants
*/
#define TML_LOG_OFF                       0x0

/**
* @brief    special develop log
* @ingroup tmlConstants
*/
#define TML_LOG_DEVELOP                   0x1

/**
* @brief    log API commands to vortex library
* @ingroup tmlConstants
*/
#define TML_LOG_VORTEX_CMD                0x4

/**
* @brief    core I/O logging
* @ingroup tmlConstants
*/
#define TML_LOG_CORE_IO                   0x8

/**
* @brief    log core API
* @ingroup tmlConstants
*/
#define TML_LOG_CORE_API                  0x10

/**
* @brief    Vortex frame logging
* @ingroup tmlConstants
*/
#define TML_LOG_VORTEX_FRAMES             0x20

/**
* @brief    Vortex channel pool logging
* @ingroup tmlConstants
*/
#define TML_LOG_VORTEX_CH_POOL            0x40


/**
* @brief    Vortex mutex logging
* @ingroup tmlConstants
*/
#define TML_LOG_VORTEX_MUTEX              0x80


/**
* @brief    Vortex multi sync commands
* @ingroup tmlConstants
*/
#define TML_LOG_MULTI_SYNC_CMDS           0x100

/**
* @brief    internal command dispatcher log
* @ingroup tmlConstants
*/
#define TML_LOG_INTERNAL_DISPATCH         0x800

/**
* @brief    stream handling log
* @ingroup tmlConstants
*/
#define TML_LOG_STREAM_HANDLING           0x2000

/**
* @brief    event logging
* @ingroup tmlConstants
*/
#define TML_LOG_EVENT                     0x8000

#endif  // LOGVALUES_H

