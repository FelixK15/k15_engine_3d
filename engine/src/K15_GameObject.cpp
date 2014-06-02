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

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  K15_IMPLEMENT_RTTI_BASE(Core,GameObject,Core::Object);
  /*********************************************************************************/
  GameObject::GameObject()
	  : Object(),
	  m_Node(),
	  m_Components()
  {
  
  }
  /*********************************************************************************/
  GameObject::GameObject(const ObjectName& p_Name)
	  : Object(p_Name),
	  m_Node(),
	  m_Components()
  {

  }
  /*********************************************************************************/
  GameObject::~GameObject()
  {
	  
  }
  /*********************************************************************************/
  void GameObject::addComponent(GameObjectComponentBase* p_Component)
  {
    K15_ASSERT(p_Component,"Component is NULL.");

    for(ComponentList::const_iterator iter = m_Components.begin();iter != m_Components.end();++iter)
    {
      if((*iter) == p_Component || (*iter)->getName() == p_Component->getName())
      {
        _LogWarning("Component \"%s\"(Type:%s) has already been added to GameObject \"%s\".",p_Component->getName().c_str(),p_Component->getTypeName().c_str(),m_Name.c_str());
        return;
      }
    }

    p_Component->setGameObject(this);
    m_Components.push_back(p_Component);

    return;
  }
  /*********************************************************************************/
  GameObjectComponentBase* GameObject::getComponentByName(const ObjectName& p_TypeName) const
  {
    for(ComponentList::const_iterator iter = m_Components.begin();iter != m_Components.end();++iter)
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
	  for(ComponentList::iterator iter = m_Components.begin();iter != m_Components.end();++iter)
	  {
		  (*iter)->update(p_GameTime);
	  }
  }
  /*********************************************************************************/
}}// end of K15_Engine::Core namespace