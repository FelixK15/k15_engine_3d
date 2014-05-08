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
		//virtual void onShutdown() OVERRIDE;
		virtual void onPreTick() OVERRIDE;

	protected:
		virtual void _onRendererInitialized() OVERRIDE;

	private:
		RenderProcess* m_RenderProcess;
		GameObject* m_Camera;
		Level* m_Level;
	}; //end of GameModule class declaration
} //End of TowerDefense namespace

#endif //_TD_ApplicationModule_h_