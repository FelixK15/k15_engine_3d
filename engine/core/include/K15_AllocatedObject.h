/**
 * @file K15_AllocatedObject.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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

#ifndef _K15Engine_Core_AllocatedObject_h_
#define _K15Engine_Core_AllocatedObject_h_

#include "K15_Prerequisites.h"
#include "K15_BaseAllocator.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API BaseAllocatedObject
	{
	public:
		/*********************************************************************************/
		struct K15_CORE_API CoreAllocatorSizes
		{
		  static uint32 GeneralAllocatorSize;
		  static uint32 ModuleAllocatorSize;
		  static uint32 DebugAllocatorSize;
		  static uint32 ResourceAllocatorSize;
		  static uint32 RenderAllocatorSize;
		  static uint32 ProfilingAllocatorSize;
		  static uint32 GameEventAllocatorSize;
		  static uint32 ThreadingAllocatorSize;
		  static uint32 TaskAllocatorSize;
		  static uint32 LoggingAllocatorSize;
		  static uint32 RenderOperationPoolCount;
		  static uint32 ProfilingNodePoolCount;
		  static uint32 VertexPoolCount;
		}; //end of CoreAllocatorSizes class declaration
		/*********************************************************************************/

		/*********************************************************************************/
		static const uint32 MaxAllocators = 32;
		/*********************************************************************************/
		enum eAllocationCategory
		{
			AC_CORE = 0,
			AC_GENERAL,
			AC_MODULE,
			AC_DEBUG,
			AC_PROFILING,
			AC_PROFILING_NODE_POOL,
			AC_RENDERING,
			AC_GAMEVENTS,
			AC_THREADING,
			AC_TASKS,
			AC_LOGGING,
			AC_RESOURCE,

			//Pools
			AC_RENDEROP_POOL,
			AC_VERTEX_POOL,
			AC_COUNT = MaxAllocators - AC_VERTEX_POOL
		}; //AllocationCategory
		/*********************************************************************************/
		typedef FixedArray(BaseAllocator*,AC_COUNT) AllocatorArray;
		/*********************************************************************************/

		static AllocatorArray Allocators;
		static uint32 MemorySize;
		static uint32 AllocatorCount;

		static void removeAllocators();
		static void createCoreAllocators();

		template<class AllocatorType> 
		static void addAllocator(const String& p_AllocatorName, uint32 p_Size, BaseAllocator* p_ParentAllocator = Allocators[AC_CORE]);
	}; //end of BaseAllocatedObject class declaration
	/*********************************************************************************/
	template<Enum AllocatorCategory>
	class AllocatedObject : public BaseAllocatedObject
	{
	public:
		AllocatedObject();
		virtual ~AllocatedObject();

	#if defined (K15_DEBUG)
		/*********************************************************************************/
		static void* alloc(size_t,const char*,uint32,bool,const char*);
		static void  dealloc(void*,size_t,const char*,uint32,bool,const char*);
		/*********************************************************************************/
		static void* operator new(size_t,const char*,uint32,const char*);
		static void* operator new[](size_t,const char*,uint32,const char*);
		/*********************************************************************************/
		static void operator delete(void*,size_t);
		static void operator delete[](void*,size_t);
		/*********************************************************************************/
	#else
		/*********************************************************************************/
		static void* alloc(size_t);
		static void  dealloc(void*,size_t);
		/*********************************************************************************/
		static void* operator new(size_t);
		static void* operator new[](size_t);
		/*********************************************************************************/
		static void operator delete(void*,size_t);
		static void operator delete[](void*,size_t);
		/*********************************************************************************/
	#endif //K15_DEBUG
	}; //end of AllocatedObject class declaration

	typedef AllocatedObject<BaseAllocatedObject::AC_CORE> CoreAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_GENERAL> GeneralAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_GENERAL> MemoryAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_MODULE> ModuleAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_DEBUG> DebugAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_RENDERING> RenderingAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_PROFILING> ProfilingAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_GAMEVENTS> GameEventAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_GENERAL> InputManagerAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_GENERAL> DynamicLibraryAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_THREADING> ThreadingAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_TASKS> TaskManagerAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_LOGGING> LoggingAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_RESOURCE> ResourceAllocatedObject;

#include "K15_AllocatedObject.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_AllocatedObject_h_