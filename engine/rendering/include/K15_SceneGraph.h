/**
 * @file K15_SceneGraph.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Rendering_SceneGraph_h_
#define _K15Engine_Rendering_SceneGraph_h_

#include "K15_Prerequisites.h"
#include "K15_Object.h"
#include "K15_AllocatedObject.h"
#include "K15_GameObject.h"

#include "K15_RendererBase.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API SceneGraph : public Object, public RenderingAllocatedObject
	{
    K15_DECLARE_RTTI;

	public:
		typedef DynamicArray(NodeComponent*) SceneNodeArray;
		typedef DynamicArray(CameraComponent*) CameraArray;
		
	public:
		SceneGraph();
		~SceneGraph();

		void addCamera(GameObject* p_Camera);
		void removeCamera(GameObject* p_Camera);

		void addGameObject(GameObject* p_GameObject);
		void removeGameObject(GameObject* p_GameObject);

		void traverse(RenderQueue* p_RenderQueue);

		const RendererBase::LightArray& getVisibleLights() const {return m_VisibleLights;}

		static SceneGraph* getDefault();

	private:
		CameraArray _getActiveCameras() const;
		SceneNodeArray _getVisibleObjects(const CameraArray& p_ActiveCameras) const;
        GameObject::ComponentArray _collectVisibleComponents(SceneNodeArray p_VisibleNodes) const;
		RendererBase::LightArray _collectLights(SceneNodeArray p_VisibileNodes) const;

	private:
		static SceneGraph ms_Default;

  private:
		RendererBase::LightArray m_VisibleLights;
		SceneNodeArray m_SceneNodes;
		CameraArray m_Cameras;
	};// end of SceneGraph class declaration
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_SceneGraph_h_
