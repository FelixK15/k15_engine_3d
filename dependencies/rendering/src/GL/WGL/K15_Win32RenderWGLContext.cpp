#include "GL/WGL/K15_Win32RenderWGLContext.h"

#include "GL/K15_RenderGLContext.h"
#include "GL/WGL/wglext.h"

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>

#include <win32/K15_WindowWin32.h>
#include <win32/K15_EnvironmentWin32.h>

/*********************************************************************************/
intern uint8 K15_Win32CreateDummyContext(HWND hwnd, HDC dc, HGLRC* p_Context)
{
	HGLRC context = 0;
	int pixelFormatIndex = 0;

	if (dc)
	{
		PIXELFORMATDESCRIPTOR px;
		ZeroMemory(&px,sizeof(px));

		px.nSize = sizeof(px);
		px.nVersion = 1;
		px.iPixelType = PFD_TYPE_RGBA;
		px.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
		px.cColorBits = 32;
		px.cDepthBits = 24;
		px.cStencilBits = 8;
		px.dwLayerMask = PFD_MAIN_PLANE;

		if((pixelFormatIndex = ChoosePixelFormat(dc, &px)) == 0)
		{
			return K15_ERROR_SYSTEM;
		}

		SetPixelFormat(dc, pixelFormatIndex, &px);

		context = wglCreateContext(dc);
		wglMakeCurrent(dc, context);

		*p_Context = context;
	}

	return context != 0 ? K15_SUCCESS : K15_ERROR_SYSTEM;
}
/*********************************************************************************/
intern GLvoid* K15_Win32GetProcAddress(const char* p_ProcName)
{
	return (GLvoid*)wglGetProcAddress(p_ProcName);
}
/*********************************************************************************/
intern GLboolean K15_Win32SwapBuffers(K15_GLRenderContext* p_RenderContext)
{
	K15_Win32GLContext* win32GLContext = (K15_Win32GLContext*)p_RenderContext->userData;
	HDC dc = win32GLContext->dc;

	SwapBuffers(dc);

	return GL_TRUE;
}
/*********************************************************************************/
uint8 K15_Win32CreateGLContext(K15_GLRenderContext* p_RenderContext, K15_OSLayerContext* p_OSContext)
{
	K15_Win32Context* win32Context = (K15_Win32Context*)p_OSContext->userData;

	if (!p_OSContext->window.window)
	{
		return K15_ERROR_NO_WINDOW;
	}

	K15_Win32Window* win32Window = (K15_Win32Window*)p_OSContext->window.window->userData;
	HWND hwnd = win32Window->hwnd;
	HDC dc = GetDC(hwnd);

	HGLRC context = 0;

 	uint8 result = K15_Win32CreateDummyContext(hwnd, dc, &context);

	if (result != K15_SUCCESS)
	{
		return result;
	}


	kwglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	kwglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	assert(kwglChoosePixelFormatARB);
	assert(kwglCreateContextAttribsARB);

	//unbind and delete dummy context
	wglMakeCurrent(dc, 0);
	wglDeleteContext(context);

	//create real context
	const int pixelFormatAttributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,     32,
		WGL_DEPTH_BITS_ARB,     24,
		WGL_STENCIL_BITS_ARB,   8,
		0
	};

	int pixelFormatIndex = 0;
	unsigned int formatCount = 1;

	kwglChoosePixelFormatARB(dc, pixelFormatAttributes, 0, 1, &pixelFormatIndex, &formatCount);

	if (!pixelFormatIndex)
	{
		return K15_ERROR_SYSTEM;
	}

	SetPixelFormat(dc, pixelFormatIndex, 0);

	int contextFlags = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;

#ifdef K15_DEBUG
	contextFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif //K15_DEBUG

	const int contextAttributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, K15_MIN_GL_VERSION_MAJOR,
		WGL_CONTEXT_MINOR_VERSION_ARB, K15_MIN_GL_VERSION_MINOR,
		WGL_CONTEXT_FLAGS_ARB, contextFlags,
		0 //END
	};

	context = kwglCreateContextAttribsARB(dc, 0, contextAttributes);

	if (!context)
	{
		return K15_ERROR_SYSTEM;
	}

	if (wglMakeCurrent(dc, context) == FALSE)
	{
		return K15_ERROR_SYSTEM;
	}

	K15_Win32GLContext* win32GLContext = (K15_Win32GLContext*)malloc(sizeof(K15_Win32GLContext));

	if (!win32GLContext)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	win32GLContext->context = context;
	win32GLContext->dc = dc;

	p_RenderContext->userData = (void*)win32GLContext;

	kglSwapBuffers = K15_Win32SwapBuffers;
	kglGetProcAddress = K15_Win32GetProcAddress;

	return K15_SUCCESS;
}
/*********************************************************************************/