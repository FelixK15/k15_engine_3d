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

		void addRop(RenderOperation* p_Rop);
		
		virtual void renderSingleFrame() OVERRIDE;

	private:
		RenderOperationArray m_RopArray;
	};
}

#endif //_TD_RenderProcess_h_