#include "K15_Application.h"

#if defined K15_OS_ANDROID
	#include "TD_GameModule.h"
	#include "GLES2\K15_GLES2_Renderer.h"
#endif //K15_OS_ANDROID

using namespace K15_Engine::Core;

#if defined K15_OS_ANDROID
using namespace TowerDefense;
void android_main(struct android_app* app)
#else
int main(int argc,char** argv)
#endif //K15_OS_ANDROID
{
	BaseAllocatedObject::createCoreAllocators();

	Application* application = K15_NEW Application();
#if defined K15_OS_ANDROID
	app_dummy();
	application->initialize(app);
	application->addApplicationModule(K15_NEW GameModule());
	application->setRenderer(K15_NEW GLES2::Renderer());
#else
	application->initialize(argc,argv);
#endif //K15_OS_ANDROID

	application->run();
	application->shutdown();

#if defined K15_DEBUG && defined K15_OS_WINDOWS
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
#endif //K15_DEBUG

#if defined K15_OS_WINDOWS
	return 0;
#endif
}