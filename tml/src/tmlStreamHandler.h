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

#ifndef TMLSTREAMHANDLER_H
#define TMLSTREAMHANDLER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlUniversalHashTable.h"
#include "tmlCollectCall.h"
#include "tmlLogHandler.h"
#include "tmlCriticalSectionObj.h"

/**
 * @brief    Communication structure in case of using the Vortex payload feeder:
 */
struct VortexPayloadStreamFeederData{
  TML_ON_SND_STRM_GET_SIZE_CB_FUNC m_pCBFuncGetSize;
  TML_POINTER m_pCBDataGetSize;
  TML_ON_SND_STRM_READ_CB_FUNC m_pCBFuncRead;
  TML_POINTER m_pCBDataRead;
  TML_ON_SND_STRM_SEEK_CB_FUNC m_pCBFuncSeek;
  TML_POINTER m_pCBDataSeek;
  TML_ON_SND_STRM_CLOSE_CB_FUNC m_pCBFuncClose;
  TML_POINTER m_pCBDataClose;
  TML_ON_SND_STRM_COM_FAULT_CB_FUNC m_pCBFuncOnError;
  TML_POINTER m_pCBDataOnError;
  TML_INT64 iStreamSize;
  TML_INT64 iTotalBytesRead;
  TML_STREAM_ID_TYPE iStreamID;
  TMLUniversalHashTable* sndStreamObjects;
  bool bReadFailed;
  tmlCriticalSectionObj* criticalSection;
  tmlLogHandler* log;
};


class  tmlStreamHandler
{

private:
	  /* data */

    /**
     * @brief    Hash table for the initialized sender stream objects:
     */
    TMLUniversalHashTable* m_sndStreamObjects;

    /**
     * @brief    Hash table for the initialized receiver stream objects:
     */
    TMLUniversalHashTable* m_recStreamObjects;

    /**
     * @brief    log handler
     */
    tmlLogHandler* m_log;

    tmlCriticalSectionObj* m_CriticalSectionReceiver;
    tmlCriticalSectionObj* m_CriticalSectionSender;

	  /* methods */

public:
	  /* data */

	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlStreamHandler
     */
    explicit tmlStreamHandler(tmlLogHandler* tmlLogHandler);


    /**
     * @brief    Destructor.
     */
    ~tmlStreamHandler();


    /**
     * @brief    Open a receiver stream.
     *
     * @param   coreHandle  The TML_CORE_HANDLE to operate.
     * @param   iID         Clear stream identifiation / returned by tml_SndStream_Open().
     * @param   sPort       The destination Port.
     * @param   sHost       The destination host name / IP number.
     * @param   profile     URI of the destination profile for the requested operation.
     * @param   sender      The sender for all command operations.
     * @param   iWindowSize The window size for command operations.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_ALREADY_IN_USE The stream is already in use - can't be opened for a second time.<br>
     *          TML_ERR_STREAM_OPEN_ADDRESS_MISMATCH Retain open problem / profile & host & port don't match with the ID.<br>
     *          TML_ERR_STREAM_INVALID_ADDRESS if one of the destination parameter is invalid.
     *
     * @see tmlErrors.h
     */
    int recStream_Open(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const char* sPort, const char* sHost, const char* profile, tmlCollectCall* sender, int iWindowSize);


    /**
     * @brief    Close a receiver stream.
     *
     * @param   iID         Clear stream identifiation.
     * @param   bRetainOpen Flag. If TML_TRUE the  receiver stream will be retained open / the close is only logically(Stream still stand open). A recStream_Open is allowed afterwards.
     * @param   bSendCancel Flag. If TML_TRUE the  receiver stream sends internal cancel message.
     *
     * @returns TML_SUCCESS in case of success<br>.
     *          TML_ERR_INFORMATION_UNDEFINED in case of an invalid iID / maybe of a missing tml_RecStream_Open().
     *
     */
    int recStream_Close(TML_STREAM_ID_TYPE iID, TML_BOOL bRetainOpen, bool bSendCancel);


