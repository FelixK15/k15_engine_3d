#ifndef _K15_Rendering_RenderContext_h_
#define _K15_Rendering_RenderContext_h_

#include "K15_RenderPrerequisites.h"

#define K15_RENDERING_MAX_COMMANDS 128
#define K15_RENDERING_MAX_COMMAND_QUEUES 8
#define K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE size_megabyte(32)

struct K15_RenderCommand
{
	uint32 type;

};

struct K15_RenderCommandQueue
{
		K15_RenderCommand commands[K15_RENDERING_MAX_COMMANDS];
		byte commandParameter[K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE];
};

struct K15_RenderContext
{
	K15_RenderCommandQueue commandQueues[K15_RENDERING_MAX_COMMAND_QUEUES];

	void* userData;
};

uint8 K15_CreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSContext);
uint8 K15_CreateRenderCommandQueue(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderContext* p_RenderContext);

#endif 