#include "K15_MemoryBuffer.h"

/*********************************************************************************/
K15_MemoryBuffer K15_CreateMemoryBuffer(uint32 p_SizeInBytes)
{
	K15_CustomMemoryAllocator defaultAllocator = K15_CreateDefaultMemoryAllocator();
	return K15_CreateMemoryBufferFromCustomAllocator(&defaultAllocator, p_SizeInBytes);
}
/*********************************************************************************/
K15_MemoryBuffer K15_CreateMemoryBuffer(byte* p_MemoryBuffer, uint32 p_SizeInBytes)
{
	K15_MemoryBuffer memoryBuffer = {};

	memoryBuffer.buffer = p_MemoryBuffer;
	memoryBuffer.sizeInBytes = p_SizeInBytes;

	return memoryBuffer;
}
/*********************************************************************************/
K15_MemoryBuffer K15_CreateMemoryBufferFromCustomAllocator(K15_CustomMemoryAllocator* p_Allocator, uint32 p_SizeInBytes)
{
	byte* memory = (byte*)K15_AllocateFromMemoryAllocator(p_Allocator, p_SizeInBytes);

	K15_ASSERT(memory);

	return K15_CreateMemoryBuffer(memory, p_SizeInBytes);
}
/*********************************************************************************/