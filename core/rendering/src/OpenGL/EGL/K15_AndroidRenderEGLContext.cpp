#include "OpenGL/EGL/K15_AndroidRenderEGLContext.h"
#include "OpenGL/K15_RenderGLContext.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#include <android/native_window_jni.h>

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_Window.h>
#include <android/K15_EnvironmentAndroid.h>
#include <android/K15_WindowAndroid.h>

/*********************************************************************************/
intern GLboolean K15_AndroidSwapBuffers(K15_GLRenderContext* p_RenderContext)
{
	K15_AndroidGLContext* glContext = (K15_AndroidGLContext*)p_RenderContext->userData;

	EGLDisplay display = glContext->display;
	EGLSurface surface = glContext->surface;

	eglSwapBuffers(display, surface);

	return GL_TRUE;
}
/*********************************************************************************/
intern GLvoid* K15_AndroidGetProcAddress(const char* p_ProcName)
{
	return (GLvoid*)eglGetProcAddress(p_ProcName);
}
/*********************************************************************************/

/*********************************************************************************/
uint8 K15_AndroidCreateGLContext(K15_GLRenderContext* p_RenderContext, K15_OSLayerContext* p_OSContext)
{
	K15_AndroidContext* androidContext = (K15_AndroidContext*)p_OSContext->userData;
	K15_AndroidWindow* androidWindow = (K15_AndroidWindow*)p_OSContext->window.window->userData;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	EGLint majorVer = 0;
	EGLint minorVer = 0;

	if (eglInitialize(display, &majorVer, &minorVer) == EGL_FALSE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	EGLConfig config = 0;
	EGLint configCount = 0;
	EGLint format = 0;

	const EGLint attributes[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, -1,
		EGL_DEPTH_SIZE, -1,
		EGL_STENCIL_SIZE, -1,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_NONE
	};

	if (eglChooseConfig(display, attributes, &config, 1, &configCount) == EGL_FALSE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	if (eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format) == EGL_FALSE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	ANativeWindow* nativeWindow = androidWindow->nativeWindow;
	EGLSurface surface = eglCreateWindowSurface(display, config, nativeWindow, 0);
	GLint error = eglGetError(); 
	//surface = surface == 0 ? eglCreateWindowSurface(display, config, FORMAT_RGB_565, attributes) : surface;

	if (surface == 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	const EGLint contextAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
	#ifdef K15_OPENGL_CREATE_DEBUG_CONTEXT
		EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
	#endif //K15_OPENGL_ENABLE_DEBUG_CONTEXT
		EGL_CONTEXT_MAJOR_VERSION_KHR, K15_MIN_GL_VERSION_MAJOR,
		EGL_CONTEXT_MINOR_VERSION_KHR, K15_MIN_GL_VERSION_MINOR,

		EGL_NONE
	};

	EGLContext context = eglCreateContext(display, config, 0, contextAttributes);

	if (context == 0)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	EGLBoolean contextMadeCurrent = eglMakeCurrent(display, surface, surface, context);

	if (contextMadeCurrent == EGL_FALSE)
	{
		return K15_OS_ERROR_SYSTEM;
	}

	K15_AndroidGLContext* eglContext = (K15_AndroidGLContext*)malloc(sizeof(K15_AndroidGLContext));

	if (!eglContext)
	{
		return K15_OS_ERROR_OUT_OF_MEMORY;
	}

	eglContext->display = display;
	eglContext->surface = surface;
	eglContext->context = context;

	p_RenderContext->userData = (void*)eglContext;

	kglSwapBuffers = K15_AndroidSwapBuffers;
	kglGetProcAddress = K15_AndroidGetProcAddress;

	return K15_SUCCESS;
}
/*********************************************************************************/
