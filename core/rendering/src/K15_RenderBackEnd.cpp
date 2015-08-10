#include "K15_RenderBackEnd.h"

#include "K15_TextureFormat.h"
#include "K15_SamplerFormat.h"

#include "K15_Rectangle.h"
#include "K15_Vector2.h"

#include "K15_RenderCommandBuffer.h"
#include "K15_ErrorCodes.h"
#include "K15_RenderMaterialDesc.h"

#include "K15_RenderFormats.cpp"

/*********************************************************************************/
intern void K15_InitializeCachedRenderMemory(K15_RenderCachedResources* p_RenderCachedResources)
{
	//clear memory
	memset(&p_RenderCachedResources->bufferDescs, 0, sizeof(p_RenderCachedResources->bufferDescs));
	memset(&p_RenderCachedResources->programDescs, 0, sizeof(p_RenderCachedResources->programDescs));
	memset(&p_RenderCachedResources->textureDescs, 0, sizeof(p_RenderCachedResources->textureDescs));
	memset(&p_RenderCachedResources->samplerDescs, 0, sizeof(p_RenderCachedResources->samplerDescs));
	memset(&p_RenderCachedResources->renderTargetDescs, 0, sizeof(p_RenderCachedResources->renderTargetDescs));
}
/*********************************************************************************/
intern void K15_CreateDefaultRenderState(K15_RenderState* p_RenderState)
{
	K15_RenderBlendStateDesc defaultBlendState = {};
	K15_RenderStencilStateDesc defaultStencilState = {};
	K15_RenderDepthStateDesc defaultDepthState = {};
	K15_RenderRasterizerStateDesc defaultRasterizerState = {};

	/*********************************************************************************/
	defaultRasterizerState.cullingMode = K15_CULLING_MODE_BACK;
	defaultRasterizerState.fillMode = K15_FILLMODE_SOLID;
	defaultRasterizerState.scissoringEnabled = FALSE;
	defaultRasterizerState.vertexOrder = K15_VERTEX_ORDER_COUNTER_CLOCKWISE;
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
}
/*********************************************************************************/
intern result8 K15_SetDefaultRenderState(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_ASSERT(p_RenderBackEnd);
	K15_ASSERT_TEXT(p_RenderBackEnd->interfaceInitialized, "Render Interface it not yet initialized.");

	if (p_RenderBackEnd->interfaceInitialized)
	{
		K15_RenderState* defaultState = &p_RenderBackEnd->defaultState;
		p_RenderBackEnd->interface.setBlendState(p_RenderBackEnd, &defaultState->blendState);
	}
}
/*********************************************************************************/
intern result8 K15_InitializeRenderBackEnd(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderContext* p_RenderContext, K15_RenderBackEnd* p_RenderBackEnd)
{
	p_RenderBackEnd->interfaceInitialized = K15_FALSE;

	K15_InitializeCachedRenderMemory(&p_RenderBackEnd->cachedResources);
	K15_CreateDefaultRenderState(&p_RenderBackEnd->defaultState);

	p_RenderBackEnd->renderContext = p_RenderContext;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern K15_RenderResourceHandle K15_AddRenderBufferDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBufferDesc* p_RenderBufferDesc)
{
	K15_RenderResourceHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 bufferDescIndex = 0;
		bufferDescIndex < K15_RENDER_MAX_GPU_BUFFER;
		++bufferDescIndex)
	{
		if (!p_RenderBackEnd->cachedResources.bufferDescs[bufferDescIndex])
		{
			p_RenderBackEnd->cachedResources.bufferDescs[bufferDescIndex] = p_RenderBufferDesc;
			handle = (K15_RenderResourceHandle)p_RenderBufferDesc;

			break;
		}
	}

	p_RenderBackEnd->interface.createBuffer(p_RenderBackEnd, p_RenderBufferDesc, &handle);

	return handle;
}
/*********************************************************************************/
intern K15_RenderResourceHandle K15_AddRenderTextureDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderTextureDesc* p_RenderTextureDesc)
{
	K15_RenderResourceHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 textureDescIndex = 0;
		textureDescIndex < K15_RENDER_MAX_GPU_TEXTURES;
		++textureDescIndex)
	{
		if (!p_RenderBackEnd->cachedResources.textureDescs[textureDescIndex])
		{
			p_RenderBackEnd->cachedResources.textureDescs[textureDescIndex] = p_RenderTextureDesc;
			handle = (K15_RenderResourceHandle)textureDescIndex;

			break;
		}
	}

	p_RenderBackEnd->interface.createTexture(p_RenderBackEnd, p_RenderTextureDesc, &handle);

	return handle;
}
/*********************************************************************************/
intern K15_RenderResourceHandle K15_AddRenderSamplerDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderSamplerDesc* p_RenderSamplerDesc)
{
	K15_RenderResourceHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 samplerDescIndex = 0;
		samplerDescIndex < K15_RENDER_MAX_GPU_SAMPLERS;
		++samplerDescIndex)
	{
		if (!p_RenderBackEnd->cachedResources.samplerDescs[samplerDescIndex])
		{
			p_RenderBackEnd->cachedResources.samplerDescs[samplerDescIndex] = p_RenderSamplerDesc;
			handle = (K15_RenderResourceHandle)samplerDescIndex;

			break;
		}
	}

	p_RenderBackEnd->interface.createSampler(p_RenderBackEnd, p_RenderSamplerDesc, &handle);

	return handle;
}
/*********************************************************************************/
intern void* K15_AllocateFromRenderBuffer(K15_RenderResourceHandle p_ResourceHandle, uint32 p_SizeInBytes)
{
	//TODO
	return 0;
}
/*********************************************************************************/
intern void K15_InitializeRenderResources(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_RenderBufferDesc immediateVertexBufferDesc = {};

	immediateVertexBufferDesc.access = K15_RENDER_BUFFER_ACCESS_WRITE;
	immediateVertexBufferDesc.sizeInBytes = size_megabyte(5);
	immediateVertexBufferDesc.usage = K15_RENDER_BUFFER_USAGE_DYNAMIC_DRAW;
	immediateVertexBufferDesc.type = K15_RENDER_BUFFER_TYPE_VERTEX;

	p_RenderBackEnd->resources.intermediateVertexBuffer = K15_AddRenderBufferDesc(p_RenderBackEnd, &immediateVertexBufferDesc);
}
/*********************************************************************************/



