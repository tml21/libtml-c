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

#include "sidexLogHandler.h"
#include "sidexUtils.h"
#include "sidexCom.h"
#include "unicode.h"
#include <sstream>
#include <errno.h>

using namespace std;

///////////////////////////////////////////////////////////////////
// in case of an optional group (may be NULL) the following
// define has to be set to a value.
// If group is mandatory the define has to be set into comments.
//#define GROUP_IS_OPTIONAL 1

/**
 * @brief    Constructor.
 */
sidexCom::sidexCom(const char *pname)
{
  setDocument(axl_doc_create("1.0", "UTF-8", true));
  setRootnode(axl_node_create (pname));

  SIDEX_INT32 iSize;
  //////////////////////////////////////////////////////////////////////
  // Maybe we are in the unicode world so make a copy:
  m_utf16DocName = (char16_t*)UTF8toUTF16((char*)pname, &iSize);
  m_utf32DocName = UTF8toUTF32((char*)pname, &iSize);

  axl_doc_set_root((axlDoc*)(getDocument()), (axlNode*)(getRootnode()));
#ifndef USESTREAMS
  m_strRepresentation = new char[512];
#endif // USESTREAMS
#ifdef ANDROID
  chdir("/sdcard/");
#endif // ANDROID
}


/**
 * @brief    Destructor.
 */
sidexCom::~sidexCom()
{
  SIDEX_DOC doc = getDocument();
  if (NULL != doc){
    axl_doc_free((axlDoc *)doc);
    setDocument(NULL);
  }
  setRootnode(NULL);
  //////////////////////////////////////
  // Maybe we are in the unicode world:
  if (NULL != m_utf16DocName){
    delete m_utf16DocName;
    m_utf16DocName = NULL;
  }
  if (NULL != m_utf32DocName){
    delete m_utf32DocName;
    m_utf32DocName = NULL;
  }

#ifndef USESTREAMS
  if (NULL != m_strRepresentation){
    delete []m_strRepresentation;
    m_strRepresentation = NULL;
  }
#endif // USESTREAMS
}

    
/**
 * @brief    Change the document name / XML root node name.
 */
int sidexCom::sidexcom_Set_DocumentName(const char *pname){
  int iRet = SIDEX_SUCCESS;

  if (SIDEX_SUCCESS != sidexcom_IsValidKey((char*)pname)){
    iRet = SIDEX_ERR_WRONG_DOC;
  }
  else{
    SIDEX_INT32 iSize;
    ////////////////////////////////////
    // Make a copy of the wchar_t string:
    if (m_utf32DocName != NULL){
      delete m_utf32DocName;
    }
    m_utf32DocName = UTF8toUTF32((char*)pname, &iSize);
    ////////////////////////////////////
    // Make a copy of the wchar_t string:
    if (m_utf16DocName != NULL){
      delete m_utf16DocName;
    }
    m_utf16DocName = (char16_t*)UTF8toUTF16((char*)pname, &iSize);

    axlNode* rootNode = (axlNode*) getRootnode();
    axl_node_set_name(rootNode, pname);
  }

  return iRet;
}


/**
 * @brief    Get the document name / XML root node name.
 */
const char* sidexCom::sidexcom_Get_DocumentName_A(){
  const char* s = axl_node_get_name((axlNode *)getRootnode());
  return s;
}


/**
 * @brief    Get the document name / XML root node name.
 */
const wchar_t* sidexCom::sidexcom_Get_DocumentName_X(){
  return m_utf32DocName;
}


/**
 * @brief    Get the document name / XML root node name.
 */
const char16_t* sidexCom::sidexcom_Get_DocumentName_W(){
  return m_utf16DocName;
}


/**
 * @brief    Remove the XML tree content except of the root node.
 */
void sidexCom::sidexcom_Clear(){
  const char* sRoot = sidexcom_Get_DocumentName_A();
  int iRootLength = (SIDEX_INT32) strlen(sRoot);
  char* sTemp = new char[iRootLength+1];
#if defined (LINUX) || defined (MINGW_BUILD)
        strcpy (sTemp, sRoot);
#else
       strcpy_s (sTemp, iRootLength + 1, sRoot);
#endif
  axlDoc* doc = (axlDoc*) getDocument();

  if (NULL != doc){
    axl_doc_free (doc);
  }
 //setDocument(axl_doc_create("1.0", "ISO-8859-1", true));
 setDocument(axl_doc_create("1.0", "UTF-8", true));
 setRootnode(axl_node_create (sTemp));
 axl_doc_set_root((axlDoc*)(getDocument()), (axlNode*)(getRootnode()));
 delete []sTemp;
}


/**
 * @brief    Set the document member.
 */
void sidexCom::setDocument(SIDEX_DOC document){
  m_document = document;
}


/**
 * @brief    Get the document member content.
 */
SIDEX_DOC sidexCom::getDocument(){
  return m_document;
}


/**
 * @brief    Set the rootnode member.
 */
void sidexCom::setRootnode(SIDEX_NODE rootnode){
  m_rootnode = rootnode;
}


/**
 * @brief    Get the rootnode member content.
 */
SIDEX_NODE sidexCom::getRootnode(){
  return m_rootnode;
}


/**
 * @brief Saves axl content as an XML formatted document.
 */
int sidexCom::sidexcom_DumpToFile(const char* path){
  int iRet = SIDEX_SUCCESS;

  if (NULL == path || strlen(path) == 0){
    iRet = SIDEX_ERR_WRONG_PARAMETER;
  }
  else{
//    if (!axl_doc_dump_pretty_to_file((axlDoc *)getDocument(), (char *) path, 3))
//      iRet =  SIDEX_ERR_DUMPCONTENT;
    int size = 0;
    char* content = NULL;
    if (!axl_doc_dump_pretty((axlDoc *)getDocument(), &content, &size, 3)){
      iRet =  SIDEX_ERR_DUMPCONTENT;
    }
    else{
      FILE* fp = _fsopen(path, "wb", _SH_DENYNO);
      if (NULL != fp){
        size_t bytesWritten = fwrite (content, 1, size, fp);
        axl_free (content);
        if (bytesWritten != (unsigned int) size){
          iRet =  SIDEX_ERR_DUMPCONTENT;
        }
        fclose(fp);
      }
      else{
        iRet =  SIDEX_ERR_DUMPCONTENT;
      }
    }
  }
  return iRet;
}


/**
 * @brief Saves a SIDEX_HANDLE as an XML formatted char string.
 */
int sidexCom::sidexcom_DumpToString(char **content, SIDEX_INT32* iContentLength){
  int iRet = SIDEX_SUCCESS;
  int iLength;
  if (!axl_doc_dump_pretty ((axlDoc *)getDocument(), content, &iLength, 3)){
     iRet = SIDEX_ERR_DUMPCONTENT;
  }
  else{
    *iContentLength = iLength;
  }
  return iRet;
}


/**
  * @brief Returns the amount of buffer size  internal allocated in sidexcom_DumpToString request.
  */
int sidexCom::sidexcom_DumpToStringLength(SIDEX_INT32* iContentLength){
  int iRet = SIDEX_SUCCESS;
  int iLength;

  iLength = axl_doc_get_flat_size_pretty 	((axlDoc *)getDocument(), 3);
  if (-1 == iLength){
     iRet = SIDEX_ERR_DUMPCONTENT;
  }
  else{
    *iContentLength = iLength;
  }

  return iRet;
}


/**
 * @brief Opens an XML file, write content into SIDEX_HANDLE.
 */
