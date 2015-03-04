#ifndef _K15_Rendering_GLContext_h_
#define _K15_Rendering_GLContext_h_

#include "K15_RenderPrerequisites.h"
#include "K15_RenderGLTypes.h"

struct K15_GLRenderContext
{
	struct  
	{
		uint32 count;
		char** names;
	} extensions;

	struct  
	{
		GLint major;
		GLint minor;
	} version;

	struct  
	{
		const GLubyte* vendorString;
		const GLubyte* rendererString;
	};

	void* userData;
};

uint8 K15_GLCreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSLayerContext);

#endif //_K15_Rendering_GLContext_h_