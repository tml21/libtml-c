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

#ifndef TMLCOREWRAPPERBASE_H
#define TMLCOREWRAPPERBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vortex.h>

class  tmlCoreWrapperBase
{

private:
    /* data */


protected: 
    /* data */
    /**
     * @brief    reference to vortex execution context
     */
    VortexCtx* m_ctx;

    void* m_pTLS_FailureCB;
    void* m_pTLS_FailureData;
    void* m_pTLS_AcceptCB;
    void* m_pTLS_CertReqCB;
    void* m_pTLS_PrivateKeyReqCB;

public:
    /* data */
    /* methods */

    tmlCoreWrapperBase();


    /**
     * @brief    Destructor.
     */
    virtual ~tmlCoreWrapperBase();


    /**
     * @brief    Get the Vortex execution context
     *
     * @returns the Vortex execution context
     */
    VortexCtx* getVortexCtx();


    /**
      * @brief   Set the TLS failure callback method
      *
      * @param pAcceptCB    reference to the callback method
      * @param pFailureData reference to optional user pointer to be passed into the function when the handler is called. 
      */
    void setTlsFailureCB(void* pFailureCB, void* pFailureData);


    /**
      * @brief   Get the TLS failure callback method and user data
      *
      * @param pTLS_FailureCB   returns the reference to the callback method
      * @param pTLS_FailureData returns the reference to optional user pointer to be passed into the function when the handler is called. 
      */
    void getTlsFailureCB(void** pTLS_FailureCB, void** pTLS_FailureData);


    /**
      * @brief   Set the TLS accept callback method
      *
      * @param pAcceptCB reference to the callback method
      */
    void setTlsAcceptCB(void* pAcceptCB);


    /**
      * @brief   Get the TLS accept callback method
      *
      * @returns reference to the callback method
      */
    void* getTlsAcceptCB();


    /**
      * @brief   Set the TLS certifiction file request callback method
      *
      * @param pCertReqCB reference to the callback method
      */
    void setTlsCertReqCB(void* pCertReqCB);


    /**
      * @brief   Get the TLS certifiction file request callback method
      *
      * @returns reference to the callback method
      */
    void* getTlsCertReqCB();


    /**
      * @brief   Set the TLS private key file request callback method
      *
      * @param pPrivateKeyReqCB reference to the callback method
      */
    void setTlsPrivateKeyReqCB(void* pPrivateKeyReqCB);

    /**
      * @brief   Get the TLS private key file request callback method
      *
      * @returns reference to the callback method
      */
    void* getTlsPrivateKeyReqCB();
};
#endif  // TMLCOREWRAPPERBASE_H