int sidexCom::sidexcom_Load_Content(const char *path){
  int iRet = SIDEX_SUCCESS;
  axlDoc* doc = NULL;

  axlError *error = NULL;

  doc = (axlDoc*) getDocument();

  if (NULL != doc){
    axl_doc_free (doc);
  }

  //////////////////////////////////////
  // Maybe we are in the unicode world:
  if (NULL != m_utf16DocName){
    delete m_utf16DocName;
    m_utf16DocName = NULL;
  }
  if (NULL != m_utf32DocName){
    delete m_utf32DocName;
    m_utf32DocName = NULL;
  }

  doc = NULL;

  ////////////////////////////
  // Open File:
  FILE* fp = _fsopen(path, "rb", _SH_DENYNO);
  if (NULL != fp){
    SIDEX_INT32 lSize = 0;
    char* content = NULL;
    ////////////////////////////
    // File size:
    fseek (fp ,0 ,SEEK_END);
    lSize = ftell (fp);
    rewind (fp);
    ////////////////////////////
    // Alloc memory;
    content = new char[lSize+1];
    ////////////////////////////
    // Read:
    SIDEX_INT32 bytesRead;
    bytesRead = (SIDEX_INT32) fread (content, 1, lSize, fp);
    ////////////////////////////
    // Parse the string:
    doc = axl_doc_parse (content, lSize, &error);
    ////////////////////////////
    // free memory;
    delete []content;

    ////////////////////////////
    // Set document:
    setDocument(doc);
    ////////////////////////////
    // Close File:
    fclose(fp);
  }
  else{
    iRet =  SIDEX_ERR_DUMPCONTENT;
  }

  if (NULL != doc){
    setRootnode(axl_doc_get_root(doc));
    //////////////////////////////////////
    // Maybe we are in the unicode world:
    // Get the new document name:
    const char* s = axl_node_get_name((axlNode *)getRootnode());
    SIDEX_INT32 iLengthUtf16;
    m_utf32DocName = UTF8toUTF32((char*)s, &iLengthUtf16);
    if (NULL == m_utf32DocName){
      setRootnode(NULL);
      iRet = SIDEX_ERR_UNICODE;
    }
    else{
      m_utf16DocName = (char16_t*)UTF8toUTF16((char*)s, &iLengthUtf16);
      if (NULL == m_utf16DocName){
        setRootnode(NULL);
        iRet = SIDEX_ERR_UNICODE;
      }
    }
  }
  else{
    /* Possible use of Errortext in the future:
    *errmsg = strdup(axl_error_get (error));
    printf ("Error %s occurred", *errmsg);
    axl_error_free (error);
    */     
    setRootnode(NULL);
    setDocument(doc);
    iRet = SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}


/**
 * @brief Parse a char string, write content into SIDEX_HANDLE.
 */
int sidexCom::sidexcom_Set_Content(char *content){
  int iRet = SIDEX_SUCCESS;
  axlDoc* doc = NULL;

  axlError *error = NULL;

  doc = (axlDoc*) getDocument();

  if (NULL != doc){
    axl_doc_free (doc);
  }

  //////////////////////////////////////
  // Maybe we are in the unicode world:
  if (NULL != m_utf16DocName){
    delete m_utf16DocName;
    m_utf16DocName = NULL;
  }
  if (NULL != m_utf32DocName){
    delete m_utf32DocName;
    m_utf32DocName = NULL;
  }

  // parse the provided file
  doc = axl_doc_parse (content, (int)strlen(content), &error);

  setDocument(doc);

  if (NULL != doc){
    setRootnode(axl_doc_get_root(doc));
    //////////////////////////////////////
    // Maybe we are in the unicode world:
    // Get the new document name:
    const char* s = axl_node_get_name((axlNode *)getRootnode());
    SIDEX_INT32 iLengthUtf16;
    m_utf32DocName = UTF8toUTF32((char*)s, &iLengthUtf16);
    if (NULL == m_utf32DocName){
      setRootnode(NULL);
      iRet = SIDEX_ERR_UNICODE;
    }
    else{
      m_utf16DocName = (char16_t*)UTF8toUTF16((char*)s, &iLengthUtf16);
      if (NULL == m_utf16DocName){
        setRootnode(NULL);
        iRet = SIDEX_ERR_UNICODE;
      }
    }
  }
  else{
    /* Possible use of Errortext in the future:
    *errmsg = strdup(axl_error_get (error));
    printf ("Error %s occurred", *errmsg);
    axl_error_free (error);
    */     
    setRootnode(NULL);
    setDocument(doc);
    iRet = SIDEX_ERR_DUMPCONTENT;
  }
  return iRet;
}


/**
 * @brief   Allows to get the content of a node.
 */
 const char* sidexCom::sidexcom_GetNodeContent(SIDEX_NODE node)
{
   return axl_node_get_content((axlNode *)node,NULL);
}




/**
 * @brief   Allows to get the content of a node (references like "&amp;" or "&quot;" will be translated).
 */
 const char* sidexCom::sidexcom_GetNodeContentTrans(SIDEX_NODE node)
{
   return axl_node_get_content_trans((axlNode *)node,NULL);
}


/**
 * @brief   Allows to set the content of a node.
 */
 void sidexCom::sidexcom_SetNodeContent(SIDEX_NODE node, const char* content, int length, const char* type)
{
   axlNode *new_node = axl_node_create(axl_node_get_name((axlNode *) node));
   axl_node_set_content(new_node,content,length);
   axl_node_set_attribute(new_node,SIDEX_TYPE_ATTR,type);
   axl_node_replace((axlNode *)node,new_node,true);
}


/**
 * @brief   Allows to set the content of a node.
 */
 void sidexCom::sidexcom_SetNodeEncodedContent(SIDEX_NODE node, const char* content, int length, const char* type, SIDEX_ENCODE encoding, SIDEX_STRING_ENCODING_TYPE format)
{
   axlNode *new_node = axl_node_create(axl_node_get_name((axlNode *) node));
   if (SIDEX_ENCODE_CDATA == encoding){
     // Write the data explicit into a CDATA section:
     axl_node_set_cdata_content(new_node,content,length);
   }
   else{
     // raw or base64 encoded
	   if (SIDEX_ENCODE_NONE == encoding){
       // SIDEX_STRING_FORMAT_UNKNOWN is the default - don't write the default
       if (SIDEX_STRING_FORMAT_UNKNOWN != format){
         axl_node_set_attribute(new_node,SIDEX_STRING_FORMAT_ATTR, sidexutil_StringFormatToString(format));
       }
	   }
     axl_node_set_content(new_node,content,length);
   }
   axl_node_set_attribute(new_node,SIDEX_TYPE_ATTR,type);
   axl_node_set_attribute(new_node,SIDEX_ENC_ATTR,sidexutil_sidexEncodingToString(encoding));
   axl_node_replace((axlNode *)node,new_node,true);
}

 
/**
 * @brief   Get SIDEX_DATA_TYPE of a node.
 */
SIDEX_DATA_TYPE sidexCom::sidexcom_GetNodeType(SIDEX_NODE node)
{
   SIDEX_DATA_TYPE type;

   if (!axl_node_has_attribute((axlNode *)node,SIDEX_TYPE_ATTR)) // No type attribute
      type = SIDEX_DATA_TYPE_UNKNOWN;
   else
      type = sidexutil_strToTypeEnum(axl_node_get_attribute_value((axlNode *)node,SIDEX_TYPE_ATTR));
   return type;
}


/**
 * @brief   Allocates a new sidexBoolean, gets it's value from the provided node.
 */
int sidexCom::sidexcom_BoolFromNode(SIDEX_NODE node, sidexBoolean** sBool)
{
   bool newValue = 0;
   int error = sidexcom_ReadBoolean(node, &newValue);
   if (SIDEX_SUCCESS == error)
      *sBool = new sidexBoolean(newValue);
    return error;
}


/**
 * @brief   Allocates a new sidexInteger, gets it's value from the provided node.
 */
int sidexCom::sidexcom_IntFromNode(SIDEX_NODE node, sidexInteger** sInt)
{
   SIDEX_INT64 newValue = 0;
   int error = sidexcom_ReadInteger(node, &newValue);
   if (SIDEX_SUCCESS == error)
      *sInt = new sidexInteger(newValue);
    return error;
}


/**
 * @brief   Allocates a new sidexFloat, gets it's value from the provided node.
 */
int sidexCom::sidexcom_FloatFromNode(SIDEX_NODE node, sidexFloat** sFloat)
{
   double newValue = 0.0;
   int error = sidexcom_ReadFloat(node, &newValue);
   if (SIDEX_SUCCESS == error)
      *sFloat = new sidexFloat(newValue);
    return error;
}


/**
 * @brief   Allocates a new sidexDateTime, gets it's value from the provided node.
 */
int sidexCom::sidexcom_DateTimeFromNode(SIDEX_NODE node, sidexDateTime** sDateTime)
{
   int error = sidexcom_ReadDateTime(node, sDateTime);
   return error;
}


/**
 * @brief   Allocates a new sidexString (raw encoded), gets it's value from the provided node.
 */
int sidexCom::sidexcom_StringFromNode(SIDEX_NODE node, sidexString** sString)
{
   char* newValue= NULL;
   SIDEX_INT32 size = 0;
   SIDEX_ENCODE iEncoding;
   SIDEX_STRING_ENCODING_TYPE iFormat;
   
   iEncoding = SIDEX_ENCODE_NONE;
   // I have to remember the original string encoding of the following read:
   int error = sidexcom_ReadString(node, &newValue, &size, &iEncoding, &iFormat);
   if (error == SIDEX_SUCCESS)
   {
      char *content;
      switch (iEncoding){
        case SIDEX_ENCODE_BASE64:
          if (NULL != newValue){
            ///////////////////////////////////////////////////
            // The string isn't NULL so I have to decode it first:
            content = (char *)sidexutil_decodeBase64(newValue, &size);
          }
          else{
            content = newValue;
          }
          break;
        case SIDEX_ENCODE_NONE: 
        case SIDEX_ENCODE_CDATA:
        default:
          content = newValue;
          break;
      }
      *sString = new sidexString(content, size, iEncoding);
      (*sString)->setStringFormat(iFormat);
      if (NULL != newValue){
        ////////////////////////////////////////////////////////////////////////////
        // The string isn't NULL so I have to free memory depending of the encoding:
        if (SIDEX_ENCODE_BASE64 == iEncoding){
          ///////////////////////////////////////////////////////////////
          // In case of base64 encoding I have to free memory now:
          delete[] content;
        }
        else{
          ///////////////////////////////////////////////////////////////
          // in case of a "axl_node_get_content_trans" we do have a copy
          // of a node content, that has to be freed explicit
          axl_free(newValue);
        }
      }
   }
   return error;
}


/**
 * @brief   Allocates a new char string (raw encoded), gets it's value from the provided node.
 */
int sidexCom::sidexcom_StringFromNode(SIDEX_NODE node, char** sString, SIDEX_INT32* iStrLength, SIDEX_DATA_TYPE expectedType)
{
   char* newValue= NULL;
   SIDEX_INT32 size = 0;
   SIDEX_ENCODE iEncoding;
   SIDEX_STRING_ENCODING_TYPE iFormat;
   int error = SIDEX_SUCCESS;
   
   iEncoding = SIDEX_ENCODE_NONE;
   SIDEX_DATA_TYPE iType = sidexcom_GetNodeType(node);
   if (expectedType != iType){
     error = SIDEX_ERR_WRONG_TYPE;
   }
   else{
     // I have to remember the original string encoding of the following read:
     error = sidexcom_ReadString(node, &newValue, &size, &iEncoding,&iFormat);
     if (error == SIDEX_SUCCESS)
     {
       if (NULL != newValue){
         ///////////////////////////////////////////////////////////
         // The string is not NULL, so take care of it:
         if (SIDEX_ENCODE_BASE64 == iEncoding){
           /////////////////////
           // Encoding is base64:
           SIDEX_INT32 iRetSize = -1;
           *sString = sidexutil_decodeBase64(newValue, &iRetSize);
           *iStrLength = iRetSize;
         }
         else{
           /////////////////////
           // Encoding is raw or cdata:
           // if we get here, we assume raw (encoding = raw or no encoding type given)
           char* outbuf = new char[size+1];
    #if defined(LINUX) || defined (MINGW_BUILD)
            strcpy (outbuf, newValue);
    #else
           strcpy_s (outbuf, size + 1, newValue);
    #endif
           *sString = outbuf;
           *iStrLength = size;

            ///////////////////////////////////////////////////////////////
            // in case of a "axl_node_get_content_trans" we do have a copy
            // of a node content, that has to be freed explicit
            axl_free(newValue);
         }
       }
       else{
         ///////////////////////////////////////////////////////////
         // The string is NULL, so it's empty:
         char* outbuf = new char[1];
         outbuf[0] = '\0';
         *sString = outbuf;
         *iStrLength = 0;
       }
     }
   }
   return error;
}


/**
 * @brief   Allocates a new char string (raw encoded), gets it's value from the provided node.
 */
int sidexCom::sidexcom_StringFromNodeLength(SIDEX_NODE node, SIDEX_INT32* iStrLength, SIDEX_DATA_TYPE expectedType, tmlUnicodeID iUnicode)
{
   char* newValue= NULL;
   SIDEX_INT32 size = 0;
   SIDEX_ENCODE iEncoding;
   SIDEX_STRING_ENCODING_TYPE iFormat;
   int error = SIDEX_SUCCESS;
   
   iEncoding = SIDEX_ENCODE_NONE;
   SIDEX_DATA_TYPE iType = sidexcom_GetNodeType(node);
   if (expectedType != iType){
     error = SIDEX_ERR_WRONG_TYPE;
   }
   else{
     // I have to remember the original string encoding of the following read:
     int error = sidexcom_ReadString(node, &newValue, &size, &iEncoding, &iFormat);
     if (error == SIDEX_SUCCESS)
     {
       if (NULL != newValue){
         ///////////////////////////////////////////////////////////
         // The string is not NULL, so take care of it:
         if (SIDEX_ENCODE_BASE64 == iEncoding){
           /////////////////////
           // Encoding is base64:
           *iStrLength = sidexutil_decodeBase64Length(newValue);
         }
         else{
            switch (iUnicode){
              case idUNICODE_ASCII:
                  *iStrLength = size;
                  break;
              case idUNICODE_WCHAR_T:
                  {
                    *iStrLength = UTF8toUTF32_length_request(newValue);
                  }
                  break;
              case idUNICODE_CHAR16_T:
                  {
                    *iStrLength = UTF8toUTF16_length_request(newValue);
                  }
                   break;
            }
            ///////////////////////////////////////////////////////////////
            // in case of a "axl_node_get_content_trans" we do have a copy
            // of a node content, that has to be freed explicit
            axl_free(newValue);
         }
       }
       else{
         ///////////////////////////////////////////////////////////
         // The string is NULL, so it's empty:
         *iStrLength = 0;
       }
     }
   }
   return error;
}


/**
 * @brief   Allocates a new sidexList, gets it's value from the provided node.
 */
int sidexCom::sidexcom_ListFromNode(SIDEX_NODE node, sidexList** sList)
{
   *sList = new sidexList;
   int error = sidexcom_ReadList(node, *sList);
   if (error != SIDEX_SUCCESS){
      delete(*sList);
      *sList = NULL;
   }
    return error;
}


/**
 * @brief   Allocates a new sidexDict, gets it's value from the provided node.
 */
int sidexCom::sidexcom_DictFromNode(SIDEX_NODE node, sidexDict** sDict)
{
   *sDict = new sidexDict;
   int error = sidexcom_ReadDict(node, *sDict);
   if (error != SIDEX_SUCCESS){
      delete(*sDict);
      *sDict = NULL;
   }
   return error;
}


/**
 * @brief   Allocates a new sidexTable, gets it's value from the provided node.
 */
int sidexCom::sidexcom_TableFromNode(SIDEX_NODE node, sidexTable** sTable)
{
   *sTable = new sidexTable;
   int error = sidexcom_ReadTable(node, *sTable);
   if (SIDEX_SUCCESS != error){
      delete(*sTable);
      *sTable = NULL;
   }
   return error;
}


/**
 * @brief  Returns SIDEX_NODE dependent of a childNodeName.
 */
int sidexCom::sidexcom_GetNodeForChildNodeName(SIDEX_NODE rootNode, char* cChildNodeName, bool bCreate, bool* bMatch, SIDEX_NODE* newNode)
{
  int iRet = SIDEX_SUCCESS;
  SIDEX_NODE subNode = (SIDEX_NODE) axl_node_get_child_called ((axlNode*) rootNode, cChildNodeName);
  if (NULL == subNode){
    *bMatch = false;
    if (bCreate){
      iRet = sidexcom_CreateNode(rootNode, cChildNodeName, &subNode);
    }
  }
  else{
    *bMatch = true;
  }
  if (SIDEX_SUCCESS == iRet){
    *newNode = subNode;
  }
  return iRet;
}


/**
* @brief  write the sidexList into a node
*/
int sidexCom::sidexcom_WriteListTree(SIDEX_NODE node, sidexList* sList)
{
  int iRet = SIDEX_SUCCESS;

  // set the list attribute:
  axl_node_set_attribute((axlNode*)node,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_LIST);

  sidexList::listElement *e = sList->getFirst();
  while (NULL != e && SIDEX_SUCCESS == iRet)
  {
    sidexBase *eValue;
    eValue = (sidexBase* )e->value;
    axlNode* subNode;
    char* cStringRepresentation;
    switch ( eValue->getType() )
    {
      case SIDEX_DATA_TYPE_NONE:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_NONE);
          axl_node_set_content(subNode, SIDEX_NONE_STRING, -1);
        }
        break;
      case SIDEX_DATA_TYPE_BOOLEAN:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_BOOLEAN);
          cStringRepresentation = ( (sidexBoolean *)(e->value) ) ->toString();
          axl_node_set_content(subNode, cStringRepresentation, -1);
          // The returned character array is owned by the sidexBoolean 
          // Thus, the caller must not modify or free the memory !
          //delete[] cStringRepresentation;
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      case SIDEX_DATA_TYPE_INTEGER:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_INTEGER);
          cStringRepresentation = ( (sidexInteger *)(e->value) ) ->toString();
          axl_node_set_content(subNode, cStringRepresentation, -1);
          // The returned character array is owned by the sidexInteger 
          // Thus, the caller must not modify or free the memory !
          //delete[] cStringRepresentation;
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      case SIDEX_DATA_TYPE_FLOAT:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_FLOAT);
          cStringRepresentation = ( (sidexFloat *)(e->value) ) ->toString();
          axl_node_set_content(subNode, cStringRepresentation, -1);
          // The returned character array is owned by the sidexFloat 
          // Thus, the caller must not modify or free the memory !
          //delete[] cStringRepresentation
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      case SIDEX_DATA_TYPE_DATETIME:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_DATETIME);
          cStringRepresentation = ( (sidexDateTime *)(e->value) ) ->toString();
          axl_node_set_content(subNode, cStringRepresentation, -1);
          // The returned character array is owned by the sidexDateTime 
          // Thus, the caller must not modify or free the memory !
          //delete[] cStringRepresentation
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      case SIDEX_DATA_TYPE_STRING:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          axl_node_set_attribute(subNode, SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_STRING);
          axl_node_set_attribute(subNode, SIDEX_ENC_ATTR, sidexutil_sidexEncodingToString(((sidexString *)(e->value)) ->getEncoding()));
          SIDEX_STRING_ENCODING_TYPE format = ((sidexString *)(e->value)) ->getStringFormat();
          if (SIDEX_STRING_FORMAT_UNKNOWN != format){
            axl_node_set_attribute(subNode, SIDEX_STRING_FORMAT_ATTR, sidexutil_StringFormatToString(format));
          }
          cStringRepresentation = ((sidexString *)(e->value) ) ->toString();
          axl_node_set_content(subNode, cStringRepresentation, -1);
          // The character array from sidexString.toString() has to be dallocated:
          delete[] cStringRepresentation;
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      case SIDEX_DATA_TYPE_BINARY:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          axl_node_set_attribute(subNode, SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_BINARY);
          axl_node_set_attribute(subNode, SIDEX_ENC_ATTR, sidexutil_sidexEncodingToString(((sidexString *)(e->value)) ->getEncoding()));
          cStringRepresentation = ((sidexString *)(e->value) ) ->toString();
          axl_node_set_content(subNode, cStringRepresentation, -1);
          // The character array from sidexString.toString() has to be dallocated:
          delete[] cStringRepresentation;
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      case SIDEX_DATA_TYPE_LIST:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          sidexcom_WriteListTree(subNode, (sidexList *)(e->value));
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      case SIDEX_DATA_TYPE_DICT:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          sidexcom_WriteDictTree(subNode, (sidexDict *)(e->value) );
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      case SIDEX_DATA_TYPE_TABLE:
        iRet =  sidexcom_CreateNode(node, SIDEX_LIST_ITEM, (SIDEX_NODE*) &subNode);
        if (SIDEX_SUCCESS == iRet){
          sidexcom_WriteTableTree(subNode, (sidexTable *)(e->value) );
          axl_node_set_child((axlNode *) node, subNode);
        }
        break;
      default:
        iRet = SIDEX_ERR_WRONG_TYPE;
        break;
    }
    if (SIDEX_SUCCESS == iRet){
      e = e->next;
    }
  }
  return iRet;
}


int sidexCom::sidexcom_WriteDictTree(SIDEX_NODE node, sidexDict* sDict)
{
  int iRet = SIDEX_SUCCESS;

  // set the dict attribute:
  axl_node_set_attribute((axlNode*)node,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_DICT);

  int dictSize = sDict->getNumberOfDictEntrys();
  char** keyArray = NULL;

  // Don't care about the return value 
  // For an empty dictionary it may be SIDEX_ERR_NOCONTENT
  // but writing of an empty dict must also be possible:
  /*iRet =*/ sDict->getKeys(&keyArray);

  char sItemIdentifier[256];
  char sKeyIdentifier[256];
  char sValueIdentifier[256];
  SIDEX_DATA_TYPE sType = SIDEX_DATA_TYPE_UNKNOWN;
  axlNode* subNode;
  axlNode* subNodeContainer;
  char* cStringRepresentation;

  for (long i = 0; i < dictSize && SIDEX_SUCCESS == iRet; i++)
  {
      #if defined(LINUX) || defined (MINGW_BUILD)
        sprintf(sItemIdentifier, "%s", DICT_ITEM_IDENTIFIER_DEF);
      #else // LINUX
        sprintf_s(sItemIdentifier, 256, "%s", DICT_ITEM_IDENTIFIER_DEF);
      #endif // LINUX
      iRet =  sidexcom_CreateNode(node, sItemIdentifier, (SIDEX_NODE*) &subNodeContainer);
      axl_node_set_child((axlNode *) node, subNodeContainer);


      SIDEX_VARIANT sDictValue;
      char *sDictKey;

      sDictKey = keyArray[i];
      iRet = sDict->getValue(sDictKey, &sDictValue);
      if (SIDEX_SUCCESS != iRet){
        delete[] keyArray;
      }

      if (SIDEX_SUCCESS == iRet){
        sType = ((sidexBase*)sDictValue)->getType();
        if (SIDEX_DATA_TYPE_UNKNOWN != sType){
          #if defined(LINUX) || defined (MINGW_BUILD)
            sprintf(sKeyIdentifier, "%s", DICT_KEY_IDENTIFIER_DEF);
            sprintf(sValueIdentifier, "%s", DICT_VALUE_IDENTIFIER_DEF);
          #else // LINUX
            sprintf_s(sKeyIdentifier, 256, "%s", DICT_KEY_IDENTIFIER_DEF);
            sprintf_s(sValueIdentifier, 256, "%s", DICT_VALUE_IDENTIFIER_DEF);
          #endif // LINUX
          iRet =  sidexcom_CreateNode(subNodeContainer, sKeyIdentifier, (SIDEX_NODE*) &subNode);
          if (SIDEX_SUCCESS == iRet){
            axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_STRING);
            axl_node_set_attribute(subNode, SIDEX_ENC_ATTR, SIDEX_ENC_ATTR_NONE);
            axl_node_set_content(subNode, sDictKey, -1);
          }
        }
      }
      if (SIDEX_SUCCESS == iRet){
        switch (sType)
        {
          case SIDEX_DATA_TYPE_NONE:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_NONE);
              axl_node_set_content(subNode, SIDEX_NONE_STRING, -1);
            }
            break;
          case SIDEX_DATA_TYPE_BOOLEAN:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_BOOLEAN);
              cStringRepresentation = ((sidexBoolean *)(sDictValue))->toString();
              axl_node_set_content(subNode, cStringRepresentation, -1);
              // The returned character array is owned by the sidexBoolean 
              // Thus, the caller must not modify or free the memory !
            }
            break;
          case SIDEX_DATA_TYPE_INTEGER:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_INTEGER);
              cStringRepresentation = ((sidexInteger *)(sDictValue))->toString();
              axl_node_set_content(subNode, cStringRepresentation, -1);
              // The returned character array is owned by the sidexInteger 
              // Thus, the caller must not modify or free the memory !
            }
            break;
          case SIDEX_DATA_TYPE_FLOAT:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_FLOAT);
              cStringRepresentation = ((sidexFloat *)(sDictValue))->toString();
              axl_node_set_content(subNode, cStringRepresentation, -1);
              // The returned character array is owned by the sidexFloat 
              // Thus, the caller must not modify or free the memory !
            }
            break;
          case SIDEX_DATA_TYPE_DATETIME:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_DATETIME);
              cStringRepresentation = ((sidexDateTime *)(sDictValue))->toString();
              axl_node_set_content(subNode, cStringRepresentation, -1);
              // The returned character array is owned by the sidexDateTime
              // Thus, the caller must not modify or free the memory !
            }
            break;
          case SIDEX_DATA_TYPE_STRING:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_STRING);
              axl_node_set_attribute(subNode, SIDEX_ENC_ATTR, sidexutil_sidexEncodingToString(((sidexString *)(sDictValue))->getEncoding()));
              SIDEX_STRING_ENCODING_TYPE format = ((sidexString *)(sDictValue)) ->getStringFormat();
              if (SIDEX_STRING_FORMAT_UNKNOWN != format){
                axl_node_set_attribute(subNode, SIDEX_STRING_FORMAT_ATTR, sidexutil_StringFormatToString(format));
              }
              cStringRepresentation = ((sidexString *)(sDictValue))->toString();
              axl_node_set_content(subNode, cStringRepresentation, -1);
              // The character array from sidexString.toString() has to be dallocated:
              delete[]cStringRepresentation;
            }
            break;
          case SIDEX_DATA_TYPE_BINARY:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              axl_node_set_attribute(subNode,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_BINARY);
              axl_node_set_attribute(subNode, SIDEX_ENC_ATTR, sidexutil_sidexEncodingToString(((sidexString *)(sDictValue))->getEncoding()));
              cStringRepresentation = ((sidexString *)(sDictValue))->toString();
              axl_node_set_content(subNode, cStringRepresentation, -1);
              // The character array from sidexString.toString() has to be dallocated:
              delete[]cStringRepresentation;
            }
            break;
          case SIDEX_DATA_TYPE_LIST:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              sidexcom_WriteListTree(subNode, (sidexList *)(sDictValue));
              axl_node_set_child((axlNode *) subNodeContainer, subNode);
            }
            break;
          case SIDEX_DATA_TYPE_DICT:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              sidexcom_WriteDictTree(subNode, (sidexDict *)(sDictValue));
              axl_node_set_child((axlNode *) subNodeContainer, subNode);
            }
            break;
          case SIDEX_DATA_TYPE_TABLE:
            iRet =  sidexcom_CreateNode(subNodeContainer, sValueIdentifier, (SIDEX_NODE*) &subNode);
            if (SIDEX_SUCCESS == iRet){
              sidexcom_WriteTableTree(subNode, (sidexTable *)(sDictValue));
              axl_node_set_child((axlNode *) subNodeContainer, subNode);
            }
            break;
          default:
            iRet = SIDEX_ERR_WRONG_TYPE;
            break;
         }
      }
  }
  if (NULL != keyArray){
    delete[] keyArray;
  }
  return iRet;
}


