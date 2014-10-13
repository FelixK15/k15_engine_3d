/**
 * @file K15_GameObject.cpp
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

#include "K15_GameObject.h"
#include "K15_GameObjectComponentBase.h"
#include "K15_AABB.h"

#include "K15_CameraComponent.h"
#include "K15_ModelComponent.h"
#include "K15_LightComponent.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,GameObject,Core::Object);
	/*********************************************************************************/
	GameObject::GameObject()
		: Object(),
		m_Components(),
		m_NodeComponent(K15_NEW NodeComponent(this)),
		m_CameraComponent(0)
	{
    
	}
	/*********************************************************************************/
	GameObject::GameObject(const ObjectName& p_Name)
		: Object(p_Name),
		m_Components(),
		m_NodeComponent(K15_NEW NodeComponent(this)),
		m_CameraComponent(0)
	{
		
	}
	/*********************************************************************************/
	GameObject::~GameObject()
	{
	  for(uint32 i = 0; i < m_Components.size(); ++i)
	  {
		  K15_DELETE m_Components[i];
	  }

	  m_Components.clear();
	}
	/*********************************************************************************/
	const AABB& GameObject::getAABB()
	{
		static AABB aabb;

		if(m_NodeComponent->needUpdate())
		{
			Vector3 cornerPos;
			for(ComponentArray::iterator iter = m_Components.begin(); iter != m_Components.end(); ++iter)
			{
				aabb.merge((*iter)->getAABB());
			}

			AABB::CornerArray& corners = aabb.getCorners();

			for(int i = 0; i < AABB::CT_COUNT; ++i)
			{
				corners[i] += getPosition();
			}
		}
		
		return aabb;
	}
	/*********************************************************************************/
	void GameObject::addComponent(GameObjectComponentBase* p_Component)
	{
		K15_ASSERT(p_Component,"Component is NULL.");

		for(ComponentArray::const_iterator iter = m_Components.begin();iter != m_Components.end();++iter)
		{
			if((*iter) == p_Component || (*iter)->getName() == p_Component->getName())
			{
				K15_LOG_WARNING("Component \"%s\"(Type:%s) has already been added to GameObject \"%s\".",p_Component->getName().c_str(),p_Component->getTypeName().c_str(),m_Name.c_str());
				return;
			}
		}

		p_Component->setGameObject(this);
		m_Components.push_back(p_Component);

		if(p_Component->getType().isInstanceOf(NodeComponent::TYPE))
		{
			m_NodeComponent = static_cast<NodeComponent*>(p_Component);
		}
		else if(p_Component->getType().isInstanceOf(CameraComponent::TYPE))
		{
			m_CameraComponent = static_cast<CameraComponent*>(p_Component);
		}
		else if(p_Component->getType().isInstanceOf(ModelComponent::TYPE))
		{
			m_ModelComponent = static_cast<ModelComponent*>(p_Component);
		}
		else if(p_Component->getType().isInstanceOf(LightComponent::TYPE))
		{
			m_LightComponent = static_cast<LightComponent*>(p_Component);
		}
		return;
	}
	/*********************************************************************************/
	GameObjectComponentBase* GameObject::getComponentByName(const ObjectName& p_TypeName) const
	{
		for(ComponentArray::const_iterator iter = m_Components.begin();iter != m_Components.end();++iter)
		{
			if((*iter)->getName() == p_TypeName)
			{
				return (*iter);
			}
		}

		return 0;
	}
	/*********************************************************************************/
	void GameObject::update(const GameTime& p_GameTime)
	{
		for(ComponentArray::iterator iter = m_Components.begin();iter != m_Components.end();++iter)
		{
			(*iter)->update(p_GameTime);
		}
	}
	/*********************************************************************************/
	void GameObject::onAddedToRenderQueue(RenderQueue* p_RenderQueue)
	{
		for(ComponentArray::iterator iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			(*iter)->onAddedToRenderQueue(p_RenderQueue);
		}
	}
	/*********************************************************************************/
	void GameObject::lookAt(const Vector3& p_Position)
	{
		m_NodeComponent->lookAt(p_Position);
	}
	/*********************************************************************************/
	void GameObject::setPosition( float p_PosX, float p_PosY, float p_PosZ )
	{
		setPosition(Vector3(p_PosX, p_PosY, p_PosZ));
	}
	/*********************************************************************************/
	void GameObject::setPosition(const Vector3& p_Position)
	{
		m_NodeComponent->setPosition(p_Position);
	}
	/*********************************************************************************/
	void GameObject::setOrientation(const Quaternion& p_Orientation)
	{		
		m_NodeComponent->setOrientation(p_Orientation);
	}
	/*********************************************************************************/
	void GameObject::setScale(const Vector3& p_Scale)
	{
		m_NodeComponent->setScale(p_Scale);
	}
	/*********************************************************************************/
	void GameObject::translate(const Vector3& p_Translation)
	{
		m_NodeComponent->translate(p_Translation);
	}
	/*********************************************************************************/
	void GameObject::translate(float x, float y, float z)
	{
		m_NodeComponent->translate(x, y, z);
	}
	/*********************************************************************************/
	void GameObject::scale(const Vector3& p_Scale)
	{
		m_NodeComponent->scale(p_Scale);
	}
	/*********************************************************************************/
	void GameObject::scale(float x, float y, float z)
	{
		m_NodeComponent->scale(x, y, z);
	}
	/*********************************************************************************/
	void GameObject::rotate(const Vector3& p_Axis, float p_Radians)
	{
		m_NodeComponent->rotate(p_Axis, p_Radians);
	}
	/*********************************************************************************/
	void GameObject::roll(float p_Radians)
	{
		m_NodeComponent->roll(p_Radians);
	}
	/*********************************************************************************/
	void GameObject::pitch(float p_Radians)
	{
		m_NodeComponent->pitch(p_Radians);
	}
	/*********************************************************************************/
	void GameObject::yaw(float p_Radians)
	{
		m_NodeComponent->yaw(p_Radians);
	}
	/*********************************************************************************/
	const Vector3& GameObject::getPosition() const
	{
		return m_NodeComponent->getPosition();
	}
	/*********************************************************************************/
	const Vector3& GameObject::getScale() const
	{
		return m_NodeComponent->getScale();
	}
	/*********************************************************************************/
	const Quaternion& GameObject::getOrientation() const
	{
		return m_NodeComponent->getOrientation();
	}
	/*********************************************************************************/
	const Vector3& GameObject::getLookAt() const
	{
		return m_NodeComponent->getLookAt();
	}
	/*********************************************************************************/
	const Matrix4& GameObject::getTransformation()
	{
		return m_NodeComponent->getTransformation();
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace