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
GLenum K15_GLConvertUniformTypeToGLType(K15_UniformType p_UniformType)
{
	GLenum nativeUniformType = GL_INVALID_ENUM;

	switch(p_UniformType)
	{
		case K15_UNIFORM_TYPE_INT8:
		case K15_UNIFORM_TYPE_INT16:
		case K15_UNIFORM_TYPE_INT32:
		{
			nativeUniformType = GL_INT;
			break;
		}

		case K15_UNIFORM_TYPE_UINT8:
		case K15_UNIFORM_TYPE_UINT16:
		case K15_UNIFORM_TYPE_UINT32:
		{
			nativeUniformType = GL_UNSIGNED_INT;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT:
		{
			nativeUniformType = GL_FLOAT;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT2:
		{
			nativeUniformType = GL_FLOAT_VEC2;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT3:
		{
			nativeUniformType = GL_FLOAT_VEC3;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT4:
		{
			nativeUniformType = GL_FLOAT_VEC4;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT2x2:
		{
			nativeUniformType = GL_FLOAT_MAT2;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT3x3:
		{
			nativeUniformType = GL_FLOAT_MAT3;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT4x4:
		{
			nativeUniformType = GL_FLOAT_MAT4;
			break;
		}

		default:
		{
			assert(false);
		}
	}


	return nativeUniformType;
}
/*********************************************************************************/
K15_UniformType K15_GLConvertGLTypeToUniformType(GLenum p_GLType, GLint p_TypeSizeInBytes)
{
	K15_UniformType uniformType = K15_UNIFORM_TYPE_INVALID;

	switch(p_GLType)
	{
		case GL_INT:
		{
			if (p_TypeSizeInBytes == sizeof(int8))
			{
				uniformType = K15_UNIFORM_TYPE_INT8;
			}
			else if(p_TypeSizeInBytes == sizeof(int16))
			{
				uniformType = K15_UNIFORM_TYPE_INT16;
			}
			else if(p_TypeSizeInBytes == sizeof(int32))
			{
				uniformType = K15_UNIFORM_TYPE_INT32;
			}
			else
			{
				assert(false);
			}

			break;
		}

		case GL_UNSIGNED_INT:
		{
			if (p_TypeSizeInBytes == sizeof(uint8))
			{
				uniformType = K15_UNIFORM_TYPE_UINT8;
			}
			else if(p_TypeSizeInBytes == sizeof(uint16))
			{
				uniformType = K15_UNIFORM_TYPE_UINT16;
			}
			else if(p_TypeSizeInBytes == sizeof(uint32))
			{
				uniformType = K15_UNIFORM_TYPE_UINT32;
			}
			else
			{
				assert(false);
			}

			break;
		}

		case GL_FLOAT:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT;
			break;
		}

		case GL_FLOAT_VEC2:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT2;
			break;
		}

		case GL_FLOAT_VEC3:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT3;
			break;
		}

		case GL_FLOAT_VEC4:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT4;
			break;
		}

		case GL_FLOAT_MAT2:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT2x2;
			break;
		}

		case GL_FLOAT_MAT3:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT3x3;
			break;
		}

		case GL_FLOAT_MAT4:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT4x4;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return uniformType;
}
/*********************************************************************************/