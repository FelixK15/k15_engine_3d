/**
 * @file TD_MainGameState.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/19
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
#include "TD_MainGameState.h"
#include "TD_Level.h"
#include "TD_GameObjectFactory.h"
#include "TD_RenderProcess.h"
#include "TD_MoveComponent.h"

#include "K15_Application.h"
#include "K15_Mesh.h"
#include "K15_SubMesh.h"
#include "K15_RenderTask.h"
#include "K15_GameObject.h"
#include "K15_ModelComponent.h"
#include "K15_Texture.h"
#include "K15_TextureSampler.h"
#include "K15_CameraComponent.h"
#include "K15_RenderQueue.h"
#include "K15_RenderOperation.h"
#include "K15_RendererBase.h"
#include "K15_ResourceManager.h"
#include "K15_ResourceArchiveZip.h"
#include "K15_ResourceImporterFont.h"
#include "K15_ResourceImporterObj.h"
#include "K15_ResourceImporterTiff.h"
#include "K15_ResourceImporterMaterial.h"

namespace TowerDefense {
	/*********************************************************************************/
	MainGameState::MainGameState()
		: GameStateBase("MainGameState"),
		m_Camera(0),
		m_Level(0),
		m_WaveCounter(0),
		m_SpawnRate(1.0f),
		m_SpawnRateTimer(1.0f)
	{
		for(int i = 0;i < AMOUNT_ENEMIES;++i)
		{
			m_Enemies[i] = 0;
		}
	}
	/*********************************************************************************/
	MainGameState::~MainGameState()
	{

	}
	/*********************************************************************************/
	void MainGameState::initialize(GameStateBase* p_PrevGameState)
	{
		g_ResourceManager->addResourceArchive(K15_NEW ResourceArchiveZip(g_Application->getGameRootDir() + "resources.zip"));
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterTiff());
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterObj());
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterMaterial());
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterFont());

		m_Camera = K15_NEW GameObject("Camera");
		m_Camera->addComponent(K15_NEW CameraComponent());
		m_Camera->getComponentByType<CameraComponent>()->setZoom(.15f);
		m_Camera->getComponentByType<CameraComponent>()->setProjectionType(CameraComponent::PT_ORTHOGRAPHIC);

		m_Level = K15_NEW Level("level0");
		m_Camera->getNode().rotate(Vector3(0.0f,1.0f,0.0f),-glm::radians(45.0f));
		m_Camera->getNode().rotate(Vector3(1.0f,0.0f,0.0f),-glm::radians(45.264f));

		Vector4 cameraLookAt = m_Camera->getNode().getOrientation() * Vector4(0.0f,0.0f,10.f,1.0f);
		m_Camera->getNode().translate(cameraLookAt);
		g_Application->getRenderer()->setActiveCameraGameObject(m_Camera);

		TextureSampler* sampler = K15_NEW TextureSampler();
		g_Application->getRenderer()->bindTextureSampler(sampler,Texture::TS_SLOT1);
	}
	/*********************************************************************************/
	void MainGameState::shutdown()
	{

	}
	/*********************************************************************************/
	void MainGameState::update(const GameTime& p_GameTime)
	{
		if(g_Application->getRenderer() && g_Application->getRenderer()->isInitialized())
		{
			static RenderProcess* renderProcess = (RenderProcess*)g_Application->getRenderTask()->getRenderProcess();
			static RenderQueue* rops = K15_NEW RenderQueue();		
			static int currentEnemy = 0;
			m_SpawnRateTimer -= p_GameTime.getDeltaTime();
			if(m_SpawnRateTimer <= 0.0f)
			{
				if(currentEnemy != AMOUNT_ENEMIES)
				{
					//m_SpawnRateTimer = m_SpawnRate;
					//m_Enemies[currentEnemy] = GameObjectFactory::createSpider();
					//m_Enemies[currentEnemy++]->getNode().setPosition(m_Level->getStartPosition() + Vector3(0.0f,1.0f,0.0f));
				}
			}

			m_Level->draw(rops);

			for(int i = 0;i < AMOUNT_ENEMIES;++i)
			{
				if(m_Enemies[i])
				{
					m_Enemies[i]->update(p_GameTime);
					_checkCollision(m_Enemies[i]);

					if(_isInGoal(m_Enemies[i]))
					{
						K15_DELETE m_Enemies[i];
						m_Enemies[i] = 0;
					}
					else
					{
						RenderOperation* rop = K15_NEW RenderOperation();
						rop->gameobject = m_Enemies[i];
						rop->indexBuffer = m_Enemies[i]->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getIndexBuffer();
						rop->vertexBuffer = m_Enemies[i]->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getVertexBuffer();
						rop->material = m_Enemies[i]->getComponentByType<ModelComponent>()->getMesh()->getSubMeshes()[0]->getMaterial();
						rop->topology = RenderOperation::T_TRIANGLE;
						rops->addRenderOperation(rop);
					}
				}
			}

			if(renderProcess)
			{
				renderProcess->setRenderQueue(rops);
			}
		}
	}
	/*********************************************************************************/
	void MainGameState::_checkCollision(GameObject* p_Enemy)
	{
		const Level::GameObjectArray& walls = m_Level->getWalls();
		bool collided = true;
		for(uint32 i = 0;i < walls.size();++i)
		{
			const AABB& aabb = walls.at(i)->getComponentByType<ModelComponent>()->getMesh()->getAABB(true);
			const Vector3& meshPosition = walls.at(i)->getNode().getPosition();
			const Vector3& enemyPos = p_Enemy->getNode().getPosition() + p_Enemy->getComponentByType<MoveComponent>()->getSpeed();

			if(enemyPos.x > aabb.getMaxX() + meshPosition.x) collided = false;
			//if(enemyPos.y > aabb.getMaxY()) collided = false;
			if(enemyPos.z > aabb.getMaxZ() + meshPosition.z) collided = false;

			if(enemyPos.x < aabb.getMinX() + meshPosition.x) collided = false;
			//if(enemyPos.x < aabb.getMinY()) collided = false;
			if(enemyPos.z < aabb.getMinZ() + meshPosition.z) collided = false;

			if(collided)
			{
				if(p_Enemy->getComponentByType<MoveComponent>()->getSpeed() == Vector3(0.0f,0.0f,1.0f))
				{
					p_Enemy->getComponentByType<MoveComponent>()->setSpeed(Vector3(1.0f,0.0f,0.0f));
				}
				else
				{
					p_Enemy->getComponentByType<MoveComponent>()->setSpeed(Vector3(0.0f,0.0f,1.0f));
				}
			}

			collided = true;

		}
		
	}
	/*********************************************************************************/
	bool MainGameState::_isInGoal(GameObject* p_Enemy)
	{
		const AABB& endAABB = m_Level->getEnd()->getComponentByType<ModelComponent>()->getMesh()->getAABB(true);
		const Vector3& meshPosition = m_Level->getEnd()->getNode().getPosition();
		const Vector3& enemyPos = p_Enemy->getNode().getPosition();
		bool collided = true;

		if(enemyPos.x > endAABB.getMaxX() + meshPosition.x) collided = false;
		//if(enemyPos.y > aabb.getMaxY()) collided = false;
		if(enemyPos.z > endAABB.getMaxZ() + meshPosition.z) collided = false;

		if(enemyPos.x < endAABB.getMinX() + meshPosition.x) collided = false;
		//if(enemyPos.x < aabb.getMinY()) collided = false;
		if(enemyPos.z < endAABB.getMinZ() + meshPosition.z) collided = false;

		return collided;
	}
	/*********************************************************************************/
	bool MainGameState::onEventMousePressed(GameEvent* p_Event)
	{
		return true;
	}
	/*********************************************************************************/
} // end of TowerDefense namespace
