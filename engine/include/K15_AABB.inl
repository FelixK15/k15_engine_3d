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
const Vector4& AABB::getMin() const
{
	return m_Min;
}
/*********************************************************************************/
const Vector4& AABB::getMax() const
{
	return m_Max;
}
/*********************************************************************************/
void AABB::setMin(const Vector4& p_Min)
{
	m_Min = p_Min;
	_calculateCorners();
}
/*********************************************************************************/
void AABB::setMax(const Vector4& p_Max)
{
	m_Max = p_Max;
	_calculateCorners();
}
/*********************************************************************************/
const Vector4& AABB::getFarLeftTop() const
{
	return m_Corners[CT_FAR_LEFT_TOP];
}
/*********************************************************************************/
const Vector4& AABB::getFarRightTop() const
{
	return m_Corners[CT_FAR_RIGHT_TOP];
}
/*********************************************************************************/
const Vector4& AABB::getFarLeftBottom() const
{
	return m_Corners[CT_FAR_LEFT_BOTTOM];
}
/*********************************************************************************/
const Vector4& AABB::getFarRightBottom() const
{
	return m_Corners[CT_FAR_RIGHT_BOTTOM];
}
/*********************************************************************************/
const Vector4& AABB::getNearLeftTop() const
{
	return m_Corners[CT_NEAR_LEFT_TOP];
}
/*********************************************************************************/
const Vector4& AABB::getNearRightTop() const
{
	return m_Corners[CT_NEAR_RIGHT_TOP];
}
/*********************************************************************************/
const Vector4& AABB::getNearLeftBottom() const
{
	return m_Corners[CT_NEAR_LEFT_BOTTOM];
}
/*********************************************************************************/
const Vector4& AABB::getNearRightBottom() const
{
	return m_Corners[CT_NEAR_RIGHT_BOTTOM];
}
/*********************************************************************************/