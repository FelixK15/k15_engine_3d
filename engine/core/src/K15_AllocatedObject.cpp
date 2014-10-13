/**
 * @file K15_AllocatedObject.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/06
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

#include "K15_PrecompiledHeader.h"

#include "K15_AllocatedObject.h"
#include "K15_BlockAllocator.h"
#include "K15_PoolAllocator.h"

#include "K15_IniFileParser.h"

#include "K15_MemoryBlock.h"
#include "K15_ProfilingNode.h"
#include "K15_RenderOperation.h"
#include "K15_Vertex.h"

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  uint32 BaseAllocatedObject::CoreAllocatorSizes::GeneralAllocatorSize = size_megabyte(5);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::ModuleAllocatorSize = size_megabyte(1);
#if defined K15_DEBUG
  uint32 BaseAllocatedObject::CoreAllocatorSizes::DebugAllocatorSize = size_megabyte(10);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::ProfilingAllocatorSize = size_megabyte(9);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::ProfilingNodePoolCount = 128;
#endif //K15_DEBUG
  uint32 BaseAllocatedObject::CoreAllocatorSizes::RenderAllocatorSize = size_megabyte(15);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::GameEventAllocatorSize = size_megabyte(1);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::ThreadingAllocatorSize = size_megabyte(1);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::TaskAllocatorSize = size_megabyte(1);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::LoggingAllocatorSize = size_megabyte(1);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::ResourceAllocatorSize = size_megabyte(5);
  uint32 BaseAllocatedObject::CoreAllocatorSizes::VertexPoolCount = 10000;
  uint32 BaseAllocatedObject::CoreAllocatorSizes::RenderOperationPoolCount = 512;
  /*********************************************************************************/

  /*********************************************************************************/
  BaseAllocatedObject::AllocatorArray BaseAllocatedObject::Allocators;
#if defined K15_DEBUG
  uint32 BaseAllocatedObject::MemorySize = size_megabyte(45); //Default - Debug is 150MB
#else
  uint32 BaseAllocatedObject::MemorySize = size_megabyte(30); //Default is 100MB
#endif //K15_DEBUG
  uint32 BaseAllocatedObject::AllocatorCount = 0;
  /*********************************************************************************/

	/*********************************************************************************/
	void BaseAllocatedObject::createCoreAllocators()
	{
		BaseAllocator* CoreAllocator = new StackAllocator(MemorySize,"CoreAllocator");

		Allocators[AC_CORE] = CoreAllocator;
		AllocatorCount = 1;

		addAllocator<BlockAllocator>("GeneralAllocator",CoreAllocatorSizes::GeneralAllocatorSize);
		addAllocator<BlockAllocator>("ModuleAllocator",CoreAllocatorSizes::ModuleAllocatorSize);

	#if defined K15_DEBUG
		addAllocator<BlockAllocator>("DebugAllocator",CoreAllocatorSizes::DebugAllocatorSize);
		addAllocator<BlockAllocator>("ProfilingAllocator",CoreAllocatorSizes::ProfilingAllocatorSize,Allocators[AC_DEBUG]);
		addAllocator<PoolAllocator<ProfilingNode> >("ProfilingNodePool",CoreAllocatorSizes::ProfilingNodePoolCount * sizeof(ProfilingNode),Allocators[AC_PROFILING]);
	#endif //K15_DEBUG

		addAllocator<BlockAllocator>("RenderAllocator",CoreAllocatorSizes::RenderAllocatorSize);
		addAllocator<BlockAllocator>("GameEventAllocator",CoreAllocatorSizes::GameEventAllocatorSize);
		addAllocator<BlockAllocator>("TreadingAllocator",CoreAllocatorSizes::ThreadingAllocatorSize);
		addAllocator<BlockAllocator>("TaskAllocator",CoreAllocatorSizes::TaskAllocatorSize);
		addAllocator<BlockAllocator>("LoggingAllocator",CoreAllocatorSizes::LoggingAllocatorSize);
		addAllocator<BlockAllocator>("ResourceAllocator",CoreAllocatorSizes::ResourceAllocatorSize);
		addAllocator<PoolAllocator<RenderOperation> >("RenderOperationPool",CoreAllocatorSizes::RenderOperationPoolCount * sizeof(RenderOperation),Allocators[AC_RENDERING]);
		addAllocator<PoolAllocator<Vertex> >("VertexPool",CoreAllocatorSizes::VertexPoolCount * sizeof(Vertex),Allocators[AC_RENDERING]);
	}
	/*********************************************************************************/
	void BaseAllocatedObject::removeAllocators()
	{
		K15_DELETE_T(Allocators[AC_RENDERING],Allocators[AC_VERTEX_POOL],PoolAllocator<Vertex>); Allocators[AC_VERTEX_POOL] = 0;
		K15_DELETE_T(Allocators[AC_RENDERING],Allocators[AC_RENDEROP_POOL],PoolAllocator<RenderOperation>); Allocators[AC_RENDEROP_POOL] = 0;
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_RESOURCE],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_LOGGING],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_TASKS],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_THREADING],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_GAMEVENTS],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_RENDERING],BlockAllocator);

		#if defined K15_DEBUG
			K15_DELETE_T(Allocators[AC_PROFILING],Allocators[AC_PROFILING_NODE_POOL],PoolAllocator<ProfilingNode>);
			K15_DELETE_T(Allocators[AC_DEBUG],Allocators[AC_PROFILING],BlockAllocator);
			K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_DEBUG],BlockAllocator);
		#endif

		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_MODULE],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_GENERAL],BlockAllocator);

		Allocators[AC_CORE]->clear();

		delete Allocators[AC_CORE]; //only one that got created on the heap.
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace
