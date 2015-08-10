#include "K15_RenderCommandBuffer.h"
#include "K15_RenderCommand.h"

#include "K15_CustomMemoryAllocator.h"

/*********************************************************************************/
result8 K15_InitializeRenderCommandBuffer(K15_CustomMemoryAllocator* p_CustomMemoryAllocator, K15_RenderCommandBuffer* p_RenderCommandBuffer)
{
	byte* buffer = (byte*)K15_AllocateFromMemoryAllocator(p_CustomMemoryAllocator, K15_RENDERING_COMMAND_BUFFER_SIZE);

	if (!buffer)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	p_RenderCommandBuffer->buffer = buffer;
	p_RenderCommandBuffer->offset = 0;

	//clear buffer
	memset(p_RenderCommandBuffer->buffer, 0, K15_RENDERING_COMMAND_BUFFER_SIZE);

	return K15_SUCCESS;
}
/*********************************************************************************/
result8 K15_GetMemoryFromCommandBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_SizeInBytes, void** p_OutBytes)
{
	K15_ASSERT_TEXT(p_RenderCommandBuffer, "Render Command Buffer is NULL.");
	K15_ASSERT_TEXT(p_SizeInBytes > 0, "Requested Render Command Buffer memory size is 0.");
	K15_ASSERT_TEXT(p_OutBytes, "Pointer to command buffer memory is NULL.");

	uint32 offset = p_RenderCommandBuffer->offset;

	*p_OutBytes = 0;

	if (offset + p_SizeInBytes >= K15_RENDERING_COMMAND_BUFFER_SIZE)
	{
		return K15_ERROR_MAX_RENDER_COMMAND_BUFFER_FULL;
	}

	*p_OutBytes = p_RenderCommandBuffer->buffer + offset;
	p_RenderCommandBuffer->offset = offset + p_SizeInBytes;

	return K15_SUCCESS;
}
/*********************************************************************************/
void K15_ReadMemoryFromCommandBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_OffsetInBytes, uint32 p_SizeInBytes, void* p_DestinationMemory)
{
	K15_ASSERT_TEXT(p_RenderCommandBuffer, "Render Command Buffer is NULL.");
	K15_ASSERT_TEXT(p_OffsetInBytes + p_SizeInBytes < p_RenderCommandBuffer->offset, "Offset + Size is too large.");
	K15_ASSERT_TEXT(p_DestinationMemory, "Destination memory is NULL.");

	memcpy(p_DestinationMemory, p_RenderCommandBuffer->buffer + p_OffsetInBytes, p_SizeInBytes);
}
/*********************************************************************************/