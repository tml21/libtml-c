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

#include "sidexBoolean.h"
#ifdef USESTREAMS
#include <sstream>
using namespace std;
#endif // USESTREAMS

/**
 * @brief    Constructor.
 */
sidexBoolean::sidexBoolean()
{
   value = 0;
   incRef();
}


/**
 * @brief    Constructor with initial value.
 */
sidexBoolean::sidexBoolean(bool newValue)
{
   value = newValue;
   incRef();
}


/**
 * @brief    Destructor.
 */
sidexBoolean::~sidexBoolean()
{
	cleanUp();
}


/**
 * @brief    Cleans up refCounter dependent allocations
 */
void sidexBoolean::cleanUp(){
  if (getRef())
    decRef();
}


/**
 * @brief   Get type of this data object.
 */
SIDEX_DATA_TYPE sidexBoolean::getType()
{
   return type;
}


/**
 * @brief   Get the string representation of this data object.
 */
char* sidexBoolean::toString()
{
#ifdef USESTREAMS
  stringstream  stream;
  stream << value << ends;

  sReturnString = stream.str();
  return (char*)sReturnString.data();
#else // USESTREAMS
  if (value){
    return (char*) "1";
  }
  else{
    return (char*) "0";
  }
#endif // USESTREAMS
}


/**
 * @brief   Set the boolean value of this object.
 */
void sidexBoolean::setValue(bool newValue)
{
   value = newValue;
}


/**
 * @brief   Get the boolean value of this object.
 */
bool sidexBoolean::getValue()
{
   return value;
}
