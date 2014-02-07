/**
 * @file K15_MemoryProfiler.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/08
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

#ifndef _K15Engine_Core_MemoryProfiler_h_
#define _K15Engine_Core_MemoryProfiler_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_Singleton.h"
#	include "K15_BaseAllocator.h"
#endif //K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Core {
	class K15_CORE_API MemoryProfiler : public Singleton<MemoryProfiler>,
										public CoreAllocatedObject
	{
	public:
		/*********************************************************************************/
		typedef List(BaseAllocator*) AllocatorList;
		/*********************************************************************************/
	public:
		MemoryProfiler();
		~MemoryProfiler();

		void addAllocator(BaseAllocator* p_Allocator);
		void removeAllocator(BaseAllocator* p_Allocator);

		const AllocatorList& getAllocators() const;
	private:
		AllocatorList m_Allocator;
	};// end of MemoryProfiler class declaration
}}// end of K15_Engine::Core namespace

#endif //