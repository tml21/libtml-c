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

#include <stdio.h>
#include <string.h>
#include "sidex.h"
#include "tmlNetBinding.h"

/**
 * @brief    Constructor.
 *
 */
tmlNetBinding::tmlNetBinding(const char* sNetAddress)
{
  m_iBindings = 0;

  int iErr = 0;
  SIDEX_INT32 sRet;
  SIDEX_INT32 iPos;

  int sAddressLen = strlen(sNetAddress);
  m_hosts = sidex_Variant_New_List();
  m_ports = sidex_Variant_New_List();
  m_isIPV6 = sidex_Variant_New_List();
  bool bContinue = true;

  int iActIndex = 0;
  do{
    int ipStart = iActIndex;
    int ipEnd = iActIndex;
    char* sHost;

    if (sAddressLen <= ipEnd){
     iErr = 1; // Unexpected length
    }
    else{
      if ('[' == sNetAddress[iActIndex]){
        //////////
        // IPV6

        iErr = addIPAttrItem(m_iBindings, SIDEX_TRUE);

        ++ipStart;
        if (0 == iErr){
          do{
            if (sAddressLen <= ++ipEnd){
              iErr = 1; // Unexpected length
            }
          }while ((0 == iErr) && (']' != sNetAddress[ipEnd]));
        }
        if (0 == iErr){
          int iLength = ipEnd - ipStart;
          if (iLength > 1){
            sHost = new char [(ipEnd - ipStart) +1];
            strncpy(sHost, &(sNetAddress[ipStart]), iLength);
            sHost[iLength] = '\0';

            iErr = addHostItem(m_iBindings, sHost);

            delete[] sHost;
            if (sAddressLen <= ++ipEnd){
              iErr = 1; // Unexpected length
            }
          }
          else{
            iErr = 2; // No IPV6 content
          }
        }
      }
      else{
        //////////
        // IPV4

        iErr = addIPAttrItem(m_iBindings, SIDEX_FALSE);

        do{
          if (sAddressLen <= ++ipEnd){
            iErr = 1; // Unexpected length
          }
        }while ((0 == iErr) && (':' != sNetAddress[ipEnd]));
        if (0 == iErr){
          int iLength = ipEnd - ipStart;
          if (iLength >= 1){
            sHost = new char [(ipEnd - ipStart)+1];
            strncpy(sHost, &(sNetAddress[ipStart]), iLength);
            sHost[iLength] = '\0';

            iErr = addHostItem(m_iBindings, sHost);

            delete[] sHost;
          }
          else{
            iErr = 3; // No IPV4 content
          }
        }
      }
      if (0 == iErr){
        //////////
        // Port:
        int portStart = ipEnd+1;
        int portEnd = ipEnd+1;
        char* sPort;

        if (sAddressLen <= portEnd){
         iErr = 1; // Unexpected length
        }
        else{
          do{
            if (sAddressLen <= ++portEnd){
              bContinue = false;
            }
          }while (bContinue && (',' != sNetAddress[portEnd]));
          int iLength = portEnd - portStart;
          if (iLength >= 1){
            sPort = new char [(portEnd - portStart)+1];
            strncpy(sPort, &(sNetAddress[portStart]), iLength);
            sPort[iLength] = '\0';

            iErr = addPortItem(m_iBindings, sPort);

            delete[] sPort;

            iActIndex = portEnd + 1;
          }
          else{
            iErr = 3; // No IPV4 content
          }
        }
      }
    }
    if (0 == iErr){
      ++m_iBindings;
    }
  }
  while (0 == iErr && bContinue);
  if (0 != iErr){
    printf ("error %d occurred\n", iErr);
    m_bValid = TML_FALSE;
  }
  else{
    m_bValid = TML_TRUE;
  }
}


/**
 * @brief    Destructor.
 */
tmlNetBinding::~tmlNetBinding()
{
  sidex_Variant_DecRef(m_hosts);
  sidex_Variant_DecRef(m_ports);
  sidex_Variant_DecRef(m_isIPV6);
  m_iBindings = 0;
}


/**
  * @brief    add a IP attribute to SIDEX list.
  */
int tmlNetBinding::addIPAttrItem(int iBindings, SIDEX_BOOL bIsIPV6){

  int iErr = 0;
  SIDEX_VARIANT vIPV6;
  SIDEX_INT32 iPos = 0;
  SIDEX_INT32   sRet;

  printf ("isIPB6[%d] = %d\n", iBindings, bIsIPV6);
  vIPV6 = sidex_Variant_New_Boolean(bIsIPV6);
  sRet = sidex_Variant_List_Append(m_isIPV6, vIPV6, &iPos);
  if (SIDEX_SUCCESS != sRet){
    iErr = 5; // SIDEX_ERR
  }
  sidex_Variant_DecRef(vIPV6);
  return iErr;
}


/**
 * @brief    add a host item to SIDEX list.
 */
int tmlNetBinding::addHostItem(int iBindings, char* sHost){

  int iErr = 0;
  SIDEX_INT32 iPos = 0;
  printf ("Host[%d] = %s\n", iBindings, sHost);
  //////////////////////
  // m_hosts attribute:
  SIDEX_VARIANT vHost;
  SIDEX_INT32   sRet;
  sRet = sidex_Variant_New_String(sHost, &vHost);
  if (SIDEX_SUCCESS != sRet){
    iErr = 5; // SIDEX_ERR
  }
  else{
    sRet = sidex_Variant_List_Append(m_hosts, vHost, &iPos);
    if (SIDEX_SUCCESS != sRet){
      iErr = 5; // SIDEX_ERR
    }
    else{
      sidex_Variant_DecRef(vHost);
    }
  }
  return iErr;
}


/**
 * @brief    add a port item to SIDEX list.
 */
int tmlNetBinding::addPortItem(int iBindings, char* sPort){

  int iErr = 0;
  SIDEX_INT32 iPos = 0;
  printf ("Port[%d] = %s\n", iBindings, sPort);

  //////////////////////
  // m_ports attribute:
  SIDEX_VARIANT vPort;
  SIDEX_INT32   sRet;
  sRet = sidex_Variant_New_String(sPort, &vPort);
  if (SIDEX_SUCCESS != sRet){
    iErr = 5; // SIDEX_ERR
  }
  else{
    sRet = sidex_Variant_List_Append(m_ports, vPort, &iPos);
    if (SIDEX_SUCCESS != sRet){
      iErr = 5; // SIDEX_ERR
    }
    else{
      sidex_Variant_DecRef(vPort);
    }
  }
  return iErr;
}


/**
 * @brief Get the network host / ip.
 */
char* tmlNetBinding::getHost(){
  return getHost(0);
}


/**
 * @brief Get the network host / ip.
 */
char* tmlNetBinding::getHost(TML_INT32 index){
  char* sHost = NULL;
  SIDEX_INT32 iLength = 0;
  if (m_iBindings){
    SIDEX_VARIANT vHost;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_hosts, index, &vHost);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String(vHost, &sHost, &iLength);
    }
  }
  return sHost;
}


/**
  * @brief Get the network port.
  */
char* tmlNetBinding::getPort(){
  return getPort(0);
}


/**
  * @brief Get the network port.
  */
char* tmlNetBinding::getPort(TML_INT32 index){
  char* sPort = NULL;
  SIDEX_INT32 iLength = 0;
  if (m_iBindings){
    SIDEX_VARIANT vPort;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_ports, index, &vPort);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String(vPort, &sPort, &iLength);
    }
  }
  return sPort;
}


/**
  * @brief check the network host type.
  */
TML_BOOL tmlNetBinding::isIPV6(){
  return isIPV6(0);
}


/**
  * @brief check the network host type.
  */
TML_BOOL tmlNetBinding::isIPV6(TML_INT32 index){
  TML_BOOL bIsIPV6 = NULL;
  if (m_iBindings){
    SIDEX_VARIANT vType;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_isIPV6, index, &vType);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_Boolean(vType, &bIsIPV6);
    }
  }
  return bIsIPV6;
}


/**
  * @brief check if binding is valid.
  */
TML_BOOL tmlNetBinding::isValid(){
  return m_bValid;
}

