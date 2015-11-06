#ifndef _K15_Runtime_Memory_Buffer_h_
#define _K15_Runtime_Memory_Buffer_h_

#include "K15_MemoryPrerequisites.h"

struct K15_MemoryBuffer
{
	byte* buffer;
	uint32 sizeInBytes;
};

K15_MemoryBuffer K15_CreateMemoryBuffer(uint32 p_SizeInBytes);
K15_MemoryBuffer K15_CreateMemoryBuffer(byte* p_MemoryBuffer, uint32 p_SizeInBytes);
K15_MemoryBuffer K15_CreateMemoryBufferFromCustomAllocator(K15_CustomMemoryAllocator* p_Allocator, uint32 p_SizeInBytes);

#endif //_K15_Runtime_Memory_Buffer_h_