/**
 * @file K15_VertexBuffer.cpp
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
 */

#include "K15_PrecompiledHeader.h"

#include "K15_Vertex.h"
#include "K15_VertexBuffer.h"
#include "K15_VertexDeclaration.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	VertexBuffer::VertexBuffer()
		: GpuBuffer(GpuBuffer::BT_VERTEX_BUFFER,GpuBuffer::LO_DISCARD,GpuBuffer::UO_STATIC,GpuBuffer::BA_WRITE_ONLY,false),
		m_VertexCount(0)
	{

	}
	/*********************************************************************************/
	VertexBuffer::~VertexBuffer()
	{

	}
	/*********************************************************************************/
	Vertex* VertexBuffer::getVertex(uint32 p_Index)
	{
		K15_ASSERT(p_Index <= m_Declaration->getElementCount(),"Tried to access out of bounds.");

		IndexVertexMap::iterator iter = m_VertexCache.find(p_Index);

		if(iter == m_VertexCache.end())
		{
			VertexElement element = m_Declaration->getElement(p_Index);
			Vertex* vertex = K15_NEW Vertex(this,m_Declaration);

			return vertex;
		}

		return iter->second;
	}
	/*********************************************************************************/
	void VertexBuffer::setDirty(bool p_Dirty)
	{
		GpuBuffer::setDirty(p_Dirty);

		if(p_Dirty == false)
		{

		}
	}
	/*********************************************************************************/
// 	uint32 VertexBuffer::getVertexSize() const
// 	{
//     if(m_Declaration)
//     {
// 		  return m_Declaration->getVertexSize();
//     }
// 
//     return 0;
//   }
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace