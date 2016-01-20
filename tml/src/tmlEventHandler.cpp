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

#include "tmlErrors.h"
#include "tmlEventHandler.h"
#ifdef LINUX
#include <time.h>
#include <errno.h>
#endif // LINUX

//////////////////////////////////////////////////////
// On LINUX there are two different kind of sempahore
// POSIX- semaphore and SYSTEM V- semaphore
// The SYSTEM V- semaphore are system wide.
// They can be seen by different processes - in case of
// a process crash the will not be destroyed !
// For our use LINUX- semaphore are the better choice:
#define USE_POSIX 1

#ifdef LINUX
  #ifndef ANDROID
    #include <sys/sem.h>
  #endif // ANDROID
  #ifdef OS_X
    #include <mach/mach.h>
    #include <mach/semaphore.h>
    #include <mach/task.h>
    #include <mach/clock_types.h>
  #else // OS_X
    #include <semaphore.h>
  #endif // OS_X
#endif // LINUX


/**
 * @brief    Constructor.
 */
tmlEventHandler::tmlEventHandler(tmlLogHandler* log){
  m_log = log;
  m_hashTable = new TMLUniversalHashTable(log);
  m_hashTable->createHashTable(true);
#ifdef LINUX
  m_semid = -1;
  m_iSemIdx = 0;
#ifndef USE_POSIX
  //Definition of variables
  m_semid = semget(IPC_PRIVATE,   // create a new in every case
                   10,                // number of semaphore in the semaphore set
                   0666 | IPC_CREAT); // permissions (rwxrwxrwx) on the new semaphore set and creation flag
                                      //    semaphore set and creation flag
  if (-1 == m_semid){
    printf ("Error occurred on \"semget\": %d\n", errno);
  }
#endif // USE_POSIX
#endif // LINUX
}


/**
 * @brief    Destructor.
 */
tmlEventHandler::~tmlEventHandler(){
#ifdef LINUX
#ifndef USE_POSIX
  if (-1 != m_semid){
    union semun
    {
      int val;
      struct semid_ds *buf;
      unsigned short *array;
    } ignored_argument;
    // Don't use the return value:
    /*int iRet = */ semctl(m_semid, 0, IPC_RMID, ignored_argument);
  }
#endif // USE_POSIX
#endif // LINUX
  delete m_hashTable;
}


/**
 * @brief    Create a HANDLE to be able to signal a condition
 */
bool tmlEventHandler::CreateEventHandle(const char* cIdentification)
{
    bool bOk = false;
#ifdef LINUX
#ifndef USE_POSIX
    void* pHandle;
    if (-1 != m_semid){
      union semun
      {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
      } semctl_arg;

      //Set Initial value for the resource
      semctl_arg.val = 0; //Setting semval to 0
      int iRet = semctl(m_semid, m_iSemIdx, SETVAL, semctl_arg);
      if (-1 != iRet){
        pHandle = new int(1);
        *((int*)pHandle) = m_iSemIdx++;
        m_hashTable->setValue((char*)cIdentification, pHandle);
        bOk = true;
      }
      else{
        printf ("Error occurred on \"semctl\": %d\n", errno);
      }
    }
#else //USE_POSIX
    int   retCode;
    // Initialize event semaphore
#ifdef OS_X
    semaphore_t* sem = new semaphore_t;
    retCode = semaphore_create(mach_task_self(),   // task to perform operation
                               (semaphore_t *)sem, // handle to the event semaphore
                               SYNC_POLICY_FIFO,   // policy
                               0);                 // initially set to non signaled state
#else // OS_X
    sem_t* sem = new sem_t;
    retCode = sem_init(sem,   // handle to the event semaphore
                       0,     // not shared
                       0);    // initially set to non signaled state
#endif // OS_X
    if (-1 != retCode){
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Pointer to HANDLE into the hashtable because of automatic deallocation in the hash table's destructor:
      void* pHandle;
      pHandle = new TML_INT64(1);
      *((TML_INT64*)pHandle) = (TML_INT64)sem;
      m_hashTable->setValue((char*)cIdentification, pHandle);
      bOk = true;
    }
#endif //USE_POSIX
#else // LINUX
    HANDLE handle = NULL;
    if (handle = CreateEvent(NULL,FALSE,FALSE,NULL)){
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Pointer to HANDLE into the hashtable because of automatic deallocation in the hash table's destructor:
      HANDLE* pHandle = new HANDLE();
      *pHandle = handle;
      m_hashTable->setValue((char*)cIdentification, pHandle);
      bOk = true;
    }
#endif // LINUX
    return bOk;
}


