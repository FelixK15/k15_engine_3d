#include "K15_Application.h"

#include "K15_EngineTestModule.h"
#include "K15_GameStateManager.h"

using namespace K15_Engine::Core;

/*********************************************************************************/
int main(int argc,char** argv)
{
	BaseAllocatedObject::createCoreAllocators();

	Application* application = K15_NEW Application();

	application->initialize(argc,argv);
	
	g_GameStateManager->pushGameState(K15_NEW K15_Engine::Test::TestGameState());

	application->run();
 	application->shutdown();

    return 0;
}
/*********************************************************************************/
