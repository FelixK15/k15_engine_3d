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

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	SubMesh::SubMesh(Mesh* p_Mesh)
		: m_Material(0),
		m_IndexBuffer(0),
		m_VertexBuffer(0),
		m_Mesh(p_Mesh)
	{
		m_Mesh->addSubMesh(this);
	}
	/*********************************************************************************/
	SubMesh::~SubMesh()
	{
		
	}
	/*********************************************************************************/
	RenderOperation* SubMesh::createRenderOperation()
	{
		static RenderOperation rop;

		rop.indexBuffer = m_IndexBuffer;
		rop.vertexBuffer = m_VertexBuffer;
		rop.subMesh = this;
		rop.topology = RenderOperation::T_TRIANGLE;
		rop.material = m_Material;
		
		return &rop;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace