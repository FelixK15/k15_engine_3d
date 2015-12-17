#ifndef _K15_Rendering_GLContext_h_
#define _K15_Rendering_GLContext_h_

#include "K15_RenderPrerequisites.h"
#include "K15_RenderGLTypes.h"
#include "K15_RenderTextureDesc.h"
#include "K15_RenderProgramDesc.h"
#include "K15_RenderBufferDesc.h"

enum K15_GLObjectType
{
	K15_GL_TYPE_INVALID = 0,
	K15_GL_TYPE_TEXTURE,
	K15_GL_TYPE_BUFFER,
	K15_GL_TYPE_SAMPLER,
	K15_GL_TYPE_PROGRAM,
	K15_GL_TYPE_RENDER_TARGET,
	K15_GL_TYPE_VERTEX_FORMAT,
	K15_GL_TYPE_INPUT_LAYOUT
};

struct K15_GLObjectHeader
{
	K15_GLObjectType type;
	byte* glObjectData;
};

struct K15_GLExtensions
{
	uint32 count;
	char** names;

	struct  
	{
		char board[10];
		bool8 supported;
	} vertex_array_object;

	struct  
	{
		char board[10];
		bool8 supported;
	} sparse_buffer;

	struct 
	{
		char board[10];
		bool8 supported;
	} sparse_texture;

	struct  
	{
		char board[10];
		bool8 supported;
	} sampler_objects;

	struct  
	{
		char board[10];
		bool8 supported;
	} direct_state_access;

	struct 
	{
		char board[10];
		bool8 supported;
		GLuint programPipelineHandle; //handle to global program pipeline
	} separate_shader_objects;

	struct 
	{
		char board[10];
		bool8 supported;
		GLuint frameBufferHandle; //handle to the currently attached framebuffer
		K15_GLRenderTarget renderTargets[K15_RENDER_MAX_GPU_RENDER_TARGETS];
	} framebuffer_object;

	struct 
	{
		char board[10];
		bool8 supported;
	} debug;

	struct 
	{
		uint32 supportedCompressionMask;
	} compression;
};

struct K15_GLVertexCacheEntry
{
	K15_RenderResourceHandle bufferHandle;
	uint32 vertexFormatHash;
	uint32 sizeInBytes;
	uint32 capacityInBytes;
};

struct K15_GLVertexCache
{
	K15_GLVertexCacheEntry cacheEntries[K15_RENDER_GL_MAX_VERTEX_CACHE_ENTRIES];
	uint32 numEntries;
};

struct K15_GLPendingBufferUpdate
{
	GLuint glBufferHandle;
	GLintptr glOffsetInBytes;
	GLsizei glSizeInBytes;
	void* data;
};

struct K15_GLPendingBufferUpdateManager
{
	K15_GLPendingBufferUpdate pendingUpdate[K15_RENDER_GL_MAX_PENDING_BUFFER_UPDATES];
	uint32 numPendingUpdates;
};

struct K15_GLTextureSlot
{
	K15_GLTexture* glTexture;
	K15_GLSampler* glSampler;
};

struct K15_GLTextureManager
{
	K15_GLTextureSlot* textureSlots;
	uint32 numTextureSlots;
	uint32 maxTextureSlots;
};

struct K15_GLRenderContext
{
	K15_GLExtensions extensions;
	K15_GLObjectHeader glObjects[K15_RENDER_GL_MAX_OBJECTS];
	K15_GLVertexCache vertexCache;
	K15_GLPendingBufferUpdateManager pendingBufferUpdateManager;
	K15_GLTextureManager textureManager;
	K15_CustomMemoryAllocator* memoryAllocator;

	//uint32 lastFreeGLObjectIndex;

	const char* vendorString;
	const char* rendererString;
	const char* glslVersionString; //#version ...

	struct  
	{
		GLint major;
		GLint minor;
	} version;

	struct 
	{
		//K15_GLVertexFormat* boundVertexFormat;
		uint32 boundVertexFormatHash;
		K15_RenderMaterialPassDesc* boundMaterialPass;
		K15_GLBuffer* boundBuffers[K15_RENDER_BUFFER_TYPE_COUNT];
		K15_GLProgram* boundPrograms[K15_RENDER_PROGRAM_TYPE_COUNT];
	} glBoundObjects;
		
	void* platformContextData;
};

result8 K15_GLCreateRenderContext(K15_CustomMemoryAllocator* p_MemoryAllocator, K15_RenderBackEnd* p_RenderBackEnd, K15_OSContext* p_OSLayerContext);

#endif //_K15_Rendering_GLContext_h_