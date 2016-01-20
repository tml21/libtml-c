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
 * Contributors:
 *    wobe-systems GmbH
 */
 
#include "sidexDateTime.h"
#include "sidexErrors.h"
#include <string.h>
#include "unicode.h"


using namespace std;

#define DEFAULT_DATETIME "2002-02-02 22:22:22:222"
#define DATE_TIME_STR_LENGTH 24

/**
 * @brief    Constructor.
 */
sidexDateTime::sidexDateTime()
{
   value = new char[DATE_TIME_STR_LENGTH];
#if defined(LINUX) || defined (MINGW_BUILD)
     strcpy (value, DEFAULT_DATETIME);
#else
    strcpy_s (value, DATE_TIME_STR_LENGTH, DEFAULT_DATETIME);
#endif
   incRef();
   xValue = NULL;
   wValue = NULL;
}


/**
 * @brief    Constructor with initial value.
 */
sidexDateTime::sidexDateTime(char* newValue)
{
   value = new char[DATE_TIME_STR_LENGTH];
   
   if (sidexDateCheck(newValue)){
#if defined(LINUX) || defined (MINGW_BUILD)
     strcpy (value, newValue);
#else
    strcpy_s (value, DATE_TIME_STR_LENGTH, newValue);
#endif
   }
   else{
#if defined(LINUX) || defined (MINGW_BUILD)
     strcpy (value, DEFAULT_DATETIME);
#else
    strcpy_s (value, DATE_TIME_STR_LENGTH, DEFAULT_DATETIME);
#endif
   }
   incRef();
   xValue = NULL;
   wValue = NULL;
}

/**
 * @brief    Destructor.
 */
sidexDateTime::~sidexDateTime()
{
	cleanUp();
}


/**
 * @brief    Check the DateTime string for correct format
 */
bool sidexDateTime::sidexDateCheck(char* sDateTime){
  bool bRet = true;

  ///////////////////
  // NULL
  if (NULL == sDateTime){
    bRet = false;
  }
  else{
    ///////////////////
    // Length:
    if (bRet && DATE_TIME_STR_LENGTH-1 != strlen(sDateTime)){
      bRet = false;
    }
    ///////////////////
    // Spaces:
    if (bRet && sDateTime[4] != '-'){
      bRet = false;
    }
    if (bRet && sDateTime[7] != '-'){
      bRet = false;
    }
    if (bRet && sDateTime[10] != ' '){
      bRet = false;
    }
    if (bRet && sDateTime[13] != ':'){
      bRet = false;
    }
    if (bRet && sDateTime[16] != ':'){
      bRet = false;
    }
    if (bRet && sDateTime[19] != ':'){
      bRet = false;
    }
  }
  ///////////////////
  // undefined date:
  if (bRet){
    if (sDateTime[8] == '0' && sDateTime[9] == '0'){
      if (sDateTime[0] != '0' || sDateTime[1] != '0' || sDateTime[2] != '0' || sDateTime[3] != '0' || sDateTime[5] != '0' || sDateTime[6] != '0'){
        bRet = false;
      }
    }
  }
  if (bRet){
    if (sDateTime[5] == '0' && sDateTime[6] == '0'){
      if (sDateTime[0] != '0' || sDateTime[1] != '0' || sDateTime[2] != '0' || sDateTime[3] != '0' || sDateTime[8] != '0' || sDateTime[9] != '0'){
        bRet = false;
      }
    }
  }
  if (bRet){
    if (sDateTime[0] == '0' && sDateTime[1] == '0' && sDateTime[2] == '0' && sDateTime[3] == '0'){
      if (sDateTime[5] != '0' || sDateTime[6] != '0'  || sDateTime[8] != '0' || sDateTime[9] != '0'){
        bRet = false;
      }
    }
  }
  ///////////////////
  // YEAR:
  if (bRet && (sDateTime[0] < '0' || sDateTime[0] > '9')){
    bRet = false;
  }
  if (bRet && (sDateTime[1] < '0' || sDateTime[1] > '9')){
    bRet = false;
  }
  if (bRet && (sDateTime[2] < '0' || sDateTime[2] > '9')){
    bRet = false;
  }
  if (bRet && (sDateTime[3] < '0' || sDateTime[3] > '9')){
    bRet = false;
  }
  ///////////////////
  // Month:
  if (bRet && (sDateTime[5] < '0' || sDateTime[5] > '1')){
    bRet = false;
  }
  if (bRet && (sDateTime[6] < '0' || sDateTime[6] > '9')){
    bRet = false;
  }
  if (bRet && sDateTime[5] == '1'){
    if (bRet && (sDateTime[6] < '0' || sDateTime[6] > '2')){
      bRet = false;
    }
  }
  ///////////////////
  // Day of month:
  if (bRet && (sDateTime[8] < '0' || sDateTime[8] > '3')){
    bRet = false;
  }
  if (bRet && (sDateTime[9] < '0' || sDateTime[9] > '9')){
    bRet = false;
  }
  if (bRet && sDateTime[8] == '3'){
    if (bRet && (sDateTime[9] < '0' || sDateTime[9] > '1')){
      bRet = false;
    }
  }
  ///////////////////
  // Hour
  if (bRet && (sDateTime[11] < '0' || sDateTime[11] > '2')){
    bRet = false;
  }
  if (bRet && (sDateTime[12] < '0' || sDateTime[12] > '9')){
    bRet = false;
  }
  if (bRet && sDateTime[11] == '2'){
    if (bRet && (sDateTime[12] < '0' || sDateTime[12] > '3')){
      bRet = false;
    }
  }
  ///////////////////
  // Minute
  if (bRet && (sDateTime[14] < '0' || sDateTime[14] > '5')){
    bRet = false;
  }
  if (bRet && (sDateTime[15] < '0' || sDateTime[15] > '9')){
    bRet = false;
  }
  ///////////////////
  // Second
  if (bRet && (sDateTime[17] < '0' || sDateTime[17] > '5')){
    bRet = false;
  }
  if (bRet && (sDateTime[18] < '0' || sDateTime[18] > '9')){
    bRet = false;
  }
  ///////////////////
  // Milliecond
  if (bRet && (sDateTime[20] < '0' || sDateTime[20] > '9')){
    bRet = false;
  }
  if (bRet && (sDateTime[21] < '0' || sDateTime[21] > '9')){
    bRet = false;
  }
  if (bRet && (sDateTime[22] < '0' || sDateTime[22] > '9')){
    bRet = false;
  }
  return bRet;
}


