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

#include "sidex.h"
#include "sidexUtils.h"
#include "sidexCom.h"
#include "unicode.h"
#include "sidexLogHandler.h"
#include <axl.h>

/** @file sidex.cpp
  * 
  * @brief Definitions and dll import file for SIDEX (Simple Data Exchange Format)
  *
  * To use the SIDEX interface library add the globalDefines.h, sidex.h, sidexStdTypes.h, sidexErrors.h and sidex11.lib into your project and take
  * care to have the sidex11.dll and the libaxl.dll in your path.
  */

/**
 * @brief    global attribute to store a converted version request
 */
wchar_t*     m_cVersionX = SIDEX_HANDLE_TYPE_NULL;
char16_t*    m_cVersionW = SIDEX_HANDLE_TYPE_NULL;

/**
 * @brief    global attribute to store a converted copyright request
 */
wchar_t*     m_cCopyrightX = SIDEX_HANDLE_TYPE_NULL;
SIDEX_INT32  m_iLengthCopyrightX = 0;
char16_t*    m_cCopyrightW = SIDEX_HANDLE_TYPE_NULL;
SIDEX_INT32  m_iLengthCopyrightW = 0;

/**
 * @brief    Returns current interface api and library version.
 */
SIDEX_API void DLL_CALL_CONV sidex_Get_Version(SIDEX_INT32* apiVer, SIDEX_INT32* libVer, SIDEX_CTSTR** verStr);
/**
 * char* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Get_Version_A(SIDEX_INT32* apiVer, SIDEX_INT32* libVer, char** verStr){
  *apiVer = API_VERSION_NUMBER;
  *libVer = LIB_VERSION_NUMBER;
  *verStr = LIB_VERSION_STRING_A;
};
/**
 * wchar_t* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Get_Version_X(SIDEX_INT32* apiVer, SIDEX_INT32* libVer, wchar_t** verStr){
  *apiVer = API_VERSION_NUMBER;
  *libVer = LIB_VERSION_NUMBER;
   if (SIDEX_HANDLE_TYPE_NULL != m_cVersionX){
    *verStr = m_cVersionX;
  }
  else{
    SIDEX_INT32 iLength;
    m_cVersionX = UTF8toUTF32(LIB_VERSION_STRING_A, &iLength);
    *verStr = m_cVersionX;
  }
};
/**
 * char16_t* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Get_Version_W(SIDEX_INT32* apiVer, SIDEX_INT32* libVer, char16_t** verStr){
  *apiVer = API_VERSION_NUMBER;
  *libVer = LIB_VERSION_NUMBER;
  if (SIDEX_HANDLE_TYPE_NULL != m_cVersionW){
    *verStr = m_cVersionW;
  }
  else{
    SIDEX_INT32 iLength;
    m_cVersionW = (char16_t*)UTF8toUTF16(LIB_VERSION_STRING_A, &iLength);
    *verStr = m_cVersionW;
  }
};


/**
 * @brief   Returns the copyright information.
 */
SIDEX_API void DLL_CALL_CONV sidex_Get_Copyright(SIDEX_CTSTR** sValue, SIDEX_INT32* iLength);
/**
 * char* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Get_Copyright_A(char** sValue, SIDEX_INT32* iLength){
  *iLength = (SIDEX_INT32) strlen(SIDEX_STRING_COPYRIGHT_A);
  *sValue = SIDEX_STRING_COPYRIGHT_A;
};


/**
 * wchar_t* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Get_Copyright_X(wchar_t** sValue, SIDEX_INT32* iLength){
  if (SIDEX_HANDLE_TYPE_NULL != m_cCopyrightX){
    *sValue = m_cCopyrightX;
    *iLength = m_iLengthCopyrightX;
  }
  else{
    m_cCopyrightX = UTF8toUTF32(SIDEX_STRING_COPYRIGHT_A, &m_iLengthCopyrightX);
    *sValue = m_cCopyrightX;
    *iLength = m_iLengthCopyrightX;
  }
};
/**
 * char16_t* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Get_Copyright_W(char16_t** sValue, SIDEX_INT32* iLength){
  if (SIDEX_HANDLE_TYPE_NULL != m_cCopyrightW){
    *sValue = m_cCopyrightW;
    *iLength = m_iLengthCopyrightW;
  }
  else{
    m_cCopyrightW = (char16_t*)UTF8toUTF16(SIDEX_STRING_COPYRIGHT_A, &m_iLengthCopyrightW);
    *sValue = m_cCopyrightW;
    *iLength = m_iLengthCopyrightW;
  }
};


/**
  * @brief    Set the user name and password to get licensed product access
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Password(const SIDEX_CTSTR* pUserName, const SIDEX_CTSTR* pPassWord);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Password_A(const char* pUserName, const char* pPassWord){
  // Note: Method has been left in the API to be downward compatible.
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Password_X(const wchar_t* pUserName, const wchar_t* pPassWord){
  // Note: Method has been left in the API to be downward compatible.
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Password_W(const char16_t* pUserName, const char16_t* pPassWord){
  // Note: Method has been left in the API to be downward compatible.
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  return iRet;
}

/**
 * @brief    Creates a SIDEX_HANDLE.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Create(const SIDEX_CTSTR* pname, SIDEX_HANDLE* shandle);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Create_A(const char* pname, SIDEX_HANDLE* shandle) {
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  if (NULL != pname){
    try{
      if (SIDEX_SUCCESS != sidexCom::sidexcom_IsValidKey((char*)pname)){
        iRet = SIDEX_ERR_WRONG_DOC;
      }
      else{
        sidexCom* sCom = new sidexCom(pname);
        *shandle = (SIDEX_HANDLE) sCom;
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Create_X(const wchar_t* pname, SIDEX_HANDLE* shandle) {
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (NULL != pname){
    try{
      SIDEX_INT32 iLengthUtf8;
      char* utf8Name = UTF32toUTF8((wchar_t*)pname, &iLengthUtf8);
      if (NULL != utf8Name){
        iRet = sidex_Create_A(utf8Name, shandle);
        delete utf8Name;
      }
      else{
        iRet = SIDEX_ERR_UNICODE;  
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Create_W(const char16_t* pname, SIDEX_HANDLE* shandle) {
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (NULL != pname){
    try{
      SIDEX_INT32 iLengthUtf8;
      char* utf8Name = UTF16toUTF8((wchar_t*)pname, &iLengthUtf8);
      if (NULL != utf8Name){
        iRet = sidex_Create_A(utf8Name, shandle);
        delete utf8Name;
      }
      else{
        iRet = SIDEX_ERR_UNICODE;  
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}


/**
 * @brief    Change the document name / XML root node name of the provided handle.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_DocumentName(SIDEX_HANDLE shandle, const SIDEX_CTSTR pname);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_DocumentName_A(SIDEX_HANDLE shandle, const char* pname){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      iRet = ((sidexCom*)shandle)->sidexcom_Set_DocumentName(pname);
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_DocumentName_X(SIDEX_HANDLE shandle,  const wchar_t* pname){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      SIDEX_INT32 iLengthUtf8;
      char* utf8Name = UTF32toUTF8((wchar_t*)pname, &iLengthUtf8);
      if (NULL != utf8Name){
        iRet = sidex_Set_DocumentName_A(shandle, utf8Name);
        delete utf8Name;
      }
      else{
        iRet = SIDEX_ERR_UNICODE;
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_DocumentName_W(SIDEX_HANDLE shandle,  const char16_t* pname){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      SIDEX_INT32 iLengthUtf8;
      char* utf8Name = UTF16toUTF8((wchar_t*)pname, &iLengthUtf8);
      if (NULL != utf8Name){
        iRet = sidex_Set_DocumentName_A(shandle, utf8Name);
        delete utf8Name;
      }
      else{
        iRet = SIDEX_ERR_UNICODE;
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}

/**
 * @brief    Get the document name / XML root node name.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_DocumentName(SIDEX_HANDLE shandle, SIDEX_CTSTR* pname);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_DocumentName_A(SIDEX_HANDLE shandle, char** pname){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      const char* name;
      name = ((sidexCom*)shandle)->sidexcom_Get_DocumentName_A();
      *pname = (char*)name;
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_DocumentName_X(SIDEX_HANDLE shandle, wchar_t** pname){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      const wchar_t* name;
      name = ((sidexCom*)shandle)->sidexcom_Get_DocumentName_X();
      *pname = (wchar_t*)name;
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_DocumentName_W(SIDEX_HANDLE shandle, char16_t** pname){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      const char16_t* name;
      name = ((sidexCom*)shandle)->sidexcom_Get_DocumentName_W();
      *pname = (char16_t*)name;
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}


/**
 * @brief    Remove the XML tree content except of the root node / document name of the provided handle.
 */
SIDEX_API void DLL_CALL_CONV sidex_Clear(SIDEX_HANDLE shandle){
  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      ((sidexCom*)shandle)->sidexcom_Clear();
    }
    catch (...){
      sidex_log(0xFFFFFFFF, "sidex", "sidex_Clear", "exception", "caught", 66);
    }
  }
  return;
}


/**
 * @brief    Releases memory allocated in sidex_Create().
 */
SIDEX_API void DLL_CALL_CONV sidex_Free(SIDEX_HANDLE* shandle) {
  try{
    sidex_Free_ReadString_W(m_cVersionW);
    sidex_Free_ReadString_W(m_cCopyrightW);
    sidex_Free_ReadString_X(m_cVersionX);
    sidex_Free_ReadString_X(m_cCopyrightX);
    m_cVersionW = SIDEX_HANDLE_TYPE_NULL;
    m_cCopyrightW = SIDEX_HANDLE_TYPE_NULL;
    m_cVersionX = SIDEX_HANDLE_TYPE_NULL;
    m_cCopyrightX = SIDEX_HANDLE_TYPE_NULL;
    // Do make the cast to (sidexCom*) / In that case the delete will call the destructor automatically via the scalar destructor:
    sidexCom* sCom = (sidexCom*)*shandle;
    delete (sCom);
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Free", "exception", "caught", 66);
  }
  return;
}


/**
 * @brief Opens an XML file, write content into SIDEX_HANDLE.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Load_Content(SIDEX_HANDLE shandle, const SIDEX_CTSTR *path);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Load_Content_A(SIDEX_HANDLE shandle, const char *path)
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      iRet = ((sidexCom*)shandle)->sidexcom_Load_Content(path);
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Load_Content_X(SIDEX_HANDLE shandle, const wchar_t *path){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      SIDEX_INT32 iLengthUtf8;
      char* utf8Path = UTF32toUTF8((wchar_t*)path, &iLengthUtf8);
      if (NULL != utf8Path){
        iRet = ((sidexCom*)shandle)->sidexcom_Load_Content(utf8Path);
        delete utf8Path;
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Load_Content_W(SIDEX_HANDLE shandle, const char16_t *path){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      SIDEX_INT32 iLengthUtf8;
      char* utf8Path = UTF16toUTF8((wchar_t*)path, &iLengthUtf8);
      if (NULL != utf8Path){
        iRet = ((sidexCom*)shandle)->sidexcom_Load_Content(utf8Path);
        delete utf8Path;
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}

/**
 * @brief Parse an XML formatted char array, write content into a provided handle.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Content(SIDEX_HANDLE shandle, SIDEX_CTSTR *content);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Content_A(SIDEX_HANDLE shandle, char *content)
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      iRet = ((sidexCom*)shandle)->sidexcom_Set_Content(content);
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Content_X(SIDEX_HANDLE shandle, wchar_t *content){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;
    char* utf8Content = UTF32toUTF8((wchar_t*)content, &iLengthUtf8);
    if (NULL != utf8Content){
      iRet = sidex_Set_Content_A(shandle, utf8Content);
      delete utf8Content;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Set_Content_W(SIDEX_HANDLE shandle, char16_t *content){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;
    char* utf8Content = UTF16toUTF8((wchar_t*)content, &iLengthUtf8);
    if (NULL != utf8Content){
      iRet = sidex_Set_Content_A(shandle, utf8Content);
      delete utf8Content;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Saves a SIDEX_HANDLE as an XML formatted document file.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Save_Content(SIDEX_HANDLE shandle, const SIDEX_CTSTR *path);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Save_Content_A(SIDEX_HANDLE shandle, const char *path)
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      iRet = ((sidexCom*)shandle)->sidexcom_DumpToFile(path);
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Save_Content_X(SIDEX_HANDLE shandle, const wchar_t *path){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      SIDEX_INT32 iLengthUtf8;
      char* utf8Path = UTF32toUTF8((wchar_t*)path, &iLengthUtf8);
      if (NULL != utf8Path){
        iRet = sidex_Save_Content_A(shandle, utf8Path);
        delete utf8Path;
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Save_Content_W(SIDEX_HANDLE shandle, const char16_t *path){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      SIDEX_INT32 iLengthUtf8;
      char* utf8Path = UTF16toUTF8((wchar_t*)path, &iLengthUtf8);
      if (NULL != utf8Path){
        iRet = sidex_Save_Content_A(shandle, utf8Path);
        delete utf8Path;
      }
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}


/**
 * @brief Saves a SIDEX_HANDLE as an XML formatted char array.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content(SIDEX_HANDLE shandle, SIDEX_CTSTR **content, SIDEX_INT32* iContentLength); 
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content_A(SIDEX_HANDLE shandle, char **content, SIDEX_INT32* iContentLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      iRet = ((sidexCom*)shandle)->sidexcom_DumpToString(content, iContentLength);
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content_X(SIDEX_HANDLE shandle, wchar_t** content, SIDEX_INT32* iContentLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    char* utf8Content;
    SIDEX_INT32 iLengthUtf8;
    iRet = sidex_Get_Content_A(shandle, &utf8Content, &iLengthUtf8);
    if (SIDEX_SUCCESS == iRet){
      wchar_t* utf16Content = UTF8toUTF32(utf8Content, iContentLength);
      if (NULL != utf16Content){
        *content = utf16Content;
      }
      else{
        iRet = SIDEX_ERR_UNICODE;
      }
      sidex_Free_Content(utf8Content);
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content_W(SIDEX_HANDLE shandle, char16_t** content, SIDEX_INT32* iContentLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    char* utf8Content;
    SIDEX_INT32 iLengthUtf8;
    iRet = sidex_Get_Content_A(shandle, &utf8Content, &iLengthUtf8);
    if (SIDEX_SUCCESS == iRet){
      wchar_t* utf16Content = UTF8toUTF16(utf8Content, iContentLength);
      if (NULL != utf16Content){
        *content = (char16_t*)utf16Content;
      }
      else{
        iRet = SIDEX_ERR_UNICODE;
      }
      sidex_Free_Content(utf8Content);
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Returns the amount of buffer size internal allocated in sidex_Get_Content request.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Get_Content_Length(SIDEX_HANDLE shandle, SIDEX_INT32* iContentLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  if (SIDEX_HANDLE_TYPE_NULL != shandle){
    try{
      iRet = ((sidexCom*)shandle)->sidexcom_DumpToStringLength(iContentLength);
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}


/**
 * @brief Free an allocated string returnd by sidex_Get_Content()
 */
SIDEX_API void DLL_CALL_CONV sidex_Free_Content(SIDEX_CTSTR *string); 
/**
 * char* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Free_Content_A(char *string){
  try{
    axl_free(string);
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Free_Content_A", "exception", "caught", 66);
  }
  return;
}
/**
 * wchar_t* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Free_Content_X(wchar_t *string){
  try{
    delete string;
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Free_Content_X", "exception", "caught", 66);
  }
  return;
}
/**
 * char16_t* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Free_Content_W(char16_t *string){
  try{
    delete string;
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Free_Content_W", "exception", "caught", 66);
  }
  return;
}


/**
 * @brief   Decrements the reference counter of the SIDEX_VARIANT object.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_DecRef(SIDEX_VARIANT variant) 
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    sidexutil_Variant_DecRef(variant);
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Variant_DecRef", "exception", "caught", 66);
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Increments the reference counter of the SIDEX_VARIANT object.
 */
SIDEX_API void DLL_CALL_CONV sidex_Variant_IncRef(SIDEX_VARIANT variant) 
{
  try{
    sidexutil_Variant_IncRef(variant);
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Variant_IncRef", "exception", "caught", 66);
  }
  return;
}


/**
 *  @brief   Sets SIDEX_VARIANT value as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT variant);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_WriteVariant(ngroup, nkey, variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Variant_Write_A(shandle, utf8Group, utf8Key, variant);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Variant_Write_A(shandle, utf8Group, utf8Key, variant);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Reads content of the provided group / key, interpreting it as a SIDEX_VARIANT.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Read(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* variant);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Read_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT* variant)
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_ReadVariant(ngroup, nkey, variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Read_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Variant_Read_A(shandle, utf8Group, utf8Key, variant);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Read_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Variant_Read_A(shandle, utf8Group, utf8Key, variant);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Create a SIDEX_VARIANT instance of None / empty
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_None() 
{
   sidexNone* sNone = new sidexNone();
   return (SIDEX_VARIANT)sNone;
}


/**
 * @brief   Create a SIDEX_VARIANT instance out of an boolean value
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Boolean(SIDEX_BOOL value) 
{
   sidexBoolean* sBool = new sidexBoolean((SIDEX_FALSE == value)? false : true);
   return (SIDEX_VARIANT)sBool;
}


/**
 * @brief   Create a SIDEX_VARIANT instance out of an Integer value
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Integer(SIDEX_INT64 value) 
{
   sidexInteger* sInteger = new sidexInteger(value);
   return (SIDEX_VARIANT)sInteger;
}


/**
 * @brief   Create a SIDEX_VARIANT instance out of a Float value
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Float(SIDEX_DOUBLE value) 
{
   sidexFloat* sFloat = new sidexFloat(value);
   return (SIDEX_VARIANT)sFloat;
}


/**
 * @brief   Create a SIDEX_VARIANT instance out of a DateTime formatted char array
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_DateTime(char* sDateTime, SIDEX_VARIANT* variant);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_DateTime_A(char* sDateTime, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
     sidexDateTime* vDateTime = new sidexDateTime();
     iRet = vDateTime->setValue(sDateTime);
     if (SIDEX_SUCCESS == iRet){
       *variant = (SIDEX_VARIANT)vDateTime;
     }
     else{
       sidexutil_Variant_DecRef ((SIDEX_VARIANT)vDateTime);
     }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
   return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_DateTime_X(wchar_t* sDateTime, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Str = UTF32toUTF8(sDateTime, &iLengthUtf8);
    if (NULL != utf8Str){
      iRet = sidex_Variant_New_DateTime_A(utf8Str, variant);
      delete utf8Str;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_DateTime_W(char16_t* sDateTime, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Str = UTF16toUTF8((wchar_t*)sDateTime, &iLengthUtf8);
    if (NULL != utf8Str){
      iRet = sidex_Variant_New_DateTime_A(utf8Str, variant);
      delete utf8Str;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Set the string format attribute of a SIDEX_VARIANT object.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_SetFormat(SIDEX_VARIANT variant, const SIDEX_CTSTR* format);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_SetFormat_A(SIDEX_VARIANT variant, const char* format){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  try{
    if (sidex_Variant_String_Check(variant)){
      iRet = ((sidexString*)variant)->setStringFormat(format);
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }


  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_SetFormat_X(SIDEX_VARIANT variant, const wchar_t* format){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    if (NULL != format){
      SIDEX_INT32 iLengthUtf8 = 0;
      char* utf8Str = NULL;

      utf8Str = UTF32toUTF8((wchar_t*) format, &iLengthUtf8);
      iRet = sidex_Variant_String_SetFormat_A(variant, utf8Str);
      if (NULL != utf8Str){
        delete utf8Str;
      }
    }
    else{
      iRet = SIDEX_ERR_WRONG_ENCODING_FORMAT;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_SetFormat_W(SIDEX_VARIANT variant, const char16_t* format){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    if (NULL != format){
      SIDEX_INT32 iLengthUtf8 = 0;
      char* utf8Str = NULL;

      utf8Str = UTF16toUTF8((wchar_t*) format, &iLengthUtf8);
      iRet = sidex_Variant_String_SetFormat_A(variant, utf8Str);
      if (NULL != utf8Str){
        delete utf8Str;
      }
    }
    else{
      iRet = SIDEX_ERR_WRONG_ENCODING_FORMAT;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Set the string format attribute of a SIDEX_VARIANT object.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_GetFormat(SIDEX_VARIANT variant, const SIDEX_CTSTR** format);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_GetFormat_A(SIDEX_VARIANT variant, const char** format){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (sidex_Variant_String_Check(variant)){
    ((sidexString*)variant)->getStringFormat_A(format);
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_GetFormat_X(SIDEX_VARIANT variant, const wchar_t** format){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (sidex_Variant_String_Check(variant)){
    ((sidexString*)variant)->getStringFormat_X(format);
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_String_GetFormat_W(SIDEX_VARIANT variant, const char16_t** format){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (sidex_Variant_String_Check(variant)){
    ((sidexString*)variant)->getStringFormat_W(format);
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Create a SIDEX_VARIANT instance out of a NULL terminated char array
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_String(SIDEX_CTSTR* value, SIDEX_VARIANT* variant);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_String_A(char* value, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    SIDEX_INT32 iStrLength = 0;
    if (NULL != value){
      iStrLength = (SIDEX_INT32) strlen(value);
    }
    sidexString* sString = new sidexString(value, iStrLength, SIDEX_ENCODE_NONE);
    *variant = (SIDEX_VARIANT)sString;
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_String_X(wchar_t* value, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8 = 0;
    char* utf8Str = NULL;

    if (NULL != value){
      utf8Str = UTF32toUTF8(value, &iLengthUtf8);
    }
    iRet = sidex_Variant_New_String_A(utf8Str, variant);
    if (NULL != utf8Str){
      delete utf8Str;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_String_W(char16_t* value, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8 = 0;
    char* utf8Str = NULL;

    if (NULL != value){
      utf8Str = UTF16toUTF8((wchar_t*)value, &iLengthUtf8);
    }
    iRet = sidex_Variant_New_String_A(utf8Str, variant);
    if (NULL != utf8Str){
      delete utf8Str;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Create a SIDEX_VARIANT instance out of a binary char array
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_New_Binary(unsigned char* value, SIDEX_INT32 iStrLength, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL == value && iStrLength > 0){
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  else{
    try{
      sidexString* sString = new sidexString((char*)value, iStrLength, SIDEX_ENCODE_BASE64);
      *variant = (SIDEX_VARIANT)sString;
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief   Create a SIDEX_VARIANT instance of a List type
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_List() 
{
   sidexList* sList = new sidexList();
   return (SIDEX_VARIANT)sList;
}


/**
 * @brief   Create a SIDEX_VARIANT instance of a Dict type with the initial amount of 100 entrys.
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Dict() 
{
   sidexDict* sDict = new sidexDict();
   return (SIDEX_VARIANT)sDict;
}


/**
 * @brief   Create a SIDEX_VARIANT instance of a Dict type
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_DictBySize(SIDEX_INT32 iSize) 
{
   sidexDict* sDict = new sidexDict(iSize);
   return (SIDEX_VARIANT)sDict;
}


/**
 * @brief   Create a SIDEX_VARIANT instance of a Table type
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_New_Table() 
{
   sidexTable* sTable = new sidexTable();
   return (SIDEX_VARIANT)sTable;
}


/**
 * @brief   Returns the type of a SIDEX_VARIANT value.
 */
SIDEX_API SIDEX_DATA_TYPE DLL_CALL_CONV sidex_Variant_GetType (SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_getDataType" / returns SIDEX_DATA_TYPE_UNKNOWN in Case of exception
  return sidexutil_getDataType(value);
};


/**
 * @brief   Returns a copy of a SIDEX_VARIANT.
 */
SIDEX_API SIDEX_VARIANT DLL_CALL_CONV sidex_Variant_Copy (SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_Variant_Copy" / returns TML_HANDLE_TYPE_NULL in Case of exception
  return sidexutil_Variant_Copy (value);
}


/**
 * @brief   Return SIDEX_TRUE if value is a None object / of type SIDEX_DATA_TYPE_NONE.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_None_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_dataTypeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_NONE);
};


/**
 * @brief   Return SIDEX_TRUE if value is a boolean object / of type SIDEX_DATA_TYPE_BOOLEAN.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Boolean_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_dataTypeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_BOOLEAN);
};


/**
 * @brief   Return SIDEX_TRUE if value is an Integer object / of type SIDEX_DATA_TYPE_INTEGER.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Integer_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_dataTypeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_INTEGER);
};


/**
 * @brief   Return SIDEX_TRUE if value is a Float object / of type SIDEX_DATA_TYPE_FLOAT.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Float_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_dataTypeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_FLOAT);
};


/**
 * @brief   Return SIDEX_TRUE if value is a DateTime object / of type SIDEX_DATA_TYPE_DATETIME.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_DateTime_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_dataTypeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_DATETIME);
}


/**
* @brief   Return SIDEX_TRUE if value is a null terminated character array value / of type SIDEX_DATA_TYPE_STRING.
*/
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_String_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_stringDataEncodeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_stringDataEncodeCheck(value, SIDEX_ENCODE_NONE);
};


/**
* @brief   Return SIDEX_TRUE if value is a binary character array value / of type SIDEX_DATA_TYPE_BINARY.
*/
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Binary_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_stringDataEncodeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_stringDataEncodeCheck(value, SIDEX_ENCODE_BASE64);
};


/**
 * @brief   Return SIDEX_TRUE if value is a List object / of type SIDEX_DATA_TYPE_LIST.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_List_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_dataTypeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_LIST);
};


/**
 * @brief   Return SIDEX_TRUE if value is a Dict object / of type SIDEX_DATA_TYPE_DICT.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Dict_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_dataTypeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_DICT);
};


/**
 * @brief   Return SIDEX_TRUE if value is a Table object / of type SIDEX_DATA_TYPE_TABLE.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_Variant_Table_Check(SIDEX_VARIANT value){
  // Exception handling will be done in the "sidexutil_dataTypeCheck" / returns SIDEX_FALSE in Case of exception
  return sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_TABLE);
};


/**
 * @brief   Return the number of items in a List.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Size(SIDEX_VARIANT sList, SIDEX_INT32* iSize){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilList_Size(sList, iSize);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Return the number of entries in a dictionary.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Size(SIDEX_VARIANT sDict, SIDEX_INT32* iSize){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_Size(sDict, iSize);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Return all the key names of a dictionary.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Keys(SIDEX_VARIANT sDict, SIDEX_VARIANT* sKeys){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_Keys(sDict, sKeys);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Resets internal reference pointer to the beginning of the dictionary for fast contiguous Dict read operations.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_First(SIDEX_VARIANT sDict){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_First(sDict);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Returns the actual internal referenced SIDEX_VARIANT entry and moves pointer to the next entry of the dictionary for fast contiguous Dict read operations.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Next(SIDEX_VARIANT sDict, SIDEX_CTSTR** sKey, SIDEX_VARIANT* vNext);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Next_A(SIDEX_VARIANT sDict, char** sKey, SIDEX_VARIANT* vNext){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_Next_A(sDict, sKey, vNext);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Next_X(SIDEX_VARIANT sDict, wchar_t** sKey, SIDEX_VARIANT* vNext){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_Next_X(sDict, sKey, vNext);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Next_W(SIDEX_VARIANT sDict, char16_t** sKey, SIDEX_VARIANT* vNext){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_Next_W(sDict, sKey, vNext);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Check for the existence of an entry with the sKey in a Dict.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_HasKey(SIDEX_VARIANT variant, SIDEX_CTSTR* sKey, SIDEX_BOOL* bRet);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_HasKey_A(SIDEX_VARIANT variant, char* sKey, SIDEX_BOOL* bRet){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_HasKey(variant, sKey, bRet);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return  iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_HasKey_X(SIDEX_VARIANT variant, wchar_t* sKey, SIDEX_BOOL* bRet){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Key = UTF32toUTF8(sKey, &iLengthUtf8);
    if (NULL != utf8Key){
      iRet = sidex_Variant_Dict_HasKey_A(variant, utf8Key, bRet);
      delete utf8Key;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_HasKey_W(SIDEX_VARIANT variant, char16_t* sKey, SIDEX_BOOL* bRet){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Key = UTF16toUTF8((wchar_t*)sKey, &iLengthUtf8);
    if (NULL != utf8Key){
      iRet = sidex_Variant_Dict_HasKey_A(variant, utf8Key, bRet);
      delete utf8Key;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Return the number of rows of a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_Rows(SIDEX_VARIANT sTable, SIDEX_INT32* iRows){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_Rows(sTable, iRows);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Return the number of columns of a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_Columns(SIDEX_VARIANT sTable, SIDEX_INT32* iColumns){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_Columns(sTable, iColumns);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Return all the column names from the Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_ColumnNames(SIDEX_VARIANT sTable, SIDEX_VARIANT* varNames){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_ColumnNames(sTable, varNames);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Check for the existence of the sColumnName in a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_HasColumn(SIDEX_VARIANT sTable, SIDEX_CTSTR* sColumnName, SIDEX_BOOL* bRet);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_HasColumn_A(SIDEX_VARIANT sTable, char* sColumnName, SIDEX_BOOL* bRet){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutil_TableHasColumn(sTable, sColumnName, bRet);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_HasColumn_X(SIDEX_VARIANT sTable, wchar_t* sColumnName, SIDEX_BOOL* bRet){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF32toUTF8(sColumnName, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_HasColumn_A(sTable, utf8ColName, bRet);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_HasColumn_W(SIDEX_VARIANT sTable, char16_t* sColumnName, SIDEX_BOOL* bRet){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF16toUTF8((wchar_t*)sColumnName, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_HasColumn_A(sTable, utf8ColName, bRet);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Interpreting a SIDEX_VARIANT as a Boolean value. Returns it's value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Boolean(SIDEX_VARIANT variant, SIDEX_BOOL* bValue){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilBoolean_Value(variant, bValue);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
};


/**
 * @brief   Interpreting a SIDEX_VARIANT as an Integer value. Returns it's value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Integer(SIDEX_VARIANT variant, SIDEX_INT64* iValue){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilInteger_Value(variant, iValue);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
};


/**
 * @brief   Interpreting a SIDEX_VARIANT as a Float value. Returns it's value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Float(SIDEX_VARIANT variant, SIDEX_DOUBLE* fValue){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilFloat_Value(variant, fValue);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
};


/**
 * @brief   Interpreting a SIDEX_VARIANT as a DateTime value. Returns the reference to a DateTime formatted char array.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_DateTime(SIDEX_VARIANT variant, SIDEX_CTSTR** dtValue);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_DateTime_A(SIDEX_VARIANT variant, char** dtValue){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDateTime_Value_A(variant, dtValue);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_DateTime_X(SIDEX_VARIANT variant, wchar_t** dtValue){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDateTime_Value_X(variant, dtValue);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_DateTime_W(SIDEX_VARIANT variant, char16_t** dtValue){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDateTime_Value_W(variant, dtValue);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the reference to the char array value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String(SIDEX_VARIANT variant, SIDEX_CTSTR** sValue, SIDEX_INT32* iLength);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_A(SIDEX_VARIANT variant, char** sValue, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (sidex_Variant_String_Check(variant))
      iRet = sidexutilString_Value_A(variant, sValue, iLength);
    else
      iRet = SIDEX_ERR_WRONG_TYPE;
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_X(SIDEX_VARIANT variant, wchar_t** sValue, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (sidex_Variant_String_Check(variant))
      iRet = sidexutilString_Value_X(variant, sValue, iLength);
    else
      iRet = SIDEX_ERR_WRONG_TYPE;
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_W(SIDEX_VARIANT variant, char16_t** sValue, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (sidex_Variant_String_Check(variant))
      iRet = sidexutilString_Value_W(variant, sValue, iLength);
    else
      iRet = SIDEX_ERR_WRONG_TYPE;
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

  
/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns it's length.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_Length(SIDEX_VARIANT variant, SIDEX_INT32* iLength);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_Length_A(SIDEX_VARIANT variant, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (sidex_Variant_String_Check(variant))
      iRet = sidexutilString_Value_Length_A(variant, iLength);  
    else
      iRet = SIDEX_ERR_WRONG_TYPE;
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_Length_X(SIDEX_VARIANT variant, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (sidex_Variant_String_Check(variant))
      iRet = sidexutilString_Value_Length_X(variant, iLength);  
    else
      iRet = SIDEX_ERR_WRONG_TYPE;
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_String_Length_W(SIDEX_VARIANT variant, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (sidex_Variant_String_Check(variant))
      iRet = sidexutilString_Value_Length_W(variant, iLength);  
    else
      iRet = SIDEX_ERR_WRONG_TYPE;
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Interpreting a SIDEX_VARIANT as a binary char array value. Returns the reference to the binary char array value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Binary(SIDEX_VARIANT variant, unsigned char** sValue, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilBinaryString_Value(variant, (char**)sValue, iLength);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

  
/**
 * @brief   Interpreting a SIDEX_VARIANT as a binary char array value. Returns the reference to the binary char array value. Returns it's length.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_As_Binary_Length(SIDEX_VARIANT variant, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    unsigned char* sValue;
    iRet =  sidexutilBinaryString_Value(variant, (char**)&sValue, iLength);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Append an item to a List.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Append(SIDEX_VARIANT sList, SIDEX_VARIANT value, SIDEX_INT32* pos) 
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilList_Add(sList, value, pos);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Inserts an item to the List before the requested position.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Insert(SIDEX_VARIANT sList, SIDEX_VARIANT value, SIDEX_INT32 pos) 
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilList_Insert(sList, value, pos);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Overwrite an item in a List.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Set(SIDEX_VARIANT sList, SIDEX_VARIANT value, SIDEX_INT32 pos) 
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilList_Set(sList, value, pos);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Remove the item out of a List at the requested position.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_DeleteItem(SIDEX_VARIANT sList, SIDEX_INT32 pos){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilList_DeleteItem(sList, pos);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Removes all items out of a List.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Clear(SIDEX_VARIANT sList){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilList_Clear(sList);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Get an item out of a List.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_List_Get(SIDEX_VARIANT sList, SIDEX_INT32 index, SIDEX_VARIANT* variant) 
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilList_Get(sList, index, variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Set an object into the Dict referring the sKey
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Set(SIDEX_VARIANT sDict, SIDEX_CTSTR* nKey, SIDEX_VARIANT value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Set_A(SIDEX_VARIANT sDict, char* nKey, SIDEX_VARIANT value){
  /////////////////////////////////////////////////////
  // First check content for valid XML name convention:
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  if (NULL == nKey){
    iRet = SIDEX_ERR_MISSING_KEY;
  }
  if (SIDEX_SUCCESS == iRet){
    try{
      iRet = sidexutilDict_Set(sDict, nKey, value);
    }
    catch (...){
      iRet = SIDEX_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Set_X(SIDEX_VARIANT sDict, wchar_t* nKey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Key = UTF32toUTF8(nKey, &iLengthUtf8);
    if (NULL != utf8Key){
      iRet = sidex_Variant_Dict_Set_A(sDict, utf8Key, value);
      delete utf8Key;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Set_W(SIDEX_VARIANT sDict, char16_t* nKey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Key = UTF16toUTF8((wchar_t*)nKey, &iLengthUtf8);
    if (NULL != utf8Key){
      iRet = sidex_Variant_Dict_Set_A(sDict, utf8Key, value);
      delete utf8Key;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Remove the entry referring the sKey out of a Dict.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Delete(SIDEX_VARIANT sDict, SIDEX_CTSTR* sKey);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Delete_A(SIDEX_VARIANT sDict, char* sKey){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_Delete(sDict, sKey);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Delete_X(SIDEX_VARIANT sDict, wchar_t* sKey){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Key = UTF32toUTF8(sKey, &iLengthUtf8);
    if (NULL != utf8Key){
      iRet = sidex_Variant_Dict_Delete_A(sDict, utf8Key);
      delete utf8Key;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Delete_W(SIDEX_VARIANT sDict, char16_t* sKey){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Key = UTF16toUTF8((wchar_t*)sKey, &iLengthUtf8);
    if (NULL != utf8Key){
      iRet = sidex_Variant_Dict_Delete_A(sDict, utf8Key);
      delete utf8Key;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Removes all entries of a Dict.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Clear(SIDEX_VARIANT sDict){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_Clear(sDict);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Get an entry of a Dict.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Get(SIDEX_VARIANT sDict, SIDEX_CTSTR* sKey, SIDEX_VARIANT* variant);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Get_A(SIDEX_VARIANT sDict, char* sKey, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilDict_Get(sDict, sKey, variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Get_X(SIDEX_VARIANT sDict, wchar_t* sKey, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Key = UTF32toUTF8(sKey, &iLengthUtf8);
    if (NULL != utf8Key){
      iRet = sidex_Variant_Dict_Get_A(sDict, utf8Key, variant);
      delete utf8Key;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Dict_Get_W(SIDEX_VARIANT sDict, char16_t* sKey, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Key = UTF16toUTF8((wchar_t*)sKey, &iLengthUtf8);
    if (NULL != utf8Key){
      iRet = sidex_Variant_Dict_Get_A(sDict, utf8Key, variant);
      delete utf8Key;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Add a column to a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddColumn(SIDEX_VARIANT sTable, SIDEX_CTSTR* sColumn);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddColumn_A(SIDEX_VARIANT sTable, char* sColumn){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexCom::sidexcom_IsValidKey(sColumn);
    if (SIDEX_SUCCESS == iRet){
      iRet = sidexutilTable_AddColumn(sTable, sColumn);
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddColumn_X(SIDEX_VARIANT sTable, wchar_t* sColumn){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF32toUTF8(sColumn, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_AddColumn_A(sTable, utf8ColName);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddColumn_W(SIDEX_VARIANT sTable, char16_t* sColumn){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF16toUTF8((wchar_t*)sColumn, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_AddColumn_A(sTable, utf8ColName);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Remove a column of a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteColumn(SIDEX_VARIANT sTable, SIDEX_CTSTR* sColumn);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteColumn_A(SIDEX_VARIANT sTable, char* sColumn){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_DeleteColumn(sTable, sColumn);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteColumn_X(SIDEX_VARIANT sTable, wchar_t* sColumn){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF32toUTF8(sColumn, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_DeleteColumn_A(sTable, utf8ColName);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteColumn_W(SIDEX_VARIANT sTable, char16_t* sColumn){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF16toUTF8((wchar_t*)sColumn, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_DeleteColumn_A(sTable, utf8ColName);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief    Get the column name from a Table at the requested index.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetColumnName(SIDEX_VARIANT sTable, SIDEX_INT32 index, SIDEX_CTSTR** sColumn);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetColumnName_A(SIDEX_VARIANT sTable, SIDEX_INT32 index, char** sColumn){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet =sidexutilTable_GetColumnName_A(sTable, index, sColumn);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetColumnName_X(SIDEX_VARIANT sTable, SIDEX_INT32 index, wchar_t** sColumn){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_GetColumnName_X(sTable, index, sColumn);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetColumnName_W(SIDEX_VARIANT sTable, SIDEX_INT32 index, char16_t** sColumn){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_GetColumnName_W(sTable, index, sColumn);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Add a new empty row to a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_AddRow(SIDEX_VARIANT sTable, SIDEX_INT32* iRowIdx) 
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_AddRow(sTable, iRowIdx);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Remove the row referring the rowIndex out of a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteRow(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_DeleteRow(sTable, rowIndex);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Remove all rows out of a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_DeleteRows(SIDEX_VARIANT sTable){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_DeleteRows(sTable);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Get row out of a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetRow(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, SIDEX_VARIANT* variant)
{
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_GetRow(sTable, rowIndex, variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Get a Table field out of a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetField(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, SIDEX_CTSTR* sColumnName, SIDEX_VARIANT* variant);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetField_A(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, char* sColumnName, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_GetField(sTable, rowIndex, sColumnName, variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetField_X(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, wchar_t* sColumnName, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF32toUTF8(sColumnName, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_GetField_A(sTable, rowIndex, utf8ColName, variant);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_GetField_W(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, char16_t* sColumnName, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF16toUTF8((wchar_t*)sColumnName, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_GetField_A(sTable, rowIndex, utf8ColName, variant);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Set a Table field in a Table.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_SetField(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, SIDEX_CTSTR* sColumnName, SIDEX_VARIANT variant);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_SetField_A(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, char* sColumnName, SIDEX_VARIANT variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = sidexutilTable_SetField(sTable, rowIndex, sColumnName, variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_SetField_X(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, wchar_t* sColumnName, SIDEX_VARIANT variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF32toUTF8(sColumnName, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_SetField_A(sTable, rowIndex, utf8ColName, variant);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Variant_Table_SetField_W(SIDEX_VARIANT sTable, SIDEX_INT32 rowIndex, char16_t* sColumnName, SIDEX_VARIANT variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8ColName = UTF16toUTF8((wchar_t*)sColumnName, &iLengthUtf8);
    if (NULL != utf8ColName){
      iRet = sidex_Variant_Table_SetField_A(sTable, rowIndex, utf8ColName, variant);
      delete utf8ColName;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Sets None value (empty value) as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_None_Write(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_None_Write_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_WriteNone(ngroup, nkey);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
   return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_None_Write_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_None_Write_A(shandle, utf8Group, utf8Key);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_None_Write_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_None_Write_A(shandle, utf8Group, utf8Key);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Reads content of the provided group / key, interpreting it as a Boolean value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_BOOL* value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_BOOL* value){
  SIDEX_VARIANT variant;
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_ReadVariant(ngroup, nkey, &variant);
    if (SIDEX_SUCCESS == iRet){
       int type = ((sidexBase*)(variant))->getType();
       if (type == SIDEX_DATA_TYPE_BOOLEAN){
         bool bRet = ((sidexBoolean*)variant)->getValue();
         if (bRet){
          *value = SIDEX_TRUE;
         }
         else{
          *value = SIDEX_FALSE;
         }
       }
       else{
         iRet = SIDEX_ERR_WRONG_TYPE;
       }
       sidexutil_Variant_DecRef (variant);
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_BOOL* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Boolean_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_BOOL* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Boolean_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Sets a Boolean value as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Write(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_BOOL value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Write_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_BOOL value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_WriteBoolean(ngroup, nkey, value);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
   return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Write_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_BOOL value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Boolean_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Boolean_Write_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_BOOL value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Boolean_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Reads content of the provided group / key, interpreting it as an Integer value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_INT64 *value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_INT64 *value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    SIDEX_VARIANT variant;
    iRet = ((sidexCom*)shandle)->sidexcom_ReadVariant(ngroup, nkey, &variant);
    if (SIDEX_SUCCESS == iRet){
       int type = ((sidexBase*)(variant))->getType();
       if (type == SIDEX_DATA_TYPE_INTEGER){
         *value = ((sidexInteger*)variant)->getValue();
       }
       else{
         iRet = SIDEX_ERR_WRONG_TYPE;
       }
       sidexutil_Variant_DecRef (variant);
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_INT64 *value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Integer_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_INT64 *value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Integer_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Sets an Integer value as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Write(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_INT64 value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Write_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_INT64 value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_WriteInteger(ngroup, nkey, value);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
   return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Write_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_INT64 value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Integer_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Integer_Write_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_INT64 value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Integer_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   


/**
 * @brief   Reads content of the provided group / key, interpreting it as a Float value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_DOUBLE *value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_DOUBLE *value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    SIDEX_VARIANT variant;
    iRet = ((sidexCom*)shandle)->sidexcom_ReadVariant(ngroup, nkey, &variant);
    if (SIDEX_SUCCESS == iRet){
       int type = ((sidexBase*)(variant))->getType();
       if (type == SIDEX_DATA_TYPE_FLOAT){
         *value = ((sidexFloat*)variant)->getValue();
       }
       else{
         iRet = SIDEX_ERR_WRONG_TYPE;
       }
       sidexutil_Variant_DecRef (variant);
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_DOUBLE *value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Float_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_DOUBLE *value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Float_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Sets a Float value as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_DOUBLE value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_DOUBLE value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_WriteFloat(ngroup, nkey, value);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_DOUBLE value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Float_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Float_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_DOUBLE value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Float_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Reads content of the provided group / key, interpreting it as a DateTime value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Read(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Read_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    SIDEX_VARIANT variant;
    SIDEX_INT32 iRet = ((sidexCom*)shandle)->sidexcom_ReadVariant(ngroup, nkey, &variant);
    if (SIDEX_SUCCESS == iRet){
       int type = ((sidexBase*)(variant))->getType();
       if (type == SIDEX_DATA_TYPE_DATETIME){
         *value = variant;
       }
       else{
         iRet = SIDEX_ERR_WRONG_TYPE;
         sidexutil_Variant_DecRef (variant);
       }
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Read_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_DateTime_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Read_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_DateTime_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Sets a DateTime value as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (SIDEX_TRUE == sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_DATETIME)){
      iRet = ((sidexCom*)shandle)->sidexcom_WriteDateTime(ngroup, nkey, value);
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_DateTime_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DateTime_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_DateTime_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Reads content of the provided group / key, interpreting it as a character array.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_CTSTR** value, SIDEX_INT32* iStrLength);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, char** value, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_ReadString(ngroup, nkey, value, iStrLength, SIDEX_DATA_TYPE_STRING);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, wchar_t** value, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
      SIDEX_INT32 iLengthUtf8;

      char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
      if (NULL != utf8Group){
        char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
        if (NULL != utf8Key){
          char* sStr;
          SIDEX_INT32 iCmdLength;
          iRet = sidex_String_Read_A(shandle, utf8Group, utf8Key, &sStr, &iCmdLength);
          delete utf8Key;
          if (SIDEX_SUCCESS == iRet){
            wchar_t* utf16Value = UTF8toUTF32(sStr, iStrLength);
            if (NULL != utf16Value){
              *value = utf16Value;
            }
            sidex_Free_ReadString(sStr);
          }
        }
        delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, char16_t** value, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
      SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
        if (NULL != utf8Key){
          char* sStr;
          SIDEX_INT32 iCmdLength;
          iRet = sidex_String_Read_A(shandle, utf8Group, utf8Key, &sStr, &iCmdLength);
          delete utf8Key;
          if (SIDEX_SUCCESS == iRet){
            wchar_t* utf16Value = UTF8toUTF16(sStr, iStrLength);
            if (NULL != utf16Value){
              *value = (char16_t*)utf16Value;
            }
            sidex_Free_ReadString(sStr);
          }
        }
        delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/// @brief   Reads content of the provided group / key, interpreting it as a character array and returns it's length.
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Length(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_INT32* iStrLength);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Length_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_ReadStringLength(ngroup, nkey, iStrLength, SIDEX_DATA_TYPE_STRING, idUNICODE_ASCII);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Length_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = ((sidexCom*)shandle)->sidexcom_ReadStringLength(utf8Group, utf8Key, iStrLength, SIDEX_DATA_TYPE_STRING, idUNICODE_WCHAR_T);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Length_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = ((sidexCom*)shandle)->sidexcom_ReadStringLength(utf8Group, utf8Key, iStrLength, SIDEX_DATA_TYPE_STRING, idUNICODE_CHAR16_T);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Reads content of the provided group / key, interpreting it as a character array.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, unsigned char** value, SIDEX_INT32* iStrLength);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, unsigned char** value, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_ReadString(ngroup, nkey, (char**) value, iStrLength, SIDEX_DATA_TYPE_BINARY);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, unsigned char** value, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        unsigned char* sStr;
        iRet = sidex_Binary_Read_A(shandle, utf8Group, utf8Key, &sStr, iStrLength);
        delete utf8Key;
        if (SIDEX_SUCCESS == iRet){
          *value = sStr;
        }
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, unsigned char** value, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        unsigned char* sStr;
        iRet = sidex_Binary_Read_A(shandle, utf8Group, utf8Key, &sStr, iStrLength);
        delete utf8Key;
        if (SIDEX_SUCCESS == iRet){
          *value = sStr;
        }
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/// @brief   Reads content of the provided group / key, interpreting it as a binary character array and returns it's length.
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Length(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_INT32* iStrLength);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Length_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_ReadStringLength(ngroup, nkey, iStrLength, SIDEX_DATA_TYPE_BINARY, idUNICODE_ASCII);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Length_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = ((sidexCom*)shandle)->sidexcom_ReadStringLength(utf8Group, utf8Key, iStrLength, SIDEX_DATA_TYPE_BINARY, idUNICODE_WCHAR_T);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Length_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_INT32* iStrLength){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = ((sidexCom*)shandle)->sidexcom_ReadStringLength(utf8Group, utf8Key, iStrLength, SIDEX_DATA_TYPE_BINARY, idUNICODE_CHAR16_T);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Free the memory for the string returned by sidex_String_Read().
 */
SIDEX_API void DLL_CALL_CONV sidex_Free_ReadString(SIDEX_CTSTR* string);
/**
 * char* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Free_ReadString_A(char* string){
  try{
    if (NULL != string)
      delete []string;
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Free_ReadString_A", "exception", "caught", 66);
  }
}
/**
 * wchar_t* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Free_ReadString_X(wchar_t* string){
  try{
    if (NULL != string)
      delete string;
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Free_ReadString_X", "exception", "caught", 66);
  }
}
/**
 * wchar_t* API
**/
SIDEX_API void DLL_CALL_CONV sidex_Free_ReadString_W(char16_t* string){
  try{
    if (NULL != string)
      delete string;
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Free_ReadString_W", "exception", "caught", 66);
  }
}


/**
 * @brief   Free the memory for the binary string returned by sidex_Binary_Read().
 */
SIDEX_API void DLL_CALL_CONV sidex_Free_Binary_ReadString(unsigned char* string){
  try{
    if (NULL != string)
      delete[] string;
  }
  catch (...){
    sidex_log(0xFFFFFFFF, "sidex", "sidex_Free_Binary_ReadString", "exception", "caught", 66);
  }
}


/**
 * @brief   Sets a NULL terminated char array as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, const SIDEX_CTSTR* value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, const char* value){

  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    // To debug PHP
    //sidex_log(0xFFFFFFFF, "sidex", "sidex_String_Write", "", "");
    SIDEX_INT32 iStrLen = 0;
    if (NULL != value){
      iStrLen = (SIDEX_INT32) strlen(value);
    }
    iRet = ((sidexCom*)shandle)->sidexcom_WriteString(ngroup, nkey, value, iStrLen, SIDEX_ENCODE_NONE, SIDEX_STRING_FORMAT_UNKNOWN);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, const wchar_t* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8 =0;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        if (NULL != value){
          char* utf8Val = UTF32toUTF8((wchar_t*)value, &iLengthUtf8);
          iRet = sidex_String_Write_A(shandle, utf8Group, utf8Key, utf8Val);
          if (NULL != utf8Val){
            delete utf8Val;
          }
        }
        else{
          // NULL- String
          iRet = sidex_String_Write_A(shandle, utf8Group, utf8Key, NULL);
        }
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_String_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, const char16_t* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8 =0;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        if (NULL != value){
          char* utf8Val = UTF16toUTF8((wchar_t*)value, &iLengthUtf8);
          iRet = sidex_String_Write_A(shandle, utf8Group, utf8Key, utf8Val);
          if (NULL != utf8Val){
            delete utf8Val;
          }
        }
        else{
          // NULL- String
          iRet = sidex_String_Write_A(shandle, utf8Group, utf8Key, NULL);
        }
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}  

/**
/// @brief   Sets a binary char array as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, const unsigned char* value, SIDEX_INT32 size);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, const unsigned char* value, SIDEX_INT32 size){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_WriteString(ngroup, nkey, (char*) value, size, SIDEX_ENCODE_BASE64, SIDEX_STRING_FORMAT_UNKNOWN);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, const unsigned char* value, SIDEX_INT32 size){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Binary_Write_A(shandle, utf8Group, utf8Key, value, size);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Binary_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, const unsigned char* value, SIDEX_INT32 size){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Binary_Write_A(shandle, utf8Group, utf8Key, value, size);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Reads content of the provided group / key, interpreting it as a List value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    SIDEX_VARIANT variant;
    iRet = ((sidexCom*)shandle)->sidexcom_ReadVariant(ngroup, nkey, &variant);
    if (SIDEX_SUCCESS == iRet){
       int type = ((sidexBase*)(variant))->getType();
       if (type == SIDEX_DATA_TYPE_LIST){
         *value = variant;
       }
       else{
         iRet = SIDEX_ERR_WRONG_TYPE;
         sidexutil_Variant_DecRef (variant);
       }
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_List_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_List_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Sets a List value as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (SIDEX_TRUE == sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_LIST)){
      iRet = ((sidexCom*)shandle)->sidexcom_WriteList(ngroup, nkey, (sidexList*)value);
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_List_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_List_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_List_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Reads content of the provided group / key, interpreting it as a Dict value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    SIDEX_VARIANT variant;
    iRet = ((sidexCom*)shandle)->sidexcom_ReadVariant(ngroup, nkey, &variant);
    if (SIDEX_SUCCESS == iRet){
       int type = ((sidexBase*)(variant))->getType();
       if (type == SIDEX_DATA_TYPE_DICT){
         *value = variant;
       }
       else{
         iRet = SIDEX_ERR_WRONG_TYPE;
         sidexutil_Variant_DecRef (variant);
       }
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Dict_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Dict_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Sets a Dict value as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT value);
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (SIDEX_TRUE == sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_DICT)){
      iRet = ((sidexCom*)shandle)->sidexcom_WriteDict(ngroup, nkey, (sidexDict*) value);
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Dict_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Dict_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Dict_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Reads content of the provided group / key, interpreting it as a Table value.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Read(SIDEX_HANDLE shandle,  SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT* value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Read_A(SIDEX_HANDLE shandle,  char* ngroup, char* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    SIDEX_VARIANT variant;
    iRet = ((sidexCom*)shandle)->sidexcom_ReadVariant(ngroup, nkey, &variant);
    if (SIDEX_SUCCESS == iRet){
       int type = ((sidexBase*)(variant))->getType();
       if (type == SIDEX_DATA_TYPE_TABLE){
         *value = variant;
       }
       else{
         iRet = SIDEX_ERR_WRONG_TYPE;
         sidexutil_Variant_DecRef (variant);
       }
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Read_X(SIDEX_HANDLE shandle,  wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Table_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Read_W(SIDEX_HANDLE shandle,  char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT* value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Table_Read_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Sets a Table value as content of the provided group / key.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Write(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey, SIDEX_VARIANT value);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Write_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    if (SIDEX_TRUE == sidexutil_dataTypeCheck(value, SIDEX_DATA_TYPE_TABLE)){
      iRet = ((sidexCom*)shandle)->sidexcom_WriteTable(ngroup, nkey, (sidexTable*)value);
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Write_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Table_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Table_Write_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey, SIDEX_VARIANT value){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_Table_Write_A(shandle, utf8Group, utf8Key, value);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}  

/**
 * @brief   Returns the group name List of the provided handle.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetGroups(SIDEX_HANDLE shandle, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_GetGroups(variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Check for the existence of a group.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasGroup(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup);
/**
 * char* API
**/
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasGroup_A(SIDEX_HANDLE shandle, char* ngroup){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_HasGroup(ngroup);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasGroup_X(SIDEX_HANDLE shandle, wchar_t* ngroup){
  SIDEX_BOOL bRet = SIDEX_FALSE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      bRet = sidex_HasGroup_A(shandle, utf8Group);
      delete utf8Group;
    }
  }
  catch (...){
    // Set result to SIDEX_FALSE on an exception
    bRet = SIDEX_FALSE;
  }
  return bRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasGroup_W(SIDEX_HANDLE shandle, char16_t* ngroup){
  SIDEX_BOOL bRet = SIDEX_FALSE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      bRet = sidex_HasGroup_A(shandle, utf8Group);
      delete utf8Group;
    }
  }
  catch (...){
    // Set result to SIDEX_FALSE on an exception
    bRet = SIDEX_FALSE;
  }
  return bRet;
}   

