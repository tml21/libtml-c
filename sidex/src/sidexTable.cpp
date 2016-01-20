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

#include "sidexTable.h"
#include "sidexUtils.h"
#include "sidex.h"
#include "sidexNone.h"
#include <string.h>

/**
 * @brief    Constructor.
 */
sidexTable::sidexTable()
{
  m_nRows = 0;
  m_nColumns = 0;
  m_iPendingRows2Add = 0;
  incRef();
  m_pRows = new sidexList();
  m_pColumns = new sidexList();
}


/**
 * @brief    Destructor.
 */
sidexTable::~sidexTable()
{
	cleanUp();
}


/**
 * @brief    Cleans up refCounter dependent allocations
 */
void sidexTable::cleanUp(){
  if (decRef() == 0){
    ////////////////////////////////////////////
    // Rows:
    for (int i = 0; i < m_nRows; ++i){
      // decrement the objects reference counter:
      sidexDict* dict;
      if (SIDEX_SUCCESS == getRow(&dict, i)){
        sidexutil_Variant_DecRef((SIDEX_VARIANT)dict);
      }
    }
    m_nRows = 0;
    // I have to deallocate memory in row- list:
    delete(m_pRows);

    ////////////////////////////////////////////
    //Columns:
    for (int i = 0; i < m_nColumns; ++i){
      sidexutilList_DeleteItem((SIDEX_VARIANT)m_pColumns,i);
    }
    m_nColumns = 0;
    // I have to deallocate memory in row- list:
    delete(m_pColumns);
  }
  m_iPendingRows2Add = 0;
}

/**
 * @brief   Get type of this data object.
 */
SIDEX_DATA_TYPE sidexTable::getType()
{
   return type;
}


/**
 * @brief    Returns the number of rows.
 */
int sidexTable::countRows()
{
   return m_nRows + m_iPendingRows2Add;
}


/**
 * @brief    Returns the number of columns.
 */
int sidexTable::countColumns()
{
  return m_nColumns;
}


/**
* @brief   Get the table's column names.
*/
int sidexTable::getColumnNames(char *** columnNames){
  SIDEX_INT32 iSize;
  int iRet = sidexutilList_Size((SIDEX_VARIANT)m_pColumns, &iSize);

  char** names = new char*[iSize];
  for (int i = 0; SIDEX_SUCCESS == iRet && i < iSize; ++i){
    SIDEX_VARIANT vCol;
    iRet = sidexutilList_Get((SIDEX_VARIANT)m_pColumns, i, &vCol);
    if (SIDEX_SUCCESS ==iRet){
      names[i] = ((sidexString*)vCol)->getValue_A();
    }
  }
  if (SIDEX_SUCCESS == iRet){
    *columnNames = names;
  }
  return iRet;
}


/**
 * @brief    Add a column to a Table.
 */
