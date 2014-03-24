#include "K15_Application.h"

#include "K15_LogManager.h"

#include "K15_Language.h"

#if defined K15_OS_ANDROID
#	include <android_native_app_glue.h>
#endif //K15_OS_ANDROID

/*********************************************************************************/
//void testMultiLanguage();
/*********************************************************************************/

/*********************************************************************************/
using namespace K15_Engine::Core;
/*********************************************************************************/
#if defined K15_OS_ANDROID
void android_main(struct android_app* app)
#else
int main(int argc,char** argv)
#endif //K15_OS_ANDROID
{
	K15_Engine::Core::Application* application = 0;

#if defined K15_OS_ANDROID
	typedef K15_Engine::Core::Application* (*ApplicationFunction)();
	void* module = dlopen("Engine.so",RTLD_NOW | RTLD_LOCAL);
	app_dummy();
	K15_ASSERT(module,"Error loading Engine.so");
	ApplicationFunction func = (ApplicationFunction)dlsym(module,"K15Engine_getApplication");
	if(func)
	{
		application = func();
	}
	else
	{
		return;
	}
	
	application->initialize(app);
#else
	application = g_Application;
	application->initialize(argc,argv);
#endif //K15_OS_ANDROID
	//testMultiLanguage();
// 
// 	application->run();
// 	application->shutdown();

#if defined K15_DEBUG && defined K15_OS_WINDOWS
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
#endif //K15_DEBUG

#if defined K15_OS_WINDOWS
	return 0;
#endif
}
/*********************************************************************************/
// void testMultiLanguage()
// {
// 	Language::deserialize();
// 	Language::serialize();
// 
// 	_LogDebug("Language:\"English\":");
// 	_LogDebug("\t%s",C_LANG_STRING(ID_HELLO));
// 	_LogDebug("\t%s",C_LANG_STRING(ID_BYE));
// 	_LogDebug("\t%s",C_LANG_STRING(ID_THANKS));
// 
// 	Language::setCurrentLanguageID(Language::LID_GERMAN);
// 
// 	_LogDebug("Language:\"German\":");
// 	_LogDebug("\t%s",C_LANG_STRING(ID_HELLO));
// 	_LogDebug("\t%s",C_LANG_STRING(ID_BYE));
// 	_LogDebug("\t%s",C_LANG_STRING(ID_THANKS));
// }
/*********************************************************************************/