/**
 * @brief   Returns the key name List of the provided handle and group.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetKeys(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_VARIANT* variant);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetKeys_A(SIDEX_HANDLE shandle, char* ngroup, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_GetKeys(ngroup, variant);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetKeys_X(SIDEX_HANDLE shandle, wchar_t* ngroup, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      iRet = sidex_GetKeys_A(shandle, utf8Group, variant);
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_GetKeys_W(SIDEX_HANDLE shandle, char16_t* ngroup, SIDEX_VARIANT* variant){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      iRet = sidex_GetKeys_A(shandle, utf8Group, variant);
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Check for the existence of a key in a group.
 */
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasKey(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey);
/**
 * char* API
**/
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasKey_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_HasKey(ngroup, nkey);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasKey_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey){
  SIDEX_BOOL bRet = SIDEX_FALSE;
  try{
    SIDEX_INT32 iLengthUtf8;
    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        bRet = sidex_HasKey_A(shandle, utf8Group, utf8Key);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    // SIDEX_FALSE on exception
    bRet = SIDEX_FALSE;
  }

  return bRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_BOOL DLL_CALL_CONV sidex_HasKey_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey){
  SIDEX_BOOL bRet = SIDEX_FALSE;
  try{
    SIDEX_INT32 iLengthUtf8;
    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        bRet = sidex_HasKey_A(shandle, utf8Group, utf8Key);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    // SIDEX_FALSE on exception
    bRet = SIDEX_FALSE;
  }

  return bRet;
}   

/**
 * @brief   Remove the group and it's content out of a provided handle.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteGroup(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteGroup_A(SIDEX_HANDLE shandle, char* ngroup){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_DeleteGroup(ngroup);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteGroup_X(SIDEX_HANDLE shandle, wchar_t* ngroup){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      iRet = sidex_DeleteGroup_A(shandle, utf8Group);
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteGroup_W(SIDEX_HANDLE shandle, char16_t* ngroup){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      iRet = sidex_DeleteGroup_A(shandle, utf8Group);
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   

/**
 * @brief   Remove the key and it's content out of the provided handle and group.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteKey(SIDEX_HANDLE shandle, SIDEX_CTSTR* ngroup, SIDEX_CTSTR* nkey);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteKey_A(SIDEX_HANDLE shandle, char* ngroup, char* nkey){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)shandle)->sidexcom_DeleteKey(ngroup, nkey);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteKey_X(SIDEX_HANDLE shandle, wchar_t* ngroup, wchar_t* nkey){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_DeleteKey_A(shandle, utf8Group, utf8Key);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_DeleteKey_W(SIDEX_HANDLE shandle, char16_t* ngroup, char16_t* nkey){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)ngroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nkey, &iLengthUtf8);
      if (NULL != utf8Key){
        iRet = sidex_DeleteKey_A(shandle, utf8Group, utf8Key);
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * @brief  Make debug logs.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_log(SIDEX_INT32 iLogMask, const SIDEX_CTSTR* sClass, const SIDEX_CTSTR* sMethod, const SIDEX_CTSTR* sFormatLog, const SIDEX_CTSTR* sLog, SIDEX_INT32 iLogFileIndex);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_log_A(SIDEX_INT32 iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog, SIDEX_INT32 iLogFileIndex){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    sidexLogHandler* log = new sidexLogHandler((char*)"a", (int)iLogFileIndex);
    log->setLoggingValue((int)iLogMask);
    log->log ((int)iLogMask, sClass, sMethod, sFormatLog, sLog);
    delete log;
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_log_X(SIDEX_INT32 iLogMask, const wchar_t* sClass, const wchar_t* sMethod, const wchar_t* sFormatLog, const wchar_t* sLog, SIDEX_INT32 iLogFileIndex){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Class = UTF32toUTF8((wchar_t*)sClass, &iLengthUtf8);
    if (NULL != utf8Class){
      char* utf8Method = UTF32toUTF8((wchar_t*)sMethod, &iLengthUtf8);
      if (NULL != utf8Method){
        char* utf8Format = UTF32toUTF8((wchar_t*)sFormatLog, &iLengthUtf8);
        if (NULL != utf8Format){
          char* utf8sLog = UTF32toUTF8((wchar_t*)sLog, &iLengthUtf8);
          if (NULL != utf8sLog){
            iRet = sidex_log_A(iLogMask, utf8Class, utf8Method, utf8Format, utf8sLog, iLogFileIndex);
            delete utf8sLog;
          }
          delete utf8Format;
        }
        delete utf8Method;
      }
      delete utf8Class;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_log_W(SIDEX_INT32 iLogMask, const char16_t* sClass, const char16_t* sMethod, const char16_t* sFormatLog, const char16_t* sLog, SIDEX_INT32 iLogFileIndex){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Class = UTF16toUTF8((wchar_t*)sClass, &iLengthUtf8);
    if (NULL != utf8Class){
      char* utf8Method = UTF16toUTF8((wchar_t*)sMethod, &iLengthUtf8);
      if (NULL != utf8Method){
        char* utf8Format = UTF16toUTF8((wchar_t*)sFormatLog, &iLengthUtf8);
        if (NULL != utf8Format){
          char* utf8sLog = UTF16toUTF8((wchar_t*)sLog, &iLengthUtf8);
          if (NULL != utf8sLog){
            iRet = sidex_log_A(iLogMask, utf8Class, utf8Method, utf8Format, utf8sLog, iLogFileIndex);
            delete utf8sLog;
          }
          delete utf8Format;
        }
        delete utf8Method;
      }
      delete utf8Class;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Merge a SIDEX_HANDLE into another one.
 */
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Merge(SIDEX_HANDLE sBaseHandle, SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, SIDEX_CTSTR* nGroup, SIDEX_CTSTR* nkey);
/**
 * char* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Merge_A(SIDEX_HANDLE sBaseHandle, SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, char* nGroup, char* nkey){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;
  try{
    iRet = ((sidexCom*)sBaseHandle)->sidexcom_Merge(sMergeHandle, bOverwrite, nGroup, nkey);
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Merge_X(SIDEX_HANDLE sBaseHandle, SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, wchar_t* nGroup, wchar_t* nKey){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF32toUTF8(nGroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF32toUTF8(nKey, &iLengthUtf8);
      iRet = sidex_Merge_A(sBaseHandle, sMergeHandle, bOverwrite, utf8Group, utf8Key);
      if (NULL != utf8Key){
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   
/**
 * char16_t* API
**/
SIDEX_API SIDEX_INT32 DLL_CALL_CONV sidex_Merge_W(SIDEX_HANDLE sBaseHandle, SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, char16_t* nGroup, char16_t* nKey){
  SIDEX_INT32 iRet = SIDEX_ERR_UNICODE;
  try{
    SIDEX_INT32 iLengthUtf8;

    char* utf8Group = UTF16toUTF8((wchar_t*)nGroup, &iLengthUtf8);
    if (NULL != utf8Group){
      char* utf8Key = UTF16toUTF8((wchar_t*)nKey, &iLengthUtf8);
      iRet = sidex_Merge_A(sBaseHandle, sMergeHandle, bOverwrite, utf8Group, utf8Key);
      if (NULL != utf8Key){
        delete utf8Key;
      }
      delete utf8Group;
    }
  }
  catch (...){
    iRet = SIDEX_ERR_COMMON;
  }
  return iRet;
}   