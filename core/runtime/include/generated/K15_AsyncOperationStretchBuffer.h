/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_AsyncOperation_Stretch_Buffer_h_
#define _K15_AsyncOperation_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_AsyncOperation;

//compare function
typedef unsigned char (*K15_AsyncOperationCompareFnc)(K15_AsyncOperation** p_LHS, K15_AsyncOperation** p_RHS);

//get condition function
typedef unsigned char (*K15_AsyncOperationConditionFnc)(K15_AsyncOperation** p_Element, void* p_UserData);

struct K15_AsyncOperationStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_AsyncOperation** elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateAsyncOperationStretchBufferWithCustomAllocator(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity = 8);
void K15_CreateAsyncOperationStretchBuffer(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteAsyncOperationStretchBuffer(K15_AsyncOperationStretchBuffer* p_StretchBuffer);
void K15_ResizeAsyncOperationStretchBuffer(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearAsyncOperationStretchBuffer(K15_AsyncOperationStretchBuffer* p_StretchBuffer);

K15_AsyncOperation** K15_PushAsyncOperationStretchBufferElement(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_AsyncOperation* p_Element);
unsigned char K15_PopAsyncOperationStretchBufferIndex(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopAsyncOperationStretchBufferElement(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_AsyncOperation* p_Element);
unsigned char K15_PopAsyncOperationStretchBufferCompare(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_AsyncOperation* p_Element, K15_AsyncOperationCompareFnc p_CompareFnc);

K15_AsyncOperation** K15_GetAsyncOperationStretchBufferElementUnsafe(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_AsyncOperation** K15_GetAsyncOperationStretchBufferElement(K15_AsyncOperationStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_AsyncOperation** K15_GetAsyncOperationStretchBufferElementConditional(K15_AsyncOperationStretchBuffer* p_StretchBuffer, K15_AsyncOperationConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_AsyncOperation_Stretch_Buffer_h_s