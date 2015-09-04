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
 
/** @file sidexCom.h
  * 
  * @brief Class for communication with AXL library
  *
  */


#ifndef SIDEXCOM_H
#define SIDEXCOM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidex.h"
#include "sidexTypes.h"
#include <axl.h>

class sidexCom
{

#define DICT_ITEM_IDENTIFIER_DEF   "Item"
#define DICT_KEY_IDENTIFIER_DEF   "Key"
#define DICT_VALUE_IDENTIFIER_DEF "Value"

private:
    /* data */
    SIDEX_DOC m_document;
    SIDEX_NODE m_rootnode;
    char* m_strRepresentation;

    wchar_t*  m_utf32DocName;

    char16_t* m_utf16DocName;

protected: 
    /* data */

public:
    /* data */
    /* methods */

    /**
     * @brief    Constructor.
     *
     * @param    pname     Name of the XML root node.
     *
     * @returns an instance of sidexCom
     */
    explicit sidexCom(const char *pname);


    /**
     * @brief    Destructor.
     */
    ~sidexCom();


    /**
     * @brief    Change the document name / XML root node name.
     *
     * @param    pname   Name of the XML root node.
     *
     * @returns  SIDEX_SUCCESS in case of success.
     *
     */
    int sidexcom_Set_DocumentName(const char *pname);


    /**
     * @brief    Get the document name / XML root node name.
     *
     * @returns  SIDEX_SUCCESS in case of success.
     */
    const char* sidexcom_Get_DocumentName_A();


    /**
     * @brief    Get the document name / XML root node name.
     *
     * @returns  SIDEX_SUCCESS in case of success.
     */
    const wchar_t* sidexcom_Get_DocumentName_X();


    /**
     * @brief    Get the document name / XML root node name.
     *
     * @returns  SIDEX_SUCCESS in case of success.
     */
    const char16_t* sidexcom_Get_DocumentName_W();


    /**
     * @brief    Remove the XML tree content except of the root node.
     */
    void sidexcom_Clear();


/**
     * @brief    Set the document member.
     *
     * @param    document The instance of a SIDEX_DOC to set.
     *
     * @see sidexDefines.h
     */
    void setDocument(SIDEX_DOC document);


    /**
     * @brief    Get the document member content.
     *
     * @returns The Content of the document member.
     *
     * @see sidexDefines.h
     */
    SIDEX_DOC getDocument();


    /**
     * @brief    Set the rootnode member.
     *
     * @param    rootnode The instance of a SIDEX_NODE to set.
     *
     * @see sidexDefines.h
     */
    void setRootnode(SIDEX_NODE rootnode);


    /**
     * @brief    Get the rootnode member content.
     *
     * @returns The Content of the rootnode member.
     *
     * @see sidexDefines.h
     */
    SIDEX_NODE getRootnode();

        
    /**
     * @brief Saves axl content as an XML formatted document.
     *
     * @param path    The file path where the output will be placed. <br>
                      The function will require to have access rights to the file (or to create a new file if it doesn't exists). <br>
                      The default behaviour is to overwrite the file found if exists.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_WRONG_PARAMETER in case of a parameter problem,<br>
     *          SIDEX_ERR_DUMPCONTENT if an I/O error occured.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_DumpToFile(const char* path);

        
    /**
     * @brief Saves a SIDEX_HANDLE as an XML formatted char string.
     *
     * @param    content        Reference that will hold the dumped axl information.<br>
     *                          It has to be freed using sidex_Free_Content().
     * @param    iContentLength Reference to the length of the string
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_DUMPCONTENT if an I/O error occured.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_DumpToString(char **content, SIDEX_INT32* iContentLength); 


    /**
     * @brief Returns the amount of buffer size  internal allocated in sidexcom_DumpToString request.
     *
     * @param    iContentLength Reference to the length of the string
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_DUMPCONTENT if an I/O error occured.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_DumpToStringLength(SIDEX_INT32* iContentLength); 


    /**
     * @brief Opens an XML file, write content into SIDEX_HANDLE.
     *
     * @param path    Path to XML file.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_DUMPCONTENT if an I/O error occured.
     *
     * @see sidexErrors.h
     */
    int sidexcom_Load_Content(const char *path);


    /**
     * @brief Parse a char string, write content into SIDEX_HANDLE.
     *
     * @param content The char string.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_DUMPCONTENT if an I/O error occured.
     *
     * @see sidexErrors.h
     */
    int sidexcom_Set_Content(char *content);


    /**
    * @brief   Allows to get the content of a node.
    *
    * @param   node Node to get content from.
    *
    * @returns Content of the node.<br>
    *          Empty string if node has no content.
    *
    * @see sidexDefines.h
    */
    const char* sidexcom_GetNodeContent(SIDEX_NODE node);


    /**
    * @brief   Allows to get the content of a node (references like "&amp;" or "&quot;" will be translated).
    *
    * @param   node Node to get content from.
    *
    * @returns Content of the node.<br>
    *          Empty string if node has no content.
    *
    * @see sidexDefines.h
    */
    const char* sidexcom_GetNodeContentTrans(SIDEX_NODE node);


    /**
     * @brief   Allows to set the content of a node.
     *
     * @param   node    Node to get content from.
     * @param   content Content to be set.
     * @param   length  length of content string / if -1 it will be internal computed by strlen.
     * @param   type    Indicates type of content [SIDEX_TYPE_ATTR_XXX].<br>
     *                  Possible values:<br>
     *                  SIDEX_TYPE_ATTR_BOOLEAN<br>
     *                  SIDEX_TYPE_ATTR_INTEGER<br>
     *                  SIDEX_TYPE_ATTR_FLOAT<br>
     *                  SIDEX_TYPE_ATTR_BINARY<br>
     *                  SIDEX_TYPE_ATTR_STRING<br>
     *                  SIDEX_TYPE_ATTR_LIST<br>
     *                  SIDEX_TYPE_ATTR_DICT<br>
     *                  SIDEX_TYPE_ATTR_TABLE
     *
     * @see sidexDefines.h
     */
     void sidexcom_SetNodeContent(SIDEX_NODE node, const char* content, int length, const char *type);


    /**
     * @brief   Allows to set the content of a node.
     *
     * @param   node     Node to get content from.
     * @param   content  Content to be set.
     * @param   length   length of content string / if -1 it will be internal computed by strlen.
     * @param   type     Indicates type of content [SIDEX_TYPE_ATTR_XXX].<br>
     *                   Possible values:<br>
     *                   SIDEX_TYPE_ATTR_BOOLEAN<br>
     *                   SIDEX_TYPE_ATTR_INTEGER<br>
     *                   SIDEX_TYPE_ATTR_FLOAT<br>
     *                   SIDEX_TYPE_ATTR_BINARY<br>
     *                   SIDEX_TYPE_ATTR_STRING<br>
     *                   SIDEX_TYPE_ATTR_LIST<br>
     *                   SIDEX_TYPE_ATTR_DICT<br>
     *                   SIDEX_TYPE_ATTR_TABLE
     * @param   encoding 'SIDEX_ENCODE_NONE' (raw) or 'SIDEX_ENCODE_BASE64' (base64).
     * @param   format   The SIDEX_STRING_ENCODING_TYPE.
     *
     * @see sidexDefines.h
     */
     void sidexcom_SetNodeEncodedContent(SIDEX_NODE node, const char* content, int length, const char *type, SIDEX_ENCODE encoding, SIDEX_STRING_ENCODING_TYPE format);


    /**
     * @brief   Get SIDEX_DATA_TYPE of a node.
     *
     * @param   node Node to get type from.
     *
     * @returns The SIDEX_DATA_TYPE if the provided node.<br>
     *          Possible values:<br>
     *          SIDEX_DATA_TYPE_UNKNOWN<br>
     *          SIDEX_DATA_TYPE_BOOLEAN<br>
     *          SIDEX_DATA_TYPE_INTEGER<br>
     *          SIDEX_DATA_TYPE_FLOAT<br>
     *          SIDEX_DATA_TYPE_DATETIME<br>
     *          SIDEX_DATA_TYPE_BINARY<br>
     *          SIDEX_DATA_TYPE_STRING<br>
     *          SIDEX_DATA_TYPE_LIST<br>
     *          SIDEX_DATA_TYPE_DICT<br>
     *          SIDEX_DATA_TYPE_TABLE
     *
     * @see sidexDefines.h, sidexStdTypes.h
     */
    SIDEX_DATA_TYPE sidexcom_GetNodeType(SIDEX_NODE node);


