/**
 * @file K15_Frustum.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/07
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

#include "K15_Frustum.h"
#include "K15_Vector3.h"

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  Frustum::Frustum()
    : m_Corners(),
    m_Planes()
  {

  }
  /*********************************************************************************/
  Frustum::~Frustum()
  {

  }
  /*********************************************************************************/
  void Frustum::calculatePlanes()
  {
    m_Planes[CP_FAR].normal	  = Vector3::Cross(m_Corners[FP_FAR_LEFT_TOP] - m_Corners[FP_FAR_RIGHT_TOP], m_Corners[FP_FAR_RIGHT_BOTTOM] - m_Corners[FP_FAR_RIGHT_TOP]);
    m_Planes[CP_FAR].position = (m_Corners[FP_FAR_LEFT_TOP] - m_Corners[FP_FAR_RIGHT_BOTTOM]) * 0.5f;

    m_Planes[CP_NEAR].normal	 = -m_Planes[CP_FAR].normal;
    m_Planes[CP_NEAR].position = (m_Corners[FP_NEAR_LEFT_TOP] - m_Corners[FP_NEAR_RIGHT_BOTTOM]) * 0.5f;

    m_Planes[CP_TOP].normal	   = Vector3::Cross(m_Corners[FP_FAR_RIGHT_TOP] - m_Corners[FP_FAR_LEFT_TOP], m_Corners[FP_NEAR_LEFT_TOP] - m_Corners[FP_FAR_LEFT_TOP]);
    m_Planes[CP_BOTTOM].normal = Vector3::Cross(m_Corners[FP_FAR_LEFT_BOTTOM] - m_Corners[FP_FAR_RIGHT_BOTTOM], m_Corners[FP_FAR_LEFT_BOTTOM] - m_Corners[FP_NEAR_LEFT_BOTTOM]);
    m_Planes[CP_LEFT].normal	 = Vector3::Cross(m_Corners[FP_NEAR_LEFT_TOP] - m_Corners[FP_NEAR_LEFT_BOTTOM], m_Corners[FP_FAR_LEFT_BOTTOM] - m_Corners[FP_NEAR_LEFT_BOTTOM]);
    m_Planes[CP_RIGHT].normal  = Vector3::Cross(m_Corners[FP_NEAR_RIGHT_TOP] - m_Corners[FP_NEAR_RIGHT_BOTTOM], m_Corners[FP_FAR_RIGHT_BOTTOM] - m_Corners[FP_NEAR_RIGHT_BOTTOM]);

    m_Planes[CP_TOP].position		  = m_Corners[FP_FAR_RIGHT_TOP];
    m_Planes[CP_BOTTOM].position  = m_Corners[FP_FAR_RIGHT_BOTTOM];
    m_Planes[CP_LEFT].position	  = m_Corners[FP_FAR_LEFT_BOTTOM];
    m_Planes[CP_RIGHT].position	  = m_Corners[FP_FAR_RIGHT_BOTTOM];

    for(int i = 0; i < CP_COUNT; ++i)
    {
      m_Planes[i].normal.normalize();
    }
  }
  /*********************************************************************************/
  bool Frustum::isInside(const Vector3& p_Position)
  {
    for(int i = 0; i < CP_COUNT; ++i)
    {
      if(Vector3::Dot(p_Position - m_Planes[i].position, m_Planes[i].normal) > 0.f)
      {
        return true;
      }
    }
    
    return false;
  }
  /*********************************************************************************/
}}// end of K15_Engine::Core namespace