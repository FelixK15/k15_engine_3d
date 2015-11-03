#include "K15_StackAllocator.h"

/*********************************************************************************/
struct K15_StackAllocatorInfo
{
	K15_MemoryBuffer* memoryBuffer;
	uint32 sizeInBytes;
};
/*********************************************************************************/



/*********************************************************************************/
intern void* K15_InternalStackAlloc(size_t p_SizeInBytes, void* p_UserData)
{
	K15_StackAllocatorInfo* allocatorInfo = (K15_StackAllocatorInfo*)p_UserData;
	K15_MemoryBuffer* allocatorBuffer = allocatorInfo->memoryBuffer;

	uint32 newSizeInBytes = allocatorInfo->sizeInBytes + (uint32)p_SizeInBytes;
	uint32 allocatorMemorySizeInBytes = allocatorBuffer->sizeInBytes;
	
	if (newSizeInBytes >= allocatorMemorySizeInBytes)
	{
		//out of memory
		return 0;
	}

	void* memory =  allocatorBuffer->buffer + allocatorInfo->sizeInBytes;

	allocatorInfo->sizeInBytes = newSizeInBytes;

	return memory;
}
/*********************************************************************************/
intern void K15_InternalStackFree(void* p_Pointer, void* p_UserData)
{

}
/*********************************************************************************/
intern void K15_InternalStackClear(void* p_UserData)
{
	K15_StackAllocatorInfo* allocatorInfo = (K15_StackAllocatorInfo*)p_UserData;
	allocatorInfo->sizeInBytes = 0;
}
/*********************************************************************************/
K15_CustomMemoryAllocator K15_CreateStackAllocator(K15_MemoryBuffer* p_MemoryBuffer, char* p_AllocatorName)
{
	K15_StackAllocatorInfo* allocatorInfo = (K15_StackAllocatorInfo*)K15_GetMemoryFromMemoryBuffer(p_MemoryBuffer, sizeof(K15_StackAllocatorInfo));

	allocatorInfo->memoryBuffer = p_MemoryBuffer;
	allocatorInfo->sizeInBytes = 0;

	return K15_CreateCustomMemoryAllocator(K15_InternalStackAlloc, K15_InternalStackFree, K15_InternalStackClear, allocatorInfo, p_AllocatorName);
}
/*********************************************************************************/
