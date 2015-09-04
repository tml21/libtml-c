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

#include "tmlSenderStream.h"
#include "tmlErrors.h"
#include "globalDefines.h"
#include "tmlStdTypes.h"
#include <ctype.h>

static TML_UINT32 g_iMagic = 0;
/**
 * @brief    Definition of the length for the char array to set the date arribute
 */
#define DATE_STRING_LENGTH 256

/**
 * @brief    Constructor.
 */
tmlSenderStream::tmlSenderStream()
{
  // Callbacks are not initialized:
  m_pCBFuncGetSize = NULL;
  m_pCBDataGetSize = NULL;
  m_pCBFuncRead = NULL;
  m_pCBDataRead = NULL;
  m_pCBFuncWrite = NULL;
  m_pCBDataWrite = NULL;
  m_pCBFuncSeek = NULL;
  m_pCBDataSeek = NULL;
  m_pCBFuncGetPosition = NULL;
  m_pCBDataGetPosition = NULL;
  m_pCBFuncClose = NULL;
  m_pCBDataClose = NULL;
  m_pCBFuncOnError = NULL;
  m_pCBDataOnError = NULL;

  m_feeder = NULL;

  // Connection parameter:  
  m_sProfile = NULL;
  m_sHost = NULL;
  m_sPort = NULL;

  m_sKey = NULL;

  m_iStreamID = 0;
  m_iLastErr = TML_SUCCESS;
  m_bCancelDownload = false;

  m_date = NULL;
  m_hashCalc = new md5Hash();
}


/**
 * @brief    Destructor.
 */
tmlSenderStream::~tmlSenderStream()
{
  reset();
  delete m_hashCalc;
}

/**
 * @brief    Get the local time as a formatted string
 */
char* tmlSenderStream::getFormattedTime(char* sKey){
  // Set the CMD creation date:
#ifdef LINUX
  time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  struct timeval detail_time;
  gettimeofday(&detail_time,NULL);
  int iMilliSeconds = detail_time.tv_usec % 1000;

  sprintf(m_date, "%4d%2d%2d%2d%2d%2d%3d-%s", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,
                                                                      timeinfo->tm_mday,
                                                                       timeinfo->tm_hour,
                                                                        timeinfo->tm_min,
                                                                         timeinfo->tm_sec,
                                                                          iMilliSeconds,
                                                                          sKey);
#else // LINUX
  SYSTEMTIME lpSystemTime;
  GetLocalTime(&lpSystemTime);
  
#ifdef MINGW_BUILD
  sprintf(m_date, "%4d%2d%2d%2d%2d%2d%3d-%s", lpSystemTime.wYear,
                                                                    lpSystemTime.wMonth,
                                                                      lpSystemTime.wDay,
                                                                        lpSystemTime.wHour,
                                                                          lpSystemTime.wMinute,
                                                                            lpSystemTime.wSecond,
                                                                              lpSystemTime.wMilliseconds,
                                                                              sKey);
#else //MINGW_BUILD
  sprintf_s(m_date, DATE_STRING_LENGTH, "%4d%2d%2d%2d%2d%2d%3d-%s", lpSystemTime.wYear,
                                                                    lpSystemTime.wMonth,
                                                                      lpSystemTime.wDay,
                                                                        lpSystemTime.wHour,
                                                                          lpSystemTime.wMinute,
                                                                            lpSystemTime.wSecond,
                                                                              lpSystemTime.wMilliseconds,
                                                                              sKey);
#endif //MINGW_BUILD
#endif // LINUX
  // Replace each blank with the '0' character:
  for (unsigned int i = 0; i < strlen(m_date);++i){
    if (' ' == m_date[i]){
      m_date[i] = '0';
    }
  }
  return m_date;
}

/**
 * @brief    Open a sender stream.
 */
