/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_ConfigValue_Stretch_Buffer_h_
#define _K15_ConfigValue_Stretch_Buffer_h_

//forward declaration
struct K15_ConfigValue;
struct K15_CustomMemoryAllocator;

//compare function
typedef unsigned char (*K15_ConfigValueCompareFnc)(K15_ConfigValue* p_LHS, K15_ConfigValue* p_RHS);

//get condition function
typedef unsigned char (*K15_ConfigValueConditionFnc)(K15_ConfigValue* p_Element, void* p_UserData);

struct K15_ConfigValueStretchBuffer
{
	K15_CustomMemoryAllocator* memoryAllocator; //64bit
	K15_ConfigValue* elements;							//64bit

	unsigned int numCapacity;					//32bit
	unsigned int numElements;					//32bit
	unsigned int flags;							//32bit
};

void K15_CreateConfigValueStretchBufferWithCustomAllocator(K15_ConfigValueStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator* p_MemoryAllocator, unsigned int p_ElementCapacity);
void K15_CreateConfigValueStretchBuffer(K15_ConfigValueStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteConfigValueStretchBuffer(K15_ConfigValueStretchBuffer* p_StretchBuffer);
void K15_ResizeConfigValueStretchBuffer(K15_ConfigValueStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearConfigValueStretchBuffer(K15_ConfigValueStretchBuffer* p_StretchBuffer);

void K15_PushConfigValueStretchBufferElement(K15_ConfigValueStretchBuffer* p_StretchBuffer, K15_ConfigValue p_Element);
unsigned char K15_PopConfigValueStretchBufferIndex(K15_ConfigValueStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopConfigValueStretchBufferElement(K15_ConfigValueStretchBuffer* p_StretchBuffer, K15_ConfigValue p_Element);
unsigned char K15_PopConfigValueStretchBufferCompare(K15_ConfigValueStretchBuffer* p_StretchBuffer, K15_ConfigValue p_Element, K15_ConfigValueCompareFnc p_CompareFnc);

K15_ConfigValue* K15_GetConfigValueStretchBufferElementUnsafe(K15_ConfigValueStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_ConfigValue* K15_GetConfigValueStretchBufferElement(K15_ConfigValueStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_ConfigValue* K15_GetConfigValueStretchBufferElementConditional(K15_ConfigValueStretchBuffer* p_StretchBuffer, K15_ConfigValueConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_ConfigValue_Stretch_Buffer_h_s