/**
 * @brief    Cleans up refCounter dependent allocations
 */
void sidexDateTime::cleanUp(){
  if (getRef()){
    if (decRef() == 0){
      delete[] value;
      value = NULL;
      if (NULL != xValue){
        delete xValue;
        xValue = NULL;
      }
      if (NULL != wValue){
        delete wValue;
        wValue = NULL;
      }
    }
  }
}


/**
 * @brief   Get type of this data object.
 */
SIDEX_DATA_TYPE sidexDateTime::getType()
{
   return type;
}


/**
 * @brief   Get the string representation of this data object.
 */
char* sidexDateTime::toString()
{
  return value;
}


/**
 * @brief   Set the DateTime value of this object.
 */
int sidexDateTime::setValue(char* newValue)
{
  int iRet = SIDEX_SUCCESS;
  if (sidexDateCheck(newValue)){
#if defined(LINUX) || defined (MINGW_BUILD)
    strcpy (value, newValue);
#else
    strcpy_s (value, DATE_TIME_STR_LENGTH, newValue);
#endif

    if (NULL != xValue){
      delete xValue;
      xValue = NULL;
    }
    if (NULL != wValue){
      delete wValue;
      wValue = NULL;
    }
  }
  else{
   iRet = SIDEX_ERR_INVALID_DATETIME;
  }
  return iRet;
}


/**
 * @brief   Get the DateTime value of this object.
 */
char* sidexDateTime::getValue_A()
{
   return value;
}


/**
 * @brief   Get the char string value of this object.
 */
wchar_t* sidexDateTime::getValue_X()
{
  if (NULL == xValue){
    SIDEX_INT32 iLengthUtf16;
    if (NULL != value){
      xValue = UTF8toUTF32(value, &iLengthUtf16);
    }
  }
  return xValue;
}


/**
 * @brief   Get the char string value of this object.
 */
char16_t* sidexDateTime::getValue_W()
{
  if (NULL == wValue){
    SIDEX_INT32 iLengthUtf16;
    if (NULL != value){
      wValue = (char16_t*)UTF8toUTF16(value, &iLengthUtf16);
    }
  }
  return wValue;
}