/**
 * @brief    Remove the HANDLE entry that has been created to be able to signal a condition
 */
bool tmlEventHandler::RemoveEventHandle(const char* cIdentification)
{
  bool bOk = false;

  void* pHandle;
  if (TML_SUCCESS == m_hashTable->getValue((char*)cIdentification, &pHandle))
  {
#ifdef LINUX // LINUX
#ifndef USE_POSIX
    bOk = true;
#else // USE_POSIX
    int   retCode = 0;
    TML_INT64* iVal = (TML_INT64*)pHandle;
    // now destroy the event semaphore
    if (NULL != iVal){
#ifdef OS_X
      semaphore_t* sem = (semaphore_t*)*iVal;
      retCode = semaphore_destroy(mach_task_self(), // task to perform operation
                                *sem);            // Event semaphore handle
#else // OS_X
      sem_t* sem = (sem_t*)*iVal;
      retCode = sem_destroy(sem);// Event semaphore handle
#endif // OS_X
      if (-1 != retCode){
        bOk = true;
      }
      delete sem;
    }
#endif // USE_POSIX
    m_hashTable->removeEntry((char*)cIdentification);
    ////////////////////////////////////////
    // Delete the value stored in the hash:
    ////////////////////////////////////////
    if (NULL != pHandle){
#ifndef USE_POSIX
      delete ((int*)pHandle);
#else //USE_POSIX
      delete ((TML_INT64*)pHandle);
#endif // USE_POSIX
    }
#else // LINUX
    if (NULL != pHandle){
      CloseHandle(*((HANDLE*)pHandle));
    }
    bOk = true;
    m_hashTable->removeEntry((char*)cIdentification);
    ////////////////////////////////////////
    // Delete the value stored in the hash:
    if (NULL != pHandle){
      delete ((HANDLE*)pHandle);
    }
#endif // LINUX
  }
  return bOk;
}


/**
 * @brief    Set an event to a handle
 */
bool tmlEventHandler::SetEventOnHandle(const char* cIdentification)
{
  bool bOk = false;

  void* pHandle;
  if (TML_SUCCESS == m_hashTable->getValue((char*)cIdentification, &pHandle))
  {
#ifdef LINUX // LINUX
#ifndef USE_POSIX
    int index = *((int*)pHandle);
    if (-1 != m_semid){
      struct sembuf operation[1];
      operation[0].sem_op = 1; // Release the resource so Wait in process 1 will
      operation[0].sem_num = index;
      operation[0].sem_flg = 0;
      //Release semaphore
      int iRet = semop(m_semid, operation, 1);
      if (-1 != iRet){
        bOk = true;
      }
    }
#else // USE_POSIX
    int   retCode;
    TML_INT64* iVal = (TML_INT64*)pHandle;
#ifdef OS_X
    semaphore_t* sem = (semaphore_t*)*iVal;
    retCode =  semaphore_signal(*sem);// Event semaphore Handle
#else // OS_X
    sem_t* sem = (sem_t*)*iVal;
    retCode = sem_post(sem);          // Event semaphore Handle
#endif // OS_X
    if (-1 != retCode){
       bOk = true;
    }
#endif // USE_POSIX
#else // LINUX
    if (! SetEvent(*((HANDLE*)pHandle))){
      if (NULL != m_log) m_log->log ("tmlEventHandler", "SetEventOnHandle", "SetEvent", "failed");
    }
    else{
      bOk = true;
    }
#endif // LINUX
  }
  return bOk;
}


/**
 * @brief    Reset an event to a handle
 */
bool tmlEventHandler::ResetEventOnHandle(const char* cIdentification)
{
  bool bOk = false;

  void* pHandle;
  if (TML_SUCCESS == m_hashTable->getValue((char*)cIdentification, &pHandle))
  {
#ifdef LINUX // LINUX
#ifndef USE_POSIX
    int index = *((int*)pHandle);
    if (-1 != m_semid){
      struct sembuf operation[1];

      operation[0].sem_op = -1; // Wait until the semaphore count becomes 0
      operation[0].sem_num = index;
      operation[0].sem_flg = IPC_NOWAIT;

      bool bTerminate = false;
      do{
        //////////////////////////////////////////////////////////////
        // Check the Semaphore:
        int semRet = semop(m_semid, operation, 1);
        if(semRet < 0)
        {
          // semaphore got triggered
          bTerminate = true;
        }
      }
      while (!bTerminate);
      bOk = true;
    }
#else // USE_POSIX
    int   retCode;
    TML_INT64* iVal = (TML_INT64*)pHandle;

    //////////////////////////////////////////////////////////////
    // Check the Semaphore:
#ifdef OS_X
    semaphore_t* sem = (semaphore_t*)*iVal;
    mach_timespec_t t;
    t.tv_nsec = 0;
    t.tv_sec = 0;
    retCode = semaphore_timedwait(*sem,// event semaphore handle
                                  t);  // timeout
#else // OS_X
    sem_t* sem = (sem_t*)*iVal;
    // non blocking call;
    
    retCode = sem_trywait(sem); // event semaphore handle
#endif // OS_X
    bOk = true;
#endif // USE_POSIX
#else // LINUX
    if (! ResetEvent(*((HANDLE*)pHandle))){
      if (NULL != m_log) m_log->log ("tmlEventHandler", "SetEventOnHandle", "ResetEvent", "failed");
    }
    else{
      bOk = true;
    }
#endif // LINUX
  }
  return bOk;
}


/**
 * @brief    Wait until a single object was signaled
*/
int tmlEventHandler::WaitForSingleEvent(const char* cIdentification, DWORD timeout){
  int iRet = WAIT_ERR;

  void* pHandle;
  if (TML_SUCCESS == m_hashTable->getValue((char*)cIdentification, &pHandle))
  {
#ifdef LINUX // LINUX
#ifndef USE_POSIX
    int index = *((int*)pHandle);
    if (-1 != m_semid){
      struct sembuf operation[1];

      operation[0].sem_op = -1; // Wait until the semaphore count becomes 0
      operation[0].sem_num = index;
      operation[0].sem_flg = IPC_NOWAIT;

      bool bTerminate = false;
      do{
        //////////////////////////////////////////////////////////////
        // Check the Semaphore:
        int semRet = semop(m_semid, operation, 1);
        if(semRet >= 0)
        {
          // semaphore got triggered
          iRet = WAIT_SUCCEEDED;
          bTerminate = true;
        }
        else{
          // Is there a possible timeout condition ?
          if (sleepForOneMilliSecondAndCheckTimeout(&timeout)){
            bTerminate = true;
            iRet = WAIT_TOUT;
          }
        }
      }
      while (!bTerminate);
    }
#else // USE_POSIX
    bool bTerminate = false;
     do{
       int   retCode;
       TML_INT64* iVal = (TML_INT64*)pHandle;

       //////////////////////////////////////////////////////////////
       // Check the Semaphore:
#ifdef OS_X
       semaphore_t* sem = (semaphore_t*)*iVal;
       mach_timespec_t t;
       t.tv_nsec = 0;
       t.tv_sec = 0;
       retCode = semaphore_timedwait(*sem,// event semaphore handle
                                      t);  // timeout
#else // OS_X
       sem_t* sem = (sem_t*)*iVal;
       // non blocking call;
       retCode = sem_trywait(sem); // event semaphore handle
#endif // OS_X
       if (!retCode)  {
         // semaphore got triggered
         iRet = WAIT_SUCCEEDED;
         bTerminate = true;
       }
       else {
         if (EPERM == retCode){
           /* check whether somebody else has the mutex */
           // Is there a possible timeout condition ?
           if (sleepForOneMilliSecondAndCheckTimeout(&timeout)){
             iRet = WAIT_TOUT;
             bTerminate = true;
           }
         }
         else{
           ///////////////////////////
           // Other kind of error:
           iRet = WAIT_ERR;
           bTerminate = true;
         }
       }
     }
     while (!bTerminate);
#endif // USE_POSIX
#else // LINUX
    DWORD dwResult = WaitForSingleObject(*((HANDLE*)pHandle), timeout);
    if (WAIT_OBJECT_0 == dwResult){
      iRet = WAIT_SUCCEEDED;
    }
    else{
      iRet = WAIT_TOUT;
    }
#endif // LINUX
  }
  return iRet;
}


/**
 * @brief    Wait until one of a couple of objects has been signaled
 */
