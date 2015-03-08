#include "K15_RenderContext.h"

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_ErrorCodes.h>
#include <K15_OSLayer_Thread.h>

#include "GL/K15_RenderGLContext.h"

#include "GL/K15_RenderGLBuffer.cpp"

/*********************************************************************************/
intern uint8 K15_InternalProcessRenderCommandQueue(K15_RenderContext* p_RenderContext, K15_RenderCommandQueue* p_RenderCommandQueue)
{
	assert(p_RenderCommandQueue);

	K15_RenderCommandInstance* currentCommand = 0;
	uint8 result = K15_SUCCESS;

	K15_RenderCommandParameterBuffer** parameterFrontBuffer = &p_RenderCommandQueue->parameterBuffer[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	K15_RenderCommandBuffer** commandFrontBuffer = &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];

	//process front buffer
	for(uint32 renderCommandIndex = 0;
		renderCommandIndex < (*commandFrontBuffer)->amountCommands;
		++renderCommandIndex)
	{
		currentCommand = &(*commandFrontBuffer)->commandBuffer[renderCommandIndex];

		result = p_RenderContext->processRenderCommand(p_RenderContext, p_RenderCommandQueue, currentCommand);

		assert(result != K15_SUCCESS);
	}

	//command queue has been processed. Remove dispatched flag
	p_RenderCommandQueue->flags &= ~K15_CBF_DISPATCHED;

	return result;
}
/*********************************************************************************/
intern uint8 K15_InternalAddCommandQueueParameter(K15_RenderCommandQueue* p_RenderCommandQueue, uint32 p_ParameterSize, void* p_Parameter)
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
intern void K15_InternalSwapRenderCommandQueueBuffers(K15_RenderCommandQueue* p_RenderCommandQueue)
{
	K15_RenderCommandBuffer* tempBuffer = 0;
	K15_RenderCommandParameterBuffer* tempParameterBuffer = 0;

	K15_RenderCommandParameterBuffer** parameterFrontBuffer = &p_RenderCommandQueue->parameterBuffer[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	K15_RenderCommandParameterBuffer** parameterBackBuffer = &p_RenderCommandQueue->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];
	K15_RenderCommandBuffer** commandFrontBuffer = &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	K15_RenderCommandBuffer** commandBackBuffer = &p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];

	//swap command buffers
	tempBuffer = *commandFrontBuffer;
	*commandFrontBuffer = *commandBackBuffer;
	*commandBackBuffer = tempBuffer;

	//swap parameter buffers
	tempParameterBuffer = *parameterFrontBuffer;
	*parameterFrontBuffer = *parameterBackBuffer;
	*parameterBackBuffer = tempParameterBuffer;

	//commands and params have been processed
	(*parameterBackBuffer)->parameterBufferOffset = 0;
	(*commandBackBuffer)->amountCommands = 0;
}
/*********************************************************************************/
intern void K15_InternalSwapRenderDispatcherBuffers(K15_RenderCommandQueueDispatcher* p_RenderCommandQueueDispatcher)
{
	//render thread tries to get the swap mutex lock
	K15_LockMutex(p_RenderCommandQueueDispatcher->swapMutex);

	uint32 tempCommandQueueToProcessCounter = 0;
	uint32* frontCommandQueueToProcessCounter = &p_RenderCommandQueueDispatcher->amountCommandQueuesToProcess[K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX];
	uint32* backCommandQueueToProcessCounter = &p_RenderCommandQueueDispatcher->amountCommandQueuesToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX];

	K15_RenderCommandQueue** tempCommandQueuesToProcessArray = 0;
	K15_RenderCommandQueue*** frontCommandQueueProcessBuffer = &p_RenderCommandQueueDispatcher->renderCommandQueuesToProcess[K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX];
	K15_RenderCommandQueue*** backCommandQueueProcessBuffer = &p_RenderCommandQueueDispatcher->renderCommandQueuesToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX];

	//swap dispatch buffers
	tempCommandQueuesToProcessArray = *frontCommandQueueProcessBuffer;
	*frontCommandQueueProcessBuffer = *backCommandQueueProcessBuffer;
	*backCommandQueueProcessBuffer = tempCommandQueuesToProcessArray;

	tempCommandQueueToProcessCounter = *frontCommandQueueToProcessCounter;
	*frontCommandQueueToProcessCounter = *backCommandQueueToProcessCounter;
	*backCommandQueueToProcessCounter = tempCommandQueueToProcessCounter;

	*backCommandQueueToProcessCounter = 0;

	//render thread unlocks the swap mutex
	K15_UnlockMutex(p_RenderCommandQueueDispatcher->swapMutex);
}
/*********************************************************************************/


