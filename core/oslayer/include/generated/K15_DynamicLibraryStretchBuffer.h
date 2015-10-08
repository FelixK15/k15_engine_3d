/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_DynamicLibrary_Stretch_Buffer_h_
#define _K15_DynamicLibrary_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_DynamicLibrary;

//compare function
typedef unsigned char (*K15_DynamicLibraryCompareFnc)(K15_DynamicLibrary** p_LHS, K15_DynamicLibrary** p_RHS);

//get condition function
typedef unsigned char (*K15_DynamicLibraryConditionFnc)(K15_DynamicLibrary** p_Element, void* p_UserData);

struct K15_DynamicLibraryStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_DynamicLibrary** elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateDynamicLibraryStretchBufferWithCustomAllocator(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity = 8);
void K15_CreateDynamicLibraryStretchBuffer(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteDynamicLibraryStretchBuffer(K15_DynamicLibraryStretchBuffer* p_StretchBuffer);
void K15_ResizeDynamicLibraryStretchBuffer(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearDynamicLibraryStretchBuffer(K15_DynamicLibraryStretchBuffer* p_StretchBuffer);

K15_DynamicLibrary** K15_PushDynamicLibraryStretchBufferElement(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_DynamicLibrary* p_Element);
unsigned char K15_PopDynamicLibraryStretchBufferIndex(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopDynamicLibraryStretchBufferElement(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_DynamicLibrary* p_Element);
unsigned char K15_PopDynamicLibraryStretchBufferCompare(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_DynamicLibrary* p_Element, K15_DynamicLibraryCompareFnc p_CompareFnc);

K15_DynamicLibrary** K15_GetDynamicLibraryStretchBufferElementUnsafe(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_DynamicLibrary** K15_GetDynamicLibraryStretchBufferElement(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_DynamicLibrary** K15_GetDynamicLibraryStretchBufferElementConditional(K15_DynamicLibraryStretchBuffer* p_StretchBuffer, K15_DynamicLibraryConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_DynamicLibrary_Stretch_Buffer_h_s