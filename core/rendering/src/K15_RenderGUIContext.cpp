/*********************************************************************************/
intern uint32 K15_InternalGetGUIButtonVertexCount(K15_GUIContextStyle* p_GUIStyle, K15_GUIElementHeader* p_GUIElement)
{
	return 500;
}
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
			vertexCount += K15_InternalGetGUIButtonVertexCount(style, p_GUIElement);
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

	uint32 textOffsetX = 20;
	uint32 textOffsetY = 10;

	uint32 pixelPosLeft = p_GUIElement->posPixelX;
	uint32 pixelPosRight = p_GUIElement->posPixelX + p_GUIElement->pixelWidth + textOffsetX;
	uint32 pixelPosTop = p_GUIElement->posPixelY;
	uint32 pixelPosBottom = p_GUIElement->posPixelY + p_GUIElement->pixelHeight + textOffsetY;

	uint32 thickness = 2; //1 pixel of thickness border

	uint32 controlUpperBackgroundColor = 0;
	uint32 controlLowerBackgroundColor = 0;

	//text offset
	int32 textPixelPosLeft = pixelPosLeft + textOffsetX / 2;
	int32 textPixelPosTop = pixelPosTop + textOffsetY / 2;

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
	
	int32 edgePixelPosLeft = pixelPosLeft - thickness;
	int32 edgePixelPosRight = pixelPosRight + thickness;
	int32 edgePixelPosTop = pixelPosTop - thickness;
	int32 edgePixelPosBottom = pixelPosBottom + thickness;

	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];

	uint32 textLength = p_GUIButton->textLength;
	char* text = (char*)(guiMemory + p_GUIButton->textOffsetInBytes);

	K15_RenderFontDesc* guiFont = p_GUIContext->style.styleFont;

	//edge
	vertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd,
		p_VertexBuffer, vertexBufferIndex,
		edgePixelPosLeft, edgePixelPosRight, edgePixelPosTop, edgePixelPosBottom,
		borderUpperColor, borderUpperColor, borderLowerColor, borderLowerColor);

// 	element
	vertexBufferIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, 
		p_VertexBuffer, vertexBufferIndex,
		pixelPosLeft, pixelPosRight, pixelPosTop, pixelPosBottom,
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
				numVertices += K15_InternalGetGUIControlVertexCount(p_GUIContext, guiElement);
				numTextVertices += K15_GetTextVertexCount(guiStyleFont, text, guiButton->textLength);
				break;
			}

		case K15_GUI_TYPE_COMBO_BOX:
			{
				K15_GUIComboBox* guiComboBox = (K15_GUIComboBox*)(guiMemory + currentGUIMemoryOffset + sizeof(K15_GUIElementHeader));
				uint32 numElements = guiComboBox->numElements;
				uint32 elementStringOffset = guiComboBox->elementsOffsetInBytes;

				numVertices += 54 + (54 * guiComboBox->numElements);

				for (uint32 elementIndex = 0;
					elementIndex < numElements;
					++elementIndex)
				{
					const char* elementString = (const char*)(guiMemory + elementStringOffset);
					uint32 elementStringLength = (uint32)strlen(elementString);

					elementStringOffset += elementStringLength+1;
					numTextVertices += K15_GetTextVertexCount(guiStyleFont, elementString, elementStringLength);
				}

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

		case K15_GUI_TYPE_COMBO_BOX:
			{
				K15_GUIComboBox* guiComboBox = (K15_GUIComboBox*)(guiMemory + currentGUIMemoryOffset + sizeof(K15_GUIElementHeader));

				K15_InternalPushGUIComboBoxVertices(p_RenderBackEnd, p_GUIContext,
					guiElement, guiComboBox, p_VertexBuffer, &vertexIndex,
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