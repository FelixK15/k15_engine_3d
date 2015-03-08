#ifndef _K15_Rendering_GL_WGLContext_h_
#define _K15_Rendering_GL_WGLContext_h_

#include "K15_RenderPrerequisites.h"

struct K15_GL_WGLRenderContext;
struct K15_OSLayerContext;
struct K15_GLRenderContext;

struct K15_Win32GLContext
{
	HGLRC context;
	HDC dc;
};

uint8 K15_Win32CreateGLContext(K15_GLRenderContext* p_RenderContext, K15_OSLayerContext* p_OSContext);

#endif //_K15_Rendering_GL_WGLContext_h_