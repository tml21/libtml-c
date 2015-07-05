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

#ifndef TMLCRITICALSECTIONOBJ_H
#define TMLCRITICALSECTIONOBJ_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef LINUX
#include <pthread.h>
#endif // LINUX
#include <vortex.h>

class  tmlCriticalSectionObj
{

private:
	  /* data */

    /**
     * @brief    The critical section attribute
     */
#ifdef LINUX
    pthread_mutex_t mutex;           // Mutex
#else // LINUX
    VortexMutex m_csCriticalSection;

    /**
     * @brief    The created attribute
     */
    bool              m_bCreated;
#endif // LINUX

	  /* methods */

public:
	  /* data */

	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlCoreWrapper
     */
    tmlCriticalSectionObj();


    /**
     * @brief    Destructor.
     */
    ~tmlCriticalSectionObj();


    /**
     * @brief    Enter the critical section
     */
    bool tmlCriticalSectionEnter(const char* sID);


    /**
     * @brief    Leave the critical section
     */
    bool tmlCriticalSectionLeave(const char* sID);
};


#endif //TMLCRITICALSECTIONOBJ_H