    /**
     * @brief   Allocates a new sidexBoolean, gets it's value from the provided node.
     *
     *          This call will implicitly increment the objects reference counter of the memory management.<br>
     *          It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @param   node Node to get content from.
     * @param   sBool Reference to reference to sidexBoolean.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_INVALID_BOOLEAN on wrong boolean text representation.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate an sidexBoolean.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
     int sidexcom_BoolFromNode(SIDEX_NODE node, sidexBoolean** sBool);


    /**
     * @brief   Allocates a new sidexInteger, gets it's value from the provided node.
     *
     *          This call will implicitly increment the objects reference counter of the memory management.<br>
     *          It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @param   node Node to get content from.
     * @param   sInt Reference to reference to sidexInteger.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_INVALID_INTEGER on range overflow.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate an sidexInteger.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
     int sidexcom_IntFromNode(SIDEX_NODE node, sidexInteger** sInt);


    /**
     * @brief   Allocates a new sidexFloat, gets it's value from the provided node.
     *
     *          This call will implicitly increment the objects reference counter of the memory management.<br>
     *          It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @param   node   Node to get content from.
     * @param   sFloat Reference to reference to sidexFloat.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_INVALID_FLOAT on range overflow.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexFloat.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
      int sidexcom_FloatFromNode(SIDEX_NODE node, sidexFloat** sFloat);




    /**
     * @brief   Allocates a new sidexDateTime, gets it's value from the provided node.
     *
     *          This call will implicitly increment the objects reference counter of the memory management.<br>
     *          It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @param   node   Node to get content from.
     * @param   sDateTime Reference to reference to sidexDateTime.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_INVALID_DATETIME if the content of the requested node is in invalid DateTime format.
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexFloat.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
    int sidexcom_DateTimeFromNode(SIDEX_NODE node, sidexDateTime** sDateTime);


    /**
     * @brief   Allocates a new sidexString (raw encoded), gets it's value from the provided node.
     *
     *          If the encoding attribute of the key at the node is "base64", the string will be decoded.<br>
     *          This call will increment the objects reference counter of the memory management.<br>
     *          It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @param   node     Node to get content from.
     * @param   sString  Reference to reference to sidexString.<br>
     *                   It must be deallocated using sidexutil_Variant_DecRef().
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexString.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
     int sidexcom_StringFromNode(SIDEX_NODE node, sidexString** sString);


    /**
     * @brief   Allocates a new char string (raw encoded), gets it's value from the provided node.
     *
     *          If the encoding attribute of the key at the node is "base64", the string will be decoded.<br>
     *          It must be dealocated using delete().
     *
     * @param   node         Node to get content from.
     * @param   sString      Reference to char array.
     * @param   iStrLength   Reference to the length of the String.
     * @param   expectedType Identificate if string of binary
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexString od binaryString.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
     int sidexcom_StringFromNode(SIDEX_NODE node, char** sString, SIDEX_INT32* iStrLength, SIDEX_DATA_TYPE expectedType);


    /**
     * @brief   Returns the length of the value from the provided node interpreted as a string.
     *
     *          If the encoding attribute of the key at the node is "base64", the string will be decoded.<br>
     *          It must be dealocated using delete().
     *
     * @param   node         Node to get content from.
     * @param   iStrLength   Reference to the length of the String.
     * @param   expectedType Identificate if string of binary
     * @param   iUnicode     Unicode  identifier.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexString.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
     int sidexcom_StringFromNodeLength(SIDEX_NODE node, SIDEX_INT32* iStrLength, SIDEX_DATA_TYPE expectedType, tmlUnicodeID iUnicode);


    /**
     * @brief   Allocates a new sidexList, gets it's value from the provided node.
     *
     *          This call will implicitly increment the objects reference counter of the memory management.<br>
     *          It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @param   node  Node to get content from.
     * @param   sList Reference to reference to sidexList.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexList.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
     int sidexcom_ListFromNode(SIDEX_NODE node, sidexList** sList);


    /**
     * @brief   Allocates a new sidexDict, gets it's value from the provided node.
     *
     *          This call will implicitly increment the objects reference counter of the memory management.<br>
     *          It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @param   node  Node to get content from.
     * @param   sDict Reference to reference to sidexDict.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexDict.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
     int sidexcom_DictFromNode(SIDEX_NODE node, sidexDict** sDict);


     /**
     * @brief   Allocates a new sidexTable, gets it's value from the provided node.
     *
     *          This call will implicitly increment the objects reference counter of the memory management.<br>
     *          It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @param   node   Node to get content from.
     * @param   sTable Reference to reference to sidexTable.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexTable.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
     int sidexcom_TableFromNode(SIDEX_NODE node, sidexTable** sTable);


     /**
     * @brief  Returns SIDEX_NODE dependent of a childNodeName.
     *
     * @param   rootNode       rootNode to get child from.
     * @param   cChildNodeName Name of the child node.
     * @param   bCreate        If bCreate is true the node will be created if it don't exists.
     * @param   bMatch         Reference to a boolean,<br>
     *                         true in case of an existing node<br>
     *                         false if the node don't exist.
     * @param   newNode        Reference to the requested node.
     *
     * @returns  SIDEX_SUCCESS in case of success.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
     int sidexcom_GetNodeForChildNodeName(SIDEX_NODE rootNode, char* cChildNodeName, bool bCreate, bool* bMatch, SIDEX_NODE* newNode);


     /**
     * @brief  Write the sidexList object as content of provided node.(Subkeys will be created)
     *
     * @param   node  The SIDEX_NODE to write into.
     * @param   sList the list to write.
     *
     * @returns SIDEX_SUCCESS in case of success<br>
     *          SIDEX_ERR_WRONG_TYPE if the type of any containing item is unknown.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
    int sidexcom_WriteListTree(SIDEX_NODE node, sidexList* sList);


    /**
     * @brief  Write the sidexDict object as content of provided node.(Subkeys will be created)
     *
     * @param   node  The SIDEX_NODE to write into.
     * @param   sDict the dict to write.
     *
     * @returns SIDEX_SUCCESS in case of success<br>
     *          SIDEX_ERR_WRONG_TYPE if the type of any containing item is unknown.<br> 
     *          SIDEX_ERR_NOCONTENT if there is no dict entry for the given key.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
    int sidexcom_WriteDictTree(SIDEX_NODE node, sidexDict* sDict);


     /**
     * @brief  Write the sidexTable object as content of provided node.(Subkeys will be created)
     *
     * @param   node   The SIDEX_NODE to write into.
     * @param   sTable the table to write.
     *
     * @returns SIDEX_SUCCESS in case of success.
     *
     * @see sidexErrors.h,  sidexDefines.h
     */
    int sidexcom_WriteTableTree(SIDEX_NODE node, sidexTable* sTable);


     /**
     * @brief   Sets none value (empty value) as content of provided node.
     *
     * @param   node  The SIDEX_NODE to write into.
     *
     *
     * @see sidexDefines.h
     */
    void sidexcom_WriteNone(SIDEX_NODE node);


     /**
     * @brief   Sets boolean value as content of provided node.
     *
     * @param   node  The SIDEX_NODE to write into.
     * @param   value the SIDEX_BOOL value.
     *
     *
     * @see sidexDefines.h
     */
    void sidexcom_WriteBoolean(SIDEX_NODE node, SIDEX_BOOL value);


     /**
     * @brief   Sets integer value as content of provided node.
     *
     * @param   node  The SIDEX_NODE to write into.
     * @param   value the int value.
     *
     *
     * @see sidexDefines.h
     */
    void sidexcom_WriteInteger(SIDEX_NODE node, SIDEX_INT64 value);


     /**
     * @brief   Sets float value as content of provided node.
     *
     * @param   node  The SIDEX_NODE to write into.
     * @param   value the float value
     *
     * @see sidexDefines.h
     */
    void sidexcom_WriteFloat(SIDEX_NODE node, double value);


     /**
     * @brief   Sets DateTime value as content of provided node.
     *
     * @param   node  The SIDEX_NODE to write into.
     * @param   value   Variant object interpreted as a DateTime to be set.
     *
     * @see sidexDefines.h
     */
    void sidexcom_WriteDateTime(SIDEX_NODE node, SIDEX_VARIANT value);


