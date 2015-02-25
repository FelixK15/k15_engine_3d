#ifndef _K15_Rendering_RenderContext_h_
#define _K15_Rendering_RenderContext_h_

#include "K15_RenderPrerequisites.h"

#define K15_RENDERING_COMMAND_BACK_BUFFER_INDEX 1
#define K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX 0
#define K15_RENDERING_COMMAND_BUFFER_COUNT 2
#define K15_RENDERING_MAX_COMMANDS 128
#define K15_RENDERING_MAX_COMMAND_QUEUES 8
#define K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE size_kilobyte(64)

typedef uint8 (*K15_ProcessRenderCommandFnc)(K15_RenderContext* p_RenderContext, 
											 K15_RenderCommandQueue* p_RenderQueue, 
											 K15_RenderCommandInstance* p_RenderCommand);

enum K15_RenderCommand
{
	K15_RENDER_COMMAND_CLEAR_SCREEN = 0,
	K15_RENDER_COMMAND_CREATE_BUFFER,
	K15_RENDER_COMMAND_COUNT
};

enum K15_GpuBufferType : int32
{
	K15_GPU_BUFFER_TYPE_VERTEX = 0,
	K15_GPU_BUFFER_TYPE_INDEX,
	K15_GPU_BUFFER_TYPE_UNIFORM,

	K15_GPU_BUFFER_TYPE_COUNT
};

enum K15_CommandBufferFlags : uint32
{
	K15_CBF_LOCKED = 0x01
};

struct K15_RenderCommandInstance
{
	K15_RenderCommand type;
	uint32 parameterOffset;
	uint32 parameterSize;
};

struct K15_RenderCommandBuffer
{
	K15_RenderCommandInstance* commandBuffer;

	uint32 amountCommands;
	uint32 flags;
};

struct K15_RenderCommandParameterBuffer
{
	byte* parameterBuffer;

	uint32 parameterBufferOffset;
};

struct K15_RenderCommandQueue
{
	K15_RenderCommandBuffer* commandBuffers[K15_RENDERING_COMMAND_BUFFER_COUNT];
	K15_RenderCommandParameterBuffer* parameterBuffer[K15_RENDERING_COMMAND_BUFFER_COUNT];
	K15_RenderCommandInstance* lastCommand;
};

struct K15_RenderContext
{
	K15_RenderCommandQueue* commandQueues;
	K15_ProcessRenderCommandFnc processRenderCommand;

	void* userData;

	uint8 amountCommandQueues;
};

uint8 K15_ProcessRenderCommandQueue(K15_RenderContext* p_RenderContext, K15_RenderCommandQueue* p_RenderCommandQueue);

uint8 K15_CreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSContext);
uint8 K15_CreateRenderCommandQueue(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderContext* p_RenderContext);

uint8 K15_BeginRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderCommand p_RenderCommand);
uint8 K15_EndRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue);

uint8 K15_AddRenderUInt32Parameter(K15_RenderCommandQueue* p_RenderCommandQueue, uint32* p_Parameter);
uint8 K15_AddRenderInt32Parameter(K15_RenderCommandQueue* p_RenderCommandQueue, int32* p_Parameter);
uint8 K15_AddRenderBufferHandleParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_GpuBufferHandle* p_GpuBufferHandle);

void K15_UnlockRenderCommandQueue(K15_RenderCommandQueue* p_RenderCommandQueue);
#endif 