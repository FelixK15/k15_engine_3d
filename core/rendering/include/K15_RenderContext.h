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

struct K15_DebugRenderContext
{
	K15_RenderVertexFormatDesc debug2DVertexFormat;
	K15_RenderVertexFormatDesc debug3DVertexFormat;

	float* debug2DVertexBuffer;
	float* debug3DVertexBuffer;

	uint32 num2DFloats;
	uint32 num3DFloats;

	uint32 capacity2DFloats;
	uint32 capacity3DFloats;

	K15_RenderMaterialDesc* debugMaterial;
};

struct K15_RenderContext
{
	K15_DebugRenderContext* debugRenderContext;
	K15_RenderCommandQueue** dispatchedRenderCommandQueues[K15_RENDERING_DISPATCHED_COMMAND_QUEUE_BUFFER_COUNT];
	K15_RenderCommandQueue renderCommandQueues[K15_MAX_RENDER_COMMAND_QUEUES];
	K15_CustomMemoryAllocator memoryAllocator;
	K15_RenderBackEnd backEnd;

	uint32 numDispatchedRenderCommandQueues;
	uint32 numRenderCommandQueues;

	K15_Semaphore* dispatchLock;
	K15_Mutex* dispatchSwapLock;

	char* error;
};

//Debug Rendering
void K15_DebugRender2DRect(K15_RenderContext* p_RenderContext, uint32 p_PixelPosLeft, uint32 p_PixelPosRight,
	uint32 p_PixelPosTop, uint32 p_PixelPosBottom, uint32 p_Red, uint32 p_Green, uint32 p_Blue);

void K15_SetRenderContextError(K15_RenderContext* p_RenderContext, const char* p_ErrorMessage, uint32 p_ErrorMessageLength = 0);

K15_RenderContext* K15_CreateRenderContext(K15_OSContext* p_OSContext);
K15_RenderContext* K15_CreateRenderContextWithCustomAllocator(K15_OSContext* p_OSContext, 
	K15_CustomMemoryAllocator p_CustomMemoryAllocator);

result8 K15_DispatchRenderCommandQueue(K15_RenderContext* p_RenderContext, K15_RenderCommandQueue* p_RenderCommandQueue);
void K15_LoadStockShader(K15_RenderContext* p_RenderContext, K15_ResourceContext* p_ResourceContext);

K15_RenderCommandQueue* K15_CreateRenderCommandQueue(K15_RenderContext* p_RenderContext, 
	const char* p_RenderCommandQueueName = 0);

void K15_SwapDispatchedRenderCommandQueueBuffers(K15_RenderContext* p_RenderContext);
void K15_ProcessDispatchedRenderCommandQueues(K15_RenderContext* p_RenderContext);

#endif 