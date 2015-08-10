#ifndef _K15_RenderCommandBuffer_h_
#define _K15_RenderCommandBuffer_h_

#include "K15_RenderPrerequisites.h"

#define K15_RENDERING_COMMAND_BUFFER_SIZE size_kilobyte(128)

struct K15_RenderCommandBuffer
{
	byte* buffer;
	uint32 offset;
};

result8 K15_InitializeRenderCommandBuffer(K15_CustomMemoryAllocator* p_CustomMemoryAllocator, K15_RenderCommandBuffer* p_RenderCommandBuffer);
result8 K15_GetMemoryFromCommandBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_SizeInBytes, void** p_OutBytes);
void K15_ReadMemoryFromCommandBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_OffsetInBytes, uint32 p_SizeInBytes, void* p_DestinationMemory);

#endif //_K15_RenderCommandBuffer_h_