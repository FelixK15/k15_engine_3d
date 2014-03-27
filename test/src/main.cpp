#include "K15_Application.h"

#if defined K15_OS_ANDROID
#	include "K15_LogManager.h"
#	include "GLES2\K15_GLES2_Renderer.h"
#endif //K15_OS_ANDROID

/*********************************************************************************/
#if defined K15_OS_ANDROID
void android_main(struct android_app* app)
#else
int main(int argc,char** argv)
#endif //K15_OS_ANDROID
{
#if defined K15_OS_ANDROID
	app_dummy();
	g_Application->initialize(app);
	GLES2::Renderer* renderer = K15_NEW GLES2::Renderer();
	if(!renderer)
	{
		_LogError("Could not instatiate GLES2 renderer.");
	}
	//g_Application->setRenderer(K15_NEW GLES2::Renderer());
	//g_Application->getRenderer()->initialize();
#else
	g_Application->initialize(argc,argv);
#endif //K15_OS_ANDROID

 	g_Application->run();
 	g_Application->shutdown();

#if defined K15_DEBUG && defined K15_OS_WINDOWS
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
#endif //K15_DEBUG

#if defined K15_OS_WINDOWS
	return 0;
#endif
}
/*********************************************************************************/