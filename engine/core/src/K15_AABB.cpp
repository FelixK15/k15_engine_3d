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
	{
		m_Max.x =  0.5f; m_Max.y =  0.5f; m_Max.z = 0.5f;
		m_Min.x = -0.5f; m_Min.y = -0.5f; m_Min.z = -0.5f;
	}
	/*********************************************************************************/
	AABB::AABB(const Vector3& p_Min, const Vector3& p_Max)
		: m_Min(p_Min),
		m_Max(p_Max)
	{

	}
	/*********************************************************************************/
	AABB::~AABB()
	{

	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace