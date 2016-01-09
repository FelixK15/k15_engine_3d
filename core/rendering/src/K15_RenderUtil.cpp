#include "K15_Vector3.h"

/*********************************************************************************/
enum K15_InternalRoundCornerFlags
{
	K15_LEFT_TOP_CORNER		= 0x01,
	K15_RIGHT_TOP_CORNER	= 0x02,
	K15_LEFT_BOTTOM_CORNER	= 0x04,
	K15_RIGHT_BOTTOM_CORNER = 0x08,

	K15_ONLY_TOP_CORNERS	= K15_LEFT_TOP_CORNER | K15_RIGHT_TOP_CORNER,
	K15_ONLY_BOTTOM_CORNERS = K15_LEFT_BOTTOM_CORNER | K15_RIGHT_BOTTOM_CORNER,
	K15_ALL_CORNERS = K15_ONLY_BOTTOM_CORNERS | K15_ONLY_TOP_CORNERS
};
/*********************************************************************************/



/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredSquareVertices(K15_RenderBackEnd* p_RenderBackEnd,
	float* p_VertexBuffer, uint32 p_StartIndex,
	int32 p_PixelPosX, int32 p_PixelPosY, uint32 p_PixelRadius,
	uint32 p_CenterColor, uint32 p_EdgeColor)
{
	float viewportHeight = p_RenderBackEnd->viewportHeight;
	float viewportWidth = p_RenderBackEnd->viewportWidth;

	uint32 vertexIndex = p_StartIndex;

	K15_Vector3 unpackedCenterColor = K15_UnpackVector3(p_CenterColor) / 255.f;
	K15_Vector3 unpackedEdgeColor= K15_UnpackVector3(p_EdgeColor) / 255.f;

	float theta = K15_TWO_PI / K15_GUI_2D_SQUARE_SAMPLES;
	float angle = theta;

	float centerNDCPosX = K15_CONVERT_TO_NDC_X(p_PixelPosX / viewportWidth);
	float centerNDCPosY = K15_CONVERT_TO_NDC_Y(p_PixelPosY / viewportHeight);

	float lastPixelPosX = p_PixelPosX + p_PixelRadius;
	float lastPixelPosY = p_PixelPosY;

	for (uint32 sampleIndex = 0;
		sampleIndex < K15_GUI_2D_SQUARE_SAMPLES;
		++sampleIndex)
	{
		float pixelPosX = K15_HighPrecisionCos(angle) * p_PixelRadius;
		float pixelPosY = K15_HighPrecisionSin(angle) * p_PixelRadius;

		pixelPosX += p_PixelPosX;
		pixelPosY += p_PixelPosY;

		//center vertex
		p_VertexBuffer[vertexIndex++] = centerNDCPosX;
		p_VertexBuffer[vertexIndex++] = centerNDCPosY;
		p_VertexBuffer[vertexIndex++] = unpackedCenterColor.x;
		p_VertexBuffer[vertexIndex++] = unpackedCenterColor.y;
		p_VertexBuffer[vertexIndex++] = unpackedCenterColor.z;

		//edge vertex last
		p_VertexBuffer[vertexIndex++] = K15_CONVERT_TO_NDC_X(lastPixelPosX / viewportWidth);
		p_VertexBuffer[vertexIndex++] = K15_CONVERT_TO_NDC_Y(lastPixelPosY / viewportHeight);
		p_VertexBuffer[vertexIndex++] = unpackedEdgeColor.x;
		p_VertexBuffer[vertexIndex++] = unpackedEdgeColor.y;
		p_VertexBuffer[vertexIndex++] = unpackedEdgeColor.z;

		//edge vertex current
		p_VertexBuffer[vertexIndex++] = K15_CONVERT_TO_NDC_X(pixelPosX / viewportWidth);
		p_VertexBuffer[vertexIndex++] = K15_CONVERT_TO_NDC_Y(pixelPosY / viewportHeight);
		p_VertexBuffer[vertexIndex++] = unpackedEdgeColor.x;
		p_VertexBuffer[vertexIndex++] = unpackedEdgeColor.y;
		p_VertexBuffer[vertexIndex++] = unpackedEdgeColor.z;

		angle += theta;

		lastPixelPosX = pixelPosX;
		lastPixelPosY = pixelPosY;
	}

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredRectVertices(K15_RenderBackEnd* p_RenderBackEnd,
	float* p_VertexBuffer, uint32 p_StartIndex,
	int32 p_PixelPosLeft, int32 p_PixelPosRight, int32 p_PixelPosTop, int32 p_PixelPosBottom,
	uint32 p_ColorLeftTop, uint32 p_ColorRightTop, uint32 p_ColorLeftBottom, uint32 p_ColorRightBottom)
{
	float viewportHeight = p_RenderBackEnd->viewportHeight;
	float viewportWidth = p_RenderBackEnd->viewportWidth;

	float posLeftNDC = K15_CONVERT_TO_NDC_X((float)p_PixelPosLeft / viewportWidth);
	float posRightNDC = K15_CONVERT_TO_NDC_X((float)p_PixelPosRight / viewportWidth);
	float posTopNDC = K15_CONVERT_TO_NDC_Y((float)p_PixelPosTop / viewportHeight);
	float posBottomNDC = K15_CONVERT_TO_NDC_Y((float)p_PixelPosBottom / viewportHeight);

	K15_Vector3 unpackedLeftBottomColor = K15_UnpackVector3(p_ColorLeftBottom) / 255.f;
	K15_Vector3 unpackedLeftTopColor = K15_UnpackVector3(p_ColorLeftTop) / 255.f;
	K15_Vector3 unpackedRightBottomColor = K15_UnpackVector3(p_ColorRightBottom) / 255.f;
	K15_Vector3 unpackedRightTopColor = K15_UnpackVector3(p_ColorRightTop) / 255.f;

	uint32 vertexIndex = p_StartIndex;

	p_VertexBuffer[vertexIndex++] = posLeftNDC;
	p_VertexBuffer[vertexIndex++] = posTopNDC;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.z;

	p_VertexBuffer[vertexIndex++] = posLeftNDC;
	p_VertexBuffer[vertexIndex++] = posBottomNDC;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.z;

	p_VertexBuffer[vertexIndex++] = posRightNDC;
	p_VertexBuffer[vertexIndex++] = posTopNDC;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.z;

	p_VertexBuffer[vertexIndex++] = posLeftNDC;
	p_VertexBuffer[vertexIndex++] = posBottomNDC;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.z;

	p_VertexBuffer[vertexIndex++] = posRightNDC;
	p_VertexBuffer[vertexIndex++] = posBottomNDC;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.z;

	p_VertexBuffer[vertexIndex++] = posRightNDC;
	p_VertexBuffer[vertexIndex++] = posTopNDC;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.z;

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredRoundRectVertices(K15_RenderBackEnd* p_RenderBackEnd,
	float* p_VertexBuffer, uint32 p_StartIndex,
	int32 p_PixelPosLeft, int32 p_PixelPosRight, int32 p_PixelPosTop, int32 p_PixelPosBottom,
	uint32 p_ColorLeftTop, uint32 p_ColorRightTop, uint32 p_ColorLeftBottom, uint32 p_ColorRightBottom,
	uint32 p_RoundCornerFlags, float p_EdgeRoundFactor)
{
	float roundFactor = K15_ClampReal(p_EdgeRoundFactor, 1.f, 0.f);
	uint32 cornerPixelRadius = roundFactor * ((p_PixelPosBottom - p_PixelPosTop) / 2);
	uint32 vertexIndex = p_StartIndex;

	if (roundFactor == 0.f)
	{
		return K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, p_VertexBuffer, p_StartIndex,
			p_PixelPosLeft, p_PixelPosRight, p_PixelPosTop, p_PixelPosBottom,
			p_ColorLeftTop, p_ColorRightTop, p_ColorLeftBottom, p_ColorRightBottom);
	}

	K15_Vector3 unpackedLeftTopColor	 = K15_UnpackVector3(p_ColorLeftTop);
	K15_Vector3 unpackedLeftBottomColor  = K15_UnpackVector3(p_ColorLeftBottom);
	K15_Vector3 unpackedRightTopColor	 = K15_UnpackVector3(p_ColorRightTop);
	K15_Vector3 unpackedRightBottomColor = K15_UnpackVector3(p_ColorRightBottom);

	K15_Vector3 colorLeftBelowCorner  = K15_Lerp(unpackedLeftTopColor, unpackedLeftBottomColor, roundFactor);
	K15_Vector3 colorRightBelowCorner = K15_Lerp(unpackedRightTopColor, unpackedRightBottomColor, roundFactor);
	K15_Vector3 colorLeftAboveCorner  = K15_Lerp(unpackedLeftBottomColor, unpackedLeftTopColor, roundFactor);
	K15_Vector3 colorRightAboveCorner = K15_Lerp(unpackedRightBottomColor, unpackedRightTopColor, roundFactor);
		
	K15_Vector3 colorRightFromUpperCorner = K15_Lerp(unpackedLeftTopColor, unpackedRightTopColor, roundFactor);
	K15_Vector3 colorLeftFromUpperCorner  = K15_Lerp(unpackedRightTopColor, unpackedLeftTopColor, roundFactor);
	K15_Vector3 colorRightFromLowerCorner = K15_Lerp(unpackedLeftBottomColor, unpackedRightBottomColor, roundFactor);
	K15_Vector3 colorLeftFromLowerCorner  = K15_Lerp(unpackedRightBottomColor, unpackedLeftBottomColor, roundFactor);

	uint32 packedColorLeftBelowCorner  = K15_PackVector3(colorLeftBelowCorner);
	uint32 packedColorRightBelowCorner = K15_PackVector3(colorRightBelowCorner);
	uint32 packedColorLeftAboveCorner  = K15_PackVector3(colorLeftAboveCorner);
	uint32 packedColorRightAboveCorner = K15_PackVector3(colorRightAboveCorner);
		
	uint32 packedColorRightFromUpperCorner = K15_PackVector3(colorRightFromUpperCorner);
	uint32 packedColorLeftFromUpperCorner  = K15_PackVector3(colorLeftFromUpperCorner);
	uint32 packedColorRightFromLowerCorner = K15_PackVector3(colorRightFromLowerCorner);
	uint32 packedColorLeftFromLowerCorner  = K15_PackVector3(colorLeftFromLowerCorner);

	if (p_RoundCornerFlags & K15_LEFT_TOP_CORNER)
	{
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredSquareVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
			p_PixelPosLeft + cornerPixelRadius, p_PixelPosTop + cornerPixelRadius, cornerPixelRadius,
			packedColorLeftBelowCorner, p_ColorLeftTop);
	}

	if (p_RoundCornerFlags & K15_LEFT_BOTTOM_CORNER)
	{
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredSquareVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
			p_PixelPosLeft + cornerPixelRadius, p_PixelPosBottom - cornerPixelRadius, cornerPixelRadius,
			packedColorLeftAboveCorner, p_ColorLeftBottom);
	}

	if (p_RoundCornerFlags & K15_RIGHT_TOP_CORNER)
	{
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredSquareVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
			p_PixelPosRight - cornerPixelRadius, p_PixelPosTop + cornerPixelRadius, cornerPixelRadius,
			packedColorRightBelowCorner, p_ColorRightTop);
	}

	if (p_RoundCornerFlags & K15_RIGHT_BOTTOM_CORNER)
	{
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredSquareVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
			p_PixelPosRight - cornerPixelRadius, p_PixelPosBottom - cornerPixelRadius, cornerPixelRadius,
			packedColorRightAboveCorner, p_ColorRightBottom);
	}

	//mid non rounded part (horizontal)
	vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
		p_PixelPosLeft, p_PixelPosRight, 
		p_PixelPosTop + cornerPixelRadius, p_PixelPosBottom - cornerPixelRadius,
		packedColorLeftBelowCorner, packedColorRightBelowCorner, 
		packedColorLeftAboveCorner, packedColorRightAboveCorner);

	//mid non rounded part (vertical)
	vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
		p_PixelPosLeft + cornerPixelRadius, p_PixelPosRight - cornerPixelRadius,
		p_PixelPosTop, p_PixelPosBottom,
		packedColorLeftFromUpperCorner, packedColorRightFromUpperCorner,
		packedColorLeftFromLowerCorner, packedColorRightFromLowerCorner);
	
	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTextureRectVertices(K15_RenderBackEnd* p_RenderBackEnd, 
													float* p_VertexBuffer, uint32 p_StartIndex, 
													uint32 p_PixelPosLeft, uint32 p_PixelPosRight, uint32 p_PixelPosTop, uint32 p_PixelPosBottom,
													float p_UCoordLeft, float p_UCoordRight, float p_VCoordTop, float p_VCoordBottom,
													uint32 p_ColorLeftTop, uint32 p_ColorRightTop, uint32 p_ColorLeftBottom, uint32 p_ColorRightBottom)
{
	float viewportHeight = p_RenderBackEnd->viewportHeight;
	float viewportWidth = p_RenderBackEnd->viewportWidth;

	float posLeftNDC = K15_CONVERT_TO_NDC_X((float)p_PixelPosLeft / viewportWidth);
	float posRightNDC = K15_CONVERT_TO_NDC_X((float)p_PixelPosRight / viewportWidth);
	float posTopNDC = K15_CONVERT_TO_NDC_Y((float)p_PixelPosTop / viewportHeight);
	float posBottomNDC = K15_CONVERT_TO_NDC_Y((float)p_PixelPosBottom / viewportHeight);

	K15_Vector3 unpackedLeftBottomColor = K15_UnpackVector3(p_ColorLeftBottom) / 255.f;
	K15_Vector3 unpackedLeftTopColor = K15_UnpackVector3(p_ColorLeftTop) / 255.f;
	K15_Vector3 unpackedRightBottomColor = K15_UnpackVector3(p_ColorRightBottom) / 255.f;
	K15_Vector3 unpackedRightTopColor = K15_UnpackVector3(p_ColorRightTop) / 255.f;

	uint32 vertexIndex = p_StartIndex;

	p_VertexBuffer[vertexIndex++] = posLeftNDC;
	p_VertexBuffer[vertexIndex++] = posTopNDC;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.z;

	p_VertexBuffer[vertexIndex++] = posLeftNDC;
	p_VertexBuffer[vertexIndex++] = posBottomNDC;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.z;

	p_VertexBuffer[vertexIndex++] = posRightNDC;
	p_VertexBuffer[vertexIndex++] = posTopNDC;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.z;

	p_VertexBuffer[vertexIndex++] = posLeftNDC;
	p_VertexBuffer[vertexIndex++] = posBottomNDC;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.z;

	p_VertexBuffer[vertexIndex++] = posRightNDC;
	p_VertexBuffer[vertexIndex++] = posBottomNDC;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.z;

	p_VertexBuffer[vertexIndex++] = posRightNDC;
	p_VertexBuffer[vertexIndex++] = posTopNDC;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.z;

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTextVertices(K15_RenderBackEnd* p_RenderBackEnd,
	K15_RenderFontDesc* p_RenderFontDesc,
	float* p_VertexBuffer, uint32 p_VertexBufferOffsetIndex,
	uint32 p_PixelPosLeft, uint32 p_PixelPosTop, uint32 p_TextColor, const char* p_Text,
	uint32 p_TextLength)
{
	uint32 vertexIndex = p_VertexBufferOffsetIndex;

	uint32 leftPos = p_PixelPosLeft;
	uint32 topPos = p_PixelPosTop;
	
	float fontTextureWidth	 = p_RenderFontDesc->textureWidth;
	float fontTextureHeight  = p_RenderFontDesc->textureHeight;

	float maxGlyphPixelHeight = p_RenderFontDesc->ascent - p_RenderFontDesc->descent;

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

		float baseLine = topPos + (p_RenderFontDesc->ascent);

		float glyphLeftPixelPos	  = glyphPixelX;
		float glyphTopPixelPos	  = glyphPixelY;
		float glyphRightPixelPos  = glyphPixelX + glyphPixelWidth;
		float glyphBottomPixelPos = glyphPixelY + glyphPixelHeight;

		float glyphTexelLeft   = glyphLeftPixelPos / fontTextureWidth;
		float glyphTexelTop    = glyphTopPixelPos / fontTextureHeight;
		float glyphTexelRight  = glyphRightPixelPos / fontTextureWidth;
		float glyphTexelBottom = glyphBottomPixelPos / fontTextureHeight;

		float advanceX = advancePixelX;
		float advanceY = advancePixelY;

		float bottomOffset = bottomPixelOffset;

		float glyphHeight = glyphPixelHeight;
		float glyphWidth  = glyphPixelWidth;

		//new line - reset x and advance y
		if (character == '\n')
		{
			leftPos = p_PixelPosLeft;
			topPos -= advanceY;  
		}
		
		if (renderableCharacter)
		{			
			vertexIndex = K15_InternalPush2DScreenspacePixelColoredTextureRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
				leftPos, leftPos + glyphWidth, baseLine - glyphHeight - advanceY + bottomOffset, baseLine - advanceY + bottomOffset,
				glyphTexelLeft, glyphTexelRight, glyphTexelTop, glyphTexelBottom,
				p_TextColor, p_TextColor, p_TextColor, p_TextColor);
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
		K15_ATTRIBUTE_SEMANTIC_POSITION, K15_TYPE_FLOAT_VECTOR2,
		K15_ATTRIBUTE_SEMANTIC_TEXCOORD1, K15_TYPE_FLOAT_VECTOR2);

	uint32 numVertices = 6;
	uint32 sizeVerticesInBytes = vertexFormatDesc.stride * numVertices;

	float* vertexMemory = (float*)K15_AllocateFromMemoryAllocator(renderAllocator, sizeVerticesInBytes);

// 	K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, vertexMemory, 0, 
// 		p_PosLeft, p_PosRight, p_PosTop, p_PosBottom, 
// 		p_UCoordLeft, p_UCoordRight, p_VCoordTop, p_VCoordBottom);

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