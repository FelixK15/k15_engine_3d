#include "K15_Application.h"

int main(int argc,char** argv)
{
	g_Application->initialize(argc,argv);
	g_Application->run();
	g_Application->shutdown();

	return 0;
}