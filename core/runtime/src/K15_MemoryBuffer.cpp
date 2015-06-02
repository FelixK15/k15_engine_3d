#include "K15_MemoryBuffer.h"
#include "K15_DefaultCLibraries.h"

/*********************************************************************************/
void K15_InitializeDefaultMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, uint32 p_SizeInBytes, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_SizeInBytes > 0, "Memory Buffer size is 0.");

	K15_InitializeCustomPreallocatedMemoryBuffer(p_MemoryBuffer, (byte*)malloc(p_SizeInBytes), malloc, free, p_SizeInBytes, p_Flags);
}
/*********************************************************************************/
void K15_InitializeCustomMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, K15_MemoryBufferAllocFnc p_MemoryAllocFunction, K15_MemoryBufferFreeFnc p_MemoryFreeFunction, uint32 p_SizeInBytes, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_MemoryAllocFunction, "MemAlloc Function is NULL.");
	K15_ASSERT_TEXT(p_SizeInBytes > 0, "Memory Buffer size is 0.");

	K15_InitializeCustomPreallocatedMemoryBuffer(p_MemoryBuffer, (byte*)p_MemoryAllocFunction(p_SizeInBytes), p_MemoryAllocFunction, p_MemoryFreeFunction, p_SizeInBytes, p_Flags);
}
/*********************************************************************************/
void K15_InitializeDefaultPreallocatedMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, byte* p_PreallocatedMemory, uint32 p_SizeInBytes, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_SizeInBytes > 0, "Memory Buffer size is 0.");
	K15_ASSERT_TEXT(p_PreallocatedMemory, "Preallocated memmory is NULL.");

	K15_InitializeCustomPreallocatedMemoryBuffer(p_MemoryBuffer, p_PreallocatedMemory, malloc, free, p_SizeInBytes, p_Flags);
}
/*********************************************************************************/
void K15_InitializeCustomPreallocatedMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, byte* p_PreallocatedMemory, K15_MemoryBufferAllocFnc p_MemoryAllocFunction, K15_MemoryBufferFreeFnc p_MemoryFreeFunction, uint32 p_SizeInBytes, uint32 p_Flags)
{
	K15_ASSERT_TEXT(p_MemoryBuffer, "Memory Buffer is NULL.");
	K15_ASSERT_TEXT(p_SizeInBytes > 0, "Memory Buffer size is 0.");
	K15_ASSERT_TEXT(p_PreallocatedMemory, "Preallocated memmory is NULL.");
	K15_ASSERT_TEXT(p_MemoryAllocFunction, "MemAlloc Function is NULL.");
	K15_ASSERT_TEXT(p_MemoryFreeFunction, "MemAlloc Function is NULL.");

	p_MemoryBuffer->buffer = p_PreallocatedMemory;
	p_MemoryBuffer->sizeInBytes = p_SizeInBytes;
	p_MemoryBuffer->usedBytesOffset = 0;
	p_MemoryBuffer->flags = p_Flags;
	p_MemoryBuffer->memAlloc = p_MemoryAllocFunction;
	p_MemoryBuffer->memFree = p_MemoryFreeFunction;
}
/*********************************************************************************/
byte* K15_GetMemoryFromMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, uint32 p_SizeInBytes)
{
	K15_ASSERT_TEXT(p_MemoryBuffer, "Memory Buffer is NULL.");
	K15_ASSERT_TEXT(p_SizeInBytes > 0, "Memory Buffer size is 0.");

	uint32 memoryBufferSize = p_MemoryBuffer->sizeInBytes;
	uint32 newOffset = p_SizeInBytes + p_MemoryBuffer->usedBytesOffset;
	uint32 flags = p_MemoryBuffer->flags;
	byte* memory = p_MemoryBuffer->buffer;

	if (newOffset > memoryBufferSize)
	{
		if ((flags & K15_RESIZABLE_MEMORY_BUFFER) > 0)
		{
			K15_MemoryBufferAllocFnc memAlloc = p_MemoryBuffer->memAlloc;
			K15_MemoryBufferFreeFnc memFree = p_MemoryBuffer->memFree;

			uint32 newMemorySize = memoryBufferSize + p_SizeInBytes;

			byte* newMemory = (byte*)memAlloc(newMemorySize); 
			byte* oldMemory = p_MemoryBuffer->buffer;

			memcpy(newMemory, oldMemory, memoryBufferSize);

			memFree(oldMemory);

			p_MemoryBuffer->buffer = newMemory;
			memory = newMemory;
		}
		else
		{
			K15_ASSERT_TEXT(false, "Insufficient memory buffer size.");
		}
	}
	
	byte* requestedMemory = memory + p_MemoryBuffer->usedBytesOffset;

	p_MemoryBuffer->usedBytesOffset = newOffset;

	return requestedMemory;
}
/*********************************************************************************/
void K15_DeleteMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer)
{
	K15_ASSERT_TEXT(p_MemoryBuffer, "Memory Buffer is NULL.");
	K15_MemoryBufferFreeFnc memFree = p_MemoryBuffer->memFree;

	memFree(p_MemoryBuffer->buffer);
}
/*********************************************************************************/