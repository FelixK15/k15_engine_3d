#include "K15_RenderContext.h"
#include "K15_OSContext.h"
#include "K15_Logging.h"
#include "K15_Thread.h"

#include "K15_RenderBackEnd.cpp"
#include "K15_RenderCommandBuffer.cpp"
#include "K15_RenderCommandQueue.cpp"
#include "K15_RenderCommands.cpp"

#include "K15_RenderProgramDesc.cpp"

#include "OpenGL/K15_RenderGLContext.cpp"

#include <assert.h>

/*********************************************************************************/
intern result8 K15_InternalCreateSpecificRenderContext(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderBackEnd* p_RenderBackEnd, K15_OSContext* p_OSContext)
{
	K15_ASSERT(p_OSContext);

	bool8 createGLRenderContext = TRUE;

	if (createGLRenderContext)
	{
		result8 result = K15_GLCreateRenderContext(p_MemoryAllocator, p_RenderBackEnd, p_OSContext);

		if (result != K15_SUCCESS)
		{
			return result;
		}
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern void K15_InternalSwapDispatchedRenderCommandQueueBuffers(K15_RenderContext* p_RenderContext, uint32* p_NumDispatchedRenderCommandQueues)
{
	//lock mutex so another thread can't dispatch command queues while this thread is swapping
	K15_LockMutex(p_RenderContext->dispatchSwapLock);

	*p_NumDispatchedRenderCommandQueues = p_RenderContext->numDispatchedRenderCommandQueues;

	K15_RenderCommandQueue*** tempRenderCommandQueue = 0;
	K15_RenderCommandQueue*** frontRenderCommandQueue = (K15_RenderCommandQueue***)&p_RenderContext->dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_FRONT_INDEX];
	K15_RenderCommandQueue*** backRenderCommandQueue = (K15_RenderCommandQueue***)&p_RenderContext->dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_BACK_INDEX];

	*tempRenderCommandQueue = *frontRenderCommandQueue;
	*frontRenderCommandQueue = *backRenderCommandQueue;

	p_RenderContext->numDispatchedRenderCommandQueues = 0;

	//buffers can be dispatched again
	K15_UnlockMutex(p_RenderContext->dispatchSwapLock);
}
/*********************************************************************************/



/*********************************************************************************/
K15_RenderContext* K15_CreateRenderContext(K15_OSContext* p_OSContext)
{
	return K15_CreateRenderContextWithCustomAllocator(p_OSContext, K15_CreateDefaultMemoryAllocator());
}
/*********************************************************************************/
K15_RenderContext* K15_CreateRenderContextWithCustomAllocator(K15_OSContext* p_OSContext, K15_CustomMemoryAllocator p_CustomMemoryAllocator)
{
	result8 result = K15_SUCCESS;

	//try to initialize the specific render context first
	K15_RenderContext* renderContext = (K15_RenderContext*)K15_AllocateFromMemoryAllocator(&p_CustomMemoryAllocator, sizeof(K15_RenderContext));
	renderContext->memoryAllocator = p_CustomMemoryAllocator;

	//initialize render command queues
	for (uint32 queueIndex = 0;
		queueIndex < K15_MAX_RENDER_COMMAND_QUEUES;
		++queueIndex)
	{
		K15_InitializeRenderCommandQueue(&p_CustomMemoryAllocator, &renderContext->renderCommandQueues[queueIndex]);
	}

	//initialize dispatched render command queue array
	for (uint32 queueIndex = 0;
		queueIndex < K15_MAX_RENDER_COMMAND_QUEUES;
		++queueIndex)
	{
		renderContext->dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_FRONT_INDEX][queueIndex] =  0;
		renderContext->dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_FRONT_INDEX][queueIndex] =  0;
	}

	renderContext->dispatchLock = K15_CreateSemaphore();
	renderContext->numRenderCommandQueues = 0;
	renderContext->numDispatchedRenderCommandQueues = 0;

	K15_InitializeRenderBackEnd(&p_CustomMemoryAllocator, renderContext, &renderContext->backEnd);

	result = K15_InternalCreateSpecificRenderContext(&p_CustomMemoryAllocator, &renderContext->backEnd, p_OSContext);

	if (result != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not create render context (%s).", K15_GetErrorCodeString(result));
	}

	//K15_IntitializeRenderBackEndResources(&p_CustomMemoryAllocator, &renderContext->backEnd);

	return renderContext;
}
/*********************************************************************************/
result8 K15_DispatchRenderCommandQueue(K15_RenderContext* p_RenderContext, K15_RenderCommandQueue* p_RenderCommandQueue)
{
	//dispatched render command queues are getting stored in the higher indexed part of the array.
	//once the dispatched render command queues are getting sent to the back end, the queues from the "back" of the
	//array are getting moved to the front of the array (which will get sent to the back end). The "back" of the
	//array is getting 0-ed afterwards.

	K15_ASSERT_TEXT(p_RenderContext, "Render context is NULL.");
	K15_ASSERT_TEXT(p_RenderCommandQueue, "Render command queue is NULL.");
	K15_ASSERT_TEXT(p_RenderContext->numDispatchedRenderCommandQueues <= p_RenderContext->numRenderCommandQueues, "Tried to dispatch more render command queues than there are available.");
	K15_ASSERT_TEXT(p_RenderCommandQueue->lastCommand, "Render Command Queue last command not finished.");

	//wait until the dispatch buffers have been swapped by the render thread
	K15_LockMutex(p_RenderContext->dispatchSwapLock);

#ifdef K15_CHECK_FOR_ALREADY_DISPATCHED_RENDER_COMMAND_QUEUE
	for (uint32 queueIndex = 0;
		queueIndex < K15_MAX_RENDER_COMMAND_QUEUES;
		++queueIndex)
	{
		if (p_RenderContext->dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_BACK_INDEX][queueIndex] == 0)
		{
			break;
		}

		if (p_RenderContext->dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_BACK_INDEX][queueIndex] == p_RenderCommandQueue)
		{
			K15_LOG_WARNING_MESSAGE("Render command queue '%s' has already been dispatched (Ignoring command queue).", p_RenderCommandQueue->name);
			return K15_ERROR_RENDER_COMMAND_QUEUE_ALREADY_DISPATCHED;
		}
	}