int tmlSenderStream::open(TML_STREAM_ID_TYPE* iStreamID, const char* sPort, const char* sHost, const char* profile){
  int iRet = TML_ERR_STREAM_INVALID_ADDRESS;

  if (NULL == m_date){
    ////////////////////////////////
    // alloc memory for char array to set the date attribute
    m_date = new char[DATE_STRING_LENGTH];
  }

  if (NULL != profile && 0 < strlen(profile) &&
      NULL != sHost && 0 < strlen(sHost) &&
      NULL != sPort && 0 < strlen(sPort)){
    int iSize;
    /////////////////////////////
    // Profile:
    if (NULL != m_sProfile)
      delete[] m_sProfile;
    iSize = (int)strlen(profile)+1;
    m_sProfile = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
    strcpy (m_sProfile, profile);
#else
    strcpy_s (m_sProfile, iSize, profile);
#endif
    /////////////////////////////
    // Host:
    if (NULL != m_sHost)
      delete[] m_sHost;
    iSize = (int)strlen(sHost)+1;
    m_sHost = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
    strcpy (m_sHost, sHost);
#else
    strcpy_s (m_sHost, iSize, sHost);
#endif
    /////////////////////////////
    // Port:
    if (NULL != m_sPort)
      delete[] m_sPort;
    iSize = (int)strlen(sPort)+1;
    m_sPort = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
    strcpy (m_sPort, sPort);
#else
    strcpy_s (m_sPort, iSize, sPort);
#endif


    ///////////////////////////////////////////////////////////////
    // Maybe the global magic number wasn't initialized randomly:
    if (0 == g_iMagic){
#ifdef LINUX
      time_t rawtime;
      struct tm * timeinfo;

      time (&rawtime);
      timeinfo = localtime (&rawtime);

      struct timeval detail_time;
      gettimeofday(&detail_time,NULL);
      int iMilliSeconds = detail_time.tv_usec % 1000;
      srand(iMilliSeconds);
#else
      SYSTEMTIME lpSystemTime;
      GetLocalTime(&lpSystemTime);
      srand(lpSystemTime.wMilliseconds);
#endif // LINUX
      g_iMagic = rand();
    } 
    ++g_iMagic;
    /////////////////////////////
    // Key:
    if (NULL != m_sKey)
      delete[] m_sKey;
    iSize = (int)strlen(profile) + (int)strlen(sHost) + (int)strlen (sPort) + 32 + 3 + 1;
    m_sKey = new char[iSize];
#if defined (LINUX) || defined (MINGW_BUILD)
    sprintf(m_sKey, "%s-%s-%s-%d", profile, sHost, sPort, g_iMagic);
#else // LINUX
    sprintf_s(m_sKey, iSize, "%s-%s-%s-%d", profile, sHost, sPort, g_iMagic);
#endif // LINUX

    char* FormattedString = getFormattedTime(m_sKey);

    TML_INT64 lValue = 0;
    TML_INT64 hValue = 0;
    size_t length = strlen(FormattedString);
    char* sHash = m_hashCalc->process((uint8_t*)FormattedString, length, &hValue, &lValue); 
    ////////////////////////////////////////
    // XOR OPERATION:
    TML_INT64 value = hValue ^ lValue;

    TML_INT64 magicPatch = g_iMagic;
    ////////////////////////////////////////
    // Only the last 16 Bit:
    magicPatch = magicPatch & 0xFFFF;
    ////////////////////////////////////////
    // Only high 48 Bit:
#ifdef LINUX
    value = value & 0xFFFFFFFFFFFF0000LLU;
#else // LINUX
    value = value & 0xFFFFFFFFFFFF0000;
#endif // LINUX
    ////////////////////////////////////////
    // Now patch it into the result:
    magicPatch = magicPatch | value;

    ////////////////////////////////////////
    // Only a positive value:
    if (magicPatch < 0){
      magicPatch *= -1;
    }
    ////////////////////////////////////////
    // Result:
    *iStreamID = m_iStreamID = magicPatch;

    iRet = TML_SUCCESS;
  }

  return iRet;
}


/**
 * @brief    Close a sender stream.
 */
int tmlSenderStream::close(){
  int iRet = TML_SUCCESS;

  reset();
  return iRet;
}


/**
 * @brief    Helper method to get the key for hash computing.
 */
void tmlSenderStream::getKey(char** sKey){
  *sKey = m_sKey;
}


/**
 * @brief    Helper method to get the streamID for hash computing.
 */
void tmlSenderStream::getID(TML_STREAM_ID_TYPE* iID){
  *iID = m_iStreamID;
}


/**
 * @brief    Helper method resets all parameter.
 */
void tmlSenderStream::reset(){
  m_pCBFuncGetSize = NULL;
  m_pCBDataGetSize = NULL;
  m_pCBFuncRead = NULL;
  m_pCBDataRead = NULL;
  m_pCBFuncWrite = NULL;
  m_pCBDataWrite = NULL;
  m_pCBFuncSeek = NULL;
  m_pCBDataSeek = NULL;
  m_pCBFuncGetPosition = NULL;
  m_pCBDataGetPosition = NULL;
  m_pCBFuncClose = NULL;
  m_pCBDataClose = NULL;
  m_pCBFuncOnError = NULL;
  m_pCBDataOnError = NULL;

  if (NULL != m_sProfile)
    delete[] m_sProfile;
  m_sProfile = NULL;
  if (NULL != m_sHost)
    delete[] m_sHost;
  m_sHost = NULL;
  if (NULL != m_sPort)
    delete[] m_sPort;
  m_sPort = NULL;

  if (NULL != m_sKey)
    delete[] m_sKey;
  m_sKey = NULL;

  m_iStreamID = 0;
  m_iLastErr = TML_SUCCESS;
  m_bCancelDownload = false;

  if (NULL != m_date)
    delete (m_date);
  m_date = NULL;
}


/**
 * @brief    Register a callback method to get the sender stream size.
 */
int tmlSenderStream::register_GetSize (TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_pCBFuncGetSize = pCBFunc;
  m_pCBDataGetSize = pCBData;
  return iRet;
}


/**
 * @brief    Register a callback method to perform stream seek operations.
 */
int tmlSenderStream::register_Seek (TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_pCBFuncSeek = pCBFunc;
  m_pCBDataSeek = pCBData;
  return iRet;
}


/**
 * @brief    Register a callback method to get the stream position.
 */
int tmlSenderStream::register_GetPosition (TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_pCBFuncGetPosition = pCBFunc;
  m_pCBDataGetPosition = pCBData;
  return iRet;
}


/**
 * @brief    Get registered callback method to get the sender stream size.
 */
int tmlSenderStream::registered_GetSize (TML_ON_SND_STRM_GET_SIZE_CB_FUNC * pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  if (NULL != m_pCBFuncGetSize){
    *pCBFunc = m_pCBFuncGetSize;
    *pCBData = m_pCBDataGetSize;
  }
  else{
    iRet = TML_ERR_STREAM_INVALID_CALLBACK;
  }
  return iRet;
}


/**
 * @brief    Get registered callback method to perform the sender stream seek operations.
 */
int tmlSenderStream::registered_Seek (TML_ON_SND_STRM_SEEK_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  if (NULL != m_pCBFuncSeek){
    *pCBFunc = m_pCBFuncSeek;
    *pCBData = m_pCBDataSeek;
  }
  else{
    iRet = TML_ERR_STREAM_INVALID_CALLBACK;
  }
  return iRet;
}


/**
 * @brief    Get registered callback method to get the sender stream position.
 */
int tmlSenderStream::registered_GetPosition (TML_ON_SND_STRM_GET_POSITION_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  if (NULL != m_pCBFuncGetPosition){
    *pCBFunc = m_pCBFuncGetPosition;
    *pCBData = m_pCBDataGetPosition;
  }
  else{
    iRet = TML_ERR_STREAM_INVALID_CALLBACK;
  }
  return iRet;
}


