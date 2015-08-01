/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_Thread_Stretch_Buffer_h_
#define _K15_Thread_Stretch_Buffer_h_

//forward declaration
struct K15_Thread;
struct K15_CustomMemoryAllocator;

//compare function
typedef unsigned char (*K15_ThreadCompareFnc)(K15_Thread** p_LHS, K15_Thread** p_RHS);

//get condition function
typedef unsigned char (*K15_ThreadConditionFnc)(K15_Thread** p_Element, void* p_UserData);

struct K15_ThreadStretchBuffer
{
	K15_CustomMemoryAllocator* memoryAllocator; //64bit
	K15_Thread** elements;							//64bit

	unsigned int numCapacity;					//32bit
	unsigned int numElements;					//32bit
	unsigned int flags;							//32bit
};

void K15_CreateThreadStretchBufferWithCustomAllocator(K15_ThreadStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator* p_MemoryAllocator, unsigned int p_ElementCapacity);
void K15_CreateThreadStretchBuffer(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteThreadStretchBuffer(K15_ThreadStretchBuffer* p_StretchBuffer);
void K15_ResizeThreadStretchBuffer(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearThreadStretchBuffer(K15_ThreadStretchBuffer* p_StretchBuffer);

K15_Thread** K15_PushThreadStretchBufferElement(K15_ThreadStretchBuffer* p_StretchBuffer, K15_Thread* p_Element);
unsigned char K15_PopThreadStretchBufferIndex(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopThreadStretchBufferElement(K15_ThreadStretchBuffer* p_StretchBuffer, K15_Thread* p_Element);
unsigned char K15_PopThreadStretchBufferCompare(K15_ThreadStretchBuffer* p_StretchBuffer, K15_Thread* p_Element, K15_ThreadCompareFnc p_CompareFnc);

K15_Thread** K15_GetThreadStretchBufferElementUnsafe(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_Thread** K15_GetThreadStretchBufferElement(K15_ThreadStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_Thread** K15_GetThreadStretchBufferElementConditional(K15_ThreadStretchBuffer* p_StretchBuffer, K15_ThreadConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_Thread_Stretch_Buffer_h_s