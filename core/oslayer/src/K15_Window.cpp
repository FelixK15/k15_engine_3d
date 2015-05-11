#include "K15_Window.h"
#include "K15_OSContext.h"

#include "K15_Thread.h"
#include "K15_DefaultCLibraries.h"

/*********************************************************************************/
K15_Window* K15_CreateWindow(uint32 p_Flags, uint8 p_MonitorIndex)
{
	K15Context* OSContext = K15_GetOSLayerContext();

//	K15_ASSERT_TEXT(OSContext->window.window == 0, "Window has already been created.");

	K15_Window* window = (K15_Window*)K15_OS_MALLOC(sizeof(K15_Window));

	uint8 result = OSContext->window.createWindow(OSContext, window, p_Flags, p_MonitorIndex);

	if (result != K15_SUCCESS)
	{
		K15_OS_FREE(window);
		window = 0;
	}

	window->monitorIndex = p_MonitorIndex;

#ifdef K15_DEBUG_MRT
	window->debugging.assignedThread = K15_GetCurrentThread();
#endif //K15_DEBUG_MRT

	OSContext->window.window = window;

	return window;
}
/*********************************************************************************/
uint8 K15_SetWindowDimension(K15_Window* p_Window, uint32 p_Height, uint32 p_Width)
{
	assert(p_Window);

	K15Context* OSContext = K15_GetOSLayerContext();

	uint8 result = OSContext->window.setWindowDimension(OSContext, p_Window, p_Height, p_Width);

	if (result == K15_SUCCESS)
	{
		p_Window->height = p_Height;
		p_Window->width = p_Width;
	}

	return result;
}
/*********************************************************************************/
uint8 K15_SetWindowFullscreen(K15_Window* p_Window, bool8 p_Fullscreen)
{
	assert(p_Window);

	K15Context* OSContext = K15_GetOSLayerContext();

	uint8 result = OSContext->window.setWindowFullscreen(OSContext, p_Window, p_Fullscreen);

	if (result == K15_SUCCESS)
	{
		if (p_Fullscreen == K15_SUCCESS)
		{
			p_Window->flags |= K15_WINDOW_FLAG_FULLSCREEN;
		}
		else
		{
			p_Window->flags %= ~K15_WINDOW_FLAG_FULLSCREEN;
		}
	}

	return result;
}
/*********************************************************************************/
uint8 K15_SetWindowTitle(K15_Window* p_Window, char* p_Title)
{
	assert(p_Window && p_Title);
	
	K15Context* OSContext = K15_GetOSLayerContext();

	size_t titleLength = strlen(p_Title);
	char* copiedTitle = (char*)K15_OS_MALLOC(titleLength);

	if (!copiedTitle)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	memcpy(copiedTitle, p_Title, titleLength);

	uint8 result = OSContext->window.setWindowTitle(OSContext, p_Window, copiedTitle);

	if (result == K15_SUCCESS)
	{
		p_Window->title = copiedTitle;
	}

	return result;
}
/*********************************************************************************/
void K15_CloseWindow(K15_Window* p_Window)
{
	assert(p_Window);

	K15Context* OSContext = K15_GetOSLayerContext();

	assert(OSContext->window.window);

	OSContext->window.closeWindow(OSContext, p_Window);

	K15_OS_FREE(p_Window);

	OSContext->window.window = 0;
}
/*********************************************************************************/
