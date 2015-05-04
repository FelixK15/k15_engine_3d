/*********************************************************************************/
inline uint8 K15_GLCreateBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;
	uint8 result = K15_SUCCESS;

	K15_GLBuffer* glBuffer = &glContext->glObjects.buffers[*p_RenderBufferHandlePtr];

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
		GLuint glBufferHandle = 0;
		GLuint glCurrentBoundBufferHandle = 0;

		K15_OPENGL_CALL(glGenBuffers(1, &glBufferHandle));
		K15_OPENGL_CALL(glBindBuffer(glBufferType, glBufferHandle));
		K15_OPENGL_CALL(glBufferData(glBufferType, bufferDataSize, bufferData, glUsageType));
		K15_OPENGL_CALL(glBindBuffer(glBufferType, 0));

		glBuffer->buffer = glBufferHandle;
		glBuffer->data = bufferData;
		glBuffer->size = bufferDataSize;
		glBuffer->bufferType = glBufferType;
	}

	return result;
}
/*********************************************************************************/
inline uint8 K15_GLUpdateBuffer(K15_RenderContext* p_RenderContext, K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc, K15_RenderBufferHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	uint32 glBufferIndex = *p_RenderBufferHandlePtr;

	assert(glBufferIndex < K15_RENDER_MAX_GPU_BUFFER);
	assert(p_RenderBufferUpdateDesc->data != 0);

	K15_GLBuffer* glBuffer = &glContext->glObjects.buffers[glBufferIndex];
	GLenum glBufferType = glBuffer->bufferType;

	void* updateData = p_RenderBufferUpdateDesc->data;

	GLuint glBufferHandle = glBuffer->buffer;
	GLintptr glBufferOffset = (GLintptr)p_RenderBufferUpdateDesc->offset;
	GLsizeiptr glBufferUpdateSize = (GLsizeiptr)p_RenderBufferUpdateDesc->size;

	K15_OPENGL_CALL(glBindBuffer(glBufferType, glBufferHandle));
	K15_OPENGL_CALL(glBufferSubData(glBufferType, glBufferOffset, glBufferUpdateSize, updateData));
	K15_OPENGL_CALL(glBindBuffer(glBufferType, 0));

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
	GLuint glBufferHandle = glBuffer->buffer;

	glDeleteBuffers(1, &glBufferHandle);

	//invalidate the buffer access struct
	*p_RenderBufferHandlePtr = K15_INVALID_GL_BUFFER_INDEX;

	return result;
}
/*********************************************************************************/