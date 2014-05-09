#include "TD_GameModule.h"

#include "TD_Level.h"
#include "TD_RenderProcess.h"

#include "K15_RenderTask.h"
#include "K15_TextureSampler.h"
#include "K15_Application.h"
#include "K15_RendererBase.h"
#include "K15_GameObject.h"
#include "K15_Node.h"
#include "K15_CameraComponent.h"
#include "K15_ResourceManager.h"
#include "K15_ResourceArchiveZip.h"
#include "K15_ResourceImporterObj.h"
#include "K15_ResourceImporterTiff.h"
#include "K15_ResourceImporterMaterial.h"
#include "K15_Keyboard.h"

namespace TowerDefense
{
	/*********************************************************************************/
	GameModule::GameModule()
		: ApplicationModule(),
		m_Camera(0),
		m_Level(0),
		m_RenderProcess(0)
	{

	}
	/*********************************************************************************/
	GameModule::~GameModule()
	{
		K15_DELETE m_Camera;
		K15_DELETE m_RenderProcess;
	}
	/*********************************************************************************/
	void GameModule::onInitialize()
	{

	}
	/*********************************************************************************/
	void GameModule::_onRendererInitialized()
	{
		g_ResourceManager->addResourceArchive(K15_NEW ResourceArchiveZip(g_Application->getGameRootDir() + "resources.zip"));
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterTiff());
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterObj());
		g_ResourceManager->addResourceImporter(K15_NEW ResourceImporterMaterial());

		m_Camera = K15_NEW GameObject("Camera");
		m_Camera->addComponent(K15_NEW CameraComponent());
		m_RenderProcess = K15_NEW RenderProcess();
		m_Level = K15_NEW Level("level0");
		m_Camera->getNode().translate(0.0f,5.0f,0.0f);
		m_Camera->getNode().rotate(Vector3(1.0f,0.0f,0.0f),-glm::half_pi<float>() * 0.5f);
		g_Application->getRenderer()->setActiveCameraGameObject(m_Camera);

		TextureSampler* sampler = K15_NEW TextureSampler();
		g_Application->getRenderer()->bindTextureSampler(sampler,Texture::TS_SLOT1);
		g_Application->getRenderTask()->setRenderProcess(m_RenderProcess);
	}
	/*********************************************************************************/
	void GameModule::onPreTick()
	{
		if(m_RendererInitialized)
		{
			static bool forward = false;
			static int counter = 100;
			if(forward)
			{
				forward = ++counter >= 100;
				m_Camera->getNode().translate(0.0f,0.0f,0.05f);
			}
			else
			{
				forward = --counter <= 100;
				m_Camera->getNode().translate(0.0f,0.0f,-0.05f);
			}

			if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_W))
			{
				m_Camera->getNode().rotate(Vector3(1.0f,0.0f,0.0f),0.02f);
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_S))
			{
				m_Camera->getNode().rotate(Vector3(1.0f,0.0f,0.0f),-0.02f);
			}

			if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_A))
			{
				m_Camera->getNode().rotate(Vector3(0.0f,1.0f,0.0f),0.02f);
			}
			else if(InputDevices::Keyboard::isPressed(InputDevices::Keyboard::KEY_D))
			{
				m_Camera->getNode().rotate(Vector3(0.0f,1.0f,0.0f),-0.02f);
			}
// 			m_Camera->getNode()->rotate(Vector3(1.0f,0.0f,0.0f),-(glm::pi<float>() * 2) * 0.4f);
// 			m_Camera->getNode()->rotate(Vector3(0.0f,1.0f,0.0f),(glm::pi<float>() * 2) * 0.01f);
 			m_Level->draw(m_RenderProcess);
		}
	}
	/*********************************************************************************/
}

