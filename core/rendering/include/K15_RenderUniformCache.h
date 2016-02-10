#ifndef _K15_Rendering_RenderUniformCache_h_
#define _K15_Rendering_RenderUniformCache_h_

#include "K15_RenderProgramDesc.h"
#include "K15_RenderShaderSemantics.h"

struct K15_RenderUniformCacheEntry
{
	const char* name;
	uint32 nameHash;
	byte* data;
	uint32 sizeInBytes;
	uint32 typeID;
	uint32 semanticID;
	uint32 semanticGroupID;
};

struct K15_RenderUniformCache
{
	K15_RenderUniformCacheEntry* cachedUniforms;
	bool8 dirtyUniformSemantics[K15_UNIFORM_SEMANTIC_COUNT];

	uint32 numUniforms;
};

void K15_InitializeRenderUniformCache(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderUniformCache* p_UniformCache);

void K15_UpdateUniformCacheEntry(K15_RenderUniformCache* p_RenderUniformCache, 
								 K15_ShaderUniformSemantics p_SemanticID, 
								 byte* p_Data); //size is implied by the uniform type

void K15_ResetDirtyUniformFlags(K15_RenderUniformCache* p_RenderUniformCache);
void K15_MarkAllUniformsDirty(K15_RenderUniformCache* p_RenderUniformCache);

//void K15_SetAutoUniformGroupAsDirty(K15_RenderUniformCache* p_RenderUniformCache, uint32 p_SemanticGroupID);
#endif //_K15_Rendering_RenderUniformCache_h_