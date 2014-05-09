#ifndef _TD_RenderProcess_h_
#define _TD_RenderProcess_h_

#include "TD_Prerequisities.h"
#include "K15_RenderProcessBase.h"

namespace TowerDefense
{
	class RenderProcess : public RenderProcessBase
	{
	public:
		typedef DynamicArray(RenderOperation*) RenderOperationArray;

	public:
		RenderProcess();
		~RenderProcess();

		void setRenderQueue(RenderQueue* p_RenderQueue);
		
		virtual void renderSingleFrame() OVERRIDE;

	private:
		RenderQueue* m_RenderQueue;
	};
}

#endif //_TD_RenderProcess_h_