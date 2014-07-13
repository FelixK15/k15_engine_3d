/**
 * @file K15_OpenGL_GLX.cpp
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

#ifdef K15_OS_LINUX

#include "GLX/K15_OpenGL_GLX.h"

/*********************************************************************************/
PFNK15GLINIT        kglInit =			K15_Engine::Rendering::OpenGL::_glxInit;
PFNK15GLSWAPBUFFERS kglSwapBuffers =	K15_Engine::Rendering::OpenGL::_glxSwapBuffers;
PFNK15GLSHUTDOWN    kglShutdown =		K15_Engine::Rendering::OpenGL::_glxShutdown;
PFNGLGETPROCADDRESS kglGetProcAddress = K15_Engine::Rendering::OpenGL::_glxGetProcAddress;
/*********************************************************************************/

/*********************************************************************************/
GLXContext glxContext = 0;
/*********************************************************************************/

/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_glxInit(GLint p_ColorBits, GLint p_DepthBits, GLint p_StencilBits)
{
    //http://www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)

    GLint glAttributes[] = {
        GLX_RED_SIZE, p_ColorBits / 4,
        GLX_GREEN_SIZE, p_ColorBits / 4,
        GLX_BLUE_SIZE, p_ColorBits / 4,
        GLX_ALPHA_SIZE, p_ColorBits / 4,
        GLX_DEPTH_SIZE, p_DepthBits,
        GLX_STENCIL_SIZE, p_StencilBits,
        GLX_DOUBLEBUFFER, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        None
    };

    PFNGLXCHOOSEFBCONFIGPROC            kglXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC)kglGetProcAddress("glXChooseFBConfig");
    PFNGLXGETVISUALFROMFBCONFIGPROC     kglXGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC)kglGetProcAddress("glxGetVisualFromFBConfig");
    PFNGLXCREATECONTEXTATTRIBSARBPROC   kglXCreateContextAttribARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)kglGetProcAddress("glXCreateContextAttribsARB");

    if(kglXChooseFBConfig && kglXGetVisualFromFBConfig && kglXCreateContextAttribARB)
    {
        Display* display = RenderWindowImpl::ms_Display;
        Window window = RenderWindowImpl::ms_Window;
        int countConfigs = 0;

        int glx_major, glx_minor;

        glXQueryVersion(display, &glx_major, &glx_minor);

        GLXFBConfig* configs = kglXChooseFBConfig(display, DefaultScreen(display), glAttributes, &countConfigs);

        if(!configs)
        {
            K15_LOG_ERROR("Could not get GLFXConfig.");
            return GL_FALSE;
        }

        XVisualInfo* vi = 0;
        for(int i = 0; i < countConfigs; ++i)
        {
            if(vi = kglXGetVisualFromFBConfig(display, configs[i]))
            {
                break;
            }
        }

        if(vi)
        {
            Colormap cmap = XCreateColormap(display, window, vi->visual, AllocNone);

            XSetWindowColormap(display, window, cmap);

            XFree(vi); vi = 0;
        }

        GLint glVersionAttribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            #ifdef K15_DEBUG
                GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
            #endif //K15_DEBUG
            None
        };

        if(glxContext = kglXCreateContextAttribARB(display, configs[0], 0, True, glVersionAttribs))
        {
            if(glXIsDirect(display, glxContext))
            {
                K15_LOG_SUCCESS("Sucessfully created a direct GLX context.");
            }
            else
            {
                K15_LOG_SUCCESS("Sucessfully created an indirect GLX context.");
            }

            glXMakeCurrent(display, window, glxContext);
        }
        else
        {
            K15_LOG_ERROR("Could not get valid OpenGL 3.3 context.");
            return GL_FALSE;
        }

        XFree(configs); configs = 0;
    }
    else
    {
        K15_LOG_ERROR("Could not retrieve necessary functions.");
        return GL_FALSE;
    }
    K15_LOG_NORMAL("Trying to initialize glew...");
    GLenum initState = glewInit();
    if(initState == GLEW_NO_ERROR)
    {
        K15_LOG_SUCCESS("Successfully initialized glew. (Version %s)", glewGetString(GLEW_VERSION));
    }
    else
    {
        K15_LOG_ERROR("Could not initialize glew. Error: \"%s\"", glewGetString(initState));
        return GL_FALSE;
    }

    return GL_TRUE;
}
/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_glxSwapBuffers()
{
    //glXSwapBuffers(RenderWindowImpl::getDisplay(), );

	return GL_TRUE;
}
/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_glxShutdown()
{
	return GL_TRUE;
}
/*********************************************************************************/
GLvoid* K15_Engine::Rendering::OpenGL::_glxGetProcAddress(GLchar* p_ProcName)
{
    return (GLvoid*)glXGetProcAddress((const GLubyte*)p_ProcName);
}
/*********************************************************************************/

#endif //K15_OS_LINUX
