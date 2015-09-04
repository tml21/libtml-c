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

#include <string.h>
#include "sidex.h"
#include "sidexUtils.h"
#include "sidexTypes.h"


/**
 * @brief   encodes a string using base64.
 */
char* sidexutil_encodeBase64(const char *rawtext, int* iSize)
{
   static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   int i = 0;

   int size = *iSize;
   if (*iSize == -1)
      size = (int)strlen(rawtext);

   int blocks = size/3;

   int sizeob = (3*blocks + 3) * 4 / 3;
   char *outbuf = new char[sizeob+1];

   if (NULL == outbuf)
      return NULL;

   int outpidx = 0;

   for (i=0; i<blocks; i++)
   {
      unsigned char in0 = rawtext[i*3];
      unsigned char in1 = rawtext[i*3+1];
      unsigned char in2 = rawtext[i*3+2];

      outbuf[outpidx++] = cb64[ in0 >> 2 ];
      outbuf[outpidx++] = cb64[ ((in0 & 0x03) << 4) | ((in1 & 0xf0) >> 4) ];
      outbuf[outpidx++] = cb64[ ((in1 & 0x0f) << 2) | ((in2 & 0xc0) >> 6) ];
      outbuf[outpidx++] = cb64[ in2 & 0x3f ];
   }   

   if (size % 3 == 2)
   {
      outbuf[outpidx++] = cb64[ ((unsigned char*)rawtext)[size-2] >> 2 ];
      outbuf[outpidx++] = cb64[ ((((unsigned char*)rawtext)[size-2] & 0x03) << 4) | ((((unsigned char*)rawtext)[size-1] & 0xf0) >> 4) ];
      outbuf[outpidx++] = cb64[ ((((unsigned char*)rawtext)[size-1] & 0x0f) << 2) | 0 ];
      outbuf[outpidx++] = '=';
   }

   if (size % 3 == 1)
   {
      outbuf[outpidx++] = cb64[ ((unsigned char*)rawtext)[size-1] >> 2 ];
      outbuf[outpidx++] = cb64[ ((((unsigned char*)rawtext)[size-1] & 0x03) << 4) | 0];
      outbuf[outpidx++] = '=';
      outbuf[outpidx++] = '=';
   }

   outbuf[outpidx] = 0;
   *iSize = outpidx;
   return outbuf;
}


/**
 * @brief   Decodes a base64 helper
 */
char ceCalc(char c)
{
   static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

  c = (char) ((c < 43 || c > 122) ? 0 : cd64[ c - 43 ]);
  if(c) 
    c = (char) ((c == '$') ? 0 : c - 61);
  return (char) (c - 1);
}

/**
 * @brief   Decodes a base64 encoded string.
 */
char* sidexutil_decodeBase64(const char *encodedtext, SIDEX_INT32* iSize)
{
   int i = 0;

   SIDEX_INT32 size;
   if (*iSize == -1)
      size = (SIDEX_INT32)strlen(encodedtext);
   else
     size = *iSize;

   int sizeout = size * 3 / 4;
   char *outbuf = new char[sizeout+1];

   if (NULL == outbuf)
      return NULL;

   int outidx = 0;
   for( i = 0; i < size; i = i+4)
   {  
      char in0, in1, in2, in3;

      in0 = ceCalc(encodedtext[i]);
      in1 = ceCalc(encodedtext[i+1]);
      in2 = ceCalc(encodedtext[i+2]);
      in3 = ceCalc(encodedtext[i+3]);

      outbuf[outidx++] = (in0 << 2 | in1 >> 4);
      outbuf[outidx++] = (in1 << 4 | in2 >> 2);
      outbuf[outidx++] = (((in2 << 6) & 0xc0) | in3);
   }
  
   // I have to compute the right length:
   if (encodedtext[--size] == '='){
     outidx--;
     if (encodedtext[--size] == '='){
       outidx--;
     }
   }
   outbuf[outidx] = '\0';
   *iSize = outidx;
   return outbuf;
}


/**
 * @brief   Decodes a base64 encoded string.
 */
 SIDEX_INT32 sidexutil_decodeBase64Length(const char *encodedtext)
{
   SIDEX_INT32 size;
   size = (SIDEX_INT32)strlen(encodedtext);

   SIDEX_INT32 outidx = size * 3 / 4;
   // I have to compute the right length:
   if (encodedtext[--size] == '='){
     outidx--;
     if (encodedtext[--size] == '='){
       outidx--;
     }
   }
   return outidx;
}

 
 /**
 * @brief   Get SIDEX_DATA_TYPE by string
 */
SIDEX_DATA_TYPE sidexutil_strToTypeEnum(const char* c_type)
{
   SIDEX_DATA_TYPE type;
   if      ( strcmp(c_type,SIDEX_TYPE_ATTR_NONE) == 0 ) type = SIDEX_DATA_TYPE_NONE;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_BOOLEAN)   == 0 ) type = SIDEX_DATA_TYPE_BOOLEAN;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_INTEGER)   == 0 ) type = SIDEX_DATA_TYPE_INTEGER;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_FLOAT)   == 0 ) type = SIDEX_DATA_TYPE_FLOAT;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_DATETIME)   == 0 ) type = SIDEX_DATA_TYPE_DATETIME;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_STRING)  == 0 ) type = SIDEX_DATA_TYPE_STRING;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_BINARY)  == 0 ) type = SIDEX_DATA_TYPE_BINARY;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_DICT)    == 0 ) type = SIDEX_DATA_TYPE_DICT;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_TABLE)   == 0 ) type = SIDEX_DATA_TYPE_TABLE;
   else if ( strcmp(c_type,SIDEX_TYPE_ATTR_LIST)    == 0 ) type = SIDEX_DATA_TYPE_LIST;
   else type = SIDEX_DATA_TYPE_UNKNOWN;
   return type;
}


/**
 * @brief   Convert SIDEX_DATA_TYPE to string
 */
