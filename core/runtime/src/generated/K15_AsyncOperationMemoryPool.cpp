/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#include "generated/K15_AsyncOperationMemoryPool.h"

/*********************************************************************************/
struct K15_AsyncOperationMemoryPoolElement	
{
	K15_AsyncOperation element;
	uint32 inUse;
};
/*********************************************************************************/


/*********************************************************************************/
void K15_InitializeAsyncOperationMemoryPool(K15_AsyncOperationMemoryPool* p_MemoryPool, unsigned int p_NumElements)
{
	K15_ASSERT_TEXT(p_MemoryPool, "Memory Pool for Type 'K15_AsyncOperation' is NULL.");
	K15_ASSERT_TEXT(p_NumElements, "Element Count for Memory Pool 'K15_AsyncOperation' is 0.");

	K15_InitializeAsyncOperationMemoryPoolWithCustomAllocator(p_MemoryPool, K15_CreateDefaultMemoryAllocator("AsyncOperation Default Memory Pool Allocator"), p_NumElements);
}
/*********************************************************************************/
void K15_InitializeAsyncOperationMemoryPoolWithCustomAllocator(K15_AsyncOperationMemoryPool* p_MemoryPool, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_NumElements)
{
	K15_ASSERT_TEXT(p_MemoryPool, "Memory Pool for Type 'K15_AsyncOperation' is NULL.");
	K15_ASSERT_TEXT(p_NumElements, "Element Count for Memory Pool 'K15_AsyncOperation' is 0.");

	uint32 elementBufferSize = sizeof(K15_AsyncOperationMemoryPoolElement) * p_NumElements;
	byte* elementBuffer = (byte*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, elementBufferSize);

	memset(elementBuffer, 0, elementBufferSize);

	p_MemoryPool->elements = elementBuffer;
	p_MemoryPool->memoryAllocator = p_MemoryAllocator;
	p_MemoryPool->numElements = p_NumElements;
	p_MemoryPool->lastElementIndex = 0;
}
/*********************************************************************************/
K15_AsyncOperation* K15_GetAsyncOperationMemoryPoolElement(K15_AsyncOperationMemoryPool* p_MemoryPool)
{
	K15_ASSERT_TEXT(p_MemoryPool, "Memory Pool for Type 'K15_AsyncOperation' is NULL.");

	uint32 index = 0;
	K15_AsyncOperationMemoryPoolElement* poolBuffer = (K15_AsyncOperationMemoryPoolElement*)p_MemoryPool->elements;
	uint32 numElements = p_MemoryPool->numElements;
	uint32 lastElementIndex = p_MemoryPool->lastElementIndex;
	uint8 foundElement = K15_FALSE;

	K15_AsyncOperation* element = 0;

	if (lastElementIndex == numElements)
	{
		index = 0;
	}
	else
	{
		index = ++lastElementIndex;
	}

	K15_AsyncOperationMemoryPoolElement* poolElement = &poolBuffer[index];

	if (poolElement->inUse == K15_FALSE)
	{
		poolElement->inUse = K15_TRUE;

		element = (K15_AsyncOperation*)poolElement;
	}
	else
	{
		for (uint32 elementIndex = 0;
			elementIndex < numElements;
			++elementIndex)
		{
			poolElement = &poolBuffer[elementIndex];

			if (poolElement->inUse == K15_FALSE)
			{
				index = elementIndex;
				poolElement->inUse = K15_TRUE;
				element = (K15_AsyncOperation*)poolElement;
			}
		}
	}

	K15_ASSERT_TEXT(element, "Memory Pool for type 'K15_AsyncOperation' is full.");

	p_MemoryPool->lastElementIndex = index;

	return element;
}
/*********************************************************************************/
void K15_FreeAsyncOperationMemoryPoolElement(K15_AsyncOperationMemoryPool* p_MemoryPool, K15_AsyncOperation* p_Element)
{
	K15_ASSERT_TEXT(p_MemoryPool, "Memory Pool for Type 'K15_AsyncOperation' is NULL.");
	K15_ASSERT_TEXT(p_Element, "Element of Type 'K15_AsyncOperation' is NULL.");
	
	K15_AsyncOperationMemoryPoolElement* poolElement = (K15_AsyncOperationMemoryPoolElement*)p_Element;
	poolElement->inUse = K15_FALSE;
}
/*********************************************************************************/
void K15_ClearAsyncOperationMemoryPool(K15_AsyncOperationMemoryPool* p_MemoryPool)
{
	K15_FreeFromMemoryAllocator(&p_MemoryPool->memoryAllocator, p_MemoryPool->elements);
}
/*********************************************************************************/