/**
* @brief  Write the sidexTable object as content of provided node.(Subkeys will be created)
*/
int sidexCom::sidexcom_WriteTableTree(SIDEX_NODE node, sidexTable* sTable)
{
   int iRet = SIDEX_SUCCESS;

   // set the table attribute:
   axl_node_set_attribute((axlNode*)node,SIDEX_TYPE_ATTR,SIDEX_TYPE_ATTR_TABLE);


   // set the column names:
   SIDEX_NODE columnsNode = NULL;
   iRet = sidexcom_CreateNode(node, SIDEX_TABLE_COLUMNS, &columnsNode);
   if (SIDEX_SUCCESS == iRet){
     sidexList* ColumnsList;
     ColumnsList = sTable->getColumns();
     if (SIDEX_SUCCESS == iRet){
       iRet = sidexcom_WriteListTree(columnsNode, ColumnsList);
     }
   }

   // set the rows:
   int nRows = sTable->countRows();
   for(int i = 0; i < nRows && iRet == SIDEX_SUCCESS; i++)
   {
     SIDEX_NODE row = NULL;
     iRet = sidexcom_CreateNode(node, SIDEX_TABLE_ITEM, &row);
     if (SIDEX_SUCCESS == iRet){
       sidexDict* dict;
       iRet = sTable->getRow(&dict, i);
       if (SIDEX_SUCCESS == iRet){
         iRet = sidexcom_WriteDictTree(row, dict);
       }
     }
   }
   return iRet;
}


/**
 * @brief   Sets none value (empty value) as content of provided node.
 */
void sidexCom::sidexcom_WriteNone(SIDEX_NODE node)
{
  sidexcom_SetNodeContent(node, SIDEX_NONE_STRING, -1,SIDEX_TYPE_ATTR_NONE);
}


/**
 * @brief   Sets SIDEX_BOOL value as content of provided node.
 */
void sidexCom::sidexcom_WriteBoolean(SIDEX_NODE node, SIDEX_BOOL value)
{
#ifdef USESTREAMS
  stringstream  sStream;
  string        sString;

  sStream << value << ends;
  sString = sStream.str();

  sidexcom_SetNodeContent(node, sString.data(), -1,SIDEX_TYPE_ATTR_BOOLEAN);
#else // USESTREAMS
  if (SIDEX_TRUE == value){
    sidexcom_SetNodeContent(node, "1", -1,SIDEX_TYPE_ATTR_BOOLEAN);
  }
  else{
    sidexcom_SetNodeContent(node, "0", -1,SIDEX_TYPE_ATTR_BOOLEAN);
  }
#endif // USESTREAMS
}


/**
 * @brief   Sets integer value as content of provided node.
 */
void sidexCom::sidexcom_WriteInteger(SIDEX_NODE sidexHandle, SIDEX_INT64 value)
{
#ifdef USESTREAMS
  stringstream  sStream;
  string        sString;

  sStream << value << ends;
  sString = sStream.str();
  sidexcom_SetNodeContent(sidexHandle, sString.data(), -1,SIDEX_TYPE_ATTR_INTEGER);
#else // USESTREAMS
#if defined(LINUX) || defined (MINGW_BUILD)
  sprintf(m_strRepresentation, "%lld", value);
#else // LINUX
  sprintf_s(m_strRepresentation, 512, "%lld", value);
#endif // LINUX
  sidexcom_SetNodeContent(sidexHandle, m_strRepresentation, -1,SIDEX_TYPE_ATTR_INTEGER);
#endif // USESTREAMS
}


/**
 * @brief   Sets float value as content of provided node.
 */
void sidexCom::sidexcom_WriteFloat(SIDEX_NODE node, double value)
{
#ifdef USESTREAMS
  stringstream  sStream;
  string        sString;

  sStream << value << ends;
  sString = sStream.str();
  sidexcom_SetNodeContent(node, sString.data(), -1,SIDEX_TYPE_ATTR_FLOAT);
#else // USESTREAMS
#if defined(LINUX) || defined (MINGW_BUILD)
  sprintf(m_strRepresentation, "%f", value);
#else // LINUX
  sprintf_s(m_strRepresentation, 512, "%Lf", value);
#endif // LINUX



  sidexcom_SetNodeContent(node, m_strRepresentation, -1,SIDEX_TYPE_ATTR_FLOAT);
#endif // USESTREAMS
}

 /**
 * @brief   Sets DateTime value as content of provided node.
 */
void sidexCom::sidexcom_WriteDateTime(SIDEX_NODE node, SIDEX_VARIANT value){
  sidexcom_SetNodeContent(node, ((sidexDateTime*)value)->toString(), -1,SIDEX_TYPE_ATTR_DATETIME);
}



/**
 * @brief   Reads content of provided node, interpreting it as variant.
 */
int sidexCom::sidexcom_ReadVariant(SIDEX_NODE node, SIDEX_VARIANT* variant) 
{
   *variant = SIDEX_HANDLE_TYPE_NULL;
   SIDEX_DATA_TYPE type = sidexcom_GetNodeType(node);
   int iRet = SIDEX_SUCCESS;
   void* value;
  
   switch (type)
   {
      case SIDEX_DATA_TYPE_NONE:
         value = new sidexNone();
         iRet = SIDEX_SUCCESS;
         break;
      case SIDEX_DATA_TYPE_BOOLEAN:
         iRet = sidexcom_BoolFromNode(node, (sidexBoolean**) &value);
         break;
      case SIDEX_DATA_TYPE_INTEGER:
         iRet = sidexcom_IntFromNode(node, (sidexInteger**) &value);
         break;
      case SIDEX_DATA_TYPE_FLOAT:
         iRet = sidexcom_FloatFromNode(node, (sidexFloat**) &value);
         break;
      case SIDEX_DATA_TYPE_DATETIME:
         iRet = sidexcom_DateTimeFromNode(node, (sidexDateTime**) &value);
         break;
      case SIDEX_DATA_TYPE_STRING:
      case SIDEX_DATA_TYPE_BINARY:
         iRet = sidexcom_StringFromNode(node, (sidexString**) &value);
         break;
      case SIDEX_DATA_TYPE_LIST:
         iRet = sidexcom_ListFromNode(node, (sidexList**) &value);
         break;
      case SIDEX_DATA_TYPE_DICT:
         iRet = sidexcom_DictFromNode(node, (sidexDict**) &value);
         break;
      case SIDEX_DATA_TYPE_TABLE:
         iRet = sidexcom_TableFromNode(node, (sidexTable**) &value);
         break;   
      case SIDEX_DATA_TYPE_UNKNOWN:
      default:
         iRet = SIDEX_ERR_WRONG_TYPE;
         break;
   }
   if (SIDEX_SUCCESS == iRet){
     *variant = (SIDEX_VARIANT)value;
   }
   return iRet;
}


/**
 * @brief   Reads content of provided node, interpreting it as boolean.
 */
int sidexCom::sidexcom_ReadBoolean(SIDEX_NODE node, bool *value)
{
   if ( sidexcom_GetNodeType(node) != SIDEX_DATA_TYPE_BOOLEAN ) 
      return SIDEX_ERR_WRONG_TYPE;

   char *content = (char *) sidexcom_GetNodeContent(node);
   if (0 == strlen(content))
      return SIDEX_ERR_NOCONTENT;

  *value = (0 != atoi(content)); 
  if (ERANGE == errno || EINVAL == errno)
    return SIDEX_ERR_INVALID_BOOLEAN;
  return SIDEX_SUCCESS;
}


/**
 * @brief   Reads content of provided node, interpreting it as integer.
 */
int sidexCom::sidexcom_ReadInteger(SIDEX_NODE node, SIDEX_INT64* value)
{
   if ( sidexcom_GetNodeType(node) != SIDEX_DATA_TYPE_INTEGER ) 
      return SIDEX_ERR_WRONG_TYPE;

   char *content = (char *) sidexcom_GetNodeContent(node);
   if (0 == strlen(content))
      return SIDEX_ERR_NOCONTENT;

#if defined(LINUX) || defined (MINGW_BUILD)
   *value = atoll(content);
#else
   *value = _strtoi64(content, NULL, 10);
   if (ERANGE == errno || EINVAL == errno)
      return SIDEX_ERR_INVALID_INTEGER;
#endif
   return SIDEX_SUCCESS;
}


/**
 * @brief   Reads content of provided node, interpreting it as float.
 */
int sidexCom::sidexcom_ReadFloat(SIDEX_NODE node, double *value)
{
   if ( sidexcom_GetNodeType(node) != SIDEX_DATA_TYPE_FLOAT ) 
      return SIDEX_ERR_WRONG_TYPE;

   char *content = (char *) sidexcom_GetNodeContent(node);
   if (0 == strlen(content))
      return SIDEX_ERR_NOCONTENT;

   *value = atof(content);
   if (ERANGE == errno || EINVAL == errno)
      return SIDEX_ERR_INVALID_FLOAT;
   return SIDEX_SUCCESS;
}


/**
 * @brief   Reads content of provided node, interpreting it as the content DateTime.
 */
int sidexCom::sidexcom_ReadDateTime(SIDEX_NODE node, sidexDateTime** sDateTime)
{
   int iRet = SIDEX_SUCCESS;
   if ( sidexcom_GetNodeType(node) != SIDEX_DATA_TYPE_DATETIME ){
      iRet = SIDEX_ERR_WRONG_TYPE;
   }
   else{
     char *content = (char *) sidexcom_GetNodeContent(node);
     if (0 == strlen(content))
        return SIDEX_ERR_NOCONTENT;
     sidexDateTime* vDateTime = new sidexDateTime();
     iRet = vDateTime->setValue(content);
     if (SIDEX_SUCCESS == iRet){
       *sDateTime = vDateTime;
     }
     else{
       sidexutil_Variant_DecRef ((SIDEX_VARIANT)vDateTime);
     }
   }
   return iRet;
}


/**
 * @brief   Reads content of provided node, interpreting it as char array.
 */
int sidexCom::sidexcom_ReadString(SIDEX_NODE node, char **value, SIDEX_INT32* size, SIDEX_ENCODE* iEncoding, SIDEX_STRING_ENCODING_TYPE* format)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_DATA_TYPE type = sidexcom_GetNodeType(node);
   if ((type != SIDEX_DATA_TYPE_STRING) && (type != SIDEX_DATA_TYPE_BINARY)) {
      iRet = SIDEX_ERR_WRONG_TYPE;
   }
   else{
     if (axl_node_has_attribute((axlNode*) node, SIDEX_ENC_ATTR)) // encoding attribute
     {
        if(axl_node_has_attribute_value((axlNode*) node, SIDEX_ENC_ATTR, SIDEX_ENC_ATTR_BASE64)) // base64 encoding
        {
           /////////////////////
           // Encoding is base64:
           *iEncoding = SIDEX_ENCODE_BASE64; 
        }
        else{
          if(axl_node_has_attribute_value((axlNode*) node, SIDEX_ENC_ATTR, SIDEX_ENC_ATTR_CDATA)) // CDATA section encoding
          {
             /////////////////////////////
             // Encoding is CDATA section:
             *iEncoding = SIDEX_ENCODE_CDATA; 
          }
          else{
             /////////////////////////////
             // Encoding is raw:
            *iEncoding = SIDEX_ENCODE_NONE;
          }
        }
     }
     else{
       /////////////////////////////
       // Encoding is raw:
       *iEncoding = SIDEX_ENCODE_NONE;
     }
     ////////////////////////////////////////////////
     // Now I must fetch the data depending of the encoding:
     char* sNodeContent;
     switch (*iEncoding){
       case SIDEX_ENCODE_CDATA:
         // CDATA section - Possible escape characters will be translated:
         sNodeContent = (char *) sidexcom_GetNodeContentTrans(node);
         break;
       case SIDEX_ENCODE_NONE: 
         // raw - Possible escape characters will be translated:
         sNodeContent = (char *) sidexcom_GetNodeContentTrans(node);
         break;
       case SIDEX_ENCODE_BASE64:
       default:
         // base64 - get data as it is:
         sNodeContent = (char *) sidexcom_GetNodeContent(node);
         break;
     }
     *value = sNodeContent;
     int iContentSize = 0;
     if (NULL != sNodeContent){
       iContentSize = (int) strlen(sNodeContent);
     }
     *size = iContentSize;
     ////////////////////////////////////////
     // The string encoding format:
     if (axl_node_has_attribute((axlNode*) node, SIDEX_STRING_FORMAT_ATTR)) // string format attribute
     {
        const char* formatAttr = axl_node_get_attribute_value((axlNode*) node, SIDEX_STRING_FORMAT_ATTR);
        SIDEX_STRING_ENCODING_TYPE retFormat = sidexutil_StringToStringFormat(formatAttr);
        if (SIDEX_CONV_NOT_FOUND == retFormat){
          iRet = SIDEX_ERR_WRONG_ENCODING_FORMAT;
        }
        else{
          *format = retFormat;
        }
     }
     else{
       *format = SIDEX_STRING_FORMAT_UNKNOWN;
     }
   }
   return iRet;
}


/**
  * @brief   Returns thelength of the content of provided handles node (interpreting it as char array).
 */
int sidexCom::sidexcom_ReadString(char* ngroup, char* nkey, char** value, SIDEX_INT32* iStrLength, SIDEX_DATA_TYPE expectedType){
  SIDEX_NODE nNode;
  int iRet = SIDEX_SUCCESS;

#ifndef GROUP_IS_OPTIONAL
  if (NULL != ngroup){
#else //GROUP_IS_OPTIONAL
  if (1 == 1){
#endif //GROUP_IS_OPTIONAL
    if (NULL != nkey){
      if (SIDEX_SUCCESS != sidexcom_IsValidKey(nkey)){
        iRet = SIDEX_ERR_WRONG_KEY;
      }
      else{
        if (NULL != ngroup){
          if (SIDEX_SUCCESS != sidexcom_IsValidKey(ngroup)){
            iRet = SIDEX_ERR_WRONG_GROUP;
          }
          else{
            // Behandlung mit nGroup:
            iRet = sidexcom_GetChildByName(&nNode, getRootnode(), ngroup);
            if (SIDEX_SUCCESS == iRet){
              int iInternalCount;
              iRet = sidexcom_GetChildCount(&iInternalCount, nNode);
              if (SIDEX_SUCCESS == iRet){
                if (1 <= iInternalCount){
                  SIDEX_NODE nInternalNode;
                  iRet = sidexcom_GetChildByName(&nInternalNode, nNode, nkey);
                  if (SIDEX_SUCCESS == iRet){
                    if (NULL != nInternalNode){
                      iRet = sidexcom_StringFromNode(nInternalNode, value, iStrLength, expectedType);
                    }
                  }
                }
              }
            }
          }
        }
        else{
          // Behandlung ohne nGroup:
          iRet = sidexcom_GetChildByName(&nNode, getRootnode(), nkey);
          if (SIDEX_SUCCESS == iRet){
            iRet = sidexcom_StringFromNode(nNode, value, iStrLength, expectedType);
          }
        }
      }
    }
    else{
      iRet = SIDEX_ERR_MISSING_KEY;
    }
  }
  else{
    iRet = SIDEX_ERR_MISSING_GROUP;
  }
  if (NULL == *value && SIDEX_SUCCESS == iRet){
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
  * @brief   Returns the length of the value from the provided node interpreted as a string.
  */
int sidexCom::sidexcom_ReadStringLength(char* ngroup, char* nkey, SIDEX_INT32* iStrLength, SIDEX_DATA_TYPE expectedType, tmlUnicodeID iUnicode){
  SIDEX_NODE nNode;
  int iRet = SIDEX_SUCCESS;

#ifndef GROUP_IS_OPTIONAL
  if (NULL != ngroup){
#else //GROUP_IS_OPTIONAL
  if (1 == 1){
#endif //GROUP_IS_OPTIONAL
    if (NULL != nkey){
      if (SIDEX_SUCCESS != sidexcom_IsValidKey(nkey)){
        iRet = SIDEX_ERR_WRONG_KEY;
      }
      else{
        if (NULL != ngroup){
          if (SIDEX_SUCCESS != sidexcom_IsValidKey(ngroup)){
            iRet = SIDEX_ERR_WRONG_GROUP;
          }
          else{
            // Behandlung mit nGroup:
            iRet = sidexcom_GetChildByName(&nNode, getRootnode(), ngroup);
            if (SIDEX_SUCCESS == iRet){
              int iInternalCount;
              iRet = sidexcom_GetChildCount(&iInternalCount, nNode);
              if (SIDEX_SUCCESS == iRet){
                if (1 <= iInternalCount){
                  SIDEX_NODE nInternalNode;
                  iRet = sidexcom_GetChildByName(&nInternalNode, nNode, nkey);
                  if (SIDEX_SUCCESS == iRet){
                    if (NULL != nInternalNode){
                      iRet = sidexcom_StringFromNodeLength(nInternalNode, iStrLength, expectedType, iUnicode);
                    }
                  }
                }
              }
            }
          }
        }
        else{
          // Behandlung ohne nGroup:
          iRet = sidexcom_GetChildByName(&nNode, getRootnode(), nkey);
          if (SIDEX_SUCCESS == iRet){
            iRet = sidexcom_StringFromNodeLength(nNode, iStrLength, expectedType, iUnicode);
          }
        }
      }
    }
    else{
      iRet = SIDEX_ERR_MISSING_KEY;
    }
  }
  else{
    iRet = SIDEX_ERR_MISSING_GROUP;
  }
  return iRet;
}


/**
 * @brief   Reads content of provided node, interpreting it as the content of sidexList.
 */
int sidexCom::sidexcom_ReadList(SIDEX_NODE node, sidexList* sList)
{
   int iRet = SIDEX_SUCCESS;
   if ( sidexcom_GetNodeType(node) != SIDEX_DATA_TYPE_LIST ) 
      iRet = SIDEX_ERR_WRONG_TYPE;
   else{
     axlNode *item = axl_node_get_first_child((axlNode *)node);
     while (NULL != item && SIDEX_SUCCESS == iRet) 
     {	
       SIDEX_VARIANT v;
       iRet = sidexcom_ReadVariant(item, &v);
       if (SIDEX_SUCCESS == iRet){
         ((sidexBase*)v)->decRef();
          sList->addElement(v);
          item = axl_node_get_next(item);   
       }
     }
   }
   return iRet;
}


/**
 * @brief   Reads content of provided node, interpreting it as the content of sidexDict.
 */
int sidexCom::sidexcom_ReadDict(SIDEX_NODE node, sidexDict* sDict)
{
  int iRet = SIDEX_SUCCESS;
  if ( sidexcom_GetNodeType(node) != SIDEX_DATA_TYPE_DICT ) 
    iRet = SIDEX_ERR_WRONG_TYPE;
  else{
    sidexList* tempDictKey = new sidexList();
    sidexList* tempDictValue = new sidexList();
    axlNode* item = axl_node_get_first_child((axlNode *)node);
    int iIndex = 0;
    while (NULL != item && SIDEX_SUCCESS == iRet) 
    {
      char* itemName;

      axlNode *itemContent = axl_node_get_first_child((axlNode *)item);

      itemName = (char *) axl_node_get_name(item);  	
      SIDEX_VARIANT vKey;
      iRet = sidexcom_ReadVariant(itemContent, &vKey);
      if (SIDEX_SUCCESS == iRet){
        if ((iRet = tempDictKey->addElement(vKey, iIndex)) == SIDEX_SUCCESS){
          sidexutil_Variant_DecRef(vKey);
          axlNode *itemContent2 = axl_node_get_next(itemContent);   
          SIDEX_VARIANT vValue;
          iRet = sidexcom_ReadVariant(itemContent2, &vValue);
          if (SIDEX_SUCCESS == iRet){
            if ((iRet = tempDictValue->addElement(vValue, iIndex)) == SIDEX_SUCCESS){
              sidexutil_Variant_DecRef(vValue);
              item = axl_node_get_next(item); 
              ++iIndex;
            }
          }
        }
      }
    }

    if (SIDEX_SUCCESS == iRet){
      int iLength = tempDictKey->length();

      int iIndex = 0;
      for (int i = 0; i < iLength  && SIDEX_SUCCESS == iRet; ++i){

        SIDEX_VARIANT vKey = SIDEX_HANDLE_TYPE_NULL;
        SIDEX_VARIANT vValue = SIDEX_HANDLE_TYPE_NULL;
        iRet = tempDictKey->getElement(&vKey, iIndex);
        if (SIDEX_SUCCESS == iRet){
          iRet = tempDictValue->getElement(&vValue, iIndex);
          if (SIDEX_SUCCESS == iRet){
            ++iIndex;
            char* sKey;
            SIDEX_INT32 iLength;
            iRet = sidex_Variant_As_String (vKey, &sKey, &iLength);
            if (SIDEX_SUCCESS == iRet){
              iRet = sDict->setValue(sKey, vValue);
            }
          }
        }
      }
    }
    delete tempDictKey;
    delete tempDictValue;
  }
  if (SIDEX_SUCCESS != iRet){
    iRet = SIDEX_ERR_WRONG_DICT_SYNTAX;
  }
  return iRet;
}


/**
 * @brief   Reads content of provided node, interpreting it as the content of sidexTable.
 */
int sidexCom::sidexcom_ReadTable(SIDEX_NODE node, sidexTable* sTable)
{
   if ( sidexcom_GetNodeType(node) != SIDEX_DATA_TYPE_TABLE ) 
      return SIDEX_ERR_WRONG_TYPE;
   
   int iRet = SIDEX_SUCCESS;
   int iColSize = 0;

   //////////////////////////////////////////////////////////////////
   // Get the Column names:
   axlNode *columnsNode = axl_node_get_first_child((axlNode *)node); 
   if (NULL != columnsNode){
     sidexList* ColumnsList = NULL;
     iRet = sidexcom_ListFromNode(columnsNode, &ColumnsList);
     if (SIDEX_SUCCESS == iRet){
       iColSize = ColumnsList->length();
       for (int i = 0; i < iColSize && SIDEX_SUCCESS == iRet; ++i){
         SIDEX_VARIANT varCol;
         iRet = sidexutilList_Get((SIDEX_VARIANT)ColumnsList, i, &varCol);
         if (SIDEX_SUCCESS == iRet){
           if (SIDEX_TRUE == sidexutil_dataTypeCheck(varCol, SIDEX_DATA_TYPE_STRING)){
             char* sColumn;
             SIDEX_INT32 iLength;
             iRet = sidexutilString_Value_A(varCol, &sColumn, &iLength);
             if (SIDEX_SUCCESS == iRet){
               sTable->addColumn(sColumn);
             }
           }
           else{
             iRet = SIDEX_ERR_WRONG_TYPE;
           }
         }
       }
     }
     if (NULL != ColumnsList){
        sidexutil_Variant_DecRef((SIDEX_VARIANT)ColumnsList);
     }
   }
   //////////////////////////////////////////////////////////////////
   // Get the Rows:
   axlNode *row = axl_node_get_next(columnsNode); // first Row
   int iRow = 0;
   while (NULL != row && SIDEX_SUCCESS == iRet) 
   {
      sidexDict* dict = NULL;
      iRet = sidexcom_DictFromNode(row, &dict);
      if (SIDEX_SUCCESS == iRet){
        sTable->addRow();

        char** sKeys;
        iRet = dict->getKeys(&sKeys);
        for (int i = 0; i < iColSize && SIDEX_SUCCESS == iRet;++i){
          SIDEX_VARIANT vCell;
          iRet = dict->getValue(sKeys[i], &vCell);
          if (SIDEX_SUCCESS == iRet){
            iRet =sidexutilTable_SetField((SIDEX_VARIANT)sTable, iRow, sKeys[i], vCell);
          }
        }
        delete[]sKeys;
        
        row = axl_node_get_next(row);  // next Row
        ++iRow;
      }
      if (NULL != dict){
        sidexutil_Variant_DecRef((SIDEX_VARIANT)dict);
      }
   }
   return iRet;
}


/**
 * @brief   Sets SIDEX_VARIANT value as content of the ngroup and nkey.
 */
int sidexCom::sidexcom_WriteVariant(char* ngroup, char* nkey, SIDEX_VARIANT variant)
{
   int iRet = SIDEX_SUCCESS;
#ifndef GROUP_IS_OPTIONAL
   if (NULL != ngroup){
#else //GROUP_IS_OPTIONAL
   if (1 == 1){
#endif //GROUP_IS_OPTIONAL
     if (NULL != nkey){
      if (SIDEX_HANDLE_TYPE_NULL == variant){
        iRet = SIDEX_ERR_WRONG_PARAMETER;
      }
      else{
         int iType =  ((sidexBase*) variant)->getType();
         switch (iType)
         {
            case SIDEX_DATA_TYPE_NONE:
               iRet = sidexcom_WriteNone(ngroup, nkey);
               break;
            case SIDEX_DATA_TYPE_BOOLEAN:
               iRet = sidexcom_WriteBoolean(ngroup, nkey, ((sidexBoolean*) variant)->getValue());
               break;
            case SIDEX_DATA_TYPE_INTEGER:
               iRet = sidexcom_WriteInteger(ngroup, nkey, ((sidexInteger*) variant)->getValue());
               break;
            case SIDEX_DATA_TYPE_FLOAT:
               iRet = sidexcom_WriteFloat(ngroup, nkey, ((sidexFloat*) variant)->getValue());
               break;
            case SIDEX_DATA_TYPE_DATETIME:
               iRet = sidexcom_WriteDateTime(ngroup, nkey, (SIDEX_VARIANT)variant);
               break;
            case SIDEX_DATA_TYPE_STRING:
            case SIDEX_DATA_TYPE_BINARY:
               iRet = sidexcom_WriteString(ngroup, nkey, ((sidexString*) variant)->getValue_A(), ((sidexString*) variant)->getSize(), 
                                                                                                            ((sidexString*) variant)->getEncoding(),
                                                                                                            ((sidexString*) variant)->getStringFormat());
               break;
            case SIDEX_DATA_TYPE_LIST:
               iRet = sidexcom_WriteList(ngroup, nkey, (sidexList*) variant);
               break;
            case SIDEX_DATA_TYPE_DICT:
               iRet = sidexcom_WriteDict(ngroup, nkey, (sidexDict*) variant);
               break;
            case SIDEX_DATA_TYPE_TABLE:
               iRet = sidexcom_WriteTable(ngroup, nkey, (sidexTable*) variant);
               break;
            case SIDEX_DATA_TYPE_UNKNOWN:
            default:
               iRet = SIDEX_ERR_WRONG_TYPE;
               break;
         }
      }
    }
    else{
      iRet = SIDEX_ERR_MISSING_KEY;
    }
  }
  else{
    iRet = SIDEX_ERR_MISSING_GROUP;
  }
  return iRet;
}


/**
 * @brief   Sets integer value as content of the ngroup and nkey.
 */
int sidexCom::sidexcom_WriteInteger(char* ngroup, char* nkey, SIDEX_INT64 value)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   if (SIDEX_SUCCESS == iRet){
     sidexcom_WriteInteger(node, value);
   }
   return iRet;
}


/**
 * @brief   Sets none value (empty value) as content of the ngroup and nkey.
 */
int sidexCom::sidexcom_WriteNone(char* ngroup, char* nkey)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   if (SIDEX_SUCCESS == iRet){
     sidexcom_WriteNone(node);
   }
   return iRet;
}


/**
 * @brief   Sets SIDEX_BOOL value as content of the ngroup and nkey.
 */
int sidexCom::sidexcom_WriteBoolean(char* ngroup, char* nkey, SIDEX_BOOL value)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   if (SIDEX_SUCCESS == iRet){
     sidexcom_WriteBoolean(node, value);
   }
   return iRet;
}


/**
 * @brief   Sets float value as content of the ngroup and nkey.
 */
int sidexCom::sidexcom_WriteFloat(char* ngroup, char* nkey, double value)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   if (SIDEX_SUCCESS == iRet){
     sidexcom_WriteFloat(node, value);
   }
   return iRet;
}


/**
 * @brief   Sets DateTime value as content of the ngroup and nkey.
 */
int sidexCom::sidexcom_WriteDateTime(char* ngroup, char* nkey, SIDEX_VARIANT value)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   if (SIDEX_SUCCESS == iRet){
     sidexcom_WriteDateTime(node, value);
   }
   return iRet;
}