const char* sidexutil_TypeToString(SIDEX_DATA_TYPE type)
{
   switch (type)
   {
      case SIDEX_DATA_TYPE_NONE:
         return SIDEX_TYPE_ATTR_NONE;
        break;
      case SIDEX_DATA_TYPE_BOOLEAN:
         return SIDEX_TYPE_ATTR_BOOLEAN;
         break;
      case SIDEX_DATA_TYPE_INTEGER:
         return SIDEX_TYPE_ATTR_INTEGER;
         break;
      case SIDEX_DATA_TYPE_FLOAT:
         return SIDEX_TYPE_ATTR_FLOAT;
         break;
      case SIDEX_DATA_TYPE_DATETIME:
         return SIDEX_TYPE_ATTR_DATETIME;
         break;
      case SIDEX_DATA_TYPE_STRING:
         return SIDEX_TYPE_ATTR_STRING;
         break;
      case SIDEX_DATA_TYPE_BINARY:
         return SIDEX_TYPE_ATTR_BINARY;
         break;
      case SIDEX_DATA_TYPE_LIST:
         return SIDEX_TYPE_ATTR_LIST;
         break;
      case SIDEX_DATA_TYPE_DICT:
         return SIDEX_TYPE_ATTR_DICT;
         break;
      case SIDEX_DATA_TYPE_TABLE:
         return SIDEX_TYPE_ATTR_TABLE;
         break;
      default:
         return SIDEX_TYPE_ATTR_UNKNOWN;
         break;
   }
}


/**
 * @brief   Convert SIDEX_ENCODE data to string
 */
const char* sidexutil_sidexEncodingToString(SIDEX_ENCODE encoding)
{
   switch (encoding)
   {
      case SIDEX_ENCODE_NONE:
         return SIDEX_ENC_ATTR_NONE;
         break;
      case SIDEX_ENCODE_CDATA:
         return SIDEX_ENC_ATTR_CDATA;
         break;
      case SIDEX_ENCODE_BASE64:
         return SIDEX_ENC_ATTR_BASE64;
         break;
      default:
         return "Error: unknown encoding";
         break;
   }
}


 /**
 * @brief   Converts SIDEX_DATA_TYPE to string
 */
const char* sidexutil_StringFormatToString(SIDEX_STRING_ENCODING_TYPE type){
   switch (type)
   {
      case SIDEX_STRING_FORMAT_UNKNOWN:
         return SIDEX_STRING_FORMAT_ATTR_UNKNOWN_A;
         break;
      case SIDEX_STRING_FORMAT_SIDEX:
         return SIDEX_STRING_FORMAT_ATTR_SIDEX_A;
         break;
      case SIDEX_STRING_FORMAT_XML:
         return SIDEX_STRING_FORMAT_ATTR_XML_A;
         break;
      case SIDEX_STRING_FORMAT_JSON:
         return SIDEX_STRING_FORMAT_ATTR_JSON_A;
         break;
      case SIDEX_STRING_FORMAT_CVS:
         return SIDEX_STRING_FORMAT_ATTR_CVS_A;
         break;
      case SIDEX_STRING_FORMAT_INI:
         return SIDEX_STRING_FORMAT_ATTR_INI_A;
         break;
      default:
         return NULL;
         break;
   }
}


/**
 * @brief   Converts a string to a SIDEX_STRING_ENCODING_TYPE.
 */
const SIDEX_STRING_ENCODING_TYPE sidexutil_StringToStringFormat(const char* string){
  SIDEX_STRING_ENCODING_TYPE ret = SIDEX_CONV_NOT_FOUND;

  ////////////////////////////////////////////
  // Check for list plausibility:
  if (0 == strcmp(SIDEX_STRING_FORMAT_ATTR_UNKNOWN_A, string)){
    ret = SIDEX_STRING_FORMAT_UNKNOWN;
  }
  else{
    if (0 == strcmp(SIDEX_STRING_FORMAT_ATTR_SIDEX_A, string)){
      ret = SIDEX_STRING_FORMAT_SIDEX;
    }
    else{
      if (0 == strcmp(SIDEX_STRING_FORMAT_ATTR_XML_A, string)){
        ret = SIDEX_STRING_FORMAT_XML;
      }
      else{
        if (0 == strcmp(SIDEX_STRING_FORMAT_ATTR_JSON_A, string)){
          ret = SIDEX_STRING_FORMAT_JSON;
        }
        else{
          if (0 == strcmp(SIDEX_STRING_FORMAT_ATTR_CVS_A, string)){
            ret = SIDEX_STRING_FORMAT_CVS;
          }
          else{
            if (0 == strcmp(SIDEX_STRING_FORMAT_ATTR_INI_A, string)){
              ret = SIDEX_STRING_FORMAT_INI;
            }
          }
        }
      }
    }
  }
  return ret;
}


/**
 * @brief   Decrement the reference counter of the SIDEX_VARIANT object.
 */
void sidexutil_Variant_DecRef(SIDEX_VARIANT variant) 
{
  if (variant != SIDEX_HANDLE_TYPE_NULL){
    switch ( ((sidexBase*) variant)->getType())
    {
      case SIDEX_DATA_TYPE_NONE:
        ((sidexNone*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexNone*)variant);
        break;
      case SIDEX_DATA_TYPE_BOOLEAN:
        ((sidexBoolean*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexBoolean*)variant);
        break;
      case SIDEX_DATA_TYPE_INTEGER:
        ((sidexInteger*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexInteger*)variant);
        break;
      case SIDEX_DATA_TYPE_FLOAT:
        ((sidexFloat*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexFloat*)variant);
        break;
      case SIDEX_DATA_TYPE_DATETIME:
        ((sidexDateTime*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexDateTime*)variant);
        break;
      case SIDEX_DATA_TYPE_BINARY:
      case SIDEX_DATA_TYPE_STRING:
        ((sidexString*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexString*)variant);
         break;
      case SIDEX_DATA_TYPE_LIST:
        ((sidexList*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexList*)variant);
        break;
      case SIDEX_DATA_TYPE_DICT:
        ((sidexDict*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexDict*)variant);
        break;
      case SIDEX_DATA_TYPE_TABLE:
        ((sidexTable*)variant)->cleanUp();
        // if refCounter == 0 I have to delete the object:
        if (((sidexBase*) variant)->getRef() == 0)
          delete ((sidexTable*)variant);
        break;
      case SIDEX_DATA_TYPE_UNKNOWN:
      default:
        break;
    }
  }
}


/**
 * @brief   Increment the reference counter of the SIDEX_VARIANT object.<br>
 */
void sidexutil_Variant_IncRef(SIDEX_VARIANT variant) 
{
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    ((sidexBase*)variant)->incRef();
  }
}


/**
 * @brief   Get row of type SIDEX_VARIANT out of a sidexTable.
 */
int sidexutil_TableGetRow(SIDEX_VARIANT sTable, int rowIndex, SIDEX_VARIANT* variant)
{
  int iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type;
  type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    sidexDict* dict;
    iRet = ((sidexTable*)sTable)->getRow(&dict, rowIndex);
    if (SIDEX_SUCCESS == iRet)
      *variant = (SIDEX_VARIANT)dict;
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}

/**
 * @brief   Remove the row referring the rowIndex out of the table.
 */
int sidexutil_TableDeleteRow(SIDEX_VARIANT sTable, int rowIndex){
  int iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type;
  type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    SIDEX_VARIANT variant;
    iRet = sidexutil_TableGetRow(sTable, rowIndex, &variant);
    if (SIDEX_SUCCESS == iRet){
      iRet = ((sidexTable*)sTable)->deleteRow(variant);
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }

  return iRet;
}


/**
 * @brief   Remove all rows out of the table.
 */
int sidexutilTable_DeleteRows(SIDEX_VARIANT sTable){
  int iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type;
  type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    iRet = ((sidexTable*)sTable)->deleteRows();
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Compare a variants type with an expected type.
 */
SIDEX_BOOL sidexutil_dataTypeCheck(SIDEX_VARIANT variant, SIDEX_DATA_TYPE expectedType){
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    try{
      SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
      if (type == expectedType)
        return SIDEX_TRUE;
      else
        return SIDEX_FALSE;
    }
    catch (...){
      return SIDEX_FALSE;
    }
  }
  else{
    return SIDEX_FALSE;
  }
}


/**
 * @brief   Compare a string variant encoding with an expected encoding.
 */
SIDEX_BOOL sidexutil_stringDataEncodeCheck(SIDEX_VARIANT variant, SIDEX_ENCODE expectedEncode){
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    try{
      SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
      if (type == SIDEX_DATA_TYPE_STRING || type == SIDEX_DATA_TYPE_BINARY){
        SIDEX_ENCODE encoding = ((sidexString*)(variant))->getEncoding();
        if (encoding == expectedEncode){
          return SIDEX_TRUE;
        }
        else{
          return SIDEX_FALSE;
        }
      }
      else{
        return SIDEX_FALSE;
      }
    }
    catch (...){
      return SIDEX_FALSE;
    }
  }
  else{
    return SIDEX_FALSE;
  }
}


/**
 * @brief   Returns the type of a SIDEX_VARIANT value.
 */
SIDEX_DATA_TYPE sidexutil_getDataType(SIDEX_VARIANT value){
  SIDEX_DATA_TYPE retType = SIDEX_DATA_TYPE_UNKNOWN;
  if (SIDEX_HANDLE_TYPE_NULL != value){
    try{
      retType = ((sidexBase*)(value))->getType();
    }
    catch (...){
      retType = SIDEX_DATA_TYPE_UNKNOWN;
    }
  }
  else{
    retType = SIDEX_DATA_TYPE_UNKNOWN;
  }
  return retType;
}


/**
 * @brief   Returns a copy of a SIDEX_VARIANT.
 *
 * @param   value SIDEX_VARIANT to copy.
 *
 * @returns  a copy of a SIDEX_VARIANT.
 */
SIDEX_VARIANT sidexutil_Variant_Copy (SIDEX_VARIANT value){
  SIDEX_VARIANT vCopy = SIDEX_HANDLE_TYPE_NULL;
  if (SIDEX_HANDLE_TYPE_NULL != value){
    int iRet = SIDEX_SUCCESS;
    SIDEX_DATA_TYPE type = sidexutil_getDataType(value);
     switch (type)
     {
        case SIDEX_DATA_TYPE_NONE:
           vCopy = (SIDEX_VARIANT)new sidexNone();
           break;
        case SIDEX_DATA_TYPE_BOOLEAN:
           SIDEX_BOOL bValue;
           iRet = sidex_Variant_As_Boolean(value, &bValue);
           if (SIDEX_SUCCESS == iRet){
             vCopy = (SIDEX_VARIANT)new sidexBoolean((SIDEX_FALSE == bValue)? false : true);
           }
           break;
        case SIDEX_DATA_TYPE_INTEGER:
           SIDEX_INT64 iValue;
           iRet = sidex_Variant_As_Integer (value, &iValue);
           if (SIDEX_SUCCESS == iRet){
             vCopy = (SIDEX_VARIANT)new sidexInteger(iValue);
           }
           break;
        case SIDEX_DATA_TYPE_FLOAT:
           SIDEX_DOUBLE fValue;
           iRet = sidex_Variant_As_Float (value, &fValue);
           if (SIDEX_SUCCESS == iRet){
             vCopy = (SIDEX_VARIANT)new sidexFloat(fValue);
           }
           break;
        case SIDEX_DATA_TYPE_DATETIME:
           char* dtValue;
           iRet = sidex_Variant_As_DateTime (value, &dtValue);
           if (SIDEX_SUCCESS == iRet){
             vCopy = (SIDEX_VARIANT)new sidexDateTime(dtValue);
           }
           break;
        case SIDEX_DATA_TYPE_BINARY:
        case SIDEX_DATA_TYPE_STRING:
           SIDEX_ENCODE encoding;
           iRet = sidexutilString_Encoding(value, &encoding);
           if (SIDEX_SUCCESS == iRet){
            if (SIDEX_ENCODE_BASE64 == encoding){
              unsigned char* sValue;
              SIDEX_INT32 iLength;
              iRet = sidex_Variant_As_Binary (value, &sValue, &iLength);
              if (SIDEX_SUCCESS == iRet){
                vCopy = (SIDEX_VARIANT)new sidexString((char*)sValue, iLength, encoding);
              }
            }
            else{
              char* sValue;
              const char* sFormat;
              SIDEX_INT32 iLength;
              iRet = sidex_Variant_As_String (value, &sValue, &iLength);
              if (SIDEX_SUCCESS == iRet){
                 iRet = sidex_Variant_String_GetFormat(value, &sFormat);
                 if (SIDEX_SUCCESS == iRet){
                   vCopy = (SIDEX_VARIANT)new sidexString(sValue, iLength, encoding);
                   iRet = sidex_Variant_String_SetFormat(vCopy, sFormat);
                 }
              }
            }
           }
           break;
        case SIDEX_DATA_TYPE_LIST:
           {
             vCopy = (SIDEX_VARIANT)new sidexList();
             SIDEX_INT32 iSize = 0;
             ///////////////////////////////////////
             // get the list size:
             iRet = sidex_Variant_List_Size (value, &iSize);
             if (SIDEX_SUCCESS == iRet){
               ///////////////////////////////////////
               // loop over all list items:
               for (int i = 0; SIDEX_SUCCESS == iRet && i < iSize; ++i){
                 SIDEX_VARIANT sourceListItem;
                 ///////////////////////////////////////
                 // Get the item:
                 iRet = sidex_Variant_List_Get(value, i, &sourceListItem);
                 if (SIDEX_SUCCESS == iRet){
                   SIDEX_VARIANT copyListItem;
                   ///////////////////////////////////////
                   // Copy of the item:
                   copyListItem = sidexutil_Variant_Copy(sourceListItem);
                   if (SIDEX_HANDLE_TYPE_NULL != copyListItem){
                     /////////////////////////////////////////
                     // Add copy of the item to the list copy:
                     SIDEX_INT32 pos;
                     iRet = sidex_Variant_List_Append(vCopy, copyListItem, &pos);
                     sidex_Variant_DecRef(copyListItem);
                   }
                   else{
                     iRet = SIDEX_ERR_COMMON; // Necessary to set the return value to SIDEX_HANDLE_TYPE_NULL
                   }
                 }
               }
               if (SIDEX_SUCCESS != iRet){
                 sidex_Variant_DecRef(vCopy);
                 vCopy = SIDEX_HANDLE_TYPE_NULL;
               }
             }
           }
           break;
        case SIDEX_DATA_TYPE_DICT:
           {
             vCopy = (SIDEX_VARIANT)new sidexDict();
             SIDEX_INT32 iSize = 0;
             ///////////////////////////////////////
             // get the list size:
             iRet = sidex_Variant_Dict_Size (value, &iSize);
             if (SIDEX_SUCCESS == iRet){
               SIDEX_VARIANT vKeys;
               ///////////////////////////////////////
               // get the dict keys:
               iRet = sidex_Variant_Dict_Keys(value, &vKeys);
               if (SIDEX_SUCCESS == iRet){
                 ///////////////////////////////////////
                 // loop over all list Keys:
                 for (int i = 0; SIDEX_SUCCESS == iRet && i < iSize; ++i){
                   SIDEX_VARIANT vKey;
                   ///////////////////////////////////////
                   //The key item:
                   iRet = sidex_Variant_List_Get(vKeys, i, &vKey);
                   if (SIDEX_SUCCESS == iRet){
                     char* sKey;
                     SIDEX_INT32 iLength;
                     ///////////////////////////////////////
                     //The key item as string:
                     iRet = sidex_Variant_As_String(vKey, &sKey, &iLength);
                     if (SIDEX_SUCCESS == iRet){
                       SIDEX_VARIANT sourceDictItem;
                       ///////////////////////////////////////
                       // get the dict entry:
                       iRet = sidex_Variant_Dict_Get(value, sKey, &sourceDictItem);
                       if (SIDEX_SUCCESS == iRet){
                         SIDEX_VARIANT copyDictItem;
                         ///////////////////////////////////////
                         // copy of the dict entry:
                         copyDictItem = sidexutil_Variant_Copy(sourceDictItem);
                         if (SIDEX_HANDLE_TYPE_NULL != copyDictItem){
                           ///////////////////////////////////////////////////////////
                           // set the copy of the dict entry int the copy of the dict:
                           iRet = sidex_Variant_Dict_Set(vCopy, sKey, copyDictItem);
                           sidex_Variant_DecRef(copyDictItem);
                         }
                         else{
                           iRet = SIDEX_ERR_COMMON; // Necessary to set the return value to SIDEX_HANDLE_TYPE_NULL
                         }
                       }
                     }
                   }
                 }
                 sidex_Variant_DecRef(vKeys);
               }
               if (SIDEX_ERR_NOCONTENT == iRet){
                 ////////////////////////////
                 // An empty dict is allowed:
                 iRet = SIDEX_SUCCESS;
               }
               if (SIDEX_SUCCESS != iRet){
                 sidex_Variant_DecRef(vCopy);
                 vCopy = SIDEX_HANDLE_TYPE_NULL;
               }
             }
           }
           break;
        case SIDEX_DATA_TYPE_TABLE:
           {
             vCopy = (SIDEX_VARIANT)new sidexTable();
             SIDEX_INT32 iColumns = 0;
             ////////////////////////////
             // Get columns:
             iRet = sidex_Variant_Table_Columns (value, &iColumns);
             if (SIDEX_SUCCESS == iRet){
               SIDEX_INT32 iRows;
               ////////////////////////////
               // Get rows:
               iRet = sidex_Variant_Table_Rows (value, &iRows);
               if (SIDEX_SUCCESS == iRet){
                 ///////////////////////////////////////
                 // loop over all columns:
                 for (int i = 0; SIDEX_SUCCESS == iRet && i < iColumns; ++i){
                   char* sColumn;
                   ///////////////////////////////////////
                   // Get the column name:
                   iRet = sidex_Variant_Table_GetColumnName(value, i, &sColumn);
                   if (SIDEX_SUCCESS == iRet){
                     ////////////////////////////////////////////
                     // Add column name to the copy of the table:
                     iRet = sidex_Variant_Table_AddColumn (vCopy, sColumn);
                     ///////////////////////////////////////
                     // loop over all rows
                     for (int j = 0; SIDEX_SUCCESS == iRet && j < iRows; ++j){
                       if (0 == i){
                         ////////////////////////////////////////////////
                         // Add row on index 0 to the copy of the table:
                         SIDEX_INT32 iRowIdx;
                         iRet = sidex_Variant_Table_AddRow (vCopy, &iRowIdx);
                       }
                       if (SIDEX_SUCCESS == iRet){
                         SIDEX_VARIANT vField;
                         ////////////////////////////////////////////
                         // Get field of the table:
                         iRet = sidex_Variant_Table_GetField (value, j, sColumn, &vField);
                         if (SIDEX_SUCCESS == iRet){
                           SIDEX_VARIANT vFieldCopy;
                           ////////////////////////////////////////////
                           // make a copy of the field of the table:
                           vFieldCopy = sidexutil_Variant_Copy(vField);
                           if (SIDEX_HANDLE_TYPE_NULL != vFieldCopy){
                             //////////////////////////////////////////////////////
                             // set the copy of the filed to the copy of the table:
                             iRet = sidex_Variant_Table_SetField (vCopy, j, sColumn, vFieldCopy);
                             sidex_Variant_DecRef(vFieldCopy);
                           }
                           else{
                             iRet = SIDEX_ERR_COMMON; // Necessary to set the return value to SIDEX_HANDLE_TYPE_NULL
                           }
                         }
                       }
                     }
                   }
                 }
               }
             }
             if (SIDEX_SUCCESS != iRet){
               sidex_Variant_DecRef(vCopy);
               vCopy = SIDEX_HANDLE_TYPE_NULL;
             }
           }
           break;
        default:
           break;
     }
  }
  return vCopy;
}


