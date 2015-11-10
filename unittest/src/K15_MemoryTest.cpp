#include "K15_CustomMemoryAllocator.h"
#include "K15_MemoryBuffer.h"
#include "K15_BlockAllocator.h"

/*********************************************************************************/
bool8 K15_MemoryBlockAllocatorTest()
{
	K15_MemoryBuffer memoryBuffer = K15_CreateMemoryBuffer(1024);
	K15_CustomMemoryAllocator blockAllocator = K15_CreateBlockAllocator(memoryBuffer);

	void* memory = K15_AllocateFromMemoryAllocator(&blockAllocator, 512);
	memset(memory, 0xFF, 512);

	void* memory2 = K15_AllocateFromMemoryAllocator(&blockAllocator, 102);
	memset(memory2, 0xFF, 102);

	void* memory3 = K15_AllocateFromMemoryAllocator(&blockAllocator, 204);
	memset(memory3, 0xFF, 204);
	
	K15_FreeFromMemoryAllocator(&blockAllocator, memory2);
	memset(memory2, 0x00, 102);

	K15_FreeFromMemoryAllocator(&blockAllocator, memory3);
	memset(memory3, 0x00, 204);

	void* memory4 = K15_AllocateFromMemoryAllocator(&blockAllocator, 410);
	memset(memory4, 0xFF, 410);

	bool8 testSuccessful = memory4 != 0;

	K15_FreeFromMemoryAllocator(&blockAllocator, memory);
	K15_FreeFromMemoryAllocator(&blockAllocator, memory4);

	return testSuccessful;
}
/*********************************************************************************/
bool8 K15_MemoryTest()
{
	K15_START_TESTS("Memory Allocator Test");
	K15_TEST_BOOL_FUNCTION(K15_MemoryBlockAllocatorTest);
	K15_END_TESTS;

	return K15_TEST_RESULT;
}
/*********************************************************************************/