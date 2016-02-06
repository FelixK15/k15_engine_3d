/*********************************************************************************/
intern K15_RenderUniformCacheEntry K15_InternCreateRenderUniformCacheEntry(K15_CustomMemoryAllocator* p_RenderAllocator,
																		   K15_ShaderProcessorContext* p_ShaderProcessor,
																		   const char* p_UniformName,
																		   uint32 p_Semantic,
																		   uint32 p_SemanticGroup,
																		   uint32 p_Type)
{
	uint32 dataSizeInBytes = K15_GetTypeSizeInBytesByTypeID(p_ShaderProcessor, p_Type);

	K15_RenderUniformCacheEntry uniformCacheEntry = {};
	uniformCacheEntry.name = p_UniformName;
	uniformCacheEntry.semanticID = p_Semantic;
	uniformCacheEntry.typeID = p_Type;
	uniformCacheEntry.semanticGroupID = p_SemanticGroup;
	uniformCacheEntry.nameHash = K15_GenerateStringHash(p_UniformName);
	uniformCacheEntry.sizeInBytes = dataSizeInBytes;
	uniformCacheEntry.data = (byte*)K15_AllocateFromMemoryAllocator(p_RenderAllocator, dataSizeInBytes);

	return uniformCacheEntry;
}
/*********************************************************************************/
intern void K15_InternalAddRenderUniformCacheEntries(K15_RenderUniformCache* p_UniformCache, 
													 K15_CustomMemoryAllocator* p_RenderAllocator, 
													 K15_ShaderProcessorContext* p_ShaderProcessor)
{
	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_VARIABLE_NAME,
		K15_UNIFORM_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX,
		K15_UNIFORM_GROUP_PER_OBJECT,
		K15_TYPE_FLOAT_MATRIX4);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_PROJECTION_MATRIX] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_PROJECTION_MATRIX_VARIABLE_NAME,
		K15_UNIFORM_SEMANTIC_PROJECTION_MATRIX,
		K15_UNIFORM_GROUP_PER_OBJECT,
		K15_TYPE_FLOAT_MATRIX4);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_VIEW_MATRIX] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_VIEW_MATRIX_VARIABLE_NAME,
		K15_UNIFORM_SEMANTIC_VIEW_MATRIX,
		K15_UNIFORM_GROUP_PER_OBJECT,
		K15_TYPE_FLOAT_MATRIX4);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_NORMAL_MATRIX] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_NORMAL_MATRIX_VARIABLE_NAME,
		K15_UNIFORM_SEMANTIC_NORMAL_MATRIX,
		K15_UNIFORM_GROUP_PER_OBJECT,
		K15_TYPE_FLOAT_MATRIX4);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_MODEL_MATRIX] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_MODEL_MATRIX_VARIABLE_NAME,
		K15_UNIFORM_SEMANTIC_MODEL_MATRIX,
		K15_UNIFORM_GROUP_PER_OBJECT,
		K15_TYPE_FLOAT_MATRIX4);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_GUI_PROJECTION_MATRIX] =
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_GUI_PROJECTION_MATRIX_VARIABLE_NAME,
		K15_UNIFORM_SEMANTIC_GUI_PROJECTION_MATRIX,
		K15_UNIFORM_GROUP_PER_OBJECT,
		K15_TYPE_FLOAT_MATRIX4);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_TEX1_DIMENSION] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_TEX1_DIMENSION_NAME,
		K15_UNIFORM_SEMANTIC_TEX1_DIMENSION,
		K15_UNIFORM_GROUP_PER_MATERIAL,
		K15_TYPE_INT_VECTOR2);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_TEX2_DIMENSION] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_TEX2_DIMENSION_NAME,
		K15_UNIFORM_SEMANTIC_TEX2_DIMENSION,
		K15_UNIFORM_GROUP_PER_MATERIAL,
		K15_TYPE_INT_VECTOR2);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_TEX3_DIMENSION] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_TEX3_DIMENSION_NAME,
		K15_UNIFORM_SEMANTIC_TEX3_DIMENSION,
		K15_UNIFORM_GROUP_PER_MATERIAL,
		K15_TYPE_INT_VECTOR2);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_TEX4_DIMENSION] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_TEX4_DIMENSION_NAME,
		K15_UNIFORM_SEMANTIC_TEX4_DIMENSION,
		K15_UNIFORM_GROUP_PER_MATERIAL,
		K15_TYPE_INT_VECTOR2);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_TEX5_DIMENSION] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_TEX5_DIMENSION_NAME,
		K15_UNIFORM_SEMANTIC_TEX5_DIMENSION,
		K15_UNIFORM_GROUP_PER_MATERIAL,
		K15_TYPE_INT_VECTOR2);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_TEX6_DIMENSION] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_TEX6_DIMENSION_NAME,
		K15_UNIFORM_SEMANTIC_TEX6_DIMENSION,
		K15_UNIFORM_GROUP_PER_MATERIAL,
		K15_TYPE_INT_VECTOR2);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_TEX7_DIMENSION] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_TEX6_DIMENSION_NAME,
		K15_UNIFORM_SEMANTIC_TEX7_DIMENSION,
		K15_UNIFORM_GROUP_PER_MATERIAL,
		K15_TYPE_INT_VECTOR2);

	p_UniformCache->cachedUniforms[K15_UNIFORM_SEMANTIC_TEX8_DIMENSION] = 
		K15_InternCreateRenderUniformCacheEntry(p_RenderAllocator, p_ShaderProcessor,
		K15_SEMANTIC_TEX8_DIMENSION_NAME,
		K15_UNIFORM_SEMANTIC_TEX8_DIMENSION,
		K15_UNIFORM_GROUP_PER_MATERIAL,
		K15_TYPE_INT_VECTOR2);
}
/*********************************************************************************/
intern void K15_InternalAddDirtyUniform(K15_RenderUniformCache* p_UniformCache, K15_ShaderUniformSemantics p_UniformSemantic)
{
	p_UniformCache->dirtyUniformSemantics[p_UniformSemantic] = K15_TRUE;
}
/*********************************************************************************/
void K15_InitializeRenderUniformCache(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderUniformCache* p_UniformCache)
{
	K15_ASSERT(p_RenderBackEnd);
	K15_ASSERT(p_UniformCache);
	
	K15_CustomMemoryAllocator* renderAllocator = &p_RenderBackEnd->renderContext->memoryAllocator;
	K15_ShaderProcessorContext* shaderProcessorContext = p_RenderBackEnd->shaderProcessorContext;

	uint32 numUniforms = K15_UNIFORM_SEMANTIC_COUNT;

	p_UniformCache->numUniforms = numUniforms;

	p_UniformCache->cachedUniforms = 
		(K15_RenderUniformCacheEntry*)K15_AllocateFromMemoryAllocator(renderAllocator, 
		sizeof(K15_RenderUniformCacheEntry) * numUniforms);

	K15_InternalAddRenderUniformCacheEntries(p_UniformCache, renderAllocator, shaderProcessorContext);
	K15_ResetDirtyUniformFlags(p_UniformCache);
}
/*********************************************************************************/
void K15_UpdateUniformCacheEntry(K15_RenderUniformCache* p_RenderUniformCache, K15_ShaderUniformSemantics p_SemanticID, byte* p_Data)
{
	K15_ASSERT(p_RenderUniformCache);
	K15_ASSERT(p_Data);

	K15_RenderUniformCacheEntry* uniformCacheEntry = &p_RenderUniformCache->cachedUniforms[p_SemanticID];
	uint32 uniformTypeSizeInBytes = uniformCacheEntry->sizeInBytes;
	
	//check if data has changed
	if (memcmp(p_Data, uniformCacheEntry->data, uniformTypeSizeInBytes) != 0)
	{
		memcpy(uniformCacheEntry->data, p_Data, uniformTypeSizeInBytes);
		K15_InternalAddDirtyUniform(p_RenderUniformCache, p_SemanticID);
	}
}
/*********************************************************************************/
void K15_ResetDirtyUniformFlags(K15_RenderUniformCache* p_RenderUniformCache)
{
	K15_ASSERT(p_RenderUniformCache);
	memset(p_RenderUniformCache->dirtyUniformSemantics, K15_FALSE, K15_UNIFORM_SEMANTIC_COUNT);
}
/*********************************************************************************/