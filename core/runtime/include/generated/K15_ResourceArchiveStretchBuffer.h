/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_ResourceArchive_Stretch_Buffer_h_
#define _K15_ResourceArchive_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_ResourceArchive;

//compare function
typedef unsigned char (*K15_ResourceArchiveCompareFnc)(K15_ResourceArchive* p_LHS, K15_ResourceArchive* p_RHS);

//get condition function
typedef unsigned char (*K15_ResourceArchiveConditionFnc)(K15_ResourceArchive* p_Element, void* p_UserData);

struct K15_ResourceArchiveStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_ResourceArchive* elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateResourceArchiveStretchBufferWithCustomAllocator(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity = 8);
void K15_CreateResourceArchiveStretchBuffer(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteResourceArchiveStretchBuffer(K15_ResourceArchiveStretchBuffer* p_StretchBuffer);
void K15_ResizeResourceArchiveStretchBuffer(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearResourceArchiveStretchBuffer(K15_ResourceArchiveStretchBuffer* p_StretchBuffer);

K15_ResourceArchive* K15_PushResourceArchiveStretchBufferElement(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, K15_ResourceArchive p_Element);
unsigned char K15_PopResourceArchiveStretchBufferIndex(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopResourceArchiveStretchBufferElement(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, K15_ResourceArchive p_Element);
unsigned char K15_PopResourceArchiveStretchBufferCompare(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, K15_ResourceArchive p_Element, K15_ResourceArchiveCompareFnc p_CompareFnc);

K15_ResourceArchive* K15_GetResourceArchiveStretchBufferElementUnsafe(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_ResourceArchive* K15_GetResourceArchiveStretchBufferElement(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_ResourceArchive* K15_GetResourceArchiveStretchBufferElementConditional(K15_ResourceArchiveStretchBuffer* p_StretchBuffer, K15_ResourceArchiveConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_ResourceArchive_Stretch_Buffer_h_s