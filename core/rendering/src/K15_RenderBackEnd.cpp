#include "K15_RenderBackEnd.h"

#include "K15_TextureFormat.h"
#include "K15_SamplerFormat.h"
#include "K15_MaterialFormat.h"
#include "K15_FontFormat.h"

#include "K15_Rectangle.h"
#include "K15_Vector2.h"

#include "K15_RenderCommandBuffer.h"
#include "K15_RenderGeometryDesc.h"
#include "K15_RenderFontDesc.h"
#include "K15_RenderMaterialDesc.h"

#include "K15_ErrorCodes.h"

#include "generated/K15_RenderMaterialDataDescStretchBuffer.cpp"

#include "K15_RenderFormats.cpp"
#include "K15_RenderVertexFormatDesc.cpp"
#include "K15_RenderShaderSemantics.cpp"

#include "K15_ShaderCompiler.h"

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
	defaultBlendState.enabled = FALSE;
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
intern result8 K15_InitializeRenderBackEnd(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderContext* p_RenderContext, K15_RenderBackEnd* p_RenderBackEnd)
{
	p_RenderBackEnd->interfaceInitialized = K15_FALSE;

	K15_CreateDefaultRenderState(&p_RenderBackEnd->defaultState);

	p_RenderBackEnd->renderContext = p_RenderContext;
	p_RenderBackEnd->shaderProcessorContext = (K15_ShaderProcessorContext*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_ShaderProcessorContext));
	
	p_RenderBackEnd->shaderProcessorContext->semanticTable = K15_CreateSemanticTable();
	p_RenderBackEnd->shaderProcessorContext->typeTable = K15_CreateTypeTable();
	p_RenderBackEnd->shaderProcessorContext->lastProcessResult.numErrors = 0;
	p_RenderBackEnd->shaderProcessorContext->lastProcessResult.errors = 0;

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

	// 2D Vertex Program
	{
		K15_RenderProgramDesc default2DVertexProgramDesc = {};
		default2DVertexProgramDesc.type = K15_RENDER_PROGRAM_TYPE_VERTEX;
		default2DVertexProgramDesc.code = 
			"K15_Vector4 TransformVertex(in K15_Vector2 pos : POSITION,"
										"in K15_Vector2 uv : TEXCOORD1,"
										"out K15_Vector2 out_uv : TEXCOORD1)\n"
			"{\n"
			"	out_uv = uv;\n"
			"	return K15_Vector4(pos, 1.0, 1.0);\n"
			"}";

		default2DVertexProgramDesc.source = K15_RENDER_PROGRAM_SOURCE_CODE;

		p_RenderBackEnd->renderInterface.createProgram(p_RenderBackEnd, &default2DVertexProgramDesc, &p_RenderBackEnd->resources.default2DVertexProgramHandle);
	}
	/*********************************************************************************/

	// 3D Vertex Program
	{
		K15_RenderProgramDesc default3DVertexProgramDesc = {};
		default3DVertexProgramDesc.type = K15_RENDER_PROGRAM_TYPE_VERTEX;
		default3DVertexProgramDesc.code = 
			"K15_Vector4 TransformVertex(in K15_Vector4 pos : POSITION,"
										"in K15_Vector4 color : COLOR1,"
										"in K15_Matrix4 mvp : MVPMATRIX,"
										"out K15_Vector4 colorOut : COLOR1)\n"
			"{\n"
			"   K15_Vector4 transformedPos = pos * mvp;\n"
			"	colorOut = color;\n"
			"	return transformedPos;\n"
			"}";

		default3DVertexProgramDesc.source = K15_RENDER_PROGRAM_SOURCE_CODE;

		p_RenderBackEnd->renderInterface.createProgram(p_RenderBackEnd, &default3DVertexProgramDesc, &p_RenderBackEnd->resources.default3DVertexProgramHandle);
	}
	/*********************************************************************************/

	// 2D Fragment Program
	{
		K15_RenderProgramDesc default2DFragmentProgramDesc = {};
		default2DFragmentProgramDesc.type = K15_RENDER_PROGRAM_TYPE_FRAGMENT;
		default2DFragmentProgramDesc.code = 
			"K15_Vector4 ShadeFragment(in K15_Vector2 uv : TEXCOORD1,"
									  "in K15_2DTexture tex : DATA,"
									  "in K15_2DSampler sampler : DATA)\n"
			"{\n"
			"   return sampleTex2D(sampler, tex, vec2(uv.x,uv.y));\n"
			"}";

		default2DFragmentProgramDesc.source = K15_RENDER_PROGRAM_SOURCE_CODE;

		p_RenderBackEnd->renderInterface.createProgram(p_RenderBackEnd, &default2DFragmentProgramDesc, &p_RenderBackEnd->resources.default2DFragmentProgramHandle);
	}
	/*********************************************************************************/

	// 3D Fragment Program
	{
		K15_RenderProgramDesc default3DFragmentProgramDesc = {};
		default3DFragmentProgramDesc.type = K15_RENDER_PROGRAM_TYPE_FRAGMENT;
		default3DFragmentProgramDesc.code = 
			"K15_Vector4 ShadeFragment(in K15_Vector4 color : COLOR1)\n"
			"{\n"
			"   return color;\n"
			"}";

		default3DFragmentProgramDesc.source = K15_RENDER_PROGRAM_SOURCE_CODE;

		p_RenderBackEnd->renderInterface.createProgram(p_RenderBackEnd, &default3DFragmentProgramDesc, &p_RenderBackEnd->resources.default3DFragmentProgramHandle);
	}
	/*********************************************************************************/

	// default font material
	{
		K15_RenderMaterialDesc defaultFontMaterial = {};
		defaultFontMaterial.numMaterialPasses = 1;

		defaultFontMaterial.materialPasses = (K15_RenderMaterialPassDesc*)K15_AllocateFromMemoryAllocator(rendererAllocator, sizeof(K15_RenderMaterialPassDesc));
		defaultFontMaterial.materialPasses[0].fragmentShaderHandle = &p_RenderBackEnd->resources.default2DFragmentProgramHandle;
		defaultFontMaterial.materialPasses[0].vertexShaderHandle = &p_RenderBackEnd->resources.default2DVertexProgramHandle;
		K15_CreateRenderMaterialDataDescStretchBufferWithCustomAllocator(&defaultFontMaterial.materialPasses[0].materialData, *rendererAllocator, 2);

		//sampler
		K15_RenderMaterialDataDesc fontSampler = {};
		fontSampler.typeID = K15_TYPE_SAMPLER_2D_ID;
		fontSampler.semanticID = K15_SEMANTIC_DATA_ID;
		fontSampler.semanticGroupID = K15_SEMANTIC_GROUP_ID_PER_MATERIAL;
		fontSampler.nameHash = K15_GenerateStringHash("sampler");

		//texture
		K15_RenderMaterialDataDesc fontTexture = {};
		fontTexture.typeID = K15_TYPE_TEXTURE_2D_ID;
		fontTexture.semanticID = K15_SEMANTIC_DATA_ID;
		fontTexture.semanticGroupID = K15_SEMANTIC_GROUP_ID_PER_MATERIAL;
		fontTexture.nameHash = K15_GenerateStringHash("tex");

		K15_PushRenderMaterialDataDescStretchBufferElement(&defaultFontMaterial.materialPasses[0].materialData, fontSampler);
		K15_PushRenderMaterialDataDescStretchBufferElement(&defaultFontMaterial.materialPasses[0].materialData, fontTexture);

		p_RenderBackEnd->resources.defaultFontMaterial = defaultFontMaterial;
	}
	/*********************************************************************************/
	p_RenderBackEnd->renderInterface.setBlendState(p_RenderBackEnd, &p_RenderBackEnd->defaultState.blendState);
	p_RenderBackEnd->renderInterface.setDepthState(p_RenderBackEnd, &p_RenderBackEnd->defaultState.depthState);
	p_RenderBackEnd->renderInterface.setRasterizerState(p_RenderBackEnd, &p_RenderBackEnd->defaultState.rasterizerState);
	p_RenderBackEnd->renderInterface.setStencilState(p_RenderBackEnd, &p_RenderBackEnd->defaultState.stencilState);
	/*********************************************************************************/
}
/*********************************************************************************/



//RENDER COMMANDS
/*********************************************************************************/
intern result8 K15_InternalDrawGeometry(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderGeometryDesc* p_RenderGeometryDesc)
{
	return p_RenderBackEnd->renderInterface.drawGeometry(p_RenderBackEnd, p_RenderGeometryDesc);
}
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

	return p_RenderBackEnd->renderInterface.resizeViewport(p_RenderBackEnd, width, height);
}
/*********************************************************************************/
intern void K15_InternalRender2DTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;
	K15_RenderResourceHandle* textureHandle = 0;
	K15_RenderMaterialDesc renderMaterialDesc = {};
	K15_Rectangle destinationRect = {};
	K15_Rectangle sourceRect = {};

	uint32 localOffset = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, K15_PTR_SIZE, &textureHandle);
	localOffset += K15_PTR_SIZE;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_RenderMaterialDesc), &renderMaterialDesc);
	localOffset += sizeof(K15_RenderMaterialDesc);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_Rectangle), &destinationRect);
	localOffset += sizeof(K15_Rectangle);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_Rectangle), &sourceRect);
	localOffset += sizeof(K15_Rectangle);
	
	K15_RenderVertexFormatDesc vertexFormatDesc = K15_CreateRenderVertexFormatDesc(p_RenderBackEnd->renderContext, 2, 
		K15_SEMANTIC_POSITION_ID, K15_TYPE_FLOAT_VECTOR2_ID,
		K15_SEMANTIC_TEXCOORD1_ID, K15_TYPE_FLOAT_VECTOR2_ID);

	uint32 numVertices = 4;
	uint32 sizeVerticesInBytes = vertexFormatDesc.stride * numVertices;
	uint32 index = 0;

	float* vertexMemory = (float*)K15_AllocateFromMemoryAllocator(renderAllocator, sizeVerticesInBytes);

	K15_Vector2 d_ul = K15_GetUpperLeftCorner(destinationRect);
	K15_Vector2 d_bl = K15_GetBottomLeftCorner(destinationRect);
	K15_Vector2 d_ur = K15_GetUpperRightCorner(destinationRect);
	K15_Vector2 d_br = K15_GetBottomRightCorner(destinationRect);

	K15_Vector2 s_ul = K15_GetUpperLeftCorner(sourceRect);
	K15_Vector2 s_bl = K15_GetBottomLeftCorner(sourceRect);
	K15_Vector2 s_ur = K15_GetUpperRightCorner(sourceRect);
	K15_Vector2 s_br = K15_GetBottomRightCorner(sourceRect);

	//vertex 1
	vertexMemory[index++] = d_bl.x;
	vertexMemory[index++] = d_bl.y;
	vertexMemory[index++] = s_bl.x;
	vertexMemory[index++] = s_bl.y;

	//vertex 2
	vertexMemory[index++] = d_ul.x;
	vertexMemory[index++] = d_ul.y;
	vertexMemory[index++] = s_ul.x;
	vertexMemory[index++] = s_ul.y;

	//vertex 3
	vertexMemory[index++] = d_br.x;
	vertexMemory[index++] = d_br.y;
	vertexMemory[index++] = s_br.x;
	vertexMemory[index++] = s_br.y;

	//vertex 4
	vertexMemory[index++] = d_ur.x;
	vertexMemory[index++] = d_ur.y;
	vertexMemory[index++] = s_ur.x;
	vertexMemory[index++] = s_ur.y;
	
	K15_RenderVertexData* vertexData = p_RenderBackEnd->renderInterface.updateVertexData(p_RenderBackEnd, vertexMemory, numVertices, &vertexFormatDesc);
	K15_RenderGeometryDesc renderGeometry = {};

	renderGeometry.vertexData = vertexData;
	renderGeometry.topology = K15_RENDER_TOPOLOGY_TRIANGLE_STRIP;
	renderGeometry.worldMatrix = K15_GetIdentityMatrix4();
	renderGeometry.material = &renderMaterialDesc;

	K15_InternalDrawGeometry(p_RenderBackEnd, &renderGeometry);

	p_RenderBackEnd->renderInterface.freeVertexData(p_RenderBackEnd, vertexData);
	K15_FreeFromMemoryAllocator(renderAllocator, vertexMemory);
}
/*********************************************************************************/
intern void K15_InternalRender2DText(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderCommand* p_RenderCommand, K15_RenderCommandBuffer* p_RenderCommandBuffer, uint32 p_BufferOffset)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;

	K15_RenderFontDesc fontDesc = {};
	K15_Vector2 pos = {};
	char* text = 0;

	uint32 localOffset = 0;
	uint32 textLength = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_RenderFontDesc), &fontDesc);
	localOffset += sizeof(K15_RenderFontDesc);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(uint32), &textLength);
	localOffset += sizeof(uint32);

	text = (char*)alloca(textLength + 1);

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, textLength, text);
	localOffset += textLength;

	text[textLength] = 0;

	K15_ReadMemoryFromCommandBuffer(p_RenderCommandBuffer, p_BufferOffset + localOffset, sizeof(K15_Vector2), &pos);
	localOffset += sizeof(K15_Vector2);

	K15_RenderMaterialDesc* fontMaterial = &p_RenderBackEnd->resources.defaultFontMaterial;
	K15_SetRenderMaterialRenderResourceDataByName(&fontMaterial->materialPasses[0], "sampler", fontDesc.bla);
	K15_SetRenderMaterialRenderResourceDataByName(&fontMaterial->materialPasses[0], "tex", fontDesc.textureHandle);

	uint32 numVertices = textLength * 6;
	uint32 vertexIndex = 0;
	uint32 indexBufferIndex = 0;
	uint16 index = 0;

	K15_RenderVertexFormatDesc vertexFormatDesc = K15_CreateRenderVertexFormatDesc(p_RenderBackEnd->renderContext, 2, 
		K15_SEMANTIC_POSITION_ID, K15_TYPE_FLOAT_VECTOR2_ID,
		K15_SEMANTIC_TEXCOORD1_ID, K15_TYPE_FLOAT_VECTOR2_ID);

	uint32 sizeVerticesInBytes = vertexFormatDesc.stride * numVertices;

	float xOffset = 0.f;
	float yOffset = 0.f;
	float x = pos.x;
	float y = pos.y;

	float glyphX = 0.f;
	float glyphY = 0.f;
	float glyphW = 0.f;
	float glyphH = 0.f;

	float texelLeft = 0.f;
	float texelTop = 0.f;
	float texelRight = 0.f;
	float texelBottom = 0.f;

	float* vertexMemory = (float*)K15_AllocateFromMemoryAllocator(renderAllocator, sizeVerticesInBytes);
	
	for (uint32 charIndex = 0;
		charIndex < textLength;
		++charIndex)
	{
		unsigned char currentChar = text[charIndex];

		if (currentChar == 0)
		{
			continue;
		}

		if (isspace(currentChar))
		{
			glyphX = 0.0f;
			glyphY = 0.f;
			glyphW = fontDesc.scaleFactor;
			glyphH = 0.f;

			texelLeft = 0.f;
			texelRight = 0.f;
			texelBottom = 0.f;
			texelTop = 0.f;
		}
		else if (currentChar >= fontDesc.startCharacter &&
				currentChar <= fontDesc.endCharacter)
		{
			K15_RenderGlyphDesc* glyphDesc = &fontDesc.glyphDescs[currentChar - fontDesc.startCharacter];

			float kerningX = 0.f;

			if (charIndex+1 < textLength)
			{
				unsigned int shiftedCharacter = currentChar << text[charIndex+1];
				//fontDesc.kernDescs[shiftedCharacter].kerning;
			}
			//fontDesc.kernDescs[]

			glyphX = glyphDesc->x + kerningX;
			glyphY = glyphDesc->y;
			glyphW = glyphDesc->width;
			glyphH = glyphDesc->height;

			texelLeft = glyphX / fontDesc.textureWidth;
			texelTop = glyphY / fontDesc.textureWidth;
			texelRight = glyphW + texelLeft;
			texelBottom = glyphH + texelTop;
		}

		vertexMemory[vertexIndex++] = x;
		vertexMemory[vertexIndex++] = y;
		vertexMemory[vertexIndex++] = texelLeft;
		vertexMemory[vertexIndex++] = 1.f- texelBottom;//texelTop;

		vertexMemory[vertexIndex++] = x;
		vertexMemory[vertexIndex++] = y + glyphH;
		vertexMemory[vertexIndex++] = texelLeft;
		vertexMemory[vertexIndex++] = 1.f - texelTop;//texelBottom;

		vertexMemory[vertexIndex++] = x + glyphW;
		vertexMemory[vertexIndex++] = y;
		vertexMemory[vertexIndex++] = texelRight;
		vertexMemory[vertexIndex++] = 1.f - texelBottom;//texelTop;

		vertexMemory[vertexIndex++] = x;
		vertexMemory[vertexIndex++] = y + glyphH;
		vertexMemory[vertexIndex++] = texelLeft;
		vertexMemory[vertexIndex++] = 1.f - texelTop;//texelBottom;

		vertexMemory[vertexIndex++] = x + glyphW;
		vertexMemory[vertexIndex++] = y + glyphH;
		vertexMemory[vertexIndex++] = texelRight;
		vertexMemory[vertexIndex++] = 1.f - texelTop;//texelBottom;

		vertexMemory[vertexIndex++] = x + glyphW;
		vertexMemory[vertexIndex++] = y;
		vertexMemory[vertexIndex++] = texelRight;
		vertexMemory[vertexIndex++] = 1.f - texelBottom;//texelTop;

		x += glyphW;
	}

	K15_RenderVertexData* vertexData = p_RenderBackEnd->renderInterface.updateVertexData(p_RenderBackEnd, vertexMemory, numVertices, &vertexFormatDesc);
	K15_RenderGeometryDesc renderGeometry = {};

	renderGeometry.vertexData = vertexData;
	renderGeometry.topology = K15_RENDER_TOPOLOGY_TRIANGLES;
	renderGeometry.worldMatrix = K15_GetIdentityMatrix4();
	renderGeometry.material = &p_RenderBackEnd->resources.defaultFontMaterial;

	K15_InternalDrawGeometry(p_RenderBackEnd, &renderGeometry);

	p_RenderBackEnd->renderInterface.freeVertexData(p_RenderBackEnd, vertexData);
	//K15_FreeFromMemoryAllocator(renderAllocator, vertexMemory);
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

		if (programDesc.source == K15_RENDER_PROGRAM_SOURCE_FILE)
		{
			K15_LOG_ERROR_MESSAGE("Could not create shader '%s' ('%s')", programDesc.file, error);
		}
		else
		{
			K15_LOG_ERROR_MESSAGE("Could not create shader ('%s')", error);
		}
	}

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

			case K15_RENDER_COMMAND_RENDER_2D_TEXT:
			{
				K15_InternalRender2DText(p_RenderBackEnd, command, p_RenderCommandBuffer, offset);
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
void K15_EndFrame(K15_RenderBackEnd * p_RenderBackEnd)
{
	K15_ASSERT_TEXT(p_RenderBackEnd, "Render Back End is NULL.");

	p_RenderBackEnd->renderInterface.swapFrameBuffer(p_RenderBackEnd);
}
/*********************************************************************************/