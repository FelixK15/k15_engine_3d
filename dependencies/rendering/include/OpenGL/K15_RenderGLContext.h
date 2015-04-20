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

	union
	{
		struct 
		{
			K15_GLBuffer buffers[K15_RENDER_GL_MAX_BUFFER_COUNT];
			K15_GLBufferAccessData accessData[K15_RENDER_MAX_GPU_BUFFER];
			K15_GLRenderTarget renderTargets[K15_RENDER_MAX_GPU_RENDER_TARGETS];
			K15_GLProgram programs[K15_RENDER_MAX_GPU_PROGRAMS];
			K15_GLTexture textures[K15_RENDER_MAX_GPU_TEXTURES];
			K15_GLSampler samplers[K15_RENDER_MAX_GPU_SAMPLERS];

			GLuint programPipelineHandle;
			GLuint vertexArrayHandle;
			GLuint framebufferHandle;
		} gl3;

	};

	void* userData;
};

uint8 K15_GLCreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSLayerContext);

#endif //_K15_Rendering_GLContext_h_