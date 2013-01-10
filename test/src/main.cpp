#include "UnitTest++.h"
#include "K15_System.h"
#include "K15_Thread.h"

#include <iostream>

using namespace K15_EngineV2;

int main()
{
	g_pSystem->Initialize(System::SF_ENABLE_MEMORY_LOGGING);
	g_pSystem->InitializeWindow("Test Window",System::WF_AUTOALIGNMENT,30,30,500,500);
	int iReturn = UnitTest::RunAllTests();
	g_pSystem->Shutdown();
	std::system("pause");

	return iReturn;
}