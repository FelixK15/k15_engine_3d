#ifndef _K15_Rendering_RenderContext_h_
#define _K15_Rendering_RenderContext_h_

#include "K15_RenderPrerequisites.h"

#define K15_RENDERING_COMMAND_BUFFER_COUNT 2
#define K15_RENDERING_COMMAND_BACK_BUFFER_INDEX 1
#define K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX 0

#define K15_RENDERING_COMMAND_DISPATCH_BUFFER_COUNT 2
#define K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX 1
#define K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX 0


#define K15_RENDERING_MAX_COMMANDS 128
#define K15_RENDERING_MAX_COMMAND_QUEUES 8
#define K15_RENDERING_MAX_COMMAND_QUEUES_TO_PROCESS K15_RENDERING_MAX_COMMAND_QUEUES * 2
#define K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE size_kilobyte(64)

// typedef uint8 (*K15_ProcessRenderCommandFnc)(K15_RenderContext* p_RenderContext, 
// 											 K15_RenderCommandQueue* p_RenderQueue, 
// 											 K15_RenderCommandInstance* p_RenderCommand);

typedef uint8 (*K15_ClearScreenCommandFnc)(K15_RenderContext* p_RenderContext);

typedef uint8 (*K15_CreateBufferCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr);
typedef uint8 (*K15_UpdateBufferCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr);
typedef uint8 (*K15_DeleteBufferCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr);


enum K15_RenderCommand
{
	K15_RENDER_COMMAND_CLEAR_SCREEN = 0,
	K15_RENDER_COMMAND_CREATE_BUFFER,
	K15_RENDER_COMMAND_UPDATE_BUFFER,
	K15_RENDER_COMMAND_DELETE_BUFFER,
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
	K15_CBF_SWAPPING = 0x01,
	K15_CBF_DISPATCHED = 0x02
};

enum K15_CommandDispatcherFlags : uint32
{
	K15_CDF_SWAPPING = 0x01,
	K15_CDF_PROCESSING = 0x02
};

enum K15_RenderContextFlags : uint32
{
	K15_RCF_INITIALIZED = 0x01
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

	K15_RenderContext* renderContext;
	K15_Semaphore* processingSemaphore;
	K15_Semaphore* swappingSemaphore;
	K15_Mutex* processingMutex;

	volatile uint32 flags;

#ifdef K15_DEBUG
	struct 
	{
		K15_Thread* assignedThread;
	} debugging;
#endif //K15_DEBUG

};

struct K15_RenderCommandQueueDispatcher
{
	K15_RenderCommandQueue** renderCommandQueuesToProcess[K15_RENDERING_COMMAND_DISPATCH_BUFFER_COUNT];
	uint32 amountCommandQueuesToProcess[K15_RENDERING_COMMAND_DISPATCH_BUFFER_COUNT];

	K15_Mutex* swapMutex;
	uint32 flags;
};

struct K15_RenderContext
{
	K15_RenderCommandQueueDispatcher* commandQueueDispatcher;
	K15_RenderCommandQueue* commandQueues;
	
	K15_Thread* renderThread;
	K15_Mutex* createCommandQueueMutex;
	K15_Semaphore* renderThreadSync;

	void* userData;

	struct 
	{
		struct K15_ScreenManangementCommands
		{
			K15_ClearScreenCommandFnc clearScreen;
		} screenManagement;

		struct K15_BufferManagementCommands
		{
			K15_CreateBufferCommandFnc createBuffer;
			K15_UpdateBufferCommandFnc updateBuffer;
			K15_DeleteBufferCommandFnc deleteBuffer;
		} bufferManagement;
	} commandProcessing;

	struct 
	{
		K15_RenderBufferDesc* buffers;
		uint32 amountBuffers;
	} gpuBuffer;

	
	uint32 amountCommandQueues;
	uint32 flags;

#ifdef K15_DEBUG
	struct 
	{
		K15_Thread* assignedThread;
	} debugging;
#endif //K15_DEBUG
};

K15_RenderContext* K15_CreateRenderContext(K15_OSLayerContext* p_OSContext);
K15_RenderCommandQueue* K15_CreateRenderCommandQueue(K15_RenderContext* p_RenderContext);

uint8 K15_BeginRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderCommand p_RenderCommand);
uint8 K15_EndRenderCommand(K15_RenderCommandQueue* p_RenderCommandQueue);

uint8 K15_AddRenderUInt32Parameter(K15_RenderCommandQueue* p_RenderCommandQueue, uint32* p_Parameter);
uint8 K15_AddRenderInt32Parameter(K15_RenderCommandQueue* p_RenderCommandQueue, int32* p_Parameter);
uint8 K15_AddRenderBufferDescParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderBufferDesc* p_RenderBufferDesc);
uint8 K15_AddRenderBufferHandleParameter(K15_RenderCommandQueue* p_RenderCommandQueue, K15_RenderBufferHandle* p_RenderBufferHandle);

void K15_DispatchRenderCommandQueue(K15_RenderCommandQueue* p_RenderCommandQueue);
void K15_ProcessDispatchedRenderCommandQueues(K15_RenderContext* p_RenderContext);
#endif 