/**
 * @brief   Sets a char string as content of the ngroup and nkey.
 */
int sidexCom::sidexcom_WriteString(char* ngroup, char* nkey, const char *value, int size, SIDEX_ENCODE encoding, SIDEX_STRING_ENCODING_TYPE format)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   
   if (NULL == value  && 0 != size){
     iRet = SIDEX_ERR_WRONG_PARAMETER;
   }
   else{
     if (-1 == size){
       ////////////////////////////
       // Use strlen in case of -1:
       size = (int) strlen(value);
     }
   }

   
   if (SIDEX_SUCCESS == iRet){
     iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   }
   if (SIDEX_SUCCESS == iRet){
     // I have to remember object for later deallocation:
     const char* content;
     const char* type = SIDEX_TYPE_ATTR_STRING;
     switch (encoding){
       case SIDEX_ENCODE_BASE64:
         content = sidexutil_encodeBase64(value, &size);
         type = SIDEX_TYPE_ATTR_BINARY;
         break;
       case SIDEX_ENCODE_NONE: 
       case SIDEX_ENCODE_CDATA:
       default:
         content = value;
         break;
     }
     sidexcom_SetNodeEncodedContent(
        node, 
        content, 
        (encoding == SIDEX_ENCODE_BASE64) ? -1 : size, 
        type, 
        encoding, 
        format
     );
     // In case of SIDEX_ENCODE_BASE64 I have to deallocate memory:
     if (SIDEX_ENCODE_BASE64 == encoding)
       delete[] content;
   }
   return iRet;
}


/**
 * @brief   Sets sidexList value as content of the ngroup and nkey. (Subkeys will be created)
 */
int sidexCom::sidexcom_WriteList(char* ngroup, char* nkey, sidexList* sList)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   if (NULL == sList){
     iRet = SIDEX_ERR_WRONG_PARAMETER;
   }
   if (SIDEX_SUCCESS == iRet){
     iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   }
   if (SIDEX_SUCCESS == iRet){
     if (bGroupMatch && bKeyMatch){
        // overwrite a 'list' node can only be done by removing the existing one:
        axl_node_remove ((axlNode*)node, axl_true);
        // and try once more:
        iRet = sidexcom_WriteList(ngroup, nkey, sList);
     }
     else{
       iRet = sidexcom_WriteListTree(node, sList);
     }
   }
   return iRet;
}


/**
 * @brief   Sets sidexDict value as content of the ngroup and nkey. (Subkeys will be created)
 */
int sidexCom::sidexcom_WriteDict(char* ngroup, char* nkey, sidexDict* sDict)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   if (NULL == sDict){
     iRet = SIDEX_ERR_WRONG_PARAMETER;
   }
   if (SIDEX_SUCCESS == iRet){
     iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   }
   if (SIDEX_SUCCESS == iRet){
     if (bGroupMatch && bKeyMatch){
        // overwrite a 'dict' node can only be done by removing the existing one:
        axl_node_remove ((axlNode*)node, axl_true);
        // and try once more:
        iRet = sidexcom_WriteDict(ngroup, nkey, sDict);
     }
     else{
       iRet = sidexcom_WriteDictTree(node, sDict);
     }
   }
   return iRet;
}


/**
     * @brief   Sets sidexTable value as content of the ngroup and nkey. (Subkeys will be created)
 */
int sidexCom::sidexcom_WriteTable(char* ngroup, char* nkey, sidexTable* sTable)
{
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE node;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   if (NULL == sTable){
     iRet = SIDEX_ERR_WRONG_PARAMETER;
   }
   if (SIDEX_SUCCESS == iRet){
     iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, true, &bKeyMatch, &bGroupMatch, &node);
   }
   if (SIDEX_SUCCESS == iRet){
     if (bGroupMatch && bKeyMatch){
        // overwrite a 'table' node can only be done by removing the existing one:
        axl_node_remove ((axlNode*)node, axl_true);
        // and try once more:
        iRet = sidexcom_WriteTable(ngroup, nkey, sTable);
     }
     else{
       sidexcom_WriteTableTree(node, sTable);
     }
   }
   return iRet;
}


/**
 * @brief   Reads content of provided handles node, interpreting it as SIDEX_VARIANT.
 */
