/*********************************************************************************/
/*  THIS FILE HAS BEEN GENERATED AUTOMATICALLY. ANY CHANGES WILL BE OVERRIDDEN   */
/*********************************************************************************/

#ifndef _K15_SubMesh_Fixed_Buffer_h_
#define _K15_SubMesh_Fixed_Buffer_h_

#include "K15_CustomMemoryAllocator.h"

//forward declaration
struct K15_SubMesh;

//compare function
typedef unsigned char (*K15_SubMeshCompareFnc)(K15_SubMesh* p_LHS, K15_SubMesh* p_RHS);

//get condition function
typedef unsigned char (*K15_SubMeshConditionFnc)(K15_SubMesh* p_Element, void* p_UserData);

struct K15_SubMeshFixedBuffer
{
	K15_CustomMemoryAllocator  memoryAllocator;
	K15_SubMesh* elements;							

	unsigned int numCapacity;					
	unsigned int numElements;					
	unsigned int flags;							
};

void K15_CreateSubMeshFixedBufferWithCustomAllocator(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_CustomMemoryAllocator p_MemoryAllocator, unsigned int p_Capacity = 8);
void K15_CreateSubMeshFixedBuffer(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Capacity = 8);
void K15_DeleteSubMeshFixedBuffer(K15_SubMeshFixedBuffer* p_FixedBuffer);
void K15_ResizeSubMeshFixedBuffer(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Capacity);
void K15_ClearSubMeshFixedBuffer(K15_SubMeshFixedBuffer* p_FixedBuffer);

K15_SubMesh* K15_PushSubMeshFixedBufferElement(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_SubMesh p_Element);
unsigned char K15_PopSubMeshFixedBufferIndex(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Index);
unsigned char K15_PopSubMeshFixedBufferElement(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_SubMesh p_Element);
unsigned char K15_PopSubMeshFixedBufferCompare(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_SubMesh p_Element, K15_SubMeshCompareFnc p_CompareFnc);

K15_SubMesh* K15_GetSubMeshFixedBufferElementUnsafe(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Index);
K15_SubMesh* K15_GetSubMeshFixedBufferElement(K15_SubMeshFixedBuffer* p_FixedBuffer, unsigned int p_Index);
K15_SubMesh* K15_GetSubMeshFixedBufferElementConditional(K15_SubMeshFixedBuffer* p_FixedBuffer, K15_SubMeshConditionFnc p_ConditionFnc, void* p_UserData = 0);

#endif //_K15_SubMesh_Stretch_Buffer_h_s