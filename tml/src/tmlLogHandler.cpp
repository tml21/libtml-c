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
 
#ifndef LINUX
#ifdef USE_REDIRECTION
#include <io.h>
#include <iostream>
using namespace std;
#endif // USE_REDIRECTION
#include <WINSOCK2.H>
#include <share.h>
#include <stdio.h>
#include <fcntl.h>
#else // LINUX
#include <vortex.h>
#endif // LINUX
#include <time.h>
#include "tmlLogHandler.h"
#include "sidex.h"
#include "logValues.h"

#define DO_FLUSH

#ifdef LINUX

FILE *_fsopen(const char *filename,const char *mode,int shflag)
{
  struct flock fileshare;
  FILE* stream;
  int fdescriptor;

  if((stream=fopen(filename,mode))!=NULL)
  {
    if (shflag!=_SH_DENYNO) {
      if ((fdescriptor=fileno(stream))==-1) {
        fclose(stream);
        return(NULL);
      }

      fileshare.l_whence=SEEK_SET;
      fileshare.l_start=0;
      fileshare.l_len=0;
      if (shflag==_SH_DENYRW || shflag==_SH_DENYRD)
        fileshare.l_type=F_WRLCK;

      if (shflag==_SH_DENYWR)
        fileshare.l_type=F_RDLCK;

      if((fcntl(fdescriptor,F_SETLK,&fileshare))==-1) {
        fclose(stream);
        return(NULL);
      }
    }
    return(stream);
  }
  return(NULL);
}

#endif // LINUX

/**
 * @brief    Constructor.
 */
tmlLogHandler::tmlLogHandler(){
  RedirectIOToConsole();
  m_iLogFileIndex = -1;
  m_iShare = _SH_DENYNO;
  m_logFile = (char*) LOG_FILE_NAME;
  m_fileLogMode = (char*) LOG_FILE_OPEN_MODE;
  m_iAdditionalTimeout = 0;
  m_iThreadPoolAddPeriod = 0;
  m_iEventLogPeriod = DEFAULT_EVENT_MSG_LOG_PERIOD;
  m_iEventQueueRemoveLogPeriod = DEFAULT_EVENT_QUEUE_REMOVE_LOG_PERIOD;

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  axl_bool bSuccess;
  bSuccess = createCriticalSectionObject(&m_mutexCriticalSection);
#ifndef DISABLE_TML_LOGGING
  /////////////////////////////////////
  // Logpath
  m_logPath = new char[1024];
  getLogStringValue((char*)TML_OPT_LOG_PATH, &m_logPath, 1024);

  /////////////////////////////////////
  // Allocate memory for the date / time:
  m_date = new char[DATE_TIME_STRING_LENGTH];
  m_time = new char[DATE_TIME_STRING_LENGTH];
  m_hLogFile = NULL;
  m_bUseLogfile = true;

  m_bLogIntoFile = checkLogOption((char*)TML_OPT_ENABLE_FILE_LOG);
  m_bUseLogfileMask = checkLogOption((char*)TML_OPT_USE_LOG_MASK);
  if (SIDEX_TRUE == m_bUseLogfileMask){
    m_iLogfileMask = getLogIntegerValue((char*)TML_OPT_LOG_MASK_VALUE);
  }
  else{
    m_iLogfileMask = TML_LOG_OFF;
  }
  m_iAdditionalTimeout = getLogIntegerValue((char*)TML_ADDITIONAL_TIMEOUT);
  if (-1 == m_iAdditionalTimeout){
    // If not found:
    m_iAdditionalTimeout = 0;
  }

  m_iEventLogPeriod = getLogIntegerValue((char*)TML_EVENT_LOG_PERIOD);
  if (-1 == m_iEventLogPeriod){
    // If not found:
    m_iEventLogPeriod = DEFAULT_EVENT_MSG_LOG_PERIOD;
  }

  m_iEventQueueRemoveLogPeriod = getLogIntegerValue((char*)TML_EVENT_QUEUE_REMOVE_LOG_PERIOD);
  if (-1 == m_iEventQueueRemoveLogPeriod){
    // If not found:
    m_iEventQueueRemoveLogPeriod = DEFAULT_EVENT_QUEUE_REMOVE_LOG_PERIOD;
  }

  // printf ("###  tmlLogHandler / AdditionalTimeout = %lld\n", m_iAdditionalTimeout);

  m_iThreadPoolAddPeriod = getLogIntegerValue((char*)TML_THREAD_POOL_ADD_PERIOD);
  if (-1 == m_iThreadPoolAddPeriod){
    // If not found:
    m_iThreadPoolAddPeriod = -9999;
  }
  //printf ("###  tmlLogHandler / ThreadPoolAddPeriod = %lld\n", m_iThreadPoolAddPeriod);
  /////////////////////////////////////
  // Always one log into the contiguous log:
  m_iLogValue = 0xFFFFFFFF;
  //log (0xFFFFFFFF, "tmlLogHandler", "tmlLogHandler", "Start", "Log");
  /////////////////////////////////////
  // Actual log value:
  if (SIDEX_TRUE == m_bUseLogfileMask){
    m_iLogValue = (int)m_iLogfileMask; // set the logging value
  }
  else{
    m_iLogValue = TML_LOG_OFF; // set the logging value
  }
#endif // DISABLE_TML_LOGGING
}