/**
 * @brief   Return the number of items in the list.
 */
int sidexutilList_Size(SIDEX_VARIANT variant, SIDEX_INT32* iSize){
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_LIST == type){
      *iSize = ((sidexList*)variant)->length();
      return SIDEX_SUCCESS;
    }
    else
      return SIDEX_ERR_WRONG_TYPE;
  }
  else
    return SIDEX_ERR_NOCONTENT;
}


/**
 * @brief   Return the number of items in the dictionary.
 */
int sidexutilDict_Size(SIDEX_VARIANT variant, SIDEX_INT32* iSize){
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_DICT == type){
      *iSize = ((sidexDict*)variant)->getNumberOfDictEntrys();
      return SIDEX_SUCCESS;
    }
    else
      return SIDEX_ERR_WRONG_TYPE;
  }
  else
    return SIDEX_ERR_NOCONTENT;
}


/**
 * @brief   Return all the keys from the dictionary.
 */
int sidexutilDict_Keys(SIDEX_VARIANT variant, SIDEX_VARIANT* varKeys){
  int iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_DICT == type){
      int iSize = ((sidexDict*)variant)->getNumberOfDictEntrys();
      sidexList* list = new sidexList();
      if (0 < iSize){
        char** sKeys;
        iRet = ((sidexDict*)variant)->getKeys(&sKeys);
        if(SIDEX_SUCCESS == iRet){
          for (int i = 0;i < iSize; ++i){
            SIDEX_VARIANT keyItem = (SIDEX_VARIANT)new sidexString(sKeys[i], (int)strlen(sKeys[i]), SIDEX_ENCODE_NONE);
            list->addElement(keyItem);
            // Decrement of the refcounter:
            ((sidexBase*)keyItem)->decRef();
          }
          delete []sKeys;
        }
        else{
          iRet = SIDEX_ERR_NOCONTENT;
        }
      }
      if (SIDEX_SUCCESS == iRet){
        *varKeys = (SIDEX_VARIANT)list;
      }
      else{
        sidexutil_Variant_DecRef((SIDEX_VARIANT)list);
      }
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  else{
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief   Resets pointer to the beginning of the dictionary. Use with sidex_Variant_Dict_Next(). 
 */
int sidexutilDict_First(SIDEX_VARIANT variant){
  int iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_DICT == type){
      ((sidexDict*)variant)->first();
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  else{
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexutilDict_Next_A(SIDEX_VARIANT variant, char** sKey, SIDEX_VARIANT* nextValue){
  int iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_DICT == type){
      iRet = ((sidexDict*)variant)->next_A (sKey, nextValue);
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  else{
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexutilDict_Next_X(SIDEX_VARIANT variant, wchar_t** sKey, SIDEX_VARIANT* nextValue){
  int iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_DICT == type){
      iRet = ((sidexDict*)variant)->next_X (sKey, nextValue);
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  else{
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief   Returns the next referenced Variant and moves pointer to the next entry of the dictionary.
 */
int sidexutilDict_Next_W(SIDEX_VARIANT variant, char16_t** sKey, SIDEX_VARIANT* nextValue){
  int iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_DICT == type){
      iRet = ((sidexDict*)variant)->next_W (sKey, nextValue);
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  else{
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief   Check for the existence of the sKey in the dict.
 */
int sidexutilDict_HasKey(SIDEX_VARIANT variant, char* sKey, SIDEX_BOOL* bRet){
  int iRet = SIDEX_SUCCESS;
  if (NULL != sKey && strlen(sKey) > 0){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_DICT == type){
      char**sNames;
      iRet = ((sidexDict*)variant)->getKeys(&sNames);
      switch (iRet){
        case SIDEX_SUCCESS:
          {
            int iColumns = ((sidexDict*)variant)->getNumberOfDictEntrys();
            bool bFound = false;
            for (int i = 0; i < iColumns && !bFound;++i){
              if (0 == strcmp(sNames[i], sKey)){
                bFound = true;
              }
            }
            if (bFound){
              *bRet = SIDEX_TRUE;
            }
            else{
              *bRet = SIDEX_FALSE;
            }
            delete[] sNames;
          }
          break;
        case SIDEX_ERR_NOCONTENT:
          *bRet = SIDEX_FALSE;
          iRet = SIDEX_SUCCESS;
          break;
        default:
          break;
      }
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}


/**
 * @brief   Return the number of rows of the table.
 */
int sidexutilTable_Rows(SIDEX_VARIANT variant, SIDEX_INT32* iRows){
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_TABLE == type){
      *iRows = ((sidexTable*)variant)->countRows();
      return SIDEX_SUCCESS;
    }
    else
      return SIDEX_ERR_WRONG_TYPE;
  }
  else
    return SIDEX_ERR_NOCONTENT;
}


/**
 * @brief   Return the number of columns of the table.
 */
int sidexutilTable_Columns(SIDEX_VARIANT variant, SIDEX_INT32* iColumns){
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_TABLE == type){
      *iColumns = ((sidexTable*)variant)->countColumns();
      return SIDEX_SUCCESS;
    }
    else
      return SIDEX_ERR_WRONG_TYPE;
  }
  else
    return SIDEX_ERR_NOCONTENT;
}


/**
 * @brief   Return all the column names from the table.
 */
int sidexutilTable_ColumnNames(SIDEX_VARIANT variant, SIDEX_VARIANT* varNames){
  int iRet = SIDEX_SUCCESS;
  if (SIDEX_HANDLE_TYPE_NULL != variant){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_TABLE == type){
      char** sNames;
      int iRet = ((sidexTable*)variant)->getColumnNames(&sNames);
      if(SIDEX_SUCCESS == iRet){
        int iColumns = ((sidexTable*)variant)->countColumns();
        sidexList* list = new sidexList();
        if (0 < iColumns){
          for (int i = 0;i < iColumns; ++i){
            SIDEX_VARIANT colItem = (SIDEX_VARIANT)new sidexString(sNames[i], (int)strlen(sNames[i]), SIDEX_ENCODE_NONE);
            list->addElement(colItem);
            // Decrement of the refcounter:
            ((sidexBase*)colItem)->decRef();
          }
        }
        *varNames = (SIDEX_VARIANT)list;
        delete[] sNames;
      }
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  else{
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief   Check for the existence of the sColumnName in the table.
 */
int sidexutil_TableHasColumn(SIDEX_VARIANT variant, char* sColumnName, SIDEX_BOOL* bRet){
  int iRet = SIDEX_SUCCESS;
  if (NULL != sColumnName && strlen(sColumnName) > 0){
    SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
    if (SIDEX_DATA_TYPE_TABLE == type){
      char**sNames;
      iRet = ((sidexTable*)variant)->getColumnNames(&sNames);
      if (SIDEX_SUCCESS == iRet){
        int iColumns = ((sidexTable*)variant)->countColumns();
        bool bFound = false;
        for (int i = 0; i < iColumns && !bFound;++i){
          if (0 == strcmp(sNames[i], sColumnName)){
            bFound = true;
          }
        }
        if (bFound){
          *bRet = SIDEX_TRUE;
        }
        else{
          *bRet = SIDEX_FALSE;
        }
        delete[] sNames;
      }
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  return iRet;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as an boolean value. Returns the SIDEX_BOOL value.
 */
int sidexutilBoolean_Value(SIDEX_VARIANT variant, SIDEX_BOOL* bValue){
  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_BOOLEAN){
    bool bRet = ((sidexBoolean*)variant)->getValue();
    if (bRet){
      *bValue = SIDEX_TRUE;
    }
    else{
      *bValue = SIDEX_FALSE;
    }
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as an integer value. Returns the int value.
 */
int sidexutilInteger_Value(SIDEX_VARIANT variant, SIDEX_INT64* intValue){
  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_INTEGER){
    *intValue = ((sidexInteger*)variant)->getValue();
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a float value. Returns the float value.
 */
int sidexutilFloat_Value(SIDEX_VARIANT variant, SIDEX_DOUBLE* fValue){
  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_FLOAT){
    *fValue = ((sidexFloat*)variant)->getValue();
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a DateTime value. Returns the DateTime value.
 */
int sidexutilDateTime_Value_A(SIDEX_VARIANT variant, char** dtValue){
  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_DATETIME){
    char* retValue = ((sidexDateTime*)variant)->getValue_A();
    *dtValue = retValue;
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a DateTime value. Returns the DateTime value.
 */
int sidexutilDateTime_Value_X(SIDEX_VARIANT variant, wchar_t** dtValue){
  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_DATETIME){
    wchar_t* retValue = ((sidexDateTime*)variant)->getValue_X();
    *dtValue = retValue;
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a DateTime value. Returns the DateTime value.
 */
int sidexutilDateTime_Value_W(SIDEX_VARIANT variant, char16_t** dtValue){
  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_DATETIME){
    char16_t* retValue = ((sidexDateTime*)variant)->getValue_W();
    *dtValue = retValue;
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the char array value.
 */
int sidexutilString_Value_A(SIDEX_VARIANT variant, char** sValue, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_STRING){
    char* retValue = ((sidexString*)variant)->getValue_A();
    *sValue = retValue;
    *iLength = ((sidexString*)variant)->getSize();
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the char array value.
 */
int sidexutilString_Value_X(SIDEX_VARIANT variant, wchar_t** sValue, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_STRING || type == SIDEX_DATA_TYPE_BINARY){
    *sValue = ((sidexString*)variant)->getValue_X(iLength);
  }
  else{
    return SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the char array value.
 */
int sidexutilString_Value_W(SIDEX_VARIANT variant, char16_t** sValue, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_STRING || type == SIDEX_DATA_TYPE_BINARY){
    *sValue = ((sidexString*)variant)->getValue_W(iLength);
  }
  else{
    return SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the length of the char array value.
 */
int sidexutilString_Value_Length_X(SIDEX_VARIANT variant, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_STRING || type == SIDEX_DATA_TYPE_BINARY){
    *iLength = ((sidexString*)variant)->getLength_X();
  }
  else{
    return SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the length of the char array value.
 */
int sidexutilString_Value_Length_W(SIDEX_VARIANT variant, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_STRING || type == SIDEX_DATA_TYPE_BINARY){
    *iLength = ((sidexString*)variant)->getLength_W();
  }
  else{
    return SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns the length of the char array value.
 */
int sidexutilString_Value_Length_A(SIDEX_VARIANT variant, SIDEX_INT32* iLength){
  SIDEX_INT32 iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_STRING || type == SIDEX_DATA_TYPE_BINARY){
    *iLength = ((sidexString*)variant)->getSize();
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a binary char array value. Returns the binary char array value.
 */
int sidexutilBinaryString_Value(SIDEX_VARIANT variant, char** sValue, SIDEX_INT32* iLength){
  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_BINARY){

    char* retValue = ((sidexString*)variant)->getValue_A();
    *sValue = retValue;
    *iLength = ((sidexString*)variant)->getSize();
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Interpreting a SIDEX_VARIANT as a char array value. Returns it's encoding.
 */
int sidexutilString_Encoding(SIDEX_VARIANT variant, SIDEX_ENCODE* encoding){
  SIDEX_DATA_TYPE type = ((sidexBase*)(variant))->getType();
  if (type == SIDEX_DATA_TYPE_STRING || type == SIDEX_DATA_TYPE_BINARY){
    *encoding = ((sidexString*)variant)->getEncoding();
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}

/**
 * @brief   Add an element of type SIDEX_VARIANT to a sidexList. (Subkeys will be created)
 */
int sidexutilList_Add(SIDEX_VARIANT sList, SIDEX_VARIANT value, SIDEX_INT32* iPos) 
{
  SIDEX_DATA_TYPE type = ((sidexBase*)(sList))->getType();
  if (SIDEX_DATA_TYPE_LIST == type){
    ((sidexList*)sList)->addElement(value);

    SIDEX_INT32 iSize = 0;
    sidexutilList_Size(sList, &iSize);
    *iPos = --iSize;
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Inserts an element of type SIDEX_VARIANT to a sidexList. (Subkeys will be created)
 */
int sidexutilList_Insert(SIDEX_VARIANT sList, SIDEX_VARIANT value, int pos) 
{
  SIDEX_DATA_TYPE type = ((sidexBase*)(sList))->getType();
  if (SIDEX_DATA_TYPE_LIST == type){
    ((sidexList*)sList)->addElement(value, pos);
    return SIDEX_SUCCESS;
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Overwrite an element of type SIDEX_VARIANT in a sidexList.
 */
int sidexutilList_Set(SIDEX_VARIANT sList, SIDEX_VARIANT value, int pos) 
{
  //////////////////////////////////////////////////////////////////////////////
  // First delete:
  int iRet = sidexutilList_DeleteItem(sList, pos);
  if (SIDEX_SUCCESS == iRet){
    //////////////////////////////////////////////////////////////////////////////
    // Set afterwards:
    iRet = sidexutilList_Insert (sList, value, pos);
  }
  return iRet;
}


/**
 * @brief   Remove the object out of the list at the given position.
 */
int sidexutilList_DeleteItem(SIDEX_VARIANT sList, int pos){
  SIDEX_DATA_TYPE type = ((sidexBase*)(sList))->getType();
  if (SIDEX_DATA_TYPE_LIST == type){
    return ((sidexList*)sList)->DeleteItem(pos);
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Removes all objects out of a list.
 */
int  sidexutilList_Clear(SIDEX_VARIANT sList){
  int iRet = SIDEX_SUCCESS;
  SIDEX_DATA_TYPE type = ((sidexBase*)(sList))->getType();
  if (SIDEX_DATA_TYPE_LIST == type){
    ((sidexList*)sList)->Clear();
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Get an element of type SIDEX_VARIANT out of a sidexList.
 */
int sidexutilList_Get(SIDEX_VARIANT sList, int index, SIDEX_VARIANT* variant) 
{
  SIDEX_DATA_TYPE type = ((sidexBase*)(sList))->getType();
  if (SIDEX_DATA_TYPE_LIST == type){
    return ((sidexList*)sList)->getElement(variant, index);
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Set an object of type SIDEX_VARIANT into the sidexDict referring the sKey
 */
int sidexutilDict_Set(SIDEX_VARIANT sDict, char* sKey, SIDEX_VARIANT value)
{
  SIDEX_DATA_TYPE type = ((sidexBase*)(sDict))->getType();
  if (SIDEX_DATA_TYPE_DICT == type)
    return ((sidexDict*)sDict)->setValue(sKey, value);
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Remove the Item referring the sKey out of the dict.
 */
int sidexutilDict_Delete(SIDEX_VARIANT sDict, char* sKey){
  SIDEX_DATA_TYPE type = ((sidexBase*)(sDict))->getType();
  if (SIDEX_DATA_TYPE_DICT == type)
    return ((sidexDict*)sDict)->removeEntry(sKey);
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Removes all Items out of a dict.
 */
int sidexutilDict_Clear(SIDEX_VARIANT sDict){
  int iRet = SIDEX_SUCCESS;
  SIDEX_DATA_TYPE type = ((sidexBase*)(sDict))->getType();
  if (SIDEX_DATA_TYPE_DICT == type){
    ((sidexDict*)sDict)->clear();
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Get an element of type SIDEX_VARIANT out of a sidexDict.
 */
int sidexutilDict_Get(SIDEX_VARIANT sDict, char* sKey, SIDEX_VARIANT* variant)
{
  SIDEX_DATA_TYPE type = ((sidexBase*)(sDict))->getType();
  if (SIDEX_DATA_TYPE_DICT == type){
      return ((sidexDict*)sDict)->getValue(sKey, variant);
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Add a column to a Table.
 */
int sidexutilTable_AddColumn(SIDEX_VARIANT sTable, char* sColumn) 
{
  int iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type;
  type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    if (NULL != sColumn && 0 < strlen(sColumn)){
      iRet = ((sidexTable*)sTable)->addColumn(sColumn);
    }
    else{
       iRet = SIDEX_ERR_WRONG_PARAMETER;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Remove a column of a Table.
 */
int sidexutilTable_DeleteColumn(SIDEX_VARIANT sTable, char* sColumn) 
{
  int iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type;
  type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    if (NULL != sColumn && 0 < strlen(sColumn)){
      iRet = ((sidexTable*)sTable)->deleteColumn(sColumn);
    }
    else{
       iRet = SIDEX_ERR_WRONG_PARAMETER;
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief    Get the column name from a Table at the requested index.
 */
int sidexutilTable_GetColumnName_A(SIDEX_VARIANT sTable, int index, char** sColumn)
{
  int iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type;
  type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    iRet = ((sidexTable*)sTable)->getColumnName_A(index, sColumn);
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief    Get the column name from a Table at the requested index.
 */
int sidexutilTable_GetColumnName_X(SIDEX_VARIANT sTable, int index, wchar_t** sColumn)
{
  int iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type;
  type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    iRet = ((sidexTable*)sTable)->getColumnName_X(index, sColumn);
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief    Get the column name from a Table at the requested index.
 */
int sidexutilTable_GetColumnName_W(SIDEX_VARIANT sTable, int index, char16_t** sColumn)
{
  int iRet = SIDEX_SUCCESS;

  SIDEX_DATA_TYPE type;
  type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    iRet = ((sidexTable*)sTable)->getColumnName_W(index, sColumn);
  }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Add a new empty row to a Table.
 */
int sidexutilTable_AddRow(SIDEX_VARIANT sTable, SIDEX_INT32* iRowIdx) 
{
  int iRet = SIDEX_SUCCESS;

  //////////////////////////////////////////////////////////////////////////////////
  // The index after the operation will be the number of rows before the operation:
  iRet = sidexutilTable_Rows(sTable, iRowIdx);
  if (SIDEX_SUCCESS == iRet){
    SIDEX_DATA_TYPE type;
    type = ((sidexBase*)(sTable))->getType();
    if (SIDEX_DATA_TYPE_TABLE == type){
      iRet = ((sidexTable*)sTable)->addRow();
    }
    else{
      iRet = SIDEX_ERR_WRONG_TYPE;
    }
  }
  return iRet;
}


/**
 * @brief   Remove the row referring the rowIndex out of the table.
 */
int sidexutilTable_DeleteRow(SIDEX_VARIANT sTable, int rowIndex){
  SIDEX_DATA_TYPE type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    return sidexutil_TableDeleteRow(sTable, rowIndex);
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Get row of type SIDEX_VARIANT out of a sidexTable.
 */
int sidexutilTable_GetRow(SIDEX_VARIANT sTable, int rowIndex, SIDEX_VARIANT* variant)
{
  SIDEX_DATA_TYPE type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    return sidexutil_TableGetRow(sTable, rowIndex, variant);
  }
  else
    return SIDEX_ERR_WRONG_TYPE;
}


/**
 * @brief   Get a table field of type SIDEX_VARIANT out of a sidexTable.
 */
int sidexutilTable_GetField(SIDEX_VARIANT sTable, int rowIndex, char* sColumnName, SIDEX_VARIANT* variant){
  int iRet;
  SIDEX_BOOL bHasColumn;
 
  SIDEX_DATA_TYPE type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    iRet = sidexutil_TableHasColumn(sTable, sColumnName, &bHasColumn);
      if (SIDEX_SUCCESS == iRet){
        if (SIDEX_TRUE == bHasColumn){
          SIDEX_VARIANT variantRow;
          iRet = sidexutil_TableGetRow(sTable, rowIndex, &variantRow);
          if (SIDEX_SUCCESS == iRet){
            SIDEX_DATA_TYPE type = ((sidexBase*)(variantRow))->getType();
            if (SIDEX_DATA_TYPE_DICT == type){
              iRet = ((sidexDict*)variantRow)->getValue(sColumnName, variant);
            }
            else
              return SIDEX_ERR_WRONG_TYPE;
          }
        }
        else{
          iRet = SIDEX_ERR_NOCONTENT;
        }
      }
    }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   Set a table field of type SIDEX_VARIANT in a Table.
 */
int sidexutilTable_SetField(SIDEX_VARIANT sTable, int rowIndex, char* sColumnName, SIDEX_VARIANT variant){
  int iRet;
  SIDEX_BOOL bHasColumn;
 
  SIDEX_DATA_TYPE type = ((sidexBase*)(sTable))->getType();
  if (SIDEX_DATA_TYPE_TABLE == type){
    iRet = sidexutil_TableHasColumn(sTable, sColumnName, &bHasColumn);
      if (SIDEX_SUCCESS == iRet){
        if (SIDEX_TRUE == bHasColumn){
          SIDEX_INT32 iRows = 0;
          /////////////////////////////////////////////////
          // Add as much as necessary rows:
          iRet = sidexutilTable_Rows(sTable, &iRows);
          for (SIDEX_INT32 i = iRows; SIDEX_SUCCESS == iRet && i <= rowIndex; ++i){
            SIDEX_INT32 iRowIdx;
            sidexutilTable_AddRow(sTable, &iRowIdx);
          }
          if (SIDEX_SUCCESS == iRet){
            SIDEX_VARIANT variantRow;
            iRet = sidexutil_TableGetRow(sTable, rowIndex, &variantRow);
            if (SIDEX_SUCCESS == iRet){
              SIDEX_DATA_TYPE type = ((sidexBase*)(variantRow))->getType();
              if (SIDEX_DATA_TYPE_DICT == type){
                SIDEX_VARIANT tempVar;
                iRet = ((sidexDict*)variantRow)->getValue(sColumnName, &tempVar);
                if (SIDEX_SUCCESS == iRet){
                  iRet = ((sidexDict*)variantRow)->setValue(sColumnName, variant);
                }
              }
              else
                return SIDEX_ERR_WRONG_TYPE;
            }
          }
        }
        else{
          iRet = SIDEX_ERR_NOCONTENT;
        }
      }
    }
  else{
    iRet = SIDEX_ERR_WRONG_TYPE;
  }
  return iRet;
}


/**
 * @brief   counting the characters (not code points) of a UTF-16 string
 */
int strlen16(const char16_t* strarg)
{
   if(!strarg)
     return -1; //strarg is NULL pointer
   const char16_t* str = strarg;
   for(;*str;++str)
     ; // empty body
   return (int)(str-strarg);
}













