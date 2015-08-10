#ifndef _K15_RenderBackEnd_h_
#define _K15_RenderBackEnd_h_

#include "K15_RenderPrerequisites.h"
#include "K15_RenderStateDesc.h"
#include "K15_RenderBufferDesc.h"
#include "K15_RenderProgramDesc.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderInterface.h"
#include "K15_RenderTargetDesc.h"
#include "K15_RenderSamplerDesc.h"
#include "K15_RenderUniformCache.h"

//TODO render resources in one big buffer
//K15_RenderResourceHeader with type and offset and next element and stuff

struct K15_RenderState
{
	K15_RenderBlendStateDesc blendState;
	K15_RenderStencilStateDesc stencilState;
	K15_RenderDepthStateDesc depthState;
	K15_RenderRasterizerStateDesc rasterizerState;
};

struct K15_RenderCachedResources
{
	K15_RenderBufferDesc* bufferDescs[K15_RENDER_MAX_GPU_BUFFER];
	K15_RenderProgramDesc* programDescs[K15_RENDER_MAX_GPU_PROGRAMS];
	K15_RenderTextureDesc* textureDescs[K15_RENDER_MAX_GPU_TEXTURES];
	K15_RenderSamplerDesc* samplerDescs[K15_RENDER_MAX_GPU_SAMPLERS];
	K15_RenderTargetDesc* renderTargetDescs[K15_RENDER_MAX_GPU_RENDER_TARGETS];
};

struct K15_RenderResources
{
	K15_RenderResourceHandle intermediateVertexBuffer;
};

struct K15_RenderFeatures
{
	float maxAnisotropy;
	uint32 maxRenderTargets;
	uint32 maxSamples;
	uint32 maxTextureDimension;
};

struct K15_RenderInterface
{
	K15_ClearScreenCommandFnc clearScreen;

	K15_CreateBufferCommandFnc createBuffer;
	K15_UpdateBufferCommandFnc updateBuffer;
	K15_BindBufferCommandFnc bindBuffer;
	K15_DeleteBufferCommandFnc deleteBuffer;

	K15_CreateProgramCommandFnc createProgram;
	K15_DeleteProgramCommandFnc deleteProgram;
	K15_BindProgramCommandFnc bindProgram;
	K15_UpdateUniformCommandFnc updateUniform;
	K15_UpdateDirtyUniformsCommandFnc updateDirtyUniforms;

	K15_CreateTextureCommandFnc createTexture;
	K15_UpdateTextureCommandFnc updateTexture;
	K15_BindTextureCommandFnc bindTexture;
	K15_DeleteTextureCommandFnc deleteTexture;

	K15_CreateRenderTargetCommandFnc createRenderTarget;
	K15_BindRenderTargetCommandFnc bindRenderTarget;
	K15_UnbindRenderTargetCommandFnc unbindRenderTarget;
	K15_DeleteRenderTargetCommandFnc deleteRenderTarget;

	K15_CreateSamplerCommandFnc createSampler;
	K15_DeleteSamplerCommandFnc deleteSampler;

	K15_SetDepthStateCommandFnc setDepthState;
	K15_SetStencilStateCommandFnc setStencilState;
	K15_SetRasterizerStateCommandFnc setRasterizerState;
	K15_SetBlendStateCommandFnc setBlendState;

	K15_DrawFullscreenQuadCommandFnc drawFullscreenQuad;
	K15_DrawIndexedCommandFnc drawIndexed;
};

struct K15_RenderBackEnd
{
	K15_RenderContext* renderContext;

	K15_RenderUniformCache uniformCache;

	K15_RenderResources resources;
	K15_RenderCachedResources cachedResources;
	K15_RenderInterface interface;
	K15_RenderFeatures features;
	K15_RenderState state;
	K15_RenderState defaultState;

	bool8 interfaceInitialized;

	void* specificRenderPlatform;
};

void K15_ProcessRenderCommands(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommandBuffer* p_RenderCommandBuffer);

#endif //_K15_RenderBackEnd_h_