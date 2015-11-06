#ifndef _K15_Memory_BlockAllocator_h_
#define _K15_Memory_BlockAllocator_h_

#include "K15_MemoryPrerequisites.h"

K15_CustomMemoryAllocator K15_CreateBlockAllocator(K15_MemoryBuffer p_MemoryBuffer, char* p_AllocatorName = 0);

#endif //_K15_Memory_BlockAllocator_h_
