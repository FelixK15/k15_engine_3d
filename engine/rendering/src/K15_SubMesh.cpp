/**
 * @file K15_SubMesh.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/10
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

#include "K15_SubMesh.h"
#include "K15_Mesh.h"
#include "K15_RenderOperation.h"
#include "K15_VertexBuffer.h"
#include "K15_VertexData.h"
#include "K15_Vertex.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	SubMesh::SubMesh(Mesh* p_Mesh)
		: m_IndexData(0),
		m_VertexData(0),
		m_Mesh(p_Mesh)
	{
		m_Mesh->addSubMesh(this);
	}
	/*********************************************************************************/
	SubMesh::~SubMesh()
	{
		
	}
	/*********************************************************************************/
	void SubMesh::calculateAABB()
	{
		float x_max = 0.0f,y_max = 0.0f,z_max = 0.0f;
		float x_min = 0.0f,y_min = 0.0f,z_min = 0.0f;
		Vertex* vertex = 0;
		VertexBuffer* vertexBuffer = 0;
		Vector4 pos;
		uint32 vertexIndex = 0;

		if(m_VertexData)
		{
			for(uint32 i = 0;i < m_VertexData->getVertexCount();++i)
			{
				vertexIndex = i + m_VertexData->getOffsetInBytes();
				vertexBuffer = m_VertexData->getVertexBuffer();
				vertex = m_VertexData->getVertex(vertexIndex);
				pos = vertex->getPosition();

				if(pos.x > x_max)		x_max = pos.x;
				else if(pos.x < x_min)	x_min = pos.x;

				if(pos.y > y_max)		y_max = pos.y;
				else if(pos.y < y_min)	y_min = pos.y;

				if(pos.z > z_max)		z_max = pos.z;
				else if(pos.z < z_min)	z_min = pos.z;
			}
		}

		m_AABB.setMax(Vector3(x_max, y_max, z_max));
		m_AABB.setMin(Vector3(x_min, y_min, z_min));
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace