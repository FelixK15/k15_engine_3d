#ifndef _K15_Rendering_RenderUniformCache_h_
#define _K15_Rendering_RenderUniformCache_h_

#include "K15_RenderProgramDesc.h"

struct K15_RenderUniformCacheEntry
{
	const char* name;
	byte* data;
	uint32 typeID;
	uint32 semanticID;
	uint32 semanticGroupID;
};

struct K15_RenderUniformCache
{
	K15_RenderUniformCacheEntry* cachedAutoUniforms;
	K15_RenderUniformCacheEntry** dirtyAutoUniforms;

	uint32 numDirtyUniforms;
};

void K15_InitializeRenderUniformCache(K15_RenderUniformCache* p_RenderUniformCache);
void K15_UpdateUniformCacheEntry(K15_RenderUniformCache* p_RenderUniformCache, uint32 p_SemanticID, byte* p_Data); //size is implied by the uniform type
void K15_SetAutoUniformGroupAsDirty(K15_RenderUniformCache* p_RenderUniformCache, uint32 p_SemanticGroupID);
#endif //_K15_Rendering_RenderUniformCache_h_