//RENDER COMMANDS
/*********************************************************************************/
intern void K15_InternalRender2DTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle* textureHandle = 0;
	K15_RenderMaterialDesc* renderMaterialDesc = 0;
	K15_Rectangle destinationRect = {};
	K15_Rectangle sourceRect = {};

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset, K15_PTR_SIZE, &textureHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset, sizeof(K15_RenderMaterialDesc), renderMaterialDesc);
	localOffset += sizeof(K15_RenderMaterialDesc);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset, sizeof(K15_Rectangle), &destinationRect);
	localOffset += sizeof(K15_Rectangle);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset, sizeof(K15_Rectangle), &sourceRect);
	localOffset += sizeof(K15_Rectangle);

	uint32 sizeVerticesInBytes = 512; //512 byte = 4 vertices * 4 float (pos x,y + tex u,v)
	uint32 index = 0;
	float* vertexMemory = (float*)K15_AllocateFromRenderBuffer(p_RenderBackEnd->resources.intermediateVertexBuffer, sizeVerticesInBytes);
	
	K15_Vector2 d_ul = K15_GetUpperLeftCorner(destinationRect);
	K15_Vector2 d_bl = K15_GetBottomLeftCorner(destinationRect);
	K15_Vector2 d_ur = K15_GetUpperRightCorner(destinationRect);
	K15_Vector2 d_br = K15_GetBottomRightCorner(destinationRect);

	K15_Vector2 s_ul = K15_GetUpperLeftCorner(sourceRect);
	K15_Vector2 s_bl = K15_GetBottomLeftCorner(sourceRect);
	K15_Vector2 s_ur = K15_GetUpperRightCorner(sourceRect);
	K15_Vector2 s_br = K15_GetBottomRightCorner(sourceRect);

	//vertex 1
	vertexMemory[index++] = d_ul.x;
	vertexMemory[index++] = d_ul.y;
	vertexMemory[index++] = s_ul.x;
	vertexMemory[index++] = s_ul.y;

	//vertex 2
	vertexMemory[index++] = d_bl.x;
	vertexMemory[index++] = d_bl.y;
	vertexMemory[index++] = s_bl.x;
	vertexMemory[index++] = s_bl.y;

	//vertex 3
	vertexMemory[index++] = d_br.x;
	vertexMemory[index++] = d_br.y;
	vertexMemory[index++] = s_br.x;
	vertexMemory[index++] = s_br.y;

	//vertex 3
	vertexMemory[index++] = d_ur.x;
	vertexMemory[index++] = d_ur.y;
	vertexMemory[index++] = s_ur.x;
	vertexMemory[index++] = s_ur.y;



	//	p_RenderBackEnd->interface.drawElements()
}
/*********************************************************************************/
intern void K15_InternalCreateTextureFromTextureFormat(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle* textureHandle = 0;
	K15_TextureFormat* textureFormat = 0;

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset, K15_PTR_SIZE, &textureHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset, sizeof(K15_TextureFormat), textureFormat);
	localOffset += sizeof(K15_TextureFormat);

	K15_CustomMemoryAllocator* renderAllocator = &p_RenderBackEnd->renderContext->memoryAllocator;

	K15_RenderTextureDesc textureDesc = {};
	textureDesc.createMipChain = K15_FALSE;
	/*textureDesc.dimension.depth = p_TextureFormat->depth*/
	textureDesc.dimension.height = textureFormat->height;
	textureDesc.dimension.width = textureFormat->width;
	textureDesc.mipmaps.count = textureFormat->mipMapCount;
	textureDesc.nameHash = textureFormat->textureNameHash;

	textureDesc.mipmaps.data = (byte**)K15_AllocateFromMemoryAllocator(renderAllocator, K15_PTR_SIZE * textureFormat->mipMapCount);
	textureDesc.mipmaps.dataSize = (uint32*)K15_AllocateFromMemoryAllocator(renderAllocator, sizeof(uint32) * textureFormat->mipMapCount);

	for (uint32 imageIndex = 0;
		imageIndex < textureFormat->mipMapCount;
		++imageIndex)
	{
		textureDesc.mipmaps.dataSize[imageIndex] = K15_GetTextureMipMapSize(textureFormat, imageIndex);
		textureDesc.mipmaps.data[imageIndex] = K15_GetTextureMipMap(textureFormat, imageIndex);
	}

	textureDesc.format = K15_ConvertToRenderFormat(textureFormat->pixelFormat);
	textureDesc.type = K15_RENDER_TEXTURE_TYPE_2D;
	textureDesc.flags = K15_RENDER_DESC_AUTO_CLEANUP_FLAG;

	*textureHandle = K15_AddRenderTextureDesc(p_RenderBackEnd, &textureDesc);
}
/*********************************************************************************/
intern void K15_InternalCreateSamplerFromSamplerFormat(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderResourceHandle* samplerHandle = 0;
	K15_SamplerFormat* samplerFormat = 0;

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset, K15_PTR_SIZE, &samplerHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset, sizeof(K15_SamplerFormat), samplerFormat);
	localOffset += sizeof(K15_SamplerFormat);

	K15_RenderSamplerDesc samplerDesc = {};

	samplerDesc.nameHash = samplerFormat->samplerNameHash;
	samplerDesc.address.u = (K15_RenderFilterAddressMode)samplerFormat->addressModeU;
	samplerDesc.address.v = (K15_RenderFilterAddressMode)samplerFormat->addressModeV;
	samplerDesc.address.w = (K15_RenderFilterAddressMode)samplerFormat->addressModeW;
	samplerDesc.filtering.magnification =  (K15_RenderFilterMode)samplerFormat->magnificationFilter;
	samplerDesc.filtering.minification =  (K15_RenderFilterMode)samplerFormat->minificationFilter;

	*samplerHandle = K15_AddRenderSamplerDesc(p_RenderBackEnd, &samplerDesc);
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

		switch(command->type)
		{
			case K15_RENDER_COMMAND_RENDER_2D_TEXTURE:
			{
				K15_InternalRender2DTexture(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_CREATE_TEXTURE_FROM_TEXTURE_FORMAT:
			{
				K15_InternalCreateTextureFromTextureFormat(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}

			case K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT:
			{
				K15_InternalCreateSamplerFromSamplerFormat(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
				break;
			}
		}

		//advance offset
		offset += sizeof(K15_RenderCommand) + command->parameterSize;
	}
}	
/*********************************************************************************/