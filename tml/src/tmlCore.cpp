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

/** @file tmlCore.cpp
  * 
  * @brief Definitions and dll import file for the TMLCore Object API
  *
  * To use the TMLCore interface library add the tmlCore.h, tmlGlobalDefines.h, tmlErrors.h and tmlCore.lib into your project and take
  * care to have the libvortex-1.1.dll, libaxl.dll and the tmlCore.dll in your path.
  */

#include "tmlCore.h"
#include "tmlObjWrapper.h"
#include "tmlCoreWrapper.h"
#include "tmlErrors.h"
#include "tmlCoreDefines.h"
#include "unicode.h"
#include "logValues.h"

////////////////////////////////////
// Members for logging
tmlLogHandler* m_log = NULL;
tmlCriticalSectionObj* c_csObj = NULL;
static int m_iLogIndex = 1;

/**
 * Global attribute to store the vortex execution context
 */
//VortexCtx* m_ctx = NULL;

/**
 * @brief    global attribute to store a converted version request
 */
wchar_t*     m_cVersionX = SIDEX_HANDLE_TYPE_NULL;
char16_t*    m_cVersionW = SIDEX_HANDLE_TYPE_NULL;

/**
 * @brief    global attribute to store a converted copyright request
 */
wchar_t*     m_cCopyrightX = TML_HANDLE_TYPE_NULL;
TML_INT32    m_iLengthCopyrightX = 0;
char16_t*    m_cCopyrightW = TML_HANDLE_TYPE_NULL;
TML_INT32    m_iLengthCopyrightW = 0;

/**
 * Global attribute to store the vortex INITIALIZATION status
 */
//int m_status = TML_SUCCESS;

int m_iLogFileIndex = -1;

/**
 * @brief    global attribute to stores the license flag
 */
SIDEX_BOOL m_bLicensed = SIDEX_TRUE;

/**
  * @brief    global attribute  / initial thread pool size
  */
TML_INT32 m_iInitialThreadPoolSize = INITIAL_THREAD_POOL_SIZE;

/**
  * @brief    global attribute  / maximal thread pool size
  */
TML_INT32 m_iThreadPoolMaxSize = INITIAL_THREAD_POOL_MAX_SIZE;

/**
  * @brief    global attribute  / thread pool add step
  */
TML_INT32 m_iThreadAddSteps = INITIAL_THREAD_ADD_STEPS;

/**
  * @brief    global attribute  / thread pool add period
  */
TML_INT32 m_iThreadPoolAddPeriod = INITIAL_THREAD_POOL_ADD_PERIOD;

/**
  * @brief    global attribute  / thread pool remove period
  */
TML_INT32 m_iThreadPoolRemovePeriod = INITIAL_THREAD_POOL_REMOVE_PERIOD;

/**
  * @brief    global attribute  / thread pool remove step
  */
TML_INT32 m_iThreadRemoveSteps = INITIAL_THREAD_REMOVE_STEPS;

/**
  * @brief    global attribute  / auto remove thread pool
  */
TML_BOOL m_bThreadAutoRemove = INITIAL_THREAD_AUTO_REMOVE;
/**
  * @brief    global attribute  / preemptive thread pool
  */
TML_BOOL m_bThreadPreemptive = INITIAL_THREAD_PREEMPTIVE;

/**
 * @brief    Returns current interface api and library version.
 */
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Version(TML_INT32* apiVer, TML_INT32* libVer, SIDEX_CTSTR** verStr);
/**
 * char* API
**/
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Version_A(TML_INT32* apiVer, TML_INT32* libVer, char** verStr){
  *apiVer = API_VERSION_NUMBER;
  *libVer = LIB_VERSION_NUMBER;
  *verStr = LIB_VERSION_STRING_A;
};
/**
 * wchar_t* API
**/
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Version_X(TML_INT32* apiVer, TML_INT32* libVer, wchar_t** verStr){
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
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Version_W(TML_INT32* apiVer, TML_INT32* libVer, char16_t** verStr){
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
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Copyright(TML_CTSTR** sValue, TML_INT32* iLength);
/**
 * char* API
**/
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Copyright_A(char** sValue, TML_INT32* iLength){
  *iLength = (TML_INT32)strlen(TML_STRING_COPYRIGHT_A);
  *sValue = TML_STRING_COPYRIGHT_A;
};
/**
 * wchar_t* API
**/
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Copyright_X(wchar_t** sValue, TML_INT32* iLength){
  if (TML_HANDLE_TYPE_NULL != m_cCopyrightX){
    *sValue = m_cCopyrightX;
    *iLength = m_iLengthCopyrightX;
  }
  else{
    m_cCopyrightX = UTF8toUTF32(TML_STRING_COPYRIGHT_A, &m_iLengthCopyrightX);
    *sValue = m_cCopyrightX;
    *iLength = m_iLengthCopyrightX;
  }
};
/**
 * char16_t* API
**/
TML_CORE_API void DLL_CALL_CONV tml_Core_Get_Copyright_W(char16_t** sValue, TML_INT32* iLength){
  if (TML_HANDLE_TYPE_NULL != m_cCopyrightW){
    *sValue = m_cCopyrightW;
    *iLength = m_iLengthCopyrightW;
  }
  else{
    m_cCopyrightW = (char16_t*)UTF8toUTF16(TML_STRING_COPYRIGHT_A, &m_iLengthCopyrightW);
    *sValue = m_cCopyrightW;
    *iLength = m_iLengthCopyrightW;
  }
};


/**
 * @brief    Set the user name and password to get licensed product access
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_Password(const TML_CTSTR* pUserName, const TML_CTSTR* pPassWord);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_Password_A(const char *pUserName, const char *pPassWord){
  // Note: Method has been left in the API to be downward compatible.
  TML_INT32 iRet = TML_SUCCESS;
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 tml_Core_Set_Password_X(const wchar_t* pUserName, const wchar_t* pPassWord){
  // Note: Method has been left in the API to be downward compatible.
  TML_INT32 iRet = TML_SUCCESS;
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 tml_Core_Set_Password_W(const char16_t* pUserName, const char16_t* pPassWord){
  // Note: Method has been left in the API to be downward compatible.
  TML_INT32 iRet = TML_SUCCESS;
  return iRet;
}


/**
  * @brief    Configure global listener thread pool handling.
  */
TML_CORE_API void DLL_CALL_CONV tml_Configure_Thread_Pool_Handling(TML_INT32 iInitialThreadPoolSize, TML_INT32 iThreadPoolMaxSize,
                                                        TML_INT32 iThreadAddSteps, TML_INT32 iThreadPoolAddPeriod,
                                                        TML_INT32 iThreadRemoveSteps, TML_INT32 iThreadPoolRemovePeriod, 
                                                        TML_BOOL bThreadAutoRemove, TML_BOOL bThreadPreemptive)
{
  m_iInitialThreadPoolSize  = iInitialThreadPoolSize;
  m_iThreadPoolMaxSize      = iThreadPoolMaxSize;
  m_iThreadAddSteps         = iThreadAddSteps;
  m_iThreadPoolAddPeriod    = iThreadPoolAddPeriod;
  m_iThreadRemoveSteps      = iThreadRemoveSteps;
  m_iThreadPoolRemovePeriod = iThreadPoolRemovePeriod;
  m_bThreadAutoRemove       = bThreadAutoRemove;
  m_bThreadPreemptive       = bThreadPreemptive;
}


/**
 * @brief   Open an instance of a TMLCore.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Open(TML_CORE_HANDLE* coreHandle, TML_INT32 iLogValue){
  // To debug PHP
  //iLogValue = 0x10;
  //DebugBreak();
  TML_INT32 iRet;
  try{
    //tmlCoreWrapper* obj = new tmlCoreWrapper(m_ctx, iLogValue, m_bLicensed);
    tmlCoreWrapper* obj = new tmlCoreWrapper(NULL, (int)iLogValue,
                                              m_iInitialThreadPoolSize, m_iThreadPoolMaxSize, 
                                              m_iThreadAddSteps, m_iThreadPoolAddPeriod, 
                                              m_iThreadRemoveSteps, m_iThreadPoolRemovePeriod, 
                                              m_bThreadAutoRemove, m_bThreadPreemptive);
    m_iLogFileIndex = obj->getLogFileIndex();
    *coreHandle = (TML_CORE_HANDLE) obj;
    iRet = (TML_INT32)obj->tmlCoreWrapper_Set_Logging_Value((int)iLogValue);
    obj->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Open");
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief  Dismiss the instance of a TMLCore.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Close(TML_CORE_HANDLE* coreHandle){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)*coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        sidex_Free_ReadString_W(m_cVersionW);
        sidex_Free_ReadString_W(m_cCopyrightW);
        sidex_Free_ReadString_X(m_cVersionX);
        sidex_Free_ReadString_X(m_cCopyrightX);
        m_cVersionW = SIDEX_HANDLE_TYPE_NULL;
        m_cCopyrightW = SIDEX_HANDLE_TYPE_NULL;
        m_cVersionX = SIDEX_HANDLE_TYPE_NULL;
        m_cCopyrightX = SIDEX_HANDLE_TYPE_NULL;
        ((tmlCoreWrapper*)*coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Close");
        /////////////////////////////////////
        // SignalShutdown is on process:
        ((tmlCoreWrapper*)*coreHandle)->tmlCoreWrapper_SetShutdown (true);
        // Do make the cast to (tmlCoreWrapper*) / In that case the delete will call the destructor automatically via the scalar destructor:
        delete ((tmlCoreWrapper*)*coreHandle);
        *coreHandle = TML_HANDLE_TYPE_NULL;
      }
    }
    catch (...){
      printf ("### exception caught in \"tml_Core_Close\" ####\n");
      iRet = TML_ERR_COMMON;
    }
  }
  if (NULL != m_log){
    delete m_log;
    m_log = NULL;
  }
  if (NULL != c_csObj){
    delete c_csObj;
    c_csObj = NULL;
  }
  return iRet;
}


/**
 * @brief  Make all deregistrations / sometimes useful before closing.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_GeneralDeregistration(TML_CORE_HANDLE coreHandle){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_GeneralDeregistration");
    try{
      ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_General_Deregistration();
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief   Changes the callback method for thread creation.
 */
TML_CORE_API void DLL_CALL_CONV tml_Core_Thread_Set_OnCreate(void* pCBCreate){
  try{
    /*
    {//if (0 <= m_iLogFileIndex){
      // To debug PHP
      if (NULL != pCBCreate)
        tml_log((TML_INT32)0xFFFFFF, "tml_Core_Thread_Set_OnCreate", "Value", "", "");
      else
        tml_log((TML_INT32)0xFFFFFF, "tml_Core_Thread_Set_OnCreate", "NULL", "", "");
    }
    */
    vortex_thread_set_create ((VortexThreadCreateFunc)pCBCreate);
  }
  catch (...){
  }
}


/**
 * @brief   Changes the callback method for thread destruction.
 */
TML_CORE_API void DLL_CALL_CONV tml_Core_Thread_Set_OnDestroy(void* pCBDestroy){
  try{
  /*
  {//if (0 <= m_iLogFileIndex){
      // To debug PHP
      if (NULL != pCBDestroy)
        tml_log((TML_INT32)0xFFFFFF, "tml_Core_Thread_Set_OnDestroy", "Value", "", "");
      else
        tml_log((TML_INT32)0xFFFFFF, "tml_Core_Thread_Set_OnDestroy", "NULL", "", "");
    }
    */
    vortex_thread_set_destroy ((VortexThreadDestroyFunc)pCBDestroy);
  }
  catch (...){
  }
}


/**
 * @brief Helper method referring the registered tml_Core_Thread_Set_OnCreate() callback method.
 */
TML_CORE_API void DLL_CALL_CONV tml_Sys_Free(TML_POINTER ptr){
  axl_free(ptr);
}


/**
 * @brief   Register provided profile for command dispatch actions.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_A(TML_CORE_HANDLE coreHandle, const char* profile){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == profile){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Profile_Register");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Register_Profile(profile);
        if (TML_SUCCESS == iRet){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Register_A of profile", profile, " succeeded");
        }
        else{
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Register_A of profile", profile, " failed !");
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == profile){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
      if (NULL != utf8ProfileName){
        iRet = tml_Profile_Register_A(coreHandle, utf8ProfileName);
        if (TML_SUCCESS == iRet){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Register_W of profile", utf8ProfileName, " succeeded");
        }
        else{
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Register_W of profile", utf8ProfileName, " failed !");
        }
        delete utf8ProfileName;
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_W(TML_CORE_HANDLE coreHandle, const char16_t* profile){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == profile){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
      if (NULL != utf8ProfileName){
        iRet = tml_Profile_Register_A(coreHandle, utf8ProfileName);
        if (TML_SUCCESS == iRet){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Register_W of profile", utf8ProfileName, " succeeded");
        }
        else{
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Register_W of profile", utf8ProfileName, " failed !");
        }
        delete utf8ProfileName;
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Deregister provided profile for command dispatch actions. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Unregister(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Unregister_A(TML_CORE_HANDLE coreHandle, const char* profile){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Profile_Unregister");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Unregister_Profile(profile);
        if (TML_SUCCESS == iRet){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Unregister_A of profile", profile, " succeeded");
        }
        else{
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Unregister_A of profile", profile, " failed !");
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Unregister_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
  if (NULL != utf8ProfileName){
    try{
      iRet = tml_Profile_Unregister_A(coreHandle, utf8ProfileName);
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Unregister_W of profile", utf8ProfileName, " succeeded");
      }
      else{
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Unregister_W of profile", utf8ProfileName, " failed !");
      }
      delete utf8ProfileName;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Unregister_W(TML_CORE_HANDLE coreHandle, const char16_t* profile){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
  if (NULL != utf8ProfileName){
    try{
      iRet = tml_Profile_Unregister_A(coreHandle, utf8ProfileName);
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Unregister_W of profile", utf8ProfileName, " succeeded");
      }
      else{
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "tml_Profile_Unregister_W of profile", utf8ProfileName, " failed !");
      }
      delete utf8ProfileName;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Get the number of profiles which have been registered for command dispatch actions.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_Registered_Count(TML_CORE_HANDLE coreHandle, TML_INT32* iSize){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Profile_Get_Registered_Count");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_Registered_Profiles_Size(iSize);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief   Get the list of profiles which have been registered for command dispatch actions.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_Registered(TML_CORE_HANDLE coreHandle, SIDEX_VARIANT* profiles){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Profile_Get_Registered");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_Registered_Profiles(profiles);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief   Check if provided profile is registered for command dispatch actions.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_RegisterState(TML_CORE_HANDLE coreHandle, TML_CTSTR* profile, TML_BOOL* bRegistered);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_RegisterState_A(TML_CORE_HANDLE coreHandle, char* profile, TML_BOOL* bRegistered){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        bool bRet;
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Profile_Get_RegisterState");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Is_Profile_Registered(profile, &bRet);
        if (TML_SUCCESS == iRet){
          if (bRet){
            *bRegistered = TML_TRUE;
          }
          else{
            *bRegistered = TML_FALSE;
          }
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_RegisterState_X(TML_CORE_HANDLE coreHandle, wchar_t* profile, TML_BOOL* bRegistered){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
  if (NULL != utf8ProfileName){
    try{
      iRet = tml_Profile_Get_RegisterState_A(coreHandle, utf8ProfileName, bRegistered);
      delete utf8ProfileName;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Get_RegisterState_W(TML_CORE_HANDLE coreHandle, char16_t* profile, TML_BOOL* bRegistered){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
  if (NULL != utf8ProfileName){
    try{
      iRet = tml_Profile_Get_RegisterState_A(coreHandle, utf8ProfileName, bRegistered);
      delete utf8ProfileName;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Register a callback method for a command dispatch action.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_Cmd(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_Cmd_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Profile_Register_Cmd");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Register_Cmd(profile, cmd, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_Cmd_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
  if (NULL != utf8ProfileName){
    try{
      iRet = tml_Profile_Register_Cmd_A(coreHandle, utf8ProfileName, cmd, pCBFunc, pCBData);
      delete utf8ProfileName;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Register_Cmd_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_COMMAND_ID_TYPE cmd, TML_ON_CMD_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
  if (NULL != utf8ProfileName){
    try{
      iRet = tml_Profile_Register_Cmd_A(coreHandle, utf8ProfileName, cmd, pCBFunc, pCBData);
      delete utf8ProfileName;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Register a callback method that will be invoked in the case of deregistration from dispatch action.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnDeleteCmd(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnDeleteCmd_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == profile){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Profile_Set_OnDeleteCmd");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_OnDeleteCmd(profile, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnDeleteCmd_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == profile){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
      if (NULL != utf8ProfileName){
        iRet = tml_Profile_Set_OnDeleteCmd_A(coreHandle, utf8ProfileName, pCBFunc, pCBData);
        delete utf8ProfileName;
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnDeleteCmd_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_CMD_DELETION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == profile){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
      if (NULL != utf8ProfileName){
        iRet = tml_Profile_Set_OnDeleteCmd_A(coreHandle, utf8ProfileName, pCBFunc, pCBData);
        delete utf8ProfileName;
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Register a callback method for non registered dispatch commands.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnCustomDispatch(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnCustomDispatch_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Profile_Set_OnCustomDispatch");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_OnCustomDispatch(profile, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnCustomDispatch_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
  if (NULL != utf8ProfileName){
    try{
      iRet = tml_Profile_Set_OnCustomDispatch_A(coreHandle, utf8ProfileName, pCBFunc, pCBData);
      delete utf8ProfileName;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Profile_Set_OnCustomDispatch_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iLengthUtf8;

  char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
  if (NULL != utf8ProfileName){
    try{
      iRet = tml_Profile_Set_OnCustomDispatch_A(coreHandle, utf8ProfileName, pCBFunc, pCBData);
      delete utf8ProfileName;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief  Set the port number for the listener.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerPort(TML_CORE_HANDLE coreHandle, TML_CTSTR* sPort);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerPort_A(TML_CORE_HANDLE coreHandle, char* sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == sPort){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd:tml_Core_Set_ListenerPort", sPort);
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_ListenerPort_A(sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerPort_X(TML_CORE_HANDLE coreHandle, wchar_t* sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == sPort){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd:tml_Core_Set_ListenerPort", sPort);
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_ListenerPort_X(sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerPort_W(TML_CORE_HANDLE coreHandle, char16_t* sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == sPort){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd:tml_Core_Set_ListenerPort", sPort);
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_ListenerPort_W(sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Get the port number for the listener.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerPort(TML_CORE_HANDLE coreHandle, TML_CTSTR** sPort);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerPort_A(TML_CORE_HANDLE coreHandle, char** sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_ListenerPort");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_ListenerPort_A(sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerPort_X(TML_CORE_HANDLE coreHandle, wchar_t** sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_ListenerPort");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_ListenerPort_X(sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerPort_W(TML_CORE_HANDLE coreHandle, char16_t** sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_ListenerPort");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_ListenerPort_W(sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Set the IP number for the listener.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerIP(TML_CORE_HANDLE coreHandle, TML_CTSTR* sIP);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerIP_A(TML_CORE_HANDLE coreHandle, char* sIP){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == sIP){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd:tml_Core_Set_ListenerIP", sIP);
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_ListenerIP_A(sIP);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerIP_X(TML_CORE_HANDLE coreHandle, wchar_t* sIP){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == sIP){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd:tml_Core_Set_ListenerIP", sIP);
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_ListenerIP_X(sIP);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerIP_W(TML_CORE_HANDLE coreHandle, char16_t* sIP){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle || TML_HANDLE_TYPE_NULL == sIP){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd:tml_Core_Set_ListenerIP", sIP);
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_ListenerIP_W(sIP);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Get the IP number for the listener.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerIP(TML_CORE_HANDLE coreHandle, TML_CTSTR** sIP);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerIP_A(TML_CORE_HANDLE coreHandle, char** sIP){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_ListenerIP");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_ListenerIP_A(sIP);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerIP_X(TML_CORE_HANDLE coreHandle, wchar_t** sIP){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_ListenerIP");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_ListenerIP_X(sIP);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerIP_W(TML_CORE_HANDLE coreHandle, char16_t** sIP){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_ListenerIP");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_ListenerIP_W(sIP);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief   Enable / Disable the listener for incoming data.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_ListenerEnabled(TML_CORE_HANDLE coreHandle, TML_BOOL bEnable){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        if (TML_FALSE != bEnable){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd:tml_Core_Set_ListenerEnabled", "true");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Enable_Listener(true);
        }
        else{
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd:tml_Core_Set_ListenerEnabled", "false");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Enable_Listener(false);
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief   Returns the listener enable status.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_ListenerEnabled(TML_CORE_HANDLE coreHandle, TML_BOOL* bEnable){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_ListenerEnabled");
        bool bRet;
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Is_Listener_Enabled(&bRet);
        if (TML_SUCCESS == iRet){
          if (bRet){
            *bEnable = TML_TRUE;
          }
          else{
            *bEnable = TML_FALSE;
          }
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief   Set the window size for the following outgoing data.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_WindowSize(TML_CORE_HANDLE coreHandle, TML_INT32 iWindowSize){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Set_WindowSize");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_WindowSize((int)iWindowSize);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief   Get the window size for the following outgoing data.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_WindowSize(TML_CORE_HANDLE coreHandle, TML_INT32* iWindowSize){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_WindowSize");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_WindowSize(iWindowSize);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 *  @brief  Set the debug logging value.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Set_LoggingValue(TML_CORE_HANDLE coreHandle, TML_INT32 iLogValue){
  // To debug PHP
  //iLogValue = 0x10;

  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Set_LoggingValue");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_Logging_Value((int)iLogValue);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief   Returns the debug logging value.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Core_Get_LoggingValue(TML_CORE_HANDLE coreHandle, TML_INT32* iLogValue){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Core_Get_LoggingValue");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Get_Logging_Value(iLogValue);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Send a synchron message / command.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_SyncMessage(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort, TML_UINT32 iTimeout);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_SyncMessage_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile, const char* sHost, const char* sPort, TML_UINT32 iTimeout)
{
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Send_SyncMessage", "begin", tmlhandle);
#endif //TIMEOUT_LOGGING
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Send_SyncMessage");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SendSyncMessage(tmlhandle, profile, sHost, sPort, (unsigned int)iTimeout);
      }
    }
    catch (...){
      try{
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Send_SyncMessage");
        m_log->log (TML_LOG_EVENT, "TMLCore", "tml_Send_SyncMessage", "profile", profile);
        tml_log_A(TML_LOG_EVENT, "TMLCore", "tml_Send_SyncMessage", "profile", profile);
        m_log->log (TML_LOG_EVENT, "TMLCore", "tml_Send_SyncMessage", "Host", sHost);
        tml_log_A(TML_LOG_EVENT, "TMLCore", "tml_Send_SyncMessage", "Host", sHost);
        m_log->log (TML_LOG_EVENT, "TMLCore", "tml_Send_SyncMessage", "Port", sPort);
        tml_log_A(TML_LOG_EVENT, "TMLCore", "tml_Send_SyncMessage", "Port", sPort);
      }
      catch (...){
      }
      iRet = TML_ERR_COMMON;
    }
  }
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Send_SyncMessage", "end", tmlhandle);
#endif //TIMEOUT_LOGGING
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_SyncMessage_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iDebug = 0;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    iDebug = 1;
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF32toUTF8((wchar_t*)sHost, &iLengthUtf8);
      iDebug = 2;
      if (NULL != utf8HostName){
        char* utf8PortName = UTF32toUTF8((wchar_t*)sPort, &iLengthUtf8);
        iDebug = 3;
        if (NULL != utf8PortName){
          iRet = tml_Send_SyncMessage_A(coreHandle, tmlhandle, utf8ProfileName, utf8HostName, utf8PortName, iTimeout);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Send_SyncMessage_W", "EXCEPTION / DebugVal", iDebug);
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_SyncMessage_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iDebug = 0;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    iDebug = 1;
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF16toUTF8((wchar_t*)sHost, &iLengthUtf8);
      iDebug = 2;
      if (NULL != utf8HostName){
        char* utf8PortName = UTF16toUTF8((wchar_t*)sPort, &iLengthUtf8);
        iDebug = 3;
        if (NULL != utf8PortName){
          iRet = tml_Send_SyncMessage_A(coreHandle, tmlhandle, utf8ProfileName, utf8HostName, utf8PortName, iTimeout);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Send_SyncMessage_W", "EXCEPTION / DebugVal", iDebug);
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Send an asynchron message / command.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncMessage(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort, TML_UINT32 iTimeout);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncMessage_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile, const char* sHost, const char* sPort, TML_UINT32 iTimeout)
{
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Send_AsyncMessage", "begin", tmlhandle);
#endif //TIMEOUT_LOGGING
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Send_AsyncMessage");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SendAsyncMessage(tmlhandle, profile, sHost, sPort, (unsigned int) iTimeout, TMLCOM_MODE_ASYNC);
      }
    }
    catch (...){
      try{
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Send_AsyncMessage");
        m_log->log (TML_LOG_EVENT, "TMLCore", "tml_Send_AsyncMessage", "profile", profile);
        tml_log_A(TML_LOG_EVENT, "TMLCore", "tml_Send_AsyncMessage", "profile", profile);
        m_log->log (TML_LOG_EVENT, "TMLCore", "tml_Send_AsyncMessage", "Host", sHost);
        tml_log_A(TML_LOG_EVENT, "TMLCore", "tml_Send_AsyncMessage", "Host", sHost);
        m_log->log (TML_LOG_EVENT, "TMLCore", "tml_Send_AsyncMessage", "Port", sPort);
        tml_log_A(TML_LOG_EVENT, "TMLCore", "tml_Send_AsyncMessage", "Port", sPort);
      }
      catch (...){
      }
      iRet = TML_ERR_COMMON;
    }
  }
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Send_AsyncMessage", "end", tmlhandle);
#endif //TIMEOUT_LOGGING
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncMessage_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort, TML_UINT32 iTimeout){
  TML_INT32 iDebug = 0;
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    iDebug = 1;
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF32toUTF8((wchar_t*)sHost, &iLengthUtf8);
      iDebug = 2;
      if (NULL != utf8HostName){
        char* utf8PortName = UTF32toUTF8((wchar_t*)sPort, &iLengthUtf8);
        iDebug = 3;
        if (NULL != utf8PortName){
          iRet = tml_Send_AsyncMessage_A(coreHandle, tmlhandle, utf8ProfileName, utf8HostName, utf8PortName, iTimeout);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Send_AsyncMessage_W", "EXCEPTION / DebugVal", iDebug);
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncMessage_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort, TML_UINT32 iTimeout){
  TML_INT32 iDebug = 0;
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    iDebug = 1;
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF16toUTF8((wchar_t*)sHost, &iLengthUtf8);
      iDebug = 2;
      if (NULL != utf8HostName){
        char* utf8PortName = UTF16toUTF8((wchar_t*)sPort, &iLengthUtf8);
        iDebug = 3;
        if (NULL != utf8PortName){
          iRet = tml_Send_AsyncMessage_A(coreHandle, tmlhandle, utf8ProfileName, utf8HostName, utf8PortName, iTimeout);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Send_AsyncMessage_W", "EXCEPTION / DebugVal", iDebug);
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Send an asynchronous progress reply.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncProgressReply(TML_COMMAND_HANDLE tmlhandle, TML_INT32 progress)
{
  TML_INT32  iRet = TML_SUCCESS;

  if (TML_HANDLE_TYPE_NULL == tmlhandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      TML_CORE_HANDLE coreHandle;
      iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Core_Reference(&coreHandle);
      if (TML_SUCCESS == iRet){
        ////////////////////////////////////////////////////////////////////////////
        // Core is in shutdown process:
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
        if (TML_SUCCESS == iRet){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Send_AsyncProgressReply");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SendAsyncProgressReply(tmlhandle, progress);
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Send an asynchronous status reply.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncStatusReply(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iType, TML_CTSTR* sStatus);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncStatusReply_A(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iType, char* sStatus)
{
  TML_INT32  iRet = TML_SUCCESS;

  if (TML_HANDLE_TYPE_NULL == tmlhandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      TML_CORE_HANDLE coreHandle;
      iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Core_Reference(&coreHandle);
      if (TML_SUCCESS == iRet){
        ////////////////////////////////////////////////////////////////////////////
        // Core is in shutdown process:
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
        if (TML_SUCCESS == iRet){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Send_AsyncStatusReply");
          iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SendAsyncStatusReply(tmlhandle, (int)iType, sStatus);
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncStatusReply_X(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iType, wchar_t* sStatus){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8StatusName = UTF32toUTF8(sStatus, &iLengthUtf8);
    if (NULL != utf8StatusName){
      iRet = tml_Send_AsyncStatusReply_A(tmlhandle, iType, utf8StatusName);
      delete utf8StatusName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Send_AsyncStatusReply_W(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iType, char16_t* sStatus){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8StatusName = UTF16toUTF8((wchar_t*)sStatus, &iLengthUtf8);
    if (NULL != utf8StatusName){
      iRet = tml_Send_AsyncStatusReply_A(tmlhandle, iType, utf8StatusName);
      delete utf8StatusName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Subscribe a destination for event messages.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Subscribe_MessageDestination(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Subscribe_MessageDestination_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sHost, const char* sPort)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Subscribe_MessageDestination");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventSubscribeMessageDestination(profile, sHost, sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Subscribe_MessageDestination_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF32toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF32toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_Evt_Subscribe_MessageDestination_A(coreHandle, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Subscribe_MessageDestination_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF16toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF16toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_Evt_Subscribe_MessageDestination_A(coreHandle, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Get subscribed destinations for event messages for the provided profile.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_Subscribed_MessageDestinations(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, SIDEX_VARIANT* subscriptions);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_Subscribed_MessageDestinations_A(TML_CORE_HANDLE coreHandle, const char* profile, SIDEX_VARIANT* subscriptions)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Get_Subscribed_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventGetSubscribedMessageDestinations(profile, subscriptions);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_Subscribed_MessageDestinations_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, SIDEX_VARIANT* subscriptions)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;
        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);

        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Get_Subscribed_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventGetSubscribedMessageDestinations(utf8ProfileName, subscriptions);
        if (NULL != utf8ProfileName){
          delete utf8ProfileName;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_Subscribed_MessageDestinations_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, SIDEX_VARIANT* subscriptions)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;
        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);

        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Get_Subscribed_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventGetSubscribedMessageDestinations(utf8ProfileName, subscriptions);
        if (NULL != utf8ProfileName){
          delete utf8ProfileName;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Unsubscribe a destination for event messages.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_MessageDestination(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_MessageDestination_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sHost, const char* sPort)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Unsubscribe_MessageDestination");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventUnsubscribeMessageDestination(profile, sHost, sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_MessageDestination_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF32toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF32toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_Evt_Unsubscribe_MessageDestination_A(coreHandle, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_MessageDestination_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF16toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF16toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_Evt_Unsubscribe_MessageDestination_A(coreHandle, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Unsubscribe all destinations for event messages for the provided profile.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_All_MessageDestinations(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_All_MessageDestinations_A(TML_CORE_HANDLE coreHandle, const char* profile)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Unsubscribe_All_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventUnsubscribeAllMessageDestinations(profile);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_All_MessageDestinations_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;
        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Unsubscribe_All_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventUnsubscribeAllMessageDestinations(utf8ProfileName);
        if (NULL != utf8ProfileName){
          delete utf8ProfileName;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Unsubscribe_All_MessageDestinations_W(TML_CORE_HANDLE coreHandle, const char16_t* profile)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;
        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Unsubscribe_All_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventUnsubscribeAllMessageDestinations(utf8ProfileName);
        if (NULL != utf8ProfileName){
          delete utf8ProfileName;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}



/**
 * @brief    Register event error handler.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnError(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnError_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnError");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterCallbackOnError(profile, pCBFunc, pCBData, idUNICODE_ASCII);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnError_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iRet = TML_ERR_UNICODE;
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnError");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterCallbackOnError(utf8ProfileName, pCBFunc, pCBData, idUNICODE_WCHAR_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnError_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_EVT_ERROR_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iRet = TML_ERR_UNICODE;
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnError");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterCallbackOnError(utf8ProfileName, pCBFunc, pCBData, idUNICODE_CHAR16_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief    Register a callback method that will be invoked in case of event message queue overflow.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnQueueOverflow(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnQueueOverflow_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnQueueOverflow");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_OnQueueOverflow(profile, pCBFunc, pCBData, idUNICODE_ASCII);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnQueueOverflow_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnQueueOverflow");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_OnQueueOverflow(utf8ProfileName, pCBFunc, pCBData, idUNICODE_WCHAR_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnQueueOverflow_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnQueueOverflow");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_Set_OnQueueOverflow(utf8ProfileName, pCBFunc, pCBData, idUNICODE_CHAR16_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief    Register a callback method that will be invoked in case of event messages to a profile without any subscribed destination. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPopulate(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPopulate_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnPopulate");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterPopulateCallback(profile, pCBFunc, pCBData, idUNICODE_ASCII);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPopulate_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnPopulate");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterPopulateCallback(utf8ProfileName, pCBFunc, pCBData, idUNICODE_WCHAR_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPopulate_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnPopulate");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterPopulateCallback(utf8ProfileName, pCBFunc, pCBData, idUNICODE_CHAR16_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief    Send an event message / command.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_Message(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_Message_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile)
{
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Evt_Send_Message", "begin", tmlhandle);
#endif //TIMEOUT_LOGGING
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Send_Message");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventSendMessage(tmlhandle, profile);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Evt_Send_Message", "end", tmlhandle);
#endif //TIMEOUT_LOGGING
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_Message_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      iRet = tml_Evt_Send_Message_A(coreHandle, tmlhandle, utf8ProfileName);
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_Message_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      iRet = tml_Evt_Send_Message_A(coreHandle, tmlhandle, utf8ProfileName);
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Send an event subscription request.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_SubscriptionRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sSourceHost, const TML_CTSTR* sSourcePort, const TML_CTSTR* sDestHost, const TML_CTSTR* sDestPort, TML_UINT32 iTimeout);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_SubscriptionRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, TML_UINT32 iTimeout)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Send_SubscriptionRequest");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventSendSubscriptionRequest(profile, sSourceHost, sSourcePort, sDestHost, sDestPort, (unsigned int)iTimeout);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_SubscriptionRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sSourceHost, const wchar_t* sSourcePort, const wchar_t* sDestHost, const wchar_t* sDestPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8SourceHostName = UTF32toUTF8((wchar_t*)sSourceHost, &iLengthUtf8);
      if (NULL != utf8SourceHostName){
        char* utf8SourcePortName = UTF32toUTF8((wchar_t*)sSourcePort, &iLengthUtf8);
        if (NULL != utf8SourcePortName){
          char* utf8DestHostName = UTF32toUTF8((wchar_t*)sDestHost, &iLengthUtf8);
          if (NULL != utf8DestHostName){
            char* utf8DestPortName = UTF32toUTF8((wchar_t*)sDestPort, &iLengthUtf8);
            if (NULL != utf8DestPortName){
              iRet = tml_Evt_Send_SubscriptionRequest_A(coreHandle, utf8ProfileName, utf8SourceHostName, utf8SourcePortName, utf8DestHostName, utf8DestPortName, iTimeout);
              delete utf8DestPortName;
            }
            delete utf8DestHostName;
          }
          delete utf8SourcePortName;
        }
        delete utf8SourceHostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_SubscriptionRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sSourceHost, const char16_t* sSourcePort, const char16_t* sDestHost, const char16_t* sDestPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8SourceHostName = UTF16toUTF8((wchar_t*)sSourceHost, &iLengthUtf8);
      if (NULL != utf8SourceHostName){
        char* utf8SourcePortName = UTF16toUTF8((wchar_t*)sSourcePort, &iLengthUtf8);
        if (NULL != utf8SourcePortName){
          char* utf8DestHostName = UTF16toUTF8((wchar_t*)sDestHost, &iLengthUtf8);
          if (NULL != utf8DestHostName){
            char* utf8DestPortName = UTF16toUTF8((wchar_t*)sDestPort, &iLengthUtf8);
            if (NULL != utf8DestPortName){
              iRet = tml_Evt_Send_SubscriptionRequest_A(coreHandle, utf8ProfileName, utf8SourceHostName, utf8SourcePortName, utf8DestHostName, utf8DestPortName, iTimeout);
              delete utf8DestPortName;
            }
            delete utf8DestHostName;
          }
          delete utf8SourcePortName;
        }
        delete utf8SourceHostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Register a callback method for the case of a peer (un)subscription request for event messages.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPeerRegister(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPeerRegister_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnPeerRegister");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterOnPeerCallback(profile, pCBFunc, pCBData, idUNICODE_ASCII);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPeerRegister_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnPeerRegister");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterOnPeerCallback(utf8ProfileName, pCBFunc, pCBData, idUNICODE_WCHAR_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_OnPeerRegister_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_OnPeerRegister");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventRegisterOnPeerCallback(utf8ProfileName, pCBFunc, pCBData, idUNICODE_CHAR16_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief    Send an event unsubscription request.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_UnsubscriptionRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sSourceHost, const TML_CTSTR* sSourcePort, const TML_CTSTR* sDestHost, const TML_CTSTR* sDestPort, TML_UINT32 iTimeout);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_UnsubscriptionRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, TML_UINT32 iTimeout)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Send_UnsubscriptionRequest");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventSendUnsubscriptionRequest(profile, sSourceHost, sSourcePort, sDestHost, sDestPort, (unsigned int)iTimeout);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_UnsubscriptionRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sSourceHost, const wchar_t* sSourcePort, const wchar_t* sDestHost, const wchar_t* sDestPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8SourceHostName = UTF32toUTF8((wchar_t*)sSourceHost, &iLengthUtf8);
      if (NULL != utf8SourceHostName){
        char* utf8SourcePortName = UTF32toUTF8((wchar_t*)sSourcePort, &iLengthUtf8);
        if (NULL != utf8SourcePortName){
          char* utf8DestHostName = UTF32toUTF8((wchar_t*)sDestHost, &iLengthUtf8);
          if (NULL != utf8DestHostName){
            char* utf8DestPortName = UTF32toUTF8((wchar_t*)sDestPort, &iLengthUtf8);
            if (NULL != utf8DestPortName){
              iRet = tml_Evt_Send_UnsubscriptionRequest_A(coreHandle, utf8ProfileName, utf8SourceHostName, utf8SourcePortName, utf8DestHostName, utf8DestPortName, iTimeout);
              delete utf8DestPortName;
            }
            delete utf8DestHostName;
          }
          delete utf8SourcePortName;
        }
        delete utf8SourceHostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Send_UnsubscriptionRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sSourceHost, const char16_t* sSourcePort, const char16_t* sDestHost, const char16_t* sDestPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8SourceHostName = UTF16toUTF8((wchar_t*)sSourceHost, &iLengthUtf8);
      if (NULL != utf8SourceHostName){
        char* utf8SourcePortName = UTF16toUTF8((wchar_t*)sSourcePort, &iLengthUtf8);
        if (NULL != utf8SourcePortName){
          char* utf8DestHostName = UTF16toUTF8((wchar_t*)sDestHost, &iLengthUtf8);
          if (NULL != utf8DestHostName){
            char* utf8DestPortName = UTF16toUTF8((wchar_t*)sDestPort, &iLengthUtf8);
            if (NULL != utf8DestPortName){
              iRet = tml_Evt_Send_UnsubscriptionRequest_A(coreHandle, utf8ProfileName, utf8SourceHostName, utf8SourcePortName, utf8DestHostName, utf8DestPortName, iTimeout);
              delete utf8DestPortName;
            }
            delete utf8DestHostName;
          }
          delete utf8SourcePortName;
        }
        delete utf8SourceHostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Subscribe a destination for load balanced messages.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Subscribe_MessageDestination(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Subscribe_MessageDestination_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sHost, const char* sPort)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Subscribe_MessageDestination");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedSubscribeMessageDestination(profile, sHost, sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Subscribe_MessageDestination_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF32toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF32toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_Bal_Subscribe_MessageDestination_A(coreHandle, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Subscribe_MessageDestination_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF16toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF16toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_Bal_Subscribe_MessageDestination_A(coreHandle, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Get subscribed destinations for load balanced messages for the provided profile.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_Subscribed_MessageDestinations(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, SIDEX_VARIANT* subscriptions);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_Subscribed_MessageDestinations_A(TML_CORE_HANDLE coreHandle, const char* profile, SIDEX_VARIANT* subscriptions)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Get_Subscribed_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedGetSubscribedMessageDestinations(profile, subscriptions);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_Subscribed_MessageDestinations_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, SIDEX_VARIANT* subscriptions)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;
        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Get_Subscribed_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedGetSubscribedMessageDestinations(utf8ProfileName, subscriptions);
        if (NULL != utf8ProfileName){
          delete utf8ProfileName;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_Subscribed_MessageDestinations_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, SIDEX_VARIANT* subscriptions)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;
        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Get_Subscribed_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedGetSubscribedMessageDestinations(utf8ProfileName, subscriptions);
        if (NULL != utf8ProfileName){
          delete utf8ProfileName;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Unsubscribe a destination for load balanced messages.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_MessageDestination(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_MessageDestination_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sHost, const char* sPort)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Unsubscribe_MessageDestination");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedUnsubscribeMessageDestination(profile, sHost, sPort);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_MessageDestination_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF32toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF32toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_Bal_Unsubscribe_MessageDestination_A(coreHandle, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_MessageDestination_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sHost, const char16_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF16toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF16toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_Bal_Unsubscribe_MessageDestination_A(coreHandle, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Unsubscribe all destinations for load balanced messages for the provided profile.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_All_MessageDestinations(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_All_MessageDestinations_A(TML_CORE_HANDLE coreHandle, const char* profile)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Unsubscribe_All_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedUnsubscribeAllMessageDestinations(profile);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_All_MessageDestinations_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;
        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Unsubscribe_All_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedUnsubscribeAllMessageDestinations(utf8ProfileName);
        if (NULL != utf8ProfileName){
          delete utf8ProfileName;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Unsubscribe_All_MessageDestinations_W(TML_CORE_HANDLE coreHandle, const char16_t* profile)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;
        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Unsubscribe_All_MessageDestinations");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedUnsubscribeAllMessageDestinations(utf8ProfileName);
        if (NULL != utf8ProfileName){
          delete utf8ProfileName;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Register a callback method that will be invoked in case of load balanced messages to a profile without any subscribed destination. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPopulate(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPopulate_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_OnPopulate");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedRegisterPopulateCallback(profile, pCBFunc, pCBData, idUNICODE_ASCII);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPopulate_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_OnPopulate");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedRegisterPopulateCallback(utf8ProfileName, pCBFunc, pCBData, idUNICODE_WCHAR_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPopulate_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_POPULATE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_OnPopulate");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedRegisterPopulateCallback(utf8ProfileName, pCBFunc, pCBData, idUNICODE_CHAR16_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief    Register a callback method for the "busy status" requests.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnBusyStatusRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnBusyStatusRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_OnBusyStatusRequest");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedRegisterBusyStatusCallback(profile, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnBusyStatusRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      iRet = tml_Bal_Set_OnBusyStatusRequest_A(coreHandle, utf8ProfileName, pCBFunc, pCBData);
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnBusyStatusRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      iRet = tml_Bal_Set_OnBusyStatusRequest_A(coreHandle, utf8ProfileName, pCBFunc, pCBData);
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Register a callback method for the load balance calculation.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnCalculation(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnCalculation_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        if (NULL == pCBFunc)
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_OnCalculationn / reset Function");
        else
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_OnCalculationn / set Function");
        if (NULL == pCBData)
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_OnCalculationn / reset Data");
        else
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_OnCalculationn / set Data");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedRegisterCalculator(profile, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnCalculation_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      iRet = tml_Bal_Set_OnCalculation_A(coreHandle, utf8ProfileName, pCBFunc, pCBData);
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnCalculation_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_BAL_CALCULATION_REQ_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      iRet = tml_Bal_Set_OnCalculation_A(coreHandle, utf8ProfileName, pCBFunc, pCBData);
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Send a load balanced message / command.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SyncMessage(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile, TML_UINT32 iTimeout);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SyncMessage_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile, TML_UINT32 iTimeout)
{
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Bal_Send_SyncMessage", "begin", tmlhandle);
#endif //TIMEOUT_LOGGING
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Send_SyncMessage");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedSendSyncMessage(tmlhandle, profile, (unsigned int)iTimeout);
      }
    }
    catch (...){
      try{
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Send_SyncMessage");
        m_log->log (TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_SyncMessage", "profile", profile);
        tml_log_A(TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_SyncMessage", "profile", profile);
      }
      catch (...){
      }
      tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_SyncMessage", "EXCEPTION / DebugVal", 0);
    }
  }
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Bal_Send_SyncMessage", "end", tmlhandle);
#endif //TIMEOUT_LOGGING
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SyncMessage_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iDebug = 0;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    iDebug = 1;
    if (NULL != utf8ProfileName){
      iRet = tml_Bal_Send_SyncMessage_A(coreHandle, tmlhandle, utf8ProfileName, iTimeout);
      delete utf8ProfileName;
    }
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_SyncMessage_W", "EXCEPTION / DebugVal", iDebug);
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SyncMessage_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iDebug = 0;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    iDebug = 1;
    if (NULL != utf8ProfileName){
      iRet = tml_Bal_Send_SyncMessage_A(coreHandle, tmlhandle, utf8ProfileName, iTimeout);
      delete utf8ProfileName;
    }
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_SyncMessage_W", "EXCEPTION / DebugVal", iDebug);
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Send a load balanced async message / command.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_AsyncMessage(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile, TML_UINT32 iTimeout);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_AsyncMessage_A(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char* profile, TML_UINT32 iTimeout)
{
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Bal_Send_AsyncMessage", "begin", tmlhandle);
#endif //TIMEOUT_LOGGING
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Send_AsyncMessage");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedSendAsyncMessage(tmlhandle, profile, (unsigned int)iTimeout);
      }
    }
    catch (...){
      try{
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Send_AsyncMessage");
        m_log->log (TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_AsyncMessage", "profile", profile);
        tml_log_A(TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_AsyncMessage", "profile", profile);
      }
      catch (...){
      }
      iRet = TML_ERR_COMMON;
    }
  }
#ifdef TIMEOUT_LOGGING
  tml_logI(0xFFFFFFFF, "tmlCore", "tml_Bal_Send_AsyncMessage", "end", tmlhandle);
#endif //TIMEOUT_LOGGING
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_AsyncMessage_X(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iDebug = 0;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    iDebug = 1;
    if (NULL != utf8ProfileName){
      iRet = tml_Bal_Send_AsyncMessage_A(coreHandle, tmlhandle, utf8ProfileName, iTimeout);
      delete utf8ProfileName;
    }
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_AsyncMessage_W", "EXCEPTION / DebugVal", iDebug);
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_AsyncMessage_W(TML_CORE_HANDLE coreHandle, TML_COMMAND_HANDLE tmlhandle, const char16_t* profile, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  TML_INT32 iDebug = 0;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    iDebug = 1;
    if (NULL != utf8ProfileName){
      iRet = tml_Bal_Send_AsyncMessage_A(coreHandle, tmlhandle, utf8ProfileName, iTimeout);
      delete utf8ProfileName;
    }
  }
  catch (...){
    tml_logI_A(TML_LOG_EVENT, "TMLCore", "tml_Bal_Send_AsyncMessage_W", "EXCEPTION / DebugVal", iDebug);
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Send a load balanced subscription request.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SubscriptionRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sSourceHost, const TML_CTSTR* sSourcePort, const TML_CTSTR* sDestHost, const TML_CTSTR* sDestPort, TML_UINT32 iTimeout);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SubscriptionRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, TML_UINT32 iTimeout)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Send_SubscriptionRequest");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedSendSubscriptionRequest(profile, sSourceHost, sSourcePort, sDestHost, sDestPort, (unsigned int)iTimeout);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SubscriptionRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sSourceHost, const wchar_t* sSourcePort, const wchar_t* sDestHost, const wchar_t* sDestPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8SourceHostName = UTF32toUTF8((wchar_t*)sSourceHost, &iLengthUtf8);
      if (NULL != utf8SourceHostName){
        char* utf8SourcePortName = UTF32toUTF8((wchar_t*)sSourcePort, &iLengthUtf8);
        if (NULL != utf8SourcePortName){
          char* utf8DestHostName = UTF32toUTF8((wchar_t*)sDestHost, &iLengthUtf8);
          if (NULL != utf8DestHostName){
            char* utf8DestPortName = UTF32toUTF8((wchar_t*)sDestPort, &iLengthUtf8);
            if (NULL != utf8DestPortName){
              iRet = tml_Bal_Send_SubscriptionRequest_A(coreHandle, utf8ProfileName, utf8SourceHostName, utf8SourcePortName, utf8DestHostName, utf8DestPortName, iTimeout);
              delete utf8DestPortName;
            }
            delete utf8DestHostName;
          }
          delete utf8SourcePortName;
        }
        delete utf8SourceHostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_SubscriptionRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sSourceHost, const char16_t* sSourcePort, const char16_t* sDestHost, const char16_t* sDestPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8SourceHostName = UTF16toUTF8((wchar_t*)sSourceHost, &iLengthUtf8);
      if (NULL != utf8SourceHostName){
        char* utf8SourcePortName = UTF16toUTF8((wchar_t*)sSourcePort, &iLengthUtf8);
        if (NULL != utf8SourcePortName){
          char* utf8DestHostName = UTF16toUTF8((wchar_t*)sDestHost, &iLengthUtf8);
          if (NULL != utf8DestHostName){
            char* utf8DestPortName = UTF16toUTF8((wchar_t*)sDestPort, &iLengthUtf8);
            if (NULL != utf8DestPortName){
              iRet = tml_Bal_Send_SubscriptionRequest_A(coreHandle, utf8ProfileName, utf8SourceHostName, utf8SourcePortName, utf8DestHostName, utf8DestPortName, iTimeout);
              delete utf8DestPortName;
            }
            delete utf8DestHostName;
          }
          delete utf8SourcePortName;
        }
        delete utf8SourceHostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Register a callback method for the case of a peer (un)subscription request for load balanced messages.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPeerRegister(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPeerRegister_A(TML_CORE_HANDLE coreHandle, const char* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Bal_OnPeerRegister");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedRegisterOnPeerCallback(profile, pCBFunc, pCBData, idUNICODE_ASCII);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPeerRegister_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Bal_OnPeerRegister");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedRegisterOnPeerCallback(utf8ProfileName, pCBFunc, pCBData, idUNICODE_WCHAR_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_OnPeerRegister_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, TML_ON_PEER_REGISTRATION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        TML_INT32 iLengthUtf8;

        char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
        if (NULL != utf8ProfileName){
          ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Bal_OnPeerRegister");
          iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedRegisterOnPeerCallback(utf8ProfileName, pCBFunc, pCBData, idUNICODE_CHAR16_T);
          delete utf8ProfileName;
        }
        else{
          iRet = TML_ERR_UNICODE;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief    Send a load balanced unsubscription request.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_UnsubscriptionRequest(TML_CORE_HANDLE coreHandle, const TML_CTSTR* profile, const TML_CTSTR* sSourceHost, const TML_CTSTR* sSourcePort, const TML_CTSTR* sDestHost, const TML_CTSTR* sDestPort, TML_UINT32 iTimeout);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_UnsubscriptionRequest_A(TML_CORE_HANDLE coreHandle, const char* profile, const char* sSourceHost, const char* sSourcePort, const char* sDestHost, const char* sDestPort, TML_UINT32 iTimeout)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Send_UnsubscriptionRequest");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedSendUnsubscriptionRequest(profile, sSourceHost, sSourcePort, sDestHost, sDestPort, (unsigned int)iTimeout);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_UnsubscriptionRequest_X(TML_CORE_HANDLE coreHandle, const wchar_t* profile, const wchar_t* sSourceHost, const wchar_t* sSourcePort, const wchar_t* sDestHost, const wchar_t* sDestPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8SourceHostName = UTF32toUTF8((wchar_t*)sSourceHost, &iLengthUtf8);
      if (NULL != utf8SourceHostName){
        char* utf8SourcePortName = UTF32toUTF8((wchar_t*)sSourcePort, &iLengthUtf8);
        if (NULL != utf8SourcePortName){
          char* utf8DestHostName = UTF32toUTF8((wchar_t*)sDestHost, &iLengthUtf8);
          if (NULL != utf8DestHostName){
            char* utf8DestPortName = UTF32toUTF8((wchar_t*)sDestPort, &iLengthUtf8);
            if (NULL != utf8DestPortName){
              iRet = tml_Bal_Send_UnsubscriptionRequest_A(coreHandle, utf8ProfileName, utf8SourceHostName, utf8SourcePortName, utf8DestHostName, utf8DestPortName, iTimeout);
              delete utf8DestPortName;
            }
            delete utf8DestHostName;
          }
          delete utf8SourcePortName;
        }
        delete utf8SourceHostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Send_UnsubscriptionRequest_W(TML_CORE_HANDLE coreHandle, const char16_t* profile, const char16_t* sSourceHost, const char16_t* sSourcePort, const char16_t* sDestHost, const char16_t* sDestPort, TML_UINT32 iTimeout){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8SourceHostName = UTF16toUTF8((wchar_t*)sSourceHost, &iLengthUtf8);
      if (NULL != utf8SourceHostName){
        char* utf8SourcePortName = UTF16toUTF8((wchar_t*)sSourcePort, &iLengthUtf8);
        if (NULL != utf8SourcePortName){
          char* utf8DestHostName = UTF16toUTF8((wchar_t*)sDestHost, &iLengthUtf8);
          if (NULL != utf8DestHostName){
            char* utf8DestPortName = UTF16toUTF8((wchar_t*)sDestPort, &iLengthUtf8);
            if (NULL != utf8DestPortName){
              iRet = tml_Bal_Send_UnsubscriptionRequest_A(coreHandle, utf8ProfileName, utf8SourceHostName, utf8SourcePortName, utf8DestHostName, utf8DestPortName, iTimeout);
              delete utf8DestPortName;
            }
            delete utf8DestHostName;
          }
          delete utf8SourcePortName;
        }
        delete utf8SourceHostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Set the maximum load balanced connection fail count value.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Set_MaxConnectionFailCount(TML_CORE_HANDLE coreHandle, TML_UINT32 iCount)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Set_MaxConnectionFailCount");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedSetMaxConnectionFailCount((unsigned int)iCount);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Get the maximum load balanced connection fail count value.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Bal_Get_MaxConnectionFailCount(TML_CORE_HANDLE coreHandle, TML_UINT32* iCount)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Bal_Get_MaxConnectionFailCount");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_LoadBalancedGetMaxConnectionFailCount(iCount);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Set the maximum event connection fail count value.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_MaxConnectionFailCount(TML_CORE_HANDLE coreHandle, TML_UINT32 iCount)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_MaxConnectionFailCount");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventSetMaxConnectionFailCount((unsigned int)iCount);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Get the maximum event connection fail count value.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_MaxConnectionFailCount(TML_CORE_HANDLE coreHandle, TML_UINT32* iCount)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Get_MaxConnectionFailCount");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventGetMaxConnectionFailCount(iCount);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Set the maximum value of queued event messages
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Set_MaxQueuedEventMessages(TML_CORE_HANDLE coreHandle, TML_UINT32 iMaximum){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Set_MaxQueuedEventMessages");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventSetMaxQueuedEventMessages((unsigned int)iMaximum);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Get the maximum value of queued event messages
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Evt_Get_MaxQueuedEventMessages(TML_CORE_HANDLE coreHandle, TML_UINT32* iMaximum){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_Evt_Get_MaxQueuedEventMessages");
        unsigned int iMax;
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_EventGetMaxQueuedEventMessages(&iMax);
        if (TML_SUCCESS == iRet){
          *iMaximum = (TML_UINT32)iMax;
        }
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Creates a new instance of TML_COMMAND_HANDLE object.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Create(TML_COMMAND_HANDLE* tmlhandle) {
  TML_INT32 iRet = TML_SUCCESS;
  try{
    tmlObjWrapper* wrapper = new tmlObjWrapper((char*)"API");
    *tmlhandle = (TML_COMMAND_HANDLE) wrapper;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
};


/**
 * @brief    Free an instance of TML_COMMAND_HANDLE object. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Free(TML_COMMAND_HANDLE* tmlhandle) {

  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == tmlhandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      // Do make the cast to (tmlObjWrapper*) / In that case the delete will call the destructor automatically via the scalar destructor:
      delete ((tmlObjWrapper*) *tmlhandle);
      *tmlhandle = TML_HANDLE_TYPE_NULL;
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
};


/**
 * @brief   Register a callback method to get progress information
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_Progress(TML_COMMAND_HANDLE tmlhandle, TML_ON_PROGRESS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Register_Progress(pCBFunc, pCBData);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Get registered callback method to get progress information
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Registered_Progress(TML_COMMAND_HANDLE tmlhandle, TML_ON_PROGRESS_REPLY_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Registered_Progress(pCBFunc, pCBData);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Register a callback method to get status reply information
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_StatusReply(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_StatusReply_A(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Register_StatusReply(pCBFunc, pCBData, idUNICODE_ASCII);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_StatusReply_X(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Register_StatusReply(pCBFunc, pCBData, idUNICODE_WCHAR_T);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_StatusReply_W(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Register_StatusReply(pCBFunc, pCBData, idUNICODE_CHAR16_T);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Get registered callback method to get status reply information
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Registered_StatusReply(TML_COMMAND_HANDLE tmlhandle, TML_ON_STATUS_REPLY_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    tmlUnicodeID iUnicode;
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Registered_StatusReply(pCBFunc, pCBData, &iUnicode);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
  
/**
 * @brief   Register a callback method to get the result of a command execution
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Register_CommandReady(TML_COMMAND_HANDLE tmlhandle, TML_ON_COMMAND_READY_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Register_CommandReady(pCBFunc, pCBData);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Get registered callback method to get the result of a command execution.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Registered_CommandReady(TML_COMMAND_HANDLE tmlhandle,TML_ON_COMMAND_READY_CB_FUNC* pCBFunc, TML_POINTER* pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Registered_CommandReady(pCBFunc, pCBData);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Set listener core reference.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Core_Reference(TML_COMMAND_HANDLE tmlhandle, TML_CORE_HANDLE tmlcorehandle){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Set_Core_Reference(tmlcorehandle);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Get listener core reference. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Core_Reference(TML_COMMAND_HANDLE tmlhandle, TML_CORE_HANDLE* tmlcorehandle){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Core_Reference(tmlcorehandle);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Set listener communication profile. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Profile(TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR* profile);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Profile_A(TML_COMMAND_HANDLE tmlhandle, const char* profile){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Set_Profile(profile);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Profile_X(TML_COMMAND_HANDLE tmlhandle, const wchar_t* profile){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      iRet = tml_Cmd_Attr_Set_Profile_A(tmlhandle, utf8ProfileName);
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Profile_W(TML_COMMAND_HANDLE tmlhandle, const char16_t* profile){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      iRet = tml_Cmd_Attr_Set_Profile_A(tmlhandle, utf8ProfileName);
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Get listener communication profile. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Profile(TML_COMMAND_HANDLE tmlhandle, const TML_CTSTR** profile);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Profile_A(TML_COMMAND_HANDLE tmlhandle, const char** profile){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Profile_A(profile);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Profile_X(TML_COMMAND_HANDLE tmlhandle, const wchar_t** profile){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Profile_X(profile);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Profile_W(TML_COMMAND_HANDLE tmlhandle, const char16_t** profile){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Profile_W(profile);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Set listener communication session ID. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Session_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iSesionID){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Set_Session_ID((int)iSesionID);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Get listener communication session ID. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Session_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32* iSesionID){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = (TML_INT32)((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Session_ID(iSesionID);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Set listener communication channel. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Channel(TML_COMMAND_HANDLE tmlhandle, TML_POINTER channel){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Set_Channel(channel);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Get listener communication channel. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Channel(TML_COMMAND_HANDLE tmlhandle, TML_POINTER* channel){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Channel(channel);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Set listener communication channel ID. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Channel_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32 channelID){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Set_Channel_ID((int)channelID);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Get listener communication channel ID.  
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Channel_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32* channelID){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Channel_ID(channelID);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Set listener communication message ID. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Set_Message_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32 iMessageID){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Set_Message_ID((int)iMessageID);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Get listener communication message ID. 
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Attr_Get_Message_ID(TML_COMMAND_HANDLE tmlhandle, TML_INT32* iMessageID){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Attr_Get_Message_ID(iMessageID);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Get the command creation time.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCreationTime(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR** time);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCreationTime_A(TML_COMMAND_HANDLE tmlhandle, char** time){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetCreationTime(time);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCreationTime_X(TML_COMMAND_HANDLE tmlhandle, wchar_t** time){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    char* utf8Content;
    iRet = tml_Cmd_Header_GetCreationTime_A(tmlhandle, &utf8Content);
    if (TML_SUCCESS == iRet){
      TML_INT32 iLengthUtf16;
      wchar_t* utf16Content = UTF8toUTF32(utf8Content, &iLengthUtf16);
      if (NULL != utf16Content){
        *time = utf16Content;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      sidex_Free_ReadString(utf8Content);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCreationTime_W(TML_COMMAND_HANDLE tmlhandle, char16_t** time){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    char* utf8Content;
    iRet = tml_Cmd_Header_GetCreationTime_A(tmlhandle, &utf8Content);
    if (TML_SUCCESS == iRet){
      TML_INT32 iLengthUtf16;
      char16_t* utf16Content = (char16_t*)UTF8toUTF16(utf8Content, &iLengthUtf16);
      if (NULL != utf16Content){
        *time = utf16Content;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      sidex_Free_ReadString(utf8Content);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief Set the command creation time.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCreationTime(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR* time);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCreationTime_A(TML_COMMAND_HANDLE tmlhandle, char* time){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetCreationTime(time);
    if (TML_SUCCESS != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCreationTime_X(TML_COMMAND_HANDLE tmlhandle, wchar_t* time){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Time = UTF32toUTF8((wchar_t*)time, &iLengthUtf8);
    if (NULL != utf8Time){
      iRet = tml_Cmd_Header_SetCreationTime_A(tmlhandle, utf8Time);
      delete utf8Time;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCreationTime_W(TML_COMMAND_HANDLE tmlhandle, char16_t* time){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Time = UTF16toUTF8((wchar_t*)time, &iLengthUtf8);
    if (NULL != utf8Time){
      iRet = tml_Cmd_Header_SetCreationTime_A(tmlhandle, utf8Time);
      delete utf8Time;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Get the command key.    
 */
 TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetCommand(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE* cmd){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetCommand(cmd);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Set the command key.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetCommand(TML_COMMAND_HANDLE tmlhandle, TML_COMMAND_ID_TYPE cmd){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetCommand(cmd);
    if (TML_SUCCESS != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Get the error key of the command execution.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetError(TML_COMMAND_HANDLE tmlhandle, TML_INT32* error){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetError(error);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Set the error key of the command execution.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetError(TML_COMMAND_HANDLE tmlhandle, TML_INT32 error){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetError(error);
    if (TML_SUCCESS != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Get the error message of the command execution.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetErrorMessage(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR** msg, TML_INT32* iMsgLength);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetErrorMessage_A(TML_COMMAND_HANDLE tmlhandle, char** msg, TML_INT32* iMsgLength){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetErrorMessage(msg, iMsgLength);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetErrorMessage_X(TML_COMMAND_HANDLE tmlhandle, wchar_t** msg, TML_INT32* iMsgLength){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    char* utf8Content;
    TML_INT32 iLengthUtf8;
    iRet = tml_Cmd_Header_GetErrorMessage_A(tmlhandle, &utf8Content, &iLengthUtf8);
    if (TML_SUCCESS == iRet){
      wchar_t* utf16Content = UTF8toUTF32(utf8Content, iMsgLength);
      if (NULL != utf16Content){
        *msg = utf16Content;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      sidex_Free_ReadString(utf8Content);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetErrorMessage_W(TML_COMMAND_HANDLE tmlhandle, char16_t** msg, TML_INT32* iMsgLength){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    char* utf8Content;
    TML_INT32 iLengthUtf8;
    iRet = tml_Cmd_Header_GetErrorMessage_A(tmlhandle, &utf8Content, &iLengthUtf8);
    if (TML_SUCCESS == iRet){
      char16_t* utf16Content = (char16_t*)UTF8toUTF16(utf8Content, iMsgLength);
      if (NULL != utf16Content){
        *msg = utf16Content;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      sidex_Free_ReadString(utf8Content);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief Set the error message of the command execution.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetErrorMessage(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR* msg, TML_INT32 iMsgLength);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetErrorMessage_A(TML_COMMAND_HANDLE tmlhandle, char* msg, TML_INT32 iMsgLength){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = (TML_INT32)((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetErrorMessage(msg, (int) iMsgLength);
    if (TML_SUCCESS != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetErrorMessage_X(TML_COMMAND_HANDLE tmlhandle, wchar_t* msg, TML_INT32 iMsgLength){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Msg = UTF32toUTF8((wchar_t*)msg, &iLengthUtf8);
    if (NULL != utf8Msg){
      iRet = tml_Cmd_Header_SetErrorMessage_A(tmlhandle, utf8Msg, iLengthUtf8);
      delete utf8Msg;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetErrorMessage_W(TML_COMMAND_HANDLE tmlhandle, char16_t* msg, TML_INT32 iMsgLength){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Msg = UTF16toUTF8((wchar_t*)msg, &iLengthUtf8);
    if (NULL != utf8Msg){
      iRet = tml_Cmd_Header_SetErrorMessage_A(tmlhandle, utf8Msg, iLengthUtf8);
      delete utf8Msg;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief Get the command execution state.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetState(TML_COMMAND_HANDLE tmlhandle, TML_INT32* state){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetState(state);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Set the command execution state.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetState(TML_COMMAND_HANDLE tmlhandle, TML_INT32 state){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetState(state);
    if (TML_SUCCESS != iRet && TML_ERR_COMMAND_STATE_UNDEFINED != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Get the command execution mode.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetMode(TML_COMMAND_HANDLE tmlhandle, TML_INT32* mode){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetMode(mode);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Set the command execution mode.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetMode(TML_COMMAND_HANDLE tmlhandle, TML_INT32 mode){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetMode(mode);
    if (TML_SUCCESS != iRet && TML_ERR_COMMAND_MODE_UNDEFINED != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Get the command reply type.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyType(TML_COMMAND_HANDLE tmlhandle, TML_INT32* type){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetReplyType(type);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Set the command reply type.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyType(TML_COMMAND_HANDLE tmlhandle, TML_INT32 type){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetReplyType(type);
    if (TML_SUCCESS != iRet && TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Get the command execution progress.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetProgress(TML_COMMAND_HANDLE tmlhandle, TML_INT32* progress){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetProgress(progress);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Set the command execution progress.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetProgress(TML_COMMAND_HANDLE tmlhandle, TML_INT32 progress){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetProgress(progress);
    if (TML_SUCCESS != iRet && TML_ERR_COMMAND_PROGRESS_RANGE != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief Get the reply message of the command.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyMessage(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR** msg, TML_INT32* iMsgLength);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyMessage_A(TML_COMMAND_HANDLE tmlhandle, char** msg, TML_INT32* iMsgLength){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_GetReplyMessage(msg, iMsgLength);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyMessage_X(TML_COMMAND_HANDLE tmlhandle, wchar_t** msg, TML_INT32* iMsgLength){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    char* utf8Content;
    TML_INT32 iLengthUtf8;
    iRet = tml_Cmd_Header_GetReplyMessage_A(tmlhandle, &utf8Content, &iLengthUtf8);
    if (TML_SUCCESS == iRet){
      wchar_t* utf16Content = UTF8toUTF32(utf8Content, iMsgLength);
      if (NULL != utf16Content){
        *msg = utf16Content;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      sidex_Free_ReadString(utf8Content);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_GetReplyMessage_W(TML_COMMAND_HANDLE tmlhandle, char16_t** msg, TML_INT32* iMsgLength){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    char* utf8Content;
    TML_INT32 iLengthUtf8;
    iRet = tml_Cmd_Header_GetReplyMessage_A(tmlhandle, &utf8Content, &iLengthUtf8);
    if (TML_SUCCESS == iRet){
      char16_t* utf16Content = (char16_t*)UTF8toUTF16(utf8Content, iMsgLength);
      if (NULL != utf16Content){
        *msg = utf16Content;
      }
      else{
        iRet = TML_ERR_UNICODE;
      }
      sidex_Free_ReadString(utf8Content);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief Set the reply message of the command.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyMessage(TML_COMMAND_HANDLE tmlhandle, TML_CTSTR* msg, TML_INT32 iMsgLength);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyMessage_A(TML_COMMAND_HANDLE tmlhandle, char* msg, TML_INT32 iMsgLength){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = (TML_INT32)((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Header_SetReplyMessage(msg, (int)iMsgLength);
    if (TML_SUCCESS != iRet)
      iRet = TML_ERR_INITIALIZATION;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyMessage_X(TML_COMMAND_HANDLE tmlhandle, wchar_t* msg, TML_INT32 iMsgLength){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Msg = UTF32toUTF8((wchar_t*)msg, &iLengthUtf8);
    if (NULL != utf8Msg){
      iRet = tml_Cmd_Header_SetReplyMessage_A(tmlhandle, utf8Msg, iMsgLength);
      delete utf8Msg;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Header_SetReplyMessage_W(TML_COMMAND_HANDLE tmlhandle, char16_t* msg, TML_INT32 iMsgLength){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Msg = UTF16toUTF8((wchar_t*)msg, &iLengthUtf8);
    if (NULL != utf8Msg){
      iRet = tml_Cmd_Header_SetReplyMessage_A(tmlhandle, utf8Msg, iMsgLength);
      delete utf8Msg;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief   Returns the handle to the data object.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Acquire_Sidex_Handle(TML_COMMAND_HANDLE tmlhandle, SIDEX_HANDLE* shandle){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Acquire_Sidex_Handle(shandle, (char*)"API");
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Release the private use of the data object.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Release_Sidex_Handle(TML_COMMAND_HANDLE tmlhandle){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Release_Sidex_Handle((char*)"API");
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief   Debug output for internal use only.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_Cmd_Debug(TML_COMMAND_HANDLE tmlhandle){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    iRet = ((tmlObjWrapper*)tmlhandle)->tmlObjWrapper_Debug();
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Open a receiver stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Open(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Open_A(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const char* profile, const char* sHost, const char* sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_Open");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_Open");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_Open(iID, sPort, sHost, profile);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Open_X(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF32toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF32toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_RecStream_Open_A(coreHandle, iID, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Open_W(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, const char16_t* profile, const char16_t* sHost, const char16_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF16toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF16toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_RecStream_Open_A(coreHandle, iID, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Close a receiver stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Close(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_BOOL bRetainOpen){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_Close");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_Close");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_Close(iID, bRetainOpen);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Get the size of a stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_GetSize(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_INT64* rsize){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_GetSize");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_GetSize");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_GetSize(iID, rsize);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Perform a seek operation of a stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Seek(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_INT64 seekPos, tmlSeekOrigin origin){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_Seek");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_Seek");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_Seek(iID, seekPos, origin);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Get the actual position of a stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_GetPosition(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_INT64* rposition){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_GetPosition");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_GetPosition");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_GetPosition(iID,rposition);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Write data to a stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Write(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_Write");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_Write");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_Write(iID, buffer, count);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Read data of a stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_Read(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count, TML_INT32* bytesRead){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_Read");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_Read");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_Read(iID, buffer, count, bytesRead);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Read data of a stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_ReadBuffer(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_ReadBuffer");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_ReadBuffer");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_ReadBuffer(iID, buffer, count);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Initiates a download of stream data.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_RecStream_DownloadData(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_UINT32 buffersize, TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC pCBFuncDld, TML_POINTER pCBDataDld, TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC pCBFuncDldFinish, TML_POINTER pCBDataDldFinish)
{
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_RecStream_DownloadData");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_RecStream_DownloadData");
        iRet = (TML_INT32)((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_RecStream_DownloadData(iID, (int)buffersize, pCBFuncDld, pCBDataDld, pCBFuncDldFinish, pCBDataDldFinish);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Open a sender stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Open(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE* iID, const TML_CTSTR* profile, const TML_CTSTR* sHost, const TML_CTSTR* sPort);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Open_A(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE* iID, const char* profile, const char* sHost, const char* sPort){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Open");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Open");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Open(iID, sPort, sHost, profile);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Open_X(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE* iID, const wchar_t* profile, const wchar_t* sHost, const wchar_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF32toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF32toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF32toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_SndStream_Open_A(coreHandle, iID, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Open_W(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE* iID, const char16_t* profile, const char16_t* sHost, const char16_t* sPort){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8ProfileName = UTF16toUTF8((wchar_t*)profile, &iLengthUtf8);
    if (NULL != utf8ProfileName){
      char* utf8HostName = UTF16toUTF8((wchar_t*)sHost, &iLengthUtf8);
      if (NULL != utf8HostName){
      char* utf8PortName = UTF16toUTF8((wchar_t*)sPort, &iLengthUtf8);
        if (NULL != utf8PortName){
          iRet = tml_SndStream_Open_A(coreHandle, iID, utf8ProfileName, utf8HostName, utf8PortName);
          delete utf8PortName;
        }
        delete utf8HostName;
      }
      delete utf8ProfileName;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}

/**
 * @brief    Close a sender stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Close(TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID){
  TML_INT32 iRet = TML_SUCCESS;
  try{
    ////////////////////////////////////////////////////////////////////////////
    // Core is in shutdown process:
    int iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
    if (TML_SUCCESS == iRet){
      ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Close");
      ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Close");
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Close(iID);
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief    Register a callback method to get the stream size.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_GetSize (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_SIZE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Register_GetSize");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Register_GetSize");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Register_GetSize (iID, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Register a callback method to perform stream seek operations.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_Seek (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_SEEK_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Register_Seek");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Register_Seek");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Register_Seek (iID, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}

/**
 * @brief    Register a callback method to get the stream position.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_GetPosition (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_GET_POSITION_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Register_GetPosition");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Register_GetPosition");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Register_GetPosition(iID, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Register a callback method to read data of the stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_Read (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_READ_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Register_Read");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Register_Read");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Register_Read (iID, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Register a callback method to write data to a stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_Write (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID , TML_ON_SND_STRM_WRITE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Register_Write");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Register_Write");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Register_Write (iID, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Register a callback method that will be invoked on close of a referring sender stream.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_Close (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_CLOSE_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Register_Close");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Register_Close");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Register_Close (iID, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief    Register a callback method that will be invoked on communication fault during a request.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_SndStream_Register_OnError (TML_CORE_HANDLE coreHandle, TML_STREAM_ID_TYPE iID, TML_ON_SND_STRM_COM_FAULT_CB_FUNC pCBFunc, TML_POINTER pCBData){
  TML_INT32 iRet = TML_SUCCESS;
  if (TML_HANDLE_TYPE_NULL == coreHandle){
    iRet = TML_ERR_MISSING_OBJ;
  }
  else{
    try{
      ////////////////////////////////////////////////////////////////////////////
      // Core is in shutdown process:
      iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_IsAccessible();
      if (TML_SUCCESS == iRet){
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_STREAM_HANDLING, "TMLCore", "API", "Cmd", "tml_SndStream_Register_OnError");
        ((tmlCoreWrapper*)coreHandle)->log (TML_LOG_CORE_API, "TMLCore", "API", "Cmd", "tml_SndStream_Register_OnError");
        iRet = ((tmlCoreWrapper*)coreHandle)->tmlCoreWrapper_SndStream_Register_OnError (iID, pCBFunc, pCBData);
      }
    }
    catch (...){
      iRet = TML_ERR_COMMON;
    }
  }
  return iRet;
}


/**
 * @brief  Make debug logs.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_log(TML_INT32 iLogMask, const TML_CTSTR* sClass, const TML_CTSTR* sMethod, const TML_CTSTR* sFormatLog, const TML_CTSTR* sLog);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_log_A(TML_INT32 iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog){
  TML_INT32 iRet = TML_SUCCESS;
  if (NULL == c_csObj){
    c_csObj = new tmlCriticalSectionObj();
  }
  c_csObj->tmlCriticalSectionEnter("tml_log");
  try{
    if (NULL == m_log){
      m_log = new tmlLogHandler((char*)"a", (int)m_iLogIndex);
      m_iLogIndex = m_log->calcContFileIndex();
    }
    tmlLogHandler* log = new tmlLogHandler((char*)"a", (int)m_iLogIndex);
    log->setLoggingValue((int)iLogMask);
    log->log ((int)iLogMask, sClass, sMethod, sFormatLog, sLog);
    delete log;
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  c_csObj->tmlCriticalSectionLeave("tml_log");
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_log_X(TML_INT32 iLogMask, const wchar_t* sClass, const wchar_t* sMethod, const wchar_t* sFormatLog, const wchar_t* sLog){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Class = UTF32toUTF8((wchar_t*)sClass, &iLengthUtf8);
    if (NULL != utf8Class){
      char* utf8Method = UTF32toUTF8((wchar_t*)sMethod, &iLengthUtf8);
      if (NULL != utf8Method){
        char* utf8Format = UTF32toUTF8((wchar_t*)sFormatLog, &iLengthUtf8);
        if (NULL != utf8Format){
          char* utf8sLog = UTF32toUTF8((wchar_t*)sLog, &iLengthUtf8);
          if (NULL != utf8sLog){
            iRet = tml_log_A(iLogMask, utf8Class, utf8Method, utf8Format, utf8sLog);
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
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_log_W(TML_INT32 iLogMask, const char16_t* sClass, const char16_t* sMethod, const char16_t* sFormatLog, const char16_t* sLog){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Class = UTF16toUTF8((wchar_t*)sClass, &iLengthUtf8);
    if (NULL != utf8Class){
      char* utf8Method = UTF16toUTF8((wchar_t*)sMethod, &iLengthUtf8);
      if (NULL != utf8Method){
        char* utf8Format = UTF16toUTF8((wchar_t*)sFormatLog, &iLengthUtf8);
        if (NULL != utf8Format){
          char* utf8sLog = UTF16toUTF8((wchar_t*)sLog, &iLengthUtf8);
          if (NULL != utf8sLog){
            iRet = tml_log_A(iLogMask, utf8Class, utf8Method, utf8Format, utf8sLog);
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
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}


/**
 * @brief  Make debug logs.
 */
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_logI(TML_INT32 iLogMask, const TML_CTSTR* sClass, const TML_CTSTR* sMethod, const TML_CTSTR* sFormatLog, const TML_INT64 iVal);
/**
 * char* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_logI_A(TML_INT32 iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const TML_INT64 iVal){
  TML_INT32 iRet = TML_SUCCESS;
  if (NULL == c_csObj){
    c_csObj = new tmlCriticalSectionObj();
  }
  c_csObj->tmlCriticalSectionEnter("tml_logI");

  try{
    if (NULL == m_log){
      m_log = new tmlLogHandler((char*)"a", (int)m_iLogIndex);
      m_iLogIndex = m_log->calcContFileIndex();
    }
    tmlLogHandler* log = new tmlLogHandler((char*)"a", (int)m_iLogIndex);
    log->setLoggingValue((int)iLogMask);
    log->log ((int)iLogMask, sClass, sMethod, sFormatLog, iVal);
    delete (log);
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  c_csObj->tmlCriticalSectionLeave("tml_logI");
  return iRet;
}
/**
 * wchar_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_logI_X(TML_INT32 iLogMask, const wchar_t* sClass, const wchar_t* sMethod, const wchar_t* sFormatLog, const TML_INT64 iVal){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Class = UTF32toUTF8((wchar_t*)sClass, &iLengthUtf8);
    if (NULL != utf8Class){
      char* utf8Method = UTF32toUTF8((wchar_t*)sMethod, &iLengthUtf8);
      if (NULL != utf8Method){
        char* utf8Format = UTF32toUTF8((wchar_t*)sFormatLog, &iLengthUtf8);
        if (NULL != utf8Format){
          iRet = tml_logI_A(iLogMask, utf8Class, utf8Method, utf8Format, iVal);
          delete utf8Format;
        }
        delete utf8Method;
      }
      delete utf8Class;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
/**
 * char16_t* API
**/
TML_CORE_API TML_INT32 DLL_CALL_CONV tml_logI_W(TML_INT32 iLogMask, const char16_t* sClass, const char16_t* sMethod, const char16_t* sFormatLog, const TML_INT64 iVal){
  TML_INT32 iRet = TML_ERR_UNICODE;
  try{
    TML_INT32 iLengthUtf8;

    char* utf8Class = UTF16toUTF8((wchar_t*)sClass, &iLengthUtf8);
    if (NULL != utf8Class){
      char* utf8Method = UTF16toUTF8((wchar_t*)sMethod, &iLengthUtf8);
      if (NULL != utf8Method){
        char* utf8Format = UTF16toUTF8((wchar_t*)sFormatLog, &iLengthUtf8);
        if (NULL != utf8Format){
          iRet = tml_logI_A(iLogMask, utf8Class, utf8Method, utf8Format, iVal);
          delete utf8Format;
        }
        delete utf8Method;
      }
      delete utf8Class;
    }
  }
  catch (...){
    iRet = TML_ERR_COMMON;
  }
  return iRet;
}
#ifdef LINUX
void __attribute__ ((constructor)) my_load(void);
void __attribute__ ((destructor)) my_unload(void);

// Called when the library is loaded and before dlopen() returns
void my_load(void)
{
    // BEIM LADEN DER DLL:
    // Add initialization code
}

// Called when the library is unloaded and before dlclose()
// returns
void my_unload(void)
{
    // BEIM FREIGEBEN DER DLL:
    // Add clean-up code
}
#else
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{

    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
         // BEIM LADEN DER DLL:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
/*
         ////////////////////////////////
         // create vortex execution context
         m_ctx = vortex_ctx_new ();
         ////////////////////////////////
         // init vortex library
         axl_bool bSuccess;
         bSuccess = vortex_init_ctx (m_ctx);
         if (bSuccess)
           m_status = TML_SUCCESS;
         else
           m_status = TML_ERR_INITIALIZATION;
           */
         break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
         break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
         break;

        case DLL_PROCESS_DETACH:
         // BEIM FREIGEBEN DER DLL:
         // Perform any necessary cleanup.
          ////////////////////////////////////
          // Log the detach
//          tml_log (TML_LOG_CORE_API, "TMLCore", "DLL", "PROCESS", "DETACH");

         //vortex_exit_ctx (m_ctx, false);
         //vortex_ctx_free (m_ctx);
         break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#endif
