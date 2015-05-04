/**
 * @file K15_Frustum.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/12
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
void Frustum::setCorner(const Vector3& p_CornerPos, Enum p_Corner)
{
	m_Corners[p_Corner] = p_CornerPos;
}
/*********************************************************************************/
Frustum::PlaneArray& Frustum::getPlanes()
{
	return m_Planes;
}
/*********************************************************************************/
Frustum::CornerArray& Frustum::getCorners()
{
	return m_Corners;
}
/*********************************************************************************/
const Frustum::Plane& Frustum::getPlane(Enum p_Plane) const
{
	return m_Planes[p_Plane];
}
/*********************************************************************************/
const Vector3& Frustum::getCorner(Enum p_Corner) const
{
	return m_Corners[p_Corner];
}
/*********************************************************************************/
void Frustum::setFarPlaneDistance(float p_FarPlane)
{
	m_FarPlane = p_FarPlane;
}
/*********************************************************************************/
void Frustum::setNearPlaneDistance(float p_NearPlane)
{
	m_NearPlane = p_NearPlane;
}
/*********************************************************************************/