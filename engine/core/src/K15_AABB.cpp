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
		float f	= 0.5f, t = 0.5f, r = 0.5f;
		float n	= -f,	b = -t,	  l = -r;

		m_Corners[CT_FAR_LEFT_TOP] = Vector3(l,t,f);
		m_Corners[CT_FAR_RIGHT_TOP] = Vector3(r,t,f);
		m_Corners[CT_FAR_LEFT_BOTTOM] = Vector3(l,b,f);
		m_Corners[CT_FAR_RIGHT_BOTTOM] = Vector3(r,b,f);

		m_Corners[CT_NEAR_LEFT_TOP] = Vector3(l,t,n);
		m_Corners[CT_NEAR_RIGHT_TOP] = Vector3(r,t,n);
		m_Corners[CT_NEAR_LEFT_BOTTOM] = Vector3(l,b,n);
		m_Corners[CT_NEAR_RIGHT_BOTTOM] = Vector3(r,b,n);
	}
	/*********************************************************************************/
	AABB::~AABB()
	{

	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace