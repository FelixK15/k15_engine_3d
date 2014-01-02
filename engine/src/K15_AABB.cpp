/**
 * @file K15_AABB.cpp
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

#include "K15_PrecompiledHeader.h"

#include "K15_AABB.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	AABB::AABB()
		: m_Max(Math::Vector4::Zero),
		m_Min(Math::Vector4::Zero)
	{

	}
	/*********************************************************************************/
	AABB::AABB(const Math::Vector4& p_Min, const Math::Vector4& p_Max)
		: m_Min(p_Min),
		m_Max(p_Max)
	{

	}
	/*********************************************************************************/
	void AABB::_calculateCorners()
	{
		m_Corners[CT_FAR_LEFT_TOP] = m_Max;
		m_Corners[CT_FAR_RIGHT_TOP] = Math::Vector4(m_Min.x,m_Max.y,m_Max.z);
		m_Corners[CT_FAR_LEFT_BOTTOM] = Math::Vector4(m_Max.x,m_Min.y,m_Max.z);
		m_Corners[CT_FAR_RIGHT_BOTTOM] = Math::Vector4(m_Min.x,m_Min.y,m_Max.z);

		m_Corners[CT_NEAR_LEFT_TOP] = Math::Vector4(m_Max.x,m_Max.y,m_Min.z);
		m_Corners[CT_NEAR_RIGHT_TOP] = Math::Vector4(m_Min.x,m_Max.y,m_Min.z);
		m_Corners[CT_NEAR_LEFT_BOTTOM] = Math::Vector4(m_Max.x,m_Min.y,m_Min.z);
		m_Corners[CT_NEAR_RIGHT_BOTTOM] = m_Min;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace