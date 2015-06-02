/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_Resource_Fixed_Buffer_h_
#define _K15_Resource_Fixed_Buffer_h_

//forward declaration
struct K15_Resource;

//compare function
typedef unsigned char (*K15_ResourceCompareFnc)(K15_Resource* p_LHS, K15_Resource* p_RHS);

//get condition function
typedef unsigned char (*K15_ResourceConditionFnc)(K15_Resource* p_Element, void* p_UserData);

struct K15_ResourceFixedBuffer
{
	K15_Resource* elements;			//64bit

	unsigned int numCapacity;	//32bit
	unsigned int numElements;	//32bit
	unsigned int flags;			//32bit
};

void K15_CreateResourceFixedBufferWithPreallocatedMemory(K15_ResourceFixedBuffer* p_FixedBuffer, unsigned char* p_Buffer, unsigned int p_BufferCapacityInByte);
void K15_CreateResourceFixedBuffer(K15_ResourceFixedBuffer* p_FixedBuffer, unsigned int p_Capacity = 8);
void K15_DeleteResourceFixedBuffer(K15_ResourceFixedBuffer* p_FixedBuffer);
void K15_ResizeResourceFixedBuffer(K15_ResourceFixedBuffer* p_FixedBuffer, unsigned int p_Capacity);
void K15_ClearResourceFixedBuffer(K15_ResourceFixedBuffer* p_FixedBuffer);

void K15_PushResourceFixedBufferElement(K15_ResourceFixedBuffer* p_FixedBuffer, K15_Resource p_Element);
unsigned char K15_PopResourceFixedBufferIndex(K15_ResourceFixedBuffer* p_FixedBuffer, unsigned int p_Index);
unsigned char K15_PopResourceFixedBufferElement(K15_ResourceFixedBuffer* p_FixedBuffer, K15_Resource p_Element);
unsigned char K15_PopResourceFixedBufferCompare(K15_ResourceFixedBuffer* p_FixedBuffer, K15_Resource p_Element, K15_ResourceCompareFnc p_CompareFnc);

K15_Resource* K15_GetResourceFixedBufferElementUnsafe(K15_ResourceFixedBuffer* p_FixedBuffer, unsigned int p_Index);
K15_Resource* K15_GetResourceFixedBufferElement(K15_ResourceFixedBuffer* p_FixedBuffer, unsigned int p_Index);
K15_Resource* K15_GetResourceFixedBufferElementConditional(K15_ResourceFixedBuffer* p_FixedBuffer, K15_ResourceConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_Resource_Stretch_Buffer_h_s