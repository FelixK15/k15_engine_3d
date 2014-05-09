/**
 * @file K15_RenderQueue.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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

#ifndef _K15Engine_Rendering_RenderQueue_h_
#define _K15Engine_Rendering_RenderQueue_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#	include "K15_Object.h"
#endif //K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API RenderQueue : public Object, public RenderingAllocatedObject
	{
		/*********************************************************************************/
		typedef DynamicArray(RenderOperation*) RenderOperationArray;
		/*********************************************************************************/

	public:
		/*********************************************************************************/
		enum eSortMode
		{
			SP_SORT_BY_MATERIAL = 0,
			SP_SORT_FRONT_TO_BACK,
			SP_SORT_BACK_TO_FRONT,

			SP_COUNT
		}; //SortPriority
		/*********************************************************************************/

	public:
		RenderQueue();
		~RenderQueue();

		INLINE void setSortMode(Enum p_SortMode);
		INLINE void addRenderOperation(RenderOperation* p_ROP);
		
		INLINE RenderOperation* getRenderOperation(uint32 p_Index) const;

		INLINE uint32 size() const;
		INLINE Enum getSortMode() const;

		INLINE void clear();
		
		void sort();

	private:
		bool m_Dirty;
		Enum m_SortMode;
		uint32 m_RenderOperationCount;
		RenderOperationArray m_RenderOperations;
	};// end of RenderQueue class declaration
# include "K15_RenderQueue.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_RenderQueue_h_