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

#ifndef TMLCONNECTIONMANAGEOBJBASE_H
#define TMLCONNECTIONMANAGEOBJBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vortex.h>
#include "tmlStdTypes.h"

class tmlConnectionManageObjBase //connection management object class
{
private:
    /* data */

protected: 
	  /* data */
    /**
     * @brief    TML core handle
     */
    TML_CORE_HANDLE   m_coreHandle;

    /**
     * @brief    reference VortexConnection object
     */
    VortexConnection* m_vortexConnection;


public:
	  /* methods */

    /**
     * @brief    Constructor.
     *
     * @returns an instance of tmlConnectionManageObjBase.
     */
    tmlConnectionManageObjBase();


    /**
     * @brief    Destructor.
     */
    virtual ~tmlConnectionManageObjBase();


    /**
     * @brief Get the TML core handle.
     */
    TML_CORE_HANDLE getCoreHandle();


    /**
     * @brief   Get Vortex connection 
     *
     * @returns the Vortex connection / NULL if invalid
     */
    VortexConnection* getVortexConnection();


    /**
     * @brief   Set Vortex connection 
     *
     * @param   connection Reference to VortexConnection / NULL if invalid
     */
    void setVortexConnection(VortexConnection* connection);
};

#endif // TMLCONNECTIONMANAGEOBJ_H
