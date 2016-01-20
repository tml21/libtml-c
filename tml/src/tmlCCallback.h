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
 
#if !defined(CALLBACK_H)
#define CALLBACK_H

#include <stdio.h>

class CCallback
{
	public:
		virtual bool Execute(void *Param) const =0;
};


template <class cInstanceOfTheCallbackFunctionsClass>
class TCallback : public CCallback // Inheriting
{
	public:
		TCallback() // constructor
		{
			// Important : zeroing the pointer so we later can check for errors
			pFunction = 0;
      cInst = NULL;
		}

		// (You can change the callback to take more parameters or to return something)
		typedef bool (cInstanceOfTheCallbackFunctionsClass::*tFunction)(void *Param);
		
		// Execute the Callback
		virtual bool Execute(void *Param) const 
		{
			if (pFunction) return (cInst->*pFunction)(Param);
			else printf("ERROR : the callback function has not been defined !!!!\n");
			// an undefined callback function will crash the program if you don't check here !
			return false;
		}

		// Set or change the Callback
		void SetCallback (cInstanceOfTheCallbackFunctionsClass  *cInstancePointer, 
						  tFunction   pFunctionPointer)
		{
			cInst     = cInstancePointer;
			pFunction = pFunctionPointer;
		}

	private:
		cInstanceOfTheCallbackFunctionsClass  *cInst;
		tFunction  pFunction;
};

#endif // CALLBACK_H




