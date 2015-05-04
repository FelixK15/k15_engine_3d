/**
 * @file K15_EngineTestRenderProcess.cpp
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

#include "K15_EngineTestRenderProcess.h"

#include "K15_SceneGraph.h"
#include "K15_RenderQueue.h"

#include "K15_LogManager.h"
#include "K15_DebugRenderer.h"

namespace K15_Engine { namespace Test {
	/*********************************************************************************/
	RenderProcess::RenderProcess()
		: RenderProcessBase(),
		m_RenderQueue(K15_NEW RenderQueue())
	{
		
	}
	/*********************************************************************************/
	RenderProcess::~RenderProcess()
	{

	}
	/*********************************************************************************/
	void RenderProcess::renderSingleFrame()
	{
		SceneGraph* sceneGraph = SceneGraph::getDefault();
		sceneGraph->traverse(m_RenderQueue);

		getRenderer()->setLightArray(sceneGraph->getVisibleLights());
		getRenderer()->draw(m_RenderQueue);

		if(g_DebugRenderer->isActive())
		{
			RenderQueue* debugRenderQueue = g_DebugRenderer->getRenderQueue();
			getRenderer()->draw(debugRenderQueue);
			g_DebugRenderer->clearRenderQueue();
		}

		m_RenderQueue->clear();
	}
	/*********************************************************************************/
}} // end of K15_Engine::Test namespace