    /**
     * @brief   Reads content of provided node, interpreting it as variant.
     *
     * @param   node    The SIDEX_NODE to operate.
     * @param   variant Reference that will hold the SIDEX_VARIANT.<br>
     *                  This call will increment the objects reference counter of the memory management.<br>
     *                  It must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_INVALID_BOOLEAN on wrong boolean text representation.<br>
     *  SIDEX_ERR_INVALID_INTEGER on range overflow.<br>
     *  SIDEX_ERR_INVALID_FLOAT on range overflow.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the type does not indicate a char string.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadVariant(SIDEX_NODE node, SIDEX_VARIANT* variant);


    /**
     * @brief   Reads content of provided node, interpreting it as boolean.
     *
     * @param   node  The SIDEX_NODE to operate.
     * @param   value Reference that will hold the return value.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_INVALID_BOOLEAN on wrong boolean text representation.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate an integer.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadBoolean(SIDEX_NODE node, bool *value);


    /**
     * @brief   Reads content of provided node, interpreting it as integer.
     *
     * @param   node  The SIDEX_NODE to operate.
     * @param   value Reference that will hold the return value.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_INVALID_INTEGER on range overflow.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate an integer.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadInteger(SIDEX_NODE node, SIDEX_INT64* value);


    /**
     * @brief   Reads content of provided node, interpreting it as float.
     *
     * @param   node  The SIDEX_NODE to operate.
     * @param   value Reference that will hold the keys value.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_INVALID_FLOAT on range overflow.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a float.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadFloat(SIDEX_NODE node, double *value);


    /**
     * @brief   Reads content of provided node, interpreting it as the content DateTime.
     *
     * @param   node      The SIDEX_NODE to operate.
     * @param   sDateTime Reference that will hold the keys value.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a DateTime.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadDateTime(SIDEX_NODE node, sidexDateTime** sDateTime);


    /**
     * @brief   Reads content of provided node, interpreting it as char array.
     *
     * @param   node      The SIDEX_NODE to operate.
     * @param   value     Reference that will hold the string.
     * @param   size      Reference that will hold the return string's length.
     * @param   iEncoding Reference that will hold the string's encoding during the read operation.<br>
     *                    'SIDEX_ENCODE_NONE' (raw) or 'SIDEX_ENCODE_BASE64'  (base64).<br>
     *                    In case of encoding attribute 'base64', the return string will be decoded internal to "raw".
     * @param   format    Reference to the string format attribute.<br>
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a char string.
     *
     * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
     */
    int sidexcom_ReadString(SIDEX_NODE node, char **value, SIDEX_INT32* size, SIDEX_ENCODE* iEncoding, SIDEX_STRING_ENCODING_TYPE* format);


    /**
     * @brief   Reads content of provided handles node, interpreting it as char array.
     *
     *          If the encoding attribute of the key at the node is "base64", the string will be decoded.<br>
     *          It must be deallocated using delete().
     *
     * @param   ngroup     Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey       Name of the key whose content is has to be read.
     * @param   value      Reference that contains the char array to read.<br>
     *                     It must be deallocated using delete().
     * @param   iStrLength Reference to the length of the String.
     * @param   expectedType Identificate if string of binary
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *          SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_NOCONTENT if key holds no content.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadString(char* ngroup, char* nkey, char** value, SIDEX_INT32* iStrLength, SIDEX_DATA_TYPE expectedType);


    /**
     * @brief   Returns thelength of the content of provided handles node (interpreting it as char array).
     *
     *          If the encoding attribute of the key at the node is "base64", the string will be decoded.<br>
     *          It must be deallocated using delete().
     *
     * @param   ngroup       Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey         Name of the key whose content is has to be read.
     * @param   iStrLength   Reference to the length of the String.
     * @param   expectedType Identificate if string of binary
     * @param   iUnicode     Unicode identifier.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *          SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_NOCONTENT if key holds no content.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadStringLength(char* ngroup, char* nkey, SIDEX_INT32* iStrLength, SIDEX_DATA_TYPE expectedType, tmlUnicodeID iUnicode);


    /**
     * @brief   Reads content of provided node, interpreting it as the content of sidexList.
     *
     * @param   node  The SIDEX_NODE to operate.
     * @param   sList Reference to the list that has to be filled.<br>
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexList.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadList(SIDEX_NODE node, sidexList* sList);


    /**
     * @brief   Reads content of provided node, interpreting it as the content of sidexDict.
     *
     * @param   node  The SIDEX_NODE to operate.
     * @param   sDict Reference to the dict that has to be filled.<br>
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexDict.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadDict(SIDEX_NODE node, sidexDict* sDict);


    /**
     * @brief   Reads content of provided node, interpreting it as the content of sidexTable.
     *
     * @param   node   The SIDEX_NODE to operate.
     * @param   sTable Reference to the table that has to be filled.<br>
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_NOCONTENT if node holds no content.<br>
     *  SIDEX_ERR_WRONG_TYPE if the data type of the node does not indicate a sidexTable.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadTable(SIDEX_NODE node, sidexTable* sTable);


    /**
     * @brief   Sets SIDEX_VARIANT value as content of the ngroup and nkey.
     *
     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     * @param   variant Instance of a SIDEX_VARIANT or NULL.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_WriteVariant(char* ngroup, char* nkey, SIDEX_VARIANT variant);


    /**
     * @brief   Sets none value (empty value) as content of the ngroup and nkey.
     *
     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_WriteNone(char* ngroup, char* nkey);


    /**
     * @brief   Sets SIDEX_BOOL value as content of the ngroup and nkey.
     *
     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     * @param   value   Value to be set.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_WriteBoolean(char* ngroup, char* nkey, SIDEX_BOOL value);


    /**
     * @brief   Sets integer value as content of the ngroup and nkey.
     *
     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     * @param   value   Value to be set.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_WriteInteger(char* ngroup, char* nkey, SIDEX_INT64 value);


    /**
     * @brief   Sets float value as content of the ngroup and nkey.
     *
     *
     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     * @param   value   Value to be set.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_WriteFloat(char* ngroup, char* nkey, double value);


    /**
     * @brief   Sets DateTime value as content of the ngroup and nkey.
     *
     *
     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     * @param   value   Variant object interpreted as a DateTime to be set.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_WriteDateTime(char* ngroup, char* nkey, SIDEX_VARIANT value);


    /**
     * @brief   Sets a char string as content of the ngroup and nkey.
     *
     *
     * @param   ngroup   Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey     Name of the key whose content is to be set.
     * @param   value    Value to be set.
     * @param   size     Length of string [if -1 strlen is used].
     * @param   encoding 'SIDEX_ENCODE_NONE' (raw) or 'SIDEX_ENCODE_BASE64' (base64).
     * @param   format   The SIDEX_STRING_ENCODING_TYPE.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexErrors.h, sidexDefines.h, sidexStdTypes.h
     */
    int sidexcom_WriteString(char* ngroup, char* nkey, const char *value, int size, SIDEX_ENCODE encoding, SIDEX_STRING_ENCODING_TYPE format);


    /**
     * @brief   Sets sidexList value as content of the ngroup and nkey. (Subkeys will be created)
     *

     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     * @param   value   List to be dumped.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_PARAMETER if value is NULL.
     */
    int sidexcom_WriteList(char* ngroup, char* nkey, sidexList* value);


