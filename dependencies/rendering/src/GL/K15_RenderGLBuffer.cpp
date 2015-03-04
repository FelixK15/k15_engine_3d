#include "GL/K15_RenderGLConversion.cpp"

/*********************************************************************************/
inline uint8 K15_GLCreateBuffer(K15_RenderContext* p_RenderContext, K15_RenderCommandQueue* p_RenderQueue, K15_RenderCommandInstance* p_RenderCommand)
{
	uint8 result = K15_SUCCESS;

	K15_RenderCommandParameterBuffer* parameterBackBuffer = p_RenderQueue->parameterBuffer[K15_RENDERING_COMMAND_FRONT_BUFFER_INDEX];
	
	K15_GpuBufferHandle* bufferHandle = 0;
	int32 bufferType = 0;
	uint32 buffersize = 0;

	uint32 parameterSize = sizeof(ptrdiff_t) + sizeof(int32) + sizeof(uint32);
	uint32 parameterBufferOffset = p_RenderCommand->parameterOffset;
	byte* parameterBuffer = parameterBackBuffer->parameterBuffer;

	if (parameterSize != p_RenderCommand->parameterSize)
	{
		result = K15_ERROR_INVALID_PARAMETER_SIZE;
	}

	memcpy(&bufferHandle, parameterBuffer + parameterBufferOffset, sizeof(ptrdiff_t));
	memcpy(&bufferType, parameterBuffer + parameterBufferOffset + sizeof(K15_GpuBufferHandle), sizeof(int32));
	memcpy(&buffersize, parameterBuffer + parameterBufferOffset + sizeof(K15_GpuBufferHandle) + sizeof(int32), sizeof(uint32));

	GLenum glBufferType = K15_GLConvertBufferType((K15_GpuBufferType)bufferType);

	if (bufferType == GL_INVALID_ENUM)
	{
		result = K15_ERROR_INVALID_BUFFER_TYPE;
	}
	else
	{
		GLuint buffer = 0;
		glGenBuffers(1, &buffer);
		kglNamedBufferDataEXT(buffer, buffersize, 0, GL_STATIC_DRAW);

		*bufferHandle = buffer;
	}

	return result;
}
/*********************************************************************************/