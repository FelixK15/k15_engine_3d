/**
 * @file K15_Pointer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/07
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

#ifndef __K15Engine_System_Memory_Pointer_h_
#define __K15Engine_System_Memory_Pointer_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace System { namespace Memory {

	template<class T>
	class Pointer
	{
	public:
		Pointer();
		Pointer(T *pObject);
		Pointer(const Pointer<T> &pPointer);

		~Pointer();

		const Pointer<T> &operator=(const Pointer<T> &pPointer);
		const Pointer<T> &operator=(T* pObject);
		
		operator T*() const;
		T* operator->() const;
		T& operator*() const;

		bool operator==(T* pObject);
		bool operator!=(T* pObject);

		bool operator==(const Pointer<T> &pPointer);
		bool operator!=(const Pointer<T> &pPointer);

	private:
		void _GetAndIncreasePointer(const Pointer<T> &pPointer);

	private:
		T* m_pObject;
	};
	#include "K15_Pointer.inl"
}}} //end of K15_Engine::System::Memory namespace

#endif //__K15_POINTER__