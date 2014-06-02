/**
 * @file TD_AnimationComponent.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/28
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

#include "TD_AnimationComponent.h"

#include "K15_GameTime.h"
#include "K15_ModelComponent.h"
#include "K15_GameObject.h"
#include "K15_VertexBuffer.h"
#include "K15_Vertex.h"
#include "K15_SubMesh.h"
#include "K15_Mesh.h"

namespace TowerDefense {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(TowerDefense,AnimationComponent,GameObjectComponentBase);
	/*********************************************************************************/

	/*********************************************************************************/
	AnimationComponent::AnimationComponent(float p_AnimationTime,int p_AnimationSteps)
		: GameObjectComponentBase(_TN(AnimationComponent)),
		m_AnimationTime(.0f),
		m_AnimationTimeOrig(p_AnimationTime),
		m_AnimationSteps(p_AnimationSteps),
		m_AnimationOffset(1.0f / p_AnimationSteps)
	{

	}
	/*********************************************************************************/
	AnimationComponent::~AnimationComponent()
	{

	}
	/*********************************************************************************/
	void AnimationComponent::update(const GameTime& p_GameTime)
	{
		m_AnimationTime -= p_GameTime.getDeltaTime();

		if(m_AnimationTime <= 0.0f)
		{
			m_AnimationTime = m_AnimationTimeOrig;

			ModelComponent* component = getGameObject()->getComponentByType<ModelComponent>();

			if(component)
			{
				VertexBuffer* vbo = component->getMesh()->getSubMesh(0)->getVertexBuffer();
				
				for(uint32 i = 0;i < vbo->getVertexCount();++i)
				{
					Vertex* vertex = vbo->getVertex(i);
					Vector2 vertexUV = vertex->getUV();
					vertexUV += m_AnimationOffset;
					vertex->setUV(vertexUV);
				}
			}
		}
	}
	/*********************************************************************************/
}