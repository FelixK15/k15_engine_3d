#include "K15_RenderBackEnd.h"

#include "K15_TextureFormat.h"
#include "K15_SamplerFormat.h"
#include "K15_MaterialFormat.h"
#include "K15_FontFormat.h"

#include "K15_Window.h"

#include "K15_Rectangle.h"
#include "K15_Vector2.h"
#include "K15_Matrix3.h"
#include "K15_GUIContext.h"

#include "K15_RenderCommandBuffer.h"
#include "K15_RenderGeometryDesc.h"
#include "K15_RenderFontDesc.h"
#include "K15_RenderMaterialDesc.h"
#include "K15_ResourceContext.h"

#include "K15_ErrorCodes.h"

#include "generated/K15_RenderMaterialDataDescStretchBuffer.cpp"

#include "K15_RenderFormats.cpp"
#include "K15_RenderVertexFormatDesc.cpp"
#include "K15_RenderShaderSemantics.cpp"
#include "K15_RenderUtil.cpp"
#include "K15_RenderGUIContext.cpp"
#include "K15_RenderUniformCache.cpp"

#include "K15_ShaderCompiler.h"

/*********************************************************************************/
intern int K15_InternalCompareKerning(const void* p_Key, const void* p_Element)
{
  uint32 shiftedCharacter = *(uint32*)p_Key;
  K15_RenderKerningDesc* kerningDesc = (K15_RenderKerningDesc*)p_Element;

  return shiftedCharacter - kerningDesc->shiftedCharacters;
}
/*********************************************************************************/
intern void K15_CreateDefaultRenderState(K15_RenderState* p_RenderState)
{
	K15_RenderBlendStateDesc defaultBlendState = {};
	K15_RenderStencilStateDesc defaultStencilState = {};
	K15_RenderDepthStateDesc defaultDepthState = {};
	K15_RenderRasterizerStateDesc defaultRasterizerState = {};
	/*********************************************************************************/
	defaultRasterizerState.cullingMode = K15_CULLING_MODE_NONE;
	defaultRasterizerState.fillMode = K15_FILLMODE_SOLID;
	defaultRasterizerState.scissoringEnabled = FALSE;
	//defaultRasterizerState.vertexOrder = K15_VERTEX_ORDER_COUNTER_CLOCKWISE;
	defaultRasterizerState.vertexOrder = K15_VERTEX_ORDER_CLOCKWISE;
	/*********************************************************************************/
	defaultDepthState.compareFunction = K15_COMPARISON_GREATER_EQUAL;
	defaultDepthState.enabled = TRUE;
	/*********************************************************************************/
	defaultStencilState.enabled = FALSE;
	/*********************************************************************************/
	defaultBlendState.enabled = TRUE;
	defaultBlendState.sourceBlendFactorRGB = K15_BLEND_FACTOR_SRC_ALPHA;
	defaultBlendState.sourceBlendFactorAlpha = K15_BLEND_FACTOR_ONE;
	defaultBlendState.blendOperationRGB = K15_BLEND_OPERATION_ADD;
	defaultBlendState.blendOperationAlpha = K15_BLEND_OPERATION_ADD;
	defaultBlendState.destinationBlendFactorRGB = K15_BLEND_FACTOR_INVERSE_SRC_ALPHA;
	defaultBlendState.destinationBlendFactorAlpha = K15_BLEND_FACTOR_ONE;
	/*********************************************************************************/
	p_RenderState->blendState = defaultBlendState;
	p_RenderState->stencilState = defaultStencilState;
	p_RenderState->depthState = defaultDepthState;
	p_RenderState->rasterizerState = defaultRasterizerState;
	/*********************************************************************************/
}
/*********************************************************************************/
intern result8 K15_SetDefaultRenderState(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_ASSERT(p_RenderBackEnd);
	K15_ASSERT_TEXT(p_RenderBackEnd->interfaceInitialized, "Render Interface it not yet initialized.");

	if (p_RenderBackEnd->interfaceInitialized)
	{
		K15_RenderState* defaultState = &p_RenderBackEnd->defaultState;
		p_RenderBackEnd->renderInterface.setBlendState(p_RenderBackEnd, &defaultState->blendState);
	}
}
/*********************************************************************************/
intern result8 K15_InitializeRenderBackEnd(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderContext* p_RenderContext, K15_RenderBackEnd* p_RenderBackEnd, K15_OSContext* p_OSContext)
{
	p_RenderBackEnd->interfaceInitialized = K15_FALSE;

	K15_CreateDefaultRenderState(&p_RenderBackEnd->defaultState);

	p_RenderBackEnd->renderContext = p_RenderContext;
	p_RenderBackEnd->shaderProcessorContext = (K15_ShaderProcessorContext*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_ShaderProcessorContext));
	
	p_RenderBackEnd->vertexFormatCache.numVertexFormats = 0;

	p_RenderBackEnd->shaderProcessorContext->semanticTable = K15_CreateSemanticTable();
	p_RenderBackEnd->shaderProcessorContext->typeTable = K15_CreateTypeTable();
	p_RenderBackEnd->shaderProcessorContext->lastProcessResult.numErrors = 0;
	p_RenderBackEnd->shaderProcessorContext->lastProcessResult.errors = 0;

	K15_InitializeRenderUniformCache(p_RenderBackEnd, &p_RenderBackEnd->uniformCache);

	K15_Window* window = p_OSContext->window.window;

	p_RenderBackEnd->viewportHeight = (float)window->height;
	p_RenderBackEnd->viewportWidth = (float)window->width;
	p_RenderBackEnd->viewportAspectRatio = p_RenderBackEnd->viewportWidth / p_RenderBackEnd->viewportHeight;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern K15_RenderResourceHandle K15_AddRenderBufferDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBufferDesc* p_RenderBufferDesc)
{
// 	K15_RenderResourceHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;
// 
// 	for (uint32 bufferDescIndex = 0;
// 		bufferDescIndex < K15_RENDER_MAX_GPU_BUFFER;
// 		++bufferDescIndex)
// 	{
// 		if (!p_RenderBackEnd->cachedResources.bufferDescs[bufferDescIndex])
// 		{
// 			p_RenderBackEnd->cachedResources.bufferDescs[bufferDescIndex] = p_RenderBufferDesc;
// 			handle = (K15_RenderResourceHandle)p_RenderBufferDesc;
// 
// 			break;
// 		}
// 	}
// 
// 	p_RenderBackEnd->renderInterface.createBuffer(p_RenderBackEnd, p_RenderBufferDesc, &handle);
// 
// 	return handle;
}
/*********************************************************************************/
intern K15_RenderResourceHandle K15_AddRenderTextureDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderTextureDesc* p_RenderTextureDesc)
{
// 	K15_RenderResourceHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;
// 
// 	for (uint32 textureDescIndex = 0;
// 		textureDescIndex < K15_RENDER_MAX_GPU_TEXTURES;
// 		++textureDescIndex)
// 	{
// 		if (!p_RenderBackEnd->cachedResources.textureDescs[textureDescIndex])
// 		{
// 			p_RenderBackEnd->cachedResources.textureDescs[textureDescIndex] = p_RenderTextureDesc;
// 			handle = (K15_RenderResourceHandle)textureDescIndex;
// 
// 			break;
// 		}
// 	}
// 
// 	p_RenderBackEnd->renderInterface.createTexture(p_RenderBackEnd, p_RenderTextureDesc, &handle);
// 
// 	return handle;
}
/*********************************************************************************/
intern K15_RenderResourceHandle K15_AddRenderSamplerDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderSamplerDesc* p_RenderSamplerDesc)
{
// 	K15_RenderResourceHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;
// 
// 	for (uint32 samplerDescIndex = 0;
// 		samplerDescIndex < K15_RENDER_MAX_GPU_SAMPLERS;
// 		++samplerDescIndex)
// 	{
// 		if (!p_RenderBackEnd->cachedResources.samplerDescs[samplerDescIndex])
// 		{
// 			p_RenderBackEnd->cachedResources.samplerDescs[samplerDescIndex] = p_RenderSamplerDesc;
// 			handle = (K15_RenderResourceHandle)samplerDescIndex;
// 
// 			break;
// 		}
// 	}
// 
// 	p_RenderBackEnd->renderInterface.createSampler(p_RenderBackEnd, p_RenderSamplerDesc, &handle);
// 
// 	return handle;
}
/*********************************************************************************/
intern void* K15_AllocateFromRenderBuffer(K15_RenderResourceHandle p_ResourceHandle, uint32 p_SizeInBytes)
{
	//TODO
	return 0;
}
/*********************************************************************************/
intern void K15_InternalCreateDefault2DMaterial(K15_RenderBackEnd* p_RenderBackEnd, K15_CustomMemoryAllocator* p_Allocator)
{
	K15_RenderMaterialDesc default2DMaterial = {};
	default2DMaterial.numMaterialPasses = 1;

	default2DMaterial.materialPasses = (K15_RenderMaterialPassDesc*)K15_AllocateFromMemoryAllocator(p_Allocator, sizeof(K15_RenderMaterialPassDesc));
	default2DMaterial.materialPasses[0].fragmentShaderHandle = p_RenderBackEnd->resources.shaders.defaultTextureFragmentProgramHandle;
	default2DMaterial.materialPasses[0].vertexShaderHandle = p_RenderBackEnd->resources.shaders.screenspaceTexturedVertexProgramHandle;
	K15_CreateRenderMaterialDataDescStretchBufferWithCustomAllocator(&default2DMaterial.materialPasses[0].materialData, *p_Allocator, 2);

	//sampler
	K15_RenderMaterialDataDesc fontSampler = {};
	fontSampler.typeID = K15_TYPE_SAMPLER_2D;
	fontSampler.semanticID = K15_UNIFORM_SEMANTIC_DATA;
	fontSampler.semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	fontSampler.nameHash = K15_GenerateStringHash("sampler");

	//texture
	K15_RenderMaterialDataDesc fontTexture = {};
	fontTexture.typeID = K15_TYPE_TEXTURE_2D;
	fontTexture.semanticID = K15_UNIFORM_SEMANTIC_DATA;
	fontTexture.semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	fontTexture.nameHash = K15_GenerateStringHash("tex");

	K15_PushRenderMaterialDataDescStretchBufferElement(&default2DMaterial.materialPasses[0].materialData, fontSampler);
	K15_PushRenderMaterialDataDescStretchBufferElement(&default2DMaterial.materialPasses[0].materialData, fontTexture);

	K15_SetRenderMaterialRenderResourceDataByName(&default2DMaterial.materialPasses[0], "sampler", &p_RenderBackEnd->resources.samplers.linearClampSamplerHandle);

	p_RenderBackEnd->resources.materials.default2DMaterial = default2DMaterial;
}
/*********************************************************************************/
intern void K15_InternalCreateDefaultFontMaterial(K15_RenderBackEnd* p_RenderBackEnd, K15_CustomMemoryAllocator* p_Allocator)
{
	K15_RenderMaterialDesc defaultFontMaterial = {};
	defaultFontMaterial.numMaterialPasses = 1;

	defaultFontMaterial.materialPasses = (K15_RenderMaterialPassDesc*)K15_AllocateFromMemoryAllocator(p_Allocator, sizeof(K15_RenderMaterialPassDesc));
	defaultFontMaterial.materialPasses[0].fragmentShaderHandle = p_RenderBackEnd->resources.shaders.defaultFontFragmentProgramHandle;
	defaultFontMaterial.materialPasses[0].vertexShaderHandle = p_RenderBackEnd->resources.shaders.screenspaceTexturedColoredVertexProgramHandle;
	K15_CreateRenderMaterialDataDescStretchBufferWithCustomAllocator(&defaultFontMaterial.materialPasses[0].materialData, *p_Allocator, 2);

	//sampler
	K15_RenderMaterialDataDesc fontSampler = {};
	fontSampler.typeID = K15_TYPE_SAMPLER_2D;
	fontSampler.semanticID = K15_UNIFORM_SEMANTIC_DATA;
	fontSampler.semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	fontSampler.nameHash = K15_GenerateStringHash("sampler");

	//texture
	K15_RenderMaterialDataDesc fontTexture = {};
	fontTexture.typeID = K15_TYPE_TEXTURE_2D;
	fontTexture.semanticID = K15_UNIFORM_SEMANTIC_DATA;
	fontTexture.semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	fontTexture.nameHash = K15_GenerateStringHash("tex");

	K15_PushRenderMaterialDataDescStretchBufferElement(&defaultFontMaterial.materialPasses[0].materialData, fontSampler);
	K15_PushRenderMaterialDataDescStretchBufferElement(&defaultFontMaterial.materialPasses[0].materialData, fontTexture);

	K15_SetRenderMaterialRenderResourceDataByName(&defaultFontMaterial.materialPasses[0], "sampler", &p_RenderBackEnd->resources.samplers.linearClampSamplerHandle);

	p_RenderBackEnd->resources.materials.defaultFontMaterial = defaultFontMaterial;
}
/*********************************************************************************/
intern void K15_InternalCreateDefaultGUIMaterial(K15_RenderBackEnd* p_RenderBackEnd, K15_CustomMemoryAllocator* p_Allocator)
{
	K15_RenderMaterialDesc defaultGUIMaterial = {};
	defaultGUIMaterial.numMaterialPasses = 1;
	
	defaultGUIMaterial.materialPasses = (K15_RenderMaterialPassDesc*)K15_AllocateFromMemoryAllocator(p_Allocator, sizeof(K15_RenderMaterialPassDesc));
	defaultGUIMaterial.materialPasses[0].fragmentShaderHandle = p_RenderBackEnd->resources.shaders.defaultColorFragmentProgramHandle;
	defaultGUIMaterial.materialPasses[0].vertexShaderHandle = p_RenderBackEnd->resources.shaders.screenspaceColoredVertexProgramHandle;
	K15_CreateRenderMaterialDataDescStretchBufferWithCustomAllocator(&defaultGUIMaterial.materialPasses[0].materialData, *p_Allocator, 8);

	p_RenderBackEnd->resources.materials.defaultGUIMaterial = defaultGUIMaterial;
}
/*********************************************************************************/
intern void K15_InternalLoadBackEndStockShader(K15_RenderBackEnd* p_RenderBackEnd, K15_ResourceContext* p_ResourceContext, K15_CustomMemoryAllocator* p_Allocator)
{
	K15_ResourceHandle defaultColorFragmentProgramResourceHandle = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, "/shader/stock/default_color.frag", 0);
	K15_ResourceHandle defaultTextureFragmentProgramResourceHandle = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, "/shader/stock/default_texture.frag", 0);
	K15_ResourceHandle defaultFontFragmentProgramResourceHandle = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, "/shader/stock/default_font.frag", 0);
	K15_ResourceHandle screenspaceColoredVertexProgramResourceHandle = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, "/shader/stock/screenspace_colored.vert", 0);
	K15_ResourceHandle screenspaceTexturedVertexProgramResourceHandle = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, "/shader/stock/screenspace_textured.vert", 0);
	K15_ResourceHandle screenspaceTexturedColoredVertexProgramResourcehandle = K15_LoadResource(p_ResourceContext, K15_SHADER_RESOURCE_IDENTIFIER, "/shader/stock/screenspace_textured_colored.vert", 0);

	p_RenderBackEnd->resources.shaders.defaultColorFragmentProgramHandle = K15_GetResourceRenderHandle(p_ResourceContext, defaultColorFragmentProgramResourceHandle);
	p_RenderBackEnd->resources.shaders.defaultTextureFragmentProgramHandle = K15_GetResourceRenderHandle(p_ResourceContext, defaultTextureFragmentProgramResourceHandle);
	p_RenderBackEnd->resources.shaders.defaultFontFragmentProgramHandle = K15_GetResourceRenderHandle(p_ResourceContext, defaultFontFragmentProgramResourceHandle);
	p_RenderBackEnd->resources.shaders.screenspaceColoredVertexProgramHandle = K15_GetResourceRenderHandle(p_ResourceContext, screenspaceColoredVertexProgramResourceHandle);
	p_RenderBackEnd->resources.shaders.screenspaceTexturedVertexProgramHandle = K15_GetResourceRenderHandle(p_ResourceContext, screenspaceTexturedVertexProgramResourceHandle);
	p_RenderBackEnd->resources.shaders.screenspaceTexturedColoredVertexProgramHandle = K15_GetResourceRenderHandle(p_ResourceContext, screenspaceTexturedColoredVertexProgramResourcehandle);

	//default 2D material
	K15_InternalCreateDefault2DMaterial(p_RenderBackEnd, p_Allocator);

	//default font material
	K15_InternalCreateDefaultFontMaterial(p_RenderBackEnd, p_Allocator);

	//default gui material
	K15_InternalCreateDefaultGUIMaterial(p_RenderBackEnd, p_Allocator);
}
/*********************************************************************************/
intern void K15_InternalLoadDefaultSampler(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_RenderSamplerDesc linearClampDesc = {};
	K15_RenderSamplerDesc nearestClampDesc = {};

	//linear clamp
	{
		linearClampDesc.address.u = K15_RENDER_FILTER_ADDRESS_MODE_CLAMP;
		linearClampDesc.address.v = K15_RENDER_FILTER_ADDRESS_MODE_CLAMP;
		linearClampDesc.address.w = K15_RENDER_FILTER_ADDRESS_MODE_CLAMP;
		linearClampDesc.filtering.minification = K15_RENDER_FILTER_MODE_LINEAR;
		linearClampDesc.filtering.minification = K15_RENDER_FILTER_MODE_LINEAR;
		linearClampDesc.nameHash = K15_GenerateStringHash("LinearClampStockSampler");
	}

	//nearest clamp
	{
		nearestClampDesc.address.u = K15_RENDER_FILTER_ADDRESS_MODE_CLAMP;
		nearestClampDesc.address.v = K15_RENDER_FILTER_ADDRESS_MODE_CLAMP;
		nearestClampDesc.address.w = K15_RENDER_FILTER_ADDRESS_MODE_CLAMP;
		nearestClampDesc.filtering.minification = K15_RENDER_FILTER_MODE_NEAREST;
		nearestClampDesc.filtering.minification = K15_RENDER_FILTER_MODE_NEAREST;
		nearestClampDesc.nameHash = K15_GenerateStringHash("NearestClampStockSampler");
	}

	p_RenderBackEnd->renderInterface.createSamplerFromSamplerDesc(p_RenderBackEnd, &linearClampDesc, &p_RenderBackEnd->resources.samplers.linearClampSamplerHandle);
	p_RenderBackEnd->renderInterface.createSamplerFromSamplerDesc(p_RenderBackEnd, &nearestClampDesc, &p_RenderBackEnd->resources.samplers.nearestClampSamplerHandle);
}
/*********************************************************************************/
intern void K15_InternalLoadDefaultVertexFormats(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_RenderVertexFormatDesc fontVertexFormatDesc = K15_CreateRenderVertexFormatDesc(p_RenderBackEnd->renderContext, 3,
		K15_ATTRIBUTE_SEMANTIC_POSITION, K15_TYPE_FLOAT_VECTOR2,
		K15_ATTRIBUTE_SEMANTIC_TEXCOORD1, K15_TYPE_FLOAT_VECTOR2,
		K15_ATTRIBUTE_SEMANTIC_COLOR1, K15_TYPE_FLOAT_VECTOR3);

	p_RenderBackEnd->resources.vertexFormats.fontVertexFormat = fontVertexFormatDesc;
}
/*********************************************************************************/
intern void K15_InitializeRenderResources(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_CustomMemoryAllocator* rendererAllocator = &p_RenderBackEnd->renderContext->memoryAllocator;

	//immedate vertex buffer
	{
		K15_RenderBufferDesc immediateVertexBufferDesc = {};

		immediateVertexBufferDesc.access = K15_RENDER_BUFFER_ACCESS_WRITE;
		immediateVertexBufferDesc.sizeInBytes = size_megabyte(5);
		immediateVertexBufferDesc.usage = K15_RENDER_BUFFER_USAGE_DYNAMIC_DRAW;
		immediateVertexBufferDesc.type = K15_RENDER_BUFFER_TYPE_VERTEX;
	
		//p_RenderBackEnd->resources.intermediateVertexBuffer = K15_AddRenderBufferDesc(p_RenderBackEnd, &immediateVertexBufferDesc);
	}
	/*********************************************************************************/

	K15_InternalLoadDefaultSampler(p_RenderBackEnd);
	K15_InternalLoadDefaultVertexFormats(p_RenderBackEnd);

	/*********************************************************************************/
	p_RenderBackEnd->renderInterface.setBlendState(p_RenderBackEnd, &p_RenderBackEnd->defaultState.blendState);
	p_RenderBackEnd->renderInterface.setDepthState(p_RenderBackEnd, &p_RenderBackEnd->defaultState.depthState);
	p_RenderBackEnd->renderInterface.setRasterizerState(p_RenderBackEnd, &p_RenderBackEnd->defaultState.rasterizerState);
	p_RenderBackEnd->renderInterface.setStencilState(p_RenderBackEnd, &p_RenderBackEnd->defaultState.stencilState);
	/*********************************************************************************/
}
/*********************************************************************************/
intern result8 K15_InternalWindowResized2(K15_RenderBackEnd* p_RenderBackEnd, float p_Width, float p_Height)
{

}
/*********************************************************************************/


//RENDER COMMANDS
/*********************************************************************************/
intern result8 K15_InternalWindowResized(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	uint32 localOffset = 0;
	uint32 width = 0;
	uint32 height = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(uint32), &width);
	localOffset += sizeof(uint32);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(uint32), &height);
	localOffset += sizeof(uint32);

	p_RenderBackEnd->viewportHeight = (float)height;
	p_RenderBackEnd->viewportWidth = (float)width;
	p_RenderBackEnd->viewportAspectRatio = p_RenderBackEnd->viewportWidth / p_RenderBackEnd->viewportHeight;

	return p_RenderBackEnd->renderInterface.resizeViewport(p_RenderBackEnd, width, height);
}
/*********************************************************************************/
intern void K15_InternalRender2DTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
	{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;
	K15_RenderResourceHandle* textureHandle = 0;
	float posX = 0.f;
	float posY = 0.f;
	float width = 0.f;
	float height = 0.f;

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, K15_PTR_SIZE, &textureHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &posX);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &posY);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &width);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &height);
	localOffset += sizeof(float);

#ifdef K15_TOLERATE_INVALID_GPU_HANDLES
	//ignore non loaded textures
	if (*textureHandle == K15_INVALID_GPU_RESOURCE_HANDLE)
	{
		return;
	}
