#include "K15_MemoryBuffer.h"
#include <cstring>
#include <malloc.h>
#include <cassert>

/*********************************************************************************/
static void* K15_InternalMemoryBufferAllocation(size_t p_SizeInBytes, void* p_MemoryBuffer)
{
	K15_ASSERT_TEXT_SIMPLE(p_MemoryBuffer, "Memory Buffer is NULL.");

	K15_MemoryBuffer* memoryBuffer = (K15_MemoryBuffer*)p_MemoryBuffer;

	return K15_GetMemoryFromMemoryBuffer(memoryBuffer, (uint32)p_SizeInBytes);
}
/*********************************************************************************/
static void K15_InternalMemoryBufferFree(void* p_Pointer, void* p_MemoryBuffer)
{
}
/*********************************************************************************/



/*********************************************************************************/
void K15_InitializeMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, uint32 p_SizeInBytes, uint32 p_Flags)
{
	K15_ASSERT_TEXT_SIMPLE(p_SizeInBytes > 0, "Memory Buffer size is 0.");

	K15_InitializePreallocatedMemoryBufferWithCustomAllocator(p_MemoryBuffer, (byte*)malloc(p_SizeInBytes), K15_CreateDefaultMemoryAllocator(), p_SizeInBytes, p_Flags);
}
/*********************************************************************************/
void K15_InitializeMemoryBufferWithCustomAllocator(K15_MemoryBuffer* p_MemoryBuffer, K15_CustomMemoryAllocator p_CustomMemoryAllocator, uint32 p_SizeInBytes, uint32 p_Flags)
{
	K15_ASSERT_TEXT_SIMPLE(p_SizeInBytes > 0, "Memory Buffer size is 0.");

	byte* preallocatedBuffer = (byte*)K15_AllocateFromMemoryAllocator(&p_CustomMemoryAllocator, p_SizeInBytes);

	K15_InitializePreallocatedMemoryBufferWithCustomAllocator(p_MemoryBuffer, preallocatedBuffer, p_CustomMemoryAllocator, p_SizeInBytes, p_Flags);
}
/*********************************************************************************/
void K15_InitializePreallocatedMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, byte* p_PreallocatedMemory, uint32 p_SizeInBytes, uint32 p_Flags)
{
	K15_ASSERT_TEXT_SIMPLE(p_SizeInBytes > 0, "Memory Buffer size is 0.");
	K15_ASSERT_TEXT_SIMPLE(p_PreallocatedMemory, "Preallocated memmory is NULL.");

	K15_InitializePreallocatedMemoryBufferWithCustomAllocator(p_MemoryBuffer, p_PreallocatedMemory, K15_CreateDefaultMemoryAllocator(), p_SizeInBytes, p_Flags);
}
/*********************************************************************************/
void K15_InitializePreallocatedMemoryBufferWithCustomAllocator(K15_MemoryBuffer* p_MemoryBuffer, byte* p_PreallocatedMemory, K15_CustomMemoryAllocator p_CustomMemoryAllocator, uint32 p_SizeInBytes, uint32 p_Flags)
{
	K15_ASSERT_TEXT_SIMPLE(p_MemoryBuffer, "Memory Buffer is NULL.");
	K15_ASSERT_TEXT_SIMPLE(p_SizeInBytes > 0, "Memory Buffer size is 0.");
	K15_ASSERT_TEXT_SIMPLE(p_PreallocatedMemory, "Preallocated memmory is NULL.");

	p_MemoryBuffer->buffer = p_PreallocatedMemory;
	p_MemoryBuffer->sizeInBytes = p_SizeInBytes;
	p_MemoryBuffer->usedBytesOffset = 0;
	p_MemoryBuffer->flags = p_Flags;
	p_MemoryBuffer->memoryAllocator = p_CustomMemoryAllocator;
}
/*********************************************************************************/
byte* K15_GetMemoryFromMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, uint32 p_SizeInBytes)
{
	K15_ASSERT_TEXT_SIMPLE(p_MemoryBuffer, "Memory Buffer is NULL.");
	K15_ASSERT_TEXT_SIMPLE(p_SizeInBytes > 0, "Memory Buffer size is 0.");

	uint32 memoryBufferSize = p_MemoryBuffer->sizeInBytes;
	uint32 newOffset = p_SizeInBytes + p_MemoryBuffer->usedBytesOffset;
	uint32 flags = p_MemoryBuffer->flags;
	byte* memory = p_MemoryBuffer->buffer;

	if (newOffset > memoryBufferSize)
	{
		if ((flags & K15_RESIZABLE_MEMORY_BUFFER) > 0)
		{
			K15_CustomMemoryAllocator* memoryAllocator = &p_MemoryBuffer->memoryAllocator;

			uint32 newMemorySize = memoryBufferSize + p_SizeInBytes;

			byte* newMemory = (byte*)K15_AllocateFromMemoryAllocator(memoryAllocator, newMemorySize); 
			byte* oldMemory = p_MemoryBuffer->buffer;

			memcpy(newMemory, oldMemory, memoryBufferSize);

			K15_FreeFromMemoryAllocator(memoryAllocator, oldMemory);

			p_MemoryBuffer->buffer = newMemory;
			memory = newMemory;
		}
		else
		{
			K15_ASSERT_TEXT_SIMPLE(false, "Insufficient memory buffer size.");
		}
	}
	
	byte* requestedMemory = memory + p_MemoryBuffer->usedBytesOffset;

	p_MemoryBuffer->usedBytesOffset = newOffset;

	return requestedMemory;
}
/*********************************************************************************/
void K15_DeleteMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer)
{
	K15_ASSERT_TEXT_SIMPLE(p_MemoryBuffer, "Memory Buffer is NULL.");
	K15_CustomMemoryAllocator* memoryAllocator = &p_MemoryBuffer->memoryAllocator;

	K15_FreeFromMemoryAllocator(memoryAllocator, p_MemoryBuffer->buffer);
}
/*********************************************************************************/
K15_CustomMemoryAllocator K15_CreateMemoryBufferAllocator(K15_MemoryBuffer* p_MemoryBuffer)
{
	K15_ASSERT_TEXT_SIMPLE(p_MemoryBuffer, "Memory buffer is NULL.");

	K15_CustomMemoryAllocator customAllocator = {};

	K15_InitializeCustomMemoryAllocator(&customAllocator, K15_InternalMemoryBufferAllocation, K15_InternalMemoryBufferFree, p_MemoryBuffer);

	return customAllocator;
}
/*********************************************************************************/