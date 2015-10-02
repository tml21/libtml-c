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
 
#ifdef LINUX
#include "stdio.h"
#include "pthread.h"
#include "semaphore.h"
#ifdef OS_X
#include <sys/errno.h>
#else // OS_X
#ifdef FREE_BSD
#include <sys/errno.h>
#else // FREE_BSD
#include <asm-generic/errno-base.h>
#endif // FREE_BSD
#endif // OS_X
#include <unistd.h>
#else // LINUX
#endif // LINUX
#include "tmlCriticalSectionObj.h"

////////////////////////////////////
// Enable logging for debug:
// #define ENABLE_LOG 1

/**
 * @brief    Constructor.
 */
tmlCriticalSectionObj::tmlCriticalSectionObj()
{
#ifdef LINUX
   pthread_mutexattr_t mutexattr;   // Mutex attribute variable
   int irc;
  // Set the mutex as a recursive mutex
 irc = pthread_mutexattr_init(&mutexattr);
 if (0 != irc){
   printf ("tmlCriticalSectionObj Error on pthread_mutexattr_init %d\n", irc);
 }
 else{
    irc = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
    if (0 != irc){
      printf ("tmlCriticalSectionObj Error on pthread_mutexattr_settype %d\n", irc);
    }
    else{
      // create the mutex with the attributes set
      irc = pthread_mutex_init(&mutex, &mutexattr);
      if (0 != irc){
        printf ("tmlCriticalSectionObj Error on init mutex %d\n", irc);
      }
      //After initializing the mutex, the thread attribute can be destroyed
      irc = pthread_mutexattr_destroy(&mutexattr);
      if (0 != irc){
        printf ("tmlCriticalSectionObj Error on destroy mutex attr %d\n", irc);
      }
    }
  }
#else // LINUX
  axl_bool bSuccess = vortex_mutex_create (&m_csCriticalSection);
  m_bCreated = (axl_true == bSuccess);
#endif // LINUX  axl_bool bSuccess = axl_true;
}


/**
 * @brief    Destructor.
 */
tmlCriticalSectionObj::~tmlCriticalSectionObj()
{
#ifdef LINUX
  // Destroy / close the mutex
   int irc = pthread_mutex_destroy (&mutex);
   if (0 != irc){
     printf ("~tmlCriticalSectionObj Error on destroy mutex %d\n", irc);
   }
#else // LINUX
  if (m_bCreated){
    vortex_mutex_destroy (&m_csCriticalSection);
  }
#endif // LINUX  
}


/**
 * @brief    Enter the critical section
 */
bool tmlCriticalSectionObj::tmlCriticalSectionEnter(const char* sID){
#ifdef LINUX
  bool bOk = true;
#ifdef ENABLE_LOG
  pid_t tId = ::pthread_self();
  printf ("%u ENTER %s\n", tId, sID);
#endif //  ENABLE_LOG
  // Acquire the mutex to access the shared resource

  int irc = pthread_mutex_lock (&mutex);
  if (0 != irc){
    printf ("tmlCriticalSectionEnter Error on acquire mutex %d\n", irc);
  }
  return bOk;
#else // LINUX
  if (m_bCreated){
    vortex_mutex_lock (&m_csCriticalSection);
  }
  return m_bCreated;
#endif // LINUX  
}


/**
 * @brief    Leave the critical section
 */
bool tmlCriticalSectionObj::tmlCriticalSectionLeave(const char* sID){
#ifdef LINUX
  bool bOk = true;
#ifdef ENABLE_LOG
  pid_t tId = ::pthread_self();
  printf ("%u LEAVE %s\n", tId, sID);
#endif //  ENABLE_LOG
   // Release the mutex  and release the access to shared resource

  int irc = pthread_mutex_unlock (&mutex);
  if (0 != irc){
    if (EPERM == irc)
      printf ("tmlCriticalSectionLeave / current thread  did not acquire the critical section\n");
    else
      printf ("tmlCriticalSectionLeave Error on leave mutex %d\n", irc);
  }
  return bOk;
#else // LINUX
  if (m_bCreated){
    vortex_mutex_unlock (&m_csCriticalSection);
  }
  return m_bCreated;
#endif // LINUX  
}

