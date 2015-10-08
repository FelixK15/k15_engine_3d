/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_RenderMaterialDataDesc_Stretch_Buffer_h_
#define _K15_RenderMaterialDataDesc_Stretch_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_RenderMaterialDataDesc;

//compare function
typedef unsigned char (*K15_RenderMaterialDataDescCompareFnc)(K15_RenderMaterialDataDesc* p_LHS, K15_RenderMaterialDataDesc* p_RHS);

//get condition function
typedef unsigned char (*K15_RenderMaterialDataDescConditionFnc)(K15_RenderMaterialDataDesc* p_Element, void* p_UserData);

struct K15_RenderMaterialDataDescStretchBuffer
{
	K15_CustomMemoryAllocator memoryAllocator;
	K15_RenderMaterialDataDesc* elements;

	unsigned int numCapacity;
	unsigned int numElements;
	unsigned int flags;
};

void K15_CreateRenderMaterialDataDescStretchBufferWithCustomAllocator(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_ElementCapacity = 8);
void K15_CreateRenderMaterialDataDescStretchBuffer(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity = 8);
void K15_DeleteRenderMaterialDataDescStretchBuffer(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer);
void K15_ResizeRenderMaterialDataDescStretchBuffer(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, unsigned int p_ElementCapacity);
void K15_ClearRenderMaterialDataDescStretchBuffer(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer);

K15_RenderMaterialDataDesc* K15_PushRenderMaterialDataDescStretchBufferElement(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, K15_RenderMaterialDataDesc p_Element);
unsigned char K15_PopRenderMaterialDataDescStretchBufferIndex(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, unsigned int p_Index);
unsigned char K15_PopRenderMaterialDataDescStretchBufferElement(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, K15_RenderMaterialDataDesc p_Element);
unsigned char K15_PopRenderMaterialDataDescStretchBufferCompare(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, K15_RenderMaterialDataDesc p_Element, K15_RenderMaterialDataDescCompareFnc p_CompareFnc);

K15_RenderMaterialDataDesc* K15_GetRenderMaterialDataDescStretchBufferElementUnsafe(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_RenderMaterialDataDesc* K15_GetRenderMaterialDataDescStretchBufferElement(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, unsigned int p_Index);
K15_RenderMaterialDataDesc* K15_GetRenderMaterialDataDescStretchBufferElementConditional(K15_RenderMaterialDataDescStretchBuffer* p_StretchBuffer, K15_RenderMaterialDataDescConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_RenderMaterialDataDesc_Stretch_Buffer_h_s