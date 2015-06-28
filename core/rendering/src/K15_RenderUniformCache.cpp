/*********************************************************************************/
intern void K15_InternalAddUniformToDirtyCache(K15_RenderUniformCache* p_RenderUniformCache, K15_AutoUniformType p_AutoUniformType)
{
	//add uniform to 'dirty cache'
	p_RenderUniformCache->dirtyAutoUniforms[p_AutoUniformType] = &p_RenderUniformCache->cachedAutoUniforms[p_AutoUniformType];
}
/*********************************************************************************/
void K15_InitializeRenderUniformCache(K15_RenderUniformCache* p_RenderUniformCache)
{
	assert(p_RenderUniformCache);

	K15_RenderUniformCacheEntry worldMatrixCacheEntry = {};
	K15_RenderUniformCacheEntry projectionMatrixCacheEntry = {};
	K15_RenderUniformCacheEntry viewMatrixCacheEntry = {};

	worldMatrixCacheEntry.name = "worldMatrix";
	projectionMatrixCacheEntry.name = "projMatrix";
	viewMatrixCacheEntry.name = "viewMatrix";

	worldMatrixCacheEntry.autoType = K15_AUTO_UNIFORM_TYPE_WORLD_MATRIX;
	projectionMatrixCacheEntry.autoType = K15_AUTO_UNIFORM_TYPE_PROJECTION_MATRIX;
	viewMatrixCacheEntry.autoType = K15_AUTO_UNIFORM_TYPE_VIEW_MATRIX;

	worldMatrixCacheEntry.autoTypeGroup = K15_AUTO_UNIFORM_TYPE_GROUP_PER_MESH;
	projectionMatrixCacheEntry.autoTypeGroup = K15_AUTO_UNIFORM_TYPE_GROUP_PER_CAMERA;
	viewMatrixCacheEntry.autoTypeGroup = K15_AUTO_UNIFORM_TYPE_GROUP_PER_CAMERA;

	worldMatrixCacheEntry.data = (byte*)malloc(sizeof(K15_Matrix4));
	projectionMatrixCacheEntry.data = (byte*)malloc(sizeof(K15_Matrix4));
	viewMatrixCacheEntry.data = (byte*)malloc(sizeof(K15_Matrix4));

	worldMatrixCacheEntry.type = K15_UNIFORM_TYPE_FLOAT4x4;
	projectionMatrixCacheEntry.type = K15_UNIFORM_TYPE_FLOAT4x4;
	viewMatrixCacheEntry.type = K15_UNIFORM_TYPE_FLOAT4x4;

	p_RenderUniformCache->cachedAutoUniforms[K15_AUTO_UNIFORM_TYPE_VIEW_MATRIX] = viewMatrixCacheEntry;
	p_RenderUniformCache->cachedAutoUniforms[K15_AUTO_UNIFORM_TYPE_PROJECTION_MATRIX] = projectionMatrixCacheEntry;
	p_RenderUniformCache->cachedAutoUniforms[K15_AUTO_UNIFORM_TYPE_WORLD_MATRIX] = worldMatrixCacheEntry;

	memset(p_RenderUniformCache->dirtyAutoUniforms, 0, sizeof(p_RenderUniformCache->dirtyAutoUniforms));
}
/*********************************************************************************/
void K15_UpdateUniformCacheEntry(K15_RenderUniformCache* p_RenderUniformCache, K15_AutoUniformType p_AutoUniformType, byte* p_Data)
{
	assert(p_RenderUniformCache);
	assert(p_Data);

	K15_RenderUniformCacheEntry* cachedAutoUniform = &p_RenderUniformCache->cachedAutoUniforms[p_AutoUniformType];

	uint32 sizeInBytes = K15_ConvertToUniformByteSize(cachedAutoUniform->type);
	memcpy(cachedAutoUniform->data, p_Data, sizeInBytes);
	
	K15_InternalAddUniformToDirtyCache(p_RenderUniformCache, p_AutoUniformType);
}
/*********************************************************************************/
void K15_SetAutoUniformGroupAsDirty(K15_RenderUniformCache* p_RenderUniformCache, K15_AutoUniformTypeGroupFlag p_DirtyGroupFlag)
{
	assert(p_RenderUniformCache);

	for (uint32 autoUniformIndex = 0;
		autoUniformIndex < K15_AUTO_UNIFORM_TYPE_COUNT;
		++autoUniformIndex)
	{
		K15_RenderUniformCacheEntry* cachedAutoUniform = &p_RenderUniformCache->cachedAutoUniforms[autoUniformIndex];

		if ((cachedAutoUniform->autoTypeGroup & p_DirtyGroupFlag) > 0)
		{
			K15_InternalAddUniformToDirtyCache(p_RenderUniformCache, cachedAutoUniform->autoType);
		}
	}
}
/*********************************************************************************/
