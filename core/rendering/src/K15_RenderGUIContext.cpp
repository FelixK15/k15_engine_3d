/*********************************************************************************/
intern uint32 K15_InternalPushGUIButtonVertices(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext,
												K15_GUIElementHeader* p_GUIElement, K15_GUIButton* p_GUIButton,
												float* p_VertexBuffer, uint32 p_VertexBufferIndexOffset)
{
	uint32 vertexIndex = p_VertexBufferIndexOffset;

	uint32 viewportWidth = p_RenderBackEnd->viewportWidth;
	uint32 viewportHeight = p_RenderBackEnd->viewportHeight;

	K15_GUIContextStyle* guiStyle = &p_GUIContext->style;
	K15_GUIButtonState guiButtonState = p_GUIButton->state;

	float guiTextureWidth = 256.f;
	float guiTextureHeight = 128.f;

	float buttonStylePixelPosX	 = guiStyle->guiButtonStyle.stateStyle[guiButtonState].posPixelX;
	float buttonStylePixelPosY	 = guiStyle->guiButtonStyle.stateStyle[guiButtonState].posPixelY;
	float buttonStylePixelWidth	 = guiStyle->guiButtonStyle.stateStyle[guiButtonState].pixelWidth;
	float buttonStylePixelHeight = guiStyle->guiButtonStyle.stateStyle[guiButtonState].pixelHeight;

	float elementPosPixelX	 = (float)p_GUIElement->posPixelX;
	float elementPosPixelY	 = (float)p_GUIElement->posPixelY;
	float elementPixelWidth  = (float)p_GUIElement->pixelWidth;
	float elementPixelHeight = (float)p_GUIElement->pixelHeight;

	float viewWidth	 = (float)viewportWidth;
	float viewHeight = (float)viewportHeight;

	float elementMarginLeft	  = (float)guiStyle->guiButtonStyle.stateStyle[guiButtonState].marginLeft;
	float elementMarginRight  = (float)guiStyle->guiButtonStyle.stateStyle[guiButtonState].marginRight;
	float elementMarginTop	  = (float)guiStyle->guiButtonStyle.stateStyle[guiButtonState].marginTop;
	float elementMarginBottom = (float)guiStyle->guiButtonStyle.stateStyle[guiButtonState].marginBottom;

	float elementLeft	= K15_CONVERT_TO_NDC_X(elementPosPixelX / viewWidth);
	float elementTop	= K15_CONVERT_TO_NDC_Y(elementPosPixelY / viewHeight);
	float elementRight	= K15_CONVERT_TO_NDC_X((elementPosPixelX + elementPixelWidth) / viewWidth);
	float elementBottom = K15_CONVERT_TO_NDC_Y((elementPosPixelY + elementPixelHeight) / viewHeight);

	float elementInnerLeft	 = K15_CONVERT_TO_NDC_X((elementPosPixelX + elementMarginLeft) / viewWidth);
	float elementInnerRight	 = K15_CONVERT_TO_NDC_X((elementPosPixelX + elementPixelWidth - elementMarginRight) / viewWidth);
	float elementInnerTop	 = K15_CONVERT_TO_NDC_Y((elementPosPixelY + elementMarginTop) / viewHeight);
	float elementInnerBottom = K15_CONVERT_TO_NDC_Y((elementPosPixelY + elementPixelHeight - elementMarginBottom) / viewHeight);

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
	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementLeft, elementInnerLeft, elementTop, elementInnerTop,
		uvBorderLeft, uvInnerLeft, uvBorderTop, uvInnerTop);

	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementLeft, elementInnerLeft, elementInnerBottom, elementBottom,
		uvBorderLeft, uvInnerLeft, uvInnerBottom, uvBorderBottom);

	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementLeft, elementInnerLeft, elementInnerTop, elementInnerBottom,
		uvBorderLeft, uvInnerLeft, uvInnerTop, uvInnerBottom);

	//right border
	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerRight, elementRight, elementInnerTop, elementInnerBottom,
		uvInnerRight, uvBorderRight, uvInnerTop, uvInnerBottom);

	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerRight, elementRight, elementInnerBottom, elementBottom,
		uvInnerRight, uvBorderRight, uvInnerBottom, uvBorderBottom);

	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerRight, elementRight, elementTop, elementInnerTop,
		uvInnerRight, uvBorderRight, uvBorderTop, uvInnerTop);


	//top border
	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerLeft, elementInnerRight, elementTop, elementInnerTop,
		uvInnerLeft, uvInnerRight, uvBorderTop, uvInnerTop);
 
 	//bottom border
 	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex, 
		elementInnerLeft, elementInnerRight, elementInnerBottom, elementBottom,
		uvInnerLeft, uvInnerRight, uvInnerBottom, uvBorderBottom);

	//background
	/*********************************************************************************/
	vertexIndex = K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
		elementInnerLeft, elementInnerRight, elementInnerTop, elementBottom,
		uvInnerLeft, uvInnerRight, uvInnerTop, uvInnerBottom);

	return vertexIndex;
}
/*********************************************************************************/
intern uint32 K15_InternalCountGUIContextVertices(K15_GUIContext* p_GUIContext)
{
	uint32 currentGUIMemoryOffset = 0;
	uint32 numVertices = 0;

	uint32 guiMemorySize = p_GUIContext->guiMemoryCurrentSize;
	byte* guiMemory = p_GUIContext->guiMemory;

	while (currentGUIMemoryOffset < guiMemorySize)
	{
		K15_GUIElementHeader* guiElement = (K15_GUIElementHeader*)(guiMemory + currentGUIMemoryOffset);
		K15_GUIElementType guiElementType = guiElement->type;

		switch(guiElementType)
		{
		case K15_GUI_TYPE_BUTTON:
			{
				numVertices += 54;
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

	return numVertices;
}
/*********************************************************************************/
intern uint32 K15_InteralFillGUIContextVertexBuffer(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext, 
													float* p_VertexBuffer, uint32 p_NumVertices)
{
	uint32 currentGUIMemoryOffset = 0;
	uint32 vertexIndex = 0;

	uint32 viewportWidth = p_RenderBackEnd->viewportWidth;
	uint32 viewportHeight = p_RenderBackEnd->viewportHeight;

	uint32 guiMemorySize = p_GUIContext->guiMemoryCurrentSize;
	byte* guiMemory = p_GUIContext->guiMemory;

	K15_GUIContextStyle* guiStyle = &p_GUIContext->style;

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
				vertexIndex = K15_InternalPushGUIButtonVertices(p_RenderBackEnd, p_GUIContext, 
									guiElement, guiButton, p_VertexBuffer, vertexIndex);

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

	return vertexIndex;
}
/*********************************************************************************/