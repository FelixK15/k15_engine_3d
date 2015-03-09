#include "OpenGL/GL3/K15_RenderGL3Conversion.cpp"
#include "K15_RenderBufferDesc.h"

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
		GLuint buffer = 0;
		kglGenBuffers(1, &buffer);
		kglNamedBufferDataEXT(buffer, bufferDataSize, bufferData, glUsageType);
	}

	return result;
}
/*********************************************************************************/