#ifndef _K15_Rendering_RenderContext_h_
#define _K15_Rendering_RenderContext_h_

#define K15_MAX_RENDER_COMMAND_QUEUES 8

#include "K15_RenderPrerequisites.h"
#include "K15_RenderCommandQueue.h"
#include "K15_RenderBackEnd.h"

#include "K15_CustomMemoryAllocator.h"

#define K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_COUNT 2
#define K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_FRONT_INDEX 0
#define K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_BACK_INDEX 1


struct K15_RenderContext
{
	K15_RenderCommandQueue* dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_COUNT][K15_MAX_RENDER_COMMAND_QUEUES];
	K15_RenderCommandQueue renderCommandQueues[K15_MAX_RENDER_COMMAND_QUEUES];
	K15_CustomMemoryAllocator memoryAllocator;
	K15_RenderBackEnd backEnd;

	uint32 numDispatchedRenderCommandQueues;
	uint32 numRenderCommandQueues;

	K15_Semaphore* dispatchLock;
	K15_Mutex* dispatchSwapLock;

	char* error;
};

K15_RenderContext* K15_CreateRenderContext(K15_OSContext* p_OSContext);
K15_RenderContext* K15_CreateRenderContextWithCustomAllocator(K15_OSContext* p_OSContext, K15_CustomMemoryAllocator p_CustomMemoryAllocator);

result8 K15_DispatchRenderCommandQueue(K15_RenderContext* p_RenderContext, K15_RenderCommandQueue* p_RenderCommandQueue);

K15_RenderCommandQueue* K15_CreateRenderCommandQueue(K15_RenderContext* p_RenderContext, const char* p_RenderCommandQueueName = 0);

void K15_ProcessDispatchedRenderCommandQueues(K15_RenderContext* p_RenderContext);

#endif 