int sidexCom::sidexcom_ReadVariant(char* ngroup, char* nkey, SIDEX_VARIANT* variant){
  SIDEX_NODE nNode;
  *variant = SIDEX_HANDLE_TYPE_NULL;
  int iRet = SIDEX_SUCCESS;

#ifndef GROUP_IS_OPTIONAL
  if (NULL != ngroup){
#else //GROUP_IS_OPTIONAL
  if (1 == 1){
#endif //GROUP_IS_OPTIONAL
    if (NULL != nkey){
      if (SIDEX_SUCCESS != sidexcom_IsValidKey(nkey)){
        iRet = SIDEX_ERR_WRONG_KEY;
      }
      else{
        if (NULL != ngroup){
          if (SIDEX_SUCCESS != sidexcom_IsValidKey(ngroup)){
            iRet = SIDEX_ERR_WRONG_GROUP;
          }
          else{
            // Behandlung mit nGroup:
            iRet = sidexcom_GetChildByName(&nNode, getRootnode(), ngroup);
            if (SIDEX_SUCCESS == iRet){
              int iInternalCount;
              iRet = sidexcom_GetChildCount(&iInternalCount, nNode);
              if (SIDEX_SUCCESS == iRet){
                if (1 <= iInternalCount){
                  SIDEX_NODE nInternalNode;
                  iRet = sidexcom_GetChildByName(&nInternalNode, nNode, nkey);
                  if (SIDEX_SUCCESS == iRet){
                    iRet = sidexcom_ReadVariant(nInternalNode, variant);
                  }
                }
              }
            }
          }
        }
        else{
          // Behandlung ohne nGroup:
          iRet = sidexcom_GetChildByName(&nNode, getRootnode(), nkey);
          if (SIDEX_SUCCESS == iRet){
            iRet = sidexcom_ReadVariant(nNode, variant);
          }
        }
      }
    }
    else{
      iRet = SIDEX_ERR_MISSING_KEY;
    }
  }
  else{
    iRet = SIDEX_ERR_MISSING_GROUP;
  }
  if (SIDEX_HANDLE_TYPE_NULL == *variant && SIDEX_SUCCESS == iRet){
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief   Allows to create a new group/child inside the parent group.
 */
int sidexCom::sidexcom_CreateNode(SIDEX_NODE parent, const char* nNode, SIDEX_NODE* newNode)
{
   int iRet = SIDEX_SUCCESS;
   axlNode *node = axl_node_create(nNode); 
   axl_node_set_child((axlNode *) parent, node);
   *newNode = node;
   return iRet;
}


/**
 * @brief   Allows to get the nodes child depending of it's name.
 */
int sidexCom::sidexcom_GetChildByName(SIDEX_NODE* retNode, SIDEX_NODE node, char* name) 
{
   int iRet = SIDEX_SUCCESS;
   axlNode* get_node = axl_node_get_child_called((axlNode *)node, name);
   if (NULL == get_node)
     iRet = SIDEX_ERR_NOCONTENT;
   else
     *retNode = get_node;
   return iRet;
}


/**
 * @brief   Allows to get the number of childs of the provided node.
 */
int sidexCom::sidexcom_GetChildCount(int* iCount, SIDEX_NODE node)
{
   *iCount = axl_node_get_child_num((axlNode *)node);
   if (-1 == *iCount)
     return  SIDEX_ERR_NOCONTENT;
   else
     return  SIDEX_SUCCESS;
}


/**
 * @brief   Allows to get the childs name of the node at the index.
 */
int sidexCom::sidexcom_GetNodeNameByIndex(char** retName, SIDEX_NODE node, int index)
{
   int iRet = SIDEX_SUCCESS;
   axlNode* get_node = axl_node_get_child_nth((axlNode *)node, index); 	
   if (NULL != get_node)
      *retName = (char*) axl_node_get_name(get_node);
   else
     iRet = SIDEX_ERR_NOCONTENT;
   return iRet;
}


/**
 * @brief   Allows to get the child of the node at the index.
 */
int sidexCom::sidexcom_GetChildByIndex(SIDEX_NODE* retNode, SIDEX_NODE node, int index) 
{
   axlNode *get_node = axl_node_get_child_nth((axlNode *)node, index); 	
   *retNode = get_node;
   if (NULL == *retNode)
     return SIDEX_ERR_NOCONTENT;
   else
     return SIDEX_SUCCESS;
}


/**
 * @brief   Allows to remove a node inside the parent group.
 */
void sidexCom::sidexcom_DeleteNode(SIDEX_NODE parent)
{
   axl_node_deattach((axlNode *)parent);
   axl_node_free((axlNode *)parent);
}


/**
 * @brief   Get the node for a group and a key. If it don't exist it will be created.
 */
int sidexCom::sidexcom_GetNodeForGroupAndKey(char* ngroup, char* nkey, bool bCreate, bool* bKeyMatch, bool *bGroupMatch, SIDEX_NODE* node)
{
  int iRet = SIDEX_SUCCESS;

  SIDEX_NODE childNode = NULL;

#ifndef GROUP_IS_OPTIONAL
  if (NULL != ngroup){
#else //GROUP_IS_OPTIONAL
  if (1 == 1){
#endif //GROUP_IS_OPTIONAL
    if (NULL != nkey){
      if (SIDEX_SUCCESS != sidexcom_IsValidKey(nkey)){
        iRet = SIDEX_ERR_WRONG_KEY;
      }
      else{
        if (NULL != ngroup){
          if (SIDEX_SUCCESS != sidexcom_IsValidKey(ngroup)){
            iRet = SIDEX_ERR_WRONG_GROUP;
          }
          else{
            iRet = sidexcom_GetNodeForChildNodeName(getRootnode(), ngroup, bCreate, bGroupMatch, &childNode);
            if (NULL != childNode && SIDEX_SUCCESS == iRet){
              iRet = sidexcom_GetNodeForChildNodeName(childNode, nkey, bCreate, bKeyMatch, &childNode);
            }
            if (SIDEX_SUCCESS == iRet){
              *node = childNode;
            }
          }
        }
        else{
          // Behandlung ohne nGroup:
          *bGroupMatch = true;
          iRet = sidexcom_GetNodeForChildNodeName(getRootnode(), nkey, bCreate, bKeyMatch, &childNode);
          if (SIDEX_SUCCESS == iRet){
            *node = childNode;
          }
        }
      }
    }
    else{
      iRet = SIDEX_ERR_MISSING_KEY;
    }
  }
  else{
    iRet = SIDEX_ERR_MISSING_GROUP;
  }
  return iRet;
}


/**
 * @brief   Returns the group list of the rootnode.
 */
int sidexCom::sidexcom_GetGroups(SIDEX_VARIANT* variant){

  int iRet = SIDEX_SUCCESS;
  sidexList* list = NULL;

  list = (sidexList*) sidex_Variant_New_List();
  // get the first child 
  axlNode* group = axl_node_get_first_child ((axlNode*) getRootnode());
 
  // iterate over all nodes 
  while (NULL != group && SIDEX_SUCCESS == iRet) {
    const char * sAttrName = axl_node_get_attribute_value(group, SIDEX_TYPE_ATTR);
    if (NULL == sAttrName){
      // Now we have a definitely a group:
      char* sName = (char*) axl_node_get_name(group);
      SIDEX_VARIANT groupItem;
      iRet = sidex_Variant_New_String(sName, &groupItem);
      if (SIDEX_SUCCESS == iRet){
        list->addElement(groupItem);
        // Decrement of the refcounter:
        ((sidexBase*)groupItem)->decRef();
      }
    }
    group = axl_node_get_next(group);
  }
  *variant = (SIDEX_VARIANT)list;
  return iRet;
}

    
/**
 * @brief   Check for the existence of a group.
 */
SIDEX_BOOL sidexCom::sidexcom_HasGroup(char* ngroup){
  SIDEX_INT32 iGroupSize = 0;
  bool bFound = false;

  SIDEX_VARIANT vGroups = SIDEX_HANDLE_TYPE_NULL;
  ////////////////////////////////////////////
  // Check for parameter plausibility:
  if (NULL != ngroup && strlen (ngroup) != 0){
    ////////////////////////////////////////////
    // Get all groups:
    int iRet = sidexcom_GetGroups(&vGroups);
    if (SIDEX_SUCCESS == iRet){
      ////////////////////////////////////////////
      // Check for list plausibility:
      if (SIDEX_TRUE == sidexutil_dataTypeCheck(vGroups, SIDEX_DATA_TYPE_LIST)){
        ////////////////////////////////////////////
        // list size:
        iRet = sidexutilList_Size(vGroups, &iGroupSize);
      }
    }
    for (int i = 0; (SIDEX_SUCCESS == iRet) && !bFound &&(i < iGroupSize); ++i){
      SIDEX_VARIANT vAGroup = SIDEX_HANDLE_TYPE_NULL;
      iRet = sidexutilList_Get(vGroups, i, &vAGroup);
      if (SIDEX_SUCCESS == iRet){
        ////////////////////////////////////////////
        // Check for list plausibility:
        if (SIDEX_TRUE == sidexutil_dataTypeCheck(vAGroup, SIDEX_DATA_TYPE_STRING)){
          char* sValue = NULL;
          SIDEX_INT32 iLength = 0;
          iRet = sidexutilString_Value_A(vAGroup, &sValue, &iLength);
          if (SIDEX_SUCCESS == iRet){
            ////////////////////////////////////////////
            // Check for list plausibility:
            if (0 == strcmp(ngroup, sValue)){
              bFound = true;
            }
          }
        }
      }
    }
    if (SIDEX_HANDLE_TYPE_NULL != vGroups){
      ////////////////////////////////////////////
      // Free all groups:
      sidexutil_Variant_DecRef(vGroups);
    }
  }
  if (bFound){
    return SIDEX_TRUE;
  }
  else{
    return SIDEX_FALSE;
  }
}


/**
 * @brief   Returns the key list of the rootnode for the requested group.
 */
int sidexCom::sidexcom_GetKeys(char* ngroup, SIDEX_VARIANT* variant){
  int iRet = SIDEX_SUCCESS;
  sidexList* list = NULL;
  axlNode* key;

  if (NULL != ngroup){
    // ngroup exists:
    if (SIDEX_SUCCESS != sidexcom_IsValidKey(ngroup)){
      iRet = SIDEX_ERR_WRONG_GROUP;
    }
    else{
      list = (sidexList*) sidex_Variant_New_List();
      //  ngroup exists and syntax is ok:
      SIDEX_NODE subNode = (SIDEX_NODE) axl_node_get_child_called ((axlNode*) getRootnode(), ngroup);
      if (NULL != subNode){
        // get the first child 
        key = axl_node_get_first_child ((axlNode*) subNode);
         
        // iterate over all nodes 
        while (NULL != key && SIDEX_SUCCESS == iRet) {
          char* sName = (char*) axl_node_get_name(key);
          SIDEX_VARIANT keyItem;
          iRet = sidex_Variant_New_String(sName, &keyItem);
          if (SIDEX_SUCCESS == iRet){
            list->addElement(keyItem);
            // Decrement of the refcounter:
            ((sidexBase*)keyItem)->decRef();
          }
          key = axl_node_get_next(key);
        }
      }
      *variant = (SIDEX_VARIANT)list;
    }
  }
  else{
#ifndef GROUP_IS_OPTIONAL
    iRet = SIDEX_ERR_MISSING_GROUP;
#else //GROUP_IS_OPTIONAL
    list = (sidexList*) sidex_Variant_New_List();
    // ngroup doesn't exist / I have to find the singles:
    // get the first child 
    key = axl_node_get_first_child ((axlNode*) getRootnode());
   
    // iterate over all nodes 
    while (NULL != key && SIDEX_SUCCESS == iRet) {
      const char * sAttrName = axl_node_get_attribute_value(key, SIDEX_TYPE_ATTR);
      if (NULL != sAttrName){
        // Now we have a definitely a single key:
        char* sName = (char*) axl_node_get_name(key);
        SIDEX_VARIANT keyItem;
        iRet = sidex_Variant_New_String(sName, strlen(sName), SIDEX_ENCODE_NONE, &keyItem);
        if (SIDEX_SUCCESS == iRet){
          list->addElement(keyItem);
          // Decrement of the refcounter:
          ((sidexBase*)keyItem)->decRef();
        }
      }
      key = axl_node_get_next(key);
    }
    *variant = list;
#endif //GROUP_IS_OPTIONAL
  }
  return iRet;
}


/**
* @brief   Check for the existence of a key in a group.
*/
SIDEX_BOOL sidexCom::sidexcom_HasKey(char* ngroup, char* nkey){
  SIDEX_INT32 iKeysSize = 0;
  bool bFound = false;
  SIDEX_VARIANT vKeys = SIDEX_HANDLE_TYPE_NULL;
 
#ifndef GROUP_IS_OPTIONAL
  if (NULL != ngroup && strlen (ngroup) != 0)
#else //GROUP_IS_OPTIONAL
  if (1 == 1){
#endif //GROUP_IS_OPTIONAL
  {
    if (NULL != nkey && strlen (nkey) != 0)
    {
      ////////////////////////////////////////////
      // Get keys for group:
      int iRet = sidexcom_GetKeys(ngroup, &vKeys);
      if (SIDEX_SUCCESS == iRet){
        ////////////////////////////////////////////
        // Check for list plausibility:
        if (SIDEX_TRUE == sidexutil_dataTypeCheck(vKeys, SIDEX_DATA_TYPE_LIST)){
          ////////////////////////////////////////////
          // list size:
          iRet = sidexutilList_Size(vKeys, &iKeysSize);
        }
      }
      for (int i = 0; (SIDEX_SUCCESS == iRet) && !bFound &&(i < iKeysSize); ++i){
        SIDEX_VARIANT vAKey = SIDEX_HANDLE_TYPE_NULL;
        iRet = sidexutilList_Get(vKeys, i, &vAKey);
        if (SIDEX_SUCCESS == iRet){
          ////////////////////////////////////////////
          // Check for list plausibility:
          if (SIDEX_TRUE == sidexutil_dataTypeCheck(vAKey, SIDEX_DATA_TYPE_STRING)){
            char* sValue = NULL;
            SIDEX_INT32 iLength = 0;
            iRet = sidexutilString_Value_A(vAKey, &sValue, &iLength);
            if (SIDEX_SUCCESS == iRet){
              ////////////////////////////////////////////
              // Check for list plausibility:
              if (0 == strcmp(nkey, sValue)){
                bFound = true;
              }
            }
          }
        }
      }
      if (SIDEX_HANDLE_TYPE_NULL != vKeys){
        ////////////////////////////////////////////
        // Free all Keys:
        sidexutil_Variant_DecRef(vKeys);
      }
    }
  }
  if (bFound){
    return SIDEX_TRUE;
  }
  else{
    return SIDEX_FALSE;
  }
}


/**
 * @brief   Remove the group and it's content of the rootnode.
 */
int sidexCom::sidexcom_DeleteGroup(char* ngroup){
  int iRet = SIDEX_SUCCESS;

  if (NULL != ngroup){
    // ngroup exists:
    if (SIDEX_SUCCESS != sidexcom_IsValidKey(ngroup)){
      iRet = SIDEX_ERR_WRONG_GROUP;
    }
    else{
      //  ngroup exists and syntax is ok:
      SIDEX_NODE subNode = (SIDEX_NODE) axl_node_get_child_called ((axlNode*) getRootnode(), ngroup);
      if (NULL == subNode){
        iRet = SIDEX_ERR_NOCONTENT;
      }
      else{
        sidexcom_DeleteNode(subNode);
      }
    }
  }
  else{
    iRet = SIDEX_ERR_WRONG_GROUP;
  }
  return iRet;
}


/**
 * @brief   Remove the key of a group of the rootnode.
 */
int sidexCom::sidexcom_DeleteKey(char* ngroup, char* nkey){
   int iRet = SIDEX_SUCCESS;
   SIDEX_NODE subNode = NULL;
   bool bKeyMatch = false;
   bool bGroupMatch = false;

   iRet = sidexcom_GetNodeForGroupAndKey(ngroup, nkey, false, &bKeyMatch, &bGroupMatch, &subNode);
   if (SIDEX_SUCCESS == iRet){
     if (bKeyMatch && bGroupMatch){
       if (NULL != subNode){
         sidexcom_DeleteNode(subNode);
       }
       else{
         iRet = SIDEX_ERR_NOCONTENT;
       }
     }
     else{
       iRet = SIDEX_ERR_NOCONTENT;
     }
   }
   return iRet;
}


/**
 * @brief   Check sKey content for valid XML name convention
 */
int sidexCom::sidexcom_IsValidKey(char* sKey){
 
  int iRet = SIDEX_SUCCESS;
  int iAddedSize;
  if (NULL != sKey){
    if (NULL != strchr(sKey, ' ') || NULL != strchr(sKey, '\t') || 0 == strlen(sKey) || axl_node_has_invalid_chars(sKey, (int)strlen(sKey), &iAddedSize)){
      iRet = SIDEX_ERR_WRONG_KEY;
    }
  }
  else{
    iRet = SIDEX_ERR_MISSING_KEY;
  }
  return iRet;
}

SIDEX_INT32 sidexCom::sidexcom_Merge(SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, char* nGroup, char* nKey){
 
  int iRet = SIDEX_SUCCESS;

  SIDEX_VARIANT sMergeGroups = SIDEX_HANDLE_TYPE_NULL;
  // Fetch the "Merge" groups:
  iRet = sidex_GetGroups(sMergeHandle, &sMergeGroups);
  if (SIDEX_SUCCESS == iRet && SIDEX_HANDLE_TYPE_NULL != sMergeGroups){
    // Loop above groups:
    SIDEX_INT32 iSizeGroups = 0;
    iRet = sidex_Variant_List_Size (sMergeGroups, &iSizeGroups);
    for (SIDEX_INT32 i = 0; i < iSizeGroups && SIDEX_SUCCESS == iRet; ++i){
      SIDEX_VARIANT vGroup = SIDEX_HANDLE_TYPE_NULL;
      iRet = sidex_Variant_List_Get (sMergeGroups, i, &vGroup);
      if (SIDEX_SUCCESS == iRet){
        char* sGroup;
        SIDEX_INT32 iGroupValLength;
        iRet = sidex_Variant_As_String_A (vGroup, &sGroup, &iGroupValLength);
        if (SIDEX_SUCCESS == iRet){
          ///////////////////////////////////////////////////////////////
          ///////////////////////////////////////////////////////////////
          // Maybe overwrite a key:

          if (SIDEX_TRUE == sidex_HasGroup ((SIDEX_HANDLE)this, sGroup)){
            // The Group exists in "Base"
            if (bOverwrite){
              if (0 == strcmp(sGroup, nGroup)){
                // It is the group we are looking for / including the possible overwriting key:
                if (SIDEX_HANDLE_TYPE_NULL == nKey){
                  // Overwrite the whole Group:

                  // First delete the Group:
                  iRet = sidex_DeleteGroup ((SIDEX_HANDLE)this, sGroup);
                  if (SIDEX_SUCCESS == iRet){
                    // Now copy the content:
                    SIDEX_VARIANT sMergeKeys = SIDEX_HANDLE_TYPE_NULL;
                    iRet = sidex_GetKeys(sMergeHandle, sGroup, &sMergeKeys);
                    if (SIDEX_SUCCESS == iRet && SIDEX_HANDLE_TYPE_NULL != sMergeKeys){
                      // Loop above keys:
                      SIDEX_INT32 iSizeKeys = 0;
                      iRet = sidex_Variant_List_Size (sMergeKeys, &iSizeKeys);
                      for (SIDEX_INT32 j = 0; j < iSizeKeys && SIDEX_SUCCESS == iRet; ++j){
                        SIDEX_VARIANT vKey = SIDEX_HANDLE_TYPE_NULL;
                        iRet = sidex_Variant_List_Get (sMergeKeys, j, &vKey);
                        if (SIDEX_SUCCESS == iRet){
                          char* sKey;
                          SIDEX_INT32 iKeyValLength;
                          iRet = sidex_Variant_As_String_A (vKey, &sKey, &iKeyValLength);
                          SIDEX_VARIANT variant;
                          iRet = sidex_Variant_Read (sMergeHandle, sGroup, sKey, &variant);
                          if (SIDEX_SUCCESS == iRet){
                            iRet = sidex_Variant_Write((SIDEX_HANDLE)this, sGroup, sKey, variant);
                            sidex_Variant_DecRef(variant);
                          }
                        }
                      }
                      sidex_Variant_DecRef(sMergeKeys);
                    }
                  }
                }
                else{
                  // Check if the key exists in the "Merge":
                  if (sidex_HasKey (sMergeHandle, sGroup, nKey)){
                    // Fetch the Variant anf overwrite it in the Base:
                    SIDEX_VARIANT variant;
                    iRet = sidex_Variant_Read (sMergeHandle, sGroup, nKey, &variant);
                    if (SIDEX_SUCCESS == iRet){
                      iRet = sidex_Variant_Write((SIDEX_HANDLE)this, sGroup, nKey, variant);
                      sidex_Variant_DecRef(variant);
                    }
                  }
                }
              }
            }
            // End of overwrite a key
            ///////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////
          }
          else{
            ///////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////
            // The Group don't exist in the "base" / so I have to copy it:

            SIDEX_VARIANT sMergeKeys = SIDEX_HANDLE_TYPE_NULL;
            iRet = sidex_GetKeys(sMergeHandle, sGroup, &sMergeKeys);
            if (SIDEX_SUCCESS == iRet && SIDEX_HANDLE_TYPE_NULL != sMergeKeys){
              // Loop above keys:
              SIDEX_INT32 iSizeKeys = 0;
              iRet = sidex_Variant_List_Size (sMergeKeys, &iSizeKeys);
              for (SIDEX_INT32 j = 0; j < iSizeKeys && SIDEX_SUCCESS == iRet; ++j){
                SIDEX_VARIANT vKey = SIDEX_HANDLE_TYPE_NULL;
                iRet = sidex_Variant_List_Get (sMergeKeys, j, &vKey);
                if (SIDEX_SUCCESS == iRet){
                  char* sKey;
                  SIDEX_INT32 iKeyValLength;
                  iRet = sidex_Variant_As_String_A (vKey, &sKey, &iKeyValLength);
                  SIDEX_VARIANT variant;
                  iRet = sidex_Variant_Read (sMergeHandle, sGroup, sKey, &variant);
                  if (SIDEX_SUCCESS == iRet){
                    iRet = sidex_Variant_Write((SIDEX_HANDLE)this, sGroup, sKey, variant);
                    sidex_Variant_DecRef(variant);
                  }
                }
              }
              sidex_Variant_DecRef(sMergeKeys);
            }
            // The copy of the group is finished
            ///////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////
          }
        }
      }
    }
    sidex_Variant_DecRef(sMergeGroups);
  }


  return SIDEX_SUCCESS;
}


