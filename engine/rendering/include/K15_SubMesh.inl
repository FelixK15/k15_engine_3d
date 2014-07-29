/**
 * @file K15_SubMesh.inl
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
void SubMesh::setVertexData(VertexData* p_VertexData)
{
	m_VertexData = p_VertexData;
	_calculateAABB();
}
/*********************************************************************************/
VertexData* SubMesh::getVertexData() const
{
	return m_VertexData;
}
/*********************************************************************************/
void SubMesh::setIndexData(IndexData* p_IndexData)
{
	m_IndexData = p_IndexData;
}
/*********************************************************************************/
IndexData* SubMesh::getIndexData() const
{
	return m_IndexData;
}
/*********************************************************************************/
Mesh* SubMesh::getMesh() const
{
  return m_Mesh;
}
/*********************************************************************************/
const AABB& SubMesh::getAABB() const
{
  return m_AABB;
}
/*********************************************************************************/