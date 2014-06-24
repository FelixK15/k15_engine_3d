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
#include "K15_NodeComponent.h"
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
		m_ViewMatrixDirty(true),
		m_ProjMatrixDirty(true),
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
	const Matrix4& CameraComponent::getProjectionMatrix()
	{
		if(m_ProjMatrixDirty)
		{
			float aspect = RenderWindow::getAspectRatio();
			float width = (float)RenderWindow::getWidth();
			float height = (float)RenderWindow::getHeight();
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

			_calculateFrustum();
			m_ProjMatrixDirty = false;
		}

		return m_ProjectionMatrix;
	}
	/*********************************************************************************/
	const Matrix4& CameraComponent::getViewMatrix()
	{
 		if(m_ViewMatrixDirty || m_GameObject->getNodeComponent()->needUpdate())
 		{
			//update view matrix
			m_ViewMatrix = m_GameObject->getTransformation();
			m_ViewMatrix = glm::inverse(m_ViewMatrix);
			_calculateFrustum();
			m_ViewMatrixDirty = false;
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
    for(int i = 0; i < AABB::CT_COUNT; ++i)
    {
      if(m_Frustum.isInside(p_AABB.getCorner(i)))
      {
        return true;
      }
    }

		return false;
	}
	/*********************************************************************************/
	void CameraComponent::_calculateFrustum()
	{
		static FixedArray(Vector4, Frustum::FP_COUNT) FrustumPointsHomogenous;

		float left	 = -1.0f;
		float right	 =  1.0f;
		float top	 = -1.0f;
		float bottom =  1.0f;
		float f		 = -1.0f;
		float n		 =  1.0f;

		Matrix4 viewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;

		FrustumPointsHomogenous[Frustum::FP_FAR_LEFT_BOTTOM]	= Vector4(left,bottom,f,1.0f);
		FrustumPointsHomogenous[Frustum::FP_FAR_RIGHT_BOTTOM] = Vector4(right,bottom,f,1.0f);
		FrustumPointsHomogenous[Frustum::FP_FAR_LEFT_TOP]	    = Vector4(left,top,f,1.0f);
		FrustumPointsHomogenous[Frustum::FP_FAR_RIGHT_TOP]	  = Vector4(right,top,f,1.0f);

		FrustumPointsHomogenous[Frustum::FP_NEAR_LEFT_BOTTOM]   = Vector4(left,bottom,n,1.0f);
		FrustumPointsHomogenous[Frustum::FP_NEAR_RIGHT_BOTTOM]  = Vector4(right,bottom,n,1.0f);
		FrustumPointsHomogenous[Frustum::FP_NEAR_LEFT_TOP]	    = Vector4(left,top,n,1.0f);
		FrustumPointsHomogenous[Frustum::FP_NEAR_RIGHT_TOP]     = Vector4(right,top,n,1.0f);

		for(int i = 0; i < Frustum::FP_COUNT; ++i)
		{
			FrustumPointsHomogenous[i] = FrustumPointsHomogenous[i] * viewProjectionMatrix;
			m_Frustum.setCorner(Vector3(FrustumPointsHomogenous[i].x, FrustumPointsHomogenous[i].y, FrustumPointsHomogenous[i].z),i);
		}

		m_Frustum.calculatePlanes();
	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace