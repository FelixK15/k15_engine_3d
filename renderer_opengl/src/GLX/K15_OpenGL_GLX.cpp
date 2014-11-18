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

#include "K15_OpenGL_PrecompiledHeader.h"

#ifdef K15_OS_LINUX

#include "GLX/K15_OpenGL_GLX.h"
#include "K15_OpenGL_Prerequisites.h"

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
    Display* display = RenderWindowImpl::ms_Display;
    Window window = RenderWindowImpl::ms_Window;
    GLint gl_major, gl_minor;
    int glx_major, glx_minor;
    int attribList[RenderWindowImpl::X_ATTRIB_ARRAY_SIZE] = {0};
    //get glx version
    glXQueryVersion(display, &glx_major, &glx_minor);
    K15_LOG_NORMAL("Using glx version %d.%d.",
                   glx_major, glx_minor);

    RenderWindowImpl::getXAttributes(attribList, RenderWindowImpl::X_ATTRIB_ARRAY_SIZE, false);

    //get a XVisualInfo ptr for the above specified attributes
    XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attribList);

    if(!vi)
    {
        K15_LOG_ERROR("Could not get XVisualInfo via glXChooseVisual.");
        return GL_FALSE;
    }

    //create legacy context (even if we want > 3.0 context)
    GLXContext legacyContext = glXCreateContext(display, vi, 0, True);

    XFree(vi); vi = 0;

    if(!legacyContext)
    {
        K15_LOG_ERROR("Could not create legacy GLX context.");
        return GL_FALSE;
    }

    //make legacy context current to get proc pointer
    glXMakeCurrent(display, window, legacyContext);

    glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
    glGetIntegerv(GL_MINOR_VERSION, &gl_minor);

    K15_LOG_SUCCESS("Successfully created legacy OpenGL %d.%d context",
                    gl_major, gl_minor);

    //now is the time to initialize glew...
    K15_LOG_NORMAL("Trying to initialize glew...");
    GLenum initState = glewInit();
    if(initState == GLEW_NO_ERROR)
    {
        K15_LOG_SUCCESS("Successfully initialized glew. (Version %s)", glewGetString(GLEW_VERSION));
    }
    else
    {
        K15_LOG_ERROR("Could not initialize glew. Error: \"%s\"", glewGetString(initState));
    }

    //if we want a >= 3.0context, we need to do a bit more...
    if(gl_major < K15_MIN_GL_VERSION_MAJOR ||
      (gl_major == K15_MIN_GL_VERSION_MAJOR &&
       gl_minor < K15_MIN_GL_VERSION_MINOR))
    {
        K15_LOG_NORMAL("Trying to create %d.%d OpenGL context...",
                       K15_MIN_GL_VERSION_MAJOR,
                       K15_MIN_GL_VERSION_MINOR);

        GLint contextFlags = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;

        //debug context for debug build
        #ifdef K15_DEBUG
            contextFlags |= GLX_CONTEXT_DEBUG_BIT_ARB;
        #endif //K15_DEBUG

        GLint glAttribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, K15_MIN_GL_VERSION_MAJOR,
            GLX_CONTEXT_MINOR_VERSION_ARB, K15_MIN_GL_VERSION_MINOR,
            GLX_CONTEXT_FLAGS_ARB, contextFlags,
            None
        };

        //get proc address for context creation function
        PFNGLXCREATECONTEXTATTRIBSARBPROC kglXCreateContextAttribARB =
                (PFNGLXCREATECONTEXTATTRIBSARBPROC)kglGetProcAddress("glXCreateContextAttribsARB");

        PFNGLXCHOOSEFBCONFIGPROC kglXChooseFBConfig =
                (PFNGLXCHOOSEFBCONFIGPROC)kglGetProcAddress("glXChooseFBConfig");

        if(!kglXCreateContextAttribARB || !kglXChooseFBConfig)
        {
            K15_LOG_ERROR("OpenGL Version %d.%d is not supported.",
                          K15_MIN_GL_VERSION_MAJOR,
                          K15_MIN_GL_VERSION_MINOR);

            K15_LOG_WARNING("Sticking with %d.%d legacy context. Trying to emulate as much functionality as possible.",
                            gl_major, gl_minor);

            return GL_TRUE;
        }

        GLXFBConfig* framebufferCfg = NULL;
        int fbcount = 0;

        RenderWindowImpl::getXAttributes(attribList, RenderWindowImpl::X_ATTRIB_ARRAY_SIZE, true);

        if(!(framebufferCfg = kglXChooseFBConfig(display, DefaultScreen(display), attribList, &fbcount)))
        {
            K15_LOG_ERROR("Could not aquire GLXFBConifg via glXChooseFBConfig.");
            K15_LOG_WARNING("Sticking with %d.%d legacy context. Trying to emulate as much functionality as possible.",
                            gl_major, gl_minor);

            return GL_TRUE;
        }

        if(GLXContext context = kglXCreateContextAttribARB(display, framebufferCfg[0], 0, True, glAttribs))
        {
            glXDestroyContext(display, legacyContext); legacyContext = 0;
            glXMakeCurrent(display, window, context);

            glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
            glGetIntegerv(GL_MINOR_VERSION, &gl_minor);

            K15_LOG_SUCCESS("Successfully created GLX %d.%d context.",
                            gl_major, gl_minor);

            return GL_TRUE;
        }

        K15_LOG_ERROR("Could not create GLX %d.%d context.",
                      gl_major, gl_minor);

        K15_LOG_WARNING("Sticking with %d.%d legacy context. Trying to emulate as much functionality as possible.",
                        gl_major, gl_minor);
    }

    return GL_TRUE;
}
/*********************************************************************************/
GLboolean K15_Engine::Rendering::OpenGL::_glxSwapBuffers()
{
    glXSwapBuffers(RenderWindowImpl::ms_Display, RenderWindowImpl::ms_Window);

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
