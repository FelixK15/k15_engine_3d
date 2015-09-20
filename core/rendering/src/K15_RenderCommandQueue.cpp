#include "K15_RenderCommandQueue.h"
#include "K15_RenderCommandBuffer.h"

/*********************************************************************************/
void K15_SwapRenderCommandQueueBuffer(K15_RenderCommandQueue* p_RenderCommandQueue)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	
	K15_WaitSemaphore(p_RenderCommandQueue->processLock);

	K15_RenderCommandBuffer tempCommandBufferPointer = {};
	K15_RenderCommandBuffer* backCommandBufferPointer = &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];
	K15_RenderCommandBuffer* frontCommandBufferPointer = &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];

	tempCommandBufferPointer = *backCommandBufferPointer;
	*backCommandBufferPointer = *frontCommandBufferPointer;
	*frontCommandBufferPointer = tempCommandBufferPointer;

	//reset backbuffer
	backCommandBufferPointer->offset = 0;
}
/*********************************************************************************/
result8 K15_InitializeRenderCommandQueue(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderCommandQueue* p_RenderCommandQueue)
{
	//initialize buffers (front and back)
	result8 resultBackBuffer = K15_InitializeRenderCommandBuffer(p_MemoryAllocator, &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX]);
	result8 resultFrontBuffer = K15_InitializeRenderCommandBuffer(p_MemoryAllocator, &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX]);

	p_RenderCommandQueue->lastCommand = 0;
	p_RenderCommandQueue->name = 0;
	p_RenderCommandQueue->processLock = K15_CreateSemaphoreWithInitialValue(1);

	if (resultBackBuffer != K15_SUCCESS || 
		resultFrontBuffer != K15_SUCCESS)
	{
		return resultFrontBuffer == K15_SUCCESS ? resultBackBuffer : resultFrontBuffer;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
result8 K15_BeginRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderCommandType p_RenderCommandType)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	/*K15_ASSERT_TEXT(p_RenderCommandQueue)*/

	if (p_RenderCommandQueue->lastCommand)
	{
		return K15_ERROR_LAST_COMMAND_NOT_FINISHED;
	}

	K15_RenderCommandBuffer* commandBackBuffer = p_RenderCommandQueue->commandBuffers + K15_RENDERING_COMMAND_BACK_BUFFER_INDEX;
	uint32 offset = commandBackBuffer->offset;

	K15_RenderCommand* renderCommand = 0;

	result8 result = K15_GetMemoryFromCommandBuffer(commandBackBuffer, sizeof(K15_RenderCommand), (void**)&renderCommand);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	renderCommand->type = p_RenderCommandType;
	renderCommand->parameterSize = 0;

	p_RenderCommandQueue->lastCommand = renderCommand;

	return K15_SUCCESS;
}
/*********************************************************************************/
result8 K15_EndRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");

	if (!p_RenderCommandQueue->lastCommand)
	{
		return K15_ERROR_NO_RENDER_COMMAND;
	}

	p_RenderCommandQueue->lastCommand = 0;

	return K15_SUCCESS;
}
/*********************************************************************************/
result8 K15_AddRenderCommandParameter(K15_RenderCommandQueue* p_RenderCommandQueue, uint32 p_ParameterSizeInBytes, void* p_Parameter)
{
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render Command Queue is NULL.");
	K15_ASSERT_TEXT(p_ParameterSizeInBytes > 0, "Parameter byte size is 0.");
	K15_ASSERT_TEXT(p_Parameter, "Parameter is NULL.");

	if (!p_RenderCommandQueue->lastCommand)
	{
		return K15_ERROR_NO_RENDER_COMMAND;
	}

	byte* commandBufferMemory = 0;
	result8 result = K15_GetMemoryFromCommandBuffer(p_RenderCommandQueue->commandBuffers + K15_RENDERING_COMMAND_BACK_BUFFER_INDEX, p_ParameterSizeInBytes, (void**)&commandBufferMemory);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	memcpy(commandBufferMemory, p_Parameter, p_ParameterSizeInBytes);

	//add parameter size to the last command
	p_RenderCommandQueue->lastCommand->parameterSize += p_ParameterSizeInBytes;

	return K15_SUCCESS;
}
/*********************************************************************************/