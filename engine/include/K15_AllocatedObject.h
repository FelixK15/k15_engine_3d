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

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_BaseAllocator.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API BaseAllocatedObject
	{
	public:
		/*********************************************************************************/
		enum eAllocationCategory
		{
			AC_CORE = 0,
			AC_MEMORY,
			AC_MODULE,
#			if defined K15_DEBUG
			AC_DEBUG,
#			endif
			AC_RENDERING,
			AC_PROFILING,
			AC_INPUT,
			AC_GAMEVENTS,
			AC_DYNAMICLIBRARY,
			AC_THREADING,
			AC_TASKS,
			AC_LOGGING,
			//Pools
			AC_RENDEROP_POOL,
			AC_PROFILING_NODE_POOL,
			AC_COUNT
		}; //AllocationCategory
		/*********************************************************************************/
		typedef FixedArray(BaseAllocator*,AC_COUNT) AllocatorArray;
		/*********************************************************************************/

		static AllocatorArray Allocators;
		static void removeAllocators();
	private:
		static AllocatorArray _createAllocators();
	};
	/*********************************************************************************/
	template<Enum AllocatorCategory>
	class AllocatedObject : public BaseAllocatedObject
	{
	public:
		AllocatedObject();
		virtual ~AllocatedObject();

#	if defined (K15_DEBUG)
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
#	else
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
#	endif //K15_DEBUG
	};

	typedef AllocatedObject<BaseAllocatedObject::AC_CORE> CoreAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_MEMORY> MemoryAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_MODULE> ModuleAllocatedObject;
#	if defined K15_DEBUG
	typedef AllocatedObject<BaseAllocatedObject::AC_DEBUG> DebugAllocatedObject;
#	endif //K15_DEBUIG
	typedef AllocatedObject<BaseAllocatedObject::AC_RENDERING> RenderingAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_PROFILING> ProfilingAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_GAMEVENTS> GameEventAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_INPUT> InputManagerAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_DYNAMICLIBRARY> DynamicLibraryAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_THREADING> ThreadingAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_TASKS> TaskManagerAllocatedObject;
	typedef AllocatedObject<BaseAllocatedObject::AC_LOGGING> LoggingAllocatedObject;

#include "K15_AllocatedObject.inl"
}} //end of K15_Engine::Core namespace

#endif //_K15Engine_Core_AllocatedObject_h_