/**
 * @brief    Constructor.
 */
tmlLogHandler::tmlLogHandler(char* mode){
  RedirectIOToConsole();
  m_iLogFileIndex = -1;
  m_iShare = _SH_DENYNO;
  m_logFile = (char*) LOG_FILE_NAME;
  m_fileLogMode = mode;
  /////////////////////////////////////
  // Default log value:
  m_iLogValue = TML_LOG_OFF;

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  axl_bool bSuccess;
  bSuccess = createCriticalSectionObject(&m_mutexCriticalSection);
#ifndef DISABLE_TML_LOGGING
  /////////////////////////////////////
  // Logpath
  m_logPath = new char[1024];
  getLogStringValue((char*)TML_OPT_LOG_PATH, &m_logPath, 1024);

  /////////////////////////////////////
  // Allocate memory for the date / time:
  m_date = new char[DATE_TIME_STRING_LENGTH];
  m_time = new char[DATE_TIME_STRING_LENGTH];
  m_hLogFile = NULL;
  m_bUseLogfile = true;
  m_bLogIntoFile = checkLogOption((char*)TML_OPT_ENABLE_FILE_LOG);
  m_bUseLogfileMask = SIDEX_FALSE;
  m_iLogfileMask = TML_LOG_OFF;

#endif // DISABLE_TML_LOGGING
}


/**
 * @brief    Constructor.
 */
tmlLogHandler::tmlLogHandler(char* mode, int iFileIndex){
  RedirectIOToConsole();
  m_iLogFileIndex = iFileIndex;
  m_iShare = _SH_DENYNO;
  m_logFile = (char*) LOG_FILE_NAME_CONG;
  m_fileLogMode = mode;
  /////////////////////////////////////
  // Default log value:
  m_iLogValue = TML_LOG_OFF;

  ////////////////////////////////////////////////////////////////////////////
  // create the mutex that protect critial section about communication data:
  axl_bool bSuccess;
  bSuccess = createCriticalSectionObject(&m_mutexCriticalSection);
#ifndef DISABLE_TML_LOGGING
  /////////////////////////////////////
  // Logpath
  m_logPath = new char[1024];
  getLogStringValue((char*)TML_OPT_LOG_PATH, &m_logPath, 1024);

  /////////////////////////////////////
  // Allocate memory for the date / time:
  m_date = new char[DATE_TIME_STRING_LENGTH];
  m_time = new char[DATE_TIME_STRING_LENGTH];
  m_hLogFile = NULL;
  m_bUseLogfile = true;
  m_bLogIntoFile = checkLogOption((char*)TML_OPT_ENABLE_FILE_LOG);
  m_bUseLogfileMask = SIDEX_FALSE;
  m_iLogfileMask = TML_LOG_OFF;
#endif // DISABLE_TML_LOGGING
}


/**
 * @brief    Destructor.
 */
tmlLogHandler::~tmlLogHandler(){
#ifndef DISABLE_TML_LOGGING
  /////////////////////////////////////
  // Free memory for the date / time:
  delete m_date;
  delete m_time;

  if (SIDEX_TRUE == m_bLogIntoFile){
    if (NULL != m_hLogFile){
      fclose(m_hLogFile);
    }
  }
  delete m_logPath;
#endif // DISABLE_TML_LOGGING

  ////////////////////////////////////////////////////////////////////////////
  // Destroy the mutex that protect critial section about communication data:
  destroyCriticalSectionObject(&m_mutexCriticalSection);
}


/**
 * @brief    Enable / disable logging
 */
void tmlLogHandler::setLoggingValue(int iLogValue){
#ifndef DISABLE_TML_LOGGING
  if (SIDEX_TRUE == m_bUseLogfileMask){
    m_iLogValue = (int)m_iLogfileMask; // set the logging value
  }
  else{
    m_iLogValue = iLogValue; // set the logging value
  }
#endif // DISABLE_TML_LOGGING
}


/**
 * @brief    retuns the logging value
 */
int tmlLogHandler::getLoggingValue(){
#ifdef DISABLE_TML_LOGGING
  return TML_LOG_OFF;
#else // DISABLE_TML_LOGGING
  return m_iLogValue;
#endif // DISABLE_TML_LOGGING
}


/**
 * @brief   calculates the next contiguous file index
 */
int tmlLogHandler::calcContFileIndex(){
      int i = 1;
#ifndef DISABLE_TML_LOGGING
      char pathname[256];
      FILE* hLogFile = NULL;
      do{
        if (NULL != hLogFile){
          fclose(hLogFile);
          hLogFile = NULL;
        }
        if (i < 10)
  #ifdef LINUX
          sprintf(pathname, "%s%s0%d%s", m_logPath, LOG_FILE_NAME_CONG , i++, LOG_FILE_EXT);
        else
          sprintf(pathname, "%s%s%d%s", m_logPath, LOG_FILE_NAME_CONG, i++, LOG_FILE_EXT);
  #else // LINUX
    #if _MSC_VER > 1200

          sprintf_s(pathname, 256, "%s%s0%d%s", m_logPath, LOG_FILE_NAME_CONG , i++, LOG_FILE_EXT);
        else
          sprintf_s(pathname, 256, "%s%s%d%s", m_logPath, LOG_FILE_NAME_CONG, i++, LOG_FILE_EXT);
    #else
          sprintf(pathname, "%s%s0%d%s", m_logPath, LOG_FILE_NAME_CONG , i++, LOG_FILE_EXT);
        else
          sprintf(pathname, "%s%s%d%s", m_logPath, LOG_FILE_NAME_CONG, i++, LOG_FILE_EXT);
    #endif
  #endif // LINUX
        hLogFile = _fsopen(pathname, "r", _SH_DENYNO);
      }
      while (NULL != hLogFile);
#endif // DISABLE_TML_LOGGING
      return i-1;
}

