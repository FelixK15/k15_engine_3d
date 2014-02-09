/**
 * @file K15_SampleApplicationModule.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/08
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

#include "K15_SampleApplicationModule.h"

#include "K15_InputManager.h"
#include "K15_RendererBase.h"
#include "K15_RenderTask.h"
#include "K15_RenderSampleProcess.h"

namespace K15_Engine { namespace Plugins { namespace RenderTest {
	/*********************************************************************************/
	RenderTestApplicationModule::RenderTestApplicationModule()
	{

	}
	/*********************************************************************************/
	RenderTestApplicationModule::~RenderTestApplicationModule()
	{

	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onInitialize()
	{
		g_Application->getRenderTask()->setRenderProcess(K15_NEW K15_Engine::Plugins::RenderTest::RenderSampleProcess());
	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onPreTick()
	{
		if(g_InputManager->isActive(_N(Color_1)))
		{
			g_Application->getRenderer()->setClearColor(1.0f,0.0f,0.0f);
		}
		else if(g_InputManager->isActive(_N(Color_2)))
		{
			g_Application->getRenderer()->setClearColor(0.0f,1.0f,0.0f);
		}
		else if(g_InputManager->isActive(_N(Color_3)))
		{
			g_Application->getRenderer()->setClearColor(0.0f,0.0f,1.0f);
		}
		else if(g_InputManager->isActive(_N(Color_4)))
		{
			g_Application->getRenderer()->setClearColor(1.0f,1.0f,0.0f);
		}
		else if(g_InputManager->isActive(_N(Color_5)))
		{
			g_Application->getRenderer()->setClearColor(0.0f,1.0f,1.0f);
		}
	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onPostTick()
	{

	}
	/*********************************************************************************/
	void RenderTestApplicationModule::onShutdown()
	{
		K15_DELETE g_Application->getRenderTask()->getRenderProcess();
		g_Application->getRenderTask()->setRenderProcess(0);
	}
	/*********************************************************************************/
}}}// end of K15_Engine::Plugins::RenderTest namespace