/**
 * @file K15_CameraComponent.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_RendererBase.h"
#include "K15_Node.h"
#include "K15_GameObject.h"
#include "K15_CameraComponent.h"
#include "K15_MatrixUtil.h"
// #include "K15_MathUtil.h"
// #include "K15_Vector3.h"
// #include "K15_Vector2.h"
#include "K15_AABB.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Rendering,CameraComponent,GameObjectComponentBase);
	/*********************************************************************************/

	/*********************************************************************************/
	CameraComponent::CameraComponent()
		: GameObjectComponentBase(_ON(CameraComponent)),
		m_Fov(65),
		m_ProjectionType(PT_PERSPECTIVE),
		m_Dirty(true),
		m_NearClipDistance(1.0f),
		m_FarClipDistance(200.0f),
		m_Zoom(1.0f)
	{
		setProjectionType(PT_PERSPECTIVE);
	}
	/*********************************************************************************/
	CameraComponent::~CameraComponent()
	{

	}
	/*********************************************************************************/
	void CameraComponent::setProjectionType(Enum p_ProjectionType)
	{
		m_ProjectionType = p_ProjectionType;
		
		float aspect = g_Application->getRenderWindow()->getResolution().getAspectRatio();
		float width = (float)g_Application->getRenderWindow()->getResolution().width;
		float height = (float)g_Application->getRenderWindow()->getResolution().height;
		//update projection matrix 
		if(m_ProjectionType == PT_PERSPECTIVE)
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov),aspect,m_NearClipDistance,m_FarClipDistance);/*MatrixUtil::createPerspectiveProjectionMatrix(m_Fov,m_NearClipDistance,m_FarClipDistance);*/
		}
		else
		{
			float left = -aspect / m_Zoom;
			float right = aspect / m_Zoom;
			float top_bottom = 1.0f / m_Zoom;
			m_ProjectionMatrix = glm::ortho(left,right,-top_bottom,top_bottom,m_NearClipDistance,m_FarClipDistance);//MatrixUtil::createOrthographicProjectionMatrix(m_NearClipDistance,m_FarClipDistance);
		}

	}
	/*********************************************************************************/
	const Matrix4& CameraComponent::getProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}
	/*********************************************************************************/
	const Matrix4& CameraComponent::getViewMatrix()
	{
 		if(m_Dirty || m_GameObject->getNode().needUpdate())
 		{
			//update view matrix
			m_ViewMatrix = m_GameObject->getNode().getTransformation();
			m_ViewMatrix = glm::inverse(m_ViewMatrix);
			_calculateFrustumPoints();
			m_Dirty = false;
		}

		return m_ViewMatrix;
	}
	/*********************************************************************************/
	void CameraComponent::setActive(bool p_Active)
	{
		m_Active = true;
	
		if(g_Application->getRenderer())
		{
			g_Application->getRenderer()->setActiveCamera(this);
		}
	}
  /*********************************************************************************/
  bool CameraComponent::isVisible(const AABB& p_AABB)
  {
//     return glm::greaterThanEqual(p_AABB.getMin(),m_FrustumPoints[FP_NEAR_RIGHT_BOTTOM]) ||
//            glm::lessThanEqual(p_AABB.getMax(),m_FrustumPoints[FP_FAR_LEFT_TOP]);

    return true;
  }
  /*********************************************************************************/
  void CameraComponent::_calculateFrustumPoints()
  {
    
  }
  /*********************************************************************************/
  const Vector3& CameraComponent::getFrustumPoint(Enum p_FrustumPoint) const
  {
	K15_ASSERT(p_FrustumPoint >= 0 && p_FrustumPoint < FP_COUNT,"Invalid frustum point index.");

    return m_FrustumPoints[p_FrustumPoint];
  }
  /*********************************************************************************/
}}// end of K15_Engine::Rendering namespace