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
#include "sdxrt_test_get_version.h"


bool test_sidex_get_version() {
	bool success = true;
	SIDEX_INT32 apiVers = 0;
	SIDEX_INT32 libVers = 0;
	SIDEX_TCHAR* verStr;

	sidex_Get_Version(&apiVers, &libVers, &verStr);

	if (DEFAULT_API_VERSION != apiVers) {
		success = false;
		wcout << "API-Version: " << DEFAULT_API_VERSION << " " << apiVers << endl;
	}

	if (DEFAULT_LIB_VERSION != libVers) {
		success = false;
		wcout << "LIB-Version " << DEFAULT_LIB_VERSION << " " << libVers << endl;
	}
		

	if (0 != sdxrt_cmp(verStr, VERSION)) {
		success = false;
		wcout << verStr << " " << VERSION<< endl;
	}
		

	if (!success) {
		wcout << "sidex_Get_Version failed" << endl;

	}
	return success;
}