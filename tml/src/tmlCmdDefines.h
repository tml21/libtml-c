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

#ifndef TMLCMDDEFINES_H
#define TMLCMDDEFINES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
* @brief    Name of the root node for the data XML
*/
#define TML_ROOT_NODE         "TML"

/**
* @brief    Group for the stream data communication header
*/
#define TML_CMD_STREAM_HEADER_GROUP  (char*) "__STREAM_HEADER__"

/**
* @brief    Key for STREAM write data
*/
#define TML_CMD_HEADER_KEY_STREAM_WRITE_DATA (char*) "STREAM_WRITE_DATA"

/**
* @brief    Key for STREAM identification
*/
#define TML_CMD_HEADER_KEY_STREAM_ID (char*) "STREAM_ID"

/**
* @brief    Key for STREAM size
*/
#define TML_CMD_HEADER_KEY_STREAM_SIZE (char*) "STREAM_SIZE"

/**
* @brief    Key for last error of STREAM operation
*/
#define TML_CMD_HEADER_KEY_LAST_ERR (char*) "STREAM_LAST_ERR"

/**
* @brief    Key for STREAM position
*/
#define TML_CMD_HEADER_KEY_STREAM_POSITION (char*) "STREAM_POSITION"

/**
* @brief    Key for STREAM buffer to write into in the stream data communication header
*/
#define TML_CMD_HEADER_KEY_STREAM_DEST_BUFFER_ADDR (char*) "STREAM_DEST_BUFFER"

/**
* @brief    Key for STREAM buffer to read from into in the stream data communication header
*/
#define TML_CMD_HEADER_KEY_STREAM_READ_BUFFER_ADDR (char*) "STREAM_READ_BUFFER"

/**
* @brief    Key for STREAM buffer size to write into in the stream data communication header
*/
#define TML_CMD_HEADER_KEY_STREAM_BUFFER_SIZE (char*) "STREAM_BUFFER_SIZE"

/**
* @brief    Key for the STREAM whole content size
*/
#define TML_CMD_HEADER_KEY_STREAM_CONTENT_SIZE (char*) "STREAM_CONTENT_SIZE"

/**
* @brief    Receiver stream objs hash table
*/
#define TML_CMD_HEADER_KEY_STREAM_RECEIVER_HASH (char*) "STREAM_RECEIVER_HASH"
/**
* @brief    Key for critical section
*/
#define TML_CMD_HEADER_KEY_STREAM_CRITICAL_SECTION (char*) "STREAM_CRITICAL_SECTION"

/**
* @brief    Key for STREAM download callback function address
*/
#define TML_CMD_HEADER_KEY_STREAM_DLD_CALLBACK_FUNC (char*) "STREAM__DLD_CALLBACK_FUNC"

/**
* @brief    Key for STREAM download callback function data
*/
#define TML_CMD_HEADER_KEY_STREAM_DLD_CALLBACK_FUNC_DATA (char*) "STREAM__DLD_CALLBACK_FUNC_DATA"

/**
* @brief    Key for STREAM seek position value
*/
#define TML_CMD_HEADER_KEY_STREAM_SEEK_POSITION (char*) "STREAM_SEEK_POSITION"
/**
* @brief    Key for STREAM seek origin value
*/
#define TML_CMD_HEADER_KEY_STREAM_SEEK_ORIGIN (char*) "STREAM_SEEK_ORIGIN"

/**
* @brief    Key for STREAM buffer bytesRead to write into in the stream data communication header
*/
#define TML_CMD_HEADER_KEY_STREAM_BUFFER_BYTRES_READ (char*) "STREAM_BUFFER_BYTES_READ"

/**
* @brief    Key for STREAM buffer length to write into in the stream data communication header
*/
#define TML_CMD_HEADER_KEY_STREAM_BUFFER_LENGTH (char*) "STREAM_BUFFER_LENGTH"

/**
* @brief    Group for the communication header
*/
#define TML_CMD_HEADER_GROUP  (char*) "__HEADER__"

/**
* @brief    Key for timestamp entry in the communication header
*/
#define TML_CMD_HEADER_KEY_TIMESTAMP (char*) "TIMESTAMP"

