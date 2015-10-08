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
/*********************************************************************************/
result8 K15_GLBindVertexFormat(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexFormatDesc* p_VertexFormatDesc)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	uint32 vertexFormatHash = p_VertexFormatDesc->hash;

	if (vertexFormatHash != glContext->glBoundObjects.boundVertexFormatHash)
	{
		K15_GLProgram* vertexShader = glContext->glBoundObjects.boundPrograms[K15_RENDER_PROGRAM_TYPE_VERTEX];

		//check what input the vertex shader expects
		if (vertexShader)
		{
			K15_GLInputLayout* inputLayout = &vertexShader->inputLayout;

			for (uint32 inputIndex = 0;
				inputIndex < inputLayout->numInputElements;
				++inputIndex)
			{
				K15_GLInputLayoutElement* inputElement = &inputLayout->inputElements[inputIndex];
				bool8 foundVertexFormatElement = K15_FALSE;

				K15_OPENGL_CALL(kglEnableVertexAttribArray(inputIndex));

				for (uint32 formatIndex = 0;
					formatIndex < p_VertexFormatDesc->numAttributes;
					++formatIndex)
				{
					K15_RenderVertexFormatElementDesc* formatElement = &p_VertexFormatDesc->elements[formatIndex];

					if (formatElement->semanticID == inputElement->semanticID)
					{
						K15_OPENGL_CALL(kglVertexAttribPointer(inputIndex, inputElement->elementSize, 
							inputElement->glType, GL_FALSE, p_VertexFormatDesc->stride, 
							(const void*)formatElement->offsetInBytes));

						foundVertexFormatElement = K15_TRUE;
						break;
					}
				}
				// 
				// 			if (!foundVertexFormatElement)
				// 			{
				// 				K15_OPENGL_CALL(kglVertexAttrib())
				// 			}
			}
		}

		glContext->glBoundObjects.boundVertexFormatHash = vertexFormatHash;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/