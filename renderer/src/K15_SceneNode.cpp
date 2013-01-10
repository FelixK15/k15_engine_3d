/**
 * @file K15_SceneNode.cpp
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
 */

#include "K15_SceneNode.h"

using namespace K15_Renderer;

IMPLEMENT_RTTI("K15_Renderer",SceneNode,Object);

SceneNode::SceneNode()
{
	m_pParent = NULL;
}

SceneNode::~SceneNode()
{
	for(ListNode<SceneNode*> *pNode = m_lChildren.GetFirstElement();pNode;pNode = pNode->GetNext()){
		K15_DELETE pNode->GetElement();
	}

	m_lChildren.Clear();
}

void SceneNode::Update( /*GameTime &gtDeltaTime*/ )
{
	if(m_pParent){
		Transformation *= m_pParent->Transformation;
	}
}

bool SceneNode::RemoveChild(SceneNode *pSceneNode)
{
	bool bFound = false;
	for(ListNode<SceneNode*> *pNode = m_lChildren.GetFirstElement();pNode;pNode = pNode->GetNext()){
		if(pNode->GetElement() == pSceneNode){
			m_lChildren.Erase(pNode);
			bFound = true;
			break;
		}
	}

	return bFound;
}
