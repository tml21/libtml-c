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
#include "tmlCore.h"
#include "unicode.h"
#include "tmlNetBinding.h"

/**
 * @brief    Constructor.
 *
 */
tmlNetBinding::tmlNetBinding(const char* sNetAddress)
{
  m_iBindings = 0;

  int iErr = 0;

  int sAddressLen = strlen(sNetAddress);

  m_addresses   = sidex_Variant_New_List();
  m_addresses_w = sidex_Variant_New_List();
  m_addresses_x = sidex_Variant_New_List();

  m_hosts       = sidex_Variant_New_List();
  m_hosts_w     = sidex_Variant_New_List();
  m_hosts_x     = sidex_Variant_New_List();

  m_ports       = sidex_Variant_New_List();
  m_ports_w     = sidex_Variant_New_List();
  m_ports_x     = sidex_Variant_New_List();

  m_isIPV6 = sidex_Variant_New_List();
  bool bContinue = true;

  int iActIndex = 0;
  do{
    int addrStart = iActIndex;
    int ipStart = iActIndex;
    int ipEnd = iActIndex;

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
            iErr = addHostItem(m_iBindings, &(sNetAddress[ipStart]), iLength);
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
            iErr = addHostItem(m_iBindings, &(sNetAddress[ipStart]), iLength);
          }
          else{
            iErr = 3; // No IPV4 content
          }
        }
      }
      if (':' != sNetAddress[ipEnd]){
        // Must be a ':'
        iErr = 4;
      }
      if (0 == iErr){
        //////////
        // Port:
        int portStart = ipEnd+1;
        int portEnd = ipEnd+1;

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
            iErr = addPortItem(m_iBindings, &(sNetAddress[portStart]), iLength);
            if (0 == iErr){
              iLength = portEnd - addrStart;
              iErr = addAddressItem(m_iBindings, &(sNetAddress[addrStart]), iLength);

              iActIndex = portEnd + 1;
            }
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
  if (0 != iErr || 0 == m_iBindings){
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
  sidex_Variant_DecRef(m_addresses);
  sidex_Variant_DecRef(m_addresses_w);
  sidex_Variant_DecRef(m_addresses_x);

  sidex_Variant_DecRef(m_hosts);
  sidex_Variant_DecRef(m_hosts_w);
  sidex_Variant_DecRef(m_hosts_x);

  sidex_Variant_DecRef(m_ports);
  sidex_Variant_DecRef(m_ports_w);
  sidex_Variant_DecRef(m_ports_x);

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

  //printf ("isIPB6[%d] = %d\n", iBindings, bIsIPV6);
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
int tmlNetBinding::addHostItem(int iBindings, const char* sHostStartAddr, int iLength){

  int iErr = 0;
  SIDEX_INT32 iPos = 0;

  char*     sHost   = new char     [iLength+1];
  char16_t* sHost_w = new char16_t [iLength+1];
  wchar_t*  sHost_x = new wchar_t  [iLength+1];

#if defined (LINUX) || defined (MINGW_BUILD)
  strncpy(sHost, sHostStartAddr, iLength);
#else
  strncpy_s(sHost, iLength+1, sHostStartAddr, iLength);
#endif
  sHost[iLength] = '\0';

  SIDEX_INT32 utfLen;
  sHost_w = (char16_t*)UTF8toUTF16(sHost, &utfLen);
  sHost_x = UTF8toUTF32(sHost, &utfLen);

  //printf ("Host[%d] = %s\n", iBindings, sHost);

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
    sidex_Variant_DecRef(vHost);
  }

  if (SIDEX_SUCCESS == sRet){
    sRet = sidex_Variant_New_String_W(sHost_w, &vHost);
    if (SIDEX_SUCCESS != sRet){
      iErr = 5; // SIDEX_ERR
    }
    else{
      sRet = sidex_Variant_List_Append(m_hosts_w, vHost, &iPos);
      if (SIDEX_SUCCESS != sRet){
        iErr = 5; // SIDEX_ERR
      }
      sidex_Variant_DecRef(vHost);
    }
  }

  if (SIDEX_SUCCESS == sRet){
    sRet = sidex_Variant_New_String_X(sHost_x, &vHost);
    if (SIDEX_SUCCESS != sRet){
      iErr = 5; // SIDEX_ERR
    }
    else{
      sRet = sidex_Variant_List_Append(m_hosts_x, vHost, &iPos);
      if (SIDEX_SUCCESS != sRet){
        iErr = 5; // SIDEX_ERR
      }
      sidex_Variant_DecRef(vHost);
    }
  }

  delete[] sHost;
  delete[] sHost_w;
  delete[] sHost_x;
  return iErr;
}


/**
 * @brief    add a port item to SIDEX list.
 */
int tmlNetBinding::addPortItem(int iBindings, const char* sPortStartAddr, int iLength){

  int iErr = 0;
  SIDEX_INT32 iPos = 0;

  char*     sPort   = new char     [iLength+1];
  char16_t* sPort_w = new char16_t [iLength+1];
  wchar_t*  sPort_x = new wchar_t  [iLength+1];

#if defined (LINUX) || defined (MINGW_BUILD)
  strncpy(sPort, sPortStartAddr, iLength);
#else
  strncpy_s(sPort, iLength+1, sPortStartAddr, iLength);
#endif
  sPort[iLength] = '\0';

  ////////////////////////////////////////////////////////
  // Check port for plausibility:
  int i;
  char cset[] = "1234567890";
  i = strspn (sPort, cset);
  if (i != strlen (sPort)){
    iErr = 6; // Port contains unexpected characters:
  }
  else{
    SIDEX_INT32 utfLen;
    sPort_w = (char16_t*)UTF8toUTF16(sPort, &utfLen);
    sPort_x = UTF8toUTF32(sPort, &utfLen);
    //printf ("Port[%d] = %s\n", iBindings, sPort);

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
      sidex_Variant_DecRef(vPort);
    }


    if (SIDEX_SUCCESS == sRet){
      sRet = sidex_Variant_New_String_W(sPort_w, &vPort);
      if (SIDEX_SUCCESS != sRet){
        iErr = 5; // SIDEX_ERR
      }
      else{
        sRet = sidex_Variant_List_Append(m_ports_w, vPort, &iPos);
        if (SIDEX_SUCCESS != sRet){
          iErr = 5; // SIDEX_ERR
        }
        sidex_Variant_DecRef(vPort);
      }
    }

    if (SIDEX_SUCCESS == sRet){
      sRet = sidex_Variant_New_String_X(sPort_x, &vPort);
      if (SIDEX_SUCCESS != sRet){
        iErr = 5; // SIDEX_ERR
      }
      else{
        sRet = sidex_Variant_List_Append(m_ports_x, vPort, &iPos);
        if (SIDEX_SUCCESS != sRet){
          iErr = 5; // SIDEX_ERR
        }
        sidex_Variant_DecRef(vPort);
      }
    }
  }

  delete[] sPort;
  delete[] sPort_w;
  delete[] sPort_x;
  return iErr;
}


