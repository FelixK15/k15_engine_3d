#define K15_RENDER_GL_MAX_BUFFER_COUNT 32
#define K15_RENDER_GL_BUFFER_SIZE size_megabyte(10)

struct K15_GLBufferDesc
{
	byte* data;
	uint32 size;
	GLuint buffer;
};

struct K15_GLBufferAccessData
{
	uint32 glBufferIndex;
	uint32 offset;
	uint32 size;
};

intern K15_GLBufferDesc internOpenGLBuffers[K15_RENDER_GL_MAX_BUFFER_COUNT] = {0};
intern K15_GLBufferAccessData internBufferAccessDatas[K15_RENDER_MAX_GPU_BUFFER] = {0xffffffff};

/*********************************************************************************/
intern inline uint8 K15_InternGLAddToBuffer(K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;
	uint32 size = p_RenderBufferDesc->size;
	byte* data = p_RenderBufferDesc->data;
	GLuint glBufferHandle = 0;

	K15_GLBufferDesc* currentBuffer = 0;

	for (uint32 bufferIndex = 0;
		 bufferIndex < K15_RENDER_GL_MAX_BUFFER_COUNT;
		 ++bufferIndex)
	{
		currentBuffer = &internOpenGLBuffers[bufferIndex];

		if (currentBuffer->size >= size)
		{
			uint32 oldSize = currentBuffer->size;
			uint32 newSize = currentBuffer->size - size;
			byte* bufferData = currentBuffer->data;

			uint32 offset = K15_RENDER_GL_BUFFER_SIZE - oldSize;

			glBufferHandle = currentBuffer->buffer;

			K15_GLBufferAccessData accessData;

			accessData.glBufferIndex = bufferIndex;
			accessData.offset = offset;
			accessData.size = size;

			if (data)
			{
				//upload data to gpu
				K15_OPENGL_CALL(kglNamedBufferSubDataEXT(glBufferHandle, offset, size, data));

				//copy data into shadowed buffer
				memcpy(bufferData + offset, data, size);
			}

			internBufferAccessDatas[*p_RenderBufferHandlePtr] = accessData;

			break;
		}
	}

	return result;
}
/*********************************************************************************/

/*********************************************************************************/
inline uint8 K15_GLCreateBuffers()
{
	uint8 returnValue = K15_SUCCESS;

	GLuint buffers[K15_RENDER_GL_MAX_BUFFER_COUNT] = {0};

	K15_OPENGL_CALL(kglGenBuffers(K15_RENDER_GL_MAX_BUFFER_COUNT, buffers));

	for (uint32 bufferIndex = 0;
		 bufferIndex < K15_RENDER_GL_MAX_BUFFER_COUNT;
		 ++bufferIndex)
	{
		K15_OPENGL_CALL(kglNamedBufferDataEXT(buffers[bufferIndex], K15_RENDER_GL_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW));

		K15_GLBufferDesc currentBufferDesc = {};
		currentBufferDesc.buffer = buffers[bufferIndex];
		currentBufferDesc.size = K15_RENDER_GL_BUFFER_SIZE;
		currentBufferDesc.data = (byte*)malloc(K15_RENDER_GL_BUFFER_SIZE);

		if (!currentBufferDesc.data)
		{
			returnValue = K15_OS_ERROR_OUT_OF_MEMORY;
			break;
		}

		internOpenGLBuffers[bufferIndex] = currentBufferDesc;
	}

	return returnValue;
}
/*********************************************************************************/
inline uint8 K15_GLCreateBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;

	K15_RenderBufferType renderBufferType = p_RenderBufferDesc->type;
	K15_RenderBufferUsage renderBufferUsage = p_RenderBufferDesc->usage;
	byte* bufferData = p_RenderBufferDesc->data;
	uint32 bufferDataSize = p_RenderBufferDesc->size;

	GLenum glBufferType = K15_GLConvertBufferType(renderBufferType);
	GLenum glUsageType = K15_GLConvertBufferUsage(renderBufferUsage);

	if (glBufferType == GL_INVALID_ENUM)
	{
		result = K15_ERROR_INVALID_BUFFER_TYPE;
	}
	else
	{
		result = K15_InternGLAddToBuffer(p_RenderBufferDesc, p_RenderBufferHandlePtr);
	}

	return result;
}
/*********************************************************************************/
inline uint8 K15_GLUpdateBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;

	uint32 glBufferIndex = *p_RenderBufferHandlePtr;

	assert(glBufferIndex != K15_INVALID_GPU_RESOURCE_HANDLE);
	assert(glBufferIndex < K15_RENDER_MAX_GPU_BUFFER);
	assert(p_RenderBufferUpdateDesc->data != 0);

	K15_GLBufferAccessData* glBufferAccess = &internBufferAccessDatas[glBufferIndex];

	assert(glBufferAccess->glBufferIndex != K15_INVALID_GL_BUFFER_INDEX);

	K15_GLBufferDesc* glBuffer = &internOpenGLBuffers[glBufferAccess->glBufferIndex];

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
	memcpy(glBuffer->data + glBufferOffset, updateData, glBufferUpdateSize);
	
	K15_OPENGL_CALL(glUnmapBufferResult = kglUnmapNamedBufferEXT(glBufferHandle));

	if (glUnmapBufferResult != GL_TRUE)
	{
		result = K15_ERROR_RENDER_API;
	}

	return result;
}
/*********************************************************************************/
inline uint8 K15_GLDeleteBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;

	uint32 glBufferIndex = *p_RenderBufferHandlePtr;

	assert(glBufferIndex != K15_INVALID_GPU_RESOURCE_HANDLE);
	assert(glBufferIndex < K15_RENDER_MAX_GPU_BUFFER);

	K15_GLBufferAccessData* glBufferAccess = &internBufferAccessDatas[glBufferIndex];

	assert(glBufferAccess->glBufferIndex != K15_INVALID_GL_BUFFER_INDEX);

	K15_GLBufferDesc* glBuffer = &internOpenGLBuffers[glBufferAccess->glBufferIndex];

	//invalidate the buffer access struct
	glBufferAccess->glBufferIndex = K15_INVALID_GL_BUFFER_INDEX;
	
	return result;
}
/*********************************************************************************/