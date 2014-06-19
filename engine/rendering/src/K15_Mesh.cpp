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
		: ResourceBase(),
		m_SubMeshes(),
		m_AABB()
	{

	}
	/*********************************************************************************/
	Mesh::Mesh(const ObjectName& p_Name)
		: ResourceBase(p_Name),
		m_SubMeshes(),
		m_AABB()
	{

	}
	/*********************************************************************************/
	Mesh::~Mesh()
	{

	}
	/*********************************************************************************/
	const AABB& Mesh::getAABB()
	{
		bool dirty = false;
		for(SubMeshArray::iterator iter = m_SubMeshes.begin(); iter != m_SubMeshes.end(); ++iter)
		{
			if((*iter)->isDirty())
			{
				dirty = true;
				(*iter)->setIsDirty(false);
			}
		}

		if(dirty)
		{
			calculateAABB();
		}

		return m_AABB;
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

			for(int j = 0;j < AABB::CT_COUNT;++j)
			{
				const Vector3& corner = subMeshAABB.getCorner(j);

				if(corner.x > x_max)		x_max = corner.x;
				else if(corner.x < x_min)	x_min = corner.x;

				if(corner.y > y_max)		y_max = corner.y;
				else if(corner.y < y_min)	y_min = corner.y;

				if(corner.z > z_max)		z_max = corner.z;
				else if(corner.z < z_min)	z_min = corner.z;
			}
		}

		m_AABB.setCorner(Vector3(x_min,y_min,z_min),AABB::CT_FAR_LEFT_BOTTOM);
		m_AABB.setCorner(Vector3(x_max,y_min,z_min),AABB::CT_FAR_RIGHT_BOTTOM);
		m_AABB.setCorner(Vector3(x_min,y_max,z_min),AABB::CT_FAR_LEFT_TOP);
		m_AABB.setCorner(Vector3(x_max,y_max,z_min),AABB::CT_FAR_RIGHT_TOP);

		m_AABB.setCorner(Vector3(x_min,y_min,z_max),AABB::CT_NEAR_LEFT_BOTTOM);
		m_AABB.setCorner(Vector3(x_max,y_min,z_max),AABB::CT_NEAR_RIGHT_BOTTOM);
		m_AABB.setCorner(Vector3(x_min,y_max,z_max),AABB::CT_NEAR_LEFT_TOP);
		m_AABB.setCorner(Vector3(x_max,y_max,z_max),AABB::CT_NEAR_LEFT_TOP);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace