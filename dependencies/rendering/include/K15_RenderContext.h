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
#define K15_RENDERING_MAX_COMMAND_BUFFERS 8
#define K15_RENDERING_MAX_COMMAND_BUFFERS_TO_PROCESS K15_RENDERING_MAX_COMMAND_BUFFERS * 2
#define K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE size_kilobyte(64)

#define K15_RENDER_MAX_VIEWPORT_COUNT 4

typedef uint8 (*K15_ClearScreenCommandFnc)(K15_RenderContext* p_RenderContext);

//buffer
typedef uint8 (*K15_CreateBufferCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr);
typedef uint8 (*K15_UpdateBufferCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderBufferUpdateDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr);
typedef uint8 (*K15_DeleteBufferCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderBufferHandle* p_RenderBufferHandlePtr);

//programs
typedef uint8 (*K15_CreateProgramCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderProgramDesc* p_RenderProgramDesc, K15_RenderProgramHandle* p_RenderProgramHandlePtr);
typedef uint8 (*K15_DeleteProgramCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderProgramHandle* p_RenderProgramHandlePtr);
typedef uint8 (*K15_UpdateUniformCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc, K15_RenderProgramHandle* p_RenderProgramHandlePtr);

//states
typedef uint8 (*K15_SetDepthStateCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderDepthStateDesc* p_RenderDepthStateDesc);
typedef uint8 (*K15_SetStencilStateCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderStencilStateDesc* p_RenderStencilStateDesc);
typedef uint8 (*K15_SetRasterizerStateCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderRasterizerStateDesc* p_RenderRasterizerStateDesc);
typedef uint8 (*K15_SetBlendStateCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderBlendStateDesc* p_RenderDepthStateDesc);

//sampler
typedef uint8 (*K15_CreateSamplerCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderSamplerDesc* p_RenderTextureDesc, K15_RenderSamplerHandle* p_RenderTextureHandle);
typedef uint8 (*K15_DeleteSamplerCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderSamplerHandle* p_RenderTextureHandle);

//render target
typedef uint8 (*K15_CreateRenderTargetCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderTargetDesc* p_RenderTargetDesc, K15_RenderTargetHandle* p_RenderTargetHandle);
typedef uint8 (*K15_BindRenderTargetCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderTargetHandle* p_RenderTargetHandle);
typedef uint8 (*K15_UnbindRenderTargetCommandFnc)(K15_RenderContext* p_RenderContext);
typedef uint8 (*K15_DeleteRenderTargetCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderTargetHandle* p_RenderTargetHandle);

//textures
typedef uint8 (*K15_CreateTextureCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderTextureDesc* p_RenderTextureDesc, K15_RenderTextureHandle* p_RenderTextureHandle);
typedef uint8 (*K15_UpdateTextureCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderTextureUpdateDesc* p_RenderTextureDesc, K15_RenderTextureHandle* p_RenderTextureHandle);
typedef uint8 (*K15_DeleteTextureCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderTextureHandle* p_RenderTextureHandle);

//drawing
typedef uint8 (*K15_DrawFullscreenQuadCommandFnc)(K15_RenderContext* p_RenderContext, K15_RenderProgramHandle* p_RenderProgramHandle);

enum K15_RenderCommand
{
	K15_RENDER_COMMAND_CLEAR_SCREEN = 0,

	//buffer
	K15_RENDER_COMMAND_CREATE_BUFFER,
	K15_RENDER_COMMAND_UPDATE_BUFFER,
	K15_RENDER_COMMAND_DELETE_BUFFER,

	//program
	K15_RENDER_COMMAND_CREATE_PROGRAM,
	K15_RENDER_COMMAND_DELETE_PROGRAM,

	//texture
	K15_RENDER_COMMAND_CREATE_TEXTURE,
	K15_RENDER_COMMAND_UPDATE_TEXTURE,
	K15_RENDER_COMMAND_DELETE_TEXTURE,

	//sampler
	K15_RENDER_COMMAND_CREATE_SAMPLER,
	K15_RENDER_COMMAND_DELETE_SAMPLER,

	//uniforms
	K15_RENDER_COMMAND_UPDATE_UNIFORM,
	
	//render states
	K15_RENDER_COMMAND_SET_BLEND_STATE,
	K15_RENDER_COMMAND_SET_DEPTH_STATE,
	K15_RENDER_COMMAND_SET_STENCIL_STATE,
	K15_RENDER_COMMAND_SET_RASTERIZER_STATE,

	//render targets
	K15_RENDER_COMMAND_CREATE_RENDER_TARGET,
	K15_RENDER_COMMAND_BIND_RENDER_TARGET,
	K15_RENDER_COMMAND_UNBIND_RENDER_TARGET,
	K15_RENDER_COMMAND_DELETE_RENDER_TARGET,

	//drawing
	K15_RENDER_COMMAND_DRAW_FULLSCREEN_QUAD,

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

struct K15_RenderCommandBufferInstance
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

struct K15_RenderCommandBuffer
{
	K15_RenderCommandBufferInstance* commandBuffers[K15_RENDERING_COMMAND_BUFFER_COUNT];
	K15_RenderCommandParameterBuffer* parameterBuffer[K15_RENDERING_COMMAND_BUFFER_COUNT];
	K15_RenderCommandInstance* lastCommand;

	K15_RenderContext* renderContext;
	K15_Semaphore* processingSemaphore;
	K15_Semaphore* swappingSemaphore;
	K15_Mutex* processingMutex;

	uint32 flags;

#ifdef K15_DEBUG_MRT
	struct 
	{
		K15_Thread* assignedThread;
	} debugging;
#endif //K15_DEBUG_MRT

};

struct K15_RenderCommandBufferDispatcher
{
	K15_RenderCommandBuffer** renderCommandBuffersToProcess[K15_RENDERING_COMMAND_DISPATCH_BUFFER_COUNT];
	uint32 amountCommandBuffersToProcess[K15_RENDERING_COMMAND_DISPATCH_BUFFER_COUNT];

	K15_Mutex* swapMutex;
	uint32 flags;
};

struct K15_RenderCapabilities
{
	float maxAnisotropy;
	uint32 maxRenderTargets;
	uint32 maxSamples;
};

struct K15_RenderContext
{
	K15_RenderCapabilities capabilities;

	K15_RenderCommandBufferDispatcher* commandBufferDispatcher;
	K15_RenderCommandBuffer* commandBuffers;
	
	//K15_Thread* renderThread;
	K15_Mutex* createCommandBufferMutex;
	K15_Semaphore* renderThreadSync;

	void* userData;

	struct  
	{
		char* message;
		uint32 length;
	} lastError;

	struct  
	{
		uint32 x;
		uint32 y;
		uint32 width;
		uint32 height;
	} viewports[K15_RENDER_MAX_VIEWPORT_COUNT];

	uint32 viewportCount;
	uint32 activeViewportIndex;

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

		struct K15_ProgramManagementCommands
		{
			K15_CreateProgramCommandFnc createProgram;
			K15_DeleteProgramCommandFnc deleteProgram;
			K15_UpdateUniformCommandFnc updateUniform;
		} programManagement;

		struct K15_TextureManagementCommands
		{
			K15_CreateTextureCommandFnc createTexture;
			K15_UpdateTextureCommandFnc updateTexture;
			K15_DeleteTextureCommandFnc deleteTexture;
		} textureManagement;

		struct K15_RenderTargetManagementCommands
		{
			K15_CreateRenderTargetCommandFnc createRenderTarget;
			K15_BindRenderTargetCommandFnc bindRenderTarget;
			K15_UnbindRenderTargetCommandFnc unbindRenderTarget;
			K15_DeleteRenderTargetCommandFnc deleteRenderTarget;
		} renderTargetManagement;

		struct K15_SamplerManagementCommands
		{
			K15_CreateSamplerCommandFnc createSampler;
			K15_DeleteSamplerCommandFnc deleteSampler;
		} samplerManagement;

		struct K15_StateManagementCommands
		{
			K15_SetDepthStateCommandFnc setDepthState;
			K15_SetStencilStateCommandFnc setStencilState;
			K15_SetRasterizerStateCommandFnc setRasterizerState;
			K15_SetBlendStateCommandFnc setBlendState;
		} stateManagement;

		struct K15_DrawManagementCommandS
		{
			K15_DrawFullscreenQuadCommandFnc drawFullscreenQuad;
		} drawManagement;
	} commandProcessing;

	struct 
	{
		K15_RenderBufferDesc* buffers;
		uint32 amountBuffers;
	} gpuBuffer;

	struct  
	{
		K15_RenderProgramDesc* programs;
		uint32 amountPrograms;
	} gpuProgram;

	struct
	{
		K15_RenderTextureDesc* textures;
		uint32 amountTextures;
	} gpuTexture;

	struct  
	{
		K15_RenderSamplerDesc* samplers;
		uint32 amountSamplers;
	} gpuSampler;

	struct 
	{
		K15_RenderTargetDesc* renderTargets;
		uint32 amountRenderTargets;
	} gpuRenderTargets;

	struct 
	{
		K15_RenderBlendStateDesc* blendStateDesc;
		K15_RenderStencilStateDesc* stencilStateDesc;
		K15_RenderDepthStateDesc* depthStateDesc;
		K15_RenderRasterizerStateDesc* rasterizerStateDesc;
	} renderState;

	uint32 amountCommandBuffers;
	uint32 flags;

#ifdef K15_DEBUG_MRT
	struct 
	{
		K15_Thread* assignedThread;
	} debugging;
#endif //K15_DEBUG_MRT
};

//mostly used internally
void K15_SetRenderContextError(K15_RenderContext* p_RenderContext, const char* p_ErrorMessage, uint32 p_ErrorMessageLength);

const char* K15_GetLastRenderErrorMessage(K15_RenderContext* p_RenderContext);
const char* K15_GetLastRenderErrorMessageWithSize(K15_RenderContext* p_RenderContext, uint32* p_ErrorMessageSize);

K15_RenderContext* K15_CreateRenderContext(K15_OSLayerContext* p_OSContext);
K15_RenderCommandBuffer* K15_CreateRenderCommandBuffer(K15_RenderContext* p_RenderContext);

uint8 K15_BeginRenderCommand(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderCommand p_RenderCommand);
uint8 K15_EndRenderCommand(K15_RenderCommandBuffer* p_RenderCommandBuffer);

uint8 K15_AddRenderUInt32Parameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32* p_Parameter);
uint8 K15_AddRenderInt32Parameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, int32* p_Parameter);

//Buffer Parameter
uint8 K15_AddRenderBufferDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferDesc* p_RenderBufferDesc);
uint8 K15_AddRenderBufferUpdateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc);
uint8 K15_AddRenderBufferHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferHandle* p_RenderBufferHandle);

//Program Parameter
uint8 K15_AddRenderProgramHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderProgramHandle* p_RenderProgramHandle);
uint8 K15_AddRenderProgramDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderProgramDesc* p_RenderProgramDesc);

//Uniform Parameter
uint8 K15_AddRenderUniformUpdateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc);

//State Parameter
uint8 K15_AddRenderDepthStateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderDepthStateDesc* p_RenderDepthStateDesc);
uint8 K15_AddRenderStencilStateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderStencilStateDesc* p_RenderStencilStateDesc);
uint8 K15_AddRenderRasterizerStateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderRasterizerStateDesc* p_RenderRasterizerStateDesc);
uint8 K15_AddRenderBlendStateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBlendStateDesc* p_RenderBlendStateDesc);

//Texture Parameter
uint8 K15_AddRenderTextureHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTextureHandle* p_RenderTextureHandle);
uint8 K15_AddRenderTextureDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTextureDesc* p_RenderTextureDesc);
uint8 K15_AddRenderTextureUpdateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTextureUpdateDesc* p_RenderTextureDesc);

//Sampler Parameter
uint8 K15_AddRenderSamplerHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderSamplerHandle* p_RenderSamplerHandler);
uint8 K15_AddRenderSamplerDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderSamplerDesc* p_RenderSamplerDesc);

//Render Target Parameter
uint8 K15_AddRenderTargetHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTargetHandle* p_RenderTargetHandle);
uint8 K15_AddRenderTargetDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTargetDesc* p_RenderTargetDesc);

void K15_DispatchRenderCommandBuffer(K15_RenderCommandBuffer* p_RenderCommandBuffer);
void K15_ProcessDispatchedRenderCommandBuffers(K15_RenderContext* p_RenderContext);
#endif 