#ifndef _K15_Rendering_GL_EGLContext_h_
#define _K15_Rendering_GL_EGLContext_h_

#include "K15_RenderPrerequisites.h"

typedef void* EGLDisplay;
typedef void* EGLSurface;
typedef void* EGLContext;

struct K15_GLRenderContext;

struct K15_AndroidGLContext
{
	EGLDisplay display;
	EGLSurface surface;
	EGLContext ms_Context;
};

uint8 K15_AndroidCreateGLContext(K15_GLRenderContext* p_RenderContext, K15_OSLayerContext* p_OSContext);

#endif //_K15_Rendering_GL_EGLContext_h_