/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_FileWatchEntry_Stretch_Buffer_h_
#define _K15_FileWatchEntry_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_FileWatchEntry;

//compare function
typedef unsigned char (*K15_FileWatchEntryCompareFnc)(K15_FileWatchEntry* p_LHS, K15_FileWatchEntry* p_RHS);

//get condition function
typedef unsigned char (*K15_FileWatchEntryConditionFnc)(K15_FileWatchEntry* p_Element, void* p_UserData);

struct K15_FileWatchEntryStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_FileWatchEntry* elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateFileWatchEntryStretchBufferWithCustomAllocator(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity);
void K15_CreateFileWatchEntryStretchBuffer(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteFileWatchEntryStretchBuffer(K15_FileWatchEntryStretchBuffer* p_StretchBuffer);
void K15_ResizeFileWatchEntryStretchBuffer(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearFileWatchEntryStretchBuffer(K15_FileWatchEntryStretchBuffer* p_StretchBuffer);

K15_FileWatchEntry* K15_PushFileWatchEntryStretchBufferElement(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, K15_FileWatchEntry p_Element);
unsigned char K15_PopFileWatchEntryStretchBufferIndex(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopFileWatchEntryStretchBufferElement(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, K15_FileWatchEntry p_Element);
unsigned char K15_PopFileWatchEntryStretchBufferCompare(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, K15_FileWatchEntry p_Element, K15_FileWatchEntryCompareFnc p_CompareFnc);

K15_FileWatchEntry* K15_GetFileWatchEntryStretchBufferElementUnsafe(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_FileWatchEntry* K15_GetFileWatchEntryStretchBufferElement(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_FileWatchEntry* K15_GetFileWatchEntryStretchBufferElementConditional(K15_FileWatchEntryStretchBuffer* p_StretchBuffer, K15_FileWatchEntryConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_FileWatchEntry_Stretch_Buffer_h_s