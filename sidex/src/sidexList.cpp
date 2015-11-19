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

#include "sidexList.h"
#include "sidexTypeBase.h"
#include "sidexUtils.h"
#include "sidex.h"

/**
 * @brief    Constructor.
 */
sidexList::sidexList()
{
   first = last = NULL;
   incRef();
}


/**
 * @brief    Destructor.
 */
sidexList::~sidexList()
{
	cleanUp();
}


/**
 * @brief    Cleans up refCounter dependent allocations
 */
void sidexList::cleanUp(){
  if (decRef() == 0){
    listElement* l = last;
    while (NULL != l){
      listElement* lFree = l;
      // Referenz auf das Variant- Objekt zurueck nehmen:
      sidexutil_Variant_DecRef(l->value);
      l = l->prev;
      delete (lFree);
    }
    first = last = NULL;
  }
}


/**
 * @brief   Get type of this data object.
 */
SIDEX_DATA_TYPE sidexList::getType()
{
   return type;
}


/**
 * @brief   Get the first listElement object.
 */
sidexList::listElement * sidexList::getFirst()
{
   return first;
}


/**
 * @brief   Get the last listElement object.
 */
sidexList::listElement * sidexList::getLast()
{
   return last;
}


/**
 * @brief   Add an object of type SIDEX_VARIANT to the list.
 */
void sidexList::addElement(SIDEX_VARIANT val)
{
   ((sidexBase*)val)->incRef();
   listElement *newElement = new listElement;
   newElement->value = val;
   if (NULL == last)
   {
      first = newElement;
      last = newElement;
      newElement->next = NULL;
      newElement->prev = NULL;
   } 
   else
   {
      last->next = newElement;
      newElement->prev = last;
      newElement->next = NULL;
      last = newElement;
   }
}


/**
 * @brief   Remove the object out of the list at the given position.
 */
int sidexList::DeleteItem(int pos)
{
   int iRet = SIDEX_SUCCESS;
   int iLength = length();

   if (pos < 0){
     iRet = SIDEX_ERR_WRONG_PARAMETER;
   }
   else{
     if (pos > (iLength - 1)){
       iRet = SIDEX_ERR_NOCONTENT;
     }
     else{
       int i = 0;
       listElement* tmp = NULL;
       listElement* lastElement = NULL;
       listElement* actElement = first;
       while (i < pos && NULL != actElement->next)
       {
          lastElement = actElement;
          actElement = actElement->next;
          i++;
       }
       if (i == pos){
          tmp = actElement->next;
          
          if (0 == pos){
            first = tmp;
          }
          else{
            lastElement->next = tmp;
          }
          if (NULL != tmp){
            // It's not the last one:
            tmp->prev = lastElement;
          }
          if ((iLength - 1) == pos){
            if (1 == iLength){
              last = NULL;
            }
            else{
              last = lastElement;
            }
          }
          listElement* lFree = actElement;
          // Referenz auf das Variant- Objekt zurueck nehmen:
          sidexutil_Variant_DecRef(actElement->value);
          delete (lFree);
       }
       else{
           iRet = SIDEX_ERR_NOCONTENT;
       }
     }
   }
   return iRet;
}


/**
 * @brief   Removes all objects out of a list.
 */
void sidexList::Clear()
{
  int iLength = length();
  for (int i = iLength-1; i >= 0; --i){
    DeleteItem(i);
  }
}


/**
 * @brief   Remove an SIDEX_VARIANT object out of the list.
 */
int sidexList::deleteVariant(SIDEX_VARIANT refElement)
{
   int iRet = SIDEX_SUCCESS;
   int iLength = length();

   if (SIDEX_HANDLE_TYPE_NULL == refElement){
     iRet = SIDEX_ERR_WRONG_PARAMETER;
   }
   else{
     int pos = 0;
     listElement* tmp = NULL;
     listElement* lastElement = first;
     listElement* actElement = first;

     bool bFound = false;
     while (pos < iLength && !bFound)
     {
       if (actElement->value == refElement){
         bFound = true;
       }
       else{
        lastElement = actElement;
        actElement = actElement->next;
        pos++;
       }
     }
     if (bFound){
        tmp = actElement->next;
        
        if (0 == pos){
          first = tmp;
        }
        else{
          lastElement->next = tmp;
        }
        if (NULL != tmp){
          // It's not the last one:
          if (0 == pos){
            tmp->prev = NULL;
          }
          else{
            tmp->prev = lastElement;
          }
        }
        if ((iLength - 1) == pos){
          if (0 == (iLength - 1)){
            last = NULL;
          }
          else{
            last = lastElement;
          }
        }
        listElement* lFree = actElement;
        // Referenz auf das Variant- Objekt zurueck nehmen:
        sidexutil_Variant_DecRef(actElement->value);
        delete (lFree);
     }
     else{
       iRet = SIDEX_ERR_NOCONTENT;
     }
   }
   return iRet;
}


/**
 * @brief   Add an object of type SIDEX_VARIANT to the list before the given position.
 */
int sidexList::addElement(SIDEX_VARIANT val, int pos)
{
  int iRet = SIDEX_SUCCESS;

   if (pos < 0){
     iRet = SIDEX_ERR_WRONG_PARAMETER;
   }
   else{
     int iLength = length();
     if (0 == iLength || iLength <= pos){
       // add at the tail:
       addElement(val);
     }
     else{
       ((sidexBase*)val)->incRef();
       listElement *newE = new listElement;
       newE->value = val;

       int i = 0;
       listElement* lastElement = NULL;
       listElement* actElement = first;
       while (i < pos && NULL != actElement->next)
       {
          lastElement = actElement;
          actElement = actElement->next;
          i++;
       }
       actElement->prev = newE;
       newE->next = actElement;
       if (0 < pos){
         lastElement->next = newE;
         newE->prev = lastElement;
       }
       else{
         newE->prev = NULL;
         first = newE;
       }
     }
   }
   return iRet;
}


/**
 * @brief   Get the object of type SIDEX_VARIANT  at index pos (base 0).
 */
int sidexList::getElement(SIDEX_VARIANT* val, int pos)
{
   int iRet;
   int i = 0;

   if (NULL == first || 0 > pos)
     return SIDEX_ERR_NOCONTENT;

   listElement e = *first;

   *val = SIDEX_HANDLE_TYPE_NULL;
   
   bool bEmpty = false;
   while (i++ < pos && !bEmpty)
   {
     if (NULL != e.next){
       e = *e.next;
     }
     else
       bEmpty = true;
   }
   if (bEmpty){
     *val = SIDEX_HANDLE_TYPE_NULL;
      iRet = SIDEX_ERR_NOCONTENT;
   }
   else{
     *val = e.value;
      iRet = SIDEX_SUCCESS;
   }
   return iRet;
}

/**
 * @brief   Get the number of list entrys.
 */
int sidexList::length()
{
   int iCount = 0;
   if (NULL != first){
     listElement e = *first;

     bool bEmpty = false;
     ++iCount;
     while (!bEmpty)
     {
       if (NULL != e.next){
         e = *e.next;
         ++iCount;
       }
       else{
         bEmpty = true;
       }
     }
   }
   return iCount;
}