/**
* @brief    Key for command entry in the communication header
*/
#define TML_CMD_HEADER_KEY_COMMAND (char*) "COMMAND"

/**
* @brief    Key for an internal command entry in the communication header
*/
#define TML_CMD_HEADER_KEY_INTERNAL_COMMAND (char*) "INTERNAL_COMMAND"

/**
* @brief    Identifier for an event
*/
#define TML_CMD_IS_EVENT (char*) "TML_CMD_IS_EVENT"


/**
* @brief    Key for error id entry in the communication header
*/
#define TML_CMD_HEADER_KEY_ERRID (char*) "ERRID"

/**
* @brief    Key for error message entry in the communication header
*/
#define TML_CMD_HEADER_KEY_ERRMSG (char*) "ERRMSG"

/**
* @brief    Key for command state entry in the communication header
*/
#define TML_CMD_HEADER_KEY_STATE (char*) "STATE"

/**
* @brief    Key for command mode entry in the communication header
*/
#define TML_CMD_HEADER_KEY_MODE (char*) "MODE"

/**
* @brief    Key for reply type entry in the communication header
*/
#define TML_CMD_HEADER_KEY_REPLYTYPE (char*) "REPLYTYPE"

/**
* @brief    Key for progress entry in the communication header
*/
#define TML_CMD_HEADER_KEY_PROGRESS (char*) "PROGRESS"

/**
* @brief    Key for reply message entry in the communication header
*/
#define TML_CMD_HEADER_KEY_REPLYMSG (char*) "REPLYMSG"

/**
* @brief    Group for the internal communication
*/
#define TML_CMD_INTERNAL_GROUP  TML_CMD_HEADER_GROUP

/**
* @brief    Key for the internal communication for profile parameter
*/
#define TML_CMD_INTERNAL_KEY_PROFILE  (char*) "INT_CMD_PROFILE"

/**
* @brief    Key for the internal communication for host parameter
*/
#define TML_CMD_INTERNAL_KEY_HOST  (char*) "INT_CMD_HOST"

/**
* @brief    Key for the internal communication for port parameter
*/
#define TML_CMD_INTERNAL_KEY_PORT  (char*) "INT_CMD_PORT"


/**
* @brief    Internal command for load balancing busy state request
*/
#define CMD_INTERNAL_LOAD_BALANCED_BUSY_STATE_REQUEST     700


/**
* @brief    Internal command for event subscription request
*/
#define CMD_INTERNAL_EVENT_SUBSCRIPTION_REQUEST           707


/**
* @brief    Internal command for event unsubscription request
*/
#define CMD_INTERNAL_EVENT_UNSUBSCRIPTION_REQUEST         708


/**
* @brief    Internal command for load balanced subscription request
*/
#define CMD_INTERNAL_LOAD_BALANCED_SUBSCRIPTION_REQUEST   709


/**
* @brief    Internal command for unsubscription request
*/
#define CMD_INTERNAL_LOAD_BALANCED_UNSUBSCRIPTION_REQUEST 710


/**
* @brief    Internal command for stream to get the size
*/
#define CMD_INTERNAL_STREAM_GET_SIZE                      711


/**
* @brief    Internal command for stream read operation
*/
#define CMD_INTERNAL_STREAM_READ                          712

/**
* @brief    Internal command for stream get last error
*/
#define CMD_INTERNAL_STREAM_GET_LAST_ERR                  713


/**
* @brief    Internal command for stream to perform seek operation
*/
#define CMD_INTERNAL_STREAM_SEEK                          714


/**
* @brief    Internal command for stream to get the position
*/
#define CMD_INTERNAL_STREAM_GET_POSITION                  715

/**
* @brief    Internal command for stream download
*/
#define CMD_INTERNAL_STREAM_DOWNLOAD                      716

/**
* @brief    Internal command for cancel of stream download
*/
#define CMD_INTERNAL_STREAM_CANCEL_DOWNLOAD               717

/**
* @brief    Internal command for close information from receiver to sender stream
*/
#define CMD_INTERNAL_STREAM_CLOSE                         718

/**
* @brief    Internal command for stream write operation
*/
#define CMD_INTERNAL_STREAM_WRITE                         719




#endif // TMLCMDDEFINES_H

