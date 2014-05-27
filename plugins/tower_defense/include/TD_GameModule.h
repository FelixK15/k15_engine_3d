#ifndef _TD_ApplicationModule_h_
#define _TD_ApplicationModule_h_

#include "TD_Prerequisities.h"

#include "K15_ApplicationModule.h"

namespace TowerDefense
{
	class GameModule : public ApplicationModule
	{
	public:
		GameModule();
		~GameModule();

		virtual void onInitialize() OVERRIDE;
		virtual void onShutdown() OVERRIDE;

	protected:
		virtual void _onRendererInitialized(const RendererBase* p_Renderer) OVERRIDE;

	private:
		RenderProcess* m_RenderProcess;
	}; //end of GameModule class declaration
} //End of TowerDefense namespace

#endif //_TD_ApplicationModule_h_