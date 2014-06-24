/**
 * @file K15_ModelComponent.cpp
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

#include "K15_ModelComponent.h"
#include "K15_ResourceManager.h"

namespace K15_Engine { namespace Core {
  /*********************************************************************************/
  K15_IMPLEMENT_RTTI_BASE(Core,ModelComponent,GameObjectComponentBase);
  /*********************************************************************************/

  /*********************************************************************************/
  ModelComponent::ModelComponent()
	  : GameObjectComponentBase(),
	  m_Model(0)
  {

  }
  /*********************************************************************************/
  ModelComponent::ModelComponent(const String& p_ModelResourceName)
	: GameObjectComponentBase(),
	m_Model(0)
  {
    //m_Model = g_ResourceManager->getResource<Model>(p_ModelResourceName);
  }
  /*********************************************************************************/
  ModelComponent::ModelComponent(Model* p_Mesh)
	  : GameObjectComponentBase(),
	  m_Model(m_Model)
  {

  }
  /*********************************************************************************/
  ModelComponent::~ModelComponent()
  {

  }
  /*********************************************************************************/
  AABB ModelComponent::_calculateAABB()
  {
	  AABB aabb;
// 
// 	  if(m_Model)
// 	  {
// 		  aabb = m_Model->getAABB();
// 	  }
// 
 	  return aabb;
  }
  /*********************************************************************************/
}}//end of K15_Engine::Core namespace