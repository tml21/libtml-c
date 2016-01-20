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

#ifndef TMLRECEIVERSTREAM_H
#define TMLRECEIVERSTREAM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlCollectCall.h"
#include "tmlLogHandler.h"
#include <vortex.h>

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains params to call the getLastErrorIntern out of the callback
struct comParamStruct{
  char* sProfile;
  char* sHost;
  char* sPort;
  char* sKey;
  int iWindowSize;
  TML_STREAM_ID_TYPE iStreamID;
  tmlCollectCall* sender;
  TML_INT32* iLastErr;
  tmlLogHandler* log;
  TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncRet;
  TML_POINTER pCBDataRet;
  TML_CORE_HANDLE coreHandle;
};

/**
* @brief  Enum for all data types
*/
enum OPEN_STATE
{
   CLOSED,
   OPENED,
   RETAIN_OPENED
};



class  tmlReceiverStream
{

private:
	  /* data */
    char* m_sProfile;
    char* m_sHost;
    char* m_sPort;
    char* m_sKey;
    OPEN_STATE  m_bOpenState;
    TML_CORE_HANDLE m_coreHandle;

    TML_INT32 m_iLastErr;

    TML_STREAM_ID_TYPE m_iStreamID;

    tmlCollectCall* m_sender;

    int m_iWindowSize;

    tmlLogHandler* m_log;

    comParamStruct* m_cp;

	  /* methods */
    /**
     * @brief    Helper method resets all parameter.
     */
    void reset(bool bSendCancel);

public:
	  /* data */
	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @param   coreHandle  The TML_CORE_HANDLE to operate.
     * @param   sender      The sender for all command operations.
     * @param   iWindowSize The window size for command operations.
     * @param   loghandler  Handler to handle logging output.
     *
     * @returns an instance of tmlReceiverStream
     */
    tmlReceiverStream(TML_CORE_HANDLE coreHandle, tmlCollectCall* sender, int iWindowSize, tmlLogHandler* loghandler);


    /**
     * @brief    Destructor.
     */
    ~tmlReceiverStream();


    /**
     * @brief    Returns the registered profile for this stram
     */
    char* getProfile();


    /**
     * @brief    Returns the registered host for this stram
     */
    char* getHost();


    /**
     * @brief    Returns the registered port for this stram
     */
    char* getPort();


    /**
     * @brief    Open a receiver stream.
     *
     * @param   iStreamID   Clear identifiation of the opened stream of the sender.
     * @param   sPort       The destination Port.
     * @param   sHost       The destination host name / IP number.
     * @param   profile     URI of the destination profile for the requested operation.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_OPEN_ADDRESS_MISMATCH Retain open problem / profile & host & port don't match with the ID.<br>
     *          TML_ERR_STREAM_INVALID_ADDRESS if one of the destination parameter is invalid.
     *
     * @see tmlErrors.h
     */
    int open(TML_STREAM_ID_TYPE iStreamID, const char* sPort, const char* sHost, const char* profile);


    /**
     * @brief    Close a receiver stream.
     *
     * @param   bRetainOpen Flag. If TML_TRUE the  receiver stream will be retained open / the close is only logically(Stream still stand open). A recStream_Open is allowed afterwards.
     * @param   bSendCancel Flag. If TML_TRUE the  receiver stream sends internal cancel message.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int close(TML_BOOL bRetainOpen, bool bSendCancel);


    /**
     * @brief    Helper method to get the key for hash computing.
     *
     * @param   sKey     reference to char array for the key.
     */
    void getKey(char** sKey);


    /**
     * @brief    Helper method to get the streamID for hash computing.
     *
     * @param   iID     reference to the id.
     */
    void getID(TML_STREAM_ID_TYPE* iID);


    /**
     * @brief    Get the size of a receiver stream.
     *
     * @param   rsize     Reference to the size.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_SIZE_NOT_AVAILABLE if the callback returns a value of -1.
     *
     * @see tmlErrors.h
     */
    int getSize(TML_INT64* rsize);


    /**
     * @brief    Perform a seek operation of a receiver stream.
     *
     * @param   seekPos The position for the requested operation.
     * @param   origin  The origin for the requested operation.<br>
     *                  soFromBeginning: Offest relative to the beginning of the resource (Must be more than 0).
     *                  soFromCurrent: Offest relative to the current position of the resource.
     *                  soFromEnd: Offest relative to the end of the resource (Must be less than 0).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_SEEK_NOT_OPERABLE if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int seek (TML_INT64 seekPos, tmlSeekOrigin origin);


    /**
     * @brief    Get the position of a receiver stream.
     *
     * @param   rposition     Reference to the position.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_POSITION_NOT_AVAILABLE if the callback returns a value of -1.
     *
     * @see tmlErrors.h
     */
    int getPosition (TML_INT64* rposition);


    /**
     * @brief    Call internal for the last error.
     *
     * @param   iErr        Reference to the error code.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int getLastErrorIntern(TML_INT32* iErr);


    /**
     * @brief    Write a portion of data to a receiver stream.
     *
     * @param   buffer      Reference to buffer for the write operation.
     * @param   count       Number of bytes to write.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.<br>
     *          TML_ERR_STREAM_WRITE_FAILED if the callback returns an error.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int writeOneBuffer(void* buffer, TML_INT32 count);


    /**
     * @brief    Write data to a receiver stream.
     *
     * @param   buffer      Reference to buffer for the write operation.
     * @param   count       Number of bytes to write.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle.<br>
     *          TML_ERR_STREAM_WRITE_FAILED if the callback returns an error.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int write(void* buffer, TML_INT32 count);


    /**
     * @brief    Read data of a receiver stream.
     *
     * @param   buffer      Reference to the buffer to write the result into.
     * @param   count       Size of the buffer / number of bytes read.
     * @param   bytesRead   Reference to the number of bytes read.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int read(void* buffer, TML_INT32 count, TML_INT32* bytesRead);


    /**
     * @brief    Read data of a receiver stream.
     *
     * If the end of file is reached before the count of bytes were read, an error will be returned.
     *
     * @param   buffer      Reference to the buffer to write the result into.
     * @param   count       Size of the buffer / number of bytes read.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns an error.<br>
     *          TML_ERR_STREAM_READBUFFER_EOF End of file is reached before the count of bytes were read.
     *
     * @see tmlErrors.h
     */
    int readBuffer (void* buffer, TML_INT32 count);

  
    /**
     * @brief    Download data of a receiver stream.
     *
     * @param   criticalSectionReceiver Handles critical section.
     * @param   recStreamObjects        Hash table with recStream objects.
     * @param   buffersize              Amount of bytes for every read block.
     * @param   pCBFuncDld              Reference to the callback method that handles the download.
     * @param   pCBDataDld              Reference to parameter of the callback method that handles the download.<br>
     * @param   pCBFuncRet              Reference to the callback method that will be called at the end of the download.<br>
     * @param   pCBDataRet              Reference to parameter of the callback that will be called at the end of the download.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int downloadData(tmlCriticalSectionObj* criticalSectionReceiver, TMLUniversalHashTable* recStreamObjects, TML_UINT32 buffersize, TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC pCBFuncDld, TML_POINTER pCBDataDld, TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncRet, TML_POINTER pCBDataRet);
};
#endif //TMLRECEIVERSTREAM_H

