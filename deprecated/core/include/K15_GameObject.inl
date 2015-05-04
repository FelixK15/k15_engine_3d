/**
 * @file K15_GameObject.inl
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

/*********************************************************************************/
NodeComponent* GameObject::getNodeComponent() const
{
  return m_NodeComponent;
}
/*********************************************************************************/
CameraComponent* GameObject::getCameraComponent() const
{
	return m_CameraComponent;
}
/*********************************************************************************/
ModelComponent* GameObject::getModelComponent() const
{
	return m_ModelComponent;
}
/*********************************************************************************/
LightComponent* GameObject::getLightComponent() const
{
	return m_LightComponent;
}
/*********************************************************************************/
const GameObject::ComponentArray& GameObject::getComponents() const
{
	return m_Components;
}
/*********************************************************************************/
template<class ComponentType>
ComponentType* GameObject::getComponentByType() const
{
	for(ComponentArray::const_iterator iter = m_Components.begin();iter != m_Components.end();++iter)
	{
		if((*iter)->getType().isInstanceOf(ComponentType::TYPE))
		{
			return (ComponentType*)(*iter);
		}
	}
	return 0;
}
/*********************************************************************************/