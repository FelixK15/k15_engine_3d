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
#include "K15_MathUtil.h"
#include "K15_AABB.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Rendering,CameraComponent,GameObjectComponentBase);
	/*********************************************************************************/

	/*********************************************************************************/
	CameraComponent::CameraComponent()
		: GameObjectComponentBase(_ON(CameraComponent)),
		m_Fov(MathUtil::HalfPi),
		m_ProjectionType(PT_PERSPECTIVE),
		m_ViewMatrixDirty(true),
		m_ProjMatrixDirty(true),
		m_NearClipDistance(1.0f),
		m_FarClipDistance(200.0f),
		m_Zoom(1.0f),
		m_Active(true)
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
			//update projection matrix 
			if(m_ProjectionType == PT_PERSPECTIVE)
			{
                m_ProjectionMatrix = MatrixUtil::createPerspectiveMatrix(m_Fov, aspect,
                                                                         m_NearClipDistance,
                                                                         m_FarClipDistance);
			}
			else
			{
				float left = -aspect / m_Zoom;
				float right = aspect / m_Zoom;
				float top_bottom = 1.0f / m_Zoom;
                m_ProjectionMatrix = MatrixUtil::createOrthographicMatrix(left,
                                                                          right,
                                                                          -top_bottom,
                                                                          top_bottom,
                                                                          m_NearClipDistance,
                                                                          m_FarClipDistance);
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
			m_ViewMatrix = m_ViewMatrix.inverse();
			_calculateFrustum();
			m_ViewMatrixDirty = false;
		}

		return m_ViewMatrix;
	}
	/*********************************************************************************/
	bool CameraComponent::isViewMatrixDirty() const
	{
		NodeComponent* node = 0;
		bool needUpdate = false;

		if(node = getGameObject()->getComponentByType<NodeComponent>())
		{
			needUpdate = node->needUpdate();
		}

		return m_ViewMatrixDirty || needUpdate;
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
		/*AABB::CornerArray corners = p_AABB.getCorners();
		Matrix4 ViewProj = m_ProjectionMatrix * m_ViewMatrix;
		Vector3 projectedPos;
		for(int i = 0; i < 2; ++i)
		{
			if(i == 0)
			{
				projectedPos = ViewProj * p_AABB.getMin();
			}
			else
			{
				projectedPos = ViewProj * p_AABB.getMax();
			}
			
			

			if(projectedPos.x <= 1.f && projectedPos.x >= 0.f ||
			   projectedPos.y <= 1.f && projectedPos.y >= 0.f ||
			   projectedPos.z <= 1.f && projectedPos.z >= 0.f)
			{
				return true;
			}
		}

		return false;*/

		return true;
	}
	/*********************************************************************************/
	void CameraComponent::_calculateFrustum()
	{
		m_Frustum.setFarPlaneDistance(m_FarClipDistance);
		m_Frustum.setNearPlaneDistance(m_NearClipDistance);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace
