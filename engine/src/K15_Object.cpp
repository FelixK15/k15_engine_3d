/**
 * @file K15_Object.cpp
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/09
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_Object.h"

using namespace K15_EngineV2;

Rtti Object::TYPE = Rtti("K15_EngineV2.Object",NULL);
ObjectID Object::ms_iNextID = 0;

//HashMap<U32,Object*> Object::ms_hmObjectsInUse;
//HashMap<String,FactoryFunction> Object::ms_hmFactoryFunctions;

Object::Object()
{
	_SetID();
	_RegisterObject();
}

Object::~Object()
{
	_UnregisterObject();
}

Object *Object::GetObjectByName(const String &sName)
{
	Object *pDesiredObject = NULL;
	
// 	for(U32 i = 0;i < ms_hmObjectsInUse.BucketSize();++i){
// 		HashItem<ObjectID,Object*> *pCurrentItem = ms_hmObjectsInUse.GetBucketItem(i);
// 		while(pCurrentItem){
// 			Object *pCurrentObject = pCurrentItem->GetValue();
// 			if(pCurrentObject->GetName() == sName){
// 				pDesiredObject = pCurrentObject;
// 				break;
// 			}
// 
// 			pCurrentItem = pCurrentItem->GetNext();
// 		}
// 	}

	return pDesiredObject;
}

void Object::GetObjectsByName(const String &sName,DynamicArray<Object*> &arrObjectsOut)
{
// 	for(U32 i = 0;i < ms_hmObjectsInUse.BucketSize();++i){
// 		HashItem<ObjectID,Object*> *pCurrentItem = ms_hmObjectsInUse.GetBucketItem(i);
// 		while(pCurrentItem){
// 			Object *pCurrentObject = pCurrentItem->GetValue();
// 			if(pCurrentObject->GetName() == sName){
// 				arrObjectsOut.PushBack(pCurrentObject);
// 			}
// 
// 			pCurrentItem = pCurrentItem->GetNext();
// 		}
// 	}
}