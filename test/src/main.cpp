#include "K15_Application.h"

#if defined K15_OS_ANDROID
	#include "K15_SampleApplicationModule.h"
	#include "GLES2\K15_GLES2_Renderer.h"
#endif //K15_OS_ANDROID

/*********************************************************************************/
#if defined K15_OS_ANDROID
void android_main(struct android_app* app)
#else
int main(int argc,char** argv)
#endif //K15_OS_ANDROID
{
	K15_Engine::Core::Application* application = K15_NEW K15_Engine::Core::Application();
#if defined K15_OS_ANDROID
	app_dummy();
	application->addApplicationModule(K15_NEW K15_Engine::Plugins::RenderTest::RenderTestApplicationModule());
	application->initialize(app);
	application->setRenderer(K15_NEW K15_Engine::Rendering::GLES2::Renderer());
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
/*********************************************************************************/