/**
 * @brief    add an address item to SIDEX list.
 */
int tmlNetBinding::addAddressItem(int iBindings, const char* sAddrStartAddr, int iLength){

  int iErr = 0;
  SIDEX_INT32 iPos = 0;

  char*     sAddress   = new char     [iLength+1];
  char16_t* sAddress_w = new char16_t [iLength+1];
  wchar_t*  sAddress_x = new wchar_t  [iLength+1];

#if defined (LINUX) || defined (MINGW_BUILD)
  strncpy(sAddress, sAddrStartAddr, iLength);
#else
  strncpy_s(sAddress, iLength+1, sAddrStartAddr, iLength);
#endif
  sAddress[iLength] = '\0';

  SIDEX_INT32 utfLen;
  sAddress_w = (char16_t*)UTF8toUTF16(sAddress, &utfLen);
  sAddress_x = UTF8toUTF32(sAddress, &utfLen);
  //printf ("Address[%d] = %s\n", iBindings, sAddress);

  SIDEX_VARIANT vAddress;
  SIDEX_INT32   sRet;

  sRet = sidex_Variant_New_String(sAddress, &vAddress);
  if (SIDEX_SUCCESS != sRet){
    iErr = 5; // SIDEX_ERR
  }
  else{
    sRet = sidex_Variant_List_Append(m_addresses, vAddress, &iPos);
    if (SIDEX_SUCCESS != sRet){
      iErr = 5; // SIDEX_ERR
    }
    sidex_Variant_DecRef(vAddress);
  }


  if (SIDEX_SUCCESS == sRet){
    sRet = sidex_Variant_New_String_W(sAddress_w, &vAddress);
    if (SIDEX_SUCCESS != sRet){
      iErr = 5; // SIDEX_ERR
    }
    else{
      sRet = sidex_Variant_List_Append(m_addresses_w, vAddress, &iPos);
      if (SIDEX_SUCCESS != sRet){
        iErr = 5; // SIDEX_ERR
      }
      sidex_Variant_DecRef(vAddress);
    }
  }

  if (SIDEX_SUCCESS == sRet){
    sRet = sidex_Variant_New_String_X(sAddress_x, &vAddress);
    if (SIDEX_SUCCESS != sRet){
      iErr = 5; // SIDEX_ERR
    }
    else{
      sRet = sidex_Variant_List_Append(m_addresses_x, vAddress, &iPos);
      if (SIDEX_SUCCESS != sRet){
        iErr = 5; // SIDEX_ERR
      }
      sidex_Variant_DecRef(vAddress);
    }
  }

  delete[] sAddress;
  delete[] sAddress_w;
  delete[] sAddress_x;
  return iErr;
}


/**
 * @brief Get the network host / ip.
 */
SIDEX_INT32 tmlNetBinding::getHost(char** sHost){
  return getHost(0, sHost);
}


/**
 * @brief Get the network host / ip.
 */
SIDEX_INT32 tmlNetBinding::getHost(char16_t** sHost){
  return getHost(0, sHost);
}


/**
 * @brief Get the network host / ip.
 */
SIDEX_INT32 tmlNetBinding::getHost(wchar_t** sHost){
  return getHost(0, sHost);
}


/**
 * @brief Get the network host / ip.
 */
SIDEX_INT32 tmlNetBinding::getHost(TML_INT32 index, char** sHost){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vHost;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_hosts, index, &vHost);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String(vHost, sHost, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
}


/**
 * @brief Get the network host / ip.
 */
SIDEX_INT32 tmlNetBinding::getHost(TML_INT32 index, char16_t** sHost){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vHost;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_hosts_w, index, &vHost);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String_W(vHost, sHost, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
}


/**
 * @brief Get the network host / ip.
 */
SIDEX_INT32 tmlNetBinding::getHost(TML_INT32 index, wchar_t** sHost){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vHost;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_hosts_x, index, &vHost);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String_X(vHost, sHost, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
}


/**
  * @brief Get the network port.
  */
SIDEX_INT32 tmlNetBinding::getPort(char** sPort){
  return getPort(0, sPort);
}


/**
  * @brief Get the network port.
  */
SIDEX_INT32 tmlNetBinding::getPort(char16_t** sPort){
  return getPort(0, sPort);
}


/**
  * @brief Get the network port.
  */
SIDEX_INT32 tmlNetBinding::getPort(wchar_t** sPort){
  return getPort(0, sPort);
}


/**
  * @brief Get the network port.
  */
SIDEX_INT32 tmlNetBinding::getPort(TML_INT32 index, char** sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vPort;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_ports, index, &vPort);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String(vPort, sPort, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
}


/**
  * @brief Get the network port.
  */
SIDEX_INT32 tmlNetBinding::getPort(TML_INT32 index, char16_t** sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vPort;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_ports_w, index, &vPort);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String_W(vPort, sPort, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
}


/**
  * @brief Get the network port.
  */
SIDEX_INT32 tmlNetBinding::getPort(TML_INT32 index, wchar_t** sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vPort;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_ports_x, index, &vPort);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String_X(vPort, sPort, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
}


/**
  * @brief Get the network address.
  */
SIDEX_INT32 tmlNetBinding::getAddress(char** sAddress){
  return getAddress(0, sAddress);
}


/**
  * @brief Get the network address.
  */
SIDEX_INT32 tmlNetBinding::getAddress(char16_t** sAddress){
  return getAddress(0, sAddress);
}


/**
  * @brief Get the network address.
  */
SIDEX_INT32 tmlNetBinding::getAddress(wchar_t** sAddress){
  return getAddress(0, sAddress);
}


/**
  * @brief Get the network address.
  */
SIDEX_INT32 tmlNetBinding::getAddress(TML_INT32 index, char** sAddress){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vAddress;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_addresses, index, &vAddress);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String(vAddress, sAddress, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
}


/**
  * @brief Get the network address.
  */
SIDEX_INT32 tmlNetBinding::getAddress(TML_INT32 index, char16_t** sAddress){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vAddress;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_addresses_w, index, &vAddress);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String_W(vAddress, sAddress, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
}


/**
  * @brief Get the network address.
  */
SIDEX_INT32 tmlNetBinding::getAddress(TML_INT32 index, wchar_t** sAddress){
  TML_INT32 iRet = TML_SUCCESS;
  if (isValid()){
    SIDEX_INT32 iLength = 0;
    SIDEX_VARIANT vAddress;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_addresses_x, index, &vAddress);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_String_X(vAddress, sAddress, &iLength);
      if (SIDEX_SUCCESS != iRet){
        iRet = TML_ERR_INFORMATION_UNDEFINED;
      }
    }
  }
  else{
    iRet = TML_ERR_NET_BINDING;
  }
  return iRet;
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
  TML_BOOL bIsIPV6 = TML_FALSE;
  if (m_iBindings){
    SIDEX_VARIANT vType;
    SIDEX_INT32 iRet = sidex_Variant_List_Get(m_isIPV6, index, &vType);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidex_Variant_As_Boolean(vType, &bIsIPV6);
    }
    if (SIDEX_SUCCESS != iRet){
      iRet = TML_ERR_INFORMATION_UNDEFINED;
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

