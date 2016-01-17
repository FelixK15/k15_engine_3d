/*********************************************************************************/
intern uint32 K15_InternalGetGUIControlVertexCount(K15_GUIContext* p_GUIContext, K15_GUIElementHeader* p_GUIElement)
{
	K15_GUIContextStyle* style = &p_GUIContext->style;
	K15_GUIElementType guiElementType = p_GUIElement->type;
	uint32 vertexCount = 0;

	switch (guiElementType)
	{
		case K15_GUI_TYPE_BUTTON:
		{
			vertexCount += 12;
			break;
		}

		case K15_GUI_TYPE_WINDOW:
		{
			vertexCount += 228;
			break;
		}
	}

	return vertexCount;
}
/*********************************************************************************/
intern void K15_InternalPushGUIButtonVertices(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext,
												K15_GUIElementHeader* p_GUIElement, K15_GUIButton* p_GUIButton,
												float* p_VertexBuffer, uint32* p_VertexBufferIndexOffset,
												float* p_TextVertexBuffer, uint32* p_TextVertexBufferIndexOffset)
{
	uint32 vertexBufferIndex = *p_VertexBufferIndexOffset;
	uint32 textVertexBufferIndex = *p_TextVertexBufferIndexOffset;

	uint32 pixelPosLeft = p_GUIElement->posPixelX;
	uint32 pixelPosRight = p_GUIElement->posPixelX + p_GUIElement->pixelWidth;
	uint32 pixelPosTop = p_GUIElement->posPixelY;
	uint32 pixelPosBottom = p_GUIElement->posPixelY + p_GUIElement->pixelHeight;

	uint32 thickness = 2; //1 pixel of thickness border

	uint32 controlUpperBackgroundColor = 0;
	uint32 controlLowerBackgroundColor = 0;

	uint32 textPixelWidth = p_GUIButton->textPixelWidth;
	uint32 textPixelHeight = p_GUIButton->textPixelHeight;

	uint32 textOffsetX = p_GUIElement->pixelWidth / 2 - textPixelWidth / 2;
	uint32 textOffsetY = p_GUIElement->pixelHeight / 2 - textPixelHeight / 2;

	//text offset
	int32 textPixelPosLeft = pixelPosLeft + textOffsetX;
	int32 textPixelPosTop = pixelPosTop + textOffsetY;

	if (p_GUIButton->state == K15_GUI_BUTTON_STATE_HOVERED)
	{
		controlUpperBackgroundColor = p_GUIContext->style.hoveredControlUpperBackgroundColor;
		controlLowerBackgroundColor = p_GUIContext->style.hoveredControlLowerBackgroundColor;
	}
	else if (p_GUIButton->state == K15_GUI_BUTTON_STATE_NORMAL)
	{
		controlUpperBackgroundColor = p_GUIContext->style.controlUpperBackgroundColor;
		controlLowerBackgroundColor = p_GUIContext->style.controlLowerBackgroundColor;
	}
	else if (p_GUIButton->state == K15_GUI_BUTTON_STATE_PRESSED)
	{
		controlUpperBackgroundColor = p_GUIContext->style.interactedControlLowerBackgroundColor;
		controlLowerBackgroundColor = p_GUIContext->style.interactedControlUpperBackgroundColor;
	}

	uint32 textColor = p_GUIContext->style.textColor;

	uint32 borderUpperColor = p_GUIContext->style.controlUpperBorderColor;
	uint32 borderLowerColor = p_GUIContext->style.controlLowerBorderColor;	

	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];

	uint32 textLength = p_GUIButton->textLength;
	char* text = (char*)(guiMemory + p_GUIButton->textOffsetInBytes);

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;

	//edge
	vertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd,
		p_VertexBuffer, vertexBufferIndex,
		pixelPosLeft, pixelPosRight, pixelPosTop, pixelPosBottom,
		borderUpperColor, borderUpperColor, borderLowerColor, borderLowerColor);

// 	element
	vertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, 
		p_VertexBuffer, vertexBufferIndex,
		pixelPosLeft + thickness, pixelPosRight - thickness, pixelPosTop + thickness, pixelPosBottom - thickness,
		controlUpperBackgroundColor, controlUpperBackgroundColor,
		controlLowerBackgroundColor, controlLowerBackgroundColor);
 
	//text
	textVertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredTextVertices(p_RenderBackEnd,
		guiFont, p_TextVertexBuffer, textVertexBufferIndex,
		textPixelPosLeft, textPixelPosTop, textColor, text, textLength);

	*p_TextVertexBufferIndexOffset = textVertexBufferIndex;
	*p_VertexBufferIndexOffset = vertexBufferIndex;
}
/*********************************************************************************/
intern void K15_InternalPushGUIComboBoxVertices(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext,
	K15_GUIElementHeader* p_GUIElement, K15_GUIComboBox* p_GUIComboBox,
	float* p_VertexBuffer, uint32* p_VertexBufferIndexOffset,
	float* p_TextVertexBuffer, uint32* p_TextVertexBufferIndexOffset)
{
	
}
/*********************************************************************************/
intern void K15_InternalPushGUIWindowVertices(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext,
	K15_GUIElementHeader* p_GUIElement, K15_GUIWindow* p_GUIWindow,
	float* p_VertexBuffer, uint32* p_VertexBufferIndexOffset,
	float* p_TextVertexBuffer, uint32* p_TextVertexBufferIndexOffset)
{
	uint32 vertexBufferIndex = *p_VertexBufferIndexOffset;
	uint32 textVertexBufferIndex = *p_TextVertexBufferIndexOffset;

	int32 pixelPosLeft = p_GUIElement->posPixelX;
	int32 pixelPosRight = p_GUIElement->posPixelX + p_GUIElement->pixelWidth;
	int32 pixelPosTop = p_GUIElement->posPixelY;
	int32 pixelPosBottom = p_GUIElement->posPixelY + p_GUIElement->pixelHeight;
	int32 windowPixelWidth = p_GUIWindow->windowPixelWidth;
	int32 windowPixelHeight = p_GUIWindow->windowPixelHeight;

	uint32 pixelPosCenterHorizontal = p_GUIElement->pixelWidth / 2;
	uint32 pixelPosCenterVertical = p_GUIElement->pixelHeight / 2;

	K15_GUIContextStyle* style = &p_GUIContext->style;
	K15_RenderFontDesc* guiFont = style->styleFont;

	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];

	char* text = (char*)(guiMemory + p_GUIWindow->titleOffsetInBytes);
	float textPixelWidth = 0.f;
	float textPixelHeight = 0.f;

	K15_GetTextSizeInPixels(guiFont, &textPixelWidth, &textPixelHeight, text, p_GUIWindow->titleLength);

	int32 textPixelPosLeft = pixelPosCenterHorizontal - (uint32)(textPixelWidth*0.5f) + pixelPosLeft;
	int32 textPixelPosTop = pixelPosCenterVertical - (uint32)(textPixelHeight*0.5f) + pixelPosTop;

	uint32 windowTitleUpperColor = style->windowTitleBarUpperColor;
	uint32 windowTitleLowerColor = style->windowTitleBarLowerColor;
	uint32 windowBorderUpperColor = style->windowBorderUpperColor;
	uint32 windowBorderLowerColor = style->windowBorderLowerColor;
	uint32 windowUpperBackgroundColor = style->windowUpperBackgroundColor;
	uint32 windowLowerBackgroundColor = style->windowLowerBackgroundColor;
	uint32 windowTitleTextColor = style->windowTitleTextColor;

	uint32 borderPixelThickness = 2;

	//border title
	vertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredRoundRectVertices(p_RenderBackEnd, p_VertexBuffer,
		vertexBufferIndex,
		pixelPosLeft - borderPixelThickness, pixelPosRight + borderPixelThickness, 
		pixelPosTop - borderPixelThickness, pixelPosBottom + borderPixelThickness,
		windowBorderUpperColor, windowBorderUpperColor,
		windowBorderUpperColor, windowBorderUpperColor,
		K15_LEFT_TOP_CORNER | K15_RIGHT_TOP_CORNER,
		0.5f);

	//border window
	vertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, p_VertexBuffer,
		vertexBufferIndex,
		pixelPosLeft - borderPixelThickness, pixelPosRight + borderPixelThickness, 
		pixelPosBottom - borderPixelThickness, pixelPosBottom + windowPixelHeight + borderPixelThickness,
		windowBorderLowerColor, windowBorderLowerColor,
		windowBorderLowerColor, windowBorderLowerColor);

	//title
	vertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredRoundRectVertices(p_RenderBackEnd, p_VertexBuffer, 
		vertexBufferIndex,
		pixelPosLeft, pixelPosRight, pixelPosTop, pixelPosBottom,
		windowTitleUpperColor, windowTitleUpperColor,
		windowTitleLowerColor, windowTitleLowerColor,
		K15_LEFT_TOP_CORNER | K15_RIGHT_TOP_CORNER,
		0.5f);

	//window
	vertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, p_VertexBuffer,
		vertexBufferIndex,
		pixelPosLeft, pixelPosRight, pixelPosBottom, pixelPosBottom + windowPixelHeight,
		windowUpperBackgroundColor, windowUpperBackgroundColor,
		windowLowerBackgroundColor, windowLowerBackgroundColor);

	//title text
	textVertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredTextVertices(p_RenderBackEnd, guiFont, 
		p_TextVertexBuffer, textVertexBufferIndex, 
		textPixelPosLeft, textPixelPosTop, windowTitleTextColor, text, p_GUIWindow->titleLength);

	*p_VertexBufferIndexOffset = vertexBufferIndex;
	*p_TextVertexBufferIndexOffset = textVertexBufferIndex;
}
/*********************************************************************************/
intern void K15_InternalPushGUILabelVertices(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext,
	K15_GUIElementHeader* p_GUIElement, K15_GUILabel* p_GUILabel,
	float* p_VertexBuffer, uint32* p_VertexBufferIndexOffset,
	float* p_TextVertexBuffer, uint32* p_TextVertexBufferIndexOffset)
{
	uint32 textVertexBufferIndex = *p_TextVertexBufferIndexOffset;

	int32 pixelPosLeft = p_GUIElement->posPixelX;
	int32 pixelPosRight = p_GUIElement->posPixelX + p_GUIElement->pixelWidth;
	int32 pixelPosTop = p_GUIElement->posPixelY;
	int32 pixelPosBottom = p_GUIElement->posPixelY + p_GUIElement->pixelHeight;

	uint32 textPixelHeight = p_GUILabel->textPixelHeight;
	uint32 textOffsetY = p_GUIElement->pixelHeight / 2 - textPixelHeight / 2;
	
	pixelPosTop += textOffsetY;

	K15_GUIContextStyle* style = &p_GUIContext->style;
	K15_RenderFontDesc* guiFont = style->styleFont;
	uint32 textLabelColor = style->textLabelColor;

	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];
	char* text = (char*)(guiMemory + p_GUILabel->textOffsetInBytes);

	textVertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredTextVertices(p_RenderBackEnd, guiFont, p_TextVertexBuffer, textVertexBufferIndex,
		pixelPosLeft, pixelPosTop, textLabelColor, text, p_GUILabel->textLength);

	*p_TextVertexBufferIndexOffset = textVertexBufferIndex;
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
		byte* guiElementMemory = (guiMemory + currentGUIMemoryOffset + sizeof(K15_GUIElementHeader));

		switch(guiElementType)
		{
		case K15_GUI_TYPE_BUTTON:
			{
				K15_GUIButton* guiButton = (K15_GUIButton*)guiElementMemory;
				const char* text = (const char*)(guiMemory + guiButton->textOffsetInBytes);
				numVertices += K15_InternalGetGUIControlVertexCount(p_GUIContext, guiElement);
				numTextVertices += K15_GetTextVertexCount(guiStyleFont, text, guiButton->textLength);
				break;
			}

		case K15_GUI_TYPE_WINDOW:
			{
				K15_GUIWindow* guiWindow = (K15_GUIWindow*)guiElementMemory;
				const char* title = (const char*)(guiMemory + guiWindow->titleOffsetInBytes);
				numVertices += K15_InternalGetGUIControlVertexCount(p_GUIContext, guiElement);
				numTextVertices += K15_GetTextVertexCount(guiStyleFont, title, guiWindow->titleLength);
				break;
			}

		case K15_GUI_TYPE_LABEL:
			{
				K15_GUILabel* guiLabel = (K15_GUILabel*)guiElementMemory;
				const char* text = (const char*)(guiMemory + guiLabel->textOffsetInBytes);
				numTextVertices += K15_GetTextVertexCount(guiStyleFont, text, guiLabel->textLength);
				break;
			}

		default:
			{
				K15_ASSERT_TEXT(false, "Missing gui type vertex count '%d'", guiElementType);
				break;
			}
		}

		currentGUIMemoryOffset = guiElement->offset;
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
		byte* guiElementMemory = (guiMemory + currentGUIMemoryOffset + sizeof(K15_GUIElementHeader));

		switch(guiElementType) 
		{
		case K15_GUI_TYPE_BUTTON:
			{
				K15_GUIButton* guiButton = (K15_GUIButton*)guiElementMemory;
				
				K15_InternalPushGUIButtonVertices(p_RenderBackEnd, p_GUIContext, 
					guiElement, guiButton, p_VertexBuffer, &vertexIndex,
					p_TextVertexBuffer, &textVertexIndex);
				break;
			}

		case K15_GUI_TYPE_COMBO_BOX:
			{
				K15_GUIComboBox* guiComboBox = (K15_GUIComboBox*)guiElementMemory;

				K15_InternalPushGUIComboBoxVertices(p_RenderBackEnd, p_GUIContext,
					guiElement, guiComboBox, p_VertexBuffer, &vertexIndex,
					p_TextVertexBuffer, &textVertexIndex);
				break;
			}

		case K15_GUI_TYPE_WINDOW:
			{
				K15_GUIWindow* guiWindow = (K15_GUIWindow*)guiElementMemory;

				K15_InternalPushGUIWindowVertices(p_RenderBackEnd, p_GUIContext,
					guiElement, guiWindow, p_VertexBuffer, &vertexIndex,
					p_TextVertexBuffer, &textVertexIndex);
				break;
			}

		case K15_GUI_TYPE_LABEL:
			{
				K15_GUILabel* guiLabel = (K15_GUILabel*)guiElementMemory;

				K15_InternalPushGUILabelVertices(p_RenderBackEnd, p_GUIContext,
					guiElement, guiLabel, p_VertexBuffer, &vertexIndex,
					p_TextVertexBuffer, &textVertexIndex);
				break;
			}

		default:
			{
				K15_ASSERT_TEXT(false, "Missing gui type vertex information '%d'", guiElementType);
				break;
			}
		}

		currentGUIMemoryOffset = guiElement->offset;
	}

	*p_VertexBufferFloatCount = vertexIndex;
	*p_TextVertexBufferFloatCount = textVertexIndex;
}
/*********************************************************************************/