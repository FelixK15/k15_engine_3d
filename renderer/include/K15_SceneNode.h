/**
 * @file K15_SceneNode.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

#pragma once

#ifndef __K15_SCENENODE__
#define __K15_SCENENODE__

#include "K15_Object.h"
#include "K15_List.h"

#include "K15_Transformation.h"

using namespace K15_EngineV2;

namespace K15_Renderer
{
	class SceneNode : public Object
	{
		DECLARE_RTTI;

	public:
		virtual ~SceneNode();

		virtual void Update(/*GameTime &gtDeltaTime*/);

		void AddChild(SceneNode *pSceneNode);
		bool RemoveChild(SceneNode *pSceneNode);
		bool IsVisible();
		void SetVisible(bool bVisible);
		SceneNode *GetParent();
		
	protected:
		SceneNode();
		
		void _SetParent(SceneNode *pNode);

	public:
		Transformation Transformation;

	protected:
		SceneNode *m_pParent;
		List<SceneNode*> m_lChildren;
		bool m_bVisible;
	};

	#include "../src/K15_SceneNode.inl"
	typedef Pointer<SceneNode> SceneNodePtr;
}

#endif //__K15_SCENENODE__