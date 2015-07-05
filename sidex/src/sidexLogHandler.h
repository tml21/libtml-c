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
 
#ifndef sidexLogHandler_H
#define sidexLogHandler_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>      // printf
#include "sidexStdTypes.h"

#if defined(LINUX) || defined (MINGW_BUILD)
#define _SH_DENYRW      0x10    /* deny read/write mode */
#define _SH_DENYWR      0x20    /* deny write mode */
#define _SH_DENYRD      0x30    /* deny read mode */
#define _SH_DENYNO      0x40    /* deny none mode */

FILE *_fsopen(const char *filename,const char *mode,int shflag);
#endif // LINUX


class sidexLogHandler{

/**
* @brief    Definition of logfile pathame
*/
#ifdef LINUX
#define LOG_FILE_PATH      "/tmp/"
#else // LINUX
#define LOG_FILE_PATH      "C:/temp/"
#endif // LINUX
#define LOG_FILE_NAME      "tmlLog"
#define LOG_FILE_NAME_CONG "tmlContiguous"
#define LOG_FILE_EXT       ".txt"

/**
* @brief  Definitions of logfile parameter
*/
#define SIDEX_PREFERENCES_FILE    "tmlCore.sdx"
#define SIDEX_ROOT              (const char*)"ROOT"
#define SIDEX_PREFERENCES_GROUP   "tmlCorePreferences"
#define SIDEX_OPT_ENABLE_FILE_LOG "EnableFileLog"
#define SIDEX_OPT_USE_LOG_MASK    "UseLogMask"
#define SIDEX_OPT_LOG_MASK_VALUE  "LogMaskValue"
#define SIDEX_OPT_LOG_PATH        "LogPath"

#define DEFAULT_EVENT_MSG_LOG_PERIOD 500
#define DEFAULT_EVENT_QUEUE_REMOVE_LOG_PERIOD 500


/**
* @brief  Definitions of logfile parameter
*/
#define SIDEX_ADDITIONAL_TIMEOUT    "AdditionalTimeout"

/**
* @brief  Definitions of logfile parameter
*/
#define SIDEX_EVENT_LOG_PERIOD    "EventLogPeriod"

/**
* @brief  Definitions of logfile parameter
*/
#define SIDEX_EVENT_QUEUE_REMOVE_LOG_PERIOD    "EventQueueRemoveLogPeriod"

/**
* @brief  Definitions of logfile parameter
*/
#define SIDEX_THREAD_POOL_ADD_PERIOD    "ThreadAddPeriod"

  /**
* @brief    Maximum amount of logfiles
*/
#define NO_OF_LOGFILES 2000


/**
* @brief    Definition of lof file write mode
*/
#define LOG_FILE_OPEN_MODE "w" // "a+"


/**
* @brief    Definition of string length for date ot time
*/
#define DATE_TIME_STRING_LENGTH 32

private:
    /* data */


    /**
     * @brief    log path
     */
    char* m_logPath;


    /**
     * @brief    log filename
     */
    char* m_logFile;

    /**
     * @brief    log into file if true
     */
    SIDEX_BOOL m_bLogIntoFile;

    int m_iShare;

    /**
     * @brief   Log- File index for explicit logs with closing the file
     */
    int m_iLogFileIndex;

    /**
     * @brief    Flag for logging
     */
    int m_iLogValue;

    /**
     * @brief    Flag for logfile use
     */
    bool m_bUseLogfile;

    /**
     * @brief    Flag for log mask used out of logfile content
     */
    SIDEX_BOOL m_bUseLogfileMask;

    /**
     * @brief    mask returned out of logfile content
     */
    SIDEX_INT64 m_iLogfileMask;


    /**
     * @brief   additional timeout value for send commands
     */
    SIDEX_INT64 m_iAdditionalTimeout;


    /**
     * @brief   Period for event logging
     */
    SIDEX_INT64 m_iEventLogPeriod;


    /**
     * @brief   Period for event logging
     */
    SIDEX_INT64 m_iEventQueueRemoveLogPeriod;


    /**
     * @brief   time to reach untill additional threads will be created
     */
    SIDEX_INT64 m_iThreadPoolAddPeriod;


    /**
     * @brief    Handle to log file
     */
    FILE *m_hLogFile;

    /**
     * @brief    Reference to formatted date string
     */
    char* m_date;

    /**
     * @brief    Reference to formatted time string
     */
    char* m_time;

    char* m_fileLogMode;

    /**
     * @brief    Getting actual system date / time
     *
     * @param   sDate       Reference string to write te formatted date into
     * @param   iDateLength allocated length of the date string
     * @param   sTime       Reference string to write te formatted time into
     * @param   iTimeLength allocated length of the time string
     */
    void getDate(char** sDate, int iDateLength, char** sTime, int iTimeLength);

    /**
     * @brief    Init redirection in case of windows application
     */
    void RedirectIOToConsole();

public:
    /* data */
    /* Methods */

    /**
     * @brief    Getting actual system date / time
     *
     * @param   sDate       Reference string to write te formatted date into
     * @param   iDateLength allocated length of the date string
     * @param   sTime       Reference string to write te formatted time into
     * @param   iTimeLength allocated length of the time string
     */
    static SIDEX_INT64 getTime(char* sDate, int iDateLength, char* sTime, int iTimeLength);

    /**
     * @brief    Constructor.
     *
     * @returns an instance of sidexLogHandler
     */
    sidexLogHandler();


    /**
     * @brief    Constructor.
     *
     * @param   mode       File open- mode for the debug- file logging
     *
     * @returns an instance of sidexLogHandler
     */
    sidexLogHandler(char* mode);


    /**
     * @brief    Constructor.
     *
     * @param   mode       File open- mode for the debug- file logging into a contiguous log file
     * @param   iFileIndex Index of the contiguous log file
     *
     * @returns an instance of sidexLogHandler
     */
    sidexLogHandler(char* mode, int iFileIndex);



    /**
     * @brief    Destructor.
     */
    ~sidexLogHandler();


    /**
     * @brief    set the logging value
     *
     * @param   iLogValue    The debug logging value.
     *
     */
    void setLoggingValue(int iLogValue);


    /**
     * @brief    retuns the logging value
     *
     * @returns true the logging value
     *
     */
    int getLoggingValue();


    /**
     * @brief   returns the log file handle
     *
     * @returns the log file handle
     *
     */
    FILE* getLogFile();

    /**
     * @brief    write the debug log to the stdout
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     */
    void log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    write the debug log to the stdout
     *
     * @param   iLogMask    The mask. If the mask matches the iLogValue the output will be done
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   iVal        Integer value.
     *
     */
    void log(int iLogMask, const char* sClass, const char* sMethod, const char* sFormatLog, SIDEX_INT64 iVal);


    /**
     * @brief    write the debug log to the stdout
     *
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   sLog        Reference to the log text.
     *
     */
    void log(const char* sClass, const char* sMethod, const char* sFormatLog, const char* sLog);


    /**
     * @brief    write the debug log to the stdout
     *
     * @param   sClass      Reference to the class name.
     * @param   sMethod     Reference to the method name.
     * @param   sFormatLog  Reference to the log specifier text.
     * @param   iVal        Integer value.
     *
     */
    void log(const char* const sClass, const char* sMethod, const char* sFormatLog, SIDEX_INT64 iVal);
    
    
    /**
     * @brief    returns the index of the contiguous log file
     */
    int getLogFileIndex();


    /**
     * @brief   calculates the next contiguous file index
     */
    int calcContFileIndex();

    /**
     * @brief    returns additional timeout value
     *
     * @returns next contiguous lof file index
     */
    unsigned int getAdditionalTimeout();


    /**
     * @brief    returns event log period
     *
     * @returns event log period
     */
    unsigned int getEventLogPeriod();



    /**
     * @brief    returns event queue remove log period
     *
     * @returns event log period
     */
    unsigned int getEventQueueRemoveLogPeriod();


    /**
     * @brief    returns thread pool add period
     *
     * @returns thread pool add period
     */
    unsigned int getThreadPoolAddPeriod(unsigned int iDefault);

  
    /**
     * @brief    check log option in preferences file (stored in local directory)
     *
     * @param   sKey        The Key for the request.
     *
     * @returns SIDEX_TRUE, if option is valid and true.
     */
    SIDEX_BOOL checkLogOption(char* sKey);


    /**
     * @brief    Get an Integer Value out of the tmlPreferences
     *
     * @param   sKey        The Key for the request.
     *
     * @returns the integer value / if not found the value is -1.
     */
    SIDEX_INT64 getLogIntegerValue(char* sKey);


     /**
      * @brief    Get a String Value out of the tmlPreferences
      */
    void getLogStringValue(char* sKey, char** sRet, SIDEX_INT32 iRetLength);
};

#endif  // sidexLogHandler_H

