/* 
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2016 wobe-systems GmbH
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
#include <iostream>
using namespace std;
#include <sidex.h>
#include <tmlCore.h>
#include "TestingForReturns.h"
#include "TmlList.h"
#include "tmlrt_Utils.h"

TmlList::TmlList(SIDEX_TCHAR* name) {
	m_sidexList = SIDEX_HANDLE_TYPE_NULL;
	m_errorLocationOutput = NULL;
	m_sidexList = sidex_Variant_New_List();
	setErrorLocationOutput(name);
}

TmlList::~TmlList() {
	if(isSidexListCreated()) {
		sidex_Variant_DecRef(m_sidexList);
	}
	if(NULL != m_errorLocationOutput) {
		delete[] m_errorLocationOutput;
	}
}

SIDEX_INT32 TmlList::size() {
	m_iErr = sidex_Variant_List_Size(m_sidexList, &m_amount);
	checkForSuccess();
	return m_amount;
}

SIDEX_INT32 TmlList::append(SIDEX_TCHAR* string) {
	SIDEX_VARIANT item = SIDEX_HANDLE_TYPE_NULL;
	SIDEX_INT32 itemPosition = 0;
	item = formatStringToSidexVariant(string);
	m_iErr = sidex_Variant_List_Append(m_sidexList, item, &itemPosition);
	checkForSuccess();
	sidex_Variant_DecRef(item);
	return itemPosition;
}

SIDEX_VARIANT TmlList::formatStringToSidexVariant(SIDEX_TCHAR* string) {
	SIDEX_VARIANT item = SIDEX_HANDLE_TYPE_NULL;
	m_iErr = sidex_Variant_New_String(string, &item);
	checkForSuccess();
	return item;	//careful. item has to be 'decRef'ed
}

SIDEX_INT32 TmlList::append(SIDEX_BOOL boolean) {
	SIDEX_VARIANT item = SIDEX_HANDLE_TYPE_NULL;
	SIDEX_INT32 itemPosition = 0;
	item = sidex_Variant_New_Boolean(boolean);
	m_iErr = sidex_Variant_List_Append(m_sidexList, item, &itemPosition);
	checkForSuccess();
	sidex_Variant_DecRef(item);
	return itemPosition;
}

SIDEX_INT32 TmlList::setBool(SIDEX_INT32 index, SIDEX_BOOL boolean) {
	SIDEX_VARIANT item = SIDEX_HANDLE_TYPE_NULL;
	item = sidex_Variant_New_Boolean(boolean);
	m_iErr = sidex_Variant_List_Set(m_sidexList, item, index);
	checkForSuccess();
	sidex_Variant_DecRef(item);
	return m_iErr;
}

SIDEX_TCHAR* TmlList::getString(SIDEX_INT32 index) {
	SIDEX_VARIANT item = SIDEX_HANDLE_TYPE_NULL;
	m_iErr = sidex_Variant_List_Get(m_sidexList, index, &item);
	checkForSuccess();

	SIDEX_INT32 lengthOfString;
	SIDEX_TCHAR* stringListItem;
	m_iErr = sidex_Variant_As_String(item, &stringListItem, &lengthOfString);
	checkForSuccess();
	return stringListItem;
}

SIDEX_BOOL TmlList::getBool(SIDEX_INT32 index) {
	SIDEX_VARIANT item = SIDEX_HANDLE_TYPE_NULL;
	m_iErr = sidex_Variant_List_Get(m_sidexList, index, &item);
	checkForSuccess();

	SIDEX_BOOL boolListItem;
	m_iErr = sidex_Variant_As_Boolean(item, &boolListItem);
	checkForSuccess();
	return boolListItem;
}

void TmlList::setErrorLocationOutput(SIDEX_TCHAR* outputInCaseOfError) {
#if defined SIDEX_UNICODE || defined TML_UNICODE
	m_errorLocationOutput = new wchar_t[60]();
#else
	m_errorLocationOutput = new char[60]();
#endif
	tmlrt_cat(m_errorLocationOutput, outputInCaseOfError);
	tmlrt_cat(m_errorLocationOutput, tmlrtT(" - TmlList"));
}

bool TmlList::isSidexListCreated() {
	return (m_sidexList != SIDEX_HANDLE_TYPE_NULL);
}