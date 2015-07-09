#include "K15_CustomMemoryAllocator.h"
#include "K15_MemoryPrerequisites.h"

#include <malloc.h>
#include <cassert>

/*********************************************************************************/
static void* K15_InternalMallocWrapper(size_t p_SizeInBytes, void* p_UserData)
{
	return ::malloc(p_SizeInBytes);
}
/*********************************************************************************/
static void K15_InternalFreeWrapper(void* p_Pointer, void* p_UserData)
{
	::free(p_Pointer);
}
/*********************************************************************************/


/*********************************************************************************/
K15_CustomMemoryAllocator* K15_CreateDefaultMemoryAllocator()
{
	return K15_CreateCustomMemoryAllocator(K15_InternalMallocWrapper, K15_InternalFreeWrapper, 0);
}
/*********************************************************************************/
K15_CustomMemoryAllocator* K15_CreateCustomMemoryAllocator(K15_MallocFnc p_CustomMallocFnc, K15_FreeFnc p_CustomFreeFnc, void* p_UserData)
{
	K15_ASSERT(p_CustomMallocFnc, "Custom Malloc is NULL.");
	K15_ASSERT(p_CustomFreeFnc, "Custom Free is NULL.");

	K15_CustomMemoryAllocator* customAllocator = (K15_CustomMemoryAllocator*)p_CustomMallocFnc(sizeof(K15_CustomMemoryAllocator), 0);

	K15_InitializeCustomMemoryAllocator(customAllocator, p_CustomMallocFnc, p_CustomFreeFnc, p_UserData);

	return customAllocator;
}
/*********************************************************************************/
void K15_InitializeCustomMemoryAllocator(K15_CustomMemoryAllocator* p_CustomMemoryAllocator, K15_MallocFnc p_CustomMallocFnc, K15_FreeFnc p_CustomFreeFnc, void* p_UserData)
{
	K15_ASSERT(p_CustomMemoryAllocator, "Custom Memory Allocator is NULL.");
	K15_ASSERT(p_CustomMallocFnc, "Custom Malloc is NULL.");
	K15_ASSERT(p_CustomFreeFnc, "Custom Free is NULL.");

	p_CustomMemoryAllocator->alloc = p_CustomMallocFnc;
	p_CustomMemoryAllocator->free = p_CustomFreeFnc;
	p_CustomMemoryAllocator->userData = p_UserData;
}
/*********************************************************************************/
void* K15_AllocateFromMemoryAllocator(K15_CustomMemoryAllocator* p_MemoryAllocator, size_t p_SizeInBytes)
{
	K15_ASSERT(p_MemoryAllocator, "Custom Memory Allocator is NULL.");
	K15_ASSERT(p_SizeInBytes, "Size for memory allocation is 0.");

	K15_MallocFnc memoryAlloc = p_MemoryAllocator->alloc;
	void* userData = p_MemoryAllocator->userData;

	return memoryAlloc(p_SizeInBytes, userData);
}
/*********************************************************************************/
void K15_FreeFromMemoryAllocator(K15_CustomMemoryAllocator* p_MemoryAllocator, void* p_Pointer)
{
	K15_ASSERT(p_MemoryAllocator, "Custom Memory Allocator is NULL.");
	K15_ASSERT(p_Pointer, "Memory pointer is NULL.");

	K15_FreeFnc memoryFree = p_MemoryAllocator->free;
	void* userData = p_MemoryAllocator->userData;

	memoryFree(p_Pointer, userData);
}
/*********************************************************************************/