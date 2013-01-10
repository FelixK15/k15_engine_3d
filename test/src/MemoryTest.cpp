#include "UnitTest++.h"
#include "K15_Memory.h"
#include "K15_System.h"

using namespace K15_EngineV2;

SUITE(MemoryTests)
{
	TEST(MemoryNewDeleteCounter)
	{
		int iStartAllocations = Memory::GetAmountOfMissingDeleteStatements();
		char *pTestBuffer = K15_NEW char[200];

		CHECK_EQUAL(iStartAllocations + 1,Memory::GetAmountOfMissingDeleteStatements());

		K15_DELETE[] pTestBuffer;

		CHECK_EQUAL(iStartAllocations,Memory::GetAmountOfMissingDeleteStatements());
	}

	TEST(CustomMemoryFunction)
	{
		unsigned char cDestinationBuffer[500000];
		unsigned char cSourceBuffer[500000];

		double dStart = 0.0;
		double dDifference = 0.0;

		dStart = g_pSystem->TimeSinceStart();
		memcpy(&cDestinationBuffer,&cSourceBuffer,500000);
		dDifference = g_pSystem->TimeSinceStart() - dStart;

		printf("memcpy took %f ms to execute.\n",dDifference);

		dStart = g_pSystem->TimeSinceStart();
		Memory::MemCopy(&cDestinationBuffer,&cSourceBuffer,500000);
		dDifference = g_pSystem->TimeSinceStart() - dStart;

		printf("Memory::MemCopy took %f ms to execute.\n",dDifference);
	}
}