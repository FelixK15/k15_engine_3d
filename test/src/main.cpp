#include "K15_Application.h"

using namespace K15_Engine::Core;

/*********************************************************************************/
int main(int argc,char** argv)
{
	BaseAllocatedObject::createCoreAllocators();

	Application* application = K15_NEW Application();

	application->initialize(argc,argv);
	application->run();
 	application->shutdown();

    return 0;
}
/*********************************************************************************/
