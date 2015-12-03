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
	uint32 vertexIndex = p_VertexBufferOffsetIndex;

	float leftPos = p_PosLeft;
	float topPos  = p_PosTop;
	
	float fontTextureWidth	 = p_RenderFontDesc->textureWidth;
	float fontTextureHeight  = p_RenderFontDesc->textureHeight;

	float viewportWidth  = p_RenderBackEnd->viewportWidth;
	float viewportHeight = p_RenderBackEnd->viewportHeight;

	float maxGlyphPixelHeight = p_RenderFontDesc->ascent - p_RenderFontDesc->descent;
	float maxGlyphHeight = maxGlyphPixelHeight / viewportHeight;

	for (uint32 textIndex = 0;
		textIndex < p_TextLength;
		++textIndex)
	{
		const char character = p_Text[textIndex];

		float glyphPixelX		= 0.f;
		float glyphPixelY		= 0.f;
		float glyphPixelWidth	= 0.f;
		float glyphPixelHeight	= 0.f;
		float advancePixelX		= 0.f;
		float advancePixelY		= 0.f;
		float bottomPixelOffset	= 0.f;
		bool8 renderableCharacter = K15_FALSE;

		K15_GetFontCharacterInfo(p_RenderFontDesc, p_Text, p_TextLength, textIndex, 
			&glyphPixelX, &glyphPixelY, &glyphPixelWidth, &glyphPixelHeight,
			&advancePixelX, &advancePixelY, &bottomPixelOffset, 
			&renderableCharacter);

		float baseLine = topPos + (p_RenderFontDesc->ascent / viewportHeight);

		float glyphLeftPixelPos	  = glyphPixelX;
		float glyphTopPixelPos	  = glyphPixelY;
		float glyphRightPixelPos  = glyphPixelX + glyphPixelWidth;
		float glyphBottomPixelPos = glyphPixelY + glyphPixelHeight;

		float glyphTexelLeft   = glyphLeftPixelPos / fontTextureWidth;
		float glyphTexelTop    = glyphTopPixelPos / fontTextureHeight;
		float glyphTexelRight  = glyphRightPixelPos / fontTextureWidth;
		float glyphTexelBottom = glyphBottomPixelPos / fontTextureHeight;

		float advanceX = advancePixelX / viewportWidth;
		float advanceY = advancePixelY / viewportHeight;

		float bottomOffset = bottomPixelOffset / viewportHeight;

		float glyphHeight = glyphPixelHeight / viewportHeight;
		float glyphWidth  = glyphPixelWidth / viewportWidth;

		//new line - reset x and advance y
		if (character == '\n')
		{
			leftPos = p_PosLeft;
			topPos -= advanceY;  
		}
		
		if (renderableCharacter)
		{
			//float topOffset	   = 
			float leftPosNDC   = K15_CONVERT_TO_NDC_X(leftPos);
			//float topPosNDC	   = K15_CONVERT_TO_NDC_Y(baseLine - advanceY - glyphHeight);
			float rightPosNDC  = K15_CONVERT_TO_NDC_X(leftPos + glyphWidth);
			//float bottomPosNDC = K15_CONVERT_TO_NDC_Y(baseLine - advanceY);
			float topPosNDC = K15_CONVERT_TO_NDC_Y(baseLine - glyphHeight - advanceY + bottomOffset);
			float bottomPosNDC = K15_CONVERT_TO_NDC_Y(baseLine - advanceY + bottomOffset);
			vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
				leftPosNDC, rightPosNDC, topPosNDC, bottomPosNDC,
				glyphTexelLeft, glyphTexelRight, glyphTexelTop, glyphTexelBottom);
		}

		leftPos += advanceX;
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