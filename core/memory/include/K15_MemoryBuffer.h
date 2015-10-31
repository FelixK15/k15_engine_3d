#ifndef _K15_Runtime_Memory_Buffer_h_
#define _K15_Runtime_Memory_Buffer_h_

#include "K15_MemoryPrerequisites.h"

enum K15_MemoryBufferFlags
{
	K15_RESIZABLE_MEMORY_BUFFER = 0x002
};

struct K15_MemoryBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;

	byte* buffer;
	uint32 sizeInBytes;
	uint32 usedBytesOffset; //todo remove - too much information for memory block
	uint32 flags;
};

void K15_InitializeMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, uint32 p_SizeInBytes, uint32 p_Flags);
void K15_InitializeMemoryBufferWithCustomAllocator(K15_MemoryBuffer* p_MemoryBuffer, K15_CustomMemoryAllocator p_CustomMemoryAllocator, uint32 p_SizeInBytes, uint32 p_Flags);
void K15_InitializePreallocatedMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, byte* p_PreallocatedMemory, uint32 p_SizeInBytes, uint32 p_Flags);
void K15_InitializePreallocatedMemoryBufferWithCustomAllocator(K15_MemoryBuffer* p_MemoryBuffer, byte* p_PreallocatedMemory,  K15_CustomMemoryAllocator p_CustomMemoryAllocator, uint32 p_SizeInBytes, uint32 p_Flags);

byte* K15_GetMemoryFromMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, uint32 p_SizeInBytes);
void K15_ClearMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer);

K15_CustomMemoryAllocator K15_CreateMemoryBufferAllocator(K15_MemoryBuffer* p_MemoryBuffer);

#endif //_K15_Runtime_Memory_Buffer_h_