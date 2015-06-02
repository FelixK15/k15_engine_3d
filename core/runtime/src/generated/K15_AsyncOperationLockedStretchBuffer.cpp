/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#include "generated/K15_AsyncOperationLockedStretchBuffer.h"

/*********************************************************************************/
void K15_CreateAsyncOperationStretchBufferWithPreallocatedMemory(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned char* p_Buffer, unsigned int p_BufferCapacityInByte)
{
	K15_ASSERT_TEXT(p_Buffer, "Input buffer is NULL.");
	K15_ASSERT_TEXT(p_BufferCapacityInByte != 0, "Input buffer size is 0.");
	K15_ASSERT_TEXT(!p_StretchBuffer->elements, "Stretch Buffer has already been created.");

	unsigned int numCapacity = p_BufferCapacityInByte / sizeof(K15_AsyncOperation);

	K15_ASSERT_TEXT(numCapacity != 0, "Byte count '%d' is less than one element of type %s (%d byte(s)).", p_BufferCapacityInByte, "K15_AsyncOperation", sizeof(K15_AsyncOperation));

	K15_AsyncOperation* elements = (K15_AsyncOperation*)p_Buffer;

	p_StretchBuffer->elements = elements;
	p_StretchBuffer->numCapacity = numCapacity;
	p_StretchBuffer->numElements = 0;
	p_StretchBuffer->flags = K15_USE_EXTERNAL_BUFFER;
}
/*********************************************************************************/
void K15_CreateAsyncOperationStretchBuffer(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_Capacity)
{
	K15_ASSERT_TEXT(p_Capacity != 0, "Can not reserve 0 elements.");
	K15_ASSERT_TEXT(!p_StretchBuffer->elements, "Stretch Buffer has already been created.");

	unsigned int bytesToAllocate = p_Capacity * sizeof(K15_AsyncOperation);
	K15_AsyncOperation* elements = (K15_AsyncOperation*)K15_OS_MALLOC(bytesToAllocate);

	K15_ASSERT_TEXT(elements, "Out of memory.");

	p_StretchBuffer->elements = elements;
	p_StretchBuffer->numCapacity = p_Capacity;
	p_StretchBuffer->numElements = 0;
	p_StretchBuffer->flags = 0;
}
/*********************************************************************************/
void K15_DeleteAsyncOperationStretchBuffer(K15_AsyncOperationStretchBuffer* p_StretchBuffer)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	if ((p_StretchBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0)
	{
		K15_OS_FREE(p_StretchBuffer->elements);
	}

	p_StretchBuffer->elements = 0;
}
/*********************************************************************************/
void K15_ResizeAsyncOperationStretchBuffer(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_Capacity)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT((p_StretchBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0, "Stretch Buffer for Type '%s' can't be resized.", "K15_AsyncOperation");

	unsigned int freeSlotIndex = p_StretchBuffer->numElements;
	unsigned int capacity = p_StretchBuffer->numCapacity;

	if (freeSlotIndex >= capacity)
	{
		unsigned int newSizeInBytes = sizeof(K15_AsyncOperation) * p_Capacity;
		unsigned int oldSizeInBytes = sizeof(K15_AsyncOperation) * capacity;
		K15_AsyncOperation* oldMemory = p_StretchBuffer->elements;
		K15_AsyncOperation* newMemory = (K15_AsyncOperation*)K15_OS_MALLOC(newSizeInBytes);
		memcpy(newMemory, oldMemory, oldSizeInBytes);

		K15_OS_FREE(oldMemory);

		p_StretchBuffer->elements = newMemory;
		p_StretchBuffer->numCapacity = p_Capacity;
	}
}
/*********************************************************************************/
void K15_ClearAsyncOperationStretchBuffer(K15_AsyncOperationStretchBuffer* p_StretchBuffer)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	p_StretchBuffer->numElements = 0;
}
/*********************************************************************************/
void K15_PushAsyncOperationStretchBufferElement(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_AsyncOperation p_Element)
{	
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	unsigned int freeSlotIndex = p_StretchBuffer->numElements;
	unsigned int capacity = p_StretchBuffer->numCapacity;

	if (freeSlotIndex >= capacity)
	{
		K15_ResizeAsyncOperationStretchBuffer(p_StretchBuffer, capacity * 2);
	}

	p_StretchBuffer->elements[freeSlotIndex] = p_Element;
	++p_StretchBuffer->numElements;
}
/*********************************************************************************/
unsigned char K15_PopAsyncOperationStretchBufferIndex(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	K15_AsyncOperation* elements = p_StretchBuffer->elements;
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
unsigned char K15_PopAsyncOperationStretchBufferElement(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_AsyncOperation p_Element)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	K15_AsyncOperation* elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex < numElements;
		++elementIndex)
	{		
		if (memcmp(&elements[elementIndex], &p_Element, sizeof(K15_AsyncOperation)) == 0)
		{
			K15_PopAsyncOperationStretchBufferIndex(p_StretchBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
unsigned char K15_PopAsyncOperationStretchBufferCompare(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_AsyncOperation p_Element, K15_AsyncOperationCompareFnc p_CompareFnc)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_CompareFnc, "Compare Function is NULL.");

	K15_AsyncOperation* elements = p_StretchBuffer->elements;
	unsigned int numElements = p_StretchBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex< numElements;
		++elementIndex)
	{		
		if (p_CompareFnc(&p_Element, &elements[elementIndex]) == 0)
		{
			K15_PopAsyncOperationStretchBufferIndex(p_StretchBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
K15_AsyncOperation* K15_GetAsyncOperationStretchBufferElementUnsafe(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	return &p_StretchBuffer->elements[p_Index];
}
/*********************************************************************************/
K15_AsyncOperation* K15_GetAsyncOperationStretchBufferElement(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");

	unsigned int numElements = p_StretchBuffer->numElements;

	K15_AsyncOperation* returnValue = 0;

	if (numElements > p_Index)
	{
		returnValue = &p_StretchBuffer->elements[p_Index];
	}

	return returnValue;
}
/*********************************************************************************/
K15_AsyncOperation* K15_GetAsyncOperationStretchBufferElementConditional(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_AsyncOperationConditionFnc p_ConditionFnc, void* p_UserData)
{
	K15_ASSERT_TEXT(p_StretchBuffer, "Stretch Buffer is NULL.");
	K15_ASSERT_TEXT(p_StretchBuffer->elements, "Stretch Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_ConditionFnc, "Condition Function is NULL.");

	K15_AsyncOperation* elements = p_StretchBuffer->elements;
	K15_AsyncOperation* returnElement = 0;
	K15_AsyncOperation* currentElement = 0;

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