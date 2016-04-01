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
#include "tmlrt_Utils.h"

void TestingForReturns::errorOutput() {
	wcout << "Test failed at " << m_errorLocationOutput << " with ErrorCode " << m_iErr << endl;
	m_testOK = false;
}

void TestingForReturns::checkForExpectedReturnCode(TML_INT32 expectedReturnCode) {
	if(expectedReturnCode != m_iErr) {
		errorOutput();
	}
}

void TestingForReturns::checkForSuccess() {
	if(TML_SUCCESS != m_iErr) {
		errorOutput();
	}
}

TestingForReturns::TestingForReturns() {
	m_testOK = true;
	m_iErr = 0;
	m_errorLocationOutput = NULL;
}

