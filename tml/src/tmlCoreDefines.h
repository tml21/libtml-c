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

#ifndef TMLCOREDEFINES_H
#define TMLCOREDEFINES_H

#include <vortex.h>
#include "tmlLogHandler.h"
#include "tmlEventData.h"
#include "tmlEventHandler.h"
#include "tmlCollectCallDestinationObj.h"
#include "tmlMultiAsyncMsg.h"
#include "tmlStdTypes.h"

#define INITIAL_THREAD_STACK_SIZE 32776

#define DEFAULT_PLAIN_PROFILE "http://wobe-team.com/profiles/plain_profile"
#define DEFAULT_LISTENER_HOST "0.0.0.0"
#define DEFAULT_LISTENER_PORT "44000"
#define DEFAULT_MSG_WINDOW_SIZE 0x8000
#define DEFAULT_CONNECTION_TIMEOUT 60000
#define DEFAULT_EVT_CONNECTION_TIMEOUT 6000
#define DEFAULT_MAX_CONNECTION_FAILS 1
#define DEFAULT_MAX_EVENT_MSG_QUEUE_LENGTH 1000
#define DEFAULT_LISTENER_IS_ENABLED false;


/**
* @brief    API version number
*/
#define API_VERSION_NUMBER 102

/**
* @brief    Library version number
*/
#define LIB_VERSION_NUMBER  10200

/**
* @brief    Library version string
*/
#define LIB_VERSION_STRING_A  (char*) "1.2.0"

/**
  * @brief    Default value for initial thread pool size
  */
#define INITIAL_THREAD_POOL_SIZE      10

/**
  * @brief    Default value for maximal thread pool size
  */
#define INITIAL_THREAD_POOL_MAX_SIZE  40


/**
  * @brief    Default value for thread pool add step
  */
#define INITIAL_THREAD_ADD_STEPS       1

/**
  * @brief    Default value for thread pool add period
  */
#define INITIAL_THREAD_POOL_ADD_PERIOD 0

/**
  * @brief    Default value for auto remove thread pool
  */
#define INITIAL_THREAD_AUTO_REMOVE     TML_TRUE

/**
  * @brief    Default value for thread pool remove step
  */
#define INITIAL_THREAD_REMOVE_STEPS       1

/**
  * @brief    Default value for thread pool remove period
  */
#define INITIAL_THREAD_POOL_REMOVE_PERIOD 1

/**
  * @brief    Default value for preemptive thread pool option
  */
#define INITIAL_THREAD_PREEMPTIVE     TML_TRUE

/////////////////////////////////////////////////////////////////////////////////////
// Thread status definitions
#define THREAD_STOPPED                      0
#define THREAD_IN_PROCESS                   1
#define THREAD_PENDING_TO_START             2


// Prototying:
class tmlConnectionObj;

///////////////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the data and destination for a load balanced activity state request:
struct ActivityStateRequestData {
       TML_COMMAND_HANDLE  activityState;            // TML_COMMAND_HANDLE containing the command and e BUSY STATE reply
       tmlCollectCallDestinationObj* destinationObj; // The destination referring the data
       TML_INT32 iIndex;                             // Internal index
       TML_INT32 iRequestRet;                        // Return value of the request
};

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for the tmlCoreSender's frame receive method:
struct VORTEXSenderFrameReceivedCallbackData{
  const char**       senderSyncEventArray;              // Synchronisation handle - set/wait for event in case of a fullfilled reply or a dropped connection
  tmlMultiAsyncMsg*  multiAsyncMsg;                     // Class to queue muli async message replies
  TML_INT32          iPayloadFrameSize;                 // actual received frame size
  char*              cPayload;                          // actual received frame payload
  int                iMsgNo;                            // received frame message number
  int                iChannelNo;                        // received frame channel no
  VortexFrameType    type;                              // received frame frame type
  VortexFrame*       pFrame;                            // the actual received frame
  TML_COMMAND_HANDLE tmlhandle;                         // An instance of TML_COMMAND_HANDLE I/O object)
  void*              callback;                          // reference to the cass callback method
  bool               bValidChain;                       // Flag to signal an initial receive data link list
  TML_INT64          totalFrameSize;                    // Amount of received data
  struct frameReceiveListElement* firstFrameListElement;// first element of  receive data link list
  struct frameReceiveListElement* lastFrameListElement; // last element of  receive data link list
  tmlLogHandler*     pLog;                              // reference to logging handler
  TML_INT64          cmdValue;                          // The Command identification
  TML_BOOL           bInternal;                         // Is it an internal Command      
  axl_bool           bMore;                             // Flag true, if there is more data pending
};

