/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_DynamicFunctionPointer_Stretch_Buffer_h_
#define _K15_DynamicFunctionPointer_Stretch_Buffer_h_

//forward declaration
struct K15_DynamicFunctionPointer;

//compare function
typedef unsigned char (*K15_DynamicFunctionPointerCompareFnc)(K15_DynamicFunctionPointer* p_LHS, K15_DynamicFunctionPointer* p_RHS);

//get condition function
typedef unsigned char (*K15_DynamicFunctionPointerConditionFnc)(K15_DynamicFunctionPointer* p_Element, void* p_UserData);

struct K15_DynamicFunctionPointerStretchBuffer
{
	K15_DynamicFunctionPointer* elements;			//64bit

	unsigned int numCapacity;	//32bit
	unsigned int numElements;	//32bit
	unsigned int flags;			//32bit
};

void K15_CreateDynamicFunctionPointerStretchBufferWithPreallocatedMemory(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned char* p_Buffer, unsigned int p_BufferCapacityInByte);
void K15_CreateDynamicFunctionPointerStretchBuffer(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_Capacity = 8);
void K15_DeleteDynamicFunctionPointerStretchBuffer(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer);
void K15_ResizeDynamicFunctionPointerStretchBuffer(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_Capacity);
void K15_ClearDynamicFunctionPointerStretchBuffer(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer);

void K15_PushDynamicFunctionPointerStretchBufferElement(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_DynamicFunctionPointer p_Element);
unsigned char K15_PopDynamicFunctionPointerStretchBufferIndex(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopDynamicFunctionPointerStretchBufferElement(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_DynamicFunctionPointer p_Element);
unsigned char K15_PopDynamicFunctionPointerStretchBufferCompare(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_DynamicFunctionPointer p_Element, K15_DynamicFunctionPointerCompareFnc p_CompareFnc);

K15_DynamicFunctionPointer* K15_GetDynamicFunctionPointerStretchBufferElementUnsafe(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_DynamicFunctionPointer* K15_GetDynamicFunctionPointerStretchBufferElement(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_DynamicFunctionPointer* K15_GetDynamicFunctionPointerStretchBufferElementConditional(K15_DynamicFunctionPointerStretchBuffer* p_StretchBuffer, K15_DynamicFunctionPointerConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_DynamicFunctionPointer_Stretch_Buffer_h_s