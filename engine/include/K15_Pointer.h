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
#ifndef _K15Engine_Core_Memory_Pointer_h_
#define _K15Engine_Core_Memory_Pointer_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core {

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
		void _getAndIncreasePointer(const Pointer<T> &pPointer);

	private:
		T* m_pObject;
	};
	#include "K15_Pointer.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Memory_Pointer_h_