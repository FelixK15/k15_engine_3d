/*********************************************************************************/
inline GLenum K15_GLConvertBufferType(K15_RenderBufferType p_BufferType)
{
	GLenum nativeBufferType = GL_INVALID_ENUM;

	switch(p_BufferType)
	{
		case K15_RENDER_BUFFER_TYPE_VERTEX:
		{
			nativeBufferType = GL_ARRAY_BUFFER;
			break;
		}
		case K15_RENDER_BUFFER_TYPE_INDEX:
		{
			nativeBufferType = GL_ELEMENT_ARRAY_BUFFER;	
			break;
		}
		case K15_RENDER_BUFFER_TYPE_UNIFORM:
		{
			nativeBufferType = GL_UNIFORM_BUFFER;	
			break;
		}
		
		default:
		{
			assert(false);
		}
	}

	return nativeBufferType;
}
/*********************************************************************************/
inline GLenum K15_GLConvertProgramType(K15_RenderProgramType p_ProgramType)
{
	GLenum glProgramType = GL_INVALID_ENUM;

	switch(p_ProgramType)
	{
		case K15_RENDER_PROGRAM_TYPE_VERTEX:
		{
			glProgramType = GL_VERTEX_SHADER;
			break;
		}

		case K15_RENDER_PROGRAM_TYPE_FRAGMENT:
		{
			glProgramType = GL_FRAGMENT_SHADER;
			break;
		}

		case K15_RENDER_PROGRAM_TYPE_GEOMETRY:
		{
			glProgramType = GL_GEOMETRY_SHADER;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return glProgramType;
}
/*********************************************************************************/
inline GLenum K15_GLConvertBufferUsage(K15_RenderBufferUsage p_BufferUsage)
{
	GLenum nativeBufferUsage = GL_INVALID_ENUM;

	switch(p_BufferUsage)
	{
		case K15_RENDER_BUFFER_USAGE_STATIC_DRAW:
		{
			nativeBufferUsage = GL_STATIC_DRAW;
			break;
		}

		case K15_RENDER_BUFFER_USAGE_STREAM_DRAW:
		{
			nativeBufferUsage = GL_STREAM_DRAW;	
			break;
		}

		case K15_RENDER_BUFFER_USAGE_DYNAMIC_DRAW:
		{
			nativeBufferUsage = GL_DYNAMIC_DRAW;	
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return nativeBufferUsage;
}
/*********************************************************************************/