    /**
     * @brief    Get the size of a receiver stream.
     *
     * @param   iID       Clear stream identifiation.
     * @param   rsize     Reference to the size.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_SIZE_NOT_AVAILABLE if the callback returns a value of -1.     
     *
     * @see tmlErrors.h
     */
    int recStream_GetSize(TML_STREAM_ID_TYPE iID, TML_INT64* rsize);

    
    /**
     * @brief    Perform a seek operation of a receiver stream.
     *
     * @param   iID     Clear stream identifiation.
     * @param   seekPos The position for the requested operation.
     * @param   origin  The origin for the requested operation.<br>
     *                  soFromBeginning: Offest relative to the beginning of the resource (Must be more than 0).
     *                  soFromCurrent: Offest relative to the current position of the resource.
     *                  soFromEnd: Offest relative to the end of the resource (Must be less than 0).
     *
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_SEEK_NOT_OPERABLE if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int recStream_Seek(TML_STREAM_ID_TYPE iID, TML_INT64 seekPos, tmlSeekOrigin origin);


    /**
     * @brief    Get the position of a receiver stream.
     *
     * @param   iID       Clear stream identifiation.
     * @param   rposition Reference to the position.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_POSITION_NOT_AVAILABLE if the callback returns a value of -1.     
     *
     * @see tmlErrors.h
     */
    int recStream_GetPosition(TML_STREAM_ID_TYPE iID, TML_INT64* rposition);


    /**
     * @brief    Write data to a receiver stream.
     *
     * @param   iID         Clear stream identifiation.
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
    int recStream_Write(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count);


    /**
     * @brief    Read data of a receiver stream.
     *
     * @param   iID         Clear stream identifiation.
     * @param   buffer      Reference to the buffer to write the result into.
     * @param   count       Size of the buffer / number of bytes read.
     * @param   bytesRead   Reference to the number of bytes read.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns a value of -1.
     *
     * @see tmlErrors.h
     */
    int recStream_Read(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count, TML_INT32* bytesRead);


    /**
     * @brief    Read data of a receiver stream.
     *
     * If the end of file is reached before the count of bytes were read, an error will be returned.
     *
     * @param   iID         Clear stream identifiation.
     * @param   buffer      Reference to the buffer to write the result into.
     * @param   count       Size of the buffer / number of bytes read.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns a value of -1.<br>
     *          TML_ERR_STREAM_READBUFFER_EOF End of file is reached before the count of bytes were read.
     *
     * @see tmlErrors.h
     */
    int recStream_ReadBuffer(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count);


    /**
     * @brief    Initiates a download of data of a receiver stream.
     *
     * @param   iID               Clear stream identifiation.
     * @param   buffersize        buffersize for the download.
     * @param   pCBFuncDld        Reference to the callback method that handles the download.
     * @param   pCBDataDld        Reference to parameter of the callback method that handles the download.<br>
     * @param   pCBFuncDldFinish  Reference to the callback method that will be called at the end of the download.<br>
     * @param   pCBDataDldFinish  Reference to parameter of the callback that will be called at the end of the download.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_DOWNLOAD_FAILED if the download failed.
     *
     * @see tmlErrors.h
     */
    int recStream_DownloadData(TML_STREAM_ID_TYPE iID, TML_UINT32 buffersize, TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC pCBFuncDld, TML_POINTER pCBDataDld, TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncDldFinish, TML_POINTER pCBDataDldFinish);


    /**
     * @brief    Open a sender stream.
     *
     * @param   iID         Reference to a clear stream identifiation.
     * @param   sPort       The destination Port.
     * @param   sHost       The destination host name / IP number.
     * @param   profile     URI of the destination profile for the requested operation.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_ADDRESS if one of the destination parameter is invalid.
     *
     * @see tmlErrors.h
     */
     int sndStream_Open(TML_STREAM_ID_TYPE* iID, const char* sPort, const char* sHost, const char* profile);


    /**
     * @brief    Close a sender stream.
     *
     * @param   iID         Clear stream identifiation.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int sndStream_Close(TML_STREAM_ID_TYPE iID);


    /**
     * @brief    Register a callback method to get the sender stream size.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int sndStream_Register_GetSize (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to perform stream seek operations.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int sndStream_Register_Seek (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to get the stream position.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int sndStream_Register_GetPosition (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to read data of the sender stream.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int sndStream_Register_Read (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_READ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to write data to a sender stream.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc   Reference to the callback method.<br>
     *                    In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData   Reference to parameter of the callback method.<br>
     *                    In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int sndStream_Register_Write (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method that will be called in case of closing the sender stream.
     *
     * The callback will be called in the manner:<br>
     *  void pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER pCBData)<br>
     *  -Parameter iID        Clear stream identifiation.<br>
     *  -Parameter pCBData    Reference to parameter of the callback method.
     *
     * @param   iID       Clear stream identifiation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle of sHandle.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int sndStream_Register_Close (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method that will be called on communication fault during a request.
     *
     * The callback will be called in the manner:<br>
     *  void pCBFunc (TML_STREAM_ID_TYPE iID, TML_INT32 iError, TML_POINTER pCBData)<br>
     *  -Parameter iID        Clear stream identifiation.<br>
     *  -Parameter iError     The detailed error code of the communication fault.<br>
     *  -Parameter pCBData    Reference to parameter of the callback method.
     *
     * @param   iID        Clear stream identifiation.
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_MISSING_OBJ in case of an invalid coreHandle of sHandle.
     *
     * @see tmlStdTypes.h, tmlErrors.h
     */
    int sndStream_Register_OnError (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc, TML_POINTER pCBData);

    /**
     * @brief    Handle receiver stream close to the sender stream
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *
     * @see tmlErrors.h
     */
    int handle_close (SIDEX_HANDLE sHandle);


    /**
     * @brief    Handle a size request
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     * @param   iSize     Reference to the handle's size.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_SIZE_NOT_AVAILABLE if the callback returns a value of -1.
     *
     * @see tmlErrors.h
     */
    int handle_getSize (SIDEX_HANDLE sHandle, TML_INT64* iSize);


    /**
     * @brief    Handle a size seek operation
     *
     * @param   sHandle      The SIDEX_HANDLE for the return value.
     * @param   seekPosition The position for the requested operation.
     * @param   seekOrigin   The origin for the requested operation.<br>
     *                       soFromBeginning: Offest relative to the beginning of the resource (Must be more than 0).
     *                       soFromCurrent: Offest relative to the current position of the resource.
     *                       soFromEnd: Offest relative to the end of the resource (Must be less than 0).
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_SEEK_NOT_OPERABLE if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int handle_seek (SIDEX_HANDLE sHandle, TML_INT64 seekPosition, TML_INT64 seekOrigin);


    /**
     * @brief    Handle download via Vortex payload feeder
     *
     * @param   ctx          Vortex execution context
     * @param   iStreamID    Sender stream identification
     * @param   feeder       Rederence to reference of a Vortex payload feeder.
     * @param   data         Reference to internal communication structure for the Vortex payload feeder communication.
     *
     * @returns TML_SUCCESS in case of success, otherwise the error code.
     *
     * @see tmlErrors.h
     */
    int handle_VortexFeederDownload (VortexCtx* ctx, TML_STREAM_ID_TYPE iStreamID, VortexPayloadFeeder** feeder, VortexPayloadStreamFeederData* data);


    /**
     * @brief    Handle a position request
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_POSITION_NOT_AVAILABLE if the callback returns a value of -1.
     *
     * @see tmlErrors.h
     */
    int handle_getPosition (SIDEX_HANDLE sHandle);


    /**
     * @brief    Handle the set last err for raw data operations
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     * @param   iErr      The error value to set.
     *
     */
    void handle_setLastErr (SIDEX_HANDLE sHandle, int iErr);


    /**
     * @brief    Handle the get last errror request
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.
     *
     * @see tmlErrors.h
     */
    int handle_getLastErr (SIDEX_HANDLE sHandle);


    /**
     * @brief    Cancel a pending stream download
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.
     *
     * @see tmlErrors.h
     */
    int handle_cancelStreamDownload (SIDEX_HANDLE sHandle);


    /**
     * @brief    Handle a read preparation / reset the cancel flag
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     * @param   iStreamID The stream identification.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.
     *
     * @see tmlErrors.h
     */
    int handle_readPreparation (SIDEX_HANDLE sHandle, TML_STREAM_ID_TYPE* iStreamID);


    /**
     * @brief    Handle a communication fault
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     * @param   iError    The error code.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.
     *
     * @see tmlErrors.h
     */
    int handle_OnError (SIDEX_HANDLE sHandle, TML_INT32 iError);



    /**
     * @brief    Handle a read request
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     * @param   bytesRead Reference to the number of bytes read.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int handle_read (SIDEX_HANDLE sHandle, TML_INT32* bytesRead);


    /**
     * @brief    Handle a write operation
     *
     * @param   sHandle   The SIDEX_HANDLE for the return value.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_IDENTIFIER The stream identification is invalid.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.<br>
     *          TML_ERR_STREAM_READ_FAILED if the callback returns an error.
     *
     * @see tmlErrors.h
     */
    int handle_write (SIDEX_HANDLE sHandle);

    /**
     * @brief    Free allocated memory
     *
     * @param   sHandle   The SIDEX_HANDLE containing the address to free.
     *
     */
    void handle_Free (SIDEX_HANDLE sHandle);
};


#endif //TMLSTREAMHANDLER_H

