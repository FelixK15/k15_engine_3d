#include "K15_Application.h"

int main(int argc,char** argv)
{
 	g_Application->initialize(argc,argv);
 	g_Application->run();
	g_Application->shutdown();

#if defined K15_DEBUG
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
#endif //K15_DEBUG

	return 0;
}