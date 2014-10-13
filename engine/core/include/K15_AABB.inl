/**
 * @file K15_AABB.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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
Vector3 AABB::getFarLeftTop() const
{
	return Vector3(m_Min.x, m_Max.y, m_Max.z);
}
/*********************************************************************************/
Vector3 AABB::getFarRightTop() const
{
	return Vector3(m_Max.x, m_Max.y, m_Max.z);
}
/*********************************************************************************/
Vector3 AABB::getFarLeftBottom() const
{
	return Vector3(m_Min.x, m_Min.y, m_Max.z);
}
/*********************************************************************************/
Vector3 AABB::getFarRightBottom() const
{
	return Vector3(m_Max.x, m_Min.y, m_Max.z);
}
/*********************************************************************************/
Vector3 AABB::getNearLeftTop() const
{
	return Vector3(m_Min.x, m_Max.y, m_Min.z);
}
/*********************************************************************************/
Vector3 AABB::getNearRightTop() const
{
	return Vector3(m_Max.x, m_Max.y, m_Min.z);
}
/*********************************************************************************/
Vector3 AABB::getNearLeftBottom() const
{
	return Vector3(m_Min.x, m_Min.y, m_Min.z);
}
/*********************************************************************************/
Vector3 AABB::getNearRightBottom() const
{
	return Vector3(m_Max.x, m_Min.y, m_Min.z);
}
/*********************************************************************************/
AABB::CornerArray AABB::getCorners() const
{
	CornerArray corners;

	corners[CT_FAR_LEFT_BOTTOM]		= getFarLeftBottom();
	corners[CT_FAR_RIGHT_BOTTOM]	= getFarRightBottom();
	corners[CT_FAR_LEFT_TOP]		= getFarLeftTop();
	corners[CT_FAR_RIGHT_TOP]		= getFarRightTop();
	corners[CT_NEAR_LEFT_BOTTOM]	= getNearLeftBottom();
	corners[CT_NEAR_RIGHT_BOTTOM]	= getNearRightBottom();
	corners[CT_NEAR_LEFT_TOP]		= getNearLeftTop();
	corners[CT_NEAR_RIGHT_TOP]		= getNearRightTop();

	return corners;
}
/*********************************************************************************/
const Vector3& AABB::getMin() const
{
	return m_Min;
}
/*********************************************************************************/
const Vector3& AABB::getMax() const
{
	return m_Max;
}
/*********************************************************************************/
void AABB::setMin(const Vector3& p_Min)
{
	m_Min = p_Min;
}
/*********************************************************************************/
void AABB::setMax(const Vector3& p_Max)
{
	m_Max = p_Max;
}
/*********************************************************************************/
void AABB::merge(const AABB& p_AABB)
{
	if(m_Min < p_AABB.getMin()) m_Min = p_AABB.getMin();
	if(m_Max > p_AABB.getMax()) m_Max = p_AABB.getMax();
}
/*********************************************************************************/
bool AABB::operator<(const AABB& p_Rhs)
{
	return m_Min < p_Rhs.getMin() && m_Max > p_Rhs.getMax();
}
/*********************************************************************************/
bool AABB::operator>(const AABB& p_Rhs)
{
	return !operator<(p_Rhs);
}
/*********************************************************************************/