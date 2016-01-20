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
 
#include "systemBase.h"

axl_bool intern_mutex_create(VortexMutex* mutex_def){
#ifdef LINUX
   pthread_mutexattr_t mutexattr;   // Mutex attribute variable
   int irc;
  // Set the mutex as a recursive mutex
 irc = pthread_mutexattr_init(&mutexattr);
 if (0 != irc){
   printf ("tmlCriticalSectionObj Error on pthread_mutexattr_init %d\n", irc);
   return axl_false;
 }
 else{
    irc = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
    if (0 != irc){
      printf ("tmlCriticalSectionObj Error on pthread_mutexattr_settype %d\n", irc);
      return axl_false;
    }
    else{
      // create the mutex with the attributes set
      irc = pthread_mutex_init(mutex_def, &mutexattr);
      if (0 != irc){
        printf ("tmlCriticalSectionObj Error on init mutex %d\n", irc);
        return axl_false;
      }
      //After initializing the mutex, the thread attribute can be destroyed
      irc = pthread_mutexattr_destroy(&mutexattr);
      if (0 != irc){
        printf ("tmlCriticalSectionObj Error on destroy mutex attr %d\n", irc);
      }
      return axl_true;
    }
  }
#else // LINUX
  return vortex_mutex_create (mutex_def);
#endif // LINUX
}

axl_bool intern_mutex_destroy(VortexMutex* mutex_def, char* source){
#ifdef LINUX
  // Destroy / close the mutex
   int irc = pthread_mutex_destroy (mutex_def);
   if (0 != irc){
     printf ("intern_mutex_destroy Error on destroy mutex %d - %s\n", irc, source);
     return axl_false;
   }
   else{
     return axl_true;
   }
#else // LINUX
  return vortex_mutex_destroy (mutex_def);
#endif // LINUX  
}

void intern_mutex_lock(VortexMutex* mutex_def, tmlLogHandler* log, const char* identifier){

#ifdef LINUX
  // Acquire the mutex to access the shared resource

  int irc = pthread_mutex_lock (mutex_def);
  if (0 != irc){
    printf ("intern_mutex_lock Error on acquire mutex %d\n", irc);
  }
#else // LINUX
  vortex_mutex_lock (mutex_def);
#endif // LINUX  
}


void intern_mutex_unlock(VortexMutex* mutex_def, tmlLogHandler* log, const char* identifier){

#ifdef LINUX
   // Release the mutex  and release the access to shared resource

  int irc = pthread_mutex_unlock (mutex_def);
  if (0 != irc){
    if (EPERM == irc)
      printf ("intern_mutex_unlock / current thread  did not acquire the critical section\n");
    else
      printf ("intern_mutex_unlock Error on leave mutex %d\n", irc);
  }
#endif // LINUX
  vortex_mutex_unlock (mutex_def);
}
