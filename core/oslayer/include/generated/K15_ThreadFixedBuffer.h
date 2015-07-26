/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_Thread_Fixed_Buffer_h_
#define _K15_Thread_Fixed_Buffer_h_

//forward declaration
struct K15_Thread;
struct K15_CustomMemoryAllocator;

//compare function
typedef unsigned char (*K15_ThreadCompareFnc)(K15_Thread** p_LHS, K15_Thread** p_RHS);

//get condition function
typedef unsigned char (*K15_ThreadConditionFnc)(K15_Thread** p_Element, void* p_UserData);

struct K15_ThreadFixedBuffer
{
	K15_CustomMemoryAllocator* memoryAllocator;	//64bit
	K15_Thread** elements;							//64bit

	unsigned int numCapacity;					//32bit
	unsigned int numElements;					//32bit
	unsigned int flags;							//32bit
};

void K15_CreateThreadFixedBufferWithCustomAllocator(K15_ThreadFixedBuffer* p_FixedBuffer, K15_CustomMemoryAllocator* p_MemoryAllocator, unsigned int p_Capacity = 8);
void K15_CreateThreadFixedBuffer(K15_ThreadFixedBuffer* p_FixedBuffer, unsigned int p_Capacity = 8);
void K15_DeleteThreadFixedBuffer(K15_ThreadFixedBuffer* p_FixedBuffer);
void K15_ResizeThreadFixedBuffer(K15_ThreadFixedBuffer* p_FixedBuffer, unsigned int p_Capacity);
void K15_ClearThreadFixedBuffer(K15_ThreadFixedBuffer* p_FixedBuffer);

void K15_PushThreadFixedBufferElement(K15_ThreadFixedBuffer* p_FixedBuffer, K15_Thread* p_Element);
unsigned char K15_PopThreadFixedBufferIndex(K15_ThreadFixedBuffer* p_FixedBuffer, unsigned int p_Index);
unsigned char K15_PopThreadFixedBufferElement(K15_ThreadFixedBuffer* p_FixedBuffer, K15_Thread* p_Element);
unsigned char K15_PopThreadFixedBufferCompare(K15_ThreadFixedBuffer* p_FixedBuffer, K15_Thread* p_Element, K15_ThreadCompareFnc p_CompareFnc);

K15_Thread** K15_GetThreadFixedBufferElementUnsafe(K15_ThreadFixedBuffer* p_FixedBuffer, unsigned int p_Index);
K15_Thread** K15_GetThreadFixedBufferElement(K15_ThreadFixedBuffer* p_FixedBuffer, unsigned int p_Index);
K15_Thread** K15_GetThreadFixedBufferElementConditional(K15_ThreadFixedBuffer* p_FixedBuffer, K15_ThreadConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_Thread_Stretch_Buffer_h_s