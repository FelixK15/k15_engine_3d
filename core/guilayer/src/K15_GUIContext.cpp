#include "K15_GUIContext.h"
#include "K15_CustomMemoryAllocator.h"
#include "K15_DefaultCLibraries.h"

#include "K15_OSContext.h"
#include "K15_Window.h"
#include "K15_SystemEvents.h"

#include "K15_MaterialFormat.h"
#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"

#include "K15_RenderFontDesc.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderCommands.h"
#include "K15_RenderContext.h"
#include "K15_ResourceContext.h"
#include "K15_Logging.h"

#include "K15_Math.h"
#include "K15_String.h"
#include "K15_Thread.h"


/*********************************************************************************/
intern inline K15_GUIContextStyle K15_InternalCreateDefaultStyle(K15_ResourceContext* p_ResourceContext)
{
	K15_GUIContextStyle defaultStyle = {};

	K15_ResourceHandle styleFontResource = 
		K15_LoadResource(p_ResourceContext, K15_FONT_RESOURCE_IDENTIFIER, "fonts/gui_font.ttf", 0); 

	defaultStyle.buttonStyle.borderLowerColor = 0x101010;
	defaultStyle.buttonStyle.borderUpperColor = 0x606060;
	defaultStyle.buttonStyle.borderPixelThickness = 2;
	defaultStyle.buttonStyle.font = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);
	defaultStyle.windowTitlePixelHeight = 20;
	defaultStyle.sliderPixelHeight = 18;
	defaultStyle.sliderPixelWidth = 10;
	defaultStyle.sliderLinePixelWidth = 150;
	defaultStyle.windowContentPixelPadding = 5;
	defaultStyle.buttonContentPixelPadding = 3;

	defaultStyle.controlUpperBackgroundColor = 0x303030;
	defaultStyle.controlLowerBackgroundColor = 0x505050;
	defaultStyle.controlUpperBorderColor = 0x606060;
	defaultStyle.controlLowerBorderColor = 0x101010;
	defaultStyle.hoveredControlUpperBackgroundColor = 0x476e72;
	defaultStyle.hoveredControlLowerBackgroundColor = 0x3a5357;
	defaultStyle.interactedControlLowerBackgroundColor = 0x476e72;
	defaultStyle.interactedControlUpperBackgroundColor = 0x488085;
	defaultStyle.textColor = 0xFFFFFF;
	defaultStyle.windowTitleTextColor = 0xEEEEEE;
	defaultStyle.windowTitleBarLowerColor = 0x4580FF;
	defaultStyle.windowTitleBarUpperColor = 0x4544FF;
	defaultStyle.windowUpperBackgroundColor = 0x101010;
	defaultStyle.windowLowerBackgroundColor = 0x808080;
	defaultStyle.windowBorderUpperColor = 0x050545;
	defaultStyle.windowBorderLowerColor = 0x000000;
	defaultStyle.styleFont = K15_GetResourceRenderFontDesc(p_ResourceContext, styleFontResource);

	return defaultStyle;
}
/*********************************************************************************/

/*********************************************************************************/
K15_GUIContext* K15_CreateGUIContext(K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue)
{
	return K15_CreateGUIContextWithCustomAllocator(K15_CreateDefaultMemoryAllocator(), p_ResourceContext, p_RenderCommandQueue);
}
/*********************************************************************************/
K15_GUIContext* K15_CreateGUIContextWithCustomAllocator(K15_CustomMemoryAllocator p_MemoryAllocator, K15_ResourceContext* p_ResourceContext, K15_RenderCommandQueue* p_RenderCommandQueue)
{
	K15_OSContext* osContext = K15_GetOSLayerContext();
	K15_GUIContext* guiContext = (K15_GUIContext*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, sizeof(K15_GUIContext));

	uint32 windowHeight = 0;
	uint32 windowWidth = 0;

	uint32 bookKeeping = size_kilobyte(5);

	if (osContext->window.window)
	{
		windowHeight = osContext->window.window->height;
		windowWidth = osContext->window.window->width;
	}

	uint32 actualGUIMemorySize = K15_GUI_CONTEXT_MEMORY_SIZE - bookKeeping;
	uint32 guiMemoryBufferSize = actualGUIMemorySize / 2;

	byte* guiMemory = (byte*)K15_AllocateFromMemoryAllocator(&p_MemoryAllocator, actualGUIMemorySize);
	memset(guiMemory, 0, actualGUIMemorySize);

	K15_ASSERT_TEXT(guiMemory, "Could not acquire gui memory (%dkb) from allocator '%s'",
		actualGUIMemorySize / 1024, p_MemoryAllocator.name);

	guiContext->memoryAllocator = p_MemoryAllocator;

	return guiContext;
}