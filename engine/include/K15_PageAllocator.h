/**
 * @file K15_PageAllocator.h
 * @author Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/09
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
 */

#ifndef _K15Engine_System_PageAllocator_h_
#define _K15Engine_System_PageAllocator_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_BaseAllocator.h"

namespace K15_Engine { namespace System {

	template<uint16 PageSize = 128,uint32 TotalSize = 8192>
	class PageAllocator : public BaseAllocator
	{
	private:
		/*********************************************************************************/
		struct PageData
		{
// #		if defined (K15_DEBUG)
// 			byte Memory[PageSize + sizeof(MemoryHeader)];
// #		else
			byte Memory[PageSize];
//#		endif //K15_DEBUG
			bool Used;
			uint32 Size;
		};
		/*********************************************************************************/

	public:
		//allocate memory using malloc
		PageAllocator();
    //allocate memory from another allocator
    PageAllocator(BaseAllocator* p_Allocator);

    virtual void clear();

  protected:
    virtual void* alloc(uint32 p_Size);
    virtual void free(void* p_Pointer);

	private:
		void createPages();

	public:
		static const uint32 PageCount;

	private:
		PageData m_PageData[TotalSize / PageSize];
		uint32 m_CurrentPageIndex;
		uint32 m_PageSize;
	};//end of PageAllocator class
#include "K15_PageAllocator.inl"
}} //end of K15_Engine::System namespace

#endif //_K15Engine_System_PageAllocator_h_