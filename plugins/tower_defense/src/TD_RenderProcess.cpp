#include "TD_RenderProcess.h"
#include "K15_RenderQueue.h"

namespace TowerDefense
{
	/*********************************************************************************/
	RenderProcess::RenderProcess()
		: RenderProcessBase(),
		m_RenderQueue(0)
	{

	}
	/*********************************************************************************/
	RenderProcess::~RenderProcess()
	{

	}
	/*********************************************************************************/
	void RenderProcess::renderSingleFrame()
	{
		m_RenderQueue->sort();

		for(uint32 i = 0;i < m_RenderQueue->size();++i)
		{
			g_Application->getRenderer()->draw(m_RenderQueue->getRenderOperation(i));
		}

		g_Application->getRenderer()->drawText("This is a sample Text!","fonts/arial.font",0.3f,0.3f);
	}
	/*********************************************************************************/
	void RenderProcess::setRenderQueue(RenderQueue* p_RenderQueue)
	{
		m_RenderQueue = p_RenderQueue;
	}
	/*********************************************************************************/
}