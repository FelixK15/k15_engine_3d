/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_2DSceneElement_Stretch_Buffer_h_
#define _K15_2DSceneElement_Stretch_Buffer_h_

//forward declaration
struct K15_2DSceneElement;
struct K15_CustomMemoryAllocator;

//compare function
typedef unsigned char (*K15_2DSceneElementCompareFnc)(K15_2DSceneElement* p_LHS, K15_2DSceneElement* p_RHS);

//get condition function
typedef unsigned char (*K15_2DSceneElementConditionFnc)(K15_2DSceneElement* p_Element, void* p_UserData);

struct K15_2DSceneElementStretchBuffer
{
	K15_CustomMemoryAllocator* memoryAllocator; //64bit
	K15_2DSceneElement* elements;							//64bit

	unsigned int numCapacity;					//32bit
	unsigned int numElements;					//32bit
	unsigned int flags;							//32bit
};

void K15_Create2DSceneElementStretchBufferWithCustomAllocator(K15_2DSceneElementStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator* p_MemoryAllocator, unsigned int p_ElementCapacity);
void K15_Create2DSceneElementStretchBuffer(K15_2DSceneElementStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_Delete2DSceneElementStretchBuffer(K15_2DSceneElementStretchBuffer* p_StretchBuffer);
void K15_Resize2DSceneElementStretchBuffer(K15_2DSceneElementStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_Clear2DSceneElementStretchBuffer(K15_2DSceneElementStretchBuffer* p_StretchBuffer);

void K15_Push2DSceneElementStretchBufferElement(K15_2DSceneElementStretchBuffer* p_StretchBuffer, K15_2DSceneElement p_Element);
unsigned char K15_Pop2DSceneElementStretchBufferIndex(K15_2DSceneElementStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_Pop2DSceneElementStretchBufferElement(K15_2DSceneElementStretchBuffer* p_StretchBuffer, K15_2DSceneElement p_Element);
unsigned char K15_Pop2DSceneElementStretchBufferCompare(K15_2DSceneElementStretchBuffer* p_StretchBuffer, K15_2DSceneElement p_Element, K15_2DSceneElementCompareFnc p_CompareFnc);

K15_2DSceneElement* K15_Get2DSceneElementStretchBufferElementUnsafe(K15_2DSceneElementStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_2DSceneElement* K15_Get2DSceneElementStretchBufferElement(K15_2DSceneElementStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_2DSceneElement* K15_Get2DSceneElementStretchBufferElementConditional(K15_2DSceneElementStretchBuffer* p_StretchBuffer, K15_2DSceneElementConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_2DSceneElement_Stretch_Buffer_h_s