#endif //K15_TOLERATE_INVALID_GPU_HANDLES

	K15_InternalDrawDefault2DTexture(p_RenderBackEnd, textureHandle, 
		posX, posX + width, posY, posY + height, 0.f, 1.f, 0.f, 1.f);
}
/*********************************************************************************/
intern void K15_InternalRender2DTextureEX(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;
	K15_RenderResourceHandle* textureHandle = 0;
	float posX = 0.f;
	float posY = 0.f;
	float width = 0.f;
	float height = 0.f;
	float texU = 0.f;
	float texV = 0.f;
	float texWidth = 0.f;
	float texHeight = 0.f;

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, K15_PTR_SIZE, &textureHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &posX);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &posY);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &width);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &height);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &texU);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &texV);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &texWidth);
	localOffset += sizeof(float);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, 
		p_BufferOffset + localOffset, sizeof(float), &texHeight);
	localOffset += sizeof(float);

#ifdef K15_TOLERATE_INVALID_GPU_HANDLES
	//ignore non loaded textures
	if (*textureHandle == K15_INVALID_GPU_RESOURCE_HANDLE)
	{
		return;
	}
#endif //K15_TOLERATE_INVALID_GPU_HANDLES

	K15_InternalDrawDefault2DTexture(p_RenderBackEnd, textureHandle, 
		posX, posX + width, posY, posY + height,
		texU, texV, texU + texWidth, texV + texHeight);
}
/*********************************************************************************/
intern void K15_InternalRender2DText(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;

	K15_RenderFontDesc fontDesc = {};
	char* text = 0;

	K15_Rectangle textRect = {};

	uint32 localOffset = 0;
	uint32 textLength = 0;
	uint32 packedColor = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_RenderFontDesc), &fontDesc);
	localOffset += sizeof(K15_RenderFontDesc);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(uint32), &packedColor);
	localOffset += sizeof(uint32);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(uint32), &textLength);
	localOffset += sizeof(uint32);

	text = (char*)alloca(textLength + 1);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, textLength, text);
	localOffset += textLength;

	text[textLength] = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_Rectangle), &textRect);
	localOffset += sizeof(K15_Rectangle);


#ifdef K15_TOLERATE_INVALID_GPU_HANDLES
	//ignore non loaded textures
	if (*fontDesc.textureHandle == K15_INVALID_GPU_RESOURCE_HANDLE)
	{
		return;
	}
#endif //K15_TOLERATE_INVALID_GPU_HANDLES

	K15_RenderMaterialDesc* fontMaterial = &p_RenderBackEnd->resources.materials.defaultFontMaterial;
	//K15_SetRenderMaterialRenderResourceDataByName(&fontMaterial->materialPasses[0], "sampler", fontDesc.samplerHandle);
	K15_SetRenderMaterialRenderResourceDataByName(&fontMaterial->materialPasses[0], "tex", fontDesc.textureHandle);

	uint32 numVertices = textLength * 6;
	uint32 vertexIndex = 0;
	uint32 indexBufferIndex = 0;
	uint16 index = 0;

	K15_RenderVertexFormatDesc vertexFormatDesc = p_RenderBackEnd->resources.vertexFormats.fontVertexFormat;

	uint32 sizeVerticesInBytes = vertexFormatDesc.stride * numVertices;

	float* vertexMemory = (float*)K15_AllocateFromMemoryAllocator(renderAllocator, sizeVerticesInBytes);

	if (textRect.rightPos == FLT_MAX || textRect.bottomPos == FLT_MAX)
	{
		K15_InternalPush2DScreenspacePixelColoredTextVertices(&fontDesc, vertexMemory, 0,
			textRect.leftPos, textRect.topPos, packedColor, text, textLength);
	}
	else
	{
		K15_InternalPush2DScreenspacePixelColoredTextVertices(&fontDesc, vertexMemory, 0,
			textRect.leftPos, textRect.topPos, textRect.rightPos, textRect.bottomPos,
			packedColor, text, textLength);
	}

	K15_RenderVertexData* vertexData = p_RenderBackEnd->renderInterface.updateVertexData(p_RenderBackEnd, vertexMemory, numVertices, &vertexFormatDesc);
	K15_RenderGeometryDesc renderGeometry = {};

	renderGeometry.vertexData = vertexData;
	renderGeometry.topology = K15_RENDER_TOPOLOGY_TRIANGLES;
	renderGeometry.worldMatrix = K15_GetIdentityMatrix4();
	renderGeometry.material = fontMaterial;

	K15_InternalDrawGeometry(p_RenderBackEnd, &renderGeometry);

	p_RenderBackEnd->renderInterface.freeVertexData(p_RenderBackEnd, vertexData);
	K15_FreeFromMemoryAllocator(renderAllocator, vertexMemory);
}
/*********************************************************************************/
intern void K15_InternalRender2DGUI(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;

	K15_GUIContext guiContext = {};
	uint32 guiMemorySize = 0;
	byte* guiMemoryBuffer = 0;

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, 
		sizeof(K15_GUIContext), &guiContext);
	localOffset += sizeof(K15_GUIContext);

	uint32 currentGUIMemoryOffset = 0;

	K15_Matrix4 orthoProj = K15_CreateOrthographicProjectionMatrix(p_RenderBackEnd->viewportWidth, p_RenderBackEnd->viewportHeight);
	K15_UpdateUniformCacheEntry(&p_RenderBackEnd->uniformCache, K15_UNIFORM_SEMANTIC_PROJECTION_MATRIX,
		(byte*)&orthoProj);

	/*K15_GetTransformedVector(K15_CreateVector(400, 400, 1), orthoProj);*/

	K15_RenderVertexFormatDesc vertexFormatP3C3 = K15_CreateRenderVertexFormatDesc(p_RenderBackEnd->renderContext, 2, 
		K15_ATTRIBUTE_SEMANTIC_POSITION, K15_TYPE_FLOAT_VECTOR2,
		K15_ATTRIBUTE_SEMANTIC_COLOR1, K15_TYPE_FLOAT_VECTOR3);
	
	K15_RenderVertexFormatDesc vertexFormatP3T2C3 = K15_CreateRenderVertexFormatDesc(p_RenderBackEnd->renderContext, 3,
		K15_ATTRIBUTE_SEMANTIC_POSITION, K15_TYPE_FLOAT_VECTOR2,
		K15_ATTRIBUTE_SEMANTIC_TEXCOORD1, K15_TYPE_FLOAT_VECTOR2,
		K15_ATTRIBUTE_SEMANTIC_COLOR1, K15_TYPE_FLOAT_VECTOR3);

	//count vertices
	K15_GUIDrawInformation guiDrawInfo = {};
	K15_GUIIterateRetainedElements(&guiContext, K15_InternalCountGUIElementVertices, &guiDrawInfo);

	//early out
	if (guiDrawInfo.numVerticesP3C3 == 0 && guiDrawInfo.numVerticesP3T2C3 == 0)
	{
		goto free_resources;
	}

	guiDrawInfo.vertexBufferP3C3 = guiDrawInfo.numVerticesP3C3 == 0 ? 0 :
		(float*)malloc(guiDrawInfo.numVerticesP3C3 * vertexFormatP3C3.stride);

	guiDrawInfo.vertexBufferP3T2C3 = guiDrawInfo.numVerticesP3T2C3 == 0 ? 0 :
		(float*)malloc(guiDrawInfo.numVerticesP3T2C3 * vertexFormatP3T2C3.stride);

	//fill vertex buffer
	K15_GUIIterateRetainedElements(&guiContext, K15_InternalPushGUIElementVertices, &guiDrawInfo);

	//signal semaphore so the retained memory can be used again
	K15_PostSemaphore(guiContext.memoryLock);

	uint32 actualNumberOfVerticesP3C3 = 
		(guiDrawInfo.numFloatsVertexBufferP3C3 * sizeof(float)) / vertexFormatP3C3.stride;
	uint32 actualNumberOfVerticesP3T2C3	= 
		(guiDrawInfo.numFloatsVertexBufferP3T2C3 * sizeof(float)) / vertexFormatP3T2C3.stride;

	K15_ASSERT(actualNumberOfVerticesP3C3 <= guiDrawInfo.numVerticesP3C3 && 
		actualNumberOfVerticesP3T2C3 <= guiDrawInfo.numVerticesP3T2C3);

	K15_RenderMaterialDesc* guiMaterial = &p_RenderBackEnd->resources.materials.defaultGUIMaterial;
	K15_RenderMaterialDesc* fontMaterial = &p_RenderBackEnd->resources.materials.defaultFontMaterial;

	K15_RenderFontDesc* guiStyleFont = guiContext.style.windowStyle.font;

	//render gui
	if (guiDrawInfo.numFloatsVertexBufferP3C3 > 0)
	{
		K15_RenderVertexData* vertexDataP3C3 = p_RenderBackEnd->renderInterface.updateVertexData(p_RenderBackEnd,
			guiDrawInfo.vertexBufferP3C3, actualNumberOfVerticesP3C3, &vertexFormatP3C3);

		K15_RenderGeometryDesc renderGeometry = {};

		renderGeometry.vertexData = vertexDataP3C3;
		renderGeometry.topology = K15_RENDER_TOPOLOGY_TRIANGLES;
		renderGeometry.worldMatrix = K15_GetIdentityMatrix4();
		renderGeometry.material = guiMaterial;

		K15_InternalDrawGeometry(p_RenderBackEnd, &renderGeometry);
		p_RenderBackEnd->renderInterface.freeVertexData(p_RenderBackEnd, vertexDataP3C3);
	}
	
	K15_SetRenderMaterialRenderResourceDataByName(&fontMaterial->materialPasses[0], "tex", guiStyleFont->textureHandle);

	//render gui text
	if (guiDrawInfo.numVerticesP3T2C3 > 0)
	{
		K15_RenderVertexData* vertexDataP3T2C3 = p_RenderBackEnd->renderInterface.updateVertexData(p_RenderBackEnd,
			guiDrawInfo.vertexBufferP3T2C3, actualNumberOfVerticesP3T2C3, &vertexFormatP3T2C3);

		K15_RenderGeometryDesc textGeometry = {};

		textGeometry.vertexData = vertexDataP3T2C3;
		textGeometry.topology = K15_RENDER_TOPOLOGY_TRIANGLES;
		textGeometry.worldMatrix = K15_GetIdentityMatrix4();
		textGeometry.material = fontMaterial;

		K15_InternalDrawGeometry(p_RenderBackEnd, &textGeometry);
		p_RenderBackEnd->renderInterface.freeVertexData(p_RenderBackEnd, vertexDataP3T2C3);
	}