/**
 * @brief    Register a callback method to read data of the sender stream.
 */
int tmlSenderStream::register_Read (TML_ON_SND_STRM_READ_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_pCBFuncRead = pCBFunc;
  m_pCBDataRead = pCBData;
  return iRet;
}


/**
 * @brief    Get registered callback method for the read from the sender stream.
 */
int tmlSenderStream::registered_Read (TML_ON_SND_STRM_READ_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  if (NULL != m_pCBFuncRead){
    *pCBFunc = m_pCBFuncRead;
    *pCBData = m_pCBDataRead;
  }
  else{
    iRet = TML_ERR_STREAM_INVALID_CALLBACK;
  }
  return iRet;
}


/**
 * @brief    Register a callback method to write data to a sender stream.
 */
int tmlSenderStream::register_Write (TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_pCBFuncWrite = pCBFunc;
  m_pCBDataWrite = pCBData;
  return iRet;
}


/**
 * @brief    Get registered callback method for the write to the sender stream.
 */
int tmlSenderStream::registered_Write (TML_ON_SND_STRM_WRITE_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  if (NULL != m_pCBFuncWrite){
    *pCBFunc = m_pCBFuncWrite;
    *pCBData = m_pCBDataWrite;
  }
  else{
    iRet = TML_ERR_STREAM_INVALID_CALLBACK;
  }
  return iRet;
}


/**
 * @brief    Register a callback method that will be called in case of closing the sender stream.
 */
int tmlSenderStream::register_Close (TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_pCBFuncClose = pCBFunc;
  m_pCBDataClose = pCBData;
  return iRet;
}


/**
 * @brief    Get registered callback method for the close of the sender stream.
 */
int tmlSenderStream::registered_Close (TML_ON_SND_STRM_CLOSE_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  if (NULL != m_pCBFuncClose){
    *pCBFunc = m_pCBFuncClose;
    *pCBData = m_pCBDataClose;
  }
  else{
    iRet = TML_ERR_STREAM_INVALID_CALLBACK;
  }
  return iRet;
}


/**
 * @brief    Register a callback method that will be called on communication fault during a request.
 */
int tmlSenderStream::register_OnError (TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_pCBFuncOnError = pCBFunc;
  m_pCBDataOnError = pCBData;
  return iRet;
}


/**
 * @brief    Get registered callback method that will be called on communication fault during a request.
 */
int tmlSenderStream::registered_OnError (TML_ON_SND_STRM_COM_FAULT_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  int iRet = TML_SUCCESS;

  if (NULL != m_pCBFuncOnError){
    *pCBFunc = m_pCBFuncOnError;
    *pCBData = m_pCBDataOnError;
  }
  else{
    iRet = TML_ERR_STREAM_INVALID_CALLBACK;
  }
  return iRet;
}


/**
 * @brief    Set the feeder object
 */
void tmlSenderStream::setFeeder (VortexPayloadFeeder* feeder){
  m_feeder = feeder;
}


/**
 * @brief    Get the feeder reference object
 */
void tmlSenderStream::getFeeder (VortexPayloadFeeder** feeder){
  *feeder = m_feeder;
}


/**
 * @brief    Set the last error code.
 */
void tmlSenderStream::setLastErr (int iError){
  m_iLastErr = iError;
}


/**
 * @brief    Returns the last error code.
 */
int tmlSenderStream::getLastErr (){
  return m_iLastErr;
}


/**
 * @brief    Set the cancel flag for the download operation.
 */
void tmlSenderStream::setCancelDownload (bool bCancel){
  m_bCancelDownload = bCancel;
}


/**
 * @brief    Returns true in case of a pending cancel.
 */
bool tmlSenderStream::isCancelDownloadPending (){
  return m_bCancelDownload;
}