FILE* tmlLogHandler::getLogFile(){
#ifndef DISABLE_TML_LOGGING
  if (SIDEX_TRUE == m_bLogIntoFile){
    if (NULL == m_hLogFile && m_bUseLogfile){

      char pathname[256];
      int i = 1;

      if (m_iLogFileIndex > 0){
        i = m_iLogFileIndex;
        do{
          if (i < 10)
    #ifdef LINUX
            sprintf(pathname, "%s%s0%d%s", m_logPath, m_logFile , i++, LOG_FILE_EXT);
          else
            sprintf(pathname, "%s%s%d%s", m_logPath, m_logFile, i++, LOG_FILE_EXT);
    #else // LINUX
      #if _MSC_VER > 1200

            sprintf_s(pathname, 256, "%s%s0%d%s", m_logPath, m_logFile , i++, LOG_FILE_EXT);
          else
            sprintf_s(pathname, 256, "%s%s%d%s", m_logPath, m_logFile, i++, LOG_FILE_EXT);
      #else
            sprintf(pathname, "%s%s0%d%s", m_logPath, m_logFile , i++, LOG_FILE_EXT);
          else
            sprintf(pathname, "%s%s%d%s", m_logPath, m_logFile, i++, LOG_FILE_EXT);
      #endif
    #endif // LINUX
          m_hLogFile = _fsopen(pathname, m_fileLogMode, m_iShare);
        }
        while (NULL == m_hLogFile && (NO_OF_LOGFILES+1) > i);
      }
      else{
        bool bFound = false;
        do{
          bFound = false;
          if (i < 10)
    #ifdef LINUX
            sprintf(pathname, "%s%s0%d%s", m_logPath, m_logFile , i++, LOG_FILE_EXT);
          else
            sprintf(pathname, "%s%s%d%s", m_logPath, m_logFile, i++, LOG_FILE_EXT);
    #else // LINUX
      #if _MSC_VER > 1200

            sprintf_s(pathname, 256, "%s%s0%d%s", m_logPath, m_logFile , i++, LOG_FILE_EXT);
          else
            sprintf_s(pathname, 256, "%s%s%d%s", m_logPath, m_logFile, i++, LOG_FILE_EXT);
      #else
            sprintf(pathname, "%s%s0%d%s", m_logPath, m_logFile , i++, LOG_FILE_EXT);
          else
            sprintf(pathname, "%s%s%d%s", m_logPath, m_logFile, i++, LOG_FILE_EXT);
      #endif
    #endif // LINUX

          FILE* tmpFile = _fsopen(pathname, "r", _SH_DENYNO);
          if (NULL != tmpFile){
            bFound = true;
            fclose(tmpFile);
          }
          else{
            m_hLogFile = _fsopen(pathname, m_fileLogMode, m_iShare);
          }
        }
        while (bFound || NULL == m_hLogFile);
      }

      if (NULL == m_hLogFile && (NO_OF_LOGFILES+1) == i){
        m_bUseLogfile = false;
      }
      else{
        m_iLogFileIndex = i-1;
      }
    }
  }
#endif // DISABLE_TML_LOGGING
  return m_hLogFile;
}

/**
 * @brief    Getting actual system date / time
 */
void tmlLogHandler::getDate(char** sDate, int iDateLength, char** sTime, int iTimeLength)
{
#ifdef LINUX
  time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  struct timeval detail_time;
  gettimeofday(&detail_time,NULL);
  int iMilliSeconds = detail_time.tv_usec % 1000;

  sprintf(*sDate, "%4d.%2d.%2d", timeinfo->tm_year + 1900,
                                    timeinfo->tm_mon + 1,
                                      timeinfo->tm_mday);
  sprintf(*sTime, "%2d:%2d:%2d:%3d", timeinfo->tm_hour,
                                      timeinfo->tm_min,
                                        timeinfo->tm_sec,
                                           iMilliSeconds);
#else // LINUX
  SYSTEMTIME lpSystemTime;
  GetLocalTime(&lpSystemTime);


#if _MSC_VER > 1200
  sprintf_s(*sDate, iDateLength, "%4d.%2d.%2d", lpSystemTime.wYear,
                                                        lpSystemTime.wMonth,
                                                          lpSystemTime.wDay);
  sprintf_s(*sTime, iTimeLength, "%2d:%2d:%2d:%3d", lpSystemTime.wHour,
                                                            lpSystemTime.wMinute,
                                                              lpSystemTime.wSecond,
                                                               lpSystemTime.wMilliseconds);
#else
  sprintf(*sDate, "%4d.%2d.%2d", lpSystemTime.wYear,
                                                        lpSystemTime.wMonth,
                                                          lpSystemTime.wDay);
  sprintf(*sTime, "%2d:%2d:%2d:%3d", lpSystemTime.wHour,
                                                            lpSystemTime.wMinute,
                                                              lpSystemTime.wSecond,
                                                               lpSystemTime.wMilliseconds);
#endif // _MSC_VER > 1000
#endif // LINUX
  // Replace each blank with the '0' character:
  char* c;
  c = *sDate;
  unsigned int i;
  for (i = 0; i < strlen(c);++i){
    if (' ' == c[i]){
      c[i] = '0';
    }
  }
  c = *sTime;
  for (i = 0; i < strlen(c);++i){
    if (' ' == c[i]){
      c[i] = '0';
    }
  }
}


/**
 * @brief    Getting actual system date / time
 */
TML_INT64 tmlLogHandler::getTime(char* sDate, int iDateLength, char* sTime, int iTimeLength)
{
  TML_INT64 iTime = 0;
#ifdef LINUX
  time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  struct timeval detail_time;
  gettimeofday(&detail_time,NULL);
  int iMilliSeconds = detail_time.tv_usec % 1000;

  sprintf(sDate, "%4d.%2d.%2d", timeinfo->tm_year + 1900,
                                    timeinfo->tm_mon + 1,
                                      timeinfo->tm_mday);
  sprintf(sTime, "%2d:%2d:%2d:%3d", timeinfo->tm_hour,
                                      timeinfo->tm_min,
                                        timeinfo->tm_sec,
                                           iMilliSeconds);
  iTime = (((((timeinfo->tm_mday * 24) + (timeinfo->tm_hour * 60)+ timeinfo->tm_min)*60)+ timeinfo->tm_sec)*1000) + iMilliSeconds;
#else // LINUX
  SYSTEMTIME lpSystemTime;
  GetLocalTime(&lpSystemTime);
  iTime = (((((lpSystemTime.wDay * 24) + (lpSystemTime.wHour * 60)+ lpSystemTime.wMinute)*60)+ lpSystemTime.wSecond)*1000) + lpSystemTime.wMilliseconds;


#if _MSC_VER > 1200
  sprintf_s(sDate, iDateLength, "%4d.%2d.%2d", lpSystemTime.wYear,
                                                        lpSystemTime.wMonth,
                                                          lpSystemTime.wDay);
  sprintf_s(sTime, iTimeLength, "%2d:%2d:%2d:%3d", lpSystemTime.wHour,
                                                            lpSystemTime.wMinute,
                                                              lpSystemTime.wSecond,
                                                               lpSystemTime.wMilliseconds);
#else
  sprintf(sDate, "%4d.%2d.%2d", lpSystemTime.wYear,
                                                        lpSystemTime.wMonth,
                                                          lpSystemTime.wDay);
  sprintf(sTime, "%2d:%2d:%2d:%3d", lpSystemTime.wHour,
                                                            lpSystemTime.wMinute,
                                                              lpSystemTime.wSecond,
                                                               lpSystemTime.wMilliseconds);
#endif // _MSC_VER > 1000
#endif // LINUX
  // Replace each blank with the '0' character:
  char* c;
  c = sDate;
  unsigned int i;
  for (i = 0; i < strlen(c);++i){
    if (' ' == c[i]){
      c[i] = '0';
    }
  }
  c = sTime;
  for (i = 0; i < strlen(c);++i){
    if (' ' == c[i]){
      c[i] = '0';
    }
  }
  return iTime;
}


/**
 * @brief    write the debug log to the stdout
 */
void tmlLogHandler::log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
#ifndef DISABLE_TML_LOGGING
  //bool bLog = (m_iLogValue & iLogMask || -1 == iLogMask) != 0;
  bool bLog = (m_iLogValue & iLogMask) != 0;
  if (bLog){
    ///////////////////////////////////////////////////////////////////////////
    // Begin of critical section
    enterCriticalSection (&m_mutexCriticalSection);

    getDate (&m_date, DATE_TIME_STRING_LENGTH, &m_time, DATE_TIME_STRING_LENGTH);
    printf ("%s %s %s:%s:%s:%s\n", m_date, m_time, sClass, sMethod, sFormatLog, sLog);
    if (SIDEX_TRUE == m_bLogIntoFile){
      FILE* hLogFile = getLogFile();
      if (NULL != hLogFile){
        fprintf(hLogFile, "%s %s %s:%s:%s:%s\n", m_date, m_time, sClass, sMethod, sFormatLog, sLog);
#ifdef DO_FLUSH
        fflush (hLogFile);    // flushing or repositioning required
#endif // DO_FLUSH
      }
    }

    ///////////////////////////////////////////////////////////////////////////
    // End of critical section
    leaveCriticalSection (&m_mutexCriticalSection);
  }
