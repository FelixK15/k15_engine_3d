#ifndef _K15_Rendering_GLContext_h_
#define _K15_Rendering_GLContext_h_

#include "K15_RenderPrerequisites.h"
#include "K15_RenderGLTypes.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderProgramDesc.h"
#include "K15_RenderBufferDesc.h"

struct K15_GLExtensions
{
	uint32 count;
	char** names;

	struct  
	{
		bool8 supported;
		GLuint vertexArrayHandle; //handle to global vertex array
	} vertex_array_object;

	struct  
	{
		bool8 supported;
	} sampler_objects;

	struct 
	{
		bool8 supported;
		GLuint programPipelineHandle; //handle to global program pipeline
	} separate_shader_objects;

	struct 
	{
		bool8 supported;
		GLuint frameBufferHandle; //handle to the currently attached framebuffer
		K15_GLRenderTarget renderTargets[K15_RENDER_MAX_GPU_RENDER_TARGETS];
	} framebuffer_object;

	struct 
	{
		bool8 supported;
	} debug_output;

	struct 
	{
		uint32 supportedCompressionMask;
	} compression;
};

struct K15_GLRenderContext
{
	K15_GLExtensions extensions;

	struct  
	{
		GLint major;
		GLint minor;
	} version;

	struct  
	{
		K15_GLBuffer buffers[K15_RENDER_GL_MAX_BUFFER_COUNT];
		K15_GLTexture textures[K15_RENDER_MAX_GPU_TEXTURES];
		K15_GLSampler samplers[K15_RENDER_MAX_GPU_SAMPLERS];
		K15_GLProgram programs[K15_RENDER_MAX_GPU_PROGRAMS];
	} glObjects;

	struct 
	{
		K15_GLBuffer* boundBuffers[K15_RENDER_BUFFER_TYPE_COUNT];
		K15_GLTexture* boundTextures[K15_RENDER_TEXTURE_TYPE_COUNT];
		K15_GLProgram* boundPrograms[K15_RENDER_PROGRAM_TYPE_COUNT];
	} glBoundObjects;
		
	void* platformContextData;
};

result8 K15_GLCreateRenderContext(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderBackEnd* p_RenderBackEnd, K15_OSContext* p_OSLayerContext);

#endif //_K15_Rendering_GLContext_h_