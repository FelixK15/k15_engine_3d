/**
 * @file TD_GameObjectFactory.cpp
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

#include "TD_GameObjectFactory.h"
#include "TD_MoveComponent.h"
#include "TD_CharacterStatsComponent.h"
#include "TD_AnimationComponent.h"

#include "K15_ModelComponent.h"
#include "K15_GameObject.h"
#include "K15_ResourceManager.h"
#include "K15_SubMesh.h"
#include "K15_Material.h"

namespace TowerDefense {
	/*********************************************************************************/
	GameObject* GameObjectFactory::createSpider()
	{
		GameObject* spider = K15_NEW GameObject("Spider");

		spider->addComponent(K15_NEW MoveComponent(Vector3(0.0f,0.0f,1.0f)));
		spider->addComponent(K15_NEW ModelComponent("meshes/plane_spider.obj"));
		spider->addComponent(K15_NEW CharacterStatsComponent(10,1));
		spider->addComponent(K15_NEW AnimationComponent(1.0f,4));
		SubMesh* mesh = spider->getComponentByType<ModelComponent>()->getMesh()->getSubMesh(0);
		
		Material* spiderMaterial = g_ResourceManager->getResource<Material>("materials/spider.json");
		MaterialPass* pass = spiderMaterial->getPass(0,true);
		AlphaState state = pass->getAlphaState();
		state.setSourceBlendFunction(AlphaState::BF_SRC_ALPHA);
		state.setDestinationBlendFunction(AlphaState::BF_ONE_MINUS_SRC_ALPHA);
		state.setEnabled(true);

		pass->setAlphaState(state);
		mesh->setMaterial(spiderMaterial);
		return spider;
	}
	/*********************************************************************************/
}// end of K15_Engine::Core namespace