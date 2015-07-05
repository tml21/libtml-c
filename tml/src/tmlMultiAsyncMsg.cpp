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

#include "tmlObjWrapper.h"
#include "tmlMultiAsyncMsg.h"
#include <sidex.h>
#include <vortex.h>
#include "tmlCore.h"
#include "tmlCmdDefines.h"
#include "tmlCoreDefines.h"

#define GROUP_KEY             "TML_MULTI"
#define PAYLOAD_LIST_KEY      "TML_MULTI"
#define PAYLOAD_LIST_SIZE_KEY "PAYLOAD_LIST_SIZE"
#define SIDEX_OBJ_LIST_KEY    "SIDEX_OBJ_LIST"
#define DUMMY_KEY             "DUMMY_KEY"

/**
 * @brief    Constructor.
 */
tmlMultiAsyncMsg::tmlMultiAsyncMsg()
{
  m_csObj = new tmlCriticalSectionObj();
  m_csObjStream = new tmlCriticalSectionObj();
  m_payloadList = sidex_Variant_New_List ();
  m_payloadStreamList = sidex_Variant_New_List ();
}


/**
 * @brief    Destructor.
 */
tmlMultiAsyncMsg::~tmlMultiAsyncMsg()
{
  TML_INT32 iSize = -1;
  TML_INT64 iPayload = 0;

  TML_INT32 iRet = sidex_Variant_List_Size (m_payloadList, &iSize);
  for (int i = 0; i < iSize && TML_SUCCESS == iRet; ++i){
    SIDEX_VARIANT varPayload;
    iRet = sidex_Variant_List_Get(m_payloadList, 0, &varPayload);
    if (TML_SUCCESS == iRet){
      iRet = sidex_Variant_As_Integer (varPayload, &iPayload);
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_List_DeleteItem (m_payloadList, 0);
      }
    }
    if (iRet == TML_SUCCESS && iPayload){
      payloadDataStructure* data;
      data = (payloadDataStructure*) iPayload;
      delete data;
    }
  }
  sidex_Variant_DecRef(m_payloadList);

  clearStreamList(false);

  sidex_Variant_DecRef(m_payloadStreamList);
  delete (m_csObj);



  delete (m_csObjStream);
}


/**
  * @brief    Clear the stream list
  */
int tmlMultiAsyncMsg::clearStreamList(bool bFreeFrames){
  TML_INT32 iSize = -1;
  TML_INT32 iRet;

  TML_INT64 iStreamPayloadList = 0;
  iRet = sidex_Variant_List_Size (m_payloadStreamList, &iSize);
  for (int i = 0; i < iSize && TML_SUCCESS == iRet; ++i){
    SIDEX_VARIANT varStreamPayload;
    iRet = sidex_Variant_List_Get(m_payloadStreamList, 0, &varStreamPayload);
    if (TML_SUCCESS == iRet){
      iRet = sidex_Variant_As_Integer (varStreamPayload, &iStreamPayloadList);
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_List_DeleteItem (m_payloadStreamList, 0);
      }
    }
    if (iRet == TML_SUCCESS && iStreamPayloadList){
      streamDataStructure* data;
      data = (streamDataStructure*) iStreamPayloadList;
      vortex_frame_unref (data->pFrame);
      delete data;
    }
  }
  return iRet;
}


/**
 * @brief    Get a SIDEX_HANDLE out of the payload
 */
int tmlMultiAsyncMsg::getSidexFromPayoad(SIDEX_HANDLE* retHandle, int iPayloadFrameSize, char* cPayload){
  TML_INT32 iRet = TML_SUCCESS;

  //printf ("getSidexFromPayoad\n");
  SIDEX_HANDLE sHandle = SIDEX_HANDLE_TYPE_NULL;
  if (0 < iPayloadFrameSize){
    iRet = sidex_Create(DUMMY_KEY, &sHandle);
    if (TML_SUCCESS == iRet){
      iRet = sidex_Set_Content(sHandle, cPayload);
    }
    *retHandle = sHandle;
  }
  return (int)iRet;
}


/**
 * @brief    Get a command and stream identification out of the callback data
 */
int tmlMultiAsyncMsg::getCmdFromCallbackData(void* cbvalue, bool* bStream, TML_INT64* cmdValue){
  int iRet = TML_SUCCESS;
  VORTEXSenderFrameReceivedCallbackData* cbdata = (VORTEXSenderFrameReceivedCallbackData*) cbvalue;
  *cmdValue = cbdata->cmdValue;
  *bStream = false;
  TML_BOOL bInternal = cbdata->bInternal;

  if (bInternal){
    switch (*cmdValue){
      case CMD_INTERNAL_STREAM_READ:
      case CMD_INTERNAL_STREAM_DOWNLOAD:
           *bStream = true;
            break;
      default:
           *bStream = false;
            break;
    }
  }
  return iRet;
}


/**
 * @brief    Put stream data into the queue
 */
int tmlMultiAsyncMsg::DataIntoStreamQueue(streamDataStructure* data){
    TML_INT32 iRet = TML_SUCCESS;

    TML_INT32 pos;
    m_csObjStream->tmlCriticalSectionEnter("tmlMultiAsyncMsg::DataIntoStreamQueue");

    SIDEX_VARIANT vData = sidex_Variant_New_Integer((TML_INT64) data);
    iRet = sidex_Variant_List_Append (m_payloadStreamList, vData, &pos);
    if (SIDEX_SUCCESS == iRet){
      sidex_Variant_DecRef(vData);
    }

    m_csObjStream->tmlCriticalSectionLeave("tmlMultiAsyncMsg::DataIntoStreamQueue");
    return (int)iRet;
}


/**
 * @brief    Put payload data into the queue
 */
int tmlMultiAsyncMsg::DataIntoQueue(payloadDataStructure* data){
    TML_INT32 iRet = TML_SUCCESS;

    TML_INT32 pos;
    m_csObj->tmlCriticalSectionEnter("tmlMultiAsyncMsg::DataIntoQueue");

    SIDEX_VARIANT vPayload = sidex_Variant_New_Integer((TML_INT64) data);
    iRet = sidex_Variant_List_Append (m_payloadList, vPayload, &pos);
    if (SIDEX_SUCCESS == iRet){
    sidex_Variant_DecRef(vPayload);
    }

    m_csObj->tmlCriticalSectionLeave("tmlMultiAsyncMsg::DataIntoQueue");
    return (int)iRet;
}


/**
  * @brief    Return first stream queue entry / enters a critialsection obj
  */
int tmlMultiAsyncMsg::GetNextAsyncStreamMsg(streamDataStructure** data, TML_INT32* iRetSize){
  m_csObjStream->tmlCriticalSectionEnter("tmlMultiAsyncMsg::GetNextAsyncStreamMsg");

  TML_INT32 iSize = -1;
  TML_INT64 iStreamPayloadList = 0;

  TML_INT32 iRet = sidex_Variant_List_Size (m_payloadStreamList, &iSize);
  if (TML_SUCCESS == iRet && iSize){
    SIDEX_VARIANT varStreamPayload;
    iRet = sidex_Variant_List_Get(m_payloadStreamList, 0, &varStreamPayload);
    if (TML_SUCCESS == iRet){
      iRet = sidex_Variant_As_Integer (varStreamPayload, &iStreamPayloadList);
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_List_DeleteItem (m_payloadStreamList, 0);
      }
    }
    if (iRet == TML_SUCCESS && iStreamPayloadList){
      *data = (streamDataStructure*) iStreamPayloadList;
    }
  }
  *iRetSize = iSize;
  return (int)iRet;
}


/**
 * @brief    Return first queue entry / enters a critialsection obj
 */
int tmlMultiAsyncMsg::GetNextAsyncMsg(TML_COMMAND_HANDLE tmlhandle, TML_INT64* iState, TML_INT32* iRetSize){
  m_csObj->tmlCriticalSectionEnter("tmlMultiAsyncMsg::GetNextAsyncMsg");

  TML_INT32 iSize = -1;
  TML_INT64 iPayload = 0;

  TML_INT32 iRet = sidex_Variant_List_Size (m_payloadList, &iSize);
  if (TML_SUCCESS == iRet && iSize){
    SIDEX_VARIANT varPayload;
    iRet = sidex_Variant_List_Get(m_payloadList, 0, &varPayload);
    if (TML_SUCCESS == iRet){
      iRet = sidex_Variant_As_Integer (varPayload, &iPayload);
      if (TML_SUCCESS == iRet){
        iRet = sidex_Variant_List_DeleteItem (m_payloadList, 0);
      }
    }
  }

  if (iRet == TML_SUCCESS && iPayload){
    payloadDataStructure* data = (payloadDataStructure*) iPayload;
    SIDEX_HANDLE shandle;
    tml_Cmd_Acquire_Sidex_Handle (tmlhandle, &shandle);
    SIDEX_HANDLE sSidex;
    sSidex = (SIDEX_HANDLE) data->sHandle;

    char* sContent;
    TML_INT32 iContentLength;
    iRet = sidex_Get_Content (sSidex, &sContent, &iContentLength);
    if (TML_SUCCESS == iRet){
      iRet = sidex_Set_Content (shandle, sContent);
      if (TML_SUCCESS == iRet){
        iRet = sidex_Integer_Read (sSidex, TML_CMD_HEADER_GROUP, TML_CMD_HEADER_KEY_STATE, iState);
      }
      sidex_Free_Content(sContent);
      sidex_Free(&sSidex);
    }
    tml_Cmd_Release_Sidex_Handle(tmlhandle);
    delete data;
  }
  *iRetSize = iSize;
  return (int)iRet;
}


/**
 * @brief    Release critical section object
 */
void tmlMultiAsyncMsg::ReleaseNextAsyncMsg(){
  //printf ("ReleaseNextAsyncMsg\n");
  m_csObj->tmlCriticalSectionLeave("tmlMultiAsyncMsg::ReleaseNextAsyncMsg");
}

/**
 * @brief    Release critical section object
 */
void tmlMultiAsyncMsg::ReleaseNextAsyncMsgStream(){
  //printf ("ReleaseNextAsyncMsg\n");
  m_csObjStream->tmlCriticalSectionLeave("tmlMultiAsyncMsg::ReleaseNextAsyncMsgStream");
}

