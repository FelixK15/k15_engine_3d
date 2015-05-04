/**
 * @file K15_Mesh.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_Mesh.h"
#include "K15_SubMesh.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	Mesh::Mesh()
		: m_SubMeshes(),
		m_AABB()
	{

	}
	/*********************************************************************************/
	Mesh::~Mesh()
	{

	}
	/*********************************************************************************/
	void Mesh::calculateAABB()
	{
		float x_max = 0.0f,y_max = 0.0f,z_max = 0.0f;
		float x_min = 0.0f,y_min = 0.0f,z_min = 0.0f;

		SubMesh* subMesh = 0;
		for(uint32 i = 0;i < m_SubMeshes.size();++i)
		{
			subMesh = m_SubMeshes.at(i);

			const AABB& subMeshAABB = subMesh->getAABB();
			const AABB::CornerArray& corners = subMeshAABB.getCorners();
			for(int j = 0;j < AABB::CT_COUNT;++j)
			{
				const Vector3& corner = corners[j];

				if(corner.x > x_max)		x_max = corner.x;
				else if(corner.x < x_min)	x_min = corner.x;

				if(corner.y > y_max)		y_max = corner.y;
				else if(corner.y < y_min)	y_min = corner.y;

				if(corner.z > z_max)		z_max = corner.z;
				else if(corner.z < z_min)	z_min = corner.z;
			}
		}

		m_AABB.setMax(Vector3(x_max, y_max, z_max));
		m_AABB.setMin(Vector3(x_min, y_min, z_min));
	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace