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
void SubMesh::setVertexBuffer(VertexBuffer* p_VertexBuffer)
{
	m_VertexBuffer = p_VertexBuffer;
}
/*********************************************************************************/
VertexBuffer* SubMesh::getVertexBuffer() const
{
	return m_VertexBuffer;
}
/*********************************************************************************/
void SubMesh::setIndexBUffer(IndexBuffer* p_IndexBuffer)
{
	m_IndexBuffer = p_IndexBuffer;
}
/*********************************************************************************/
IndexBuffer* SubMesh::getIndexBuffer() const
{
	return m_IndexBuffer;
}
/*********************************************************************************/
void SubMesh::setMaterial(Material* p_Material)
{
	m_Material = p_Material;
}
/*********************************************************************************/
Material* SubMesh::getMaterial() const
{
	return m_Material;
}
/*********************************************************************************/
Mesh* SubMesh::getMesh() const
{
  return m_Mesh;
}