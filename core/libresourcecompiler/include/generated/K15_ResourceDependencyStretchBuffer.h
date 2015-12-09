/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_ResourceDependency_Stretch_Buffer_h_
#define _K15_ResourceDependency_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_ResourceDependency;

//compare function
typedef unsigned char (*K15_ResourceDependencyCompareFnc)(K15_ResourceDependency* p_LHS, K15_ResourceDependency* p_RHS);

//get condition function
typedef unsigned char (*K15_ResourceDependencyConditionFnc)(K15_ResourceDependency* p_Element, void* p_UserData);

struct K15_ResourceDependencyStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_ResourceDependency* elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateResourceDependencyStretchBufferWithCustomAllocator(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity = 8);
void K15_CreateResourceDependencyStretchBuffer(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteResourceDependencyStretchBuffer(K15_ResourceDependencyStretchBuffer* p_StretchBuffer);
void K15_ResizeResourceDependencyStretchBuffer(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearResourceDependencyStretchBuffer(K15_ResourceDependencyStretchBuffer* p_StretchBuffer);

K15_ResourceDependency* K15_PushResourceDependencyStretchBufferElement(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, K15_ResourceDependency p_Element);
unsigned char K15_PopResourceDependencyStretchBufferIndex(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopResourceDependencyStretchBufferElement(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, K15_ResourceDependency p_Element);
unsigned char K15_PopResourceDependencyStretchBufferCompare(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, K15_ResourceDependency p_Element, K15_ResourceDependencyCompareFnc p_CompareFnc);

K15_ResourceDependency* K15_GetResourceDependencyStretchBufferElementUnsafe(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_ResourceDependency* K15_GetResourceDependencyStretchBufferElement(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_ResourceDependency* K15_GetResourceDependencyStretchBufferElementConditional(K15_ResourceDependencyStretchBuffer* p_StretchBuffer, K15_ResourceDependencyConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_ResourceDependency_Stretch_Buffer_h_s