int sidexTable::addColumn(char* sColumn){

  ///////////////////////////
  // Check, if column exists:
  SIDEX_INT32 iSize = 0;
  int iRet;
  iRet = sidexutilList_Size((SIDEX_VARIANT)m_pColumns, &iSize);
  for (int i = 0; i < iSize && SIDEX_SUCCESS == iRet; ++i){
    SIDEX_VARIANT vColumn;
    iRet = sidexutilList_Get((SIDEX_VARIANT)m_pColumns, i, &vColumn);
    if (SIDEX_SUCCESS == iRet){
      char* sCompare;
      SIDEX_INT32 iLength;
      iRet = sidexutilString_Value_A(vColumn, &sCompare, &iLength);
      if (SIDEX_SUCCESS == iRet){
        if (0 == strcmp(sColumn, sCompare)){
          iRet = SIDEX_ERR_TABLE_COLUMN_ALREADY_EXISTS;
        }
      }
    }
  }

  ///////////////////////////
  // Add the column name:
  if (SIDEX_SUCCESS == iRet){
    SIDEX_VARIANT vColumn;
    vColumn = (SIDEX_VARIANT)new sidexString(sColumn, (int)strlen(sColumn), SIDEX_ENCODE_NONE);
    SIDEX_INT32 iPos;
    iRet = sidexutilList_Add((SIDEX_VARIANT)m_pColumns, vColumn, &iPos);
    ((sidexString*)vColumn)->decRef();
    ++m_nColumns;
  }

  ///////////////////////////
  // Now fill up all existing rows with None data objects:
  if (iRet == SIDEX_SUCCESS)
  {
    int iRows = m_nRows; // countRows(); - the pending rows will not be included
    for (int i = 0; i < iRows && SIDEX_SUCCESS == iRet; ++i){
      sidexDict* sDict;
      iRet = getRow(&sDict, i);
      if (SIDEX_SUCCESS == iRet){
        SIDEX_VARIANT var = (SIDEX_VARIANT)new sidexNone();
        iRet = sDict->setValue(sColumn, var);
        sidexutil_Variant_DecRef(var);
      }
    }
  }

  ///////////////////////////
  // Possible rows to add:
  int iPendingRows2Add = m_iPendingRows2Add;
  for (int i = 0; i < iPendingRows2Add; ++i){
    /////////////////////////////////////////
    // Don't take care about the return here:
    /* iRet = */ addRow();
    --m_iPendingRows2Add;
  }

  return iRet;
}


/**
 * @brief    Remove a column of a Table.
 */
int sidexTable::deleteColumn(char* sColumn){

  ///////////////////////////
  // Check, if column exists:
  SIDEX_INT32 iSize = 0;
  int iRet;
  iRet = sidexutilList_Size((SIDEX_VARIANT)m_pColumns, &iSize);
  bool bFound = false;
  for (int i = 0; i < iSize && SIDEX_SUCCESS == iRet && !bFound; ++i){
    SIDEX_VARIANT vColumn;
    iRet = sidexutilList_Get((SIDEX_VARIANT)m_pColumns, i, &vColumn);
    if (SIDEX_SUCCESS == iRet){
      char* sCompare;
      SIDEX_INT32 iLength;
      iRet = sidexutilString_Value_A(vColumn, &sCompare, &iLength);
      if (SIDEX_SUCCESS == iRet){
        if (0 == strcmp(sColumn, sCompare)){
          bFound = true;
          --m_nColumns;
          sidexutilList_DeleteItem((SIDEX_VARIANT)m_pColumns, i);

          int iRows = countRows();
          //////////////////////////////////////
          // Now delete the dictionary entries:
          for (int i = 0; i < iRows && SIDEX_SUCCESS == iRet; ++i){
            sidexDict* aRow;
            iRet = getRow(&aRow, i);
            if (SIDEX_SUCCESS == iRet){
              // Don't mind the return value if the column don't exist:
              /*iRet = */ sidexutilDict_Delete((SIDEX_VARIANT)aRow, sColumn);
            }
          }
          if (SIDEX_SUCCESS == iRet){
            if (0 == countColumns()){
              // If there are not any columns left, I have do delete all rows:
              deleteRows();
            }
          }
        }
      }
    }
  }

  if (!bFound)
  {
    iRet = SIDEX_ERR_NOCONTENT;
  }
  return iRet;
}


/**
 * @brief    Get the column name from a Table at the requested index.
 */
int sidexTable::getColumnName_A(int index, char** sColumn){

  SIDEX_VARIANT vColumn;
  int iRet = sidexutilList_Get((SIDEX_VARIANT)m_pColumns, index, &vColumn);
  if (SIDEX_SUCCESS == iRet){
    SIDEX_INT32 iLength;
    iRet = sidexutilString_Value_A(vColumn, sColumn, &iLength);
  }
  return iRet;
}


/**
 * @brief    Get the column name from a Table at the requested index.
 */
