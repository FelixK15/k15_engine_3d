/**
 * @file K15_Object.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/07
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
 *
 * @section DESCRIPTION
 *
 * 
 */
#pragma once

#ifndef __K15_OBJECT__
#define __K15_OBJECT__

#include "K15_UsefulMacros.h"
#include "K15_DynamicArray.h"
#include "K15_HashMap.h"
#include "K15_Rtti.h"
#include "K15_Stream.h"
#include "K15_Pointer.h"

namespace K15_EngineV2
{
	class Object;

	typedef U32 ObjectID;
	typedef Pointer<Object> ObjectPtr;

	class K15ENGINE2_API Object
	{

	public:
		Object();
		virtual ~Object();
	
		void IncreaseReferences();
		void DecreaseReferences();
		
		bool IsInstanceOf(const Rtti &rtType) const;
		bool IsInstanceOf(const Object *pObject) const;

		bool IsDerivedFrom(const Rtti &rtType) const;
		bool IsDerivedFrom(const Object *pObject) const;

		U32 GetReferenceCount() const;
		ObjectID GetUniqueID() const;

		void SetName(const String& sName);
		const String &GetName() const;
		
		virtual const Rtti& GetType() const;

// 		virtual void Register(Stream &stStream) const;
// 		virtual void Save(Stream &stStream) const;
// 		virtual bool Load(Stream &stStream);

		//virtual XMLTree SaveToXML() const;

// 		virtual String ToString() const = 0;
// 		virtual size_t GetDiscUsed() const = 0;
// 		virtual size_t GetMemoryUsed() const = 0;

	public:
		static void PrintObjectsInUse();

		static Object *GetObjectByID(ObjectID iID);
		static Object *GetObjectByName(const String &sName);

		static void GetObjectsByName(const String &sName,DynamicArray<Object*> &arrObjectsOut);

	private:
		void _SetID();

		void _RegisterObject();
		void _UnregisterObject();

	public:
		static Rtti TYPE;
		//static HashMap<U32,Object*> ms_hmObjectsInUse;
		//static HashMap<String,FactoryFunction> ms_hmFactoryFunctions;
		static ObjectID ms_iNextID;

	private:
		ObjectID m_iUniqueID;
		U32 m_iReferences;

		String m_sName;
	};

	#include "../src/K15_Object.inl"
}

#endif //__K15_OBJECT__