/**
 * @file K15_SubMesh.h
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

#ifndef _K15Engine_Rendering_SubMesh_h_
#define _K15Engine_Rendering_SubMesh_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"

#include "K15_AABB.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API SubMesh : public RenderingAllocatedObject
	{
	public:
		SubMesh(Mesh* p_Mesh);
		virtual ~SubMesh();

		INLINE void setVertexData(VertexData* p_VertexData);
		INLINE VertexData* getVertexData() const;

		INLINE void setIndexData(IndexData* p_IndexData);
		INLINE IndexData* getIndexData() const;

		INLINE void setMaterial(Material* p_Material);
		INLINE Material* getMaterial() const;

		INLINE Mesh* getMesh() const;

        INLINE const AABB& getAABB() const;
        void calculateAABB();

	protected:
		AABB m_AABB;
		VertexData* m_VertexData;
		IndexData* m_IndexData;
		Material* m_Material;
		Mesh* m_Mesh;
	};// end of SubMesh class declaration
#	include "K15_SubMesh.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_SubMesh_h_
