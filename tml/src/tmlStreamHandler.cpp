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

#include "tmlStreamHandler.h"
#include "tmlSenderStream.h"
#include "tmlReceiverStream.h"
#include "tmlCmdDefines.h"
#include "sidex.h"
#include "tmlErrors.h"
#include "logValues.h"



#define COMMON_STREAM_CALLBACK_ERR -1

axl_bool __vortex_payload_feeder_stream (VortexCtx  * ctx,
               VortexPayloadFeederOp     op_type,
               VortexPayloadFeeder*      feeder,
               axlPointer                param1,
               axlPointer                param2,
               axlPointer                user_data)
{
  VortexPayloadStreamFeederData * data  = (VortexPayloadStreamFeederData*)user_data;
  tmlCriticalSectionObj* criticalSection = data->criticalSection;
  tmlLogHandler* log;
  log = data->log;

  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  criticalSection->tmlCriticalSectionEnter("__vortex_payload_feeder_stream");

  int                       * size   = (int*) param1;
  char                      * buffer = (char*) param2;
  axl_bool bRet = axl_true;

  switch (op_type) {
  case PAYLOAD_FEEDER_GET_SIZE:
        {
          tmlSenderStream* stream;
          int iRet = data->sndStreamObjects->getValue(data->iStreamID, (void**) &stream);
          if (TML_SUCCESS != iRet || NULL == stream){
            data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_GET_SIZE", "stream ID doesn't exist anymore");
            *size = 0;// A value of "1" caused sporadically exceptions on file source side , so:
            bRet = axl_false;
          }
          else{
            if (stream->isCancelDownloadPending()){
              /////////////////////////////////////////////////////////////
              // The server has called to cancel the transmission:
              data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_GET_SIZE Canceled by user", data->iStreamID);
              *size = 1;// Fill it with the size of 1 to reach the finish / if not the PAYLOAD_FEEDER_IS_FINISHED will not be called !
              bRet = axl_false;
            }
            else{
              TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc = data->m_pCBFuncGetSize;
              TML_POINTER pCBData = data->m_pCBDataGetSize;
              if (NULL != pCBFunc){
                ///////////////////////////////////////////////////////////////////////////////////////////////
                // Callback in the manner:
                //  TML_INT64 pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER pCBData)

                // Note: Filesize 64 Bit / z.Zt. casting
                data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "Call the PAYLOAD_FEEDER_GET_SIZE Callback Method", data->iStreamID);
                *size = (int)((TML_INT64(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER))pCBFunc)(data->iStreamID, pCBData);
                data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_GET_SIZE / Size = ", *size);
                data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "Return from PAYLOAD_FEEDER_GET_SIZE Callback", data->iStreamID);
                data->iStreamSize = *size;
                if (COMMON_STREAM_CALLBACK_ERR == *size){
                  bRet = axl_false;
                }
              }
              else{
                bRet = axl_false;
              }
            }
          }
        }
        break;
  case PAYLOAD_FEEDER_GET_CONTENT:
        {
          tmlSenderStream* stream;
          int iRet = data->sndStreamObjects->getValue(data->iStreamID, (void**) &stream);
          if (TML_SUCCESS != iRet || NULL == stream){
            data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_GET_CONTENT", "stream ID doesn't exist anymore");
            *size = 0;// A value of "1" caused sporadically exceptions on file source side , so:
            bRet = axl_false;
          }
          else{
            if (stream->isCancelDownloadPending()){
              /////////////////////////////////////////////////////////////
              // The server has called to cancel the transmission:
              data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_GET_CONTENT Canceled by user", data->iStreamID);
              *size = 1;// Fill it with the size of 1 to reach the finish / if not the PAYLOAD_FEEDER_IS_FINISHED will not be called !
              bRet = axl_false;
            }
            else{
              TML_ON_SND_STRM_READ_CB_FUNC pCBFunc = data->m_pCBFuncRead;
              TML_POINTER pCBData = data->m_pCBDataRead;
              if (NULL != pCBFunc){
                ///////////////////////////////////////////////////////////////////////////////////////////////
                // Callback in the manner:
                //  TML_INT32 pCBFunc (TML_STREAM_ID_TYPE iId, TML_POINTER pCBData, TML_POINTER buffer, TML_INT32 count, TML_INT32* bytesRead)
                TML_INT32 bytesRead;
                data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "Call the PAYLOAD_FEEDER_GET_CONTENT Callback Method ID", data->iStreamID);
                int cbRet = ((TML_INT32(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER, TML_POINTER, TML_INT32, TML_INT32*))pCBFunc)(data->iStreamID, pCBData, buffer, (TML_INT32)*size, &bytesRead);
                data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_GET_CONTENT / Size = ", bytesRead);
                data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "Return from PAYLOAD_FEEDER_GET_CONTENT Callback", data->iStreamID);
                if (TML_SUCCESS != cbRet){
                  //////////////////////
                  // it's a read error:
                  data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_GET_CONTENT", "Read Error");
                  bytesRead = 0;
                  data->bReadFailed = true;
                }
                *size = bytesRead;
                data->iTotalBytesRead = data->iTotalBytesRead + bytesRead;
                if (TML_SUCCESS != cbRet){
                  bRet = axl_false;
                }
              }
              else{
                bRet = axl_false;
              }
            }
          }
        }
        break;
  case PAYLOAD_FEEDER_IS_FINISHED:
        {
          tmlSenderStream* stream;
          int iRet = data->sndStreamObjects->getValue(data->iStreamID, (void**) &stream);
          if (TML_SUCCESS != iRet || NULL == stream){
            data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_IS_FINISHED", "stream ID doesn't exist anymore");
            *size = 1;// Fill it with the size of 1 to reach the finish / if not the PAYLOAD_FEEDER_IS_FINISHED will not be called !
            bRet = axl_false;
          }
          else{
            if (stream->isCancelDownloadPending()){
              /////////////////////////////////////////////////////////////
              // The server has called to cancel the transmission:
              data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_IS_FINISHED Canceled by user", data->iStreamID);
              *size = 1;
            }
            else{
              if (data->bReadFailed){
                data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_IS_FINISHED Read failed", data->iStreamID);
                *size = 1;
              }
              else{
                *size = data->iTotalBytesRead == data->iStreamSize;
                if (*size){
                  data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_IS_FINISHED FINISHED", data->iStreamID);
                }
                else{
                  data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_IS_FINISHED NOT FINISHED YET", data->iStreamID);
                }
              }
            }
          }
        }
        break;
  case PAYLOAD_FEEDER_RELEASE:
        {
          tmlSenderStream* stream;
          int iRet = data->sndStreamObjects->getValue(data->iStreamID, (void**) &stream);
          if (TML_SUCCESS != iRet || NULL == stream){
            data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "PAYLOAD_FEEDER_RELEASE", "stream ID doesn't exist anymore");
            ////////////////////////////////////////
            // Maybe in this case the *size is NULL:
            if (NULL != size){
              *size = 1;// Fill it with the size of 1 to reach the finish / if not the PAYLOAD_FEEDER_IS_FINISHED will not be called !
            }
            bRet = axl_false;
          }
          else{
            TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc = data->m_pCBFuncClose;
            TML_POINTER pCBData = data->m_pCBDataClose;
            if (NULL != pCBFunc){
              ///////////////////////////////////////////////////////////////////////////////////////////////
              // Callback in the manner:
              //  void pCBFunc (TML_STREAM_ID_TYPE iID,  TML_POINTER pCBData)
              data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "Call the PAYLOAD_FEEDER_RELEASE Callback Method", data->iStreamID);
              ((void(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER))pCBFunc)(data->iStreamID, pCBData);
              data->log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "__vortex_payload_feeder_stream", "Return from PAYLOAD_FEEDER_RELEASE Callback", data->iStreamID);
            }
            else{
              bRet = axl_false;
            }
            delete data;
          }
        }
        break;
  } /* end switch */

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  criticalSection->tmlCriticalSectionLeave("__vortex_payload_feeder_stream");
  return bRet;
}


/**
 * @brief    Constructor.
 */
tmlStreamHandler::tmlStreamHandler(tmlLogHandler* loghandler)
{
  m_log = loghandler;
  ////////////////////////////////
  // sender stream objects hash table:
  m_sndStreamObjects = NULL;
  m_sndStreamObjects = new TMLUniversalHashTable(loghandler);
  m_sndStreamObjects->createHashTable(false);
  ////////////////////////////////
  // receiver stream objects hash table:
  m_recStreamObjects = NULL;
  m_recStreamObjects = new TMLUniversalHashTable(loghandler);
  m_recStreamObjects->createHashTable(false);
  m_CriticalSectionReceiver = new tmlCriticalSectionObj();
  m_CriticalSectionSender = new tmlCriticalSectionObj();
}


/**
 * @brief    Destructor.
 */
tmlStreamHandler::~tmlStreamHandler()
{
  ////////////////////////////////
  // sender stream objects hash table:
  if (NULL != m_sndStreamObjects){
    ///////////////////////////////////////////
    // Unregister possible registered keys:
    TML_STREAM_ID_TYPE* iIds;
    int iRet = m_sndStreamObjects->getKeys((TML_INT64**)&iIds);
    if (TML_SUCCESS == iRet){
      if (NULL != iIds){
        int iSize;
        iRet = m_sndStreamObjects->hashSize(&iSize);
        if (TML_SUCCESS == iRet){
          for (int i = 0; i < iSize; ++i){
            void* sHandle = NULL;
            iRet = m_sndStreamObjects->getValue(iIds[i], (void**) &sHandle);
            if (TML_SUCCESS == iRet && NULL != sHandle){
              sndStream_Close(iIds[i]);
            }
          }
        }
        /////////////////////////////////
        // Now I have to free the memory:
        delete iIds;
        iIds = NULL;
      }
    }
    delete (m_sndStreamObjects);
    m_sndStreamObjects = NULL;
  }

  ////////////////////////////////
  // receiver stream objects hash table:
  if (NULL != m_recStreamObjects){
    ///////////////////////////////////////////
    // Unregister possible registered keys:
    TML_STREAM_ID_TYPE* iIds;
    int iRet = m_recStreamObjects->getKeys((TML_INT64**)&iIds);
    if (TML_SUCCESS == iRet){
      if (NULL != iIds){
        int iSize;
        iRet = m_recStreamObjects->hashSize(&iSize);
        if (TML_SUCCESS == iRet){
          for (int i = 0; i < iSize; ++i){
            void* sHandle = NULL;
            iRet = m_recStreamObjects->getValue(iIds[i], (void**) &sHandle);
            if (TML_SUCCESS == iRet && NULL != sHandle){
              recStream_Close(iIds[i], TML_FALSE, false);
            }
          }
        }
        /////////////////////////////////
        // Now I have to free the memory:
        delete iIds;
        iIds = NULL;
      }
    }
    delete (m_recStreamObjects);
    m_recStreamObjects = NULL;
  }
  delete(m_CriticalSectionReceiver);
  m_CriticalSectionReceiver = NULL;
  delete(m_CriticalSectionSender);
  m_CriticalSectionSender = NULL;
}


/**
 * @brief    Close a sender stream.
 */
int tmlStreamHandler::sndStream_Close(TML_STREAM_ID_TYPE iID){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  m_CriticalSectionSender->tmlCriticalSectionEnter("sndStream_Close");


  int iRet = TML_SUCCESS;
  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Close", "", iID);

  tmlSenderStream* stream = NULL;
  iRet = m_sndStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Close", "iID", "not found");
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  else{
    ///////////////////////////////////////////////////////////////////////
    // Be shure that we cancel the operation / necessary in case of a
    // pending download and an exit on the client:
    stream->setCancelDownload(true);
    ///////////////////////////////////////////////////////////////////////
    // Afterwards I can close:
    iRet = stream->close();
    m_sndStreamObjects->removeEntry(iID);// this will delete the object
    ////////////////////////////////////////
    // Delete the value stored in the hash:
    delete (stream);
    ////////////////////////////////////////
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("sndStream_Close");
  return iRet;
}


/**
 * @brief    Open a receiver stream.
 */
int tmlStreamHandler::recStream_Open(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iStreamID, const char* sPort, const char* sHost, const char* profile, tmlCollectCall* sender, int iWindowSize){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Open", "", iStreamID);

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iStreamID, (void**) &stream);
  if (TML_SUCCESS == iRet){
    if (NULL != stream){
      iRet = stream->open(iStreamID, sPort, sHost, profile);
    }
    else{
      stream = new tmlReceiverStream(coreHandle, sender, iWindowSize, m_log);

      iRet = stream->open(iStreamID, sPort, sHost, profile);
      if (TML_SUCCESS == iRet){
        ////////////////////////////////////////////
        // Now I have to remember the stream object:
        m_recStreamObjects->setValue(iStreamID, stream);
      }
      else{
        delete(stream);
      }
    }
  }
  return iRet;
}


/**
 * @brief    Close a receiver stream.
 */
int tmlStreamHandler::recStream_Close(TML_STREAM_ID_TYPE iID, TML_BOOL bRetainOpen, bool bSendCancel){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  m_CriticalSectionReceiver->tmlCriticalSectionEnter("recStream_Close");


  int iRet = TML_SUCCESS;

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Close", "iID", "not found");
    iRet = TML_ERR_INFORMATION_UNDEFINED;
  }
  else{
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Close", "", iID);
    iRet = stream->close(bRetainOpen, bSendCancel);

    if (TML_FALSE == bRetainOpen){
      ////////////////////////////////////////////
      // Now I have to remove the stream object out of the hashtable:
      m_recStreamObjects->removeEntry(iID);// this will delete the object
      ////////////////////////////////////////
      // Delete the value stored in the hash:
      delete (stream);
      ////////////////////////////////////////
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionReceiver->tmlCriticalSectionLeave("recStream_Close");
  return iRet;
}


/**
 * @brief    Get the size of a receiver stream.
 */
int tmlStreamHandler::recStream_GetSize(TML_STREAM_ID_TYPE iID, TML_INT64* rsize){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_GetSize", "", "");

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_GetSize", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->getSize(rsize);
  return iRet;
}


/**
 * @brief    Perform a seek operation of a receiver stream.
 */
int tmlStreamHandler::recStream_Seek(TML_STREAM_ID_TYPE iID, TML_INT64 seekPos, tmlSeekOrigin origin){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Seek", "", "");

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Seek", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->seek(seekPos, origin);
  return iRet;
}


/**
 * @brief    Get the position of a receiver stream.
 */
int tmlStreamHandler::recStream_GetPosition(TML_STREAM_ID_TYPE iID, TML_INT64* rposition){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_GetPosition", "", "");

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_GetPosition", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->getPosition(rposition);
  return iRet;
}


/**
 * @brief    Write data to a receiver stream.
 */
int tmlStreamHandler::recStream_Write(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Write", "", "");

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Write", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->write(buffer, count);
  return iRet;
}


/**
 * @brief    Read data of a receiver stream.
 */
int tmlStreamHandler::recStream_Read(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count, TML_INT32* bytesRead){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Read", "", "");

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_Read", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->read(buffer, count, bytesRead);
  return iRet;
}


/**
 * @brief    Read data of a receiver stream.
 */
int tmlStreamHandler::recStream_ReadBuffer(TML_STREAM_ID_TYPE iID, void* buffer, TML_INT32 count){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_ReadBuffer", "", "");

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_ReadBuffer", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->readBuffer(buffer, count);
  return iRet;
}




/**
 * @brief    Initiates a download of data of a receiver stream.
 */
int tmlStreamHandler::recStream_DownloadData(TML_STREAM_ID_TYPE iID, TML_UINT32 buffersize, TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC pCBFuncDld, TML_POINTER pCBDataDld, TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncDldFinish, TML_POINTER pCBDataDldFinish){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_DownloadData", "", "");

  tmlReceiverStream* stream = NULL;
  iRet = m_recStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "recStream_DownloadData", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->downloadData(m_CriticalSectionReceiver, m_recStreamObjects, buffersize, pCBFuncDld, pCBDataDld, pCBFuncDldFinish, pCBDataDldFinish);
  return iRet;
}

    
/**
 * @brief    Open a sender stream.
 */
int tmlStreamHandler::sndStream_Open(TML_STREAM_ID_TYPE* iStreamID, const char* sPort, const char* sHost, const char* profile){

  int iRet = TML_SUCCESS;


  tmlSenderStream* stream = new tmlSenderStream();

  iRet = stream->open(iStreamID, sPort, sHost, profile);
  if (TML_SUCCESS == iRet){
    ////////////////////////////////////////////
    // Now I have to remember the stream object:
    m_sndStreamObjects->setValue(*iStreamID, stream);
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Open", "", *iStreamID);
  }
  else{
    delete(stream);
  }

  return iRet;
}

  
/**
 * @brief    Register a callback method to get the sender stream size.
 */
int tmlStreamHandler::sndStream_Register_GetSize (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_GetSize", "", "");

  tmlSenderStream* stream = NULL;
  iRet = m_sndStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_GetSize", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->register_GetSize(pCBFunc, pCBData);
  return iRet;
}


/**
 * @brief    Register a callback method to perform stream seek operations.
 */
int tmlStreamHandler::sndStream_Register_Seek (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_Seek", "", "");

  tmlSenderStream* stream = NULL;
  iRet = m_sndStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_Seek", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->register_Seek(pCBFunc, pCBData);
  return iRet;
}


/**
 * @brief    Register a callback method to get the stream position.
 */
int tmlStreamHandler::sndStream_Register_GetPosition (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_GetPosition", "", "");

  tmlSenderStream* stream = NULL;
  iRet = m_sndStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_GetPosition", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->register_GetPosition(pCBFunc, pCBData);
  return iRet;
}


/**
 * @brief    Register a callback method to read data of the sender stream.
 */
int tmlStreamHandler::sndStream_Register_Read (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_READ_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_Read", "", "");

  tmlSenderStream* stream = NULL;
  iRet = m_sndStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_Read", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->register_Read(pCBFunc, pCBData);

  return iRet;
}


/**
 * @brief    Register a callback method to write data to a sender stream.
 */
int tmlStreamHandler::sndStream_Register_Write (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_Write", "", "");

  tmlSenderStream* stream = NULL;
  iRet = m_sndStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_Write", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->register_Write(pCBFunc, pCBData);

  return iRet;
}


/**
 * @brief    Register a callback method that will be called in case of closing the sender stream.
 */
int tmlStreamHandler::sndStream_Register_Close (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_Close", "", "");

  tmlSenderStream* stream = NULL;
  iRet = m_sndStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_Close", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->register_Close(pCBFunc, pCBData);

  return iRet;
}


/**
 * @brief    Register a callback method that will be called on communication fault during a request.
 */
int tmlStreamHandler::sndStream_Register_OnError (TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc, TML_POINTER pCBData){
  int iRet = TML_SUCCESS;

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_OnError", "", "");

  tmlSenderStream* stream = NULL;
  iRet = m_sndStreamObjects->getValue(iID, (void**) &stream);
  if (TML_SUCCESS != iRet || NULL == stream){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "sndStream_Register_OnError", "iID", "not found");
    return TML_ERR_INFORMATION_UNDEFINED;
  }

  iRet = stream->register_OnError(pCBFunc, pCBData);

  return iRet;
}


/**
 * @brief    Handle receiver stream close to the sender stream
 */
int tmlStreamHandler::handle_close (SIDEX_HANDLE sHandle){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  m_CriticalSectionSender->tmlCriticalSectionEnter("handle_close");

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_close", "", "");

  TML_STREAM_ID_TYPE iStreamID;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    tmlSenderStream* senderStreamObj = NULL;
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc = NULL;
      TML_POINTER pCBData = NULL;
      iRet = senderStreamObj->registered_Close(&pCBFunc, &pCBData);
      if (TML_SUCCESS == iRet){
        ///////////////////////////////////////////////////////////////////////////////////////////////
        // Callback in the manner:
        //  void pCBFunc (TML_STREAM_ID_TYPE iID,  TML_POINTER pCBData)
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_close", "Call the  Callback Method", iStreamID);
       ((void(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER))pCBFunc)((TML_STREAM_ID_TYPE)iStreamID, pCBData);
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_close", "Return from Callback", iStreamID);
      }
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("handle_close");
  return (int)iRet;
}


/**
 * @brief    Handle a size request
 */
int tmlStreamHandler::handle_getSize (SIDEX_HANDLE sHandle, TML_INT64* iSize){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  m_CriticalSectionSender->tmlCriticalSectionEnter("handle_getSize");

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_getSize", "", "");

  TML_STREAM_ID_TYPE iStreamID;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    tmlSenderStream* senderStreamObj = NULL;
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc = NULL;
      TML_POINTER pCBData = NULL;
      iRet = senderStreamObj->registered_GetSize(&pCBFunc, &pCBData);
      if (TML_SUCCESS == iRet){
        ///////////////////////////////////////////////////////////////////////////////////////////////
        // Callback in the manner:
        //  TML_INT64 pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER pCBData)
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_getSize", "Call the Callback Method", iStreamID);
        *iSize = ((TML_INT64(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER))pCBFunc)(iStreamID, pCBData);
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_getSize", "Size", *iSize);
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_getSize", "Return from Callback", iStreamID);
        if (COMMON_STREAM_CALLBACK_ERR == *iSize){
          iRet = TML_ERR_STREAM_SIZE_NOT_AVAILABLE;
        }
        else{
          iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_SIZE, *iSize);
        }
      }
      ///////////////////////////////
      // Set the last error here:
      senderStreamObj->setLastErr(iRet);
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("handle_getSize");
  return (int)iRet;
}


/**
 * @brief    Handle download via Vortex payload feeder
 */
int tmlStreamHandler::handle_VortexFeederDownload (VortexCtx* ctx, TML_STREAM_ID_TYPE iStreamID, VortexPayloadFeeder** feeder, VortexPayloadStreamFeederData* data){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  m_CriticalSectionSender->tmlCriticalSectionEnter("handle_VortexFeederDownload");

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_VortexFeederDownload", "", "");

  ////////////////////////////////////////////
  // Total bytes set to 0:
  data->iTotalBytesRead = 0;
  ////////////////////////////////////////////
  // No read fail:
  data->bReadFailed = false;
  data->criticalSection = m_CriticalSectionSender;
  data->log = m_log;


  tmlSenderStream* senderStreamObj = NULL;
  TML_INT32 iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
  if (NULL != senderStreamObj){
    data->sndStreamObjects = m_sndStreamObjects;
    data->iStreamID = iStreamID;
    TML_ON_SND_STRM_SEEK_CB_FUNC pCBFuncSeek = NULL;
    TML_POINTER pCBDataSeek = NULL;
    if (TML_SUCCESS == iRet){
      iRet = senderStreamObj->registered_Seek(&pCBFuncSeek, &pCBDataSeek);
    }
    TML_ON_SND_STRM_READ_CB_FUNC pCBFuncRead = NULL;
    TML_POINTER pCBDataRead = NULL;
    if (TML_SUCCESS == iRet){
      data->m_pCBFuncSeek = pCBFuncSeek;
      data->m_pCBDataSeek = pCBDataSeek;
      iRet = senderStreamObj->registered_Read(&pCBFuncRead, &pCBDataRead);
    }
    TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFuncGetSize = NULL;
    TML_POINTER pCBDataGetSize = NULL;
    if (TML_SUCCESS == iRet){
      data->m_pCBFuncRead = pCBFuncRead;
      data->m_pCBDataRead = pCBDataRead;
      iRet = senderStreamObj->registered_GetSize(&pCBFuncGetSize, &pCBDataGetSize);
    }
    TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFuncClose = NULL;
    TML_POINTER pCBDataClose = NULL;
    if (TML_SUCCESS == iRet){
      data->m_pCBFuncGetSize = pCBFuncGetSize;
      data->m_pCBDataGetSize = pCBDataGetSize;
      iRet = senderStreamObj->registered_Close(&pCBFuncClose, &pCBDataClose);
    }
    TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFuncComFault = NULL;
    TML_POINTER pCBDataComFault = NULL;
    if (TML_SUCCESS == iRet){
      data->m_pCBFuncClose = pCBFuncClose;
      data->m_pCBDataClose = pCBDataClose;
      iRet = senderStreamObj->registered_OnError(&pCBFuncComFault, &pCBDataComFault);
    }
    if (TML_SUCCESS == iRet){
      data->m_pCBFuncOnError = pCBFuncComFault;
      data->m_pCBDataOnError = pCBDataComFault;
    }
  }
  if (TML_SUCCESS == iRet){
    ////////////////////////////////////////////////////////////////////////////
    // Vortex Payload Feeder:
    *feeder = vortex_payload_feeder_new (__vortex_payload_feeder_stream, data);
    ///////////////////////////////////////////////////
    // Set the reference object for a possible cancel:
    senderStreamObj->setFeeder(*feeder);
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("handle_VortexFeederDownload");
  return iRet;
}


/**
 * @brief    Handle a size seek operation
 */
int tmlStreamHandler::handle_seek (SIDEX_HANDLE sHandle, TML_INT64 seekPosition, TML_INT64 seekOrigin){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  m_CriticalSectionSender->tmlCriticalSectionEnter("handle_seek");

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_seek", "", "");

  TML_STREAM_ID_TYPE iStreamID;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    tmlSenderStream* senderStreamObj = NULL;
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc = NULL;
      TML_POINTER pCBData = NULL;
      iRet = senderStreamObj->registered_Seek(&pCBFunc, &pCBData);
      if (TML_SUCCESS == iRet){
        ///////////////////////////////////////////////////////////////////////////////////////////////
        // Callback in the manner:
        //  TML_INT32 pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER pCBData, TML_INT64 seekPosition, tmlSeekOrigin seekOrigin)
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_seek", "Call the Callback Method", iStreamID);
        iRet = ((TML_INT32(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER, TML_INT64, tmlSeekOrigin))pCBFunc)(iStreamID, pCBData, seekPosition, (tmlSeekOrigin)seekOrigin);
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_seek", "Return from Callback", iStreamID);
        ///////////////////////////////////////////////////////////////////////////////////////////////
        // Callback processing ok ?
        if (TML_SUCCESS != iRet){
          iRet = TML_ERR_STREAM_SEEK_NOT_OPERABLE;
        }
      }
      ///////////////////////////////
      // Set the last error here:
      senderStreamObj->setLastErr(iRet);
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("handle_seek");
  return (int)iRet;
}


/**
 * @brief    Handle a position request
 */
int tmlStreamHandler::handle_getPosition (SIDEX_HANDLE sHandle){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  m_CriticalSectionSender->tmlCriticalSectionEnter("handle_getPosition");

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_getPosition", "", "");

  TML_STREAM_ID_TYPE iStreamID;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    tmlSenderStream* senderStreamObj = NULL;
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc = NULL;
      TML_POINTER pCBData = NULL;
      iRet = senderStreamObj->registered_GetPosition(&pCBFunc, &pCBData);
      if (TML_SUCCESS == iRet){
        ///////////////////////////////////////////////////////////////////////////////////////////////
        // Callback in the manner:
        //  TML_INT64 pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER pCBData)
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_getPosition", "Call the Callback Method", iStreamID);
        TML_INT64 iPosition = ((TML_INT64(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER))pCBFunc)(iStreamID, pCBData);
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_getPosition", "Return from Callback", iStreamID);
        if (COMMON_STREAM_CALLBACK_ERR == iPosition){
          iRet = TML_ERR_STREAM_POSITION_NOT_AVAILABLE;
        }
        else{
          iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_POSITION, iPosition);
        }
      }
      ///////////////////////////////
      // Set the last error here:
      senderStreamObj->setLastErr(iRet);
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("handle_getPosition");
  return (int)iRet;
}


/**
 * @brief    Handle the set last err for raw data operations
 */
void tmlStreamHandler::handle_setLastErr (SIDEX_HANDLE sHandle, int iErr){
  TML_STREAM_ID_TYPE iStreamID;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    tmlSenderStream* senderStreamObj = NULL;
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      senderStreamObj->setLastErr(iErr);
    }
  }
}


/**
 * @brief    Handle the get last errror request
 */
int tmlStreamHandler::handle_getLastErr (SIDEX_HANDLE sHandle){
  TML_STREAM_ID_TYPE iStreamID;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    tmlSenderStream* senderStreamObj = NULL;
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      int iErr = senderStreamObj->getLastErr();
      iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_LAST_ERR, iErr);
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }
  return (int)iRet;
}


/**
 * @brief    Cancel a pending stream download
 */
int tmlStreamHandler::handle_cancelStreamDownload (SIDEX_HANDLE sHandle){

  TML_STREAM_ID_TYPE iStreamID;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_cancelStreamDownload", "", iStreamID);

    tmlSenderStream* senderStreamObj = NULL;

    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      VortexPayloadFeeder* feeder;
      ////////////////////////////////////////////
      // Get the reference to the feeder object:
      senderStreamObj->getFeeder(&feeder);
      m_log->log (TML_LOG_VORTEX_CMD, "tmlStreamHandler", "Vortex CMD", "Vortex CMD", "vortex_payload_feeder_pause");
      ////////////////////////////////////////////
      // Cancel the feeder download:
      axl_bool bRet = vortex_payload_feeder_pause(feeder, axl_true);
      senderStreamObj->setCancelDownload(true);
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }
  return (int)iRet;
}


/**
 * @brief    Handle a read preparation / reset the cancel flag
 */
int tmlStreamHandler::handle_readPreparation (SIDEX_HANDLE sHandle, TML_STREAM_ID_TYPE* iStreamID){
  tmlSenderStream* senderStreamObj = NULL;


  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)iStreamID);
  if (TML_SUCCESS == iRet){
    m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_readPreparation", "", *iStreamID);
    iRet = m_sndStreamObjects->getValue(*iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      senderStreamObj->setCancelDownload(false);
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }
  return (int)iRet;
}

/**
 * @brief    Handle a communication fault
 */
int tmlStreamHandler::handle_OnError (SIDEX_HANDLE sHandle, TML_INT32 iError){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because a close may happen on X:
  m_CriticalSectionSender->tmlCriticalSectionEnter("handle_OnError");

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_OnError", "", "");

  TML_STREAM_ID_TYPE iStreamID;
  tmlSenderStream* senderStreamObj = NULL;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc = NULL;
      TML_POINTER pCBData = NULL;
      iRet = senderStreamObj->registered_OnError(&pCBFunc, &pCBData);
      if (TML_SUCCESS == iRet){
        ///////////////////////////////////////////////////////////////////////////////////////////////
        // Callback in the manner:
        //  void pCBFunc (TML_STREAM_ID_TYPE iID, TML_INT32 iError, TML_POINTER pCBData)
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_OnError", "Calling Callback", iStreamID);
       ((void(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_INT32, TML_POINTER))pCBFunc)(iStreamID, iError, pCBData);
        m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_OnError", "Return from Callback", iStreamID);
      }
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("handle_OnError");
  return iRet;
}


/**
 * @brief    Handle a read request
 */
int tmlStreamHandler::handle_read (SIDEX_HANDLE sHandle, TML_INT32* bytesRead){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because a close may happen on X:
  m_CriticalSectionSender->tmlCriticalSectionEnter("handle_read");

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_read", "", "");

  TML_STREAM_ID_TYPE iStreamID;

  tmlSenderStream* senderStreamObj = NULL;

  TML_INT32 bytes = 0;

  ///////////////////////////////////
  // Set the bytes read initial to 0:
  *bytesRead = 0;

  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      //////////////////////////////////////////////////////
      // First have a look if a cancel is pending:
      if (!senderStreamObj->isCancelDownloadPending()){
        TML_ON_SND_STRM_READ_CB_FUNC pCBFunc = NULL;
        TML_POINTER pCBData = NULL;
        iRet = senderStreamObj->registered_Read(&pCBFunc, &pCBData);
        if (TML_SUCCESS == iRet){
          TML_INT64 count = 0;
          iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_SIZE, &count);
          if (TML_SUCCESS == iRet){
            char* buffer = NULL;
            buffer = new char[(int)count];
            ///////////////////////////////////////////////////////////////////////////////////////////////
            // Callback in the manner:
            //  TML_INT32 pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER pCBData, TML_POINTER buffer, TML_INT32 count, TML_INT32* bytesRead)

            m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_read", "Calling Callback", iStreamID);
            int cbRet = ((TML_INT32(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER, TML_POINTER, TML_INT32, TML_INT32*))pCBFunc)(iStreamID, pCBData, buffer, (TML_INT32)count, &bytes);
            m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_read", "Return from Callback", iStreamID);
            ///////////////////////////////////////////////////////////////////////////////////////////////
            // Callback processing ok ?
            if (TML_SUCCESS != cbRet){
              //////////////////////
              // it's an error:
              bytes = 0;
            }
            iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_BUFFER_BYTRES_READ, bytes);
            if (TML_SUCCESS == iRet){
              iRet = sidex_Integer_Write(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_READ_BUFFER_ADDR, (TML_INT64)buffer);
            }
          }
        }
        *bytesRead = bytes;
      }
      else{
        /////////////////////////////////////////
        // Reset the cancel flag:
        senderStreamObj->setCancelDownload(false);
        iRet = TML_ERR_STREAM_DOWNLOAD_CANCELED;
        bytes = -1;
      }
      ///////////////////////////////
      // Set the last error here:
      if (0 == bytes)
        senderStreamObj->setLastErr(TML_ERR_STREAM_READ_FAILED);
      else
        senderStreamObj->setLastErr(iRet);
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("handle_read");
  return (int)iRet;
}


/**
 * @brief    Handle a write operation
 */
int tmlStreamHandler::handle_write (SIDEX_HANDLE sHandle){
  ///////////////////////////////////////////////////////////////////////
  // Critical section, because of other pending downloads:
  m_CriticalSectionSender->tmlCriticalSectionEnter("handle_write");

  m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_write", "", "");

  TML_STREAM_ID_TYPE iStreamID;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_ID, (SIDEX_INT64*)&iStreamID);
  if (TML_SUCCESS == iRet){
    tmlSenderStream* senderStreamObj = NULL;
    iRet = m_sndStreamObjects->getValue(iStreamID, (void**)&senderStreamObj);
    if (TML_SUCCESS == iRet && NULL != senderStreamObj){
      TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc = NULL;
      TML_POINTER pCBData = NULL;
      iRet = senderStreamObj->registered_Write(&pCBFunc, &pCBData);
      if (TML_SUCCESS == iRet){
        unsigned char* buffer = NULL;
        TML_INT32 iLength = 0;
        ///////////////////////////////////
        // The buffer to write:
        iRet = sidex_Binary_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_WRITE_DATA, &buffer, &iLength);
        if (TML_SUCCESS == iRet){
          ///////////////////////////////////////////////////////////////////////////////////////////////
          // Callback in the manner:
          // TML_INT32 pCBFunc (TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count, TML_POINTER pCBData)
          m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_write", "Calling Callback", iStreamID);
          iRet = ((TML_INT32(FUNC_C_DECL *)(TML_STREAM_ID_TYPE, TML_POINTER, TML_INT32, TML_POINTER))pCBFunc)(iStreamID, buffer, iLength, pCBData);
            m_log->log (TML_LOG_STREAM_HANDLING, "tmlStreamHandler", "handle_write", "Return from Callback", iStreamID);
          ///////////////////////////////////
          // Free the the written buffer
          sidex_Free_Binary_ReadString(buffer);
          if (COMMON_STREAM_CALLBACK_ERR == iRet){
            iRet = TML_ERR_STREAM_WRITE_FAILED;
          }
        }
      }
    }
    else{
      iRet = TML_ERR_STREAM_INVALID_IDENTIFIER;
    }
  }

  ///////////////////////////////////////////////////////////////////////
  // Leave the critical section:
  m_CriticalSectionSender->tmlCriticalSectionLeave("handle_write");
  return (int)iRet;
}


/**
 * @brief    Free allocated memory
 */
void tmlStreamHandler::handle_Free (SIDEX_HANDLE sHandle){

  m_log->log (TML_LOG_STREAM_HANDLING, "handle_Free", "handle_getSize", "", "");

  TML_INT64 addr;
  TML_INT32 iRet = sidex_Integer_Read(sHandle, TML_CMD_STREAM_HEADER_GROUP, TML_CMD_HEADER_KEY_STREAM_READ_BUFFER_ADDR, &addr);
  if (TML_SUCCESS == iRet && 0 != addr){
    char* buffer = (char*)addr;
    delete (buffer);
  }
}