int tmlEventHandler::WaitForMultipleEvent(const char** cIdentifications, DWORD iCount, DWORD timeout, DWORD* iIndex){
  int iRet = WAIT_SUCCEEDED;

#ifdef LINUX // LINUX
#ifndef USE_POSIX
  struct sembuf operation[1];

  operation[0].sem_op = -1; // Wait until the semaphore count becomes 0
  operation[0].sem_flg = IPC_NOWAIT;

  bool bTerminate = false;
  do{
    /////////////////////////////////////////////////////////////////////////////
    // Loop all semaphore's:
    for (DWORD i = 0; i < iCount && iRet == WAIT_SUCCEEDED && !bTerminate; ++i){
      void* pHandle = NULL;
      if (TML_SUCCESS == m_hashTable->getValue((char*)cIdentifications[i], &pHandle)){
        if (NULL != pHandle){
          int index = *((int*)pHandle);
          operation[0].sem_num = index;
          if (-1 != m_semid){
            int semRet = semop(m_semid, operation, 1);
            if(semRet >= 0)
            {
              // semaphore got triggered
              iRet = WAIT_SUCCEEDED;
              bTerminate = true;
              *iIndex = i;
            }
          }
          else{
            iRet = WAIT_ERR;
          }
        }
        else{
          iRet = WAIT_ERR;
        }
      }
      else{
        iRet = WAIT_ERR;
      }
    }

    if  (iRet == WAIT_SUCCEEDED && !bTerminate){
      // Is there a possible timeout condition ?
      if (sleepForOneMilliSecondAndCheckTimeout(&timeout)){
        bTerminate = true;
        iRet = WAIT_TOUT;
      }
    }
  }
  while (iRet == WAIT_SUCCEEDED && !bTerminate);
#else // USE_POSIX
  bool bTerminate = false;
  do{
    /////////////////////////////////////////////////////////////////////////////
    // Loop all semaphore's:
    for (DWORD i = 0; i < iCount && iRet == WAIT_SUCCEEDED && !bTerminate; ++i){
      void* pHandle;
      if (TML_SUCCESS == m_hashTable->getValue((char*)cIdentifications[i], &pHandle)){
        if (NULL != pHandle){
          int retCode;
          TML_INT64* iVal = (TML_INT64*)pHandle;

          //////////////////////////////////////////////////////////////
          // Check the Semaphore:
  #ifdef OS_X
          semaphore_t* sem = (semaphore_t*)*iVal;
          mach_timespec_t t;
          t.tv_nsec = 0;
          t.tv_sec = 0;
          retCode = semaphore_timedwait(*sem,// event semaphore handle
                                        t);  // timeout
  #else // OS_X
          sem_t* sem = (sem_t*)*iVal;
          // non blocking call;
          retCode = sem_trywait(sem); // event semaphore handle
  #endif // OS_X
          if (!retCode)  {
            // semaphore got triggered
            iRet = WAIT_SUCCEEDED;
            bTerminate = true;
            *iIndex = i;
          }
        }
      }
      else{
        iRet = WAIT_ERR;
      }
    }

    if  (iRet == WAIT_SUCCEEDED && !bTerminate){
      // Is there a possible timeout condition ?
      if (sleepForOneMilliSecondAndCheckTimeout(&timeout)){
        bTerminate = true;
        iRet = WAIT_TOUT;
      }
    }
  }
  while (iRet == WAIT_SUCCEEDED && !bTerminate);
#endif // USE_POSIX
#else // LINUX
  ///////////////////////////////////////////////////////////////////
  // The array of Handles to listen to:
  HANDLE* handleArr = new HANDLE[iCount];
  for (DWORD i = 0; i < iCount && WAIT_SUCCEEDED == iRet; ++i){
    void* pHandle;
    if (TML_SUCCESS == m_hashTable->getValue((char*)cIdentifications[i], &pHandle)){
      handleArr[i] = *((HANDLE*)pHandle);
    }
    else{
      iRet = WAIT_ERR;
    }
  }
  if (iRet == WAIT_SUCCEEDED)
  {
    DWORD dwWait = WaitForMultipleObjects(iCount, handleArr, FALSE, timeout);
    DWORD Nr = dwWait - WAIT_OBJECT_0;
    if (0 <= Nr && iCount > Nr){
      iRet = WAIT_SUCCEEDED;
      *iIndex = Nr;
    }
    else{
      if (WAIT_TIMEOUT == Nr){
        iRet = WAIT_TOUT;
      }
      else{
        iRet = WAIT_ERR;
      }
    }
  }
  delete[] handleArr;
#endif // LINUX
  return iRet;
}


#ifdef LINUX // LINUX
/**
 * @brief  sleep for one millisecond and check for timeout condition
*/
bool tmlEventHandler::sleepForOneMilliSecondAndCheckTimeout(DWORD* pTimeout){
  DWORD timeout = *pTimeout;
  bool bTimeOut = false;

  ///////////////////////////////////////////////////////////////////////////
  // Delay for one millisecond:
  timespec delay;
  delay.tv_sec = 0;
  delay.tv_nsec = 1000000;  // 1 milli sec
  // sleep for delay time
  nanosleep(&delay, NULL);
  if (timeout != INFINITE){
    --timeout;
    if (0 == timeout){
      bTimeOut = true;
    }
    else{
      *pTimeout = timeout;
    }
  }
  return bTimeOut;
}
#endif // LINUX


