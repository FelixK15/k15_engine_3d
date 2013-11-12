/**
 * @file K15_PoolAllocator.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/12
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

#ifndef _K15Engine_Core_PoolAllocator_h_
#define _K15Engine_Core_PoolAllocator_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_BaseAllocator.h"

namespace K15_Engine { namespace Core {
	template<class T>
	class K15_CORE_API PoolAllocator : public BaseAllocator
	{
	public:
		PoolAllocator(uint32 p_ObjectCount, const ObjectName& p_Name);
		PoolAllocator(BaseAllocator* p_Allocator,uint32 p_ObjectCount, const ObjectName& p_Name);
		virtual ~PoolAllocator();

		virtual void* alloc(uint32 p_Size) OVERRIDE;
		virtual void free(void* p_Pointer) OVERRIDE;

	private:
		uint32 m_Position;
	};// end of PoolAllocator class declaration
#	include "K15_PoolAllocator.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_PoolAllocator_h_