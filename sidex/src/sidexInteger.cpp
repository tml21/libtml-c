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
 
#include "sidexInteger.h"
#include <sstream>
#include <stdio.h>

using namespace std;

/**
 * @brief    Constructor.
 */
sidexInteger::sidexInteger()
{
#ifndef USESTREAMS
   m_strRepresentation = new char[64];
#endif // USESTREAMS
   value = 0;
   incRef();
}


/**
 * @brief    Constructor with initial value.
 */
sidexInteger::sidexInteger(SIDEX_INT64 newValue)
{
#ifndef USESTREAMS
   m_strRepresentation = new char[64];
#endif // USESTREAMS

   value = newValue;
   incRef();
}


/**
 * @brief    Destructor.
 */
sidexInteger::~sidexInteger()
{
  cleanUp();
}


/**
 * @brief    Cleans up refCounter dependent allocations
 */
void sidexInteger::cleanUp(){
  if (getRef()){
    if (decRef() == 0){
#ifndef USESTREAMS
      if (NULL != m_strRepresentation){
        delete []m_strRepresentation;
        m_strRepresentation = NULL;
      }
#endif // USESTREAMS
    }
  }
}


/**
 * @brief   Get type of this data object.
 */
SIDEX_DATA_TYPE sidexInteger::getType()
{
   return type;
}


/**
 * @brief   Get the string representation of this data object.
 */
char* sidexInteger::toString()
{
#ifdef USESTREAMS
  stringstream  stream;
  stream << value << ends;

  sReturnString = stream.str();
  return (char*)sReturnString.data();
#else // USESTREAMS
#if defined(LINUX) || defined (MINGW_BUILD)
  sprintf(m_strRepresentation, "%lld", (long long) value);
#else // LINUX
  sprintf_s(m_strRepresentation, 64, "%lld", value);
#endif // LINUX
  return m_strRepresentation;
#endif // USESTREAMS
}


/**
 * @brief   Set the integer value of this object.
 */
void sidexInteger::setValue(SIDEX_INT64 newValue)
{
   value = newValue;
}


/**
 * @brief   Get the integer value of this object.
 */
SIDEX_INT64 sidexInteger::getValue()
{
   return value;
}
