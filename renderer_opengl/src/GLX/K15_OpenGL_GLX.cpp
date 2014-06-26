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

#include "GLX/K15_OpenGL_GLX.h"

#include "K15_LogManager.h"
#include "K15_RenderWindow.h"

namespace K15_Engine { namespace Rendering { namespace OpenGL {
    GLXContext glxContext = 0;
    /*********************************************************************************/
    GLboolean _glxInit(GLuint p_ColorBits, GLuint p_DepthBits, GLuint p_StencilBits)
    {
        int glAttributes[] = {
            GLX_BUFFER_SIZE, p_ColorBits,
            GLX_DEPTH_SIZE, p_DepthBits,
            GLX_STENCIL_SIZE, p_StencilBits,
            GLX_X_RENDERABLE, GL_TRUE,
            GLX_DOUBLEBUFFEr, GL_TRUE,
            None
        };


        Display* display = RenderWindowImpl::getDisplay();
        Window window = RenderWindowImpl::getWindow();
        GLXFBConfig config = glXChooseFBConfig(display, DefaultScreen(display), glAttributes,
                                               sizeof(glAttributes) / sizeof(glAttributes[0]));

        if(!config)
        {
            K15_LOG_ERROR("Could not create GLFXConfig.");
            return GL_FALSE;
        }

        GLXWindow glWindow = glXCreateWindow(display, config, window, 0);

        if(glWindow == BadMatch || glWindow == BadWindow || glWindow == BadAlloc)
        {
            K15_LOG_ERROR("Could not create GLXWindow. Error:%s (%d)",
                          OSLayer::getLastError().c_str(), errno);

            return GL_FALSE;
        }

        K15_LOG_SUCCESS("Succesfully created GLXWindow.");

        K15_LOG_NORMAL("Trying to create temporary GLX context...");

        GLXContext context = glXCreateNewContext(display, config, GLX_RGBA_TYPE, 0, True);

        if(context == BadValue || context == BadMatch || context = GLX_BAD_CONTEXT)
        {
            K15_LOG_ERROR("Could not create temporary GLX context. Error:\"%s\" (%d)",
                          OSLayer::getLastError().c_str(), errno);
            return GL_FALSE;
        }

        K15_LOG_SUCCESS("Successfully created temporary GLX context...");

        glewInit();

        GLint glVersionAttribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            #ifdef K15_DEBUG
                GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
            #endif //K15_DEBUG
            None
        };

        K15_LOG_NORMAL("Trying to create final GLX context...");

        glxContext = glXCreateContextAttribARB(display, config, 0, True, glVersionAttribs);
        if(context == BadValue || context == BadMatch || context = GLX_BAD_CONTEXT)
        {
            K15_LOG_ERROR("Could not create final GLX context");
        }

        K15_LOG_SUCCESS("Successfully created final GLX context");

        return GL_TRUE;

    }
    /*********************************************************************************/
    GLboolean _glxSwapBuffers()
    {
        glXSwapBuffers(RenderWindowImpl::getDisplay(), );
    }
    /*********************************************************************************/
    GLboolean _glxShutdown()
    {

    }
    /*********************************************************************************/
    GLvoid* _glxGetProcAddress(GLchar* p_ProcName)
    {
        return (GLvoid*)glXGetProcAddress(p_ProcName);
    }
    /*********************************************************************************/
}}} //end of K15_Engine::Rendering::OpenGL namespace
