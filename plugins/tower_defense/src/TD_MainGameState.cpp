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


#include "K15_Application.h"
#include "K15_RenderTask.h"
#include "K15_GameObject.h"
#include "K15_Texture.h"
#include "K15_TextureSampler.h"
#include "K15_CameraComponent.h"
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
		m_Level(0)
	{

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
		static RenderProcess* renderProcess = (RenderProcess*)g_Application->getRenderTask()->getRenderProcess();
		m_Level->draw(renderProcess);
	}
	/*********************************************************************************/
} // end of TowerDefense namepsace