int sidexTable::getColumnName_X(int index, wchar_t** sColumn){

  SIDEX_VARIANT vColumn;
  int iRet = sidexutilList_Get((SIDEX_VARIANT)m_pColumns, index, &vColumn);
  if (SIDEX_SUCCESS == iRet){
    SIDEX_INT32 iLength;
    iRet = sidexutilString_Value_X(vColumn, sColumn, &iLength);
  }
  return iRet;
}


/**
 * @brief    Get the column name from a Table at the requested index.
 */
int sidexTable::getColumnName_W(int index, char16_t** sColumn){

  SIDEX_VARIANT vColumn;
  int iRet = sidexutilList_Get((SIDEX_VARIANT)m_pColumns, index, &vColumn);
  if (SIDEX_SUCCESS == iRet){
    SIDEX_INT32 iLength;
    iRet = sidexutilString_Value_W(vColumn, sColumn, &iLength);
  }
  return iRet;
}


/**
 * @brief   Add a new empty row to a Table.
 */
int sidexTable::addRow()
{
  int iRet = SIDEX_SUCCESS;

  if (0 < m_nColumns){
    sidexDict *sDict = new sidexDict();
    for (int i = 0; i < m_nColumns && SIDEX_SUCCESS == iRet; ++i){
      char* sColName;
      iRet = getColumnName_A(i, &sColName);
      SIDEX_VARIANT var = (SIDEX_VARIANT)new sidexNone();
      iRet = sDict->setValue(sColName, var);
      sidexutil_Variant_DecRef(var);
    }
    m_nRows++;
    m_pRows->addElement((SIDEX_VARIANT)sDict);
  }
  else{
    ++m_iPendingRows2Add;
  }
  return iRet;
}


/**
 * @brief    Remove a row of the table.
 */
int sidexTable::deleteRow(SIDEX_VARIANT sRow){
   int iRet = m_pRows->deleteVariant(sRow);
   if (SIDEX_SUCCESS == iRet){
      sidexutil_Variant_DecRef(sRow);
     m_nRows--;
   }
   return iRet;
}


/**
 * @brief   Remove all rows out of the table.
 */
int sidexTable::deleteRows(){
  ////////////////////////////////////////////
  // Rows:
  for (int i = 0; i < m_nRows; ++i){
    // decrement the objects reference counter:
    sidexDict* dict;
    if (SIDEX_SUCCESS == getRow(&dict, i)){
      if (SIDEX_SUCCESS == m_pRows->deleteVariant((SIDEX_VARIANT)dict)){
        sidexutil_Variant_DecRef((SIDEX_VARIANT)dict);
      }
    }
  }
  m_nRows = 0;
  m_iPendingRows2Add = 0;
  return SIDEX_SUCCESS;
}


/**
 * @brief    Returns the list of rows.
 */
sidexList* sidexTable::getRows()
{
   return m_pRows;
}


/**
 * @brief    Returns the list of columns.
 */
sidexList* sidexTable::getColumns()
{
   return m_pColumns;
}


/**
 * @brief    Get row at index iRow.
 */
int sidexTable::getRow(sidexDict** dict, int iRow)
{
   if (iRow >= m_nRows || iRow < 0)
      return SIDEX_ERR_NOCONTENT;
   sidexList::listElement *row = m_pRows->getFirst();
   for(int i = 0; (i < iRow) && (NULL != row->next); i++)
      row = row->next;
   *dict = (sidexDict *) (row->value);
   return SIDEX_SUCCESS;
}


/**
 * @brief    Get column name at index iCol.
 */
int sidexTable::getColumn(sidexString** sColumn, int iCol)
{
   if (iCol >= m_nColumns || iCol < 0)
      return SIDEX_ERR_NOCONTENT;
   sidexList::listElement *col = m_pColumns->getFirst();
   for(int i = 0; (i < iCol) && (NULL != col->next); i++)
      col = col->next;
   *sColumn = (sidexString *) (col->value);
   return SIDEX_SUCCESS;
}
