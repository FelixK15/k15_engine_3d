/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_DirectoryWatchEntry_Stretch_Buffer_h_
#define _K15_DirectoryWatchEntry_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_DirectoryWatchEntry;

//compare function
typedef unsigned char (*K15_DirectoryWatchEntryCompareFnc)(K15_DirectoryWatchEntry* p_LHS, K15_DirectoryWatchEntry* p_RHS);

//get condition function
typedef unsigned char (*K15_DirectoryWatchEntryConditionFnc)(K15_DirectoryWatchEntry* p_Element, void* p_UserData);

struct K15_DirectoryWatchEntryStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_DirectoryWatchEntry* elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateDirectoryWatchEntryStretchBufferWithCustomAllocator(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity = 8);
void K15_CreateDirectoryWatchEntryStretchBuffer(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteDirectoryWatchEntryStretchBuffer(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer);
void K15_ResizeDirectoryWatchEntryStretchBuffer(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearDirectoryWatchEntryStretchBuffer(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer);

K15_DirectoryWatchEntry* K15_PushDirectoryWatchEntryStretchBufferElement(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, K15_DirectoryWatchEntry p_Element);
unsigned char K15_PopDirectoryWatchEntryStretchBufferIndex(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopDirectoryWatchEntryStretchBufferElement(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, K15_DirectoryWatchEntry p_Element);
unsigned char K15_PopDirectoryWatchEntryStretchBufferCompare(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, K15_DirectoryWatchEntry p_Element, K15_DirectoryWatchEntryCompareFnc p_CompareFnc);

K15_DirectoryWatchEntry* K15_GetDirectoryWatchEntryStretchBufferElementUnsafe(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_DirectoryWatchEntry* K15_GetDirectoryWatchEntryStretchBufferElement(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_DirectoryWatchEntry* K15_GetDirectoryWatchEntryStretchBufferElementConditional(K15_DirectoryWatchEntryStretchBuffer* p_StretchBuffer, K15_DirectoryWatchEntryConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_DirectoryWatchEntry_Stretch_Buffer_h_s