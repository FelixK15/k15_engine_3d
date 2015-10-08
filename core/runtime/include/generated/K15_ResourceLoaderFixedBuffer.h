/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_ResourceLoader_Fixed_Buffer_h_
#define _K15_ResourceLoader_Fixed_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_ResourceLoader;

//compare function
typedef unsigned char (*K15_ResourceLoaderCompareFnc)(K15_ResourceLoader* p_LHS, K15_ResourceLoader* p_RHS);

//get condition function
typedef unsigned char (*K15_ResourceLoaderConditionFnc)(K15_ResourceLoader* p_Element, void* p_UserData);

struct K15_ResourceLoaderFixedBuffer
{
	K15_CustomMemoryAllocator  memoryAllocator;
	K15_ResourceLoader* elements;							

	unsigned int numCapacity;					
	unsigned int numElements;					
	unsigned int flags;							
};

void K15_CreateResourceLoaderFixedBufferWithCustomAllocator(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_Capacity = 8);
void K15_CreateResourceLoaderFixedBuffer(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Capacity = 8);
void K15_DeleteResourceLoaderFixedBuffer(K15_ResourceLoaderFixedBuffer* p_FixedBuffer);
void K15_ResizeResourceLoaderFixedBuffer(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Capacity);
void K15_ClearResourceLoaderFixedBuffer(K15_ResourceLoaderFixedBuffer* p_FixedBuffer);

K15_ResourceLoader* K15_PushResourceLoaderFixedBufferElement(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_ResourceLoader p_Element);
unsigned char K15_PopResourceLoaderFixedBufferIndex(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Index);
unsigned char K15_PopResourceLoaderFixedBufferElement(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_ResourceLoader p_Element);
unsigned char K15_PopResourceLoaderFixedBufferCompare(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_ResourceLoader p_Element, K15_ResourceLoaderCompareFnc p_CompareFnc);

K15_ResourceLoader* K15_GetResourceLoaderFixedBufferElementUnsafe(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Index);
K15_ResourceLoader* K15_GetResourceLoaderFixedBufferElement(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, unsigned int p_Index);
K15_ResourceLoader* K15_GetResourceLoaderFixedBufferElementConditional(K15_ResourceLoaderFixedBuffer* p_FixedBuffer, K15_ResourceLoaderConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_ResourceLoader_Stretch_Buffer_h_s