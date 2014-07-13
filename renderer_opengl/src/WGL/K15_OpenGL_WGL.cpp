/**
 * @file K15_OpenGL_WGL.cpp
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

#include "K15_LogManager.h"
#include "K15_RenderWindow.h"

#ifdef K15_OS_WINDOWS

#include "WGL/K15_OpenGL_WGL.h"

/*********************************************************************************/
PFNK15GLINIT        kglInit =			K15_Engine::Rendering::OpenGL::_wglInit;
PFNK15GLSWAPBUFFERS kglSwapBuffers =	K15_Engine::Rendering::OpenGL::_wglSwapBuffers;
PFNK15GLSHUTDOWN    kglShutdown =		K15_Engine::Rendering::OpenGL::_wglShutdown;
PFNGLGETPROCADDRESS kglGetProcAddress = K15_Engine::Rendering::OpenGL::_wglGetProcAddress;
/*********************************************************************************/

/*********************************************************************************/
HDC   ms_DeviceContext  = 0;
HGLRC ms_RenderContext  = 0;
/*********************************************************************************/

/*********************************************************************************/
bool createDummyContext(HWND* p_Hwnd,HDC* p_DC)
{
  //Create dummy window
  HWND tempHandle = 0;
  HDC tempDC = 0; int pixelFormat = 0;
  if((tempHandle = CreateWindow("K15_RenderWindowClass","OGL DUMMY WINDOW",0,0,0,0,0,0,0,0,0)) != 0)
  {
    tempDC = GetDC(tempHandle);

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

    if((pixelFormat = ChoosePixelFormat(tempDC,&px)) == 0)
    {
      return false;
    }

    SetPixelFormat(tempDC,pixelFormat,&px);

    ms_RenderContext = wglCreateContext(tempDC);
    wglMakeCurrent(tempDC,ms_RenderContext);

    return true;
  }

  return false;
}
/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_wglInit(GLint p_ColorBits, GLint p_DepthBits, GLint p_StencilBits)
{
  K15_LOG_NORMAL("Creating dummy OGL context to initialize GLEW library.");
  HWND tempHwnd = 0;
  HDC tempDC = 0;
  if(!createDummyContext(&tempHwnd, &tempDC))
  {
    K15_LOG_ERROR("Could not create dummy OGL context (%s)", g_Application->getLastError());
    return GL_FALSE;
  }
  K15_LOG_SUCCESS("Successfully created dummy OGL context!");

  K15_LOG_NORMAL("Trying to initialize GLEW library...");

  GLenum error = glewInit();
  if(error != GLEW_OK)
  {
    K15_LOG_ERROR("Could not initialize GLEW library. (%s)", glewGetErrorString(error));
    return GL_FALSE;
  }

  K15_LOG_SUCCESS("Successfully initialized GLEW library!");
  K15_LOG_SUCCESS("Supported GLEW Version:%s", glewGetString(GLEW_VERSION));

  K15_LOG_NORMAL("Destroying dummy OGL context...");
  _wglShutdown();
  //destroy the objects for the temp handles
  ReleaseDC(tempHwnd, tempDC);
  DestroyWindow(tempHwnd);

  if(!GLEW_VERSION_3_3)
  {
    K15_LOG_ERROR("OpenGL 3.3 is not supported.");
    return GL_FALSE;
  }

  ms_DeviceContext = RenderWindow_Win32::getDeviceContext();

  int colorBits   = p_ColorBits;
  int depthBits   = p_DepthBits;
  int stencilBits = p_StencilBits;

  K15_LOG_NORMAL("Creating real OGL context...");

  const int pixelFormatAttributes[] = {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB,     colorBits,
    WGL_DEPTH_BITS_ARB,     depthBits,
    WGL_STENCIL_BITS_ARB,   stencilBits,
    0
  };

  int pixelFormat = 0;
  unsigned int formatCount = 1;

  K15_LOG_NORMAL("Trying to create pixelformat\n\tColorbuffer size per pixel:%i\n\tDepthbuffer size per pixel:%i\n\tStencilbuffer size per pixel:%i.",
    colorBits, depthBits, stencilBits);

  wglChoosePixelFormatARB(ms_DeviceContext, pixelFormatAttributes, 0, 1, &pixelFormat, &formatCount);

  if(!pixelFormat)
  {
    K15_LOG_ERROR("Can't create pixel format because the is not supported. Error:%s", glGetString(glGetError()));
  }

  SetPixelFormat(ms_DeviceContext, pixelFormat, 0);

  const int contextAttributes[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_FLAGS_ARB,WGL_CONTEXT_DEBUG_BIT_ARB,
    0 //END
  };

  K15_LOG_NORMAL("Creating OGL rendering context...");
  if((ms_RenderContext = wglCreateContextAttribsARB(ms_DeviceContext, 0, contextAttributes)) == 0)
  {
    K15_LOG_ERROR("Could not create OGL rendering context. Error:%s", g_Application->getLastError());
    return GL_FALSE;
  }

  K15_LOG_SUCCESS("Successfully created OGL rendering context!");
  K15_LOG_NORMAL("Setting OGL rendering context as current context.");

  if(wglMakeCurrent(ms_DeviceContext, ms_RenderContext) == FALSE)
  {
    K15_LOG_ERROR("Could not set OGL rendering context as current context. %s", g_Application->getLastError());
    return GL_FALSE;
  }

  K15_LOG_SUCCESS("Succesfully set OGL rendering context.");
  K15_LOG_SUCCESS("Supported OpenGL Version:\"%s\"", glGetString(GL_VERSION));

  return GL_TRUE;
}
/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_wglSwapBuffers()
{
  	SwapBuffers(ms_DeviceContext);

    return GL_TRUE;
}
/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_wglShutdown()
{
  if(ms_RenderContext)
  {
    wglMakeCurrent(0,0);
    wglDeleteContext(ms_RenderContext);

	return GL_TRUE;
  }

  return GL_FALSE;
}
/*********************************************************************************/
GLvoid* K15_Engine::Rendering::OpenGL::_wglGetProcAddress(GLchar* p_ProcName)
{
  return (GLvoid*)wglGetProcAddress(p_ProcName);
}
/*********************************************************************************/

#endif //K15_OS_WINDOWS
