#ifndef _K15_Runtime_Memory_Buffer_h_
#define _K15_Runtime_Memory_Buffer_h_

#include "K15_Prerequisites_RT.h"

typedef void* (*K15_MemoryBufferAllocFnc)(size_t);
typedef void (*K15_MemoryBufferFreeFnc)(void*);

enum K15_MemoryBufferFlags
{
	K15_RESIZABLE_MEMORY_BUFFER = 0x002
};

struct K15_MemoryBuffer
{
	K15_MemoryBufferAllocFnc memAlloc;
	K15_MemoryBufferFreeFnc memFree;

	byte* buffer;
	uint32 sizeInBytes;
	uint32 usedBytesOffset;
	uint32 flags;
};

void K15_InitializeDefaultMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, uint32 p_SizeInBytes, uint32 p_Flags);
void K15_InitializeCustomMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, K15_MemoryBufferAllocFnc p_MemoryAllocFunction, K15_MemoryBufferFreeFnc p_MemoryFreeFunction, uint32 p_SizeInBytes, uint32 p_Flags);
void K15_InitializeDefaultPreallocatedMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, byte* p_PreallocatedMemory, uint32 p_SizeInBytes, uint32 p_Flags);
void K15_InitializeCustomPreallocatedMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, byte* p_PreallocatedMemory,  K15_MemoryBufferAllocFnc p_MemoryAllocFunction, K15_MemoryBufferFreeFnc p_MemoryFreeFunction, uint32 p_SizeInBytes, uint32 p_Flags);

byte* K15_GetMemoryFromMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer, uint32 p_SizeInBytes);

void K15_ClearMemoryBuffer(K15_MemoryBuffer* p_MemoryBuffer);

#endif //_K15_Runtime_Memory_Buffer_h_