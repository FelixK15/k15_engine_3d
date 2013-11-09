/**
 * @file K15_RenderTask.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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
#include "K15_RenderTask.h"
#include "K15_RenderProcessBase.h"
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	const uint32 RenderTask::TaskPriority = 0;
	K15_IMPLEMENT_RTTI(Core,RenderTask);
	/*********************************************************************************/

	/*********************************************************************************/
	RenderTask::RenderTask()
		: TaskBase(TaskPriority),
		  m_RenderProcess(0)
	{

	}
	/*********************************************************************************/
	RenderTask::~RenderTask()
	{
		K15_DELETE m_RenderProcess;
	}
	/*********************************************************************************/
	void RenderTask::setRenderer(Rendering::RendererBase* p_Renderer)
	{
		m_Renderer = p_Renderer;
	}
	/*********************************************************************************/
	void RenderTask::setRenderProcess(RenderProcessBase* p_RenderProcess)
	{
		m_RenderProcess = p_RenderProcess;
	}
	/*********************************************************************************/
	RenderProcessBase* RenderTask::getRenderProcess() const
	{
		return m_RenderProcess;
	}
	/*********************************************************************************/
	Rendering::RendererBase* RenderTask::getRenderer() const
	{
		return m_Renderer;
	}
	/*********************************************************************************/
	void RenderTask::update(const GameTime& p_GameTime)
	{
		m_Renderer->beginFrame();
		if(m_RenderProcess)
		{
			m_RenderProcess->renderSingleFrame();
		}
		m_Renderer->endFrame();
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace