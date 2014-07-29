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
const Vector3& AABB::getFarLeftTop() const
{
	return m_Corners[CT_FAR_LEFT_TOP];
}
/*********************************************************************************/
const Vector3& AABB::getFarRightTop() const
{
	return m_Corners[CT_FAR_RIGHT_TOP];
}
/*********************************************************************************/
const Vector3& AABB::getFarLeftBottom() const
{
	return m_Corners[CT_FAR_LEFT_BOTTOM];
}
/*********************************************************************************/
const Vector3& AABB::getFarRightBottom() const
{
	return m_Corners[CT_FAR_RIGHT_BOTTOM];
}
/*********************************************************************************/
const Vector3& AABB::getNearLeftTop() const
{
	return m_Corners[CT_NEAR_LEFT_TOP];
}
/*********************************************************************************/
const Vector3& AABB::getNearRightTop() const
{
	return m_Corners[CT_NEAR_RIGHT_TOP];
}
/*********************************************************************************/
const Vector3& AABB::getNearLeftBottom() const
{
	return m_Corners[CT_NEAR_LEFT_BOTTOM];
}
/*********************************************************************************/
const Vector3& AABB::getNearRightBottom() const
{
	return m_Corners[CT_NEAR_RIGHT_BOTTOM];
}
/*********************************************************************************/
void AABB::setCorner(const Vector3& p_Positon, Enum p_Corner)
{
	m_Corners[p_Corner] = p_Positon;
}
/*********************************************************************************/
const Vector3& AABB::getCorner(Enum p_Corner) const
{
	return m_Corners[p_Corner];
}
/*********************************************************************************/
AABB::CornerArray& AABB::getCorners()
{
	return m_Corners;
}
/*********************************************************************************/
float AABB::getMaxX() const
{
	float max_x = 0.0f;

	for(int i =0; i<CT_COUNT;++i)
	{
		if(max_x < m_Corners[i].x)
		{
			max_x = m_Corners[i].x;
		}
	}

	return max_x;
}
/*********************************************************************************/
float AABB::getMaxY() const
{
	float max_y = 0.0f;

	for(int i = 0; i < CT_COUNT; ++i)
	{
		if(max_y < m_Corners[i].y)
		{
			max_y = m_Corners[i].y;
		}
	}

	return max_y;
}
/*********************************************************************************/
float AABB::getMaxZ() const
{
	float max_z = 0.0f;

	for(int i = 0; i < CT_COUNT; ++i)
	{
		if(max_z < m_Corners[i].z)
		{
			max_z = m_Corners[i].z;
		}
	}

	return max_z;
}
/*********************************************************************************/
float AABB::getMinX() const
{
	float min_x = 0.0f;

	for(int i = 0; i < CT_COUNT;++i)
	{
		if(min_x > m_Corners[i].x)
		{
			min_x = m_Corners[i].x;
		}
	}

	return min_x;
}
/*********************************************************************************/
float AABB::getMinY() const
{
	float min_y = 0.0f;

	for(int i = 0; i < CT_COUNT; ++i)
	{
		if(min_y > m_Corners[i].y)
		{
			min_y = m_Corners[i].y;
		}
	}

	return min_y;
}
/*********************************************************************************/
float AABB::getMinZ() const
{
	float min_z = 0.0f;

	for(int i = 0; i < CT_COUNT; ++i)
	{
		if(min_z > m_Corners[i].z)
		{
			min_z = m_Corners[i].z;
		}
	}

	return min_z;
}
/*********************************************************************************/
void AABB::merge(const AABB& p_AABB)
{
	for(int i = 0; i < CT_COUNT; ++i)
	{
		if(getCorner(i) < p_AABB.getCorner(i))
		{
			m_Corners[i] = p_AABB.m_Corners[i];
		}
	}
}
/*********************************************************************************/
bool AABB::operator<(const AABB& p_Rhs)
{
	for(int i = 0; i < CT_COUNT; ++i)
	{
		if(getCorner(i) < p_Rhs.getCorner(i))
		{
			return true;
		}
	}

	return false;
}
/*********************************************************************************/
bool AABB::operator>(const AABB& p_Rhs)
{
	return !operator<(p_Rhs);
}
/*********************************************************************************/