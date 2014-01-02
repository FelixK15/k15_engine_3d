/**
 * @file K15_MemoryPools.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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
 * This class holds various memory pools that will get used to manage allocation/deallocation of frequently allocated objects
 */

#ifndef _K15Engine_Core_MemoryPools_h_
#define _K15Engine_Core_MemoryPools_h_

#ifndef K15_USE_PRECOMPILED_HEADER
# include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_BlockAllocator.h"
#include "K15_PoolAllocator.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
// 	class K15_CORE_API AlphaStateAllocator : public Singleton<AlphaStateAllocator>, public PoolAllocator<200>, public MemoryPoolsAllocatedObject
// 	{
// 	public:
// 		/*********************************************************************************/
// 		static const uint32 AlphaStateCount;
// 		/*********************************************************************************/
// 		AlphaStateAllocator();
// 		virtual ~AlphaStateAllocator();
// 	}; //end of RenderOperationAllocator class declaration
// 	/*********************************************************************************/
	class K15_CORE_API RenderOperationAllocator : public Singleton<RenderOperationAllocator>, public PoolAllocator<K15_SIZE_RENDER_OPERATION>, public MemoryPoolsAllocatedObject
	{
	public:
		/*********************************************************************************/
		static const uint32 RenderOperationCount;
		/*********************************************************************************/
		RenderOperationAllocator();
		virtual ~RenderOperationAllocator();
	}; //end of RenderOperationAllocator class declaration
	/*********************************************************************************/
	class K15_CORE_API STLBlockAllocator : public Singleton<STLBlockAllocator>, public BlockAllocator, public MemoryPoolsAllocatedObject
	{
	public:
		/*********************************************************************************/
		static const uint32 STLBlockAllocatorSize;
		/*********************************************************************************/
		STLBlockAllocator();
		virtual ~STLBlockAllocator();
	}; // end of STLBlockAllocator class declaration
	/*********************************************************************************/
	class K15_CORE_API MemoryBlockAllocator : public Singleton<MemoryBlockAllocator>, public PoolAllocator<K15_SIZE_MEMORY_BLOCK>, public MemoryPoolsAllocatedObject
	{
	public:
		/*********************************************************************************/
		static const uint32 MemoryBlockCount;
		/*********************************************************************************/
		MemoryBlockAllocator();
		virtual ~MemoryBlockAllocator();
	}; // end of MemoryBlockAllocator class declaration
	/*********************************************************************************/
	class K15_CORE_API MaterialAllocator : public Singleton<MaterialAllocator>, public PoolAllocator<K15_SIZE_MATERIAL>, public MemoryPoolsAllocatedObject
	{
	public:
		/*********************************************************************************/
		static const uint32 MaterialCount;
		/*********************************************************************************/
		MaterialAllocator();
		virtual ~MaterialAllocator();
	}; // end of MaterialAllocator class declaration
	/*********************************************************************************/
	class K15_CORE_API MemoryPools : public Singleton<MemoryPools>, public StackAllocator, public ApplicationAllocatedObject
	{
	public:
		MemoryPools();
		virtual ~MemoryPools();

	private:
		MaterialAllocator* m_MaterialAllocator;
		MemoryBlockAllocator* m_MemoryBlockAllocator;
		STLBlockAllocator* m_STLBlockAllocator;
		RenderOperationAllocator* m_RopAllocator;
	}; // end of MemoryPools class declaration
	/*********************************************************************************/
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_MemoryPools_h_