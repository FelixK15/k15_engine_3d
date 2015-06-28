#ifndef _K15_Rendering_RenderUniformCache_h_
#define _K15_Rendering_RenderUniformCache_h_

#include "K15_RenderProgramDesc.h"

struct K15_RenderUniformCacheEntry
{
	const char* name;
	byte* data;
	K15_AutoUniformType autoType;
	K15_AutoUniformTypeGroupFlag autoTypeGroup;
	K15_UniformType type;
};

struct K15_RenderUniformCache
{
	K15_RenderUniformCacheEntry cachedAutoUniforms[K15_AUTO_UNIFORM_TYPE_COUNT];
	K15_RenderUniformCacheEntry* dirtyAutoUniforms[K15_AUTO_UNIFORM_TYPE_COUNT];
};

void K15_InitializeRenderUniformCache(K15_RenderUniformCache* p_RenderUniformCache);
void K15_UpdateUniformCacheEntry(K15_RenderUniformCache* p_RenderUniformCache, K15_AutoUniformType p_AutoUniformType, byte* p_Data); //size is implied by the uniform type
void K15_SetAutoUniformGroupAsDirty(K15_RenderUniformCache* p_RenderUniformCache, K15_AutoUniformTypeGroupFlag p_DirtyGroupFlag);
#endif //_K15_Rendering_RenderUniformCache_h_