#include "K15_RenderContext.h"

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_ErrorCodes.h>

#include "GL/K15_RenderGLContext.h"

#include "GL/K15_RenderGLBuffer.cpp"

/*********************************************************************************/
static uint8 K15_InternalAddCommandQueueParameter(K15_RenderCommandQueue* p_RenderCommandQueue, uint32 p_ParameterSize, void* p_Parameter)
{
	assert(p_RenderCommandQueue && p_Parameter);
	assert(p_ParameterSize > 0);

	K15_RenderCommandInstance* lastCommand = p_RenderCommandQueue->lastCommand;
	K15_RenderCommandParameterBuffer* commandParameterBackBuffer = p_RenderCommandQueue->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];
	
	if (!lastCommand)
	{
		return K15_ERROR_NO_RENDER_COMMAND;
	}

	uint32 currentParameterBufferOffset = commandParameterBackBuffer->parameterBufferOffset;
	uint32 newParameterBufferOffset = currentParameterBufferOffset + p_ParameterSize;

	if (newParameterBufferOffset >= K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE)
	{
		return K15_ERROR_MAX_COMMAND_PARAMETER_SIZE_BUFFER_REACHED;
	}

	memcpy(commandParameterBackBuffer->parameterBuffer + currentParameterBufferOffset, p_Parameter, p_ParameterSize);

	lastCommand->parameterSize = newParameterBufferOffset;

	commandParameterBackBuffer->parameterBufferOffset = newParameterBufferOffset;

	return K15_SUCCESS;
}
/*********************************************************************************/


