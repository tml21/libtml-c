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

#ifndef SIDEXTYPETABLE_H
#define SIDEXTYPETABLE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidexTypeBase.h"
#include "sidexList.h"
#include "sidexDict.h"
#include "sidexString.h"

class sidexTable : public sidexBase
{
private:
	  /* data */
    int         m_nRows;
    sidexList*  m_pRows;
    int         m_nColumns;
    sidexList*  m_pColumns;
    int         m_iPendingRows2Add;

protected: 
	  /* data */
    static const SIDEX_DATA_TYPE type = SIDEX_DATA_TYPE_TABLE;
	  
public:
	  /* methods */
    /**
     * @brief    Constructor.
     *
     * @returns an instance of sidexTable.
     */
    sidexTable();


    /**
     * @brief    Destructor.
     */
    ~sidexTable();


    /**
     * @brief    Cleans up refCounter dependent allocations
     */
    void cleanUp();


    /**
     * @brief   Get type of this data object.
     *
     * @returns SIDEX_DATA_TYPE_TABLE.
     *
     * @see sidexStdTypes.h
     */
    SIDEX_DATA_TYPE getType();


    /**
     * @brief    Returns the number of rows.
     *
     * @returns the number of rows.
     */
    int countRows();


    /**
     * @brief    Returns the number of columns.
     *
     * @returns the number of columns.
     */
    int countColumns();


    /**
     * @brief   Get the table's column names.
     * @param   columnNames   referring to an array for the column names.<br>
                After using the columnNames you must have to deallocate memory using delete() for the columnNames but not for it's content.
     *    
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no dict entry.
     *
     * @see sidexErrors.h
     */
    int getColumnNames(char *** columnNames);


    /**
     * @brief   Add a new empty row to a Table.
     *
     * It is possible to add empty rows before any column definition, but<br>
     * getRow() will return SIDEX_ERR_NOCONTENT in case of a request referring that empty row(s),<br>
     * until the first column has been defined by addColumn().<br>
     * countRows() will return the amount of empty rows, or the amount of defined rows if the first column has been defined by addColumn().
     *
     * @returns SIDEX_SUCCESS in case of success.
     */
    int addRow();


    /**
     * @brief    Add a column to a Table.
     *
     * @param   sColumn reference to the column name.
     *
     * @returns SIDEX_SUCCESS in case of success<br>
     *          SIDEX_ERR_TABLE_COLUMN_ALREADY_EXISTS if column exists.
     */
    int addColumn(char* sColumn);


    /**
     * @brief    Remove a column of a Table.
     *
     * @param   sColumn reference to the column name.
     *
     * @returns SIDEX_SUCCESS in case of success<br>
     *          SIDEX_ERR_NOCONTENT if column don't exist.
     */
    int deleteColumn(char* sColumn);


    /**
     * @brief    Get the column name from a Table at the requested index.
     *
     * @param   index requested column index  (base 0).
     * @param   sColumn reference to reference for the column name.<br>
     *          It is owned by the sTable and has not to be modified or deallocated.
     *
     * @returns SIDEX_SUCCESS in case of success<br>
     *          SIDEX_ERR_NOCONTENT if there is no column at the requested index.
     */
    int getColumnName_A(int index, char** sColumn);


    /**
     * @brief    Get the column name from a Table at the requested index.
     *
     * @param   index requested column index  (base 0).
     * @param   sColumn reference to reference for the column name.<br>
     *          It is owned by the sTable and has not to be modified or deallocated.
     *
     * @returns SIDEX_SUCCESS in case of success<br>
     *          SIDEX_ERR_NOCONTENT if there is no column at the requested index.
     */
    int getColumnName_X(int index, wchar_t** sColumn);


    /**
     * @brief    Get the column name from a Table at the requested index.
     *
     * @param   index requested column index  (base 0).
     * @param   sColumn reference to reference for the column name.<br>
     *          It is owned by the sTable and has not to be modified or deallocated.
     *
     * @returns SIDEX_SUCCESS in case of success<br>
     *          SIDEX_ERR_NOCONTENT if there is no column at the requested index.
     */
    int getColumnName_W(int index, char16_t** sColumn);


    /**
     * @brief    Remove a row of the table.
     *
     * @param   sRow the row (type SIDEX_VARIANT) to be removed out of the list.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if the table don't contain the sRow.<br>
     *          SIDEX_ERR_WRONG_PARAMETER if sRow is NULL.
     * @see sidexErrors.h
     */
    int deleteRow(SIDEX_VARIANT sRow);


    /**
     * @brief   Remove all rows out of the table.
     *
     * Also empty rows will be removed - see addRow().
     *
     * @returns SIDEX_SUCCESS in case of success.
     * @see sidexErrors.h
     */
    int deleteRows();


    /**
     * @brief    Returns the list of rows.
     *
     * @returns list of rows.
     */
    sidexList* getRows();


    /**
     * @brief    Returns the list of columns.
     *
     * @returns list of columns.
     */
    sidexList* getColumns();


    /**
     * @brief    Get row at index iRow.
     *
     * @param   dict Reference to reference of sidexDict for the return value / NULL if it don't exist.
     * @param   iRow The index (base 0) of the requested row.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no content at the given iRow.
     *
     * @see sidexErrors.h
     */
    int getRow(sidexDict** dict, int iRow);


    /**
     * @brief    Get column name at index iCol.
     *
     * @param   sColumn Reference to sidexString for the return value / NULL if it don't exist.
     * @param   iCol The index (base 0) of the requested column.
     *
     * @returns SIDEX_SUCCESS in case of success.<br>
     *          SIDEX_ERR_NOCONTENT if there is no content at the given iCol.
     *
     * @see sidexErrors.h
     */
    int getColumn(sidexString** sColumn, int iCol);
};

#endif // SIDEXTYPETABLE_H
