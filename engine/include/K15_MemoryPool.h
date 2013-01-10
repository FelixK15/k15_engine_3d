/**
 * @file K15_DynamicMemoryPool.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/06
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

#ifndef __K15_DYNAMICMEMORYPOOL__
#define __K15_DYNAMICMEMORYPOOL__

#include "K15_StdInclude.h"
#include "K15_MemoryPoolNode.h"

namespace K15_EngineV2
{
	template <class T,U32 CAPACITY>
	class MemoryPool
	{
	public:
		MemoryPool();
		~MemoryPool();

		T* Allocate();
		T* Allocate(U32 iNumberObjects);

		void Free(void *pPointer);
		void Clear();

		U32 GetUsedMemory() const;
		U32 GetFreeMemory() const;
		U32 GetTotalMemory() const;

	private:
		void _CreateNodes();

	private:
		static const U32 ms_iSizePerMemoryNode;
		MemoryPoolNode *m_pFreeNode;
		byte *m_pMemory;

		U32 m_iSize;
		U32 m_iUsedMemory;
	};

	#include "../src/K15_MemoryPool.inl"
}


#endif //__K15_DYNAMICMEMORYPOOL__