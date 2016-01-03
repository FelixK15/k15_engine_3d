/*********************************************************************************/
intern void K15_InternalPushGUIButtonVertices(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext,
												K15_GUIElementHeader* p_GUIElement, K15_GUIButton* p_GUIButton,
												float* p_VertexBuffer, uint32* p_VertexBufferIndexOffset,
												float* p_TextVertexBuffer, uint32* p_TextVertexBufferIndexOffset)
{
	uint32 vertexIndex = *p_VertexBufferIndexOffset;
	uint32 textVertexIndex = *p_TextVertexBufferIndexOffset;

	uint32 viewportWidth = p_RenderBackEnd->viewportWidth;
	uint32 viewportHeight = p_RenderBackEnd->viewportHeight;

	K15_GUIContextStyle* guiStyle = &p_GUIContext->style;
	K15_GUIButtonState guiButtonState = p_GUIButton->state;
	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];

	K15_RenderFontDesc* guiStyleFont = guiStyle->styleFont;

	const char* text = (const char*)(guiMemory + p_GUIButton->textOffsetInBytes);
	uint32 textLength = p_GUIButton->textLength;

	float guiTextureWidth = 256.f;
	float guiTextureHeight = 128.f;

	float buttonStylePixelPosX	 = guiStyle->guiButtonStyle.stateStyle[guiButtonState].posPixelX;
	float buttonStylePixelPosY	 = guiStyle->guiButtonStyle.stateStyle[guiButtonState].posPixelY;
	float buttonStylePixelWidth	 = guiStyle->guiButtonStyle.stateStyle[guiButtonState].pixelWidth;
	float buttonStylePixelHeight = guiStyle->guiButtonStyle.stateStyle[guiButtonState].pixelHeight;

	float pixelPadding = guiStyle->guiButtonStyle.padding;

	float elementPosPixelX	 = (float)p_GUIElement->posPixelX;
	float elementPosPixelY	 = (float)p_GUIElement->posPixelY;
	float elementPixelWidth  = (float)p_GUIElement->pixelWidth + pixelPadding*2;
	float elementPixelHeight = (float)p_GUIElement->pixelHeight + pixelPadding*2;

	float viewWidth	 = (float)viewportWidth;
	float viewHeight = (float)viewportHeight;

	float paddingHorizontal = pixelPadding / viewWidth;
	float paddingVertical = pixelPadding / viewHeight;

	float elementMarginLeft	  = (float)guiStyle->guiButtonStyle.stateStyle[guiButtonState].marginLeft;
	float elementMarginRight  = (float)guiStyle->guiButtonStyle.stateStyle[guiButtonState].marginRight;
	float elementMarginTop	  = (float)guiStyle->guiButtonStyle.stateStyle[guiButtonState].marginTop;
	float elementMarginBottom = (float)guiStyle->guiButtonStyle.stateStyle[guiButtonState].marginBottom;

	float elementInnerLeft = (elementPosPixelX + elementMarginLeft) / viewWidth;
	float elementInnerTop  = (elementPosPixelY + elementMarginTop) / viewHeight;

	float elementLeftNDC	= K15_CONVERT_TO_NDC_X(elementPosPixelX / viewWidth);
	float elementTopNDC		= K15_CONVERT_TO_NDC_Y(elementPosPixelY / viewHeight);
	float elementRightNDC	= K15_CONVERT_TO_NDC_X((elementPosPixelX + elementPixelWidth) / viewWidth);
	float elementBottomNDC  = K15_CONVERT_TO_NDC_Y((elementPosPixelY + elementPixelHeight) / viewHeight);

	float elementInnerLeftNDC	 = K15_CONVERT_TO_NDC_X(elementInnerLeft);
	float elementInnerRightNDC	 = K15_CONVERT_TO_NDC_X((elementPosPixelX + elementPixelWidth - elementMarginRight) / viewWidth);
	float elementInnerTopNDC	 = K15_CONVERT_TO_NDC_Y(elementInnerTop);
	float elementInnerBottomNDC  = K15_CONVERT_TO_NDC_Y((elementPosPixelY + elementPixelHeight - elementMarginBottom) / viewHeight);

	float uvBorderLeft	 = buttonStylePixelPosX / guiTextureWidth;
	float uvBorderTop	 = buttonStylePixelPosY/ guiTextureHeight;
	float uvBorderRight	 = (buttonStylePixelPosX + buttonStylePixelWidth) / guiTextureWidth;
	float uvBorderBottom = (buttonStylePixelPosY + buttonStylePixelHeight) / guiTextureHeight;

	float uvInnerLeft = (buttonStylePixelPosX + elementMarginLeft) / guiTextureWidth;
	float uvInnerTop  = (buttonStylePixelPosY + elementMarginTop) / guiTextureHeight;
	float uvInnerRight = (buttonStylePixelPosX + buttonStylePixelWidth - elementMarginRight) / guiTextureWidth;
	float uvInnerBottom = (buttonStylePixelPosY + buttonStylePixelHeight - elementMarginBottom) / guiTextureHeight;

	//			  Top Border
	//	  B *-*-------------*-*    B
	//	L o	| *--------------* | R o
	//	e r	| |			     | | i r
	//	f d	| |  Content	 | | g d
	//	t e	| |			     | | h e
	//	  r	| *--------------* | t r
	//		*-*--------------*-* 
	//		   Bottom Border

	//left border  
	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementLeftNDC, elementInnerLeftNDC, elementTopNDC, elementInnerTopNDC,
		uvBorderLeft, uvInnerLeft, uvBorderTop, uvInnerTop);

	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementLeftNDC, elementInnerLeftNDC, elementInnerBottomNDC, elementBottomNDC,
		uvBorderLeft, uvInnerLeft, uvInnerBottom, uvBorderBottom);

	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementLeftNDC, elementInnerLeftNDC, elementInnerTopNDC, elementInnerBottomNDC,
		uvBorderLeft, uvInnerLeft, uvInnerTop, uvInnerBottom);

	//right border
	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerRightNDC, elementRightNDC, elementInnerTopNDC, elementInnerBottomNDC,
		uvInnerRight, uvBorderRight, uvInnerTop, uvInnerBottom);

	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerRightNDC, elementRightNDC, elementInnerBottomNDC, elementBottomNDC,
		uvInnerRight, uvBorderRight, uvInnerBottom, uvBorderBottom);

	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerRightNDC, elementRightNDC, elementTopNDC, elementInnerTopNDC,
		uvInnerRight, uvBorderRight, uvBorderTop, uvInnerTop);


	//top border
	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerLeftNDC, elementInnerRightNDC, elementTopNDC, elementInnerTopNDC,
		uvInnerLeft, uvInnerRight, uvBorderTop, uvInnerTop);
 
 	//bottom border
 	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerLeftNDC, elementInnerRightNDC, elementInnerBottomNDC, elementBottomNDC,
		uvInnerLeft, uvInnerRight, uvInnerBottom, uvBorderBottom);

	//background
	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
		elementInnerLeftNDC, elementInnerRightNDC, elementInnerTopNDC, elementBottomNDC,
		uvInnerLeft, uvInnerRight, uvInnerTop, uvInnerBottom);

	//text
	/*********************************************************************************/
	textVertexIndex = K15_InternalPush2DTextVertices(p_RenderBackEnd, guiStyleFont, p_TextVertexBuffer,
		textVertexIndex, elementInnerLeft + paddingHorizontal, elementInnerTop + paddingVertical, text, textLength);

	*p_VertexBufferIndexOffset = vertexIndex;
	*p_TextVertexBufferIndexOffset = textVertexIndex;
}
/*********************************************************************************/
intern void K15_InternalCountGUIContextVertices(K15_GUIContext* p_GUIContext, 
												uint32* p_GeometryVertices, 
												uint32* p_TextVertices)
{
	uint32 currentGUIMemoryOffset = 0;
	uint32 numVertices = 0;
	uint32 numTextVertices = 0;

	uint32 guiMemorySize = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_BACK_BUFFER];
	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];

	K15_RenderFontDesc* guiStyleFont = p_GUIContext->style.styleFont;

	while (currentGUIMemoryOffset < guiMemorySize)
	{
		K15_GUIElementHeader* guiElement = (K15_GUIElementHeader*)(guiMemory + currentGUIMemoryOffset);
		K15_GUIElementType guiElementType = guiElement->type;

		switch(guiElementType)
		{
		case K15_GUI_TYPE_BUTTON:
			{
				K15_GUIButton* guiButton = (K15_GUIButton*)(guiMemory + currentGUIMemoryOffset + sizeof(K15_GUIElementHeader));
				const char* text = (const char*)(guiMemory + guiButton->textOffsetInBytes);
				numVertices += 54;
				numTextVertices += K15_GetTextVertexCount(guiStyleFont, text, guiButton->textLength);
				break;
			}

		default:
			{
				K15_ASSERT_TEXT(false, "Missing gui type vertex count '%d'", guiElementType);
				break;
			}
		}

		currentGUIMemoryOffset += guiElement->offset;
	}

	*p_GeometryVertices = numVertices;
	*p_TextVertices = numTextVertices;
}
/*********************************************************************************/
intern void K15_InternalFillGUIContextVertexBuffer(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext, 
													float* p_VertexBuffer, uint32 *p_VertexBufferFloatCount, 
													float* p_TextVertexBuffer, uint32 *p_TextVertexBufferFloatCount)
{
	uint32 currentGUIMemoryOffset = 0;
	uint32 vertexIndex = 0;
	uint32 textVertexIndex = 0;

	uint32 viewportWidth = p_RenderBackEnd->viewportWidth;
	uint32 viewportHeight = p_RenderBackEnd->viewportHeight;

	uint32 guiMemorySize = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_BACK_BUFFER];
	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];

	K15_GUIContextStyle* guiStyle = &p_GUIContext->style;
	K15_RenderFontDesc* guiStyleFont = guiStyle->styleFont;

	uint32 guiTextureWidth = 256;
	uint32 guiTextureHeight = 128;

	while (currentGUIMemoryOffset < guiMemorySize)
	{
		K15_GUIElementHeader* guiElement = (K15_GUIElementHeader*)(guiMemory + currentGUIMemoryOffset);
		K15_GUIElementType guiElementType = guiElement->type;

		switch(guiElementType) 
		{
		case K15_GUI_TYPE_BUTTON:
			{
				K15_GUIButton* guiButton = (K15_GUIButton*)(guiMemory + currentGUIMemoryOffset + sizeof(K15_GUIElementHeader));
				
				K15_InternalPushGUIButtonVertices(p_RenderBackEnd, p_GUIContext, 
					guiElement, guiButton, p_VertexBuffer, &vertexIndex,
					p_TextVertexBuffer, &textVertexIndex);
				break;
			}

		default:
			{
				K15_ASSERT_TEXT(false, "Missing gui type vertex information '%d'", guiElementType);
				break;
			}
		}

		currentGUIMemoryOffset += guiElement->offset;
	}

	*p_VertexBufferFloatCount = vertexIndex;
	*p_TextVertexBufferFloatCount = textVertexIndex;
}
/*********************************************************************************/