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
  return vortex_mutex_create_full (mutex_def, VORTEX_MUTEX_CONF_RECURSIVE);
}

axl_bool intern_mutex_destroy(VortexMutex* mutex_def, char* source){
  return vortex_mutex_destroy (mutex_def);
}

void intern_mutex_lock(VortexMutex* mutex_def, tmlLogHandler* log, const char* identifier){
  vortex_mutex_lock (mutex_def);
}


void intern_mutex_unlock(VortexMutex* mutex_def, tmlLogHandler* log, const char* identifier){
  vortex_mutex_unlock (mutex_def);
}

/**
 * @brief  helper method / sleep for millisecond
*/
void SleepForMilliSeconds(long mSecs){
#ifdef LINUX // LINUX
  ///////////////////////////////////////////////////////////////////////////
  // Delay for one millisecond:
  timespec delay;
  delay.tv_sec = 0;
  delay.tv_nsec = 1000000 * mSecs;  // 1 milli sec * mSecs
  // sleep for delay time
  nanosleep(&delay, NULL);
  return;
#else // LINUX
  Sleep (mSecs);
#endif // LINUX
}
