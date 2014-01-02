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

#include "K15_RenderTask.h"
#include "K15_RendererBase.h"
#include "K15_Node.h"
#include "K15_GameObject.h"
#include "K15_CameraComponent.h"
#include "K15_MatrixUtil.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Rendering,CameraComponent,Core::GameObjectComponentBase);
	/*********************************************************************************/

	/*********************************************************************************/
	CameraComponent::CameraComponent(GameObject* p_Parent)
		: GameObjectComponentBase(p_Parent,_N(CameraComponent)),
		m_Fov(65),
		m_ProjectionType(PT_PERSPECTIVE)
	{

	}
	/*********************************************************************************/
	CameraComponent::~CameraComponent()
	{

	}
	/*********************************************************************************/
	const Math::Matrix4& CameraComponent::getProjectionMatrix()
	{
		if(m_Dirty)
		{
			//update projection matrix 
			if(m_ProjectionType == PT_PERSPECTIVE)
			{
				m_ProjectionMatrix = MatrixUtil::createPerspectiveProjectionMatrix(m_Fov,m_NearClipDistance,m_FarClipDistance);
			}
			else
			{
				m_ProjectionMatrix = MatrixUtil::createOrthographicProjectionMatrix(m_NearClipDistance,m_FarClipDistance);
			}
      
			m_Dirty = false;
		}

		return m_ProjectionMatrix;
	}
	/*********************************************************************************/
	const Math::Matrix4& CameraComponent::getViewMatrix()
	{
		if(!m_Dirty)
		{
			if(m_GameObject->getNode()->needUpdate())
			{
				m_Dirty = true;
			}
		}

		if(m_Dirty)
		{
			//update view matrix
			m_ViewMatrix = m_GameObject->getNode()->getTransformation();
			m_ViewMatrix = m_ViewMatrix.inverse();
			m_Dirty = false;
		}

		return m_ViewMatrix;
	}
	/*********************************************************************************/
	void CameraComponent::setActive(bool p_Active)
	{
		m_Active = true;
	
		if(g_Application->getRenderTask()->getRenderer())
		{
			g_Application->getRenderTask()->getRenderer()->setActiveCamera(this);
		}
	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace