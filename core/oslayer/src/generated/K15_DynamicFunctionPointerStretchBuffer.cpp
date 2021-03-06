/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#include "generated/K15_DynamicFunctionPointerStretchBuffer.h"


/*********************************************************************************/
void K15_CreateDynamicFunctionPointerStretchBufferWithCustomAllocator(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity)
{
	K15_ASSERT_TEXT(p_ElementCapacity != 0, "Can not reserve 0 elements.");

	unsigned int bytesToAllocate = p_ElementCapacity * sizeof(K15_DynamicFunctionPointer);
	
	K15_DynamicFunctionPointer* elements = (K15_DynamicFunctionPointer*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, bytesToAllocate);

	K15_ASSERT_TEXT(elements, "Out of memory.");

	p_StretchBuffer->memoryAllocator = p_MemoryAllocator;
	p_StretchBuffer->elements = elements;
	p_StretchBuffer->numCapacity = p_ElementCapacity;
	p_StretchBuffer->numElements = 0;
	p_StretchBuffer->flags = 0;

}
/*********************************************************************************/
void K15_CreateDynamicFunctionPointerStretchBuffer(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity)
{
	K15_CreateDynamicFunctionPointerStretchBufferWithCustomAllocator(p_StretchBuffer, K15_CreateDefaultMemoryAllocator("DynamicFunctionPointer Default Stretch Buffer Allocator"), p_ElementCapacity);
}
/*********************************************************************************/
void K15_DeleteDynamicFunctionPointerStretchBuffer(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");

	if ((p_StretchBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0)
	{
		K15_FreeFromMemoryAllocator(&p_StretchBuffer->memoryAllocator, p_StretchBuffer->elements);
	}

	p_StretchBuffer->elements = 0;
}
/*********************************************************************************/
void K15_ResizeDynamicFunctionPointerStretchBuffer(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT((p_StretchBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0, "Stretch Buffer for Type '%s' can't be resized.", "K15_DynamicFunctionPointer");

	unsigned int freeSlotIndex = p_StretchBuffer->numElements;
	unsigned int capacity = p_StretchBuffer->numCapacity;
	K15_CustomMemoryAllocator* memoryAllocator = &p_StretchBuffer->memoryAllocator;

	if (freeSlotIndex >= capacity)
	{
		unsigned int newSizeInBytes = sizeof(K15_DynamicFunctionPointer) * p_ElementCapacity;
		unsigned int oldSizeInBytes = sizeof(K15_DynamicFunctionPointer) * capacity;
		K15_DynamicFunctionPointer* oldMemory = p_StretchBuffer->elements;
		K15_DynamicFunctionPointer* newMemory = (K15_DynamicFunctionPointer*)K15_AllocateFromMemoryAllocator(memoryAllocator, newSizeInBytes);
		memcpy(newMemory, oldMemory, oldSizeInBytes);

		K15_FreeFromMemoryAllocator(memoryAllocator, oldMemory);

		p_StretchBuffer->elements = newMemory;
		p_StretchBuffer->numCapacity = p_ElementCapacity;
	}
}
/*********************************************************************************/
void K15_ClearDynamicFunctionPointerStretchBuffer(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	p_StretchBuffer->numElements = 0;
}
/*********************************************************************************/
K15_DynamicFunctionPointer* K15_PushDynamicFunctionPointerStretchBufferElement(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_DynamicFunctionPointer p_Element)
{	
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	unsigned int freeSlotIndex = p_StretchBuffer->numElements;
	unsigned int capacity = p_StretchBuffer->numCapacity;

	if (freeSlotIndex >= capacity)
	{
		K15_ResizeDynamicFunctionPointerStretchBuffer(p_StretchBuffer, capacity * 2);
	}

	p_StretchBuffer->elements[freeSlotIndex] = p_Element;
	++p_StretchBuffer->numElements;

	return &p_StretchBuffer->elements[freeSlotIndex];
}
/*********************************************************************************/
unsigned char K15_PopDynamicFunctionPointerStretchBufferIndex(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	K15_DynamicFunctionPointer* elements = p_StretchBuffer->elements;
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
unsigned char K15_PopDynamicFunctionPointerStretchBufferElement(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_DynamicFunctionPointer p_Element)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	K15_DynamicFunctionPointer* elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex < numElements;
		++elementIndex)
	{		
		if (memcmp(&elements[elementIndex], &p_Element, sizeof(K15_DynamicFunctionPointer)) == 0)
		{
			K15_PopDynamicFunctionPointerStretchBufferIndex(p_StretchBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
unsigned char K15_PopDynamicFunctionPointerStretchBufferCompare(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_DynamicFunctionPointer p_Element, K15_DynamicFunctionPointerCompareFnc p_CompareFnc)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_CompareFnc, "Compare Function is NULL.");

	K15_DynamicFunctionPointer* elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex< numElements;
		++elementIndex)
	{		
		if (p_CompareFnc(&p_Element, &elements[elementIndex]) == 0)
		{
			K15_PopDynamicFunctionPointerStretchBufferIndex(p_StretchBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
K15_DynamicFunctionPointer* K15_GetDynamicFunctionPointerStretchBufferElementUnsafe(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	return &p_StretchBuffer->elements[p_Index];
}
/*********************************************************************************/
K15_DynamicFunctionPointer* K15_GetDynamicFunctionPointerStretchBufferElement(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	unsigned int numElements = p_StretchBuffer->numElements;

	K15_DynamicFunctionPointer* returnValue = 0;

	if (numElements > p_Index)
	{
		returnValue = &p_StretchBuffer->elements[p_Index];
	}

	return returnValue;
}
/*********************************************************************************/
K15_DynamicFunctionPointer* K15_GetDynamicFunctionPointerStretchBufferElementConditional(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_DynamicFunctionPointerConditionFnc p_ConditionFnc, void* p_UserData)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_ConditionFnc, "Condition Function is NULL.");

	K15_DynamicFunctionPointer* elements = p_StretchBuffer->elements;
	K15_DynamicFunctionPointer* returnElement = 0;
	K15_DynamicFunctionPointer* currentElement = 0;

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