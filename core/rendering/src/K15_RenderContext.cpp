#include "K15_RenderContext.h"

#include <K15_OSContext.h>
#include <K15_ErrorCodes.h>
#include <K15_Thread.h>
#include <K15_Window.h>
#include <K15_DefaultCLibraries.h>

#include <K15_Matrix4.h>

#include <K15_GUIContext.h>

#include "K15_RenderProgramDesc.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderSamplerDesc.h"
#include "K15_RenderFontDesc.h"
#include "K15_RenderTargetDesc.h"
#include "K15_RenderCameraDesc.h"
#include "K15_RenderMaterialDesc.h"
#include "K15_RenderViewportDesc.h"
#include "K15_RenderMeshDesc.h"

#include "K15_MeshFormat.h"
#include "K15_FontFormat.h"
#include "K15_SamplerFormat.h"
#include "K15_MaterialFormat.h"

#include "OpenGL/K15_RenderGLContext.h"

#include "K15_RenderUniformCache.cpp"
#include "K15_RenderProgramDesc.cpp"
#include "K15_RenderCommands.cpp"

#include "K15_CustomMemoryAllocator.h"

//#include "generated/K15_2DSceneElementStretchBuffer.cpp"

/*********************************************************************************/
intern inline void K15_InternalConvertCameraDescToInternalCameraDesc(K15_RenderContext* p_RenderContext, K15_RenderCameraDesc* p_RenderCameraDesc, K15_InternalRenderCameraDesc* p_InternalRenderCameraDesc)
{
	assert(p_RenderCameraDesc);
	assert(p_InternalRenderCameraDesc);

	K15_Vector3 position = p_RenderCameraDesc->position;
	K15_Quaternion orientation = p_RenderCameraDesc->orientation;

	float fov = p_RenderCameraDesc->fov;
	float nearPlane = p_RenderCameraDesc->nearPlane;
	float farPlane = p_RenderCameraDesc->farPlane;

	K15_RenderViewportDesc* activeViewport = &p_RenderContext->renderResources.viewportDescs[0];

	float tanHalfFovy = ::tan(fov / 2.f);
	float aspectRatio = (float)activeViewport->height / (float)activeViewport->width;

	K15_Matrix4 projectionMatrix = {};

	projectionMatrix._1_1 = 1.0f / (aspectRatio * tanHalfFovy);
	projectionMatrix._2_2 = 1.0f / tanHalfFovy;
	projectionMatrix._3_3 = - (farPlane + nearPlane) / (farPlane - nearPlane);
	projectionMatrix._4_3 = - 1.f;
	projectionMatrix._3_4 = - (2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

	K15_Matrix4 viewMatrix = K15_GetIdentityMatrix4();//K15_CreateComposedMatrix(position, K15_CreateVector(1.0f, 1.0f, 1.0f), orientation);
	
	p_InternalRenderCameraDesc->projectionMatrix = projectionMatrix;
	p_InternalRenderCameraDesc->viewMatrix = viewMatrix;

	//TODO: RENDER TARGET
}
/*********************************************************************************/
intern inline void K15_InternalCheckRenderBufferDescFlags(K15_RenderBufferDesc* p_RenderBufferDesc)
{
	assert(p_RenderBufferDesc);

	if ((p_RenderBufferDesc->flags & K15_RENDER_DESC_AUTO_CLEANUP_FLAG) > 0
		&& p_RenderBufferDesc->data)
	{
		free(p_RenderBufferDesc->data);
	}
}
/*********************************************************************************/
intern inline void K15_InternalCheckRenderBufferUpdateDescFlags(K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc)
{
	assert(p_RenderBufferUpdateDesc);

	if ((p_RenderBufferUpdateDesc->flags & K15_RENDER_DESC_AUTO_CLEANUP_FLAG) > 0
		&& p_RenderBufferUpdateDesc->data)
	{
		free(p_RenderBufferUpdateDesc->data);
	}
}
/*********************************************************************************/
intern inline void K15_InternalCheckRenderUniformUpdateDescFlags(K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	assert(p_RenderUniformUpdateDesc);

	if ((p_RenderUniformUpdateDesc->flags & K15_RENDER_DESC_AUTO_CLEANUP_FLAG) > 0
		&& p_RenderUniformUpdateDesc->data)
	{
		free(p_RenderUniformUpdateDesc->data);
	}
}
/*********************************************************************************/
intern inline void K15_InternalCheckRenderTextureDescFlags(K15_RenderTextureDesc* p_RenderTextureDesc)
{
	assert(p_RenderTextureDesc);

	if ((p_RenderTextureDesc->flags & K15_RENDER_DESC_AUTO_CLEANUP_FLAG) > 0
		&& p_RenderTextureDesc->mipmaps.data)
	{
		for (uint32 mipmapIndex = 0;
			mipmapIndex < p_RenderTextureDesc->mipmaps.count;
			++mipmapIndex)
		{
			free(p_RenderTextureDesc->mipmaps.data[mipmapIndex]);
		}
		
		free(p_RenderTextureDesc->mipmaps.data);
		free(p_RenderTextureDesc->mipmaps.dataSize);
	}
}
/*********************************************************************************/
intern inline void K15_InternalCheckRenderTextureUpdateDescFlags(K15_RenderTextureUpdateDesc* p_RenderTextureUpdateDesc)
{
	assert(p_RenderTextureUpdateDesc);

	if ((p_RenderTextureUpdateDesc->flags & K15_RENDER_DESC_AUTO_CLEANUP_FLAG) > 0
		&& p_RenderTextureUpdateDesc->data)
	{
		free(p_RenderTextureUpdateDesc->data);
	}
}
/*********************************************************************************/
intern inline void K15_InternalSetViewportDesc(K15_RenderContext* p_RenderContext, K15_RenderViewportDesc *p_Viewport, uint32 p_ViewportIndex)
{
	assert(p_ViewportIndex < K15_RENDER_MAX_VIEWPORTS);

	p_RenderContext->renderResources.viewportDescs[p_ViewportIndex] = *p_Viewport;
}
/*********************************************************************************/
intern inline K15_RenderTextureHandle K15_InternalAddRenderTextureDesc(K15_RenderContext* p_RenderContext, K15_RenderTextureDesc* p_RenderTextureDesc)
{
	K15_RenderTextureHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 textureDescIndex = 0;
		textureDescIndex < K15_RENDER_MAX_GPU_TEXTURES;
		++textureDescIndex)
	{
		//find next free texture desc slot
		if (p_RenderContext->renderResources.textureDescs[textureDescIndex] == 0)
		{
			K15_RenderTextureDesc* renderTextureDescCopy = (K15_RenderTextureDesc*)malloc(sizeof(K15_RenderTextureDesc));
			memcpy(renderTextureDescCopy, p_RenderTextureDesc, sizeof(K15_RenderTextureDesc));

			p_RenderContext->renderResources.textureDescs[textureDescIndex] = renderTextureDescCopy;
			
			handle = (K15_RenderTextureHandle)textureDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline K15_RenderMaterialHandle K15_InternalAddRenderMaterialDesc(K15_RenderContext* p_RenderContext, K15_RenderMaterialDesc* p_RenderMaterialDesc)
{
	K15_RenderMaterialHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 materialDescIndex = 0;
		materialDescIndex < K15_RENDER_MAX_GPU_TEXTURES;
		++materialDescIndex)
	{
		//find next free texture desc slot
		if (p_RenderContext->renderResources.textureDescs[materialDescIndex] == 0)
		{
			K15_RenderMaterialDesc* renderMaterialDescCopy = (K15_RenderMaterialDesc*)malloc(sizeof(K15_RenderMaterialDesc));
			memcpy(renderMaterialDescCopy, p_RenderMaterialDesc, sizeof(K15_RenderMaterialDesc));

			p_RenderContext->renderResources.materialDescs[materialDescIndex] = renderMaterialDescCopy;

			handle = (K15_RenderMaterialHandle)materialDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline K15_RenderProgramHandle K15_InternalAddRenderProgramDesc(K15_RenderContext* p_RenderContext, K15_RenderProgramDesc* p_RenderProgramDesc)
{
	K15_RenderProgramHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 programDescIndex = 0;
		programDescIndex < K15_RENDER_MAX_GPU_PROGRAMS;
		++programDescIndex)
	{
		//find next free program desc slot
		if (p_RenderContext->renderResources.programDescs[programDescIndex] == 0)
		{
			K15_RenderProgramDesc* renderProgramDescCopy = (K15_RenderProgramDesc*)malloc(sizeof(K15_RenderProgramDesc));
			memcpy(renderProgramDescCopy, p_RenderProgramDesc, sizeof(K15_RenderProgramDesc));

			p_RenderContext->renderResources.programDescs[programDescIndex] = renderProgramDescCopy;
			handle = (K15_RenderProgramHandle)programDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline K15_RenderBufferHandle K15_InternalAddRenderBufferDesc(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc)
{
	K15_RenderBufferHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 bufferDescIndex = 0;
		bufferDescIndex < K15_RENDER_MAX_GPU_BUFFER;
		++bufferDescIndex)
	{
		//find next free buffer desc slot
		if (p_RenderContext->renderResources.bufferDescs[bufferDescIndex] == 0)
		{
			K15_RenderBufferDesc* renderBufferDescCopy = (K15_RenderBufferDesc*)malloc(sizeof(K15_RenderBufferDesc));
			memcpy(renderBufferDescCopy, p_RenderBufferDesc, sizeof(K15_RenderBufferDesc));

			p_RenderContext->renderResources.bufferDescs[bufferDescIndex] = renderBufferDescCopy;
			handle = (K15_RenderBufferHandle)bufferDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline K15_RenderSamplerHandle K15_InternalAddRenderSamplerDesc(K15_RenderContext* p_RenderContext, K15_RenderSamplerDesc* p_RenderSamplerDesc)
{
	K15_RenderSamplerHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 samplerDescIndex = 0;
		samplerDescIndex < K15_RENDER_MAX_GPU_SAMPLERS;
		++samplerDescIndex)
	{
		//find next free sampler desc slot
		if (p_RenderContext->renderResources.samplerDescs[samplerDescIndex] == 0)
		{
			K15_RenderSamplerDesc* renderSamplerDescCopy = (K15_RenderSamplerDesc*)malloc(sizeof(K15_RenderSamplerDesc));
			memcpy(renderSamplerDescCopy, p_RenderSamplerDesc, sizeof(K15_RenderSamplerDesc));

			p_RenderContext->renderResources.samplerDescs[samplerDescIndex] = renderSamplerDescCopy;
			handle = (K15_RenderSamplerHandle)samplerDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline K15_RenderTargetHandle K15_InternalAddRenderTargetDesc(K15_RenderContext* p_RenderContext, K15_RenderTargetDesc* p_RenderTargetDesc)
{
	K15_RenderTargetHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 renderTargetDescIndex = 0;
		renderTargetDescIndex < K15_RENDER_MAX_GPU_RENDER_TARGETS;
		++renderTargetDescIndex)
	{
		//find next free sampler desc slot
		if (p_RenderContext->renderResources.renderTargetDescs[renderTargetDescIndex] == 0)
		{
			K15_RenderTargetDesc* renderTargetDescCopy = (K15_RenderTargetDesc*)malloc(sizeof(K15_RenderTargetDesc));
			memcpy(renderTargetDescCopy, p_RenderTargetDesc, sizeof(K15_RenderTargetDesc));

			p_RenderContext->renderResources.renderTargetDescs[renderTargetDescIndex] = renderTargetDescCopy;
			handle = (K15_RenderTargetHandle)renderTargetDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline K15_RenderMeshHandle K15_InternalAddRenderMeshDesc(K15_RenderContext* p_RenderContext, K15_RenderMeshDesc* p_RenderMeshDesc)
{
	K15_RenderMeshHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 meshDescIndex = 0;
		meshDescIndex < K15_RENDER_MAX_GPU_MESHES;
		++meshDescIndex)
	{
		//find next free sampler desc slot
		if (p_RenderContext->renderResources.meshDescs[meshDescIndex] == 0)
		{
			K15_RenderMeshDesc* renderMeshDescCopy = (K15_RenderMeshDesc*)malloc(sizeof(K15_RenderMeshDesc));
			memcpy(renderMeshDescCopy, p_RenderMeshDesc, sizeof(K15_RenderMeshDesc));

			p_RenderContext->renderResources.meshDescs[meshDescIndex] = renderMeshDescCopy;
			handle = (K15_RenderMeshHandle)meshDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline K15_RenderFontHandle K15_InternalAddRenderFontDesc(K15_RenderContext* p_RenderContext, K15_RenderFontDesc* p_RenderFontDesc)
{
	K15_RenderFontHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 fontDescIndex = 0;
		fontDescIndex < K15_RENDER_MAX_GPU_FONTS;
		++fontDescIndex)
	{
		//find next free sampler desc slot
		if (p_RenderContext->renderResources.fontDescs[fontDescIndex] == 0)
		{
			K15_RenderFontDesc* renderFontDescCopy = (K15_RenderFontDesc*)malloc(sizeof(K15_RenderFontDesc));
			memcpy(renderFontDescCopy, p_RenderFontDesc, sizeof(K15_RenderFontDesc));

			p_RenderContext->renderResources.fontDescs[fontDescIndex] = renderFontDescCopy;
			handle = (K15_RenderFontHandle)fontDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline K15_RenderCameraHandle K15_InternalAddRenderCameraDesc(K15_RenderContext* p_RenderContext, K15_RenderCameraDesc* p_RenderCameraDesc)
{
	K15_RenderCameraHandle handle = K15_INVALID_GPU_RESOURCE_HANDLE;

	for (uint32 cameraDescIndex = 0;
		cameraDescIndex < K15_RENDER_MAX_CAMERAS;
		++cameraDescIndex)
	{
		//find next free camera desc slot
		if (p_RenderContext->renderResources.cameras.cameraDescs[cameraDescIndex] == 0)
		{
			K15_InternalRenderCameraDesc* internalCameraDesc = (K15_InternalRenderCameraDesc*)malloc(sizeof(K15_InternalRenderCameraDesc));
			K15_InternalConvertCameraDescToInternalCameraDesc(p_RenderContext, p_RenderCameraDesc, internalCameraDesc);

			p_RenderContext->renderResources.cameras.cameraDescs[cameraDescIndex] = internalCameraDesc;
			handle = (K15_RenderCameraHandle)cameraDescIndex;
			break;
		}
	}

	return handle;
}
/*********************************************************************************/
intern inline uint8 K15_InternalCreateBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandle)
{
	uint8 result = K15_SUCCESS;

	*p_RenderBufferHandle = K15_InternalAddRenderBufferDesc(p_RenderContext, p_RenderBufferDesc);

	result = p_RenderContext->commandProcessing.bufferManagement.createBuffer(p_RenderContext, p_RenderBufferDesc, p_RenderBufferHandle);

	K15_InternalCheckRenderBufferDescFlags(p_RenderBufferDesc);

	return result;
}
/*********************************************************************************/
intern inline uint8 K15_InternalReadParameter(K15_RenderCommandParameterBuffer* p_ParameterFrontBuffer, K15_RenderCommandInstance* p_RenderCommand, uint32 p_ParameterSize, uint32 p_ParameterOffset, void* p_ParameterDestiny)
{
	assert(p_ParameterFrontBuffer);	
	assert(p_ParameterDestiny);		

	uint32 offset = p_ParameterOffset + p_RenderCommand->parameterOffset;

	assert(offset + p_ParameterSize <= p_ParameterFrontBuffer->parameterBufferOffset);

	memcpy(p_ParameterDestiny, p_ParameterFrontBuffer->parameterBuffer + offset, p_ParameterSize);

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_InternalSetRasterizerStateDesc(K15_RenderContext* p_RenderContext, K15_RenderRasterizerStateDesc* p_RasterizerStateDesc)
{
	assert(p_RenderContext && p_RasterizerStateDesc);

	uint8 result = K15_SUCCESS;

	//Check if the new rasterizer state differs from the old rasterizer state. Don't issue a GPU command if they dont differ
	if (memcmp(p_RasterizerStateDesc, p_RenderContext->renderState.rasterizerStateDesc, sizeof(K15_RenderRasterizerStateDesc)) != 0)
	{
		result = p_RenderContext->commandProcessing.stateManagement.setRasterizerState(p_RenderContext, p_RasterizerStateDesc);

		if (result == K15_SUCCESS)
		{
			//overwrite old rasterizer state
			memcpy(p_RenderContext->renderState.rasterizerStateDesc, p_RasterizerStateDesc, sizeof(K15_RenderRasterizerStateDesc));
		}
	}

	return result;
}
/*********************************************************************************/
intern inline uint8 K15_InternalSetDepthStateDesc(K15_RenderContext* p_RenderContext, K15_RenderDepthStateDesc* p_RenderDepthStateDesc)
{
	assert(p_RenderContext && p_RenderDepthStateDesc);

	uint8 result = K15_SUCCESS;

	//Check if the new depth state differs from the old depth state. Don't issue a GPU command if they dont differ
	if (memcmp(p_RenderDepthStateDesc, p_RenderContext->renderState.depthStateDesc, sizeof(K15_RenderDepthStateDesc)) != 0)
	{
		result = p_RenderContext->commandProcessing.stateManagement.setDepthState(p_RenderContext, p_RenderDepthStateDesc);

		if (result == K15_SUCCESS)
		{
			//overwrite old depth state
			memcpy(p_RenderContext->renderState.depthStateDesc, p_RenderDepthStateDesc, sizeof(K15_RenderDepthStateDesc));
		}
	}

	return result;
}
/*********************************************************************************/
intern inline uint8 K15_InternalSetBlendStateDesc(K15_RenderContext* p_RenderContext, K15_RenderBlendStateDesc* p_RenderBlendStateDesc)
{
	assert(p_RenderContext && p_RenderBlendStateDesc);
	
	uint8 result = K15_SUCCESS;

	//Check if the new blend state differs from the old blend state. Don't issue a GPU command if they dont differ
	if (memcmp(p_RenderBlendStateDesc, p_RenderContext->renderState.blendStateDesc, sizeof(K15_RenderBlendStateDesc)) != 0)
	{
		result = p_RenderContext->commandProcessing.stateManagement.setBlendState(p_RenderContext, p_RenderBlendStateDesc);

		if (result == K15_SUCCESS)
		{
			//overwrite old blend state
			memcpy(p_RenderContext->renderState.blendStateDesc, p_RenderBlendStateDesc, sizeof(K15_RenderBlendStateDesc));
		}
	}

	return result;
}

/*********************************************************************************/
intern inline uint8 K15_InternalSetStencilStateDesc(K15_RenderContext* p_RenderContext, K15_RenderStencilStateDesc* p_RenderStencilStateDesc)
{
	assert(p_RenderContext && p_RenderStencilStateDesc);

	uint8 result = K15_SUCCESS;

	//Check if the new stencil state differs from the old stencil state. Don't issue a GPU command if they dont differ
	if (memcmp(p_RenderStencilStateDesc, p_RenderContext->renderState.stencilStateDesc, sizeof(K15_RenderStencilStateDesc)) != 0)
	{
		result = p_RenderContext->commandProcessing.stateManagement.setStencilState(p_RenderContext, p_RenderStencilStateDesc);

		if (result == K15_SUCCESS)
		{
			//overwrite old stencil state
			memcpy(p_RenderContext->renderState.stencilStateDesc, p_RenderStencilStateDesc, sizeof(K15_RenderStencilStateDesc));
		}
	}

	return result;
}
/*********************************************************************************/
intern float K15_InternalGetRenderFontKerning(K15_RenderKerningDesc* p_RenderFontKerningDescs, uint32 p_NumKerningDescs, uint32 p_ShiftedCharacters)
{
	float kerning = 0.f;
	
	uint32 l = 0;
	uint32 r = p_NumKerningDescs;
	uint32 i = 0;

	while (l <= r)
	{
		i = l + ((r - l) / 2);

		K15_RenderKerningDesc* kerningDesc = &p_RenderFontKerningDescs[i];

		if (p_ShiftedCharacters == kerningDesc->shiftedCharacters)
		{
			kerning = kerningDesc->kerning;
			break;
		}
		else if (p_ShiftedCharacters < kerningDesc->shiftedCharacters)
		{
			r = i - 1;
		}
		else
		{
			l = i + 1;
		}
	}
	

	return kerning;
}
/*********************************************************************************/
intern void K15_InternalCalculate2DGlyphRectangle(K15_RenderFontDesc* p_RenderFontDesc, uint16 p_Character, float* p_PosX, float* p_PosY, float* p_Width, float* p_Height)
{
	float fontSize = p_RenderFontDesc->fontSize;
	float verticalSpacing = 0.25f * fontSize;
	float horizontalSpacing = 0.1f * fontSize;

	if (p_Character == '\n')
	{
		*p_Height = fontSize + verticalSpacing;
	}
	else if (isspace(p_Character))
	{
		*p_Width = horizontalSpacing;
	}
	else
	{
		*p_PosX = p_RenderFontDesc->renderGlyphDescs[p_Character].texturePosX;
		*p_PosY = p_RenderFontDesc->renderGlyphDescs[p_Character].texturePosY;
		*p_Width = p_RenderFontDesc->renderGlyphDescs[p_Character].glyphWidth;
		*p_Height = p_RenderFontDesc->renderGlyphDescs[p_Character].glyphHeight;
	}
}
/*********************************************************************************/
intern void K15_InternalCalculate2DTextRectangle(K15_RenderFontDesc* p_RenderFontDesc, const char* p_Text, float* p_Width, float* p_Height)
{
	uint32 textLength = (uint32)strlen(p_Text);
	
	float x = 0.f;
	float y = 0.f;

	float tX = 0.f;
	float tY = 0.f;
	float width = 0.f;
	float height = 0.f;

	for (uint32 textIndex = 0;
		textIndex < textLength;
		++textIndex)
	{
		uint16 character = p_Text[textIndex];
		K15_InternalCalculate2DGlyphRectangle(p_RenderFontDesc, character, &tX, &tY, &width, &height);

		x += width;
		y += height;

		width = 0.f;
		height = 0.f;

		if (character == '\n')
		{
			x = 0.f;
		}
		else if (textIndex != textLength)
		{
			uint16 nextCharacter = p_Text[textIndex + 1];
			uint32 shiftedCharacters = (character << 16) | nextCharacter;

			float kerning = K15_InternalGetRenderFontKerning(p_RenderFontDesc->renderKerningDescs, p_RenderFontDesc->numGlyphs * p_RenderFontDesc->numGlyphs, shiftedCharacters);
			x += kerning;
		}
	}
}
/*********************************************************************************/
intern void K15_InternalUpdateUniforms(K15_RenderContext* p_RenderContext)
{
	p_RenderContext->commandProcessing.programManagement.updateDirtyUniforms(p_RenderContext);
}
/*********************************************************************************/
intern void K15_InternalBindCamera(K15_RenderContext* p_RenderContext, K15_RenderCameraHandle* p_RenderCameraHandle)
{
	p_RenderContext->renderResources.cameras.activeCameraIndex = *p_RenderCameraHandle;

	K15_RenderUniformCache* uniformCache = &p_RenderContext->uniformCache;
	K15_InternalRenderCameraDesc* renderCameraDesc = &p_RenderContext->renderResources.cameras.cameraDescs[*p_RenderCameraHandle];

	K15_UpdateUniformCacheEntry(uniformCache, K15_AUTO_UNIFORM_TYPE_PROJECTION_MATRIX, (byte*)&renderCameraDesc->projectionMatrix);
	K15_UpdateUniformCacheEntry(uniformCache, K15_AUTO_UNIFORM_TYPE_VIEW_MATRIX, (byte*)&renderCameraDesc->viewMatrix);
}
/*********************************************************************************/
intern uint8 K15_InternalDrawMesh(K15_RenderContext* p_RenderContext, K15_RenderMeshHandle* p_RenderMeshHandle, K15_Matrix4* p_WorldMatrix)
{
	assert(p_RenderContext);
	assert(p_RenderMeshHandle);

	K15_RenderMeshDesc* renderMeshDesc = p_RenderContext->renderResources.meshDescs[*p_RenderMeshHandle];
	K15_RenderUniformCache* uniformCache = &p_RenderContext->uniformCache;

	K15_UpdateUniformCacheEntry(uniformCache, K15_AUTO_UNIFORM_TYPE_WORLD_MATRIX, (byte*)p_WorldMatrix);

	K15_InternalUpdateUniforms(p_RenderContext);

	uint32 subMeshCount = renderMeshDesc->submeshCount;
	uint8 result = K15_SUCCESS;

	for (uint32 subMeshIndex = 0;
		subMeshIndex < subMeshCount;
		++subMeshIndex)
	{
		K15_RenderBufferHandle vertexBufferHandle = renderMeshDesc->vertexBufferHandles[subMeshIndex];
		K15_RenderBufferHandle indexBufferHandle = renderMeshDesc->indexBufferHandles[subMeshIndex];

		K15_RenderBufferDesc* indexBufferDesc = p_RenderContext->renderResources.bufferDescs[indexBufferHandle];

		uint32 numIndeces = indexBufferDesc->sizeInBytes / indexBufferDesc->singleElementSizeInBytes;

		uint8 bindIndexBufferResult = p_RenderContext->commandProcessing.bufferManagement.bindBuffer(p_RenderContext, &indexBufferHandle);
		uint8 bindVertexBufferResult = p_RenderContext->commandProcessing.bufferManagement.bindBuffer(p_RenderContext, &vertexBufferHandle);
		uint8 drawIndexedResult = p_RenderContext->commandProcessing.drawManagement.drawIndexed(p_RenderContext, numIndeces, 0);

		if (bindIndexBufferResult != K15_SUCCESS)
		{
			result = bindIndexBufferResult;
			break;
		}

		if (bindVertexBufferResult != K15_SUCCESS)
		{
			result = bindVertexBufferResult;
			break;
		}

		if (drawIndexedResult != K15_SUCCESS)
		{
			result = drawIndexedResult;
			break;
		}
	}

	return result;
}
/*********************************************************************************/
intern inline void K15_InternalDeleteSampler(K15_RenderContext* p_RenderContext, K15_RenderSamplerHandle p_SamplerHandle)
{
	assert(p_RenderContext->renderResources.samplerDescs[p_SamplerHandle]);

	free(p_RenderContext->renderResources.samplerDescs[p_SamplerHandle]);
	p_RenderContext->renderResources.samplerDescs[p_SamplerHandle] = 0;
}
/*********************************************************************************/
intern inline void K15_InternalDeleteBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferHandle p_BufferHandle)
{
	assert(p_RenderContext->renderResources.bufferDescs[p_BufferHandle]);

	free(p_RenderContext->renderResources.bufferDescs[p_BufferHandle]);
	p_RenderContext->renderResources.bufferDescs[p_BufferHandle] = 0;
}
/*********************************************************************************/
intern inline void K15_InternalDeleteTexture(K15_RenderContext* p_RenderContext, K15_RenderTextureHandle p_TextureHandle)
{
	assert(p_RenderContext->renderResources.textureDescs[p_TextureHandle]);

	free(p_RenderContext->renderResources.textureDescs[p_TextureHandle]);
	p_RenderContext->renderResources.textureDescs[p_TextureHandle] = 0;
}
/*********************************************************************************/
intern inline void K15_InternalDeleteRenderTarget(K15_RenderContext* p_RenderContext, K15_RenderTargetHandle p_RenderTargetHandle)
{
	assert(p_RenderContext->renderResources.renderTargetDescs[p_RenderTargetHandle]);

	free(p_RenderContext->renderResources.renderTargetDescs[p_RenderTargetHandle]);
	p_RenderContext->renderResources.renderTargetDescs[p_RenderTargetHandle] = 0;
}
/*********************************************************************************/
intern inline void K15_InternalDeleteFont(K15_RenderContext* p_RenderContext, K15_RenderFontHandle p_RenderFontHandle)
{
	assert(p_RenderContext->renderResources.fontDescs[p_RenderFontHandle]);

	free(p_RenderContext->renderResources.fontDescs[p_RenderFontHandle]);
	p_RenderContext->renderResources.fontDescs[p_RenderFontHandle] = 0;
}
/*********************************************************************************/
intern inline void K15_InternalDeleteMesh(K15_RenderContext* p_RenderContext, K15_RenderMeshHandle p_RenderMeshHandle)
{
	assert(p_RenderContext->renderResources.meshDescs[p_RenderMeshHandle]);

	free(p_RenderContext->renderResources.meshDescs[p_RenderMeshHandle]);
	p_RenderContext->renderResources.meshDescs[p_RenderMeshHandle] = 0;
}
/*********************************************************************************/
intern inline void K15_InternalDeleteProgram(K15_RenderContext* p_RenderContext, K15_RenderProgramHandle p_RenderProgramHandle)
{
	assert(p_RenderContext->renderResources.programDescs[p_RenderProgramHandle]);

	free(p_RenderContext->renderResources.programDescs[p_RenderProgramHandle]);
	p_RenderContext->renderResources.programDescs[p_RenderProgramHandle] = 0;
}
/*********************************************************************************/
intern inline uint8 K15_InternalDraw2DTextureNormalizedCoordinates(K15_RenderContext* p_RenderContext, K15_RenderTextureHandle p_TextureHandle, 
																   K15_RenderMaterialHandle p_MaterialHandle,
																   float p_DestinationX, float p_DestinationY, 
																   float p_DestinationWidth, float p_DestinationHeight,
																   float p_SourceX, float p_SourceY,
																   float p_SourceWidth, float p_SourceHeight)
{
	//create vertices and add them to the immediate buffer
}
/*********************************************************************************/
intern inline uint8 K15_InternalDraw2DTexturePixelCoordinates(K15_RenderContext* p_RenderContext, K15_RenderTextureHandle p_TextureHandle, 
															  K15_RenderMaterialHandle p_MaterialHandle,
															  int32 p_DestinationX, int32 p_DestinationY, 
															  uint32 p_DestinationWidth, uint32 p_DestinationHeight,
															  int32 p_SourceX, int32 p_SourceY,
															  uint32 p_SourceWidth, uint32 p_SourceHeight)
{
	K15_RenderTextureDesc* textureDesc = p_RenderContext->renderResources.textureDescs[p_TextureHandle];
	K15_RenderViewportDesc* viewportDesc = &p_RenderContext->renderResources.viewportDescs[0]; //first viewport (main screen)
	
	float dx = p_DestinationX / viewportDesc->width;
	float dy = p_DestinationY / viewportDesc->height;
	float dw = p_DestinationWidth / viewportDesc->width;
	float dh = p_DestinationHeight / viewportDesc->height;

	float sx = p_SourceX / textureDesc->dimension.width;
	float sy = p_SourceY / textureDesc->dimension.height;
	float sw = p_SourceWidth / textureDesc->dimension.width;
	float sh = p_SourceHeight / textureDesc->dimension.height;

	return K15_InternalDraw2DTextureNormalizedCoordinates(p_RenderContext, p_TextureHandle, p_MaterialHandle, dx, dy, dw, dh, sx, sy, sw, sh);
}
/*********************************************************************************/
intern inline uint8 K15_InternalDraw2DGUI(K15_RenderContext* p_RenderContext, K15_GUIContext* p_GUIContext, K15_RenderMaterialHandle p_MaterialHandle)
{
	uint32 guiContextMemoryOffset = 0;

	while (guiContextMemoryOffset < p_GUIContext->guiMemoryCurrentSize)
	{
		K15_GUIElementHeader* guiElementHeader = (K15_GUIElementHeader*)p_GUIContext->guiMemory + guiContextMemoryOffset;

		switch (guiElementHeader->type)
		{
			case K15_GUI_TYPE_BUTTON:
			{
				K15_GUIButton* guiButton = (K15_GUIButton*)p_GUIContext->guiMemory + guiContextMemoryOffset + sizeof(K15_GUIElementHeader);
				
			}
		}

		guiContextMemoryOffset += guiElementHeader->offset;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_InternalDraw2DText(K15_RenderContext* p_RenderContext, K15_RenderFontHandle p_FontHandle, K15_RenderSamplerHandle* p_SamplerHandle, 
										   const char* p_Text, 
										   float p_PositionX, float p_PositionY)
{
	K15_RenderFontDesc* fontDesc = p_RenderContext->renderResources.fontDescs[p_FontHandle];
	K15_RenderTextureHandle fontTexture = fontDesc->fontTextureHandle;

	float fontSize = fontDesc->fontSize;
	float verticalSpacing = 0.25f * fontSize;
	float horizontalSpacing = 0.1f * fontSize;

	float x = p_PositionX;
	float y = p_PositionY;

	float glyphPosX = 0.f;
	float glyphPosY = 0.f;
	float glyphWidth = 0.f;
	float glyphHeight = 0.f;

	uint32 textLength = (uint32)strlen(p_Text);

	for (uint32 textIndex = 0;
		textIndex < textLength;
		++textIndex)
	{
		uint16 character = p_Text[textIndex];
		K15_InternalCalculate2DGlyphRectangle(fontDesc, character, &glyphPosX, &glyphPosY, &glyphWidth, &glyphHeight);

		x += glyphWidth;
		y += glyphHeight;

		glyphWidth = 0.f;
		glyphHeight = 0.f;

		if (character == '\n')
		{
			x = 0.f;
			y += verticalSpacing;
		}
		else if (isspace(character))
		{
			x += horizontalSpacing;
		}
		else if (textIndex != textLength)
		{
			uint16 nextCharacter = p_Text[textIndex + 1];
			uint32 shiftedCharacters = (character << 16) | nextCharacter;

			float kerning = K15_InternalGetRenderFontKerning(fontDesc->renderKerningDescs, fontDesc->numGlyphs * fontDesc->numGlyphs, shiftedCharacters);
			x += kerning;

			K15_InternalDraw2DTextureNormalizedCoordinates(p_RenderContext, fontTexture, x, y, glyphWidth, glyphHeight, glyphPosX, glyphPosY, glyphWidth, glyphHeight);
		}
	}
}
/*********************************************************************************/
intern inline uint8 K15_InternalDraw2DTextRectangle(K15_RenderContext* p_RenderContext, K15_RenderFontHandle p_FontHandle, const char* p_Text, 
													float p_PositionX, float PositionY, float p_Width, float p_Height)
{

}
/*********************************************************************************/
intern inline uint8 K15_InternalCreateMeshDescFromMeshFormat(K15_RenderContext* p_RenderContext, K15_MeshFormat* p_MeshFormat, K15_RenderMeshDesc* p_RenderMeshDesc)
{
	assert(p_RenderContext);
	assert(p_MeshFormat);

	uint32 subMeshCount = p_MeshFormat->submeshCount;
	p_RenderMeshDesc->submeshCount = subMeshCount;
	p_RenderMeshDesc->vertexBufferHandles = (K15_RenderBufferHandle*)malloc(sizeof(K15_RenderBufferHandle) * subMeshCount);
	p_RenderMeshDesc->indexBufferHandles = (K15_RenderBufferHandle*)malloc(sizeof(K15_RenderBufferHandle) * subMeshCount);

	uint8 result = K15_SUCCESS;

	for (uint32 subMeshIndex = 0;
		subMeshIndex < subMeshCount;
		++subMeshIndex)
	{
		K15_SubMeshFormat* subMeshFormat = &p_MeshFormat->submeshes[subMeshIndex];

		K15_RenderBufferDesc vertexBufferDesc = {};
		K15_RenderBufferDesc indexBufferDesc = {};

		/*********************************************************************************/
		vertexBufferDesc.access = K15_RENDER_BUFFER_ACCESS_ALL;
		vertexBufferDesc.data = (byte*)subMeshFormat->vertexData;
		vertexBufferDesc.sizeInBytes = K15_GetSubMeshFormatVertexDataSize(subMeshFormat);
		vertexBufferDesc.type = K15_RENDER_BUFFER_TYPE_VERTEX;
		vertexBufferDesc.usage = K15_RENDER_BUFFER_USAGE_STATIC_DRAW;
		//vertexBufferDesc.singleElementSizeInBytes = K15_GetSubMeshFormatVertexDataSize(subMeshFormat) / subMeshFormat->vertexCount;
		/*********************************************************************************/
		indexBufferDesc.access = K15_RENDER_BUFFER_ACCESS_ALL;
		indexBufferDesc.data = (byte*)subMeshFormat->triangleData;
		indexBufferDesc.sizeInBytes = K15_GetSubMeshFormatTriangleDataSize(subMeshFormat);
		indexBufferDesc.type = K15_RENDER_BUFFER_TYPE_INDEX;
		indexBufferDesc.usage = K15_RENDER_BUFFER_USAGE_STATIC_DRAW;
		indexBufferDesc.singleElementSizeInBytes = (subMeshFormat->triangleCount * 3) > 0xFFFF ? 4 : 2;
		/*********************************************************************************/

		K15_RenderBufferHandle subMeshVertexBufferHandle = K15_INVALID_GPU_RESOURCE_HANDLE;
		K15_RenderBufferHandle subMeshIndexBufferHandle = K15_INVALID_GPU_RESOURCE_HANDLE;

		uint8 createVertexBufferResult = K15_InternalCreateBuffer(p_RenderContext, &vertexBufferDesc, &subMeshVertexBufferHandle);
		uint8 createIndexBufferResult = K15_InternalCreateBuffer(p_RenderContext, &indexBufferDesc, &subMeshIndexBufferHandle);

		if (createVertexBufferResult != K15_SUCCESS)
		{
			result = createVertexBufferResult;
			break;
		}
		else if(createIndexBufferResult != K15_SUCCESS)
		{
			result = createIndexBufferResult;
			break;
		}

		p_RenderMeshDesc->indexBufferHandles[subMeshIndex] = subMeshIndexBufferHandle;
		p_RenderMeshDesc->vertexBufferHandles[subMeshIndex] = subMeshVertexBufferHandle;
	}

	return result;
}
/*********************************************************************************/
intern void K15_InternalCreateSamplerDescFromSamplerFormat(K15_RenderContext* p_RenderContext, K15_SamplerFormat* p_SamplerFormat, K15_RenderSamplerDesc* p_RenderSamplerDesc)
{
	assert(p_RenderContext);
	assert(p_SamplerFormat);
	assert(p_RenderSamplerDesc);

	p_RenderSamplerDesc->address.u = (K15_RenderFilterAddressMode)p_SamplerFormat->addressModeU;
	p_RenderSamplerDesc->address.v = (K15_RenderFilterAddressMode)p_SamplerFormat->addressModeV;
	p_RenderSamplerDesc->address.w = (K15_RenderFilterAddressMode)p_SamplerFormat->addressModeW;

	p_RenderSamplerDesc->filtering.magnification = (K15_RenderFilterMode)p_SamplerFormat->magnificationFilter;
	p_RenderSamplerDesc->filtering.minification = (K15_RenderFilterMode)p_SamplerFormat->minificationFilter;
}
/*********************************************************************************/
intern uint8 K15_InternalCreateMaterialDescFromMaterialFormat(K15_RenderContext* p_RenderContext, K15_MaterialFormat* p_MaterialFormat, K15_RenderMaterialDesc* p_RenderMaterialDesc)
{
	assert(p_RenderContext);
	assert(p_MaterialFormat);
	assert(p_RenderMaterialDesc);

	uint8 result = K15_SUCCESS;

	uint32 numMaterialPasses = p_MaterialFormat->numMaterialPasses;

	p_RenderMaterialDesc->numMaterialPasses = numMaterialPasses;
	p_RenderMaterialDesc->materialPasses = (K15_RenderMaterialPassDesc*)malloc(sizeof(K15_RenderMaterialPassDesc) * numMaterialPasses);

	for (uint32 materialPassIndex = 0;
		materialPassIndex < numMaterialPasses;
		++materialPassIndex)
	{
		K15_RenderMaterialPassDesc* materialPassDesc = &p_RenderMaterialDesc->materialPasses[materialPassIndex];
		K15_MaterialPassTemplateFormat* materialPassTemplateFormat = &p_MaterialFormat->passTemplates[materialPassIndex];

		//load shader
		const char* vertexShaderPath = materialPassTemplateFormat->vertexShaderPath;
		const char* fragmentShaderPath = materialPassTemplateFormat->fragmentShaderPath;

		K15_RenderProgramDesc vertexShaderDesc = {};
		K15_RenderProgramDesc fragmentShaderDesc = {};

		vertexShaderDesc.file = vertexShaderPath;
		vertexShaderDesc.source = K15_RENDER_PROGRAM_SOURCE_FILE;
		vertexShaderDesc.type = K15_RENDER_PROGRAM_TYPE_VERTEX;
		
		fragmentShaderDesc.file = fragmentShaderPath;
		fragmentShaderDesc.source = K15_RENDER_PROGRAM_SOURCE_FILE;
		fragmentShaderDesc.type = K15_RENDER_PROGRAM_TYPE_FRAGMENT;

		K15_RenderProgramHandle vertexShaderHandle = K15_InternalAddRenderProgramDesc(p_RenderContext, &vertexShaderDesc);
		K15_RenderProgramHandle fragmentShaderHandle = K15_InternalAddRenderProgramDesc(p_RenderContext, &fragmentShaderDesc);

		result = p_RenderContext->commandProcessing.programManagement.createProgram(p_RenderContext, &vertexShaderDesc, &vertexShaderHandle);

		if (result != K15_SUCCESS)
		{
			break;
		}

		result = p_RenderContext->commandProcessing.programManagement.createProgram(p_RenderContext, &fragmentShaderDesc, &fragmentShaderHandle);

		if (result != K15_SUCCESS)
		{
			break;
		}

		materialPassDesc->fragmentShaderHandle = fragmentShaderHandle;
		materialPassDesc->vertexShaderHandle = vertexShaderHandle;

		//load samplers
		uint32 numSamplers = materialPassTemplateFormat->numSamplers;
		materialPassDesc->numSamplerHandles = numSamplers;
		materialPassDesc->samplerHandles = (K15_RenderSamplerHandle*)malloc(numSamplers * sizeof(K15_RenderSamplerHandle));

		for (uint32 samplerIndex = 0;
			samplerIndex < numSamplers;
			++samplerIndex)
		{
			K15_MaterialPassTemplateSamplerFormat* materialPassSamplerFormat = &materialPassTemplateFormat->samplerFormats[samplerIndex];
			
			const char* materialPassSamplerFormatPath = materialPassSamplerFormat->samplerPath;

			K15_SamplerFormat samplerFormat = {};
			result = K15_LoadSamplerFormatFromFile(&samplerFormat, materialPassSamplerFormatPath);

			if (result != K15_SUCCESS)
			{
				K15_LOG_ERROR_MESSAGE("Could not load sampler format '%s' for material '%s'", materialPassSamplerFormatPath, p_MaterialFormat->materialName);
				break;
			}

			K15_RenderSamplerDesc renderSamplerDesc = {};
			K15_InternalCreateSamplerDescFromSamplerFormat(p_RenderContext, &samplerFormat, &renderSamplerDesc);

			K15_RenderSamplerHandle renderSamplerHandle = K15_InternalAddRenderSamplerDesc(p_RenderContext, &renderSamplerDesc);
			result = p_RenderContext->commandProcessing.samplerManagement.createSampler(p_RenderContext, &renderSamplerDesc, &renderSamplerHandle);

			materialPassDesc->samplerHandles[samplerIndex] = renderSamplerHandle;
		}

		if (result != K15_SUCCESS)
		{
			break;
		}
	}
	
	return result;
}
/*********************************************************************************/
intern uint8 K15_InternalCreateFontDescFromFontFormat(K15_RenderContext* p_RenderContext, K15_FontFormat* p_FontFormat, K15_RenderFontDesc* p_RenderFontDesc)
{
	assert(p_RenderContext);
	assert(p_FontFormat);

	uint8 result = K15_SUCCESS;

	K15_RenderTextureDesc fontTextureDesc = {};

	fontTextureDesc.createMipChain = K15_FALSE;
	fontTextureDesc.dimension.width = p_FontFormat->texture.width;
	fontTextureDesc.dimension.height = p_FontFormat->texture.height;
	fontTextureDesc.format = K15_RENDER_FORMAT_R8_UINT;
	fontTextureDesc.mipmaps.count = 1;
	fontTextureDesc.mipmaps.data[0] = p_FontFormat->texture.data;
	fontTextureDesc.mipmaps.dataSize[0] = p_FontFormat->texture.dataSize;
	fontTextureDesc.type = K15_RENDER_TEXTURE_TYPE_2D;
	
	//create font texture
	K15_RenderTextureHandle fontTextureHandle = K15_InternalAddRenderTextureDesc(p_RenderContext, &fontTextureDesc);
	result = p_RenderContext->commandProcessing.textureManagement.createTexture(p_RenderContext, &fontTextureDesc, &fontTextureHandle);

	if (result == K15_SUCCESS)
	{
		uint32 amountGlyphs = p_FontFormat->endCharacter - p_FontFormat->startCharacter;
		uint32 amountKerning = K15_GetKerningDataCount(p_FontFormat);

		uint32 fontTextureWidth = p_FontFormat->texture.width;
		uint32 fontTextureHeight = p_FontFormat->texture.height;

		p_RenderFontDesc->fontTextureHandle = fontTextureHandle;
		p_RenderFontDesc->fontSize = p_FontFormat->fontSize;
		p_RenderFontDesc->numGlyphs = amountGlyphs;
		p_RenderFontDesc->renderGlyphDescs = (K15_RenderGlyphDesc*)malloc(sizeof(K15_RenderGlyphDesc) * amountGlyphs);
		p_RenderFontDesc->renderKerningDescs = (K15_RenderKerningDesc*)malloc(sizeof(K15_RenderKerningDesc) * amountKerning);

		//transfer glyph data
		for (uint32 glyphIndex = 0;
			glyphIndex < amountGlyphs;
			++glyphIndex)
		{
			uint32 glyphCharacter = glyphIndex + p_FontFormat->startCharacter;
			K15_GlyphFormat* glyphFormat = K15_GetFontGlyphData(p_FontFormat, glyphCharacter);

			//transfrom from pixel to [0,1]
			if (glyphFormat)
			{
				p_RenderFontDesc->renderGlyphDescs[glyphIndex].character = glyphCharacter;
				p_RenderFontDesc->renderGlyphDescs[glyphIndex].texturePosX = glyphFormat->posX / fontTextureWidth;
				p_RenderFontDesc->renderGlyphDescs[glyphIndex].texturePosY = glyphFormat->posY / fontTextureHeight;
				p_RenderFontDesc->renderGlyphDescs[glyphIndex].glyphWidth = glyphFormat->width / fontTextureWidth;
				p_RenderFontDesc->renderGlyphDescs[glyphIndex].glyphHeight= glyphFormat->height / fontTextureHeight;
			}
		}

		//transfer kern data
		for (uint32 kernIndex = 0;
			kernIndex < amountKerning;
			++kernIndex)
		{
			K15_KerningFormat* kerningFormat = &p_FontFormat->kernFormats[kernIndex];

			p_RenderFontDesc->renderKerningDescs[kernIndex].shiftedCharacters = kerningFormat->shiftedCharacters;
			p_RenderFontDesc->renderKerningDescs[kernIndex].kerning = (real32)kerningFormat->kerning * p_FontFormat->fontSize;
		}
	}
	
	return result;
}
/*********************************************************************************/
intern void K15_InternalCreateImmediateBuffer(K15_RenderContext* p_RenderContext)
{
	assert(p_RenderContext);

	K15_RenderBufferDesc immediateRenderBufferDesc = {};

	immediateRenderBufferDesc.access = K15_RENDER_BUFFER_ACCESS_WRITE;
	immediateRenderBufferDesc.usage = K15_RENDER_BUFFER_USAGE_DYNAMIC_DRAW;
	immediateRenderBufferDesc.type = K15_RENDER_BUFFER_TYPE_VERTEX;
	immediateRenderBufferDesc.sizeInBytes = K15_RENDER_IMMEDIATE_BUFFER_SIZE;
	
	K15_RenderBufferHandle frontImmediateBufferHandle = K15_InternalAddRenderBufferDesc(p_RenderContext, &immediateRenderBufferDesc);
	K15_RenderBufferHandle backImmediateBufferHandle = K15_InternalAddRenderBufferDesc(p_RenderContext, &immediateRenderBufferDesc);

	//create buffers
	p_RenderContext->commandProcessing.bufferManagement.createBuffer(p_RenderContext, &immediateRenderBufferDesc, &frontImmediateBufferHandle);
	p_RenderContext->commandProcessing.bufferManagement.createBuffer(p_RenderContext, &immediateRenderBufferDesc, &backImmediateBufferHandle);

	p_RenderContext->immediateVertexBuffer.backBufferHandle = backImmediateBufferHandle;
	p_RenderContext->immediateVertexBuffer.frontBufferHandle = frontImmediateBufferHandle;
}
/*********************************************************************************/
intern void K15_InternalCreateDefaultRenderStates(K15_RenderContext* p_RenderContext)
{
	assert(p_RenderContext);

	K15_RenderSamplerDesc defaultSampler = {};
	K15_RenderRasterizerStateDesc defaultRasterizerState = {};
	K15_RenderDepthStateDesc defaultDepthState = {};
	K15_RenderStencilStateDesc defaultStencilState = {};
	K15_RenderBlendStateDesc defaultBlendState = {};
	//K15_RenderProgramDesc vertexShader = {};
	//K15_RenderProgramDesc pixelShader = {};
	K15_RenderViewportDesc viewport = {};
	K15_RenderCameraDesc cameraDesc = {};

// 	const char* glVertexShaderCode =	"in vec3 position;\n"
// 										"uniform mat4 worldMatrix;\n"
// 										"uniform mat4 viewMatrix;\n"
// 										"uniform mat4 projMatrix;\n"
// 										"void main(void) {\n"
// 										"vec4 outPosition = vec4(position, 1.0f);\n"
// 										"mat4 mvp = projMatrix * (viewMatrix * worldMatrix);\n"
// 										"outPosition = mvp * outPosition;\n"
// 										"outPosition.xyz /= outPosition.w;\n"
// 										//"mat4 mvp = projMatrix * (viewMatrix * worldMatrix);\n"
// 										"gl_Position = outPosition; }\n";
// 
// 	const char* glPixelShaderCode =		"void main(void) {\n"
// 										"gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); }\n";

	/*********************************************************************************/
	K15_OSContext* osContext = K15_GetOSLayerContext();
	viewport.x = 0;
	viewport.y = 0;
	viewport.height = osContext->window.window->height;
	viewport.width = osContext->window.window->width;
	/*********************************************************************************/
	cameraDesc.farPlane = 10.f;
	cameraDesc.nearPlane = 1.0f;
	cameraDesc.fov = K15_HALF_PI;
	cameraDesc.type = K15_RENDER_CAMERA_TYPE_PERSPECTIVE_PROJECTION;
	/*********************************************************************************/
// 	vertexShader.code = (char*)glVertexShaderCode;
// 	vertexShader.source = K15_RENDER_PROGRAM_SOURCE_CODE;
// 	vertexShader.type = K15_RENDER_PROGRAM_TYPE_VERTEX;
// 	/*********************************************************************************/
// 	pixelShader.code = (char*)glPixelShaderCode;
// 	pixelShader.source = K15_RENDER_PROGRAM_SOURCE_CODE;
// 	pixelShader.type = K15_RENDER_PROGRAM_TYPE_FRAGMENT;
	/*********************************************************************************/

	//Rasterizer State
	/*********************************************************************************/
	defaultRasterizerState.cullingMode = K15_CULLING_MODE_BACK;
	defaultRasterizerState.fillMode = K15_FILLMODE_SOLID;
	defaultRasterizerState.scissoringEnabled = FALSE;
	defaultRasterizerState.vertexOrder = K15_VERTEX_ORDER_COUNTER_CLOCKWISE;
	/*********************************************************************************/

	//Depth State
	/*********************************************************************************/
	defaultDepthState.compareFunction = K15_COMPARISON_GREATER_EQUAL;
	defaultDepthState.enabled = FALSE;
	/*********************************************************************************/

	//Stencil State
	/*********************************************************************************/
	defaultStencilState.enabled = FALSE;
	/*********************************************************************************/

	//Blend State
	/*********************************************************************************/
	defaultBlendState.enabled = FALSE;
	//blendState.enabled = TRUE;
	defaultBlendState.sourceBlendFactorRGB = K15_BLEND_FACTOR_SRC_ALPHA;
	defaultBlendState.sourceBlendFactorAlpha = K15_BLEND_FACTOR_ONE;
	defaultBlendState.blendOperationRGB = K15_BLEND_OPERATION_ADD;
	defaultBlendState.blendOperationAlpha = K15_BLEND_OPERATION_ADD;
	defaultBlendState.destinationBlendFactorRGB = K15_BLEND_FACTOR_INVERSE_SRC_ALPHA;
	defaultBlendState.destinationBlendFactorAlpha = K15_BLEND_FACTOR_ONE;
	/*********************************************************************************/

	//Sampler
	/*********************************************************************************/
	defaultSampler.filtering.magnification = K15_RENDER_FILTER_MODE_LINEAR;
	defaultSampler.filtering.minification = K15_RENDER_FILTER_MODE_LINEAR;
	defaultSampler.address.u = K15_RENDER_FILTER_ADDRESS_MODE_REPEAT;
	defaultSampler.address.v = K15_RENDER_FILTER_ADDRESS_MODE_REPEAT;
	defaultSampler.address.w = K15_RENDER_FILTER_ADDRESS_MODE_REPEAT;
	/*********************************************************************************/

	/*********************************************************************************/
// 	K15_RenderProgramHandle defaultVertexShaderHandle = K15_InternalAddRenderProgramDesc(p_RenderContext, &vertexShader);
// 	K15_RenderProgramHandle defaultPixelShaderHandle = K15_InternalAddRenderProgramDesc(p_RenderContext, &pixelShader);
 	/*********************************************************************************/
// 	p_RenderContext->commandProcessing.programManagement.createProgram(p_RenderContext, &vertexShader, &defaultVertexShaderHandle);
// 	p_RenderContext->commandProcessing.programManagement.createProgram(p_RenderContext, &pixelShader, &defaultPixelShaderHandle);
// 	p_RenderContext->commandProcessing.programManagement.bindProgram(p_RenderContext, &defaultVertexShaderHandle);
// 	p_RenderContext->commandProcessing.programManagement.bindProgram(p_RenderContext, &defaultPixelShaderHandle);
	/*********************************************************************************/
	p_RenderContext->defaultStates.defaultRenderContextState.blendStateDesc = defaultBlendState;
	p_RenderContext->defaultStates.defaultRenderContextState.depthStateDesc = defaultDepthState;
	p_RenderContext->defaultStates.defaultRenderContextState.rasterizerStateDesc = defaultRasterizerState;
	p_RenderContext->defaultStates.defaultRenderContextState.stencilStateDesc = defaultStencilState;
	
	p_RenderContext->defaultStates.defaultRenderSampler = K15_InternalAddRenderSamplerDesc(p_RenderContext, &defaultSampler);
	uint8 createdDefaultSampler = p_RenderContext->commandProcessing.samplerManagement.createSampler(p_RenderContext, &defaultSampler, &p_RenderContext->defaultStates.defaultRenderSampler);
	assert(createdDefaultSampler == K15_SUCCESS);

	/*********************************************************************************/
	K15_InternalSetBlendStateDesc(p_RenderContext, &defaultBlendState);
	K15_InternalSetDepthStateDesc(p_RenderContext, &defaultDepthState);
	K15_InternalSetStencilStateDesc(p_RenderContext, &defaultStencilState);
	K15_InternalSetRasterizerStateDesc(p_RenderContext, &defaultRasterizerState);
	K15_InternalAddRenderCameraDesc(p_RenderContext, &cameraDesc);
	K15_InternalSetViewportDesc(p_RenderContext, &viewport, 0);
	/*********************************************************************************/
}
/*********************************************************************************/
intern inline uint8 K15_InternalProcessRenderCommand(K15_RenderContext* p_RenderContext, K15_RenderCommandParameterBuffer* p_ParameterFrontBuffer, K15_RenderCommandInstance* p_RenderCommand)
{
	uint8 result = K15_SUCCESS;

	switch(p_RenderCommand->type)
	{
		case K15_RENDER_COMMAND_CLEAR_SCREEN:
		{
			result = p_RenderContext->commandProcessing.screenManagement.clearScreen(p_RenderContext);
			break;
		}

		case K15_RENDER_COMMAND_CREATE_BUFFER:
		{
			assert(p_RenderCommand->parameterSize == sizeof(K15_RenderBufferDesc) + K15_PTR_SIZE);
			
			K15_RenderBufferDesc renderBufferDesc = {0};
			K15_RenderBufferHandle* renderBufferHandle = 0;
				
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderBufferHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderBufferDesc), K15_PTR_SIZE, &renderBufferDesc);

			*renderBufferHandle = K15_InternalAddRenderBufferDesc(p_RenderContext, &renderBufferDesc);

			result = p_RenderContext->commandProcessing.bufferManagement.createBuffer(p_RenderContext, &renderBufferDesc, renderBufferHandle);
			K15_InternalCheckRenderBufferDescFlags(&renderBufferDesc);

			break;
		}

		case K15_RENDER_COMMAND_UPDATE_BUFFER:
		{
			K15_RenderBufferUpdateDesc renderBufferUpdateDesc = {0};
			K15_RenderBufferHandle* renderBufferHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderBufferHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderBufferUpdateDesc), K15_PTR_SIZE, &renderBufferUpdateDesc);

			if ((*renderBufferHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.bufferManagement.updateBuffer(p_RenderContext, &renderBufferUpdateDesc, renderBufferHandle);
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the buffer handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			K15_InternalCheckRenderBufferUpdateDescFlags(&renderBufferUpdateDesc);

			break;
		}

		case K15_RENDER_COMMAND_DELETE_BUFFER:
		{
			K15_RenderBufferHandle* renderBufferHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderBufferHandle);

			if ((*renderBufferHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.bufferManagement.deleteBuffer(p_RenderContext, renderBufferHandle);

				if (result == K15_SUCCESS)
				{
					K15_InternalDeleteBuffer(p_RenderContext, *renderBufferHandle);
				}
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the buffer handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			break;
		}

		case K15_RENDER_COMMAND_CREATE_FONT_FROM_FONT_FORMAT:
		{
			K15_FontFormat fontFormat = {};
			K15_RenderFontHandle* renderFontHandle = 0;
			K15_RenderFontDesc renderFontDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderFontHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_FontFormat), K15_PTR_SIZE, &fontFormat);

			result = K15_InternalCreateFontDescFromFontFormat(p_RenderContext, &fontFormat, &renderFontDesc);
			*renderFontHandle = K15_InternalAddRenderFontDesc(p_RenderContext, &renderFontDesc);

			break;
		}

		case K15_RENDER_COMMAND_CREATE_MESH_FROM_MESH_FORMAT:
		{
			K15_MeshFormat meshFormat = {};
			K15_RenderMeshHandle* renderMeshHandle = 0;
			K15_RenderMeshDesc renderMeshDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderMeshHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_MeshFormat), K15_PTR_SIZE, &meshFormat);

			result = K15_InternalCreateMeshDescFromMeshFormat(p_RenderContext, &meshFormat, &renderMeshDesc);

			if (result == K15_SUCCESS)
			{
				*renderMeshHandle = K15_InternalAddRenderMeshDesc(p_RenderContext, &renderMeshDesc);
			}
			
			break;
		}

		case K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT:
		{
			K15_SamplerFormat samplerFormat = {};
			K15_RenderSamplerHandle* renderSamplerHandle = 0;
			K15_RenderSamplerDesc renderSamplerDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderSamplerHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_SamplerFormat), K15_PTR_SIZE, &samplerFormat);

			K15_InternalCreateSamplerDescFromSamplerFormat(p_RenderContext, &samplerFormat, &renderSamplerDesc);
			*renderSamplerHandle = K15_InternalAddRenderSamplerDesc(p_RenderContext, &renderSamplerDesc);

			result = p_RenderContext->commandProcessing.samplerManagement.createSampler(p_RenderContext, &renderSamplerDesc, renderSamplerHandle);

			break;
		}

		case K15_RENDER_COMMAND_CREATE_MATERIAL_FROM_MATERIAL_FORMAT:
		{
			K15_MaterialFormat materialFormat = {};
			K15_RenderMaterialHandle* renderMaterialHandle = 0;
			K15_RenderMaterialDesc renderMaterialDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderMaterialHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_MaterialFormat), K15_PTR_SIZE, &materialFormat);

			K15_InternalCreateMaterialDescFromMaterialFormat(p_RenderContext, &materialFormat, &renderMaterialDesc);
			*renderMaterialHandle = K15_InternalAddRenderMaterialDesc(p_RenderContext, &renderMaterialDesc);

			break;
		}

		case K15_RENDER_COMMAND_CREATE_PROGRAM:
		{
			K15_RenderProgramDesc renderProgramDesc = {0};
			K15_RenderProgramHandle* renderProgramHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderProgramHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderProgramDesc), K15_PTR_SIZE, &renderProgramDesc);

			*renderProgramHandle = K15_InternalAddRenderProgramDesc(p_RenderContext, &renderProgramDesc);

			result = p_RenderContext->commandProcessing.programManagement.createProgram(p_RenderContext, &renderProgramDesc, renderProgramHandle);

			break;
		}

		case K15_RENDER_COMMAND_DELETE_PROGRAM:
		{
			K15_RenderProgramHandle* renderProgramHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderProgramHandle);

			if ((*renderProgramHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.programManagement.deleteProgram(p_RenderContext, renderProgramHandle);

				if (result == K15_SUCCESS)
				{
					K15_InternalDeleteProgram(p_RenderContext, *renderProgramHandle);
				}
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the program handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			break;
		}

		case K15_RENDER_COMMAND_CREATE_TEXTURE:
		{
			K15_RenderTextureHandle* renderTextureHandle = 0;
			K15_RenderTextureDesc renderTextureDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderTextureHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderTextureDesc), K15_PTR_SIZE, &renderTextureDesc);

			*renderTextureHandle = K15_InternalAddRenderTextureDesc(p_RenderContext, &renderTextureDesc);

			result = p_RenderContext->commandProcessing.textureManagement.createTexture(p_RenderContext, &renderTextureDesc, renderTextureHandle);

			K15_InternalCheckRenderTextureDescFlags(&renderTextureDesc);

			break;
		}

		case K15_RENDER_COMMAND_UPDATE_TEXTURE:
		{
			K15_RenderTextureHandle* renderTextureHandle = 0;
			K15_RenderTextureUpdateDesc renderTextureUpdateDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderTextureHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderTextureUpdateDesc), K15_PTR_SIZE, &renderTextureUpdateDesc);
			
			if ((*renderTextureHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.textureManagement.updateTexture(p_RenderContext, &renderTextureUpdateDesc, renderTextureHandle);
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the texture handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			K15_InternalCheckRenderTextureUpdateDescFlags(&renderTextureUpdateDesc);

			break;
		}

		case K15_RENDER_COMMAND_DELETE_TEXTURE:
		{
			K15_RenderTextureHandle* renderTextureHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderTextureHandle);

			if ((*renderTextureHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.textureManagement.deleteTexture(p_RenderContext, renderTextureHandle);

				if (result == K15_SUCCESS)
				{
					K15_InternalDeleteTexture(p_RenderContext, *renderTextureHandle);
				}
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the texture handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			break;
		}

		case K15_RENDER_COMMAND_CREATE_SAMPLER:
		{
			K15_RenderSamplerHandle* renderSamplerHandle = 0;
			K15_RenderSamplerDesc renderSamplerDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderSamplerHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderSamplerDesc), K15_PTR_SIZE, &renderSamplerDesc);

			*renderSamplerHandle = K15_InternalAddRenderSamplerDesc(p_RenderContext, &renderSamplerDesc);

			result = p_RenderContext->commandProcessing.samplerManagement.createSampler(p_RenderContext, &renderSamplerDesc, renderSamplerHandle);

			break;
		}

		case K15_RENDER_COMMAND_DELETE_SAMPLER:
		{
			K15_RenderSamplerHandle* renderSamplerHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderSamplerHandle);

			if ((*renderSamplerHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.samplerManagement.deleteSampler(p_RenderContext, renderSamplerHandle);

				if (result == K15_SUCCESS)
				{
					K15_InternalDeleteSampler(p_RenderContext, *renderSamplerHandle);
				}
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the sampler handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			break;
		}

		case K15_RENDER_COMMAND_UPDATE_UNIFORM:
		{
			K15_RenderProgramHandle* renderProgramHandle = 0;
			K15_RenderUniformUpdateDesc renderUniformUpdateDesc = {0};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderProgramHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderUniformUpdateDesc), K15_PTR_SIZE, &renderUniformUpdateDesc);
			
			if ((*renderProgramHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.programManagement.updateUniform(p_RenderContext, &renderUniformUpdateDesc, renderProgramHandle);
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the program handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			K15_InternalCheckRenderUniformUpdateDescFlags(&renderUniformUpdateDesc);

			break;
		}

		case K15_RENDER_COMMAND_SET_BLEND_STATE:
		{
			K15_RenderBlendStateDesc renderBlendStateDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderBlendStateDesc), 0, &renderBlendStateDesc);

			result = K15_InternalSetBlendStateDesc(p_RenderContext, &renderBlendStateDesc);
			
			break;
		}

		case K15_RENDER_COMMAND_SET_STENCIL_STATE:
		{
			K15_RenderStencilStateDesc renderStencilStateDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderStencilStateDesc), 0, &renderStencilStateDesc);

			result = K15_InternalSetStencilStateDesc(p_RenderContext, &renderStencilStateDesc);
			
			break;
		}

		case K15_RENDER_COMMAND_SET_DEPTH_STATE:
		{
			K15_RenderDepthStateDesc renderDepthStateDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderDepthStateDesc), 0, &renderDepthStateDesc);

			result = K15_InternalSetDepthStateDesc(p_RenderContext, &renderDepthStateDesc);
			
			break;
		}

		case K15_RENDER_COMMAND_SET_RASTERIZER_STATE:
		{
			K15_RenderRasterizerStateDesc renderRasterizerStateDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderRasterizerStateDesc), 0, &renderRasterizerStateDesc);
			
			result = K15_InternalSetRasterizerStateDesc(p_RenderContext, &renderRasterizerStateDesc);

			break;
		}

		case K15_RENDER_COMMAND_CREATE_RENDER_TARGET:
		{
			K15_RenderTargetDesc renderTargetDesc = {};
			K15_RenderTargetHandle* renderTargetHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderTargetHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderTargetDesc), K15_PTR_SIZE, &renderTargetDesc);

			*renderTargetHandle = K15_InternalAddRenderTargetDesc(p_RenderContext, &renderTargetDesc);

			result = p_RenderContext->commandProcessing.renderTargetManagement.createRenderTarget(p_RenderContext, &renderTargetDesc, renderTargetHandle);
			break;
		}

		case K15_RENDER_COMMAND_BIND_RENDER_TARGET:
		{
			K15_RenderTargetHandle* renderTargetHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderTargetHandle);

			if ((*renderTargetHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.renderTargetManagement.bindRenderTarget(p_RenderContext, renderTargetHandle);
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the render target handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			break;
		}

		case K15_RENDER_COMMAND_DELETE_RENDER_TARGET:
		{
			K15_RenderTargetHandle* renderTargetHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderTargetHandle);

			if ((*renderTargetHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.renderTargetManagement.deleteRenderTarget(p_RenderContext, renderTargetHandle);

				if (result == K15_SUCCESS)
				{
					K15_InternalDeleteRenderTarget(p_RenderContext, *renderTargetHandle);
				}
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the render target handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG
			break;
		}

		case K15_RENDER_COMMAND_DRAW_FULLSCREEN_QUAD:
		{
			K15_RenderProgramHandle* renderProgramHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderProgramHandle);

			if ((*renderProgramHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = p_RenderContext->commandProcessing.drawManagement.drawFullscreenQuad(p_RenderContext, renderProgramHandle);
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the program handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG
			break;
		}

		case K15_RENDER_COMMAND_DRAW_MESH:
		{
			K15_RenderMeshHandle* renderMeshHandle = 0;
			K15_Matrix4 worldMatrix = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderMeshHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_Matrix4), K15_PTR_SIZE, &worldMatrix);

			if ((*renderMeshHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				result = K15_InternalDrawMesh(p_RenderContext, renderMeshHandle, &worldMatrix);
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the mesh handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG
			break;
		}

		case K15_RENDER_COMMAND_DRAW_2D_GUI:
		{
			K15_GUIContext guiContext = {};
			K15_RenderMaterialHandle materialHandle = K15_INVALID_GPU_RESOURCE_HANDLE;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_GUIContext), 0, &guiContext);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, sizeof(K15_GUIContext), &materialHandle);
			
			result = K15_InternalDraw2DGUI(p_RenderContext, &guiContext, materialHandle);
			
			break;
		}

		case K15_RENDER_COMMAND_DRAW_2D_TEXT:
		{
			uint32 offset = 0;
			uint32 textLength = 0;
			K15_RenderFontHandle* fontHandle = 0;
			K15_RenderSamplerHandle* samplerHandle = 0;
			char* text = 0;
			float posX = 0.f;
			float posY = 0.f;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, offset, &fontHandle);
			offset += K15_PTR_SIZE;
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, offset, &samplerHandle);
			offset += K15_PTR_SIZE;
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(uint32), offset, &textLength);
			offset += sizeof(uint32);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, textLength, offset, &text);
			offset += textLength;
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(float), offset, &posX);
			offset += sizeof(float);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(float), offset, &posY);
			
			result = K15_InternalDraw2DText(p_RenderContext, *fontHandle, *samplerHandle, text, posX, posY);
			break;
		}

		case K15_RENDER_COMMAND_CREATE_CAMERA:
		{
			K15_RenderCameraHandle* renderCameraHandle = 0;
			K15_RenderCameraDesc renderCameraDesc = {};
		
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderCameraHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderCameraDesc), K15_PTR_SIZE, &renderCameraDesc);

			*renderCameraHandle = K15_InternalAddRenderCameraDesc(p_RenderContext, &renderCameraDesc);

			break;
		}

		case K15_RENDER_COMMAND_UPDATE_CAMERA:
		{
			K15_RenderCameraHandle* renderCameraHandle = 0;
			K15_RenderCameraDesc renderCameraDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderCameraHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderCameraDesc), K15_PTR_SIZE, &renderCameraDesc);

			if ((*renderCameraHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				K15_InternalAddRenderCameraDesc(p_RenderContext, &renderCameraDesc);
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the camera handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG
			break;
		}

		case K15_RENDER_COMMAND_BIND_CAMERA:
		{
			K15_RenderCameraHandle* renderCameraHandle = 0;

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderCameraHandle);

			if ((*renderCameraHandle) != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				K15_InternalBindCamera(p_RenderContext, renderCameraHandle);
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the camera handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return result;
}
/*********************************************************************************/
intern uint8 K15_InternalAddCommandBufferParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_ParameterSize, void* p_Parameter)
{
	assert(p_RenderCommandBuffer && p_Parameter);
	assert(p_ParameterSize > 0);

	K15_RenderCommandInstance* lastCommand = p_RenderCommandBuffer->lastCommand;
	K15_RenderCommandParameterBuffer* commandParameterBackBuffer = p_RenderCommandBuffer->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];

	if (!lastCommand)
	{
		return K15_ERROR_NO_RENDER_COMMAND;
	}

	uint32 currentParameterBufferOffset = commandParameterBackBuffer->parameterBufferOffset;
	uint32 newParameterBufferOffset = currentParameterBufferOffset + p_ParameterSize;

	if (newParameterBufferOffset >= K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE)
	{
		return K15_ERROR_MAX_COMMAND_PARAMETER_SIZE_BUFFER_REACHED;
	}

	memcpy(commandParameterBackBuffer->parameterBuffer + currentParameterBufferOffset, p_Parameter, p_ParameterSize);

	lastCommand->parameterSize = newParameterBufferOffset;

	commandParameterBackBuffer->parameterBufferOffset = newParameterBufferOffset;
	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_InternalProcessRenderCommandBuffer(K15_RenderContext* p_RenderContext, K15_RenderCommandBuffer* p_RenderCommandBuffer)
{
	assert(p_RenderCommandBuffer);

	K15_RenderCommandInstance* currentCommand = 0;
	uint8 result = K15_SUCCESS;

	K15_RenderCommandParameterBuffer** parameterFrontBuffer = &p_RenderCommandBuffer->parameterBuffer[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	K15_RenderCommandBufferInstance** commandFrontBuffer = &p_RenderCommandBuffer->commandBuffers[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];

	//process front buffer
	for(uint32 renderCommandIndex = 0;
		renderCommandIndex < (*commandFrontBuffer)->amountCommands;
		++renderCommandIndex)
	{
		currentCommand = &(*commandFrontBuffer)->commandBuffer[renderCommandIndex];

		result = K15_InternalProcessRenderCommand(p_RenderContext, *parameterFrontBuffer, currentCommand);

		if (result != K15_SUCCESS)
		{
			K15_LOG_ERROR_MESSAGE("Error during render '%s' render command processing (%s).", K15_ConvertRenderCommandToString(currentCommand->type), p_RenderContext->lastError.message);
		}

		assert(result == K15_SUCCESS);
	}

	//command queue has been processed. Remove dispatched flag
	K15_PostSemaphore(p_RenderCommandBuffer->processingSemaphore);

	return result;
}
/*********************************************************************************/
intern void K15_InternalSwapRenderCommandQueueBuffers(K15_RenderCommandBuffer* p_RenderCommandBuffer)
{
	assert(p_RenderCommandBuffer);

	K15_RenderCommandBufferInstance* tempBuffer = 0;
	K15_RenderCommandParameterBuffer* tempParameterBuffer = 0;

	K15_RenderCommandParameterBuffer** parameterFrontBuffer = &p_RenderCommandBuffer->parameterBuffer[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	K15_RenderCommandParameterBuffer** parameterBackBuffer = &p_RenderCommandBuffer->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];
	K15_RenderCommandBufferInstance** commandFrontBuffer = &p_RenderCommandBuffer->commandBuffers[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	K15_RenderCommandBufferInstance** commandBackBuffer = &p_RenderCommandBuffer->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];

	//swap command buffers
	tempBuffer = *commandFrontBuffer;
	*commandFrontBuffer = *commandBackBuffer;
	*commandBackBuffer = tempBuffer;

	//swap parameter buffers
	tempParameterBuffer = *parameterFrontBuffer;
	*parameterFrontBuffer = *parameterBackBuffer;
	*parameterBackBuffer = tempParameterBuffer;

	//commands and params have been processed
	(*parameterBackBuffer)->parameterBufferOffset = 0;
	(*commandBackBuffer)->amountCommands = 0;
}
/*********************************************************************************/
intern void K15_InternalSwapRenderDispatcherBuffers(K15_RenderCommandBufferDispatcher* p_RenderCommandBufferDispatcher)
{
	assert(p_RenderCommandBufferDispatcher);

	//render thread tries to get the swap mutex lock
	K15_LockMutex(p_RenderCommandBufferDispatcher->swapMutex);

	uint32 tempCommandQueueToProcessCounter = 0;
	uint32* frontCommandQueueToProcessCounter = &p_RenderCommandBufferDispatcher->amountCommandBuffersToProcess[K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX];
	uint32* backCommandQueueToProcessCounter = &p_RenderCommandBufferDispatcher->amountCommandBuffersToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX];

	K15_RenderCommandBuffer** tempCommandQueuesToProcessArray = 0;
	K15_RenderCommandBuffer*** frontCommandQueueProcessBuffer = &p_RenderCommandBufferDispatcher->renderCommandBuffersToProcess[K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX];
	K15_RenderCommandBuffer*** backCommandQueueProcessBuffer = &p_RenderCommandBufferDispatcher->renderCommandBuffersToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX];

	//swap dispatch buffers
	tempCommandQueuesToProcessArray = *frontCommandQueueProcessBuffer;
	*frontCommandQueueProcessBuffer = *backCommandQueueProcessBuffer;
	*backCommandQueueProcessBuffer = tempCommandQueuesToProcessArray;

	tempCommandQueueToProcessCounter = *frontCommandQueueToProcessCounter;
	*frontCommandQueueToProcessCounter = *backCommandQueueToProcessCounter;
	*backCommandQueueToProcessCounter = tempCommandQueueToProcessCounter;

	*backCommandQueueToProcessCounter = 0;

	//render thread unlocks the swap mutex
	K15_UnlockMutex(p_RenderCommandBufferDispatcher->swapMutex);
}
/*********************************************************************************/
intern uint8 K15_InternalInitializeRenderContext(K15_RenderContext* p_RenderContext, K15_OSContext* p_OSContext)
{
	assert(p_RenderContext && p_OSContext);

	//TODO: DX 11/10/9
	uint8 createGLRenderContext = TRUE;

	if (createGLRenderContext == TRUE)
	{
		uint8 result = K15_GLCreateRenderContext(p_RenderContext, p_OSContext);

		if (result != K15_SUCCESS)
		{
			return result;
		}
	}

	//set render context initialized flag
	p_RenderContext->flags |= K15_RCF_INITIALIZED;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern void K15_InternalProcessRenderCommandBuffer(K15_RenderContext* p_RenderContext)
{
	//swap dispatching buffer so the threads that are dispatching command queues can immediately start to fill the back buffer again
	K15_InternalSwapRenderDispatcherBuffers(p_RenderContext->commandBufferDispatcher);

	int renderCommandQueuesToProcess = p_RenderContext->commandBufferDispatcher->amountCommandBuffersToProcess[K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX];

	for (int renderCommandQueueIndex = 0;
		renderCommandQueueIndex < renderCommandQueuesToProcess;
		++renderCommandQueueIndex)
	{
		K15_RenderCommandBuffer** renderCommandBuffersToProcess = p_RenderContext->commandBufferDispatcher->renderCommandBuffersToProcess[K15_RENDERING_DISPATCH_FRONT_BUFFER_INDEX];
		K15_InternalProcessRenderCommandBuffer(p_RenderContext, renderCommandBuffersToProcess[renderCommandQueueIndex]);
	}

	p_RenderContext->commandProcessing.screenManagement.clearScreen(p_RenderContext);
}
/*********************************************************************************/
intern inline void K15_InternalSetRenderContextStubFunctions(K15_RenderContext* p_RenderContext)
{
	p_RenderContext->commandProcessing.screenManagement.clearScreen = K15_ClearScreenCommandFnc_stubFnc;

	//buffer
	p_RenderContext->commandProcessing.bufferManagement.createBuffer = K15_CreateBufferCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.bufferManagement.updateBuffer = K15_UpdateBufferCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.bufferManagement.deleteBuffer = K15_DeleteBufferCommandFnc_stubFnc;

	//programs
	p_RenderContext->commandProcessing.programManagement.createProgram = K15_CreateProgramCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.programManagement.deleteProgram = K15_DeleteProgramCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.programManagement.updateUniform = K15_UpdateUniformCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.programManagement.updateDirtyUniforms = K15_UpdateDirtyUniformsCommandFnc_stubFnc;

	//states
	p_RenderContext->commandProcessing.stateManagement.setBlendState = K15_SetBlendStateCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.stateManagement.setStencilState = K15_SetStencilStateCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.stateManagement.setRasterizerState = K15_SetRasterizerStateCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.stateManagement.setDepthState = K15_SetDepthStateCommandFnc_stubFnc;

	//sampler
	p_RenderContext->commandProcessing.samplerManagement.createSampler = K15_CreateSamplerCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.samplerManagement.deleteSampler = K15_DeleteSamplerCommandFnc_stubFnc;

	//render target
	p_RenderContext->commandProcessing.renderTargetManagement.createRenderTarget = K15_CreateRenderTargetCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.renderTargetManagement.bindRenderTarget = K15_BindRenderTargetCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.renderTargetManagement.unbindRenderTarget = K15_UnbindRenderTargetCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.renderTargetManagement.deleteRenderTarget = K15_DeleteSamplerCommandFnc_stubFnc;

	//textures
	p_RenderContext->commandProcessing.textureManagement.createTexture = K15_CreateTextureCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.textureManagement.updateTexture = K15_UpdateTextureCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.textureManagement.deleteTexture = K15_DeleteTextureCommandFnc_stubFnc;

	//drawing
	p_RenderContext->commandProcessing.drawManagement.drawFullscreenQuad = K15_DrawFullscreenQuadCommandFnc_stubFnc;
	p_RenderContext->commandProcessing.drawManagement.drawIndexed = K15_DrawIndexedCommandFnc_stubFnc;
}
/*********************************************************************************/


/*********************************************************************************/
void K15_SetRenderContextError(K15_RenderContext* p_RenderContext, const char* p_ErrorMessage, uint32 p_ErrorMessageLength)
{
	if (p_RenderContext->lastError.message)
	{
		if (p_RenderContext->lastError.length <= p_ErrorMessageLength)
		{
			free((void*)p_RenderContext->lastError.message);
			p_RenderContext->lastError.message = (char*)malloc(p_ErrorMessageLength + 1); //+1 for 0 terminator
			p_RenderContext->lastError.length = p_ErrorMessageLength;
		}
	}
	else
	{
		p_RenderContext->lastError.message = (char*)malloc(p_ErrorMessageLength);
	}

	memcpy(p_RenderContext->lastError.message, p_ErrorMessage, p_ErrorMessageLength);

	p_RenderContext->lastError.message[p_ErrorMessageLength] = 0;
}
/*********************************************************************************/
const char* K15_GetLastRenderErrorMessage(K15_RenderContext* p_RenderContext)
{
	return p_RenderContext->lastError.message;
}
/*********************************************************************************/
const char* K15_GetLastRenderErrorMessageWithSize(K15_RenderContext* p_RenderContext, uint32* p_ErrorMessageSize)
{
	if (p_ErrorMessageSize)
	{
		*p_ErrorMessageSize = p_RenderContext->lastError.length;
	}

	return p_RenderContext->lastError.message;
}
/*********************************************************************************/
K15_RenderContext* K15_CreateRenderContext(K15_OSContext* p_OSContext)
{
	assert(p_OSContext);

	K15_RenderContext* renderContext = (K15_RenderContext*)malloc(sizeof(K15_RenderContext));

	if (!renderContext)
	{
		assert(false);
		return 0;
	}

	renderContext->userData = 0;

	//create command queue dispatcher
	K15_RenderCommandBufferDispatcher* renderCommandBufferDispatcher = (K15_RenderCommandBufferDispatcher*)malloc(sizeof(K15_RenderCommandBufferDispatcher));

	if (!renderCommandBufferDispatcher)
	{
		assert(false);
		return 0;
	}
	
	//create double dispatch buffer
	for (uint32 renderCommandBufferDispatchBufferIndex = 0;
		 renderCommandBufferDispatchBufferIndex < K15_RENDERING_COMMAND_DISPATCH_BUFFER_COUNT;
		 ++renderCommandBufferDispatchBufferIndex)
	{
		renderCommandBufferDispatcher->renderCommandBuffersToProcess[renderCommandBufferDispatchBufferIndex] = (K15_RenderCommandBuffer**)malloc(K15_PTR_SIZE * K15_RENDERING_MAX_COMMAND_BUFFERS_TO_PROCESS);
		renderCommandBufferDispatcher->amountCommandBuffersToProcess[renderCommandBufferDispatchBufferIndex] = 0;
	}

	renderCommandBufferDispatcher->swapMutex = K15_CreateMutex();

	//create uniform cache
	K15_InitializeRenderUniformCache(&renderContext->uniformCache);

	//create command queues
	K15_RenderCommandBuffer* renderCommandBuffers = (K15_RenderCommandBuffer*)malloc(sizeof(K15_RenderCommandBuffer) * K15_RENDERING_MAX_COMMAND_BUFFERS);

	if (!renderCommandBuffers)
	{
		assert(false);
		return 0;
	}
	
	/*********************************************************************************/
	//gpu materials
	for (uint32 i = 0;
		i < K15_RENDER_MAX_GPU_MATERIALS;
		++i)
	{
		renderContext->renderResources.materialDescs[i] = 0;
	}

	/*********************************************************************************/
	//gpu meshes
	for (uint32 i = 0;
		i < K15_RENDER_MAX_GPU_MESHES;
		++i)
	{
		renderContext->renderResources.meshDescs[i] = 0;
	}
	/*********************************************************************************/
	

	/*********************************************************************************/
	//gpu buffers
	for (uint32 i = 0;
		i < K15_RENDER_MAX_GPU_BUFFER;
		++i)
	{
		renderContext->renderResources.bufferDescs[i] = 0;
	}
	/*********************************************************************************/


	/*********************************************************************************/
	//gpu programs
	for (uint32 i = 0;
		i < K15_RENDER_MAX_GPU_PROGRAMS;
		++i)
	{
		renderContext->renderResources.programDescs[i] = 0;
	}
	/*********************************************************************************/


	/*********************************************************************************/
	//gpu textures
	for (uint32 i = 0;
		i < K15_RENDER_MAX_GPU_TEXTURES;
		++i)
	{
		renderContext->renderResources.textureDescs[i] = 0;
	}
	/*********************************************************************************/


	/*********************************************************************************/
	//gpu samplers
	for (uint32 i = 0;
		i < K15_RENDER_MAX_GPU_SAMPLERS;
		++i)
	{
		renderContext->renderResources.samplerDescs[i] = 0;
	}
	/*********************************************************************************/


	/*********************************************************************************/
	//gpu render targets
	for (uint32 i = 0;
		i < K15_RENDER_MAX_GPU_RENDER_TARGETS;
		++i)
	{
		renderContext->renderResources.renderTargetDescs[i] = 0;
	}
	/*********************************************************************************/


	/*********************************************************************************/
	//gpu fonts
	for (uint32 i = 0;
		i < K15_RENDER_MAX_GPU_FONTS;
		++i)
	{
		renderContext->renderResources.fontDescs[i] = 0;
	}
	/*********************************************************************************/


	/*********************************************************************************/
	//camera(s)
	for (uint32 i = 0;
		i < K15_RENDER_MAX_CAMERAS;
		++i)
	{
		renderContext->renderResources.cameras.cameraDescs[i] = 0;
	}
	renderContext->renderResources.cameras.activeCameraIndex = 0;
	/*********************************************************************************/

	renderContext->lastError.length = 0;
	renderContext->lastError.message = 0;

	renderContext->flags = 0;
	renderContext->commandBuffers = renderCommandBuffers;
	renderContext->amountCommandBuffers = 0;
	renderContext->userData = 0;
	renderContext->commandBufferDispatcher = renderCommandBufferDispatcher;
	renderContext->createCommandBufferMutex = K15_CreateMutex();
	renderContext->renderThreadSync = K15_CreateSemaphore();

#ifdef K15_DEBUG_MRT
	renderContext->debugging.assignedThread = K15_GetCurrentThread();
#endif //K15_DEBUG_MRT

	//mainwindow
	K15_Window* window = p_OSContext->window.window;

	//set first viewport manually
	renderContext->renderResources.viewportDescs[0].x = 0;
	renderContext->renderResources.viewportDescs[0].y = 0;
	renderContext->renderResources.viewportDescs[0].width = window->width;
	renderContext->renderResources.viewportDescs[0].height = window->height;

	//set stub functions
	K15_InternalSetRenderContextStubFunctions(renderContext);

	uint8 contextCreateResult = K15_InternalInitializeRenderContext(renderContext, p_OSContext);

	//Set default render states
	K15_InternalCreateDefaultRenderStates(renderContext);
	K15_InternalCreateImmediateBuffer(renderContext);

	return renderContext;
}
/*********************************************************************************/
K15_RenderCommandBuffer* K15_CreateRenderCommandBuffer(K15_RenderContext* p_RenderContext)
{
	assert(p_RenderContext->amountCommandBuffers < K15_RENDERING_MAX_COMMAND_BUFFERS);
	
	if (p_RenderContext->amountCommandBuffers >= K15_RENDERING_MAX_COMMAND_BUFFERS)
	{
		return 0;
	}

	//lock mutex so this thread is currently the only one that can create command queues
	K15_LockMutex(p_RenderContext->createCommandBufferMutex);

	uint32 commandQueueIndex = p_RenderContext->amountCommandBuffers;

	K15_RenderCommandBuffer* renderCommandBuffer = &p_RenderContext->commandBuffers[commandQueueIndex];

	for (commandQueueIndex = 0;
		 commandQueueIndex < K15_RENDERING_COMMAND_BUFFER_COUNT;
		 ++commandQueueIndex)
	{
		K15_RenderCommandInstance* commandBuffer = (K15_RenderCommandInstance*)malloc(sizeof(K15_RenderCommandInstance) * K15_RENDERING_MAX_COMMANDS);
		byte* parameterBuffer = (byte*)malloc(K15_RENDERING_MAX_PARAMETER_BUFFER_SIZE);

		renderCommandBuffer->commandBuffers[commandQueueIndex] = (K15_RenderCommandBufferInstance*)malloc(sizeof(K15_RenderCommandBufferInstance));
		renderCommandBuffer->parameterBuffer[commandQueueIndex] = (K15_RenderCommandParameterBuffer*)malloc(sizeof(K15_RenderCommandParameterBuffer));

		if (!renderCommandBuffer->commandBuffers[commandQueueIndex]
			|| !renderCommandBuffer->parameterBuffer[commandQueueIndex]
			|| !commandBuffer
			|| !parameterBuffer)
		{
			return 0;
		}
		
		//initialize command buffer data
		renderCommandBuffer->commandBuffers[commandQueueIndex]->commandBuffer = commandBuffer;
		renderCommandBuffer->commandBuffers[commandQueueIndex]->amountCommands = 0;
		renderCommandBuffer->commandBuffers[commandQueueIndex]->flags = 0;

		//initialize parameter buffer data
		renderCommandBuffer->parameterBuffer[commandQueueIndex]->parameterBuffer = parameterBuffer;
		renderCommandBuffer->parameterBuffer[commandQueueIndex]->parameterBufferOffset = 0;
	}

	renderCommandBuffer->flags = 0;
	renderCommandBuffer->renderContext = p_RenderContext;
	renderCommandBuffer->processingMutex = K15_CreateMutex();
	renderCommandBuffer->processingSemaphore = K15_CreateSemaphoreWithInitialValue(1);
	renderCommandBuffer->swappingSemaphore = K15_CreateSemaphoreWithInitialValue(1);
	renderCommandBuffer->lastCommand = 0;

#ifdef K15_DEBUG_MRT
	renderCommandBuffer->debugging.assignedThread = K15_GetCurrentThread();
#endif //K15_DEBUG_MRT
	
	//increasing the command queue count should be the last operation
	++p_RenderContext->amountCommandBuffers;

	//unlock mutex so another thread can start creating command queues
	K15_UnlockMutex(p_RenderContext->createCommandBufferMutex);

	return renderCommandBuffer;
}
/*********************************************************************************/
uint8 K15_BeginRenderCommand(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderCommand p_RenderCommand)
{
	assert(p_RenderCommandBuffer);
	assert(p_RenderCommandBuffer->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX]->amountCommands < K15_RENDERING_MAX_COMMANDS);
	
#ifdef K15_DEBUG_MRT
	assert(p_RenderCommandBuffer->debugging.assignedThread == K15_GetCurrentThread());
#endif //K15_DEBUG_MRT

	assert(!p_RenderCommandBuffer->lastCommand);

	K15_RenderCommandBufferInstance* commandBackBuffer = p_RenderCommandBuffer->commandBuffers[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];
	K15_RenderCommandParameterBuffer* commandParameterBackBuffer = p_RenderCommandBuffer->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];

	if (commandBackBuffer->amountCommands == K15_RENDERING_MAX_COMMANDS)
	{
		return K15_ERROR_MAX_RENDER_COMMANDS_REACHED;
	}

	if (p_RenderCommandBuffer->lastCommand)
	{
		return K15_ERROR_LAST_COMMAND_NOT_FINISHED;
	}

	uint32 renderCommandIndex = commandBackBuffer->amountCommands++;

	K15_RenderCommandInstance* renderCommandInstance = &commandBackBuffer->commandBuffer[renderCommandIndex];
	p_RenderCommandBuffer->lastCommand = renderCommandInstance;

	renderCommandInstance->type = p_RenderCommand;
	renderCommandInstance->parameterOffset = commandParameterBackBuffer->parameterBufferOffset;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_EndRenderCommand(K15_RenderCommandBuffer* p_RenderCommandBuffer)
{
	assert(p_RenderCommandBuffer);

	K15_RenderCommandInstance* lastCommand = p_RenderCommandBuffer->lastCommand;
	K15_RenderCommandParameterBuffer* commandParameterBackBuffer = p_RenderCommandBuffer->parameterBuffer[K15_RENDERING_COMMAND_BACK_BUFFER_INDEX];

	if (!lastCommand)
	{
		return K15_ERROR_NO_RENDER_COMMAND;
	}

	lastCommand->parameterSize = (commandParameterBackBuffer->parameterBufferOffset - lastCommand->parameterOffset);

	p_RenderCommandBuffer->lastCommand = 0;

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_AddRenderUInt32Parameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32* p_Parameter)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer, 
		sizeof(uint32), 
		p_Parameter);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderInt32Parameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, int32* p_Parameter)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer, 
		sizeof(int32), 
		p_Parameter);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderBufferDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferDesc* p_RenderBufferDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderBufferDesc),
		p_RenderBufferDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderBufferUpdateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderBufferUpdateDesc),
		p_RenderBufferUpdateDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderBufferHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBufferHandle* p_RenderBufferHandle)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		K15_PTR_SIZE,
		&p_RenderBufferHandle);
	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderProgramHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		K15_PTR_SIZE,
		&p_RenderProgramHandle);
	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderProgramDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderProgramDesc* p_RenderProgramDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderProgramDesc),
		p_RenderProgramDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderUniformUpdateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderUniformUpdateDesc* p_RenderUniformUpdateDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderUniformUpdateDesc),
		p_RenderUniformUpdateDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderDepthStateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderDepthStateDesc* p_RenderDepthStateDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderDepthStateDesc),
		p_RenderDepthStateDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderStencilStateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderStencilStateDesc* p_RenderStencilStateDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderStencilStateDesc),
		p_RenderStencilStateDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderRasterizerStateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderRasterizerStateDesc* p_RenderRasterizerStateDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderRasterizerStateDesc),
		p_RenderRasterizerStateDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderBlendStateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderBlendStateDesc* p_RenderBlendStateDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderBlendStateDesc),
		p_RenderBlendStateDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderTextureHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTextureHandle* p_RenderTextureHandle)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		K15_PTR_SIZE,
		&p_RenderTextureHandle);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderTextureDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTextureDesc* p_RenderTextureDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderTextureDesc),
		p_RenderTextureDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderTextureUpdateDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTextureUpdateDesc* p_RenderTextureUpdateDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderTextureUpdateDesc),
		p_RenderTextureUpdateDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderSamplerHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderSamplerHandle* p_RenderSamplerHandler)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		K15_PTR_SIZE,
		&p_RenderSamplerHandler);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderSamplerDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderSamplerDesc* p_RenderSamplerDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderSamplerDesc),
		p_RenderSamplerDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderTargetHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTargetHandle* p_RenderTargetHandle)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		K15_PTR_SIZE,
		&p_RenderTargetHandle);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderTargetDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderTargetDesc* p_RenderTargetDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderTargetDesc),
		p_RenderTargetDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderMeshHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderMeshHandle* p_RenderMeshHandle)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		K15_PTR_SIZE,
		&p_RenderMeshHandle);

	return result;
}
/*********************************************************************************/
uint8 K15_AddMeshFormatParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_MeshFormat* p_MeshFormat)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_MeshFormat),
		p_MeshFormat);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderCameraDescParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderCameraDesc* p_RenderCameraDesc)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_RenderCameraDesc),
		p_RenderCameraDesc);

	return result;
}
/*********************************************************************************/
uint8 K15_AddRenderCameraHandleParameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_RenderCameraHandle* p_RenderCameraHandle)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		K15_PTR_SIZE,
		&p_RenderCameraHandle);

	return result;
}
/*********************************************************************************/
uint8 K15_AddMatrix4Parameter(K15_RenderCommandBuffer* p_RenderCommandBuffer, K15_Matrix4* p_MatrixParameter)
{
	uint8 result = K15_InternalAddCommandBufferParameter(p_RenderCommandBuffer,
		sizeof(K15_Matrix4),
		p_MatrixParameter);

	return result;
}
/*********************************************************************************/
void K15_DispatchRenderCommandBuffer(K15_RenderCommandBuffer* p_RenderCommandQueue)
{
	assert(p_RenderCommandQueue);
	assert(!p_RenderCommandQueue->lastCommand);

#ifdef K15_DEBUG_MRT
	assert(p_RenderCommandQueue->debugging.assignedThread == K15_GetCurrentThread());
#endif //K15_DEBUG_MRT

	K15_RenderContext* renderContext = p_RenderCommandQueue->renderContext;
	K15_RenderCommandBufferDispatcher* renderDispatcher = renderContext->commandBufferDispatcher;
	
	//Wait until the render thread has processed the render queue
	K15_WaitSemaphore(p_RenderCommandQueue->processingSemaphore);

	//the caller thread tries to get the swap mutex so the render thread can not swap the dispatcher 
	//when we try to add a command queue
	K15_LockMutex(renderDispatcher->swapMutex);

	int commandQueuesToProcessIndex = renderContext->commandBufferDispatcher->amountCommandBuffersToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX];

	assert(commandQueuesToProcessIndex != K15_RENDERING_MAX_COMMAND_BUFFERS_TO_PROCESS);

	//add render queue to the dispatcher
	renderContext->commandBufferDispatcher->renderCommandBuffersToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX][commandQueuesToProcessIndex++] = p_RenderCommandQueue;
	renderContext->commandBufferDispatcher->amountCommandBuffersToProcess[K15_RENDERING_DISPATCH_BACK_BUFFER_INDEX] = commandQueuesToProcessIndex;

	//swap render command queue so we can immediately start pumping render commands again
	K15_InternalSwapRenderCommandQueueBuffers(p_RenderCommandQueue);

	//unclock swap mutex so the renderthread can swap the dispatcher.
	K15_UnlockMutex(renderDispatcher->swapMutex);
}
/*********************************************************************************/
void K15_ProcessDispatchedRenderCommandBuffers(K15_RenderContext* p_RenderContext)
{
	assert(p_RenderContext);
#ifdef K15_DEBUG_MRT
	assert(p_RenderContext->debugging.assignedThread == K15_GetCurrentThread());
#endif //K15_DEBUG_MRT	
	
	//signal the render thread to start processing the dispatched render buffers
	//K15_PostSemaphore(p_RenderContext->renderThreadSync);

	K15_InternalProcessRenderCommandBuffer(p_RenderContext);
}
/*********************************************************************************/
