/**
 * @file K15_IndexBuffer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/09
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

#ifndef _K15Engine_Renderer_IndexBuffer_h_
#define _K15Engine_Renderer_IndexBuffer_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_GpuBuffer.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API IndexBuffer : public GpuBuffer
	{
	public:
		/*********************************************************************************/
		enum eIndexType
		{
			IT_UINT8 = 0,
			IT_UINT16,
			IT_UINT32,

			IT_COUNT
		};// IndexType
		/*********************************************************************************/
		static const uint32 IndexSize[IT_COUNT];
		/*********************************************************************************/

	public:
		IndexBuffer();
		virtual ~IndexBuffer();

		INLINE Enum getIndexType() const;
		INLINE void setIndexType(Enum p_IndexType);

    INLINE uint32 getIndexCount() const;

	protected:
		Enum m_IndexType;
	};// end of IndexBuffer class declaration
#include "K15_IndexBuffer.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_IndexBuffer_h_