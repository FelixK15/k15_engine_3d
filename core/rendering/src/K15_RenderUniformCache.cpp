/*********************************************************************************/
intern void K15_InternalAddUniformToDirtyCache(K15_RenderUniformCache* p_RenderUniformCache, uint32 p_SemanticID)
{

}
/*********************************************************************************/
intern K15_RenderUniformCacheEntry K15_InternCreateModelMatrixCacheEntry(K15_CustomMemoryAllocator* p_RenderAllocator)
{
	K15_RenderUniformCacheEntry modelMatrixCacheEntry = {};
	modelMatrixCacheEntry.name = K15_SEMANTIC_MODEL_MATRIX_VARIABLE_NAME;
	modelMatrixCacheEntry.semanticID = K15_UNIFORM_SEMANTIC_MODEL_MATRIX;
	modelMatrixCacheEntry.semanticGroupID = K15_UNIFORM_GROUP_PER_OBJECT;
	modelMatrixCacheEntry.data = (byte*)K15_AllocateFromMemoryAllocator(p_RenderAllocator, sizeof(K15_Matrix4));
	modelMatrixCacheEntry.typeID = K15_TYPE_FLOAT_MATRIX4;

	return modelMatrixCacheEntry;
}
/*********************************************************************************/
intern K15_RenderUniformCacheEntry K15_InternCreateProjectionMatrixCacheEntry(K15_CustomMemoryAllocator* p_RenderAllocator)
{
	K15_RenderUniformCacheEntry projectionMatrixCacheEntry = {};
	projectionMatrixCacheEntry.name = K15_SEMANTIC_PROJECTION_MATRIX_VARIABLE_NAME;
	projectionMatrixCacheEntry.semanticID = K15_UNIFORM_SEMANTIC_PROJECTION_MATRIX;
	projectionMatrixCacheEntry.semanticGroupID = K15_UNIFORM_GROUP_PER_CAMERA;
	projectionMatrixCacheEntry.data = (byte*)K15_AllocateFromMemoryAllocator(p_RenderAllocator, sizeof(K15_Matrix4));
	projectionMatrixCacheEntry.typeID = K15_TYPE_FLOAT_MATRIX4;
	
	return projectionMatrixCacheEntry;
}
/*********************************************************************************/
intern K15_RenderUniformCacheEntry K15_InternCreateViewMatrixCacheEntry(K15_CustomMemoryAllocator* p_RenderAllocator)
{
	K15_RenderUniformCacheEntry viewMatrixCacheEntry = {};
	viewMatrixCacheEntry.name = K15_SEMANTIC_VIEW_MATRIX_VARIABLE_NAME;
	viewMatrixCacheEntry.semanticID = K15_UNIFORM_SEMANTIC_VIEW_MATRIX;
	viewMatrixCacheEntry.semanticGroupID = K15_UNIFORM_GROUP_PER_CAMERA;
	viewMatrixCacheEntry.data = (byte*)K15_AllocateFromMemoryAllocator(p_RenderAllocator, sizeof(K15_Matrix4));
	viewMatrixCacheEntry.typeID = K15_TYPE_FLOAT_MATRIX4;

	return viewMatrixCacheEntry;
}
/*********************************************************************************/
intern K15_RenderUniformCacheEntry K15_InternCreateModelViewProjectionMatrixCacheEntry(K15_CustomMemoryAllocator* p_RenderAllocator)
{
	K15_RenderUniformCacheEntry modelViewProjectionMatrixCacheEntry = {};
	modelViewProjectionMatrixCacheEntry.name = K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_VARIABLE_NAME;
	modelViewProjectionMatrixCacheEntry.semanticID = K15_UNIFORM_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX;
	modelViewProjectionMatrixCacheEntry.semanticGroupID = K15_UNIFORM_GROUP_PER_OBJECT;
	modelViewProjectionMatrixCacheEntry.data = (byte*)K15_AllocateFromMemoryAllocator(p_RenderAllocator, sizeof(K15_Matrix4));
	modelViewProjectionMatrixCacheEntry.typeID = K15_TYPE_FLOAT_MATRIX4;

	return modelViewProjectionMatrixCacheEntry;
}
/*********************************************************************************/
intern K15_RenderUniformCacheEntry K15_InternCreateTextureDimensionCacheEntry(K15_CustomMemoryAllocator* p_RenderAllocator, int p_TextureSlot)
{
	K15_RenderUniformCacheEntry textureDimensionCacheEntry = {};

	

	textureDimensionCacheEntry.semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD1_DIMENSION + p_TextureSlot;
	textureDimensionCacheEntry.semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	textureDimensionCacheEntry.data = (byte*)K15_AllocateFromMemoryAllocator(p_RenderAllocator, sizeof(K15_Vector2));
	textureDimensionCacheEntry.typeID = K15_TYPE_INT_VECTOR2;
}
/*********************************************************************************/
void K15_InitializeRenderUniformCache(K15_CustomMemoryAllocator* p_RenderAllocator, K15_RenderUniformCache* p_RenderUniformCache)
{
	K15_ASSERT(p_RenderUniformCache);
	K15_ASSERT(p_RenderAllocator);
	

	memset(p_RenderUniformCache->dirtyAutoUniforms, 0, sizeof(p_RenderUniformCache->dirtyAutoUniforms));
}
/*********************************************************************************/
void K15_UpdateUniformCacheEntry(K15_RenderUniformCache* p_RenderUniformCache, uint32 p_SemanticID, byte* p_Data)
{
	K15_ASSERT(p_RenderUniformCache);
	K15_ASSERT(p_Data);

	K15_InternalAddUniformToDirtyCache(p_RenderUniformCache, p_AutoUniformType);
}
/*********************************************************************************/
// void K15_SetAutoUniformGroupAsDirty(K15_RenderUniformCache* p_RenderUniformCache, K15_AutoUniformTypeGroupFlag p_DirtyGroupFlag)
// {
// 	assert(p_RenderUniformCache);
// 
// 	for (uint32 autoUniformIndex = 0;
// 		autoUniformIndex < K15_AUTO_UNIFORM_TYPE_COUNT;
// 		++autoUniformIndex)
// 	{
// 		K15_RenderUniformCacheEntry* cachedAutoUniform = &p_RenderUniformCache->cachedAutoUniforms[autoUniformIndex];
// 
// 		if ((cachedAutoUniform->autoTypeGroup & p_DirtyGroupFlag) > 0)
// 		{
// 			K15_InternalAddUniformToDirtyCache(p_RenderUniformCache, cachedAutoUniform->autoType);
// 		}
// 	}
// }
/*********************************************************************************/
