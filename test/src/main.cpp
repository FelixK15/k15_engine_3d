#include "K15_Application.h"

#include "K15_LogManager.h"

#include "K15_Language.h"

/*********************************************************************************/
void testMultiLanguage();
/*********************************************************************************/

/*********************************************************************************/
using namespace K15_Engine::Core;
/*********************************************************************************/
int main(int argc,char** argv)
{
 	g_Application->initialize(argc,argv);

	testMultiLanguage();

	g_Application->run();
	g_Application->shutdown();

#if defined K15_DEBUG
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
#endif //K15_DEBUG

	return 0;
}
/*********************************************************************************/
void testMultiLanguage()
{
	Language::deserialize();
	Language::serialize();

	_LogDebug("Language:\"English\":\n");
	_LogDebug("\t%s\n",LANG_STRING(ID_HELLO).c_str());
	_LogDebug("\t%s\n",LANG_STRING(ID_BYE).c_str());
	_LogDebug("\t%s\n",LANG_STRING(ID_THANKS).c_str());

	Language::setCurrentLanguageID(Language::LID_GERMAN);

	_LogDebug("Language:\"German\":\n");
	_LogDebug("\t%s\n",LANG_STRING(ID_HELLO).c_str());
	_LogDebug("\t%s\n",LANG_STRING(ID_BYE).c_str());
	_LogDebug("\t%s\n",LANG_STRING(ID_THANKS).c_str());
}
/*********************************************************************************/