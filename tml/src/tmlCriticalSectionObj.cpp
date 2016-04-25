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
 
#ifdef LINUX
#include "stdio.h"
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
  axl_bool bSuccess = vortex_mutex_create_full (&m_csCriticalSection, VORTEX_MUTEX_CONF_RECURSIVE);
  m_bCreated = (axl_true == bSuccess);
}


/**
 * @brief    Destructor.
 */
tmlCriticalSectionObj::~tmlCriticalSectionObj()
{
  if (m_bCreated){
    vortex_mutex_destroy (&m_csCriticalSection);
  }
}


/**
 * @brief    Enter the critical section
 */
bool tmlCriticalSectionObj::tmlCriticalSectionEnter(const char* sID){
  if (m_bCreated){
    vortex_mutex_lock (&m_csCriticalSection);
  }
  return m_bCreated;
}


/**
 * @brief    Leave the critical section
 */
bool tmlCriticalSectionObj::tmlCriticalSectionLeave(const char* sID){
  if (m_bCreated){
    vortex_mutex_unlock (&m_csCriticalSection);
  }
  return m_bCreated;
}