    /**
     * @brief   Sets sidexDict value as content of the ngroup and nkey. (Subkeys will be created)
     *
     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     * @param   value   Dict to be dumped.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_PARAMETER if value is NULL.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_WriteDict(char* ngroup, char* nkey, sidexDict* value);


    /**
     * @brief   Sets sidexTable value as content of the ngroup and nkey. (Subkeys will be created)
     *

     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is to be set.
     * @param   value   Table to be dumped.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *          SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_WRONG_PARAMETER if value is NULL.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_WriteTable(char* ngroup, char* nkey, sidexTable* value);


    /**
     * @brief   Reads content of provided handles node, interpreting it as SIDEX_VARIANT.
     *
     * @param   ngroup  Name of the group holding the nkey / optional NULL if you don't want to specify a group.
     * @param   nkey    Name of the key whose content is has to be read.
     * @param   variant Reference that contains the SIDEX_VARIANT.<br>
     *                  It must be deallocated using sidexutil_Variant_DecRef.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *          SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_INVALID_DATETIME if the content of the requested node is in invalid DateTime format (if it's type is SIDEX_DATA_TYPE_DATETIME).
     *          SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_NOCONTENT if key holds no content.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_ReadVariant(char* ngroup, char* nkey, SIDEX_VARIANT* variant);


    /**
     * @brief   Allows to create a new group/child inside the parent group.
     *
     * @param   parent  The SIDEX_NODE to operate.
     * @param   nNode   Name of the group/child to create.
     * @param   newNode Reference to the created node.
     *
     * @returns  SIDEX_SUCCESS in case of success.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_CreateNode(SIDEX_NODE parent, const char *nNode, SIDEX_NODE* newNode);


    /**
     * @brief   Allows to get the nodes child depending of it's name.
     *
     * @param   retNode reference to SIDEX_NODE object for the child node to get.
     * @param   node    The SIDEX_NODE to operate.
     * @param   name    Name of the node
     *
     * @returns  SIDEX_SUCCESS in case of success.<br>
     *           SIDEX_ERR_NOCONTENT if the request failed.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_GetChildByName(SIDEX_NODE* retNode, SIDEX_NODE node, char* name);


    /**
     * @brief   Allows to get the number of childs of the provided node.
     *
     * @param   iCount reference to int for numbers of childs.
     * @param   node   The SIDEX_NODE to operate.
     *
     * @returns  SIDEX_SUCCESS in case of success.<br>
     *           SIDEX_ERR_NOCONTENT if the request failed.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_GetChildCount(int* iCount, SIDEX_NODE node);


    /**
     * @brief   Allows to get the childs name of the node at the index.
     *
     * @param   retName reference to char array for the childs nname.
     * @param   node    The SIDEX_NODE to operate.
     * @param   index   Index of the child.
     * 
     * @returns  SIDEX_SUCCESS in case of success.<br>
     *           SIDEX_ERR_NOCONTENT if there is no content at the given index.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_GetNodeNameByIndex(char** retName, SIDEX_NODE node, int index);


    /**
     * @brief   Allows to get the child of the node at the index.
     *
     * @param   retNode reference to SIDEX_NODE object for the child node to get.
     * @param   node    The SIDEX_NODE to operate.
     * @param   index   Index of child
     * 
     * @returns  SIDEX_SUCCESS in case of success.<br>
     *           SIDEX_ERR_NOCONTENT if there is no content at the given index.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_GetChildByIndex(SIDEX_NODE* retNode, SIDEX_NODE node, int index);


    /**
     * @brief   Allows to remove a node inside the parent group.
     *
     * @param   parent The SIDEX_NODE to operate.
     *
     * @see sidexDefines.h
     */
    void sidexcom_DeleteNode(SIDEX_NODE parent);


    /**
     * @brief   Get the node for a group and a key.
     *
     * @param   ngroup      Name of the group for the node request / optional NULL if you don't want to specify a group.
     * @param   nkey        Name of the key for the node request.
     * @param   bCreate     If bCreate is true the node will be created if it don't exists.
     * @param   bKeyMatch   Reference to a boolean,<br>
     *                      true in case of an existing node for the key<br>
     *                      false if the node for the key don't exist.
     * @param   bGroupMatch Reference to a boolean,<br>
     *                      true in case of an existing node for the group<br>
     *                      false if the node for the group don't exist,
     *                      true in case of ngroup is NULL.
     * @param   node        Reference to the requested node.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *  SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *  SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *  SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexDefines.h
     */
    int sidexcom_GetNodeForGroupAndKey(char* ngroup, char* nkey, bool bCreate, bool* bKeyMatch, bool *bGroupMatch, SIDEX_NODE* node);

    /**
     * @brief   Returns the group list of the rootnode.
     *
     * @param   variant Reference to the list of group names.<br>
     *          It's a sidexList object and must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no group.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_GetGroups(SIDEX_VARIANT* variant);

    
    /**
     * @brief   Check for the existence of a group.
     *
     * @param   ngroup  Name of the requested group.
     *
     * @returns true if a requested group exists.
     */
    SIDEX_BOOL sidexcom_HasGroup(char* ngroup);


    /**
     * @brief   Returns the key list of the rootnode for the requested group.
     *
     * @param   ngroup  Name of the group holding the requeset keys.<br>
     *          It is optional NULL. In that case all single key names will be returned.
     * @param   variant Reference to the list of key names.<br>
     *          It's a sidexList object and must be decremented using sidexutil_Variant_DecRef, if the object is not longer in use.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_NOCONTENT if the ngroup don't exist or if there is no key in the ngroup.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_GetKeys(char* ngroup, SIDEX_VARIANT* variant);


    /**
    * @brief   Check for the existence of a key in a group.
    *
    * @param   ngroup  Name of the requested group.
    * @param   nkey    Name of the requested key.
    *
    * @returns true if a requested group exists.
    */
    SIDEX_BOOL sidexcom_HasKey(char* ngroup, char* nkey);


    /**
     * @brief   Remove the group and it's content of the rootnode.
     *
     * @param   ngroup  Name of the group to be removed.<br>
     *          It is optional NULL. In that case all single key's will be removed.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_WRONG_GROUP if ngroup contains blanks or tabs, it's length is 0 or ngroup is NULL.<br>
     *          SIDEX_ERR_NOCONTENT if the ngroup don't exist.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_DeleteGroup(char* ngroup);


    /**
     * @brief   Remove the key of a group of the rootnode.
     *
     * @param   ngroup  Name of the group with the key to be removed.<br>
     *          It is optional NULL. In that case a single key will be removed.
     * @param   nkey    Name of the key to be removed.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *          SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_WRONG_GROUP if ngroup is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).<br>
     *          SIDEX_ERR_NOCONTENT if the ngroup or key in ngroup don't exist.
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    int sidexcom_DeleteKey(char* ngroup, char* nkey);


    /**
     * @brief   Check sKey content for valid XML name convention
     *
     * @param   sKey    Reference containing the key
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_MISSING_KEY if key is NULL.<br>
     *          SIDEX_ERR_WRONG_KEY if key is an invalid XML name (contains characters like '\' '"' '&' '<' '>' ']]>' blanks or is empty).
     *
     * @see sidexErrors.h, sidexDefines.h
     */
    static int sidexcom_IsValidKey(char* sKey);

    /**
     * @brief   Merge a SIDEX_HANDLE into another one.
     *
     * @param   sMergeHandle  The SIDEX_HANDLE to get the merge data from.
     * @param   bOverwrite    Flag, if true, the data referring nGroup and nKey will be overwritten.<br>
     *                        If nGroup and nKey don't exist on the sBaseHandle, the data will be copied.
     * @param   nGroup        Name of the requested group for the overwrite operation.
     * @param   nKey          Name of the requested key for the overwrite operation.<br>
     *                        This parameter may be SIDEX_HANDLETYPE_NULL. In that case the whole nGroup content will be overwritten / copied.
     *
     * @returns  SIDEX_SUCCESS in case of success.
     *
     * @see globalDefines.h, sidexStdTypes.h
     */
    SIDEX_INT32 sidexcom_Merge(SIDEX_HANDLE sMergeHandle, SIDEX_BOOL bOverwrite, char* nGroup, char* nKey);

};


#endif // SIDEXCOM_H