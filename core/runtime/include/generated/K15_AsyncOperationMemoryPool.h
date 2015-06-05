/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_AsyncOperation_Memory_Pool_h_
#define _K15_AsyncOperation_Memory_Pool_h_

//forward declaration
struct K15_AsyncOperation;

typedef void* (*K15_AsyncOperationMemoryPoolAllocFnc)(size_t);
typedef void (*K15_AsyncOperationMemoryPoolFreeFnc)(void*);

struct K15_AsyncOperationMemoryPool //256 bit / 32 byte
{
	K15_AsyncOperationMemoryPoolAllocFnc customMalloc;	//64bit
	K15_AsyncOperationMemoryPoolFreeFnc  customFree;	//64bit
	byte* elements;									//64bit
	unsigned int numElements;						//32bit
	unsigned int lastElementIndex;					//32bit
};

void K15_InitializeAsyncOperationMemoryPool(K15_AsyncOperationMemoryPool* p_MemoryPool, unsigned int p_NumElements);
void K15_InitializeAsyncOperationMemoryPoolWithCustomAllocator(K15_AsyncOperationMemoryPool* p_MemoryPool, K15_AsyncOperationMemoryPoolAllocFnc p_CustomMalloc, K15_AsyncOperationMemoryPoolFreeFnc p_CustomFree, unsigned int p_NumElements);

K15_AsyncOperation* K15_GetAsyncOperationMemoryPoolElement(K15_AsyncOperationMemoryPool* p_MemoryPool);

void K15_FreeAsyncOperationMemoryPoolElement(K15_AsyncOperationMemoryPool* p_MemoryPool, K15_AsyncOperation* p_Element);

void K15_ClearAsyncOperationMemoryPool(K15_AsyncOperationMemoryPool* p_MemoryPool);

#endif _K15_AsyncOperation_Memory_Pool_h_