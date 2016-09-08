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

#include "tmlrt_GetVersion.h"
#include "TestParams.h"

bool testTmlGetVersion() {
	bool success = true;
	TML_INT32 apiVers = 0;
	TML_INT32 libVers = 0;
	SIDEX_TCHAR* verStr;

	tml_Core_Get_Version(&apiVers, &libVers, &verStr);

	if (DEFAULT_APIVERSION != apiVers) {
		success = false;
		wcout << "API-Version: " << DEFAULT_APIVERSION << " " << apiVers << endl;
	}
		

	if (DEFAULT_LIBVERSION != libVers) {
		success = false;
		wcout << "LIB-Version: " << DEFAULT_LIBVERSION << " " << libVers << endl;
	}
		

	if (0 != tmlrt_cmp(verStr, S_TD_VERSION)) {
		success = false;
		wcout << verStr << " vs " << S_TD_VERSION << endl;
	}
		
	if (!success) {
		wcout << "tml_Core_Get_Version failed" << endl;
	}
	else {
		wcout << "tml_Core_Get_Version success" << endl;
	}
	return success;
}