/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_GUIElement_Stretch_Buffer_h_
#define _K15_GUIElement_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_GUIElement;

//compare function
typedef unsigned char (*K15_GUIElementCompareFnc)(K15_GUIElement** p_LHS, K15_GUIElement** p_RHS);

//get condition function
typedef unsigned char (*K15_GUIElementConditionFnc)(K15_GUIElement** p_Element, void* p_UserData);

struct K15_GUIElementStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_GUIElement** elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateGUIElementStretchBufferWithCustomAllocator(K15_GUIElementStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity = 8);
void K15_CreateGUIElementStretchBuffer(K15_GUIElementStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteGUIElementStretchBuffer(K15_GUIElementStretchBuffer* p_StretchBuffer);
void K15_ResizeGUIElementStretchBuffer(K15_GUIElementStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearGUIElementStretchBuffer(K15_GUIElementStretchBuffer* p_StretchBuffer);

K15_GUIElement** K15_PushGUIElementStretchBufferElement(K15_GUIElementStretchBuffer* p_StretchBuffer, K15_GUIElement* p_Element);
unsigned char K15_PopGUIElementStretchBufferIndex(K15_GUIElementStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopGUIElementStretchBufferElement(K15_GUIElementStretchBuffer* p_StretchBuffer, K15_GUIElement* p_Element);
unsigned char K15_PopGUIElementStretchBufferCompare(K15_GUIElementStretchBuffer* p_StretchBuffer, K15_GUIElement* p_Element, K15_GUIElementCompareFnc p_CompareFnc);

K15_GUIElement** K15_GetGUIElementStretchBufferElementUnsafe(K15_GUIElementStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_GUIElement** K15_GetGUIElementStretchBufferElement(K15_GUIElementStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_GUIElement** K15_GetGUIElementStretchBufferElementConditional(K15_GUIElementStretchBuffer* p_StretchBuffer, K15_GUIElementConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_GUIElement_Stretch_Buffer_h_s