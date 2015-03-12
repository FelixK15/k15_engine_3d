#include "K15_RenderContext.h"

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_ErrorCodes.h>
#include <K15_OSLayer_Thread.h>

#include "K15_RenderBufferDesc.h"
#include "K15_RenderProgramDesc.h"

#include "OpenGL/K15_RenderGLContext.h"

/*********************************************************************************/
intern inline K15_RenderProgramHandle K15_InternalAddRenderProgramDesc(K15_RenderContext* p_RenderContext, K15_RenderProgramDesc* p_RenderProgramDesc)
{
	//get current program index
	uint32 gpuProgramIndex = p_RenderContext->gpuProgram.amountPrograms++;

	assert(gpuProgramIndex < K15_RENDER_MAX_GPU_PROGRAMS);

	p_RenderContext->gpuProgram.programs[gpuProgramIndex] = *p_RenderProgramDesc;

	return (K15_RenderProgramHandle)gpuProgramIndex;
}
/*********************************************************************************/
intern inline K15_RenderBufferHandle K15_InternalAddRenderBufferDesc(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc)
{
	//get current buffer index and assign amount
	uint32 gpuBufferIndex = p_RenderContext->gpuBuffer.amountBuffers++;

	assert(gpuBufferIndex < K15_RENDER_MAX_GPU_BUFFER);

	p_RenderContext->gpuBuffer.buffers[gpuBufferIndex] = *p_RenderBufferDesc;

	return (K15_RenderBufferHandle)gpuBufferIndex;
}
/*********************************************************************************/
intern inline uint8 K15_InternalReadParameter(K15_RenderCommandParameterBuffer* p_ParameterFrontBuffer, K15_RenderCommandInstance* p_RenderCommand, uint32 p_ParameterSize, uint32 p_ParameterOffset, void* p_ParameterDestiny)
{
	assert(p_ParameterFrontBuffer);	
	assert(p_ParameterDestiny);		
	assert(p_ParameterOffset + p_ParameterSize <= p_ParameterFrontBuffer->parameterBufferOffset);

	uint32 offset = p_ParameterOffset + p_RenderCommand->parameterOffset;

	memcpy(p_ParameterDestiny, p_ParameterFrontBuffer->parameterBuffer + offset, p_ParameterSize);

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_InternalProcessRenderCommand(K15_RenderContext* p_RenderContext, K15_RenderCommandParameterBuffer* p_ParameterFrontBuffer, K15_RenderCommandInstance* p_RenderCommand)
{
	uint8 result = K15_SUCCESS;

	switch(p_RenderCommand->type)
	{
		case K15_RENDER_COMMAND_CLEAR_SCREEN:
		{
			result = p_RenderContext->commandProcessing.screenManagement.clearScreen(p_RenderContext);
			break;
		}

		case K15_RENDER_COMMAND_CREATE_BUFFER:
		{
			assert(p_RenderCommand->parameterSize == sizeof(K15_RenderBufferDesc) + K15_PTR_SIZE);
			
			K15_RenderBufferDesc renderBufferDesc = {0};
			K15_RenderBufferHandle* renderBufferHandle = 0;
				
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderBufferHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderBufferDesc), K15_PTR_SIZE, &renderBufferDesc);

			*renderBufferHandle = K15_InternalAddRenderBufferDesc(p_RenderContext, &renderBufferDesc);

			result = p_RenderContext->commandProcessing.bufferManagement.createBuffer(p_RenderContext, &renderBufferDesc, renderBufferHandle);

			break;
		}

		case K15_RENDER_COMMAND_UPDATE_BUFFER:
		{
			K15_RenderBufferUpdateDesc renderBufferUpdateDesc = {0};
			K15_RenderBufferHandle* renderBufferHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderBufferHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderBufferUpdateDesc), K15_PTR_SIZE, &renderBufferUpdateDesc);

			result = p_RenderContext->commandProcessing.bufferManagement.updateBuffer(p_RenderContext, &renderBufferUpdateDesc, renderBufferHandle);

			break;
		}

		case K15_RENDER_COMMAND_DELETE_BUFFER:
		{
			K15_RenderBufferHandle* renderBufferHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderBufferHandle);

			result = p_RenderContext->commandProcessing.bufferManagement.deleteBuffer(p_RenderContext, renderBufferHandle);
			break;
		}

		case K15_RENDER_COMMAND_CREATE_PROGRAM:
		{
			K15_RenderProgramDesc renderProgramDesc = {0};
			K15_RenderProgramHandle* renderProgramHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderProgramHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderProgramDesc), K15_PTR_SIZE, &renderProgramDesc);

			*renderProgramHandle = K15_InternalAddRenderProgramDesc(p_RenderContext, &renderProgramDesc);

			result = p_RenderContext->commandProcessing.programManagement.createProgram(p_RenderContext, &renderProgramDesc, renderProgramHandle);
			break;
		}

		/*case K15_RENDER_COMMAND_DRAW_INDEXED:
		{
			K15_RenderBufferHandle* vertexBufferHandle = 0;
			K15_RenderBufferHandle* indexBufferHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &vertexBufferHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, K15_PTR_SIZE, &indexBufferHandle);

			result = p_RenderContext->commandProcessing.renderMangement.drawIndexed(p_RenderContext, vertexBufferHandle, indexBufferHandle);

			break;
		}

		case K15_RENDER_COMMAND_DRAW_INDEXED_INSTANCED:
		{
			break;
		}

		case K15_RENDER_COMMAND_DRAW_INSTANCED:
		{
			break;
		}

		case K15_RENDER_COMMAND_DRAW:
		{
			break;
		}*/

		default:
		{
			assert(false);
		}
	}

	return result;
}
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

		result = K15_InternalProcessRenderCommand(p_RenderContext, *parameterFrontBuffer, currentCommand);

		//result = p_RenderContext->processRenderCommand(p_RenderContext, p_RenderCommandQueue, currentCommand);

		assert(result == K15_SUCCESS);
	}

	//command queue has been processed. Remove dispatched flag
	K15_PostSemaphore(p_RenderCommandQueue->processingSemaphore);
	//p_RenderCommandQueue->flags &= ~K15_CBF_DISPATCHED;

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

		renderContext->commandProcessing.screenManagement.clearScreen(renderContext);
	}


	return K15_SUCCESS;
}
/*********************************************************************************/



/*********************************************************************************/
void K15_SetRenderContextError(K15_RenderContext* p_RenderContext, const char* p_ErrorMessage, uint32 p_ErrorMessageLength)
{
	if (p_RenderContext->lastError.message)
	{
		if (p_RenderContext->lastError.length <= p_ErrorMessageLength)
		{
			free((void*)p_RenderContext->lastError.message);
			p_RenderContext->lastError.message = (char*)malloc(p_ErrorMessageLength + 1); //+1 for 0 terminator
			p_RenderContext->lastError.length = p_ErrorMessageLength;
		}
	}

	memcpy(p_RenderContext->lastError.message, p_ErrorMessage, p_ErrorMessageLength);

	p_RenderContext->lastError.message[p_ErrorMessageLength] = 0;
}
/*********************************************************************************/
const char* K15_GetLastRenderErrorMessage(K15_RenderContext* p_RenderContext)
{
	return p_RenderContext->lastError.message;
}
/*********************************************************************************/
const char* K15_GetLastRenderErrorMessageWithSize(K15_RenderContext* p_RenderContext, uint32* p_ErrorMessageSize)
{
	if (p_ErrorMessageSize)
	{
		*p_ErrorMessageSize = p_RenderContext->lastError.length;
	}

	return p_RenderContext->lastError.message;
}
/*********************************************************************************/
K15_RenderContext* K15_CreateRenderContext(K15_OSLayerContext* p_OSContext)
{
	assert(p_OSContext);

	K15_RenderContext* renderContext = (K15_RenderContext*)malloc(sizeof(K15_RenderContext));

	if (!renderContext)
	{
		assert(false);
		return 0;
	}

	//create command queue dispatcher
	K15_RenderCommandQueueDispatcher* renderCommandQueueDispatcher = (K15_RenderCommandQueueDispatcher*)malloc(sizeof(K15_RenderCommandQueueDispatcher));

	if (!renderCommandQueueDispatcher)
	{
		assert(false);
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
		assert(false);
		return 0;
	}
	
	/*********************************************************************************/
	//gpu buffers
	K15_RenderBufferDesc* gpuBuffers = (K15_RenderBufferDesc*)malloc(sizeof(K15_RenderBufferDesc) * K15_RENDER_MAX_GPU_BUFFER);

	if(!gpuBuffers)
	{
		assert(false);
		return 0;
	}

	renderContext->gpuBuffer.buffers = gpuBuffers;
	renderContext->gpuBuffer.amountBuffers = 0;
	/*********************************************************************************/

	/*********************************************************************************/
	//gpu programs
	K15_RenderProgramDesc* gpuPrograms = (K15_RenderProgramDesc*)malloc(sizeof(K15_RenderProgramDesc) * K15_RENDER_MAX_GPU_PROGRAMS);

	if(!gpuPrograms)
	{
		assert(false);
		return 0;
	}

	renderContext->gpuProgram.programs = gpuPrograms;
	renderContext->gpuProgram.amountPrograms = 0;
	/*********************************************************************************/

	renderContext->lastError.length = 0;
	renderContext->lastError.message = 0;

	renderContext->flags = 0;
	renderContext->commandQueues = renderCommandQueues;
	renderContext->amountCommandQueues = 0;
	renderContext->userData = 0;
	renderContext->commandQueueDispatcher = renderCommandQueueDispatcher;
	renderContext->createCommandQueueMutex = K15_CreateMutex();
	renderContext->renderThreadSync = K15_CreateSemaphore();

#ifdef K15_DEBUG
	renderContext->debugging.assignedThread = K15_GetCurrentThread();
#endif //K15_DEBUG

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
	renderCommandQueue->processingSemaphore = K15_CreateSemaphoreWithInitialValue(1);
	renderCommandQueue->swappingSemaphore = K15_CreateSemaphoreWithInitialValue(1);
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
uint8 K15_AddRenderBufferDescParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderBufferDesc* p_RenderBufferDesc)
{
	uint8 result = K15_InternalAddCommandQueueParameter(p_RenderCommandQueue,
														sizeof(K15_RenderBufferDesc),
														p_RenderBufferDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderBufferUpdateDescParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc)
{
	uint8 result = K15_InternalAddCommandQueueParameter(p_RenderCommandQueue,
														sizeof(K15_RenderBufferUpdateDesc),
														p_RenderBufferUpdateDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderBufferHandleParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderBufferHandle* p_RenderBufferHandle)
{
	uint8 result = K15_InternalAddCommandQueueParameter(p_RenderCommandQueue,
														K15_PTR_SIZE,
														&p_RenderBufferHandle);

	*p_RenderBufferHandle = K15_INVALID_GPU_RESOURCE_HANDLE;

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderProgramHandleParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_InternalAddCommandQueueParameter(p_RenderCommandQueue,
														K15_PTR_SIZE,
														&p_RenderProgramHandle);

	*p_RenderProgramHandle = K15_INVALID_GPU_RESOURCE_HANDLE;

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderProgramDescParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderProgramDesc* p_RenderProgramDesc)
{
	uint8 result = K15_InternalAddCommandQueueParameter(p_RenderCommandQueue,
														sizeof(K15_RenderProgramDesc),
														p_RenderProgramDesc);

	return result;
}
/*********************************************************************************/
void K15_DispatchRenderCommandQueue(K15_RenderCommandQueue* p_RenderCommandQueue)
{
	assert(p_RenderCommandQueue);
	assert(!p_RenderCommandQueue->lastCommand);
	assert(p_RenderCommandQueue->debugging.assignedThread == K15_GetCurrentThread());

	K15_RenderContext* renderContext = p_RenderCommandQueue->renderContext;
	K15_RenderCommandQueueDispatcher* renderDispatcher = renderContext->commandQueueDispatcher;
	
	//Wait until the render thread has processed the render queue
	K15_WaitSemaphore(p_RenderCommandQueue->processingSemaphore);

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
}
/*********************************************************************************/
void K15_ProcessDispatchedRenderCommandQueues(K15_RenderContext* p_RenderContext)
{
	assert(p_RenderContext);

	//signal the render thread to start processing the dispatched render buffers
	K15_PostSemaphore(p_RenderContext->renderThreadSync);
}
/*********************************************************************************/