free_resources:
	//signal gui context that we are finished so that it can flip the buffers again
	//K15_PostSemaphore(guiContext.memoryLock);
	free(guiDrawInfo.vertexBufferP3C3);
	free(guiDrawInfo.vertexBufferP3T2C3);
	//K15_FreeFromMemoryAllocator(renderAllocator, vertexBuffer);S
}
/*********************************************************************************/
intern void K15_InternalCreateTextureFromTextureFormat(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle* textureHandle = 0;
	K15_TextureFormat textureFormat = {};

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, K15_PTR_SIZE, &textureHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_TextureFormat), &textureFormat);
	localOffset += sizeof(K15_TextureFormat);
 
	K15_CustomMemoryAllocator* renderAllocator = &p_RenderBackEnd->renderContext->memoryAllocator;

	K15_RenderTextureDesc textureDesc = {};
	textureDesc.createMipChain = textureFormat.mipMapCount <= 1;
	textureDesc.nameHash = textureFormat.textureNameHash;
	textureDesc.dimension.height = textureFormat.height;
	textureDesc.dimension.width = textureFormat.width;
	textureDesc.mipmaps.count = textureFormat.mipMapCount;
	textureDesc.nameHash = textureFormat.textureNameHash;

// 	textureDesc.mipmaps.data = (byte**)K15_AllocateFromMemoryAllocator(renderAllocator, K15_PTR_SIZE * textureFormat.mipMapCount);
// 	textureDesc.mipmaps.dataSize = (uint32*)K15_AllocateFromMemoryAllocator(renderAllocator, sizeof(uint32) * textureFormat.mipMapCount);

	for (uint32 imageIndex = 0;
		imageIndex < textureFormat.mipMapCount;
		++imageIndex)
	{
		textureDesc.mipmaps.dataSize[imageIndex] = K15_GetTextureMipMapSize(&textureFormat, imageIndex);
		textureDesc.mipmaps.data[imageIndex] = K15_GetTextureMipMap(&textureFormat, imageIndex);
	}

	textureDesc.format = K15_ConvertToRenderFormat(textureFormat.pixelFormat);
	textureDesc.type = K15_RENDER_TEXTURE_TYPE_2D;
	textureDesc.flags = K15_RENDER_DESC_AUTO_CLEANUP_FLAG;

	p_RenderBackEnd->renderInterface.createTextureFromTextureDesc(p_RenderBackEnd, &textureDesc, textureHandle);
}
/*********************************************************************************/
intern void K15_InternalCreateTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderTextureDesc renderTextureDesc = {};
	K15_RenderResourceHandle* renderTextureHandle = 0;

	uint32 localOffset = 0;
	
	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, K15_PTR_SIZE, &renderTextureHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_RenderTextureDesc), &renderTextureDesc);
	localOffset += sizeof(K15_RenderTextureDesc);

	p_RenderBackEnd->renderInterface.createTextureFromTextureDesc(p_RenderBackEnd, &renderTextureDesc, renderTextureHandle);
}
/*********************************************************************************/
intern void K15_InternalCreateSamplerFromSamplerFormat(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle* samplerHandle = 0;
	K15_SamplerFormat samplerFormat = {};

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, K15_PTR_SIZE, &samplerHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_SamplerFormat), &samplerFormat);
	localOffset += sizeof(K15_SamplerFormat);

	K15_RenderSamplerDesc samplerDesc = {};

	samplerDesc.nameHash = samplerFormat.samplerNameHash;
	samplerDesc.address.u = (K15_RenderFilterAddressMode)samplerFormat.addressModeU;
	samplerDesc.address.v = (K15_RenderFilterAddressMode)samplerFormat.addressModeV;
	samplerDesc.address.w = (K15_RenderFilterAddressMode)samplerFormat.addressModeW;
	samplerDesc.filtering.magnification =  (K15_RenderFilterMode)samplerFormat.magnificationFilter;
	samplerDesc.filtering.minification =  (K15_RenderFilterMode)samplerFormat.minificationFilter;

	p_RenderBackEnd->renderInterface.createSamplerFromSamplerDesc(p_RenderBackEnd, &samplerDesc, samplerHandle);
}
/*********************************************************************************/
intern result8 K15_InternalCreateShader(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle* shaderHandle = 0;
	K15_RenderProgramDesc programDesc = {};

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, K15_PTR_SIZE, &shaderHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_RenderProgramDesc), &programDesc);
	localOffset += sizeof(K15_SamplerFormat);

	result8 result = p_RenderBackEnd->renderInterface.createProgram(p_RenderBackEnd, &programDesc, shaderHandle);

	if (result != K15_SUCCESS)
	{
		char* error = p_RenderBackEnd->renderContext->error;
		K15_LOG_ERROR_MESSAGE("Could not create shader '%s' ('%s')", programDesc.name, error);
	}

	return result;
}
/*********************************************************************************/
intern result8 K15_InternalDeleteTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle textureHandle = 0;
	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_RenderResourceHandle), &textureHandle);
	localOffset += sizeof(K15_RenderResourceHandle);

	result8 result = p_RenderBackEnd->renderInterface.deleteTexture(p_RenderBackEnd, textureHandle);
	return result;
}
/*********************************************************************************/
intern result8 K15_InternalDeleteSampler(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle samplerHandle = 0;
	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_RenderResourceHandle), &samplerHandle);
	localOffset += sizeof(K15_RenderResourceHandle);

	result8 result = p_RenderBackEnd->renderInterface.deleteSampler(p_RenderBackEnd, samplerHandle);
	return result;
}
/*********************************************************************************/
intern result8 K15_InternalDeleteProgram(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle programHandle = 0;
	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_RenderResourceHandle), &programHandle);
	localOffset += sizeof(K15_RenderResourceHandle);

	result8 result = p_RenderBackEnd->renderInterface.deleteProgram(p_RenderBackEnd, programHandle);
	return result;
}
/*********************************************************************************/



/*********************************************************************************/
void K15_ProcessRenderCommands(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommandBuffer* p_RenderCommandBuffer)
{
	K15_ASSERT_TEXT(p_RenderBackEnd, "Render Back End is NULL.");
	K15_ASSERT_TEXT(p_RenderCommandBuffer, "Render Command Buffer is NULL.");

	uint32 offset = 0;
	uint32 size = p_RenderCommandBuffer->offset;

	K15_RenderCommand* command = 0;

	while(offset != size)
	{
		//grab render command from the buffer
		command = (K15_RenderCommand*)(p_RenderCommandBuffer->buffer + offset);
		offset += sizeof(K15_RenderCommand);

		switch(command->type)
		{
			case K15_RENDER_COMMAND_RENDER_2D_TEXTURE:
			{
				K15_InternalRender2DTexture(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_RENDER_2D_TEXTURE_EX:
			{
				K15_InternalRender2DTextureEX(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_RENDER_2D_TEXT:
			{
				K15_InternalRender2DText(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_RENDER_2D_GUI:
			{
				K15_InternalRender2DGUI(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_CREATE_TEXTURE_FROM_TEXTURE_FORMAT:
			{
				K15_InternalCreateTextureFromTextureFormat(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_CREATE_TEXTURE:
			{
				K15_InternalCreateTexture(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT:
			{
				K15_InternalCreateSamplerFromSamplerFormat(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_CREATE_PROGRAM:
			{
				K15_InternalCreateShader(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_DELETE_TEXTURE:
			{
				K15_InternalDeleteTexture(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_DELETE_SAMPLER:
			{
				K15_InternalDeleteSampler(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_DELETE_PROGRAM:
			{
				K15_InternalDeleteProgram(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_WINDOW_RESIZED:
			{
				K15_InternalWindowResized(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}
		}

		//advance offset
		offset += command->parameterSize;
	}
}	
/*********************************************************************************/
void K15_BeginFrame(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_ASSERT_TEXT(p_RenderBackEnd, "Render Back End is NULL.");

	p_RenderBackEnd->renderInterface.clearScreen(p_RenderBackEnd);
}
/*********************************************************************************/
intern inline void K15_InternalDraw2DDebugGeometry(K15_RenderBackEnd* p_RenderBackEnd, 
	K15_RenderMaterialDesc* p_Material, K15_RenderVertexFormatDesc* p_VertexFormat,
	float* p_VertexBuffer, uint32 p_NumVertices)
{
	K15_Matrix4 orthoProj = K15_CreateOrthographicProjectionMatrix(p_RenderBackEnd->viewportWidth,
		p_RenderBackEnd->viewportHeight);

	K15_UpdateUniformCacheEntry(&p_RenderBackEnd->uniformCache,
		K15_UNIFORM_SEMANTIC_PROJECTION_MATRIX, (byte*)&orthoProj);

	K15_RenderVertexData* vertexData = p_RenderBackEnd->renderInterface.updateVertexData(p_RenderBackEnd,
		p_VertexBuffer, p_NumVertices, p_VertexFormat);

	K15_RenderGeometryDesc renderGeometry = {};
	renderGeometry.vertexData = vertexData;
	renderGeometry.worldMatrix = K15_GetIdentityMatrix4();
	renderGeometry.topology = K15_RENDER_TOPOLOGY_TRIANGLES;
	renderGeometry.material = p_Material;

	p_RenderBackEnd->renderInterface.drawGeometry(p_RenderBackEnd, &renderGeometry);

	free(vertexData);
}
/*********************************************************************************/
intern inline void K15_InternalDrawDebugGeometry(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_DebugRenderContext* debugRenderContext = renderContext->debugRenderContext;

	if (debugRenderContext->num3DFloats > 0)
	{
		//K15_InternalDraw3DDebugGeometry();
		debugRenderContext->num3DFloats = 0;
	}

	if (debugRenderContext->num2DFloats> 0)
	{
		uint32 numVertices = debugRenderContext->num2DFloats * sizeof(float)
			/ debugRenderContext->debug2DVertexFormat.stride;

		K15_InternalDraw2DDebugGeometry(p_RenderBackEnd, debugRenderContext->debugMaterial,
			&debugRenderContext->debug2DVertexFormat, debugRenderContext->debug2DVertexBuffer,
			numVertices);

		debugRenderContext->num2DFloats = 0;
	}
}
/*********************************************************************************/
void K15_EndFrame(K15_RenderBackEnd * p_RenderBackEnd)
{
	K15_ASSERT_TEXT(p_RenderBackEnd, "Render Back End is NULL.");

	K15_InternalDrawDebugGeometry(p_RenderBackEnd);

	p_RenderBackEnd->renderInterface.swapFrameBuffer(p_RenderBackEnd);
}
/*********************************************************************************/