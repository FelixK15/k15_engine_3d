#include "K15_Application.h"
#include "K15_Keyboard.h"
#include "K15_InputManager.h"
#include "K15_HashedString.h"

int main(int argc,char** argv)
{
	g_Application->initialize(argc,argv);
	g_Application->run();
	g_Application->shutdown();

	return 0;
}