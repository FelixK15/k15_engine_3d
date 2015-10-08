/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#include "INPUT_FILE"


/*********************************************************************************/
void K15_CreateResourceLoaderFixedBufferWithCustomAllocator(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_Capacity)
{
	K15_ASSERT_TEXT(p_Capacity != 0, "Can not reserve 0 elements.");
	K15_ASSERT_TEXT(!p_FixedBuffer->elements, "Fixed Buffer has already been created.");

	unsigned int bytesToAllocate = p_Capacity * sizeof(K15_ResourceLoader);
	K15_ResourceLoader* elements = (K15_ResourceLoader*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, bytesToAllocate);

	K15_ASSERT_TEXT(elements, "Out of memory.");

	p_FixedBuffer->memoryAllocator = p_MemoryAllocator;
	p_FixedBuffer->elements = elements;
	p_FixedBuffer->numCapacity = p_Capacity;
	p_FixedBuffer->numElements = 0;
	p_FixedBuffer->flags = 0;
}
/*********************************************************************************/
void K15_CreateResourceLoaderFixedBuffer(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Capacity)
{
	K15_CreateResourceLoaderFixedBufferWithCustomAllocator(p_FixedBuffer, K15_CreateDefaultMemoryAllocator("ResourceLoader Default Fixed Buffer Allocator"), p_Capacity);
}
/*********************************************************************************/
void K15_DeleteResourceLoaderFixedBuffer(K15_ResourceLoaderFixedBuffer* p_FixedBuffer)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	if ((p_FixedBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0)
	{
		K15_FreeFromMemoryAllocator(&p_FixedBuffer->memoryAllocator, p_FixedBuffer->elements);
	}

	p_FixedBuffer->elements = 0;
}
/*********************************************************************************/
void K15_ClearResourceLoaderFixedBuffer(K15_ResourceLoaderFixedBuffer* p_FixedBuffer)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	p_FixedBuffer->numElements = 0;
}
/*********************************************************************************/
K15_ResourceLoader* K15_PushResourceLoaderFixedBufferElement(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_ResourceLoader p_Element)
{	
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	unsigned int freeSlotIndex = p_FixedBuffer->numElements;
	unsigned int capacity = p_FixedBuffer->numCapacity;

	K15_ASSERT_TEXT(capacity >= freeSlotIndex, "Fixed Buffer is full.");

	p_FixedBuffer->elements[freeSlotIndex] = p_Element;
	++p_FixedBuffer->numElements;

	return &p_FixedBuffer->elements[freeSlotIndex];
}
/*********************************************************************************/
unsigned char K15_PopResourceLoaderFixedBufferIndex(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	K15_ResourceLoader* elements = p_FixedBuffer->elements;
	unsigned int numElements = p_FixedBuffer->numElements;

	if (p_Index != (numElements - 1))
	{
		for (unsigned int elementIndex = p_Index;
			(elementIndex + 1) < numElements;
			++elementIndex)
		{		
			elements[elementIndex] = elements[elementIndex + 1];
		}
	}

	//elements[numElements] = 0;

	p_FixedBuffer->elements = elements;
	p_FixedBuffer->numElements = numElements - 1;

	return 1;
}
/*********************************************************************************/
unsigned char K15_PopResourceLoaderFixedBufferElement(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_ResourceLoader p_Element)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	K15_ResourceLoader* elements = p_FixedBuffer->elements;
	unsigned int numElements = p_FixedBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex < numElements;
		++elementIndex)
	{		
		if (memcmp(&elements[elementIndex], &p_Element, sizeof(K15_ResourceLoader)) == 0)
		{
			K15_PopResourceLoaderFixedBufferIndex(p_FixedBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
unsigned char K15_PopResourceLoaderFixedBufferCompare(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_ResourceLoader p_Element, K15_ResourceLoaderCompareFnc p_CompareFnc)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_CompareFnc, "Compare Function is NULL.");

	K15_ResourceLoader* elements = p_FixedBuffer->elements;
	unsigned int numElements = p_FixedBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex< numElements;
		++elementIndex)
	{		
		if (p_CompareFnc(&p_Element, &elements[elementIndex]) == 0)
		{
			K15_PopResourceLoaderFixedBufferIndex(p_FixedBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
K15_ResourceLoader* K15_GetResourceLoaderFixedBufferElementUnsafe(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	return &p_FixedBuffer->elements[p_Index];
}
/*********************************************************************************/
K15_ResourceLoader* K15_GetResourceLoaderFixedBufferElement(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	unsigned int numElements = p_FixedBuffer->numElements;

	K15_ResourceLoader* returnValue = 0;

	if (numElements > p_Index)
	{
		returnValue = &p_FixedBuffer->elements[p_Index];
	}

	return returnValue;
}
/*********************************************************************************/
K15_ResourceLoader* K15_GetResourceLoaderFixedBufferElementConditional(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_ResourceLoaderConditionFnc p_ConditionFnc, void* p_UserData)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_ConditionFnc, "Condition Function is NULL.");

	K15_ResourceLoader* elements = p_FixedBuffer->elements;
	K15_ResourceLoader* returnElement = 0;
	K15_ResourceLoader* currentElement = 0;

	unsigned int numElements = p_FixedBuffer->numElements;

	for (unsigned int elementIndex = 0;
		elementIndex < numElements;
		++elementIndex)
	{		
		currentElement = &elements[elementIndex];

		if (p_ConditionFnc(currentElement, p_UserData) == 0)
		{
			returnElement = currentElement;
			break;
		}
	}

	return returnElement;
}
/*********************************************************************************/