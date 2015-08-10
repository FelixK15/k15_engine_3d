/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_AsyncOperation_Memory_Pool_h_
#define _K15_AsyncOperation_Memory_Pool_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_AsyncOperation;

struct K15_AsyncOperationMemoryPool 
{
	K15_CustomMemoryAllocator memoryAllocator;
	byte* elements;
	unsigned int numElements;
	unsigned int lastElementIndex;
};

void K15_InitializeAsyncOperationMemoryPool(K15_AsyncOperationMemoryPool* p_MemoryPool, unsigned int p_NumElements);
void K15_InitializeAsyncOperationMemoryPoolWithCustomAllocator(K15_AsyncOperationMemoryPool* p_MemoryPool, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_NumElements);

K15_AsyncOperation* K15_GetAsyncOperationMemoryPoolElement(K15_AsyncOperationMemoryPool* p_MemoryPool);

void K15_FreeAsyncOperationMemoryPoolElement(K15_AsyncOperationMemoryPool* p_MemoryPool, K15_AsyncOperation* p_Element);

void K15_ClearAsyncOperationMemoryPool(K15_AsyncOperationMemoryPool* p_MemoryPool);

#endif _K15_AsyncOperation_Memory_Pool_h_