#ifndef _K15_RenderBackEnd_h_
#define _K15_RenderBackEnd_h_

#include "K15_RenderPrerequisites.h"
#include "K15_RenderStateDesc.h"
#include "K15_RenderBufferDesc.h"
#include "K15_RenderProgramDesc.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderVertexFormatDesc.h"
#include "K15_RenderInterface.h"
#include "K15_RenderTargetDesc.h"
#include "K15_RenderSamplerDesc.h"
#include "K15_RenderMaterialDesc.h"
#include "K15_RenderUniformCache.h"

struct K15_RenderState
{
	K15_RenderBlendStateDesc blendState;
	K15_RenderStencilStateDesc stencilState;
	K15_RenderDepthStateDesc depthState;
	K15_RenderRasterizerStateDesc rasterizerState;
};

struct K15_RenderVertexData
{
	K15_RenderResourceHandle vertexBufferHandle;
	K15_RenderVertexFormatDesc* vertexFormat;
	uint32 sizeInBytes;
	uint32 offsetInBytes;
	uint32 startVertexIndex;
	uint32 numVertices;
	void* buffer;
};

struct K15_RenderVertexFormatCache
{
	K15_RenderVertexFormatDesc vertexFormats[K15_RENDER_MAX_VERTEX_FORMATS];
	uint32 numVertexFormats;
};

struct K15_RenderResources
{
	K15_RenderResourceHandle intermediateVertexBufferHandle;
	
	struct 
	{
		K15_RenderMaterialDesc defaultFontMaterial;
		K15_RenderMaterialDesc default2DMaterial;
	} materials;

	struct 
	{
		K15_RenderResourceHandle* screenspaceColoredVertexProgramHandle;
		K15_RenderResourceHandle* screenspaceTexturedVertexProgramHandle;
		K15_RenderResourceHandle* defaultTextureFragmentProgramHandle;
		K15_RenderResourceHandle* defaultColorFragmentProgramHandle;
		K15_RenderResourceHandle* defaultFontFragmentProgramHandle;
	} shaders;

	struct 
	{
		K15_RenderResourceHandle linearClampSamplerHandle;
		K15_RenderResourceHandle nearestClampSamplerHandle;
	} samplers;
};

struct K15_RenderFeatures
{
	float maxAnisotropy;
	uint32 maxRenderTargets;
	uint32 maxSamples;
	uint32 maxTextureUnits;
	uint32 maxTextureDimension;
};

struct K15_RenderInterface
{
	K15_ClearScreenCommandFnc clearScreen;
	K15_SwapFrameBufferCommandFnc swapFrameBuffer;
	K15_SetDepthStateCommandFnc setDepthState;
	K15_SetStencilStateCommandFnc setStencilState;
	K15_SetRasterizerStateCommandFnc setRasterizerState;
	K15_SetBlendStateCommandFnc setBlendState;

	K15_CreateVertexFormatFromVertexFormatDescCommandFnc createVertexFormatFromVertexFormatDesc;
	K15_CreateTextureFromTextureDescCommandFnc createTextureFromTextureDesc;
	K15_CreateSamplerFromSamplerDescCommandFnc createSamplerFromSamplerDesc;
	K15_CreateProgramCommandFnc createProgram;
	K15_CreateMaterialFromMaterialFormatCommandFnc createMaterialFromMaterialFormat;

	K15_DeleteTextureCommandFnc deleteTexture;
	K15_DeleteSamplerCommandFnc deleteSampler;
	K15_DeleteProgramCommandFnc deleteProgram;

	K15_UpdateVertexDataCommandFnc updateVertexData;
	K15_FreeVertexDataCommandFnc freeVertexData;

	K15_ResizeViewportCommandFnc resizeViewport;

	K15_DrawGeometryCommandFnc drawGeometry;
};

struct K15_RenderBackEnd
{
	K15_RenderContext* renderContext;
	K15_ShaderProcessorContext* shaderProcessorContext;

	K15_RenderUniformCache uniformCache;
	K15_RenderVertexFormatCache vertexFormatCache;

 	K15_RenderResources resources;
	K15_RenderInterface renderInterface;
	K15_RenderFeatures features;
	K15_RenderState state;
	K15_RenderState defaultState;

	float viewportWidth;
	float viewportHeight;
	float viewportAspectRatio;

	bool8 interfaceInitialized;

	void* specificRenderPlatform;
};

void K15_ProcessRenderCommands(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommandBuffer* p_RenderCommandBuffer);
void K15_BeginFrame(K15_RenderBackEnd* p_RenderBackEnd);
void K15_EndFrame(K15_RenderBackEnd	* p_RenderBackEnd);

#endif //_K15_RenderBackEnd_h_