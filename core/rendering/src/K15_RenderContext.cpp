#include "K15_RenderContext.h"

#include <K15_OSContext.h>
#include <K15_ErrorCodes.h>
#include <K15_Thread.h>
#include <K15_Window.h>
#include <K15_DefaultCLibraries.h>

#include <K15_Matrix4.h>

#include <K15_GUIContext.h>

#include "K15_RenderBufferDesc.h"
#include "K15_RenderProgramDesc.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderSamplerDesc.h"
#include "K15_RenderStateDesc.h"
#include "K15_RenderFontDesc.h"
#include "K15_RenderTargetDesc.h"
#include "K15_RenderCameraDesc.h"
#include "K15_RenderViewportDesc.h"
#include "K15_RenderMeshDesc.h"

#include "K15_MeshFormat.h"

#include "OpenGL/K15_RenderGLContext.h"

#include "K15_RenderUniformCache.cpp"
#include "K15_RenderProgramDesc.cpp"
#include "K15_RenderCommands.cpp"


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

	uint32 activeViewportIndex = p_RenderContext->activeViewportIndex;
	K15_RenderViewportDesc* activeViewport = &p_RenderContext->viewports[activeViewportIndex];

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
	assert(p_ViewportIndex < K15_RENDER_MAX_VIEWPORT_COUNT);

	p_RenderContext->viewports[p_ViewportIndex] = *p_Viewport;
}
/*********************************************************************************/
intern inline K15_RenderTextureHandle K15_InternalAddRenderTextureDesc(K15_RenderContext* p_RenderContext, K15_RenderTextureDesc* p_RenderTextureDesc)
{
	//get current texture index
	uint32 gpuTextureIndex = p_RenderContext->gpuTexture.amountTextures++;

	assert(gpuTextureIndex < K15_RENDER_MAX_GPU_PROGRAMS);

	p_RenderContext->gpuTexture.textures[gpuTextureIndex] = *p_RenderTextureDesc;

	return (K15_RenderTextureHandle)gpuTextureIndex;
}
/*********************************************************************************/
intern inline K15_RenderProgramHandle K15_InternalAddRenderProgramDesc(K15_RenderContext* p_RenderContext, K15_RenderProgramDesc* p_RenderProgramDesc)
{
	//get current program index
	uint32 gpuProgramIndex = p_RenderContext->gpuProgram.amountPrograms++;

	assert(gpuProgramIndex < K15_RENDER_MAX_GPU_PROGRAMS);

	p_RenderContext->gpuProgram.programs[gpuProgramIndex] = *p_RenderProgramDesc;

	return (K15_RenderProgramHandle)gpuProgramIndex;
}
/*********************************************************************************/
intern inline K15_RenderBufferHandle K15_InternalAddRenderBufferDesc(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc)
{
	//get current buffer index and assign amount
	uint32 gpuBufferIndex = p_RenderContext->gpuBuffer.amountBuffers++;

	assert(gpuBufferIndex < K15_RENDER_MAX_GPU_BUFFER);

	p_RenderContext->gpuBuffer.buffers[gpuBufferIndex] = *p_RenderBufferDesc;

	return (K15_RenderBufferHandle)gpuBufferIndex;
}
/*********************************************************************************/
intern inline K15_RenderSamplerHandle K15_InternalAddRenderSamplerDesc(K15_RenderContext* p_RenderContext, K15_RenderSamplerDesc* p_RenderSamplerDesc)
{
	//get current buffer index and assign amount
	uint32 gpuSamplerIndex = p_RenderContext->gpuSampler.amountSamplers++;

	assert(gpuSamplerIndex < K15_RENDER_MAX_GPU_SAMPLERS);

	p_RenderContext->gpuSampler.samplers[gpuSamplerIndex] = *p_RenderSamplerDesc;

	return (K15_RenderSamplerHandle)gpuSamplerIndex;
}
/*********************************************************************************/
intern inline K15_RenderTargetHandle K15_InternalAddRenderTargetDesc(K15_RenderContext* p_RenderContext, K15_RenderTargetDesc* p_RenderTargetDesc)
{
	//get current render target index and assign amount
	uint32 gpuRenderTargetIndex = p_RenderContext->gpuRenderTargets.amountRenderTargets++;

	assert(gpuRenderTargetIndex < K15_RENDER_MAX_GPU_RENDER_TARGETS);

	p_RenderContext->gpuRenderTargets.renderTargets[gpuRenderTargetIndex] = *p_RenderTargetDesc;

	return (K15_RenderTargetHandle)gpuRenderTargetIndex;
}
/*********************************************************************************/
intern inline K15_RenderMeshHandle K15_InternalAddRenderMeshDesc(K15_RenderContext* p_RenderContext, K15_RenderMeshDesc* p_RenderMeshDesc)
{
	uint32 gpuRenderMeshIndex = p_RenderContext->gpuMeshes.amountMeshes++;

	assert (gpuRenderMeshIndex < K15_RENDER_MAX_GPU_MESHES);

	p_RenderContext->gpuMeshes.meshes[gpuRenderMeshIndex] = *p_RenderMeshDesc;

	return (K15_RenderMeshHandle)gpuRenderMeshIndex;
}
/*********************************************************************************/
intern inline K15_RenderCameraHandle K15_InternalAddRenderCameraDesc(K15_RenderContext* p_RenderContext, K15_RenderCameraDesc* p_RenderCameraDesc)
{
	uint32 cameraIndex = p_RenderContext->amountCameras++;

	assert (cameraIndex < K15_RENDER_MAX_CAMERAS);

	K15_InternalRenderCameraDesc internalCameraDesc = {};

	K15_InternalConvertCameraDescToInternalCameraDesc(p_RenderContext, p_RenderCameraDesc, &internalCameraDesc);

	p_RenderContext->cameras[cameraIndex] = internalCameraDesc;

	return (K15_RenderCameraHandle)cameraIndex;
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
intern void K15_InternalUpdateUniforms(K15_RenderContext* p_RenderContext)
{
	p_RenderContext->commandProcessing.programManagement.updateDirtyUniforms(p_RenderContext);
}
/*********************************************************************************/
intern void K15_InternalBindCamera(K15_RenderContext* p_RenderContext, K15_RenderCameraHandle* p_RenderCameraHandle)
{
	p_RenderContext->activeCameraIndex = *p_RenderCameraHandle;

	K15_RenderUniformCache* uniformCache = p_RenderContext->uniformCache;
	K15_InternalRenderCameraDesc* renderCameraDesc = &p_RenderContext->cameras[*p_RenderCameraHandle];

	K15_UpdateUniformCacheEntry(uniformCache, K15_AUTO_UNIFORM_TYPE_PROJECTION_MATRIX, (byte*)&renderCameraDesc->projectionMatrix);
	K15_UpdateUniformCacheEntry(uniformCache, K15_AUTO_UNIFORM_TYPE_VIEW_MATRIX, (byte*)&renderCameraDesc->viewMatrix);
}
/*********************************************************************************/
intern uint8 K15_InternalDrawMesh(K15_RenderContext* p_RenderContext, K15_RenderMeshHandle* p_RenderMeshHandle, K15_Matrix4* p_WorldMatrix)
{
	assert(p_RenderContext);
	assert(p_RenderMeshHandle);

	K15_RenderMeshDesc* renderMeshDesc = &p_RenderContext->gpuMeshes.meshes[*p_RenderMeshHandle];
	K15_RenderUniformCache* uniformCache = p_RenderContext->uniformCache;

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

		K15_RenderBufferDesc* indexBufferDesc = &p_RenderContext->gpuBuffer.buffers[indexBufferHandle];

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
intern inline uint8 K15_InternalCreateMeshDesc(K15_RenderContext* p_RenderContext, K15_MeshFormat* p_MeshFormat, K15_RenderMeshDesc* p_RenderMeshDesc)
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
		vertexBufferDesc.singleElementSizeInBytes = K15_GetSubMeshFormatVertexDataSize(subMeshFormat) / subMeshFormat->vertexCount;
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
intern void K15_InternalSetDefaultRenderStates(K15_RenderContext* p_RenderContext)
{
	assert(p_RenderContext);

	K15_RenderRasterizerStateDesc rasterizerState = {};
	K15_RenderDepthStateDesc depthState = {};
	K15_RenderStencilStateDesc stencilState = {};
	K15_RenderBlendStateDesc blendState = {};
	K15_RenderProgramDesc vertexShader = {};
	K15_RenderProgramDesc pixelShader = {};
	K15_RenderViewportDesc viewport = {};
	K15_RenderCameraDesc cameraDesc = {};

	const char* glVertexShaderCode =	"in vec3 position;\n"
										"uniform mat4 worldMatrix;\n"
										"uniform mat4 viewMatrix;\n"
										"uniform mat4 projMatrix;\n"
										"void main(void) {\n"
										"vec4 outPosition = vec4(position, 1.0f);\n"
										"mat4 mvp = projMatrix * (viewMatrix * worldMatrix);\n"
										"outPosition = mvp * outPosition;\n"
										"outPosition.xyz /= outPosition.w;\n"
										//"mat4 mvp = projMatrix * (viewMatrix * worldMatrix);\n"
										"gl_Position = outPosition; }\n";

	const char* glPixelShaderCode =		"void main(void) {\n"
										"gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); }\n";

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
	vertexShader.code = (char*)glVertexShaderCode;
	vertexShader.source = K15_RENDER_PROGRAM_SOURCE_CODE;
	vertexShader.type = K15_RENDER_PROGRAM_TYPE_VERTEX;
	/*********************************************************************************/
	pixelShader.code = (char*)glPixelShaderCode;
	pixelShader.source = K15_RENDER_PROGRAM_SOURCE_CODE;
	pixelShader.type = K15_RENDER_PROGRAM_TYPE_FRAGMENT;
	/*********************************************************************************/
	rasterizerState.cullingMode = K15_CULLING_MODE_BACK;
	rasterizerState.fillMode = K15_FILLMODE_SOLID;
	rasterizerState.scissoringEnabled = FALSE;
	rasterizerState.vertexOrder = K15_VERTEX_ORDER_COUNTER_CLOCKWISE;
	/*********************************************************************************/

	/*********************************************************************************/
	depthState.compareFunction = K15_COMPARISON_GREATER_EQUAL;
	//depthState.enabled = TRUE;
	depthState.enabled = FALSE;
	/*********************************************************************************/

	/*********************************************************************************/
	stencilState.enabled = FALSE;
	/*********************************************************************************/

	/*********************************************************************************/
	blendState.enabled = FALSE;
	//blendState.enabled = TRUE;
	blendState.sourceBlendFactorRGB = K15_BLEND_FACTOR_SRC_ALPHA;
	blendState.sourceBlendFactorAlpha = K15_BLEND_FACTOR_ONE;
	blendState.blendOperationRGB = K15_BLEND_OPERATION_ADD;
	blendState.blendOperationAlpha = K15_BLEND_OPERATION_ADD;
	blendState.destinationBlendFactorRGB = K15_BLEND_FACTOR_INVERSE_SRC_ALPHA;
	blendState.destinationBlendFactorAlpha = K15_BLEND_FACTOR_ONE;
	/*********************************************************************************/
	K15_RenderProgramHandle defaultVertexShaderHandle = K15_InternalAddRenderProgramDesc(p_RenderContext, &vertexShader);
	K15_RenderProgramHandle defaultPixelShaderHandle = K15_InternalAddRenderProgramDesc(p_RenderContext, &pixelShader);
	/*********************************************************************************/
	p_RenderContext->commandProcessing.programManagement.createProgram(p_RenderContext, &vertexShader, &defaultVertexShaderHandle);
	p_RenderContext->commandProcessing.programManagement.createProgram(p_RenderContext, &pixelShader, &defaultPixelShaderHandle);
	p_RenderContext->commandProcessing.programManagement.bindProgram(p_RenderContext, &defaultVertexShaderHandle);
	p_RenderContext->commandProcessing.programManagement.bindProgram(p_RenderContext, &defaultPixelShaderHandle);
	/*********************************************************************************/
	K15_InternalSetBlendStateDesc(p_RenderContext, &blendState);
	K15_InternalSetDepthStateDesc(p_RenderContext, &depthState);
	K15_InternalSetStencilStateDesc(p_RenderContext, &stencilState);
	K15_InternalSetRasterizerStateDesc(p_RenderContext, &rasterizerState);
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
			}
#ifdef K15_SHOW_PER_COMMAND_DEBUG_LOG
			else
			{
				K15_LOG_DEBUG_MESSAGE("Did not sent command '%s' to the GPU because the buffer handle is invalid.", K15_ConvertRenderCommandToString(p_RenderCommand->type));
			}
#endif //K15_SHOW_PER_COMMAND_DEBUG_LOG

			break;
		}

		case K15_RENDER_COMMAND_CREATE_MESH:
		{
			K15_MeshFormat meshFormat = {};
			K15_RenderMeshHandle* renderMeshHandle = 0;
			K15_RenderMeshDesc renderMeshDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, K15_PTR_SIZE, 0, &renderMeshHandle);
			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_MeshFormat), K15_PTR_SIZE, &meshFormat);

			result = K15_InternalCreateMeshDesc(p_RenderContext, &meshFormat, &renderMeshDesc);
			*renderMeshHandle = K15_InternalAddRenderMeshDesc(p_RenderContext, &renderMeshDesc);
			
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

		case K15_RENDER_COMMAND_DRAW_GUI:
		{
			K15_GUIContext guiContext = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_GUIContext), 0, &guiContext);

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
				K15_InternalRenderCameraDesc internalRenderCameraDesc = {};
				K15_InternalConvertCameraDescToInternalCameraDesc(p_RenderContext, &renderCameraDesc, &internalRenderCameraDesc);
				p_RenderContext->cameras[*renderCameraHandle] = internalRenderCameraDesc;
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

		case K15_RENDER_COMMAND_SET_GUI_TEXTURE:
		{
			K15_RenderTextureHandle* guiTextureHandle = &p_RenderContext->resources.guiTexture;

			K15_RenderTextureDesc renderTextureDesc = {};

			K15_InternalReadParameter(p_ParameterFrontBuffer, p_RenderCommand, sizeof(K15_RenderTextureDesc), 0, &renderTextureDesc);

			//try to load new gui texture
			K15_RenderTextureHandle newGUITextureHandle = K15_InternalAddRenderTextureDesc(p_RenderContext, &renderTextureDesc);
			result = p_RenderContext->commandProcessing.textureManagement.createTexture(p_RenderContext, &renderTextureDesc, &newGUITextureHandle);

			if (result == K15_SUCCESS &&
				newGUITextureHandle != K15_INVALID_GPU_RESOURCE_HANDLE)
			{
				if (*guiTextureHandle != K15_INVALID_GPU_RESOURCE_HANDLE)
				{
					//delete old gui texture
					result = p_RenderContext->commandProcessing.textureManagement.deleteTexture(p_RenderContext, guiTextureHandle);
				}

				*guiTextureHandle = newGUITextureHandle;
			}

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
	K15_RenderUniformCache* uniformCache = (K15_RenderUniformCache*)malloc(sizeof(K15_RenderUniformCache));
	K15_InitializeRenderUniformCache(uniformCache);

	renderContext->uniformCache = uniformCache;


	//create render resources
	renderContext->resources.guiTexture = K15_INVALID_GPU_RESOURCE_HANDLE;

	//create command queues
	K15_RenderCommandBuffer* renderCommandBuffers = (K15_RenderCommandBuffer*)malloc(sizeof(K15_RenderCommandBuffer) * K15_RENDERING_MAX_COMMAND_BUFFERS);

	if (!renderCommandBuffers)
	{
		assert(false);
		return 0;
	}
	
	/*********************************************************************************/
	//gpu meshes
	K15_RenderMeshDesc* gpuMeshes = (K15_RenderMeshDesc*)malloc(sizeof(K15_RenderMeshDesc) * K15_RENDER_MAX_GPU_MESHES);

	if (!gpuMeshes)
	{
		assert(false);
		return 0;
	}

	renderContext->gpuMeshes.meshes = gpuMeshes;
	renderContext->gpuMeshes.amountMeshes = 0;
	/*********************************************************************************/

	/*********************************************************************************/
	//gpu buffers
	K15_RenderBufferDesc* gpuBuffers = (K15_RenderBufferDesc*)malloc(sizeof(K15_RenderBufferDesc) * K15_RENDER_MAX_GPU_BUFFER);

	if(!gpuBuffers)
	{
		assert(false);
		return 0;
	}

	renderContext->gpuBuffer.buffers = gpuBuffers;
	renderContext->gpuBuffer.amountBuffers = 0;
	/*********************************************************************************/

	/*********************************************************************************/
	//gpu programs
	K15_RenderProgramDesc* gpuPrograms = (K15_RenderProgramDesc*)malloc(sizeof(K15_RenderProgramDesc) * K15_RENDER_MAX_GPU_PROGRAMS);

	if(!gpuPrograms)
	{
		assert(false);
		return 0;
	}

	renderContext->gpuProgram.programs = gpuPrograms;
	renderContext->gpuProgram.amountPrograms = 0;
	/*********************************************************************************/

	/*********************************************************************************/
	//gpu textures
	K15_RenderTextureDesc* gpuTextures = (K15_RenderTextureDesc*)malloc(sizeof(K15_RenderTextureDesc) * K15_RENDER_MAX_GPU_TEXTURES);

	if (!gpuTextures)
	{
		assert(false);
		return 0;
	}

	renderContext->gpuTexture.textures = gpuTextures;
	renderContext->gpuTexture.amountTextures = 0;
	/*********************************************************************************/

	/*********************************************************************************/
	//gpu samplers
	K15_RenderSamplerDesc* gpuSamplers = (K15_RenderSamplerDesc*)malloc(sizeof(K15_RenderSamplerDesc) * K15_RENDER_MAX_GPU_SAMPLERS);

	if (!gpuSamplers)
	{
		assert(false);
		return 0;
	}

	renderContext->gpuSampler.samplers = gpuSamplers;
	renderContext->gpuSampler.amountSamplers = 0;
	/*********************************************************************************/

	/*********************************************************************************/
	//gpu render targets
	K15_RenderTargetDesc* gpuRenderTargets = (K15_RenderTargetDesc*)malloc(sizeof(K15_RenderTargetDesc) * K15_RENDER_MAX_GPU_RENDER_TARGETS);
	
	if (!gpuRenderTargets)
	{
		assert(false);
		return 0;
	}

	renderContext->gpuRenderTargets.renderTargets = gpuRenderTargets;
	renderContext->gpuRenderTargets.amountRenderTargets = 0;
	/*********************************************************************************/

	/*********************************************************************************/
	//camera(s)
	K15_InternalRenderCameraDesc* cameraDescs = (K15_InternalRenderCameraDesc*)malloc(sizeof(K15_InternalRenderCameraDesc) * K15_RENDER_MAX_CAMERAS);

	if (!cameraDescs)
	{
		assert(false);
		return 0;
	}

	renderContext->cameras = cameraDescs;
	renderContext->amountCameras = 0;
	/*********************************************************************************/

	/*********************************************************************************/
	//viewport(s)
	K15_RenderViewportDesc* viewports = (K15_RenderViewportDesc*)malloc(sizeof(K15_RenderViewportDesc) * K15_RENDER_MAX_VIEWPORT_COUNT);

	if (!viewports)
	{
		assert(false);
		return 0;
	}

	renderContext->viewports = viewports;
	renderContext->viewportCount = 0;
	renderContext->activeViewportIndex = 0;
	/*********************************************************************************/

	//states
	renderContext->renderState.blendStateDesc = (K15_RenderBlendStateDesc*)malloc(sizeof(K15_RenderBlendStateDesc));
	renderContext->renderState.depthStateDesc = (K15_RenderDepthStateDesc*)malloc(sizeof(K15_RenderDepthStateDesc));
	renderContext->renderState.stencilStateDesc = (K15_RenderStencilStateDesc*)malloc(sizeof(K15_RenderStencilStateDesc));
	renderContext->renderState.rasterizerStateDesc = (K15_RenderRasterizerStateDesc*)malloc(sizeof(K15_RenderRasterizerStateDesc));
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
	renderContext->viewports[0].x = 0;
	renderContext->viewports[0].y = 0;
	renderContext->viewports[0].width = window->width;
	renderContext->viewports[0].height = window->height;

	for(uint32 viewportIndex = 1;
		viewportIndex < K15_RENDER_MAX_VIEWPORT_COUNT;
		++viewportIndex)
	{
		renderContext->viewports[viewportIndex].x = 0;
		renderContext->viewports[viewportIndex].y = 0;
		renderContext->viewports[viewportIndex].width = 0;
		renderContext->viewports[viewportIndex].height = 0;
	}

	renderContext->viewportCount = 1;
	renderContext->activeViewportIndex = 0;

	//set stub functions
	K15_InternalSetRenderContextStubFunctions(renderContext);

	uint8 contextCreateResult = K15_InternalInitializeRenderContext(renderContext, p_OSContext);

	//Set default render states
	K15_InternalSetDefaultRenderStates(renderContext);

	return renderContext;
}
/*********************************************************************************/
K15_RenderCommandBuffer* K15_CreateRenderCommandBuffer(K15_RenderContext* p_RenderContext)
{
	//only continue when the rendercontext has been initialized
// 	uint32 renderContextFlags = p_RenderContext->flags;
// 
// 	while ((renderContextFlags & K15_RCF_INITIALIZED) > 0); //busy wait until render context is initialized

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
	//renderCommandQueue->swapSection = K15_CreateNamedMutex("RenderCommandQueue Swap");
	
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