#endif // DISABLE_TML_LOGGING
}


/**
 * @brief    write the debug log to the stdout
 */
void tmlLogHandler::log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, SIDEX_INT64 iVal)
{
#ifndef DISABLE_TML_LOGGING
  bool bLog = (m_iLogValue & iLogMask) != 0;
  if (bLog){
    ///////////////////////////////////////////////////////////////////////////
    // Begin of critical section
    enterCriticalSection (&m_mutexCriticalSection);

    getDate (&m_date, DATE_TIME_STRING_LENGTH, &m_time, DATE_TIME_STRING_LENGTH);
    printf ("%s %s %s:%s:%s:%lld\n", m_date, m_time, sClass, sMethod, sFormatLog, iVal);
    if (SIDEX_TRUE == m_bLogIntoFile){
      FILE* hLogFile = getLogFile();
      if (NULL != hLogFile){
        fprintf(hLogFile, "%s %s %s:%s:%s:%lld\n", m_date, m_time, sClass, sMethod, sFormatLog, iVal);
#ifdef DO_FLUSH
        fflush (hLogFile);    // flushing or repositioning required
#endif // DO_FLUSH
      }
    }

    ///////////////////////////////////////////////////////////////////////////
    // End of critical section
    leaveCriticalSection (&m_mutexCriticalSection);
  }
#endif // DISABLE_TML_LOGGING
}

/**
 * @brief    write the debug log to the stdout
 */
