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

#ifndef _K15Engine_Core_Object_h_
#define _K15Engine_Core_Object_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#include "K15_Prerequisites.h"
#include "K15_HashedString.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_Rtti.h"
#include "K15_Pointer.h"

namespace K15_Engine { namespace Core { 

	class K15_CORE_API Object
	{
		K15_DECLARE_RTTI;

	public:
		Object();
		Object(const ObjectName& p_Name);

		virtual ~Object();
	
		INLINE static const TypeName& getTypeName();

		INLINE static bool isInstanceOf(const Rtti& p_Type);
		INLINE static bool isInstanceOf(const Object* p_Object);

		INLINE static bool isDerivedFrom(const Rtti& p_Type);
		INLINE static bool isDerivedFrom(const Object* p_Object);
		
		INLINE void setName(const ObjectName& p_Name);
		INLINE const ObjectName& getName() const;
		
	protected:
		ObjectName m_Name;
	};
  
	K15_PTR(Object);
   
	#include "K15_Object.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Object_h_