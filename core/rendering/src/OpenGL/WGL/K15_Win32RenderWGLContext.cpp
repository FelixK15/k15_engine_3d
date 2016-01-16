#include "OpenGL/WGL/K15_Win32RenderWGLContext.h"

#include "OpenGL/K15_RenderGLContext.h"
#include "OpenGL/WGL/wglext.h"

#include <K15_CustomMemoryAllocator.h>

#include <K15_DefaultCLibraries.h>
#include <K15_OSContext.h>
#include <K15_Window.h>

#include <win32/K15_WindowWin32.h>
#include <win32/K15_OSContextWin32.h>

/*********************************************************************************/
intern inline uint8 K15_Win32InternalCreateAndBindDummyContext(HGLRC* p_Context, HWND* p_WindowHandle, HDC* p_DeviceContext)
{
	HGLRC context = 0;
	int pixelFormatIndex = 0;

	//create dummy window
	HWND windowHandle = CreateWindowExA(0, "K15_Win32WindowClass", "", 0, 0, 0, 0, 0, 0, 0, 0, 0);

	if (windowHandle)
	{
		HDC dc = GetDC(windowHandle);

		if (dc)
		{
			//dummy pixel format
			PIXELFORMATDESCRIPTOR px =
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //flags
				PFD_TYPE_RGBA,												//framebuffer type
				32,															//colordepth
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				24,															//depth buffer
				8,															//stencil buffer
				0,															//aux buffers
				PFD_MAIN_PLANE,
				0,										
				0, 0, 0
			};

			if((pixelFormatIndex = ChoosePixelFormat(dc, &px)) == 0)
			{
				return K15_OS_ERROR_SYSTEM;
			}

			if (!SetPixelFormat(dc, pixelFormatIndex, &px))
			{
				return K15_OS_ERROR_SYSTEM;
			}

			context = kwglCreateContext(dc);
			kwglMakeCurrent(dc, context);

			*p_Context = context;
			*p_WindowHandle = windowHandle;
			*p_DeviceContext = dc;
		}
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
	K15_CHECK_ASSIGNMENT(kwglGetCurrentContext, (PFNWGLGETCURRENTCONTEXTPROC)GetProcAddress(p_LibModule, "wglGetCurrentContext"));
	K15_CHECK_ASSIGNMENT(kwglGetCurrentDC, (PFNWGLGETCURRENTDCPROC)GetProcAddress(p_LibModule, "wglGetCurrentDC"));

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

	K15_CHECK_ASSIGNMENT(kglTexImage1D, (PFNGLTEXIMAGE1DPROC)GetProcAddress(p_LibModule, "glTexImage1D"));
	K15_CHECK_ASSIGNMENT(kglTexImage2D, (PFNGLTEXIMAGE2DPROC)GetProcAddress(p_LibModule, "glTexImage2D"));
	K15_CHECK_ASSIGNMENT(kglTexParameteri, (PFNGLTEXPARAMETERIPROC)GetProcAddress(p_LibModule, "glTexParameteri"));
	K15_CHECK_ASSIGNMENT(kglDrawArrays, (PFNGLDRAWARRAYSPROC)GetProcAddress(p_LibModule, "glDrawArrays"));
}
/*********************************************************************************/
intern inline void K15_Win32InternalGetWGLContextFunctionPointer()
{
	//These function have to be loaded pre extension checking or directly from the opengl32.dll library
	K15_CHECK_ASSIGNMENT(kwglChoosePixelFormatARB, (PFNWGLCHOOSEPIXELFORMATARBPROC)kwglGetProcAddress("wglChoosePixelFormatARB"));
	K15_CHECK_ASSIGNMENT(kglGetStringi,	(PFNGLGETSTRINGIPROC)kwglGetProcAddress("glGetStringi"));
	K15_CHECK_ASSIGNMENT(kwglCreateContextAttribsARB, (PFNWGLCREATECONTEXTATTRIBSARBPROC)kwglGetProcAddress("wglCreateContextAttribsARB"));
}
/*********************************************************************************/
intern GLvoid* K15_Win32GetProcAddress(const char* p_ProcName)
{
	return (GLvoid*)kwglGetProcAddress(p_ProcName);
}
/*********************************************************************************/
intern GLboolean K15_Win32SwapBuffers(K15_GLRenderContext* p_RenderContext)
{
	K15_Win32GLContext* win32GLContext = (K15_Win32GLContext*)p_RenderContext->platformContextData;
	HDC dc = win32GLContext->dc;

	SwapBuffers(dc);

	return GL_TRUE;
}
/*********************************************************************************/
uint8 K15_Win32CreateGLContext(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_GLRenderContext* p_RenderContext, K15_OSContext* p_OSContext)
{
	K15_Win32Context* win32Context = (K15_Win32Context*)p_OSContext->userData;

	if (!p_OSContext->window.window)
	{
		return K15_OS_ERROR_NO_WINDOW;
	}

	if (p_RenderContext->platformContextData)
	{
		return K15_OS_ERROR_RENDER_CONTEXT_ALREADY_CREATED;
	}

	HMODULE openglModule = GetModuleHandleA("opengl32.dll");

	if (!openglModule)
	{
		openglModule = LoadLibraryA("opengl32.dll");
	}

	if (!openglModule)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	K15_Win32InternalGetWGLFunctionPointer(openglModule);

	HGLRC tempContext = 0;
	HWND tempWindowHandle = 0;
	HDC tempDeviceContext = 0;

	//Create a new dummy window, create a dummy pixel format and create a dummy opengl context
 	uint8 result = K15_Win32InternalCreateAndBindDummyContext(&tempContext, &tempWindowHandle, &tempDeviceContext);

	if (result != K15_SUCCESS)
	{
		return result;
	}

	//context got created. retrieve function pointer
	K15_Win32InternalGetWGLContextFunctionPointer();

	//get real window
	K15_Win32Window* win32Window = (K15_Win32Window*)p_OSContext->window.window->userData;
	HWND hwnd = win32Window->hwnd;
	HDC dc = GetDC(hwnd);

	HGLRC context = 0;

	int dummyWindowPixelFormat = GetPixelFormat(tempDeviceContext);
	PIXELFORMATDESCRIPTOR dummyWindowPixelFormatDesc = {};

	DescribePixelFormat(tempDeviceContext, dummyWindowPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &dummyWindowPixelFormatDesc);

	//create real context
	const int pixelFormatAttributes[] = {
		WGL_DRAW_TO_WINDOW_ARB,				GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,				GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,				GL_TRUE,
		WGL_ACCELERATION_ARB,				WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB,					WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,					dummyWindowPixelFormatDesc.cColorBits,
		WGL_DEPTH_BITS_ARB,					dummyWindowPixelFormatDesc.cDepthBits,
		WGL_STENCIL_BITS_ARB,				dummyWindowPixelFormatDesc.cStencilBits,
		0
	};

	int pixelFormatIndex = 0;
	unsigned int formatCount = 1;

	K15_OPENGL_CALL(kwglChoosePixelFormatARB(dc, pixelFormatAttributes, 0, 1, &pixelFormatIndex, &formatCount));

	if (!pixelFormatIndex)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	if (!SetPixelFormat(dc, pixelFormatIndex, 0))
	{
		return K15_OS_ERROR_SYSTEM;
	}

	int contextFlags = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
	int majorVersion = K15_MIN_GL_VERSION_MAJOR;
	int minorVersion = K15_MIN_GL_VERSION_MINOR;

#ifdef K15_OPENGL_CREATE_DEBUG_CONTEXT
	contextFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif //K15_DEBUG

#ifdef K15_OPENGL_FORCE_ES_2_0_CONTEXT
	contextFlags |= WGL_CONTEXT_ES2_PROFILE_BIT_EXT;
	majorVersion = 2;
	minorVersion = 0;
#elif defined K15_OPENGL_FORCE_ES_3_0_CONTEXT
	contextFlags |= WGL_CONTEXT_ES3_PROFILE_BIT_EXT;
	majorVersion = 3;
	minorVersion = 0;
#elif defined K15_OPENGL_FORCE_ES_3_1_CONTEXT
	contextFlags |= WGL_CONTEXT_ES3_1_PROFILE_BIT_EXT;
	majorVersion = 3;
	minorVersion = 1;
#elif defined K15_OPENGL_FORCE_ES_3_2_CONTEXT
	contextFlags |= WGL_CONTEXT_ES3_2_PROFILE_BIT_EXT;
	majorVersion = 3;
	minorVersion = 2;
#endif 

	const int contextAttributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
		WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
		WGL_CONTEXT_FLAGS_ARB, contextFlags,
		0 //END
	};

	K15_OPENGL_CALL(context = kwglCreateContextAttribsARB(dc, 0, contextAttributes));

	if (!context)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	bool8 contextMadeCurrent = K15_FALSE;

	K15_OPENGL_CALL(contextMadeCurrent = kwglMakeCurrent(dc, context));

	if (contextMadeCurrent)
	{
		//delete temp context, window and device context
		K15_OPENGL_CALL(kwglDeleteContext(tempContext));
		DeleteDC(tempDeviceContext);
		DestroyWindow(tempWindowHandle);
	}
	else
	{
		return K15_OS_ERROR_SYSTEM;
	}	

	//re-retrieve function pointer for the new context
	K15_Win32InternalGetWGLContextFunctionPointer();

	K15_Win32GLContext* win32GLContext = (K15_Win32GLContext*)K15_AllocateFromMemoryAllocator(p_MemoryAllocator, sizeof(K15_Win32GLContext));

	if (!win32GLContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	win32GLContext->context = context;
	win32GLContext->dc = dc;

	p_RenderContext->platformContextData = (void*)win32GLContext;

	kglSwapBuffers = K15_Win32SwapBuffers;
	kglGetProcAddress = K15_Win32GetProcAddress;

	return K15_SUCCESS;
}
/*********************************************************************************/