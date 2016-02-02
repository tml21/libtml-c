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

#include <stdio.h>
#ifdef LINUX
  #include <wchar.h> 
  #include <string.h>
#endif // LINUX
#include <sidex.h>
#include "sdxrt_tchar.h"
#include <iostream>
using namespace std;
#include "sdxrt_Utils.h"

SIDEX_INT32 tabErr      = SIDEX_SUCCESS; // API return value

SIDEX_HANDLE docTabHandle = SIDEX_HANDLE_TYPE_NULL;
SIDEX_VARIANT tabHandle = SIDEX_HANDLE_TYPE_NULL;

/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_New_Table, writing it, with empty rows, to a sidex_document
*
* A new table is created and assigned to a sidex_variant and a column is added to the table.
* The table is written to a newly created sidex_document and
* read again. Afterwards it is checked whether the table stayed the same as before being written 
* to the sidex_document.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_01() {
    bool testOkay = true;
    SIDEX_TCHAR* columnName = NULL;
    SIDEX_INT32 columnSize = NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_01", "sidex_Variant_Table_Check", 1);

    //normal sidex_Create
    tabErr = sidex_Create(DOCUMENT, &docTabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_01", "sidex_Create", 1);

    //add a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_01", "sidex_Variant_Table_AddColumn", 1);

    //write the table to the sidex_document
    tabErr = sidex_Table_Write(docTabHandle, GROUP, KEY, tabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_01", "sidex_Table_Write", 1);

    //read the table again
    sidex_Variant_DecRef(tabHandle);
    tabErr = sidex_Table_Read(docTabHandle, GROUP, KEY, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_01", "sidex_Table_Read", 1);

    //compare the table with the original one
    tabErr = sidex_Variant_Table_Columns(tabHandle, &columnSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_01", "sidex_Variant_Table_Columns", 1);
    testOkay = testOkay && errLog(1 == columnSize, "test_table_01", "columnSize != size", 1);
    //compare the name of the column
    tabErr = sidex_Variant_Table_GetColumnName(tabHandle, 0, &columnName);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_01", "sidex_Variant_Table_GetColumnName", 1);
    testOkay = testOkay && errLog(sdxrt_cmp(columnName, COLUMN) == 0, "test_table_01", "columnName != name", 1);

    sidex_Variant_DecRef(tabHandle);
    sidex_Free(&docTabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_AddColumn for expected behavior, also testing for errors
*
* A new table is created and a column is added to the table. Different errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_02() {
    bool testOkay = true;
    SIDEX_VARIANT tempVariant = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_02", 
                                  "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_02", "sidex_Variant_Table_AddColumn", 1);

    tempVariant = sidex_Variant_New_Integer(1234567);
    //testing a WRONG_TYPE error
    tabErr = sidex_Variant_Table_AddColumn(tempVariant, COLUMN);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_02", "WRONG_TYPE", 1);

    //testing a WRONG_KEY error
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, wKEY);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == tabErr, "test_table_02", "WRONG_KEY", 1);

    //testing a MISSING_KEY error or UNICODE
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, NULL);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == tabErr || SIDEX_ERR_UNICODE == tabErr,
                                  "test_table_02", "MISSING_KEY or UNICODE", 1);

    //testing a TABLE_COLUMN_ALREADY_EXISTS error
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_ERR_TABLE_COLUMN_ALREADY_EXISTS == tabErr, 
                                  "test_table_02", "TABLE_COLUMN_ALREADY_EXISTS", 1);

    sidex_Variant_DecRef(tempVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_AddRow for expected behavior, also testing an error
*
* A new table is created and a column added. A row is added. Afterwards a sidex_variant
* handling an integer is created and used as param for sTable in a function call sidex_Variant_Table_AddRow.
* This tests a WRONG_TYPE error.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_03() {
    bool testOkay = true;
    SIDEX_VARIANT tempVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 rowId = 0;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_03", 
                                  "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_03", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a row to the table
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_03", "sidex_Variant_Table_AddRow", 1);

    tempVariant = sidex_Variant_New_Integer(1234567);
    //testing a WRONG_TYPE error
    tabErr = sidex_Variant_Table_AddRow(tempVariant, &rowId);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_03", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(tempVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks function sidex_Variant_Table_Check
*
* A new table is created and checked, whether it is a sidex_table.
* Then a sidex_variant handling an integer is created and checked, whether the
* function call function sidex_Variant_Table_Check returns SIDEX_FALSE if 
* the sidex_variant handling an integer is given as param.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_04() {
    bool testOkay = true;
    SIDEX_VARIANT tempVariant = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_04", "sidex_Variant_Table_Check", 1);

    //return SIDEX_FALSE
    tempVariant = sidex_Variant_New_Integer(1234567);
    testOkay = testOkay && errLog(SIDEX_FALSE == sidex_Variant_Table_Check(tempVariant), "test_table_03", "sidex_Variant_Table_Check", 2);

    sidex_Variant_DecRef(tempVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}

/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_ColumnNames, also testing errors
*
* First the sidex_variant tabHandle is set to SIDEX_HANDLE_TYPE_NULL and given to the 
* function call of sidex_Variant_Table_ColumnNames as param sTable, testing a NOCONTENT error.
* A table is created and two columns added. Function sidex_Variant_Table_ColumnNames is called and
* the columnNames from the function call are compared to the original ones.
* Then an error is tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_05() {
    bool testOkay = true;
    SIDEX_VARIANT columnNames = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_VARIANT listItem = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 index = 0;
    SIDEX_TCHAR* columnName = NULL;
    SIDEX_INT32 strLength = NULL;
    SIDEX_VARIANT wrongTypeVariant = SIDEX_HANDLE_TYPE_NULL;

    tabHandle = SIDEX_HANDLE_TYPE_NULL;
    //testing a NOCONTENT error
    tabErr = sidex_Variant_Table_ColumnNames(tabHandle, &columnNames);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_05", "NOCONTENT", 1);

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_05", 
                                  "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_05", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, KEY);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_05", "sidex_Variant_Table_AddColumn", 2);

    //normal sidex_Variant_Table_ColumnNames
    tabErr = sidex_Variant_Table_ColumnNames(tabHandle, &columnNames);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_05", "sidex_Variant_Table_ColumnNames", 1);
    //compare the list of columnNames with the original ones, get the first listItem
    tabErr = sidex_Variant_List_Get(columnNames, index, &listItem);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_05", "sidex_Variant_List_Get", 1);
    tabErr = sidex_Variant_As_String(listItem, &columnName, &strLength);  //get the column name
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_05", "sidex_Variant_As_String", 1);
    //compare it to the original value
    testOkay = testOkay && errLog(0 == sdxrt_cmp(columnName, COLUMN), "test_table_05", "columnName != name", 1);
    //compare second columnName
    index = 1;
    tabErr = sidex_Variant_List_Get(columnNames, index, &listItem);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_05", "sidex_Variant_List_Get", 2);
    tabErr = sidex_Variant_As_String(listItem, &columnName, &strLength);  //get the column name
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_05", "sidex_Variant_As_String", 2);
    //compare to original value
    testOkay = testOkay && errLog(0 == sdxrt_cmp(columnName, KEY), "test_table_05", "columnName != name", 2);

    //testing a WRONG_TYPE error
    wrongTypeVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_ColumnNames(wrongTypeVariant, &columnNames);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_05", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(wrongTypeVariant);
    sidex_Variant_DecRef(columnNames);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}

/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_Columns, also testing errors
*
* First, the sidex_variant tabHandle is set to SIDEX_HANDLE_TYPE_NULL and given to the 
* function call of sidex_Variant_Table_Columns as param sTable, testing a NOCONTENT error.
* Afterwards a table is created and with sidex_Variant_Table_Columns the size is checked, it is empty alias 0.
* A column is added and the amount of columns is checked again, now containing one item.
* Then an error is tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_06() {
    bool testOkay = true;
    SIDEX_INT32 columnCount = NULL;
    SIDEX_VARIANT wrongTypeVariant = SIDEX_HANDLE_TYPE_NULL;

    tabHandle = SIDEX_HANDLE_TYPE_NULL;
    //testing a NOCONTENT error
    tabErr = sidex_Variant_Table_Columns(tabHandle, &columnCount);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_06", "NOCONTENT", 1);

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_06", "sidex_Variant_Table_Check", 1);

    //normal getting the amount of columns from the table
    tabErr = sidex_Variant_Table_Columns(tabHandle, &columnCount);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_06", "sidex_Variant_Table_Columns", 1);
    testOkay = testOkay && errLog(0 == columnCount, "test_table_06", "columnCount != amount", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_06", "sidex_Variant_Table_AddColumn", 1);

    //normal getting the amount of columns from the table
    tabErr = sidex_Variant_Table_Columns(tabHandle, &columnCount);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_06", "sidex_Variant_Table_Columns", 2);
    testOkay = testOkay && errLog(1 == columnCount, "test_table_06", "columnCount != amount", 2);

    //testing a WRONG_TYPE error
    wrongTypeVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_Columns(wrongTypeVariant, &columnCount);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_06", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(wrongTypeVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}

/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_DeleteColumn for expected behavior, also testing errors
*
* First, a table is created and a column added. The amount of columns is checked, being 1.
* Afterwards the column is deleted and again it is checked how many columns are in the table, 0.
* Different errors are tested.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_07() {
    bool testOkay = true;
    SIDEX_INT32 columnCount = NULL;
    SIDEX_VARIANT wrongTypeVariant = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_07", "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_07", "sidex_Variant_Table_AddColumn", 1);

    //getting the amount of columns from the table -> 1
    tabErr = sidex_Variant_Table_Columns(tabHandle, &columnCount);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_07", "sidex_Variant_Table_Columns", 1);
    testOkay = testOkay && errLog(1 == columnCount, "test_table_07", "columnCount != amount", 1);

    //normal delete column
    tabErr = sidex_Variant_Table_DeleteColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_07", "sidex_Variant_Table_DeleteColumn", 1);

    //getting the amount of columns from the table -> 0 (empty)
    tabErr = sidex_Variant_Table_Columns(tabHandle, &columnCount);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_07", "sidex_Variant_Table_Columns", 1);
    testOkay = testOkay && errLog(0 == columnCount, "test_table_07", "columnCount != amount", 1);

    //testing WRONG_TYPE error
    wrongTypeVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_DeleteColumn(wrongTypeVariant, COLUMN);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_07", "WRONG_TYPE", 1);

    //testing WRONG_PARAMETER error or UNICODE
    tabErr = sidex_Variant_Table_DeleteColumn(tabHandle, NULL);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == tabErr || SIDEX_ERR_UNICODE == tabErr, 
                                  "test_table_07", "WRONG_PARAMETER or UNICODE", 1);

    //testing NOCONTENT error
    tabErr = sidex_Variant_Table_DeleteColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_07", "NOCONTENT", 1);

    sidex_Variant_DecRef(wrongTypeVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_DeleteRow for expected behavior, also testing for errors
*
* First a table is created and a column is added, then two rows. The amount of rows is checked, containing 2.
* The last row is deleted and the amount of rows is checked again, containing only 1 now.
* Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_08() {
    bool testOkay = true;
    SIDEX_INT32 rowSize = NULL;
    SIDEX_INT32 rowId = NULL;
    SIDEX_VARIANT wrongTypeVariant = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), 
                                  "test_table_08", "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_08", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a row to the table
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_08", "sidex_Variant_Table_AddRow", 1);
    //add another row
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_08", "sidex_Variant_Table_AddRow", 2);

    //check amount of rows (=2)
    tabErr = sidex_Variant_Table_Rows(tabHandle, &rowSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_08", "sidex_Variant_Table_Rows", 1);
    testOkay = testOkay && errLog(2 == rowSize, "test_table_08", "rowSize != size", 1);

    //delete a row
    tabErr = sidex_Variant_Table_DeleteRow(tabHandle, rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_08", "sidex_Variant_Table_DeleteRow", 1);

    //compare the size of the rows after deleting one (=1)
    tabErr = sidex_Variant_Table_Rows(tabHandle, &rowSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_08", "sidex_Variant_Table_Rows", 2);
    testOkay = testOkay && errLog(1 == rowSize, "test_table_08", "rowSize != size", 2);

    //testing WRONG_TYPPE error 
    wrongTypeVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_DeleteRow(wrongTypeVariant, rowId);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_08", "WRONG_TYPE", 1);

    //testing NOCONTENT error
    tabErr = sidex_Variant_Table_DeleteRow(tabHandle, rowId);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_08", "NOCONTENT", 1);

    sidex_Variant_DecRef(wrongTypeVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_DeleteRows for expected behavior, also testing for an error
*
* First a table is created and a column is added, then two rows. The amount of rows is checked, for being 2.
* The rows are deleted and the amount of rows is checked again for being 0.
* An error is tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_09() {
    bool testOkay = true;
    SIDEX_INT32 rowId = NULL;
    SIDEX_INT32 rowSize = NULL;
    SIDEX_VARIANT wrongTypeVariant = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_09", 
                                  "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_09", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a row to the table
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_09", "sidex_Variant_Table_AddRow", 1);
    //add another row
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_09", "sidex_Variant_Table_AddRow", 2);

    //check amount of rows (=2)
    tabErr = sidex_Variant_Table_Rows(tabHandle, &rowSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_09", "sidex_Variant_Table_Rows", 1);
    testOkay = testOkay && errLog(2 == rowSize, "test_table_09", "rowSize != size", 1);

    //delete all rows
    tabErr = sidex_Variant_Table_DeleteRows(tabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_09", "sidex_Variant_Table_DeleteRows", 1);

    //compare the size of the rows after deleting them (=0)
    tabErr = sidex_Variant_Table_Rows(tabHandle, &rowSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_09", "sidex_Variant_Table_Rows", 2);
    testOkay = testOkay && errLog(0 == rowSize, "test_table_09", "rowSize != size", 2);

    //testing WRONG_TYPPE error 
    wrongTypeVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_DeleteRow(wrongTypeVariant, rowId);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_09", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(wrongTypeVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}

/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_GetColumnName for expected behaviour, also testing for errors
*
* First a table is created and a column added. The column is checked and compared
* through Table_GetColumnName. Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_10() {
    bool testOkay = true;
    SIDEX_TCHAR* columnName = NULL;
    SIDEX_VARIANT wrongTypeVariant = SIDEX_HANDLE_TYPE_NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_10", "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_10", "sidex_Variant_Table_AddColumn", 1);

    //normal GetColumnName
    tabErr = sidex_Variant_Table_GetColumnName(tabHandle, 0, &columnName);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_10", "sidex_Variant_Table_GetColumnName", 1);
    testOkay = testOkay && errLog(0 == sdxrt_cmp(COLUMN, columnName), "test_table_10", "columnName != name", 1);

    //testing a WRONG_TYPE error
    wrongTypeVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_GetColumnName(wrongTypeVariant, 0, &columnName);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_10", "WRONG_TYPE", 1);

    // testing a NOCONTENT error
    tabErr = sidex_Variant_Table_GetColumnName(tabHandle, -5, &columnName);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_10", "NOCONTENT", 1);

    sidex_Variant_DecRef(wrongTypeVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_GetField for expected behavior, also testing for errors
*
* A table is created and a column and a row is added. A field is set with a sidex_variant
* handling an integer. The field value is gotten back and compared to the original.
* Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_11() {
    bool testOkay = true;
    SIDEX_VARIANT intVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 rowId = NULL;
    SIDEX_VARIANT tempVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64 intValue = NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_11", "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_11", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a row to the table
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_11", "sidex_Variant_Table_AddRow", 1);

    //set a value in a field
    intVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_SetField(tabHandle, rowId, COLUMN, intVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_11", "sidex_Variant_Table_SetField", 1);

    //get a value of a field
    tabErr = sidex_Variant_Table_GetField(tabHandle, rowId, COLUMN, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_11", "sidex_Variant_Table_GetField", 1);   //transform variant from field into integer
    tabErr = sidex_Variant_As_Integer(tempVariant, &intValue);  //get int value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_11", "sidex_Variant_As_Integer", 1);
    //compare int value with the original one
    testOkay = testOkay && errLog(1234567 == intValue, "test_table_11", "intValue != value", 1);

    //testing a WRONG_TYPE error
    tabErr = sidex_Variant_Table_GetField(tempVariant, rowId, COLUMN, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_11", "WRONG_TYPE", 1);

    //testing a NOCONTENT error
    tabErr = sidex_Variant_Table_GetField(tabHandle, -5, COLUMN, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_11", "NOCONTENT", 1);

    sidex_Variant_DecRef(intVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}

/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_GetRow for expected behavior, also testing for errors
*
* A table is created and a column and a row is added. A field is set with a sidex_variant
* handling an integer. Afterwards with getRow the rows are gotten back from a dictionary.
* First the size of the dict is checked for being 1, then the entry is compared to the original.
* Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_12() {
    bool testOkay = true;
    SIDEX_VARIANT intVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 rowId = NULL;
    SIDEX_VARIANT rowDict = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 dictSize = NULL;
    SIDEX_VARIANT tempVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64 intValue = NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_12", 
                                  "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_12", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a row to the table
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_12", "sidex_Variant_Table_AddRow", 1);

    //set a value in a field
    intVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_SetField(tabHandle, rowId, COLUMN, intVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_12", "sidex_Variant_Table_SetField", 1);

    //normal getRow
    tabErr = sidex_Variant_Table_GetRow(tabHandle, rowId, &rowDict);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_12", "sidex_Variant_Table_GetRow", 1);

    //comparing the dictionary containing the row with the original row
    //it should contain only one entry
    tabErr = sidex_Variant_Dict_Size(rowDict, &dictSize);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_12", "sidex_Variant_Dict_Size", 1);
    //compare the entry with the original
    testOkay = testOkay && errLog(1 == dictSize, "test_table_12", "rowDict-size != size", 1);
    tabErr = sidex_Variant_Dict_Get(rowDict, COLUMN, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_12", "sidex_Variant_Dict_Get", 1);   //transform variant from dict into integer
    tabErr = sidex_Variant_As_Integer(tempVariant, &intValue);  //get int value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_12", "sidex_Variant_As_Integer", 1);
    //compare int value with the original one
    testOkay = testOkay && errLog(1234567 == intValue, "test_table_12", "intValue != value", 1);

    //testing a WRONG_TYPE error
    tabErr = sidex_Variant_Table_GetRow(intVariant, 0, &rowDict);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_12", "WRONG_TYPE", 1);

    // testing a NOCONTENT error
    tabErr = sidex_Variant_Table_GetRow(tabHandle, -5, &rowDict);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_12", "NOCONTENT", 1);

    sidex_Variant_DecRef(intVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}

/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_HasColumn for expected behavior, also testing for errors
*
* A table is created and a column added. Afterwards it is checked with HasColumn if that 
* column is inside. Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_13() {
    bool testOkay = true;
    tabHandle = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_BOOL hasColumn = NULL;
    SIDEX_VARIANT intVariant = SIDEX_HANDLE_TYPE_NULL;

    // testing a NOCONTENT error
    tabErr = sidex_Variant_Table_HasColumn(tabHandle, COLUMN, &hasColumn);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_13", "NOCONTENT", 1);

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_13", "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_13", "sidex_Variant_Table_AddColumn", 1);

    //normal hasColumn, return SIDEX_TRUE
    tabErr = sidex_Variant_Table_HasColumn(tabHandle, COLUMN, &hasColumn);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_13", "sidex_Variant_Table_HasColumn", 1);
    testOkay = testOkay && errLog(SIDEX_TRUE == hasColumn, "test_table_13", "hasColumn != bool", 1);

    //normal hasColumn, returns SIDEX_FALSE
    tabErr = sidex_Variant_Table_HasColumn(tabHandle, KEY, &hasColumn);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_13", "sidex_Variant_Table_HasColumn", 2);
    //compare with original
    testOkay = testOkay && errLog(SIDEX_FALSE == hasColumn, "test_table_13", "hasColumn != bool", 2);

    //testing a WRONG_TYPE error
    intVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_HasColumn(intVariant, COLUMN, &hasColumn);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_13", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(intVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_Rows, also testing errors
*
* First, the sidex_variant tabHandle is set to SIDEX_HANDLE_TYPE_NULL and given to the 
* function call of sidex_Variant_Table_Rows as param sTable, testing for an error.
* A table is created and with sidex_Variant_Table_Rows the size is checked for being 0.
* A column and a row are added and the amount of rows is checked again, now containing one item.
* A sidex_variant handling an integer is created and given as param sTable to the function,
* testing for an error.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_14() {
    bool testOkay = true;
    SIDEX_INT32 rowCount = NULL;
    SIDEX_INT32 rowId = NULL;
    SIDEX_VARIANT wrongTypeVariant = SIDEX_HANDLE_TYPE_NULL;

    tabHandle = SIDEX_HANDLE_TYPE_NULL;
    //testing a NOCONTENT error
    tabErr = sidex_Variant_Table_Rows(tabHandle, &rowCount);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_14", "NOCONTENT", 1);

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_14", "sidex_Variant_Table_Check", 1);

    //normal getting the amount of rows from the table
    tabErr = sidex_Variant_Table_Rows(tabHandle, &rowCount);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_14", "sidex_Variant_Table_Rows", 1);
    testOkay = testOkay && errLog(0 == rowCount, "test_table_14", "rowCount != amount", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_14", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a row to the table
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_14", "sidex_Variant_Table_AddRow", 1);

    //normal getting the amount of rows from the table
    tabErr = sidex_Variant_Table_Rows(tabHandle, &rowCount);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_14", "sidex_Variant_Table_Rows", 2);
    testOkay = testOkay && errLog(1 == rowCount, "test_table_14", "rowCount != amount", 1);

    //testing a WRONG_TYPE error
    wrongTypeVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_Rows(wrongTypeVariant, &rowCount);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_14", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(wrongTypeVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Variant_Table_SetField for expected behavior, also testing errors
*
* First a table is created and a column and a row added. A field in the table is set with an sidex_integer
* value inside a sidex_variant, at the just added row. Afterwards another field is set, at row 5, which was nonexistent until now.
* The amount of rows is checked, it should contain 6 (0-5) now, and both values are extracted from the table and compared to their 
* original values. Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_15() {
    bool testOkay = true;
    SIDEX_INT32 rowId = NULL;
    SIDEX_VARIANT intVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_VARIANT intMinusVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32 rowCount = NULL;
    SIDEX_VARIANT tempVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64 intValue = NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_15", "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a row to the table
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_Table_AddRow", 1);

    //normal setting a field of the table
    intVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_SetField(tabHandle, rowId, COLUMN, intVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_Table_SetField", 1);

    //setting another field and checking whether missing(inbetween) rows will be added
    intMinusVariant = sidex_Variant_New_Integer(-1234567);
    tabErr = sidex_Variant_Table_SetField(tabHandle, 5, COLUMN, intMinusVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_Table_SetField", 1);

    //checking how many rows there are now, should be 6
    tabErr = sidex_Variant_Table_Rows(tabHandle, &rowCount);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_Table_Rows", 1);
    //compare with real amount
    testOkay = testOkay && errLog(6 == rowCount, "test_table_15", "rowCount != amount", 1);

    //get a value of the 'first' field
    tabErr = sidex_Variant_Table_GetField(tabHandle, rowId, COLUMN, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_Table_GetField", 1);   //transform variant from field into integer
    tabErr = sidex_Variant_As_Integer(tempVariant, &intValue);  //get int value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_As_Integer", 1);
    //compare int value with the original one
    testOkay = testOkay && errLog(1234567 == intValue, "test_table_15", "intValue != value", 1);

    //get the value of the second field
    tabErr = sidex_Variant_Table_GetField(tabHandle, 5, COLUMN, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_Table_GetField", 1);
    //transform variant from field into integer
    tabErr = sidex_Variant_As_Integer(tempVariant, &intValue);  //get int value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_15", "sidex_Variant_As_Integer", 1);
    //compare int value with original one
    testOkay = testOkay && errLog(-1234567 == intValue, "test_table_15", "intValue != value", 2);

    //testing a WRONG_TYPE error
    tabErr = sidex_Variant_Table_SetField(intVariant, rowId, COLUMN, tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_15", "WRONG_TYPE", 1);

    //NOCONTENT error
    tabErr = sidex_Variant_Table_SetField(tabHandle, rowId, KEY, intVariant);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_15", "NOCONTENT", 1);

    //WRONG_PARAMETER or UNICODE error
    tabErr = sidex_Variant_Table_SetField(tabHandle, rowId, NULL, intVariant);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_PARAMETER == tabErr || SIDEX_ERR_UNICODE == tabErr, 
                                  "test_table_15", "UNICODE or WRONG_PARAMETER", 1);

    //NOCONTENT error
    tabErr = sidex_Variant_Table_SetField(tabHandle, -3, COLUMN, intVariant);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_15", "NOCONTENT", 1);

    sidex_Variant_DecRef(intMinusVariant);
    sidex_Variant_DecRef(intVariant);
    sidex_Variant_DecRef(tabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Table_Write for expected behavior, also testing for errors
*
* First a table is created and a column and row are added, as well as a sidex_variant handling an integer
* is set into a field of the table. A sidex_document is created. Afterwards the table is written to
* the sidex_document and read again right after. It is compared then to the original table.
* Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_16() {
    bool testOkay = true;
    SIDEX_INT32 rowId = NULL;
    SIDEX_VARIANT intVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_VARIANT tempVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64 intValue = NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_16", "sidex_Variant_Table_Check", 1);

    //normal adding of a column to the table
    tabErr = sidex_Variant_Table_AddColumn(tabHandle, COLUMN);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_16", "sidex_Variant_Table_AddColumn", 1);

    //normal adding of a row to the table
    tabErr = sidex_Variant_Table_AddRow(tabHandle, &rowId);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_16", "sidex_Variant_Table_AddRow", 1);

    //normal setting a field of the table
    intVariant = sidex_Variant_New_Integer(1234567);
    tabErr = sidex_Variant_Table_SetField(tabHandle, rowId, COLUMN, intVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_16", "sidex_Variant_Table_SetField", 1);

    //normal sidex_Create
    tabErr = sidex_Create(DOCUMENT, &docTabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_16", "sidex_Create", 1);

    //normal write to sidex_document
    tabErr = sidex_Table_Write(docTabHandle, GROUP, KEY, tabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_16", "sidex_Table_Write", 1);

    // normal read of the table 
    sidex_Variant_DecRef(tabHandle);
    tabErr = sidex_Table_Read(docTabHandle, GROUP, KEY, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_16", "sidex_Table_Read", 1);

    //compare the table from the sidex_document to the original
    //get a value of the field
    tabErr = sidex_Variant_Table_GetField(tabHandle, rowId, COLUMN, &tempVariant);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_16", "sidex_Variant_Table_GetField", 1);   //transform variant from field into integer
    tabErr = sidex_Variant_As_Integer(tempVariant, &intValue);  //get int value
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_16", "sidex_Variant_As_Integer", 1);
    //compare int value with original one
    testOkay = testOkay && errLog(1234567 == intValue, "test_table_16", "intValue != value", 1);

    //-------normal write is done, the followingtests only errors

    //testing a MISSING_KEY error or UNICODE
    tabErr = sidex_Table_Write(docTabHandle, GROUP, NULL, tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == tabErr || SIDEX_ERR_UNICODE == tabErr, 
                                  "test_table_16", "MISSING_KEY or UNICODE", 1);

    //testing a MISSING_GROUP error or UNICODE
    tabErr = sidex_Table_Write(docTabHandle, NULL, KEY, tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == tabErr || SIDEX_ERR_UNICODE == tabErr, 
                                  "test_table_16", "MISSING_GROUP or UNICODE", 1);

    //testing a WRONG_KEY error
    tabErr = sidex_Table_Write(docTabHandle, GROUP, wKEY, tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == tabErr, "test_table_16", "WRONG_KEY", 1);

    //testing a WRONG_GROUP error
    tabErr = sidex_Table_Write(docTabHandle, wGROUP, KEY, tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == tabErr, "test_table_16", "WRONG_GROUP", 1);

    //testing a WRONG_TYPE error
    //first write a non-Table entry into the sidex_document
    tabErr = sidex_Table_Write(docTabHandle, GROUP, KEY2, intVariant);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_16", "WRONG_TYPE", 1);

    sidex_Variant_DecRef(tabHandle);
    sidex_Free(&docTabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Checks sidex_Table_Read for expected behavior, also testing for errors
*
* First a table and a sidex_document are created. Afterwards the table is written to
* the sidex_document and read again right after. It is compared to the original table.
* Different errors are tested afterwards.
*
* @returns bool testOkay : false on failure of test, true on success.
*/
bool test_table_17() {
    bool testOkay = true;
    SIDEX_INT32 columnCount = NULL;

    //normal creating of a new table
    tabHandle = sidex_Variant_New_Table();
    testOkay = testOkay && errLog(SIDEX_TRUE == sidex_Variant_Table_Check(tabHandle), "test_table_17", "sidex_Variant_Table_Check", 1);

    //normal sidex_Create
    tabErr = sidex_Create(DOCUMENT, &docTabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_17", "sidex_Create", 1);

    //normal write to sidex_document
    tabErr = sidex_Table_Write(docTabHandle, GROUP, KEY, tabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_17", "sidex_Table_Write", 1);

    // normal read of the table 
    sidex_Variant_DecRef(tabHandle);
    tabErr = sidex_Table_Read(docTabHandle, GROUP, KEY, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_17", "sidex_Table_Read", 1);

    //compare the table from the sidex_document to the original
    //normal getting the amount of columns from the table
    tabErr = sidex_Variant_Table_Columns(tabHandle, &columnCount);
    testOkay = testOkay && errLog(SIDEX_SUCCESS == tabErr, "test_table_17", "sidex_Variant_Table_Columns", 1);
    //compare to original
    testOkay = testOkay && errLog(0 == columnCount, "test_table_17", "columnCount != amount", 1);

    //-------normal read is done, the following tests only errors

    sidex_Variant_DecRef(tabHandle);
    //testing a MISSING_KEY error or UNICODE
    tabErr = sidex_Table_Read(docTabHandle, GROUP, NULL, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_KEY == tabErr || SIDEX_ERR_UNICODE == tabErr, 
                                  "test_table_17", "MISSING_KEY or UNICODE", 1);

    //testing a MISSING_GROUP error or UNICODE
    tabErr = sidex_Table_Read(docTabHandle, NULL, KEY, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_MISSING_GROUP == tabErr || SIDEX_ERR_UNICODE == tabErr,
                                  "test_table_17", "MISSING_GROUP or UNICODE", 1);

    //testing a WRONG_KEY error
    tabErr = sidex_Table_Read(docTabHandle, GROUP, wKEY, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_KEY == tabErr, "test_table_17", "WRONG_KEY", 1);

    //testing a WRONG_GROUP error
    tabErr = sidex_Table_Read(docTabHandle, wGROUP, KEY, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_GROUP == tabErr, "test_table_17", "WRONG_GROUP", 1);

    //testing a WRONG_TYPE error
    //first write a non-Table entry into the sidex_document
    tabErr = sidex_Boolean_Write(docTabHandle, GROUP, KEY2, SIDEX_FALSE);
    tabErr = sidex_Table_Read(docTabHandle, GROUP, KEY2, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_WRONG_TYPE == tabErr, "test_table_17", "WRONG_TYPE", 1);

    //testing a NOCONTENT error
    tabErr = sidex_List_Read(docTabHandle, GROUP2, KEY, &tabHandle);
    testOkay = testOkay && errLog(SIDEX_ERR_NOCONTENT == tabErr, "test_table_17", "NOCONTENT", 1);

    sidex_Free(&docTabHandle);
    return testOkay;
}


/** @ingroup Sidex_Table_Tests
* @brief Calls all subtests
* 
* @param int test: If 0- all subtests are executed, if 1-17- that one subtest is executed, returns false otherwise.
* @param bool stop: If true- when a subtest fails, the test returns immediately. If false- all subtests are executed.
* Different functions are tested, see test_table01-test_table17 for more details.
*
* @returns bool testsOkay : false on failure of subtest, true on success.
*/
bool test_sidex_table(int test, bool stop) {
    bool testsOkay = true;

    switch(test) {
    case 0:
    case 1:
        //test_table_01
        testsOkay = errLog(test_table_01(), "test_table_01");
        if(stop && !testsOkay) {
            break;
        }
        if(test == 1) {
            break;
        }
    
    case 2:
        //test_table_02
        testsOkay = errLog(test_table_02(), "test_table_02") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 2) {
            break;
        }

    case 3:
        //test_table_03
        testsOkay = errLog(test_table_03(), "test_table_03") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 3) {
            break;
        }

    case 4:
        //test_table_04
        testsOkay = errLog(test_table_04(), "test_table_04") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 4) {
            break;
        }
    
    case 5:
        //test_table_05
        testsOkay = errLog(test_table_05(), "test_table_05") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 5) {
            break;
        }

    case 6:
        //test_table_06
        testsOkay = errLog(test_table_06(), "test_table_06") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 6) {
            break;
        }

    case 7:
        //test_table_07
        testsOkay = errLog(test_table_07(), "test_table_07") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 7) {
            break;
        }

    case 8:
        //test_table_08
        testsOkay = errLog(test_table_08(), "test_table_08") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 8) {
            break;
        }
    case 9:
        //test_table_09
        testsOkay = errLog(test_table_09(), "test_table_09") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 9) {
            break;
        }
    case 10:
        //test_table_10
        testsOkay = errLog(test_table_10(), "test_table_10") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 10) {
            break;
        }
    case 11:
        //test_table_11
        testsOkay = errLog(test_table_11(), "test_table_11") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 11) {
            break;
        }
    case 12:
        //test_table_12
        testsOkay = errLog(test_table_12(), "test_table_12") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 12) {
            break;
        }
    case 13:
        //test_table_13
        testsOkay = errLog(test_table_13(), "test_table_13") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 13) {
            break;
        }
    case 14:
        //test_table_14
        testsOkay = errLog(test_table_14(), "test_table_14") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 14) {
            break;
        }
    case 15:
        //test_table_15
        testsOkay = errLog(test_table_15(), "test_table_15") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 15) {
            break;
        }
    case 16:
        //test_table_16
        testsOkay = errLog(test_table_16(), "test_table_16") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 16) {
            break;
        }
    case 17:
        //test_table_17
        testsOkay = errLog(test_table_17(), "test_table_17") && testsOkay;
        if(stop && !testsOkay) {
            break;
        }
        if(test == 17) {
            break;
        }
    }
    return testsOkay;
}