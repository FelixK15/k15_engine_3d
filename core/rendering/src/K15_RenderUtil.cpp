#include "K15_Vector4.h"

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
intern inline K15_Vector4 K15_InternalCalculateCircularColor(float p_Angle, const K15_Vector4& p_LeftColor, 
	const K15_Vector4& p_RightColor, const K15_Vector4& p_TopColor, 
	const K15_Vector4& p_BottomColor, const K15_Vector4& p_CenterColor)
{
	float angle = fmod(p_Angle, K15_TWO_PI);

	float c = K15_HighPrecisionCos(p_Angle);
	float s = K15_HighPrecisionSin(p_Angle);
	float l = 0.f;

	K15_Vector4 horizontalColor = {};
	K15_Vector4 verticalColor = {};
	K15_Vector4 color = {};

	//1st quadrant?
	if (angle >= 0.f && angle <= K15_HALF_PI)
	{
		l = K15_CLAMP(angle / K15_HALF_PI, 1.f, 0.f);
		horizontalColor = K15_Lerp(p_CenterColor, p_RightColor, c);
		verticalColor = K15_Lerp(p_CenterColor, p_TopColor, s);
		color = K15_Lerp(horizontalColor, verticalColor, l);
	}
	//2nd quadrant?
	else if (angle >= K15_HALF_PI && angle <= K15_PI)
	{
		l = K15_CLAMP((angle - K15_HALF_PI) / K15_HALF_PI, 1.f, 0.f);

		c *= -1.f;
		horizontalColor = K15_Lerp(p_CenterColor, p_LeftColor, c);
		verticalColor = K15_Lerp(p_CenterColor, p_TopColor, s);
		color = K15_Lerp(verticalColor, horizontalColor, l);
	}
	//3rd quadrant?
	else if (angle >= K15_PI && angle <= (K15_PI + K15_HALF_PI))
	{
		l = K15_CLAMP((angle - K15_PI) / K15_HALF_PI, 1.f, 0.f);

		c *= -1.f;
		s *= -1.f;
		horizontalColor = K15_Lerp(p_CenterColor, p_LeftColor, c);
		verticalColor = K15_Lerp(p_CenterColor, p_BottomColor, s);
		color = K15_Lerp(horizontalColor, verticalColor, l);
	}
	//4th quadrant?
	else if (angle >= (K15_PI + K15_HALF_PI) && angle <= K15_TWO_PI)
	{
		l = K15_CLAMP((angle - (K15_PI + K15_HALF_PI)) / K15_HALF_PI, 1.f, 0.f);

		s *= -1.f;
		horizontalColor = K15_Lerp(p_CenterColor, p_RightColor, c);
		verticalColor = K15_Lerp(p_CenterColor, p_BottomColor, s);
		color = K15_Lerp(verticalColor, horizontalColor, l);
	}
	
	return color;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelCircularColoredSquareVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, int32 p_PixelPosX, int32 p_PixelPosY, uint32 p_PixelRadius,
	const K15_Vector4& p_CenterColor, const K15_Vector4& p_EdgeColor)
{
	K15_Vector4 centerColor = p_CenterColor;
	K15_Vector4 edgeColor = p_EdgeColor;

	centerColor = centerColor / 255.f;
	edgeColor = edgeColor / 255.f;

	uint32 vertexIndex = p_StartIndex;

	float theta = K15_TWO_PI / K15_GUI_2D_SQUARE_SAMPLES;
	float angle = theta;

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
		p_VertexBuffer[vertexIndex++] = p_PixelPosX;
		p_VertexBuffer[vertexIndex++] = p_PixelPosY;
		p_VertexBuffer[vertexIndex++] = centerColor.x;
		p_VertexBuffer[vertexIndex++] = centerColor.y;
		p_VertexBuffer[vertexIndex++] = centerColor.z;
		p_VertexBuffer[vertexIndex++] = centerColor.w;

		//edge vertex last
		p_VertexBuffer[vertexIndex++] = lastPixelPosX;
		p_VertexBuffer[vertexIndex++] = lastPixelPosY;
		p_VertexBuffer[vertexIndex++] = edgeColor.x;
		p_VertexBuffer[vertexIndex++] = edgeColor.y;
		p_VertexBuffer[vertexIndex++] = edgeColor.z;
		p_VertexBuffer[vertexIndex++] = edgeColor.w;

		//edge vertex current
		p_VertexBuffer[vertexIndex++] = pixelPosX;
		p_VertexBuffer[vertexIndex++] = pixelPosY;
		p_VertexBuffer[vertexIndex++] = edgeColor.x;
		p_VertexBuffer[vertexIndex++] = edgeColor.y;
		p_VertexBuffer[vertexIndex++] = edgeColor.z;
		p_VertexBuffer[vertexIndex++] = edgeColor.w;

		angle += theta;

		lastPixelPosX = pixelPosX;
		lastPixelPosY = pixelPosY;
	}

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelCircularColoredSquareVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, int32 p_PixelPosX, int32 p_PixelPosY, uint32 p_PixelRadius,
	uint32 p_CenterColor, uint32 p_EdgeColor)
{
	K15_Vector4 unpackedCenterColor = K15_UnpackVector4(p_CenterColor);
	K15_Vector4 unpackedEdgeColor = K15_UnpackVector4(p_EdgeColor);

	return K15_InternalPush2DScreenspacePixelCircularColoredSquareVertices(p_VertexBuffer, p_StartIndex,
		p_PixelPosX, p_PixelPosY, p_PixelRadius, unpackedCenterColor, unpackedEdgeColor);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredSphereVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, int32 p_PixelPosX, int32 p_PixelPosY, uint32 p_PixelRadius,
	const K15_Vector4& p_CenterColor,
	const K15_Vector4& p_LeftColor, const K15_Vector4& p_RightColor, 
	const K15_Vector4& p_TopColor, const K15_Vector4& p_BottomColor)
{
	uint32 vertexIndex = p_StartIndex;

	float theta = K15_TWO_PI / K15_GUI_2D_SQUARE_SAMPLES;
	float angle = theta;

	float lastPixelPosX = p_PixelPosX;
	float lastPixelPosY = p_PixelPosY;

	lastPixelPosX += p_PixelRadius;

	K15_Vector4 leftColor = p_LeftColor;
	K15_Vector4 rightColor = p_RightColor;
	K15_Vector4 topColor = p_TopColor;
	K15_Vector4 bottomColor = p_BottomColor;
	K15_Vector4 centerColor = p_CenterColor;

	leftColor = leftColor / 255.f;
	rightColor = rightColor / 255.f;
	topColor = topColor / 255.f;
	bottomColor = bottomColor / 255.f;
	centerColor = centerColor / 255.f;

	K15_Vector4 lastColor = K15_InternalCalculateCircularColor(0.f, leftColor, rightColor,
		topColor, bottomColor, centerColor);

	for (uint32 sampleIndex = 0;
	sampleIndex < K15_GUI_2D_SQUARE_SAMPLES;
		++sampleIndex)
	{
		float c = K15_HighPrecisionCos(angle);
		float s = K15_HighPrecisionSin(angle);
		float l = 0.f;

		float pixelPosX = (c * p_PixelRadius);
		float pixelPosY = -(s * p_PixelRadius);

		pixelPosX += p_PixelPosX;
		pixelPosY += p_PixelPosY;

		K15_Vector4 color = K15_InternalCalculateCircularColor(angle, leftColor, rightColor, 
			topColor, bottomColor, centerColor);

		//center vertex
		p_VertexBuffer[vertexIndex++] = p_PixelPosX;
		p_VertexBuffer[vertexIndex++] = p_PixelPosY;
		p_VertexBuffer[vertexIndex++] = centerColor.x;
		p_VertexBuffer[vertexIndex++] = centerColor.y;
		p_VertexBuffer[vertexIndex++] = centerColor.z;
		p_VertexBuffer[vertexIndex++] = centerColor.w;

		//edge vertex last
		p_VertexBuffer[vertexIndex++] = lastPixelPosX;
		p_VertexBuffer[vertexIndex++] = lastPixelPosY;
		p_VertexBuffer[vertexIndex++] = lastColor.x;
		p_VertexBuffer[vertexIndex++] = lastColor.y;
		p_VertexBuffer[vertexIndex++] = lastColor.z;
		p_VertexBuffer[vertexIndex++] = lastColor.w;

		//edge vertex current
		p_VertexBuffer[vertexIndex++] = pixelPosX;
		p_VertexBuffer[vertexIndex++] = pixelPosY;
		p_VertexBuffer[vertexIndex++] = color.x;
		p_VertexBuffer[vertexIndex++] = color.y;
		p_VertexBuffer[vertexIndex++] = color.z;
		p_VertexBuffer[vertexIndex++] = color.w;

		angle += theta;

		lastPixelPosX = pixelPosX;
		lastPixelPosY = pixelPosY;
		lastColor = color;
	}

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredSphereVertices(float* p_VertexBuffer, uint32 p_StartIndex,
	int32 p_PixelPosX, int32 p_PixelPosY, uint32 p_PixelRadius,
	uint32 p_CenterColor, uint32 p_LeftColor, uint32 p_RightColor, uint32 p_TopColor, uint32 p_BottomColor)
{
	K15_Vector4 unpackedLeftColor = K15_UnpackVector4(p_LeftColor);
	K15_Vector4 unpackedRightColor = K15_UnpackVector4(p_RightColor);
	K15_Vector4 unpackedTopColor = K15_UnpackVector4(p_TopColor);
	K15_Vector4 unpackedBottomColor = K15_UnpackVector4(p_BottomColor);
	K15_Vector4 unpackedCenterColor = K15_UnpackVector4(p_CenterColor);

	return K15_InternalPush2DScreenspacePixelColoredSphereVertices(p_VertexBuffer, p_StartIndex,
		p_PixelPosX, p_PixelPosY, p_PixelRadius, unpackedCenterColor,
		unpackedLeftColor, unpackedRightColor, 
		unpackedTopColor, unpackedBottomColor);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredRectVertices(float* p_VertexBuffer, uint32 p_StartIndex,
	int32 p_PixelPosLeft, int32 p_PixelPosRight, int32 p_PixelPosTop, int32 p_PixelPosBottom,
	uint32 p_ColorLeftTop, uint32 p_ColorRightTop, uint32 p_ColorLeftBottom, uint32 p_ColorRightBottom)
{
	K15_Vector4 unpackedLeftBottomColor = K15_UnpackVector4(p_ColorLeftBottom) / 255.f;
	K15_Vector4 unpackedLeftTopColor = K15_UnpackVector4(p_ColorLeftTop) / 255.f;
	K15_Vector4 unpackedRightBottomColor = K15_UnpackVector4(p_ColorRightBottom) / 255.f;
	K15_Vector4 unpackedRightTopColor = K15_UnpackVector4(p_ColorRightTop) / 255.f;

	uint32 vertexIndex = p_StartIndex;

	p_VertexBuffer[vertexIndex++] = p_PixelPosLeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosTop;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.z;
	p_VertexBuffer[vertexIndex++] = unpackedLeftTopColor.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosLeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosBottom;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.z;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosRight;
	p_VertexBuffer[vertexIndex++] = p_PixelPosTop;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.z;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosLeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosBottom;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.z;
	p_VertexBuffer[vertexIndex++] = unpackedLeftBottomColor.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosRight;
	p_VertexBuffer[vertexIndex++] = p_PixelPosBottom;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.z;
	p_VertexBuffer[vertexIndex++] = unpackedRightBottomColor.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosRight;
	p_VertexBuffer[vertexIndex++] = p_PixelPosTop;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.x;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.y;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.z;
	p_VertexBuffer[vertexIndex++] = unpackedRightTopColor.w;

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredRectVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, int32 p_PixelPosLeft, int32 p_PixelPosRight, int32 p_PixelPosTop, int32 p_PixelPosBottom,
	const K15_Vector4& p_ColorLeftTop, const K15_Vector4& p_ColorRightTop, 
	const K15_Vector4& p_ColorLeftBottom, const K15_Vector4& p_ColorRightBottom)
{
	uint32 packedLeftTopColor = K15_PackVector4(p_ColorLeftTop);
	uint32 packedLeftBottomColor = K15_PackVector4(p_ColorLeftBottom);
	uint32 packedRightTopColor = K15_PackVector4(p_ColorRightTop);
	uint32 packedRightBottomColor = K15_PackVector4(p_ColorRightBottom);

	return K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, p_StartIndex,
		p_PixelPosLeft, p_PixelPosRight, p_PixelPosTop, p_PixelPosBottom,
		packedLeftTopColor, packedRightTopColor, packedLeftBottomColor, packedRightBottomColor);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredRoundRectVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, int32 p_PixelPosLeft, int32 p_PixelPosRight, int32 p_PixelPosTop, int32 p_PixelPosBottom,
	const K15_Vector4& p_ColorLeftTop, const K15_Vector4& p_ColorRightTop, 
	const K15_Vector4& p_ColorLeftBottom, const K15_Vector4& p_ColorRightBottom,
	uint32 p_RoundCornerFlags, float p_EdgeRoundFactor)
{
	float roundFactor = K15_CLAMP(p_EdgeRoundFactor, 1.f, 0.f);
	uint32 cornerPixelRadius = roundFactor * ((p_PixelPosBottom - p_PixelPosTop) / 2);
	uint32 vertexIndex = p_StartIndex;

	if (roundFactor == 0.f ||
		p_RoundCornerFlags == 0)
	{
		return K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, p_StartIndex,
			p_PixelPosLeft, p_PixelPosRight, p_PixelPosTop, p_PixelPosBottom,
			p_ColorLeftTop, p_ColorRightTop, p_ColorLeftBottom, p_ColorRightBottom);
	}

	int32 pixelPosLeft = p_PixelPosLeft;
	int32 pixelPosRight = p_PixelPosRight;
	int32 pixelPosTop = p_PixelPosTop;
	int32 pixelPosBottom = p_PixelPosBottom;

	float width = (float)(pixelPosRight - pixelPosLeft);
	float height = (float)(pixelPosBottom - pixelPosTop);
	
	float widthLerpFactor = (float)cornerPixelRadius / width;
	float heightLerpFactor = (float)cornerPixelRadius / height;

	K15_Vector4 black = K15_CreateVector(0.f, 0.f, 0.f, 1.f);

	K15_Vector4 colorLeftTop = p_ColorLeftTop;
	K15_Vector4 colorRightTop = p_ColorRightTop;
	K15_Vector4 colorLeftBottom = p_ColorLeftBottom;
	K15_Vector4 colorRightBottom = p_ColorRightBottom;

	K15_Vector4 lerpColorLeftTopLeftBottom = K15_Lerp(colorLeftTop, colorLeftBottom, heightLerpFactor);
	K15_Vector4 lerpColorLeftBottomLeftTop = K15_Lerp(colorLeftBottom, colorLeftTop, heightLerpFactor);
	K15_Vector4 lerpColorRightTopRightBottom = K15_Lerp(colorRightTop, colorRightBottom, heightLerpFactor);
	K15_Vector4 lerpColorRightBottomRightTop = K15_Lerp(colorRightBottom, colorRightTop, heightLerpFactor);

	K15_Vector4 lerpColorLeftTopRightTop= K15_Lerp(colorLeftTop, colorRightTop, widthLerpFactor);
	K15_Vector4 lerpColorRightTopLeftTop = K15_Lerp(colorRightTop, colorLeftTop, widthLerpFactor);
	K15_Vector4 lerpColorLeftBottomRightBottom = K15_Lerp(colorLeftBottom, colorRightBottom, widthLerpFactor);
	K15_Vector4 lerpColorRightBottomLeftBottom = K15_Lerp(colorRightBottom, colorLeftBottom, widthLerpFactor);

	K15_Vector4 lerpColorLeftTopRightTopLeftTopLeftBottom = K15_Lerp(lerpColorLeftTopRightTop,
		lerpColorLeftTopLeftBottom,
		0.5f);

	K15_Vector4 lerpColorRightTopLeftTopRightTopRightBottom = K15_Lerp(lerpColorRightTopLeftTop,
		lerpColorRightTopRightBottom,
		0.5f);

	K15_Vector4 lerpColorLeftBottomLeftTopLeftBottomRightBottom = K15_Lerp(lerpColorLeftBottomLeftTop ,
		lerpColorLeftBottomRightBottom,
		0.5f);

	K15_Vector4 lerpColorRightBottomRightTopRightBottomLeftBottom = K15_Lerp(lerpColorRightBottomRightTop,
		lerpColorRightBottomLeftBottom,
		0.5f);

	pixelPosRight -= cornerPixelRadius;
	pixelPosTop += cornerPixelRadius;
	pixelPosLeft += cornerPixelRadius;
	pixelPosBottom -= cornerPixelRadius;

	if (p_RoundCornerFlags & K15_LEFT_TOP_CORNER)
	{
		colorLeftTop = lerpColorLeftTopRightTopLeftTopLeftBottom;

		vertexIndex = K15_InternalPush2DScreenspacePixelColoredSphereVertices(p_VertexBuffer, vertexIndex,
			pixelPosLeft, pixelPosTop, cornerPixelRadius,
			colorLeftTop,
			lerpColorLeftTopLeftBottom,
			black,
			lerpColorLeftTopRightTop,
			black);
	}

	if (p_RoundCornerFlags & K15_RIGHT_TOP_CORNER)
	{
		colorRightTop = lerpColorRightTopLeftTopRightTopRightBottom;

		vertexIndex = K15_InternalPush2DScreenspacePixelColoredSphereVertices(p_VertexBuffer, vertexIndex,
			pixelPosRight, pixelPosTop, cornerPixelRadius,
			colorRightTop,
			black,
			lerpColorRightTopRightBottom,
			lerpColorRightTopLeftTop,
			black);
	}
 
	if (p_RoundCornerFlags & K15_LEFT_BOTTOM_CORNER)
	{
		colorLeftBottom = lerpColorLeftBottomLeftTopLeftBottomRightBottom;

		vertexIndex = K15_InternalPush2DScreenspacePixelColoredSphereVertices(p_VertexBuffer, vertexIndex,
			pixelPosLeft, pixelPosBottom, cornerPixelRadius,
			colorLeftBottom,
			lerpColorLeftBottomLeftTop,
			black,
			black ,
			lerpColorLeftBottomRightBottom);
	}

	if (p_RoundCornerFlags & K15_RIGHT_BOTTOM_CORNER)
	{
		colorRightBottom = lerpColorRightBottomRightTopRightBottomLeftBottom;

		vertexIndex = K15_InternalPush2DScreenspacePixelColoredSphereVertices(p_VertexBuffer, vertexIndex,
			pixelPosRight, pixelPosBottom, cornerPixelRadius,
			colorRightBottom,
			black ,
			lerpColorRightBottomRightTop,
			black,
			lerpColorRightBottomLeftBottom);
	}

 	//left border
	{
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
			p_PixelPosLeft, pixelPosLeft, pixelPosTop, pixelPosBottom,
			lerpColorLeftTopLeftBottom, lerpColorLeftTopRightTopLeftTopLeftBottom,
			lerpColorLeftBottomLeftTop, lerpColorLeftBottomLeftTopLeftBottomRightBottom);
	}
 
	//right border
	{
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
			pixelPosRight, p_PixelPosRight, pixelPosTop, pixelPosBottom,
			lerpColorRightTopLeftTopRightTopRightBottom, lerpColorRightTopRightBottom,
			lerpColorRightBottomRightTopRightBottomLeftBottom, lerpColorRightBottomRightTop);
	}
 
 	//top border
	{
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
			pixelPosLeft, pixelPosRight, p_PixelPosTop, pixelPosTop,
			lerpColorLeftTopRightTop, lerpColorRightTopLeftTop,
			lerpColorLeftTopRightTopLeftTopLeftBottom, lerpColorRightTopLeftTopRightTopRightBottom);
	}

	//bottom border
	{
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
			pixelPosLeft, pixelPosRight, pixelPosBottom, p_PixelPosBottom,
			lerpColorLeftBottomLeftTopLeftBottomRightBottom, lerpColorRightBottomRightTopRightBottomLeftBottom,
			lerpColorLeftBottomRightBottom, lerpColorRightBottomLeftBottom);
	}

	if ((p_RoundCornerFlags & K15_RIGHT_TOP_CORNER) == 0)
	{
		colorRightTop = lerpColorRightTopLeftTopRightTopRightBottom;
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
			p_PixelPosRight - cornerPixelRadius, p_PixelPosRight,
			p_PixelPosTop, p_PixelPosTop + cornerPixelRadius,
			lerpColorRightTopLeftTop, p_ColorRightTop,
			lerpColorRightTopLeftTopRightTopRightBottom, lerpColorRightTopRightBottom);
	}

	if ((p_RoundCornerFlags & K15_LEFT_TOP_CORNER) == 0)
	{
		colorLeftTop = lerpColorLeftTopRightTopLeftTopLeftBottom;
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
			p_PixelPosLeft, p_PixelPosLeft + cornerPixelRadius,
			p_PixelPosTop, p_PixelPosTop + cornerPixelRadius,
			p_ColorLeftTop, lerpColorLeftTopRightTop,
			lerpColorLeftTopLeftBottom, lerpColorLeftTopRightTopLeftTopLeftBottom);
	}
 
	if ((p_RoundCornerFlags & K15_LEFT_BOTTOM_CORNER) == 0)
	{
		colorLeftBottom = lerpColorLeftBottomLeftTopLeftBottomRightBottom;
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
			p_PixelPosLeft, p_PixelPosLeft + cornerPixelRadius,
			p_PixelPosBottom - cornerPixelRadius, p_PixelPosBottom,
			lerpColorLeftBottomLeftTop, lerpColorLeftBottomLeftTopLeftBottomRightBottom,
			p_ColorLeftBottom, lerpColorLeftBottomRightBottom);
	}

	if ((p_RoundCornerFlags & K15_RIGHT_BOTTOM_CORNER) == 0)
	{
		colorRightBottom = lerpColorRightBottomRightTopRightBottomLeftBottom;
		vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
			p_PixelPosRight - cornerPixelRadius, p_PixelPosRight,
			p_PixelPosBottom - cornerPixelRadius, p_PixelPosBottom,
			lerpColorRightBottomRightTopRightBottomLeftBottom, lerpColorRightBottomRightTop,
			lerpColorRightBottomLeftBottom, p_ColorRightBottom);
	}

	//mid non rounded part
	vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_VertexBuffer, vertexIndex,
		pixelPosLeft, pixelPosRight, pixelPosTop, pixelPosBottom,
		colorLeftTop, colorRightTop,
		colorLeftBottom, colorRightBottom);

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredRoundRectVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, int32 p_PixelPosLeft, int32 p_PixelPosRight, int32 p_PixelPosTop, int32 p_PixelPosBottom,
	uint32 p_ColorLeftTop, uint32 p_ColorRightTop, uint32 p_ColorLeftBottom, uint32 p_ColorRightBottom,
	uint32 p_RoundCornerFlags, float p_EdgeRoundFactor)
{
	K15_Vector4 unpackedColorLeftTop = K15_UnpackVector4(p_ColorLeftTop);
	K15_Vector4 unpackedColorRightTop = K15_UnpackVector4(p_ColorRightTop);
	K15_Vector4 unpackedColorLeftBottom = K15_UnpackVector4(p_ColorLeftBottom);
	K15_Vector4 unpackedColorRightBottom = K15_UnpackVector4(p_ColorRightBottom);

	return K15_InternalPush2DScreenspacePixelColoredRoundRectVertices(p_VertexBuffer, p_StartIndex,
		p_PixelPosLeft, p_PixelPosRight, p_PixelPosTop, p_PixelPosBottom,
		unpackedColorLeftTop, unpackedColorRightTop,
		unpackedColorLeftBottom, unpackedColorRightBottom,
		p_RoundCornerFlags, p_EdgeRoundFactor);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredLineVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, int32 p_PixelStartPosLeft, int32 p_PixelStartPosTop, 
	int32 p_PixelEndPosLeft, int32 p_PixelEndPosTop, uint32 p_LinePixelWidth, 
	const K15_Vector4& p_StartColor, const K15_Vector4& p_EndColor)
{
	uint32 vertexIndex = p_StartIndex;

	float halfLinePixelWidth = (float)p_LinePixelWidth / 2.f;

	K15_Vector2 pixelStartPos = K15_CreateVector(p_PixelStartPosLeft, p_PixelStartPosTop);
	K15_Vector2 pixelEndPos = K15_CreateVector(p_PixelEndPosLeft, p_PixelEndPosTop);

	K15_Vector2 pixelDirection = pixelEndPos - pixelStartPos;
	K15_NormalizeVector(pixelDirection);

	K15_Vector2 perpendicularPixelDirection = K15_CreateVector(pixelDirection.y * -1.f, pixelDirection.x) * halfLinePixelWidth;

	K15_Vector2 pixelUpperStartPos = pixelStartPos + perpendicularPixelDirection;
	K15_Vector2 pixelUpperEndPos = pixelEndPos + perpendicularPixelDirection;
	K15_Vector2 pixelLowerStartPos = pixelStartPos - perpendicularPixelDirection;
	K15_Vector2 pixelLowerEndPos = pixelEndPos - perpendicularPixelDirection;

	p_VertexBuffer[vertexIndex++] = pixelUpperStartPos.x;
	p_VertexBuffer[vertexIndex++] = pixelUpperStartPos.y;
	p_VertexBuffer[vertexIndex++] = p_StartColor.x;
	p_VertexBuffer[vertexIndex++] = p_StartColor.y;
	p_VertexBuffer[vertexIndex++] = p_StartColor.z;
	p_VertexBuffer[vertexIndex++] = p_StartColor.w;

	p_VertexBuffer[vertexIndex++] = pixelLowerStartPos.x;
	p_VertexBuffer[vertexIndex++] = pixelLowerStartPos.y;
	p_VertexBuffer[vertexIndex++] = p_StartColor.x;
	p_VertexBuffer[vertexIndex++] = p_StartColor.y;
	p_VertexBuffer[vertexIndex++] = p_StartColor.z;
	p_VertexBuffer[vertexIndex++] = p_StartColor.w;

	p_VertexBuffer[vertexIndex++] = pixelUpperEndPos.x;
	p_VertexBuffer[vertexIndex++] = pixelUpperEndPos.y;
	p_VertexBuffer[vertexIndex++] = p_EndColor.x;
	p_VertexBuffer[vertexIndex++] = p_EndColor.y;
	p_VertexBuffer[vertexIndex++] = p_EndColor.z;
	p_VertexBuffer[vertexIndex++] = p_EndColor.w;

	p_VertexBuffer[vertexIndex++] = pixelUpperEndPos.x;
	p_VertexBuffer[vertexIndex++] = pixelUpperEndPos.y;
	p_VertexBuffer[vertexIndex++] = p_EndColor.x;
	p_VertexBuffer[vertexIndex++] = p_EndColor.y;
	p_VertexBuffer[vertexIndex++] = p_EndColor.z;
	p_VertexBuffer[vertexIndex++] = p_EndColor.w;

	p_VertexBuffer[vertexIndex++] = pixelLowerEndPos.x;
	p_VertexBuffer[vertexIndex++] = pixelLowerEndPos.y;
	p_VertexBuffer[vertexIndex++] = p_EndColor.x;
	p_VertexBuffer[vertexIndex++] = p_EndColor.y;
	p_VertexBuffer[vertexIndex++] = p_EndColor.z;
	p_VertexBuffer[vertexIndex++] = p_EndColor.w;

	p_VertexBuffer[vertexIndex++] = pixelLowerStartPos.x;
	p_VertexBuffer[vertexIndex++] = pixelLowerStartPos.y;
	p_VertexBuffer[vertexIndex++] = p_StartColor.x;
	p_VertexBuffer[vertexIndex++] = p_StartColor.y;
	p_VertexBuffer[vertexIndex++] = p_StartColor.z;
	p_VertexBuffer[vertexIndex++] = p_StartColor.w;

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredLineVertices(float* p_VertexBuffer,
	uint32 p_StartIndex, int32 p_PixelStartPosLeft, int32 p_PixelStartPosTop, 
	int32 p_PixelEndPosLeft, int32 p_PixelEndPosTop, uint32 p_LinePixelWidth, 
	uint32 p_StartColor, uint32 p_EndColor)
{
	K15_Vector4 startColorUnpacked = K15_UnpackVector4(p_StartColor);
	K15_Vector4 endColorUnpacked = K15_UnpackVector4(p_EndColor);

	startColorUnpacked = startColorUnpacked / 255.f;
	endColorUnpacked = endColorUnpacked / 255.f;

	return K15_InternalPush2DScreenspacePixelColoredLineVertices(p_VertexBuffer, p_StartIndex, 
		p_PixelStartPosLeft, p_PixelStartPosTop, p_PixelEndPosLeft, p_PixelEndPosTop, 
		p_LinePixelWidth, startColorUnpacked, endColorUnpacked);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTextureRectVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, int32 p_PixelPosLeft, int32 p_PixelPosRight, int32 p_PixelPosTop, int32 p_PixelPosBottom,
	float p_UCoordLeft, float p_UCoordRight, float p_VCoordTop, float p_VCoordBottom,
	const K15_Vector4& p_ColorLeftTop, const K15_Vector4& p_ColorRightTop,
	const K15_Vector4& p_ColorLeftBottom, const K15_Vector4& p_ColorRightBottom)
{
	uint32 vertexIndex = p_StartIndex;

	p_VertexBuffer[vertexIndex++] = p_PixelPosLeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftTop.x;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftTop.y;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftTop.z;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftTop.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosLeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftBottom.x;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftBottom.y;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftBottom.z;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftBottom.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosRight;
	p_VertexBuffer[vertexIndex++] = p_PixelPosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	p_VertexBuffer[vertexIndex++] = p_ColorRightTop.x;
	p_VertexBuffer[vertexIndex++] = p_ColorRightTop.y;
	p_VertexBuffer[vertexIndex++] = p_ColorRightTop.z;
	p_VertexBuffer[vertexIndex++] = p_ColorRightTop.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosLeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftBottom.x;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftBottom.y;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftBottom.z;
	p_VertexBuffer[vertexIndex++] = p_ColorLeftBottom.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosRight;
	p_VertexBuffer[vertexIndex++] = p_PixelPosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	p_VertexBuffer[vertexIndex++] = p_ColorRightBottom.x;
	p_VertexBuffer[vertexIndex++] = p_ColorRightBottom.y;
	p_VertexBuffer[vertexIndex++] = p_ColorRightBottom.z;
	p_VertexBuffer[vertexIndex++] = p_ColorRightBottom.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosRight;
	p_VertexBuffer[vertexIndex++] = p_PixelPosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	p_VertexBuffer[vertexIndex++] = p_ColorRightTop.x;
	p_VertexBuffer[vertexIndex++] = p_ColorRightTop.y;
	p_VertexBuffer[vertexIndex++] = p_ColorRightTop.z;
	p_VertexBuffer[vertexIndex++] = p_ColorRightTop.w;

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTriangleVertices(float* p_VertexBuffer,
	uint32 p_StartIndex, uint32 p_PixelPosALeft, uint32 p_PixelPosATop,
	uint32 p_PixelPosBLeft, uint32 p_PixelPosBTop,
	uint32 p_PixelPosCLeft, uint32 p_PixelPosCTop,
	const K15_Vector4& p_TriangleColor)
{
	uint32 vertexIndex = p_StartIndex;
	
	p_VertexBuffer[vertexIndex++] = p_PixelPosALeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosATop;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.x;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.y;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.z;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosBLeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosBTop;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.x;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.y;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.z;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.w;

	p_VertexBuffer[vertexIndex++] = p_PixelPosCLeft;
	p_VertexBuffer[vertexIndex++] = p_PixelPosCTop;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.x;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.y;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.z;
	p_VertexBuffer[vertexIndex++] = p_TriangleColor.w;

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTriangleVertices(float* p_VertexBuffer,
	uint32 p_StartIndex, uint32 p_PixelPosALeft, uint32 p_PixelPosATop,
	uint32 p_PixelPosBLeft, uint32 p_PixelPosBTop,
	uint32 p_PixelPosCLeft, uint32 p_PixelPosCTop,
	uint32 p_TriangleColor)
{
	K15_Vector4 unpackedColor = K15_UnpackVector4(p_TriangleColor) / 255.f;
	
	return K15_InternalPush2DScreenspacePixelColoredTriangleVertices(p_VertexBuffer, p_StartIndex,
		p_PixelPosALeft, p_PixelPosATop, p_PixelPosBLeft, p_PixelPosBTop, p_PixelPosCLeft, p_PixelPosCTop,
		unpackedColor);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTextureRectVertices(float* p_VertexBuffer, 
	uint32 p_StartIndex, uint32 p_PixelPosLeft, uint32 p_PixelPosRight, 
	uint32 p_PixelPosTop, uint32 p_PixelPosBottom, float p_UCoordLeft, float p_UCoordRight, 
	float p_VCoordTop, float p_VCoordBottom, uint32 p_ColorLeftTop, uint32 p_ColorRightTop, 
	uint32 p_ColorLeftBottom, uint32 p_ColorRightBottom)
{
	K15_Vector4 unpackedLeftBottomColor = K15_UnpackVector4(p_ColorLeftBottom) / 255.f;
	K15_Vector4 unpackedLeftTopColor = K15_UnpackVector4(p_ColorLeftTop) / 255.f;
	K15_Vector4 unpackedRightBottomColor = K15_UnpackVector4(p_ColorRightBottom) / 255.f;
	K15_Vector4 unpackedRightTopColor = K15_UnpackVector4(p_ColorRightTop) / 255.f;

	return K15_InternalPush2DScreenspacePixelColoredTextureRectVertices(p_VertexBuffer, p_StartIndex,
		p_PixelPosLeft, p_PixelPosRight, p_PixelPosTop, p_PixelPosBottom,
		p_UCoordLeft, p_UCoordRight, p_VCoordTop, p_VCoordBottom,
		unpackedLeftTopColor, unpackedRightTopColor,
		unpackedLeftBottomColor, unpackedRightBottomColor);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTextVertices(K15_RenderFontDesc* p_RenderFontDesc,
	float* p_VertexBuffer, uint32 p_VertexBufferOffsetIndex, int32 p_PixelPosLeft, int32 p_PixelPosRight,
	int32 p_PixelPosTop, int32 p_PixelPosBottom, const K15_Vector4& p_TextColor, const char* p_Text,
	uint32 p_TextLength)
{
	uint32 vertexIndex = p_VertexBufferOffsetIndex;

	int32 leftPos = p_PixelPosLeft;
	int32 topPos = p_PixelPosTop;

	float fontTextureWidth = p_RenderFontDesc->textureWidth;
	float fontTextureHeight = p_RenderFontDesc->textureHeight;

	float maxGlyphPixelHeight = p_RenderFontDesc->ascent - p_RenderFontDesc->descent;

	for (uint32 textIndex = 0;
		textIndex < p_TextLength;
		++textIndex)
	{
		const char character = p_Text[textIndex];

		float glyphPixelX = 0.f;
		float glyphPixelY = 0.f;
		float glyphPixelWidth = 0.f;
		float glyphPixelHeight = 0.f;
		float advancePixelX = 0.f;
		float advancePixelY = 0.f;
		float bottomPixelOffset = 0.f;
		bool8 renderableCharacter = K15_FALSE;

		K15_GetFontCharacterInfo(p_RenderFontDesc, p_Text, p_TextLength, textIndex,
			&glyphPixelX, &glyphPixelY, &glyphPixelWidth, &glyphPixelHeight,
			&advancePixelX, &advancePixelY, &bottomPixelOffset,
			&renderableCharacter);

		float baseLine = topPos + (p_RenderFontDesc->ascent);

		float glyphLeftPixelPos = glyphPixelX;
		float glyphTopPixelPos = glyphPixelY;
		float glyphRightPixelPos = glyphPixelX + glyphPixelWidth;
		float glyphBottomPixelPos = glyphPixelY + glyphPixelHeight;

		float glyphTexelLeft = glyphLeftPixelPos / fontTextureWidth;
		float glyphTexelTop = glyphTopPixelPos / fontTextureWidth;
		float glyphTexelWidth = (glyphRightPixelPos - glyphLeftPixelPos) / fontTextureWidth;
		float glyphTexelHeight = (glyphBottomPixelPos - glyphTopPixelPos) / fontTextureHeight;

		float advanceX = advancePixelX;
		float advanceY = advancePixelY;

		float bottomOffset = bottomPixelOffset;

		float glyphHeight = glyphPixelHeight;
		float glyphWidth = glyphPixelWidth;

		float glyphTopPos = baseLine - glyphHeight - advanceY + bottomOffset;
		//float glyphBottomPos = baseLine - advanceY + bottomOffset;

		//clip if necessary
		if (glyphWidth + leftPos > p_PixelPosRight)
		{
			float clippedWidth = (leftPos + glyphWidth) - p_PixelPosRight;
			float clipPercent = K15_SafeDivide(clippedWidth, glyphWidth);
			glyphTexelWidth *= (1.f - clipPercent);
			glyphWidth -= clippedWidth;
		}

		if (glyphHeight + topPos > p_PixelPosBottom)
		{
			float clippedHeight = (topPos + glyphHeight) - p_PixelPosBottom;
			float clipPercent = K15_SafeDivide(clippedHeight, glyphHeight);
			glyphTexelHeight *= (1.f - clipPercent);
			glyphHeight -= clippedHeight;
		}

		renderableCharacter = glyphHeight > 0.f && glyphWidth > 0.f;

		//new line - reset x and advance y
		if (character == '\n')
		{
			leftPos = p_PixelPosLeft;
			topPos -= advanceY;
		}

		if (renderableCharacter)
		{
			vertexIndex = K15_InternalPush2DScreenspacePixelColoredTextureRectVertices(p_VertexBuffer, vertexIndex,
				leftPos, leftPos + glyphWidth, glyphTopPos,
				glyphTopPos + glyphHeight,
				glyphTexelLeft, glyphTexelLeft + glyphTexelWidth, 
				glyphTexelTop, glyphTexelTop + glyphTexelHeight,
				p_TextColor, p_TextColor, p_TextColor, p_TextColor);
		}

		leftPos += advanceX;
	}

	return vertexIndex;
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTextVertices(K15_RenderFontDesc* p_RenderFontDesc,
	float* p_VertexBuffer, uint32 p_VertexBufferOffsetIndex, int32 p_PixelPosLeft, int32 p_PixelPosRight,
	int32 p_PixelPosTop, int32 p_PixelPosBottom, uint32 p_TextColor, const char* p_Text, uint32 p_TextLength)
{
	K15_Vector4 textColor = K15_UnpackVector4(p_TextColor);

	return K15_InternalPush2DScreenspacePixelColoredTextVertices(p_RenderFontDesc, p_VertexBuffer,
		p_VertexBufferOffsetIndex, p_PixelPosLeft, p_PixelPosRight, p_PixelPosTop, p_PixelPosBottom,
		textColor, p_Text, p_TextLength);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTextVertices(K15_RenderFontDesc* p_RenderFontDesc,
	float* p_VertexBuffer, uint32 p_VertexBufferOffsetIndex, int32 p_PixelPosLeft, int32 p_PixelPosTop, 
	const K15_Vector4& p_TextColor, const char* p_Text, uint32 p_TextLength)
{
	float textWidth = 0.f;
	float textHeight = 0.f;

	K15_GetTextSizeInPixels(p_RenderFontDesc, &textWidth, &textHeight, p_Text, p_TextLength);

	return K15_InternalPush2DScreenspacePixelColoredTextVertices(p_RenderFontDesc, p_VertexBuffer,
		p_VertexBufferOffsetIndex, p_PixelPosLeft, p_PixelPosTop, p_PixelPosLeft + textWidth,
		p_PixelPosTop + textHeight, p_TextColor, p_Text, p_TextLength);
}
/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspacePixelColoredTextVertices(K15_RenderFontDesc* p_RenderFontDesc,
	float* p_VertexBuffer, uint32 p_VertexBufferOffsetIndex,
	int32 p_PixelPosLeft, int32 p_PixelPosTop, uint32 p_TextColor, const char* p_Text,
	uint32 p_TextLength)
{
	K15_Vector4 unpackedTextColor = K15_UnpackVector4(p_TextColor);

	return K15_InternalPush2DScreenspacePixelColoredTextVertices(p_RenderFontDesc, p_VertexBuffer,
		p_VertexBufferOffsetIndex, p_PixelPosLeft, p_PixelPosTop, unpackedTextColor, p_Text, p_TextLength);
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