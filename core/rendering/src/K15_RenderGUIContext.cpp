/*********************************************************************************/
// intern uint32 K15_InternalGetGUIControlVertexCount(K15_GUIContext* p_GUIContext, K15_GUIElementHeader* p_GUIElement)
// {
// 	K15_GUIContextStyle* style = &p_GUIContext->style;
// 	K15_GUIElementType guiElementType = p_GUIElement->type;
// 	uint32 vertexCount = 0;
// 
// 	switch (guiElementType)
// 	{
// 		case K15_GUI_TYPE_BUTTON:
// 		{
// 			vertexCount += 12;
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_WINDOW:
// 		{
// 			vertexCount += 228;
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_FLOAT_SLIDER:
// 		{
// 			vertexCount += 24;
// 		}
// 	}
// 
// 	return vertexCount;
// }
/*********************************************************************************/
intern void K15_InternalPushGUIButtonVertices(K15_GUIElement* p_GUIElement, K15_GUIDrawInformation* p_DrawInfo)
{
	uint32 P3C3Index = p_DrawInfo->numFloatsVertexBufferP3C3;
	uint32 P3T2C3Index = p_DrawInfo->numFloatsVertexBufferP3T2C3;

	float* P2C3Buffer = p_DrawInfo->vertexBufferP3C3;
	float* P3T2C3Buffer = p_DrawInfo->vertexBufferP3T2C3;

	K15_GUIButtonData* buttonData = (K15_GUIButtonData*)K15_GUIGetGUIElementMemory(p_GUIElement);
	K15_GUIButtonStyle* style = buttonData->style;
	K15_RenderFontDesc* font = style->font;
	uint32 textLength = buttonData->textLength;

	char* text = buttonData->text;
	float textHeight = 0.f;
	float textWidth = 0.f;

	K15_GetTextSizeInPixels(font, &textWidth, &textHeight, text, textLength);

	uint32 borderPixelThickness = style->borderPixelThickness;

	uint32 borderUpperColor = style->borderUpperColor;
	uint32 borderLowerColor = style->borderLowerColor;
	uint32 backgroundUpperColor = style->upperBackgroundColor;
	uint32 backgroundLowerColor = style->lowerBackgroundColor;
	uint32 textColor = style->textColor;

	int32 posLeft = p_GUIElement->rect.pixelPosLeft + borderPixelThickness;
	int32 posTop = p_GUIElement->rect.pixelPosTop + borderPixelThickness;
	int32 posRight = p_GUIElement->rect.pixelPosRight - borderPixelThickness;
	int32 posBottom = p_GUIElement->rect.pixelPosBottom - borderPixelThickness;

	uint32 elementWidth = (posRight - posLeft) - borderPixelThickness * 2;
	uint32 elementHeight = (posBottom - posTop) - borderPixelThickness * 2;

	int32 elementCenterX = posLeft + (elementWidth / 2);
	int32 elementCenterY = posTop + (elementHeight / 2);

	int32 textPosLeft = elementCenterX - textWidth / 2;
	int32 textPosRight = elementCenterX + textWidth / 2;
	int32 textPosTop = elementCenterY - textHeight / 2;
	int32 textPosBottom = elementCenterY + textHeight / 2;

	textPosLeft = textPosLeft < posLeft ? posLeft : textPosLeft;
	textPosRight = textPosRight > posRight ? posRight : textPosRight;
	textPosTop = textPosTop < posTop ? posTop : textPosTop;
	textPosBottom = textPosBottom > posBottom ? posBottom : textPosBottom;

	P3C3Index = K15_InternalPush2DScreenspacePixelColoredLineVertices(P2C3Buffer, P3C3Index,
					0, 0, 500, 500, 10, 0xFF00FF, 0x00FF00);

	//border
// 	P3C3Index = K15_InternalPush2DScreenspacePixelColoredRectVertices(P2C3Buffer, P3C3Index,
// 		p_GUIElement->rect.pixelPosLeft, p_GUIElement->rect.pixelPosRight, 
// 		p_GUIElement->rect.pixelPosTop, p_GUIElement->rect.pixelPosBottom,
// 		borderUpperColor, borderUpperColor,
// 		borderLowerColor, borderLowerColor);
// 
// 	//element
// 	P3C3Index = K15_InternalPush2DScreenspacePixelColoredRectVertices(P2C3Buffer, P3C3Index,
// 		posLeft, posRight, posTop, posBottom,
// 		backgroundUpperColor, backgroundUpperColor,
// 		backgroundLowerColor, backgroundLowerColor);
//  
// 	//text
// 	P3T2C3Index = K15_InternalPush2DScreenspacePixelColoredTextVertices(font,
// 		P3T2C3Buffer, P3T2C3Index, textPosLeft, textPosTop, textPosRight, textPosBottom,
// 		textColor, text, textLength);

	p_DrawInfo->numFloatsVertexBufferP3C3 = P3C3Index;
	p_DrawInfo->numFloatsVertexBufferP3T2C3 = P3T2C3Index;
}
/*********************************************************************************/
// intern void K15_InternalPushGUIComboBoxVertices(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext,
// 	K15_GUIElementHeader* p_GUIElement, K15_GUIComboBox* p_GUIComboBox,
// 	float* p_VertexBuffer, uint32* p_VertexBufferIndexOffset,
// 	float* p_TextVertexBuffer, uint32* p_TextVertexBufferIndexOffset)
// {
// 	
// }
/*********************************************************************************/
intern void K15_InternalPushGUIWindowVertices(K15_GUIElement* p_GUIElement, K15_GUIDrawInformation* p_DrawInfo)
{
	K15_GUIWindowData* windowData = (K15_GUIWindowData*)K15_GUIGetGUIElementMemory(p_GUIElement);
	K15_GUIWindowStyle* windowStyle = windowData->style;

	char* title = windowData->title;
	float textPixelWidth = 0.f;
	float textPixelHeight = 0.f;

	K15_GetTextSizeInPixels(windowStyle->font, &textPixelWidth, &textPixelHeight, title, windowData->textLength);

	int32 pixelPosLeft = p_GUIElement->rect.pixelPosLeft;
	int32 pixelPosRight = p_GUIElement->rect.pixelPosRight;
	int32 pixelPosTop = p_GUIElement->rect.pixelPosTop;
	int32 pixelPosBottom = p_GUIElement->rect.pixelPosBottom;
	int32 titlePixelPosBottom = pixelPosTop + textPixelHeight + windowStyle->titlePixelPadding * 2;

	int32 textPixelPosLeft = pixelPosLeft + windowStyle->titlePixelPadding;
	int32 textPixelPosTop = pixelPosTop + windowStyle->titlePixelPadding;
	int32 textPixelPosRight = textPixelPosLeft + textPixelWidth;
	int32 textPixelPosBottom = textPixelPosTop + textPixelHeight;

	uint32 windowUpperBackgroundColor = windowStyle->upperBackgroundColor;
	uint32 windowLowerBackgroundColor = windowStyle->lowerBackgroundColor;
	uint32 windowBorderUpperColor = windowStyle->borderUpperColor;
	uint32 windowBorderLowerColor = windowStyle->borderLowerColor;
	uint32 windowTitleUpperColor = windowStyle->upperTitleBackgroundColor;
	uint32 windowTitleLowerColor = windowStyle->lowerTitleBackgroundColor;
	uint32 windowTitleTextColor = windowStyle->titleTextColor;

	uint32 borderPixelThickness = windowStyle->borderPixelThickness;

	uint32 P3C3Index = p_DrawInfo->numFloatsVertexBufferP3C3;
	uint32 P3T2C3Index = p_DrawInfo->numFloatsVertexBufferP3T2C3;

	float* P2C3Buffer = p_DrawInfo->vertexBufferP3C3;
	float* P3T2C3Buffer = p_DrawInfo->vertexBufferP3T2C3;

	K15_Vector3 borderTopColor = K15_UnpackVector3(windowStyle->borderUpperColor);
	K15_Vector3 borderLowerColor = K15_UnpackVector3(windowStyle->borderLowerColor);
	K15_Vector3 borderLerpTopLowerColor = K15_Lerp(borderTopColor, borderLowerColor, 
		(float)(titlePixelPosBottom - pixelPosTop) / (float)(pixelPosBottom - pixelPosTop));


	P3C3Index = K15_InternalPush2DScreenspacePixelColoredLineVertices(P2C3Buffer, P3C3Index,
		0, 0, 500, 500, 30, 0xFF00FF, 0x00FF00);

	//border title
// 	P3C3Index = K15_InternalPush2DScreenspacePixelColoredRoundRectVertices(P2C3Buffer, P3C3Index,
// 		pixelPosLeft - borderPixelThickness, pixelPosRight + borderPixelThickness, 
// 		pixelPosTop - borderPixelThickness, titlePixelPosBottom,
// 		borderTopColor, borderTopColor,
// 		borderLerpTopLowerColor, borderLerpTopLowerColor,
// 		K15_LEFT_TOP_CORNER | K15_RIGHT_TOP_CORNER,
// 		0.5f);
// 
// 	//border window
// 	P3C3Index = K15_InternalPush2DScreenspacePixelColoredRectVertices(P2C3Buffer, P3C3Index,
// 		pixelPosLeft - borderPixelThickness, pixelPosRight + borderPixelThickness, 
// 		titlePixelPosBottom, pixelPosBottom + borderPixelThickness,
// 		borderLerpTopLowerColor, borderLerpTopLowerColor,
// 		borderLowerColor, borderLowerColor);
// 
// 	//title
// 	P3C3Index = K15_InternalPush2DScreenspacePixelColoredRoundRectVertices(P2C3Buffer, P3C3Index,
// 		pixelPosLeft, pixelPosRight, pixelPosTop, titlePixelPosBottom,
// 		windowTitleUpperColor, windowTitleUpperColor,
// 		windowTitleLowerColor, windowTitleLowerColor,
// 		K15_LEFT_TOP_CORNER | K15_RIGHT_TOP_CORNER,
// 		0.5f);
// 
// 	//window
// 	P3C3Index = K15_InternalPush2DScreenspacePixelColoredRectVertices(P2C3Buffer, P3C3Index,
// 		pixelPosLeft, pixelPosRight, titlePixelPosBottom, pixelPosBottom,
// 		windowUpperBackgroundColor, windowUpperBackgroundColor,
// 		windowLowerBackgroundColor, windowLowerBackgroundColor);
// 
// 	//title text
// 	P3T2C3Index = K15_InternalPush2DScreenspacePixelColoredTextVertices(windowStyle->font,
// 		P3T2C3Buffer, P3T2C3Index, textPixelPosLeft, textPixelPosTop, 
// 		textPixelPosRight, textPixelPosBottom, windowTitleTextColor, title, windowData->textLength);

	p_DrawInfo->numFloatsVertexBufferP3C3 = P3C3Index;
	p_DrawInfo->numFloatsVertexBufferP3T2C3 = P3T2C3Index;
}
/*********************************************************************************/
intern void K15_InternalPushGUILabelVertices(K15_GUIElement* p_GUIElement, K15_GUIDrawInformation* p_DrawInfo)
{
	uint32 P3C3Index = p_DrawInfo->numFloatsVertexBufferP3C3;
	uint32 P3T2C3Index = p_DrawInfo->numFloatsVertexBufferP3T2C3;

	float* P2C3Buffer = p_DrawInfo->vertexBufferP3C3;
	float* P3T2C3Buffer = p_DrawInfo->vertexBufferP3T2C3;

	int32 pixelPosLeft = p_GUIElement->rect.pixelPosLeft;
	int32 pixelPosRight = p_GUIElement->rect.pixelPosRight;
	int32 pixelPosTop = p_GUIElement->rect.pixelPosTop;
	int32 pixelPosBottom = p_GUIElement->rect.pixelPosBottom;

	uint32 pixelHeight = (pixelPosBottom - pixelPosTop);

	K15_GUILabelData* labelData = (K15_GUILabelData*)K15_GUIGetGUIElementMemory(p_GUIElement);

	float textHeight = 0.f;
	float textWidth = 0.f;
	K15_GetTextSizeInPixels(labelData->style->font, &textWidth, &textHeight, labelData->text, labelData->textLength);

	uint32 textOffsetY = pixelHeight / 2 - textHeight / 2;
	
	pixelPosTop += textOffsetY;

	P3T2C3Index = K15_InternalPush2DScreenspacePixelColoredTextVertices(labelData->style->font,
		P3T2C3Buffer, P3T2C3Index, pixelPosLeft, pixelPosTop + textOffsetY,
		pixelPosRight, pixelPosBottom + textOffsetY,
		labelData->style->textColor, labelData->text, labelData->textLength);

	p_DrawInfo->numFloatsVertexBufferP3C3 = P3C3Index;
	p_DrawInfo->numFloatsVertexBufferP3T2C3 = P3T2C3Index;
}
/*********************************************************************************/
// intern void K15_InternalPushGUIFloatSliderVertices(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext,
// 	K15_GUIElementHeader* p_GUIElement, K15_GUIFloatSlider* p_GUISlider,
// 	float* p_VertexBuffer, uint32* p_VertexBufferIndexOffset,
// 	float* p_TextVertexBuffer, uint32* p_TextVertexBufferIndexOffset)
// {
// 	uint32 vertexIndex = *p_VertexBufferIndexOffset;
// 	uint32 textVertexIndex = *p_TextVertexBufferIndexOffset;
// 
// 	K15_GUIContextStyle* style = &p_GUIContext->style;
// 	K15_RenderFontDesc* guiFont = style->styleFont;
// 	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];
// 
// 	int32 pixelPosLeft = p_GUIElement->posPixelX;
// 	int32 pixelPosTop = p_GUIElement->posPixelY;
// 	int32 pixelPosRight = p_GUIElement->posPixelX + p_GUIElement->pixelWidth;
// 	int32 pixelPosBottom = p_GUIElement->posPixelY + p_GUIElement->pixelHeight;
// 
// 	uint32 pixelThickness = 2;
// 	uint32 pixelLineThickness = 2;
// 
// 	uint32 upperBackgroundColor = style->controlUpperBackgroundColor;
// 	uint32 lowerBackgroundColor = style->controlLowerBackgroundColor;
// 
// 	if (p_GUIElement->hovered)
// 	{
// 		upperBackgroundColor = style->hoveredControlUpperBackgroundColor;
// 		lowerBackgroundColor = style->hoveredControlLowerBackgroundColor;
// 	}
// 
// 	if (p_GUIElement->activated)
// 	{
// 		upperBackgroundColor = style->interactedControlUpperBackgroundColor;
// 		lowerBackgroundColor = style->hoveredControlLowerBackgroundColor;
// 	}
// 
// 	int32 sliderPixelOffsetLeft = (p_GUISlider->sliderPixelPosLeft + style->sliderPixelWidth / 2) - pixelPosLeft;
// 
// 	//mid line 
// 	vertexIndex = K15_InternalPush2DScreenspacePixelColoredLineVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
// 		pixelPosLeft, pixelPosTop + p_GUIElement->pixelHeight / 2,
// 		pixelPosRight, pixelPosTop + p_GUIElement->pixelHeight / 2,
// 		pixelLineThickness, 0x000000, 0x000000);
// 
// 	//border (slider)
// 	vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
// 		p_GUISlider->sliderPixelPosLeft, p_GUISlider->sliderPixelPosRight,
// 		p_GUISlider->sliderPixelPosTop, p_GUISlider->sliderPixelPosBottom,
// 		style->controlUpperBorderColor, style->controlUpperBorderColor,
// 		style->controlLowerBorderColor, style->controlLowerBorderColor);
// 
// 	//element (slider)
// 	vertexIndex = K15_InternalPush2DScreenspacePixelColoredRectVertices(p_RenderBackEnd, p_VertexBuffer, vertexIndex,
// 		p_GUISlider->sliderPixelPosLeft + pixelThickness, p_GUISlider->sliderPixelPosRight - pixelThickness,
// 		p_GUISlider->sliderPixelPosTop + pixelThickness, p_GUISlider->sliderPixelPosBottom - pixelThickness,
// 		upperBackgroundColor, upperBackgroundColor,
// 		lowerBackgroundColor, lowerBackgroundColor);
// 
// 	*p_VertexBufferIndexOffset = vertexIndex;
// 	*p_TextVertexBufferIndexOffset = textVertexIndex;
// }
/*********************************************************************************/
// intern void K15_InternalCountGUIElementVertices(K15_GUIContext* p_GUIContext, K15_GUIElementHeader* p_GUIElementHeader, 
// 	void* p_VertexCounter)
// {
// 	uint32* vertexCount = (uint32*)p_VertexCounter;
// 	uint32 guiMemorySize = p_GUIContext->guiMemoryCurrentSize[K15_GUI_MEMORY_BACK_BUFFER];
// 	byte* guiMemory = p_GUIContext->guiMemory[K15_GUI_MEMORY_BACK_BUFFER];
// 
// 	K15_RenderFontDesc* guiStyleFont = p_GUIContext->style.styleFont;
// 	K15_GUIElementType guiElementType = p_GUIElementHeader->type;
// 	byte* guiElementMemory = (byte*)(p_GUIElementHeader + 1);
// 
// 	switch (guiElementType)
// 	{
// 		case K15_GUI_TYPE_BUTTON:
// 		{
// 			K15_GUIButton* guiButton = (K15_GUIButton*)guiElementMemory;
// 			const char* text = (const char*)(guiMemory + guiButton->textOffsetInBytes);
// 			vertexCount[0] += K15_InternalGetGUIControlVertexCount(p_GUIContext, p_GUIElementHeader);
// 			vertexCount[1] += K15_GetTextVertexCount(guiStyleFont, text, guiButton->textLength);
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_WINDOW:
// 		{
// 			K15_GUIWindow* guiWindow = (K15_GUIWindow*)guiElementMemory;
// 			const char* title = (const char*)(guiMemory + guiWindow->titleOffsetInBytes);
// 			vertexCount[0] += K15_InternalGetGUIControlVertexCount(p_GUIContext, p_GUIElementHeader);
// 			vertexCount[1] += K15_GetTextVertexCount(guiStyleFont, title, guiWindow->titleLength);
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_LABEL:
// 		{
// 			K15_GUILabel* guiLabel = (K15_GUILabel*)guiElementMemory;
// 			const char* text = (const char*)(guiMemory + guiLabel->textOffsetInBytes);
// 			vertexCount[1] += K15_GetTextVertexCount(guiStyleFont, text, guiLabel->textLength);
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_FLOAT_SLIDER:
// 		{
// 			vertexCount[0] += K15_InternalGetGUIControlVertexCount(p_GUIContext, p_GUIElementHeader);
// 			break;
// 		}
// 		default:
// 		{
// 			K15_ASSERT_TEXT(false, "Missing gui type vertex count '%d'", guiElementType);
// 			break;
// 		}
// 	}
// }
/*********************************************************************************/
// intern void K15_InternalCountGUIContextVertices(K15_GUIContext* p_GUIContext, 
// 												uint32* p_GeometryVertices, 
// 												uint32* p_TextVertices)
// {
// 	uint32 vertexCount[2] = {};
// 
// 	K15_IterateGUIElementsHelper(p_GUIContext, K15_InternalCountGUIElementVertices, 
// 		(void*)&vertexCount, K15_GUI_MEMORY_BACK_BUFFER);
// 
// 	*p_GeometryVertices = vertexCount[0];
// 	*p_TextVertices = vertexCount[1];
// }
/*********************************************************************************/
// struct K15_InternalGUIRenderInformation
// {
// 	K15_RenderBackEnd* renderBackEnd;
// 	float* vertexBuffer;
// 	float* textVertexBuffer;
// 	uint32 vertexIndex;
// 	uint32 textVertexIndex;
// };
// 
// intern void K15_InternalFillGUIElementVertexBuffer(K15_GUIContext* p_GUIContext, K15_GUIElementHeader* p_GUIElementHeader,
// 	void* p_UserData)
// {
// 	K15_GUIElementType guiElementType = p_GUIElementHeader->type;
// 	byte* guiElementMemory = (byte*)(p_GUIElementHeader + 1);
// 
// 	K15_InternalGUIRenderInformation* renderInformation = (K15_InternalGUIRenderInformation*)p_UserData;
// 	K15_RenderBackEnd* renderBackEnd = renderInformation->renderBackEnd;
// 	float* vertexBuffer = renderInformation->vertexBuffer;
// 	float* textVertexBuffer = renderInformation->textVertexBuffer;
// 	uint32* vertexIndex = &renderInformation->vertexIndex;
// 	uint32* textVertexIndex = &renderInformation->textVertexIndex;
// 
// 	switch (guiElementType)
// 	{
// 		case K15_GUI_TYPE_BUTTON:
// 		{
// 			K15_GUIButton* guiButton = (K15_GUIButton*)guiElementMemory;
// 
// 			K15_InternalPushGUIButtonVertices(renderBackEnd, p_GUIContext,
// 				p_GUIElementHeader, guiButton, vertexBuffer, vertexIndex,
// 				textVertexBuffer, textVertexIndex);
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_COMBO_BOX:
// 		{
// 			K15_GUIComboBox* guiComboBox = (K15_GUIComboBox*)guiElementMemory;
// 
// 			K15_InternalPushGUIComboBoxVertices(renderBackEnd, p_GUIContext,
// 				p_GUIElementHeader, guiComboBox, vertexBuffer, vertexIndex,
// 				textVertexBuffer, textVertexIndex);
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_WINDOW:
// 		{
// 			K15_GUIWindow* guiWindow = (K15_GUIWindow*)guiElementMemory;
// 
// 			K15_InternalPushGUIWindowVertices(renderBackEnd, p_GUIContext,
// 				p_GUIElementHeader, guiWindow, vertexBuffer, vertexIndex,
// 				textVertexBuffer, textVertexIndex);
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_LABEL:
// 		{
// 			K15_GUILabel* guiLabel = (K15_GUILabel*)guiElementMemory;
// 
// 			K15_InternalPushGUILabelVertices(renderBackEnd, p_GUIContext,
// 				p_GUIElementHeader, guiLabel, vertexBuffer, vertexIndex,
// 				textVertexBuffer, textVertexIndex);
// 			break;
// 		}
// 
// 		case K15_GUI_TYPE_FLOAT_SLIDER:
// 		{
// 			K15_GUIFloatSlider* guiSlider = (K15_GUIFloatSlider*)guiElementMemory;
// 
// 			K15_InternalPushGUIFloatSliderVertices(renderBackEnd, p_GUIContext,
// 				p_GUIElementHeader, guiSlider, vertexBuffer, vertexIndex,
// 				textVertexBuffer, textVertexIndex);
// 			break;
// 		}
// 
// 		default:
// 		{
// 			K15_ASSERT_TEXT(false, "Missing gui type vertex information '%d'", guiElementType);
// 			break;
// 		}
// 	}
// }
/*********************************************************************************/
// intern void K15_InternalFillGUIContextVertexBuffer(K15_RenderBackEnd* p_RenderBackEnd, K15_GUIContext* p_GUIContext, 
// 													float* p_VertexBuffer, uint32 *p_VertexBufferFloatCount, 
// 													float* p_TextVertexBuffer, uint32 *p_TextVertexBufferFloatCount)
// {
// 	K15_InternalGUIRenderInformation renderInformation = {};
// 	renderInformation.renderBackEnd = p_RenderBackEnd;
// 	renderInformation.vertexBuffer = p_VertexBuffer;
// 	renderInformation.textVertexBuffer = p_TextVertexBuffer;
// 	
// 	K15_IterateGUIElementsHelper(p_GUIContext, K15_InternalFillGUIElementVertexBuffer, 
// 		(void*)&renderInformation, K15_GUI_MEMORY_BACK_BUFFER);
// 
// 	*p_VertexBufferFloatCount = renderInformation.vertexIndex;
// 	*p_TextVertexBufferFloatCount = renderInformation.textVertexIndex;
// }
/*********************************************************************************/