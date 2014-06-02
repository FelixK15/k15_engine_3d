#include "TD_GameModule.h"

#include "TD_Level.h"
#include "TD_RenderProcess.h"
#include "TD_MainGameState.h"

#include "K15_Application.h"
#include "K15_RendererBase.h"
#include "K15_RenderTask.h"

namespace TowerDefense
{
	/*********************************************************************************/
	GameModule::GameModule()
		: ApplicationModule(),
		m_RenderProcess(0)
	{

	}
	/*********************************************************************************/
	GameModule::~GameModule()
	{
		
	}
	/*********************************************************************************/
	void GameModule::onInitialize()
	{
		
	}
	/*********************************************************************************/
	void GameModule::onShutdown()
	{
		g_Application->getRenderTask()->setRenderProcess(0);
		K15_DELETE m_RenderProcess;
		m_RenderProcess = 0;
	}
	/*********************************************************************************/
	void GameModule::_onRendererInitialized(const RendererBase* p_Renderer)
	{
		//MainGameState* state = K15_NEW MainGameState();
		static MainGameState state;
		m_RenderProcess = K15_NEW RenderProcess();
		g_Application->getRenderTask()->setRenderProcess(m_RenderProcess);
		g_Application->setGameState(&state);
	}
	/*********************************************************************************/
}

