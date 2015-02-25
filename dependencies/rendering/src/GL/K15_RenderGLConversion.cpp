/*********************************************************************************/
inline GLenum K15_GLConvertBufferType(K15_GpuBufferType p_BufferType)
{
	GLenum nativeBufferType = GL_INVALID_ENUM;

	switch(p_BufferType)
	{
		case K15_GPU_BUFFER_TYPE_VERTEX:
		{
			nativeBufferType = GL_ARRAY_BUFFER;
			break;
		}
		case K15_GPU_BUFFER_TYPE_INDEX:
		{
			nativeBufferType = GL_ELEMENT_ARRAY_BUFFER;	
			break;
		}
		case K15_GPU_BUFFER_TYPE_UNIFORM:
		{
			nativeBufferType = GL_UNIFORM_BUFFER;	
		}
	}

	return nativeBufferType;
}
/*********************************************************************************/
