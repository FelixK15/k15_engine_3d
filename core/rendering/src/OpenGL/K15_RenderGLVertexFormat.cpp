// /*********************************************************************************/
// result8 K15_GLBindVertexFormat_vertex_array_object(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexFormatDesc* p_VertexFormatDesc)
// {
// 	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
// 	uint32 vertexFormatHash = p_VertexFormatDesc->hash;
// 	uint32 numCacheEntries = glContext->extensions.vertex_array_object.vertexFormatCache.numCacheEntries;
// 	uint32 cacheIndex = 0;
// 	GLuint glVertexArrayObject = 0;
// 	bool8 foundCacheEntry = K15_FALSE;
// 
// 	for (cacheIndex = 0;
// 		cacheIndex < numCacheEntries;
// 		++cacheIndex)
// 	{
// 		if (glContext->extensions.vertex_array_object.vertexFormatCache.cacheEntry[cacheIndex].vertexFormatHash == vertexFormatHash)
// 		{
// 			foundCacheEntry = K15_TRUE;
// 			break;
// 		}
// 	}
// 
// 	if (!foundCacheEntry)
// 	{
// 		//create cache entry
// 		K15_OPENGL_CALL(kglGenVertexArrays(1, &glVertexArrayObject));
// 
// 		glContext->extensions.vertex_array_object.vertexFormatCache.cacheEntry[numCacheEntries].vertexFormatHash = vertexFormatHash;
// 		glContext->extensions.vertex_array_object.vertexFormatCache.cacheEntry[numCacheEntries].glVertexArrayHandle = glVertexArrayObject;
// 
// 		K15_GLBindVertexFormat(p_RenderBackEnd, p_VertexFormatDesc);
// 		
// 		cacheIndex = numCacheEntries;
// 		++glContext->extensions.vertex_array_object.vertexFormatCache.numCacheEntries;
// 	}
// 
// 	glVertexArrayObject = glContext->extensions.vertex_array_object.vertexFormatCache.cacheEntry[cacheIndex].glVertexArrayHandle;
// 
// 	K15_OPENGL_CALL(kglBindVertexArray(glVertexArrayObject));
// 
// 	return K15_SUCCESS;
// }
// /*********************************************************************************/
// result8 K15_GLCreateVertexFormat(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexFormatDesc* p_VertexFormatDesc, K15_RenderResourceHandle* p_RenderResourceHandlePtr)
// {
// 
// 	for (uint32 elementIndex = 0;
// 		elementIndex < p_VertexFormatDesc->numAttributes;
// 		++elementIndex)
// 	{
// 		K15_RenderVertexFormatElementDesc* elementDesc = &p_VertexFormatDesc->elements[elementIndex];
// 
// 
// 	}
// }
// /*********************************************************************************/
// result8 K15_GLBindVertexFormat(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexFormatDesc* p_VertexFormatDesc)
// {
// 	for (uint32 elementIndex = 0;
// 		elementIndex < p_VertexFormatDesc->numAttributes;
// 		++elementIndex)
// 	{
// 		K15_RenderVertexFormatElementDesc* elementDesc = &p_VertexFormatDesc->elements[elementIndex];
// 
// 		
// 	}
// }
// /*********************************************************************************/