/**
 * @file K15_SceneGraph.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/04
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
#include "K15_SceneGraph.h"

#include "K15_SortUtil.h"
#include "K15_CameraComponent.h"
#include "K15_NodeComponent.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,SceneGraph,Object);
	SceneGraph SceneGraph::ms_Default;
	/*********************************************************************************/

	/*********************************************************************************/
	SceneGraph::SceneGraph()
	{

	}
	/*********************************************************************************/
	SceneGraph::~SceneGraph()
	{

	}
	/*********************************************************************************/
	void SceneGraph::traverse(RenderQueue* p_RenderQueue)
	{
		//find active cameras
		CameraArray const activeCameras = _getActiveCameras();
		
		if(activeCameras.size() > 0)
		{
			//find visible objects
			SceneNodeArray const visibleNodes = _getVisibleObjects(activeCameras);
			GameObject::ComponentArray const visibleComponents = _collectVisibleComponents(visibleNodes);
			
			GameObjectComponentBase* component = 0;

			for(GameObject::ComponentArray::const_iterator iter = visibleComponents.begin();
				iter != visibleComponents.end(); ++iter)
			{
				component = (*iter);
				component->onAddedToRenderQueue(p_RenderQueue);
			}
		}
	}
	/*********************************************************************************/
	SceneGraph* SceneGraph::getDefault()
	{
		return &ms_Default;
	}
	/*********************************************************************************/
	SceneGraph::CameraArray SceneGraph::_getActiveCameras() const
	{
		CameraArray activeCameras;
		CameraComponent* camera = 0;

		for(CameraArray::const_iterator iter = m_Cameras.begin(); iter != m_Cameras.end(); ++iter)
		{
			camera = (*iter);

			if(camera->getActive())
			{
				activeCameras.push_back(camera);
			}
		}

		return activeCameras;
	}
	/*********************************************************************************/
	SceneGraph::SceneNodeArray SceneGraph::_getVisibleObjects(const CameraArray& p_ActiveCameras) const
	{
		SceneNodeArray visibleObjects;
		CameraComponent* camera = 0;
		NodeComponent* node = 0;
		for(CameraArray::const_iterator c_iter = p_ActiveCameras.begin();
			c_iter != p_ActiveCameras.end(); ++c_iter)
		{
			camera = (*c_iter);

			for(SceneNodeArray::const_iterator n_iter = m_SceneNodes.begin();
				n_iter != m_SceneNodes.end(); ++n_iter)
			{
				node = (*n_iter);

				if(camera->isVisible(node->getAABB()))
				{
					visibleObjects.push_back(node);
				}
			}
		}

		return visibleObjects;
	}
	/*********************************************************************************/
	GameObject::ComponentArray SceneGraph::_collectVisibleComponents(SceneNodeArray p_VisibleNodes) const
	{
		GameObject* gameobject = 0;
		GameObject::ComponentArray components;

		for(SceneNodeArray::const_iterator iter = p_VisibleNodes.begin();
			iter != p_VisibleNodes.end(); ++iter)
		{
			gameobject = (*iter)->getGameObject();

			const GameObject::ComponentArray& gameobjectComponents = gameobject->getComponents();
			components.insert(components.end(), gameobjectComponents.begin(), gameobjectComponents.end());
		}

		//sort components by component type
		Sort(components.begin(), components.end(), SortUtil::sortComponentsByType);

		return components;
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace
