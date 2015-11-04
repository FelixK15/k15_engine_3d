/*********************************************************************************/
intern inline K15_RenderBufferDesc K15_InternalCreateDefaultVertexBufferDesc(uint32 p_BufferSizeInBytes)
{
	static int vboCounter = 0; //for hash creation

	char* vboName = K15_GenerateString("VBO_%d", (char*)alloca(64), vboCounter++);

	K15_RenderBufferDesc defaultVertexBufferDesc = {};

	defaultVertexBufferDesc.access = K15_RENDER_BUFFER_ACCESS_ALL;
	defaultVertexBufferDesc.type = K15_RENDER_BUFFER_TYPE_VERTEX;
	defaultVertexBufferDesc.usage = K15_RENDER_BUFFER_USAGE_DYNAMIC_DRAW;
	defaultVertexBufferDesc.sizeInBytes = p_BufferSizeInBytes;
	defaultVertexBufferDesc.nameHash = K15_GenerateStringHash(vboName);

	return defaultVertexBufferDesc;
}
/*********************************************************************************/
intern inline void K15_InternalGLAddPendingBufferUpdate(K15_GLRenderContext* p_GLRenderContext, GLuint p_GLBufferHandle,
														GLintptr p_OffsetInBytes, GLsizei p_SizeInBytes, void* p_Data)
{
	K15_GLPendingBufferUpdateManager* pendingBufferUpdateManager = &p_GLRenderContext->pendingBufferUpdateManager;
	uint32 numPendingUpdates = pendingBufferUpdateManager->numPendingUpdates++;
	uint32 pendingUpdateIndex = numPendingUpdates;

	K15_ASSERT(pendingUpdateIndex != K15_RENDER_GL_MAX_PENDING_BUFFER_UPDATES);

	if (pendingUpdateIndex != K15_RENDER_GL_MAX_PENDING_BUFFER_UPDATES)
	{
		K15_GLPendingBufferUpdate* pendingUpdate = &pendingBufferUpdateManager->pendingUpdate[pendingUpdateIndex];

		pendingUpdate->glBufferHandle = p_GLBufferHandle;
		pendingUpdate->glOffsetInBytes = p_OffsetInBytes;
		pendingUpdate->glSizeInBytes = p_SizeInBytes;
		pendingUpdate->data = p_Data;
	}
}
/*********************************************************************************/
void K15_GLResolvePendingBufferUpdates(K15_GLRenderContext* p_GLRenderContext, K15_GLBuffer* p_GLBuffer)
{
	K15_GLPendingBufferUpdateManager* pendingBufferUpdateManager = &p_GLRenderContext->pendingBufferUpdateManager;
	uint32 numPendingUpdates = pendingBufferUpdateManager->numPendingUpdates;

	GLuint glBufferHandle = p_GLBuffer->glBufferHandle;
	GLenum glBufferType = p_GLBuffer->glBufferType;

	for (uint32 updateIndex = 0;
		updateIndex < numPendingUpdates;
		++updateIndex)
	{
		K15_GLPendingBufferUpdate* pendingUpdate = &pendingBufferUpdateManager->pendingUpdate[updateIndex];

		if (pendingUpdate->glBufferHandle == glBufferHandle)
		{
			K15_OPENGL_CALL(kglBufferSubData(glBufferType, pendingUpdate->glOffsetInBytes, pendingUpdate->glSizeInBytes, pendingUpdate->data));

			//remove the current index from the array and shift all the right entries
			memmove(pendingBufferUpdateManager->pendingUpdate + updateIndex, pendingBufferUpdateManager->pendingUpdate + (updateIndex+1), (numPendingUpdates-updateIndex) * sizeof(K15_GLPendingBufferUpdate));

			--numPendingUpdates;
			--updateIndex;
		}
	}

	pendingBufferUpdateManager->numPendingUpdates = numPendingUpdates;
}
/*********************************************************************************/
inline void K15_GLBindBuffer(K15_GLRenderContext* p_GLContext, K15_GLBuffer* p_GLBuffer)
{
	GLenum glBufferType = p_GLBuffer->glBufferType;
	K15_RenderBufferType bufferType = p_GLBuffer->bufferType;

	//don't bind already bound buffer
	if (p_GLContext->glBoundObjects.boundBuffers[bufferType] == p_GLBuffer)
	{
		return;
	}

	GLuint glBufferHandle = p_GLBuffer->glBufferHandle;

	K15_OPENGL_CALL(kglBindBuffer(glBufferType, glBufferHandle));

	//resolve pending updates for newly bound buffer
	K15_GLResolvePendingBufferUpdates(p_GLContext, p_GLBuffer);

	//save bound buffer
	p_GLContext->glBoundObjects.boundBuffers[bufferType] = p_GLBuffer;
}
/*********************************************************************************/




/*********************************************************************************/
inline uint8 K15_GLCreateBuffer(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBufferDesc* p_RenderBufferDesc, K15_RenderResourceHandle* p_RenderBufferHandlePtr)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	uint8 result = K15_SUCCESS;

	K15_RenderBufferType renderBufferType = p_RenderBufferDesc->type;
	K15_RenderBufferUsage renderBufferUsage = p_RenderBufferDesc->usage;
	byte* bufferData = p_RenderBufferDesc->data;
	uint32 bufferDataSize = p_RenderBufferDesc->sizeInBytes;
	uint32 hash = p_RenderBufferDesc->nameHash;

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
		K15_OPENGL_CALL(kglBindBuffer(glBufferType, glBufferHandle));
		K15_OPENGL_CALL(kglBufferData(glBufferType, bufferDataSize, bufferData, glUsageType));

		K15_GLBuffer glBuffer = {};
		glBuffer.glBufferHandle = glBufferHandle;
		glBuffer.glBufferType = glBufferType;
		glBuffer.bufferType = renderBufferType;

		*p_RenderBufferHandlePtr = K15_InternalAddGLObject(glContext, &glBuffer, sizeof(glBuffer), hash, K15_GL_TYPE_BUFFER);
	}

	return result;
}
/*********************************************************************************/
inline uint8 K15_GLUpdateBuffer(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBufferUpdateDesc* p_RenderBufferUpdateDesc, K15_RenderResourceHandle* p_RenderBufferHandlePtr)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	uint32 glBufferIndex = *p_RenderBufferHandlePtr;

	assert(glBufferIndex != K15_INVALID_GPU_RESOURCE_HANDLE);
	assert(glBufferIndex < K15_RENDER_MAX_GPU_BUFFER);
	assert(p_RenderBufferUpdateDesc->data != 0);

	K15_GLBuffer* glBuffer = (K15_GLBuffer*)K15_InternalGetGLObjectData(glContext, *p_RenderBufferHandlePtr, K15_GL_TYPE_BUFFER);

	void* updateData = p_RenderBufferUpdateDesc->data;

	GLuint glBufferHandle = glBuffer->glBufferHandle;
	GLenum glBufferType = glBuffer->glBufferType;
	GLintptr glBufferOffset = (GLintptr)p_RenderBufferUpdateDesc->offset;
	GLsizeiptr glBufferUpdateSize = (GLsizeiptr)p_RenderBufferUpdateDesc->size;

	K15_OPENGL_CALL(kglBindBuffer(glBufferType, glBufferHandle));
	K15_OPENGL_CALL(kglBufferSubData(glBufferType, glBufferOffset, glBufferUpdateSize, updateData));
	K15_OPENGL_CALL(kglBindBuffer(glBufferType, 0));

	return result;
}
/*********************************************************************************/
inline uint8 K15_GLDeleteBuffer(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle p_RenderBufferHandle)
{
	uint8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	uint32 glBufferIndex = p_RenderBufferHandle;

	assert(glBufferIndex != K15_INVALID_GPU_RESOURCE_HANDLE);
	assert(glBufferIndex < K15_RENDER_MAX_GPU_BUFFER);

	K15_GLBuffer* glBuffer = (K15_GLBuffer*)K15_InternalGetGLObjectData(glContext, p_RenderBufferHandle, K15_GL_TYPE_BUFFER);
	K15_RenderBufferType bufferType = glBuffer->bufferType;

	if (glContext->glBoundObjects.boundBuffers[bufferType] == glBuffer)
	{
		glContext->glBoundObjects.boundBuffers[bufferType] = 0;
	}

	GLuint glBufferHandle = glBuffer->glBufferHandle;
	K15_OPENGL_CALL(kglDeleteBuffers(1, &glBufferHandle));

	K15_InternalRemoveGLObject(glContext, p_RenderBufferHandle, K15_GL_TYPE_BUFFER);

	return result;
}
/*********************************************************************************/
void K15_GLFreeVertexData(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexData* p_RenderVertexData)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	K15_GLVertexCacheEntry* cacheEntries = glContext->vertexCache.cacheEntries;
	uint32 numEntries = glContext->vertexCache.numEntries;

	for (uint32 entryIndex = 0;
		entryIndex < numEntries;
		++entryIndex)
	{
		if (cacheEntries[entryIndex].bufferHandle == p_RenderVertexData->vertexBufferHandle)
		{
			cacheEntries[entryIndex].sizeInBytes -= p_RenderVertexData->sizeInBytes;
			break;
		}
	}

	K15_FreeFromMemoryAllocator(glContext->memoryAllocator, p_RenderVertexData);
}
/*********************************************************************************/
inline K15_RenderVertexData* K15_GLUpdateVertexData(K15_RenderBackEnd* p_RenderBackEnd, void* p_VertexData, uint32 p_NumVertices, K15_RenderVertexFormatDesc* p_VertexFormat)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	K15_GLVertexCacheEntry* cacheEntries = glContext->vertexCache.cacheEntries;
	K15_GLVertexCacheEntry* usedCacheEntry = 0;

	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* memoryAllocator = &renderContext->memoryAllocator;

	//TODO MEMORY POOL
	K15_RenderVertexData* vertexData = (K15_RenderVertexData*)K15_AllocateFromMemoryAllocator(memoryAllocator, sizeof(K15_RenderVertexData));

	uint32 sizeInBytes = p_NumVertices * p_VertexFormat->stride;
	uint32 numEntries = glContext->vertexCache.numEntries;
	uint32 vertexFormatHash = p_VertexFormat->hash;
	bool8 foundCacheEntry = K15_FALSE;

	for (uint32 entryIndex = 0;
		entryIndex < numEntries;
		++entryIndex)
	{
		usedCacheEntry = &cacheEntries[entryIndex];
		if (usedCacheEntry->vertexFormatHash == vertexFormatHash &&
			(usedCacheEntry->capacityInBytes - usedCacheEntry->sizeInBytes) >= sizeInBytes)
		{
			foundCacheEntry = K15_TRUE;
			break;
		}
	}

	if (!foundCacheEntry)
	{
		uint32 newCacheIndex = numEntries + 1;
		K15_ASSERT_TEXT(newCacheIndex != K15_RENDER_GL_MAX_VERTEX_CACHE_ENTRIES, "Vertex cache is full.");

		if (newCacheIndex < K15_RENDER_GL_MAX_VERTEX_CACHE_ENTRIES)
		{
			K15_GLVertexCacheEntry* newEntry = &cacheEntries[numEntries];
			uint32 defaultSizeInBytes = K15_RENDER_GL_DEFAULT_VERTEX_CACHE_ENTRY_SIZE;
			uint32 bufferSize = sizeInBytes > defaultSizeInBytes ? sizeInBytes : defaultSizeInBytes;
			newEntry->vertexFormatHash = vertexFormatHash;
			newEntry->capacityInBytes = bufferSize;
			newEntry->sizeInBytes = 0;

			//create vertex buffer
			K15_RenderBufferDesc vertexBufferDesc = K15_InternalCreateDefaultVertexBufferDesc(bufferSize);
			K15_GLCreateBuffer(p_RenderBackEnd, &vertexBufferDesc, &newEntry->bufferHandle);

			glContext->vertexCache.numEntries = newCacheIndex;

			usedCacheEntry = newEntry;
		}
	}

	vertexData->vertexBufferHandle = usedCacheEntry->bufferHandle;
	vertexData->offsetInBytes = usedCacheEntry->sizeInBytes;
	vertexData->sizeInBytes = sizeInBytes;
	vertexData->buffer = p_VertexData;
	vertexData->numVertices = p_NumVertices;
	vertexData->startVertexIndex = p_VertexFormat->stride / usedCacheEntry->capacityInBytes;
	vertexData->vertexFormat = p_VertexFormat;

	K15_GLBuffer* glBuffer = (K15_GLBuffer*)K15_InternalGetGLObjectData(glContext, usedCacheEntry->bufferHandle, K15_GL_TYPE_BUFFER);
	K15_InternalGLAddPendingBufferUpdate(glContext, glBuffer->glBufferHandle, vertexData->offsetInBytes, vertexData->sizeInBytes, p_VertexData);

	usedCacheEntry->sizeInBytes += sizeInBytes;

	return vertexData;
}
/*********************************************************************************/