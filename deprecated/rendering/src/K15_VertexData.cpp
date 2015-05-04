/**
 * @file K15_VertexData.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/12
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

#include "K15_VertexData.h"
#include "K15_Vertex.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
  VertexData::VertexData(VertexDeclaration* p_VertexDeclaration, VertexBuffer* p_VertexBuffer, uint32 p_Offset, uint32 p_VertexCount)
    : m_VertexDeclaration(p_VertexDeclaration),
    m_VertexBuffer(p_VertexBuffer),
    m_Offset(p_Offset),
    m_VertexCount(p_VertexCount),
    m_VertexCache()
  {

  }
  /*********************************************************************************/
  VertexData::~VertexData()
  {
    for(uint32 i = 0; i < m_VertexCache.size(); ++i)
    {
      delete m_VertexCache[i];
    }

    m_VertexCache.clear();
  }
  /*********************************************************************************/
  Vertex* VertexData::getVertex(uint32 p_Index)
  {
    Vertex* vertex = 0;

    for(uint32 i = 0; i < m_VertexCache.size(); ++i)
    {
      vertex = m_VertexCache[i];

      if(vertex->getIndex() == p_Index)
      {
        return vertex;
      }
    }

    vertex = K15_NEW Vertex(m_VertexBuffer, m_VertexDeclaration, p_Index);

    m_VertexCache.push_back(vertex);

    return vertex;
  }
  /*********************************************************************************/
}}// end of K15_Engine::Rendering namespace
