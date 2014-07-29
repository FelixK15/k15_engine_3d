/**
 * @file K15_MeshInstance.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/14
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
Mesh* MeshInstance::getMesh() const
{
    return m_Mesh;
}
/*********************************************************************************/
SubMeshInstance* MeshInstance::getSubMeshInstance(int32 p_Index) const
{
    K15_ASSERT(p_Index < m_SubModelInstances.size() && p_Index >= 0, "Access out of bounds.");

    return m_SubModelInstances[p_Index];
}
/*********************************************************************************/
const MeshInstance::SubMeshInstanceArray& MeshInstance::getSubMeshInstances() const
{
    return m_SubModelInstances;
}
/*********************************************************************************/
int32 MeshInstance::getSubMeshInstanceCount() const
{
    return m_SubModelInstances.size();
}
/*********************************************************************************/
const AABB& MeshInstance::getAABB() const
{
    return AABB();
}
/*********************************************************************************/
