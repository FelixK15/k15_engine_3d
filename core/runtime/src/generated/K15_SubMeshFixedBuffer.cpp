/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#include "generated/K15_SubMeshFixedBuffer.h"

/*********************************************************************************/
void K15_CreateSubMeshFixedBufferWithPreallocatedMemory(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned char* p_Buffer, unsigned int p_BufferCapacityInByte)
{
	K15_ASSERT_TEXT(p_Buffer, "Input buffer is NULL.");
	K15_ASSERT_TEXT(p_BufferCapacityInByte != 0, "Input buffer size is 0.");
	K15_ASSERT_TEXT(!p_FixedBuffer->elements, "Fixed Buffer has already been created.");

	unsigned int numCapacity = p_BufferCapacityInByte / sizeof(K15_SubMesh);

	K15_ASSERT_TEXT(numCapacity != 0, "Byte count '%d' is less than one element of type %s (%d byte(s)).", p_BufferCapacityInByte, "K15_SubMesh", sizeof(K15_SubMesh));

	K15_SubMesh* elements = (K15_SubMesh*)p_Buffer;

	p_FixedBuffer->elements = elements;
	p_FixedBuffer->numCapacity = numCapacity;
	p_FixedBuffer->numElements = 0;
	p_FixedBuffer->flags = K15_USE_EXTERNAL_BUFFER;
}
/*********************************************************************************/
void K15_CreateSubMeshFixedBuffer(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Capacity)
{
	K15_ASSERT_TEXT(p_Capacity != 0, "Can not reserve 0 elements.");
	K15_ASSERT_TEXT(!p_FixedBuffer->elements, "Fixed Buffer has already been created.");

	unsigned int bytesToAllocate = p_Capacity * sizeof(K15_SubMesh);
	K15_SubMesh* elements = (K15_SubMesh*)K15_RT_MALLOC(bytesToAllocate);

	K15_ASSERT_TEXT(elements, "Out of memory.");

	p_FixedBuffer->elements = elements;
	p_FixedBuffer->numCapacity = p_Capacity;
	p_FixedBuffer->numElements = 0;
	p_FixedBuffer->flags = 0;
}
/*********************************************************************************/
void K15_DeleteSubMeshFixedBuffer(K15_SubMeshFixedBuffer* p_FixedBuffer)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	if ((p_FixedBuffer->flags & K15_USE_EXTERNAL_BUFFER) == 0)
	{
		K15_RT_FREE(p_FixedBuffer->elements);
	}

	p_FixedBuffer->elements = 0;
}
/*********************************************************************************/
void K15_ClearSubMeshFixedBuffer(K15_SubMeshFixedBuffer* p_FixedBuffer)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	p_FixedBuffer->numElements = 0;
}
/*********************************************************************************/
void K15_PushSubMeshFixedBufferElement(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_SubMesh p_Element)
{	
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	unsigned int freeSlotIndex = p_FixedBuffer->numElements;
	unsigned int capacity = p_FixedBuffer->numCapacity;

	K15_ASSERT_TEXT(capacity >= freeSlotIndex, "Fixed Buffer is full.");

	p_FixedBuffer->elements[freeSlotIndex] = p_Element;
	++p_FixedBuffer->numElements;
}
/*********************************************************************************/
unsigned char K15_PopSubMeshFixedBufferIndex(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	K15_SubMesh* elements = p_FixedBuffer->elements;
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
unsigned char K15_PopSubMeshFixedBufferElement(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_SubMesh p_Element)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	K15_SubMesh* elements = p_FixedBuffer->elements;
	unsigned int numElements = p_FixedBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex < numElements;
		++elementIndex)
	{		
		if (memcmp(&elements[elementIndex], &p_Element, sizeof(K15_SubMesh)) == 0)
		{
			K15_PopSubMeshFixedBufferIndex(p_FixedBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
unsigned char K15_PopSubMeshFixedBufferCompare(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_SubMesh p_Element, K15_SubMeshCompareFnc p_CompareFnc)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_CompareFnc, "Compare Function is NULL.");

	K15_SubMesh* elements = p_FixedBuffer->elements;
	unsigned int numElements = p_FixedBuffer->numElements;
	unsigned char returnValue = 0;

	for (unsigned int elementIndex = 0;
		elementIndex< numElements;
		++elementIndex)
	{		
		if (p_CompareFnc(&p_Element, &elements[elementIndex]) == 0)
		{
			K15_PopSubMeshFixedBufferIndex(p_FixedBuffer, elementIndex);
			returnValue = 1;
			break;
		}
	}

	return returnValue;
}
/*********************************************************************************/
K15_SubMesh* K15_GetSubMeshFixedBufferElementUnsafe(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	return &p_FixedBuffer->elements[p_Index];
}
/*********************************************************************************/
K15_SubMesh* K15_GetSubMeshFixedBufferElement(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Index)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");

	unsigned int numElements = p_FixedBuffer->numElements;

	K15_SubMesh* returnValue = 0;

	if (numElements > p_Index)
	{
		returnValue = &p_FixedBuffer->elements[p_Index];
	}

	return returnValue;
}
/*********************************************************************************/
K15_SubMesh* K15_GetSubMeshFixedBufferElementConditional(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_SubMeshConditionFnc p_ConditionFnc, void* p_UserData)
{
	K15_ASSERT_TEXT(p_FixedBuffer, "Fixed Buffer is NULL.");
	K15_ASSERT_TEXT(p_FixedBuffer->elements, "Fixed Buffer has not yet been created.");
	K15_ASSERT_TEXT(p_ConditionFnc, "Condition Function is NULL.");

	K15_SubMesh* elements = p_FixedBuffer->elements;
	K15_SubMesh* returnElement = 0;
	K15_SubMesh* currentElement = 0;

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