void tmlLogHandler::log(const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog)
{
#ifndef DISABLE_TML_LOGGING
  ///////////////////////////////////////////////////////////////////////////
  // Begin of critical section
  enterCriticalSection (&m_mutexCriticalSection);

  getDate (&m_date, DATE_TIME_STRING_LENGTH, &m_time, DATE_TIME_STRING_LENGTH);
  printf ("%s %s %s:%s:%s:%s\n", m_date, m_time, sClass, sMethod, sFormatLog, sLog);
  if (SIDEX_TRUE == m_bLogIntoFile){
    FILE* hLogFile = getLogFile();
    if (NULL != hLogFile){
      fprintf(hLogFile, "%s %s %s:%s:%s:%s\n", m_date, m_time, sClass, sMethod, sFormatLog, sLog);
#ifdef DO_FLUSH
      fflush (hLogFile);    // flushing or repositioning required
#endif // DO_FLUSH
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  // End of critical section
  leaveCriticalSection (&m_mutexCriticalSection);
#endif // DISABLE_TML_LOGGING
}


/**
 * @brief    write the debug log to the stdout
 */
void tmlLogHandler::log(const char* sClass,const  char* sMethod, const char* sFormatLog, SIDEX_INT64 iVal)
{
#ifndef DISABLE_TML_LOGGING
  ///////////////////////////////////////////////////////////////////////////
  // Begin of critical section
  enterCriticalSection (&m_mutexCriticalSection);

  getDate (&m_date, DATE_TIME_STRING_LENGTH, &m_time, DATE_TIME_STRING_LENGTH);
  printf ("%s %s %s:%s:%s:%lld\n", m_date, m_time, sClass, sMethod, sFormatLog, iVal);
  if (SIDEX_TRUE == m_bLogIntoFile){
    FILE* hLogFile = getLogFile();
    if (NULL != hLogFile){
      fprintf(hLogFile, "%s %s %s:%s:%s:%lld\n", m_date, m_time, sClass, sMethod, sFormatLog, iVal);
#ifdef DO_FLUSH
      fflush (hLogFile);    // flushing or repositioning required
#endif // DO_FLUSH
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  // End of critical section
  leaveCriticalSection (&m_mutexCriticalSection);
#endif // DISABLE_TML_LOGGING
}


/**
  * @brief    returns the index of the contiguous log file
  */
int tmlLogHandler::getLogFileIndex(){
  return m_iLogFileIndex;
}


/**
  * @brief    returns additional timeout value
  */
unsigned int tmlLogHandler::getAdditionalTimeout(){
  unsigned int iRet = (unsigned int) m_iAdditionalTimeout;
  return iRet;
}


/**
  * @brief    returns event  queue remove log period
  *
  * @returns event log period
  */
unsigned int tmlLogHandler::getEventQueueRemoveLogPeriod(){
  unsigned int iRet = (unsigned int) m_iEventQueueRemoveLogPeriod;
  return iRet;
}


/**
  * @brief    returns event log period
  *
  * @returns event log period
  */
unsigned int tmlLogHandler::getEventLogPeriod(){
  unsigned int iRet = (unsigned int) m_iEventLogPeriod;
  return iRet;
}


/**
     * @brief    returns thread pool add period
  */
unsigned int tmlLogHandler::getThreadPoolAddPeriod(unsigned int iDefault){
  unsigned int iRet = iDefault;
  if (-9999 != m_iThreadPoolAddPeriod)
    iRet = (unsigned int) m_iThreadPoolAddPeriod;
  return iRet;
}


/**
 * @brief    Init redirection in case of windows application
 */
void tmlLogHandler::RedirectIOToConsole()
{
#ifdef USE_REDIRECTION
  int hConHandle;
  long lStdHandle;
  CONSOLE_SCREEN_BUFFER_INFO coninfo;
  FILE *fp;

  // allocate a console for this app

  AllocConsole();

  // set the screen buffer to be big enough to let us scroll text

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&coninfo);
  coninfo.dwSize.Y = 10000;///MAX_CONSOLE_LINES;
  SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
  coninfo.dwSize);
  // redirect unbuffered STDOUT to the console
  lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "w" );
  *stdout = *fp;
  setvbuf( stdout, NULL, _IONBF, 0 );

  // redirect unbuffered STDIN to the console
  lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "r" );
  *stdin = *fp;
  setvbuf( stdin, NULL, _IONBF, 0 );

  // redirect unbuffered STDERR to the console

  lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "w" );
  *stderr = *fp;
  setvbuf( stderr, NULL, _IONBF, 0 );

  // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
  // point to console as well

  ios::sync_with_stdio();
#endif // USE_REDIRECTION
}


/**
  * @brief    Check a tmlPreferences Option
  */
