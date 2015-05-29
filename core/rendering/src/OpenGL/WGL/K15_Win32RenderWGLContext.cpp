#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"

#include "OpenGL/K15_RenderGLContext.h"
#include "OpenGL/WGL/wglext.h"

#include <K15_DefaultCLibraries.h>
#include <K15_OSContext.h>
#include <K15_Window.h>

#include <win32/K15_WindowWin32.h>
#include <win32/K15_OSContextWin32.h>

/*********************************************************************************/
intern inline uint8 K15_Win32InternalCreateDummyContext(HWND hwnd, HDC dc, HGLRC* p_Context)
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
			return K15_OS_ERROR_SYSTEM;
		}

		SetPixelFormat(dc, pixelFormatIndex, &px);

		context = kwglCreateContext(dc);
		kwglMakeCurrent(dc, context);

		*p_Context = context;
	}

	return context != 0 ? K15_SUCCESS : K15_OS_ERROR_SYSTEM;
}
/*********************************************************************************/
intern inline void K15_Win32InternalGetWGLFunctionPointer(HMODULE p_LibModule)
{
	K15_CHECK_ASSIGNMENT(kwglGetProcAddress, (PFNWGLGETPROCADDRESSPROC)GetProcAddress(p_LibModule, "wglGetProcAddress"));
	K15_CHECK_ASSIGNMENT(kwglCreateContext, (PFNWGLCREATECONTEXTPROC)GetProcAddress(p_LibModule, "wglCreateContext"));
	K15_CHECK_ASSIGNMENT(kwglDeleteContext, (PFNWGLDELETECONTEXTPROC)GetProcAddress(p_LibModule, "wglDeleteContext"));
	K15_CHECK_ASSIGNMENT(kwglMakeCurrent, (PFNWGLMAKECURRENTPROC)GetProcAddress(p_LibModule, "wglMakeCurrent"));
}
/*********************************************************************************/
intern inline void K15_Win32InternalGetWGLContextFunctionPointer(HMODULE p_LibModule)
{
	//These function have to be loaded pre extension checking or directly from the opengl32.dll library
	K15_CHECK_ASSIGNMENT(kwglChoosePixelFormatARB, (PFNWGLCHOOSEPIXELFORMATARBPROC)kwglGetProcAddress("wglChoosePixelFormatARB"));
	K15_CHECK_ASSIGNMENT(kglGetStringi,	(PFNGLGETSTRINGIPROC)kwglGetProcAddress("glGetStringi"));
	K15_CHECK_ASSIGNMENT(kwglCreateContextAttribsARB, (PFNWGLCREATECONTEXTATTRIBSARBPROC)kwglGetProcAddress("wglCreateContextAttribsARB"));

	K15_CHECK_ASSIGNMENT(kglBindTexture, (PFNGLBINDTEXTUREPROC)GetProcAddress(p_LibModule, "glBindTexture"));
	K15_CHECK_ASSIGNMENT(kglClear, (PFNGLCLEARPROC)GetProcAddress(p_LibModule, "glClear"));
	K15_CHECK_ASSIGNMENT(kglClearColor, (PFNGLCLEARCOLORPROC)GetProcAddress(p_LibModule, "glClearColor"));
	K15_CHECK_ASSIGNMENT(kglCullFace, (PFNGLCULLFACEPROC)GetProcAddress(p_LibModule, "glCullFace"));
	K15_CHECK_ASSIGNMENT(kglDeleteTextures, (PFNGLDELETETEXTURESPROC)GetProcAddress(p_LibModule, "glDeleteTextures"));
	K15_CHECK_ASSIGNMENT(kglDepthFunc, (PFNGLDEPTHFUNCPROC)GetProcAddress(p_LibModule, "glDepthFunc"));
	K15_CHECK_ASSIGNMENT(kglDisable, (PFNGLDISABLEPROC)GetProcAddress(p_LibModule, "glDisable"));
	K15_CHECK_ASSIGNMENT(kglDrawElements, (PFNGLDRAWELEMENTSPROC)GetProcAddress(p_LibModule, "glDrawElements"));
	K15_CHECK_ASSIGNMENT(kglEnable, (PFNGLENABLEPROC)GetProcAddress(p_LibModule, "glEnable"));
	K15_CHECK_ASSIGNMENT(kglFrontFace, (PFNGLFRONTFACEPROC)GetProcAddress(p_LibModule, "glFrontFace"));
	K15_CHECK_ASSIGNMENT(kglGenTextures, (PFNGLGENTEXTURESPROC)GetProcAddress(p_LibModule, "glGenTextures"));
	K15_CHECK_ASSIGNMENT(kglGetError, (PFNGLGETERRORPROC)GetProcAddress(p_LibModule, "glGetError"));
	K15_CHECK_ASSIGNMENT(kglGetFloatv, (PFNGLGETFLOATVPROC)GetProcAddress(p_LibModule, "glGetFloatv"));
	K15_CHECK_ASSIGNMENT(kglPolygonMode, (PFNGLPOLYGONMODEPROC)GetProcAddress(p_LibModule, "glPolygonMode"));
	K15_CHECK_ASSIGNMENT(kglViewport, (PFNGLVIEWPORTPROC)GetProcAddress(p_LibModule, "glViewport"));
	K15_CHECK_ASSIGNMENT(kglGetIntegerv, (PFNGLGETINTEGERVPROC)GetProcAddress(p_LibModule, "glGetIntegerv"));
	K15_CHECK_ASSIGNMENT(kglGetString, (PFNGLGETSTRINGPROC)GetProcAddress(p_LibModule, "glGetString"));
}
/*********************************************************************************/
intern GLvoid* K15_Win32GetProcAddress(const char* p_ProcName)
{
	return (GLvoid*)kwglGetProcAddress(p_ProcName);
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
uint8 K15_Win32CreateGLContext(K15_GLRenderContext* p_RenderContext, K15_OSContext* p_OSContext)
{
	K15_Win32Context* win32Context = (K15_Win32Context*)p_OSContext->userData;

	if (!p_OSContext->window.window)
	{
		return K15_OS_ERROR_NO_WINDOW;
	}

	HMODULE libModule = LoadLibraryA("opengl32.dll");

	if (!libModule)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	K15_Win32InternalGetWGLFunctionPointer(libModule);

	K15_Win32Window* win32Window = (K15_Win32Window*)p_OSContext->window.window->userData;
	HWND hwnd = win32Window->hwnd;
	HDC dc = GetDC(hwnd);

	HGLRC context = 0;

 	uint8 result = K15_Win32InternalCreateDummyContext(hwnd, dc, &context);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	//context got created. retrieve function pointer
	K15_Win32InternalGetWGLContextFunctionPointer(libModule);

	//unbind and delete dummy context
	kwglMakeCurrent(dc, 0);
	kwglDeleteContext(context);

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
		return K15_OS_ERROR_SYSTEM;
	}

	SetPixelFormat(dc, pixelFormatIndex, 0);

	int contextFlags = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;

#ifdef K15_OPENGL_CREATE_DEBUG_CONTEXT
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
		return K15_OS_ERROR_SYSTEM;
	}

	if (kwglMakeCurrent(dc, context) == FALSE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	K15_Win32GLContext* win32GLContext = (K15_Win32GLContext*)malloc(sizeof(K15_Win32GLContext));

	if (!win32GLContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	win32GLContext->context = context;
	win32GLContext->dc = dc;

	p_RenderContext->userData = (void*)win32GLContext;

	kglSwapBuffers = K15_Win32SwapBuffers;
	kglGetProcAddress = K15_Win32GetProcAddress;

	return K15_SUCCESS;
}
/*********************************************************************************/