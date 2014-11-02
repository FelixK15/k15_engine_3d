/**
 * @file K15_EngineTestModule.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#include "K15_EngineTestModule.h"
#include "K15_EngineTestRenderProcess.h"
#include "K15_EngineTestFirstPersonControls.h"

#include "K15_InputManager.h"
#include "K15_GameObject.h"

#include "K15_CameraComponent.h"
#include "K15_NodeComponent.h"
#include "K15_ModelComponent.h"
#include "K15_LightComponent.h"

#include "K15_TextureSampler.h"

#include "K15_ResourceManager.h"
#include "K15_ResourceArchiveFileSystem.h"
#include "K15_ResourceImporterObj.h"
#include "K15_ResourceImporterMaterial.h"
#include "K15_ResourceImporterTiff.h"

#include "K15_SceneGraph.h"
#include "K15_RenderTask.h"
#include "K15_RendererBase.h"

#include "K15_DebugRenderer.h"
#include "K15_MathUtil.h"

//debug
#include "K15_Keyboard.h"


namespace K15_Engine { namespace Test {
	/*********************************************************************************/
	TestGameState::TestGameState()
		: GameStateBase("TestGameState"),
		m_Camera(0),
		m_Bunny(0),
		m_Light1(0),
		m_Light2(0)
	{
		g_ResourceManager->addResourceArchive(K15_NEW ResourceArchiveFileSystem(g_Application->getGameRootDir() + "resources/"));
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterObj());
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterMaterial());
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterTiff());
	}
	/*********************************************************************************/
	TestGameState::~TestGameState()
	{

	}
	/*********************************************************************************/
	void TestGameState::initialize(GameStateBase* p_PrevGameState)
	{
		m_Camera = K15_NEW GameObject("Camera");
		m_Camera->addComponent(K15_NEW CameraComponent());
		m_Camera->addComponent(K15_NEW NodeComponent());
		m_Camera->addComponent(K15_NEW FirstPersonControlComponent());
		m_Camera->setPosition(0.f, 0.f, 5.f);
		TextureSampler* sampler = K15_NEW TextureSampler();
		sampler->setSlot(Texture::TS_SLOT1);

		g_Application->getRenderer()->bindTextureSampler(sampler, Texture::TS_SLOT1);
		g_Application->getRenderer()->setActiveCamera(m_Camera->getCameraComponent());
		g_Application->getRenderer()->setAmbientColor(ColorRGBA(20, 20, 20, 255));

		m_Bunny = K15_NEW GameObject("Bunny");
		m_Bunny->addComponent(K15_NEW NodeComponent());
		m_Bunny->addComponent(K15_NEW ModelComponent("crate.obj"));
		m_Bunny->setPosition(0.f, 0.f, 0.f);
		SceneGraph::getDefault()->addGameObject(m_Bunny);

		_addLights();

		SceneGraph::getDefault()->addCamera(m_Camera);

		g_Application->getRenderTask()->setRenderProcess(K15_NEW RenderProcess());
	}
	/*********************************************************************************/
	void TestGameState::shutdown()
	{

	}
	/*********************************************************************************/
	void TestGameState::update(const GameTime& p_GameTime)
	{
		m_Bunny->yaw(0.01f);
		m_Camera->update(p_GameTime);
		m_Bunny->update(p_GameTime);

		if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_LEFT))
		{
			m_Light1->translate(-1.0f, 0.0f, 0.0f);
		}
		else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_RIGHT))
		{
			m_Light1->translate(1.0f, 0.0f, 0.0f);
		}

		if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_UP))
		{
			m_Light1->translate(0.0f, 1.0f, 0.0f);
		}
		else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_DOWN))
		{
			m_Light1->translate(0.0f, -1.0f, 0.0f);
		}

		g_DebugRenderer->drawCross(m_Light1->getPosition(), 1.0f, m_Light1->getLightComponent()->getDiffuseColor());
		g_DebugRenderer->drawNormals(m_Bunny, ColorRGBA::Green);
	}
	/*********************************************************************************/
	void TestGameState::_addLights()
	{
		m_Light1 = K15_NEW GameObject("Light");
		m_Light2 = K15_NEW GameObject("Light2");
		m_Light3 = K15_NEW GameObject("Light3");
		m_Light1->addComponent(K15_NEW NodeComponent());
		m_Light1->addComponent(K15_NEW LightComponent());
		m_Light2->addComponent(K15_NEW NodeComponent());
		m_Light2->addComponent(K15_NEW LightComponent());
		m_Light3->addComponent(K15_NEW NodeComponent());
		m_Light3->addComponent(K15_NEW LightComponent());

		SceneGraph::getDefault()->addGameObject(m_Light1);
// 		SceneGraph::getDefault()->addGameObject(m_Light2);
// 		SceneGraph::getDefault()->addGameObject(m_Light3);

		m_Light1->translate(8.f, 0.f, 0.f);
		m_Light2->translate(-8.f, 0.f, 0.f);
		m_Light3->translate(0.f, 8.f, 0.f);

		LightComponent* light1 = m_Light1->getLightComponent();
		light1->setLightType(LightComponent::LT_POINT);
		light1->setDiffuseColor(ColorRGBA::Red);
		light1->setSpecularColor(ColorRGBA::White);
		light1->setRadius(10.0f);

		LightComponent* light2 = m_Light2->getLightComponent();
		light2->setLightType(LightComponent::LT_POINT);
		light2->setDiffuseColor(ColorRGBA::Green);
		light2->setRadius(8.0f);
		light2->setSpecularColor(ColorRGBA::White);

		LightComponent* light3 = m_Light3->getLightComponent();
		light3->setLightType(LightComponent::LT_SPOT);
		light3->setDiffuseColor(ColorRGBA::Blue);
		light3->setRadius(8.0f);
		light3->setDirection(-Vector3::Up);
		light3->setConeAngle(0.0002f);
		light3->setSpecularColor(ColorRGBA::DarkBlue);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Test namespace
