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
	VertexBuffer::VertexBuffer(const CreationOptions& p_Options)
		: GpuBuffer(p_Options),
		m_Declaration(p_Options.VertexLayout)
	{
		setType(GpuBuffer::BT_VERTEX_BUFFER);	
	}
	/*********************************************************************************/
	VertexBuffer::~VertexBuffer()
	{
		for(IndexVertexMap::iterator iter = m_VertexCache.begin();iter != m_VertexCache.end();++iter)
		{
			K15_DELETE iter->second;
		}

		m_VertexCache.clear();
	}
	/*********************************************************************************/
	Vertex* VertexBuffer::getVertex(uint32 p_Index)
	{
		K15_ASSERT(p_Index <= getVertexCount(),"Tried to access out of bounds.");

		IndexVertexMap::iterator iter = m_VertexCache.find(p_Index);

		if(iter == m_VertexCache.end())
		{
			Vertex* vertex = K15_NEW Vertex(this,p_Index);

			m_VertexCache.insert(Pair(uint32,Vertex*)(p_Index,vertex));

			return vertex;
		}

		return iter->second;
	}
	/*********************************************************************************/
	uint32 VertexBuffer::getVertexSize() const
	{
		if(m_Declaration)
		{
			  return m_Declaration->getVertexSize();
		}
		
		return 0;
	}
	/*********************************************************************************/
	uint32 VertexBuffer::getVertexCount() const
	{
		if(m_Declaration)
		{
			return getSize() / m_Declaration->getVertexSize();
		}

		return 0;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace