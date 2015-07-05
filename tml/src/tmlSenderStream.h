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
 *  For commercial support on build UJO enabled solutions contact us:
 *  
 * Contributors:
 *    wobe-systems GmbH
 */

#ifndef TMLSENDERSTREAM_H
#define TMLSENDERSTREAM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "tmlStdTypes.h"
#include "tmlCoreHash.h"
#include <vortex.h>

class  tmlSenderStream
{

private:
	  /* data */
    md5Hash* m_hashCalc;
    char*    m_sProfile;
    char*    m_sHost;
    char*    m_sPort;
    char*    m_sKey;

    TML_ON_SND_STRM_GET_SIZE_CB_FUNC m_pCBFuncGetSize;
    TML_POINTER m_pCBDataGetSize;
    TML_ON_SND_STRM_READ_CB_FUNC m_pCBFuncRead;
    TML_POINTER m_pCBDataRead;
    TML_ON_SND_STRM_WRITE_CB_FUNC m_pCBFuncWrite;
    TML_POINTER m_pCBDataWrite;
    TML_ON_SND_STRM_SEEK_CB_FUNC m_pCBFuncSeek;
    TML_POINTER m_pCBDataSeek;
    TML_ON_SND_STRM_GET_POSITION_CB_FUNC m_pCBFuncGetPosition;
    TML_POINTER m_pCBDataGetPosition;
    TML_ON_SND_STRM_CLOSE_CB_FUNC m_pCBFuncClose;
    TML_POINTER m_pCBDataClose;
    TML_ON_SND_STRM_COM_FAULT_CB_FUNC m_pCBFuncOnError;
    TML_POINTER m_pCBDataOnError;

    TML_STREAM_ID_TYPE m_iStreamID;

    int m_iLastErr;
    bool m_bCancelDownload;

    /**
     * @brief    char array to set the date arribute
     */
    char* m_date;

    /**
     * @brief    VortexPayloadFeeder reference
     */
    VortexPayloadFeeder* m_feeder;

	  /* methods */
    /**
     * @brief    Helper method resets all parameter.
     */
    void reset();


    /**
     * @brief    Get the local time as a formatted string
     *
     * @param   sKey       Additional address key information.
      *
     * @returns reference to a time formatted string.
     */
     char* getFormattedTime(char* sKey);


public:
	  /* data */

	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlSenderStream
     */
    tmlSenderStream();


    /**
     * @brief    Destructor.
     */
    ~tmlSenderStream();

    /**
     * @brief    Open a sender stream.
     *
     * @param   iStreamID   reference to a clear identifiation.
     * @param   sPort       The destination Port.
     * @param   sHost       The destination host name / IP number.
     * @param   profile     URI of the destination profile for the requested operation.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_ADDRESS if one of the destination parameter is invalid.
     *
     * @see tmlErrors.h
     */
    int open(TML_STREAM_ID_TYPE* iStreamID, const char* sPort, const char* sHost, const char* profile);


    /**
     * @brief    Close a sender stream.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int close();


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
     * @brief    Register a callback method to perform stream seek operations.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int register_Seek (TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to get the stream position.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int register_GetPosition (TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Register a callback method to get the sender stream size.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int register_GetSize (TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Get registered callback method to perform the sender stream seek operations.
     *
     * @param   pCBFunc    Reference to reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.
     *
     * @see tmlErrors.h
     */
    int registered_Seek (TML_ON_SND_STRM_SEEK_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Get registered callback method to get the sender stream position.
     *
     * @param   pCBFunc    Reference to reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.
     *
     * @see tmlErrors.h
     */
    int registered_GetPosition (TML_ON_SND_STRM_GET_POSITION_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Get registered callback method to get the sender stream size.
     *
     * @param   pCBFunc    Reference to reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.<br>
     *          TML_ERR_STREAM_INVALID_CALLBACK if the callback is not registered.
     *
     * @see tmlErrors.h
     */
    int registered_GetSize (TML_ON_SND_STRM_GET_SIZE_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Register a callback method to read data of the sender stream.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int register_Read (TML_ON_SND_STRM_READ_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Get registered callback method for the read from the sender stream.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int registered_Read (TML_ON_SND_STRM_READ_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Register a callback method to write data to a sender stream.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int register_Write (TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Get registered callback method for the write to the sender stream.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int registered_Write (TML_ON_SND_STRM_WRITE_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Register a callback method that will be called in case of closing the sender stream.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int register_Close (TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Get registered callback method for the close of the sender stream.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int registered_Close (TML_ON_SND_STRM_CLOSE_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Register a callback method that will be called on communication fault during a request.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int register_OnError (TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc, TML_POINTER pCBData);


    /**
     * @brief    Get registered callback method that will be called on communication fault during a request.
     *
     * @param   pCBFunc    Reference to the callback method.<br>
     *                     In case of NULL a callback method will be unregistered and pCBData will be ignored.
     * @param   pCBData    Reference to parameter of the callback method.<br>
     *                     In case of NULL the callback method has no parameter.
     *
     * @returns TML_SUCCESS in case of success.
     *
     * @see tmlErrors.h
     */
    int registered_OnError (TML_ON_SND_STRM_COM_FAULT_CB_FUNC* pCBFunc, TML_POINTER* pCBData);


    /**
     * @brief    Set the feeder object
     *
     * @param   feeder     Reference to VortexPayloadFeeder.
     */
    void setFeeder (VortexPayloadFeeder* feeder);


    /**
     * @brief    Get the feeder reference object
     *
     * @param   feeder     Reference to VortexPayloadFeeder.
     */
    void getFeeder (VortexPayloadFeeder** feeder);


    /**
     * @brief    Set the last error code.
     *
     * @param   iError   The error code value.
     */
    void setLastErr (int iError);


    /**
     * @brief    Returns the last error code.
     */
    int getLastErr ();


    /**
     * @brief    Set the cancel flag for the download operation.
     */
    void setCancelDownload (bool bCancel);

    /**
     * @brief    Returns true in case of a pending cancel.
     */
    bool isCancelDownloadPending ();
};

#endif //TMLSENDERSTREAM_H

