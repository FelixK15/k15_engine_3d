/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_ResourceLoader_Stretch_Buffer_h_
#define _K15_ResourceLoader_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_ResourceLoader;

//compare function
typedef unsigned char (*K15_ResourceLoaderCompareFnc)(K15_ResourceLoader* p_LHS, K15_ResourceLoader* p_RHS);

//get condition function
typedef unsigned char (*K15_ResourceLoaderConditionFnc)(K15_ResourceLoader* p_Element, void* p_UserData);

struct K15_ResourceLoaderStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_ResourceLoader* elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateResourceLoaderStretchBufferWithCustomAllocator(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity = 8);
void K15_CreateResourceLoaderStretchBuffer(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteResourceLoaderStretchBuffer(K15_ResourceLoaderStretchBuffer* p_StretchBuffer);
void K15_ResizeResourceLoaderStretchBuffer(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearResourceLoaderStretchBuffer(K15_ResourceLoaderStretchBuffer* p_StretchBuffer);

K15_ResourceLoader* K15_PushResourceLoaderStretchBufferElement(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, K15_ResourceLoader p_Element);
unsigned char K15_PopResourceLoaderStretchBufferIndex(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopResourceLoaderStretchBufferElement(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, K15_ResourceLoader p_Element);
unsigned char K15_PopResourceLoaderStretchBufferCompare(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, K15_ResourceLoader p_Element, K15_ResourceLoaderCompareFnc p_CompareFnc);

K15_ResourceLoader* K15_GetResourceLoaderStretchBufferElementUnsafe(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_ResourceLoader* K15_GetResourceLoaderStretchBufferElement(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_ResourceLoader* K15_GetResourceLoaderStretchBufferElementConditional(K15_ResourceLoaderStretchBuffer* p_StretchBuffer, K15_ResourceLoaderConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_ResourceLoader_Stretch_Buffer_h_s