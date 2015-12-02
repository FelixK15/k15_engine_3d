/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspaceRectVertices(K15_RenderBackEnd* p_RenderBackEnd, 
													float* p_VertexBuffer, uint32 p_VertexBufferOffsetIndex, 
													float p_PosLeft, float p_PosRight, float p_PosTop, float p_PosBottom,
													float p_UCoordLeft, float p_UCoordRight, float p_VCoordTop, float p_VCoordBottom)
{
	uint32 vertexIndex = p_VertexBufferOffsetIndex;

	p_VertexBuffer[vertexIndex++] = p_PosLeft;
	p_VertexBuffer[vertexIndex++] = p_PosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	
	p_VertexBuffer[vertexIndex++] = p_PosLeft;
	p_VertexBuffer[vertexIndex++] = p_PosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	
	p_VertexBuffer[vertexIndex++] = p_PosRight;
	p_VertexBuffer[vertexIndex++] = p_PosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	
	p_VertexBuffer[vertexIndex++] = p_PosLeft;
	p_VertexBuffer[vertexIndex++] = p_PosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	
	p_VertexBuffer[vertexIndex++] = p_PosRight;
	p_VertexBuffer[vertexIndex++] = p_PosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	
	p_VertexBuffer[vertexIndex++] = p_PosRight;
	p_VertexBuffer[vertexIndex++] = p_PosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DTextVertices(K15_RenderBackEnd* p_RenderBackEnd,
													K15_RenderFontDesc* p_RenderFontDesc,
													float* p_VertexBuffer, uint32 p_VertexBufferOffsetIndex,
													float p_PosLeft, float p_PosTop, const char* p_Text,
													uint32 p_TextLength)
{
	float xOffset = 0.f;
	float yOffset = 0.f;
	float x = p_PosLeft;
	float y = p_PosTop;

	float glyphX = 0.f;
	float glyphY = 0.f;
	float glyphW = 0.f;
	float glyphH = 0.f;

	float texelLeft = 0.f;
	float texelTop = 0.f;
	float texelRight = 0.f;
	float texelBottom = 0.f;

	float baseLine = p_RenderFontDesc->ascent / p_RenderFontDesc->textureHeight;

	uint32 vertexIndex = p_VertexBufferOffsetIndex;
	uint32 numCharacter = p_RenderFontDesc->endCharacter - p_RenderFontDesc->startCharacter;
	uint32 numKerningElements = numCharacter * numCharacter;
	float gx = 0.f;

	for (uint32 charIndex = 0;
		charIndex < p_TextLength;
		++charIndex)
	{
		float offsetY = 0.f;
		float advanceX = 0.f;
		float advanceY = 0.f;
		float glyphX = 0.f;
		float glyphY = 0.f;
		float glyphWidth = 0.f;
		float glyphHeight = 0.f;
		bool8 renderable = K15_FALSE;

		K15_GetFontCharacterInfo(p_RenderFontDesc, p_Text, p_TextLength, charIndex, &glyphX, &glyphY, &glyphWidth, &glyphHeight, &advanceX, &advanceY, &renderable);

		if (p_Text[charIndex] == '\n')
		{
			x = p_PosLeft;
			y -= advanceY / p_RenderBackEnd->viewportHeight;
			continue;
		}
		else
		{
			offsetY += advanceY / p_RenderBackEnd->viewportHeight;
		}

		glyphX = glyphX;
		glyphY = glyphY;
		glyphW = glyphWidth / p_RenderFontDesc->textureWidth;
		glyphH = glyphHeight / p_RenderFontDesc->textureHeight;

		float glyphWScreen = glyphWidth/ p_RenderBackEnd->viewportWidth;
		float glyphHScreen = glyphHeight/ p_RenderBackEnd->viewportHeight;

		float glyphBaseLineDiff = (baseLine - glyphHScreen) + glyphHScreen;

		float leftBound = K15_CONVERT_TO_NDC_X(x);
		float rightBound = K15_CONVERT_TO_NDC_X(x + glyphWScreen);
		float topBound = K15_CONVERT_TO_NDC_Y(y + glyphBaseLineDiff);
		float bottomBound = K15_CONVERT_TO_NDC_Y(y + glyphHScreen + glyphBaseLineDiff);

		texelLeft	= glyphX / p_RenderFontDesc->textureWidth;
		texelTop	= glyphY / p_RenderFontDesc->textureWidth;
		texelRight  = glyphW + texelLeft;
		texelBottom = glyphH + texelTop;

		if (renderable)
		{
			vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
				leftBound, rightBound, topBound, bottomBound, 
				texelLeft, texelRight, texelTop, texelBottom);
		}

		x += glyphWScreen;
		gx += glyphWScreen;
	}

	return vertexIndex;
}
/*********************************************************************************/
intern result8 K15_InternalDrawGeometry(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderGeometryDesc* p_RenderGeometryDesc)
{
	return p_RenderBackEnd->renderInterface.drawGeometry(p_RenderBackEnd, p_RenderGeometryDesc);
}
/*********************************************************************************/
intern void K15_InternalDrawDefault2DTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_TextureHandle,
											 float p_PosLeft, float p_PosRight, float p_PosTop, float p_PosBottom,
											 float p_UCoordLeft, float p_UCoordRight, float p_VCoordTop, float p_VCoordBottom)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;

	K15_RenderVertexFormatDesc vertexFormatDesc = K15_CreateRenderVertexFormatDesc(p_RenderBackEnd->renderContext, 2, 
		K15_SEMANTIC_POSITION_ID, K15_TYPE_FLOAT_VECTOR2_ID,
		K15_SEMANTIC_TEXCOORD1_ID, K15_TYPE_FLOAT_VECTOR2_ID);

	uint32 numVertices = 6;
	uint32 sizeVerticesInBytes = vertexFormatDesc.stride * numVertices;

	float* vertexMemory = (float*)K15_AllocateFromMemoryAllocator(renderAllocator, sizeVerticesInBytes);

	K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, vertexMemory, 0, 
		p_PosLeft, p_PosRight, p_PosTop, p_PosBottom, 
		p_UCoordLeft, p_UCoordRight, p_VCoordTop, p_VCoordBottom);

	K15_RenderVertexData* vertexData = p_RenderBackEnd->renderInterface.updateVertexData(p_RenderBackEnd, vertexMemory, numVertices, &vertexFormatDesc);
	K15_RenderGeometryDesc renderGeometry = {};

	K15_SetRenderMaterialRenderResourceDataByName(&p_RenderBackEnd->resources.materials.default2DMaterial.materialPasses[0], "tex", p_TextureHandle);

	renderGeometry.vertexData = vertexData;
	renderGeometry.topology = K15_RENDER_TOPOLOGY_TRIANGLE_STRIP;
	renderGeometry.worldMatrix = K15_GetIdentityMatrix4();
	renderGeometry.material = &p_RenderBackEnd->resources.materials.default2DMaterial;

	K15_InternalDrawGeometry(p_RenderBackEnd, &renderGeometry);

	p_RenderBackEnd->renderInterface.freeVertexData(p_RenderBackEnd, vertexData);
	K15_FreeFromMemoryAllocator(renderAllocator, vertexMemory);
}
/*********************************************************************************/