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
#include "K15_MeshManager.h"

#include "K15_MeshInstance.h"
#include "K15_SubMesh.h"
#include "K15_Mesh.h"

#include "K15_RenderOperation.h"
#include "K15_RenderQueue.h"


//temporary
#include "K15_ResourceManager.h"
#include "K15_Material.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ModelComponent,GameObjectComponentBase);
	/*********************************************************************************/

	/*********************************************************************************/
	ModelComponent::ModelComponent()
		: GameObjectComponentBase(),
		m_MeshInstance(0)
	{

	}
	/*********************************************************************************/
	ModelComponent::ModelComponent(const String& p_ModelResourceName)
	: GameObjectComponentBase(),
    m_MeshInstance(0)
	{
		m_MeshInstance = g_MeshManager->getMesh(p_ModelResourceName);
	}
	/*********************************************************************************/
	ModelComponent::ModelComponent(MeshInstance* p_MeshInstance)
		: GameObjectComponentBase(),
		m_MeshInstance(p_MeshInstance)
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

		if(m_MeshInstance)
		{
			aabb = m_MeshInstance->getAABB();
		}

 		return aabb;
	}
	/*********************************************************************************/
	void ModelComponent::onAddedToRenderQueue(RenderQueue* p_RenderQueue)
	{
		RenderOperation* rop = 0;
		SubMesh* submesh = 0;
		for(uint32 i = 0; i < m_MeshInstance->getMesh()->getSubMeshes().size(); ++i)
		{
			submesh = m_MeshInstance->getMesh()->getSubMesh(i);
			rop = K15_NEW RenderOperation();
			rop->gameobject = getGameObject();
			rop->vertexData = submesh->getVertexData();
			rop->indexData = submesh->getIndexData();
			rop->material = g_ResourceManager->getResource<Material>("wall.json");
			/*rop->material = submesh->getMaterial();*/
			rop->topology = RenderOperation::T_TRIANGLE;

			p_RenderQueue->addRenderOperation(rop);
		}
	}
	/*********************************************************************************/
}}//end of K15_Engine::Core namespace
