#ifndef _K15_Memory_CustomMemoryAllocator_h_
#define _K15_Memory_CustomMemoryAllocator_h_

#include "K15_MemoryPrerequisites.h"

/*********************************************************************************/
struct K15_CustomMemoryAllocator
{
	K15_MallocFnc alloc;
	K15_FreeFnc free;
	void* userData;
};
/*********************************************************************************/

K15_CustomMemoryAllocator* K15_CreateDefaultMemoryAllocator();
K15_CustomMemoryAllocator* K15_CreateCustomMemoryAllocator(K15_MallocFnc p_CustomMallocFnc, K15_FreeFnc p_CustomFreeFnc, void* p_UserData);
void K15_InitializeCustomMemoryAllocator(K15_CustomMemoryAllocator* p_CustomMemoryAllocator, K15_MallocFnc p_CustomMallocFnc, K15_FreeFnc p_CustomFreeFnc, void* p_UserData);

void* K15_AllocateFromMemoryAllocator(K15_CustomMemoryAllocator* p_MemoryAllocator, size_t p_SizeInBytes);
void K15_FreeFromMemoryAllocator(K15_CustomMemoryAllocator* p_MemoryAllocator, void* p_Pointer);

#endif //_K15_Memory_CustomMemoryAllocator_h_