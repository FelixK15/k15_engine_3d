/**
 * @file K15_OpenGL_GLES2.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "GLES2/K15_OpenGL_GLES2.h"
#include "K15_LogManager.h"

#if defined K15_OS_ANDROID || defined K15_OS_IOS

/*********************************************************************************/
PFNK15GLINIT        kglInit =			K15_Engine::Rendering::OpenGL::_gles2Init;
PFNK15GLSWAPBUFFERS kglSwapBuffers =	K15_Engine::Rendering::OpenGL::_gles2SwapBuffers;
PFNK15GLSHUTDOWN    kglShutdown =		K15_Engine::Rendering::OpenGL::_gles2Shutdown;
PFNGLGETPROCADDRESS kglGetProcAddress = K15_Engine::Rendering::OpenGL::_gles2GetProcAddress;
/*********************************************************************************/

/*********************************************************************************/
EGLDisplay ms_Display;
EGLSurface ms_Surface;
EGLContext ms_Context;
/*********************************************************************************/

/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_gles2Init(GLint p_ColorBits, GLint p_DepthBits, GLint p_StencilBits)
{
	ms_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	EGLint ver_Major = 0;
	EGLint ver_Minor = 0;

	if(eglInitialize(ms_Display,&ver_Major,&ver_Minor) == EGL_FALSE)
	{
		K15_LOG_ERROR("Could not initialize EGL Display.");
		return GL_FALSE;
	}

	K15_LOG_SUCCESS("Successfully initialize EGL Display.");
	K15_LOG_SUCCESS("EGL Version: %i.%i",ver_Major,ver_Minor);

	EGLConfig config = 0;
	EGLint configCount = 0;
	EGLint format = 0;

	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};

	if(eglChooseConfig(ms_Display,attribs,&config,1,&configCount) == EGL_FALSE)
	{
		K15_LOG_ERROR("Could not get EGL config.");
		return GL_FALSE;
	}

	K15_LOG_SUCCESS("EGL config retrieved.");

	if(eglGetConfigAttrib(ms_Display,config,EGL_NATIVE_VISUAL_ID,&format) == EGL_FALSE)
	{
		K15_LOG_ERROR("Could not get config attributes 'EGL_NATIVE_VISUAL_ID'.");
		return GL_FALSE;
	}

	K15_LOG_SUCCESS("Retrieved 'EGL_NATIVE_VISUAL_ID'. (%i)",format);

#if defined K15_OS_ANDROID
	ANativeWindow_setBuffersGeometry(RenderWindowType::getNativeWindow(), 0, 0, format);
	if((ms_Surface = eglCreateWindowSurface(ms_Display, config, RenderWindowType::getNativeWindow(), 0)) == 0)
#endif //K15_OS_ANDROID
	{
		K15_LOG_ERROR("Could not create EGL window surface.");
		return GL_FALSE;
	}

	K15_LOG_SUCCESS("Created EGL window surface.");

	const EGLint contextAttribList[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	if((ms_Context = eglCreateContext(ms_Display,config,0,contextAttribList)) == 0)
	{
		K15_LOG_ERROR("Could not create GLES context.");
		return GL_FALSE;
	}

	K15_LOG_SUCCESS("Created GLES context.");

	if(eglMakeCurrent(ms_Display,ms_Surface,ms_Surface,ms_Context) == EGL_FALSE)
	{
		K15_LOG_ERROR("Could not set GLES context as current context.");
		return GL_FALSE;
	}

	K15_LOG_SUCCESS("Set GLES context as current context.");

	const unsigned char* glesVersion = glGetString(GL_VERSION);

	K15_LOG_SUCCESS("GLES Version: %s",glesVersion);

	GLint width,height;
	eglQuerySurface(ms_Display,ms_Surface,EGL_WIDTH,&width);
	eglQuerySurface(ms_Display,ms_Surface,EGL_HEIGHT,&height);

	Resolution resolution;
	resolution.width = width;
	resolution.height = height;

	K15_LOG_NORMAL("Setting resolution to %ix%i...",width,height);
	RenderWindow::setResolution(resolution,false);

	K15_LOG_DEBUG("Supported OpenGL ES Extensions:%s",glGetString(GL_EXTENSIONS));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);

	return GL_TRUE;
}
/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_gles2SwapBuffers()
{
	eglSwapBuffers(ms_Display,ms_Surface);

	return GL_TRUE;
}
/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_gles2Shutdown()
{
	if(ms_Display)
	{
		eglMakeCurrent(ms_Display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);

		if(ms_Surface)
		{
			eglDestroySurface(ms_Display,ms_Surface);
			ms_Surface = EGL_NO_SURFACE;
		}

		if(ms_Context)
		{
			eglDestroyContext(ms_Display,ms_Context);
			ms_Context = EGL_NO_CONTEXT;
		}

		if(ms_Display)
		{
			eglTerminate(ms_Display);
			ms_Display = EGL_NO_DISPLAY;
		}

		return GL_TRUE;
	}

	return GL_FALSE;
}
/*********************************************************************************/
GLvoid* K15_Engine::Rendering::OpenGL::_gles2GetProcAddress(GLchar* p_ProcName)
{
	return (GLvoid*)eglGetProcAddress(p_ProcName);
}
/*********************************************************************************/

#endif //K15_OS_ANDROID || defined K15_OS_IOS
