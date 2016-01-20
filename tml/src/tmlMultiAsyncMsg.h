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

#ifndef TMLMULTIASYNCMSG_H
#define TMLMULTIASYNCMSG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tmlCriticalSectionObj.h"
#include "tmlLogHandler.h"
#include <vortex.h>


struct streamDataStructure{
  SIDEX_HANDLE sHandle;
  TML_INT32         iPayloadFrameSize;
  char*             cPayload;
  TML_INT64         totalFrameSize;
  VortexFrame*      pFrame;
  TML_INT64         cmd;
  tmlLogHandler*    log;
};

struct payloadDataStructure{
  SIDEX_HANDLE sHandle;
  int iPayloadFrameSize;
  char* cPayload;
};

class  tmlMultiAsyncMsg
{

private:
	  /* data */

    /**
     * @brief    The critical section attribute
     */
   tmlCriticalSectionObj *m_csObj;
   tmlCriticalSectionObj *m_csObjStream;

   SIDEX_VARIANT m_payloadList;
   SIDEX_VARIANT m_payloadStreamList;

	  /* methods */

public:
	  /* data */

	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlCoreWrapper
     */
    tmlMultiAsyncMsg();


    /**
     * @brief    Destructor.
     */
    ~tmlMultiAsyncMsg();


    /**
     * @brief    Get a SIDEX_HANDLE out of the payload
     */
    int getSidexFromPayoad(SIDEX_HANDLE* retHandle, int iPayloadFrameSize, char* cPayload);
 
    
    /**
     * @brief    Clear the stream list
     */
     int clearStreamList(bool bFreeFrames);


    /**
     * @brief    Get a command and stream identification out of the callback data
     */
    int getCmdFromCallbackData(void* cbData, bool* bStream, TML_INT64* cmdValue);


    /**
     * @brief    Put payload data into the queue
     *
     * @param   data      Reference to a payloadDataStructure structure.
     *
     * @returns TML_SUCCESS in case of success
     */
    int DataIntoQueue(payloadDataStructure* data);


    /**
     * @brief    Put stream data into the queue
     *
     * @param   data      Reference to a streamDataStructure structure.
     *
     * @returns TML_SUCCESS in case of success
     */
    int DataIntoStreamQueue(streamDataStructure* data);


    /**
      * @brief    Return first stream queue entry / enters a critialsection obj
      */
    int GetNextAsyncStreamMsg(streamDataStructure** data, TML_INT32* iRetSize);


    /**
      * @brief    Return first message queue entry / enters a critialsection obj
      */
    int GetNextAsyncMsg(TML_COMMAND_HANDLE tmlhandle, TML_INT64* iState, TML_INT32* iRetSize);


    /**
     * @brief    Release critical section object
     */
    void ReleaseNextAsyncMsg();


    /**
     * @brief    Release critical section object
     */
    void ReleaseNextAsyncMsgStream();
};


#endif //TMLMULTIASYNCMSG_H

