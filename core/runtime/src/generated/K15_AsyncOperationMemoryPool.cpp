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
void* K15_DefaultAsyncOperationMalloc(size_t p_SizeInBytes)
{
	return ::malloc(p_SizeInBytes);
}
/*********************************************************************************/
void K15_DefaultAsyncOperationFree(void* p_Pointer)
{
	return ::free(p_Pointer);
}
/*********************************************************************************/





/*********************************************************************************/
void K15_InitializeAsyncOperationMemoryPool(K15_AsyncOperationMemoryPool* p_MemoryPool, unsigned int p_NumElements)
{
	K15_ASSERT_TEXT(p_MemoryPool, "Memory Pool for Type 'K15_AsyncOperation' is NULL.");
	K15_ASSERT_TEXT(p_NumElements, "Element Count for Memory Pool 'K15_AsyncOperation' is 0.");

	K15_InitializeAsyncOperationMemoryPoolWithCustomAllocator(p_MemoryPool, K15_DefaultAsyncOperationMalloc, K15_DefaultAsyncOperationFree, p_NumElements);
}
/*********************************************************************************/
void K15_InitializeAsyncOperationMemoryPoolWithCustomAllocator(K15_AsyncOperationMemoryPool* p_MemoryPool, K15_AsyncOperationMemoryPoolAllocFnc p_CustomMalloc, K15_AsyncOperationMemoryPoolFreeFnc p_CustomFree, unsigned int p_NumElements)
{
	K15_ASSERT_TEXT(p_MemoryPool, "Memory Pool for Type 'K15_AsyncOperation' is NULL.");
	K15_ASSERT_TEXT(p_CustomMalloc, "Custom Malloc Function for Memory Pool 'K15_AsyncOperation' is NULL.");
	K15_ASSERT_TEXT(p_CustomFree, "Custom Free Function for Memory Pool 'K15_AsyncOperation' is NULL.");
	K15_ASSERT_TEXT(p_NumElements, "Element Count for Memory Pool 'K15_AsyncOperation' is 0.");

	uint32 elementBufferSize = sizeof(K15_AsyncOperationMemoryPoolElement) * p_NumElements;
	byte* elementBuffer = (byte*)p_CustomMalloc(elementBufferSize);

	memset(elementBuffer, 0, elementBufferSize);

	p_MemoryPool->elements = elementBuffer;
	p_MemoryPool->customMalloc = p_CustomMalloc;
	p_MemoryPool->customFree = p_CustomFree;
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
	K15_AsyncOperationMemoryPoolFreeFnc  customFree = p_MemoryPool->customFree;
	byte* memoryPoolBuffer = p_MemoryPool->elements;
	
	customFree(memoryPoolBuffer);
}
/*********************************************************************************/