SIDEX_BOOL tmlLogHandler::checkLogOption(char* sKey)
{
  SIDEX_BOOL bRet = SIDEX_FALSE;

  SIDEX_HANDLE shandle;
  SIDEX_INT32 iRet = sidex_Create_A(SIDEX_ROOT, &shandle);
  if (SIDEX_SUCCESS == iRet){
  char pathname[256];
#ifdef LINUX
     sprintf(pathname, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
#else // LINUX
  #if _MSC_VER > 1200
     sprintf_s(pathname, 256, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
  #else
     sprintf(pathname, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
  #endif
#endif // LINUX
    iRet = sidex_Load_Content_A (shandle, pathname);
    if (SIDEX_SUCCESS == iRet){
      SIDEX_BOOL bValue;
      iRet = sidex_Boolean_Read_A (shandle, (char*)TML_PREFERENCES_GROUP, sKey, &bValue);
      if (SIDEX_SUCCESS == iRet){
        bRet = bValue;
      }
    }
    sidex_Free (&shandle);
  }
  return bRet;
}


/**
  * @brief    Get an Integer Value out of the tmlPreferences
  */
SIDEX_INT64 tmlLogHandler::getLogIntegerValue(char* sKey)
{
  SIDEX_INT64 iRetValue = -1;

  SIDEX_HANDLE shandle;
  SIDEX_INT32 iRet = sidex_Create_A(SIDEX_ROOT, &shandle);
  if (SIDEX_SUCCESS == iRet){
  char pathname[256];
#ifdef LINUX
     sprintf(pathname, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
#else // LINUX
  #if _MSC_VER > 1200
     sprintf_s(pathname, 256, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
  #else
     sprintf(pathname, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
  #endif
#endif // LINUX
    iRet = sidex_Load_Content_A (shandle, pathname);
    if (SIDEX_SUCCESS == iRet){
      SIDEX_INT64 iValue = 0;
      iRet = sidex_Integer_Read_A (shandle, (char*)TML_PREFERENCES_GROUP, sKey, &iValue);
      if (SIDEX_SUCCESS == iRet){
        iRetValue = iValue;
      }
    }
    sidex_Free (&shandle);
  }
  return iRetValue;
}



/**
  * @brief    Get a String Value out of the tmlPreferences
  */
void tmlLogHandler::getLogStringValue(char* sKey, char** sRet, SIDEX_INT32 iRetLength)
{
  SIDEX_INT32 iStringLength = -1;

  SIDEX_HANDLE shandle;
  SIDEX_INT32 iRet = sidex_Create_A(SIDEX_ROOT, &shandle);
  if (SIDEX_SUCCESS == iRet){
  char pathname[256];
#ifdef LINUX
     sprintf(pathname, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
#else // LINUX
  #if _MSC_VER > 1200
     sprintf_s(pathname, 256, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
  #else
     sprintf(pathname, "%s%s", LOG_FILE_PATH, TML_PREFERENCES_FILE);
  #endif
#endif // LINUX
    iRet = sidex_Load_Content_A (shandle, pathname);
    if (SIDEX_SUCCESS == iRet){
      char* sValue = 0;
      iRet = sidex_String_Read_A (shandle, (char*)TML_PREFERENCES_GROUP, sKey, &sValue, &iStringLength);
      if (SIDEX_SUCCESS == iRet){
        if (iRetLength > iStringLength){
          #ifdef LINUX
            strcpy (*sRet, sValue);
          #else // LINUX
            #if _MSC_VER > 1200
              strcpy_s (*sRet, iRetLength, sValue);
            #else
              strcpy(*sRet, sValue);
            #endif // _MSC_VER > 1000
          #endif // LINUX
        }
        sidex_Free_ReadString_A(sValue);
      }
    }
    sidex_Free (&shandle);
  }
  if (SIDEX_SUCCESS != iRet){
    if (iRetLength > iStringLength){
      #ifdef LINUX
        strcpy (*sRet, LOG_FILE_PATH);
      #else // LINUX
        #if _MSC_VER > 1200
         strcpy_s (*sRet, iRetLength, LOG_FILE_PATH);
        #else
         strcpy(*sRet, LOG_FILE_PATH);
        #endif // _MSC_VER > 1000
      #endif // LINUX
    }
  }
  return;
}

    /**
     * @brief    Create the mutex that protect critial section about communication data
     */
    axl_bool tmlLogHandler::createCriticalSectionObject(VortexMutex* mutex)
    {
      axl_bool bSuccess = axl_true;
      bSuccess = intern_mutex_create (mutex);
      return bSuccess;
    }


    /**
     * @brief    Destroy the mutex that protect critial section about communication data
     */
    axl_bool tmlLogHandler::destroyCriticalSectionObject(VortexMutex* mutex)
    {
      axl_bool bSuccess = axl_true;
      bSuccess = intern_mutex_destroy (mutex, (char*)"destroyCriticalSectionObject");
      return bSuccess;
    }


    /**
     * @brief    Enter a critical Section
     */
    void tmlLogHandler::enterCriticalSection(VortexMutex* mutex)
    {
      intern_mutex_lock (mutex, NULL, NULL);
      return;
    }


    /**
     * @brief    Leave a critical Section
     */
    void tmlLogHandler::leaveCriticalSection(VortexMutex* mutex)
    {
      intern_mutex_unlock (mutex, NULL, NULL);
      return;
    }




