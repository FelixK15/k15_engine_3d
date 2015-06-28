/*********************************************************************************/
// intern inline uint8 K15_InternGLAddToBuffer(K15_GLRenderContext* p_GLContext, K15_RenderBufferType p_BufferType, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
// {
// 	uint8 result = K15_SUCCESS;
// 	uint32 size = p_RenderBufferDesc->sizeInBytes;
// 	byte* data = p_RenderBufferDesc->data;
// 	GLuint glBufferHandle = 0;
// 
// 	K15_GLBuffer* currentBuffer = 0;
// 
// 	for (uint32 bufferIndex = 0;
// 		 bufferIndex < K15_RENDER_GL_MAX_BUFFER_COUNT;
// 		 ++bufferIndex)
// 	{
// 		currentBuffer = &p_GLContext->glObjects.buffers[bufferIndex];
// 		
// 		if (currentBuffer->size >= size)
// 		{
// 			uint32 oldSize = currentBuffer->size;
// 			uint32 newSize = currentBuffer->size - size;
// 			byte* bufferData = currentBuffer->data;
// 
// 			uint32 offset = K15_RENDER_GL_BUFFER_SIZE - oldSize;
// 
// 			glBufferHandle = currentBuffer->buffer;
// 
// 			K15_GLBufferAccessData accessData;
// 
// 			accessData.bufferType = p_BufferType;
// 			accessData.glBufferIndex = bufferIndex;
// 			accessData.offset = offset;
// 			accessData.size = size;
// 
// 			if (data)
// 			{
// 				//upload data to gpu
// 				K15_OPENGL_CALL(kglNamedBufferSubDataEXT(glBufferHandle, offset, size, data));
// 
// 				//copy data into shadowed buffer
// 				memcpy(bufferData + offset, data, size);
// 			}
// 
// 			p_GLContext->gl3.accessData[*p_RenderBufferHandlePtr] = accessData;
// 
// 			break;
// 		}
// 	}
// 
// 	return result;
// }
/*********************************************************************************/


/*********************************************************************************/
inline uint8 K15_GLCreateBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;
	uint8 result = K15_SUCCESS;

	K15_RenderBufferType renderBufferType = p_RenderBufferDesc->type;
	K15_RenderBufferUsage renderBufferUsage = p_RenderBufferDesc->usage;
	byte* bufferData = p_RenderBufferDesc->data;
	uint32 bufferDataSize = p_RenderBufferDesc->sizeInBytes;

	GLenum glBufferType = K15_GLConvertBufferType(renderBufferType);
	GLenum glUsageType = K15_GLConvertBufferUsage(renderBufferUsage);

	if (glBufferType == GL_INVALID_ENUM)
	{
		result = K15_ERROR_INVALID_BUFFER_TYPE;
	}
	else
	{
		GLsizeiptr sizeInBytes = (GLsizeiptr)p_RenderBufferDesc->sizeInBytes;
		GLvoid* data = (GLvoid*)p_RenderBufferDesc->data;
		GLuint glBufferHandle = 0;

		K15_OPENGL_CALL(kglGenBuffers(1, &glBufferHandle));
		K15_OPENGL_CALL(kglNamedBufferDataEXT(glBufferHandle, sizeInBytes, data, glUsageType));

		K15_GLBuffer* glBuffer = &glContext->glObjects.buffers[*p_RenderBufferHandlePtr];
		glBuffer->buffer = glBufferHandle;
		glBuffer->glBufferType = glBufferType;
		glBuffer->singleElementSizeInBytes = p_RenderBufferDesc->singleElementSizeInBytes;
		glBuffer->bufferType = renderBufferType;
		glBuffer->data = data;
		glBuffer->sizeInBytes = sizeInBytes;
	}

	return result;
}
/*********************************************************************************/
inline uint8 K15_GLUpdateBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	uint32 glBufferIndex = *p_RenderBufferHandlePtr;

	assert(glBufferIndex != K15_INVALID_GPU_RESOURCE_HANDLE);
	assert(glBufferIndex < K15_RENDER_MAX_GPU_BUFFER);
	assert(p_RenderBufferUpdateDesc->data != 0);

	K15_GLBuffer* glBuffer = &glContext->glObjects.buffers[glBufferIndex];

	void* updateData = p_RenderBufferUpdateDesc->data;

	GLuint glBufferHandle = glBuffer->buffer;
	GLintptr glBufferOffset = (GLintptr)p_RenderBufferUpdateDesc->offset;
	GLsizeiptr glBufferUpdateSize = (GLsizeiptr)p_RenderBufferUpdateDesc->size;
	GLbitfield glBufferMapAccess = GL_MAP_WRITE_BIT;

#ifdef K15_OPENGL_ENABLE_INVALIDATE_MAP_BUFFER_RANGE
	glBufferMapAccess |= GL_MAP_INVALIDATE_RANGE_BIT;
#endif

	GLvoid* glBufferMemory = 0;
	GLboolean glUnmapBufferResult = GL_TRUE;

	K15_OPENGL_CALL(glBufferMemory = kglMapNamedBufferRangeEXT(glBufferHandle, glBufferOffset, glBufferUpdateSize, glBufferMapAccess));

	assert(glBufferMemory);

	//update gpu memory
	memcpy(glBufferMemory, updateData, glBufferUpdateSize);
	
	//update shadow copy
	memcpy((byte*)glBuffer->data + glBufferOffset, updateData, glBufferUpdateSize);
	
	K15_OPENGL_CALL(glUnmapBufferResult = kglUnmapNamedBufferEXT(glBufferHandle));

	if (glUnmapBufferResult != GL_TRUE)
	{
		result = K15_ERROR_RENDER_API;
	}

	return result;
}
/*********************************************************************************/
inline uint8 K15_GLBindBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	uint32 glBufferIndex = *p_RenderBufferHandlePtr;

	assert(glBufferIndex != K15_INVALID_GPU_RESOURCE_HANDLE);
	assert(glBufferIndex < K15_RENDER_MAX_GPU_BUFFER);

	K15_GLBuffer* glBuffer = &glContext->glObjects.buffers[glBufferIndex];	
	GLenum glBufferType = glBuffer->glBufferType;
	K15_RenderBufferType bufferType = glBuffer->bufferType;

	//don't bind already bound buffer
	if (glContext->glBoundObjects.boundBuffers[bufferType] == glBuffer)
	{
		return K15_SUCCESS;
	}
	
	GLuint glBufferHandle = glBuffer->buffer;

	K15_OPENGL_CALL(kglBindBuffer(glBufferType, glBufferHandle));

	//save bound buffer
	glContext->glBoundObjects.boundBuffers[bufferType] = glBuffer;

	return result;
}
/*********************************************************************************/
inline uint8 K15_GLDeleteBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	uint32 glBufferIndex = *p_RenderBufferHandlePtr;

	assert(glBufferIndex != K15_INVALID_GPU_RESOURCE_HANDLE);
	assert(glBufferIndex < K15_RENDER_MAX_GPU_BUFFER);

	K15_GLBuffer* glBuffer = &glContext->glObjects.buffers[glBufferIndex];
	K15_RenderBufferType bufferType = glBuffer->bufferType;

	if (glContext->glBoundObjects.boundBuffers[bufferType] == glBuffer)
	{
		glContext->glBoundObjects.boundBuffers[bufferType] = 0;
	}

	GLuint glBufferHandle = glBuffer->buffer;

	K15_OPENGL_CALL(kglDeleteBuffers(1, &glBufferHandle));

	return result;
}
/*********************************************************************************/