/*********************************************************************************/
intern uint8 K15_InternalRenderThreadFunction(void* p_Parameter)
{
	//extract parameter from buffer
	K15_OSLayerContext* osContext = 0;
	K15_RenderContext* renderContext = 0;
	byte* parameterBuffer = (byte*)p_Parameter;

	memcpy(&osContext, parameterBuffer, K15_PTR_SIZE);
	memcpy(&renderContext, parameterBuffer + K15_PTR_SIZE, K15_PTR_SIZE);
	
	//we don't need that anymore
	free(parameterBuffer);

	//create actual context
	uint8 createGLRenderContext = TRUE;

	//TODO: DX 11/10/9

	if (createGLRenderContext == TRUE)
	{
		uint8 result = K15_GLCreateRenderContext(renderContext, osContext);

		if (result != K15_SUCCESS)
		{
			return result;
		}
	}

	//set render context initialized flag
	renderContext->flags |= K15_RCF_INITIALIZED;

	while(true)
	{
		//wait for signal...
		K15_WaitSemaphore(renderContext->renderThreadSync);

		//swap dispatching buffer so the threads that are dispatching command queues can immediately start to fill the back buffer again
		K15_InternalSwapRenderDispatcherBuffers(renderContext->commandQueueDispatcher);

		int renderCommandQueuesToProcess = renderContext->commandQueueDispatcher->amountCommandQueuesToProcess[K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX];

		for (int renderCommandQueueIndex = 0;
			renderCommandQueueIndex < renderCommandQueuesToProcess;
			++renderCommandQueueIndex)
		{
			K15_RenderCommandQueue** renderCommandQueuesToProcess = renderContext->commandQueueDispatcher->renderCommandQueuesToProcess[K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX];
			K15_InternalProcessRenderCommandQueue(renderContext, renderCommandQueuesToProcess[renderCommandQueueIndex]);
		}
	}


	return K15_SUCCESS;
}
/*********************************************************************************/




/*********************************************************************************/
K15_RenderContext* K15_CreateRenderContext(K15_OSLayerContext* p_OSContext)
{
	assert(p_OSContext);

	K15_RenderContext* renderContext = (K15_RenderContext*)malloc(sizeof(K15_RenderContext));

	if (!renderContext)
	{
		return 0;
	}

	//create command queue dispatcher
	K15_RenderCommandQueueDispatcher* renderCommandQueueDispatcher = (K15_RenderCommandQueueDispatcher*)malloc(sizeof(K15_RenderCommandQueueDispatcher));

	if (!renderCommandQueueDispatcher)
	{
		return 0;
	}
	
	//create double dispatch buffer
	for (uint32 renderCommandQueueDispatchBufferIndex = 0;
		 renderCommandQueueDispatchBufferIndex < K15_RENDERING_COMMAND_DISPATCH_BUFFER_COUNT;
		 ++renderCommandQueueDispatchBufferIndex)
	{
		renderCommandQueueDispatcher->renderCommandQueuesToProcess[renderCommandQueueDispatchBufferIndex] = (K15_RenderCommandQueue**)malloc(K15_PTR_SIZE * K15_RENDERING_MAX_COMMAND_QUEUES_TO_PROCESS);
		renderCommandQueueDispatcher->amountCommandQueuesToProcess[renderCommandQueueDispatchBufferIndex] = 0;
	}

	renderCommandQueueDispatcher->swapMutex = K15_CreateMutex();

	//create command queues
	K15_RenderCommandQueue* renderCommandQueues = (K15_RenderCommandQueue*)malloc(sizeof(K15_RenderCommandQueue) * K15_RENDERING_MAX_COMMAND_QUEUES);

	if (!renderCommandQueues)
	{
		return 0;
	}

	renderContext->flags = 0;
	renderContext->commandQueues = renderCommandQueues;
	renderContext->amountCommandQueues = 0;
	renderContext->userData = 0;
	renderContext->processRenderCommand = 0;
	renderContext->commandQueueDispatcher = renderCommandQueueDispatcher;
	renderContext->createCommandQueueMutex = K15_CreateMutex();
	renderContext->renderThreadSync = K15_CreateSemaphore(0);
	renderContext->debugging.assignedThread = K15_GetCurrentThread();

	// allocate memory for 2 pointer
	byte* threadParameterBuffer = (byte*)malloc(K15_PTR_SIZE * 2);

	//1. parameter : os context
	memcpy(threadParameterBuffer, &p_OSContext, K15_PTR_SIZE);

	//2. parameter : render context
	memcpy(threadParameterBuffer + K15_PTR_SIZE, &renderContext, K15_PTR_SIZE);

	//create render thread
	K15_Thread* renderThread = K15_CreateThread(K15_InternalRenderThreadFunction, (void*)threadParameterBuffer);
	K15_SetThreadName(renderThread, "K15_RenderThread");

	if (renderThread)
	{
		renderContext->renderThread = renderThread;
	}

	return renderContext;
}
/*********************************************************************************/
K15_RenderCommandQueue* K15_CreateRenderCommandQueue(K15_RenderContext* p_RenderContext)
{
	//only continue when the rendercontext has been initialized
	uint32 renderContextFlags = p_RenderContext->flags;

	while ((renderContextFlags & K15_RCF_INITIALIZED) > 0); //busy wait until render context is initialized

	assert(p_RenderContext->amountCommandQueues < K15_RENDERING_MAX_COMMAND_QUEUES);
	
	if (p_RenderContext->amountCommandQueues >= K15_RENDERING_MAX_COMMAND_QUEUES)
	{
		return 0;
	}

	//lock mutex so this thread is currently the only one that can create command queues
	K15_LockMutex(p_RenderContext->createCommandQueueMutex);

	uint32 commandQueueIndex = p_RenderContext->amountCommandQueues;

	K15_RenderCommandQueue* renderCommandQueue = &p_RenderContext->commandQueues[commandQueueIndex];

	for (commandQueueIndex = 0;
		 commandQueueIndex < K15_RENDERING_COMMAND_BUFFER_COUNT;
		 ++commandQueueIndex)
	{
		K15_RenderCommandInstance* commandBuffer = (K15_RenderCommandInstance*)malloc(sizeof(K15_RenderCommandInstance) * K15_RENDERING_MAX_COMMANDS);
		byte* parameterBuffer = (byte*)malloc(K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE);

		renderCommandQueue->commandBuffers[commandQueueIndex] = (K15_RenderCommandBuffer*)malloc(sizeof(K15_RenderCommandBuffer));
		renderCommandQueue->parameterBuffer[commandQueueIndex] = (K15_RenderCommandParameterBuffer*)malloc(sizeof(K15_RenderCommandParameterBuffer));

		if (!renderCommandQueue->commandBuffers[commandQueueIndex]
			|| !renderCommandQueue->parameterBuffer[commandQueueIndex]
			|| !commandBuffer
			|| !parameterBuffer)
		{
			return 0;
		}
		
		//initialize command buffer data
		renderCommandQueue->commandBuffers[commandQueueIndex]->commandBuffer = commandBuffer;
		renderCommandQueue->commandBuffers[commandQueueIndex]->amountCommands = 0;
		renderCommandQueue->commandBuffers[commandQueueIndex]->flags = 0;

		//initialize parameter buffer data
		renderCommandQueue->parameterBuffer[commandQueueIndex]->parameterBuffer = parameterBuffer;
		renderCommandQueue->parameterBuffer[commandQueueIndex]->parameterBufferOffset = 0;
	}

	renderCommandQueue->flags = 0;
	renderCommandQueue->renderContext = p_RenderContext;
	renderCommandQueue->processingMutex = K15_CreateMutex();
	renderCommandQueue->lastCommand = 0;

#ifdef K15_DEBUG
	renderCommandQueue->debugging.assignedThread = K15_GetCurrentThread();
#endif //K15_DEBUG
	//renderCommandQueue->swapSection = K15_CreateNamedMutex("RenderCommandQueue Swap");
	
	//increasing the command queue count should be the last operation
	++p_RenderContext->amountCommandQueues;

	//unlock mutex so another thread can start creating command queues
	K15_UnlockMutex(p_RenderContext->createCommandQueueMutex);

	return renderCommandQueue;
}
/*********************************************************************************/
uint8 K15_BeginRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderCommand p_RenderCommand)
{
	assert(p_RenderCommandQueue);

	while((p_RenderCommandQueue->flags & K15_CBF_SWAPPING) > 0); //busy wait in case the render command queue is current getting swapped

	assert(p_RenderCommandQueue->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX]->amountCommands < K15_RENDERING_MAX_COMMANDS);
	assert(p_RenderCommandQueue->debugging.assignedThread == K15_GetCurrentThread());
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
void K15_DispatchRenderCommandQueue(K15_RenderCommandQueue* p_RenderCommandQueue)
{
	assert(p_RenderCommandQueue);
	assert(!p_RenderCommandQueue->lastCommand);

	K15_RenderContext* renderContext = p_RenderCommandQueue->renderContext;
	K15_RenderCommandQueueDispatcher* renderDispatcher = renderContext->commandQueueDispatcher;
	
	while((p_RenderCommandQueue->flags & K15_CBF_DISPATCHED) > 0); //busy wait until the command queue has been processed

	//the caller thread tries to get the swap mutex so the render thread can not swap the dispatcher 
	//when we try to add a command queue
	K15_LockMutex(renderDispatcher->swapMutex);

	int commandQueuesToProcessIndex = renderContext->commandQueueDispatcher->amountCommandQueuesToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX];

	assert(commandQueuesToProcessIndex != K15_RENDERING_MAX_COMMAND_QUEUES_TO_PROCESS);

	//add render queue to the dispatcher
	renderContext->commandQueueDispatcher->renderCommandQueuesToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX][commandQueuesToProcessIndex++] = p_RenderCommandQueue;
	renderContext->commandQueueDispatcher->amountCommandQueuesToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX] = commandQueuesToProcessIndex;

	//swap render command queue so we can immediately start pumping render commands again
	K15_InternalSwapRenderCommandQueueBuffers(p_RenderCommandQueue);

	//unclock swap mutex so the renderthread can swap the dispatcher.
	K15_UnlockMutex(renderDispatcher->swapMutex);

	//set the dispatched flag to communicate that this command queue has been dispatched. (dispatched flag will get removed once the command queue has been processed);
	p_RenderCommandQueue->flags |= K15_CBF_DISPATCHED;
}
/*********************************************************************************/
void K15_ProcessDispatchedRenderCommandQueues(K15_RenderContext* p_RenderContext)
{
	assert(p_RenderContext);

	K15_PostSemaphore(p_RenderContext->renderThreadSync);
}
/*********************************************************************************/