/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#include "generated/K15_DynamicLibraryStretchBuffer.h"


/*********************************************************************************/
void K15_CreateDynamicLibraryStretchBufferWithCustomAllocator(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator* p_MemoryAllocator, unsigned int p_ElementCapacity)
{
	K15_ASSERT_TEXT(!p_StretchBuffer->elements, "Stretch Buffer has already been created.");
	K15_ASSERT_TEXT(p_MemoryAllocator, "No memory allocator defined.");
	K15_ASSERT_TEXT(p_ElementCapacity != 0, "Can not reserve 0 elements.");

	unsigned int bytesToAllocate = p_ElementCapacity * sizeof(K15_DynamicLibrary*);
	
	K15_DynamicLibrary** elements = (K15_DynamicLibrary**)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, bytesToAllocate);

	K15_ASSERT_TEXT(elements, "Out of memory.");

	p_StretchBuffer->memoryAllocator = p_MemoryAllocator;
	p_StretchBuffer->elements = elements;
	p_StretchBuffer->numCapacity = p_ElementCapacity;
	p_StretchBuffer->numElements = 0;
	p_StretchBuffer->flags = 0;

}
/*********************************************************************************/
void K15_CreateDynamicLibraryStretchBuffer(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity)
{
	K15_CreateDynamicLibraryStretchBufferWithCustomAllocator(p_StretchBuffer, K15_CreateDefaultMemoryAllocator("DynamicLibrary Default Stretch Buffer Allocator"), p_ElementCapacity);
}
/*********************************************************************************/
void K15_DeleteDynamicLibraryStretchBuffer(K15_DynamicLibraryStretchBuffer* p_StretchBuffer)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	if ((p_StretchBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0)
	{
		K15_FreeFromMemoryAllocator(p_StretchBuffer->memoryAllocator, p_StretchBuffer->elements);
	}

	p_StretchBuffer->elements = 0;
}
/*********************************************************************************/
void K15_ResizeDynamicLibraryStretchBuffer(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT((p_StretchBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0, "Stretch Buffer for Type '%s' can't be resized.", "K15_DynamicLibrary*");

	unsigned int freeSlotIndex = p_StretchBuffer->numElements;
	unsigned int capacity = p_StretchBuffer->numCapacity;
	K15_CustomMemoryAllocator* memoryAllocator = p_StretchBuffer->memoryAllocator;

	if (freeSlotIndex >= capacity)
	{
		unsigned int newSizeInBytes = sizeof(K15_DynamicLibrary*) * p_ElementCapacity;
		unsigned int oldSizeInBytes = sizeof(K15_DynamicLibrary*) * capacity;
		K15_DynamicLibrary** oldMemory = p_StretchBuffer->elements;
		K15_DynamicLibrary** newMemory = (K15_DynamicLibrary**)K15_AllocateFromMemoryAllocator(memoryAllocator, newSizeInBytes);
		memcpy(newMemory, oldMemory, oldSizeInBytes);

		K15_FreeFromMemoryAllocator(memoryAllocator, oldMemory);

		p_StretchBuffer->elements = newMemory;
		p_StretchBuffer->numCapacity = p_ElementCapacity;
	}
}
/*********************************************************************************/
void K15_ClearDynamicLibraryStretchBuffer(K15_DynamicLibraryStretchBuffer* p_StretchBuffer)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	p_StretchBuffer->numElements = 0;
}
/*********************************************************************************/
void K15_PushDynamicLibraryStretchBufferElement(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_DynamicLibrary* p_Element)
{	
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	unsigned int freeSlotIndex = p_StretchBuffer->numElements;
	unsigned int capacity = p_StretchBuffer->numCapacity;

	if (freeSlotIndex >= capacity)
	{
		K15_ResizeDynamicLibraryStretchBuffer(p_StretchBuffer, capacity * 2);
	}

	p_StretchBuffer->elements[freeSlotIndex] = p_Element;
	++p_StretchBuffer->numElements;
}
/*********************************************************************************/
unsigned char K15_PopDynamicLibraryStretchBufferIndex(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	K15_DynamicLibrary** elements = p_StretchBuffer->elements;
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
unsigned char K15_PopDynamicLibraryStretchBufferElement(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_DynamicLibrary* p_Element)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	K15_DynamicLibrary** elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex < numElements;
		++elementIndex)
	{		
		if (memcmp(&elements[elementIndex], &p_Element, sizeof(K15_DynamicLibrary*)) == 0)
		{
			K15_PopDynamicLibraryStretchBufferIndex(p_StretchBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
unsigned char K15_PopDynamicLibraryStretchBufferCompare(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_DynamicLibrary* p_Element, K15_DynamicLibraryCompareFnc p_CompareFnc)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_CompareFnc, "Compare Function is NULL.");

	K15_DynamicLibrary** elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex< numElements;
		++elementIndex)
	{		
		if (p_CompareFnc(&p_Element, &elements[elementIndex]) == 0)
		{
			K15_PopDynamicLibraryStretchBufferIndex(p_StretchBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
K15_DynamicLibrary** K15_GetDynamicLibraryStretchBufferElementUnsafe(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	return &p_StretchBuffer->elements[p_Index];
}
/*********************************************************************************/
K15_DynamicLibrary** K15_GetDynamicLibraryStretchBufferElement(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	unsigned int numElements = p_StretchBuffer->numElements;

	K15_DynamicLibrary** returnValue = 0;

	if (numElements > p_Index)
	{
		returnValue = &p_StretchBuffer->elements[p_Index];
	}

	return returnValue;
}
/*********************************************************************************/
K15_DynamicLibrary** K15_GetDynamicLibraryStretchBufferElementConditional(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_DynamicLibraryConditionFnc p_ConditionFnc, void* p_UserData)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_ConditionFnc, "Condition Function is NULL.");

	K15_DynamicLibrary** elements = p_StretchBuffer->elements;
	K15_DynamicLibrary** returnElement = 0;
	K15_DynamicLibrary** currentElement = 0;

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