/*********************************************************************************/
uint8 K15_ProcessRenderCommandQueue(K15_RenderContext* p_RenderContext, K15_RenderCommandQueue* p_RenderCommandQueue)
{
	assert(p_RenderCommandQueue);

	K15_RenderCommandInstance* currentCommand = 0;
	uint8 result = K15_SUCCESS;

	K15_RenderCommandBuffer* tempBuffer = 0;
	K15_RenderCommandParameterBuffer* tempParameterBuffer = 0;

	K15_RenderCommandParameterBuffer** parameterFrontBuffer = &p_RenderCommandQueue->parameterBuffer[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	K15_RenderCommandParameterBuffer** parameterBackBuffer = &p_RenderCommandQueue->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];
	K15_RenderCommandBuffer** commandFrontBuffer = &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	K15_RenderCommandBuffer** commandBackBuffer = &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];

	// check if current front buffer has been processed
	if (((*commandBackBuffer)->flags & K15_CBF_LOCKED) > 0)
	{
		// dont wait until buffer is unlocked. Try again later
		return K15_ERROR_RENDER_BUFFER_LOCKED;
	}

	//swap command buffers
	tempBuffer = *commandFrontBuffer;
	*commandFrontBuffer = *commandBackBuffer;
	*commandBackBuffer = tempBuffer;

	//swap parameter buffers
	tempParameterBuffer = *parameterFrontBuffer;
	*parameterFrontBuffer = *parameterBackBuffer;
	*parameterBackBuffer = tempParameterBuffer;

	//process front buffer
	for(uint32 renderCommandIndex = 0;
		renderCommandIndex < (*commandFrontBuffer)->amountCommands;
		++renderCommandIndex)
	{
		currentCommand = &(*commandFrontBuffer)->commandBuffer[renderCommandIndex];

		result = p_RenderContext->processRenderCommand(p_RenderContext, p_RenderCommandQueue, currentCommand);
	
		if (result != K15_SUCCESS)
		{
			break;
		}
	}

	//commands and params have been processed
	(*parameterBackBuffer)->parameterBufferOffset = 0;
	(*commandBackBuffer)->amountCommands = 0;

	//lock back buffer
	(*commandBackBuffer)->flags |= K15_CBF_LOCKED;

	return result;
}
/*********************************************************************************/
uint8 K15_CreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSContext)
{
	assert(p_RenderContext && p_OSContext);

	K15_RenderContext* renderContext = (K15_RenderContext*)malloc(sizeof(K15_RenderContext));

	if (!renderContext)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	//create command queues
	K15_RenderCommandQueue* renderCommandQueues = (K15_RenderCommandQueue*)malloc(sizeof(K15_RenderCommandQueue) * K15_RENDERING_MAX_COMMAND_QUEUES);
	
	if (!renderCommandQueues)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	renderContext->commandQueues = renderCommandQueues;
	renderContext->amountCommandQueues = 0;
	renderContext->userData = 0;
	renderContext->processRenderCommand = 0;

	uint8 createGLRenderContext = TRUE;

	//TODO: DX 11/10/9

	if (createGLRenderContext == TRUE)
	{
		uint8 result = K15_GLCreateRenderContext(renderContext, p_OSContext);
		
		if (result != K15_SUCCESS)
		{
			return result;
		}
	}

	*p_RenderContext = *renderContext;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_CreateRenderCommandQueue(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderContext* p_RenderContext)
{
	assert(p_RenderCommandQueue && p_RenderContext);
	assert(p_RenderContext->amountCommandQueues < K15_RENDERING_MAX_COMMAND_QUEUES);
	
	uint8 result = K15_SUCCESS;

	if (p_RenderContext->amountCommandQueues >= K15_RENDERING_MAX_COMMAND_QUEUES)
	{
		return K15_ERROR_MAX_RENDER_COMMAND_QUEUE_REACHED;
	}

	uint32 commandQueueIndex = p_RenderContext->amountCommandQueues++;

	*p_RenderCommandQueue = p_RenderContext->commandQueues[commandQueueIndex];

	for (commandQueueIndex = 0;
		 commandQueueIndex < K15_RENDERING_COMMAND_BUFFER_COUNT;
		 ++commandQueueIndex)
	{
		K15_RenderCommandInstance* commandBuffer = (K15_RenderCommandInstance*)malloc(sizeof(K15_RenderCommandInstance) * K15_RENDERING_MAX_COMMANDS);
		byte* parameterBuffer = (byte*)malloc(K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE);

		p_RenderCommandQueue->commandBuffers[commandQueueIndex] = (K15_RenderCommandBuffer*)malloc(sizeof(K15_RenderCommandBuffer));
		p_RenderCommandQueue->parameterBuffer[commandQueueIndex] = (K15_RenderCommandParameterBuffer*)malloc(sizeof(K15_RenderCommandParameterBuffer));
		if (!p_RenderCommandQueue->commandBuffers[commandQueueIndex]
			|| !p_RenderCommandQueue->parameterBuffer[commandQueueIndex]
			|| !commandBuffer
			|| !parameterBuffer)
		{
			result = K15_ERROR_OUT_OF_MEMORY;
			break;
		}
		
		//initialize command buffer data
		p_RenderCommandQueue->commandBuffers[commandQueueIndex]->commandBuffer = commandBuffer;
		p_RenderCommandQueue->commandBuffers[commandQueueIndex]->amountCommands = 0;
		p_RenderCommandQueue->commandBuffers[commandQueueIndex]->flags = 0;

		//initialize parameter buffer data
		p_RenderCommandQueue->parameterBuffer[commandQueueIndex]->parameterBuffer = parameterBuffer;
		p_RenderCommandQueue->parameterBuffer[commandQueueIndex]->parameterBufferOffset = 0;

		p_RenderCommandQueue->lastCommand = 0;
	}

	//lock back buffer
	p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX]->flags |= K15_CBF_LOCKED;
	
	return result;
}
/*********************************************************************************/
uint8 K15_BeginRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderCommand p_RenderCommand)
{
	assert(p_RenderCommandQueue);
	assert(p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX]->amountCommands < K15_RENDERING_MAX_COMMANDS);
	assert(!p_RenderCommandQueue->lastCommand);

	K15_RenderCommandBuffer* commandBackBuffer = p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];
	K15_RenderCommandParameterBuffer* commandParameterBackBuffer = p_RenderCommandQueue->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];

	if (commandBackBuffer->amountCommands == K15_RENDERING_MAX_COMMANDS)
	{
		return K15_ERROR_MAX_RENDER_COMMANDS_REACHED;
	}

	if (p_RenderCommandQueue->lastCommand)
	{
		return K15_ERROR_LAST_COMMAND_NOT_FINISHED;
	}

	uint32 renderCommandIndex = commandBackBuffer->amountCommands++;

	K15_RenderCommandInstance* renderCommandInstance = &commandBackBuffer->commandBuffer[renderCommandIndex];
	p_RenderCommandQueue->lastCommand = renderCommandInstance;

	renderCommandInstance->type = p_RenderCommand;
	renderCommandInstance->parameterOffset = commandParameterBackBuffer->parameterBufferOffset;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_EndRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue)
{
	assert(p_RenderCommandQueue);

	K15_RenderCommandInstance* lastCommand = p_RenderCommandQueue->lastCommand;
	K15_RenderCommandParameterBuffer* commandParameterBackBuffer = p_RenderCommandQueue->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];

	if (!lastCommand)
	{
		return K15_ERROR_NO_RENDER_COMMAND;
	}

	lastCommand->parameterSize = (commandParameterBackBuffer->parameterBufferOffset - lastCommand->parameterOffset);

	p_RenderCommandQueue->lastCommand = 0;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AddRenderUInt32Parameter(K15_RenderCommandQueue* p_RenderCommandQueue, uint32* p_Parameter)
{
	uint8 result = K15_InternalAddCommandQueueParameter(p_RenderCommandQueue, 
														sizeof(uint32), 
														p_Parameter);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderInt32Parameter(K15_RenderCommandQueue* p_RenderCommandQueue, int32* p_Parameter)
{
	uint8 result = K15_InternalAddCommandQueueParameter(p_RenderCommandQueue, 
														sizeof(int32), 
														p_Parameter);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderBufferHandleParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_GpuBufferHandle* p_GpuBufferHandle)
{
	uint8 result = K15_InternalAddCommandQueueParameter(p_RenderCommandQueue, 
														sizeof(ptrdiff_t), 
														&p_GpuBufferHandle);

	*p_GpuBufferHandle = K15_INVALID_GPU_HANDLE;

	return result;
}
/*********************************************************************************/
void K15_UnlockRenderCommandQueue(K15_RenderCommandQueue* p_RenderCommandQueue)
{
	assert(p_RenderCommandQueue);

	p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX]->flags &= ~K15_CBF_LOCKED;
}
/*********************************************************************************/