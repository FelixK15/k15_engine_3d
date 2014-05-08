#include "TD_RenderProcess.h"

namespace TowerDefense
{
	/*********************************************************************************/
	RenderProcess::RenderProcess()
		: RenderProcessBase(),
		m_RopArray()
	{

	}
	/*********************************************************************************/
	RenderProcess::~RenderProcess()
	{

	}
	/*********************************************************************************/
	void RenderProcess::addRop(RenderOperation* p_Rop)
	{
		m_RopArray.push_back(p_Rop);
	}
	/*********************************************************************************/
	void RenderProcess::renderSingleFrame()
	{
		for(RenderOperationArray::iterator iter = m_RopArray.begin();iter != m_RopArray.end();++iter)
		{
			g_Application->getRenderer()->draw((*iter));
		}

		m_RopArray.clear();
	}
	/*********************************************************************************/
}