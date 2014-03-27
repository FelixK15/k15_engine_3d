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

#include "K15_MemoryBlock.h"
#include "K15_ProfilingNode.h"
#include "K15_RenderOperation.h"
#include "K15_Vertex.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	BaseAllocatedObject::AllocatorArray BaseAllocatedObject::Allocators = BaseAllocatedObject::_createAllocators();
	/*********************************************************************************/

	/*********************************************************************************/
	BaseAllocatedObject::AllocatorArray BaseAllocatedObject::_createAllocators()
	{
		static AllocatorArray _Allocators;
		BaseAllocator* CoreAllocator = new StackAllocator(20*MEGABYTE,"CoreAllocator");

		_Allocators[AC_CORE] = CoreAllocator;
		_Allocators[AC_MEMORY] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(0.5*MEGABYTE,"MemoryAllocator",CoreAllocator);
		_Allocators[AC_MODULE] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(0.5*MEGABYTE,"ModuleAllocator",CoreAllocator);
#		if defined K15_DEBUG
		_Allocators[AC_DEBUG] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(2*MEGABYTE,"DebugAllocator",CoreAllocator);
#		endif //K15_DEBUG
		_Allocators[AC_RENDERING] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(1*MEGABYTE,"RenderAllocator",CoreAllocator);
		_Allocators[AC_PROFILING] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(2*MEGABYTE,"ProfilingAllocator",CoreAllocator);
		_Allocators[AC_INPUT] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(0.5*MEGABYTE,"InputAllocator",CoreAllocator);
		_Allocators[AC_GAMEVENTS] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(0.5*MEGABYTE,"GameEventAllocator",CoreAllocator);
		_Allocators[AC_DYNAMICLIBRARY] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(0.5*MEGABYTE,"DynamicLibrarayAllocator",CoreAllocator);
		_Allocators[AC_THREADING] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(0.5*MEGABYTE,"ThreadingAllocator",CoreAllocator);
		_Allocators[AC_TASKS] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(0.5*MEGABYTE,"TaskAllocator",CoreAllocator);
		_Allocators[AC_LOGGING] = K15_NEW_T(CoreAllocator,BlockAllocator) BlockAllocator(0.5*MEGABYTE,"LoggingAllocator",CoreAllocator);
		_Allocators[AC_VERTEX_POOL] = K15_NEW_T(_Allocators[AC_RENDERING],PoolAllocator<Vertex>) PoolAllocator<Vertex>(10000,"VertexPool",_Allocators[AC_RENDERING]);
		_Allocators[AC_RENDEROP_POOL] = K15_NEW_T(_Allocators[AC_RENDERING],PoolAllocator<RenderOperation>) PoolAllocator<RenderOperation>(256,"RenderOperationPool",_Allocators[AC_RENDERING]);
		_Allocators[AC_PROFILING_NODE_POOL] = K15_NEW_T(_Allocators[AC_PROFILING],PoolAllocator<ProfilingNode>) PoolAllocator<ProfilingNode>(10240,"ProfilingNodePool",_Allocators[AC_PROFILING]);

 		return _Allocators;
	}
	/*********************************************************************************/
	void BaseAllocatedObject::removeAllocators()
	{
		K15_DELETE_T(Allocators[AC_PROFILING],Allocators[AC_PROFILING_NODE_POOL],PoolAllocator<ProfilingNode>);
		K15_DELETE_T(Allocators[AC_RENDERING],Allocators[AC_RENDEROP_POOL],PoolAllocator<RenderOperation>);
		K15_DELETE_T(Allocators[AC_RENDERING],Allocators[AC_VERTEX_POOL],PoolAllocator<Vertex>);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_LOGGING],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_TASKS],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_THREADING],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_DYNAMICLIBRARY],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_GAMEVENTS],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_INPUT],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_PROFILING],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_RENDERING],BlockAllocator);

#		ifdef K15_DEBUG
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_DEBUG],BlockAllocator);
#		endif //K15_DEBUG

		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_MODULE],BlockAllocator);
		K15_DELETE_T(Allocators[AC_CORE],Allocators[AC_MEMORY],BlockAllocator);
		K15_DELETE Allocators[AC_CORE];
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace