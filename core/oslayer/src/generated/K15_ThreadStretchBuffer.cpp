/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#include "generated/K15_ThreadStretchBuffer.h"


/*********************************************************************************/
void K15_CreateThreadStretchBufferWithCustomAllocator(K15_ThreadStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity)
{
	K15_ASSERT_TEXT(!p_StretchBuffer->elements, "Stretch Buffer has already been created.");
	K15_ASSERT_TEXT(p_ElementCapacity != 0, "Can not reserve 0 elements.");

	unsigned int bytesToAllocate = p_ElementCapacity * sizeof(K15_Thread*);
	
	K15_Thread** elements = (K15_Thread**)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, bytesToAllocate);

	K15_ASSERT_TEXT(elements, "Out of memory.");

	p_StretchBuffer->memoryAllocator = p_MemoryAllocator;
	p_StretchBuffer->elements = elements;
	p_StretchBuffer->numCapacity = p_ElementCapacity;
	p_StretchBuffer->numElements = 0;
	p_StretchBuffer->flags = 0;

}
/*********************************************************************************/
void K15_CreateThreadStretchBuffer(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity)
{
	K15_CreateThreadStretchBufferWithCustomAllocator(p_StretchBuffer, K15_CreateDefaultMemoryAllocator("Thread Default Stretch Buffer Allocator"), p_ElementCapacity);
}
/*********************************************************************************/
void K15_DeleteThreadStretchBuffer(K15_ThreadStretchBuffer* p_StretchBuffer)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	if ((p_StretchBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0)
	{
		K15_FreeFromMemoryAllocator(&p_StretchBuffer->memoryAllocator, p_StretchBuffer->elements);
	}

	p_StretchBuffer->elements = 0;
}
/*********************************************************************************/
void K15_ResizeThreadStretchBuffer(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT((p_StretchBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0, "Stretch Buffer for Type '%s' can't be resized.", "K15_Thread*");

	unsigned int freeSlotIndex = p_StretchBuffer->numElements;
	unsigned int capacity = p_StretchBuffer->numCapacity;
	K15_CustomMemoryAllocator* memoryAllocator = &p_StretchBuffer->memoryAllocator;

	if (freeSlotIndex >= capacity)
	{
		unsigned int newSizeInBytes = sizeof(K15_Thread*) * p_ElementCapacity;
		unsigned int oldSizeInBytes = sizeof(K15_Thread*) * capacity;
		K15_Thread** oldMemory = p_StretchBuffer->elements;
		K15_Thread** newMemory = (K15_Thread**)K15_AllocateFromMemoryAllocator(memoryAllocator, newSizeInBytes);
		memcpy(newMemory, oldMemory, oldSizeInBytes);

		K15_FreeFromMemoryAllocator(memoryAllocator, oldMemory);

		p_StretchBuffer->elements = newMemory;
		p_StretchBuffer->numCapacity = p_ElementCapacity;
	}
}
/*********************************************************************************/
void K15_ClearThreadStretchBuffer(K15_ThreadStretchBuffer* p_StretchBuffer)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	p_StretchBuffer->numElements = 0;
}
/*********************************************************************************/
K15_Thread** K15_PushThreadStretchBufferElement(K15_ThreadStretchBuffer* p_StretchBuffer, K15_Thread* p_Element)
{	
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	unsigned int freeSlotIndex = p_StretchBuffer->numElements;
	unsigned int capacity = p_StretchBuffer->numCapacity;

	if (freeSlotIndex >= capacity)
	{
		K15_ResizeThreadStretchBuffer(p_StretchBuffer, capacity * 2);
	}

	p_StretchBuffer->elements[freeSlotIndex] = p_Element;
	++p_StretchBuffer->numElements;

	return &p_StretchBuffer->elements[freeSlotIndex];
}
/*********************************************************************************/
unsigned char K15_PopThreadStretchBufferIndex(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	K15_Thread** elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;

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

	p_StretchBuffer->elements = elements;
	p_StretchBuffer->numElements = numElements - 1;

	return 1;
}
/*********************************************************************************/
unsigned char K15_PopThreadStretchBufferElement(K15_ThreadStretchBuffer* p_StretchBuffer, K15_Thread* p_Element)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	K15_Thread** elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex < numElements;
		++elementIndex)
	{		
		if (memcmp(&elements[elementIndex], &p_Element, sizeof(K15_Thread*)) == 0)
		{
			K15_PopThreadStretchBufferIndex(p_StretchBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
unsigned char K15_PopThreadStretchBufferCompare(K15_ThreadStretchBuffer* p_StretchBuffer, K15_Thread* p_Element, K15_ThreadCompareFnc p_CompareFnc)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_CompareFnc, "Compare Function is NULL.");

	K15_Thread** elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex< numElements;
		++elementIndex)
	{		
		if (p_CompareFnc(&p_Element, &elements[elementIndex]) == 0)
		{
			K15_PopThreadStretchBufferIndex(p_StretchBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
K15_Thread** K15_GetThreadStretchBufferElementUnsafe(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	return &p_StretchBuffer->elements[p_Index];
}
/*********************************************************************************/
K15_Thread** K15_GetThreadStretchBufferElement(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	unsigned int numElements = p_StretchBuffer->numElements;

	K15_Thread** returnValue = 0;

	if (numElements > p_Index)
	{
		returnValue = &p_StretchBuffer->elements[p_Index];
	}

	return returnValue;
}
/*********************************************************************************/
K15_Thread** K15_GetThreadStretchBufferElementConditional(K15_ThreadStretchBuffer* p_StretchBuffer, K15_ThreadConditionFnc p_ConditionFnc, void* p_UserData)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_ConditionFnc, "Condition Function is NULL.");

	K15_Thread** elements = p_StretchBuffer->elements;
	K15_Thread** returnElement = 0;
	K15_Thread** currentElement = 0;

	unsigned int numElements = p_StretchBuffer->numElements;

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