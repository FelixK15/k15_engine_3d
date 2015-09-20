#ifndef _K15_RenderCommandQueue_h_
#define _K15_RenderCommandQueue_h_

#include "K15_RenderPrerequisites.h"
#include "K15_RenderCommandBuffer.h"
#include "K15_RenderCommand.h"

#define K15_RENDERING_COMMAND_BUFFER_COUNT 2
#define K15_RENDERING_COMMAND_BACK_BUFFER_INDEX 1
#define K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX 0

struct K15_RenderCommandQueue
{
	K15_RenderCommandBuffer commandBuffers[K15_RENDERING_COMMAND_BUFFER_COUNT];
	K15_RenderCommand* lastCommand;

	K15_Semaphore* processLock;
	const char* name;

#ifdef K15_DEBUG_MRT
	struct 
	{
		K15_Thread* assignedThread;
	} debugging;
#endif //K15_DEBUG_MRT
};

void K15_SwapRenderCommandQueueBuffer(K15_RenderCommandQueue* p_RenderCommandQueue);
result8 K15_InitializeRenderCommandQueue(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderCommandQueue* p_RenderCommandQueue);
result8 K15_BeginRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderCommandType p_RenderCommandType);
result8 K15_EndRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue);

result8 K15_AddRenderCommandParameter(K15_RenderCommandQueue* p_RenderCommandQueue, uint32 p_ParameterSizeInBytes, void* p_Parameter);

#endif //_K15_RenderCommandQueue_h_