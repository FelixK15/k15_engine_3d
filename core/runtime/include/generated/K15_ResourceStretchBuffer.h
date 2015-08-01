/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_Resource_Stretch_Buffer_h_
#define _K15_Resource_Stretch_Buffer_h_

//forward declaration
struct K15_Resource;
struct K15_CustomMemoryAllocator;

//compare function
typedef unsigned char (*K15_ResourceCompareFnc)(K15_Resource* p_LHS, K15_Resource* p_RHS);

//get condition function
typedef unsigned char (*K15_ResourceConditionFnc)(K15_Resource* p_Element, void* p_UserData);

struct K15_ResourceStretchBuffer
{
	K15_CustomMemoryAllocator* memoryAllocator; //64bit
	K15_Resource* elements;							//64bit

	unsigned int numCapacity;					//32bit
	unsigned int numElements;					//32bit
	unsigned int flags;							//32bit
};

void K15_CreateResourceStretchBufferWithCustomAllocator(K15_ResourceStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator* p_MemoryAllocator, unsigned int p_ElementCapacity);
void K15_CreateResourceStretchBuffer(K15_ResourceStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteResourceStretchBuffer(K15_ResourceStretchBuffer* p_StretchBuffer);
void K15_ResizeResourceStretchBuffer(K15_ResourceStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearResourceStretchBuffer(K15_ResourceStretchBuffer* p_StretchBuffer);

K15_Resource* K15_PushResourceStretchBufferElement(K15_ResourceStretchBuffer* p_StretchBuffer, K15_Resource p_Element);
unsigned char K15_PopResourceStretchBufferIndex(K15_ResourceStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopResourceStretchBufferElement(K15_ResourceStretchBuffer* p_StretchBuffer, K15_Resource p_Element);
unsigned char K15_PopResourceStretchBufferCompare(K15_ResourceStretchBuffer* p_StretchBuffer, K15_Resource p_Element, K15_ResourceCompareFnc p_CompareFnc);

K15_Resource* K15_GetResourceStretchBufferElementUnsafe(K15_ResourceStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_Resource* K15_GetResourceStretchBufferElement(K15_ResourceStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_Resource* K15_GetResourceStretchBufferElementConditional(K15_ResourceStretchBuffer* p_StretchBuffer, K15_ResourceConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_Resource_Stretch_Buffer_h_s