#endif //K15_CHECK_FOR_ALREADY_DISPATCHED_RENDER_COMMAND_QUEUE

	//swap front with back buffer
	K15_SwapRenderCommandQueueBuffer(p_RenderCommandQueue);

	uint32 numDispatchedRenderCommandQueues = p_RenderContext->numDispatchedRenderCommandQueues++;
	p_RenderContext->dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_BACK_INDEX][numDispatchedRenderCommandQueues] = p_RenderCommandQueue;

	//wait until the dispatch buffers have been swapped by the render thread
	K15_UnlockMutex(p_RenderContext->dispatchSwapLock);

	return K15_SUCCESS;
}
/*********************************************************************************/
K15_RenderCommandQueue* K15_CreateRenderCommandQueue(K15_RenderContext* p_RenderContext, const char* p_RenderCommandQueueName)
{
	K15_ASSERT_TEXT(p_RenderContext, "Render context is NULL.");
	K15_ASSERT_TEXT(p_RenderContext->numRenderCommandQueues < K15_MAX_RENDER_COMMAND_QUEUES, "Number of maximum command queues reached");

	if (!p_RenderCommandQueueName)
	{
		char* renderCommandQueueNameBuffer = (char*)K15_AllocateFromMemoryAllocator(&p_RenderContext->memoryAllocator, 512);
		p_RenderCommandQueueName = K15_GenerateString("RenderCommandQueue#%d", renderCommandQueueNameBuffer, p_RenderContext->numRenderCommandQueues);
	}

	uint32 renderCommandQueueIndex = p_RenderContext->numRenderCommandQueues++;

	K15_RenderCommandQueue* commandQueue = p_RenderContext->renderCommandQueues + renderCommandQueueIndex;
	commandQueue->name = p_RenderCommandQueueName;

	return commandQueue;
}
/*********************************************************************************/
void K15_ProcessDispatchedRenderCommandQueues(K15_RenderContext* p_RenderContext)
{
	K15_ASSERT_TEXT(p_RenderContext, "Render context is NULL.");
	
	//wait until the previous dispatched queues have been processed
	K15_WaitSemaphore(p_RenderContext->dispatchLock);

	uint32 numDispatchedRenderCommandQueues = 0;
	K15_InternalSwapDispatchedRenderCommandQueueBuffers(p_RenderContext, &numDispatchedRenderCommandQueues);
	
	//process dispatched command queues
	for (uint32 queueIndex = 0;
		queueIndex < numDispatchedRenderCommandQueues;
		++queueIndex)
	{
		K15_RenderCommandQueue* dispatchedCommandQueue = p_RenderContext->dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_FRONT_INDEX][queueIndex];

		K15_ProcessRenderCommands(&p_RenderContext->backEnd, 
			&dispatchedCommandQueue->commandBuffers[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX]);
	}

	//release the lock so that dispatched buffers can be processed again
	K15_PostSemaphore(p_RenderContext->dispatchLock);
}
/*********************************************************************************/