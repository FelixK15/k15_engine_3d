/**
 * @file K15_Mesh.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/08
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

/*********************************************************************************/
const Mesh::SubMeshArray& Mesh::getSubMeshes() const
{
	return m_SubMeshes;
}
/*********************************************************************************/
void Mesh::addSubMesh(SubMesh* p_SubMesh)
{
	m_SubMeshes.push_back(p_SubMesh);
}
/*********************************************************************************/
SubMesh* Mesh::getSubMesh(uint32 p_Index) const
{
	return m_SubMeshes.at(p_Index);
}
/*********************************************************************************/
const AABB& Mesh::getAABB() const
{
  return m_AABB;
}
/*********************************************************************************/