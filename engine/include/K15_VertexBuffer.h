/**
 * @file K15_VertexBuffer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/29
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

#ifndef _K15Engine_Rendering_VertexBuffer_h_
#define _K15Engine_Rendering_VertexBuffer_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

#include "K15_GpuBuffer.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API VertexBuffer : public GpuBuffer
	{
	public:
		/*********************************************************************************/
		typedef HashMap(uint32,Vertex*) IndexVertexMap;
		/*********************************************************************************/
	public:
		VertexBuffer(const CreationOptions& p_Options = DefaultOptions);
		virtual ~VertexBuffer();

		Vertex* getVertex(uint32 p_Index);

		INLINE void setVertexDeclaration(VertexDeclaration* p_Declaration);
		INLINE VertexDeclaration* getVertexDeclaration() const;

		uint32 getVertexCount() const;
		uint32 getVertexSize() const;
	private:
		IndexVertexMap m_VertexCache;
		VertexDeclaration* m_Declaration;
	};// end of VertexBuffer class declaration
#include "K15_VertexBuffer.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Rendering_VertexBuffer_h_