struct TMLThreadDef{
  volatile int iThreadStatus;
  VortexThread* pThreadDef;
};

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter to signal the close of a connection:
struct VORTEXSenderConnectionCloseCallbackData{
  const char**       senderSyncEventArray;              // Synchronisation handle - set/wait for event in case of a fullfilled reply or a dropped connection
  tmlLogHandler*     pLog;                              // reference to logging handler
};

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for the listener frame receive callback method:
struct VORTEXListenerFrameReceivedCallbackData{
  void*              callback;                          // reference to the cass callback method
  tmlLogHandler*     pLog;                              // reference to logging handler
  TML_CORE_HANDLE    tmlcorehandle;                     // reference to the core
};

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for the listener frame receive callback method:
struct VORTEXLimitCheckDataCallbackData{
  int                iMax;                              // The max Value for the limit check
  tmlLogHandler*     pLog;                              // reference to logging handler
};

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the message destination handling link list:
struct DestinationAddressHandlingListElement {
       DestinationAddressHandlingListElement* next;      // the next chain element
       DestinationAddressHandlingListElement* prev;      // the prev chain element
       tmlCollectCallDestinationObj* destinationObj;    // reference to the "tmlCollectCallDestinationObj"
};

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the connection link list:
struct VORTEXConnectionListElement {
       VORTEXConnectionListElement* next;               // the next chain element
       VORTEXConnectionListElement* prev;               // the prev chain element
       tmlConnectionObj* connectionObj;                 // reference to the "tmlConnectionObj"
};


/////////////////////////////////////////////////////////////////////////////////////
// Extended infos to handle event message return data
struct EventMessageReturnDataExtension {
       tmlConnectionObj* connectionObj;                 // the reference to a connectionObj that holds profile, host, port
       TML_COMMAND_HANDLE   tmlhandle;                  // The command content
       void* pCBFunc;                                   // the reference to the call callback method to handle the result
};


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the async command link list:
struct AsyncCMDListElement {
       AsyncCMDListElement* next;                       // the next chain element
       AsyncCMDListElement* prev;                       // the prev chain element
       TML_COMMAND_HANDLE   tmlhandle;                  // The command content
       int iTimeout;                                    // the command execution timeout
};


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for the tmlCoreSender's TimerThread:
struct TimerThreadData{
  const char**      timerSyncEventArray;                // Synchronisation handle - set/wait for event in case of a fullfilled reply or a dropped connection
  DWORD             dwTimeoutValue;                     // Timeout value in milliseconds
  tmlLogHandler*    pLog;                               // reference to logging handler
  tmlEventHandler*  eventHandler;
  VortexMutex*      terminationMutex;                   // mutex to signal termination
  TMLThreadDef*     threadInfo;
};


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for the tmlCoreSender's AsyncHandlingThreadMethod:
struct AsyncHandlingThreadData{
  const char**        senderSyncEventArray;             // Synchronisation handle - set/wait for event in case of a fullfilled reply or a dropped connection
  DWORD               dwTimeoutValue;                   // Timeout value in milliseconds
  TimerThreadData*    timerThreadData;                  // Datastructure contains the parameter for the tmlCoreSender's TimerThread:
  tmlLogHandler*      pLog;                             // reference to logging handler
  TML_COMMAND_HANDLE  tmlhandle;                        // The command content
  VortexMutex*        mutexCriticalSection;             // Mutex to save data 
  int                 iRet;                             // return value in case of sync massage commands
  void*               asyncCmdCallbackHandlerMethod;    // reference to the cass callback method
  tmlConnectionObj*   connectionObj;                    // reference to the instance of tmlConnectionObj
  tmlEventHandler*    eventHandler;
  tmlMultiAsyncMsg*   multiAsyncMsg;
  VortexMutex*        timerTerminationMutex;
  bool                bStopTimerThread;                 // Flag indicates to stop the timer thread
  TMLThreadDef*       threadInfo;                       // Threadinfo refering the thread to stop
};


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the data for a background event message:
struct EventMsgData{
  int                           iWindowSize;            // Window size for the message
  const char*                   profile;                // profile for the message
  unsigned int                  iTimeout;               // timeout for the message
  TML_COMMAND_ID_TYPE           iCmd;                   // The command code
  tmlEventData*                 cmdStrContent;          // TML command content    
};


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the data for a background event message:
struct EventMsgExecutionData{
  EventMsgData*                 msgData;
  tmlCollectCallDestinationObj* destinationObj;
};

// Prototype:
class tmlEventDataQueue;


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for the EventMessageHandlingThread:
struct EventMsgHandlingParams{
  const char**       handlingEventArray;                // Synchronisation handle continue / terminate
  tmlEventHandler*   eventHandler;
  VortexMutex*       terminationMutex;                  // mutex to signal termination
  tmlLogHandler*     pLog;
  void*              pCBFunc;                           // reference to the call callback method to handle the event message in background
  tmlEventDataQueue* pDataQueue;                        // reference to the queue containing the background event message data
  TMLThreadDef*      threadInfo;
};

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for the AsyncHandlingThread:
struct AsyncHandlingData{
  const char**       handlingEventArray;                // Synchronisation handle continue / terminate
  AsyncHandlingThreadData* payload;                     // Reference to the AsyncHandlingThreadData
  tmlEventHandler*    eventHandler;
  VortexMutex*        terminationMutex;                 // mutex to signal termination
  TMLThreadDef        threadInfo;
};

/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the method reference and the data for the command dispatcher action:
struct dispatcherCallbackData{
  TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc;                  // the reference to the dispatcher callback method
  TML_POINTER pCBData;                                  // the reference to the dispatcher callback method's data
};

/////////////////////////////////////////////////////////////////////////////////////
// Send Message synchronisation:
#define NO_OF_SEND_MSG_SYNC_HANDLES                      7
#define SEND_MSG_SYNC_HANDLES_FRAME_RECEIVE_INDEX        0
#define SEND_MSG_SYNC_HANDLES_ASYNC_FRAME_RECEIVE_INDEX  1
#define SEND_MSG_SYNC_HANDLES_STREAM_FRAME_RECEIVE_INDEX 2
#define SEND_MSG_SYNC_HANDLES_CONNECTION_CLOSE_INDEX     3
#define SEND_MSG_SYNC_HANDLES_TIMEOUT_INDEX              4
#define SEND_MSG_SYNC_HANDLES_STOP_TIMER_INDEX           5
#define SEND_MSG_SYNC_HANDLES_CONTINUE_INDEX             6

/////////////////////////////////////////////////////////////////////////////////////
// Send Message synchronisation:
#define NO_OF_ASYNC_HANDLING_HANDLES                     2
#define ASYNC_HANDLING_TERMINATE_EVENT                   0
#define ASYNC_HANDLING_CONTINUE_EVENT                    1


/////////////////////////////////////////////////////////////////////////////////////
// Send Message synchronisation / max timeout to wait for busy async command execution
#define MAX_WAIT_FOR_ASYNC_FINISH                       1000 // 1 Second
//#define MAX_WAIT_FOR_ASYNC_FINISH                       1200000 // 20 minutes

/**
* @brief    Coded string for copyright
*/
#define TML_STRING_COPYRIGHT_A    SIDEX_STRING_COPYRIGHT_A

#endif //TMLCOREDEFINES_H
